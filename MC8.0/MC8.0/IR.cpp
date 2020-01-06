#include "IR.h"
#include "SyntaxNode.h"

#include <llvm/IR/ValueSymbolTable.h>

#include <iostream>
#include <string>
#include <cmath>
#include <map>

using namespace llvm;
using namespace std;

extern bool hasOr;                     // at lex.cpp �������Ƿ���or
extern CSyntaxNode *struct_tree;       // at parse.cpp
extern CSyntaxNode *extern_tree;       // at parse.cpp
extern set<string> g_ThreadFuncName;   // at MSVCompiler.cpp ��װ���̺߳�����

bool IR::prsnt_sign;
map<string, StructType*> IR::udef;
map<string, IRStruct*>   IR::m_IRStruct; //�ṹ������IRStruct��ӳ��

Type *IR::m_Int1Ty = nullptr;
Type *IR::m_Int8Ty = nullptr;
Type *IR::m_Int16Ty = nullptr;
Type *IR::m_Int32Ty = nullptr;
Type *IR::m_Int64Ty = nullptr;
Type *IR::m_Int128Ty = nullptr;
Type *IR::m_FloatTy = nullptr;
Type *IR::m_DoubleTy = nullptr;

IR::IR(){
	// ��������֮���Է����������ȫ����ΪIR��ֻ�ܲ���һ��ʵ����������ʷԭ���µġ�
	m_context = new LLVMContext();
	m_smd = new SMDiagnostic();
	static unique_ptr<Module> Owner = llvm::parseIRFile(Setting::getExternalSPath().data(), *m_smd, *m_context);
	m_module = Owner.get();
	m_module->setModuleIdentifier("IRResult");
	m_dataLayout = new DataLayout(m_module);
	m_builder = new llvm::IRBuilder<>(m_module->getContext());

	// ɾ��û�õĺ���
	Module::FunctionListType &funcList = m_module->getFunctionList();
	Function *f = m_module->getFunction("ex");
	funcList.erase(f);
	f = m_module->getFunction("ex1");
	funcList.erase(f);

	m_Int1Ty = m_builder->getInt1Ty();
	m_Int8Ty = m_builder->getInt8Ty();
	m_Int16Ty = m_builder->getInt16Ty();
	m_Int32Ty = m_builder->getInt32Ty();
	m_Int64Ty = m_builder->getInt64Ty();
	m_Int128Ty = m_builder->getInt128Ty();
	m_FloatTy = m_builder->getFloatTy();
	m_DoubleTy = m_builder->getDoubleTy();
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
		__FuncFile(m_GlbVarTree, function_tree); //����.func�ļ�
	}
	else{
		__ExTree2IR(extern_tree);
		FunctionType *mainFTy = FunctionType::get(m_Int32Ty, false);
		Function *mainFunc = Function::Create(mainFTy, Function::ExternalLinkage, "main", m_module);
		BasicBlock *entrymain = BasicBlock::Create(m_module->getContext(), "entry", mainFunc, 0);
		m_builder->SetInsertPoint(entrymain);

		//�޸����ɣ���״̬��������Ϊȫ�ֱ�������ʵ�ֺ����ڷ���״̬��������
		m_StNum = new GlobalVariable(*m_module, m_Int32Ty, false, GlobalValue::ExternalLinkage, m_builder->getInt32(0), "$$state_num");
		IRSymbol *symbol = new IRSymbol(m_StNum, true);
		if (m_GlobalSTable.insert({ "$$state_num", symbol }).second == false){
			cerr << "ERROR: variable $$state_num has already been defined." << endl;
		}

		__InitWinApiMap();
		//__InitWinStruct(); //��ʼ��һЩwindows.h�еĽṹ��
		__StructTree2IR(struct_tree);

		OrGlobal2IR(); //����or��ʱ��Ԥ�Ƚ�һЩ����������ű�
		Global2IR(m_GlbVarTree->GetRoot()); //ȫ�ֱ�������

		m_InFunc = true;//��m_InFunc��Ϊtrue����ʾ������

		if (Setting::isOutput()){
			__CreateIsOutput();
			__CreateGlbOutFunc(); //�����������ȫ�ֱ����ĺ���
			Function *f = m_module->getFunction("GetMsvVar");
			vector<Value*> args;
			args.push_back(m_StNum);
			args.push_back(m_module->getFunction("$$GlobalOutput"));
			args.push_back(m_module->getFunction("$$IsOutput"));
			m_builder->CreateCall(f,args);
		}
		else{
			Function *CalleeF = m_module->getFunction("GetMsvVar");
			Function::arg_iterator args = CalleeF->arg_begin();
			Value *arg1 = m_module->getGlobalVariable("$$state_num");  ++args;
			Value *arg2 = ConstantPointerNull::get((PointerType*)args->getType()); ++args;
			Value *arg3 = ConstantPointerNull::get((PointerType*)args->getType());
			vector<Value*> margs;
			margs.push_back(arg1);
			margs.push_back(arg2);
			margs.push_back(arg3);
			m_builder->CreateCall(CalleeF, margs);
		}

		if (Setting::IsModeling()){
			vector<Value*> sub(2, m_builder->getInt32(0));
			Function *f = m_module->getFunction("GetExternalVars");
			vector<Value*> args;
			args.push_back(__STableSearch("$$current"));
			args.push_back(__STableSearch("$$length"));
			args.push_back(m_builder->CreateInBoundsGEP(__STableSearch("$$flag"), sub));
			args.push_back(m_module->getFunction("$$MyLongJmp"));
			
			Value *value = m_builder->CreateGlobalString((Setting::getOutPath() + Setting::getOutDotName()));
			value = m_builder->CreateInBoundsGEP(value, sub);
			args.push_back(value);
			m_builder->CreateCall(f, args);

			__GetVarsValue();

			f = m_module->getFunction("GetModelVar");
			args.clear();
			args.push_back(m_module->getGlobalVariable("$$varsInfo"));
			args.push_back(m_module->getGlobalVariable("$$tempLen"));
			args.push_back(m_module->getFunction("$$GetVarsValue"));
			m_builder->CreateCall(f, args);


			f = m_module->getFunction("setModelOption");
			args.clear();
			args.push_back(m_builder->getInt1(hasOr));
			m_builder->CreateCall(f,args);
		}

		TraverseFuncTree(function_tree);
		function_tree = nullptr;
		m_InFunc = false;

		if (Setting::IsModeling()) {//��ģ��ʱ��ÿ���������������Լ��ľ�̬���� $$������_num���������е�������������������������func2IR������
			new GlobalVariable(*m_module, m_Int32Ty, false, GlobalValue::ExternalLinkage, m_builder->getInt32(0), "$$main_num");
		}

		Stmt2IR(IRTree->GetRoot());

		m_builder->CreateRet(m_builder->getInt32(0));
		delete IRTree->GetRoot();
		IRTree->SetRoot(nullptr);
	}

	std::error_code ec;
	string path = Setting::getOutBCFilePath();
	raw_fd_ostream S(path, ec, llvm::sys::fs::F_RW);
	if (ec) {
		cerr << "ERROR: " << ec.message() << endl;
	}

	S << *m_module; 
	S.flush();
	S.close();

	if (!Setting::isFuncFile() && Setting::isGenExe()){
		system((CLANGCMD + Setting::getOutBCFilePath() + " " +  Setting::getHome() + "MSVLLIB.lib  -o " + Setting::getOutFilePath()).data());
#ifndef	RELEASE_MC
		system(Setting::getOutFilePath().data());
#endif
	}
}

void IR::__InitWinStruct()
{
	map<string, int> name_pos;
	vector<bool> sign;
	IRStruct *s;

	//RTL_CRITICAL_SECTION
	name_pos["DebugInfo"] = 0;		name_pos["LockCount"] = 1;		name_pos["RecursionCount"] = 2;
	name_pos["OwningThread"] = 3;	name_pos["LockSemaphore"] = 4;	name_pos["SpinCount"] = 5;
	sign.push_back(0);				sign.push_back(1);				sign.push_back(1);
	sign.push_back(0);				sign.push_back(0);				sign.push_back(0);
	s = new IRStruct(m_module->getTypeByName("struct._RTL_CRITICAL_SECTION"), name_pos, sign, 40, 8);
	m_IRStruct["struct._RTL_CRITICAL_SECTION"] = s;

	name_pos.clear(); sign.clear();
	//RTL_CRITICAL_SECTION_DEBUG
	name_pos["Type"] = 0;				name_pos["CreatorBackTraceIndex"] = 1;		name_pos["CriticalSection"] = 2;
	name_pos["ProcessLocksList"] = 3;	name_pos["EntryCount"] = 4;					name_pos["ContentionCount"] = 5;
	name_pos["Flags"] = 6;				name_pos["CreatorBackTraceIndexHigh"] = 7;	name_pos["SpareWORD"] = 8;
	sign.push_back(0);					sign.push_back(0);							sign.push_back(0);
	sign.push_back(0);					sign.push_back(0);							sign.push_back(0);
	sign.push_back(0);					sign.push_back(0);							sign.push_back(0);
	s = new IRStruct(m_module->getTypeByName("struct._RTL_CRITICAL_SECTION_DEBUG"), name_pos, sign, 48, 8);
	m_IRStruct["struct._RTL_CRITICAL_SECTION_DEBUG"] = s;

	name_pos.clear(); sign.clear();
	//LIST_ENTRY
	name_pos["Flink"] = 0;	name_pos["Blink"] = 1;
	sign.push_back(0);		sign.push_back(0);
	s = new IRStruct(m_module->getTypeByName("struct._LIST_ENTRY"), name_pos, sign, 16, 8);
	m_IRStruct["struct._LIST_ENTRY"] = s;

	name_pos.clear(); sign.clear();
	//TIMECAPS
	name_pos["wPeriodMin"] = 0;		name_pos["wPeriodMax"] = 1;
	sign.push_back(0);				sign.push_back(0);
	s = new IRStruct(m_module->getTypeByName("struct.timecaps_tag"), name_pos, sign, 8, 4);
	m_IRStruct["struct.timecaps_tag"] = s;

	name_pos.clear(); sign.clear();
	//SECURITY_ATTRIBUTES
	name_pos["nLength"] = 0;	name_pos["lpSecurityDescriptor"] = 1;	name_pos["bInheritHandle "] = 1;
	sign.push_back(0);			sign.push_back(0);						sign.push_back(0);
	s = new IRStruct(m_module->getTypeByName("struct._SECURITY_ATTRIBUTES"), name_pos, sign, 24, 8);
	m_IRStruct["struct._SECURITY_ATTRIBUTES"] = s;
}

void IR::__InitWinApiMap()
{
	m_WinApiMap.insert({ "Sleep", "Sleep" });
	m_WinApiMap.insert({ "EnterCriticalSection", "EnterCriticalSection" });
	m_WinApiMap.insert({ "LeaveCriticalSection", "LeaveCriticalSection" });
	m_WinApiMap.insert({ "InitializeCriticalSection", "InitializeCriticalSection" });
	m_WinApiMap.insert({ "WaitForSingleObject", "WaitForSingleObject" });
	m_WinApiMap.insert({ "ReleaseSemaphore", "ReleaseSemaphore" });
	m_WinApiMap.insert({ "CreateSemaphore", "CreateSemaphore" });
	m_WinApiMap.insert({ "SetThreadAffinityMask", "SetThreadAffinityMask" });
	m_WinApiMap.insert({ "CreateThread", "CreateThread" });
	m_WinApiMap.insert({ "SuspendThread", "SuspendThread" });
	m_WinApiMap.insert({ "ResumeThread", "ResumeThread" });
	m_WinApiMap.insert({ "GetCurrentThreadId", "GetCurrentThreadId" });
	m_WinApiMap.insert({ "SetThreadPriority", "SetThreadPriority" });
	m_WinApiMap.insert({ "SetThreadPriorityBoost", "SetThreadPriorityBoost" });
	m_WinApiMap.insert({ "CloseHandle", "CloseHandle" });
	m_WinApiMap.insert({ "CreateEvent", "CreateEvent" });
	m_WinApiMap.insert({ "ResetEvent", "ResetEvent" });
	m_WinApiMap.insert({ "timeSetEvent", "timeSetEvent" });
	m_WinApiMap.insert({ "timeKillEvent", "timeKillEvent" });
	m_WinApiMap.insert({ "timeEndPeriod", "timeEndPeriod" });
	m_WinApiMap.insert({ "timeGetDevCaps", "timeGetDevCaps" });
	m_WinApiMap.insert({ "timeBeginPeriod", "timeBeginPeriod" });
	m_WinApiMap.insert({ "SetPriorityClass", "SetPriorityClass" });
	m_WinApiMap.insert({ "GetTickCount", "GetTickCount" });

	m_WinApiMap.insert({ "localtime", "localtime" });
	m_WinApiMap.insert({ "DeleteCriticalSection", "DeleteCriticalSection" });

	m_WinApiMap.insert({ "AreFileApisANSI", "AreFileApisANSI" });
	m_WinApiMap.insert({ "CloseHandle", "CloseHandle" });
	m_WinApiMap.insert({ "CreateFileA", "CreateFileA" });
	m_WinApiMap.insert({ "CreateFileW", "CreateFileW" });
	m_WinApiMap.insert({ "CreateFileMappingA", "CreateFileMappingA" });
	m_WinApiMap.insert({ "CreateFileMappingW", "CreateFileMappingW" });
	m_WinApiMap.insert({ "CreateMutexW", "CreateMutexW" });
	m_WinApiMap.insert({ "DeleteFileA", "DeleteFileA" });
	m_WinApiMap.insert({ "DeleteFileW", "DeleteFileW" });
	m_WinApiMap.insert({ "FlushFileBuffers", "FlushFileBuffers" });
	m_WinApiMap.insert({ "FormatMessageA", "FormatMessageA" });
	m_WinApiMap.insert({ "FormatMessageW", "FormatMessageW" });
	m_WinApiMap.insert({ "FreeLibrary", "FreeLibrary" });
	m_WinApiMap.insert({ "GetCurrentProcessId", "GetCurrentProcessId" });
	m_WinApiMap.insert({ "GetDiskFreeSpaceA", "GetDiskFreeSpaceA" });
	m_WinApiMap.insert({ "GetDiskFreeSpaceW", "GetDiskFreeSpaceW" });
	m_WinApiMap.insert({ "GetFileAttributesA", "GetFileAttributesA" });
	m_WinApiMap.insert({ "GetFileAttributesW", "GetFileAttributesW" });
	m_WinApiMap.insert({ "GetFileAttributesExW", "GetFileAttributesExW" });
	m_WinApiMap.insert({ "GetFileSize", "GetFileSize" });
	m_WinApiMap.insert({ "GetFullPathNameA", "GetFullPathNameA" });
	m_WinApiMap.insert({ "GetFullPathNameW", "GetFullPathNameW" });
	m_WinApiMap.insert({ "GetLastError", "GetLastError" });
	m_WinApiMap.insert({ "GetSystemInfo", "GetSystemInfo" });
	m_WinApiMap.insert({ "GetProcAddress", "GetProcAddress" });
	m_WinApiMap.insert({ "GetSystemTime", "GetSystemTime" });
	m_WinApiMap.insert({ "GetSystemTimeAsFileTime", "GetSystemTimeAsFileTime" });
	m_WinApiMap.insert({ "GetTempPathA", "GetTempPathA" });
	m_WinApiMap.insert({ "GetTempPathW", "GetTempPathW" });
	m_WinApiMap.insert({ "GetTickCount", "GetTickCount" });
	m_WinApiMap.insert({ "GetVersionExA", "GetVersionExA" });
	m_WinApiMap.insert({ "GetVersionExW", "GetVersionExW" });
	m_WinApiMap.insert({ "HeapAlloc", "HeapAlloc" });
	m_WinApiMap.insert({ "HeapCreate", "HeapCreate" });
	m_WinApiMap.insert({ "HeapDestroy", "HeapDestroy" });
	m_WinApiMap.insert({ "HeapFree", "HeapFree" });
	m_WinApiMap.insert({ "HeapReAlloc", "HeapReAlloc" });
	m_WinApiMap.insert({ "HeapSize", "HeapSize" });
	m_WinApiMap.insert({ "HeapValidate", "HeapValidate" });
	m_WinApiMap.insert({ "HeapCompact", "HeapCompact" });
	m_WinApiMap.insert({ "LoadLibraryA", "LoadLibraryA" });
	m_WinApiMap.insert({ "LoadLibraryW", "LoadLibraryW" });
	m_WinApiMap.insert({ "LocalFree", "LocalFree" });
	m_WinApiMap.insert({ "LockFile", "LockFile" });
	m_WinApiMap.insert({ "LockFileEx", "LockFileEx" });
	m_WinApiMap.insert({ "MapViewOfFile", "MapViewOfFile" });
	m_WinApiMap.insert({ "MultiByteToWideChar", "MultiByteToWideChar" });
	m_WinApiMap.insert({ "QueryPerformanceCounter", "QueryPerformanceCounter" });
	m_WinApiMap.insert({ "ReadFile", "ReadFile" });
	m_WinApiMap.insert({ "SetEndOfFile", "SetEndOfFile" });
	m_WinApiMap.insert({ "SetFilePointer", "SetFilePointer" });
	m_WinApiMap.insert({ "Sleep", "Sleep" });
	m_WinApiMap.insert({ "SystemTimeToFileTime", "SystemTimeToFileTime" });
	m_WinApiMap.insert({ "UnlockFile", "UnlockFile" });
	m_WinApiMap.insert({ "UnlockFileEx", "UnlockFileEx" });
	m_WinApiMap.insert({ "UnmapViewOfFile", "UnmapViewOfFile" });
	m_WinApiMap.insert({ "WideCharToMultiByte", "WideCharToMultiByte" });
	m_WinApiMap.insert({ "WriteFile", "WriteFile" });
	m_WinApiMap.insert({ "WaitForSingleObject", "WaitForSingleObject" });
	m_WinApiMap.insert({ "WaitForSingleObjectEx", "WaitForSingleObjectEx" });
	m_WinApiMap.insert({ "OutputDebugStringA", "OutputDebugStringA" });
	m_WinApiMap.insert({ "OutputDebugStringW", "OutputDebugStringW" });
	m_WinApiMap.insert({ "GetProcessHeap", "GetProcessHeap" });
	m_WinApiMap.insert({ "FlushViewOfFile", "FlushViewOfFile" });
}

/**
* ��ȡ������ŵ�ַ, �������ű����Ƿ���ڸ����ֵķ���
* @param name(������)
* @return ������ŵ�ַ
*/
Value* IR::__STableSearch(string name, bool outputerror)
{
	// �����ֲ�����
	auto lit = m_IRSTable.find(name);
	if (lit != m_IRSTable.end()){
		prsnt_sign = lit->second->GetIsSigned();
		return lit->second->GetAllocaInstVar();
	}
	// ����ȫ�ַ��ţ�ȫ�ֱ�������������
	auto git = m_GlobalSTable.find(name);
	if (git != m_GlobalSTable.end()){
		prsnt_sign = git->second->GetIsSigned();
		return git->second->GetGlobalVar();
	}
	// ����δ���庯����
	if (m_FuncNames.find(name) != m_FuncNames.end()){
		func2IR(m_Funcs.find(name)->second);
		return __STableSearch(name, outputerror);
	}

	// external.cpp�ж���ķ���
	ValueSymbolTable &tab = m_module->getValueSymbolTable();
	Value *value = tab.lookup(name);
	if (value != nullptr){
		return value;
	}
	// �������
	if (outputerror)
		cerr << "ERROR: variable " << name << " does not been declared." << endl;
	return nullptr;
}

// mode: 0 stdin�� 1 stdout�� 2 stderr
Value* IR::__GetStdIO(int mode)
{
	Function *f = m_module->getFunction("__acrt_iob_func"); //__iob_func
	return m_builder->CreateCall(f, m_builder->getInt32(mode));
	//return m_builder->CreateInBoundsGEP(value, m_builder->getInt32(mode));
}

//����or��ʱ��Ԥ�Ƚ�һЩ����������ű�
void IR::OrGlobal2IR()
{
	Value *var = nullptr;
	IRSymbol *symbol = nullptr;
	var = new GlobalVariable(*m_module, m_Int32Ty, false, GlobalValue::ExternalLinkage, m_builder->getInt32(0), "$$len_i");
	symbol = new IRSymbol(var, true);
	if (m_GlobalSTable.insert({ "$$len_i", symbol }).second == false){
		cerr << "Global2IR : variable \"$$len_i\" has already defined" << endl;
	}

	var = new GlobalVariable(*m_module, m_Int32Ty, false, GlobalValue::ExternalLinkage, m_builder->getInt32(0), "$$length");
	symbol = new IRSymbol(var, true);
	if (m_GlobalSTable.insert({ "$$length", symbol }).second == false){
		cerr << "Global2IR : variable \"$$length\" has already defined" << endl;
	}

	var = new GlobalVariable(*m_module, m_Int32Ty, false, GlobalValue::ExternalLinkage, m_builder->getInt32(0), "$$current");
	symbol = new IRSymbol(var, true);
	if (m_GlobalSTable.insert({ "$$current", symbol }).second == false){
		cout << "Global2IR : variable \"$$current\" has already defined" << endl;
	}

	var = new GlobalVariable(*m_module, m_Int32Ty, false, GlobalValue::ExternalLinkage, m_builder->getInt32(0), "$$pathNum");
	symbol = new IRSymbol(var, true);
	if (m_GlobalSTable.insert({ "$$pathNum", symbol }).second == false){
		cerr << "Global2IR : variable \"$$pathNum\" has already defined" << endl;
	}

	var = new GlobalVariable(*m_module, m_Int32Ty, false, GlobalValue::ExternalLinkage, m_builder->getInt32(0), "$$firstTime");
	symbol = new IRSymbol(var, true);
	if (m_GlobalSTable.insert({ "$$firstTime", symbol }).second == false){
		cerr << "Global2IR : variable \"$$firstTime\" has already defined" << endl;
	}

	var = new GlobalVariable(*m_module, m_Int32Ty, false, GlobalValue::ExternalLinkage, m_builder->getInt32(0), "$$flag1");
	symbol = new IRSymbol(var, true);
	if (m_GlobalSTable.insert({ "$$flag1", symbol }).second == false){
		cerr << "Global2IR : variable \"$$flag1\" has already defined" << endl;
	}

	var = new GlobalVariable(*m_module, m_Int32Ty, false, GlobalValue::ExternalLinkage, m_builder->getInt32(0), "$$flag2");
	symbol = new IRSymbol(var, true);
	if (m_GlobalSTable.insert({ "$$flag2", symbol }).second == false){
		cerr << "Global2IR : variable \"$$flag2\" has already defined" << endl;
	}

	var = m_module->getGlobalVariable("$$myJmpbuf");
	symbol = new IRSymbol(var, true);
	if (m_GlobalSTable.insert({ "$$myJmpbuf", symbol }).second == false){
		cerr << "Global2IR : variable \"$$myJmpbuf\" has already defined" << endl;
	}

	Type *arraytype = ArrayType::get(m_Int8Ty, 1000000);
	var = new GlobalVariable(*m_module, arraytype, false, GlobalValue::ExternalLinkage, ConstantAggregateZero::get(arraytype), "$$flag");
	symbol = new IRSymbol(var, true);
	if (m_GlobalSTable.insert({ "$$flag", symbol }).second == false){
		cerr << "Global2IR : variable \"$$flag\" has already defined" << endl;
	}
}

/**
* ����ȫ������
* @param pTree(ȫ�ֱ�����)
*/
void IR::Global2IR(CSyntaxNode *pTree)
{
	if (pTree == nullptr)
		return;

	switch (pTree->GetNType()){
	case DECLARE_STA:
		__GlobalInit(pTree, true);	//������ȫ�ֱ�������
		break;
	case CHOP_STA:	//����ȫ�ֱ�����
		Global2IR(pTree->GetChild0());
		Global2IR(pTree->GetChild1());
		break;
	default:	//��������ȫ�ֱ������ڵ�
		cerr << "ERROR: in Global variables tree has not-declare node." << endl;
	}
}

/**
* ����ȫ�ֱ�����ʼ��
* @param pTrlee(ȫ�ֱ�����) sign(�Ƿ��з���)
*/
void IR::__GlobalInit(CSyntaxNode *pTree, bool sign)
{
	//��ô�����������
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

		if (pTree->GetRType() == FUNCPTYPE) // function pointer �ڵ��Child1�Ǻ������ͣ���Ϊ�գ�����Ҫ��ǰ���ء�
			return;
		visit = visit->GetChild1();
	}
}

//���ݴ���Ļ������ͺ��﷨��
VarInfo IR::__GetPointerInfo(Type *type, CSyntaxNode *pTree)
{
	VarInfo vInfo;
	while (pTree != nullptr){
		switch (pTree->GetNType()){
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
		default:
			cerr << "error" << endl;
		}
		pTree = pTree->GetChild0();
	}
	vInfo.type = type;
	return vInfo;
}

//���ݴ���Ļ������ͺ��﷨��������Array_Declare_STA�����
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

//���ݴ���Ļ������ͺ��﷨�����������������
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

//���ݴ���Ļ������ͺ��﷨�������������βΡ�����ֵ
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
		return;
	}
	switch (pTree->GetNType()){
	case DECLARE_STA:
		__Declr2IR(pTree, true);//����signed���͵��������
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
		//ȫ�ֱ�������
	case GLOBAL_INITSET_STA:{
		__GlbSetZero();
		//���GLOBAL_INITSET_STA��λ�ǵ��޸���������Ǹ�ȫ�ֱ�����Chn��Ԫ�ظ�ֵ�ĵط�
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
		//m_builder->CreateCall(m_module->getFunction("printPathTime"));
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
	Function *draw = m_module->getFunction("Draw");
	m_builder->CreateCall(draw);
}

void IR::__GenerateEdge(CSyntaxNode *pTree)
{
	BasicBlock *IfEnd = nullptr;
	if (m_InFunc){
		Function *TheFunction = m_builder->GetInsertBlock()->getParent();
		BasicBlock *ThenBB = BasicBlock::Create(m_module->getContext(), "then", TheFunction);
		IfEnd = BasicBlock::Create(m_module->getContext(), "ifend", TheFunction);
		//������ת
		Value *ExtValue = m_builder->CreateLoad(__STableSearch("$$Ext"));
		Value *ExtCondition = m_builder->CreateICmpEQ(ExtValue, m_builder->getInt32(0));
		Value *v = m_builder->CreateCondBr(ExtCondition, ThenBB, IfEnd);//�������0�Ļ�����ʾ���ڲ����ã���ʱִ��then��֧�������״̬
		m_builder->SetInsertPoint(ThenBB);
	}

	m_builder->CreateCall(m_module->getFunction("$$GetVarsValue"));
	__GetLocalVar(pTree);

	Function *AddEdge = m_module->getFunction("AddEdge");

	Value *varsInfo = m_module->getGlobalVariable("$$varsInfo");
	vector<Value*> pos;
	pos.push_back(m_builder->getInt32(0));
	pos.push_back(m_builder->getInt32(0));
	varsInfo = m_builder->CreateInBoundsGEP(varsInfo, pos);

	vector<Value*> args;
	args.push_back(varsInfo);
	args.push_back(m_builder->getInt32(pTree->GetiValue()));
	args.push_back(m_builder->getInt32(pTree->GetcValue()));
	m_builder->CreateCall(AddEdge, args);
	m_builder->CreateMemSet(varsInfo, m_builder->getInt8(0), 10000, 1, false);
	m_builder->CreateStore(m_builder->getInt32(0), m_module->getGlobalVariable("$$tempLen"));

	if (m_InFunc){
		m_builder->CreateBr(IfEnd);
		m_builder->SetInsertPoint(IfEnd);
	}
}

void IR::__GetVarsValue()
{
	BasicBlock *fatherinsertpoint = m_builder->GetInsertBlock();
	//���캯������
	FunctionType* FTy = FunctionType::get(m_builder->getVoidTy(), vector<Type*>(), false);
	//��������
	Function *Func = Function::Create(FTy, Function::ExternalLinkage, "$$GetVarsValue", m_module);
	BasicBlock *entry = BasicBlock::Create(m_module->getContext(), "entry", Func, 0);
	m_builder->SetInsertPoint(entry);

	/*ȫ�ֱ���*/
	Function *addChar = m_module->getFunction("$$addChar");
	for (auto iter = m_GlobalSTable.begin(); iter != m_GlobalSTable.end(); iter++){
		string name = iter->first;
		if (name.find("$$") == 0 || name.find("##") == 0)
			continue; //��$$��##��ͷ�ı���ʲô������

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
	Function *addChar = m_module->getFunction("$$addChar");
	/*�ֲ�����*/
	if (!(pTree->GetST().empty())){ //����б����Ļ����������²���������ʲôҲ����
		__CatValue(m_builder->CreateGlobalString("   " + string(m_builder->GetInsertBlock()->getParent()->getName()) + ":    "));
		vector<string> outPutSymTbl = pTree->GetST();//��Ҫ����ı�������vector�洢
		for (auto iter = outPutSymTbl.begin(); iter != outPutSymTbl.end(); iter++){
			string name = *iter;
			if (name.find("$$") == 0 || name.find("##") == 0)
				continue; //��$$��##��ͷ�ı���ʲô������

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

void IR::__CatValue(Value *value) // add by wangmeng for unifiedmc ���߳�
{
	Type *type = value->getType()->getContainedType(0);
	if (type->isIntegerTy(32)){
		Function * addInt = m_module->getFunction("$$addInt");
		value = m_builder->CreateLoad(value, true);
		m_builder->CreateCall(addInt, value);
		return;
	}
	if (type->isIntegerTy(8)){
		Function * addChar = m_module->getFunction("$$addChar");
		value = m_builder->CreateLoad(value, true);
		m_builder->CreateCall(addChar, value);
		return;
	}
	if (type->isFloatTy()){
		Function * addFloat = m_module->getFunction("$$addFloat");
		value = m_builder->CreateLoad(value, true);
		m_builder->CreateCall(addFloat, value);
		return;
	}
	if ((type->isPointerTy() || type->isArrayTy()) && type->getContainedType(0)->isIntegerTy(8)){
		Function * addString = m_module->getFunction("$$addString");
		value = __Cast2IR(value, m_builder->getInt8PtrTy(), true);
		m_builder->CreateCall(addString, value);
		return;
	}
	if (type->isPointerTy()){
		Function * addInt = m_module->getFunction("$$addInt");
		value = m_builder->CreateLoad(value, true);
		value = m_builder->CreatePtrToInt(value, m_Int64Ty);
		m_builder->CreateCall(addInt, value);
		return;
	}
	if (type->isArrayTy() || type->isStructTy()){
		int elementNum;
		if (type->isStructTy())
			elementNum = type->getStructNumElements();
		else
			elementNum = type->getArrayNumElements();

		Function *addChar = m_module->getFunction("$$addChar");

		vector<Value*> subScript;
		m_builder->CreateCall(addChar, m_builder->getInt8('['));

		for (int i = 0; i < elementNum; i++){
			subScript.push_back(m_builder->getInt32(0));
			subScript.push_back(m_builder->getInt32(i)); //�����±�
			Value *arrayElement = m_builder->CreateInBoundsGEP(value, subScript); //ȡ�±괦��ֵ
			__CatValue(arrayElement);
			subScript.clear();
			if (i != (elementNum - 1)){ //����������һ������Ԫ�أ����Ԫ��֮�������������Ϊ�˺���һ��Ԫ�طֿ�
				m_builder->CreateCall(addChar, m_builder->getInt8(','));
			}
		}
		m_builder->CreateCall(addChar, m_builder->getInt8(']'));
	}
}

/**
* ������ת�ɶ�Ӧ��IR����
* @param ������������﷨��
* @return void
*/
void IR::__Declr2IR(CSyntaxNode *pTree, bool sign)
{
	Type *type = GetType(pTree); //��ô�����������
	if (type == nullptr){
		cerr << "ERROR: Basic Type is not considered in __Declr2IR(). TypeID = " << pTree->GetRType() << endl;
		return;
	}
	
	BasicBlock *bb = m_builder->GetInsertBlock();
	BasicBlock::iterator it = bb->getParent()->getEntryBlock().begin();
	m_builder->SetInsertPoint(&*it);

	bool isAsync = __AsyncSubscript(pTree);
	CSyntaxNode* visit = pTree->GetChild0();
	while (visit != nullptr){
		VarInfo vInfo = __GetVarInfo(type, visit);
		AllocaInst *alloc = m_builder->CreateAlloca(vInfo.type);
		alloc->setName(vInfo.name);
		alloc->setAlignment(CalculateAlign(vInfo.type));

		IRSymbol *symbol = new IRSymbol(alloc, prsnt_sign);

		if (m_IRSTable.insert({ vInfo.name, symbol }).second == false){
			cerr << "ERROR: variable " << vInfo.name << " has already been defined.(declare error)" << endl;
		}

		if (pTree->GetRType() == FUNCPTYPE) // function pointer �ڵ��Child1�Ǻ������ͣ���Ϊ�գ�����Ҫ��ǰ���ء�
			break;

		if (isAsync && visit->GetChild0()->GetNType() == ARRAY_DECLARE_STA)
			__AsyncAss(alloc);

		visit = visit->GetChild1();
	}
	m_builder->SetInsertPoint(bb);
}

//ȫ�ֱ�������
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
	m_builder->CreateMemSet(m_module->getGlobalVariable("$$varsInfo"), m_builder->getInt8(0), 100000, 1, false);
	m_builder->CreateStore(m_builder->getInt32(0), m_module->getGlobalVariable("$$tempLen"));
}

/*
*Ϊ�ۼ����͸���ֵ�������顢�ṹ��
*@param  left(�����±�)  value(ֵ)  right(Ҫ����ֵ)
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
	// MTYPE p[] = {...}
	if (type->isArrayTy()){
		//����������ʼ��ַ
		vector<Value*> pos;
		pos.push_back(m_builder->getInt32(0));
		pos.push_back(m_builder->getInt32(0));
		value = m_builder->CreateInBoundsGEP(value, pos);

		//��������Ԫ��
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
			value = m_builder->CreateInBoundsGEP(value, m_builder->getInt32(1));
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
*Ϊ�ۼ����͸���ֵ�������顢�ṹ��
*@param  left(�����±�)  value(ֵ)  right(Ҫ����ֵ)
*/
void IR::__AssStructTy(Value *value, CSyntaxNode *right)
{
	Type *type = value->getType()->getContainedType(0);

	vector<Value*> pos;
	pos.push_back(m_builder->getInt32(0));

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
	if (LType == RType) {
		prsnt_sign = LSigned && RSigned;
		return LType;
	}

	//bool || char
	if (LType->isIntegerTy(1) || LType->isIntegerTy(8)){
		if (RType->isIntegerTy(1) || RType->isIntegerTy(8) || RType->isIntegerTy(16)){
			prsnt_sign = true;
			return m_Int32Ty;
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
	// long long
	else if (LType->isIntegerTy(64)) {
		if (RType->isIntegerTy(1) || RType->isIntegerTy(8) || RType->isIntegerTy(16) || RType->isIntegerTy(32)) {
			prsnt_sign = LSigned;
			return LType;
		}
		else if (RType->isIntegerTy(64)) {
			prsnt_sign = LSigned && RSigned;
			return LType;
		}
		else if (RType->isFloatTy() || RType->isDoubleTy()) {
			prsnt_sign = true;
			return RType;
		}
		else if (RType->isPointerTy() || RType->isArrayTy()) {
			prsnt_sign = RSigned;
			return RType;
		}
		else {
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

	if (LType == RType) {
		prsnt_sign = LSigned && RSigned;
		return LType;
	}

	//bool || char
	if (LType->isIntegerTy(1) || LType->isIntegerTy(8)){
		if (RType->isIntegerTy(1) || RType->isIntegerTy(8) || RType->isIntegerTy(16)){
			prsnt_sign = true;
			Type *rtn = m_Int32Ty;
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
			LValue = __Cast2IR(LValue, m_Int64Ty, LSigned);
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
			LValue = __Cast2IR(LValue, m_Int64Ty, LSigned);
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
	//int
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
			LValue = __Cast2IR(LValue, m_Int64Ty, LSigned);
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
			LValue = __Cast2IR(LValue, m_Int64Ty, LSigned);
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
			LValue = __Cast2IR(LValue, m_Int64Ty, LSigned);
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
			RValue = __Cast2IR(RValue, m_Int64Ty, RSigned);
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

// ����chop���͵Ľ��
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
* ��ֵ���ת�ɶ�Ӧ��IR����
* @param ������������﷨��
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

	// ���㸴�ӵ���ֵ
	Value *LeftValue = __Expr2IR(pTree->GetChild0(), false);
	if (LeftValue == nullptr || !LeftValue->getType()->isPointerTy()){
		cerr << "ERROR: invalid assignment expression." << endl;
		return;
	}

	bool lsign = prsnt_sign;
	Type *LType = LeftValue->getType()->getContainedType(0);
	

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
* ���ʽת�ɶ�Ӧ��IR����
* @param ������������﷨��
* @return ת��֮��Ľ��
*/
Value* IR::__Expr2IR(CSyntaxNode *pTree, bool load)
{
	if (pTree == nullptr){
		cerr << "ERROR: invalid expression." << endl;
		return nullptr;
	}

	switch (pTree->GetNType()){
	case INTEGER_EXP:{
		return m_builder->getInt32(pTree->GetiValue());
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
		return m_builder->getInt8(pTree->GetcValue());
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
		return ConstantPointerNull::get(PointerType::get(m_Int32Ty, 0));
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
			//����ָ��Ӽ����ö�ȡ�����ڼӼ��ж�ȡ, �����ķ���ֵҲ��Ҫ��ȡ
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

		Function *f = m_module->getFunction(name);
		if ( f != nullptr){
			load = false;
			return f;
		}

		// δ���庯��
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
		Type *type = GetType(pTree->GetChild0());
		for (auto tt = pTree->GetChild0()->GetChild0(); tt != nullptr; tt = tt->GetChild0()){
			type = PointerType::get(type, 0);
		}
			
		Value *value = __Expr2IR(pTree->GetChild1(), true);
		value = __Cast2IR(value, type, prsnt_sign);
		
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

//������
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
		vector<Value*> args;
		args.push_back(m_builder->CreateInBoundsGEP(format, sub));
		args.push_back(m_builder->CreateLoad(__STableSearch("$$firstTime")));
		m_builder->CreateCall(putsFunc, args);

		format = m_builder->CreateGlobalString("%s");
		args.clear();
		args.push_back(m_builder->CreateInBoundsGEP(format, sub));
		args.push_back(m_builder->CreateGlobalString("\n"));
		m_builder->CreateCall(putsFunc, args);
		return;
	}
	Value *format = m_builder->CreateGlobalString("%s");
	vector<Value*> args;
	args.push_back(m_builder->CreateInBoundsGEP(format, sub));
	args.push_back(m_builder->CreateGlobalString(str + "\n"));
	m_builder->CreateCall(putsFunc, args);

}


//�ж��Ƿ��ں����У�����ں����У���Ҫ�ж�$$Ext����
void IR::__WhetherOutput(CSyntaxNode *pTree)
{
	if (m_InFunc)//����ں����еĻ���Ҫ���ж�һ��$$Ext�����������1�Ļ���������__StateOut2IR�����������0�Ļ�������__StateOut2IR����
	{
		Function *TheFunction = m_builder->GetInsertBlock()->getParent();

		BasicBlock *ThenBB = BasicBlock::Create(m_module->getContext(), "then", TheFunction);
		BasicBlock *ElseBB = BasicBlock::Create(m_module->getContext(), "else", TheFunction);
		BasicBlock *IfEnd = BasicBlock::Create(m_module->getContext(), "ifend", TheFunction);

		//������ת
		Value *ExtValue = m_builder->CreateLoad(m_IRSTable["$$Ext"]->GetLocalVar());
		//�������0�Ļ�����ʾ���ڲ����ã���ʱִ��then��֧�������״̬
		Value *cond = m_builder->CreateICmpEQ(ExtValue, m_builder->getInt32(0));
		Value *v = m_builder->CreateCondBr(cond, ThenBB, ElseBB);

		m_builder->SetInsertPoint(ThenBB);
		__StateOut2IR(pTree);
		m_builder->CreateBr(IfEnd);

		m_builder->SetInsertPoint(ElseBB);
		m_builder->CreateBr(IfEnd);

		m_builder->SetInsertPoint(IfEnd);
	}
	else//���Ǻ�������ʱ��ֱ�Ӿ͵���__Out2IR����
		__StateOut2IR(pTree);
}

void IR::__CreateIsOutput()
{
	BasicBlock *fatherinsertpoint = m_builder->GetInsertBlock();
	//���캯������
	FunctionType* FT = FunctionType::get(m_builder->getInt32Ty(), vector<Type*>(0), false);
	//��������
	Function* Func = Function::Create(FT, Function::ExternalLinkage, "$$IsOutput", m_module);
	BasicBlock *entry = BasicBlock::Create(m_module->getContext(), "entry", Func, 0);
	m_builder->SetInsertPoint(entry);
	Function *TheFunction = m_builder->GetInsertBlock()->getParent();
	BasicBlock *ThenBB = BasicBlock::Create(m_module->getContext(), "then", TheFunction);

	//������ת
	Value * v = m_builder->CreateICmpEQ(m_builder->CreateLoad(m_StNum), m_builder->getInt32(Setting::getMaxStatus()));
	BasicBlock *IfEnd = BasicBlock::Create(m_module->getContext(), "ifend", TheFunction);
	m_builder->CreateCondBr(v, ThenBB, IfEnd);
	m_builder->SetInsertPoint(ThenBB);

	m_builder->CreateCall(m_module->getFunction("exit"), m_builder->getInt32(0));
	m_builder->CreateBr(IfEnd);
	m_builder->SetInsertPoint(IfEnd);

	Value *temp = m_builder->CreateSRem(m_builder->CreateLoad(m_StNum), m_builder->getInt32(Setting::getStatusInterval()));
	temp = m_builder->CreateICmpEQ(temp, m_builder->getInt32(0));
	temp = __Cast2IR(temp, m_builder->getInt32Ty(), false);
	m_builder->CreateRet(temp);
	m_builder->SetInsertPoint(fatherinsertpoint);
}


//����printf���������ֵ
void IR::__StateOut2IR(CSyntaxNode *pTree)
{
	static Value *strstate = m_builder->CreateGlobalStringPtr("#State");
	static Value *numF = m_builder->CreateGlobalStringPtr("[%d]");
	static Value *eqsF = m_builder->CreateGlobalStringPtr("=");
	static Value *strand = m_builder->CreateGlobalStringPtr(" and ");
	static Value *nl = m_builder->CreateGlobalStringPtr("\n");

	Function *TheFunction = m_builder->GetInsertBlock()->getParent();
	BasicBlock *ThenBB = BasicBlock::Create(m_module->getContext(), "then", TheFunction);

	//������ת
	Value *v = m_builder->CreateCall(m_module->getFunction("$$IsOutput"));
	v = __Cast2IR(v, m_builder->getInt1Ty(), false);
	BasicBlock *IfEnd = BasicBlock::Create(m_module->getContext(), "ifend", TheFunction);
	m_builder->CreateCondBr(v, ThenBB, IfEnd);
	m_builder->SetInsertPoint(ThenBB);

	//������printf������Ȼ����ݲ�ͬ�ı������ͣ����е���
	std::vector<llvm::Type *> putsArgs;
	putsArgs.push_back(m_builder->getInt8Ty()->getPointerTo());
	llvm::ArrayRef<llvm::Type*>  argsRef(putsArgs);
	llvm::FunctionType *putsType = llvm::FunctionType::get(m_builder->getInt32Ty(), argsRef, true);
	llvm::Constant *putsFunc = m_module->getOrInsertFunction("printf", putsType);

	//ÿ�����������ֵ֮ǰ��������ǵڼ���״̬,��state 0��
	m_builder->CreateCall(putsFunc, strstate);
	LoadInst *m_StNumVal = m_builder->CreateLoad(m_StNum, true);
	vector<Value*> args; 
	args.push_back(numF);
	args.push_back(m_StNumVal);
	m_builder->CreateCall(putsFunc, args);

	m_builder->CreateCall(m_module->getFunction("$$GlobalOutput"));

	if (pTree->GetST().size() != 0)//����б����Ļ����������²���������ʲôҲ����
	{
		vector<string> outPutSymTbl;//��Ҫ����ı�������vector�洢
		outPutSymTbl = pTree->GetST();

		vector<string>::iterator iter;
		bool globalFlag = 0;//����һ����־λ����ʾ����ı����Ƿ���ȫ�ֱ���
		for (iter = outPutSymTbl.begin(); iter != outPutSymTbl.end(); iter++){
			string a = *iter;
			int position = a.find("##");//Ҫô��##��������Ҫô��##Global
			if (position == 0){
				string outputString(a, 2);
				m_builder->CreateCall(putsFunc, m_builder->CreateGlobalStringPtr(outputString + ": "));
				if (a == "##Global")
					globalFlag = 1;//��ʾ����ı�������ȫ�ֱ���������־λ��Ϊ1
				else
					globalFlag = 0;//����־λ��Ϊ0
			}
			else{
				position = a.find("$$");
				if (position != 0){
					//����ÿһ�������������ʽ����x=1
					m_builder->CreateCall(putsFunc, m_builder->CreateGlobalStringPtr(*iter));
					m_builder->CreateCall(putsFunc, eqsF); // "="
					Value *outPutVar;
					if (globalFlag == 1){ //ȫ�ֱ���
						outPutVar = m_GlobalSTable[*iter]->GetGlobalVar();//ȫ�ֱ�����ͨ��������������m_IRSTable���ҵ���Ӧ��AllocaInst����ָ��
						prsnt_sign = m_GlobalSTable[*iter]->GetIsSigned();//���ӻ�ȡ���ţ�����signed��unsigned�������������
					}
					else{ //�ֲ�����
						outPutVar = m_IRSTable[*iter]->GetLocalVar();//�ֲ�������ͨ��������������m_IRSTable���ҵ���Ӧ��AllocaInst����ָ��
						prsnt_sign = m_IRSTable[*iter]->GetIsSigned();//���ӻ�ȡ���ţ�����signed��unsigned�������������
					}
					__TypeOutput(putsFunc, outPutVar);
					m_builder->CreateCall(putsFunc, strand);//ÿ���������֮����������ո��Ա����һ���������������
				}
			}
		}
	}
	m_builder->CreateCall(putsFunc, nl);//ÿ��״̬���֮�󣬻���

	m_builder->CreateBr(IfEnd);
	m_builder->SetInsertPoint(IfEnd);
	__AddOne2IR(m_StNum);//��״̬����1
}

//�Բ�ͬ�Ļ������ͽ������
void IR::__TypeOutput(Constant *putsFunc, Value *outPutVar)
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
	vector<Value*> args;
	if (type->isIntegerTy(8)){
		LoadInst *a = m_builder->CreateLoad(outPutVar, 1);
		if (prsnt_sign) {
			args.push_back(cF);
			args.push_back(a);
		}
		else {
			args.push_back(uF);
			args.push_back(a);
		}
		m_builder->CreateCall(putsFunc, args);
	}
	else if (type->isIntegerTy(16)){
		LoadInst *a = m_builder->CreateLoad(outPutVar, true);
		if (prsnt_sign) {
			args.push_back(hdF);
			args.push_back(a);
		}
		else {
			args.push_back(huF);
			args.push_back(a);
		}
		m_builder->CreateCall(putsFunc, args);
	}
	else if (type->isIntegerTy(32)){
		LoadInst *a = m_builder->CreateLoad(outPutVar, true);
		if (prsnt_sign) {
			args.push_back(dF);
			args.push_back(a);
		}
		else {
			args.push_back(uF);
			args.push_back(a);
		}
		m_builder->CreateCall(putsFunc, args);
	}
	else if (type->isIntegerTy(64)){
		LoadInst *a = m_builder->CreateLoad(outPutVar, true);
		if (prsnt_sign) {
			args.push_back(ldF);
			args.push_back(a);
		}
		else {
			args.push_back(luF);
			args.push_back(a);
		}
		m_builder->CreateCall(putsFunc, args);
	}
	else if (type->isFloatTy()){
		LoadInst *a = m_builder->CreateLoad(outPutVar);
		a->setAlignment(CalculateAlign(type));
		Value *f2d = m_builder->CreateFPExt(a, Type::getDoubleTy(m_module->getContext()));
		args.push_back(fF);
		args.push_back(f2d);
		m_builder->CreateCall(putsFunc, args);
	}
	else if (type->isDoubleTy()){
		LoadInst *a = m_builder->CreateLoad(outPutVar);
		a->setAlignment(CalculateAlign(type));
		args.push_back(lfF);
		args.push_back(a);
		m_builder->CreateCall(putsFunc, args);
	}
	else if (type->isPointerTy())
	{
		args.push_back(sF);
		args.push_back(m_builder->CreateLoad(outPutVar, true));
		m_builder->CreateCall(putsFunc, args);
	}
	else if (type->isArrayTy() && type->getContainedType(0)->isIntegerTy(8))
	{
		args.push_back(sF);
		args.push_back(outPutVar);
		m_builder->CreateCall(putsFunc, args);
	}
	else if (type->isPointerTy())//ָ�����
	{
		LoadInst *a = m_builder->CreateLoad(outPutVar, true);
		//���һ��ǿ������ת����ֱ�����ָ������Ϊ0��Ϊ�����ֵ�ַ�����ǿ������ת��
		args.push_back(dF);
		args.push_back(m_builder->CreatePtrToInt(a, m_Int64Ty));
		m_builder->CreateCall(putsFunc, args);
	}
	else if (type->isArrayTy())//��������,add by yubin 2015/5/15
	{
		auto elementNum = type->getArrayNumElements();
		vector<Value*> subScript;
		m_builder->CreateCall(putsFunc, lsbF);
		for (unsigned int i = 0; i < elementNum; i++)
		{
			subScript.push_back(m_builder->getInt32(0));
			//�����±�
			subScript.push_back(m_builder->getInt32(i));
			//ȡ�±괦��ֵ
			Value *arrayElement = m_builder->CreateInBoundsGEP(outPutVar, subScript);
			__TypeOutput(putsFunc, arrayElement);
			subScript.clear();
			if (i != (elementNum - 1))//����������һ������Ԫ�أ����Ԫ��֮�������������Ϊ�˺���һ��Ԫ�طֿ�
				m_builder->CreateCall(putsFunc, cmF);
		}
		m_builder->CreateCall(putsFunc, rsbF);
	}
	else if (type->isStructTy())
	{
		auto elementNum = type->getStructNumElements();
		vector<Value*> subScript;
		m_builder->CreateCall(putsFunc, lsbF);

		for (unsigned int i = 0; i < elementNum; i++)
		{
			subScript.push_back(m_builder->getInt32(0));
			//�����±�
			subScript.push_back(m_builder->getInt32(i));
			//ȡ�±괦��ֵ
			Value *arrayElement = m_builder->CreateInBoundsGEP(outPutVar, subScript);
			__TypeOutput(putsFunc, arrayElement);
			subScript.clear();
			if (i != (elementNum - 1))//����������һ������Ԫ�أ����Ԫ��֮�������������Ϊ�˺���һ��Ԫ�طֿ�
				m_builder->CreateCall(putsFunc, cmF);
		}
		m_builder->CreateCall(putsFunc, rsbF);
	}
}

//Ϊ�˼�С������Ĺ�ģ����ȫ�ֱ����������Ϊһ������
void IR::__CreateGlbOutFunc()
{
	static Value *strglobal = m_builder->CreateGlobalStringPtr("Global: ");
	static Value *strand = m_builder->CreateGlobalStringPtr(" and ");

	BasicBlock *fatherinsertpoint = m_builder->GetInsertBlock();
	//���캯������
	FunctionType *FT = FunctionType::get(m_builder->getVoidTy(), vector<Type*>(0), false);
	//��������
	Function* Func = Function::Create(FT, Function::ExternalLinkage, "$$GlobalOutput", m_module);
	BasicBlock *entry = BasicBlock::Create(m_module->getContext(), "entry", Func, 0);
	m_builder->SetInsertPoint(entry);

	std::vector<llvm::Type *> putsArgs;
	putsArgs.push_back(m_builder->getInt8Ty()->getPointerTo());
	llvm::ArrayRef<llvm::Type*>  argsRef(putsArgs);
	llvm::FunctionType *putsType = llvm::FunctionType::get(m_Int32Ty, argsRef, true);
	llvm::Constant *putsFunc = m_module->getOrInsertFunction("printf", putsType);

	m_builder->CreateCall(putsFunc, strglobal);

	for (auto it = m_GlobalSTable.begin(); it != m_GlobalSTable.end(); it++)
	{
		string name = (*it).first;
		if (name.at(0) == '$' && name.at(1) == '$')
			continue;

		Value *value = it->second->GetGlobalVar();
		prsnt_sign = it->second->GetIsSigned();

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
	//������Ԫ�ص�ַ
	Value *value = __Expr2IR(pTree->GetChild0(), false);
	if (value == nullptr){
		cerr << "ERROR: invalid array expression owing to invalid array identifier." << endl;
		return nullptr;
	}

	bool temp = prsnt_sign;

	vector<Value*> indexs;
	indexs.push_back(m_builder->getInt32(0));

	CSyntaxNode *option = pTree->GetChild1();
	while (option != nullptr){
		Value *num = __Expr2IR(option->GetChild0(), true); //�����±�
		if (num == nullptr){
			cerr << "ERROR: invalid array subscript." << endl;
			num = m_builder->getInt32(0);
		}

		Type *nTy = num->getType();
		if (!nTy->isIntegerTy(32)){
			num = __Cast2IR(num, m_Int32Ty, prsnt_sign);
		}

		indexs.push_back(num);
		option = option->GetChild1();
	}

	Type *type = value->getType();
	Type *cType = type->getContainedType(0);

	if (cType->isPointerTy()){ // ָ�뵱�������õ����
		if (indexs.size() != 2){
			cerr << "ERROR: cannot use pointer as a multidimensional array."<< endl;
		}
		auto v = m_builder->CreateLoad(value, true);
		value = m_builder->CreateInBoundsGEP(v, indexs[1]);
	}
	else if (cType->isArrayTy()){ // ��������
		value = m_builder->CreateInBoundsGEP(value, indexs); // ȡ�±괦��ֵ
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
	//�����ַ
	Value *value = __Expr2IR(pTree->GetChild0(), false);
	if (value == nullptr){
		cerr << "ERROR: invalid value list statement." << endl;
		return nullptr;
	}

	bool temp = prsnt_sign;
	pTree = pTree->GetChild1();
	while (pTree != nullptr && pTree->GetChild0() != nullptr)
	{
		//�����±�
		Value *num = __Expr2IR(pTree->GetChild0(), true);
		if (num == nullptr){
			cerr << "ERROR: invalid array subscript expression." << endl;
			num = m_builder->getInt32(0);
		}
		
		if (prsnt_sign == false && (num->getType()->isIntegerTy(8) || num->getType()->isIntegerTy(16))){
			num = m_builder->CreateZExt(num, m_Int32Ty);
		}

		Type *type = value->getType()->getContainedType(0);
		
		if (type->isPointerTy()){ // ָ����Ϊ����
			value = m_builder->CreateInBoundsGEP(m_builder->CreateLoad(value, true), num);
		}
		else if (type->isArrayTy()){ // ����������
			vector<Value*> subScript;
			subScript.push_back(m_builder->getInt32(0));
			subScript.push_back(num);
			value = m_builder->CreateInBoundsGEP(value, subScript);
		}
		else{ // ����
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
* input����ת�ɶ�Ӧ��IR����
* @param �﷨��
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

		//����scanf����
		std::vector<llvm::Type *> getsArgs;
		getsArgs.push_back(m_builder->getInt8PtrTy());
		llvm::ArrayRef<llvm::Type*>  argsRef(getsArgs);
		llvm::FunctionType *getsType = llvm::FunctionType::get(m_builder->getInt32Ty(), argsRef, true);
		llvm::Constant *putsFunc = m_module->getOrInsertFunction("scanf", getsType);

		Value *addr = __Expr2IR(visit, false);  //��ȡ�����ڷ��ű���ĵ�ַ
		Type *valueType = addr->getType()->getContainedType(0);
		vector<Value*> args;
		if (valueType->isIntegerTy(8))         //�ַ���char
		{
			args.push_back(cF);
		}
		else if (valueType->isIntegerTy(16))    //����int
		{
			args.push_back(hdF);
		}
		else if (valueType->isIntegerTy(32))   //����int
		{
			args.push_back(dF);
		}
		else if (valueType->isIntegerTy(64))   //����int
		{
			args.push_back(ldF);
		}
		else if (valueType->isFloatTy())       //������float
		{
			args.push_back(fF);
		}
		else if ((valueType->isPointerTy() || valueType->isArrayTy()) && valueType->getContainedType(0)->isIntegerTy(8)) // �ַ���
			args.push_back(sF);

		args.push_back(addr);
		m_builder->CreateCall(putsFunc, args);
		visitVar = visitVar->GetChild1();
	}
}

/**
* output����ת�ɶ�Ӧ��IR����
* @param ������ı���
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
	//������printf������Ȼ����ݲ�ͬ�ı������ͣ����е���

	std::vector<llvm::Type *> putsArgs;
	putsArgs.push_back(m_builder->getInt8PtrTy());
	llvm::ArrayRef<llvm::Type*>  argsRef(putsArgs);
	llvm::FunctionType *putsType = llvm::FunctionType::get(m_builder->getInt32Ty(), argsRef, true);
	llvm::Constant *putsFunc = m_module->getOrInsertFunction("printf", putsType);

	for (pTree = pTree->GetChild0(); pTree != nullptr; pTree = pTree->GetChild1())
	{
		if (pTree->GetChild0()->GetNType() == STR_EXP)   //�ַ���
		{
			m_builder->CreateCall(putsFunc, m_builder->CreateGlobalStringPtr(pTree->GetChild0()->GetsValue()));
			continue;
		}
		else if (pTree->GetChild0()->GetNType() == IDENT_EXP)  //output(x)�����ı���
		{
			Value* value = __Expr2IR(pTree->GetChild0(), false);
			__TypeOutput(putsFunc, value);
			continue;
		}

		//output(���ʽ) ,output(����),output(*p)
		Value *value = __Expr2IR(pTree->GetChild0(), true);
		if (value == nullptr)
			continue;
		Type* type = value->getType();

		vector<Value*> args;
		if (type->isIntegerTy(32))    //int��
		{
			if (prsnt_sign)
				args.push_back(dF); //m_builder->CreateCall2(putsFunc, dF, value);
			else
				args.push_back(uF); //m_builder->CreateCall2(putsFunc, uF, value);
		}
		else if (type->isFloatTy())   //float�� // ������ת������float����ת����double���ͣ��������ʱ�����
		{
			value = m_builder->CreateFPExt(value, Type::getDoubleTy(m_module->getContext()));
			args.push_back(fF);//m_builder->CreateCall2(putsFunc, fF, floatTyToDoubleTy);
		}
		else if (type->isDoubleTy())   //double��
			args.push_back(lfF); //m_builder->CreateCall2(putsFunc, lfF, value);
		else if (type->isIntegerTy(8)) //char��
		{
			if (prsnt_sign)
				args.push_back(cF);//m_builder->CreateCall2(putsFunc, cF, value);
			else
				args.push_back(uF);//m_builder->CreateCall2(putsFunc, uF, value);
		}
		else if (type->isIntegerTy(16)) //short��
		{
			if (prsnt_sign)
				args.push_back(hdF); //m_builder->CreateCall2(putsFunc, hdF, value);
			else
				args.push_back(huF); //m_builder->CreateCall2(putsFunc, huF, value);
		}
		else if (type->isIntegerTy(64)) //long��
		{
			if (prsnt_sign)
				args.push_back(ldF); //m_builder->CreateCall2(putsFunc, ldF, value);
			else
				args.push_back(luF); //m_builder->CreateCall2(putsFunc, luF, value);
		}
		else if (type->isPointerTy() && type->getContainedType(0)->isIntegerTy(8))
		{
			args.push_back(sF); //m_builder->CreateCall2(putsFunc, sF, value);
		}
		else if (type->isPointerTy())
		{
			//���һ��ǿ������ת����ֱ�����ָ������Ϊ0��Ϊ�����ֵ�ַ�����ǿ������ת��
			value = m_builder->CreatePtrToInt(value, m_Int32Ty);
			args.push_back(dF);
		}
		else{
			cerr << "ERROR: invalid output parameter." << endl;
			PAUSE;
		}

		args.push_back(value);
		m_builder->CreateCall(putsFunc,args);
	}
}


/**
*ȡ�ñ��ʽ�нṹ���ֵ
*@param pTree(�ṹ�����ڵ��﷨��) StructP(����������� ->��true��.(false))
*@return ת�����ֵ
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
*ȡ�ñ��ʽ�нṹ���Ա��ֵ,�ݹ鴦��ṹ���Ա���ǽṹ������
*@param pTree(�������ڵ��﷨��) value(����ṹ��ָ��,���շ��ؽṹ���Ա��ַ)
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
	pos.push_back(m_builder->getInt32(0));

	switch (pTree->GetNType()){
	case IDENT_EXP:{
		int index = s->GetElementPos(eleName);
		if (index < 0){
			cerr << "ERROR: struct " << sname << " does not have element: " << eleName << endl;
			//PAUSE;
			//exit(-1);
			return nullptr;
		}
		pos.push_back(m_builder->getInt32(index));
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
			if (num->getType()->isIntegerTy(8) && prsnt_sign == false)//��8λ�±���չ��32λ����ֹ255���-1ʹ��
				num = m_builder->CreateZExt(num, m_builder->getInt32Ty());

			if (value->getType()->getContainedType(0)->isPointerTy()) { //ָ�뵱�������õ����
				Value *v = m_builder->CreateLoad(value, true);
				value = m_builder->CreateInBoundsGEP(v, num);
			}
			else { //��������
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
* �����ԼӲ���ת�ɶ�Ӧ��IR����
* @param ������ı���
* @return void
*/
void IR::__AddOne2IR(Value *alloc)
{
	LoadInst *load = m_builder->CreateLoad(alloc, true);
	Value *One = m_builder->getInt32(1);
	Value *result = m_builder->CreateNSWAdd(load, One, "inc");
	StoreInst *store = m_builder->CreateStore(result, alloc, false);
}

//��һ����ַ�е����ݶ�ȡ���������ݵ�ַ������Ϊ�������ָ�룬�в�ͬ�Ķ�ȡ����
Value *IR::__Load2IR(Value *alloc)
{
	Type *type = alloc->getType();
	Type *ctype = type->getContainedType(0);

	if (type->isPointerTy() && ctype->isArrayTy()){ //ָ������Ķ�ȡ��ʽ��ͬ
		vector<Value*> subScript;
		subScript.push_back(m_builder->getInt32(0));
		subScript.push_back(m_builder->getInt32(0));
		return m_builder->CreateInBoundsGEP(alloc, subScript);
	}
	else{
		return m_builder->CreateLoad(alloc, true);
	}
}

void IR::__While2IR(CSyntaxNode* pTree)
{
	static unsigned int whilenum = 0;//while�ı�ţ���ģ��ʹ��
	++whilenum;

	int tempWhileNum = whilenum;
	Function *TheFunction = m_builder->GetInsertBlock()->getParent();
	BasicBlock *WhileConBB = BasicBlock::Create(m_module->getContext(), "whileCond", TheFunction);
	m_builder->CreateBr(WhileConBB);
	m_builder->SetInsertPoint(WhileConBB);

	//����������ת
	Value *v = __Cond2IR(pTree->GetChild0());
	BasicBlock *WhileBodyBB = BasicBlock::Create(m_module->getContext(), "whileBody", TheFunction);
	BasicBlock *WhileEndBB = BasicBlock::Create(m_module->getContext(), "whilEnd", TheFunction);
	m_builder->CreateCondBr(v, WhileBodyBB, WhileEndBB);

	m_builder->SetInsertPoint(WhileBodyBB);
	if (Setting::IsModeling() && !hasOr && TheFunction->getName().substr(0, 2) != "$$"){
		m_builder->CreateCall(m_module->getFunction("$$GetVarsValue"));
		__GetLocalVar(pTree);
		string name = ("$$" + TheFunction->getName() + "_num").str();
		Function *f = m_module->getFunction("whileBegin");
		GlobalValue *g = m_module->getGlobalVariable(name);
		vector<Value*> args;
		args.push_back(m_builder->CreateLoad(g));
		args.push_back(m_builder->getInt32(tempWhileNum));
		m_builder->CreateCall(f, args);
	}
	if (pTree->GetChild1() != nullptr){
		Stmt2IR(pTree->GetChild1());
	}
	m_builder->CreateBr(WhileConBB);

	m_builder->SetInsertPoint(WhileEndBB);
	if (Setting::IsModeling() && !hasOr && TheFunction->getName().substr(0, 2) != "$$"){
		Function *f = m_module->getFunction("whileEnd");
		GlobalValue *g = m_module->getGlobalVariable(("$$" + TheFunction->getName() + "_num").str());
		vector<Value*> args;
		args.push_back(m_builder->CreateLoad(g));
		args.push_back(m_builder->getInt32(tempWhileNum));
		m_builder->CreateCall(f, args);
	}
}

void IR::__If2IR(CSyntaxNode *pTree)
{
	Function *TheFunction = m_builder->GetInsertBlock()->getParent();

	BasicBlock *ThenBB = BasicBlock::Create(m_module->getContext(), "then", TheFunction);
	//m_builder->SetInsertPoint(entry);

	//������ת
	Value * v = __Cond2IR(pTree->GetChild0());
	if (pTree->GetChild2() != nullptr)
	{
		BasicBlock *ElseBB = BasicBlock::Create(m_module->getContext(), "else", TheFunction);
		BasicBlock *IfEnd = BasicBlock::Create(m_module->getContext(), "ifend", TheFunction);
		m_builder->CreateCondBr(v, ThenBB, ElseBB);
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
		m_builder->CreateCondBr(v, ThenBB, IfEnd);
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
* �������תΪIR����
* @param ������������﷨��
* @return ת�����IR����
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
		value = __Cast2IR(value, m_Int32Ty, sign);
		if (value != nullptr)
			return m_builder->CreateICmpNE(value, m_builder->getInt32(0));
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

//��Ŀ�����
Value *IR::__ThreeOp2IR(CSyntaxNode* pTree)
{
	Value *cond = __Cond2IR(pTree->GetChild0());

	Function *TheFunction = m_builder->GetInsertBlock()->getParent();
	BasicBlock *ThenBB = BasicBlock::Create(m_module->getContext(), "then", TheFunction);
	BasicBlock *ElseBB = BasicBlock::Create(m_module->getContext(), "else", TheFunction);
	BasicBlock *IfEnd = BasicBlock::Create(m_module->getContext(), "ifend", TheFunction);

	m_builder->CreateCondBr(cond, ThenBB, ElseBB);

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

//����
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

//������
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

//����
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
			Value *v1 = m_builder->CreatePtrToInt(Left, m_builder->getInt32Ty());
			Value *v2 = m_builder->CreatePtrToInt(Right, m_builder->getInt32Ty());
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

//С��
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
			Value *v1 = m_builder->CreatePtrToInt(Left, m_builder->getInt32Ty());
			Value *v2 = m_builder->CreatePtrToInt(Right, m_builder->getInt32Ty());
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

//���ڵ���
Value* IR::__GE2IR(CSyntaxNode *pTree)
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
			Value *v1 = m_builder->CreatePtrToInt(Left, m_builder->getInt32Ty());
			Value *v2 = m_builder->CreatePtrToInt(Right, m_builder->getInt32Ty());
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

//С�ڵ���
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
			Value *v1 = m_builder->CreatePtrToInt(Left, m_builder->getInt32Ty());
			Value *v2 = m_builder->CreatePtrToInt(Right, m_builder->getInt32Ty());
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
		Left = __Cast2IR(Left, m_Int32Ty, false);
		if (Left == nullptr){
			cerr << "ERROR: invalid negative expression." << endl;
			return nullptr;
		}
	}

	Left = m_builder->CreateICmpNE(Left, m_builder->getInt32(0));
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
	m_builder->CreateCondBr(Left, rand, andend);
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
	m_builder->CreateCondBr(Left, orend, ror);
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
		return m_builder->getInt32(0);
	}

	Value *right = __Expr2IR(pTree->GetChild1(), true);
	bool rsign = prsnt_sign;

	if (right == nullptr){
		cerr << "ERROR: invalid XOR expression owing to right value." << endl;
		return m_builder->getInt32(0);
	}

	left = __Cast2IR(left, m_builder->getInt32Ty(), false);
	right = __Cast2IR(right, m_builder->getInt32Ty(), false);

	return m_builder->CreateXor(left, right);
}

Value* IR::__BNeg2IR(CSyntaxNode *pTree){
	Value *left = __Expr2IR(pTree->GetChild0(), true);
	if (left == nullptr){
		cerr << "ERROR: invalid binary negative expression." << endl;
		return m_builder->getInt32(0);
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
������ֵתΪ��Ӧ���͵ĵ�ַ
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
* �ӷ�����ת�ɶ�Ӧ��IR����
* @param ��������﷨��
* @return ת֮��Ľ��
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
* ��������ת�ɶ�Ӧ��IR����
* @param ��������﷨��
* @return ת֮��Ľ��
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
				Value *_Left = __Cast2IR(Left, m_Int32Ty, LSigned);
				Value *_Right = __Cast2IR(Right, m_Int32Ty, RSigned);
				
				//��ȡ����ָ��ĵ�ַ��ֵ
				Value* difference = m_builder->CreateSub(_Left, _Right);
				
				//��ȡָ��ָ�������
				Type* elementType = Left->getType()->getContainedType(0);
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
* �˷�����ת�ɶ�Ӧ��IR����
* @param ��������﷨��
* @return ת֮��Ľ��
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
* ��������ת�ɶ�Ӧ��IR����
* @param ��������﷨��
* @return ת֮��Ľ��
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
* ȡ�����ת�ɶ�Ӧ��IR����
* @param ��������﷨��
* @return ת֮��Ľ��
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
* ǿ��ת�����תΪIR����
* @param ������������﷨��
* @return ת�����IR����
*/
Value* IR::__Cast2IR(Value *value, Type *type, bool sign)
{
	Type *vType = value->getType(); auto vid = vType->getTypeID(); auto id = type->getTypeID();
	if (vType == type)
		return value;

	// float����ת��
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
	// double����ת��
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
	// int����ת��
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
	// ָ������ת��
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
	// ��������ת��
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

//�Լ�ʵ��sizeof
Value* IR::__IRSizeof(CSyntaxNode *pTree)
{
	if (pTree == nullptr){
		return m_builder->getInt32(0);
	}
	else if (pTree->GetChild0() == nullptr){
		// sizeof(basictype), ���磺 sizeof(int),sizeof(XXX) // XXX is a struct
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
		case FLOATTYPE:{
			return m_builder->getInt32(4);
		}
		case LONGTYPE:
		case ULONGTYPE:
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
		case FLOATLITERAL_EXP: {
			return m_builder->getInt32(4);
		}
		case POINTER_EXP:{     // sizeof(int*)
			return m_builder->getInt32(8);
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
				return m_builder->getInt32(8);
			}
			int size = m_dataLayout->getTypeAllocSize(type);
			return m_builder->getInt32(size);
		}
		default:
			cerr << "ERROR: sizeof: unknown type."<< endl;
			return m_builder->getInt32(0);
		}
	}
	else if (pTree->GetChild0() != nullptr && pTree->GetChild0()->GetChild0()->GetNType() == POINTER_EXP) {
		return m_builder->getInt32(8);
	}
	else{
		Value *v = __Expr2IR(pTree->GetChild0(), true);
		Type *type = v->getType();
		return m_builder->getInt32(CalculateSize(type));
	}
	cerr << "ERROR: sizeof: unknown type." << endl;
	return m_builder->getInt32(0);
}


/**
* �����������תΪIR����
* @param ������������﷨��
* @return ת�����IR����
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
	string func_name = __GetFunctionName(pTree, isWinApi); //��ȡ���ú�����

	bool loadsign = false;
	Value *func = __GetFunction(pTree, func_name, loadsign); //��ȡ���ú���, func's type is pointer to function.
	FunctionType *funcTy = (FunctionType*)func->getType()->getContainedType(0);
	vector<Value*> argsList;

	for (CSyntaxNode *paraNode = pTree->GetChild1(); paraNode != nullptr; paraNode = paraNode->GetChild1()){
		CSyntaxNode *currNode = paraNode->GetChild0();

		string paraName = currNode->GetNameNode() == nullptr ? "" : currNode->GetNameNode()->GetNName();

		if (paraName == "RValue"){ //ȥ��RValue
			continue;
		}

		argsList.push_back(__Expr2IR(currNode, true));
	}

	//�ж��βκ�ʵ�θ�������ϣ����ڷǿɱ��������
	if (funcTy->getNumParams() != argsList.size() && !funcTy->isVarArg()){

		if (funcTy->getNumParams() == argsList.size() + 1){
			argsList.insert(argsList.begin(), (Value*)m_builder->getInt32(1)); // extern call
		}

		if (funcTy->getNumParams() != argsList.size()){
			cerr << "ERROR: " << func_name << " : Formal argument size is " << funcTy->getNumParams() << " != Actual argument size is " << argsList.size() << endl;
		}
	}

	auto actual = argsList.begin();
	for (int j = 1; j < funcTy->getNumContainedTypes(); j++) {
		Type *fTy = funcTy->getContainedType(j);
		Type *aTy = (*actual)->getType();

		if (aTy != fTy) {
			*actual = __Cast2IR(*actual, fTy, true);
		}

		actual++;
	}

	//���캯������
	ArrayRef<Value*> Args(argsList);
	CallInst *call = m_builder->CreateCall(func, Args);

	if (isWinApi || g_ThreadFuncName.find(func_name) != g_ThreadFuncName.end())
		call->setCallingConv(CallingConv::X86_StdCall);
	else
		call->setCallingConv(CallingConv::C);

	return call;
}

string IR::__GetFunctionName(CSyntaxNode *pTree, bool &isWinAPI){
	string func_name = "";

	if (pTree == nullptr){
		cerr << "ERROR: unknown error." << endl;
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
		//func_name = it->second;
		isWinAPI = true;
	}
	//else if (func_name.find("$$") == 0){  //�����$$��ͷ�ĺ���,����ñ���д���ĺ���
		//func_name = func_name;
	//}

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
		PAUSE;
		exit(-1);
		return nullptr;
	}
}

/**
*���ݽṹ�嶨���ģ���declare���õ���Ӧ��llvm type
*@param module (�����ṹ���ģ��) pTree(�������)
*@return ��������Ӧ��llvm type
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
		prsnt_sign = true;
		result = Type::getInt32Ty(m_module->getContext());
		break;
	case UINTTYPE:
		prsnt_sign = false;
		result = Type::getInt32Ty(m_module->getContext());
		break;
	case LONGTYPE:
	case LLONGTYPE:
		prsnt_sign = true;
		result = Type::getInt64Ty(m_module->getContext());
		break;
	case ULONGTYPE:
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
	if (rtnTree == nullptr || rtnTree->GetChild0() == nullptr) //���Ϊ�գ�˵��û�з���ֵ������Ϊvoid
		returntype = Type::getVoidTy(m_module->getContext());	
	else
		returntype = __GetFuncParaInfo(rtnTree->GetChild0()).type;

	if (returntype == nullptr)
		returntype = Type::getVoidTy(m_module->getContext());

	//������ȡ
	vector<Type*> paraTypes;
	CSyntaxNode *paraTree = nullptr;
	if (rtnTree != nullptr)
		paraTree = rtnTree->GetChild1();
	while (paraTree != nullptr){
		VarInfo vi = __GetFuncParaInfo(paraTree);
		paraTypes.push_back(vi.type);
		paraTree = paraTree->GetChild1();
	}
	//�������ͻ�ȡ
	FunctionType *fTy = FunctionType::get(returntype, paraTypes, false);
	return fTy;
}

/**
* �����������תΪIR����
* @param ������������﷨��
* @return ת�����IR����
*/
void IR::func2IR(CSyntaxNode *pTree)
{
	if (pTree == nullptr){
		return; //��û�к������壬��ֱ���˳�
	}

	string func_name = pTree->GetNName(); //��ȡ��������

	IR::prsnt_sign = true; //����ֵ�ķ��ţ��ݶ�Ϊtrue.
	Type *rettype;
	bool rtn_sign;

	CSyntaxNode *rtnNode = pTree->GetChild0()->GetChild0();
	if (rtnNode != nullptr){ //�жϷ���ֵ�����Ƿ���
		rettype = __GetFuncParaInfo(rtnNode).type;
		rtn_sign = IR::prsnt_sign;
	}
	else{
		rettype = Type::getVoidTy(m_module->getContext()); //����ֵ���ͳ�ʼ��Ϊvoid
		rtn_sign = true;
	}

	std::vector<Type*> vectype;  //����-����
	std::vector<string> vecname; //����-��
	std::vector<bool> vecsign;   //����-����

	CSyntaxNode *paraNode = pTree->GetChild0()->GetChild1();
	while (paraNode != nullptr){
		VarInfo vInfo = __GetFuncParaInfo(paraNode);
		vectype.push_back(vInfo.type);
		vecname.push_back(vInfo.name);
		vecsign.push_back(prsnt_sign);

		paraNode = paraNode->GetChild1();
	}

	FunctionType *fTy = FunctionType::get(rettype, vectype, false); //��������
	Function *Func = Function::Create(fTy, Function::ExternalLinkage, func_name, m_module); //��������

	if (g_ThreadFuncName.find(func_name) != g_ThreadFuncName.end()){
		Func->setCallingConv(CallingConv::X86_StdCall);
	}
	else{
		Func->setCallingConv(CallingConv::C); //���÷�ʽ,һ��Ϊccc: C Calling Convention
	}

	IRSymbol *symbol = new IRSymbol(Func, true);
	if (m_GlobalSTable.insert({ func_name, symbol }).second == false){
		cerr << "ERROR: variable " << func_name << " has already been defined." << endl;
	}
	//�洢ȫ��״̬
	BasicBlock *fatherinsertpoint = m_builder->GetInsertBlock();

	map<string, IRSymbol*> fatherTable = m_IRSTable;
	m_IRSTable.clear(); //�޸ķ��ű�������

	BasicBlock *tempREnd = rightEnd;

	//��Ӻ�����
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

			temparg->setName(tempname); //�����б���ϱ�����
			IRSymbol *symbol;
			if (!arg_it->hasByValAttr()){ //Ϊ�����б�����ڴ�
				AllocaInst *allocDeclr = m_builder->CreateAlloca(temptype);
				allocDeclr->setName(tempname + ".addr");
				allocDeclr->setAlignment(CalculateAlign(temptype));

				StoreInst *store = m_builder->CreateStore(temparg, allocDeclr, false); //�洢����
				symbol = new IRSymbol(allocDeclr, sign); //��������ű�sign
			}
			else{
				symbol = new IRSymbol(((AllocaInst*)temparg), sign); // byval�Ľṹ��ָ�벻��Ҫ���·���ռ�
			}
			arg_it++;
			if (m_IRSTable.insert({ tempname, symbol }).second == false){
				cerr << "ERROR: variable " << tempname << " has already been defined." << endl;
			}
		}
		//������ڽ�ģ�У����ݺ���������ȫ�ֱ��� unsigned int $$������_num �����ں�����ʼʱ��1����������ʱ��1 
		Value *temp = nullptr;
		if (Setting::IsModeling() && func_name[0] != '$' && func_name[1] != '$'){
			string tempName = "$$" + func_name + "_num";

			temp = new GlobalVariable(*m_module, m_Int32Ty, false, GlobalValue::ExternalLinkage, m_builder->getInt32(0), tempName);

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

void IR::__MyCreateThread(CSyntaxNode *pTree)
{
	Function *CalleeF = m_module->getFunction("MyCreateThread");
	Function::arg_iterator args = CalleeF->arg_begin();
	if (pTree->GetNName() == "ManagerThread"){
		Value *arg1 = ConstantPointerNull::get((PointerType*)args->getType()); ++args;
		Value *arg2 = ConstantPointerNull::get((PointerType*)args->getType());
		Value *arg3 = m_builder->getInt32(pTree->GetiValue());
		vector<Value*> args;
		args.push_back(arg1);
		args.push_back(arg2);
		args.push_back(arg3);
		m_builder->CreateCall(CalleeF, args);
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

		vector<Value*> args;
		args.push_back(arg1);
		args.push_back(arg2);
		args.push_back(arg3);
		m_builder->CreateCall(CalleeF, args);
	}
}

void IR::__StructTree2IR(CSyntaxNode *sTree)
{
	vector<CSyntaxNode*> tree;
	vector<string> name;
	//�����﷨��
	while (sTree != nullptr){
		name.push_back(sTree->GetNName());
		tree.push_back(sTree->GetChild0());
		sTree = sTree->GetChild1();
	}
	//���ɶ���
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
				s->CalculateSize(); //���㳤��
			}
			
			if (m_IRStruct.insert({ s->GetStructName(), s }).second == false){
				cerr << "ERROR: struct " << *nit << " has already been defined." << endl;
			}
		}
		++nit;
		++tit;
	}
}

//������ļ�,����extern_tree
void IR::__FuncFile(CSyntaxTree *m_GlbVarTree, CSyntaxNode *function_tree)
{
	vector<string> name;
	Module::FunctionListType &funcList = m_module->getFunctionList();
	for (auto it = funcList.begin(); it != funcList.end(); ++it){
		if (!it->isDeclaration())
			name.push_back(it->getName().str());
	}
	//ɾ������
	for (int i = 0; i < name.size(); ++i){
		Function *f = m_module->getFunction(name.at(i));
		funcList.erase(f);
	}

	Function::Create(FunctionType::get(m_builder->getVoidTy(), ArrayRef<Type*>(m_Int32Ty), false), Function::ExternalLinkage, "$$changeState", m_module);
	if (Setting::isOutput()){
		Function::Create(FunctionType::get(m_builder->getVoidTy(), vector<Type*>(0), false), Function::ExternalLinkage, "$$GlobalOutput", m_module);
		Function::Create(FunctionType::get(m_builder->getInt32Ty(), vector<Type*>(0), false), Function::ExternalLinkage, "$$IsOutput", m_module);
	}

	if (Setting::IsModeling()){
		Function::Create(FunctionType::get(m_builder->getVoidTy(), vector<Type*>(1, m_builder->getInt32Ty()), false), Function::ExternalLinkage, "$$addInt", m_module);
		Function::Create(FunctionType::get(m_builder->getVoidTy(), vector<Type*>(1, m_builder->getInt8Ty()), false), Function::ExternalLinkage, "$$addChar", m_module);
		Function::Create(FunctionType::get(m_builder->getVoidTy(), vector<Type*>(1, m_builder->getFloatTy()), false), Function::ExternalLinkage, "$$addFloat", m_module);
		Function::Create(FunctionType::get(m_builder->getVoidTy(), vector<Type*>(1, m_builder->getInt8PtrTy()), false), Function::ExternalLinkage, "$$addString", m_module);
		Function::Create(FunctionType::get(m_builder->getVoidTy(), vector<Type*>(0), false), Function::ExternalLinkage, "$$GetVarsValue", m_module);
	}

	Module::GlobalListType &globalList = m_module->getGlobalList();

	GlobalVariable *v = m_module->getGlobalVariable("$$myJmpbuf");
	Type *type = v->getType();
	globalList.erase(v);
	new GlobalVariable(*m_module, type->getContainedType(0), false, GlobalValue::ExternalLinkage, nullptr, "$$myJmpbuf");

	v = m_module->getGlobalVariable("$$varsInfo");
	type = v->getType();
	globalList.erase(v);
	new GlobalVariable(*m_module, type->getContainedType(0), false, GlobalValue::ExternalLinkage, nullptr, "$$varsInfo");

	v = m_module->getGlobalVariable("$$tempLen");
	type = v->getType();
	globalList.erase(v);
	new GlobalVariable(*m_module, type->getContainedType(0), false, GlobalValue::ExternalLinkage, nullptr, "$$tempLen");

	//extern int $$state_num
	m_StNum = new GlobalVariable(*m_module, m_Int32Ty, false, GlobalValue::ExternalLinkage, nullptr, "$$state_num");

	IRSymbol *symbol = new IRSymbol(m_StNum, true);
	if (m_GlobalSTable.insert({ "$$state_num", symbol }).second == false){
		cerr << "ERROR: variable $$state_num has already been defined" << endl;
	}

	for (auto it = globalList.begin(); it != globalList.end(); ++it){
		if (it->getName().str() == "start_time" || it->getName().str() == "j_buf")
			it->setInitializer(nullptr);
	}
	__ExTree2IR(extern_tree);
	__StructTree2IR(struct_tree);
	Global2IR(m_GlbVarTree->GetRoot());
	m_InFunc = true;
	TraverseFuncTree(function_tree);
	m_InFunc = false;
}

//����extern_tree
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

//�첽ͨ����ʹ�ã��������﷨��������Ԫ�ظ�����һ,��Ϊ��һ��Ԫ������������
bool IR::__AsyncSubscript(CSyntaxNode *pTree)
{
	//ֻ����Chn���ͽṹ����±�
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

//�첽ͨ����ʹ�ã������������õ���Ԫ�ظ�ֵ,�ṹ���һ��Ԫ�ش泤��,�ڶ���Ԫ��Ϊ0
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
