#include "IR.h"
#include "SyntaxNode.h"

#include <llvm/IR/ValueSymbolTable.h>

#include <iostream>
#include <string>
#include <cmath>
#include <map>

using namespace llvm;
using namespace std;

extern bool hasOr;                     // at lex.cpp 程序中是否含有or
extern CSyntaxNode *struct_tree;       // at parse.cpp
extern CSyntaxNode *extern_tree;       // at parse.cpp
extern set<string> g_ThreadFuncName;   // at MSVCompiler.cpp 封装的线程函数名

bool IR::prsnt_sign;
map<string, StructType*> IR::udef;
map<string, IRStruct*>   IR::m_IRStruct; //结构体名到IRStruct的映射

IR::IR(){
	// 以下内容之所以放在在这里，完全是因为IR类只能产生一个实例，这是历史原因导致的。
	m_context = new LLVMContext();
	m_smd = new SMDiagnostic();
	static unique_ptr<Module> Owner = llvm::parseIRFile(Setting::getExternalSPath().data(), *m_smd, *m_context);
	m_module = Owner.get();
	m_module->setModuleIdentifier("IRResult.cpp");
	m_dataLayout = new DataLayout(m_module);
	m_builder = new llvm::IRBuilder<>(m_module->getContext());

	// 删除没用的函数
	Module::FunctionListType &funcList = m_module->getFunctionList();
	Function *f = m_module->getFunction("\01?ex@@YAXXZ");
	funcList.erase(f);
	f = m_module->getFunction("\01?ex1@@YAXXZ");
	funcList.erase(f);
}

void IR::Trslt2IR(CSyntaxTree *m_GlbVarTree, CSyntaxNode *function_tree, CSyntaxTree *IRTree)
{
	if (IRTree == nullptr){
		cerr << "ERROR: the program tree is empty." << endl;
		PAUSE;
		exit(-1);
		return;
	}

	if (Setting::isFuncFile()){
		__FuncFile(m_GlbVarTree, function_tree); //处理.func文件
	}
	else{
		__ExTree2IR(extern_tree);
		FunctionType *FuncTypeOfMain = FunctionType::get(m_builder->getInt32Ty(), false);
		Function *MainFunc = Function::Create(FuncTypeOfMain, Function::ExternalLinkage, "main", m_module);
		BasicBlock *entrymain = BasicBlock::Create(m_module->getContext(), "entry", MainFunc, 0);
		m_builder->SetInsertPoint(entrymain);

		//修改理由：将状态计数器作为全局变量，以实现函数内访问状态计数器。
		m_StNum = new GlobalVariable(*m_module, IntegerType::get(m_module->getContext(), 32), false,
			GlobalValue::ExternalLinkage, ConstantInt::get(m_module->getContext(), APInt(32, 0)), "$$state_num");
		IRSymbol *symbol = new IRSymbol(m_StNum, true);
		if (m_GlobalSTable.insert({ "$$state_num", symbol }).second == false){
			cerr << "ERROR: variable $$state_num has already been defined." << endl;
		}

		__InitWinApiMap();
		__InitWinStruct(); //初始化一些windows.h中的结构体
		__StructTree2IR(struct_tree);

		//IRTemp = new GlobalVariable(*m_module, m_builder->getInt8PtrTy(), false, GlobalValue::ExternalLinkage, ConstantAggregateZero::get(m_builder->getInt8PtrTy()), "$$IRTemp");

		OrGlobal2IR(); //处理or的时候，预先将一些变量加入符号表
		Global2IR(m_GlbVarTree->GetRoot()); //全局变量定义

		m_InFunc = true;//将m_InFunc置为true，表示处理函数

		if (Setting::isOutput()){
			__CreateIsOutput();
			__CreateGlbOutFunc(); //创建输出所有全局变量的函数
			m_builder->CreateCall3(m_module->getFunction("\01?GetMsvVar@@YAXPAHP6AXXZP6A_NXZ@Z"), m_StNum, (Value*)m_module->getFunction("$$GlobalOutput"), (Value*)m_module->getFunction("$$IsOutput"));
		}
		else{
			Function *CalleeF = m_module->getFunction("\01?GetMsvVar@@YAXPAHP6AXXZP6A_NXZ@Z");
			Function::arg_iterator args = CalleeF->arg_begin();
			Value *arg1 = m_module->getGlobalVariable("$$state_num");  ++args;
			Value *arg2 = ConstantPointerNull::get((PointerType*)args->getType()); ++args;
			Value *arg3 = ConstantPointerNull::get((PointerType*)args->getType());
			m_builder->CreateCall3(CalleeF, arg1, arg2, arg3);
		}

		if (Setting::IsModeling()){
			vector<Value *> sub(2, m_builder->getInt32(0));
			m_builder->CreateCall5(m_module->getFunction("\01?GetExternalVars@@YAXPAH0PADP6AXXZ1@Z"),
				__STableSearch("$$current"),
				__STableSearch("$$length"),
				m_builder->CreateInBoundsGEP(__STableSearch("$$flag"), sub),
				(Value*)m_module->getFunction("\01?$$MyLongJmp@@YAXXZ"),
				m_builder->CreateInBoundsGEP(m_builder->CreateGlobalString((Setting::getOutPath() + Setting::getOutDotName())), sub));

			__GetVarsValue();

			m_builder->CreateCall3(m_module->getFunction("\01?GetModelVar@@YAXPAY0BIGKA@DPAHP6AXXZ@Z"),
				m_module->getGlobalVariable("\01?$$varsInfo@@3PADA"),
				m_module->getGlobalVariable("\01?$$tempLen@@3HA"),
				(Value*)m_module->getFunction("$$GetVarsValue"));

			m_builder->CreateCall(m_module->getFunction("\01?setModelOption@@YAX_N@Z"), m_builder->getInt1(hasOr));
		}

		TraverseFuncTree(function_tree);
		function_tree = nullptr;
		m_InFunc = false;

		if (Setting::IsModeling()) //建模的时候每个函数都有属于自己的静态变量 $$函数名_num，主函数中的在这里声明，其它函数的在func2IR中声明
			new GlobalVariable(*m_module, IntegerType::get(m_module->getContext(), 32), false, GlobalValue::ExternalLinkage, ConstantInt::get(m_module->getContext(), APInt(32, 0)), "$$main_num");

		Stmt2IR(IRTree->GetRoot());

		m_builder->CreateRet(m_builder->getInt32(0));
		delete IRTree->GetRoot();
		IRTree->SetRoot(nullptr);
	}

	std::error_code ec;
	raw_fd_ostream S(Setting::getOutBCFilePath(), ec, llvm::sys::fs::F_RW);
	S << *m_module; S.flush(); S.close();

	if (!Setting::isFuncFile() && Setting::isGenExe()){
		system((CLANGCMD + Setting::getHome() + "ThreadLib.lib " + Setting::getHome() + "Modeling.lib " + Setting::getOutBCFilePath() + " -o " + Setting::getOutFilePath()).data());
#ifndef	RELEASE_MC
		system(Setting::getOutFilePath().data());
#endif
	}
}

void IR::__InitWinStruct()
{
	map<string, int> name_pos;
	vector<bool> sign;
	IRStruct * s;
	//RTL_CRITICAL_SECTION
	name_pos["DebugInfo"] = 0;    name_pos["LockCount"] = 1;     name_pos["RecursionCount"] = 2;
	name_pos["OwningThread"] = 3; name_pos["LockSemaphore"] = 4; name_pos["SpinCount"] = 5;
	sign.push_back(1); sign.push_back(1); sign.push_back(1);
	sign.push_back(0); sign.push_back(0); sign.push_back(0);
	s = new IRStruct(m_module->getTypeByName("struct._RTL_CRITICAL_SECTION"), name_pos, sign, 24, 4);
	m_IRStruct["struct._RTL_CRITICAL_SECTION"] = s;

	//RTL_CRITICAL_SECTION_DEBUG
	name_pos["Type"] = 0;    name_pos["CreatorBackTraceIndex"] = 1;     name_pos["CriticalSection"] = 2;
	name_pos["ProcessLocksList"] = 3; name_pos["EntryCount"] = 4; name_pos["ContentionCount"] = 5;
	name_pos["Flags"] = 6; name_pos["CreatorBackTraceIndexHigh"] = 7; name_pos["SpareWORD"] = 8;
	sign.push_back(1); sign.push_back(1); sign.push_back(1);
	sign.push_back(1); sign.push_back(1); sign.push_back(1);
	sign.push_back(1); sign.push_back(1); sign.push_back(1);
	s = new IRStruct(m_module->getTypeByName("struct._RTL_CRITICAL_SECTION"), name_pos, sign, 40, 4);
	m_IRStruct["struct._RTL_CRITICAL_SECTION_DEBUG"] = s;

	//LIST_ENTRY
	name_pos["Flink"] = 0;    name_pos["Blink"] = 1;
	sign.push_back(1); sign.push_back(1);
	s = new IRStruct(m_module->getTypeByName("struct._RTL_CRITICAL_SECTION"), name_pos, sign, 8, 4);
	m_IRStruct["struct._LIST_ENTRY"] = s;

	//timecaps_tag
	name_pos["wPeriodMin"] = 0;    name_pos["wPeriodMax"] = 1;
	sign.push_back(0); sign.push_back(0);
	s = new IRStruct(m_module->getTypeByName("struct.timecaps_tag"), name_pos, sign, 8, 4);
	m_IRStruct["struct.timecaps_tag"] = s;

	//_SECURITY_ATTRIBUTES
	name_pos["nLength"] = 0;    name_pos["lpSecurityDescriptor"] = 1; name_pos["bInheritHandle "] = 1;
	sign.push_back(1); sign.push_back(0); sign.push_back(1);
	s = new IRStruct(m_module->getTypeByName("struct._SECURITY_ATTRIBUTES"), name_pos, sign, 12, 4);
	m_IRStruct["struct._SECURITY_ATTRIBUTES"] = s;
}

void IR::__InitWinApiMap()
{
	m_WinApiMap.insert({ "Sleep", "\01_Sleep@4" });
	m_WinApiMap.insert({ "EnterCriticalSection", "\01_EnterCriticalSection@4" });
	m_WinApiMap.insert({ "LeaveCriticalSection", "\01_LeaveCriticalSection@4" });
	m_WinApiMap.insert({ "InitializeCriticalSection", "\01_InitializeCriticalSection@4" });
	m_WinApiMap.insert({ "WaitForSingleObject", "\01_WaitForSingleObject@8" });
	m_WinApiMap.insert({ "ReleaseSemaphore", "\01_ReleaseSemaphore@12" });
	m_WinApiMap.insert({ "CreateSemaphore", "\01_CreateSemaphoreA@16" });
	m_WinApiMap.insert({ "SetThreadAffinityMask", "\01_SetThreadAffinityMask@8" });
	m_WinApiMap.insert({ "CreateThread", "\01_CreateThread@24" });
	m_WinApiMap.insert({ "SuspendThread", "\01_SuspendThread@4" });
	m_WinApiMap.insert({ "ResumeThread", "\01_ResumeThread@4" });
	m_WinApiMap.insert({ "GetCurrentThreadId", "\01_GetCurrentThreadId@0" });
	m_WinApiMap.insert({ "SetThreadPriority", "\01_SetThreadPriority@8" });
	m_WinApiMap.insert({ "SetThreadPriorityBoost", "\01_SetThreadPriorityBoost@8" });
	m_WinApiMap.insert({ "CloseHandle", "\01_CloseHandle@4" });
	m_WinApiMap.insert({ "CreateEvent", "\01_CreateEventA@16" });
	m_WinApiMap.insert({ "ResetEvent", "\01_ResetEvent@4" });
	m_WinApiMap.insert({ "timeSetEvent", "\01_timeSetEvent@20" });
	m_WinApiMap.insert({ "timeKillEvent", "\01_timeKillEvent@4" });
	m_WinApiMap.insert({ "timeEndPeriod", "\01_timeEndPeriod@4" });
	m_WinApiMap.insert({ "timeGetDevCaps", "\01_timeGetDevCaps@8" });
	m_WinApiMap.insert({ "timeBeginPeriod", "\01_timeBeginPeriod@4" });
	m_WinApiMap.insert({ "SetPriorityClass", "\01_SetPriorityClass@8" });
	m_WinApiMap.insert({ "GetTickCount", "\01_GetTickCount@0" });

	m_WinApiMap.insert({ "localtime", "\01?localtime@@YAPAUtm@@PB_J@Z" });
	m_WinApiMap.insert({ "DeleteCriticalSection", "\01_DeleteCriticalSection@4" });

	m_WinApiMap.insert({ "AreFileApisANSI", "\01_AreFileApisANSI@0" });
	m_WinApiMap.insert({ "01_CloseHandle", "\01_CloseHandle@4" });
	m_WinApiMap.insert({ "CreateFileA", "\01_CreateFileA@28" });
	m_WinApiMap.insert({ "CreateFileW", "\01_CreateFileW@28" });
	m_WinApiMap.insert({ "CreateFileMappingA", "\01_CreateFileMappingA@24" });
	m_WinApiMap.insert({ "CreateFileMappingW", "\01_CreateFileMappingW@24" });
	m_WinApiMap.insert({ "CreateMutexW", "\01_CreateMutexW@12" });
	m_WinApiMap.insert({ "DeleteFileA", "\01_DeleteFileA@4" });
	m_WinApiMap.insert({ "DeleteFileW", "\01_DeleteFileW@4" });
	m_WinApiMap.insert({ "FlushFileBuffers", "\01_FlushFileBuffers@4" });
	m_WinApiMap.insert({ "FormatMessageA", "\01_FormatMessageA@28" });
	m_WinApiMap.insert({ "FormatMessageW", "\01_FormatMessageW@28" });
	m_WinApiMap.insert({ "FreeLibrary", "\01_FreeLibrary@4" });
	m_WinApiMap.insert({ "GetCurrentProcessId", "\01_GetCurrentProcessId@0" });
	m_WinApiMap.insert({ "GetDiskFreeSpaceA", "\01_GetDiskFreeSpaceA@20" });
	m_WinApiMap.insert({ "GetDiskFreeSpaceW", "\01_GetDiskFreeSpaceW@20" });
	m_WinApiMap.insert({ "GetFileAttributesA", "\01_GetFileAttributesA@4" });
	m_WinApiMap.insert({ "GetFileAttributesW", "\01_GetFileAttributesW@4" });
	m_WinApiMap.insert({ "GetFileAttributesExW", "\01_GetFileAttributesExW@12" });
	m_WinApiMap.insert({ "GetFileSize", "\01_GetFileSize@8" });
	m_WinApiMap.insert({ "GetFullPathNameA", "\01_GetFullPathNameA@16" });
	m_WinApiMap.insert({ "GetFullPathNameW", "\01_GetFullPathNameW@16" });
	m_WinApiMap.insert({ "GetLastError", "\01_GetLastError@0" });
	m_WinApiMap.insert({ "GetSystemInfo", "\01_GetSystemInfo@4" });
	m_WinApiMap.insert({ "GetProcAddress", "\01_GetProcAddress@8" });
	m_WinApiMap.insert({ "GetSystemTime", "\01_GetSystemTime@4" });
	m_WinApiMap.insert({ "GetSystemTimeAsFileTime", "\01_GetSystemTimeAsFileTime@4" });
	m_WinApiMap.insert({ "GetTempPathA", "\01_GetTempPathA@8" });
	m_WinApiMap.insert({ "GetTempPathW", "\01_GetTempPathW@8" });
	m_WinApiMap.insert({ "GetTickCount", "\01_GetTickCount@0" });
	m_WinApiMap.insert({ "GetVersionExA", "\01_GetVersionExA@4" });
	m_WinApiMap.insert({ "GetVersionExW", "\01_GetVersionExW@4" });
	m_WinApiMap.insert({ "HeapAlloc", "\01_HeapAlloc@12" });
	m_WinApiMap.insert({ "HeapCreate", "\01_HeapCreate@12" });
	m_WinApiMap.insert({ "HeapDestroy", "\01_HeapDestroy@4" });
	m_WinApiMap.insert({ "HeapFree", "\01_HeapFree@12" });
	m_WinApiMap.insert({ "HeapReAlloc", "\01_HeapReAlloc@16" });
	m_WinApiMap.insert({ "HeapSize", "\01_HeapSize@12" });
	m_WinApiMap.insert({ "HeapValidate", "\01_HeapValidate@12" });
	m_WinApiMap.insert({ "HeapCompact", "\01_HeapCompact@8" });
	m_WinApiMap.insert({ "LoadLibraryA", "\01_LoadLibraryA@4" });
	m_WinApiMap.insert({ "LoadLibraryW", "\01_LoadLibraryW@4" });
	m_WinApiMap.insert({ "LocalFree", "\01_LocalFree@4" });
	m_WinApiMap.insert({ "LockFile", "\01_LockFile@20" });
	m_WinApiMap.insert({ "LockFileEx", "\01_LockFileEx@24" });
	m_WinApiMap.insert({ "MapViewOfFile", "\01_MapViewOfFile@20" });
	m_WinApiMap.insert({ "MultiByteToWideChar", "\01_MultiByteToWideChar@24" });
	m_WinApiMap.insert({ "QueryPerformanceCounter", "\01_QueryPerformanceCounter@4" });
	m_WinApiMap.insert({ "ReadFile", "\01_ReadFile@20" });
	m_WinApiMap.insert({ "SetEndOfFile", "\01_SetEndOfFile@4" });
	m_WinApiMap.insert({ "SetFilePointer", "\01_SetFilePointer@16" });
	m_WinApiMap.insert({ "Sleep", "\01_Sleep@4" });
	m_WinApiMap.insert({ "SystemTimeToFileTime", "\01_SystemTimeToFileTime@8" });
	m_WinApiMap.insert({ "UnlockFile", "\01_UnlockFile@20" });
	m_WinApiMap.insert({ "UnlockFileEx", "\01_UnlockFileEx@20" });
	m_WinApiMap.insert({ "UnmapViewOfFile", "\01_UnmapViewOfFile@4" });
	m_WinApiMap.insert({ "WideCharToMultiByte", "\01_WideCharToMultiByte@32" });
	m_WinApiMap.insert({ "WriteFile", "\01_WriteFile@20" });
	m_WinApiMap.insert({ "WaitForSingleObject", "\01_WaitForSingleObject@8" });
	m_WinApiMap.insert({ "WaitForSingleObjectEx", "\01_WaitForSingleObjectEx@12" });
	m_WinApiMap.insert({ "OutputDebugStringA", "\01_OutputDebugStringA@4" });
	m_WinApiMap.insert({ "OutputDebugStringW", "\01_OutputDebugStringW@4" });
	m_WinApiMap.insert({ "GetProcessHeap", "\01_GetProcessHeap@0" });
	m_WinApiMap.insert({ "FlushViewOfFile", "\01_FlushViewOfFile@8" });
}

/**
* 获取变量存放地址, 搜索符号表中是否存在该名字的符号
* @param name(变量名)
* @return 变量存放地址
*/
Value* IR::__STableSearch(string name, bool outputerror)
{
	// 搜索局部变量
	auto lit = m_IRSTable.find(name);
	if (lit != m_IRSTable.end()){
		prsnt_sign = lit->second->GetIsSigned();
		return lit->second->GetAllocaInstVar();
	}
	// 搜索全局符号（全局变量，函数名）
	auto git = m_GlobalSTable.find(name);
	if (git != m_GlobalSTable.end()){
		prsnt_sign = git->second->GetIsSigned();
		return git->second->GetGlobalVar();
	}
	// 搜索未定义函数表
	if (m_FuncNames.find(name) != m_FuncNames.end()){
		func2IR(m_Funcs.find(name)->second);
		return __STableSearch(name, outputerror);
	}

	// external.cpp中定义的符号
	ValueSymbolTable &tab = m_module->getValueSymbolTable();
	Value *value = tab.lookup(name);
	if (value != nullptr){
		return value;
	}
	// 输出错误
	if (outputerror)
		cerr << "ERROR: variable " << name << " does not been declared." << endl;
	return nullptr;
}

// mode: 0 stdin、 1 stdout、 2 stderr
Value* IR::__GetStdIO(int mode)
{
	Value *value = m_builder->CreateCall(m_module->getFunction("__iob_func"));
	return m_builder->CreateInBoundsGEP(value, m_builder->getInt32(mode));
}

//处理or的时候，预先将一些变量加入符号表
void IR::OrGlobal2IR()
{
	Value *var = nullptr;
	IRSymbol *symbol = nullptr;
	var = new GlobalVariable(*m_module, IntegerType::get(m_module->getContext(), 32), false,
		GlobalValue::ExternalLinkage, ConstantInt::get(m_module->getContext(), APInt(32, 0)), "$$len_i");
	symbol = new IRSymbol(var, true);
	if (m_GlobalSTable.insert({ "$$len_i", symbol }).second == false){
		cerr << "Global2IR : variable \"$$len_i\" has already defined" << endl;
	}

	var = new GlobalVariable(*m_module, IntegerType::get(m_module->getContext(), 32), false,
		GlobalValue::ExternalLinkage, ConstantInt::get(m_module->getContext(), APInt(32, 0)), "$$length");
	symbol = new IRSymbol(var, true);
	if (m_GlobalSTable.insert({ "$$length", symbol }).second == false){
		cerr << "Global2IR : variable \"$$length\" has already defined" << endl;
	}

	var = new GlobalVariable(*m_module, IntegerType::get(m_module->getContext(), 32), false,
		GlobalValue::ExternalLinkage, ConstantInt::get(m_module->getContext(), APInt(32, 0)), "$$current");
	symbol = new IRSymbol(var, true);
	if (m_GlobalSTable.insert({ "$$current", symbol }).second == false){
		cout << "Global2IR : variable \"$$current\" has already defined" << endl;
	}

	var = new GlobalVariable(*m_module, IntegerType::get(m_module->getContext(), 32), false,
		GlobalValue::ExternalLinkage, ConstantInt::get(m_module->getContext(), APInt(32, 0)), "$$pathNum");
	symbol = new IRSymbol(var, true);
	if (m_GlobalSTable.insert({ "$$pathNum", symbol }).second == false){
		cerr << "Global2IR : variable \"$$pathNum\" has already defined" << endl;
	}

	var = new GlobalVariable(*m_module, IntegerType::get(m_module->getContext(), 32), false,
		GlobalValue::ExternalLinkage, ConstantInt::get(m_module->getContext(), APInt(32, 0)), "$$firstTime");
	symbol = new IRSymbol(var, true);
	if (m_GlobalSTable.insert({ "$$firstTime", symbol }).second == false){
		cerr << "Global2IR : variable \"$$firstTime\" has already defined" << endl;
	}

	var = new GlobalVariable(*m_module, IntegerType::get(m_module->getContext(), 32), false,
		GlobalValue::ExternalLinkage, ConstantInt::get(m_module->getContext(), APInt(32, 0)), "$$flag1");
	symbol = new IRSymbol(var, true);
	if (m_GlobalSTable.insert({ "$$flag1", symbol }).second == false){
		cerr << "Global2IR : variable \"$$flag1\" has already defined" << endl;
	}

	var = new GlobalVariable(*m_module, IntegerType::get(m_module->getContext(), 32), false,
		GlobalValue::ExternalLinkage, ConstantInt::get(m_module->getContext(), APInt(32, 0)), "$$flag2");
	symbol = new IRSymbol(var, true);
	if (m_GlobalSTable.insert({ "$$flag2", symbol }).second == false){
		cerr << "Global2IR : variable \"$$flag2\" has already defined" << endl;
	}

	var = m_module->getGlobalVariable("\01?$$myJmpbuf@@3PAHA");
	symbol = new IRSymbol(var, true);
	if (m_GlobalSTable.insert({ "$$myJmpbuf", symbol }).second == false){
		cerr << "Global2IR : variable \"$$myJmpbuf\" has already defined" << endl;
	}

	Type *basictype = IntegerType::get(m_module->getContext(), 8);
	Type *arraytype = ArrayType::get(basictype, 1000000);
	var = new GlobalVariable(*m_module, arraytype, false, GlobalValue::ExternalLinkage, ConstantAggregateZero::get(arraytype), "$$flag");
	symbol = new IRSymbol(var, true);
	if (m_GlobalSTable.insert({ "$$flag", symbol }).second == false){
		cerr << "Global2IR : variable \"$$flag\" has already defined" << endl;
	}
}

/**
* 处理全局声明
* @param pTree(全局变量树)
*/
void IR::Global2IR(CSyntaxNode *pTree)
{
	if (pTree == nullptr)
		return;

	switch (pTree->GetNType()){
	case DECLARE_STA:
		__GlobalInit(pTree, true);	//带符号全局变量声明
		break;
	case CHOP_STA:	//遍历全局变量树
		Global2IR(pTree->GetChild0());
		Global2IR(pTree->GetChild1());
		break;
	default:	//非声明的全局变量树节点
		cerr << "ERROR: in Global variables tree has not-declare node." << endl;
	}
}

/**
* 处理全局变量初始化
* @param pTrlee(全局变量树) sign(是否有符号)
*/
void IR::__GlobalInit(CSyntaxNode *pTree, bool sign)
{
	//获得待声明的类型
	Type *type = GetType(pTree);
	if (type == nullptr){
		cerr << "ERROR: Basic Type is not considered." << endl;
		return;
	}
	__AsyncSubscript(pTree);
	CSyntaxNode *visit = pTree->GetChild0();
	while (visit != nullptr){
		VarInfo vInfo = __GetVarInfo(type, visit);
		ConstantAggregateZero *init = ConstantAggregateZero::get(vInfo.type);
		Value *var = new GlobalVariable(*m_module, vInfo.type, false, GlobalValue::ExternalLinkage, init, vInfo.name);

		IRSymbol *symbol = new IRSymbol(var, prsnt_sign);
		if (m_GlobalSTable.insert({ vInfo.name, symbol }).second == false){
			cerr << "ERROR : Global variable " << vInfo.name << " has already been defined." << endl;
		}

		if (pTree->GetRType() == FUNCPTYPE) // function pointer 节点的Child1是函数类型，不为空，所以要提前返回。
			return;
		visit = visit->GetChild1();
	}
}

//根据传入的基础类型和语法树
VarInfo IR::__GetPointerInfo(Type *type, CSyntaxNode *pTree)
{
	VarInfo vInfo;
	while (pTree != nullptr){
		switch (pTree->GetNType()){
			//case ARRAY_PARAMETER_EXP:
		case POINTER_EXP:
			type = PointerType::get(type, 0);
			break;
		case IDENT_EXP:
			vInfo.name = pTree->GetNName();
			vInfo.type = type;
			return vInfo;
		case ARRAY_EXP:
		case ARRAY_DECLARE_STA:
			return  __GetArrayInfo(type, pTree);
		}
		pTree = pTree->GetChild0();
	}
	vInfo.type = type;
	return vInfo;
}

//根据传入的基础类型和语法树，处理Array_Declare_STA的情况
VarInfo IR::__GetArrayInfo(Type *type, CSyntaxNode *pTree)
{
	VarInfo vInfo = {nullptr,""};
	CSyntaxNode *temp = pTree;

	vector<int> subscript;
	pTree = pTree->GetChild1();
	while (pTree != nullptr){
		if (pTree->GetChild0()->GetNType() == INTEGER_EXP){
			subscript.push_back(pTree->GetChild0()->GetiValue());
		}
		else if (pTree->GetChild0() != nullptr){
			Value *tempInt = __Expr2IR(pTree->GetChild0(), true);

			if (tempInt->getType()->isIntegerTy(32) && tempInt->getValueID() == Constant::ConstantIntVal){
				subscript.push_back(((ConstantInt*)tempInt)->getSExtValue());
			}
			else{
				cerr << "ERROR: The array subscript is not constant." << endl;
				return vInfo;
			}
		}
		else{
			cerr << "ERROR: The array's type DON'T have subscript." << endl;
			return vInfo;
		}
		pTree = pTree->GetChild1();
	}

	for (int i = subscript.size() - 1; i >= 0; i--)
		type = ArrayType::get(type, subscript.at(i));

	vInfo.type = type;
	return __GetPointerInfo(type, temp->GetChild0());
}

//根据传入的基础类型和语法树，处理变量的声明
VarInfo IR::__GetVarInfo(Type *type, CSyntaxNode * pTree)
{
	VarInfo vInfo;
	CSyntaxNode *visit = pTree->GetChild0();
	switch (visit->GetNType()){
	case IDENT_EXP:{
		vInfo.name = visit->GetNName();
		vInfo.type = type;
		return vInfo;
	}
	case POINTER_EXP:
		return __GetPointerInfo(type, visit);
	case ARRAY_EXP:
	case ARRAY_DECLARE_STA:
		return __GetArrayInfo(type, visit);
	default:
		cerr << "ERROR: The type is not considered in __GetVarInfo(). TypeID = " << visit->GetNType() << endl;
		vInfo.type = nullptr;
	}
	return vInfo;
}

//根据传入的基础类型和语法树，处理函数的形参、返回值
VarInfo IR::__GetFuncParaInfo(CSyntaxNode *pTree)
{
	if (pTree == nullptr || pTree->GetChild0() == nullptr){
		return{ nullptr, pTree != nullptr ? (pTree->GetNName()) : "" };
	}

	VarInfo vInfo;
	CSyntaxNode *visit = pTree->GetChild0();
	switch (visit->GetNType()){
	case IDENT_EXP:{
		vInfo.type = GetType(pTree);
		vInfo.name = visit->GetNName();
		break;
	}
	case POINTER_EXP:{
		Type *t = GetType(pTree);
		return __GetPointerInfo(t, visit);
	}
	case  FUNCP_PARAMETER_EXP:{
		Type *fType = GetType(pTree);
		return __GetVarInfo(fType, visit);
	}
	case ARRAY_DECLARE_STA:{
		Type *aType = GetType(pTree);
		return __GetArrayInfo(aType, visit);
	}
	default:
		cerr << "ERROR: undefined type in __GetFuncParaInfo()." << endl;
	}
	return vInfo;
}

void IR::Stmt2IR(CSyntaxNode *pTree)
{
	if (pTree == nullptr){
		//cerr << "ERROR: Program Tree is empty in Stmt2IR()" << endl;
		return;
	}
	switch (pTree->GetNType()){
	case DECLARE_STA:
		__Declr2IR(pTree, true);//处理signed类型的声明语句
		break;
	case CHOP_STA:
		__Chop2IR(pTree);
		break;
	case ASS_EQU_EXP:
	case EX_ASS_EXP:
		__Ass2IR(pTree);
		break;
	case STATE_OUTPUT_STA:
		__WhetherOutput(pTree);
		break;
	case WHILE_DO_STA:
		__While2IR(pTree);
		break;
	case IF_ELSE_STA:
		__If2IR(pTree);
		break;
	case FUNCTION_STA:
		__Call2IR(pTree);
		break;
	case EXT_FUNCTION_STA:
		__Call2IR(pTree->GetChild0());
		break;
	case DISPLAY_STA:
		__Output2IR(pTree);
		break;
	case REQUEST_STA:
		__Input2IR(pTree);
		break;
	case RETURN_STA:
		//__Return2IR(pTree);
		break;
		//全局变量清零
	case GLOBAL_INITSET_STA:{
		__GlbSetZero();
		//如果GLOBAL_INITSET_STA移位记得修改这里，这里是给全局变量的Chn首元素赋值的地方
		for (auto it = m_GlobalSTable.begin(); it != m_GlobalSTable.end(); ++it){
			Value *value = it->second->GetGlobalVar();
			if (value == nullptr)
				continue;
			Type *type = value->getType()->getContainedType(0);
			if (type->isArrayTy() && type->getContainedType(0)->isStructTy() && type->getContainedType(0)->getStructName().str() == "Chn")
				__AsyncAss(value);
		}
		break;
	}
	case CREATE_THREAD:
		__MyCreateThread(pTree);
		break;
	case CAL_PATH_TIME:
		//m_builder->CreateCall(m_module->getFunction("\01?printPathTime@@YAXXZ"));
		break;
	case GENERATE_EDGE_STA:
		__GenerateEdge(pTree);
		break;
	case DRAW_STA:
		__Draw();
		break;
	}
}

void IR::__Draw()
{
	Function *draw = m_module->getFunction("\01?Draw@@YAXXZ");
	m_builder->CreateCall(draw);
}

void IR::__GenerateEdge(CSyntaxNode* pTree)
{
	//对于纯C程序为了加快速度取消注释，否则应该注释
	/*if (!hasOr)
	{
	Function * AddEdge = m_module->getFunction("\01?AddEdge@@YAXPBDHH@Z");
	Value* varsInfo = m_module->getGlobalVariable("\01?$$varsInfo@@3PADA");
	vector<Value*> pos;
	pos.push_back(ConstantInt::get(m_module->getContext(), APInt(32, 0)));
	pos.push_back(ConstantInt::get(m_module->getContext(), APInt(32, 0)));
	varsInfo = m_builder->CreateInBoundsGEP(varsInfo, pos);

	m_builder->CreateCall3(AddEdge, varsInfo, m_builder->getInt32(pTree->GetiValue()), m_builder->getInt32(pTree->GetcValue()));
	return;
	}*/

	BasicBlock *IfEnd = nullptr;
	if (m_InFunc){
		Function *TheFunction = m_builder->GetInsertBlock()->getParent();
		BasicBlock *ThenBB = BasicBlock::Create(m_module->getContext(), "then", TheFunction);
		IfEnd = BasicBlock::Create(m_module->getContext(), "ifend", TheFunction);
		//条件跳转
		Value *ExtValue = m_builder->CreateLoad(__STableSearch("$$Ext"));
		Value *zero_int32 = m_builder->getInt32(0);   //创建int32位整数0
		Value *ExtCondition = m_builder->CreateICmpEQ(ExtValue, zero_int32);
		Value *v = m_builder->CreateCondBr(ExtCondition, ThenBB, IfEnd, 0);//如果等于0的话，表示是内部调用，此时执行then分支，即输出状态
		m_builder->SetInsertPoint(ThenBB);
	}

	m_builder->CreateCall(m_module->getFunction("$$GetVarsValue"));
	__GetLocalVar(pTree);

	Function *AddEdge = m_module->getFunction("\01?AddEdge@@YAXPBDHH@Z");

	Value *varsInfo = m_module->getGlobalVariable("\01?$$varsInfo@@3PADA");
	vector<Value*> pos;
	pos.push_back(ConstantInt::get(m_module->getContext(), APInt(32, 0)));
	pos.push_back(ConstantInt::get(m_module->getContext(), APInt(32, 0)));
	varsInfo = m_builder->CreateInBoundsGEP(varsInfo, pos);

	m_builder->CreateCall3(AddEdge, varsInfo, m_builder->getInt32(pTree->GetiValue()), m_builder->getInt32(pTree->GetcValue()));
	m_builder->CreateMemSet(varsInfo, m_builder->getInt8(0), 10000, 1, false);
	m_builder->CreateStore(m_builder->getInt32(0), m_module->getGlobalVariable("\01?$$tempLen@@3HA"));

	if (m_InFunc){
		m_builder->CreateBr(IfEnd);
		m_builder->SetInsertPoint(IfEnd);
	}
}

void IR::__GetVarsValue()
{
	BasicBlock *fatherinsertpoint = m_builder->GetInsertBlock();
	//构造函数类型
	FunctionType* FT = FunctionType::get(m_builder->getVoidTy(), vector<Type*>(0), false);
	//函数定义
	Function* Func = Function::Create(FT, Function::ExternalLinkage, "$$GetVarsValue", m_module);
	BasicBlock *entry = BasicBlock::Create(m_module->getContext(), "entry", Func, 0);
	m_builder->SetInsertPoint(entry);

	/*全局变量*/
	Function *addChar = m_module->getFunction(__GetRealFunctionName("$$addChar"));
	for (auto iter = m_GlobalSTable.begin(); iter != m_GlobalSTable.end(); iter++){
		string name = iter->first;
		if (name.find("$$") == 0 || name.find("##") == 0)
			continue; //以$$和##开头的变量什么都不做

		m_builder->CreateCall(addChar, m_builder->getInt8(' '));
		__CatValue(m_builder->CreateGlobalString(name + "="));

		Value *outputVar = __STableSearch(name);
		if (outputVar == nullptr)
			cerr << "ERROR: cannot find variable " << name << endl;
		else
			__CatValue(outputVar);
	}
	m_builder->CreateRetVoid();
	m_builder->SetInsertPoint(fatherinsertpoint);
}

void IR::__GetLocalVar(CSyntaxNode *pTree)
{
	Function *addChar = m_module->getFunction(__GetRealFunctionName("$$addChar"));
	/*局部变量*/
	if (!(pTree->GetST().empty())){ //如果有变量的话，进行如下操作，否则什么也不做
		__CatValue(m_builder->CreateGlobalString("   " + string(m_builder->GetInsertBlock()->getParent()->getName()) + ":    "));
		vector<string> outPutSymTbl = pTree->GetST();//需要输出的变量，用vector存储
		for (auto iter = outPutSymTbl.begin(); iter != outPutSymTbl.end(); iter++){
			string name = *iter;
			if (name.find("$$") == 0 || name.find("##") == 0)
				continue; //以$$和##开头的变量什么都不做

			m_builder->CreateCall(addChar, m_builder->getInt8(' '));
			__CatValue(m_builder->CreateGlobalString(name + "="));

			Value *outputVar = __STableSearch(name);
			if (outputVar == nullptr)
				cerr << "ERROR: cannot find variable " << name << endl;
			else
				__CatValue(outputVar);
		}
	}
}

void IR::__CatValue(Value *value) // add by wangmeng for unifiedmc 多线程
{
	Type *type = value->getType()->getContainedType(0);
	if (type->isIntegerTy(32)){
		Function * addInt = m_module->getFunction(__GetRealFunctionName("$$addInt"));
		value = m_builder->CreateLoad(value, true);
		m_builder->CreateCall(addInt, value);
		return;
	}
	if (type->isIntegerTy(8)){
		Function * addChar = m_module->getFunction(__GetRealFunctionName("$$addChar"));
		value = m_builder->CreateLoad(value, true);
		m_builder->CreateCall(addChar, value);
		return;
	}
	if (type->isFloatTy()){
		Function * addFloat = m_module->getFunction(__GetRealFunctionName("$$addFloat"));
		value = m_builder->CreateLoad(value, true);
		m_builder->CreateCall(addFloat, value);
		return;
	}
	if ((type->isPointerTy() || type->isArrayTy()) && type->getContainedType(0)->isIntegerTy(8)){
		Function * addString = m_module->getFunction(__GetRealFunctionName("$$addString"));
		value = __Cast2IR(value, m_builder->getInt8PtrTy(), true);
		m_builder->CreateCall(addString, value);
		return;
	}
	if (type->isPointerTy()){
		Function * addInt = m_module->getFunction(__GetRealFunctionName("$$addInt"));
		value = m_builder->CreateLoad(value, true);
		value = m_builder->CreatePtrToInt(value, IntegerType::get(m_module->getContext(), 32));
		m_builder->CreateCall(addInt, value);
		return;
	}
	if (type->isArrayTy() || type->isStructTy()){
		int elementNum;
		if (type->isStructTy())
			elementNum = type->getStructNumElements();
		else
			elementNum = type->getArrayNumElements();

		Function *addChar = m_module->getFunction(__GetRealFunctionName("$$addChar"));

		vector<Value*> subScript;
		m_builder->CreateCall(addChar, m_builder->getInt8('['));

		for (int i = 0; i < elementNum; i++){
			subScript.push_back(ConstantInt::get(m_module->getContext(), APInt(32, 0)));
			subScript.push_back(ConstantInt::get(m_module->getContext(), APInt(32, i))); //数组下标
			Value *arrayElement = m_builder->CreateInBoundsGEP(value, subScript); //取下标处的值
			__CatValue(arrayElement);
			subScript.clear();
			if (i != (elementNum - 1)){ //如果不是最后一个数组元素，输出元素之后，输出“，”，为了和下一个元素分开
				m_builder->CreateCall(addChar, m_builder->getInt8(','));
			}
		}
		m_builder->CreateCall(addChar, m_builder->getInt8(']'));
	}
}

/**
* 将声明转成对应的IR代码
* @param 传入待分析的语法树
* @return void
*/
void IR::__Declr2IR(CSyntaxNode *pTree, bool sign)
{
	Type *type = GetType(pTree); //获得待声明的类型
	if (type == nullptr){
		cerr << "ERROR: Basic Type is not considered in __Declr2IR(). TypeID = " << pTree->GetRType() << endl;
		return;
	}
	
	llvm::BasicBlock *bb = m_builder->GetInsertBlock();
	llvm::BasicBlock::iterator it = m_builder->GetInsertPoint();
	m_builder->SetInsertPoint((m_builder->GetInsertBlock()->getParent()->getEntryBlock().begin()++));

	bool isAsync = __AsyncSubscript(pTree);
	CSyntaxNode* visit = pTree->GetChild0();
	while (visit != nullptr){
		VarInfo vInfo = __GetVarInfo(type, visit);
		AllocaInst *allocDeclr = m_builder->CreateAlloca(vInfo.type);
		allocDeclr->setName(vInfo.name);
		allocDeclr->setAlignment(CalculateAlign(vInfo.type));

		IRSymbol *symbol = new IRSymbol(allocDeclr, prsnt_sign);

		if (m_IRSTable.insert({ vInfo.name, symbol }).second == false){
			cerr << "ERROR: variable " << vInfo.name << " has already been defined.(__Declr2IR())" << endl;
		}

		if (pTree->GetRType() == FUNCPTYPE) // function pointer 节点的Child1是函数类型，不为空，所以要提前返回。
			break;

		if (isAsync && visit->GetChild0()->GetNType() == ARRAY_DECLARE_STA)
			__AsyncAss(allocDeclr);

		visit = visit->GetChild1();
	}
	m_builder->SetInsertPoint(bb);
}

//全局变量清零
void IR::__GlbSetZero()
{
	m_builder->CreateStore(m_builder->getInt32(0), m_StNum);
	for (auto it = m_GlobalSTable.begin(); it != m_GlobalSTable.end(); it++){
		string name = it->first;
		if (name.at(0) == '$' && name.at(1) == '$')
			continue;

		Value * value = (*it).second->GetGlobalVar();
		Value * mem = __Cast2IR(value, Type::getInt8PtrTy(m_module->getContext()), false);
		Type * type = value->getType()->getContainedType(0);

		if (type->isFunctionTy())
			continue;

		int size = CalculateSize(type);
		int align = CalculateAlign(type);
		m_builder->CreateMemSet(mem, m_builder->getInt8(0), size, align, false);
	}
	m_builder->CreateMemSet(m_module->getGlobalVariable("\01?$$varsInfo@@3PADA"), m_builder->getInt8(0), 1000, 1, false);
	m_builder->CreateStore(m_builder->getInt32(0), m_module->getGlobalVariable("\01?$$tempLen@@3HA"));
}

/*
*为聚集类型赋初值，如数组、结构体
*@param  left(数组下标)  value(值)  right(要赋的值)
*/
void IR::__AssAggregateTy(Value *value, CSyntaxNode *right)
{
	if (value == nullptr || right == nullptr)
		return;

	Type *type = value->getType()->getContainedType(0);

	if (right->GetNType() == ELEMENT_LIST){
		right = right->GetChild0();
	}
	// char p[] = "xxxxxx"
	if (type->isArrayTy() && right->GetChild0() != nullptr && right->GetChild0()->GetNType() == STR_EXP){
		Type *ctype = type->getContainedType(0);
		if (ctype->isPointerTy()){
			// char *p[] = {"xxxxx","ddddd"}
		}
		else {
			Value *str = m_builder->CreateGlobalStringPtr(right->GetChild0()->GetsValue());
			int size = right->GetChild0()->GetsValue().size();
			int arraySize = type->getArrayNumElements();
			if (size > arraySize)
			{
				cerr << "ERROR: string is too long for char array." << endl;
				return;
			}
			m_builder->CreateMemCpy(value, str, size, 1);
			return;
		}
	}
	// MTYPE p[] = {}
	if (type->isArrayTy()){
		//获得数组的起始地址
		vector<Value*> pos;
		pos.push_back(ConstantInt::get(m_module->getContext(), APInt(32, 0)));
		pos.push_back(ConstantInt::get(m_module->getContext(), APInt(32, 0)));
		value = m_builder->CreateInBoundsGEP(value, pos);

		//遍历数组元素
		int len = type->getArrayNumElements();
		if (right->GetChild0() == nullptr)
			return;

		__AssAggregateTy(value, right->GetChild0());
		for (int i = 1; i < len; i++){
			if (right->GetChild1() == nullptr)
				return;
			right = right->GetChild1();
			if (right->GetChild0() == nullptr)
				return;
			value = m_builder->CreateInBoundsGEP(value, ConstantInt::get(m_module->getContext(), APInt(32, 1)));
			__AssAggregateTy(value, right->GetChild0());
		}
		if (right->GetChild1() != nullptr) {
			cerr << "ERROR: invalid assignment expression." << endl;
			return;
		}
	}
	// char *p = "xxxxxx"
	else if (type->isPointerTy() && right->GetChild0() != nullptr && right->GetChild0()->GetNType() == STR_EXP){
		Value *str = m_builder->CreateGlobalStringPtr(right->GetChild0()->GetsValue());
		m_builder->CreateStore(str, value)->setAlignment(CalculateAlign(type));
		return;
	}
	else if (type->isStructTy()){
		__AssStructTy(value, right);
		return;
	}
	else{
		if (right->GetNType() == ELEMENT)
			right = right->GetChild0();

		Value *RValue = __Expr2IR(right, true);
		RValue = __Cast2IR(RValue, type, prsnt_sign);

		if (RValue == nullptr){
			cerr << "ERROR: invalid assignment statement to array." << endl;
			return;
		}

		StoreInst *store = m_builder->CreateStore(RValue, value);
		store->setAlignment(CalculateAlign(type));
	}
}

/*
*为聚集类型赋初值，如数组、结构体
*@param  left(数组下表)  value(值)  right(要赋的值)
*/
void IR::__AssStructTy(Value *value, CSyntaxNode *right)
{
	Type *type = value->getType()->getContainedType(0);

	vector<Value*> pos;
	pos.push_back(ConstantInt::get(m_module->getContext(), APInt(32, 0)));

	for (int i = 0, n = type->getStructNumElements(); i < n; i++){
		pos.push_back(m_builder->getInt32(i));
		Value *v = m_builder->CreateInBoundsGEP(value, pos);
		pos.pop_back();

		Type *eleTy = type->getStructElementType(i);

		if (eleTy->isArrayTy()){
			__AssAggregateTy(v, right);
		}
		else if (eleTy->isStructTy()){
			__AssStructTy(v, right->GetChild0());
		}
		else{
			Value *RValue = __Expr2IR(right->GetChild0(), true);
			RValue = __Cast2IR(RValue, eleTy, prsnt_sign);

			if (RValue == nullptr){
				cerr << "ERROR: cannot init struct." << endl;
				return;
			}
			StoreInst *store = m_builder->CreateStore(RValue, v);
			store->setAlignment(CalculateAlign(eleTy));
		}

		right = right->GetChild1();
		if (right == nullptr)
			break;
	}
}

Type* IR::__CheckType(Type* LType, bool LSigned, Type* RType, bool RSigned)
{
	//bool || char
	if (LType->isIntegerTy(1) || LType->isIntegerTy(8)){
		if (RType->isIntegerTy(1) || RType->isIntegerTy(8) || RType->isIntegerTy(16)){
			prsnt_sign = true;
			return Type::getInt32Ty(m_module->getContext());
		}
		else if (RType->isIntegerTy(32) || RType->isIntegerTy(64)){
			prsnt_sign = RSigned;
			return RType;
		}
		else if (RType->isFloatTy() || RType->isDoubleTy()){
			prsnt_sign = true;
			return RType;
		}
		else if (RType->isPointerTy() || RType->isArrayTy()){
			prsnt_sign = RSigned;
			return RType;
		}
		else{
			cerr << "ERROR: CheckType: unsupported type" << endl;
			return LType;
		}
	}
	//short
	else if (LType->isIntegerTy(16)){
		if (RType->isIntegerTy(1) || RType->isIntegerTy(8)){
			prsnt_sign = LSigned;
			return LType;
		}
		else if (RType->isIntegerTy(16)){
			prsnt_sign = LSigned && RSigned;
			return LType;
		}
		else if (RType->isIntegerTy(32) || RType->isIntegerTy(64)){
			prsnt_sign = true;
			return RType;
		}
		else if (RType->isFloatTy() || RType->isDoubleTy()){
			prsnt_sign = true;
			return RType;
		}
		else if (RType->isPointerTy() || RType->isArrayTy()){
			prsnt_sign = RSigned;
			return RType;
		}
		else{
			cerr << "ERROR: CheckType: unsupported type" << endl;
			return LType;
		}
	}
	//int
	else if (LType->isIntegerTy(32)){
		if (RType->isIntegerTy(1) || RType->isIntegerTy(8) || RType->isIntegerTy(16)){
			prsnt_sign = LSigned;
			return LType;
		}
		else if (RType->isIntegerTy(32)){
			prsnt_sign = LSigned && RSigned;
			return LType;
		}
		else if (RType->isIntegerTy(64) || RType->isFloatTy() || RType->isDoubleTy()){
			prsnt_sign = true;
			return RType;
		}
		else if (RType->isPointerTy() || RType->isArrayTy()){
			prsnt_sign = RSigned;
			return RType;
		}
		else{
			cerr << "ERROR: CheckType: unsupported type" << endl;
			return LType;
		}
	}
	//float
	else if (LType->isFloatTy()){
		if (RType->isIntegerTy()){
			prsnt_sign = true;
			return LType;
		}
		else if (RType->isFloatTy()){
			prsnt_sign = true;
			return LType;
		}
		else if (RType->isDoubleTy()){
			prsnt_sign = true;
			return RType;
		}
		else if (RType->isPointerTy() || RType->isArrayTy()){
			prsnt_sign = RSigned;
			return RType;
		}
		else{
			cerr << "ERROR: CheckType: unsupported type" << endl;
			return LType;
		}
	}
	//double
	else if (LType->isDoubleTy()){
		if (RType->isIntegerTy() || RType->isFloatTy() || LType->isDoubleTy()){
			prsnt_sign = true;
			return LType;
		}
		else if (RType->isPointerTy() || RType->isArrayTy()){
			prsnt_sign = RSigned;
			return RType;
		}
		else{
			cerr << "ERROR: CheckType: unsupported type" << endl;
			return LType;
		}
	}
	//pointer or array
	else if (LType->isPointerTy() || LType->isArrayTy()){
		if (RType->isIntegerTy()){
			prsnt_sign = LSigned;
			return LType;
		}
		else if (RType->isFloatTy() || RType->isDoubleTy()){
			prsnt_sign = LSigned;
			return LType;
		}
		else if (RType->isPointerTy() || RType->isArrayTy()){
			prsnt_sign = LSigned;
			return LType;
		}
		else{
			cerr << "ERROR: CheckType: unsupported type" << endl;
			return LType;
		}
	}
	//unknown
	else{
		cerr << "ERROR: CheckType: unsupported type" << endl;
		return LType;
	}
}

Type* IR::__CheckType(Value *&LValue, bool LSigned, Value *&RValue, bool RSigned)
{
	Type *LType = LValue->getType();
	Type *RType = RValue->getType();
	//bool || char
	if (LType->isIntegerTy(1) || LType->isIntegerTy(8)){
		if (RType->isIntegerTy(1) || RType->isIntegerTy(8) || RType->isIntegerTy(16)){
			prsnt_sign = true;
			Type *rtn = Type::getInt32Ty(m_module->getContext());
			LValue = __Cast2IR(LValue, rtn, LSigned);
			RValue = __Cast2IR(RValue, rtn, RSigned);
			return rtn;
		}
		else if (RType->isIntegerTy(32) || RType->isIntegerTy(64)){
			prsnt_sign = RSigned;
			LValue = __Cast2IR(LValue, RType, LSigned);
			return RType;
		}
		else if (RType->isFloatTy() || RType->isDoubleTy()){
			prsnt_sign = true;
			LValue = __Cast2IR(LValue, RType, LSigned);
			return RType;
		}
		else if (RType->isPointerTy() || RType->isArrayTy()){
			prsnt_sign = RSigned;
			LValue = __Cast2IR(LValue, Type::getInt32Ty(m_module->getContext()), LSigned);
			Value *value = LValue;
			LValue = RValue;
			RValue = value;
			return RType;
		}
		else{
			cerr << "ERROR: CheckType: unsupported type" << endl;
			return LType;
		}
	}
	//short
	else if (LType->isIntegerTy(16)){
		if (RType->isIntegerTy(1) || RType->isIntegerTy(8)){
			prsnt_sign = LSigned;
			RValue = __Cast2IR(RValue, LType, RSigned);
			return LType;
		}
		else if (RType->isIntegerTy(16)){
			prsnt_sign = LSigned && RSigned;
			return LType;
		}
		else if (RType->isIntegerTy(32) || RType->isIntegerTy(64)){
			prsnt_sign = RSigned;
			LValue = __Cast2IR(LValue, RType, LSigned);
			return RType;
		}
		else if (RType->isFloatTy() || RType->isDoubleTy()){
			prsnt_sign = true;
			LValue = __Cast2IR(LValue, RType, LSigned);
			return RType;
		}
		else if (RType->isPointerTy() || RType->isArrayTy()){
			prsnt_sign = RSigned;
			LValue = __Cast2IR(LValue, Type::getInt32Ty(m_module->getContext()), LSigned);
			Value *value = LValue;
			LValue = RValue;
			RValue = value;
			return RType;
		}
		else{
			cerr << "ERROR: CheckType: unsupported type" << endl;
			return LType;
		}
	}
	//int || long
	else if (LType->isIntegerTy(32)){
		if (RType->isIntegerTy(1) || RType->isIntegerTy(8) || RType->isIntegerTy(16)){
			prsnt_sign = LSigned;
			RValue = __Cast2IR(RValue, LType, RSigned);
			return LType;
		}
		else if (RType->isIntegerTy(32)){
			prsnt_sign = LSigned && RSigned;
			return LType;
		}
		else if (RType->isIntegerTy(64)){
			prsnt_sign = RSigned;
			LValue = __Cast2IR(LValue, RType, LSigned);
			return RType;
		}
		else if (RType->isFloatTy() || RType->isDoubleTy()){
			prsnt_sign = true;
			LValue = __Cast2IR(LValue, RType, LSigned);
			return RType;
		}
		else if (RType->isPointerTy() || RType->isArrayTy()){
			prsnt_sign = RSigned;
			Value *value = LValue;
			LValue = RValue;
			RValue = value;
			return RType;
		}
		else{
			cerr << "ERROR: CheckType: unsupported type" << endl;
			return LType;
		}
	}
	// long long
	else if (LType->isIntegerTy(64)){
		if (RType->isIntegerTy(1) || RType->isIntegerTy(8) || RType->isIntegerTy(16) || RType->isIntegerTy(32)){
			prsnt_sign = LSigned;
			RValue = __Cast2IR(RValue, LType, RSigned);
			return LType;
		}
		else if (RType->isIntegerTy(64)){
			prsnt_sign = LSigned && RSigned;
			return LType;
		}
		else if (RType->isFloatTy() || RType->isDoubleTy()){
			prsnt_sign = true;
			LValue = __Cast2IR(LValue, RType, LSigned);
			return RType;
		}
		else if (RType->isPointerTy() || RType->isArrayTy()){
			prsnt_sign = RSigned;
			LValue = __Cast2IR(LValue, Type::getInt32Ty(m_module->getContext()), LSigned);
			Value *value = LValue;
			LValue = RValue;
			RValue = value;
			return RType;
		}
		else{
			cerr << "ERROR: CheckType: unsupported type" << endl;
			return LType;
		}
	}
	//float
	else if (LType->isFloatTy()){
		if (RType->isIntegerTy()){
			prsnt_sign = true;
			RValue = __Cast2IR(RValue, LType, RSigned);
			return LType;
		}
		else if (RType->isFloatTy()){
			prsnt_sign = true;
			return LType;
		}
		else if (RType->isDoubleTy()){
			prsnt_sign = true;
			LValue = __Cast2IR(LValue, RType, LSigned);
			return RType;
		}
		else if (RType->isPointerTy() || RType->isArrayTy()){
			prsnt_sign = RSigned;
			LValue = __Cast2IR(LValue, Type::getInt32Ty(m_module->getContext()), LSigned);
			Value *value = LValue;
			LValue = RValue;
			RValue = value;
			return RType;
		}
		else{
			cerr << "ERROR: CheckType: unsupported type" << endl;
			return LType;
		}
	}
	//double
	else if (LType->isDoubleTy()){
		if (RType->isIntegerTy() || RType->isFloatTy()){
			prsnt_sign = true;
			RValue = __Cast2IR(RValue, LType, RSigned);
			return LType;
		}
		else if (RType->isPointerTy() || RType->isArrayTy()){
			prsnt_sign = RSigned;
			LValue = __Cast2IR(LValue, Type::getInt32Ty(m_module->getContext()), LSigned);
			Value *value = LValue;
			LValue = RValue;
			RValue = value;
			return RType;
		}
		else{
			cerr << "ERROR: CheckType: unsupported type" << endl;
			return LType;
		}
	}
	//pointer or array
	else if (LType->isPointerTy() || LType->isArrayTy()){
		if (RType->isIntegerTy() || RType->isFloatTy() || RType->isDoubleTy()){
			prsnt_sign = LSigned;
			RValue = __Cast2IR(RValue, Type::getInt32Ty(m_module->getContext()), RSigned);
			return LType;
		}
		else if (RType->isPointerTy() || RType->isArrayTy()){
			if (LValue->getValueID() == Value::ConstantPointerNullVal){
				LValue = __Cast2IR(LValue, RType, LSigned);
				LSigned = RSigned;
			}
			else if (RValue->getValueID() == Value::ConstantPointerNullVal)
				RValue = __Cast2IR(RValue, LType, RSigned);

			prsnt_sign = LSigned;
			return LType;
		}
		else{
			cerr << "ERROR: CheckType: unsupported type" << endl;
			return LType;
		}
	}
	else{
		cerr << "ERROR: CheckType: unsupported type" << endl;
		return LType;
	}
}

// 处理chop类型的结点
void IR::__Chop2IR(CSyntaxNode *pTree)
{
	if (pTree == nullptr){
		cerr << "ERROR: invalid chop statement." << endl;
		return;
	}
	if (pTree->GetChild0() == nullptr || pTree->GetChild1() == nullptr){
		cerr << "ERROR: invalid chop statement." << endl;
		return;
	}
	Stmt2IR(pTree->GetChild0());
	Stmt2IR(pTree->GetChild1());
}

/**
* 赋值语句转成对应的IR代码
* @param 传入待分析的语法树
* @return void
*/
void IR::__Ass2IR(CSyntaxNode *pTree)
{
	if (pTree->GetChild0() == nullptr || pTree->GetChild1() == nullptr){
		cerr << "ERROR: invalid assignment expression." << endl;
		return;
	}

	if (pTree->GetChild1()->GetNType() == ELEMENT_LIST){
		Value *v = __Expr2IR(pTree->GetChild0(), false);
		__AssAggregateTy(v, pTree->GetChild1());
		return;
	}

	if (pTree->GetChild1()->GetNType() == STR_EXP){
		Value *str = m_builder->CreateGlobalStringPtr(pTree->GetChild1()->GetsValue());
		Value *left = __Expr2IR(pTree->GetChild0(),false);
		Type *type = left->getType();

		if (!type->isPointerTy()){
			cerr << "ERROR: invalid assignment expression, right value is string: " << pTree->GetChild1()->GetsValue() << endl;
			return;
		}

		type = type->getContainedType(0);

		if (type->isPointerTy()){
			m_builder->CreateStore(str, left)->setAlignment(CalculateAlign(type));
		}
		else if (type->isArrayTy()){
			int size = pTree->GetChild1()->GetsValue().size();
			if (size > type->getArrayNumElements()){
				cout << "ERROR: string is too long for char array. " << pTree->GetChild0()->GetNName() << endl;
				return;
			}
			m_builder->CreateMemCpy(left, str, size, 1);
			if (size < type->getArrayNumElements()){
				vector<Value*> subScript;
				subScript.push_back(m_builder->getInt32(0));
				subScript.push_back(m_builder->getInt32(size));
				m_builder->CreateStore(m_builder->getInt8('\0'), m_builder->CreateInBoundsGEP(left, subScript));
			}
		}
		else{
			cerr << "ERROR: invalid assignment expression, right value is string: " << pTree->GetChild1()->GetsValue() << endl;
		}
		return;
	}

	// 计算复杂的左值
	Value *LeftValue = __Expr2IR(pTree->GetChild0(), false);
	if (LeftValue == nullptr || !LeftValue->getType()->isPointerTy()){
		cerr << "ERROR: invalid assignment expression." << endl;
		return;
	}

	Type *LType = LeftValue->getType()->getContainedType(0);
	bool lsign = prsnt_sign;

	Value *v = __Expr2IR(pTree->GetChild1(), true);
	if (v == nullptr){
		cerr << "ERROR: invalid assignment expression." << endl;
		return;
	}
	bool rsign = prsnt_sign;
	prsnt_sign = lsign;
	Value *RightValue = __Cast2IR(v, LType, rsign);

	if (RightValue == nullptr){
		cerr << "ERROR: invalid assignment expression." << endl;
		return;
	}

	StoreInst *store = m_builder->CreateStore(RightValue, LeftValue);
	store->setAlignment(CalculateAlign(LType));
}

/**
* 表达式转成对应的IR代码
* @param 传入待分析的语法树
* @return 转换之后的结果
*/
Value* IR::__Expr2IR(CSyntaxNode *pTree, bool load)
{
	if (pTree == nullptr){
		cerr << "ERROR: invalid expression." << endl;
		return nullptr;
	}

	switch (pTree->GetNType()){
	case INTEGER_EXP:{
		return ConstantInt::get(m_module->getContext(), APInt(32, pTree->GetiValue()));
	}
	case TRUE_EXP:{
		return m_builder->getTrue();
	}
	case FALSE_EXP:{
		return m_builder->getFalse();
	}
	case UNARY_SUB_EXP:{
		Value *value = __Expr2IR(pTree->GetChild0(), load);
		if (value != nullptr){
			if (value->getType()->isIntegerTy()){
				value = m_builder->CreateNeg(value);
			}
			else if (value->getType()->isFloatTy() || value->getType()->isDoubleTy()){
				value = m_builder->CreateFNeg(value);
			}
		}
		return value;
	}
	case FLOATLITERAL_EXP:{
		return ConstantFP::get(m_module->getContext(), APFloat(pTree->GetfValue()));
	}
	case CHARLITERAL_EXP:{
		return ConstantInt::get(m_module->getContext(), APInt(8, pTree->GetcValue()));
	}
	case ADDRESS_EXP:{
		return __Expr2IR(pTree->GetChild0(), false);
	}
	case ADDR_VAR:{
		Value *value = __AddVar2IR(pTree);
		if (load)
			value = m_builder->CreateLoad(value);
		return value;
	}
	case ADDR_VAR_LIST:{
		Value * value = __AddVarList2IR(pTree);
		if (load)
			value = m_builder->CreateLoad(value);
		return value;
	}
	case NULL_EXP:{
		return ConstantPointerNull::get(PointerType::get(Type::getInt32Ty(m_module->getContext()), 0));
	}
	case ARRAY_EXP:{
		Value *value = __Array2IR(pTree, load);
		if (load && value != nullptr){
			value = __Load2IR(value);
		}
		return value;
	}
	case DOT_EXP:{
		Value *value = __Struct2IR(pTree, false);
		if (load && value != nullptr){
			value = __Load2IR(value);
		}
		return value;
	}
	case ARROW_EXP:{
		Value *value = __Struct2IR(pTree, true);
		if (load && value != nullptr){
			value = __Load2IR(value);
		}
		return value;
	}
	case POINTER_EXP:{
		Value *value = __Expr2IR(pTree->GetChild0(), load);
		if (value != nullptr){
			//带有指针加减不用读取，会在加减中读取, 函数的返回值也不要读取
			if (load || (pTree->GetChild0()->GetNType() != ADD_EXP && pTree->GetChild0()->GetNType() != SUB_EXP && pTree->GetChild0()->GetNType() != FUNCTION_STA))
			{
				value = __Load2IR(value);
			}
		}
		return value;
	}
	case STR_EXP:{
		return m_builder->CreateGlobalStringPtr(pTree->GetsValue());
	}
	case ARRAY_DECLARE_STA:
		pTree = pTree->GetChild0();
	case IDENT_EXP:{
		string name = pTree->GetNName();

		if (name == "stdin")
			return __GetStdIO(0);
		if (name == "stdout")
			return __GetStdIO(1);
		if (name == "stderr")
			return __GetStdIO(2);

		auto it = m_WinApiMap.find(name);
		if (it != m_WinApiMap.end()){
			name = it->second;
		}

		Function *f = m_module->getFunction(__GetRealFunctionName(name));
		if ( f != nullptr){
			load = false;
			return f;
		}

		// 未定义函数
		if (m_FuncNames.find(name) != m_FuncNames.end()){
			func2IR(m_Funcs[pTree->GetNName()]);
			load = false;
		}

		auto value = __STableSearch(name);
		if (load && value != nullptr){
			value = __Load2IR(value);
		}
		return value;
	}
	case ADD_EXP:{
		return __Add2IR(pTree);
	}
	case SUB_EXP:{
		return __Sub2IR(pTree);
	}
	case MUL_EXP:{
		return __Mul2IR(pTree);
	}
	case DIV_EXP:{
		return __Div2IR(pTree);
	}
	case MOD_EXP:{
		return __Mod2IR(pTree);
	}
	case EQU_EXP:{
		return __EQ2IR(pTree);
	}
	case NE_EXP:{
		return __NE2IR(pTree);
	}
	case GT_EXP:{
		return __GT2IR(pTree);
	}
	case LT_EXP:{
		return __LT2IR(pTree);
	}
	case GE_EXP:{
		return __GE2IR(pTree);
	}
	case LE_EXP:{
		return __LE2IR(pTree);
	}
	case NEGATION_EXP:{
		return __Neg2IR(pTree);
	}
	case XOR_EXP:{
		return __BXOr2IR(pTree);
	}
	case BNE_EXP:{
		return __BNeg2IR(pTree);
	}
	case BAN_EXP:{
		return __BAnd2IR(pTree);
	}
	case AND_EXP:{
		return __And2IR(pTree);
	}
	case BOR_EXP:{
		return __BOr2IR(pTree);
	}
	case OR_EXP:{
		return __Or2IR(pTree);
	}
	case LST_EXP:{
		return __Shl2IR(pTree);
	}
	case RST_EXP:{
		return __Shr2IR(pTree);
	}

	case FUNCTION_STA:{
		return __Call2IR(pTree);
	}
	case EXT_FUNCTION_STA:{
		return __Call2IR(pTree->GetChild0());
	}
	case TYPE_CAST_STA:{
		bool sign = prsnt_sign;
		Type *type = GetType(pTree->GetChild0());
		for (auto tt = pTree->GetChild0()->GetChild0(); tt != nullptr; tt = tt->GetChild0()){
			type = PointerType::get(type, 0);
		}
			
		Value *value = __Expr2IR(pTree->GetChild1(), true);
		
		value = __Cast2IR(value, type, sign);
		
		if (load)
			return value;

		AllocaInst *alloc = m_builder->CreateAlloca(type);
		alloc->setName("tempvar");
		alloc->setAlignment(CalculateAlign(type));
		m_builder->CreateStore(value, alloc);
		return alloc;
	}
	case SIZEOF_EXP:{
		return __IRSizeof(pTree);
	}
	case IF_ELSE_EXP:{
		return __ThreeOp2IR(pTree);
	}
	case ELEMENT:{
		return __Expr2IR(pTree->GetChild0(), true);
	}
	default:
		cerr << "ERROR: invalid expression. " << pTree->GetNName() << endl;
	}
	return nullptr;
}

//测试用
void IR::__PrintIRPrsnt(string str)
{
	std::vector<llvm::Type *> putsArgs;
	putsArgs.push_back(m_builder->getInt8PtrTy());
	llvm::ArrayRef<llvm::Type*>  argsRef(putsArgs);
	llvm::FunctionType *putsType = llvm::FunctionType::get(m_builder->getInt32Ty(), argsRef, true);
	llvm::Constant *putsFunc = m_module->getOrInsertFunction("printf", putsType);
	std::vector<llvm::Value *> sub;
	sub.push_back(m_builder->getInt32(0));
	sub.push_back(m_builder->getInt32(0));
	if (str == "$$firstTime"){
		Value *format = m_builder->CreateGlobalString("%d");
		m_builder->CreateCall2(putsFunc, m_builder->CreateInBoundsGEP(format, sub), m_builder->CreateLoad(__STableSearch("$$firstTime")));
		format = m_builder->CreateGlobalString("%s");
		m_builder->CreateCall2(putsFunc, m_builder->CreateInBoundsGEP(format, sub), m_builder->CreateGlobalString("\n"));
		return;
	}
	Value *format = m_builder->CreateGlobalString("%s");
	m_builder->CreateCall2(putsFunc, m_builder->CreateInBoundsGEP(format, sub), m_builder->CreateGlobalString(str + "\n"));

}

//调用$$开头的内部写死的函数时，替换为真正的函数名
string IR::__GetRealFunctionName(string name)
{
	if (name == "$$Sort")
		return "\01?$$sortPrsnt@@YAXXZ";
	if (name == "$$changeState")
		return name;
	if (name == "$$CreateElement")
		return "\01?$$CreateElement@@YAXPAPAPADHH@Z";
	if (name == "$$Enqueue")
		return "\01?$$Enqueue@@YAXPAPAPADPAPADHH@Z";
	if (name == "$$Dequeue")
		return "\01?$$Dequeue@@YAXPAPAPADPAPADHH@Z";
	if (name == "$$addChar")
		return "\01?$$addChar@@YAXD@Z";
	if (name == "$$addInt")
		return "\01?$$addInt@@YAXH@Z";
	if (name == "$$addFloat")
		return "\01?$$addFloat@@YAXM@Z";
	if (name == "$$addString")
		return "\01?$$addString@@YAXPAD@Z";
	if (name == "$$UpdateCurrentNode")
		return "\01?UpdateCurrentNode@@YAXXZ";
	if (name == "$$SaveCurrentNode")
		return "\01?SaveCurrentNode@@YAXXZ";
	if (name =="Spe_InterlockedCompareExchange")
		return "\01?Spe_InterlockedCompareExchange@@YAHPAJJJ@Z";
	if (name == "Spe_GetSystemInfo")
		return "\01?Spe_GetSystemInfo@@YAXPAUSPE_SYSTEM_INFO@@@Z";
	if (name == "Spe_GetVersionExA")
		return "\01?Spe_GetVersionExA@@YAXPAU_OSVERSIONINFOA@@@Z";
	if (name == "geterror")
		return "\01?geterror@@YAXXZ";

	return name;
}

//判断是否在函数中，如果在函数中，需要判断$$Ext变量
void IR::__WhetherOutput(CSyntaxNode *pTree)
{
	if (m_InFunc)//如果在函数中的话，要先判断一下$$Ext变量，如果是1的话，不调用__StateOut2IR函数，如果是0的话，调用__StateOut2IR函数
	{
		Function *TheFunction = m_builder->GetInsertBlock()->getParent();

		BasicBlock *ThenBB = BasicBlock::Create(m_module->getContext(), "then", TheFunction);
		BasicBlock *ElseBB = BasicBlock::Create(m_module->getContext(), "else", TheFunction);
		BasicBlock *IfEnd = BasicBlock::Create(m_module->getContext(), "ifend", TheFunction);

		//条件跳转
		Value* ExtValue = m_builder->CreateLoad(m_IRSTable["$$Ext"]->GetLocalVar());
		Value* zero_int32 = m_builder->getInt32(0);   //创建int32位整数0
		Value* ExtCondition = m_builder->CreateICmpEQ(ExtValue, zero_int32);

		Value *v = m_builder->CreateCondBr(ExtCondition, ThenBB, ElseBB, 0);//如果等于0的话，表示是内部调用，此时执行then分支，即输出状态

		m_builder->SetInsertPoint(ThenBB);
		__StateOut2IR(pTree);
		m_builder->CreateBr(IfEnd);

		m_builder->SetInsertPoint(ElseBB);
		m_builder->CreateBr(IfEnd);

		m_builder->SetInsertPoint(IfEnd);
	}
	else//不是函数调用时，直接就调用__Out2IR函数
		__StateOut2IR(pTree);
}

void IR::__CreateIsOutput()
{
	BasicBlock *fatherinsertpoint = m_builder->GetInsertBlock();
	//构造函数类型
	FunctionType* FT = FunctionType::get(m_builder->getInt1Ty(), vector<Type*>(0), false);
	//函数定义
	Function* Func = Function::Create(FT, Function::ExternalLinkage, "$$IsOutput", m_module);
	BasicBlock *entry = BasicBlock::Create(m_module->getContext(), "entry", Func, 0);
	m_builder->SetInsertPoint(entry);
	Function *TheFunction = m_builder->GetInsertBlock()->getParent();
	BasicBlock *ThenBB = BasicBlock::Create(m_module->getContext(), "then", TheFunction);

	//条件跳转
	Value * v = m_builder->CreateICmpEQ(m_builder->CreateLoad(m_StNum), m_builder->getInt32(Setting::getMaxStatus()));
	BasicBlock *IfEnd = BasicBlock::Create(m_module->getContext(), "ifend", TheFunction);
	m_builder->CreateCondBr(v, ThenBB, IfEnd, 0);
	m_builder->SetInsertPoint(ThenBB);

	m_builder->CreateCall(m_module->getFunction("exit"), m_builder->getInt32(0));
	m_builder->CreateBr(IfEnd);
	m_builder->SetInsertPoint(IfEnd);

	Value * temp = m_builder->CreateSRem(m_builder->CreateLoad(m_StNum), m_builder->getInt32(Setting::getStatusInterval()));
	temp = m_builder->CreateICmpEQ(temp, m_builder->getInt32(0));

	m_builder->CreateRet(temp);
	m_builder->SetInsertPoint(fatherinsertpoint);
}


//调用printf输出变量的值
void IR::__StateOut2IR(CSyntaxNode *pTree)
{
	static Value *strstate = m_builder->CreateGlobalStringPtr("#State");
	static Value *numF = m_builder->CreateGlobalStringPtr("[%d]");
	static Value *eqsF = m_builder->CreateGlobalStringPtr("=");
	static Value *strand = m_builder->CreateGlobalStringPtr(" and ");
	static Value *nl = m_builder->CreateGlobalStringPtr("\n");

	Function *TheFunction = m_builder->GetInsertBlock()->getParent();
	BasicBlock *ThenBB = BasicBlock::Create(m_module->getContext(), "then", TheFunction);

	//条件跳转
	Value * v = m_builder->CreateCall(m_module->getFunction("$$IsOutput"));
	BasicBlock *IfEnd = BasicBlock::Create(m_module->getContext(), "ifend", TheFunction);
	m_builder->CreateCondBr(v, ThenBB, IfEnd, 0);
	m_builder->SetInsertPoint(ThenBB);

	//先声明printf函数，然后根据不同的变量类型，进行调用
	std::vector<llvm::Type *> putsArgs;
	putsArgs.push_back(m_builder->getInt8Ty()->getPointerTo());
	llvm::ArrayRef<llvm::Type*>  argsRef(putsArgs);
	llvm::FunctionType *putsType = llvm::FunctionType::get(m_builder->getInt32Ty(), argsRef, true);
	llvm::Constant *putsFunc = m_module->getOrInsertFunction("printf", putsType);

	//每次输出变量的值之前，先输出是第几个状态,如state 0：
	m_builder->CreateCall(putsFunc, strstate);
	LoadInst *m_StNumVal = m_builder->CreateLoad(m_StNum, true);
	m_builder->CreateCall2(putsFunc, numF, m_StNumVal);

	m_builder->CreateCall(m_module->getFunction("$$GlobalOutput"));

	if (pTree->GetST().size() != 0)//如果有变量的话，进行如下操作，否则什么也不做
	{
		vector<string> outPutSymTbl;//需要输出的变量，用vector存储
		outPutSymTbl = pTree->GetST();

		vector<string>::iterator iter;
		bool globalFlag = 0;//增加一个标志位，表示输出的变量是否是全局变量
		for (iter = outPutSymTbl.begin(); iter != outPutSymTbl.end(); iter++){
			string a = *iter;
			int position = a.find("##");//要么是##函数名，要么是##Global
			if (position == 0){
				string outputString(a, 2);
				m_builder->CreateCall(putsFunc, m_builder->CreateGlobalStringPtr(outputString + ": "));
				if (a == "##Global")
					globalFlag = 1;//表示下面的变量都是全局变量，将标志位置为1
				else
					globalFlag = 0;//将标志位置为0
			}
			else{
				position = a.find("$$");
				if (position != 0){
					//对于每一个变量，输出形式如下x=1
					m_builder->CreateCall(putsFunc, m_builder->CreateGlobalStringPtr(*iter));
					m_builder->CreateCall(putsFunc, eqsF); // "="
					Value *outPutVar;
					if (globalFlag == 1){ //全局变量
						outPutVar = m_GlobalSTable[*iter]->GetGlobalVar();//全局变量，通过变量的名字在m_IRSTable中找到对应的AllocaInst类型指针
						prsnt_sign = m_GlobalSTable[*iter]->GetIsSigned();//增加获取符号，区别signed和unsigned，方便下面输出
					}
					else{ //局部变量
						outPutVar = m_IRSTable[*iter]->GetLocalVar();//局部变量，通过变量的名字在m_IRSTable中找到对应的AllocaInst类型指针
						prsnt_sign = m_IRSTable[*iter]->GetIsSigned();//增加获取符号，区别signed和unsigned，方便下面输出
					}
					__TypeOutput(putsFunc, outPutVar);
					m_builder->CreateCall(putsFunc, strand);//每个变量输出之后，输出两个空格，以便和下一个变量的输出隔开
				}
			}
		}
	}
	m_builder->CreateCall(putsFunc, nl);//每个状态输出之后，换行

	m_builder->CreateBr(IfEnd);
	m_builder->SetInsertPoint(IfEnd);
	__AddOne2IR(m_StNum);//将状态数加1
}

//对不同的基本类型进行输出
void IR::__TypeOutput(Constant* putsFunc, Value* outPutVar)
{
	static Value *dF = m_builder->CreateGlobalStringPtr("%d");
	static Value *uF = m_builder->CreateGlobalStringPtr("%u");
	static Value *ldF = m_builder->CreateGlobalStringPtr("%ld");
	static Value *luF = m_builder->CreateGlobalStringPtr("%lu");
	static Value *hdF = m_builder->CreateGlobalStringPtr("%hd");
	static Value *huF = m_builder->CreateGlobalStringPtr("%hu");
	static Value *fF = m_builder->CreateGlobalStringPtr("%.15f");
	static Value *lfF = m_builder->CreateGlobalStringPtr("%lf");
	static Value *cF = m_builder->CreateGlobalStringPtr("%c");
	static Value *sF = m_builder->CreateGlobalStringPtr("%s");
	static Value *pF = m_builder->CreateGlobalStringPtr("%p");

	static Value *lsbF = m_builder->CreateGlobalStringPtr("[");
	static Value *rsbF = m_builder->CreateGlobalStringPtr("]");
	static Value *cmF = m_builder->CreateGlobalStringPtr(",");

	Type *type = outPutVar->getType()->getContainedType(0);
	
	if (type->isIntegerTy(8)){
		LoadInst *a = m_builder->CreateLoad(outPutVar, 1);
		if (prsnt_sign)
			m_builder->CreateCall2(putsFunc, cF, a);
		else
			m_builder->CreateCall2(putsFunc, uF, a);
	}
	else if (type->isIntegerTy(16)){
		LoadInst *a = m_builder->CreateLoad(outPutVar, true);
		if (prsnt_sign)
			m_builder->CreateCall2(putsFunc, hdF, a);
		else
			m_builder->CreateCall2(putsFunc, huF, a);
	}
	else if (type->isIntegerTy(32)){
		LoadInst *a = m_builder->CreateLoad(outPutVar, true);
		if (prsnt_sign)
			m_builder->CreateCall2(putsFunc, dF, a);
		else
			m_builder->CreateCall2(putsFunc, uF, a);
	}
	else if (type->isIntegerTy(64)){
		LoadInst *a = m_builder->CreateLoad(outPutVar, true);
		if (prsnt_sign)
			m_builder->CreateCall2(putsFunc, ldF, a);
		else
			m_builder->CreateCall2(putsFunc, luF, a);
	}
	else if (type->isFloatTy()){
		LoadInst *a = m_builder->CreateLoad(outPutVar);
		a->setAlignment(CalculateAlign(type));
		Value* f2d = m_builder->CreateFPExt(a, Type::getDoubleTy(m_module->getContext()));
		m_builder->CreateCall2(putsFunc, fF, f2d);
	}
	else if (type->isDoubleTy()){
		LoadInst *a = m_builder->CreateLoad(outPutVar);
		a->setAlignment(CalculateAlign(type));
		m_builder->CreateCall2(putsFunc, lfF, a);
	}
	else if (type->isPointerTy())
	{
		m_builder->CreateCall2(putsFunc, sF, m_builder->CreateLoad(outPutVar, true));
	}
	else if (type->isArrayTy() && type->getContainedType(0)->isIntegerTy(8))
	{
		m_builder->CreateCall2(putsFunc, sF, outPutVar);
	}
	else if (type->isPointerTy())//指针输出
	{
		LoadInst *a = m_builder->CreateLoad(outPutVar, true);
		//添加一次强制类型转换，直接输出指针类型为0，为了体现地址故添加强制类型转换
		m_builder->CreateCall2(putsFunc, dF, m_builder->CreatePtrToInt(a, IntegerType::get(m_module->getContext(), 32)));
	}
	else if (type->isArrayTy())//数组类型,add by yubin 2015/5/15
	{
		int elementNum = type->getArrayNumElements();
		vector<Value*> subScript;
		m_builder->CreateCall(putsFunc, lsbF);
		for (int i = 0; i < elementNum; i++)
		{
			subScript.push_back(ConstantInt::get(m_module->getContext(), APInt(32, 0)));
			//数组下标
			subScript.push_back(ConstantInt::get(m_module->getContext(), APInt(32, i)));
			//取下标处的值
			Value *arrayElement = m_builder->CreateInBoundsGEP(outPutVar, subScript);
			__TypeOutput(putsFunc, arrayElement);
			subScript.clear();
			if (i != (elementNum - 1))//如果不是最后一个数组元素，输出元素之后，输出“，”，为了和下一个元素分开
				m_builder->CreateCall(putsFunc, cmF);
		}
		m_builder->CreateCall(putsFunc, rsbF);
	}
	else if (type->isStructTy())
	{
		int elementNum = type->getStructNumElements();
		vector<Value*> subScript;
		m_builder->CreateCall(putsFunc, lsbF);

		for (int i = 0; i < elementNum; i++)
		{
			subScript.push_back(ConstantInt::get(m_module->getContext(), APInt(32, 0)));
			//数组下标
			subScript.push_back(ConstantInt::get(m_module->getContext(), APInt(32, i)));
			//取下标处的值
			Value *arrayElement = m_builder->CreateInBoundsGEP(outPutVar, subScript);
			__TypeOutput(putsFunc, arrayElement);
			subScript.clear();
			if (i != (elementNum - 1))//如果不是最后一个数组元素，输出元素之后，输出“，”，为了和下一个元素分开
				m_builder->CreateCall(putsFunc, cmF);
		}
		m_builder->CreateCall(putsFunc, rsbF);
	}
}

//为了减小输出语句的规模，将全局变量的输出变为一个函数
void IR::__CreateGlbOutFunc()
{
	static Value *strglobal = m_builder->CreateGlobalStringPtr("Global: ");
	static Value *strand = m_builder->CreateGlobalStringPtr(" and ");

	BasicBlock *fatherinsertpoint = m_builder->GetInsertBlock();
	//构造函数类型
	FunctionType *FT = FunctionType::get(m_builder->getVoidTy(), vector<Type*>(0), false);
	//函数定义
	Function* Func = Function::Create(FT, Function::ExternalLinkage, "$$GlobalOutput", m_module);
	BasicBlock *entry = BasicBlock::Create(m_module->getContext(), "entry", Func, 0);
	m_builder->SetInsertPoint(entry);

	std::vector<llvm::Type *> putsArgs;
	putsArgs.push_back(m_builder->getInt8Ty()->getPointerTo());
	llvm::ArrayRef<llvm::Type*>  argsRef(putsArgs);
	llvm::FunctionType *putsType = llvm::FunctionType::get(m_builder->getInt32Ty(), argsRef, true);
	llvm::Constant *putsFunc = m_module->getOrInsertFunction("printf", putsType);

	m_builder->CreateCall(putsFunc, strglobal);

	for (auto it = m_GlobalSTable.begin(); it != m_GlobalSTable.end(); it++)
	{
		string name = (*it).first;
		if (name.at(0) == '$' && name.at(1) == '$')
			continue;

		Value *value = (*it).second->GetGlobalVar();
		prsnt_sign = (*it).second->GetIsSigned();

		Type *type = value->getType()->getContainedType(0);
		if (type->isFunctionTy())
			continue;

		m_builder->CreateCall(putsFunc, m_builder->CreateGlobalStringPtr(name + "="));
		__TypeOutput(putsFunc, value);
		m_builder->CreateCall(putsFunc, strand);
	}

	m_builder->CreateRetVoid();
	m_builder->SetInsertPoint(fatherinsertpoint);
}


Value* IR::__Array2IR(CSyntaxNode* pTree, bool load)
{
	if (pTree == nullptr){
		cerr << "ERROR: invalid array expression." << endl;
		return nullptr;
	}
	//数组首元素地址
	Value *value = __Expr2IR(pTree->GetChild0(), false);
	if (value == nullptr){
		cerr << "ERROR: invalid array expression owing to invalid array identifier." << endl;
		return nullptr;
	}

	bool temp = prsnt_sign;

	vector<Value*> indexs;
	indexs.push_back(ConstantInt::get(m_module->getContext(), APInt(32, 0)));

	CSyntaxNode *option = pTree->GetChild1();
	while (option != nullptr){
		Value *num = __Expr2IR(option->GetChild0(), true); //数组下标
		if (num == nullptr){
			cerr << "ERROR: invalid array subscript." << endl;
			num = ConstantInt::get(m_module->getContext(), APInt(32, 0));
		}

		Type *nTy = num->getType();
		if (!nTy->isIntegerTy(32)){
			num = __Cast2IR(num, Type::getInt32Ty(m_module->getContext()), prsnt_sign);
		}

		indexs.push_back(num);
		option = option->GetChild1();
	}

	Type *type = value->getType();
	Type *cType = type->getContainedType(0);

	if (cType->isPointerTy()){ // 指针当做数组用的情况
		if (indexs.size() != 2){
			cerr << "ERROR: cannot use pointer as a multidimensional array."<< endl;
		}
		auto v = m_builder->CreateLoad(value, true);
		value = m_builder->CreateInBoundsGEP(v, indexs[1]);
	}
	else if (cType->isArrayTy()){ // 数组的情况
		value = m_builder->CreateInBoundsGEP(value, indexs); // 取下标处的值
	}
	else{
		if (indexs.size() != 2){
			cerr << "ERROR: cannot use pointer as a multidimensional array." << endl;
		}
		value = m_builder->CreateInBoundsGEP(value, indexs[1]);
	}

	prsnt_sign = temp;
	return value;
}

Value* IR::__ValueList2IR(CSyntaxNode * pTree)
{
	//数组地址
	Value *value = __Expr2IR(pTree->GetChild0(), false);
	if (value == nullptr){
		cerr << "ERROR: invalid value list statement." << endl;
		return nullptr;
	}

	bool temp = prsnt_sign;
	pTree = pTree->GetChild1();
	while (pTree != nullptr && pTree->GetChild0() != nullptr)
	{
		//数组下标
		Value *num = __Expr2IR(pTree->GetChild0(), true);
		if (num == nullptr){
			cerr << "ERROR: invalid array subscript expression." << endl;
			num = ConstantInt::get(m_module->getContext(), APInt(32, 0));
		}
		
		if (prsnt_sign == false && (num->getType()->isIntegerTy(8) || num->getType()->isIntegerTy(16))){
			num = m_builder->CreateZExt(num, m_builder->getInt32Ty());
		}

		Type *type = value->getType()->getContainedType(0);
		
		if (type->isPointerTy()){ // 指针作为数组
			value = m_builder->CreateInBoundsGEP(m_builder->CreateLoad(value, true), num);
		}
		else if (type->isArrayTy()){ // 本身是数组
			vector<Value*> subScript;
			subScript.push_back(ConstantInt::get(m_module->getContext(), APInt(32, 0)));
			subScript.push_back(num);
			value = m_builder->CreateInBoundsGEP(value, subScript);
		}
		else{ // 其他
			cerr << "ERROR: invalid array subscript expression." << endl;
			PAUSE;
			exit(-1);
		}
		pTree = pTree->GetChild1();
	}
	prsnt_sign = temp;
	return value;
}

void IR::__Return2IR(CSyntaxNode* pTree)
{
	m_builder->CreateRet(__Expr2IR(pTree->GetChild0(), true));
}

/**
* input操作转成对应的IR代码
* @param 语法树
* @return void
*/
void IR::__Input2IR(CSyntaxNode* pTree)
{
	static Value *dF = m_builder->CreateGlobalStringPtr("%d");
	static Value *ldF = m_builder->CreateGlobalStringPtr("%ld");
	static Value *hdF = m_builder->CreateGlobalStringPtr("%hd");
	static Value *cF = m_builder->CreateGlobalStringPtr("%c");
	static Value *fF = m_builder->CreateGlobalStringPtr("%f");
	static Value *sF = m_builder->CreateGlobalStringPtr("%s");

	CSyntaxNode* visitVar = pTree;
	while (visitVar != nullptr)
	{
		CSyntaxNode* visit = visitVar->GetChild0();

		//声明scanf函数
		std::vector<llvm::Type *> getsArgs;
		getsArgs.push_back(m_builder->getInt8PtrTy());
		llvm::ArrayRef<llvm::Type*>  argsRef(getsArgs);
		llvm::FunctionType *getsType = llvm::FunctionType::get(m_builder->getInt32Ty(), argsRef, true);
		llvm::Constant *putsFunc = m_module->getOrInsertFunction("scanf", getsType);

		Value *addr = __Expr2IR(visit, false);  //获取变量在符号表里的地址
		Type *valueType = addr->getType()->getContainedType(0);

		if (valueType->isIntegerTy(8))         //字符型char
			m_builder->CreateCall2(putsFunc, cF, addr);

		else if (valueType->isIntegerTy(16))    //整型int
			m_builder->CreateCall2(putsFunc, hdF, addr);

		else if (valueType->isIntegerTy(32))   //整型int
			m_builder->CreateCall2(putsFunc, dF, addr);

		else if (valueType->isIntegerTy(64))   //整型int
			m_builder->CreateCall2(putsFunc, ldF, addr);

		else if (valueType->isFloatTy())       //浮点型float
			m_builder->CreateCall2(putsFunc, fF, addr);

		else if ((valueType->isPointerTy() || valueType->isArrayTy())
			&& valueType->getContainedType(0)->isIntegerTy(8)) // 字符串
			m_builder->CreateCall2(putsFunc, sF, addr);

		visitVar = visitVar->GetChild1();
	}
}

/**
* output操作转成对应的IR代码
* @param 待处理的变量
* @return void
*/
void IR::__Output2IR(CSyntaxNode* pTree)
{
	static Value *dF = m_builder->CreateGlobalStringPtr("%d");
	static Value *uF = m_builder->CreateGlobalStringPtr("%u");
	static Value *ldF = m_builder->CreateGlobalStringPtr("%ld");
	static Value *luF = m_builder->CreateGlobalStringPtr("%lu");
	static Value *hdF = m_builder->CreateGlobalStringPtr("%hd");
	static Value *huF = m_builder->CreateGlobalStringPtr("%hu");
	static Value *fF = m_builder->CreateGlobalStringPtr("%.15f");
	static Value *lfF = m_builder->CreateGlobalStringPtr("%lf");
	static Value *cF = m_builder->CreateGlobalStringPtr("%c");
	static Value *sF = m_builder->CreateGlobalStringPtr("%s");
	static Value *pF = m_builder->CreateGlobalStringPtr("%p");

	if (pTree->GetChild0() == nullptr){
		cerr << "ERROR: invalid output expression." << endl;
		return;
	}
	//先声明printf函数，然后根据不同的变量类型，进行调用

	std::vector<llvm::Type *> putsArgs;
	putsArgs.push_back(m_builder->getInt8PtrTy());
	llvm::ArrayRef<llvm::Type*>  argsRef(putsArgs);
	llvm::FunctionType *putsType = llvm::FunctionType::get(m_builder->getInt32Ty(), argsRef, true);
	llvm::Constant *putsFunc = m_module->getOrInsertFunction("printf", putsType);

	for (pTree = pTree->GetChild0(); pTree != nullptr; pTree = pTree->GetChild1())
	{
		if (pTree->GetChild0()->GetNType() == STR_EXP)   //字符串
		{
			m_builder->CreateCall(putsFunc, m_builder->CreateGlobalStringPtr(pTree->GetChild0()->GetsValue()));
			continue;
		}
		else if (pTree->GetChild0()->GetNType() == IDENT_EXP)  //output(x)单独的变量
		{
			Value* value = __Expr2IR(pTree->GetChild0(), false);
			__TypeOutput(putsFunc, value);
			continue;
		}

		//output(表达式) ,output(数组),output(*p)
		Value *value = __Expr2IR(pTree->GetChild0(), true);
		if (value == nullptr)
			continue;
		Type* type = value->getType();

		if (type->isIntegerTy(32))    //int型
		{
			if (prsnt_sign)
				m_builder->CreateCall2(putsFunc, dF, value);
			else
				m_builder->CreateCall2(putsFunc, uF, value);
		}
		else if (type->isFloatTy())   //float型
		{// 制类型转换，将float类型转换成double类型，否则输出时会崩溃
			Value* floatTyToDoubleTy = m_builder->CreateFPExt(value, Type::getDoubleTy(m_module->getContext()));
			m_builder->CreateCall2(putsFunc, fF, floatTyToDoubleTy);
		}
		else if (type->isDoubleTy())   //double型
			m_builder->CreateCall2(putsFunc, lfF, value);
		else if (type->isIntegerTy(8)) //char型
		{
			if (prsnt_sign)
				m_builder->CreateCall2(putsFunc, cF, value);
			else
				m_builder->CreateCall2(putsFunc, uF, value);
		}
		else if (type->isIntegerTy(16)) //short型
		{
			if (prsnt_sign)
				m_builder->CreateCall2(putsFunc, hdF, value);
			else
				m_builder->CreateCall2(putsFunc, huF, value);
		}
		else if (type->isIntegerTy(64)) //long型
		{
			if (prsnt_sign)
				m_builder->CreateCall2(putsFunc, ldF, value);
			else
				m_builder->CreateCall2(putsFunc, luF, value);
		}
		else if (type->isPointerTy() && type->getContainedType(0)->isIntegerTy(8))
		{
			m_builder->CreateCall2(putsFunc, sF, value);
		}
		else if (type->isPointerTy())
		{
			//添加一次强制类型转换，直接输出指针类型为0，为了体现地址故添加强制类型转换（LLVM 3.4）
			m_builder->CreateCall2(putsFunc, dF, m_builder->CreatePtrToInt(value, IntegerType::get(m_module->getContext(), 32)));
		}
		else{
			cerr << "ERROR: invalid output parameter." << endl;
			PAUSE;
		}
	}
}


/**
*取得表达式中结构体的值
*@param pTree(结构体所在的语法树) StructP(代表操作符是 ->（true）.(false))
*@return 转换后的值
*/
Value* IR::__Struct2IR(CSyntaxNode *pTree, bool StructP)
{
	if (pTree == nullptr){
		cerr << "ERROR: invalid struct value expression." << endl;
		return nullptr;
	}
	string name = "Inner Struct";
	CSyntaxNode *nameNode = pTree->GetChild0()->GetNameNode();
	if (nameNode != nullptr)
		name = nameNode->GetNName();

	Value *value = __Expr2IR(pTree->GetChild0(), false);
	if (value == nullptr){
		cerr << "ERROR: invalid struct value expression." << endl;
		return nullptr;
	}

	Type *type = value->getType();
	if (StructP && type->isPointerTy() && type->getContainedType(0)->isPointerTy())
		value = __Load2IR(value); // value's type is pointer to struct
	
	value = __StructEle2IR(pTree->GetChild1(), value, name);
	return value;
}

/**
*取得表达式中结构体成员的值,递归处理结构体成员还是结构体的情况
*@param pTree(数组所在的语法树) value(传入结构体指针,最终返回结构体成员地址)
*@return
*/
Value* IR::__StructEle2IR(CSyntaxNode *pTree, Value *value, string sname)
{
	if (pTree == nullptr){
		cerr << "ERROR: invalid get struct element expression."<< endl;
		return nullptr;
	}

	string eleName = pTree->GetNName();

	IRStruct *s;
	if (pTree->GetNType() == ARROW_EXP)
		s = m_IRStruct[value->getType()->getContainedType(0)->getContainedType(0)->getStructName().str()];
	else{
		Type *type = value->getType();
		
		if (type->isPointerTy() && type->getContainedType(0)->isStructTy()){
			type = type->getContainedType(0);
		}
	
		if (!type->isStructTy()){
			cerr << "\nERROR: \"" << sname << "\" is NOT a struct value or pointer to struct." << endl;
			PAUSE;
			exit(-1);
		}

		sname = type->getStructName().str();
		auto fit = m_IRStruct.find(sname);
		if (fit == m_IRStruct.end()){
			cerr << "\nERROR: struct " << sname << " is undefined" << endl;
			PAUSE;
			exit(-1);
		}
		s = fit->second;
	}

	vector<Value*> pos;
	pos.push_back(ConstantInt::get(m_module->getContext(), APInt(32, 0)));

	switch (pTree->GetNType()){
	case IDENT_EXP:{
		int index = s->GetElementPos(eleName);
		if (index < 0){
			cerr << "ERROR: struct " << sname << " does not have element: " << eleName << endl;
			//PAUSE;
			//exit(-1);
			return nullptr;
		}
		pos.push_back(ConstantInt::get(m_module->getContext(), APInt(32, index)));
		prsnt_sign = s->GetIsSigned(eleName);
		value = m_builder->CreateInBoundsGEP(value, pos); // value's type is pointer to struct's element
		break;
	}
	case ARRAY_EXP:{
		value = __StructEle2IR(pTree->GetChild0(), value, sname);
		bool temp = prsnt_sign;
		pTree = pTree->GetChild1();
		while (pTree != nullptr){
			Value *num = __Expr2IR(pTree->GetChild0(), true);
			if (num == nullptr)
				return nullptr;
			if (num->getType()->isIntegerTy(8) && prsnt_sign == false)//将8位下标扩展到32位，防止255变成-1使用
				num = m_builder->CreateZExt(num, m_builder->getInt32Ty());

			if (value->getType()->getContainedType(0)->isPointerTy()) { //指针当做数组用的情况
				Value *v = m_builder->CreateLoad(value, true);
				value = m_builder->CreateInBoundsGEP(v, num);
			}
			else { //数组的情况
				pos.pop_back();
				pos.push_back(num);
				value = m_builder->CreateInBoundsGEP(value, pos);
			}
			pTree = pTree->GetChild1();
		}
		prsnt_sign = temp;
		break;
	}
	case POINTER_EXP:
	{
		value = m_builder->CreateLoad(value, true);
		__StructEle2IR(pTree->GetChild0(), value, sname);
		break;
	}
	case DOT_EXP:
	{
		__StructEle2IR(pTree->GetChild0(), value, sname);
		__StructEle2IR(pTree->GetChild1(), value, sname);
		break;
	}
	case ARROW_EXP:
	{
		value = __Load2IR(value);
		__StructEle2IR(pTree->GetChild0(), value, sname);
		__StructEle2IR(pTree->GetChild1(), value, sname);
		break;
	}
	default:
		cerr << "ERROR: cannot get struct " << sname << "'s element: " << eleName << endl;
	}
	return value;
}

/**
* 变量自加操作转成对应的IR代码
* @param 待处理的变量
* @return void
*/
void IR::__AddOne2IR(Value * alloc)
{
	LoadInst *load = m_builder->CreateLoad(alloc, true);
	Value *One = m_builder->getInt32(1);
	Value *result = m_builder->CreateNSWAdd(load, One, "inc");
	StoreInst *store = m_builder->CreateStore(result, alloc, false);
}

//将一个地址中的数据读取出来，根据地址的类型为数组或者指针，有不同的读取方法
Value *IR::__Load2IR(Value *alloc)
{
	Type *type = alloc->getType();
	Type *ctype = type->getContainedType(0);

	if (type->isPointerTy() && ctype->isArrayTy()){ //指针数组的读取方式不同
		vector<Value*> subScript;
		subScript.push_back(ConstantInt::get(m_module->getContext(), APInt(32, 0)));
		subScript.push_back(ConstantInt::get(m_module->getContext(), APInt(32, 0)));
		return m_builder->CreateInBoundsGEP(alloc, subScript);
	}
	else{
		return m_builder->CreateLoad(alloc, true);
	}
}

void IR::__While2IR(CSyntaxNode* pTree)
{
	static unsigned int whilenum = 0;//while的编号，建模中使用
	++whilenum;

	int tempWhileNum = whilenum;
	Function *TheFunction = m_builder->GetInsertBlock()->getParent();
	BasicBlock *WhileConBB = BasicBlock::Create(m_module->getContext(), "whileCond", TheFunction);
	m_builder->CreateBr(WhileConBB);
	m_builder->SetInsertPoint(WhileConBB);

	//创建条件跳转
	Value *v = __Cond2IR(pTree->GetChild0());
	BasicBlock *WhileBodyBB = BasicBlock::Create(m_module->getContext(), "whileBody", TheFunction);
	BasicBlock *WhileEndBB = BasicBlock::Create(m_module->getContext(), "whilEnd", TheFunction);
	m_builder->CreateCondBr(v, WhileBodyBB, WhileEndBB, 0);

	m_builder->SetInsertPoint(WhileBodyBB);
	if (Setting::IsModeling() && !hasOr && TheFunction->getName().substr(0, 2) != "$$"){
		m_builder->CreateCall(m_module->getFunction("$$GetVarsValue"));
		__GetLocalVar(pTree);
		string name = ("$$" + TheFunction->getName() + "_num").str();
		Function *f = m_module->getFunction("\01?whileBegin@@YAXII@Z");
		GlobalValue *g = m_module->getGlobalVariable(name);
		m_builder->CreateCall2(f, m_builder->CreateLoad(g), m_builder->getInt32(tempWhileNum));
	}
	if (pTree->GetChild1() != nullptr){
		Stmt2IR(pTree->GetChild1());
	}
	m_builder->CreateBr(WhileConBB);

	m_builder->SetInsertPoint(WhileEndBB);
	if (Setting::IsModeling() && !hasOr && TheFunction->getName().substr(0, 2) != "$$"){
		Function *f = m_module->getFunction("\01?whileEnd@@YAXII@Z");
		GlobalValue *g = m_module->getGlobalVariable(("$$" + TheFunction->getName() + "_num").str());
		m_builder->CreateCall2(f, m_builder->CreateLoad(g), m_builder->getInt32(tempWhileNum));
	}
}

void IR::__If2IR(CSyntaxNode *pTree)
{
	Function *TheFunction = m_builder->GetInsertBlock()->getParent();

	BasicBlock *ThenBB = BasicBlock::Create(m_module->getContext(), "then", TheFunction);
	//m_builder->SetInsertPoint(entry);

	//条件跳转
	Value * v = __Cond2IR(pTree->GetChild0());
	if (pTree->GetChild2() != nullptr)
	{
		BasicBlock *ElseBB = BasicBlock::Create(m_module->getContext(), "else", TheFunction);
		BasicBlock *IfEnd = BasicBlock::Create(m_module->getContext(), "ifend", TheFunction);
		m_builder->CreateCondBr(v, ThenBB, ElseBB, 0);
		m_builder->SetInsertPoint(ThenBB);

		if (pTree->GetChild1() != nullptr)
			Stmt2IR(pTree->GetChild1());

		llvm::BasicBlock::const_iterator it = m_builder->GetInsertBlock()->end();
		if (it == m_builder->GetInsertBlock()->begin() || !(--it)->isTerminator())
			m_builder->CreateBr(IfEnd);

		m_builder->SetInsertPoint(ElseBB);

		if (pTree->GetChild2() != nullptr)
			Stmt2IR(pTree->GetChild2());

		it = m_builder->GetInsertBlock()->end();
		if (it == m_builder->GetInsertBlock()->begin() || !(--it)->isTerminator())
			m_builder->CreateBr(IfEnd);

		m_builder->SetInsertPoint(IfEnd);
	}
	else
	{
		BasicBlock *IfEnd = BasicBlock::Create(m_module->getContext(), "ifend", TheFunction);
		m_builder->CreateCondBr(v, ThenBB, IfEnd, 0);
		m_builder->SetInsertPoint(ThenBB);

		if (pTree->GetChild1() != nullptr)
			Stmt2IR(pTree->GetChild1());

		llvm::BasicBlock::const_iterator it = m_builder->GetInsertBlock()->end();

		if (it == m_builder->GetInsertBlock()->begin() || !(--it)->isTerminator())
			m_builder->CreateBr(IfEnd);

		m_builder->SetInsertPoint(IfEnd);
	}
}

/**
* 条件语句转为IR代码
* @param 传入待分析的语法树
* @return 转换后的IR代码
*/
Value * IR::__Cond2IR(CSyntaxNode* pTree)
{
	if (pTree == nullptr){
		cerr << "ERROR: invalid condition experssion." << endl;
		return ConstantInt::getFalse(m_module->getContext());
	}

	Value *value = __Expr2IR(pTree, true);
	if (value == nullptr){
		cerr << "ERROR: invalid condition experssion." << endl;
		return ConstantInt::getFalse(m_module->getContext());
	}

	bool sign = prsnt_sign;
	Type *type = value->getType();
	if (type->isIntegerTy(1)){
		return value;
	}
	else if (type->isIntegerTy()){
		value = __Cast2IR(value, Type::getInt32Ty(m_module->getContext()), sign);
		if (value != nullptr)
			return m_builder->CreateICmpNE(value, ConstantInt::get(m_module->getContext(), APInt(32, 0)));
		return ConstantInt::getFalse(m_module->getContext());
	}
	else if (type->isFloatTy() || type->isDoubleTy()){
		return m_builder->CreateFCmpONE(value, ConstantFP::get(m_module->getContext(), APFloat(0.0)));
	}
	else if (type->isPointerTy() || type->isArrayTy()){
		return m_builder->CreateICmpNE(value, ConstantPointerNull::get((PointerType*)value->getType()));
	}
	else{
		cout << "ERROR: invalid condition expression. Unknown Type." << endl;
		return ConstantInt::getFalse(m_module->getContext());
	}
}

//三目运算符
Value *IR::__ThreeOp2IR(CSyntaxNode* pTree)
{
	Value *cond = __Cond2IR(pTree->GetChild0());

	Function *TheFunction = m_builder->GetInsertBlock()->getParent();
	BasicBlock *ThenBB = BasicBlock::Create(m_module->getContext(), "then", TheFunction);
	BasicBlock *ElseBB = BasicBlock::Create(m_module->getContext(), "else", TheFunction);
	BasicBlock *IfEnd = BasicBlock::Create(m_module->getContext(), "ifend", TheFunction);

	m_builder->CreateCondBr(cond, ThenBB, ElseBB, 0);

	// then BasicBlock
	m_builder->SetInsertPoint(ThenBB);
	Value *left = __Expr2IR(pTree->GetChild1(), true);
	bool Lsign = prsnt_sign;
	ThenBB = m_builder->GetInsertBlock();

	// else BasicBlock
	m_builder->SetInsertPoint(ElseBB);
	Value *right = __Expr2IR(pTree->GetChild2(), true);
	bool Rsign = prsnt_sign;
	Type *type = __CheckType(left->getType(), Lsign, right->getType(), prsnt_sign);
	bool sign = prsnt_sign;
	right = __Cast2IR(right, type, Rsign);
	ElseBB = m_builder->GetInsertBlock();
	m_builder->CreateBr(IfEnd);

	m_builder->SetInsertPoint(ThenBB);
	prsnt_sign = sign;
	left = __Cast2IR(left, type, Lsign);
	ThenBB = m_builder->GetInsertBlock();
	m_builder->CreateBr(IfEnd);

	// end
	m_builder->SetInsertPoint(IfEnd);
	PHINode *phi = m_builder->CreatePHI(left->getType(), 2);
	phi->addIncoming(left, ThenBB);
	phi->addIncoming(right, ElseBB);
	return phi;
}

//等于
Value* IR::__EQ2IR(CSyntaxNode* pTree)
{
	if (pTree->GetChild0() == nullptr || pTree->GetChild1() == nullptr){
		cerr << "ERROR: invalid equal condition expression." << endl;
		return nullptr;
	}

	Value *Left = __Expr2IR(pTree->GetChild0(), true);
	bool LSigned = prsnt_sign;
	if (Left == nullptr){
		cerr << "ERROR: invalid equal condition expression." << endl;
		return nullptr;
	}

	Value *Right = __Expr2IR(pTree->GetChild1(), true);
	if (Right == nullptr){
		cerr << "ERROR: invalid equal condition expression." << endl;
		return nullptr;
	}

	Type *type = __CheckType(Left, LSigned, Right, prsnt_sign);
	if (type == nullptr){
		cerr << "ERROR: invalid equal condition expression." << endl;
		return nullptr;
	}

	if (type->isIntegerTy()){
		prsnt_sign = false;
		return m_builder->CreateICmpEQ(Left, Right, "eq");
	}
	else if (type->isFloatTy() || type->isDoubleTy()){
		prsnt_sign = false;
		return m_builder->CreateFCmpOEQ(Left, Right, "feq");
	}
	else if (type->isPointerTy() || type->isArrayTy()){
		prsnt_sign = false;
		if (Right->getType()->isPointerTy() || Right->getType()->isArrayTy() || Right->getType()->isIntegerTy()){
			Value *v1 = m_builder->CreatePtrToInt(Left, m_builder->getInt32Ty());
			Value *v2 = m_builder->CreatePtrToInt(Right, m_builder->getInt32Ty());
			return m_builder->CreateICmpEQ(v1, v2, "eq");
		}
		else{
			cerr << "ERROR: cannot compare pointer value." << endl; // pTree->TreeToString() << endl;
			return nullptr;
		}
	}
	else{
		prsnt_sign = false;
		cerr << "ERROR: invalid equal condition expression." << endl;
		return nullptr;
	}
}

//不等于
Value * IR::__NE2IR(CSyntaxNode* pTree)
{
	if (pTree->GetChild0() == nullptr || pTree->GetChild1() == nullptr){
		cerr << "ERROR: invalid not equal condition expression." << endl;
		return nullptr;
	}

	Value *Left = __Expr2IR(pTree->GetChild0(), true);
	bool LSigned = prsnt_sign;
	if (Left == nullptr){
		cerr << "ERROR: invalid not equal condition expression." << endl;
		return nullptr;
	}

	Value *Right = __Expr2IR(pTree->GetChild1(), true);
	if (Right == nullptr){
		cerr << "ERROR: invalid not equal condition expression." << endl;
		return nullptr;
	}

	Type *type = __CheckType(Left, LSigned, Right, prsnt_sign);
	if (type == nullptr){
		cerr << "ERROR: invalid not equal condition expression." << endl;
		return nullptr;
	}

	if (type->isIntegerTy()){
		prsnt_sign = false;
		return m_builder->CreateICmpNE(Left, Right, "ne");
	}
	else if (type->isFloatTy() || type->isDoubleTy()){
		prsnt_sign = false;
		return m_builder->CreateFCmpONE(Left, Right, "fne");
	}
	else if (type->isPointerTy() || type->isArrayTy()){
		prsnt_sign = false;
		if (Right->getType()->isPointerTy() || Right->getType()->isArrayTy() || Right->getType()->isIntegerTy()){
			Value * v1 = m_builder->CreatePtrToInt(Left, m_builder->getInt32Ty());
			Value * v2 = m_builder->CreatePtrToInt(Right, m_builder->getInt32Ty());
			return m_builder->CreateICmpNE(v1, v2, "ne");
		}
		else{
			cerr << "ERROR: invalid not equal condition expression on pointer value." << endl;
			return nullptr;
		}
	}
	else{
		cerr << "ERROR: invalid not equal condition expression." << endl;
		return nullptr;
	}
}

//大于
Value* IR::__GT2IR(CSyntaxNode* pTree)
{
	if (pTree->GetChild0() == nullptr || pTree->GetChild1() == nullptr){
		cerr << "ERROR: invalid greater than condition expression." << endl;
		return nullptr;
	}

	Value *Left = __Expr2IR(pTree->GetChild0(), true);
	bool LSigned = prsnt_sign;
	if (Left == nullptr){
		cerr << "ERROR: invalid greater than condition expression." << endl;
		return nullptr;
	}

	Value *Right = __Expr2IR(pTree->GetChild1(), true);
	if (Right == nullptr){
		cerr << "ERROR: invalid greater than condition expression." << endl;
		return nullptr;
	}

	Type *type = __CheckType(Left, LSigned, Right, prsnt_sign);
	if (type == nullptr){
		cerr << "ERROR: invalid greater than condition expression." << endl;
		return nullptr;
	}

	if (type->isIntegerTy()){
		if (prsnt_sign){
			prsnt_sign = false;
			return m_builder->CreateICmpSGT(Left, Right, "gt");
		}
		else
			return m_builder->CreateICmpUGT(Left, Right, "gt");
	}
	else if (type->isFloatTy() || type->isDoubleTy()){
		prsnt_sign = false;
		return m_builder->CreateFCmpOGT(Left, Right, "fgt");
	}
	else if (type->isPointerTy() || type->isArrayTy()){
		prsnt_sign = false;
		if (Right->getType()->isPointerTy() || Right->getType()->isArrayTy()){
			Value * v1 = m_builder->CreatePtrToInt(Left, m_builder->getInt32Ty());
			Value * v2 = m_builder->CreatePtrToInt(Right, m_builder->getInt32Ty());
			return m_builder->CreateICmpUGT(Left, Right, "gt");
		}
		else{
			cerr << "ERROR: invalid greater than condition expression on pointer value." << endl;
			return nullptr;
		}
	}
	else{
		cerr << "ERROR: invalid greater than condition expression." << endl;
		return nullptr;
	}
}

//小于
Value * IR::__LT2IR(CSyntaxNode* pTree)
{
	if (pTree->GetChild0() == nullptr || pTree->GetChild1() == nullptr){
		cerr << "ERROR: invalid less than condition expression." << endl;
		return nullptr;
	}

	Value *Left = __Expr2IR(pTree->GetChild0(), true);
	bool LSigned = prsnt_sign;
	if (Left == nullptr){
		cerr << "ERROR: invalid less than condition expression." << endl;
		return nullptr;
	}

	Value *Right = __Expr2IR(pTree->GetChild1(), true);
	if (Right == nullptr){
		cerr << "ERROR: invalid less than condition expression." << endl;
		return nullptr;
	}

	Type *type = __CheckType(Left, LSigned, Right, prsnt_sign);
	if (type == nullptr){
		cerr << "ERROR: invalid less than condition expression." << endl;
		return nullptr;
	}

	if (type->isIntegerTy()){
		if (prsnt_sign){
			prsnt_sign = false;
			return m_builder->CreateICmpSLT(Left, Right, "lt");
		}
		else
			return m_builder->CreateICmpULT(Left, Right, "lt");
	}
	else if (type->isFloatTy() || type->isDoubleTy()){
		prsnt_sign = false;
		return m_builder->CreateFCmpOLT(Left, Right, "flt");
	}
	else if (type->isPointerTy() || type->isArrayTy()){
		prsnt_sign = false;
		if (Right->getType()->isPointerTy() || Right->getType()->isArrayTy()){
			Value * v1 = m_builder->CreatePtrToInt(Left, m_builder->getInt32Ty());
			Value * v2 = m_builder->CreatePtrToInt(Right, m_builder->getInt32Ty());
			return m_builder->CreateICmpULT(Left, Right, "lt");
		}
		else{
			cerr << "ERROR: invalid less than condition expression on pointer value." << endl;
			return nullptr;
		}
	}
	else{
		cerr << "ERROR: invalid less than condition expression." << endl;
		return nullptr;
	}
}

//大于等于
Value* IR::__GE2IR(CSyntaxNode* pTree)
{
	if (pTree->GetChild0() == nullptr || pTree->GetChild1() == nullptr){
		cerr << "ERROR: invalid greater than or equal condition expression." << endl;
		return nullptr;
	}

	Value *Left = __Expr2IR(pTree->GetChild0(), true);
	bool LSigned = prsnt_sign;
	if (Left == nullptr){
		cerr << "ERROR: invalid greater than or equal than condition expression." << endl;
		return nullptr;
	}

	Value *Right = __Expr2IR(pTree->GetChild1(), true);
	if (Right == nullptr){
		cerr << "ERROR: invalid greater than or equal condition expression." << endl;
		return nullptr;
	}

	Type *type = __CheckType(Left, LSigned, Right, prsnt_sign);
	if (type == nullptr){
		cerr << "ERROR: invalid greater than or equal condition expression." << endl;
		return nullptr;
	}

	if (type->isIntegerTy()){
		if (prsnt_sign){
			prsnt_sign = false;
			return m_builder->CreateICmpSGE(Left, Right, "ge");
		}
		else
			return m_builder->CreateICmpUGE(Left, Right, "ge");
	}
	else if (type->isFloatTy() || type->isDoubleTy()){
		prsnt_sign = false;
		return m_builder->CreateFCmpOGE(Left, Right, "fge");
	}
	else if (type->isPointerTy() || type->isArrayTy()){
		prsnt_sign = false;
		if (Right->getType()->isPointerTy() || Right->getType()->isArrayTy()){
			Value * v1 = m_builder->CreatePtrToInt(Left, m_builder->getInt32Ty());
			Value * v2 = m_builder->CreatePtrToInt(Right, m_builder->getInt32Ty());
			return m_builder->CreateICmpUGE(Left, Right, "ge");
		}
		else{
			cerr << "ERROR: invalid greater than or equal condition expression on pointer value." << endl;
			return nullptr;
		}
	}
	else{
		cerr << "ERROR: invalid greater than or equal condition expression." << endl;
		return nullptr;
	}
}

//小于等于
Value * IR::__LE2IR(CSyntaxNode* pTree)
{
	if (pTree->GetChild0() == nullptr || pTree->GetChild1() == nullptr){
		cerr << "ERROR: invalid less than or equal condition expression." << endl;
		return nullptr;
	}

	Value *Left = __Expr2IR(pTree->GetChild0(), true);
	bool LSigned = prsnt_sign;
	if (Left == nullptr){
		cerr << "ERROR: invalid less than or equal condition expression." << endl;
		return nullptr;
	}

	Value *Right = __Expr2IR(pTree->GetChild1(), true);
	if (Right == nullptr){
		cerr << "ERROR: invalid less than or equal condition expression." << endl;
		return nullptr;
	}

	Type *type = __CheckType(Left, LSigned, Right, prsnt_sign);
	if (type == nullptr){
		cerr << "ERROR: invalid less than or equal condition expression." << endl;
		return nullptr;
	}

	if (type->isIntegerTy()){
		if (prsnt_sign){
			prsnt_sign = false;
			return m_builder->CreateICmpSLE(Left, Right, "le");
		}
		else
			return m_builder->CreateICmpULE(Left, Right, "le");
	}
	else if (type->isFloatTy() || type->isDoubleTy()){
		prsnt_sign = false;
		return m_builder->CreateFCmpOLE(Left, Right, "fle");
	}
	else if (type->isPointerTy() || type->isArrayTy()){
		prsnt_sign = false;
		if (Right->getType()->isPointerTy() || Right->getType()->isArrayTy()){
			Value * v1 = m_builder->CreatePtrToInt(Left, m_builder->getInt32Ty());
			Value * v2 = m_builder->CreatePtrToInt(Right, m_builder->getInt32Ty());
			return m_builder->CreateICmpULE(Left, Right, "le");
		}
		else{
			cerr << "ERROR: invalid less than or equal condition expression on pointer value" << endl;
			return nullptr;
		}
	}
	else{
		cerr << "ERROR: invalid less than or equal condition expression." << endl;
		return nullptr;
	}
}

Value* IR::__Neg2IR(CSyntaxNode* pTree)
{
	if (pTree->GetChild0() == nullptr){
		cerr << "ERROR: invalid negative expression." << endl;
		return nullptr;
	}

	Value *Left = __Expr2IR(pTree->GetChild0(), true);
	bool sign = prsnt_sign;
	if (Left == nullptr){
		cerr << "ERROR: invalid negative expression." << endl;
		return nullptr;
	}

	Type *type = Left->getType();

	if (!type->isIntegerTy() || !type->isIntegerTy(32)){
		Left = __Cast2IR(Left, Type::getInt32Ty(m_module->getContext()), false);
		if (Left == nullptr){
			cerr << "ERROR: invalid negative expression." << endl;
			return nullptr;
		}
	}

	Left = m_builder->CreateICmpNE(Left, ConstantInt::get(m_module->getContext(), APInt(32, 0)));
	prsnt_sign = false;
	return m_builder->CreateXor(Left, ConstantInt::get(m_module->getContext(), APInt(1, 1)));
}

Value * IR::__And2IR(CSyntaxNode* pTree)
{
	if (pTree->GetChild0() == nullptr || pTree->GetChild1() == nullptr){
		cerr << "ERROR: invalid AND expression." << endl;
		return nullptr;
	}

	Value *Left = __Cond2IR(pTree->GetChild0());
	bool LSigned = prsnt_sign;
	if (Left == nullptr){
		cerr << "ERROR: invalid AND expression." << endl;
		return nullptr;
	}

	Function *TheFunction = m_builder->GetInsertBlock()->getParent();

	BasicBlock *land = m_builder->GetInsertBlock();
	BasicBlock *rand = BasicBlock::Create(m_module->getContext(), "rand", TheFunction);
	BasicBlock *andend = BasicBlock::Create(m_module->getContext(), "andend", TheFunction);
	rightEnd = rand;
	m_builder->CreateCondBr(Left, rand, andend, 0);
	m_builder->SetInsertPoint(rand);
	Value* Right = __Cond2IR(pTree->GetChild1());

	if (Right == nullptr){
		cerr << "ERROR: invalid AND expression." << endl;
		return nullptr;
	}

	m_builder->CreateBr(andend);
	m_builder->SetInsertPoint(andend);
	PHINode * phi = m_builder->CreatePHI(m_builder->getInt1Ty(), 2);
	phi->addIncoming(m_builder->getInt1(0), land);
	phi->addIncoming(Right, rightEnd);
	rightEnd = andend;
	return phi;
}

Value* IR::__BAnd2IR(CSyntaxNode *pTree)
{
	if (pTree->GetChild0() == nullptr || pTree->GetChild1() == nullptr){
		cerr << "ERROR: invalid binary AND expression." << endl;
		return nullptr;
	}

	Value *Left = __Expr2IR(pTree->GetChild0(), true);
	bool LSigned = prsnt_sign;
	if (Left == nullptr){
		cerr << "ERROR: invalid binary AND expression." << endl;
		return nullptr;
	}

	Value* Right = __Expr2IR(pTree->GetChild1(), true);

	if (Right == nullptr){
		cerr << "ERROR: invalid binary AND expression." << endl;
		return nullptr;
	}

	Type *type = __CheckType(Left, LSigned, Right, prsnt_sign);
	if (type == nullptr){
		cerr << "ERROR: invalid binary AND expression." << endl;
		return nullptr;
	}

	if (type->isIntegerTy())
	{
		return  m_builder->CreateAnd(Left, Right, "and");
	}
	else{
		cerr << "ERROR: invalid binary AND expression." << endl;
		return nullptr;
	}
}

Value* IR::__Or2IR(CSyntaxNode *pTree)
{
	if (pTree->GetChild0() == nullptr || pTree->GetChild1() == nullptr){
		cerr << "ERROR: invalid OR expression." << endl;
		return nullptr;
	}
	Value *Left = __Cond2IR(pTree->GetChild0());
	bool LSigned = prsnt_sign;
	if (Left == nullptr){
		cerr << "ERROR: invalid OR expression." << endl;
		return nullptr;
	}

	Function *TheFunction = m_builder->GetInsertBlock()->getParent();

	BasicBlock *lor = m_builder->GetInsertBlock();
	BasicBlock *ror = BasicBlock::Create(m_module->getContext(), "ror", TheFunction);
	BasicBlock *orend = BasicBlock::Create(m_module->getContext(), "orend", TheFunction);
	rightEnd = ror;
	m_builder->CreateCondBr(Left, orend, ror, 0);
	m_builder->SetInsertPoint(ror);

	Value* Right = __Cond2IR(pTree->GetChild1());
	if (Right == nullptr){
		cerr << "ERROR: invalid OR expression." << endl;
		return nullptr;
	}

	m_builder->CreateBr(orend);
	m_builder->SetInsertPoint(orend);
	PHINode * phi = m_builder->CreatePHI(m_builder->getInt1Ty(), 2);
	phi->addIncoming(m_builder->getInt1(1), lor);
	phi->addIncoming(Right, rightEnd);
	rightEnd = orend;
	return phi;
}

Value * IR::__BOr2IR(CSyntaxNode* pTree)
{
	if (pTree->GetChild0() == nullptr || pTree->GetChild1() == nullptr){
		cerr << "ERROR: invalid binary OR expression." << endl;
		return nullptr;
	}

	Value *Left = __Expr2IR(pTree->GetChild0(), true);
	bool LSigned = prsnt_sign;
	if (Left == nullptr){
		cerr << "ERROR: invalid binary OR expression." << endl;
		return nullptr;
	}

	Value *Right = __Expr2IR(pTree->GetChild1(), true);
	if (Right == nullptr){
		cerr << "ERROR: invalid binary OR expression." << endl;
		return nullptr;
	}

	Type *type = __CheckType(Left, LSigned, Right, prsnt_sign);
	if (type == nullptr){
		cerr << "ERROR: invalid binary OR expression." << endl;
		return nullptr;
	}

	if (type->isIntegerTy())
	{
		return m_builder->CreateOr(Left, Right, "or");
	}
	else if (type->isPointerTy()){
		Value *v = __Cast2IR(Left,Right->getType(),false);
		return m_builder->CreateOr(v, Right, "or");
	}
	else{
		cerr << "ERROR: invalid binary OR expression." << endl;
		return nullptr;
	}
}

Value* IR::__BXOr2IR(CSyntaxNode *pTree){
	Value *left = __Expr2IR(pTree->GetChild0(), true);
	bool lsign = prsnt_sign;
	if (left == nullptr){
		cerr << "ERROR: invalid XOR expression owing to left value." << endl;
		return ConstantInt::get(m_module->getContext(),APInt(32,0));
	}

	Value *right = __Expr2IR(pTree->GetChild1(), true);
	bool rsign = prsnt_sign;

	if (right == nullptr){
		cerr << "ERROR: invalid XOR expression owing to right value." << endl;
		return ConstantInt::get(m_module->getContext(), APInt(32, 0));
	}

	left = __Cast2IR(left, m_builder->getInt32Ty(), false);
	right = __Cast2IR(right, m_builder->getInt32Ty(), false);

	return m_builder->CreateXor(left, right);
}

Value* IR::__BNeg2IR(CSyntaxNode *pTree){
	Value *left = __Expr2IR(pTree->GetChild0(), true);
	if (left == nullptr){
		cerr << "ERROR: invalid binary negative expression." << endl;
		return ConstantInt::get(m_module->getContext(),APInt(32,0));
	}

	Value *right = m_builder->getInt(APInt(left->getType()->getPrimitiveSizeInBits(), -1));
	return m_builder->CreateXor(left, right);
}

Value* IR::__Shl2IR(CSyntaxNode *pTree)
{
	if (pTree->GetChild0() == nullptr || pTree->GetChild1() == nullptr){
		cerr << "ERROR: invalid shift to left expression." << endl;
		return nullptr;
	}

	Value *Left = __Expr2IR(pTree->GetChild0(), true);
	Type* type = Left->getType();
	if (Left == nullptr){
		cerr << "ERROR: invalid shift to left expression." << endl;
		return nullptr;
	}

	Value *Right = __Expr2IR(pTree->GetChild1(), true);
	if (Right == nullptr){
		cerr << "ERROR: invalid shift to left expression." << endl;
		return nullptr;
	}

	if (type->isIntegerTy())
	{
		if (!Right->getType()->isIntegerTy(32)){
			Right = __Cast2IR(Right, m_builder->getInt32Ty(), false);
		}
		Left = __Cast2IR(Left, m_builder->getInt32Ty(), prsnt_sign);
		return m_builder->CreateShl(Left, Right, "shl");
	}
	else{
		cerr << "ERROR: invalid shift to left expression." << endl;
		return nullptr;
	}
}

Value* IR::__Shr2IR(CSyntaxNode* pTree)
{
	if (pTree->GetChild0() == nullptr || pTree->GetChild1() == nullptr){
		cerr << "ERROR: invalid shift to right expression." << endl;
		return nullptr;
	}

	Value *Left = __Expr2IR(pTree->GetChild0(), true);
	bool LSigned = prsnt_sign;
	if (Left == nullptr){
		cerr << "ERROR: invalid shift to right expression." << endl;
		return nullptr;
	}

	Value *Right = __Expr2IR(pTree->GetChild1(), true);
	if (Right == nullptr){
		cerr << "ERROR: invalid shift to right expression." << endl;
		return nullptr;
	}

	if (Left->getType()->isIntegerTy() && Left->getType()->getPrimitiveSizeInBits() < 32)
		LSigned = true;

	Type *type = __CheckType(Left, LSigned, Right, prsnt_sign);
	prsnt_sign = LSigned;
	if (type == nullptr){
		cerr << "ERROR: invalid shift to right expression." << endl;
		return nullptr;
	}
		

	if (type->isIntegerTy()){
		if (LSigned)
			return m_builder->CreateAShr(Left, Right, "ashr");
		else
			return m_builder->CreateLShr(Left, Right, "lshr");
	}
	else{
		cerr << "ERROR: invalid shift to right expression." << endl;
		return nullptr;
	}
}

/*
将变量值转为对应类型的地址
*/
Value* IR::__AddVar2IR(CSyntaxNode *pTree)
{
	Type *type = PointerType::get(GetType(pTree), 0);
	Value *value = __Expr2IR(pTree->GetChild0(), true);
	value = __Cast2IR(value, type, false);
	return value;
}

Value* IR::__AddVarList2IR(CSyntaxNode *pTree)
{
	int basicSize = 4;
	if (pTree->GetRType() == CHARTYPE || pTree->GetRType() == UCHARTYPE)
		basicSize = 1;
	Value *value = m_builder->getInt32(pTree->GetiValue() + pTree->GetChild0()->GetiValue()*basicSize);

	Type *type = PointerType::get(GetType(pTree), 0);
	value = __Cast2IR(value, type, false);

	return value;
}

/**
* 加法操作转成对应的IR代码
* @param 待处理的语法树
* @return 转之后的结果
*/
Value* IR::__Add2IR(CSyntaxNode* pTree)
{
	if (pTree->GetChild0() == nullptr || pTree->GetChild1() == nullptr){
		cerr << "ERROR: invalie addition expression." << endl;
		return nullptr;
	}

	Value *Left = __Expr2IR(pTree->GetChild0(), true);
	bool LSigned = prsnt_sign;

	if (Left == nullptr){
		cerr << "ERROR: invalie addition expression." << endl;
		return nullptr;
	}

	Value *Right = __Expr2IR(pTree->GetChild1(), true);

	if (Right == nullptr){
		cerr << "ERROR: invalie addition expression." << endl;
		return nullptr;
	}

	Type *type = __CheckType(Left, LSigned, Right, prsnt_sign);

	if (type == nullptr){
		cerr << "ERROR: invalid addition expression." << endl;
		return nullptr;
	}

	if (type->isIntegerTy()){
		if (prsnt_sign)
			return m_builder->CreateNSWAdd(Left, Right);
		else
			return m_builder->CreateAdd(Left, Right);
	}
	else if (type->isFloatTy() || type->isDoubleTy()){
		return m_builder->CreateFAdd(Left, Right);
	}
	else if (type->isPointerTy()){
		if (Right->getType()->isIntegerTy()){
			return m_builder->CreateInBoundsGEP(Left, Right);
		}
		else{
			cerr << "ERROR: invalie addition expression on pointer value." << endl;
			return nullptr;
		}
	}
	else if (type->isArrayTy()){
		cerr << "ERROR: invalie addition expression on array." << endl;
		return nullptr;
	}
	else{
		cerr << "ERROR: invalie addition expression." << endl;
		return nullptr;
	}
}

/**
* 减法操作转成对应的IR代码
* @param 待处理的语法树
* @return 转之后的结果
*/
Value* IR::__Sub2IR(CSyntaxNode* pTree)
{
	if (pTree->GetChild0() == nullptr || pTree->GetChild1() == nullptr){
		cerr << "ERROR: invalie subtraction expression." << endl;
		return nullptr;
	}

	Value *Left = __Expr2IR(pTree->GetChild0(), true);
	bool LSigned = prsnt_sign;
	if (Left == nullptr){
		cerr << "ERROR: invalie subtraction expression." << endl;
		return nullptr;
	}

	Value* Right = __Expr2IR(pTree->GetChild1(), true);
	bool RSigned = prsnt_sign;
	if (Right == nullptr){
		cerr << "ERROR: invalie subtraction expression." << endl;
		return nullptr;
	}

	Type *type = __CheckType(Left, LSigned, Right, prsnt_sign);
	if (type == nullptr){
		cerr << "ERROR: invalie subtraction expression." << endl;
		return nullptr;
	}

	if (type->isIntegerTy()){
		if (prsnt_sign)
			return m_builder->CreateNSWSub(Left, Right, "sub");
		else
			return m_builder->CreateSub(Left, Right, "sub");
	}
	else if (type->isFloatTy() || type->isDoubleTy()){
		return m_builder->CreateFSub(Left, Right, "fsub");
	}
	else if (type->isPointerTy() || type->isArrayTy()){
		if (Right->getType()->isIntegerTy()){
			return m_builder->CreateInBoundsGEP(Left, m_builder->CreateNeg(Right));
		}
		else if (Right->getType()->isPointerTy()){
			if (Left->getType()->getTypeID() == Right->getType()->getTypeID()){
				prsnt_sign = true;
				Value* _Left = __Cast2IR(Left, Type::getInt32Ty(m_module->getContext()), LSigned);
				Value* _Right = __Cast2IR(Right, Type::getInt32Ty(m_module->getContext()), RSigned);
				
				//获取两个指针的地址差值
				Value* difference = m_builder->CreateSub(_Left, _Right);
				
				//获取指针指向的类型
				Type* elementType = Left->getType()->getArrayElementType();
				int eleSize = CalculateSize(elementType);
				return m_builder->CreateSDiv(difference, m_builder->getInt32(eleSize));
			}
			else{
				cerr << "ERROR: invalie subtraction expression on pointer value." << endl;
				return nullptr;
			}
		}
		else{
			cerr << "ERROR: invalie subtraction expression on pointer value." << endl;
			return nullptr;
		}
	}
	else{
		cerr << "ERROR: invalie subtraction expression." << endl;
		return nullptr;
	}
	return nullptr;
}

/**
* 乘法操作转成对应的IR代码
* @param 待处理的语法树
* @return 转之后的结果
*/
Value *IR::__Mul2IR(CSyntaxNode* pTree)
{
	if (pTree->GetChild0() == nullptr || pTree->GetChild1() == nullptr){
		cerr << "ERROR: invalie multiplication expression." << endl;
		return nullptr;
	}

	Value *Left = __Expr2IR(pTree->GetChild0(), true);
	bool LSigned = prsnt_sign;
	if (Left == nullptr){
		cerr << "ERROR: invalie multiplication expression." << endl;
		return nullptr;
	}

	Value* Right = __Expr2IR(pTree->GetChild1(), true);

	if (Right == nullptr){
		cerr << "ERROR: invalie multiplication expression." << endl;
		return nullptr;
	}

	Type *type = __CheckType(Left, LSigned, Right, prsnt_sign);
	if (type == nullptr){
		cerr << "ERROR: invalie multiplication expression." << endl;
		return nullptr;
	}
		
	if (type->isIntegerTy()){
		if (prsnt_sign)
			return m_builder->CreateNSWMul(Left, Right, "mul");
		else
			return m_builder->CreateMul(Left, Right, "mul");
	}
	else if (type->isFloatTy() || type->isDoubleTy()){
		return m_builder->CreateFMul(Left, Right, "fmul");
	}
	else if (type->isPointerTy() || type->isArrayTy()){
		cerr << "ERROR: invalie multiplication expression on pointer value." << endl;
		return nullptr;
	}
	else{
		cerr << "ERROR: invalie multiplication expression." << endl;
		return nullptr;
	}
}


/**
* 除法操作转成对应的IR代码
* @param 待处理的语法树
* @return 转之后的结果
*/
Value* IR::__Div2IR(CSyntaxNode* pTree)
{
	if (pTree->GetChild0() == nullptr || pTree->GetChild1() == nullptr){
		cerr << "ERROR: invalie division expression." << endl;
		return nullptr;
	}

	Value *Left = __Expr2IR(pTree->GetChild0(), true);
	bool LSigned = prsnt_sign;
	if (Left == nullptr){
		cerr << "ERROR: invalie division expression." << endl;
		return nullptr;
	}

	Value* Right = __Expr2IR(pTree->GetChild1(), true);
	if (Right == nullptr){
		cerr << "ERROR: invalie division expression." << endl;
		return nullptr;
	}

	Type *type = __CheckType(Left, LSigned, Right, prsnt_sign);
	if (type == nullptr){
		cerr << "ERROR: invalie division expression." << endl;
		return nullptr;
	}

	if (type->isIntegerTy())
	{
		if (prsnt_sign)
			return m_builder->CreateSDiv(Left, Right, "sdiv");
		else
			return m_builder->CreateUDiv(Left, Right, "udiv");
	}
	else if (type->isFloatTy() || type->isDoubleTy()){
		return m_builder->CreateFDiv(Left, Right, "fdiv");
	}
	else if (type->isPointerTy() || type->isArrayTy()){
		cerr << "ERROR: invalie division expression on pointer value." << endl;
		return nullptr;
	}
	else{
		cerr << "ERROR: invalie division expression." << endl;
		return nullptr;
	}
}

/**
* 取余操作转成对应的IR代码
* @param 待处理的语法树
* @return 转之后的结果
*/
Value * IR::__Mod2IR(CSyntaxNode* pTree)
{
	if (pTree->GetChild0() == nullptr || pTree->GetChild1() == nullptr){
		cerr << "ERROR: invalie modulo expression." << endl;
		return nullptr;
	}

	Value *Left = __Expr2IR(pTree->GetChild0(), true);
	bool LSigned = prsnt_sign;
	if (Left == nullptr){
		cerr << "ERROR: invalie modulo expression." << endl;
		return nullptr;
	}

	Value* Right = __Expr2IR(pTree->GetChild1(), true);
	if (Right == nullptr){
		cerr << "ERROR: invalie modulo expression." << endl;
		return nullptr;
	}

	Type *type = __CheckType(Left, LSigned, Right, prsnt_sign);
	if (type == nullptr){
		cerr << "ERROR: invalie modulo expression." << endl;
		return nullptr;
	}

	if (type->isIntegerTy()){
		if (prsnt_sign)
			return m_builder->CreateSRem(Left, Right, "smod");
		else
			return m_builder->CreateURem(Left, Right, "umod");
	}
	else{
		cerr << "ERROR: invalie modulo expression." << endl;
		return nullptr;
	}
}

/**
* 强制转换语句转为IR代码
* @param 传入待分析的语法树
* @return 转换后的IR代码
*/
Value* IR::__Cast2IR(Value *value, Type *type, bool sign)
{
	Type *vType = value->getType(); auto vid = vType->getTypeID(); auto id = type->getTypeID();
	if (vType == type)
		return value;

	// float类型转换
	if (vType->isFloatTy()){
		if (type->isFloatTy())
			return value;
		// float -> double
		if (type->isDoubleTy())
			return m_builder->CreateFPExt(value, type);
		// float -> int
		if (type->isIntegerTy()){
			if (sign)
				return m_builder->CreateFPToSI(value, type);
			else
				return m_builder->CreateFPToUI(value, type);
		}
		else{
			cerr << "ERROR: Invalid cast on float value." << endl;
			return nullptr;
		}
	}
	// double类型转换
	if (vType->isDoubleTy()){
		if (type->isDoubleTy())
			return value;
		// double -> float
		if (type->isFloatTy())
			return m_builder->CreateFPTrunc(value, type);
		// double -> int
		if (type->isIntegerTy())
			return m_builder->CreateFPToSI(value, type);
		else{
			cerr << "ERROR: Invalid cast on float value." << endl;
			return nullptr;
		}
	}
	// int类型转换
	else if (vType->isIntegerTy()){
		// int -> int
		if (type->isIntegerTy()){
			auto vTySize = m_dataLayout->getTypeSizeInBits(vType);
			auto tSize = m_dataLayout->getTypeSizeInBits(type);
			if (vTySize > tSize){
				return m_builder->CreateTrunc(value, type);
			}
			else if (vTySize == tSize){
				return m_builder->CreateBitCast(value, type);
			}
			else{
				if (sign)
					return m_builder->CreateSExt(value, type);
				else
					return m_builder->CreateZExt(value, type);
			}
		}
		// int -> float
		if (type->isFloatTy()){
			if (sign)
				return m_builder->CreateSIToFP(value, type);
			else
				return m_builder->CreateUIToFP(value, type);
		}
		// int -> double
		if (type->isDoubleTy()){
			if (sign)
				return m_builder->CreateSIToFP(value, type);
			else
				return m_builder->CreateUIToFP(value, type);
		}
		// int -> pointer
		if (type->isPointerTy()){
			return m_builder->CreateIntToPtr(value, type);
		}
	}
	// 指针类型转换
	else if (vType->isPointerTy()){
		// pointer -> int
		if (type->isIntegerTy())
			return m_builder->CreatePtrToInt(value, type);
		// pointer -> pointer || array
		else if (type->isPointerTy() || type->isArrayTy()){
			return m_builder->CreateBitCast(value, type);
		}
		else{
			cerr << "ERROR: Invalid cast on pointer value." << endl;
			return nullptr;
		}
	}
	// 数组类型转换
	if (vType->isArrayTy()){
		// array -> pointer || array
		if (type->isPointerTy() || type->isArrayTy())
			return m_builder->CreateBitCast(value, type);
		// array -> int
		else if (type->isIntegerTy()){
			Value *a2p = m_builder->CreateBitCast(value, PointerType::get(vType->getContainedType(0), 0));
			return m_builder->CreatePtrToInt(a2p, type);
		}
		else{
			cerr << "ERROR: Invalid cast on array value." << endl;
			return nullptr;
		}
	}
	cerr << "ERROR: Unsupported type cast." << endl;
	return nullptr;
}

//自己实现sizeof
Value* IR::__IRSizeof(CSyntaxNode *pTree)
{
	if (pTree == nullptr){
		return m_builder->getInt32(0);
	}
	else if (pTree->GetChild0() == nullptr){
		// sizeof(basictype), 例如： sizeof(int),sizeof(XXX) // XXX is a struct
		switch (pTree->GetRType()){
		case BOOLTYPE:
		case CHARTYPE:
		case UCHARTYPE:{
			return m_builder->getInt32(1);
		}
		case SHORTTYPE:
		case USHORTTYPE:{
			return m_builder->getInt32(2);
		}
		case INTTYPE:
		case UINTTYPE:
		case LONGTYPE:
		case ULONGTYPE:
		case FLOATTYPE:{
			return m_builder->getInt32(4);
		}
		case DOUBLETYPE:
		case LLONGTYPE:
		case ULLONGTYPE:{
			return m_builder->getInt32(8);
		}
		case STRUCTTYPE:{
			int size = 0;
			string name = pTree->GetNName();
			auto it = m_IRStruct.find(name);
			if (it != m_IRStruct.end())
				size = it->second->GetStructSize();
			else
				cerr << "ERROR: struct " << name << " undefined." << endl;
			return m_builder->getInt32(size);
		}
		case VOIDTYPE:{
			return m_builder->getInt32(0);
		}
		}
	}
	else if (pTree->GetChild0() != nullptr && pTree->GetChild0()->GetChild0() == nullptr){
		auto tp = pTree->GetChild0();
		switch (tp->GetNType()){
		case TRUE_EXP:
		case FALSE_EXP:
		case CHARLITERAL_EXP:{ // sizeof('c')
			return m_builder->getInt32(1);
		}
		case INTEGER_EXP:
		case FLOATLITERAL_EXP:
		case ADDRESS_EXP:
		case POINTER_EXP:{     // sizeof(int*)
			return m_builder->getInt32(4);
		}
		case STR_EXP:{         // sizeof("1234") 
			int size = tp->GetsValue().size() + 1;
			return m_builder->getInt32(size);
		}
		case IDENT_EXP:{
			string name = tp->GetNName();
			Value *v = __STableSearch(name, true);
			Type *type = v->getType()->getContainedType(0);
			if (type->isFunctionTy()){
				return m_builder->getInt32(4);
			}
			int size = m_dataLayout->getTypeAllocSize(type);
			return m_builder->getInt32(size);
		}
		default:
			return m_builder->getInt32(0);
		}
	}
	else if (pTree->GetChild0() != nullptr && pTree->GetChild0()->GetChild0()->GetNType()==POINTER_EXP){
			return m_builder->getInt32(4);
	}
	else{
		Value *v = __Expr2IR(pTree->GetChild0(), false);//temp
		Type *type = v->getType()->getContainedType(0);
		return m_builder->getInt32(CalculateSize(type));
	}
	return m_builder->getInt32(0);
}


/**
* 函数调用语句转为IR代码
* @param 传入待分析的语法树
* @return 转换后的IR代码
*/
Value* IR::__Call2IR(CSyntaxNode *pTree)
{
	if (pTree == nullptr){
		cerr << "ERROR: invalid function call statement." << endl;
		return nullptr;
	}

	if (pTree->GetNName() == "sizeof"){
		return __IRSizeof(pTree);
	}
		
	bool isWinApi = false;
	string func_name = __GetFunctionName(pTree, isWinApi); //获取调用函数名

	bool loadsign = false;
	Function *Callee = __GetFunction(pTree, func_name, loadsign); //获取调用函数, Callee's type is pointer to function.
	
	//获取参数列表
	Function::arg_iterator args;
	if (!loadsign)
		args = Callee->arg_begin();

	vector<Value*> argsList;

	for (CSyntaxNode *paraNode = pTree->GetChild1(); paraNode != nullptr; paraNode = paraNode->GetChild1()){
		CSyntaxNode *currNode = paraNode->GetChild0();

		string paraName = currNode->GetNameNode() == nullptr ? "" : currNode->GetNameNode()->GetNName();

		if (paraName == "RValue"){ //去掉RValue
			continue;
		}

		if (!loadsign && args != Callee->arg_end() && args->hasByValAttr()){
			argsList.push_back(__Expr2IR(currNode, false));
		}
		else{
			argsList.push_back(__Expr2IR(currNode, true));
		}

		if (!loadsign && args != Callee->arg_end()){
			args++;
		}
	}

	//判断形参和实参个数相符合，对于非可变参数函数
	if (Callee->arg_size() != argsList.size() && !Callee->isVarArg()){

		if (Callee->arg_size() == argsList.size() + 1){
			argsList.insert(argsList.begin(), (Value*)m_builder->getInt32(1)); // extern call
		}

		if (Callee->arg_size() != argsList.size()){
			cout << "ERROR: " << func_name << " : Formal argument size is " << Callee->arg_size() << " != Actual argument size is " << argsList.size() << endl;
		}
	}

	auto actual = argsList.begin();

	if (loadsign == false){
		auto formal = Callee->arg_begin();
		while (formal != Callee->arg_end()){
			Value *aArg = *actual;
			Type *aArg_t = aArg->getType();
			Type *fArg_t = formal->getType();

			if (formal->hasByValAttr()){
				BasicBlock *bb = m_builder->GetInsertBlock();
				BasicBlock::iterator it = m_builder->GetInsertPoint();
				
				Instruction *inst = m_builder->GetInsertBlock()->getParent()->getEntryBlock().begin()++;
				m_builder->SetInsertPoint( inst );
				Value *to = m_builder->CreateAlloca(aArg_t->getContainedType(0));

				m_builder->SetInsertPoint(bb);

				to = m_builder->CreateBitCast(to, m_builder->getInt8PtrTy());
				int size = CalculateSize(aArg_t->getContainedType(0));
				Value *from = m_builder->CreateBitCast(aArg, m_builder->getInt8PtrTy());
				m_builder->CreateMemCpy(to, from, size, 1);
				*actual = m_builder->CreateBitCast(to, aArg_t);
			}

			if (fArg_t != aArg_t){
				*actual = __Cast2IR(aArg, fArg_t, true);
			}

			++formal;
			++actual;
		}
	}
	else if (loadsign){
		FunctionType *type = (FunctionType*)(Callee->getType()->getContainedType(0));
		
		for (int j = 1; j < type->getNumContainedTypes(); j++)
		{
			Value *aArg = *actual;
			if (aArg->getType()->isStructTy())
			{
				BasicBlock *bb = m_builder->GetInsertBlock();
				BasicBlock &fbb = m_builder->GetInsertBlock()->getParent()->getEntryBlock();//.begin()
				m_builder->SetInsertPoint(&fbb);

				Value *temp = m_builder->CreateAlloca(aArg->getType()->getContainedType(0));
				m_builder->SetInsertPoint(bb);
				temp = m_builder->CreateBitCast(temp, m_builder->getInt8PtrTy());

				int size = CalculateSize(aArg->getType()->getContainedType(0));

				m_builder->CreateMemCpy(temp, m_builder->CreateBitCast(aArg, m_builder->getInt8PtrTy()), size, 1);
				*actual = m_builder->CreateBitCast(temp, aArg->getType());
			}

			if (aArg->getType() != type->getContainedType(j)){
				*actual = __Cast2IR(aArg, type->getContainedType(j), true);
			}

			actual++;
		}
	}

	//构造函数调用
	ArrayRef<Value*> Args(argsList);
	CallInst *call = m_builder->CreateCall(Callee, Args);

	if (isWinApi || g_ThreadFuncName.find(func_name) != g_ThreadFuncName.end())
		call->setCallingConv(CallingConv::X86_StdCall);
	else
		call->setCallingConv(CallingConv::C);

	return call;
}

string IR::__GetFunctionName(CSyntaxNode *pTree, bool &isWinAPI){
	string func_name = "";

	if (pTree == nullptr){
		return func_name;
	}

	auto tmp = pTree->GetChild0();
	if (tmp->GetNType() == DOT_EXP || tmp->GetNType() == ARROW_EXP){
		func_name = tmp->GetChild1()->GetNameNode()->GetNName();
	}
	else{
		func_name = pTree->GetNameNode()->GetNName();
	}
		
	auto it = m_WinApiMap.find(func_name);
	if (it != m_WinApiMap.end()){
		func_name = it->second;
		isWinAPI = true;
	}
	else if (func_name.find("$$") == 0){  //如果以$$开头的函数,则调用本地写死的函数
		func_name = __GetRealFunctionName(func_name);
	}
	else if (func_name == "memmove"){
		func_name = "\01?memmove@@YAPAXPAXPBXH@Z";
	}
	else if (func_name == "time"){
		func_name = "\01?time@@YA_JPA_J@Z";
	}
	func_name = __GetRealFunctionName(func_name);
	return func_name;
}

Function* IR::__GetFunction(CSyntaxNode *pTree, string name, bool &load){

	if (m_module->getFunction(name) != nullptr){
		Function *func = m_module->getFunction(name);
		load = false;
		return func;
	}
	else if (m_FuncNames.find(name) != m_FuncNames.end()){
		func2IR(m_Funcs[name]);
		Function *func = m_module->getFunction(name);
		load = false;
		return func;
	}
	else if ((name == "" && pTree->GetChild0()->GetNType() == ARRAY_EXP)){
		Value *v = __Expr2IR(pTree->GetChild0(), false);
		LoadInst *func = m_builder->CreateLoad(v);
		load = true;
		return reinterpret_cast<Function*>(func);
	}
	else if (pTree->GetChild0()->GetNType() == DOT_EXP || pTree->GetChild0()->GetNType() == ARROW_EXP){
		Value *v = __Expr2IR(pTree->GetChild0(), false);
		LoadInst *func = m_builder->CreateLoad(v);
		load = true;
		return reinterpret_cast<Function*>(func);
	}
	else if (__STableSearch(name, false) != nullptr){
		Value *v = __Expr2IR(pTree->GetChild0(), false);
		LoadInst *func = m_builder->CreateLoad(v);
		load = true;
		return reinterpret_cast<Function*>(func);
	}
	else{
		cerr << "ERROR: Unknown function reference: " << pTree->GetChild0()->GetNName() << endl;
		return nullptr;
	}
}

/**
*根据结构体定义的模块和declare树得到对应的llvm type
*@param module (声明结构体的模块) pTree(声明语句)
*@return 与声明对应的llvm type
*/
Type* IR::GetType(CSyntaxNode *pTree)
{
	Type *result = nullptr;
	switch (pTree->GetRType()){
	case VOIDTYPE:
		prsnt_sign = false;
		//result = Type::getVoidTy(m_module->getContext());
		result = Type::getInt8Ty(m_module->getContext());
		break;
	case BOOLTYPE:
		prsnt_sign = false;
		result = Type::getInt1Ty(m_module->getContext());
		break;
	case CHARTYPE:
		prsnt_sign = true;
		result = Type::getInt8Ty(m_module->getContext());
		break;
	case UCHARTYPE:
		prsnt_sign = false;
		result = Type::getInt8Ty(m_module->getContext());
		break;
	case SHORTTYPE:
		prsnt_sign = true;
		result = Type::getInt16Ty(m_module->getContext());
		break;
	case USHORTTYPE:
		prsnt_sign = false;
		result = Type::getInt16Ty(m_module->getContext());
		break;
	case INTTYPE:
	case LONGTYPE:
		prsnt_sign = true;
		result = Type::getInt32Ty(m_module->getContext());
		break;
	case UINTTYPE:
	case ULONGTYPE:
		prsnt_sign = false;
		result = Type::getInt32Ty(m_module->getContext());
		break;
	case LLONGTYPE:
		prsnt_sign = true;
		result = Type::getInt64Ty(m_module->getContext());
		break;
	case ULLONGTYPE:
		prsnt_sign = false;
		result = Type::getInt64Ty(m_module->getContext());
		break;
	case FLOATTYPE:
		prsnt_sign = true;
		result = Type::getFloatTy(m_module->getContext());
		break;
	case DOUBLETYPE:
		prsnt_sign = true;
		result = Type::getDoubleTy(m_module->getContext());
		break;
	case STRUCTTYPE:{
		string name = pTree->GetNName();
		if (name == "" && pTree->GetChild0() != nullptr)
			name = pTree->GetChild0()->GetNName();

		auto it = m_IRStruct.find(name);
		if (it != m_IRStruct.end())
			result = it->second->GetStructType();
		else{
			auto uit = udef.find(name);
			if (uit != udef.end())
				result = uit->second;
			else if (pTree->GetNType() == FORMAL_PARAMETER_EXP){
				StructType *s_type = StructType::create(m_module->getContext(), name);
				udef.insert({ name, s_type });
				result = s_type;
			}
		}
		break;
	}
	case FUNCPTYPE:
		result = __getFunctionType(pTree);
		break;
	case FILETYPE:
		result = m_module->getTypeByName("struct._iobuf");
		break;
	case CRITICAL_SECTION_TYPE:
		result = m_module->getTypeByName("struct._RTL_CRITICAL_SECTION");
		break;
	case TIMECAPS_TYPE:
		result = m_module->getTypeByName("struct.timecaps_tag");
		break;
	default:
		cerr << "ERROR: Type is not considered in function GetType()." << endl;
	}
	return result;
}

Type* IR::__getFunctionType(CSyntaxNode *pTree)
{
	Type *returntype;
	auto decTree = pTree->GetChild0();
	auto rtnTree = decTree->GetChild1();
	if (rtnTree == nullptr || rtnTree->GetChild0() == nullptr) //如果为空，说明没有返回值，设置为void
		returntype = Type::getVoidTy(m_module->getContext());	
	else
		returntype = __GetFuncParaInfo(rtnTree->GetChild0()).type;

	if (returntype == nullptr)
		returntype = Type::getVoidTy(m_module->getContext());

	//参数获取
	vector<Type*> paraTypes;
	CSyntaxNode *paraTree = nullptr;
	if (rtnTree != nullptr)
		paraTree = rtnTree->GetChild1();
	while (paraTree != nullptr){
		VarInfo vi = __GetFuncParaInfo(paraTree);
		paraTypes.push_back(vi.type);
		paraTree = paraTree->GetChild1();
	}
	//函数类型获取
	FunctionType *fTy = FunctionType::get(returntype, paraTypes, false);
	return fTy;
}

/**
* 函数定义语句转为IR代码
* @param 传入待分析的语法树
* @return 转换后的IR代码
*/
void IR::func2IR(CSyntaxNode *pTree)
{
	if (pTree == nullptr){
		return; //若没有函数定义，则直接退出
	}
	string func_name = pTree->GetNName(); //获取函数名字

	IR::prsnt_sign = true; //返回值的符号，暂定为true.
	Type *rettype;
	bool rtn_sign;

	CSyntaxNode *rtnNode = pTree->GetChild0()->GetChild0();
	if (rtnNode != nullptr){ //判断返回值类型是否定义
		rettype = __GetFuncParaInfo(rtnNode).type;
		rtn_sign = IR::prsnt_sign;
	}
	else{
		rettype = Type::getVoidTy(m_module->getContext()); //返回值类型初始化为void
		rtn_sign = true;
	}

	std::vector<Type*> vectype;  //参数-类型
	std::vector<string> vecname; //参数-名
	std::vector<bool> vecsign;   //参数-符号
	std::vector<int> byval;      //属性为byval的参数，主要用于将结构体参数变为结构体指针参数

	CSyntaxNode *paraNode = pTree->GetChild0()->GetChild1();
	while (paraNode != nullptr){
		VarInfo vInfo = __GetFuncParaInfo(paraNode);
		if (vInfo.type->isStructTy()){
			vectype.push_back(PointerType::get(vInfo.type, 0));
			byval.push_back(vectype.size()); // index
		}
		else
			vectype.push_back(vInfo.type);

		vecname.push_back(vInfo.name);
		vecsign.push_back(prsnt_sign);

		paraNode = paraNode->GetChild1();
	}

	FunctionType *fTy = FunctionType::get(rettype, vectype, false); //函数类型
	Function *Func = Function::Create(fTy, Function::ExternalLinkage, func_name, m_module); //函数定义

	//为函数增加byval的属性
	for (auto it = byval.begin(); it != byval.end(); it++)
		Func->addAttribute(*it, Attribute::ByVal);

	if (g_ThreadFuncName.find(func_name) != g_ThreadFuncName.end()){
		Func->setCallingConv(CallingConv::X86_StdCall);
	}
	else{
		Func->setCallingConv(CallingConv::C); //调用方式,一般为ccc: C Calling Convention
	}

	IRSymbol *symbol = new IRSymbol(Func, true);
	if (m_GlobalSTable.insert({ func_name, symbol }).second == false){
		cerr << "ERROR: variable " << func_name << " has already been defined." << endl;
	}
	//存储全局状态
	BasicBlock *fatherinsertpoint = m_builder->GetInsertBlock();

	map<string, IRSymbol*> fatherTable = m_IRSTable;
	m_IRSTable.clear(); //修改符号表上下文

	BasicBlock *tempREnd = rightEnd;

	//添加函数体
	BasicBlock *entry = BasicBlock::Create(m_module->getContext(), "entry", Func, nullptr);
	m_builder->SetInsertPoint(entry);

	if (pTree->GetChild1() != nullptr){
		if (!rettype->isVoidTy()){
			AllocaInst *allocDeclr = m_builder->CreateAlloca(rettype);
			allocDeclr->setName("RValue.addr");
			allocDeclr->setAlignment(CalculateAlign(rettype));
			IRSymbol *symbol = new IRSymbol(allocDeclr, rtn_sign);
			if (m_IRSTable.insert({ "RValue", symbol }).second == false){
				cerr << "ERROR: variable RValue has already been defined in function " << func_name << endl;
			}
		}
		auto arg_it = Func->arg_begin();
		auto argname_it = vecname.begin();
		auto type_it = vectype.begin();
		auto sign_it = vecsign.begin();

		while (arg_it != Func->arg_end()){
			string tempname = *(argname_it++);
			Type *temptype = *(type_it++);
			Value *temparg = arg_it;
			bool sign = *(sign_it++);

			temparg->setName(tempname); //参数列表加上变量名
			IRSymbol *symbol;
			if (!arg_it->hasByValAttr()){ //为参数列表分配内存
				AllocaInst *allocDeclr = m_builder->CreateAlloca(temptype);
				allocDeclr->setName(tempname + ".addr");
				allocDeclr->setAlignment(CalculateAlign(temptype));

				StoreInst *store = m_builder->CreateStore(temparg, allocDeclr, false); //存储参数
				symbol = new IRSymbol(allocDeclr, sign); //添加至符号表sign
			}
			else{
				symbol = new IRSymbol(((AllocaInst*)temparg), sign); // byval的结构体指针不需要重新分配空间
			}
			arg_it++;
			if (m_IRSTable.insert({ tempname, symbol }).second == false){
				cerr << "ERROR: variable " << tempname << " has already been defined." << endl;
			}
		}
		//如果是在建模中，根据函数名定义全局变量 unsigned int $$函数名_num ，并在函数开始时加1，函数结束时减1 
		Value *temp = nullptr;
		if (Setting::IsModeling() && func_name[0] != '$' && func_name[1] != '$'){
			string tempName = "$$" + func_name + "_num";

			ConstantInt *c = ConstantInt::get(m_module->getContext(), APInt(32, 0));
			temp = new GlobalVariable(*m_module, m_builder->getInt32Ty(), false, GlobalValue::ExternalLinkage, c, tempName);

			if (m_GlobalSTable.insert({ tempName, new IRSymbol(temp, false) }).second == false){
				cerr << "ERROR: variable " << tempName << " has already been defined" << endl;
			}

			Value *v = m_builder->CreateAdd(m_builder->CreateLoad(temp), m_builder->getInt32(1), "add");
			m_builder->CreateStore(v, temp);
		}

		Stmt2IR(pTree->GetChild1());

		if (Setting::IsModeling() && func_name[0] != '$' && func_name[1] != '$'){
			m_builder->CreateStore(m_builder->CreateSub(m_builder->CreateLoad(temp), m_builder->getInt32(1), "sub"), temp);
		}
	}

	if (fTy->getReturnType()->isVoidTy()){
		m_builder->CreateRetVoid();
	}
	else{
		Value *retValue = __STableSearch("RValue");
		LoadInst *LoadValue = m_builder->CreateLoad(retValue);
		m_builder->CreateRet(LoadValue);
	}
	rightEnd = tempREnd;
	m_IRSTable = fatherTable;

	if (fatherinsertpoint != nullptr)
		m_builder->SetInsertPoint(fatherinsertpoint);

	m_FuncNames.erase(func_name);
}

void IR::TraverseFuncTree(CSyntaxNode *pTree)
{
	while (pTree != nullptr){
		string name = pTree->GetNName();
		auto rtn = m_Funcs.insert({ name, pTree });
		if (rtn.second == false)
			cerr << "ERROR: function " << name << " has been defined." << endl;
		else
			m_FuncNames.insert(name);
		pTree = pTree->GetChild2();
	}
	if (Setting::isFuncFile())
		m_FuncNames.erase("$$changeState");

	for (auto it = m_Funcs.begin(); it != m_Funcs.end(); it++){
		if (m_FuncNames.find(it->first) != m_FuncNames.end())
			func2IR(it->second);
	}
}

IRStruct* IR::GetIRStruct(string name)
{
	auto it = m_IRStruct.find(name);
	if (it == m_IRStruct.end())
		return nullptr;
	return it->second;
}

void IR::__MyCreateThread(CSyntaxNode* pTree)
{
	Function *CalleeF = m_module->getFunction("\01?MyCreateThread@@YAXP6GIPAX@Z0H@Z");
	Function::arg_iterator args = CalleeF->arg_begin();
	if (pTree->GetNName() == "ManagerThread"){
		Value *arg1 = ConstantPointerNull::get((PointerType*)args->getType()); ++args;
		Value *arg2 = ConstantPointerNull::get((PointerType*)args->getType());
		Value *arg3 = m_builder->getInt32(pTree->GetiValue());
		m_builder->CreateCall3(CalleeF, arg1, arg2, arg3);
	}
	else{
		Value *arg1;
		if (pTree->GetNName() == "")
			arg1 = ConstantPointerNull::get((PointerType*)args->getType());
		else
			arg1 = __STableSearch(pTree->GetNName());
		Value *arg2 = __Expr2IR(pTree->GetChild0()->GetChild0(), true);
		if (!arg2->getType()->getContainedType(0)->isIntegerTy(8))
			arg2 = m_builder->CreateBitCast(arg2, m_builder->getInt8PtrTy());
		Value *arg3 = m_builder->getInt32(pTree->GetiValue());
		m_builder->CreateCall3(CalleeF, arg1, arg2, arg3);
	}
}

void IR::__StructTree2IR(CSyntaxNode *sTree)
{
	vector<CSyntaxNode*> tree;
	vector<string> name;
	//逆置语法树
	while (sTree != nullptr){
		name.push_back(sTree->GetNName());
		tree.push_back(sTree->GetChild0());
		sTree = sTree->GetChild1();
	}
	//生成定义
	auto nit = name.rbegin();
	auto tit = tree.rbegin();
	while (nit != name.rend() && tit != tree.rend()){
		if (IR::GetIRStruct(*nit) != nullptr){
			cerr << "ERROR: struct " << *nit << " has already been defined." << endl;
		}
		else{
			IRStruct *s = new IRStruct(m_module, this, *nit, *tit);
			if (!s->isDefined()){
				cerr << "ERROR: failed to define struct " << *nit << endl;
			}
			else{
				s->CalculateSize(); //计算长度
			}
			
			if (m_IRStruct.insert({ s->GetStructName(), s }).second == false){
				cerr << "ERROR: struct " << *nit << " has already been defined." << endl;
			}
		}
		++nit;
		++tit;
	}
}

//处理多文件,处理extern_tree
void IR::__FuncFile(CSyntaxTree *m_GlbVarTree, CSyntaxNode *function_tree)
{
	vector<string> name;
	Module::FunctionListType &funcList = m_module->getFunctionList();
	for (auto it = funcList.begin(); it != funcList.end(); ++it){
		if (!it->isDeclaration())
			name.push_back(it->getName().str());
	}
	//删除函数
	for (int i = 0; i < name.size(); ++i){
		Function *f = m_module->getFunction(name.at(i));
		funcList.erase(f);
	}

	Function::Create(FunctionType::get(m_builder->getVoidTy(), ArrayRef<Type*>(m_builder->getInt32Ty()), false), Function::ExternalLinkage, "$$changeState", m_module);
	if (Setting::isOutput()){
		Function::Create(FunctionType::get(m_builder->getVoidTy(), vector<Type*>(0), false), Function::ExternalLinkage, "$$GlobalOutput", m_module);
		Function::Create(FunctionType::get(m_builder->getInt1Ty(), vector<Type*>(0), false), Function::ExternalLinkage, "$$IsOutput", m_module);
	}
	if (Setting::IsModeling()){
		Function::Create(FunctionType::get(m_builder->getVoidTy(), vector<Type*>(1, m_builder->getInt32Ty()), false), Function::ExternalLinkage, __GetRealFunctionName("$$addInt"), m_module);
		Function::Create(FunctionType::get(m_builder->getVoidTy(), vector<Type*>(1, m_builder->getInt8Ty()), false), Function::ExternalLinkage, __GetRealFunctionName("$$addChar"), m_module);
		Function::Create(FunctionType::get(m_builder->getVoidTy(), vector<Type*>(1, m_builder->getFloatTy()), false), Function::ExternalLinkage, __GetRealFunctionName("$$addFloat"), m_module);
		Function::Create(FunctionType::get(m_builder->getVoidTy(), vector<Type*>(1, m_builder->getInt8PtrTy()), false), Function::ExternalLinkage, __GetRealFunctionName("$$addString"), m_module);
		Function::Create(FunctionType::get(m_builder->getVoidTy(), vector<Type*>(0), false), Function::ExternalLinkage, "$$GetVarsValue", m_module);
	}

	Module::GlobalListType &globalList = m_module->getGlobalList();

	GlobalVariable *v = m_module->getGlobalVariable("\01?$$myJmpbuf@@3PAHA");
	Type *type = v->getType();
	globalList.erase(v);
	new GlobalVariable(*m_module, type->getContainedType(0), false, GlobalValue::ExternalLinkage, nullptr, "\01?$$myJmpbuf@@3PAHA");

	v = m_module->getGlobalVariable("\01?$$varsInfo@@3PADA");
	type = v->getType();
	globalList.erase(v);
	new GlobalVariable(*m_module, type->getContainedType(0), false, GlobalValue::ExternalLinkage, nullptr, "\01?$$varsInfo@@3PADA");

	v = m_module->getGlobalVariable("\01?$$tempLen@@3HA");
	type = v->getType();
	globalList.erase(v);
	new GlobalVariable(*m_module, type->getContainedType(0), false, GlobalValue::ExternalLinkage, nullptr, "\01?$$tempLen@@3HA");

	//extern int $$state_num
	m_StNum = new GlobalVariable(*m_module, IntegerType::get(m_module->getContext(), 32), false, GlobalValue::ExternalLinkage, nullptr, "$$state_num");

	IRSymbol *symbol = new IRSymbol(m_StNum, true);
	if (m_GlobalSTable.insert({ "$$state_num", symbol }).second == false){
		cerr << "ERROR: variable $$state_num has already been defined" << endl;
	}

	//IRTemp = new GlobalVariable(*m_module, m_builder->getInt8PtrTy(), false, GlobalValue::ExternalLinkage, nullptr, "$$IRTemp");

	for (auto it = globalList.begin(); it != globalList.end(); ++it){
		if (it->getName().str() == "\01?start_time@@3KA" || it->getName().str() == "\01?j_buf@@3PAHA")
			it->setInitializer(nullptr);
	}
	__ExTree2IR(extern_tree);
	__StructTree2IR(struct_tree);
	Global2IR(m_GlbVarTree->GetRoot());
	m_InFunc = true;
	TraverseFuncTree(function_tree);
	m_InFunc = false;
}

//处理extern_tree
void IR::__ExTree2IR(CSyntaxNode *eTree)
{
	while (eTree != nullptr){
		switch (eTree->GetNType()){
		case EXTERN_VAR_DEC:
			break;
		case EXTERN_FUNC_DEC:{
			CSyntaxNode *paraNode = eTree->GetChild0();
			vector<Type*> vectype(1, m_builder->getInt32Ty());
			Type *retType = m_builder->getVoidTy();
			while (paraNode != nullptr){
				VarInfo vInfo = __GetFuncParaInfo(paraNode);

				if (vInfo.name == "RValue")
					retType = vInfo.type;
				else
					vectype.push_back(vInfo.type);

				paraNode = paraNode->GetChild1();
			}
			Function::Create(FunctionType::get(retType, vectype, false), Function::ExternalLinkage, eTree->GetNName(), m_module);
			break;
		}
		default:
			cerr << "ERROR: Unknown extern declare type." << endl;
		}
		eTree = eTree->GetChild1();
	}
}

//异步通信中使用，将声明语法树中数组元素个数加一,因为第一个元素有特殊作用
bool IR::__AsyncSubscript(CSyntaxNode *pTree)
{
	//只更改Chn类型结构体的下标
	if (pTree->GetRType() != STRUCTTYPE || pTree->GetNName() != "Chn")
		return false;

	CSyntaxNode* visit = pTree->GetChild0();
	while (visit != nullptr){
		if (visit->GetChild0()->GetNType() == ARRAY_DECLARE_STA){
			CSyntaxNode* temp = visit->GetChild0()->GetChild1()->GetChild0();
			temp->SetiValue(temp->GetiValue() + 1);
		}
		visit = visit->GetChild1();
	}
	return true;
}

//异步通信中使用，给有特殊作用的首元素赋值,结构体第一个元素存长度,第二个元素为0
void IR::__AsyncAss(Value *value)
{
	int number = value->getType()->getContainedType(0)->getArrayNumElements();

	vector<Value*> subScript;
	subScript.push_back(m_builder->getInt32(0));
	subScript.push_back(m_builder->getInt32(0));
	subScript.push_back(m_builder->getInt32(0));

	m_builder->CreateStore(m_builder->getInt32(number), m_builder->CreateInBoundsGEP(value, subScript));

	subScript.pop_back();
	subScript.push_back(m_builder->getInt32(1));
	m_builder->CreateStore(m_builder->getInt32(0), m_builder->CreateInBoundsGEP(value, subScript));
}


int IR::CalculateAlign(Type *type){
	if (type == nullptr){
		return -1;
	}

	if (type->isIntegerTy(1)){
		return 1;
	}

	if (type->isIntegerTy() || type->isFloatTy() || type->isDoubleTy() || type->isPointerTy()){
		return CalculateSize(type);
	}

	if (type->isStructTy()){
		string sname = type->getStructName();
		auto it = m_IRStruct.find(sname);
		if (it != m_IRStruct.end()){
			return it->second->GetBasicSize();
		}
		else{
			cerr << "ERROR: undefined struct type: " << sname << endl;
			return -1;
		}
	}

	if (type->isArrayTy()){
		return CalculateAlign(type->getContainedType(0));
	}

	cerr << "ERROR: unknown type, cannot calculate value's align." << endl;
	return -1;
}

int IR::CalculateSize(Type *type){
	if (type == nullptr)
		return 0;
	return m_dataLayout->getTypeAllocSize(type);
}
