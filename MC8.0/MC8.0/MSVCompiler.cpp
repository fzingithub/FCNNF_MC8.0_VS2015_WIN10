#include "MSVCompiler.h"
#include "function.h"
#include "Setting.h"
#include "mctypes.h"

#include <algorithm>
#include <sstream>
#include <windows.h>

string g_error = "";

extern CSyntaxNode *g_syntaxTree;
extern CSyntaxNode *struct_tree;
extern CSyntaxNode *function_tree;
extern CSyntaxNode *propositionDefineTree;
extern set<string> functionName;

extern bool g_InFunc;
extern bool g_InNewFunc;
extern bool g_InStruct;

CSyntaxNode *g_CopyFuncTree;
bool g_OutputState;
bool g_HasCreatedManager = false;
bool g_HasCreatedWaitFor = false;
set<string> g_ThreadFuncName;
set<string> g_ProcessedFuncName;//在处理并行时存储已经处理过的函数名
int g_MaxSize = 1000;//always和keep语句中用到的队列的长度
int g_HasErr = 0;


CMSV::CMSV()
{
	g_OutputState = Setting::isOutput();
	m_Program = new CSyntaxTree(g_syntaxTree);
	m_PrgmFuture = new CSyntaxTree();
	m_IRTree = new CSyntaxTree();
	m_GlbVarTree = new CSyntaxTree();
	m_LocVarTree = new CSyntaxTree();
	m_FuncDecTree = new CSyntaxTree();
	m_FrmSymbl = new SymbolTable();
	m_NFrmSymbl = new SymbolTable();
	m_GlbSymbl = new SymbolTable();
	struct_map = map<string, SymbolTable*>();
	m_More = -1;
	m_Done = -1;
	m_RdcOk = -1;
	m_FreeFrm = 0;
    m_LenNum = 0;
	m_ForNum = 0;
	m_NewStructNum = 0;
	m_NewFuncNum = 0;
	m_IsComplexStmt = false;
	m_CurrentFunc = "";
	m_InWhile = false;
	m_StatementNum = 0;
	m_InFunc = 0;
}

CMSV::~CMSV()
{
	if (m_Program != nullptr)
	{
		delete m_Program;
		m_Program = nullptr;
	}
	if (m_PrgmFuture != nullptr)
	{
		delete m_PrgmFuture;
		m_PrgmFuture = nullptr;
	}
	if (m_IRTree != nullptr)
	{
		delete m_IRTree;
		m_IRTree = nullptr;
	}

	for (auto iter = m_PrgmPrsnt.begin(); iter != m_PrgmPrsnt.end(); iter++){
		delete iter->node;
	}

	m_PrgmPrsnt.clear();
}

//2015-4-13,add by yubin,将m_Program设置成pTree,默认的参数，flag=0表示将m_IRTree置为空，flag=1表示不修改m_IRTree
void CMSV::SetProgram(CSyntaxNode* pTree, int flag)
{
	m_Program->SetRoot(pTree);
	m_More = -1;//将m_More置为1，因为pre函数一开始就会判断m_More是否为0
	if (flag == 0)
	{
		m_IRTree = new CSyntaxTree();
	}
}

///Preprocess the tree
///2015-3-9 add by wangmeng
void CMSV::Pre()
{
	int i = 0;
	while (m_More != 0 && m_Program->GetRoot() != nullptr){
		m_More = -1;
		m_RdcOk = -1;
		m_IsComplexStmt = false;
		//add by yubin 2015/4/23,先判断类型，如果是内部函数调用，将m_FuncFlag置为1，表示函数调用之后不输出状态
		if (m_Program->GetRoot()->GetNType() != FUNCTION_STA)
			m_FuncFlag = 0;
		else
			m_FuncFlag = 1;

		GetPresent(m_Program->GetRoot());

		if (m_RdcOk == 0){
			if (g_error == ""){
				if (!m_PrgmPrsnt.empty())
					PrintErrorMassage(0, "__Pre():" + (*m_PrgmPrsnt.begin()).node->TreeToString());
				else
					PrintErrorMassage(0, "__Pre() error");
			}
			else{
				g_error += "__Pre() error:";
				if (!m_PrgmPrsnt.empty()){
					g_error += (*m_PrgmPrsnt.begin()).node->TreeToString().data();
				}
				g_error += "\n";
			}

			if (m_IRTree != nullptr){
				PrintIRTree();
			}
			return;
		}

        __SortPrsnt();
		__AdPrst2IR(); ///将当前状态加入到IR树中

		///2015/4/23 modified by yubin,m_FuncFlag!=2时才输出状态,内部函数调用之后，m_FuncFlag=2，第一个状态不输出
		if (Setting::IsSimulation())
		{
			if (m_FuncFlag != 1 && m_PreFlag != 1)
			{
				if (m_InFunc && m_More == 0)
				{
					//do nothing
				}
				else if (m_More==1 && !g_InNewFunc && m_CurrentFunc != "$$changeState")//在复杂的并行,and,投影，always，keep中不添加输出，由调度程序自动输出
				{
					__OutputState();
				}
			}
		}
		else if (Setting::IsModeling())
		{
			if (m_FuncFlag != 1 && m_PreFlag != 1)
			{
				if (m_InFunc && m_More == 0)
				{
					//do nothing
				}
				else if (m_More == 1 && !g_InNewFunc && m_CurrentFunc != "$$changeState")//在复杂的并行,and,投影，always，keep中不添加输出，由调度程序自动输出
				{
					__AddEdge(0);
				}
			}
		}
		delete m_Program->GetRoot();
		m_Program->SetRoot(m_PrgmFuture->GetRoot());
		m_PrgmFuture->SetRoot(nullptr);
		i++;
	}
}
//-----------------------------------------------------------------------------
void CMSV::__AddEdge(int pIsEmpty)
{	
	vector<string> SymTbl;
	map<string, Symbol*>::iterator iter;
	map<string, Symbol*>* Smap = m_NFrmSymbl->GetSymbolMap();
	map<string, Symbol*>* FSmap = m_FrmSymbl->GetSymbolMap();
	map<string, Symbol*>* GSmap = m_GlbSymbl->GetSymbolMap();
	string print = "";
	//函数调用时根据是内部还是外部调用判断是否需要输出每个状态
	if (m_currFunc != "")
	{
		//将$$m_currFunc压入SymTbl，输出的时候，可以输出函数名
		SymTbl.push_back("##" + m_currFunc);
	}
	for (iter = Smap->begin(); iter != Smap->end(); ++iter)
	{
		if (!(*(iter->first.c_str()) == '$' && *(iter->first.c_str() + 1) == '$'))
		{
			print += iter->second->PrntSym(iter->first);
			SymTbl.push_back(iter->first);
		}
	}
	for (iter = FSmap->begin(); iter != FSmap->end(); ++iter)
	{
		if (!(*(iter->first.c_str()) == '$' && *(iter->first.c_str() + 1) == '$'))
		{
			print += iter->second->PrntSym(iter->first);
			if (iter->second->GetSymbolType() != UNDEFINE)
			{
				SymTbl.push_back(iter->first);
			}
		}
	}
	
	CSyntaxNode* generateEdge = new CSyntaxNode(GENERATE_EDGE_STA, VOIDTYPE, print, SymTbl);
	generateEdge->SetcValue(pIsEmpty);
	generateEdge->SetiValue(m_StatementNum);
	m_StatementNum++;
	m_IRTree->MergeTree(generateEdge, CHOP_STA, VOIDTYPE);
}
//-----------------------------------------------------------------------------
void CMSV::GetPresent(CSyntaxNode*pTree)
{
	if (pTree == nullptr)
		return;

	switch (pTree->GetNType()){
	case DECLARE_STA:    {__Declr(pTree); break; }  /// handle declaration: int x, y<==x
	case ASS_EQU_EXP:  	                           /// handle x<==e
	case EX_ASS_EXP:     {__Ass(pTree);   break; }  /// handle x<==e obtained by x:=e
	case UNITASSIGN_EXP: {__UAss(pTree);  break; }  /// handle x:=e
	case EMPTY_EXP:      {__Emp(pTree);   break; }  /// handle empty
	case MORE_STA:       {__More(pTree);  break; }  /// handle more //yangkai
	case TRUE_EXP:       {__True(pTree);  break; }  /// handle true //yangkai
	case SKIP_STA:       {__Skip(pTree);  break; }  /// handle skip
	case LENGTH_STA:     {__Len(pTree);  break; }  /// handle len(n)
	case NEXT_STA:       {__Next(pTree);  break; }  /// handle next p
	case AND_EXP:        {__And(pTree);  break; }  /// handle p and q
	case PARALLEL_STA:	 {__Pal(pTree);  break; }  /// handle p||q
	case PROJECTION_STA: {__Prj(pTree);  m_More = 0; break;  }  /// handle {p1,p2,...}prj{q}
	case CHOP_STA:       {__Chop(pTree);  break; }  /// handle p;q
	case IF_ELSE_STA:    {__If(pTree); break; }  /// handle if(b)then{p}else{q}
	case WHILE_DO_STA:   {__While(pTree); break; }  /// handle while(b)do{p}
	case FOR_STA:        {__For(pTree);   break; }  /// handle for(p1,b,p2){q}
	case FOR_TIMES_STA:  {__ExFor(pTree); break; }
	case FRAME_STA:      {__Frm(pTree);   break; }  /// frame(c)
	case UNFRAME_STA:    {__UnFrm(pTree); break; }  /// frame(c)
	case OR_EXP:         {__Or(pTree);    break; }  /// p or q
	case EXIST_STA:      {__Ext(pTree);   break; }  ///exists(x,y,...):p
	case FUNCTION_DEFINE_STA:     {__FuncDef(pTree); break; }//2015-3-17,add by yubin,函数定义
	case FUNCTION_STA:     {__FuncCall(pTree); break; }///2015-3-17,add by yubin,内部函数调用
	case EXT_FUNCTION_STA:  {__EFuncCall(pTree->GetChild0()); break; }//2015-3-17,add by yubin,外部函数调用
	case AWAIT_STA:      {__Await(pTree);  break; }  ///await(b)
	case FINAL_STA:      {__Fin(pTree);  break; }  ///fin(p)
	case ALWAYS_STA:     {__Alw(pTree); break; }  ///alw(p)
	case KEEP_STA:       {__Keep(pTree); break; }  ///keep(p)
	case RETURN_STA:     {__Return(pTree);  break; }  ///2015-4-13,add by yubin,增加处理return语句的函数
	case REQUEST_STA:    {__Input(pTree); break; }
	case DISPLAY_STA:    {__Output(pTree); break; }
	default: break;
	}
}
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
CSyntaxNode* CMSV::__SetNodeLength(int pFlag)
{
	CSyntaxNode* setNodeLenght = new CSyntaxNode(FUNCTION_STA, VOIDTYPE);
	setNodeLenght->SetChild0(new CSyntaxNode(IDENT_EXP, SETNODELENGTH, nullptr, VOIDTYPE));
	setNodeLenght->SetChild1(new CSyntaxNode(OPTION_EXP, VOIDTYPE));
	setNodeLenght->GetChild1()->SetChild0(new CSyntaxNode(INTEGER_EXP, pFlag, INTTYPE));
	return  setNodeLenght;
}
//-----------------------------------------------------------------------------
void CMSV::__FuncPtrDeclr(CSyntaxNode* pTree)
{
	//分析得到函数返回值类型
	CSyntaxNode* temp = pTree;
	pTree = pTree->GetChild0();
	bool existRValue = false;
	CSyntaxNode *tmpExtNode = new CSyntaxNode(FORMAL_PARAMETER_EXP, new CSyntaxNode(IDENT_EXP, "$$Ext", nullptr, VOIDTYPE), INTTYPE);
	tmpExtNode->SetChild1(pTree->GetChild1());
	pTree->SetChild1(tmpExtNode);//将$$Ext变量添加到形参列表的最开始

	CSyntaxNode *paraList = pTree->GetChild1();//获得函数的形参列表
	while (paraList->GetChild1() != nullptr)
	{
		//add by daichunchun 2015/6/5 三层函数指针
		if (paraList->GetChild1()->GetRType() == FUNCPTYPE)//如果是函数指针的话，不用加入符号表，直接处理
		{
			__FuncDefParaFuncP(paraList->GetChild1());//__FuncDefParaFuncP和__FuncDefPara函数的区别是，前者对出现的变量不用加入符号表
		}
		//end add
		if (__isRValue(paraList->GetChild1()))
		{
			if (paraList->GetChild1()->GetChild1() == nullptr)
			{
				CSyntaxNode* retNode = new CSyntaxNode(FUNC_RETURN_TYPE, paraList->GetChild1(), VOIDTYPE);
				paraList->SetChild1(nullptr);
				retNode->SetChild1(pTree->GetChild1());
				pTree->SetChild1(retNode);
				existRValue = 1;
				break;
			}
			else
			{
				PrintErrorMassage(0, "In function " + pTree->GetNName() + " definition, RValue must be the last parameter");
				return;
			}
		}

		paraList = paraList->GetChild1();

	}
	if (existRValue == false)
	{
		CSyntaxNode* retNode = new CSyntaxNode(FUNC_RETURN_TYPE, VOIDTYPE);
		retNode->SetChild1(pTree->GetChild1());
		pTree->SetChild1(retNode);
	}

	int nValue;
	CSyntaxNode* nameNode = pTree->GetNameNode(nValue);
	string varName = nameNode->GetNName();

	pTree = temp;

	//函数指针或函数指针的指针
	if (!g_InStruct){
		m_FuncPNameVec.push_back(varName);//将函数指针名字存起来，用于if和while条件中使用时能判断出来



		if (!m_InFunc)//yangkai 2017-6-1  函数中的局部变量不能在语法树中添加两次
		{
			//将函数指针变量存入符号表(不存参数部分)
			CSyntaxNode* FuncPtrVar = new CSyntaxNode();
			FuncPtrVar->CopyNode(pTree);
			FuncPtrVar->SetChild0(new CSyntaxNode());
			FuncPtrVar->GetChild0()->CopyTree(pTree->GetChild0()->GetChild0());
			__InsrtAss(FuncPtrVar);
		}

		//将函数指针的声明添加到全局变量或局部变量声明树上
		CSyntaxNode* FuncPtrDec = new CSyntaxNode();
		FuncPtrDec->CopyNode(pTree);
		FuncPtrDec->SetChild0(pTree->GetChild0());
		FuncPtrDec->SetChild1(pTree->GetChild1());
		pTree->SetChild0(nullptr);
		pTree->SetChild1(nullptr);

		if (g_InFunc)
		{
			m_LocVarTree->MergeTree(FuncPtrDec, CHOP_STA, VOIDTYPE);
		}
		else if (m_GlbSymbl->HasSymbol(varName))
		{
			m_GlbVarTree->MergeTree(FuncPtrDec, CHOP_STA, VOIDTYPE);
		}

		m_RdcOk = 1;
	}
	return;
}
//-----------------------------------------------------------------------------
void CMSV::__Declr(CSyntaxNode* pTree)
{
	if (pTree->GetRType() == FUNCPTYPE)
	{
		//处理函数指针声明
		__FuncPtrDeclr(pTree);
		return;
	}
	else if (pTree->GetChild0() == nullptr || pTree->GetChild0()->GetChild0() == nullptr)
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED);
		cout << "Declaration error: " << pTree->TreeToString() << endl;
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY);
		g_HasErr++;
		m_RdcOk = 0;
		return;
	}

	CSyntaxNode* firstvisit = pTree;
	CSyntaxNode* previsit = pTree;
	CSyntaxNode*visit = pTree->GetChild0();

	CSyntaxNode* temp = nullptr;
	while (visit != nullptr)
	{
		CSyntaxNode *store = visit;
		//2015-04-17 处理int *y<==&x 类赋值
		while (visit != nullptr && visit->GetChild0() != nullptr && visit->GetChild0()->GetNType() == POINTER_EXP)
		{
			visit = visit->GetChild0();
		}

		if (visit->GetChild0()->GetNType() == ASS_EQU_EXP) ///int x<==y分成两部分int x和x<==y分别加入链表中
		{
			///处理int a[] <== {1,2}类的数组初始化
			CSyntaxNode* temp_vist = visit->GetChild0()->GetChild0();
			if (temp_vist->GetNType() == ARRAY_DECLARE_STA && temp_vist->GetChild1() == nullptr)
			{
				string NName = visit->GetChild0()->GetChild0()->GetChild0()->GetNName();
				CSyntaxNode *node = visit->GetChild0()->GetChild1()->GetChild0();
				int num = 1;

				while (node->GetChild0() != nullptr)
				{
					if (node->GetChild1() == nullptr)
						break;

					if (node->GetChild1()->GetNType() == ELEMENT ||
						node->GetChild1()->GetNType() == ELEMENT_LIST)
					{
						num++;
						node = node->GetChild1();
					}
				}

				///temp: a[3]
				temp = new CSyntaxNode(ARRAY_DECLARE_STA, LISTTYPE);
				//temp->SetNName(NName);
				temp->SetChild0(new CSyntaxNode(IDENT_EXP, NName, nullptr, VOIDTYPE));
				temp->SetChild1(new CSyntaxNode(OPTION_EXP, VOIDTYPE));
				temp->GetChild1()->SetChild0(new CSyntaxNode(INTEGER_EXP, num, INTTYPE));

				delete visit->GetChild0()->GetChild0();
				visit->GetChild0()->SetChild0(temp);

				///ArrayDec  int a[3]<==[3,4,5]
				CSyntaxNode* ArrayDec = new CSyntaxNode();
				ArrayDec->CopyNode(pTree);

				ArrayDec->SetChild0(new CSyntaxNode(DEC_VAR_LIST, VOIDTYPE));
				ArrayDec->GetChild0()->SetChild0(new CSyntaxNode());
				ArrayDec->GetChild0()->GetChild0()->CopyTree(store->GetChild0());

				__InsrtAss(visit->GetChild0());

				temp = ArrayDec;
				while (temp->GetChild0() != nullptr)
				{
					if (temp->GetChild0()->GetNType() == ASS_EQU_EXP)
					{
						CSyntaxNode *decl = new CSyntaxNode();
						decl->CopyTree(visit->GetChild0()->GetChild0());
						temp->SetChild0(decl);
						break;
					}
					temp = temp->GetChild0();
				}
				store->SetChild0(nullptr);
				__InsrtAss(ArrayDec);

				///2015-4-22 add by wangmeng 加入全局变量
				if (!g_InFunc&&m_GlbSymbl->HasSymbol(NName))
				{
					if (!m_GlbVarTree->MergeTree(ArrayDec, CHOP_STA))
					{
						PrintErrorMassage(0, "__Declr merge tree error!");
						m_RdcOk = 0;
						return;
					}
				}
				else
				{
					if (!m_LocVarTree->MergeTree(ArrayDec, CHOP_STA))
					{
						PrintErrorMassage(0, "__Declr merge tree error!");
						m_RdcOk = 0;
						return;
					}

				}
				///end by wangmeng

				if (previsit == firstvisit)
				{
					previsit->SetChild0(store->GetChild1());
					store->SetChild1(nullptr);
					delete store;
					visit = previsit->GetChild0();
					continue;
				}
				else
				{
					previsit->SetChild1(store->GetChild1());
					store->SetChild1(nullptr);
					delete store;
					visit = previsit->GetChild1();
					continue;
				}

			}
			///处理int a[2] <== {1,2}类的数组初始化
			else if (visit->GetChild0()->GetChild0()->GetNType() == ARRAY_DECLARE_STA)
			{
				string NName = visit->GetChild0()->GetChild0()->GetChild0()->GetNName();
				///ArrayDec  int a[2]<==[1,2]
				CSyntaxNode* ArrayDec = new CSyntaxNode();
				ArrayDec->CopyNode(pTree);

				ArrayDec->SetChild0(new CSyntaxNode(DEC_VAR_LIST, VOIDTYPE));
				ArrayDec->GetChild0()->SetChild0(new CSyntaxNode());
				ArrayDec->GetChild0()->GetChild0()->CopyTree(store->GetChild0());

				__InsrtAss(visit->GetChild0());

				temp = ArrayDec;
				while (temp->GetChild0() != nullptr)
				{
					if (temp->GetChild0()->GetNType() == ASS_EQU_EXP)
					{
						CSyntaxNode *decl = new CSyntaxNode();
						decl->CopyTree(visit->GetChild0()->GetChild0());
						temp->SetChild0(decl);
						break;
					}
					temp = temp->GetChild0();
				}
				store->SetChild0(nullptr);
				__InsrtAss(ArrayDec);
				///2015-4-22 add by wangmeng 加入全局变量
				if (!g_InFunc&&m_GlbSymbl->HasSymbol(NName))
				{
					if (!m_GlbVarTree->MergeTree(ArrayDec, CHOP_STA))
					{
						PrintErrorMassage(0, "__Declr merge tree error!");
						m_RdcOk = 0;
						return;
					}
				}
				else
				{
					if (!m_LocVarTree->MergeTree(ArrayDec, CHOP_STA))
					{
						PrintErrorMassage(0, "__Declr merge tree error!");
						m_RdcOk = 0;
						return;
					}
				}
				if (previsit == firstvisit)
				{
					previsit->SetChild0(store->GetChild1());
					store->SetChild1(nullptr);
					delete store;
					visit = previsit->GetChild0();
					continue;
				}
				else
				{
					previsit->SetChild1(store->GetChild1());
					store->SetChild1(nullptr);
					delete store;
					visit = previsit->GetChild1();
					continue;
				}
			}
			else
			{
				temp = new CSyntaxNode();
				temp->CopyTree(visit->GetChild0()->GetChild0());

				///2015-4-17 add by wangmeng 右边包含函数调用的情况处理
				__AssFunc(visit->GetChild0()->GetChild1(), true);

				if (m_RdcOk == 0)
				{
					return;
				}

				__InsrtAss(visit->GetChild0()); ///插入赋值语句 x<==y 

				visit->SetChild0(temp);
				temp = nullptr;
			}
		}
		previsit = store;
		visit = store->GetChild1();
	}

	if (firstvisit->GetChild0() != nullptr)
	{
		CSyntaxNode* DecTree = new CSyntaxNode();
		DecTree->CopyNode(pTree);
		DecTree->SetChild0(pTree->GetChild0());
		pTree->SetChild0(nullptr);

		if (g_InFunc)
		{
			__InsrtAss(DecTree); ///int x,z 插入声明语句
			if (!m_LocVarTree->MergeTree(DecTree, CHOP_STA, VOIDTYPE))
			{
				PrintErrorMassage(0, "__Declr merge tree error 3!");
				m_RdcOk = 0;
				return;
			}
		}
		else
		{
			firstvisit = DecTree;
			previsit = DecTree;
			visit = DecTree->GetChild0();

			////遍历变量声明树，将全局变量加入全局变量树上
			CSyntaxNode *GlbVarTree = nullptr;
			CSyntaxNode *vstGlb = nullptr;
			while (visit != nullptr)
			{
				int nValue = 0;
				if (m_GlbSymbl->HasSymbol(visit->GetNameNode(nValue)->GetNName()))
				{
					if (GlbVarTree == nullptr)
					{
						GlbVarTree = new CSyntaxNode();
						GlbVarTree->CopyNode(DecTree);
						vstGlb = GlbVarTree;

						vstGlb->SetChild0(new CSyntaxNode(DEC_VAR_LIST, VOIDTYPE));
						vstGlb = vstGlb->GetChild0();
					}
					else
					{
						vstGlb->SetChild1(new CSyntaxNode(DEC_VAR_LIST, VOIDTYPE));
						vstGlb = vstGlb->GetChild1();
					}

					vstGlb->SetChild0(visit->GetChild0());

					visit->SetChild0(nullptr);

					if (previsit == firstvisit)
					{
						previsit->SetChild0(visit->GetChild1());
						visit->SetChild1(nullptr);
						delete visit;
						visit = previsit->GetChild0();
						continue;
					}
					else
					{
						previsit->SetChild1(visit->GetChild1());
						visit->SetChild1(nullptr);
						delete visit;
						visit = previsit->GetChild1();
						continue;
					}

				}

				previsit = visit;
				visit = visit->GetChild1();
			}

			if (GlbVarTree != nullptr)
			{
				__InsrtAss(GlbVarTree);
				if (!m_GlbVarTree->MergeTree(GlbVarTree, CHOP_STA, VOIDTYPE))
				{
					PrintErrorMassage(0,"__Declr merge tree error 2!");
					m_RdcOk = 0;
					return;
				}
			}

			if (firstvisit->GetChild0() != nullptr)
			{
				__InsrtAss(DecTree);
				if (!m_LocVarTree->MergeTree(DecTree, CHOP_STA, VOIDTYPE))
				{
					PrintErrorMassage(0,"__Declr merge tree error 4!");
					m_RdcOk = 0;
					return;
				}

			}
			else
			{
				delete DecTree;
			}
		}

		DecTree = nullptr;
	}

	m_RdcOk = 1;
	return;
}
//-----------------------------------------------------------------------------
///直接将pTree加入到m_PrgmPrsnt中
///2015-3-9 add by wangmeng
void CMSV::__Ass(CSyntaxNode* pTree)
{
	if (pTree->GetChild0() == nullptr || pTree->GetChild1() == nullptr)
	{
		PrintErrorMassage(0, "__ASS() syntax tree error1!");
		m_RdcOk = 0;
		return;
	}

	///2015-3-31 add by wangmeng
	///next x<==e
	if (pTree->GetChild0()->GetNType() == NEXT_STA)
	{
		if (pTree->GetChild0()->GetChild0() == nullptr)
		{
			PrintErrorMassage(0,"__ASS() syntax tree error2!");
			m_RdcOk = 0;
			return;
		}
		///temp : x<==e
		CSyntaxNode *temp = new CSyntaxNode(EX_ASS_EXP, VOIDTYPE);
		temp->SetChild0(pTree->GetChild0()->GetChild0());
		temp->SetChild1(pTree->GetChild1());
		pTree->GetChild0()->SetChild0(nullptr);
		pTree->SetChild1(nullptr);

		if (m_PrgmFuture->MergeTree(temp, AND_EXP, VOIDTYPE))  ///temp 加入到下一状态语法树
		{
			m_More = 1;
			m_RdcOk = 1;

		}
		else
		{
			PrintErrorMassage(0,"__UASS() MergeTree() error!");
			m_RdcOk = 0;
		}
		return;
	}

	///2015-3-16 modified by wangmeng
	CSyntaxNode* AssTree = new CSyntaxNode(pTree->GetNType(), VOIDTYPE);
	AssTree->SetChild0(pTree->GetChild0());

	//赋值语句中有函数调用的处理
	__AssFunc(pTree->GetChild0(), true);
	__AssFunc(pTree->GetChild1(), true);

	if (m_RdcOk == 0)
	{
		return;
	}
	//add by yubin 2015/4/15,右边是外部函数调用的情况，当前赋值，只能是外部函数调用
	AssTree->SetChild1(pTree->GetChild1());

	///end add by wangmeng 

	pTree->SetChild0(nullptr);
	pTree->SetChild1(nullptr);
	__InsrtAss(AssTree);
	m_RdcOk = 1;
}

/**
* Get present tree of unit assignment statement: x:=e
* @param the syntax tree to be handled
* @return void
*/
///想法：y:=x+4，这里的x取当前状态值，如果直接将y<==x+4放到下一状态，需要将其放在下一状态对x的赋值的前边.
///这里，我们将由:=得到的<==，与程序中原本存在的<==进行区分。由:=得到的<==其节点类型我们定为EX_ASS_EXP。
///2015-3-9 add by wangmeng
void CMSV::__UAss(CSyntaxNode* pTree)
{
	///x:=e
	if (pTree->GetChild0() == nullptr || pTree->GetChild1() == nullptr)
	{
		PrintErrorMassage(0,"__UASS() syntax tree error!");
		m_RdcOk = 0;
		return;
	}

	if (m_More == 0)  ///如果没有下一状态了，就报错。
	{
		PrintErrorMassage(0,"__UASS() empty and next error!");
		m_RdcOk = 0;
		return;
	}


	///2015-4-17 add by wangmeng 右边包含函数调用的情况处理
	__AssFunc(pTree->GetChild1(), false);


	CSyntaxNode *temp = nullptr;
	//add by yubin 2015/4/15,
	if (m_HasFunCal)//右边是内部函数调用的情况
	{

		//	__FuncCall(pTree->GetChild1(), 0);

		CSyntaxNode* prsntTree = new CSyntaxNode(ASS_EQU_EXP, VOIDTYPE);
		prsntTree->SetChild0(pTree->GetChild0());
		prsntTree->SetChild1(pTree->GetChild1());
		pTree->SetChild0(nullptr);
		pTree->SetChild1(nullptr);
		__InsrtAss(prsntTree);
		__SortPrsnt();
		__AdPrst2IR();
		m_More = 0;
		m_RdcOk = 1;
		m_Done = 0;
		m_HasFunCal = false;

		return;

	}
	else
	{
		///temp : x<==e and empty
		temp = new CSyntaxNode(AND_EXP, VOIDTYPE);
		temp->SetChild0(new CSyntaxNode(EX_ASS_EXP, VOIDTYPE));
		temp->SetChild1(new CSyntaxNode(EMPTY_EXP, VOIDTYPE));
		temp->GetChild0()->SetChild0(pTree->GetChild0());
		pTree->SetChild0(nullptr);


		temp->GetChild0()->SetChild1(pTree->GetChild1());
		pTree->SetChild1(nullptr);
	}



	if (m_PrgmFuture->MergeTree(temp, AND_EXP, VOIDTYPE))  ///temp 加入到下一状态语法树
	{
		m_IsComplexStmt = false;
		m_More = 1;
		m_RdcOk = 1;
		m_Done = 0;
	}
	else
	{
		PrintErrorMassage(0,"__UASS() MergeTree() error!");
		m_RdcOk = 0;
	}
	temp = nullptr;


}

/**
* Get present tree of empty statement: empty
* @param the syntax tree to be handled
* @return void
*/
///对相应的标记位进行赋值操作。
///2015-3-9 add by wangmeng
void CMSV::__Emp(CSyntaxNode* pTree)
{
	if (m_More == 1)  ///如果有下一状态了，就报错。
	{
		g_error += "__Emp() empty and next error!\n";
		m_RdcOk = 0;
		return;
	}
	else
	{
		m_More = 0;
		m_RdcOk = 1;
		m_Done = 1;
	}

}

/**
* Get present tree of more statement: more
* @param the syntax tree to be handled
* @return void
*/
///对相应的标记位进行赋值操作。
///2015-3-9 add by yangkai
void CMSV::__More(CSyntaxNode* pTree)
{
	if (m_More == 0)  ///如果没有一状态了，就报错。
	{
		PrintErrorMassage(0, "__More() empty and next error!");
		m_RdcOk = 0;
		return;
	}
	else
	{
		CSyntaxNode* temp_True = new CSyntaxNode(TRUE_EXP, VOIDTYPE);
		if (m_PrgmFuture->MergeTree(temp_True, AND_EXP, VOIDTYPE))
		{
			m_More = 1;
			m_RdcOk = 1;
		}
		else
		{
			PrintErrorMassage(0, "__More() MergeTree() error!");
			m_RdcOk = 0;
		}
	}
}

/**
* Get present tree of true statement: true
* @param the syntax tree to be handled
* @return void
*/
///对相应的标记位进行赋值操作。
///2015-3-9 add by yangkai
void CMSV::__True(CSyntaxNode* pTree)
{
	m_RdcOk = 1;
	m_More = -1;
	return;
}

/**
* Get present tree of skip statement: skip
* @param the syntax tree to be handled
* @return void
*/
///empty加入到下一状态语法树中
///2015-3-9 add by wangmeng
void CMSV::__Skip(CSyntaxNode* pTree)
{
	if (m_More == 0)  ///如果没有下一状态了，就报错。
	{
		PrintErrorMassage(0,"__Skip() empty and next error!");
		m_RdcOk = 0;
		return;
	}
	CSyntaxNode *temp = new CSyntaxNode(EMPTY_EXP, VOIDTYPE);
	if (m_PrgmFuture->MergeTree(temp, AND_EXP, VOIDTYPE)) ///empty加入到下一状态语法树中
	{
		m_More = 1;
		m_RdcOk = 1;
		m_Done = 0;
	}
	else
	{
		PrintErrorMassage(0,"__Skip() MergeTree() error!");
		m_RdcOk = 0;
	}
	temp = nullptr;
}
//-----------------------------------------------------------------------------
void CMSV::__Len(CSyntaxNode* pTree)
{
	//将len(n)变成$$len_i=0; while($$len_i<n){next ($$len_i<==$$len_i+1 and empty)}; len_i=0
	string lenVar = "$$len_" + __Int2Str(m_LenNum);
	//全局变量中声明$$len_i变量
	CSyntaxNode* leniDec = new CSyntaxNode(DECLARE_STA, new CSyntaxNode(DEC_VAR_LIST, VOIDTYPE), INTTYPE);
	leniDec->GetChild0()->SetChild0(new CSyntaxNode(IDENT_EXP, lenVar, nullptr, VOIDTYPE));
	m_LocVarTree->MergeTree(leniDec, CHOP_STA, VOIDTYPE);
	//加入到符号表
	m_FrmSymbl->InstFSymbl(lenVar);
	//加入到当前状态m_PrgmPrsnt,声明变量的类型
	__InsrtAss(leniDec);
	//$$leni初始置0
	CSyntaxNode* assStmt = new CSyntaxNode(ASS_EQU_EXP, VOIDTYPE);
	assStmt->SetChild0(new CSyntaxNode(IDENT_EXP, lenVar, nullptr, VOIDTYPE));
	assStmt->SetChild1(new CSyntaxNode(INTEGER_EXP, 0, INTTYPE));
	m_IRTree->MergeTree(assStmt, CHOP_STA, VOIDTYPE);
	CSyntaxNode * whileStat = new CSyntaxNode(WHILE_DO_STA, 1, VOIDTYPE);//iValue设置为1
	CSyntaxNode * whileCondStat = new CSyntaxNode(LT_EXP, BOOLTYPE);//
	whileCondStat->SetChild0(new CSyntaxNode(IDENT_EXP, lenVar, nullptr, VOIDTYPE));
	whileCondStat->SetChild1(pTree->GetChild0());
	whileStat->SetChild0(whileCondStat);
	//next ($$len_i<==$$len_i+1 and empty)
	CSyntaxNode* nextStat = new CSyntaxNode(NEXT_STA, VOIDTYPE);
	nextStat->SetChild0(new CSyntaxNode(AND_EXP, VOIDTYPE));
	nextStat->GetChild0()->SetChild0(new CSyntaxNode(ASS_EQU_EXP, VOIDTYPE));
	nextStat->GetChild0()->GetChild0()->SetChild0(new CSyntaxNode(IDENT_EXP, lenVar, nullptr, VOIDTYPE));
	nextStat->GetChild0()->GetChild0()->SetChild1(new CSyntaxNode(ADD_EXP, VOIDTYPE));
	nextStat->GetChild0()->GetChild0()->GetChild1()->SetChild0(new CSyntaxNode(IDENT_EXP, lenVar, nullptr, VOIDTYPE));
	nextStat->GetChild0()->GetChild0()->GetChild1()->SetChild1(new CSyntaxNode(INTEGER_EXP, 1, VOIDTYPE));
	nextStat->GetChild0()->SetChild1(new CSyntaxNode(EMPTY_EXP, VOIDTYPE));
	whileStat->SetChild1(nextStat);
	//最后将$$leni置0
	pTree = whileStat;
	GetPresent(pTree);
	assStmt = new CSyntaxNode(ASS_EQU_EXP, VOIDTYPE);
	assStmt->SetChild0(new CSyntaxNode(IDENT_EXP, lenVar, nullptr, VOIDTYPE));
	assStmt->SetChild1(new CSyntaxNode(INTEGER_EXP, 0, INTTYPE));
	m_IRTree->MergeTree(assStmt, CHOP_STA, VOIDTYPE);
	m_LenNum++;
	return;
}
//-----------------------------------------------------------------------------
void CMSV::__Next(CSyntaxNode* pTree)
{
	////next p
	if (pTree->GetChild0() == nullptr)
	{
		PrintErrorMassage(0, "__Next() syntax tree error!");
		m_RdcOk = 0;
		return;
	}
	if (m_More == 0) ///如果没有下一状态了，就报错。
	{
		PrintErrorMassage(0,"__Next() empty and next error!");
		m_RdcOk = 0;
		return;
	}

	///将p加入到下一状态语法树中
	if (m_PrgmFuture->MergeTree(pTree->GetChild0(), AND_EXP, VOIDTYPE))
	{
		pTree->SetChild0(nullptr);
		m_More = 1;
		m_RdcOk = 1;
		m_Done = 0;
		return;
	}
	else
	{
		PrintErrorMassage(0,"__Next() MergeTree() error!");
		m_RdcOk = 0;
		return;
	}

}
//-----------------------------------------------------------------------------
void CMSV::__And(CSyntaxNode* pTree)
{
	if (pTree->GetiValue() == 1)//复杂并行语句
	{
		if (Setting::IsSimulation())
		{
			__DelLastStateOutput();
		}
		else if (Setting::IsModeling())
		{
			__DelLastAddEdge();
			CSyntaxNode* getStatementNum = new CSyntaxNode(FUNCTION_STA, VOIDTYPE);
			getStatementNum->SetChild0(new CSyntaxNode(IDENT_EXP, "addStatement", nullptr, VOIDTYPE));
			getStatementNum->SetChild1(new CSyntaxNode(OPTION_EXP, new CSyntaxNode(INTEGER_EXP, m_StatementNum, VOIDTYPE), VOIDTYPE));
			m_StatementNum++;
			m_IRTree->MergeTree(getStatementNum, CHOP_STA, VOIDTYPE);
		}
		__ComplexAnd(pTree);
		m_More = 0;
		m_RdcOk = 1;
		if (!m_RdcOk) { return; }
	}
	else
	{
		//得到左边的当前状态
		m_PrgmFuture->SetRoot(nullptr);
		GetPresent(pTree->GetChild0());

		if (!m_RdcOk)
		{
			PrintErrorMassage(0,"__And() error 791!");
			return;
		}

		CSyntaxNode* leftFutr = m_PrgmFuture->GetRoot();
		int leftMore = m_More;

		m_PrgmFuture->SetRoot(nullptr);
		m_More = -1;
		//得到右边的当前状态
		GetPresent(pTree->GetChild1());
		CSyntaxNode* rightFutr = m_PrgmFuture->GetRoot();
		m_PrgmFuture->SetRoot(nullptr);
		int rightMore = m_More;

		//左边和右边都含有future部分，则将两边的future部分用and节点连接起来作为新的future
		if (leftMore == 1 && rightMore == 1)
		{
			CSyntaxNode* temp_FutrTree = new CSyntaxNode(AND_EXP, VOIDTYPE);
			temp_FutrTree->SetChild0(leftFutr);
			temp_FutrTree->SetChild1(rightFutr);
			m_PrgmFuture->SetRoot(temp_FutrTree);
			m_More = 1;
		}
		//左右都没有future部分
		else if (leftMore == 0 && rightMore == 0)
		{
			m_PrgmFuture->SetRoot(nullptr);
			m_More = 0;
		}
		//左边有future部分，右边没有规定区间长度
		else if (leftMore == 1 && rightMore == -1)
		{
			m_PrgmFuture->SetRoot(leftFutr);
			m_More = 1;
		}
		//右边有future部分，左边没有规定区间长度
		else if (leftMore == -1 && rightMore == 1)
		{
			m_PrgmFuture->SetRoot(rightFutr);
			m_More = 1;
		}
		//左边没有下一状态，右边没有规定区间长度
		else if (leftMore == 0 && rightMore == -1)
		{
			m_PrgmFuture->SetRoot(nullptr);
			m_More = 0;
		}
		//左边没有规定区间长度，右边没有下一状态
		else if (leftMore == -1 && rightMore == 0)
		{
			m_PrgmFuture->SetRoot(nullptr);
			m_More = 0;
		}
		//两边都没有规定区间长度
		else if (leftMore == -1 && rightMore == -1)
		{
			m_More = -1;
		}
		//左右区间长度不一致，报错，将整个IRTree置为false
		else//(1,0) (0,1)
		{
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED);
			PrintErrorMassage(0, "Error interval length \n" + pTree->TreeToString());
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY);
			g_HasErr++;
			m_RdcOk = 0;
			return;
		}
	}

	m_RdcOk = 1;
}
//-----------------------------------------------------------------------------
void CMSV::__Pal(CSyntaxNode* pTree)
{
	if (pTree->GetiValue() == 1)//复杂并行语句
	{
		if (Setting::IsSimulation())
		{
			__DelLastStateOutput();
		}
		else if (Setting::IsModeling())
		{
			__DelLastAddEdge();
			CSyntaxNode* getStatementNum = new CSyntaxNode(FUNCTION_STA, VOIDTYPE);
			getStatementNum->SetChild0(new CSyntaxNode(IDENT_EXP, "addStatement", nullptr, VOIDTYPE));
			getStatementNum->SetChild1(new CSyntaxNode(OPTION_EXP, new CSyntaxNode(INTEGER_EXP, m_StatementNum, VOIDTYPE), VOIDTYPE));
			m_StatementNum++;
			m_IRTree->MergeTree(getStatementNum, CHOP_STA, VOIDTYPE);
		}
		__ComplexPal(pTree);
		m_More = 0;
		m_RdcOk = 1;
		if (!m_RdcOk) { return; }
	}
	else
	{
		m_PrgmFuture->SetRoot(nullptr);
		GetPresent(pTree->GetChild0());
		CSyntaxNode* leftFutr = m_PrgmFuture->GetRoot();
		int leftMore = m_More;

		m_PrgmFuture->SetRoot(nullptr);
		m_More = -1;
		GetPresent(pTree->GetChild1());
		CSyntaxNode* rightFutr = m_PrgmFuture->GetRoot();
		m_PrgmFuture->SetRoot(nullptr);
		int rightMore = m_More;

		if ((leftMore == -1 || rightMore == -1))
		{
			if (!g_InNewFunc)
			{
				PrintErrorMassage(0, "Error interval length");
				m_RdcOk = 0;
				return;
			}
		}
		else if (leftMore == 1 && rightMore == 1)
		{
			//yangkai 处理while相关
			if (leftFutr != nullptr && rightFutr != nullptr)
			{
				CSyntaxNode* temp_FutrTree = new CSyntaxNode(PARALLEL_STA, VOIDTYPE);
				temp_FutrTree->SetChild0(leftFutr);
				temp_FutrTree->SetChild1(rightFutr);
				m_PrgmFuture->SetRoot(temp_FutrTree);
				m_More = 1;
			}
		}
		else if (leftMore == 1 && rightMore == 0)
		{
			m_PrgmFuture->SetRoot(leftFutr);
			m_More = 1;
		}
		else if (leftMore == 0 && rightMore == 1)
		{
			m_PrgmFuture->SetRoot(rightFutr);
			m_More = 1;
		}
		else if (leftMore == 0 && rightMore == 0)
		{
			m_PrgmFuture->SetRoot(nullptr);
			m_More = 0;
		}
	}

	m_RdcOk = 1;
	return;
}
//-----------------------------------------------------------------------------
void CMSV::__DelLastAddEdge()
{
	if (m_IRTree->GetRoot() != nullptr)
	{
		if (m_IRTree->GetRoot()->GetNType() == GENERATE_EDGE_STA)
		{
			//m_IRTree只有一个AddEdge语句，则直接删掉
			delete m_IRTree->GetRoot();
			m_IRTree->SetRoot(nullptr);
		}
		else if (m_IRTree->GetRoot()->GetNType() == CHOP_STA)
		{
			//m_IRTree根节点是chop时，chop的右子树一定是最后一个AddEdge语句，直接删掉右子树
			if (m_IRTree->GetRoot()->GetChild1()->GetNType() == GENERATE_EDGE_STA)
			{
				CSyntaxNode* temp = m_IRTree->GetRoot();
				m_IRTree->SetRoot(temp->GetChild0());
				temp->SetChild0(nullptr);
				delete temp;
			}
		}
	}
	m_StatementNum--;
}
//-----------------------------------------------------------------------------
void CMSV::__PalNotInFunc(CSyntaxNode* pTree)
{
	CSyntaxNode* threadFuncActualPar = nullptr;//线程函数的形参
	CSyntaxNode* funcFormalPar = nullptr;//新定义函数的形参
	CSyntaxNode* funcActualPar = nullptr;//新定义函数的实参
	string funcName = "";//新定义函数的函数名
	string threadFuncName = "";//新定义函数的函数名
	CSyntaxNode* createThread = nullptr;//新创建的线程
	CSyntaxNode* visit = pTree;
	CSyntaxNode* temp = visit;
	while (visit->GetNType() == PARALLEL_STA)
	{
		funcName = "$$f" + __Int2Str(m_NewFuncNum);
		threadFuncName = "$$t" + __Int2Str(m_NewFuncNum);
		__DefFunc(funcName, nullptr, visit->GetChild0());//定义函数
		visit->SetChild0(nullptr);
		__DefThreadFunc(threadFuncName, funcName, nullptr);//定义线程函数		
		threadFuncActualPar = new CSyntaxNode(OPTION_EXP, VOIDTYPE);//线程函数的参数，默认为nullptr
		threadFuncActualPar->SetChild0(new CSyntaxNode(NULL_EXP, VOIDTYPE));
		createThread = __MyCreateThread(threadFuncName, threadFuncActualPar, 1);//创建线程
		m_IRTree->MergeTree(createThread, CHOP_STA, VOIDTYPE);
		m_NewFuncNum++;
		temp = visit;
		visit = visit->GetChild1();
	}
	temp->SetChild1(nullptr);//将最右边的分支置为空
	funcName = "$$f" + __Int2Str(m_NewFuncNum);
	threadFuncName = "$$t" + __Int2Str(m_NewFuncNum);
	__DefFunc(funcName, nullptr, visit);//定义函数
	__DefThreadFunc(threadFuncName, funcName, nullptr);//定义线程函数	
	threadFuncActualPar = new CSyntaxNode(OPTION_EXP, VOIDTYPE);//线程函数的参数，默认为nullptr
	threadFuncActualPar->SetChild0(new CSyntaxNode(NULL_EXP, VOIDTYPE));
	createThread = __MyCreateThread(threadFuncName, threadFuncActualPar, 1);//创建线程
	m_IRTree->MergeTree(createThread, CHOP_STA, VOIDTYPE);
	__MyWaitForObject();

	CSyntaxNode* ifStmt = new CSyntaxNode(IF_ELSE_STA, VOIDTYPE);
	ifStmt->SetChild0(new CSyntaxNode(EQU_EXP, VOIDTYPE));
	ifStmt->GetChild0()->SetChild0(new CSyntaxNode(IDENT_EXP, "$$state_num", nullptr, VOIDTYPE));
	ifStmt->GetChild0()->SetChild1(new CSyntaxNode(INTEGER_EXP, -1, INTTYPE));
	ifStmt->SetChild1(new CSyntaxNode(FUNCTION_STA, new CSyntaxNode(IDENT_EXP, "$$MyLongJmp", nullptr, VOIDTYPE), VOIDTYPE));
	m_IRTree->MergeTree(ifStmt, CHOP_STA, VOIDTYPE);

	m_NewFuncNum++;
}
//-----------------------------------------------------------------------------
void CMSV::__PalInFunc(CSyntaxNode* pTree)
{
	CSyntaxNode* threadFuncActualPar = nullptr;//线程函数的形参
	CSyntaxNode* funcFormalPar = nullptr;//新定义函数的形参
	CSyntaxNode* funcActualPar = nullptr;//新定义函数的实参
	string funcName = "";//新定义函数的函数名
	string threadFuncName = "";//新定义函数的函数名
	CSyntaxNode* createThread = nullptr;//新创建的线程
	CSyntaxNode* newStruct = nullptr;
	/*如果当前并行语句在用户定义的函数中，需要定义新的结构体，新定义的函数的形参可以从新定义的结构体得到
	如果当前并行语句在新定义的函数中，则新函数的形参和当前所在的新函数的形参一样
	*/
	if (!g_InNewFunc || m_CurrentFunc.substr(0, 7) == "$$user_")
	{
		//根据当前函数的形参和局部变量定义一个新的结构体，并对新结构体的成员进行初始化
		newStruct = __DefStruct(m_LocVarTree->GetRoot(), m_FuncFormalPar);
		if (newStruct != nullptr)
		{
			__InitStruct(newStruct);
			//由新结构体得到新定义的函数的形参，前n-1个形参是结构体的成员，最后一个形参是结构体指针类型的一个变量
			funcFormalPar = __GetFuncFormalPar(newStruct);
			//线程函数实参为(void*)$$si,其中$$si为新定义的结构体类型的一个指针型变量
			threadFuncActualPar = new CSyntaxNode(OPTION_EXP, VOIDTYPE);
			threadFuncActualPar->SetChild0(new CSyntaxNode(TYPE_CAST_STA, VOIDTYPE));
			threadFuncActualPar->GetChild0()->SetChild0(new CSyntaxNode(CAST_TYPE, VOIDTYPE));
			threadFuncActualPar->GetChild0()->GetChild0()->SetChild0(new CSyntaxNode(POINTER_EXP, VOIDTYPE));
			threadFuncActualPar->GetChild0()->SetChild1(new CSyntaxNode(IDENT_EXP, "$$s" + __Int2Str(m_NewStructNum), nullptr, VOIDTYPE));
		}
		else
		{
			funcFormalPar = nullptr;
			threadFuncActualPar = new CSyntaxNode(OPTION_EXP, VOIDTYPE);//线程函数的参数，默认为nullptr
			threadFuncActualPar->SetChild0(new CSyntaxNode(NULL_EXP, VOIDTYPE));
		}
	}
	else
	{
		//新定义函数的形参和当前函数的形参一样（m_FuncFormalPar记录的是当前正在处理的函数的形参）
		funcFormalPar = m_FuncFormalPar;
		/*m_FuncFormalPar=nullptr时，当前函数没有形参，则线程函数的参数为nullptr，否则线程函数的实参为$$par（和当前函数
		的最后一个形参名一致）*/
		if (m_FuncFormalPar != nullptr)
		{
			threadFuncActualPar = new CSyntaxNode(OPTION_EXP, VOIDTYPE);
			threadFuncActualPar->SetChild0(new CSyntaxNode(IDENT_EXP, "$$par", nullptr, VOIDTYPE));
		}
		else
		{
			threadFuncActualPar = new CSyntaxNode(OPTION_EXP, VOIDTYPE);//线程函数的参数，默认为nullptr
			threadFuncActualPar->SetChild0(new CSyntaxNode(NULL_EXP, VOIDTYPE));
		}
	}
	funcActualPar = __GetFuncActualPar(funcFormalPar);//根据形参名得到新函数的实参

	//处理并行的每个分支
	CSyntaxNode* visit = pTree;
	CSyntaxNode* temp = visit;
	while (visit->GetNType() == PARALLEL_STA)
	{
		funcName = "$$f" + __Int2Str(m_NewFuncNum);
		threadFuncName = "$$t" + __Int2Str(m_NewFuncNum);
		__DefThreadFunc(threadFuncName, funcName, funcActualPar);//定义线程函数$$ti
		__AddPointer(visit->GetChild0());//给当前语句分支每个局部变量前加*
		__DefFunc(funcName, funcFormalPar, visit->GetChild0());//根据函数名，函数形参，和函数体定义新函数
		visit->SetChild0(nullptr);
		CSyntaxNode* copyThreadFuncActualPar = new CSyntaxNode();
		copyThreadFuncActualPar->CopyTree(threadFuncActualPar);
		createThread = __MyCreateThread(threadFuncName, copyThreadFuncActualPar, 1);//创建线程MyCreateThread($$ti, (void*)$$si, 0|1)
		m_IRTree->MergeTree(createThread, CHOP_STA, VOIDTYPE);
		m_NewFuncNum++;
		temp = visit;
		visit = visit->GetChild1();
	}
	temp->SetChild1(nullptr);
	funcName = "$$f" + __Int2Str(m_NewFuncNum);
	threadFuncName = "$$t" + __Int2Str(m_NewFuncNum);
	__DefThreadFunc(threadFuncName, funcName, funcActualPar);//定义线程函数$$ti
	__AddPointer(visit); //给当前语句分支每个局部变量前加*
	__DefFunc(funcName, funcFormalPar, visit);//定义函数
	createThread = __MyCreateThread(threadFuncName, threadFuncActualPar, 1);//创建线程MyCreateThread($$ti, (void*)$$si, 0|1)	
	m_IRTree->MergeTree(createThread, CHOP_STA, VOIDTYPE);
	__MyWaitForObject();
	if (newStruct != nullptr)
	{
		__MyFree("$$s" + __Int2Str(m_NewStructNum));//free($$s)
	}
	m_NewStructNum++;
	m_NewFuncNum++;
}
//-----------------------------------------------------------------------------
void CMSV::__ComplexPal(CSyntaxNode* pTree)
{
	__Trim(pTree);//整理语法树的结构
	if (!g_InNewFunc)//只在最外层创建调度线程
	{
		CSyntaxNode* threadFuncActualPar = new CSyntaxNode(OPTION_EXP, VOIDTYPE);//线程函数的参数，默认为nullptr
		threadFuncActualPar->SetChild0(new CSyntaxNode(NULL_EXP, VOIDTYPE));
		CSyntaxNode* createThread = __MyCreateThread("ManagerThread", threadFuncActualPar, 5);
		m_IRTree->MergeTree(createThread, CHOP_STA, VOIDTYPE);
	}
	if (!g_InFunc)
	{
		__PalNotInFunc(pTree);
	}
	else
	{
		__PalInFunc(pTree);
	}
}
//-----------------------------------------------------------------------------
void CMSV::__AndNotInFunc(CSyntaxNode* pTree)
{
	CSyntaxNode* threadFuncActualPar = nullptr;//线程函数的形参
	CSyntaxNode* funcFormalPar = nullptr;//新定义函数的形参
	CSyntaxNode* funcActualPar = nullptr;//新定义函数的实参
	string funcName = "";//新定义函数的函数名
	string threadFuncName = "";//新定义函数的函数名
	CSyntaxNode* createThread = nullptr;//新创建的线程
	CSyntaxNode* visit = pTree;
	CSyntaxNode* temp = visit;
	while (visit->GetNType() == AND_EXP)
	{
		funcName = "$$f" + __Int2Str(m_NewFuncNum);
		threadFuncName = "$$t" + __Int2Str(m_NewFuncNum);
		__DefFunc(funcName, nullptr, visit->GetChild0());//定义函数
		visit->SetChild0(nullptr);
		__DefThreadFunc(threadFuncName, funcName, nullptr);//定义线程函数		
		threadFuncActualPar = new CSyntaxNode(OPTION_EXP, VOIDTYPE);//线程函数的参数，默认为nullptr
		threadFuncActualPar->SetChild0(new CSyntaxNode(NULL_EXP, VOIDTYPE));
		createThread = __MyCreateThread(threadFuncName, threadFuncActualPar, 0);//创建线程
		m_IRTree->MergeTree(createThread, CHOP_STA, VOIDTYPE);
		m_NewFuncNum++;
		temp = visit;
		visit = visit->GetChild1();
	}
	temp->SetChild1(nullptr);//将最右边的分支置为空
	funcName = "$$f" + __Int2Str(m_NewFuncNum);
	threadFuncName = "$$t" + __Int2Str(m_NewFuncNum);
	__DefFunc(funcName, nullptr, visit);//定义函数
	__DefThreadFunc(threadFuncName, funcName, nullptr);//定义线程函数	
	threadFuncActualPar = new CSyntaxNode(OPTION_EXP, VOIDTYPE);//线程函数的参数，默认为nullptr
	threadFuncActualPar->SetChild0(new CSyntaxNode(NULL_EXP, VOIDTYPE));
	createThread = __MyCreateThread(threadFuncName, threadFuncActualPar, 0);//创建线程
	m_IRTree->MergeTree(createThread, CHOP_STA, VOIDTYPE);

	__MyWaitForObject();

	CSyntaxNode* ifStmt = new CSyntaxNode(IF_ELSE_STA, VOIDTYPE);
	ifStmt->SetChild0(new CSyntaxNode(EQU_EXP, VOIDTYPE));
	ifStmt->GetChild0()->SetChild0(new CSyntaxNode(IDENT_EXP, "$$state_num", nullptr, VOIDTYPE));
	ifStmt->GetChild0()->SetChild1(new CSyntaxNode(INTEGER_EXP, -1, INTTYPE));
	ifStmt->SetChild1(new CSyntaxNode(FUNCTION_STA, new CSyntaxNode(IDENT_EXP, "$$MyLongJmp", nullptr, VOIDTYPE), VOIDTYPE));
	m_IRTree->MergeTree(ifStmt, CHOP_STA, VOIDTYPE);

	m_NewFuncNum++;
}
//----------------------------------------------------------------------------
void CMSV::__AndInFunc(CSyntaxNode* pTree)
{
	CSyntaxNode* threadFuncActualPar = nullptr;//线程函数的形参
	CSyntaxNode* funcFormalPar = nullptr;//新定义函数的形参
	CSyntaxNode* funcActualPar = nullptr;//新定义函数的实参
	string funcName = "";//新定义函数的函数名
	string threadFuncName = "";//新定义函数的函数名
	CSyntaxNode* createThread = nullptr;//新创建的线程
	CSyntaxNode* newStruct = nullptr;//新定义的结构体
	/*如果当前并行语句在用户定义的函数中，需要定义新的结构体，新定义的函数的形参可以从新定义的结构体得到
	如果当前并行语句在新定义的函数中，则新函数的形参和当前所在的新函数的形参一样
	*/
	if (!g_InNewFunc || m_CurrentFunc.substr(0, 7) == "$$user_")
	{
		//根据当前函数的形参和局部变量定义一个新的结构体，并对新结构体的成员进行初始化
		newStruct = __DefStruct(m_LocVarTree->GetRoot(), m_FuncFormalPar);
		if (newStruct != nullptr)
		{
			__InitStruct(newStruct);
			//由新结构体得到新定义的函数的形参，前n-1个形参是结构体的成员，最后一个形参是结构体指针类型的一个变量
			funcFormalPar = __GetFuncFormalPar(newStruct);
			//线程函数实参为(void*)$$si,其中$$si为新定义的结构体类型的一个指针型变量
			threadFuncActualPar = new CSyntaxNode(OPTION_EXP, VOIDTYPE);
			threadFuncActualPar->SetChild0(new CSyntaxNode(TYPE_CAST_STA, VOIDTYPE));
			threadFuncActualPar->GetChild0()->SetChild0(new CSyntaxNode(CAST_TYPE, VOIDTYPE));
			threadFuncActualPar->GetChild0()->GetChild0()->SetChild0(new CSyntaxNode(POINTER_EXP, VOIDTYPE));
			threadFuncActualPar->GetChild0()->SetChild1(new CSyntaxNode(IDENT_EXP, "$$s" + __Int2Str(m_NewStructNum), nullptr, VOIDTYPE));
		}
		else
		{
			funcFormalPar = nullptr;
			threadFuncActualPar = new CSyntaxNode(OPTION_EXP, VOIDTYPE);//线程函数的参数，默认为nullptr
			threadFuncActualPar->SetChild0(new CSyntaxNode(NULL_EXP, VOIDTYPE));
		}
	}
	else
	{
		//新定义函数的形参和当前函数的形参一样（m_FuncFormalPar记录的是当前正在处理的函数的形参）
		funcFormalPar = m_FuncFormalPar;
		/*m_FuncFormalPar=nullptr时，当前函数没有形参，则线程函数的参数为nullptr，否则线程函数的实参为$$par（和当前函数
		的最后一个形参名一致）*/
		if (m_FuncFormalPar != nullptr)
		{
			threadFuncActualPar = new CSyntaxNode(OPTION_EXP, VOIDTYPE);
			threadFuncActualPar->SetChild0(new CSyntaxNode(IDENT_EXP, "$$par", nullptr, VOIDTYPE));
		}
		else
		{
			threadFuncActualPar = new CSyntaxNode(OPTION_EXP, VOIDTYPE);//线程函数的参数，默认为nullptr
			threadFuncActualPar->SetChild0(new CSyntaxNode(NULL_EXP, VOIDTYPE));
		}
	}
	funcActualPar = __GetFuncActualPar(funcFormalPar);//根据形参名得到新函数的实参

	//处理并行的每个分支
	CSyntaxNode* visit = pTree;
	CSyntaxNode* temp = visit;
	while (visit->GetNType() == AND_EXP)
	{
		funcName = "$$f" + __Int2Str(m_NewFuncNum);
		threadFuncName = "$$t" + __Int2Str(m_NewFuncNum);
		__DefThreadFunc(threadFuncName, funcName, funcActualPar);//定义线程函数$$ti
		__AddPointer(visit->GetChild0());//给当前语句分支每个局部变量前加*
		__DefFunc(funcName, funcFormalPar, visit->GetChild0());//根据函数名，函数形参，和函数体定义新函数
		visit->SetChild0(nullptr);
		CSyntaxNode* copyThreadFuncActualPar = new CSyntaxNode();
		copyThreadFuncActualPar->CopyTree(threadFuncActualPar);
		createThread = __MyCreateThread(threadFuncName, copyThreadFuncActualPar, 0);//创建线程MyCreateThread($$ti, (void*)$$si, 0|1)
		m_IRTree->MergeTree(createThread, CHOP_STA, VOIDTYPE);
		m_NewFuncNum++;
		temp = visit;
		visit = visit->GetChild1();
	}
	temp->SetChild1(nullptr);
	funcName = "$$f" + __Int2Str(m_NewFuncNum);
	threadFuncName = "$$t" + __Int2Str(m_NewFuncNum);
	__DefThreadFunc(threadFuncName, funcName, funcActualPar);//定义线程函数$$ti
	__AddPointer(visit); //给当前语句分支每个局部变量前加*
	__DefFunc(funcName, funcFormalPar, visit);//定义函数
	createThread = __MyCreateThread(threadFuncName, threadFuncActualPar, 0);//创建线程MyCreateThread($$ti, (void*)$$si, 0|1)	
	m_IRTree->MergeTree(createThread, CHOP_STA, VOIDTYPE);
	__MyWaitForObject();
	__MyFree("$$s" + __Int2Str(m_NewStructNum));//free($$s)
	m_NewStructNum++;
	m_NewFuncNum++;
}
//-----------------------------------------------------------------------------
void CMSV::__ComplexAnd(CSyntaxNode* pTree)
{
	__Trim(pTree);//整理语法树的结构
	if (!g_InNewFunc)//只在最外层创建调度线程
	{
		CSyntaxNode* threadFuncActualPar = new CSyntaxNode(OPTION_EXP, VOIDTYPE);//线程函数的参数，默认为nullptr
		threadFuncActualPar->SetChild0(new CSyntaxNode(NULL_EXP, VOIDTYPE));
		CSyntaxNode* createThread = __MyCreateThread("ManagerThread", threadFuncActualPar, 5);
		m_IRTree->MergeTree(createThread, CHOP_STA, VOIDTYPE);
	}
	if (!g_InFunc)
	{
		__AndNotInFunc(pTree);
	}
	else
	{
		__AndInFunc(pTree);
	}
}
//-----------------------------------------------------------------------------
void CMSV::__AddPointer(CSyntaxNode* pTree)
{
	/*if (g_InNewFunc || m_CurrentFunc.substr(0, 7) == "$$user_")
	{
		return;
	}*/
	switch (pTree->GetNType())
	{
	case CHOP_STA:
	case AND_EXP:
	case PARALLEL_STA:
	case GT_EXP:
	case GE_EXP:
	case LT_EXP:
	case LE_EXP:
	case NE_EXP:
	case OR_EXP:
	case BOR_EXP:
	case XOR_EXP:
	case ASS_EQU_EXP:
	case UNITASSIGN_EXP:
	case EQU_EXP:
	case LST_EXP:
	case RST_EXP:
	case ADD_EXP:
	case SUB_EXP:
	case MUL_EXP:
	case DIV_EXP:
	case MOD_EXP:
	case DOT_EXP:
	case ARROW_EXP:
		__AddPointer(pTree->GetChild0());
		__AddPointer(pTree->GetChild1());
		break;
	case IF_ELSE_STA:
		__AddPointer(pTree->GetChild0());
		__AddPointer(pTree->GetChild1());
		__AddPointer(pTree->GetChild2());
		break;
	case WHILE_DO_STA:
		__AddPointer(pTree->GetChild0());
		__AddPointer(pTree->GetChild1());
		break;
	case FUNCTION_STA:
	case EXT_FUNCTION_STA:
		if (pTree->GetChild1() != nullptr)
		{
			__AddPointer(pTree->GetChild1());
		}
		break;
	case BAN_EXP:
	case ADDRESS_EXP:
	case POINTER_EXP:
	case UNARY_SUB_EXP:
	case NEGATION_EXP:
	case BNE_EXP:
		__AddPointer(pTree->GetChild0());
		break;
	case ARRAY_EXP:
		__AddPointer(pTree->GetChild0());
		__AddPointer(pTree->GetChild1());
		break;
	case IDENT_EXP:
	{
		//如果当前变量是局部变量，则加*
		set<string>::iterator it;
		it = m_LocalVarName.find(pTree->GetNName());
		if (it != m_LocalVarName.end())
		{
			CSyntaxNode* temp = new CSyntaxNode();
			temp->CopyTree(pTree);
			pTree->SetNType(POINTER_EXP);
			pTree->SetNName("");
			pTree->SetChild0(temp);
		}
		break;
	}
	case OPTION_EXP:
	{
		CSyntaxNode* visit = pTree;
		while (visit != nullptr)
		{
			__AddPointer(visit->GetChild0());
			visit = visit->GetChild1();
		}
		break;
	}
	case DISPLAY_STA:
	{
		__AddPointer(pTree->GetChild0());
		break;
	}
	default:
		break;
	}
}
//-----------------------------------------------------------------------------
CSyntaxNode* CMSV::__MyCreateThread(const string pFuncName, CSyntaxNode* pFuncActualPar, int pNodeType)
{
	CSyntaxNode* createThread = new CSyntaxNode(CREATE_THREAD, pFuncName, nullptr, VOIDTYPE);
	createThread->SetChild0(pFuncActualPar);
	createThread->SetiValue(pNodeType);
	//m_IRTree->MergeTree(createThread, CHOP_STA, VOIDTYPE);
	return createThread;
}
//-----------------------------------------------------------------------------
void CMSV::__DefFunc(const string pFuncName, CSyntaxNode* pFormalPar, CSyntaxNode* pFuncBody)
{
	CSyntaxNode* newFunc = new CSyntaxNode(FUNCTION_DEFINE_STA, pFuncName, nullptr, VOIDTYPE);
	newFunc->SetChild0(pFormalPar);
	newFunc->SetChild1(pFuncBody);
	newFunc->SetChild2(m_NewFuncTree);
	m_NewFuncTree = newFunc;
}
//-----------------------------------------------------------------------------
void CMSV::__DefThreadFunc(string pThreadFuncName, string pFuncName, CSyntaxNode* funcActualPar)
{
	//线程函数的函数体,调用函数名为pFuncName的函数，然后是return语句
	g_ThreadFuncName.insert(pThreadFuncName);//将线程函数的函数名存到set中
	CSyntaxNode* newThreadFunc = new CSyntaxNode(FUNCTION_DEFINE_STA, pThreadFuncName, nullptr, VOIDTYPE);
	//线程函数的返回值类型是unsigned int
	newThreadFunc->SetChild0(new CSyntaxNode(FUNC_RETURN_TYPE, VOIDTYPE));
	newThreadFunc->GetChild0()->SetChild0(new CSyntaxNode(FORMAL_PARAMETER_EXP, UINTTYPE));
	newThreadFunc->GetChild0()->GetChild0()->SetChild0(new CSyntaxNode(IDENT_EXP, "", nullptr, VOIDTYPE));
	//线程函数的形参是 void *p
	CSyntaxNode* threadFuncFormalPar = new CSyntaxNode(FORMAL_PARAMETER_EXP, VOIDTYPE);
	threadFuncFormalPar->SetChild0(new CSyntaxNode(POINTER_EXP, VOIDTYPE));
	threadFuncFormalPar->GetChild0()->SetChild0(new CSyntaxNode(IDENT_EXP, "p", nullptr, VOIDTYPE));
	newThreadFunc->GetChild0()->SetChild1(threadFuncFormalPar);
	//函数体
	CSyntaxTree* newThreadFuncBody = new CSyntaxTree();
	CSyntaxNode* funcCall = new CSyntaxNode(FUNCTION_STA, VOIDTYPE);
	funcCall->SetChild0(new CSyntaxNode(IDENT_EXP, pFuncName, nullptr, VOIDTYPE));
	funcCall->SetChild1(new CSyntaxNode(OPTION_EXP, VOIDTYPE));
	funcCall->GetChild1()->SetChild0(new CSyntaxNode(INTEGER_EXP, 0, INTTYPE));
	funcCall->GetChild1()->SetChild1(funcActualPar);
	newThreadFuncBody->MergeTree(funcCall, CHOP_STA, VOIDTYPE);
	CSyntaxNode* returnStmt = new CSyntaxNode(RETURN_STA, new CSyntaxNode(INTEGER_EXP, 0, INTTYPE), VOIDTYPE);
	newThreadFuncBody->MergeTree(returnStmt, CHOP_STA, VOIDTYPE);
	newThreadFunc->SetChild1(newThreadFuncBody->GetRoot());
	newThreadFunc->SetChild2(m_ThreadFuncTree);
	m_ThreadFuncTree = newThreadFunc;
}
//-----------------------------------------------------------------------------
void CMSV::__DefThreadFuncForAlw(string pThreadFuncName,  string pFuncName, CSyntaxNode* funcActualPar)
{
	g_ThreadFuncName.insert(pThreadFuncName);//将线程函数的函数名存到set中
	CSyntaxNode* newThreadFunc = new CSyntaxNode(FUNCTION_DEFINE_STA, pThreadFuncName, nullptr, VOIDTYPE);
	//线程函数的返回值类型是unsigned int
	newThreadFunc->SetChild0(new CSyntaxNode(FUNC_RETURN_TYPE, VOIDTYPE));
	newThreadFunc->GetChild0()->SetChild0(new CSyntaxNode(FORMAL_PARAMETER_EXP, UINTTYPE));
	newThreadFunc->GetChild0()->GetChild0()->SetChild0(new CSyntaxNode(IDENT_EXP, "", nullptr, VOIDTYPE));
	//线程函数的形参是 void *p
	CSyntaxNode* threadFuncFormalPar = new CSyntaxNode(FORMAL_PARAMETER_EXP, VOIDTYPE);
	threadFuncFormalPar->SetChild0(new CSyntaxNode(POINTER_EXP, VOIDTYPE));
	threadFuncFormalPar->GetChild0()->SetChild0(new CSyntaxNode(IDENT_EXP, "p", nullptr, VOIDTYPE));
	newThreadFunc->GetChild0()->SetChild1(threadFuncFormalPar);
	//线程函数的函数体是 MyCreateThread(" ", nullptr, 4); MyPauseThread(); while(true){MyCreateThread($$tpi, nullptr, 4); MyPauseThread()}; return 0
	CSyntaxTree* newThreadFuncBody = new CSyntaxTree();
	//MyCreateThread(" ", nullptr, 4)
	CSyntaxNode* actualPar = new CSyntaxNode(OPTION_EXP, new CSyntaxNode(NULL_EXP, VOIDTYPE),VOIDTYPE);
	CSyntaxNode* createThread = __MyCreateThread("", actualPar, 4);
	newThreadFuncBody->MergeTree(createThread, CHOP_STA, VOIDTYPE);
	//MyPauseThread()
	newThreadFuncBody->MergeTree(__CallPauseThread(), CHOP_STA, VOIDTYPE);
	//while(true){MyCreateThread($$tpi, nullptr, 4); MyPauseThread()}
	CSyntaxNode* whileStmt = new CSyntaxNode(WHILE_DO_STA, VOIDTYPE);
	whileStmt->SetChild0(new CSyntaxNode(TRUE_EXP, BOOLTYPE));
	whileStmt->SetChild1(new CSyntaxNode(CHOP_STA, VOIDTYPE));
	CSyntaxNode* threadFuncActualPar = new CSyntaxNode(OPTION_EXP, new CSyntaxNode(IDENT_EXP, "p", nullptr, VOIDTYPE), VOIDTYPE);
	whileStmt->GetChild1()->SetChild0(__MyCreateThread("$$tp" + __Int2Str(m_NewFuncNum), threadFuncActualPar, 4));
	whileStmt->GetChild1()->SetChild1(__CallPauseThread());
	newThreadFuncBody->MergeTree(whileStmt, CHOP_STA, VOIDTYPE);
	CSyntaxNode* returnStmt = new CSyntaxNode(RETURN_STA, new CSyntaxNode(INTEGER_EXP, 0, INTTYPE), VOIDTYPE);
	newThreadFuncBody->MergeTree(returnStmt, CHOP_STA, VOIDTYPE);
	newThreadFunc->SetChild1(newThreadFuncBody->GetRoot());
	newThreadFunc->SetChild2(m_ThreadFuncTree);
	m_ThreadFuncTree = newThreadFunc;
	//定义线程函数"$$tpi"
	__DefThreadFunc("$$tp" + __Int2Str(m_NewFuncNum), pFuncName, funcActualPar);
}
//-----------------------------------------------------------------------------
void CMSV::__DefThreadFuncForKeep(string pThreadFuncName, string pFuncName, CSyntaxNode* funcActualPar)
{
	g_ThreadFuncName.insert(pThreadFuncName);//将线程函数的函数名存到set中
	CSyntaxNode* newThreadFunc = new CSyntaxNode(FUNCTION_DEFINE_STA, pThreadFuncName, nullptr, VOIDTYPE);
	//线程函数的返回值类型是unsigned int
	newThreadFunc->SetChild0(new CSyntaxNode(FUNC_RETURN_TYPE, VOIDTYPE));
	newThreadFunc->GetChild0()->SetChild0(new CSyntaxNode(FORMAL_PARAMETER_EXP, UINTTYPE));
	newThreadFunc->GetChild0()->GetChild0()->SetChild0(new CSyntaxNode(IDENT_EXP, "", nullptr, VOIDTYPE));
	//线程函数的形参是 void *p
	CSyntaxNode* threadFuncFormalPar = new CSyntaxNode(FORMAL_PARAMETER_EXP, VOIDTYPE);
	threadFuncFormalPar->SetChild0(new CSyntaxNode(POINTER_EXP, VOIDTYPE));
	threadFuncFormalPar->GetChild0()->SetChild0(new CSyntaxNode(IDENT_EXP, "p", nullptr, VOIDTYPE));
	newThreadFunc->GetChild0()->SetChild1(threadFuncFormalPar);
	//线程函数的函数体是 MyCreateThread(" ", nullptr, 3); MyPauseThread(); while(true){MyCreateThread($$tpi, nullptr, 3); MyPauseThread()}; return 0
	CSyntaxTree* newThreadFuncBody = new CSyntaxTree();
	//MyCreateThread(" ", nullptr, 3)
	CSyntaxNode* actualPar = new CSyntaxNode(OPTION_EXP, new CSyntaxNode(NULL_EXP, VOIDTYPE), VOIDTYPE);
	CSyntaxNode* createThread = __MyCreateThread("", actualPar, 3);
	newThreadFuncBody->MergeTree(createThread, CHOP_STA, VOIDTYPE);
	//MyPauseThread()
	newThreadFuncBody->MergeTree(__CallPauseThread(), CHOP_STA, VOIDTYPE);
	//while(true){MyCreateThread($$tpi, nullptr, 3); MyPauseThread()}
	CSyntaxNode* whileStmt = new CSyntaxNode(WHILE_DO_STA, VOIDTYPE);
	whileStmt->SetChild0(new CSyntaxNode(TRUE_EXP, BOOLTYPE));
	whileStmt->SetChild1(new CSyntaxNode(CHOP_STA, VOIDTYPE));
	CSyntaxNode* threadFuncActualPar = new CSyntaxNode(OPTION_EXP, new CSyntaxNode(IDENT_EXP, "p", nullptr, VOIDTYPE), VOIDTYPE);
	whileStmt->GetChild1()->SetChild0(__MyCreateThread("$$tp" + __Int2Str(m_NewFuncNum), threadFuncActualPar, 3));
	whileStmt->GetChild1()->SetChild1(__CallPauseThread());
	newThreadFuncBody->MergeTree(whileStmt, CHOP_STA, VOIDTYPE);
	CSyntaxNode* returnStmt = new CSyntaxNode(RETURN_STA, new CSyntaxNode(INTEGER_EXP, 0, INTTYPE), VOIDTYPE);
	newThreadFuncBody->MergeTree(returnStmt, CHOP_STA, VOIDTYPE);
	newThreadFunc->SetChild1(newThreadFuncBody->GetRoot());
	newThreadFunc->SetChild2(m_ThreadFuncTree);
	m_ThreadFuncTree = newThreadFunc;
	//定义线程函数"$$tpi"
	__DefThreadFunc("$$tp" + __Int2Str(m_NewFuncNum), pFuncName, funcActualPar);
}
//-----------------------------------------------------------------------------
CSyntaxNode* CMSV::__GetFuncFormalPar(CSyntaxNode* pTree)
{
	CSyntaxNode* formalParList = nullptr;
	//将新结构体类型的一个变量加入到函数参数中
	CSyntaxNode* par = new CSyntaxNode(FORMAL_PARAMETER_EXP, "$$S" + __Int2Str(m_NewStructNum), nullptr, STRUCTTYPE);
	par->SetChild0(new CSyntaxNode(POINTER_EXP, VOIDTYPE));
	par->GetChild0()->SetChild0(new CSyntaxNode(IDENT_EXP, "$$par", nullptr, VOIDTYPE));
	par->SetChild1(formalParList);
	formalParList = par;
	if (pTree->GetChild0() != nullptr)
	{	
		CSyntaxNode* visit = pTree->GetChild0();
		while (visit->GetNType() == AND_EXP)
		{
			RETURNTYPE returnType = visit->GetChild0()->GetRType();
			string structName = visit->GetChild0()->GetNName();//如果不是结构体类型structName为空
			CSyntaxNode* visit_1 = visit->GetChild0()->GetChild0();
			while (visit_1 != nullptr)
			{
				CSyntaxNode* par = new CSyntaxNode(FORMAL_PARAMETER_EXP, structName, visit_1->GetChild0(), nullptr, returnType);
				par->SetChild1(formalParList);
				formalParList = par;
				visit_1 = visit_1->GetChild1();
			}

			visit = visit->GetChild1();
		}

		RETURNTYPE returnType = visit->GetRType();
		string structName = visit->GetNName();
		CSyntaxNode* visit_1 = visit->GetChild0();
		while (visit_1 != nullptr)
		{
			CSyntaxNode* par = new CSyntaxNode(FORMAL_PARAMETER_EXP, structName, visit_1->GetChild0(), nullptr, returnType);
			par->SetChild1(formalParList);
			formalParList = par;
			visit_1 = visit_1->GetChild1();
		}
		//将所有ARRAY_DECLARE_STA类型改为ARRAY_PARAMETER_EXP类型
		visit = formalParList;
		while (visit != nullptr)
		{
			visit_1 = visit->GetChild0();
			while (visit_1 != nullptr)
			{
				if (visit_1->GetNType() == ARRAY_DECLARE_STA)
				{
					visit_1->SetNType(ARRAY_DECLARE_STA);
				}
				visit_1 = visit_1->GetChild0();
			}
			visit = visit->GetChild1();
		}

		return formalParList;
	}
	else
	{
		return nullptr;
	}
}
//-----------------------------------------------------------------------------
CSyntaxNode* CMSV::__GetFuncActualPar(CSyntaxNode* pFormalPar)
{
	if (pFormalPar == nullptr)
	{
		//没有形参，直接返回
		return nullptr;
	}
	//得到要强制转换到的结构体类型的结构体名
	string structName = "";
	if (!g_InNewFunc)
	{
		//$$Si是最新生成的结构体的名字
		structName = "$$S" + __Int2Str(m_NewStructNum);
	}
	else
	{
		/*$$Si要从pFormalPar的最后一个参数中获得，此时pFormalPar最后一个参数
		是一个结构类型，该结构体就是我们需要的结构体类型*/
		CSyntaxNode* visit = pFormalPar;
		while (visit != nullptr && visit->GetChild1()!=nullptr)
		{
			visit = visit->GetChild1();
		}
		if (visit->GetRType() == STRUCTTYPE)
		{
			structName = visit->GetNName();
		}
		else
		{
			PrintErrorMassage(0, "__GetFuncActualPar error");//不是结构体类型则报错
		}
	}
	CSyntaxNode* actualPar = new CSyntaxNode(OPTION_EXP, VOIDTYPE);
	CSyntaxNode* actualParList = actualPar;
	CSyntaxNode* visit = pFormalPar;
	while (visit != nullptr)
	{
		string varName = visit->GetChild0()->GetNameNode()->GetNName();
        /*最后一个参数是额外增加的参数，主要是为了记录结构体类型以便于递归
		前n - 1个实参需要变为($$Si*)p->x的形式，其中$$Si是最新生成的结构体的名字；
		第n个参数需要变为($$Si*)p的形式*/
		if (visit->GetChild1() != nullptr)
		{
	    	actualPar->SetChild0(new CSyntaxNode(ARROW_EXP, VOIDTYPE));
			actualPar->GetChild0()->SetChild0(new CSyntaxNode(TYPE_CAST_STA, VOIDTYPE));
			actualPar->GetChild0()->GetChild0()->SetChild0(new CSyntaxNode(CAST_TYPE, structName, nullptr, STRUCTTYPE));
			actualPar->GetChild0()->GetChild0()->GetChild0()->SetChild0(new CSyntaxNode(POINTER_EXP, VOIDTYPE));
			actualPar->GetChild0()->GetChild0()->SetChild1(new CSyntaxNode(IDENT_EXP, "p", nullptr, VOIDTYPE));
			actualPar->GetChild0()->SetChild1(new CSyntaxNode(IDENT_EXP, varName, nullptr, VOIDTYPE));	
			actualPar->SetChild1(new CSyntaxNode(OPTION_EXP, VOIDTYPE));
			actualPar = actualPar->GetChild1();
		}
		else
		{
			actualPar->SetChild0(new CSyntaxNode(TYPE_CAST_STA, VOIDTYPE));
			actualPar->GetChild0()->SetChild0(new CSyntaxNode(CAST_TYPE, structName, nullptr, STRUCTTYPE));
			actualPar->GetChild0()->GetChild0()->SetChild0(new CSyntaxNode(POINTER_EXP, VOIDTYPE));
			actualPar->GetChild0()->SetChild1(new CSyntaxNode(IDENT_EXP, "p", nullptr, VOIDTYPE));
		}
		visit = visit->GetChild1();
	}
	return actualParList;
}
//-----------------------------------------------------------------------------
CSyntaxNode* CMSV::__DefStruct(CSyntaxNode* pLocalVar, CSyntaxNode* pFuncFormalPar)//处理复杂并行语句
{
	if (pLocalVar != nullptr || pFuncFormalPar != nullptr)
	{
		CSyntaxNode* newStruct = new CSyntaxNode(STRUCT_DEFINE_STA, "$$S" + __Int2Str(m_NewStructNum), nullptr, VOIDTYPE);
		CSyntaxTree* structMember = new CSyntaxTree();
		if (pLocalVar != nullptr)
		{
			CSyntaxNode* visit = pLocalVar;
			while (visit->GetNType() == CHOP_STA)
			{
				RETURNTYPE type = visit->GetChild1()->GetRType();
				CSyntaxNode* visit_1 = visit->GetChild1()->GetChild0();
				while (visit_1 != nullptr)
				{
					if (visit_1->GetChild0()->GetNType() == ARRAY_DECLARE_STA)
					{
						//数组变成数组指针
						CSyntaxNode* newDec = new CSyntaxNode(DECLARE_STA, type);
						newDec->SetChild0(new CSyntaxNode(DEC_VAR_LIST, VOIDTYPE));
						newDec->GetChild0()->SetChild0(new CSyntaxNode(ARRAY_DECLARE_STA, VOIDTYPE));
						newDec->GetChild0()->GetChild0()->SetChild0(new CSyntaxNode(POINTER_EXP, VOIDTYPE));
						newDec->GetChild0()->GetChild0()->GetChild0()->SetChild0(visit_1->GetChild0()->GetChild0());
						newDec->GetChild0()->GetChild0()->SetChild1(visit_1->GetChild0()->GetChild1());

						structMember->MergeTree_2(newDec, AND_EXP, VOIDTYPE);
						visit_1 = visit_1->GetChild1();
					}
					else
					{
						//在每个变量前加“*”
						CSyntaxNode* newDec = new CSyntaxNode(DECLARE_STA, type);
						newDec->SetChild0(new CSyntaxNode(DEC_VAR_LIST, VOIDTYPE));
						newDec->GetChild0()->SetChild0(new CSyntaxNode(POINTER_EXP, VOIDTYPE));
						newDec->GetChild0()->GetChild0()->SetChild0(visit_1->GetChild0());
						structMember->MergeTree_2(newDec, AND_EXP, VOIDTYPE);
						visit_1 = visit_1->GetChild1();
					}
				}
				visit = visit->GetChild0();
			}
			RETURNTYPE type = visit->GetRType();
			visit = visit->GetChild0();
			while (visit != nullptr)
			{
				if (visit->GetChild0()->GetNType() == ARRAY_DECLARE_STA)
				{
					//数组变成数组指针
					CSyntaxNode* newDec = new CSyntaxNode(DECLARE_STA, type);
					newDec->SetChild0(new CSyntaxNode(DEC_VAR_LIST, VOIDTYPE));
					newDec->GetChild0()->SetChild0(new CSyntaxNode(ARRAY_DECLARE_STA, VOIDTYPE));
					newDec->GetChild0()->GetChild0()->SetChild0(new CSyntaxNode(POINTER_EXP, VOIDTYPE));
					newDec->GetChild0()->GetChild0()->GetChild0()->SetChild0(visit->GetChild0()->GetChild0());
					newDec->GetChild0()->GetChild0()->SetChild1(visit->GetChild0()->GetChild1());

					structMember->MergeTree_2(newDec, AND_EXP, VOIDTYPE);
					visit = visit->GetChild1();
				}
				else
				{
					//在每个变量前加“*”
					CSyntaxNode* newDec = new CSyntaxNode(DECLARE_STA, type);
					newDec->SetChild0(new CSyntaxNode(DEC_VAR_LIST, VOIDTYPE));
					newDec->GetChild0()->SetChild0(new CSyntaxNode(POINTER_EXP, VOIDTYPE));
					newDec->GetChild0()->GetChild0()->SetChild0(visit->GetChild0());
					structMember->MergeTree_2(newDec, AND_EXP, VOIDTYPE);
					visit = visit->GetChild1();
				}
			}
		}
		if (pFuncFormalPar != nullptr)
		{
			CSyntaxNode* visit = pFuncFormalPar;
			while (visit != nullptr)
			{
				//在每个变量前加“*”
				CSyntaxNode* newDec = new CSyntaxNode(DECLARE_STA, visit->GetNName(), nullptr, visit->GetRType());
				newDec->SetChild0(new CSyntaxNode(DEC_VAR_LIST, VOIDTYPE));
				newDec->GetChild0()->SetChild0(new CSyntaxNode(POINTER_EXP, VOIDTYPE));
				newDec->GetChild0()->GetChild0()->SetChild0(visit->GetChild0());
				structMember->MergeTree_2(newDec, AND_EXP, VOIDTYPE);
				visit = visit->GetChild1();
			}
		}
		newStruct->SetChild0(structMember->GetRoot());
		//将新结构体加到结构体树上
		newStruct->SetChild1(struct_tree);
		struct_tree = newStruct;
		return newStruct;
	}
	else
	{
		return nullptr;
	}
}
//-----------------------------------------------------------------------------
void CMSV::__InitStruct(CSyntaxNode* pTree)
{
	//$$S *$$s; 
	//构造声明语句 $$S *$$s; $$s=($$S*)malloc(sizeof($$S)); $$s->x=&x; $$s->y=&y
	CSyntaxNode* structPointerVarDec = new CSyntaxNode(DECLARE_STA, pTree->GetNName(), nullptr, STRUCTTYPE);
	structPointerVarDec->SetChild0(new CSyntaxNode(DEC_VAR_LIST, VOIDTYPE));
	structPointerVarDec->GetChild0()->SetChild0(new CSyntaxNode(POINTER_EXP, nullptr, VOIDTYPE));
	string name = "$$s" + __Int2Str(m_NewStructNum);
	structPointerVarDec->GetChild0()->GetChild0()->SetChild0(new CSyntaxNode(IDENT_EXP, name, nullptr, VOIDTYPE));
	m_IRTree->MergeTree(structPointerVarDec, CHOP_STA, VOIDTYPE);
	//构造赋值语句 $$s=($$S*)malloc(sizeof($$S));
	CSyntaxNode* assStmt = new CSyntaxNode(ASS_EQU_EXP, VOIDTYPE);
	assStmt->SetChild0(new CSyntaxNode(IDENT_EXP, name, nullptr, VOIDTYPE));
	assStmt->SetChild1(new CSyntaxNode(TYPE_CAST_STA, VOIDTYPE));
	assStmt->GetChild1()->SetChild0(new CSyntaxNode(CAST_TYPE, pTree->GetNName(), nullptr, STRUCTTYPE));
	assStmt->GetChild1()->GetChild0()->SetChild0(new CSyntaxNode(POINTER_EXP, nullptr, VOIDTYPE));
	assStmt->GetChild1()->SetChild1(new CSyntaxNode(FUNCTION_STA, VOIDTYPE));
	assStmt->GetChild1()->GetChild1()->SetChild0(new CSyntaxNode(IDENT_EXP, "malloc", nullptr, VOIDTYPE));
	assStmt->GetChild1()->GetChild1()->SetChild1(new CSyntaxNode(OPTION_EXP, VOIDTYPE));
	assStmt->GetChild1()->GetChild1()->GetChild1()->SetChild0(new CSyntaxNode(SIZEOF_EXP, pTree->GetNName(), nullptr, STRUCTTYPE));

	m_IRTree->MergeTree(assStmt, CHOP_STA, VOIDTYPE);
	//结构体成员初始化,如$$s->x=&x; $$s->y=&y
	CSyntaxNode* visit = pTree->GetChild0();
	while (visit->GetNType() == AND_EXP)
	{
		string memberName = visit->GetChild0()->GetNameNode()->GetNName();
		m_LocalVarName.insert(memberName);//将局部变量名存起来，后面加指针时要用到,只给局部变量前加*
		assStmt = new CSyntaxNode(ASS_EQU_EXP, VOIDTYPE);
		assStmt->SetChild0(new CSyntaxNode(ARROW_EXP, VOIDTYPE));
		assStmt->GetChild0()->SetChild0(new CSyntaxNode(IDENT_EXP, name, nullptr, VOIDTYPE));
		assStmt->GetChild0()->SetChild1(new CSyntaxNode(IDENT_EXP, memberName, nullptr, VOIDTYPE));
		assStmt->SetChild1(new CSyntaxNode(ADDRESS_EXP, VOIDTYPE));
		assStmt->GetChild1()->SetChild0(new CSyntaxNode(IDENT_EXP, memberName, nullptr, VOIDTYPE));
		m_IRTree->MergeTree(assStmt, CHOP_STA, VOIDTYPE);
		visit = visit->GetChild1();
	}
	string memberName = visit->GetNameNode()->GetNName();
	m_LocalVarName.insert(memberName);//将局部变量名存起来，后面加指针时要用到,只给局部变量前加*
	assStmt = new CSyntaxNode(ASS_EQU_EXP, VOIDTYPE);
	assStmt->SetChild0(new CSyntaxNode(ARROW_EXP, VOIDTYPE));
	assStmt->GetChild0()->SetChild0(new CSyntaxNode(IDENT_EXP, name, nullptr, VOIDTYPE));
	assStmt->GetChild0()->SetChild1(new CSyntaxNode(IDENT_EXP, memberName, nullptr, VOIDTYPE));
	assStmt->SetChild1(new CSyntaxNode(ADDRESS_EXP, VOIDTYPE));
	assStmt->GetChild1()->SetChild0(new CSyntaxNode(IDENT_EXP, memberName, nullptr, VOIDTYPE));
	m_IRTree->MergeTree(assStmt, CHOP_STA, VOIDTYPE);
}

//-----------------------------------------------------------------------------
void CMSV::__Prj(CSyntaxNode* pTree)
{
	if (pTree->GetChild0() == nullptr || pTree->GetChild2() == nullptr)
	{
		PrintErrorMassage(0, "projection statement syntax error!");
		m_RdcOk = 0;
		return;
	}
	if (Setting::IsSimulation())
	{
		__DelLastStateOutput();
	}
	else if (Setting::IsModeling())
	{
		__DelLastAddEdge();
		CSyntaxNode* getStatementNum = new CSyntaxNode(FUNCTION_STA, VOIDTYPE);
		getStatementNum->SetChild0(new CSyntaxNode(IDENT_EXP, "addStatement", nullptr, VOIDTYPE));
		getStatementNum->SetChild1(new CSyntaxNode(OPTION_EXP, new CSyntaxNode(INTEGER_EXP, m_StatementNum, VOIDTYPE), VOIDTYPE));
		m_StatementNum++;
		m_IRTree->MergeTree(getStatementNum, CHOP_STA, VOIDTYPE);
	}
	__ComplexPrj(pTree);
	m_RdcOk = 1;
	m_More = 0;
}
//-----------------------------------------------------------------------------
void CMSV::__PrjNotInFunc(CSyntaxNode* pTree)
{
	CSyntaxNode* threadFuncActualPar = nullptr;//线程函数的形参
	CSyntaxNode* funcFormalPar = nullptr;//新定义函数的形参
	CSyntaxNode* funcActualPar = nullptr;//新定义函数的实参
	string funcName = "";//新定义函数的函数名
	string threadFuncName = "";//新定义函数的函数名
	CSyntaxNode* createThread = nullptr;//新创建的线程
	//先处理投影的右边
	funcName = "$$f" + __Int2Str(m_NewFuncNum);
	threadFuncName = "$$t" + __Int2Str(m_NewFuncNum);
	__DefFunc(funcName, nullptr, pTree->GetChild2());//封装函数
	pTree->SetChild2(nullptr);
	__DefThreadFunc(threadFuncName, funcName, nullptr);//定义线程函数	
	threadFuncActualPar = new CSyntaxNode(OPTION_EXP, VOIDTYPE);
	threadFuncActualPar->SetChild0(new CSyntaxNode(NULL_EXP, VOIDTYPE));
	createThread = __MyCreateThread(threadFuncName, threadFuncActualPar, 2);//创建线程
	m_IRTree->MergeTree(createThread, CHOP_STA, VOIDTYPE);
	m_NewFuncNum++;

	CSyntaxNode* visit = pTree;
	while (visit != nullptr)
	{
		if (visit->GetChild0()->GetNType() != PROJECTION_PLUS_STA)
		{
			funcName = "$$f" + __Int2Str(m_NewFuncNum);
			__DefFunc(funcName, nullptr, visit->GetChild0());//封装函数
			visit->SetChild0(nullptr);
			__DefThreadFunc("$$t" + __Int2Str(m_NewFuncNum), funcName, nullptr);//定义线程函数	
			threadFuncActualPar = new CSyntaxNode(OPTION_EXP, VOIDTYPE);
			threadFuncActualPar->SetChild0(new CSyntaxNode(NULL_EXP, VOIDTYPE));
			createThread = __MyCreateThread("$$t" + __Int2Str(m_NewFuncNum), threadFuncActualPar, 2);//创建线程	
			m_IRTree->MergeTree(createThread, CHOP_STA, VOIDTYPE);
			m_NewFuncNum++;
		}
		else
		{
			CSyntaxTree* tempThreadTree = new CSyntaxTree();
			CSyntaxNode* visit_1 = visit->GetChild0();
			while (visit_1 != nullptr)
			{
				funcName = "$$f" + __Int2Str(m_NewFuncNum);
				__DefFunc(funcName, nullptr, visit_1->GetChild0());//封装函数
				visit_1->SetChild0(nullptr);
				__DefThreadFunc("$$t" + __Int2Str(m_NewFuncNum), funcName, nullptr);//定义线程函数
				threadFuncActualPar = new CSyntaxNode(OPTION_EXP, VOIDTYPE);
				threadFuncActualPar->SetChild0(new CSyntaxNode(NULL_EXP, VOIDTYPE));
				createThread = __MyCreateThread("$$t" + __Int2Str(m_NewFuncNum), threadFuncActualPar, 2);//创建线程	
				tempThreadTree->MergeTree(createThread, CHOP_STA, VOIDTYPE);
				visit_1 = visit_1->GetChild1();
				m_NewFuncNum++;
			}

			int n = pTree->GetChild0()->GetChild2()->GetiValue();//暂时只考虑n是常量的情况
			pTree->GetChild0()->SetChild2(nullptr);
			for (int i = 0; i < n; i++)
			{
				m_IRTree->MergeTree(tempThreadTree->GetRoot(), CHOP_STA, VOIDTYPE);
				m_NewFuncNum++;
			}
			visit->GetChild0()->SetChild0(nullptr);
		}
		visit = visit->GetChild1();
	}
	__MyWaitForObject();

	CSyntaxNode* ifStmt = new CSyntaxNode(IF_ELSE_STA, VOIDTYPE);
	ifStmt->SetChild0(new CSyntaxNode(EQU_EXP, VOIDTYPE));
	ifStmt->GetChild0()->SetChild0(new CSyntaxNode(IDENT_EXP, "$$state_num", nullptr, VOIDTYPE));
	ifStmt->GetChild0()->SetChild1(new CSyntaxNode(INTEGER_EXP, -1, INTTYPE));
	ifStmt->SetChild1(new CSyntaxNode(FUNCTION_STA, new CSyntaxNode(IDENT_EXP, "$$MyLongJmp", nullptr, VOIDTYPE), VOIDTYPE));
	m_IRTree->MergeTree(ifStmt, CHOP_STA, VOIDTYPE);
}
//-----------------------------------------------------------------------------
void CMSV::__PrjInFunc(CSyntaxNode* pTree)
{
	CSyntaxNode* threadFuncActualPar = nullptr;//线程函数的形参
	CSyntaxNode* funcFormalPar = nullptr;//新定义函数的形参
	CSyntaxNode* funcActualPar = nullptr;//新定义函数的实参
	string funcName = "";//新定义函数的函数名
	string threadFuncName = "";//新定义函数的函数名
	CSyntaxNode* createThread = nullptr;//新创建的线程
	CSyntaxNode* newStruct = nullptr;//新定义的结构体
	if (!g_InNewFunc || m_CurrentFunc.substr(0, 7) == "$$user_")
	{
		//根据当前函数的形参和局部变量定义一个新的结构体，并对新结构体的成员进行初始化
		newStruct = __DefStruct(m_LocVarTree->GetRoot(), m_FuncFormalPar);
		if (newStruct != nullptr)
		{
			__InitStruct(newStruct);
			//由新结构体得到新定义的函数的形参，前n-1个形参是结构体的成员，最后一个形参是结构体指针类型的一个变量
			funcFormalPar = __GetFuncFormalPar(newStruct);
			//线程函数实参为(void*)$$si,其中$$si为新定义的结构体类型的一个指针型变量
			threadFuncActualPar = new CSyntaxNode(OPTION_EXP, VOIDTYPE);
			threadFuncActualPar->SetChild0(new CSyntaxNode(TYPE_CAST_STA, VOIDTYPE));
			threadFuncActualPar->GetChild0()->SetChild0(new CSyntaxNode(CAST_TYPE, VOIDTYPE));
			threadFuncActualPar->GetChild0()->GetChild0()->SetChild0(new CSyntaxNode(POINTER_EXP, VOIDTYPE));
			threadFuncActualPar->GetChild0()->SetChild1(new CSyntaxNode(IDENT_EXP, "$$s" + __Int2Str(m_NewStructNum), nullptr, VOIDTYPE));
		}
		else
	    {
			funcFormalPar = nullptr;
			threadFuncActualPar = new CSyntaxNode(OPTION_EXP, VOIDTYPE);//线程函数的参数，默认为nullptr
	 	    threadFuncActualPar->SetChild0(new CSyntaxNode(NULL_EXP, VOIDTYPE));
	    }
	}
	else
	{
		//新定义函数的形参和当前函数的形参一样（m_FuncFormalPar记录的是当前正在处理的函数的形参）
		funcFormalPar = m_FuncFormalPar;
		/*m_FuncFormalPar=nullptr时，当前函数没有形参，则线程函数的参数为nullptr，否则线程函数的实参为$$par（和当前函数
		的最后一个形参名一致）*/
		if (m_FuncFormalPar != nullptr)
		{
			threadFuncActualPar = new CSyntaxNode(OPTION_EXP, VOIDTYPE);
			threadFuncActualPar->SetChild0(new CSyntaxNode(IDENT_EXP, "$$par", nullptr, VOIDTYPE));
		}
		else
		{
			threadFuncActualPar = new CSyntaxNode(OPTION_EXP, VOIDTYPE);//线程函数的参数，默认为nullptr
			threadFuncActualPar->SetChild0(new CSyntaxNode(NULL_EXP, VOIDTYPE));
		}
	}
	funcActualPar = __GetFuncActualPar(funcFormalPar);//根据形参名得到新函数的实参
	//先处理投影的右边
	funcName = "$$f" + __Int2Str(m_NewFuncNum);
	__DefFunc(funcName, funcFormalPar, pTree->GetChild2());//封装函数
	pTree->SetChild2(nullptr);
	__DefThreadFunc("$$t" + __Int2Str(m_NewFuncNum), funcName, funcActualPar);//定义线程函数
	CSyntaxNode* copyThreadFuncActualPar = new CSyntaxNode();
	copyThreadFuncActualPar->CopyTree(threadFuncActualPar);
	createThread = __MyCreateThread("$$t" + __Int2Str(m_NewFuncNum), copyThreadFuncActualPar, 2);//创建线程
	m_IRTree->MergeTree(createThread, CHOP_STA, VOIDTYPE);
	m_NewFuncNum++;

	CSyntaxNode* visit = pTree;
	while (visit != nullptr)
	{
		if (visit->GetChild0()->GetNType() != PROJECTION_PLUS_STA)
		{
			funcName = "$$f" + __Int2Str(m_NewFuncNum);
			__DefFunc(funcName, m_FuncFormalPar, visit->GetChild0());//封装函数
			visit->SetChild0(nullptr);
			__DefThreadFunc("$$t" + __Int2Str(m_NewFuncNum), funcName, funcActualPar);//定义线程函数	
			CSyntaxNode* copyThreadFuncActualPar = new CSyntaxNode();
			copyThreadFuncActualPar->CopyTree(threadFuncActualPar);
			createThread = __MyCreateThread("$$t" + __Int2Str(m_NewFuncNum), copyThreadFuncActualPar, 2);//创建线程	
			m_IRTree->MergeTree(createThread, CHOP_STA, VOIDTYPE);
			m_NewFuncNum++;
		}
		else
		{
			CSyntaxTree* tempThreadTree = new CSyntaxTree();
			CSyntaxNode* visit_1 = visit->GetChild0();
			while (visit_1 != nullptr)
			{
				funcName = "$$f" + __Int2Str(m_NewFuncNum);
				__DefFunc(funcName, m_FuncFormalPar, visit_1->GetChild0());//封装函数
				visit_1->SetChild0(nullptr);
				__DefThreadFunc("$$t" + __Int2Str(m_NewFuncNum), funcName, funcActualPar);//定义线程函数
				CSyntaxNode* copyThreadFuncActualPar = new CSyntaxNode();
				copyThreadFuncActualPar->CopyTree(threadFuncActualPar);
				createThread = __MyCreateThread("$$t" + __Int2Str(m_NewFuncNum), copyThreadFuncActualPar, 2);//创建线程	
				tempThreadTree->MergeTree(createThread, CHOP_STA, VOIDTYPE);
				visit_1 = visit_1->GetChild1();
				m_NewFuncNum++;
			}

			int n = pTree->GetChild0()->GetChild2()->GetiValue();//暂时只考虑n是常量的情况
			pTree->GetChild0()->SetChild2(nullptr);
			for (int i = 0; i < n; i++)
			{
				m_IRTree->MergeTree(tempThreadTree->GetRoot(), CHOP_STA, VOIDTYPE);
				m_NewFuncNum++;
			}
			visit->GetChild0()->SetChild0(nullptr);
		}
		visit = visit->GetChild1();
	}	
	delete threadFuncActualPar;
	threadFuncActualPar = nullptr;
	__MyWaitForObject();//MyWaitForObject()
	__MyFree("$$s" + __Int2Str(m_NewStructNum));//free($$s)
	m_NewStructNum++;
}
//-----------------------------------------------------------------------------
void CMSV::__MyFree(const string pVarName)
{
	if (!g_InNewFunc)
	{
		CSyntaxNode* free = new CSyntaxNode(FUNCTION_STA, VOIDTYPE);
		free->SetChild0(new CSyntaxNode(IDENT_EXP, "free", nullptr, VOIDTYPE));
		free->SetChild1(new CSyntaxNode(OPTION_EXP, VOIDTYPE));
		free->GetChild1()->SetChild0(new CSyntaxNode(IDENT_EXP, pVarName, nullptr, VOIDTYPE));
		m_IRTree->MergeTree(free, CHOP_STA, VOIDTYPE);
	}
}
//-----------------------------------------------------------------------------
void CMSV::__MyWaitForObject()
{
	if (!g_InNewFunc)
	{   //MyWaitForObject和CreateThread(MangerThread, nullptr, 5) 对应，有MyWaitForObject时则不用__CallPauseThread()
		CSyntaxNode* waitFor = new CSyntaxNode(FUNCTION_STA, VOIDTYPE);
		waitFor->SetChild0(new CSyntaxNode(IDENT_EXP, MYWAITFOROBJECT, nullptr, VOIDTYPE));
		m_IRTree->MergeTree(waitFor, CHOP_STA, VOIDTYPE);
	}
	else
	{
		//每次创建完线程之后暂停
		m_IRTree->MergeTree(__CallPauseThread(), CHOP_STA, VOIDTYPE);
	}
}
//-----------------------------------------------------------------------------
void CMSV::__ComplexPrj(CSyntaxNode* pTree)
{
	//创建调度线程
	if (!g_InNewFunc)
	{
		CSyntaxNode* threadFuncActualPar = new CSyntaxNode(OPTION_EXP, VOIDTYPE);
		threadFuncActualPar->SetChild0(new CSyntaxNode(NULL_EXP, VOIDTYPE));
		CSyntaxNode* createThread = __MyCreateThread("ManagerThread", threadFuncActualPar, 5);
		m_IRTree->MergeTree(createThread, CHOP_STA, VOIDTYPE);
	}
	if (!g_InFunc)
	{
		__PrjNotInFunc(pTree);
	}
	else
	{
		__PrjInFunc(pTree);
	}
}
//-----------------------------------------------------------------------------
void CMSV::__DelLastStateOutput()
{
	if (m_IRTree->GetRoot() != nullptr)
	{
		if (m_IRTree->GetRoot()->GetNType() == STATE_OUTPUT_STA || m_IRTree->GetRoot()->GetNType()==GENERATE_EDGE_STA)
		{
			//m_IRTree只有一个输出语句，则直接删掉
			delete m_IRTree->GetRoot();
			m_IRTree->SetRoot(nullptr);
		}
		else if (m_IRTree->GetRoot()->GetNType() == CHOP_STA)
		{
			//m_IRTree根节点是chop时，chop的右子树一定是最后一个输出语句，直接删掉右子树
			if (m_IRTree->GetRoot()->GetChild1()->GetNType() == STATE_OUTPUT_STA || m_IRTree->GetRoot()->GetChild1()->GetNType() == GENERATE_EDGE_STA)
			{
				CSyntaxNode* temp = m_IRTree->GetRoot();
				m_IRTree->SetRoot(temp->GetChild0());
				temp->SetChild0(nullptr);
				delete temp;
			}
		}
	}
}
//-----------------------------------------------------------------------------
void CMSV::__Chop(CSyntaxNode* pTree)
{
	if (pTree->GetChild0() == nullptr || pTree->GetChild1() == nullptr)
	{
		PrintErrorMassage(0, "__Chop() syntax tree error!");
		m_RdcOk = 0;
		return;
	}
	GetPresent(pTree->GetChild0());///处理chop左边
	if (m_RdcOk == 0)///处理左边出现错误，直接返回
	{
		PrintErrorMassage(0, "__Chop() handles child 0 error!");
		return;
	}
	if (m_More == -1)
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED);
		PrintErrorMassage(0, "The interval length is not specified: " + pTree->GetChild1()->TreeToString());
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY);
		g_HasErr++;
		return;
	}
	if (m_More == 0) ///处理左边到最后一个状态，处理右边
	{
		if (__IsComplexStmt(pTree->GetChild1()))//chop右边是复杂语句时，先将左边加到当前状态后再处理右边
		{
			__SortPrsnt();
			__AdPrst2IR();
		}
		m_More = -1;
		m_RdcOk = -1;
		GetPresent(pTree->GetChild1());
	}
	else if (m_More == 1)
	{
		if (m_PrgmFuture->MergeTree(pTree->GetChild1(), CHOP_STA, VOIDTYPE))
		{
			pTree->SetChild1(nullptr);
			m_RdcOk = 1;
		}
		else
		{
			PrintErrorMassage(0, "__Chop() MergeTree() error!" );
			m_RdcOk = 0;
		}
	}
	else if (m_More == -1)
	{
		PrintErrorMassage(0, "__Chop():Interval length error!");
		m_RdcOk = 0;
	}
}
//-----------------------------------------------------------------------------
bool  CMSV::__IsComplexStmt(CSyntaxNode* pTree)
{
	NODETYPE nodeType = pTree->GetNType();
	if (nodeType == IF_ELSE_STA)
		return true;
	if (nodeType == WHILE_DO_STA)
		return true;
	if (nodeType == ALWAYS_STA)
		return true;
	if (nodeType == KEEP_STA)
		return true;
	if (nodeType == AWAIT_STA)
		return true;
	if (nodeType == PROJECTION_STA)
		return true;
	if (nodeType == AND_EXP && pTree->GetiValue() == 1)
		return true;
	if (nodeType == PARALLEL_STA && pTree->GetiValue() == 1)
		return true;
	return false;
}
//-----------------------------------------------------------------------------
void CMSV::__If(CSyntaxNode* pTree)
{
	//if之前添加push语句和MyPauseThread
    if (g_InNewFunc)
	{
		CSyntaxNode* pushStmt = __ConstructPushStmt(pTree->GetChild0(), 4, true);
		m_IRTree->MergeTree(pushStmt, CHOP_STA, VOIDTYPE);
		m_IRTree->MergeTree(__CallPauseThread(), CHOP_STA, VOIDTYPE);
	}
	CSyntaxNode* If_IRTree = new CSyntaxNode(IF_ELSE_STA, VOIDTYPE);
	//add by yubin 2015/5/4,处理if条件中出现函数调用的情况
	__IfWhileCondFunc(pTree->GetChild0());
	If_IRTree->SetChild0(pTree->GetChild0());
	pTree->SetChild0(nullptr);
	///2015-3-20 add by wangmeng 保存一下符号表，在处理else的时候需要恢复。
	SymbolTable* tmpNFrm = new SymbolTable();
	tmpNFrm->CopyTable(m_NFrmSymbl);
	SymbolTable* tmpFrm = new SymbolTable();
	tmpFrm->CopyTable(m_FrmSymbl);
	///end add by wangmeng
	m_More = -1;
	m_Program->SetRoot(pTree->GetChild1());
	pTree->SetChild1(nullptr);

	CSyntaxNode* tmpIRTree = m_IRTree->GetRoot();
	m_IRTree->SetRoot(nullptr);

	///2015-3-24 add by wangmeng 保存m_FreeFrm的值
	int tmpFreeFrm = m_FreeFrm;
	m_FreeFrm = 0;
	///end add by wangmeng
	Pre();
	//并行语句中的if语句需要知道是否设置了区间长度
	if (g_InNewFunc)
	{
		if (m_IRTree->GetRoot() == nullptr)
		{   //当语法树是empty或true时，m_IRTree都为nullptr，因此添加了一个临时的声明语句，这样m_IRTree就不会为nullptr,
			//下面就可以设置标志位
			CSyntaxNode* tempDeclar = new CSyntaxNode(DECLARE_STA, INTTYPE);
			tempDeclar->SetChild0(new CSyntaxNode(DEC_VAR_LIST, VOIDTYPE));
			tempDeclar->GetChild0()->SetChild0(new CSyntaxNode(IDENT_EXP, "$$temp", nullptr, VOIDTYPE));
			m_IRTree->SetRoot(tempDeclar);
		}
		if (m_More == 0)
		{
			m_IRTree->GetRoot()->SetcValue('e');
		}
		else if (m_More == -1)
		{
			m_IRTree->GetRoot()->SetcValue('n');
		}
	}
	If_IRTree->SetChild1(m_IRTree->GetRoot());
	///2015-3-20 add by wangmeng 恢复符号表
	if (m_NFrmSymbl != nullptr)
	{
		delete m_NFrmSymbl;
	}
	if (m_FrmSymbl != nullptr)
	{
		delete m_FrmSymbl;
	}
	m_NFrmSymbl = tmpNFrm;
	m_FrmSymbl = tmpFrm;
	///end add by wangmeng
	m_FreeFrm = 0;
	m_More = -1;

	m_Program->SetRoot(pTree->GetChild2());
	pTree->SetChild2(nullptr);
	m_IRTree->SetRoot(nullptr);

	Pre();
	
	if (g_InNewFunc)
	{
		//并行语句中的if语句需要知道是否设置了区间长度
		if (m_IRTree->GetRoot() == nullptr)
		{
			CSyntaxNode* tempDeclar = new CSyntaxNode(DECLARE_STA, INTTYPE);
			tempDeclar->SetChild0(new CSyntaxNode(DEC_VAR_LIST, VOIDTYPE));
			tempDeclar->GetChild0()->SetChild0(new CSyntaxNode(IDENT_EXP, "$$temp", nullptr, VOIDTYPE));
			m_IRTree->SetRoot(tempDeclar);
		}
		if (m_More == 0)
		{
			m_IRTree->GetRoot()->SetcValue('e');
		}
		else if (m_More == -1)
		{
			m_IRTree->GetRoot()->SetcValue('n');
		}
	}
	If_IRTree->SetChild2(m_IRTree->GetRoot());

	delete pTree;
	pTree = nullptr;
	m_IRTree->SetRoot(tmpIRTree);
	m_IRTree->MergeTree(If_IRTree, CHOP_STA, VOIDTYPE);
	///恢复m_FreeFrm的值
	m_FreeFrm = tmpFreeFrm;
	m_More = 0;
	m_RdcOk = 1;
	m_IsComplexStmt = true;
}
//-----------------------------------------------------------------------------
void CMSV::__While(CSyntaxNode* pTree)
{
	
	//while之前添加push语句和MyPauseThread
	if (g_InNewFunc)
	{
		CSyntaxNode* pushStmt = __ConstructPushStmt(pTree->GetChild0(), 4, true);
		m_IRTree->MergeTree(pushStmt, CHOP_STA, VOIDTYPE);
		m_IRTree->MergeTree(__CallPauseThread(), CHOP_STA, VOIDTYPE);
	}
	CSyntaxNode* While_IRTree = new CSyntaxNode(WHILE_DO_STA, VOIDTYPE);


	if (true)
	{
		vector<string> SymTbl;
		map<string, Symbol*>::iterator iter;
		map<string, Symbol*>* Smap = m_NFrmSymbl->GetSymbolMap();
		map<string, Symbol*>* FSmap = m_FrmSymbl->GetSymbolMap();
		map<string, Symbol*>* GSmap = m_GlbSymbl->GetSymbolMap();
		string print = "";
		//函数调用时根据是内部还是外部调用判断是否需要输出每个状态
		if (m_currFunc != "")
		{
			//add by yubin 2015/5/11,将$$m_currFunc压入SymTbl，输出的时候，可以输出函数名
			SymTbl.push_back("##" + m_currFunc);
		}
		for (iter = Smap->begin(); iter != Smap->end(); ++iter)
		{
			if (!(*(iter->first.c_str()) == '$' && *(iter->first.c_str() + 1) == '$'))
			{
				print += iter->second->PrntSym(iter->first);
				SymTbl.push_back(iter->first);
			}
		}
		for (iter = FSmap->begin(); iter != FSmap->end(); ++iter)
		{
			if (!(*(iter->first.c_str()) == '$' && *(iter->first.c_str() + 1) == '$'))
			{
				print += iter->second->PrntSym(iter->first);
				if (iter->second->GetSymbolType() != UNDEFINE)
				{
					SymTbl.push_back(iter->first);
				}
			}
		}
		While_IRTree->SetST(SymTbl);
	}
	
	__IfWhileCondFunc(pTree->GetChild0());//add by yubin 2015/5/4,处理while条件中出现函数调用的情况
	While_IRTree->SetChild0(pTree->GetChild0());
	pTree->SetChild0(nullptr);
	m_More = -1;
	m_Program->SetRoot(pTree->GetChild1());
	pTree->SetChild1(nullptr);
	CSyntaxNode* tmpIRTree = m_IRTree->GetRoot();
	m_IRTree->SetRoot(nullptr);

	Pre();

	if (g_InNewFunc)
	{
		CSyntaxNode* pushStmt = __ConstructPushStmt(pTree->GetChild0(), 4, true);
		m_IRTree->MergeTree(pushStmt, CHOP_STA, VOIDTYPE);
		m_IRTree->MergeTree(__CallPauseThread(), CHOP_STA, VOIDTYPE);
	}
	While_IRTree->SetChild1(m_IRTree->GetRoot());
	delete pTree;
	pTree = nullptr;
	m_IRTree->SetRoot(tmpIRTree);
	m_IRTree->MergeTree(While_IRTree, CHOP_STA, VOIDTYPE);
	m_More = 0;
	m_RdcOk = 1;
	m_IsComplexStmt = true;
}
//-----------------------------------------------------------------------------
///2015-3-18 add by yubin,处理for语句：for(p1,b,p2){q}-->p1;while(b){q;p2}
void CMSV::__For(CSyntaxNode* pTree)
{
	CSyntaxNode* initNode = pTree->GetChild0()->GetChild0();//获得for语句的初始条件
	CSyntaxNode* conditionNode = pTree->GetChild0()->GetChild1();//获得for语句的判断条件
	CSyntaxNode* controlNode = pTree->GetChild0()->GetChild2();//获得for语句的控制语句
	CSyntaxNode* forStatNode = pTree->GetChild0()->GetChild2();//获得for语句的真正的内容
	CSyntaxNode* trsltToStatement = new CSyntaxNode(CHOP_STA, VOIDTYPE);//转成之后的最上层的chop语句
	trsltToStatement->SetChild0(initNode);
	CSyntaxNode* whileStatement = new CSyntaxNode(WHILE_DO_STA, VOIDTYPE);//转成之后的while语句
	whileStatement->SetChild0(conditionNode);
	CSyntaxNode* whileChopStatement = new CSyntaxNode(CHOP_STA, VOIDTYPE);//while语句中的chop语句
	whileChopStatement->SetChild0(forStatNode);
	whileChopStatement->SetChild1(controlNode);
	whileStatement->SetChild1(whileChopStatement);
	trsltToStatement->SetChild1(whileStatement);
	GetPresent(pTree);//处理完之后，是不是应该在调用一次GetPresent
	return;
}
//-----------------------------------------------------------------------------
void CMSV::__ExFor(CSyntaxNode* pTree)
{
	m_ForNum++;
	string tempForVarName = "$$For_" + __Int2Str(m_ForNum);
	CSyntaxNode* tempForVar = new CSyntaxNode(IDENT_EXP, tempForVarName, nullptr, VOIDTYPE);
	//for n times do {p} 处理成 int $$For_i and $$For_i=0 and empty; while($$Fori<n){p; $$Fori=$$Fori+1 and empty}
	//加入到符号表
	m_FrmSymbl->InstFSymbl(tempForVarName);

	CSyntaxNode* foriDec = new CSyntaxNode(DECLARE_STA, new CSyntaxNode(DEC_VAR_LIST, VOIDTYPE), INTTYPE);
	foriDec->GetChild0()->SetChild0(tempForVar);

	CSyntaxNode* foriAss = new CSyntaxNode(ASS_EQU_EXP, VOIDTYPE);
	foriAss->SetChild0(tempForVar);
	foriAss->SetChild1(new CSyntaxNode(INTEGER_EXP, 0, INTTYPE));

	CSyntaxNode* andStmt = new CSyntaxNode(AND_EXP, VOIDTYPE);
	andStmt->SetChild0(foriDec);
	andStmt->SetChild1(new CSyntaxNode(AND_EXP, VOIDTYPE));
	andStmt->GetChild1()->SetChild0(foriAss);
	andStmt->GetChild1()->SetChild1(new CSyntaxNode(EMPTY_EXP, VOIDTYPE));

	CSyntaxNode* whileStmt = new CSyntaxNode(WHILE_DO_STA, VOIDTYPE);
	whileStmt->SetChild0(new CSyntaxNode(LT_EXP, tempForVar, pTree->GetChild0(), BOOLTYPE));

	CSyntaxNode* AssStmt = new CSyntaxNode(ASS_EQU_EXP, VOIDTYPE);
	AssStmt->SetChild0(tempForVar);
	AssStmt->SetChild1(new CSyntaxNode(ADD_EXP, tempForVar, new CSyntaxNode(INTEGER_EXP, 1, INTTYPE), VOIDTYPE));
	CSyntaxNode* andStmt_1 = new CSyntaxNode(AND_EXP, AssStmt, new CSyntaxNode(EMPTY_EXP, VOIDTYPE), VOIDTYPE);

	whileStmt->SetChild1(new CSyntaxNode(CHOP_STA, pTree->GetChild1(), andStmt_1, VOIDTYPE));

	pTree->SetNType(CHOP_STA);
	pTree->SetChild0(andStmt);
	pTree->SetChild1(whileStmt);

	GetPresent(pTree);
}
/*
//将switch语句变成if语句
void CMSV::__Switch(CSyntaxNode* pTree)
{
	vector <CSyntaxNode*> ifStatVec;//存储if语句，最后在一起变成if_else语句
	CSyntaxNode* switchCondition = new CSyntaxNode();
	switchCondition->CopyTree(pTree->GetChild0());//switch语句的判断条件，这一句放在最开始，因为switch的条件是不会变的
	CSyntaxNode* caseStat = new CSyntaxNode();
	caseStat->CopyTree(pTree->GetChild1());//第一条case语句


	while (caseStat != nullptr)
	{
		if (caseStat->GetNType() == CASE_STA)
		{
			CSyntaxNode* IfStat = new CSyntaxNode(IF_ELSE_STA, VOIDTYPE);//最上层的if语句
			CSyntaxNode* IfCondtionStat = new CSyntaxNode(EQU_EXP, pTree->GetChild0(), caseStat->GetChild0(), BOOLTYPE);
			IfStat->SetChild0(IfCondtionStat);//设置if的判断条件
			CSyntaxNode* chopStat = new CSyntaxNode(CHOP_STA, VOIDTYPE);
			chopStat->SetChild0(new CSyntaxNode());
			chopStat->GetChild0()->CopyTree(caseStat->GetChild1());
			CSyntaxNode* tmpcaseStat = caseStat;

			while (tmpcaseStat->GetChild2() != nullptr && tmpcaseStat->GetChild2()->GetRType() == VOIDTYPE)//不是break语句，需要继续添加到chop语句
			{
				tmpcaseStat = tmpcaseStat->GetChild2()->GetChild0();
				CSyntaxNode* tmpChopStat = new CSyntaxNode(CHOP_STA, VOIDTYPE);
				tmpChopStat->SetChild0(new CSyntaxNode());
				if (tmpcaseStat->GetNType() == DEFAULT_STA)//add by yubin 2015/4/10增加default情况
				{
					tmpChopStat->GetChild0()->CopyTree(tmpcaseStat->GetChild0());
				}
				else//case的情况
				{
					tmpChopStat->GetChild0()->CopyTree(tmpcaseStat->GetChild1());
				}
				CSyntaxNode* tmpNode = chopStat;
				while (tmpNode->GetChild1() != nullptr)
				{
					tmpNode = tmpNode->GetChild1();
				}
				tmpNode->SetChild1(tmpChopStat);
			}
			tmpcaseStat = chopStat;
			while (tmpcaseStat->GetChild1() != nullptr)
			{
				tmpcaseStat = tmpcaseStat->GetChild1();
			}

			CSyntaxNode* tmp = tmpcaseStat->GetChild0();
			tmpcaseStat->SetChild0(nullptr);
			tmpcaseStat->CopyTree(tmp);//将最后一个chop的孩子0放在父节点的孩子1,不能直接tmpcaseStat->CopyTree(tmpcaseStat->GetChild0())
			tmp = nullptr;

			IfStat->SetChild1(chopStat);//设置if语句的then的内容
			ifStatVec.push_back(IfStat);//将if语句压入vector中
		}
		else//default语句
		{
			CSyntaxNode* defaultStat = new CSyntaxNode();
			defaultStat->CopyTree(caseStat->GetChild0());
			ifStatVec.push_back(defaultStat);//将if语句压入vector中
		}
		if (caseStat->GetChild2() != nullptr)
		{
			caseStat = caseStat->GetChild2()->GetChild0();//将case语句变成下一条case语句
		}
		else
			break;
	}

	//下面开始将vector中的if语句，组装成最后的if_else语句
	CSyntaxNode* totalIfStat = ifStatVec.front();//最上层的if语句

	vector <CSyntaxNode*>::iterator iter = ifStatVec.begin();
	iter++;
	CSyntaxNode* tmpTotalIfStat = totalIfStat;
	for (; iter != ifStatVec.end(); iter++)
	{
		tmpTotalIfStat->SetChild2(*iter);
		tmpTotalIfStat = tmpTotalIfStat->GetChild2();
	}
	pTree = totalIfStat;//讲总的if语句赋给pTree
	GetPresent(pTree);//将switch语句变成if语句后，再调用GetPresent函数
}*/

/* 可支持frame(x) and frame(y) 这种形式的版本
///2015-3-13 add by wangmeng
void CMSV::__Frm(CSyntaxNode* pTree)
{
if(__SearchAndInsert(UNDEFINE,pTree))
{
CSyntaxNode * UnFrame=new CSyntaxNode(UNFRAME_STA,VOIDTYPE);

UnFrame->SetChild0(pTree->GetChild0());
pTree->SetChild0(nullptr);
m_UnFrm.push(UnFrame);
m_RdcOk=1;
m_Frm++;
return;
}
else
{
m_RdcOk=0;
PrintErrorMassage(0, "__Frm() insert symbol error!");
return;
}
}*/

/**
* Get present tree of frame statement: frame(c)
* @param the syntax tree to be handled
* @return void
*/
/// frame(c) and P
/// 1.先处理c，将c中包含的符号加入到m_FrmSymbl符号表中，如果符号表中已经有该变量，
/// 其标记frame次数的变量m_nFrm加1.
/// 2.处理P
/// 3.(1)如果有下一状态，unframe(c) and Pf加入到下一状态，其中Pf是处理P得到的下一状态
///   (2)没有下一状态，将unframe(c)加入到栈m_PreUnFrm中，准备释放
///2015-3-16 add by wangmeng
void CMSV::__Frm(CSyntaxNode* pTree)
{
	if (pTree->GetChild0() == nullptr || pTree->GetChild1() == nullptr)
	{
		PrintErrorMassage(0, "__Frm() syntax tree error!");
		m_RdcOk = 0;
		return;
	}

	///2015-06-18 add by shiyifang
	///处理过多的frame变量导致的栈溢出
	bool insertOK = true;
	CSyntaxNode *temp = pTree->GetChild0();
	while (temp != nullptr)
	{
		insertOK = insertOK & __SearchAndInsert(UNDEFINE, temp->GetChild0());
		temp = temp->GetChild1();
	}


	if (insertOK) ///加入符号表
	{
		CSyntaxNode * UnFrame = new CSyntaxNode(UNFRAME_STA, VOIDTYPE);

		UnFrame->SetChild0(pTree->GetChild0());
		//UnFrame->SetChild1(pTree->GetChild1());
		pTree->SetChild0(nullptr);
		//pTree->SetChild1(nullptr);

		GetPresent(pTree->GetChild1()); ///frame(x) and P 处理P

		if (m_RdcOk == 0 || m_More == -1)
		{
			PrintErrorMassage(0, "__Frm() handle child 2 error!");
			return;
		}

		if (m_More == 1) ///有下一状态，将unframe(x) and Pf加入到下一状态
		{
			UnFrame->SetChild1(m_PrgmFuture->GetRoot());
			m_PrgmFuture->SetRoot(UnFrame);
		}
		else
		{
			if (m_More == 0)///没有下一状态，UnFrame压入栈中，准备释放符号表
			{
				m_PreUnFrm.push(UnFrame);
				m_FreeFrm = 1;
			}

		}

		m_RdcOk = 1;
		//m_Frm++;
		return;
	}
	else
	{
		m_RdcOk = 0;
		PrintErrorMassage(0, "__Frm() insert symbol error!");
		return;
	}

}

/**
* Get present tree of unframe statement: unframe(c)
* @param the syntax tree to be handled
* @return void
*/
/// unframe(c) and P
/// 1.处理P
/// 2.(1)如果有下一状态，unframe(c) and Pf加入到下一状态，其中Pf是处理P得到的下一状态
///   (2)没有下一状态，将unframe(c)加入到栈m_PreUnFrm中，准备释放
///2015-3-16 add by wangmeng
void CMSV::__UnFrm(CSyntaxNode* pTree)
{
	if (pTree->GetChild0() == nullptr)
	{
		PrintErrorMassage(0, "__UnFrm() syntax tree error!");
		m_RdcOk = 0;
		return;
	}

	CSyntaxNode * UnFrame = new CSyntaxNode(UNFRAME_STA, VOIDTYPE);

	UnFrame->SetChild0(pTree->GetChild0());
	//UnFrame->SetChild1(pTree->GetChild1());
	pTree->SetChild0(nullptr);
	//pTree->SetChild1(nullptr);

	if (pTree->GetChild1() == nullptr)////在if语句是frame的最后一条语句时，会出现这种情况
	{
		m_PreUnFrm.push(UnFrame);
		m_FreeFrm = 1;
		m_More = 0;
		m_RdcOk = 1;
		return;
	}

	GetPresent(pTree->GetChild1()); ///frame(x) and P 处理P

	if (m_RdcOk == 0 || m_More == -1)
	{
		PrintErrorMassage(0, "__UnFrm() handle child 2 error!");
		return;
	}

	if (m_More == 1) ///有下一状态，将unframe(x) and Pf加入到下一状态
	{
		UnFrame->SetChild1(m_PrgmFuture->GetRoot());
		m_PrgmFuture->SetRoot(UnFrame);
	}
	else
	{
		if (m_More == 0) ///没有下一状态，UnFrame压入栈中，准备释放符号表
		{
			m_PreUnFrm.push(UnFrame);
			m_FreeFrm = 1;
		}
	}

	m_RdcOk = 1;
	return;
}

void CMSV::__Or(CSyntaxNode*pTree)
{
	//开始构造statement1,current:=0;
	CSyntaxNode *statement1Left = new CSyntaxNode(IDENT_EXP, "$$current", nullptr, VOIDTYPE);
	CSyntaxNode *statement1Right = new CSyntaxNode(INTEGER_EXP, 0, INTTYPE);
	CSyntaxNode *statement1 = new CSyntaxNode(ASS_EQU_EXP, VOIDTYPE);
	statement1->SetChild0(statement1Left);
	statement1->SetChild1(statement1Right);

	//开始构造statement2，函数调用changeState();
	CSyntaxNode *statement2 = new CSyntaxNode(FUNCTION_STA, VOIDTYPE);
	statement2->SetChild0(new CSyntaxNode(IDENT_EXP, "$$changeState", nullptr, VOIDTYPE));

	//开始构造statement3，
	//if(flag[current-1]=0) then {x:=2}else{x:=3}

	//首先构造if判断条件中的flag的下标
	CSyntaxNode *arrayIndexLeft = new CSyntaxNode(IDENT_EXP, "$$current", nullptr, VOIDTYPE);
	CSyntaxNode *arrayIndexRight = new CSyntaxNode(INTEGER_EXP, 1, INTTYPE);
	CSyntaxNode *arrayIndex = new CSyntaxNode(SUB_EXP, ARITHMETICTYPE);
	arrayIndex->SetChild0(arrayIndexLeft);
	arrayIndex->SetChild1(arrayIndexRight);

	//构造if判断条件
	CSyntaxNode* arrayName = new CSyntaxNode(IDENT_EXP, "$$flag", nullptr, VOIDTYPE);
	CSyntaxNode *equationLeft = new CSyntaxNode(ARRAY_EXP, arrayName, nullptr, VOIDTYPE);
	equationLeft->SetChild1(new CSyntaxNode(OPTION_EXP, arrayIndex, VOIDTYPE));
	CSyntaxNode *equationRight = new CSyntaxNode(INTEGER_EXP, 0, INTTYPE);
	CSyntaxNode *equation = new CSyntaxNode(EQU_EXP, BOOLTYPE);
	equation->SetChild0(equationLeft);
	equation->SetChild1(equationRight);

	//开始构造statement3，ifelse语句
	CSyntaxNode *statement3 = new CSyntaxNode(IF_ELSE_STA, VOIDTYPE);
	statement3->SetChild0(equation);
	statement3->SetChild1(pTree->GetChild0());
	statement3->SetChild2(pTree->GetChild1());

	//yangkai 将并行中由or转成的if节点的intValue值置为1，和程序中原有的if节点区别
	statement3->SetiValue(1);

	pTree->SetChild0(nullptr);
	pTree->SetChild1(nullptr);


	if (m_VisitedOr == 0)//如果之前没遇见or分支
	{
		/*
		//如果之前没有遇见or，需要加current=0这句话
		m_VisitedOr = 1;//将全局变量m_VisitedOr赋为1
		CSyntaxNode *Level1 = new CSyntaxNode(AND_EXP, VOIDTYPE);//Level1是最上层的，左孩子是第一条语句
		CSyntaxNode *Level2 = new CSyntaxNode(AND_EXP, VOIDTYPE);
		Level1->SetChild0(statement1);//将statement1作为第一个语句
		Level1->SetChild1(Level2);
		Level2->SetChild0(statement2);//将statement2作为第二个语句
		Level2->SetChild1(statement3);//将statement3作为第三个语句

		pTree = Level1;//将Level1付给pTee之后，直接返回就行
		*/
		//如果之前遇见or，不需要加current=0这句话，只需新建一个CHOP_STA语法结点就可以了
		CSyntaxNode *Level1 = new CSyntaxNode(CHOP_STA, VOIDTYPE);//Level1是最上层的，左孩子是第一条语句

		CSyntaxNode *Level2 = new CSyntaxNode(AND_EXP, VOIDTYPE);
		Level2->SetChild0(statement2);
		Level2->SetChild1(new CSyntaxNode(EMPTY_EXP, VOIDTYPE));

		Level1->SetChild0(Level2);//将statement2作为第二个语句
		Level1->SetChild1(statement3);//将statement3作为第三个语句

		pTree = Level1;//将Level1付给pTee之后，直接返回就行

	}
	else
	{
		//如果之前遇见or，不需要加current=0这句话，只需新建一个CHOP_STA语法结点就可以了
		CSyntaxNode *Level1 = new CSyntaxNode(CHOP_STA, VOIDTYPE);//Level1是最上层的，左孩子是第一条语句

		CSyntaxNode *Level2 = new CSyntaxNode(AND_EXP, VOIDTYPE);
		Level2->SetChild0(statement2);
		Level2->SetChild1(new CSyntaxNode(EMPTY_EXP, VOIDTYPE));

		Level1->SetChild0(Level2);//将statement2作为第二个语句
		Level1->SetChild1(statement3);//将statement3作为第三个语句

		pTree = Level1;//将Level1付给pTee之后，直接返回就行

	}
	GetPresent(pTree);//处理完之后，是不是应该在调用一次GetPresent
	return;
}


void CMSV::__OrPost(CSyntaxNode*pTree)
{
	CSyntaxNode *equationLeft = new CSyntaxNode(IDENT_EXP, "$$flag2", nullptr, VOIDTYPE);
	CSyntaxNode *equationRight = new CSyntaxNode(INTEGER_EXP, 0, INTTYPE);
	CSyntaxNode *equation = new CSyntaxNode(EQU_EXP, BOOLTYPE);
	equation->SetChild0(equationLeft);
	equation->SetChild1(equationRight);

	CSyntaxNode *tmpIRTree = new CSyntaxNode(WHILE_DO_STA, VOIDTYPE);
	tmpIRTree->SetChild0(equation);
	tmpIRTree->SetChild1(new CSyntaxNode());
	tmpIRTree->GetChild1()->CopyTree(m_IRTree->GetRoot());

	if (Setting::IsSimulation())
	{
		//设置一下，以便调用Pre函数
		SetProgram(pTree);
		SetPreFlag(1);//不输出
		Pre();//这样就可以将tmpGloVarTree1的内容存到IRTree上

		CSyntaxNode *totalIRTree = new CSyntaxNode(CHOP_STA, VOIDTYPE);
		totalIRTree->SetChild0(m_IRTree->GetRoot());
		totalIRTree->SetChild1(tmpIRTree);
		m_IRTree->SetRoot(totalIRTree);
	}
	else if (Setting::IsModeling())
	{
		//yangkai 2016/11/22 if($$MySetJmp()){全局变量初始化；firstTime=1} (用于建模)
		CSyntaxNode* ifStmt = new CSyntaxNode(IF_ELSE_STA, VOIDTYPE);
		ifStmt->SetChild0(new CSyntaxNode(ASS_EQU_EXP, VOIDTYPE));

		ifStmt->SetChild0(new CSyntaxNode(FUNCTION_STA, VOIDTYPE));
		ifStmt->GetChild0()->SetChild0(new CSyntaxNode(IDENT_EXP, "_setjmp3", nullptr, VOIDTYPE));
		ifStmt->GetChild0()->SetChild1(new CSyntaxNode(OPTION_EXP, VOIDTYPE));
		ifStmt->GetChild0()->GetChild1()->SetChild0(new  CSyntaxNode(IDENT_EXP, "$$myJmpbuf", nullptr, VOIDTYPE));
		ifStmt->GetChild0()->GetChild1()->SetChild1(new CSyntaxNode(OPTION_EXP, VOIDTYPE));
		ifStmt->GetChild0()->GetChild1()->GetChild1()->SetChild0(new CSyntaxNode(INTEGER_EXP, 0, INTTYPE));

		//yangkai 2016/11/22 全局变量重新初始化 （用于建模）
		CSyntaxTree* thenStmt = new CSyntaxTree();
		CSyntaxNode *stmt1 = new CSyntaxNode(GLOBAL_INITSET_STA, VOIDTYPE);
		thenStmt->MergeTree(stmt1, CHOP_STA, VOIDTYPE);

		//yangkai 2016/11/22  $$firstTime = 1; （用于建模）
		CSyntaxNode *stmt3 = new CSyntaxNode(ASS_EQU_EXP, VOIDTYPE);
		stmt3->SetChild0(new CSyntaxNode(IDENT_EXP, "$$firstTime", nullptr, VOIDTYPE));
		stmt3->SetChild1(new CSyntaxNode(INTEGER_EXP, 1, INTTYPE));
		thenStmt->MergeTree(stmt3, CHOP_STA, VOIDTYPE);

		ifStmt->SetChild1(thenStmt->GetRoot());	


		CSyntaxNode* newChopStmt1 = new CSyntaxNode(CHOP_STA, VOIDTYPE);
		newChopStmt1->SetChild0(ifStmt);
		newChopStmt1->SetChild1(tmpIRTree);

		//设置一下，以便调用Pre函数
		SetProgram(pTree);
		SetPreFlag(1);//不输出
		Pre();//这样就可以将tmpGloVarTree1的内容存到IRTree上

		CSyntaxNode *totalIRTree = new CSyntaxNode(CHOP_STA, VOIDTYPE);
		totalIRTree->SetChild0(m_IRTree->GetRoot());

		totalIRTree->SetChild1(newChopStmt1);
		m_IRTree->SetRoot(totalIRTree);

		m_IRTree->MergeTree(new CSyntaxNode(DRAW_STA, VOIDTYPE), CHOP_STA, VOIDTYPE);//绘制模型图
	}
}



//2015-3-16,add by yubin,处理存在变量
//注意，修改了语法分析的inner_optional_define_identifier，以前的语法分析识别不了exists x:{x<==1 and skip},以前只能识别exists x,int y:{x<==1 and skip},
void CMSV::__Ext(CSyntaxNode* pTree)
{
	CSyntaxNode* p = pTree;
	while (p != nullptr)
	{
		//遇见一个变量，先在自己定义的map中的查找，这个map的作用是存储每个变量已经出现过的exist的次数
		string varName = p->GetChild0()->GetNName();
		auto iter = m_existVarTimes.find(varName);
		if (iter == m_existVarTimes.end()){
			//如果没有找到该变量，表明该变量之前没有出现过exist，将该变量的次数变成1，并且插入到map中
			m_existVarTimes.insert(make_pair(varName, 1));
			iter = m_existVarTimes.find(varName);
		}
		else{
			iter->second = iter->second + 1;//如果出现过，就将次数加1
		}

		//将次数变成字符串的形式，然后调用replaceExistName函数进行函数名替换
		pTree->replaceExistName(iter->first, m_currFunc, to_string(iter->second));//如果是全局变量，函数名写成空

		p = p->GetChild1();
	}
	pTree = pTree->GetChild2();//将exist x:{p}中的p内容付给pTree，然后再调用getPresent函数
	GetPresent(pTree);
}

bool CMSV::__isRValue(CSyntaxNode* pTree)
{
	CSyntaxNode* tmp = pTree->GetChild0();
	while (tmp != nullptr)
	{
		if (tmp->GetNName() == "RValue")
		{
			tmp->SetNName("");
			return 1;
		}
		tmp = tmp->GetChild0();
	}
	return 0;
}
//-----------------------------------------------------------------------------
void CMSV::__FuncDefPara(CSyntaxNode* pTree)
{
	m_FuncFormalPar = pTree->GetChild0();

	bool existRValue = false;
	CSyntaxNode *tmpExtNode = new CSyntaxNode(FORMAL_PARAMETER_EXP, new CSyntaxNode(IDENT_EXP, "$$Ext", nullptr, VOIDTYPE), INTTYPE);
	tmpExtNode->SetChild1(pTree->GetChild0());
	pTree->SetChild0(tmpExtNode);//将$$Ext变量添加到形参列表的最开始
	if (!g_InNewFunc || m_CurrentFunc.substr(0, 7) == "$$user_")
	{
		if (m_CurrentFunc.substr(0, 7) == "$$user_")
		{
			m_currFunc = m_CurrentFunc.substr(7, 1);
		}
		else
		{
			m_currFunc = pTree->GetNName();//获得函数名，付给currFunc变量，用于存在量词的变名
		}
	}
	m_Program->SetRoot(pTree->GetChild1());
	m_More = -1;

	///2015-4-15 add by wangmeng函数的参数加入到符号表中
	CSyntaxNode* VstPar = pTree->GetChild0();

	while (VstPar != nullptr)
	{
		if (VstPar->GetRType() == FUNCPTYPE)//如果是函数指针的话，不用加入符号表，直接处理
		{
			__FuncDefParaFuncP(VstPar);//__FuncDefParaFuncP和__FuncDefPara函数的区别是，前者对出现的变量不用加入符号表
			VstPar = VstPar->GetChild1();
		}
		else
		{
			SymbolType type = SymbolTable::GetSymbolType(VstPar->GetRType());
			string name = "";
			if (VstPar->GetRType() == STRUCTTYPE)
				name = VstPar->GetNName();

			if (!__SearchAndInsert(type, VstPar->GetChild0(), name, true))
			{
				PrintErrorMassage(0, "__FuncDef(): declaration of parameter : " + VstPar->TreeToString());
				m_RdcOk = 0;
				return;
			}
			VstPar = VstPar->GetChild1();
		}
	}

	///end 2015-4-15 add by wangmeng

	CSyntaxNode *paraList = pTree->GetChild0();//获得函数的形参列表
	while (paraList->GetChild1() != nullptr)
	{
		//if ((paraList->GetChild1()->GetChild0()->GetNName() == "RValue") && (paraList->GetChild1()->GetChild1() == nullptr))
		if (__isRValue(paraList->GetChild1()))
		{
			if (paraList->GetChild1()->GetChild1() == nullptr)
			{
				CSyntaxNode* retNode = new CSyntaxNode(FUNC_RETURN_TYPE, paraList->GetChild1(), VOIDTYPE);
				paraList->SetChild1(nullptr);
				retNode->SetChild1(pTree->GetChild0());
				pTree->SetChild0(retNode);
				existRValue = 1;
				break;
			}
			else
			{
				PrintErrorMassage(0, "In function " + pTree->GetNName() + " definition, RValue must be the last parameter!!!");
				return;
			}
		}
		paraList = paraList->GetChild1();
	}
	if (existRValue == false)
	{
		CSyntaxNode* retNode = new CSyntaxNode(FUNC_RETURN_TYPE, VOIDTYPE);
		retNode->SetChild1(pTree->GetChild0());
		pTree->SetChild0(retNode);
	}

}

//2015-5-5,add by yubin,把处理函数定义的函数指针参数的部分单独拿出来
void CMSV::__FuncDefParaFuncP(CSyntaxNode* pTree)
{
	pTree = pTree->GetChild0();
	int i = 0;
	m_FuncPNameVec.push_back(pTree->GetNameNode(i)->GetNName());//将函数指针名字存起来，用于if和while条件中使用时能判断出来
	//
	__SearchAndInsert(UNDEFINE, pTree->GetChild0()->GetChild0());//将函数指针加入符号表 2017/11/1 yangkai 不确定

	bool existRValue = false;
	CSyntaxNode *tmpExtNode = new CSyntaxNode(FORMAL_PARAMETER_EXP, new CSyntaxNode(IDENT_EXP, "$$Ext", nullptr, VOIDTYPE), INTTYPE);
	tmpExtNode->SetChild1(pTree->GetChild1());
	pTree->SetChild1(tmpExtNode);//将$$Ext变量添加到形参列表的最开始

	CSyntaxNode *paraList = pTree->GetChild1();//获得函数的形参列表
	while (paraList->GetChild1() != nullptr)
	{
		//add by daichunchun 2015/6/5 三层函数指针
		if (paraList->GetChild1()->GetRType() == FUNCPTYPE)//如果是函数指针的话，不用加入符号表，直接处理
		{
			__FuncDefParaFuncP(paraList->GetChild1());//__FuncDefParaFuncP和__FuncDefPara函数的区别是，前者对出现的变量不用加入符号表
		}
		//end add
		//if ((paraList->GetChild1()->GetChild0()->GetNName() == "RValue") && (paraList->GetChild1()->GetChild1() == nullptr))
		if (__isRValue(paraList->GetChild1()))
		{
			if (paraList->GetChild1()->GetChild1() == nullptr)
			{
				CSyntaxNode* retNode = new CSyntaxNode(FUNC_RETURN_TYPE, paraList->GetChild1(), VOIDTYPE);
				paraList->SetChild1(nullptr);
				retNode->SetChild1(pTree->GetChild1());
				pTree->SetChild1(retNode);
				existRValue = true;
				break;
			}
			else
			{
				PrintErrorMassage(0, "In function " + pTree->GetNName() + " definition, RValue must be the last parameter!!!" );
				return;
			}
		}

		paraList = paraList->GetChild1();

	}
	if (existRValue == false)
	{
		CSyntaxNode* retNode = new CSyntaxNode(FUNC_RETURN_TYPE, VOIDTYPE);
		retNode->SetChild1(pTree->GetChild1());
		pTree->SetChild1(retNode);
	}
}
//-----------------------------------------------------------------------------
void CMSV::__FuncDef(CSyntaxNode *pTree)
{
	//cout << "processing function: " << pTree->GetNName() << endl;

	m_CurrentFunc = pTree->GetNName();
	//对源程序中main函数重命名
	if (pTree->GetNName() == "main")
	{
		pTree->SetNName("$main");
	}

	__FuncDefPara(pTree);

	//将函数体转成IR树
	m_InFunc = 1;
	Pre();
	m_InFunc = 0;
	//添加最后一个状态的输出
	/*if (!g_InNewFunc  && m_CurrentFunc != "$$changeState")
	{
		__OutputState();
	}*/

	if (g_InNewFunc)
	{
		string name = pTree->GetNName();
		if (name[0] == '$' && name[1] == '$' && name[2] == 'f')
		{
			bool flag = __SetNodeLengthOfIfStat(m_IRTree->GetRoot());
			if (!flag)
			{
				if (m_More == 0)
				{
					m_IRTree->MergeTree(__SetNodeLength(1), CHOP_STA, VOIDTYPE);
				}
				else if (m_More == -1)
				{
					m_IRTree->MergeTree(__SetNodeLength(2), CHOP_STA, VOIDTYPE);
				}
			}
		}
	}

	CSyntaxNode* tmpIR = m_IRTree->GetRoot();
	m_IRTree->SetRoot(m_LocVarTree->GetRoot());
	m_LocVarTree->SetRoot(nullptr);
	if (tmpIR != nullptr)
	{
		m_IRTree->MergeTree(tmpIR, CHOP_STA);
	}


	pTree->SetChild1(m_IRTree->GetRoot());//将转成IR之后的函数体代码，作为函数树的右子树
	m_IRTree->SetRoot(nullptr);
	///end by wangmeng
	///2015-4-23 add by wangmeng 函数声明
	CSyntaxNode* funcdec = new CSyntaxNode();
	funcdec->CopyNode(pTree);
	funcdec->SetNType(FUNC_DEC_STA);
	funcdec->SetChild0(new CSyntaxNode());
	funcdec->GetChild0()->CopyTree(pTree->GetChild0());
	if (!m_FuncDecTree->MergeTree(funcdec, CHOP_STA))
	{
		PrintErrorMassage(0, "__FuncDef merge tree error 2!");
		return;
	}
	///end by wangmeng

	m_currFunc = "";//处理完一个函数后，将此变量置为空串

	///2015-4-15 add by wangmeng 删除符号表中的所有变量
	m_FrmSymbl->Clear();
	m_NFrmSymbl->Clear();
	__ClearUnFrm();
	///end by wangmeng

	//add by yubin 2015/5/6,处理完函数定义后，将存储函数指针的容器清空
	//m_FuncPNameVec.clear();//有问题，这样做会把全局的函数指针名也清空

	if (pTree->GetChild2() != nullptr)//处理下一个函数定义
	{
		__FuncDef(pTree->GetChild2());
	}
}
//-----------------------------------------------------------------------------
bool CMSV::__SetNodeLengthOfIfStat(CSyntaxNode* pTree)
{
	if (pTree == nullptr)
	{
		return false;
	}
	bool flag = false;
	//只有if语句是最后一个语句时才需要处理
	if (pTree->GetNType() == CHOP_STA)
	{
		flag = __SetNodeLengthOfIfStat(pTree->GetChild1());
	}
	else if (pTree->GetNType() == IF_ELSE_STA)
	{
		//if嵌套时，并且内层的if语句是当前分支的最后一个语句则只给内层的if语句设置标志位
		if (pTree->GetChild1() != nullptr)
		{
			flag = __SetNodeLengthOfIfStat(pTree->GetChild1());
			if (!flag)
			{
				CSyntaxNode* temp = new CSyntaxNode(CHOP_STA, VOIDTYPE);
				temp->SetChild0(pTree->GetChild1());
				temp->SetChild1(__SetNodeLength(pTree->GetChild1()->GetcValue() == 'e' ? 1 : 2));
				pTree->SetChild1(temp);
				flag = true;
			}
		}
		else
		{
			pTree->SetChild1(__SetNodeLength(1));
		}
		if (pTree->GetChild2() != nullptr)
		{
			flag = __SetNodeLengthOfIfStat(pTree->GetChild2());
			if (!flag)
			{
				CSyntaxNode* temp = new CSyntaxNode(CHOP_STA, VOIDTYPE);
				temp->SetChild0(pTree->GetChild2());
				temp->SetChild1(__SetNodeLength(pTree->GetChild2()->GetcValue() == 'e' ? 1 : 2));
				pTree->SetChild2(temp);
				flag = true;
			}
		}
		else
		{
			pTree->SetChild2(__SetNodeLength(1));
		}
	}
	return flag;
}
//-----------------------------------------------------------------------------
/*bool CMSV::__AddCondAtEndOfWhile(CSyntaxNode* pTree, CSyntaxNode* pCond)
{
	bool flag = false;
	if (pTree->GetNType() == CHOP_STA)
	{
		flag = __AddCondAtEndOfWhile(pTree->GetChild1(), pCond);
	}
	else if (pTree->GetNType() == IF_ELSE_STA)
	{
		flag = __AddCondAtEndOfWhile(pTree->GetChild1(), pCond);
		flag = __AddCondAtEndOfWhile(pTree->GetChild2(), pCond);
	}
	else if (pTree->GetNType() == WHILE_DO_STA)
	{
		bool flag = __AddCondAtEndOfWhile(pTree->GetChild1(), pCond);
		if (!flag)
		{
			CSyntaxNode* newCond=nullptr;
			if (pCond != nullptr)
			{
				newCond = new CSyntaxNode(AND_EXP, BOOLTYPE);
				newCond->SetChild0(pCond);
				newCond->SetChild1(new CSyntaxNode(NEGATION_EXP, new CSyntaxNode(), BOOLTYPE));
				newCond->GetChild1()->GetChild0()->CopyTree(pTree->GetChild0());
			}
			else
			{
				newCond = new CSyntaxNode(NEGATION_EXP, new CSyntaxNode(), BOOLTYPE);
				newCond->GetChild0()->CopyTree(pTree->GetChild0());
			}

			CSyntaxNode* ifStat = new CSyntaxNode(IF_ELSE_STA, VOIDTYPE);
			ifStat->SetChild0(newCond);
			ifStat->SetChild1(__SetNodeLength(1));

			CSyntaxNode* temp = new CSyntaxNode(CHOP_STA, VOIDTYPE);
			temp->SetChild0(pTree->GetChild1());
			temp->SetChild1(ifStat);
			pTree->SetChild1(temp);
			flag = true;
		}
	}
	return flag;
}*/
//用户调用函数时，如果没有写extern，则先在函数树上找，如果找到了，则按照内部函数调用，即输出状态序列，如果没有找到，则当成是库函数
//如果写了extern，也是先在函数树上找，如果找到了，则按照外部函数调用，即不输出状态序列，如果没有找到，则当成是库函数
//内部函数调用的情况：f(x)(内部函数调用不能写区间),y:=f(x)(内部函数调用赋值必须是next赋值)
//外部函数调用的情况：extern f(x) and skip, malloc(x) and skip,y<==extern f(x) and skip,y<==malloc(x) and skip
///2015-3-17，add by yubin,处理内部函数调用,记着修改状态输出时的判断，如果是内部调用，进行状态输出，如果是外部函数调用，不进行输出
//flag=1时，表示merge到m_IRTree上，flag=0时，表示在赋值的右边，不用merge到m_IRTree上
void CMSV::__FuncCall(CSyntaxNode* pTree, int flag)
{
	/*if (pTree->GetNType() == FUNCTION_STA)
	{
		string funcName = pTree->GetNameNode()->GetNName();
		if (g_InNewFunc)
		{
			//当前函数调用在新函数中
			__FuncCalInNewFunc(pTree);
		}

		CSyntaxNode* visit = pTree->GetChild1();
		while (visit != nullptr)
		{
           __FuncCall(visit->GetChild0());
		   visit = visit->GetChild1();
		}

		//删掉RValue
		__DelRValue(pTree->GetChild1());
		if (pTree->GetChild1() != nullptr && pTree->GetChild1()->GetChild0() == nullptr)
		{
			//删除完之后没有参数则将整个孩子1置为空
			delete pTree->GetChild1();
			pTree->SetChild1(nullptr);
		}
		vector<string>::iterator result;
		result = find(m_FuncPNameVec.begin(), m_FuncPNameVec.end(), funcName);
		if (functionName.find(funcName) != functionName.end() || result != m_FuncPNameVec.end())
		{
			//函数语法树中有当前函数的定义,就需要将$$ext变量设置为0，表示内部函数调用，输出每个状态
			CSyntaxNode *tmpExtNode = new CSyntaxNode(OPTION_EXP, new CSyntaxNode(INTEGER_EXP, 0, VOIDTYPE), VOIDTYPE);
			tmpExtNode->SetChild1(pTree->GetChild1());
			pTree->SetChild1(tmpExtNode);//因为是自定义函数，因此将$$Ext变量添加到实参列表的最开始，并且值为0
			//判断实参类型，如果是函数调用则添加内部或外部调用标志
			CSyntaxNode* tmpPar = pTree->GetChild1();
			while (tmpPar != nullptr)
			{
				if (tmpPar->GetChild0()->GetNType() == FUNCTION_STA)
				{
					__FuncCall(tmpPar->GetChild0(), 0);
				}
				else if (tmpPar->GetChild0()->GetNType() == EXT_FUNCTION_STA)
				{
					__EFuncCall(tmpPar->GetChild0()->GetChild0(), 0);
				}
				tmpPar = tmpPar->GetChild1();
			}
			//对源程序中的main函数重命名
			if (pTree->GetChild0()->GetNName() == "main")
			{
				pTree->GetChild0()->SetNName("$main");
			}

			if (flag == 1)//flag=1时，表示merge到m_IRTree上
			{
				CSyntaxNode* copyTree = new CSyntaxNode();
				copyTree->CopyTree(pTree);
				__InsrtAss(copyTree);
				m_RdcOk = 1;
				m_More = 0;
				//2015/4/23 add by yubin, m_FuncFlag=1时，不输出状态
				m_FuncFlag = 1;
			}
		}
		else//如果用户没定义此函数，则当成是外部函数调用
		{
			__EFuncCall(pTree, flag);
		}
	}*/

	string funcName = pTree->GetNameNode()->GetNName();

	if (g_InNewFunc && funcName != "$$changeState")
	{
		//当前函数调用在新函数中
		__FuncCalInNewFunc(pTree);
	}
	__DelRValue(pTree->GetChild1());
	if (pTree->GetChild1() != nullptr && pTree->GetChild1()->GetChild0() == nullptr)
	{
		//删除完之后没有参数则将整个孩子1置为空
		delete pTree->GetChild1();
		pTree->SetChild1(nullptr);
	}
	vector<string>::iterator result;
	CSyntaxNode* p = function_tree;//p指向函数定义语法树	
	while (p != nullptr && p->GetNName() != funcName)//while循环用于在函数定义树查找当前调用的函数
	{
		p = p->GetChild2();
	}
	result = find(m_FuncPNameVec.begin(), m_FuncPNameVec.end(), funcName);

	if (p != nullptr || result != m_FuncPNameVec.end())//函数语法树中有当前函数的定义,就需要将$$ext变量设置为0，表示内部函数调用，输出每个状态
	{
		CSyntaxNode *tmpExtNode;// = new CSyntaxNode(OPTION_EXP, new CSyntaxNode(INTEGER_EXP, 0, VOIDTYPE), VOIDTYPE);
		if (g_InFunc)
		{
			tmpExtNode = new CSyntaxNode(OPTION_EXP, new CSyntaxNode(IDENT_EXP, "$$Ext", nullptr, VOIDTYPE), VOIDTYPE);
		}
		else
		{
			tmpExtNode = new CSyntaxNode(OPTION_EXP, new CSyntaxNode(INTEGER_EXP, 0, VOIDTYPE), VOIDTYPE);
		}

		tmpExtNode->SetChild1(pTree->GetChild1());
		pTree->SetChild1(tmpExtNode);//因为是自定义函数，因此将$$Ext变量添加到实参列表的最开始，并且值为0
		//判断实参类型，如果是函数调用则添加内部或外部调用标志
		CSyntaxNode* tmpPar = pTree->GetChild1();
		while (tmpPar != nullptr)
		{
			if (tmpPar->GetChild0()->GetNType() == FUNCTION_STA)
			{
				__FuncCall(tmpPar->GetChild0(), 0);
			}
			else if (tmpPar->GetChild0()->GetNType() == EXT_FUNCTION_STA)
			{
				__EFuncCall(tmpPar->GetChild0()->GetChild0(), 0);
			}
			tmpPar = tmpPar->GetChild1();
		}
		//对源程序中的main函数重命名
		if (pTree->GetChild0()->GetNName() == "main")
		{
			pTree->GetChild0()->SetNName("$main");
		}

		if (flag == 1)//flag=1时，表示merge到m_IRTree上
		{
			CSyntaxNode* copyTree = new CSyntaxNode();
			copyTree->CopyTree(pTree);
			__InsrtAss(copyTree);
			m_RdcOk = 1;
			m_More = 0;
			//2015/4/23 add by yubin, m_FuncFlag=1时，不输出状态
			m_FuncFlag = 1;
		}
	}
	else//如果用户没定义此函数，则当成是外部函数调用
	{
		__EFuncCall(pTree, flag);
	}
}
//-----------------------------------------------------------------------------
void CMSV::__FuncCalInNewFunc(CSyntaxNode* pTree)
{
	string funcName = pTree->GetNameNode()->GetNName();
	if (functionName.find(funcName) == functionName.end())
	{
		return;
	}
	string newFuncName = "$$user_" + funcName;
	//将函数f(x, y) 重命名为$$user_f(x, y)
	pTree->GetNameNode()->SetNName(newFuncName);
	//判断当前函数是否已经处理过，没处理过则进行处理
	if (g_ProcessedFuncName.find(funcName) == g_ProcessedFuncName.end())
	{
		g_ProcessedFuncName.insert(funcName);//加入到已经处理的函数集合中
		CSyntaxNode* visit = g_CopyFuncTree;
		if (visit != nullptr)
		{
			while (visit != nullptr && visit->GetNName() != funcName)
			{
				visit = visit->GetChild2();
			}
			/*找到当前调用的函数的定义后，1. 将其重命名为$$user_f, 2,增加一个$$Si类型的形参$$par,
			3.给每个变量前加*，4.将该函数复制到m_newFuncTree上*/
			if (visit != nullptr)
			{
				CSyntaxNode* newFunc = new CSyntaxNode();
				newFunc->CopyTree(visit);
				int tempInNewFunc = g_InNewFunc;
				g_InNewFunc = 0;
				__AddPointer(newFunc->GetChild1());
				g_InNewFunc = tempInNewFunc;
				newFunc->SetNName(newFuncName);

				//得到要增加的形参(当前函数的最后一个形参)
				/*visit = m_FuncFormalPar;

				CSyntaxNode* temp = visit;
				if (visit != nullptr)
				{
				while (visit != nullptr)
				{
				temp = visit;
				visit = visit->GetChild1();
				}
				CSyntaxNode* newFormalPar = new CSyntaxNode();
				newFormalPar->CopyTree(temp);
				//给每个形参前加*, 并在最后增加一个形参$$Si $$par
				visit = newFunc->GetChild0();
				temp = visit;
				if (visit != nullptr)
				{
				while (visit != nullptr)
				{
				temp = visit;
				visit = visit->GetChild1();
				}
				temp->SetChild1(newFormalPar);
				}
				else
				{
				newFunc->SetChild0(newFormalPar);
				}

				//在最后增加一个实参$$par
				CSyntaxNode* visit = pTree->GetChild1();
				if (visit != nullptr)
				{
				while (visit->GetChild1() != nullptr)
				{
				visit = visit->GetChild1();
				}
				visit->SetChild1(new CSyntaxNode(OPTION_EXP, VOIDTYPE));
				visit->GetChild1()->SetChild0(new CSyntaxNode(IDENT_EXP, "$$par", nullptr, VOIDTYPE));
				}
				else
				{
				pTree->SetChild1(new CSyntaxNode(OPTION_EXP, VOIDTYPE));
				pTree->GetChild1()->SetChild0(new CSyntaxNode(IDENT_EXP, "$$par", nullptr, VOIDTYPE));
				}
				}*/

				newFunc->SetChild2(m_NewFuncTree);
				m_NewFuncTree = newFunc;
			}
			else
			{
				PrintErrorMassage(0, "__HandleFuncCalInNewFunc error 2989");
			}
		}
	}
}
///2015-3-17，add by yubin,处理外部函数调用
//flag=1时，表示merge到m_IRTree上，flag=0时，表示在赋值的右边，不用merge到m_IRTree上
void CMSV::__EFuncCall(CSyntaxNode* pTree, int flag)
{
	__DelRValue(pTree->GetChild1());
	if (pTree->GetChild1() != nullptr && pTree->GetChild1()->GetChild0() == nullptr)
	{
		delete pTree->GetChild1();
		pTree->SetChild1(nullptr);
	}

	CSyntaxNode* p = function_tree;//p指向函数定义语法树
	vector<string>::iterator result;
	int i = 0;
	while (p != nullptr && p->GetNName() != pTree->GetNameNode(i)->GetNName())//while循环用于在函数定义树查找当前调用的函数
	{
		p = p->GetChild2();
	}
	result = find(m_FuncPNameVec.begin(), m_FuncPNameVec.end(), pTree->GetNameNode(i)->GetNName());


	if (p != nullptr || result != m_FuncPNameVec.end())//函数语法树中有当前函数的定义,就需要将$$ext变量设置为1，表示外部函数调用，不用输出每个状态，如果用户没定义此函数，则将此函数当做是系统函数
	{
		CSyntaxNode *tmpExtNode = new CSyntaxNode(OPTION_EXP, new CSyntaxNode(INTEGER_EXP, 1, VOIDTYPE), VOIDTYPE);
		tmpExtNode->SetChild1(pTree->GetChild1());
		pTree->SetChild1(tmpExtNode);

		//判断实参类型，如果是函数调用则添加内部或外部调用标志
		CSyntaxNode* tmpPar = pTree->GetChild1();
		while (tmpPar != nullptr)
		{
			if (tmpPar->GetChild0()->GetNType() == FUNCTION_STA)
			{
				__FuncCall(tmpPar->GetChild0(), 0);
			}
			else if (tmpPar->GetChild0()->GetNType() == EXT_FUNCTION_STA)
			{
				__EFuncCall(tmpPar->GetChild0()->GetChild0(), 0);
			}
			tmpPar = tmpPar->GetChild1();
		}
	}
	if (flag == 1)//flag=1时，表示merge到m_IRTree上
	{
		CSyntaxNode* copyTree = new CSyntaxNode();
		copyTree->CopyTree(pTree);
		__InsrtAss(copyTree);
	}

	m_RdcOk = 1;
	m_More = -1;
}

///add by yubin 2015-3-24，await(b)-->while(!b){skip}
void CMSV::__Await(CSyntaxNode*pTree)
{
	CSyntaxNode * negStat = new CSyntaxNode(NEGATION_EXP, BOOLTYPE);
	negStat->SetChild0(pTree->GetChild0());
	CSyntaxNode * skipStat = new CSyntaxNode(SKIP_STA, VOIDTYPE);

	pTree->SetNType(WHILE_DO_STA);
	pTree->SetChild0(negStat);
	pTree->SetChild1(skipStat);
	GetPresent(pTree);
}

//-----------------------------------------------------------------------------
///2015-3-12 add by wangmeng
void CMSV::__Fin(CSyntaxNode* pTree)
{
	if (pTree->GetChild0() == nullptr)
	{
		PrintErrorMassage(0, "__Fin syntax tree error!");
		m_RdcOk = 0;
		return;
	}
	//暂时没处理
}
//-----------------------------------------------------------------------------
void CMSV::__AlwNotInFunc(CSyntaxNode* pTree)
{
	string funcName = "$$f" + __Int2Str(m_NewFuncNum);
	string threadFuncName = "$$t" + __Int2Str(m_NewFuncNum);
	CSyntaxNode* funcFormalPar = nullptr;
	CSyntaxNode* funcActualPar = nullptr;
	CSyntaxNode* threadFuncActualPar = nullptr;
	CSyntaxNode* createThread = nullptr;
	//定义线程函数$$ti,该线程函数中循环创建线程$$tpi, $$tpi调用函数$$fi
	threadFuncActualPar = new CSyntaxNode(OPTION_EXP, VOIDTYPE);
	threadFuncActualPar->SetChild0(new CSyntaxNode(NULL_EXP, VOIDTYPE));
	__DefThreadFuncForAlw(threadFuncName, funcName, nullptr);
	__DefFunc(funcName, nullptr, pTree->GetChild0());//定义函数$$fi,函数体是alw(P)中的P
	pTree->SetChild0(nullptr);
	threadFuncActualPar = new CSyntaxNode(OPTION_EXP, VOIDTYPE);
	threadFuncActualPar->SetChild0(new CSyntaxNode(NULL_EXP, VOIDTYPE));
	threadFuncActualPar = new CSyntaxNode(OPTION_EXP, VOIDTYPE);
	threadFuncActualPar->SetChild0(new CSyntaxNode(NULL_EXP, VOIDTYPE));
	createThread = __MyCreateThread(threadFuncName, threadFuncActualPar, 0);//创建线程,第三个参数为0，表示父节点类型为And
	m_IRTree->MergeTree(createThread, CHOP_STA, VOIDTYPE);
	__MyWaitForObject();
	m_NewFuncNum++;
}
//-----------------------------------------------------------------------------
void CMSV::__AlwInFunc(CSyntaxNode* pTree)
{
	string funcName = "$$f" + __Int2Str(m_NewFuncNum);
	string threadFuncName = "$$t" + __Int2Str(m_NewFuncNum);
	CSyntaxNode* funcFormalPar = nullptr;
	CSyntaxNode* funcActualPar = nullptr;
	CSyntaxNode* threadFuncActualPar = nullptr;
	CSyntaxNode* createThread = nullptr;
	CSyntaxNode* newStruct = nullptr;
	/*如果当前并行语句在用户定义的函数中，需要定义新的结构体，新定义的函数的形参可以从新定义的结构体得到
	如果当前并行语句在新定义的函数中，则新函数的形参和当前所在的新函数的形参一样
	*/
	if (!g_InNewFunc || m_CurrentFunc.substr(0, 7) == "$$user_")
	{
		//定义结构体$$Si并且定义一个$$Si类型的指针变量$$si,给$$si分配空间并对成员进行初始化
		newStruct = __DefStruct(m_LocVarTree->GetRoot(), m_FuncFormalPar);
		if (newStruct != nullptr)
		{
			__InitStruct(newStruct);
			//由新结构体得到新定义的函数的形参，前n-1个形参是结构体的成员，最后一个形参是结构体指针类型的一个变量
			funcFormalPar = __GetFuncFormalPar(newStruct->GetChild0());
			//线程函数$$ti的实参是(void*)$$si
			threadFuncActualPar = new CSyntaxNode(OPTION_EXP, VOIDTYPE);
			threadFuncActualPar->SetChild0(new CSyntaxNode(TYPE_CAST_STA, VOIDTYPE));
			threadFuncActualPar->GetChild0()->SetChild0(new CSyntaxNode(CAST_TYPE, VOIDTYPE));
			threadFuncActualPar->GetChild0()->GetChild0()->SetChild0(new CSyntaxNode(POINTER_EXP, VOIDTYPE));
			threadFuncActualPar->GetChild0()->SetChild1(new CSyntaxNode(IDENT_EXP, "$$s" + __Int2Str(m_NewStructNum), nullptr, VOIDTYPE));
		}
		else
		{
			funcFormalPar = nullptr;
			threadFuncActualPar = new CSyntaxNode(OPTION_EXP, VOIDTYPE);
			threadFuncActualPar->SetChild0(new CSyntaxNode(NULL_EXP, VOIDTYPE));
		}
	}
	else
	{
		//新定义函数的形参和当前函数的形参一样（m_FuncFormalPar记录的是当前正在处理的函数的形参）
		funcFormalPar = m_FuncFormalPar;
		/*m_FuncFormalPar=nullptr时，当前函数没有形参，则线程函数的参数为nullptr，否则线程函数的实参为$$par（和当前函数
		的最后一个形参名一致）*/
		if (m_FuncFormalPar != nullptr)
		{
			threadFuncActualPar = new CSyntaxNode(OPTION_EXP, VOIDTYPE);
			threadFuncActualPar->SetChild0(new CSyntaxNode(IDENT_EXP, "$$par", nullptr, VOIDTYPE));
		}
		else
		{
			threadFuncActualPar = new CSyntaxNode(OPTION_EXP, VOIDTYPE);
			threadFuncActualPar->SetChild0(new CSyntaxNode(NULL_EXP, VOIDTYPE));
		}
	}
	funcActualPar = __GetFuncActualPar(funcFormalPar);//由形参得到实参

	//定义线程函数$$ti,该线程函数中循环创建线程$$tpi, $$tpi调用函数$$fi
	__DefThreadFuncForAlw(threadFuncName, funcName, nullptr);
	__AddPointer(pTree->GetChild0());//添加指针
	__DefFunc(funcName, funcFormalPar, pTree->GetChild0());//定义新函数$$fi
	pTree->SetChild0(nullptr);
	createThread = __MyCreateThread(threadFuncName, threadFuncActualPar, 0);//创建线程	
	m_IRTree->MergeTree(createThread, CHOP_STA, VOIDTYPE);
	__MyWaitForObject();
	__MyFree("$$s" + __Int2Str(m_NewStructNum));//free($$s)
	m_NewFuncNum++;
	m_NewStructNum++;
}
//-----------------------------------------------------------------------------
void CMSV::__Alw(CSyntaxNode*pTree)
{
	__DelLastStateOutput();
	//创建调度线程
	if (!g_InNewFunc)
	{
		CSyntaxNode* threadFuncActualPar = new CSyntaxNode(OPTION_EXP, VOIDTYPE);
		threadFuncActualPar->SetChild0(new CSyntaxNode(NULL_EXP, VOIDTYPE));
		CSyntaxNode* createThread = __MyCreateThread("ManagerThread", threadFuncActualPar, 5);
		m_IRTree->MergeTree(createThread, CHOP_STA, VOIDTYPE);
	}
	if (!g_InFunc)
	{
		__AlwNotInFunc(pTree);
	}
	else
	{
		__AlwInFunc(pTree);
	}
	m_More = -1;
	m_RdcOk = 1;
}
//-----------------------------------------------------------------------------
void CMSV::__KeepNotInFunc(CSyntaxNode* pTree)
{
	string funcName = "$$f" + __Int2Str(m_NewFuncNum);
	string threadFuncName = "$$t" + __Int2Str(m_NewFuncNum);
	CSyntaxNode* funcFormalPar = nullptr;
	CSyntaxNode* funcActualPar = nullptr;
	CSyntaxNode* threadFuncActualPar = nullptr;
	CSyntaxNode* createThread = nullptr;

	//定义线程函数$$ti,该线程函数中循环创建线程$$tpi, $$tpi调用函数$$fi
	threadFuncActualPar = new CSyntaxNode(OPTION_EXP, VOIDTYPE);
	threadFuncActualPar->SetChild0(new CSyntaxNode(NULL_EXP, VOIDTYPE));
	__DefThreadFuncForKeep(threadFuncName, funcName, nullptr);
	__DefFunc(funcName, nullptr, pTree->GetChild0());//定义函数$$fi,函数体是alw(P)中的P
	pTree->SetChild0(nullptr);
	threadFuncActualPar = new CSyntaxNode(OPTION_EXP, VOIDTYPE);
	threadFuncActualPar->SetChild0(new CSyntaxNode(NULL_EXP, VOIDTYPE));
	threadFuncActualPar = new CSyntaxNode(OPTION_EXP, VOIDTYPE);
	threadFuncActualPar->SetChild0(new CSyntaxNode(NULL_EXP, VOIDTYPE));
	createThread = __MyCreateThread(threadFuncName, threadFuncActualPar, 0);//创建线程,第三个参数为0，表示父节点类型为And
	m_IRTree->MergeTree(createThread, CHOP_STA, VOIDTYPE);
	__MyWaitForObject();
	m_NewFuncNum++;
}
void CMSV::__KeepInFunc(CSyntaxNode* pTree)
{
	string funcName = "$$f" + __Int2Str(m_NewFuncNum);
	string threadFuncName = "$$t" + __Int2Str(m_NewFuncNum);
	CSyntaxNode* funcFormalPar = nullptr;
	CSyntaxNode* funcActualPar = nullptr;
	CSyntaxNode* threadFuncActualPar = nullptr;
	CSyntaxNode* createThread = nullptr;
	CSyntaxNode* newStruct = nullptr;
	/*如果当前并行语句在用户定义的函数中，需要定义新的结构体，新定义的函数的形参可以从新定义的结构体得到
	如果当前并行语句在新定义的函数中，则新函数的形参和当前所在的新函数的形参一样
	*/
	if (!g_InNewFunc || m_CurrentFunc.substr(0, 7) == "$$user_")
	{
		//定义结构体$$Si并且定义一个$$Si类型的指针变量$$si,给$$si分配空间并对成员进行初始化
		newStruct = __DefStruct(m_LocVarTree->GetRoot(), m_FuncFormalPar);
		if (newStruct != nullptr)
		{
			__InitStruct(newStruct);
			//由新结构体得到新定义的函数的形参，前n-1个形参是结构体的成员，最后一个形参是结构体指针类型的一个变量
			funcFormalPar = __GetFuncFormalPar(newStruct->GetChild0());
			//线程函数$$ti的实参是(void*)$$si
			threadFuncActualPar = new CSyntaxNode(OPTION_EXP, VOIDTYPE);
			threadFuncActualPar->SetChild0(new CSyntaxNode(TYPE_CAST_STA, VOIDTYPE));
			threadFuncActualPar->GetChild0()->SetChild0(new CSyntaxNode(CAST_TYPE, VOIDTYPE));
			threadFuncActualPar->GetChild0()->GetChild0()->SetChild0(new CSyntaxNode(POINTER_EXP, VOIDTYPE));
			threadFuncActualPar->GetChild0()->SetChild1(new CSyntaxNode(IDENT_EXP, "$$s" + __Int2Str(m_NewStructNum), nullptr, VOIDTYPE));
		}
		else
		{
			funcFormalPar = nullptr;
			threadFuncActualPar = new CSyntaxNode(OPTION_EXP, VOIDTYPE);
			threadFuncActualPar->SetChild0(new CSyntaxNode(NULL_EXP, VOIDTYPE));
		}
	}
	else
	{
		//新定义函数的形参和当前函数的形参一样（m_FuncFormalPar记录的是当前正在处理的函数的形参）
		funcFormalPar = m_FuncFormalPar;
		/*m_FuncFormalPar=nullptr时，当前函数没有形参，则线程函数的参数为nullptr，否则线程函数的实参为$$par（和当前函数
		的最后一个形参名一致）*/
		if (m_FuncFormalPar != nullptr)
		{
			threadFuncActualPar = new CSyntaxNode(OPTION_EXP, VOIDTYPE);
			threadFuncActualPar->SetChild0(new CSyntaxNode(IDENT_EXP, "$$par", nullptr, VOIDTYPE));
		}
		else
		{
			threadFuncActualPar = new CSyntaxNode(OPTION_EXP, VOIDTYPE);
			threadFuncActualPar->SetChild0(new CSyntaxNode(NULL_EXP, VOIDTYPE));
		}
	}
	funcActualPar = __GetFuncActualPar(funcFormalPar);//由形参得到实参
	//定义线程函数$$ti,该线程函数中循环创建线程$$tpi, $$tpi调用函数$$fi
	__DefThreadFuncForKeep(threadFuncName, funcName, nullptr);
	__AddPointer(pTree->GetChild0());//添加指针
	__DefFunc(funcName, funcFormalPar, pTree->GetChild0());//定义新函数$$fi
	pTree->SetChild0(nullptr);
	createThread = __MyCreateThread(threadFuncName, threadFuncActualPar, 0);//创建线程	
	m_IRTree->MergeTree(createThread, CHOP_STA, VOIDTYPE);
	__MyWaitForObject();
	__MyFree("$$s" + __Int2Str(m_NewStructNum));//free($$s)
	m_NewFuncNum++;
	m_NewStructNum++;
}
//-----------------------------------------------------------------------------
void CMSV::__Keep(CSyntaxNode*pTree)
{
	__DelLastStateOutput();
	//创建调度线程
	if (!g_InNewFunc)
	{
		CSyntaxNode* threadFuncActualPar = new CSyntaxNode(OPTION_EXP, VOIDTYPE);
		threadFuncActualPar->SetChild0(new CSyntaxNode(NULL_EXP, VOIDTYPE));
		CSyntaxNode* createThread = __MyCreateThread("ManagerThread", threadFuncActualPar, 5);
		m_IRTree->MergeTree(createThread, CHOP_STA, VOIDTYPE);
	}
	if (!g_InFunc)
	{
		__KeepNotInFunc(pTree);
	}
	else
	{
		__KeepInFunc(pTree);
	}
	m_More = -1;
	m_RdcOk = 1;
}
//-----------------------------------------------------------------------------
void CMSV::__Return(CSyntaxNode* pTree)
{
	///2015-4-16 add by wangmeng
	CSyntaxNode* RTree = new CSyntaxNode();
	RTree->CopyTree(pTree);
	__InsrtAss(RTree);
	///end 2015-4-16 add by wangmeng

	m_RdcOk = 1;
	m_More = 0;
}

//-----------------------------------------------------------------------------
void CMSV::__InsrtAss(CSyntaxNode*pTree)
{
	Node_Priority np = { pTree, 4 };
	m_PrgmPrsnt.push_back(np);
}

///输出当前状态赋值语句
///2015-3-10 add by wangmeng
void CMSV::PrintPrsnt()
{
	vector<Node_Priority>::iterator iter;
	for (iter = m_PrgmPrsnt.begin(); iter != m_PrgmPrsnt.end(); iter++)
	{
		cout << (*iter).node->TreeToString() << "  ";
	}
}
//-----------------------------------------------------------------------------
///将当前状态赋值的链表加入到IRTree中
///2015-3-10 add by wangmeng
void CMSV::__AdPrst2IR()
{
	vector<Node_Priority>::iterator iter;
	CSyntaxTree* pPrsntTree = new CSyntaxTree();
	if (!g_InNewFunc)
	{
		for (iter = m_PrgmPrsnt.begin(); iter != m_PrgmPrsnt.end(); iter++)
		{
			pPrsntTree->MergeTree((*iter).node, CHOP_STA, VOIDTYPE);
		}
	}
	else
	{
		vector<string> SymTbl;
		string print = "";
		if (!m_PrgmPrsnt.empty())
		{
			if (m_currFunc != "")
			{
				SymTbl.push_back("##" + m_currFunc);//输出函数名
			}
			//先PUSH，然后暂停
			for (iter = m_PrgmPrsnt.begin(); iter != m_PrgmPrsnt.end(); iter++)
			{
				CSyntaxNode* pushStmt = __ConstructPushStmt((*iter).node, (*iter).priority, false);
				pPrsntTree->MergeTree(pushStmt, CHOP_STA, VOIDTYPE);
			}

			pPrsntTree->MergeTree(__CallPauseThread(), CHOP_STA, VOIDTYPE);
			
			
			//前n-1个语句的每个语句之后加暂停，第n个语句之后不加暂停
			int i = 0;
			int size = m_PrgmPrsnt.size() - 1;
			for (iter = m_PrgmPrsnt.begin(); i<size; i++, iter++)
			{
				pPrsntTree->MergeTree((*iter).node, CHOP_STA, VOIDTYPE);
				string varName = __GetOutputVarName((*iter).node);
				if (varName != "")
				{
					SymTbl.push_back(varName);
				}

				pPrsntTree->MergeTree(__CallPauseThread(), CHOP_STA, VOIDTYPE);
			}
            pPrsntTree->MergeTree((*iter).node, CHOP_STA, VOIDTYPE);
			string varName = __GetOutputVarName((*iter).node);
			if (varName != "")
			{
				SymTbl.push_back(varName);
			}

			if (m_More == 1)//不是最后一个状态，则在后面加暂停
			{
				if (Setting::IsModeling())//用于得到语句编号即程序位置
				{
					CSyntaxNode* getStatementNum = new CSyntaxNode(FUNCTION_STA, VOIDTYPE);
					getStatementNum->SetChild0(new CSyntaxNode(IDENT_EXP, "addStatement", nullptr, VOIDTYPE));
					getStatementNum->SetChild1(new CSyntaxNode(OPTION_EXP, new CSyntaxNode(INTEGER_EXP, m_StatementNum, VOIDTYPE), VOIDTYPE));
					m_StatementNum++;
					pPrsntTree->MergeTree(getStatementNum, CHOP_STA, VOIDTYPE);
				}
				
				pPrsntTree->MergeTree(__CallPauseThread(), CHOP_STA, VOIDTYPE);
			}	
		}
		else if (!m_IsComplexStmt && m_More==1)
		{
			if (Setting::IsModeling())//用于得到语句编号即程序位置
			{
				CSyntaxNode* getStatementNum = new CSyntaxNode(FUNCTION_STA, VOIDTYPE);
				getStatementNum->SetChild0(new CSyntaxNode(IDENT_EXP, "addStatement", nullptr, VOIDTYPE));
				getStatementNum->SetChild1(new CSyntaxNode(OPTION_EXP, new CSyntaxNode(INTEGER_EXP, m_StatementNum, VOIDTYPE), VOIDTYPE));
				m_StatementNum++;
				pPrsntTree->MergeTree(getStatementNum, CHOP_STA, VOIDTYPE);
			}
			
			pPrsntTree->MergeTree(__CallPauseThread(), CHOP_STA, VOIDTYPE);
		}	
	}
	m_PrgmPrsnt.clear();
	if (pPrsntTree->GetRoot() != nullptr)
	{
		m_IRTree->MergeTree(pPrsntTree->GetRoot(), CHOP_STA, VOIDTYPE);
		pPrsntTree->SetRoot(nullptr);
		delete pPrsntTree;
		pPrsntTree = nullptr;
	}
}
//-----------------------------------------------------------------------------
string CMSV::__GetOutputVarName(CSyntaxNode* pPresent)
{
	//得到赋值号左边的变量的名字
	if (pPresent->GetNType() == ASS_EQU_EXP || pPresent->GetNType() == EX_ASS_EXP)
	{
		string varName = pPresent->GetChild0()->GetNameNode()->GetNName();
		if (varName.substr(0, 2) != "$$" && __IsLocalVar(varName))//是局部变量并且不是临时变量
		{	 
			 return varName;//将左值存入要输出的符号表
		}
	}
	return "";
}
//-----------------------------------------------------------------------------
bool CMSV::__IsLocalVar(const string varName)
{
		if (m_LocVarTree->GetRoot() != nullptr || m_FuncFormalPar != nullptr)
		{
			if (m_LocVarTree->GetRoot() != nullptr)//在局部变量中找
			{
				CSyntaxNode* visit = m_LocVarTree->GetRoot();
				while (visit->GetNType() == CHOP_STA)
				{
					if (visit->GetChild1()->GetNameNode()->GetNName() == varName)
					{
						return true;
					}
					visit = visit->GetChild0();
				}
				if (visit->GetNameNode()->GetNName() == varName)
				{
					return true;
				}
			}
			if (m_FuncFormalPar != nullptr)//在形参中找
			{
				CSyntaxNode* visit = m_FuncFormalPar;
				while (visit != nullptr)
				{
					if (visit->GetNameNode()->GetNName() == varName)
					{
						return true;
					}
					visit = visit->GetChild1();
				}
			}
		}
        
		return false;
}
//-----------------------------------------------------------------------------
CSyntaxNode* CMSV::__CallPauseThread()
{
	CSyntaxNode* myPause = new CSyntaxNode(FUNCTION_STA, VOIDTYPE);
	myPause->SetChild0(new CSyntaxNode(IDENT_EXP, MYPAUSETHRED, nullptr, VOIDTYPE));
	return myPause;
}
//-----------------------------------------------------------------------------
CSyntaxNode* CMSV::__ConstructPushStmt(CSyntaxNode* pStmt, int pPriority, bool pIsCond)
{
	CSyntaxNode* pushStmt = new CSyntaxNode(FUNCTION_STA, VOIDTYPE);
	pushStmt->SetChild0(new CSyntaxNode(IDENT_EXP, MYPUSH, nullptr, VOIDTYPE));


	///用于生成PUSH语句
	Asss_Struct* assStruct = new Asss_Struct;
	string leftValue = "";
	vector<string> right;
	assStruct->priority = pPriority;
	if (!pIsCond)
	{
		__GetAsssStruct(pStmt, *assStruct);
		if (!assStruct->left.empty())
		{
			leftValue = (assStruct->left).at(0);
		}
		right = assStruct->right;

	}
	else
	{
		__GetRightId(pStmt, right);
	}

	string rightValue = "";
	vector<string>::iterator it;
	for (it = right.begin(); it != right.end(); it++)
	{
		rightValue += *it + " ";
	}
	delete assStruct;

	CSyntaxNode* actualParList = new CSyntaxNode(OPTION_EXP, new CSyntaxNode(STR_EXP, leftValue, VOIDTYPE), VOIDTYPE);
	actualParList->SetChild1(new CSyntaxNode(OPTION_EXP, new CSyntaxNode(STR_EXP, rightValue, VOIDTYPE), VOIDTYPE));
	actualParList->GetChild1()->SetChild1(new CSyntaxNode(OPTION_EXP, new CSyntaxNode(INTEGER_EXP, pPriority, INTTYPE), VOIDTYPE));
	pushStmt->SetChild1(actualParList);

	return pushStmt;
}
///输出m_IRTree
///2015-3-11 add by wangmeng
string CMSV::PrintIRTree()
{
	if (m_IRTree->GetRoot())
	{
		return m_IRTree->GetRoot()->TreeToString();
	}
	else
	{
		return "";
	}
}

///可支持frame(x) and frame(y) 这种形式的版本
///当得到区间长度后处理对应的unframe语句
///2015-3-13 add by wangmeng
void CMSV::__AddUnFrm()
{
	/*	CSyntaxNode*iter;

	for (;  m_Frm!=0&&!m_UnFrm.empty();m_Frm--) ///遍历m_UnFrm,处理每个元素
	{
	iter=m_UnFrm.top();

	if (!m_PrgmFuture->MergeTree(iter,CHOP_STA,VOIDTYPE))
	{
	m_RdcOk=0;
	PrintErrorMassage(0,  "__AddUnFrm() MergeTree() error!");
	for (; m_Frm!=0&&!m_UnFrm.empty();m_Frm--)
	{
	iter=m_UnFrm.top();
	delete iter;
	m_UnFrm.pop();
	}
	return;
	}
	m_UnFrm.pop();
	}
	*/

}


///释放frame变量
///2015-3-16 add by wangmeng
void CMSV::__FreeSymbl()
{
	CSyntaxNode* iter = nullptr;
	while (!m_UnFrm.empty())///将m_UnFrm中的项弹栈，挨个frame释放
	{
		iter = m_UnFrm.top()->GetChild0();

		while (iter != nullptr)///释放frame中的每个变量
		{
			if (!m_FrmSymbl->DelFrm(iter->GetChild0()->GetNName()))
			{
				PrintErrorMassage(0, "__FreeSymbl() " + iter->GetChild0()->GetNName());
				m_RdcOk = 0;
				return;
			}
			iter = iter->GetChild1();
		}

		m_UnFrm.pop();
	}
	m_FreeFrm = 0;
}

/**
* 获得节点后代中的所有标识符
* @param pTree要遍历的节点，right存放获得的标识符
*/
///2015-03-10 add by shiyifang
void CMSV::__GetRightId(CSyntaxNode *pTree, vector<string> &right)
{
	if (pTree == nullptr)
		return;

	//节点类型为标识符时，查找符号表中是否有此名称的标识符
	//有则加入存放右值的vector，否则报错返回
	if (pTree->GetNType() == IDENT_EXP)
	{
		///2015-4-21 add by wangmeng 加入全局符号表
		Symbol* symbol = m_GlbSymbl->FindSymbol(pTree->GetNName());

		///end by wangmeng

		if (symbol == nullptr)
		{
			symbol = m_FrmSymbl->FindSymbol(pTree->GetNName());
		}

		if (symbol == nullptr)
		{
			symbol = m_NFrmSymbl->FindSymbol(pTree->GetNName());
		}

		if (symbol == nullptr)
		{
			//cout << "__GetRightId error" << endl;
			//return;
		}

		right.push_back(pTree->GetNName());
		return;
	}

	//节点类型为数组时，查找符号表中是否有此名称的标识符
	//有则判断下标是否越界是否为整数，无则报错返回
	//下标正确则加入存放右值的vector，否则报错返回
	if (pTree->GetNType() == ARRAY_EXP)
	{
		CSyntaxNode *child = pTree->GetChild1();
		switch (child->GetNType())
		{
		case INTEGER_EXP: {
			if (child->GetiValue() < 0)
			{
				PrintErrorMassage(0, "__GetRightId: array overflow : " + pTree->GetNName()); 
				m_RdcOk = 0; 
				return;
			}
			right.push_back(pTree->GetNName().append("[").append(to_string(child->GetiValue())).append("]")); return;
		}
		case FLOATLITERAL_EXP: {
			PrintErrorMassage(0, "__GetRightId: the index of array should be an integer: " + pTree->GetNName()); 
			m_RdcOk = 0; 
			return; 
		}

		default: { __GetRightId(child, right);
			if (m_RdcOk == 0) return;
			right.push_back(pTree->TreeToString()); return; }///add by syf 2015-4-9
		}
	}

	///add by syf 2015-4-9
	if (pTree->GetNType() == DOT_EXP || pTree->GetNType() == ARROW_EXP)
	{
		__GetRightInStructMember(pTree, right);
		if (m_RdcOk == 0) return;

		right.push_back(pTree->TreeToString());
		return;
	}

	if (pTree->GetNType() == POINTER_EXP)
	{
		right.push_back(pTree->TreeToString());
	}

	//其他类型节点
	__GetRightId(pTree->GetChild0(), right);
	__GetRightId(pTree->GetChild1(), right);
}


/**2015-3-9 add by shiyifang
*
* 对赋值语句排序的判断条件，算法类中sort函数的参数之一
* @paragram：vector中带比较的两个节点
* return : 比较之后的布尔值
*/
bool Comp(const Ass_Struct &a, const Ass_Struct &b)
{
	if (b.right.empty())
		return false;
	if (a.right.empty())
		return true;
	return (find(a.right.begin(), a.right.end(), b.left.at(0)) == a.right.end());
}

/*节点类型为赋值时 获得赋值的左值和右值
* @param pTree(赋值节点)  stru(存放标识符的结构体)
*/
///2015-03-12 add by shiyifang
void CMSV::__GetAssStruct(CSyntaxNode*pTree, Ass_Struct &stru)
{
	//将右边的值加入右值vector中
	__GetRightId(pTree->GetChild1(), stru.right);

	//将左边待赋值的值加入左值vector中，其余加入右值vector
	//如*(p+a+b[10])
	CSyntaxNode* left = pTree->GetChild0();
	switch (left->GetNType())
	{
	//左边以指针开始时，遍历掉所有开始的指针，调用__GetLeftId()
	case POINTER_EXP: {

		__GetRightId(left, stru.right);
		stru.left.push_back(left->TreeToString());
		break;
	}
	case ARRAY_EXP:
	{
		if (left->GetChild0()->GetNType() == IDENT_EXP)//只考虑数组名是简单变量的情况
		{
			auto name = left->GetChild0()->GetNName();
			Symbol* symbol = m_GlbSymbl->FindSymbol(name);
			if (symbol == nullptr)
			{
				symbol = m_FrmSymbl->FindSymbol(left->GetChild0()->GetNName());
			}

			if (symbol == nullptr)
				symbol = m_NFrmSymbl->FindSymbol(left->GetChild0()->GetNName());
			if (symbol == nullptr)
			{
				PrintErrorMassage(0, "__GetAssAtruct: the symbol does not exist 1: " + left->GetNName());
				m_RdcOk = 0;
				return;
			}
			if (!(symbol->IsArray() || SymbolTable::IsPointer(symbol->GetSymbolType())))
			{
				PrintErrorMassage(0, "__GetAssStruct:  the symbol is not an array: " + pTree->GetNName());
				m_RdcOk = 0;
				return;
			}
		}
		CSyntaxNode * child = left->GetChild1();
		switch (child->GetNType())
		{
		case INTEGER_EXP: {
			if (child->GetiValue() < 0)
			{
				PrintErrorMassage(0, "__GetAssAtruct:  array overflow : " + left->GetNName() );
				m_RdcOk = 0; 
				return;
			}
			stru.left.push_back(left->GetNName().append("[").append(to_string(child->GetiValue())).append("]")); break;
		}
		case FLOATLITERAL_EXP: {
			PrintErrorMassage(0, "__GetRightId: the index of array should be an integer: " + left->GetNName()); 
			m_RdcOk = 0; 
			return; 
		}
		default: { __GetRightId(child, stru.right);
			if (m_RdcOk == 0) return;
			stru.left.push_back(left->TreeToString()); break; }///add by syf 2015-4-9
		}

		break;
	}

	//左边以标识符开始时，左值就是此标识符
	//还需判断标识符是否存在
	case IDENT_EXP: {

		///2015-4-21 add by wangmeng 加入全局变量
		Symbol* symbol = m_GlbSymbl->FindSymbol(left->GetNName());

		if (symbol == nullptr)
		{
			symbol = m_FrmSymbl->FindSymbol(left->GetNName());
		}

		///end by wangmeng


		if (symbol == nullptr)
		{
			symbol = m_NFrmSymbl->FindSymbol(left->GetNName());
		}

		if (symbol == nullptr)
		{
			PrintErrorMassage(0, "__GetAssAtruct: the symbol does not exist :" + left->GetNName());
			m_RdcOk = 0;
			return;
		}

		stru.left.push_back(left->GetNName());
		break;
	}
					//将数组初始化拆开之后增加的情况
	case ARRAY_DECLARE_STA: { return; }
							///add by shiyifang 2015-4-9
	case DOT_EXP:
	case ARROW_EXP:{

		__GetRightInStructMember(left, stru.right);

		if (m_RdcOk == 0) return;

		stru.left.push_back(left->TreeToString());///结构体加入left
		break;
	}

	default: {
		PrintErrorMassage(0, "__the type has not been considered !!!" ); /*m_RdcOk = 1;*/ 
		return; 
	}
	}

	//如果左值不为1，结果错误
	if (stru.left.size() != 1)
	{
		PrintErrorMassage(0, "__GetAssAtruct:the left value is error, too more or too less" );
		m_RdcOk = 0;
	}

}
void CMSV::__GetAsssStruct(CSyntaxNode*pTree, Asss_Struct &stru)
{
	if (pTree->GetNType() == REQUEST_STA || pTree->GetNType() == DISPLAY_STA)
	{
		return;
	}
	
	//将右边的值加入右值vector中
	__GetRightId(pTree->GetChild1(), stru.right);

	//将左边待赋值的值加入左值vector中，其余加入右值vector
	//如*(p+a+b[10])
	CSyntaxNode* left = pTree->GetChild0();

	switch (left->GetNType())
	{
		//左边以指针开始时，遍历掉所有开始的指针，调用__GetLeftId()
	case POINTER_EXP: {

		__GetRightId(left, stru.right);
		stru.left.push_back(left->TreeToString());
		break;
	}

					  //左边以数组开始时，可以确定左值为此数组
					  //还需判断是否有此数组和是否越界
	case ARRAY_EXP:
	{

		///2015-4-21 add by wangmeng 加入全局变量
		Symbol* symbol = m_GlbSymbl->FindSymbol(left->GetChild0()->GetNName());

		if (symbol == nullptr)
		{
			symbol = m_FrmSymbl->FindSymbol(left->GetChild0()->GetNName());
		}

		///end by wangmeng

		if (symbol == nullptr)
			symbol = m_NFrmSymbl->FindSymbol(left->GetChild0()->GetNName());
		if (symbol == nullptr)
		{
			PrintErrorMassage(0, "__GetAssAtruct: the symbol does not exist 1: " + left->GetNName());
			m_RdcOk = 0;
			return;
		}
		if (!(symbol->IsArray() || SymbolTable::IsPointer(symbol->GetSymbolType())))
		{
			PrintErrorMassage(0, "__GetAssStruct:  the symbol is not an array: " + pTree->GetNName());
			m_RdcOk = 0;
			return;
		}
		CSyntaxNode * child = left->GetChild1();
		switch (child->GetNType())
		{
		case INTEGER_EXP: {
			if (child->GetiValue() < 0)
			{
				PrintErrorMassage(0, "__GetAssAtruct:  array overflow : " + left->GetNName()); 
				m_RdcOk = 0; 
				return;
			}
			stru.left.push_back(left->GetNName().append("[").append(to_string(child->GetiValue())).append("]")); break;
		}
		case FLOATLITERAL_EXP: {
			PrintErrorMassage(0, "__GetRightId: the index of array should be an integer: " + left->GetNName());
			m_RdcOk = 0; 
			return; 
		}
		default: { __GetRightId(child, stru.right);
			if (m_RdcOk == 0) return;
			stru.left.push_back(left->TreeToString()); break; }///add by syf 2015-4-9
		}

		break;
	}

	//左边以标识符开始时，左值就是此标识符
	//还需判断标识符是否存在
	case IDENT_EXP: {

		///2015-4-21 add by wangmeng 加入全局变量
		Symbol* symbol = m_GlbSymbl->FindSymbol(left->GetNName());

		if (symbol == nullptr)
		{
			symbol = m_FrmSymbl->FindSymbol(left->GetNName());
		}

		///end by wangmeng


		if (symbol == nullptr)
		{
			symbol = m_NFrmSymbl->FindSymbol(left->GetNName());
		}

		stru.left.push_back(left->GetNName());
		break;
	}
					//将数组初始化拆开之后增加的情况
	case ARRAY_DECLARE_STA: { return; }
							///add by shiyifang 2015-4-9
	case DOT_EXP:
	case ARROW_EXP:{

		__GetRightInStructMember(left, stru.right);

		if (m_RdcOk == 0) return;

		stru.left.push_back(left->TreeToString());///结构体加入left
		break;
	}

	default: {
		PrintErrorMassage(0, "__the type has not been considered !!!"); /*m_RdcOk = 0*/;
		return; 
	}
	}

	//如果左值不为1，结果错误
	if (stru.left.size() != 1)
	{
		PrintErrorMassage(0, "__GetAsssAtruct:the left value is error, too more or too less");
		m_RdcOk = 0;
	}

}
void CMSV::__GetRightInStructMember(CSyntaxNode* pTree, vector<string> &right)
{
	//先得到a.b[i]中的a
	CSyntaxNode* visit = pTree;
	while (visit->GetNType() == DOT_EXP || visit->GetNType() == ARROW_EXP)
	{
		visit = visit->GetChild0();
	}

	__GetRightId(visit, right);

	//得到a.b[i]中的i
	stack <CSyntaxNode*> nodeStack;
	nodeStack.push(pTree);
	while (!nodeStack.empty())
	{
		CSyntaxNode* node = nodeStack.top();
		nodeStack.pop();
		if (node->GetNType() == DOT_EXP || node->GetNType() == ARROW_EXP)
		{
			nodeStack.push(node->GetChild0());
			nodeStack.push(node->GetChild1());
		}
		else if (node->GetNType() == ARRAY_EXP)
		{
			__GetRightId(node->GetChild1(), right);
		}
	}
}


/*
* 为获得的所有当前状态排序,声明并检查变量是否存在
* 排序顺序: EX_ASS 声明 ASS_EXP
* 排序规则：使用快排，比较左值是否包含右值
*/
///2015-03-13 add by shiyifang
void CMSV::__SortPrsnt()
{
	vector<CSyntaxNode*> others, prsnt_ass, return_sta, function_call, efunction_call, input_sta, output_sta, ex_tree;//others:除了赋值语句的所有语句，result：排序结果
	vector<Ass_Struct>  ass_id;//辅助存储结构体
	vector<Node_Priority> result;
	Node_Priority np;

	int i;//循环变量

	int NumOfEX = 0;
	//将m_PrgmPrsnt分类存储
	for (i = 0; i < m_PrgmPrsnt.size(); i++)
	{
		CSyntaxNode* pTree = m_PrgmPrsnt.at(i).node;

		switch (pTree->GetNType())
		{
		case EX_ASS_EXP: {
			Ass_Struct stru;
			stru.seq = i;
			__GetAssStruct(pTree, stru);
			if (m_RdcOk == 0) { 
				PrintErrorMassage(0, pTree->TreeToString()); 
				return;
			}
			NumOfEX++;
			ex_tree.push_back(pTree);
			break; }

		case ASS_EQU_EXP: {prsnt_ass.push_back(pTree); break; }

		case RETURN_STA: {return_sta.push_back(pTree); break; }
		case FUNCTION_STA: {function_call.push_back(pTree); break; }
		case EXT_FUNCTION_STA: {efunction_call.push_back(pTree->GetChild0()); break; }

		case REQUEST_STA:{input_sta.push_back(pTree); break; }

		case DISPLAY_STA:{output_sta.push_back(pTree); break; }

		default: {others.push_back(pTree);  break; }
		}
	}

	vector<CSyntaxNode*>::iterator iter;
	if (NumOfEX > 1 || g_InNewFunc)
	{
		///处理由上一状态x:=y+z得到的x<==y+z	
		for (iter = ex_tree.begin(); iter < ex_tree.end(); iter++)
		{
			///x前面*的个数
			int nValue = 0;
			CSyntaxNode* NameNode = (*iter)->GetChild0()->GetNameNode(nValue);
			string OldName = NameNode->GetNName();
			Symbol *Var = nullptr;
			Var = m_FrmSymbl->FindSymbol(OldName);
			if (Var == nullptr)
			{
				Var = m_GlbSymbl->FindSymbol(OldName);
			}
			if (Var == nullptr)
			{
				PrintErrorMassage(0, "Variable " + OldName + " is not defined" );
				m_RdcOk = 0;
				return;
			}
			string NewName = "$$$" + OldName;
			///对 $$$x 进行声明
			CSyntaxNode* DecTree = Var->GetDecTree(NewName, nValue);
			if (DecTree == nullptr)
			{
				PrintErrorMassage(0, "__SortPrsnt define variable error!");
				m_RdcOk = 0;
				return;
			}
			//声明临时变量
			/*m_GlbVarTree->MergeTree(DecTree, CHOP_STA);
			CSyntaxNode* tmpVar = new CSyntaxNode(IDENT_EXP, NewName, nullptr, VOIDTYPE);
			//$$$x=y+z
			CSyntaxNode* AssTree = new CSyntaxNode(ASS_EQU_EXP, VOIDTYPE);
			AssTree->SetChild0(tmpVar);
			AssTree->SetChild1((*iter)->GetChild1());
			np.node = AssTree;
			np.priority = 0;
			result.push_back(np);
			//x=$$$x
			(*iter)->SetChild1(new CSyntaxNode(IDENT_EXP, NewName, nullptr, VOIDTYPE));*/
			np.node = *iter;
			np.priority = 1;
			result.push_back(np);
		}
	}
	else
	{
		for (iter = ex_tree.begin(); iter < ex_tree.end(); iter++)
		{
			np.node = *iter;
			np.priority = 1;
			result.push_back(np);
		}
	}
	
	ex_tree.clear();
	m_NFrmSymbl->Clear();
	///删除上一状态Frame符号表
	///2015-4-16 add by wangmeng 
	if (!m_UnFrm.empty())
	{
		__FreeSymbl();
		if (m_RdcOk == 0)
		{
			PrintErrorMassage(0, "__SortPrsnt() delete frame variable error!");
			return;
		}
	}
	///2015-3-17 add by wangmeng
	if (m_FreeFrm == 1)
	{
		CSyntaxNode* iter = nullptr;
		while (!m_PreUnFrm.empty())///将m_UnFrm中的项弹栈，挨个frame释放
		{
			iter = m_PreUnFrm.top();
			m_UnFrm.push(iter);
			m_PreUnFrm.pop();
		}
	}
	vector<Ass_Struct>::iterator it1;
	vector<CSyntaxNode*>::iterator it2;
	//处理声明语句，调用__SearchAndInsert()插入声明的变量到符号表中
	for (it2 = others.begin(); it2 < others.end(); it2++)
	{
		if ((*it2)->GetNType() == DECLARE_STA)
		{
			if (!__InstDec2ST(*it2))
			{
				return;
			}
		}
		//result.push_back(*it2);
	}

	///因为指针指向的内容已经被加入到变量树里边了，因此这里不用delete来释放内存
	others.clear();

	//处理输入语句，输入语句需要放在声明语句之后执行
	for (it2 = input_sta.begin(); it2 < input_sta.end(); it2++)
	{
		np.node = *it2;
		np.priority = 2;
		result.push_back(np);
	}

	for (it2 = function_call.begin(); it2 != function_call.end(); it2++)
	{
		np.node = *it2;
		np.priority = 2;//yangkai 将函数调用排在赋值语句前面
		result.push_back(np);
	}

	//将ASS_EXP的结点存入结构体中
	for (it2 = prsnt_ass.begin(), i = 0; it2 != prsnt_ass.end(); it2++, i++)
	{
		CSyntaxNode* pTree = *it2;
		Ass_Struct stru;
		stru.seq = i;
		__GetAssStruct(pTree, stru);
		if (m_RdcOk == 0)
		{
			PrintErrorMassage(0, (*it2)->TreeToString());
			return;
		}
		ass_id.push_back(stru);

	}

	//对结构体进行排序
	sort(ass_id.begin(), ass_id.end(), Comp);

	///按序号将排序后的结果加入到result中
	for (it1 = ass_id.begin(); it1 < ass_id.end(); it1++)
	{
		np.node = prsnt_ass.at(it1->seq);
		np.priority = 4;
		result.push_back(np);
	}

	//处理输出语句，输出语句需放在最后(return语句之前)执行
	for (it2 = output_sta.begin(); it2 < output_sta.end(); it2++)
	{
		np.node = *it2;
		np.priority = 5;
		result.push_back(np);
	}

	//将return语句加入到最后
	for (it2 = return_sta.begin(); it2 != return_sta.end(); it2++)
	{
		np.node = *it2;
		np.priority = 4;
		result.push_back(np);
	}
	
	for (it2 = efunction_call.begin(); it2 != efunction_call.end(); it2++)
	{
		np.node = *it2;
		np.priority = 4;
		result.push_back(np);
	}

	m_PrgmPrsnt.clear();
	m_PrgmPrsnt = result;
}

/*声明变量时 插入所有变量到符号表
* type(声明类型)  pTree(声明节点)
*/
bool CMSV::__SearchAndInsert(SymbolType type, CSyntaxNode *pTree, string name, bool IsPar, bool IsGlb, int AddrNum)
{
	if (pTree == nullptr)
		return true;

	if (pTree->GetNType() == IDENT_EXP){ //节点类型为标识符时
		if (type == UNDEFINE){ //变量声明的类型是UNDEFINE，代表将变量插入到Frame中
			if (IsGlb)
				return m_GlbSymbl->InstSymbol(pTree->GetNameNode()->GetNName(), new Symbol());
			else
				return m_FrmSymbl->InstFSymbl(pTree->GetNameNode()->GetNName());
		}

		Symbol *symbol = nullptr;
		if (!g_InFunc){
			// 从全局变量表中查找
			symbol = m_GlbSymbl->FindSymbol(pTree->GetNameNode()->GetNName());
		}

		if (symbol == nullptr)
			symbol = m_FrmSymbl->FindSymbol(pTree->GetNameNode()->GetNName());

		
		if (symbol == nullptr){ //Frame中没有这个变量，直接插入到非Frame中
			if (IsPar)
				return m_FrmSymbl->InstSymbol(pTree->GetNameNode()->GetNName(), new Symbol(type, AddrNum, name));
			else
				return m_NFrmSymbl->InstSymbol(pTree->GetNameNode()->GetNName(), new Symbol(type, AddrNum, name));
		}
		else if (symbol->GetSymbolType() == UNDEFINE){ //Frame中有这个变量，类型是UNDEFINE，改变类型
			symbol->SetSymbolType(type);
			symbol->SetSName(name);
			symbol->SetAddrNum(AddrNum);
			return true;
		}
		else{ //Frame中已有这个变量，类型也确定，不能插入
			PrintErrorMassage(0, "__SearchAndInsert : variable " + pTree->GetNName() + " has been declarated in the frame table.");
			return false;
		}
	}
	else if (pTree->GetNType() == ARRAY_DECLARE_STA || pTree->GetNType() == ARRAY_PARAMETER_EXP){
		CSyntaxNode *child = pTree->GetChild0();

		vector<int> ArrD;
		CSyntaxNode* vsArrD = pTree->GetChild1();

		if (!IsPar){
			while (vsArrD != nullptr){
				ArrD.push_back(vsArrD->GetChild0()->GetiValue());
				vsArrD = vsArrD->GetChild1();
			}
		}

		Symbol *symbol = nullptr;
		if (!g_InFunc){
			//从全局变量表中查找
			symbol = m_GlbSymbl->FindSymbol(pTree->GetNameNode()->GetNName());
		}

		if (symbol == nullptr){
			symbol = m_FrmSymbl->FindSymbol(pTree->GetNameNode()->GetNName());
		}

		//Frame中没有这个变量，直接插入到非Frame中
		if (symbol == nullptr){
			if (IsPar){
				vector<int> t;
				return m_FrmSymbl->InstSymbol(child->GetNameNode()->GetNName(), new Symbol(type, true, name, t, AddrNum));
			}
			else{
				return m_NFrmSymbl->InstSymbol(pTree->GetNameNode()->GetNName(), new Symbol(type, true, name, ArrD, AddrNum));
			}

		}
		else if (symbol->GetSymbolType() == UNDEFINE){ //Frame中有这个变量，类型是UNDEFINE，改变类型，数组长度
			symbol->SetSymbolType(type);
			symbol->SetIsArray(true);
			symbol->SetSName(name);
			symbol->SetArrayNum(&ArrD);
			symbol->SetAddrNum(AddrNum);
			return true;
		}
		else{ //Frame中已有这个变量，类型也确定，不能插入
			PrintErrorMassage(0, "__SearchAndInsert : variable" + pTree->GetNName() + " has been declarated in the frame table");
			return false;
		}
	}
	else if (pTree->GetNType() == POINTER_EXP)
	{
		//类型不能是UNDEFINE
		if (type == UNDEFINE || SymbolTable::IsPointer(type)){
			PrintErrorMassage(0, "__SearchAndInsert : pointer variable : " + pTree->GetNName() + " must be declarated as a pointer type:");
			return false;
		}

		// 多重指针变量声明
		AddrNum++;
		pTree = pTree->GetChild0();

		while (pTree->GetNType() == POINTER_EXP){
			AddrNum++;
			pTree = pTree->GetChild0();
		}
		return  (__SearchAndInsert(SYMBOLPOINTER, pTree, name, IsPar, IsGlb, AddrNum)); //(SymbolType)(1 + type)
	}
	else if (pTree->GetNType() == ASS_EQU_EXP){
		return __SearchAndInsert(type, pTree->GetChild0(), name, IsPar, IsGlb, AddrNum);
	}
	bool r1 = __SearchAndInsert(type, pTree->GetChild0(), name, IsPar, IsGlb, AddrNum);
	bool r2 = __SearchAndInsert(type, pTree->GetChild1(), name, IsPar, IsGlb, AddrNum);
	return r1 && r2;
}

/**
* 该函数用于将结构体定义插入到符号表中
* @param pTree（结构体定义）
* @return void
*/
///2015-4-9  add by shiyifang
void CMSV::__InitStructDel(CSyntaxNode *pTree)
{
	if (m_RdcOk == 0 || pTree == nullptr)
		return;

	if (pTree->GetNType() == DECLARE_STA)
	{
		if (!__SearchAndInsert(SymbolTable::GetSymbolType(pTree->GetRType()), pTree, pTree->GetNName()))
		{
			PrintErrorMassage(0, "__SortPrsnt: declaration of variable fail : " + pTree->TreeToString());
			m_RdcOk = 0;
		}
		return;
	}

	__InitStructDel(pTree->GetChild0());
	__InitStructDel(pTree->GetChild1());
}

void  CMSV::Simulate()
{
	///2015-4-21 add by wangmeng  加入全局变量
	CSyntaxNode* GlbTree = m_Program->GetRoot();
	if (GlbTree!=nullptr && GlbTree->GetNType() == FRAME_STA)
	{

		///2015-06-18 add by shiyifang
		///处理过多的frame变量导致的栈溢出
		bool insertOK = true;
		CSyntaxNode *temp = GlbTree;
		temp = temp->GetChild0();
		while (temp != nullptr && temp->GetChild0() != nullptr)
		{
			insertOK = insertOK & __SearchAndInsert(UNDEFINE, temp->GetChild0(), "", false, true);
			temp = temp->GetChild1();
		}

		if (insertOK)
		{
			m_Program->SetRoot(GlbTree->GetChild1());
			GlbTree->SetChild1(nullptr);
			delete GlbTree;
			GlbTree = nullptr;
		}
	}
	///end by wangmeng

	Pre();

	if (m_RdcOk == 0)
	{
		PrintErrorMassage(0, "simulate error!" );
		return;
	}
	m_FrmSymbl->Clear();
	m_NFrmSymbl->Clear();

	///2015-3-31 add by wangmeng
	//modified by yubin 2015/4/23
	if (m_FuncFlag != 1 && m_PreFlag != 1)
	{
		if (!g_InNewFunc  && m_CurrentFunc != "$$changeState")
		{
			__OutputState();
		}
	}

	CSyntaxNode* tmpIR = m_IRTree->GetRoot();
	m_IRTree->SetRoot(m_LocVarTree->GetRoot());
	m_LocVarTree->SetRoot(nullptr);
	if (tmpIR != nullptr)
	{
		m_IRTree->MergeTree(tmpIR, CHOP_STA);
	}
	return;
}

void  CMSV::Model()
{
	///2015-4-21 add by wangmeng  加入全局变量
	CSyntaxNode* GlbTree = m_Program->GetRoot();
	if (GlbTree != nullptr && GlbTree->GetNType() == FRAME_STA)
	{

		///2015-06-18 add by shiyifang
		///处理过多的frame变量导致的栈溢出
		bool insertOK = true;
		CSyntaxNode *temp = GlbTree;
		temp = temp->GetChild0();
		while (temp != nullptr && temp->GetChild0() != nullptr)
		{
			insertOK = insertOK & __SearchAndInsert(UNDEFINE, temp->GetChild0(), "", false, true);
			temp = temp->GetChild1();
		}

		if (insertOK)
		{
			m_Program->SetRoot(GlbTree->GetChild1());
			GlbTree->SetChild1(nullptr);
			delete GlbTree;
			GlbTree = nullptr;
		}
	}
	///end by wangmeng

	Pre();

	if (m_PreFlag != 1 && m_CurrentFunc != "$$changeState")
	{
		__AddEdge(1);
	}

	if (m_RdcOk == 0)
	{
		PrintErrorMassage(0, "Model error!");
		return;
	}
	m_FrmSymbl->Clear();
	m_NFrmSymbl->Clear();

	///2015-3-31 add by wangmeng
	//modified by yubin 2015/4/23
	CSyntaxNode* tmpIR = m_IRTree->GetRoot();
	m_IRTree->SetRoot(m_LocVarTree->GetRoot());
	m_LocVarTree->SetRoot(nullptr);
	if (tmpIR != nullptr)
	{
		m_IRTree->MergeTree(tmpIR, CHOP_STA);
	}

	return;
}

CSyntaxNode* OutPropValue()
{
	return nullptr;
}

///file* FileVar=fopen(FileName,mode);
CSyntaxNode* FOpen(string FileVar, string FileName, string mode)
{
	///FileVar=fopen(FileName,mode);
	CSyntaxNode*AssTree = new CSyntaxNode(ASS_EQU_EXP, new CSyntaxNode(IDENT_EXP, FileVar, nullptr, VOIDTYPE),
		new CSyntaxNode(FOPEN_EXP, VOIDTYPE), VOIDTYPE);
	AssTree->GetChild1()->SetChild0(new CSyntaxNode(STR_EXP, FileName, STRTYPE));
	AssTree->GetChild1()->SetChild1(new CSyntaxNode(STR_EXP, mode, STRTYPE));

	///file* FileVar=fopen(FileName,mode);
	CSyntaxNode* FileOpen = new CSyntaxNode(DECLARE_STA, FILETYPE);
	FileOpen->SetChild0(new CSyntaxNode(DEC_VAR_LIST, VOIDTYPE));

	FileOpen->GetChild0()->SetChild0(new CSyntaxNode(POINTER_EXP, AssTree, VOIDTYPE));

	return FileOpen;
}

CSyntaxNode* FPutS(string PutSValue, string FileVar)
{
	return new CSyntaxNode(FPUTS_EXP, new CSyntaxNode(STR_EXP, PutSValue, STRTYPE),
		new CSyntaxNode(STR_EXP, FileVar, STRTYPE), VOIDTYPE);

}

void  CMSV::Verify()
{

	m_IRTree->MergeTree(FOpen("$$FileVar", "C:\\__SmResult.txt", "w"), CHOP_STA);

	CSyntaxNode *p;
	p = propositionDefineTree;//令p指向存储所有标识符定义的语法树

	while (p != nullptr)
	{
		m_IRTree->MergeTree(FPutS(p->GetNName(), "$$FileVar"), CHOP_STA);
		p = p->GetChild1();
	}

	///2015-4-21 add by wangmeng  加入全局变量
	CSyntaxNode* GlbTree = m_Program->GetRoot();
	if (GlbTree->GetNType() == FRAME_STA)
	{

		///2015-06-18 add by shiyifang
		///处理过多的frame变量导致的栈溢出
		bool insertOK = true;
		CSyntaxNode *temp = GlbTree;
		while (temp != nullptr && temp->GetChild0() != nullptr)
		{
			insertOK = insertOK & __SearchAndInsert(UNDEFINE, temp->GetChild0(), "", false, true);
			temp = temp->GetChild1();
		}

		if (insertOK)
		{
			m_Program->SetRoot(GlbTree->GetChild2());
			GlbTree->SetChild2(nullptr);
			delete GlbTree;
			GlbTree = nullptr;
		}
	}
	///end by wangmeng

	Pre();

	if (m_RdcOk == 0)
	{
		PrintErrorMassage(0, "simulate error!");
		return;
	}
	m_FrmSymbl->Clear();
	m_NFrmSymbl->Clear();

	///2015-3-31 add by wangmeng
	//modified by yubin 2015/4/23
	if (m_FuncFlag != 1 && m_PreFlag != 1)
	{
        __OutputState();
	}

	if (m_LocVarTree->GetRoot() != nullptr)
	{
		m_IRTree->MergeTree_2(m_LocVarTree->GetRoot(), CHOP_STA);
		m_LocVarTree->SetRoot(nullptr);
	}
	return;
}
//-----------------------------------------------------------------------------
void CMSV::__OutputState()
{
	if (g_OutputState)
	{
		vector<string> SymTbl;
		map<string, Symbol*>::iterator iter;
		map<string, Symbol*>* Smap = m_NFrmSymbl->GetSymbolMap();
		map<string, Symbol*>* FSmap = m_FrmSymbl->GetSymbolMap();
		map<string, Symbol*>* GSmap = m_GlbSymbl->GetSymbolMap();
		string print = "";
		//函数调用时根据是内部还是外部调用判断是否需要输出每个状态
		if (m_currFunc != "")
		{
			//add by yubin 2015/5/11,将$$m_currFunc压入SymTbl，输出的时候，可以输出函数名
			SymTbl.push_back("##" + m_currFunc);
		}
		for (iter = Smap->begin(); iter != Smap->end(); ++iter)
		{
			if (!(*(iter->first.c_str()) == '$' && *(iter->first.c_str() + 1) == '$'))
			{
				print += iter->second->PrntSym(iter->first);
				SymTbl.push_back(iter->first);
			}
		}
		for (iter = FSmap->begin(); iter != FSmap->end(); ++iter)
		{
			if (!(*(iter->first.c_str()) == '$' && *(iter->first.c_str() + 1) == '$'))
			{
				print += iter->second->PrntSym(iter->first);
				if (iter->second->GetSymbolType() != UNDEFINE)
				{
					SymTbl.push_back(iter->first);
				}
			}
		}

		m_IRTree->MergeTree(new CSyntaxNode(STATE_OUTPUT_STA, VOIDTYPE, print, SymTbl), CHOP_STA, VOIDTYPE);//print是之前用于打印的，现在没有用
	}

	return;
}
//-----------------------------------------------------------------------------
CSyntaxNode* CMSV::__OutputStateInNewFunc(CSyntaxNode* pPresent)
{
	vector<string> SymTbl;
	map<string, Symbol*>::iterator iter;
	map<string, Symbol*>* Smap = m_NFrmSymbl->GetSymbolMap();
	map<string, Symbol*>* FSmap = m_FrmSymbl->GetSymbolMap();
	map<string, Symbol*>* GSmap = m_GlbSymbl->GetSymbolMap();
	string print = "";
	//函数调用时根据是内部还是外部调用判断是否需要输出每个状态
	if (m_currFunc != "")
	{
		//输出的时候，可以输出函数名
		SymTbl.push_back("##" + m_currFunc);
	}
	if (pPresent->GetNType() == ASS_EQU_EXP || pPresent->GetNType() == EX_ASS_EXP)
	{
		string varName = pPresent->GetChild0()->GetNameNode()->GetNName();
		if (varName.substr(0, 2) != "$$")
		{
			SymTbl.push_back(varName);//将左值存入要输出的符号表
		}
		else
		{
			return nullptr;
		}
	}
	else
	{
		return nullptr;
	}

	/*if (g_OutputState)
	{
		return new CSyntaxNode(STATE_OUTPUT_STA, VOIDTYPE, print, SymTbl);//print是之前用于打印的，现在没有用

		
	}*/
}
//-----------------------------------------------------------------------------
void CMSV::__Input(CSyntaxNode* pTree)
{
	int nValue = 0;
	//先根据要输入的变量名在frame符号表(这里只查找frame符号表)找到该变量
	string varName = pTree->GetNameNode(nValue)->GetNName();

	///2015-4-21 add by wangmeng 加入全局符号表
	Symbol* Var = m_GlbSymbl->FindSymbol(varName);
	if (Var == nullptr)
	{
		Var = m_FrmSymbl->FindSymbol(varName);
	}
	///end by wangmeng


	string scanfStr = "scanf(";
	if (Var == nullptr)
	{
		PrintErrorMassage(0, "__Input error (3674)!");
		return;
	}
	else
	{

		//根据变量类型进行格式化输入
		switch (Var->GetSymbolType())
		{
		case SYMBOLINT:
			scanfStr += "\"%d\""; break;
		case SYMBOLFLOAT:
			scanfStr += "\"%f\""; break;
		case SYMBOLCHAR:
		{
			if (Var->IsArray())
			{
				scanfStr += "\"%[^\\n]\"";;
			}
			else
			{
				scanfStr += "\"%c\"";
			}
			break;
		}

		default:
			break;
		}
	}
	scanfStr += ",";
	scanfStr += "&";
	scanfStr += pTree->GetChild0()->TreeToString();
	scanfStr += ")";

	CSyntaxNode* cptree = new CSyntaxNode();
	cptree->CopyTree(pTree);
	__InsrtAss(cptree);

	m_More = -1;
	m_RdcOk = 1;
}

void CMSV::__Output(CSyntaxNode* pTree)
{
	string printfStr = "";
	CSyntaxNode* cptree = new CSyntaxNode();
	cptree->CopyTree(pTree);
	__InsrtAss(cptree);

	if (pTree->GetChild1() != nullptr){
		__Output(pTree->GetChild1());
	}
	m_More = -1;
	m_RdcOk = 1;
}

string ReturnESC(char c)
{
	switch (c)
	{
	case '\n': {return "\\n"; }
	case '\a': {return "\\a"; }
	case '\b': {return "\\b"; }
	case '\f': {return "\\f"; }
	case '\r': {return "\\r"; }
	case '\t': {return "\\t"; }
	case '\v': {return "\\v"; }
	case '\\': {return "\\\\"; }
	case '\'': {return  "\\'";  }
	case '\"': {return  "\\\""; }
	case '\0': {return  "\\0"; }
			   //case '\d': {s.replace(i, 1, "\\d"); break; }
			   //case '\x': {s.replace(i, 1, "\\x"); break; }
	default:{ return ""; }
	}
}

///2015-5-6 add by wangmeng 替换转义字符 如："\n"换成"\\n"
void ReplaceESC(string& s)
{
	int i = 0;
	int n = s.length();
	while (i < n)
	{
		switch (s.at(i))
		{
		case '\n': {s.replace(i, 1, "\\n"); n++; i += 2; break; }
		case '\a': {s.replace(i, 1, "\\a"); n++; i += 2; break; }
		case '\b': {s.replace(i, 1, "\\b"); n++; i += 2; break; }
		case '\f': {s.replace(i, 1, "\\f"); n++; i += 2; break; }
		case '\r': {s.replace(i, 1, "\\r"); n++; i += 2; break; }
		case '\t': {s.replace(i, 1, "\\t"); n++; i += 2; break; }
		case '\v': {s.replace(i, 1, "\\v"); n++; i += 2; break; }
		case '\\': {s.replace(i, 1, "\\\\"); n++; i += 2; break; }
		case '\'': {s.replace(i, 1, "\\'"); n++; i += 2; break; }
		case '\"': {s.replace(i, 1, "\\\""); n++; i += 2; break; }
		case '\0': {s.replace(i, 1, "\\0"); n++; i += 2; break; }
				   //case '\d': {s.replace(i, 1, "\\d"); break; }
				   //case '\x': {s.replace(i, 1, "\\x"); break; }
		default:{ i++; break; }
		}
	}
}
string CMSV::__OutputStr(CSyntaxNode* pTree)
{
	string printfStr = "printf(";

	//输出字符串 如：output（“hello”） output("\n")
	printfStr += "\"";

	string s = pTree->GetsValue();

	ReplaceESC(s);

	printfStr += s;
	printfStr += "\"";
	printfStr += ")";

	return printfStr;
}
/*
string CMSV::__OutputVar(CSyntaxNode* pTree)
{
	string printfStr = "printf(";

	//输出的是变量，先在符号表中找到该变量
	string varName = pTree->GetNName();

	///2015-4-21 add by wangmeng 加入全局符号表
	Symbol* Var = m_GlbSymbl->FindSymbol(varName);
	if (Var == nullptr)
	{
		Var = m_FrmSymbl->FindSymbol(varName);
	}
	///end by wangmeng

	if (Var == nullptr)
	{
		PrintErrorMassage(0, "variable " + varName + "is not declared" );
		return "";
	}
	else
	{
		//根据变量类型进行格式化输出
		switch (Var->GetSymbolType())
		{
		case SYMBOLINT:
			printfStr += "\"%d\""; break;
		case SYMBOLFLOAT:
			printfStr += "\"%f\""; break;
		case SYMBOLCHAR:
			printfStr += "\"%c\""; break;
		case SYMBOLCHARP: printfStr += "\"%s\""; break;
		case SYMBOLFLOATP:
		case SYMBOLINTP:
			printfStr += "\"%p\""; break;
		default:
			PrintErrorMassage(0, "variable " + varName + " is not declared" );
			return "";
			break;
		}
	}
	printfStr += ",";
	printfStr += varName;
	printfStr += ")";
	return printfStr;
}
*/
/*
string CMSV::__OutputArrayMember(CSyntaxNode* pTree)
{
	string printfStr = "printf(";

	//输出的是变量，先在符号表中找到该变量
	string varName = pTree->GetNName();
	Symbol* Var = m_FrmSymbl->FindSymbol(varName);

	if (Var == nullptr)
	{
		Var = m_GlbSymbl->FindSymbol(varName);
		if (Var == nullptr)
		{
			PrintErrorMassage(0, "__Output error (3714)!" );
			return "";
		}
	}

	if (Var->IsArray() || Var->GetAddrNum() > 1)
	{
		//根据数组类型进行格式化输出
		switch (Var->GetSymbolType())
		{
		case SYMBOLINT:
			printfStr += "\"%d\"";
			break;
		case SYMBOLFLOAT:
			printfStr += "\"%f\"";
			break;
		case SYMBOLCHAR:
			printfStr += "\"%c\"";
			break;
		case SYMBOLINTP:
		case SYMBOLFLOATP:
			printfStr += "\"%p\"";
			break;
		case SYMBOLCHARP:
			printfStr += "\"%s\"";
			break;
		default:
			break;
		}
	}
	else
	{
		switch (Var->GetSymbolType())
		{
		case SYMBOLINT:
		case SYMBOLINTP:
			printfStr += "\"%d\"";
			break;
		case SYMBOLFLOAT:
		case SYMBOLFLOATP:
			printfStr += "\"%f\"";
			break;
		case SYMBOLCHAR:
			printfStr += "\"%c\"";
			break;
		case SYMBOLCHARP:
			printfStr += "\"%s\"";
			break;
		default:
			break;
		}
	}



	printfStr += ",";
	printfStr += pTree->TreeToString();
	printfStr += ")";
	return printfStr;

	m_More = -1;
	m_RdcOk = 1;
}
*/
/*
string CMSV::__OutputTypeCast(CSyntaxNode* pTree)
{
	string printfStr = "printf(";
	switch (pTree->GetRType())
	{
	case INTTYPE:
		printfStr += "\"%d\"";
		break;
	case FLOATTYPE:
		printfStr += "\"%f\"";
		break;
	case CHARTYPE:
		printfStr += "\"%c\"";
		break;
	case INTPTYPE:
	case FLOATPTYPE:
	case CHARPTYPE:
		printfStr += "\"%p\"";
		break;
	default:
		break;
	}
	printfStr += ",";
	printfStr += pTree->TreeToString();
	printfStr += ")";
	return printfStr;
}


string CMSV::__OutputArrayCast(CSyntaxNode* pTree)
{
	string printfStr = "printf(";
	switch (pTree->GetChild0()->GetRType())
	{
	case INTPTYPE:
		printfStr += "\"%d\"";
		break;
	case FLOATPTYPE:
		printfStr += "\"%f\"";
		break;
	case CHARPTYPE:
		printfStr += "\"%c\"";
		break;
	case UINTPTYPE:
		printfStr += "\"%d\"";
		break;
	case UCHARPTYPE:
		printfStr += "\"%x\"";
		break;
	default:
		break;
	}
	printfStr += ",";
	printfStr += pTree->TreeToString();
	printfStr += ")";
	return printfStr;
}*/

/**
* 处理赋值号右边的函数调用,如果有函数掉用，处理后，将m_HasFunCal置为true
* @param1 赋值号右边的语法树
* @param2 是不是当前状态赋值
* @return void
*/

void CMSV::__AssFunc(CSyntaxNode* RAssTree, bool IsPAss)
{
	if (RAssTree == nullptr)
	{
		return;
	}
	switch (RAssTree->GetNType())
	{
	case FUNCTION_STA:
	{
		if (IsPAss)
		{//modified by yubin 2015/4/22

			vector<string>::iterator result = find(m_FuncPNameVec.begin(), m_FuncPNameVec.end(), RAssTree->GetNName());
			if (isFuncExist(RAssTree) || result != m_FuncPNameVec.end())//如果函数存在或者是个函数指针，并且是当前赋值，则提示错误
			{
				PrintErrorMassage(0, "The right current assign cannot exist inner function call:" + RAssTree->GetNName() );
				m_RdcOk = 0;
				return;
			}
			else//如果函数不存在，则当成是库函数调用
			{
				__FuncCall(RAssTree, 0);
				m_RdcOk = 1;
				return;
			}

		}
		else
		{
			m_HasFunCal = true;
			__FuncCall(RAssTree, 0);
			m_RdcOk = 1;
			return;
		}
	}
	case EXT_FUNCTION_STA:
	{
		if (IsPAss)
		{
			__EFuncCall(RAssTree->GetChild0(), 0);
		}
		m_RdcOk = 1;
		return;
	}
	default:
	{
		if (RAssTree->GetChild0() != nullptr)
		{
			__AssFunc(RAssTree->GetChild0(), IsPAss);
		}
		if (RAssTree->GetChild1() != nullptr)
		{
			__AssFunc(RAssTree->GetChild1(), IsPAss);
		}

	}
	}
}
//-----------------------------------------------------------------------------
///2015-5-4 add by yubin,处理if条件中的外部函数调用
void CMSV::__IfWhileCondFunc(CSyntaxNode* pTree)
{
	if (pTree == nullptr)
	{
		return;
	}
	switch (pTree->GetNType())
	{
	case FUNCTION_STA:
	{
		//可能是函数调用，也可能是函数指针调用，因此判断的时候，不仅要判断此函数是否存在，还得判断是否是个函数指针
		vector<string>::iterator result = find(m_FuncPNameVec.begin(), m_FuncPNameVec.end(), pTree->GetNName());

		if (isFuncExist(pTree) || result != m_FuncPNameVec.end())//如果函数存在，并且是内部调用，则提示错误
		{
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED);
			cout << "If condition statement can not be an inner function call" << endl;
			cout << pTree->TreeToString() << endl;
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY);
			g_HasErr++;
			return;
		}
		else//如果函数不存在，则当成是库函数调用
		{
			__FuncCall(pTree, 0);
			return;
		}
	}
	case EXT_FUNCTION_STA:
	{
		__EFuncCall(pTree->GetChild0(), 0);
		return;
	}
	default:
	{
		if (pTree->GetChild0() != nullptr)
		{
			__IfWhileCondFunc(pTree->GetChild0());
		}
		if (pTree->GetChild1() != nullptr)
		{
			__IfWhileCondFunc(pTree->GetChild1());
		}

	}
	}
}

//add by yubin 2015/4/22,判断函数pTree是否出现过
bool CMSV::isFuncExist(CSyntaxNode* pTree)
{
	CSyntaxNode* p = function_tree;//p指向函数定义语法树
	while (p != nullptr)//while循环用于在函数定义树查找当前调用的函数
	{
		if (p->GetNName() == pTree->GetNName())
		{
			return 1;
		}
		p = p->GetChild2();
	}
	return 0;
}

/**
* 将变量声明加入到符号表中
* @param 待加入的变量
* @return void
*/
bool CMSV::__InstDec2ST(CSyntaxNode*pTree)
{
	SymbolType type;
	if (pTree->GetNType() == UNSIGN_DECLARATION_STA)
		type = SymbolTable::GetSymbolType( GetUnsignedType(pTree->GetChild0()->GetRType()) );
	else
		type = SymbolTable::GetSymbolType(pTree->GetRType());

	string name = "";
	if (pTree->GetRType() == STRUCTTYPE)
		name = pTree->GetNName();


	if (!__SearchAndInsert(type, pTree, name))
	{
		PrintErrorMassage(0, "__InstDec2ST: declaration of variable fail : " + pTree->TreeToString());
		m_RdcOk = 0;
		return false;
	}
	return true;

}

///清空m_UnFrm和m_PreUnFrm
///2015-4-22 add by wangmeng 
void CMSV::__ClearUnFrm()
{
	CSyntaxNode* delTree = nullptr;
	while (!m_UnFrm.empty())
	{
		delTree = m_UnFrm.top();
		delete delTree;
		m_UnFrm.pop();
	}
	while (!m_PreUnFrm.empty())
	{
		delTree = m_PreUnFrm.top();
		delete delTree;
		m_PreUnFrm.pop();
	}

}

///输出全局变量
///2015-4-22 add by wangmeng
void CMSV::PrintGlbVar()
{
	CSyntaxNode* GlbVar = m_GlbVarTree->GetRoot();
	if (GlbVar != nullptr)
	{
		cout << GlbVar->TreeToString() << ";" << endl;
	}
}

///输出全局变量
///2015-4-22 add by wangmeng
void CMSV::PrintFuncDec()
{
	CSyntaxNode* FuncDec = m_FuncDecTree->GetRoot();
	if (FuncDec != nullptr)
	{
		cout << FuncDec->TreeToString() << ";" << endl;
	}

}

//add by yubin 2015/5/21,在主函数的最后，添上以下四句话
//$$firstTime: = 1; $$state_num: = 0; output("\n") and skip; $$pathNum=$$pathNum+1
void CMSV::__AddLastStatments(CSyntaxNode*pTree)
{
	if (pTree == nullptr)
	{
		return;
	}

	//add by yubin 2015/6/3,添加stmt0，目的是让转IR的时候，能够识别到此结点，然后将所有的全局变量先置为0
	CSyntaxTree* TmpTree = new CSyntaxTree();
	CSyntaxNode *stmt0 = new CSyntaxNode(GLOBAL_INITSET_STA, VOIDTYPE);
	TmpTree->SetRoot(stmt0);

	//第一句$$current = 0;
	CSyntaxNode *stmt1 = new CSyntaxNode(ASS_EQU_EXP, VOIDTYPE);
	stmt1->SetChild0(new CSyntaxNode(IDENT_EXP, "$$current", nullptr, VOIDTYPE));
	stmt1->SetChild1(new CSyntaxNode(INTEGER_EXP, 0, INTTYPE));
	//TmpTree->SetRoot(stmt1);
	TmpTree->MergeTree(stmt1, CHOP_STA, VOIDTYPE);


	//output("Path Number : ")
	CSyntaxNode* outputVar = new CSyntaxNode(OPTION_EXP, new CSyntaxNode(STR_EXP, "Path Number : ", STRTYPE), VOIDTYPE);
	CSyntaxNode *outputstmt1 = new CSyntaxNode(DISPLAY_STA, outputVar, UNKNOWNTYPE);

	//output($$pathNum)
	CSyntaxNode* outputVar_2 = new CSyntaxNode(OPTION_EXP, new CSyntaxNode(IDENT_EXP, "$$pathNum", nullptr, VOIDTYPE), VOIDTYPE);
	CSyntaxNode *outputstmt2 = new CSyntaxNode(DISPLAY_STA, outputVar_2, UNKNOWNTYPE);
	//output("\n")
	CSyntaxNode* outputVar_3 = new CSyntaxNode(OPTION_EXP, new CSyntaxNode(STR_EXP, "\n", STRTYPE), VOIDTYPE);
	CSyntaxNode *outputstmt3 = new CSyntaxNode(DISPLAY_STA, outputVar_3, UNKNOWNTYPE);

	TmpTree->MergeTree(outputstmt1, CHOP_STA, VOIDTYPE);
	TmpTree->MergeTree(outputstmt2, CHOP_STA, VOIDTYPE);
	TmpTree->MergeTree(outputstmt3, CHOP_STA, VOIDTYPE);
	CSyntaxNode *tmp1 = new CSyntaxNode();
	//add by yubin 2015/8/28,因为copyTree需要占用很多空间，所以将 tmp1->CopyTree(pTree)进行修改
	tmp1->CopyNode(pTree);
	tmp1->SetChild0(pTree->GetChild0());
	tmp1->SetChild1(pTree->GetChild1());
	tmp1->SetChild2(pTree->GetChild2());


	TmpTree->MergeTree(tmp1, CHOP_STA, VOIDTYPE);


	/*CSyntaxNode *chop4 = new CSyntaxNode(CHOP_STA, VOIDTYPE);
	chop4->SetChild0(stmt4);
	chop4->SetChild1(new CSyntaxNode());
	chop4->GetChild1()->CopyTree(pTree);*/


	CSyntaxNode *stmt2 = new CSyntaxNode(ASS_EQU_EXP, VOIDTYPE);
	stmt2->SetChild0(new CSyntaxNode(IDENT_EXP, "$$firstTime", nullptr, VOIDTYPE));
	stmt2->SetChild1(new CSyntaxNode(INTEGER_EXP, 1, INTTYPE));

	CSyntaxNode *stmt3 = new CSyntaxNode(ASS_EQU_EXP, VOIDTYPE);
	stmt3->SetChild0(new CSyntaxNode(IDENT_EXP, "$$state_num", nullptr, VOIDTYPE));
	stmt3->SetChild1(new CSyntaxNode(INTEGER_EXP, 0, INTTYPE));

	/*CSyntaxNode *stmt3 = new CSyntaxNode(AND_EXP, VOIDTYPE);
	stmt3->SetChild0(new CSyntaxNode(DISPLAY_STA, new CSyntaxNode(STR_EXP, "\n", STRTYPE), UNKNOWNTYPE));
	stmt3->SetChild1(new CSyntaxNode(SKIP_STA, VOIDTYPE));*/

	//output("\n")
	CSyntaxNode* outputVar_4 = new CSyntaxNode(OPTION_EXP, new CSyntaxNode(STR_EXP, "\n", STRTYPE), VOIDTYPE);
	CSyntaxNode *outputstmt4 = new CSyntaxNode(DISPLAY_STA, outputVar_4, UNKNOWNTYPE);

	//$$pathNum = $$pathNum + 1
	CSyntaxNode * stmt4 = new CSyntaxNode(ASS_EQU_EXP, VOIDTYPE);
	CSyntaxNode *assiStatLeft = new CSyntaxNode(IDENT_EXP, "$$pathNum", nullptr, VOIDTYPE);
	CSyntaxNode *addStat = new CSyntaxNode(ADD_EXP, ARITHMETICTYPE);
	CSyntaxNode *addStatLeft = new CSyntaxNode(IDENT_EXP, "$$pathNum", nullptr, VOIDTYPE);
	CSyntaxNode *addStatRight = new CSyntaxNode(INTEGER_EXP, 1, INTTYPE);
	addStat->SetChild0(addStatLeft);
	addStat->SetChild1(addStatRight);
	stmt4->SetChild0(assiStatLeft);
	stmt4->SetChild1(addStat);

	CSyntaxNode * stmt5 = new CSyntaxNode(CAL_PATH_TIME, VOIDTYPE);

	TmpTree->MergeTree(stmt2, CHOP_STA, VOIDTYPE);
	TmpTree->MergeTree(stmt3, CHOP_STA, VOIDTYPE);
	TmpTree->MergeTree(outputstmt4, CHOP_STA, VOIDTYPE);

	TmpTree->MergeTree(new CSyntaxNode(LABEL_STA, "FALSE", nullptr, VOIDTYPE), CHOP_STA, VOIDTYPE);
	TmpTree->MergeTree(stmt4, CHOP_STA, VOIDTYPE);
	TmpTree->MergeTree(stmt5, CHOP_STA, VOIDTYPE);

	//将整个程序的外面套上if($$flag2=0)
	CSyntaxNode *equationLeft = new CSyntaxNode(IDENT_EXP, "$$flag2", nullptr, VOIDTYPE);
	CSyntaxNode *equationRight = new CSyntaxNode(INTEGER_EXP, 0, INTTYPE);
	CSyntaxNode *equation = new CSyntaxNode(EQU_EXP, BOOLTYPE);
	equation->SetChild0(equationLeft);
	equation->SetChild1(equationRight);

	CSyntaxNode * skipStat = new CSyntaxNode(SKIP_STA, VOIDTYPE);
	CSyntaxNode *IfTree = new CSyntaxNode(IF_ELSE_STA, equation, TmpTree->GetRoot(), skipStat, VOIDTYPE);

	//pTree = IfTree;
	//直接上面写不行，必须得下面写
	pTree->SetNType(IF_ELSE_STA);
	pTree->SetChild0(IfTree->GetChild0());
	pTree->SetChild1(IfTree->GetChild1());
	pTree->SetChild2(IfTree->GetChild2());
}
//-----------------------------------------------------------------------------
///add by yubin 2015/5/25,处理or时需要用到的全局变量加入到符号表中
void CMSV::__AddOrGloVarToSymbol()
{

	///int $$flag[10]
	vector <int> ArrayNum;
	ArrayNum.push_back(100);
	m_GlbSymbl->InstSymbol("$$flag", new Symbol(SYMBOLINT, true, " ", ArrayNum, 0));

	///int $$len
	m_GlbSymbl->InstSymbol("$$length", new Symbol(SYMBOLINT));
	///int $$current
	m_GlbSymbl->InstSymbol("$$current", new Symbol(SYMBOLINT));

	///int $$pathNum
	m_GlbSymbl->InstSymbol("$$pathNum", new Symbol(SYMBOLINT));

	///int $$firstTime
	m_GlbSymbl->InstSymbol("$$firstTime", new Symbol(SYMBOLINT));

	///int $$flag1
	m_GlbSymbl->InstSymbol("$$flag1", new Symbol(SYMBOLINT));

	///int $$flag2
	m_GlbSymbl->InstSymbol("$$flag2", new Symbol(SYMBOLINT));

	///处理await时需要用到的全局变量加入到符号表中
	///int $$current
	m_GlbSymbl->InstSymbol("$$len_i", new Symbol(SYMBOLINT));
}
//---------------------------------------------------------------------------
//将一个int转为字符串
string CMSV::__Int2Str(int pN)
{
	return to_string(pN);
}
//-----------------------------------------------------------------------------
void CMSV::__DelVector(vector<Label_Relation*> pVector)
{
	vector<Label_Relation*>::iterator iter;
	for (iter = pVector.begin(); iter != pVector.end(); iter++)
	{
		delete *iter;
	}
	pVector.clear();
}
//----------------------------------------------------------------------------
void CMSV::__DelRValue(CSyntaxNode* pTree)
{
	if (pTree == nullptr)
	{
		return;
	}
	CSyntaxNode *paraList = pTree;
	//去掉RValue
	//只有一个参数
	if (paraList->GetChild0() != nullptr)
	{
		if (__isRValue(paraList))
		{
			if (paraList->GetChild1() == nullptr)
			{
				delete paraList->GetChild0();
				paraList->SetChild0(nullptr);
			}
			else
			{
				PrintErrorMassage(0, "In function " + pTree->GetNName() + " call, RValue must be the last parameter!!!");
				return;
			}
		}
	}
	//有多个参数
	while (paraList->GetChild1() != nullptr)
	{
		if (__isRValue(paraList->GetChild1()))
		{
			if (paraList->GetChild1()->GetChild1() == nullptr)
			{
				delete paraList->GetChild1();
				paraList->SetChild1(nullptr);
				break;
			}
			else
			{
				cerr << "In function " << pTree->GetNName() << " call, RValue must be the last parameter!!!" << endl;
				return;
			}
		}

		paraList = paraList->GetChild1();
	}
}
//-----------------------------------------------------------------------------
void CMSV::__Trim(CSyntaxNode* pTree)
{
	NODETYPE rootType = pTree->GetNType();
	CSyntaxTree* simplePalStmt = new CSyntaxTree();
	CSyntaxTree* complexPalStmt = new CSyntaxTree();
	stack<CSyntaxNode*>  palStack;
	palStack.push(pTree->GetChild0());
	palStack.push(pTree->GetChild1());
	pTree->SetChild0(nullptr);
	pTree->SetChild1(nullptr);
	while (!palStack.empty())
	{
		CSyntaxNode* palBranch = palStack.top();
		palStack.pop();
		if (palBranch->GetNType() == rootType)
		{
			palStack.push(palBranch->GetChild0());
			palStack.push(palBranch->GetChild1());
			palBranch->SetChild0(nullptr);
			palBranch->SetChild1(nullptr);
			delete palBranch;
			palBranch = nullptr;
		}
		else if (!palBranch->GetiValue())//简单语句
		{
			simplePalStmt->MergeTree_2(palBranch, rootType, VOIDTYPE);
		}
		else
		{
			complexPalStmt->MergeTree_2(palBranch, rootType, VOIDTYPE);
			complexPalStmt->GetRoot()->SetiValue(1);
		}
	}
	if (simplePalStmt->GetRoot() != nullptr)
	{
		pTree->SetChild0(simplePalStmt->GetRoot());
		pTree->SetChild1(complexPalStmt->GetRoot());
		simplePalStmt->SetRoot(nullptr);
		delete simplePalStmt;
		complexPalStmt->SetRoot(nullptr);
		delete complexPalStmt;
	}
	else
	{
		pTree->SetChild0(complexPalStmt->GetRoot()->GetChild0());
		pTree->SetChild1(complexPalStmt->GetRoot()->GetChild1());
		complexPalStmt->GetRoot()->SetChild0(nullptr);
		complexPalStmt->GetRoot()->SetChild1(nullptr);
		delete complexPalStmt;
	}
}

