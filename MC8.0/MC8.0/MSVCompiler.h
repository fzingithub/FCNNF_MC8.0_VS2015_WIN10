#ifndef MSVCOMPILER_H_INCLUDED
#define MSVCOMPILER_H_INCLUDED

#define MYPAUSETHRED "MyPauseThread"
#define MYWAITFOROBJECT "MyWaitForObject"
#define MYPUSH "$$Push"
#define SETNODELENGTH "setNodeLength"

#include <stack>
#include <vector>
#include "SyntaxNode.h"
#include "SyntaxTree.h"
#include "SymbolTable.h"
#include "IR.h"//add by yubin 2015-4-3，包含IR类的头文件

extern set<string> g_ThreadFuncName;
/**2015-3-9 add by shiyifang
*
*赋值语句结构体，用于排序
*seq：在present中的序号
*left: 赋值左边的标识符,多余1个会处理
*right：应属于右边的标识符
*/
typedef struct {
	int seq;
	vector<string> left;
	vector<string> right;
} Ass_Struct;
typedef struct{
	CSyntaxNode* node;
	int priority;
} Node_Priority;

/**2015-3-9 add by yangkai
*用于存储多个while并行的程序片段
*label: 给该程序片段添加的label名
*prgm: 程序片段
*/
typedef struct{
	string label;
	string prgm;
}Loop_Block;


/*
2015-6-23 add by wangmeng
* 用于记录标签之间的跳转关系
* left: 由left标签标记的程序开始进行跳转
* right： 跳转到由right标签标记的程序
*/
typedef struct{
	string  from;
	string to;
}Label_Relation;



typedef struct{
	string label;
	string jmp;
	string start_jmp;

	string str_state; ///该字符串即为用来判断是不是第一个状态的变量的变量名。命名是$$state后边跟一数字。在投影中进行命名。

	string isCondVarName;//排序时用到
}LabelName;

typedef struct{
	bool output;
	bool trans_if;
	bool addlable;
}Flag;



/**
*Compiler class
*/
class CMSV
{
public:
	///constructor
	CMSV();
	///destructor
	virtual  ~CMSV();

	/**
	* Get present tree of pTree, including the assignment statement at present state
	* @param the syntax tree to be handled
	* @return void
	*/
	void GetPresent(CSyntaxNode* pTree);

	///Preprocess the tree
	void Pre();

	///输出当前状态赋值语句
	void PrintPrsnt();

	///输出m_IRTree
	string PrintIRTree();

	///输出全局变量
	void PrintGlbVar();

	///输出全局变量
	void PrintFuncDec();

	void Simulate();

	void Model();

	void Verify();

	//2015-4-13,add by yubin,将m_Program设置成pTree,默认的参数，flag=0表示将m_IRTree置为空，flag=1表示不修改m_IRTree
	void SetProgram(CSyntaxNode* pTree, int flag = 0);

	//2015-4-13,add by yubin,设置IR树
	inline void SetIRTree(CSyntaxNode* tree)
	{
		if (m_IRTree == NULL)
		{
			m_IRTree = new CSyntaxTree();
		}
		m_IRTree->SetRoot(tree);
	}

	inline CSyntaxTree* GetIRTree()
	{
		return m_IRTree;
	}

	inline CSyntaxTree* GetGlbVar()
	{
		return m_GlbVarTree;
	}

	inline void SetPreFlag(int flag)
	{
		m_PreFlag = flag;
	}

	inline CSyntaxNode* GetNewFuncTree()
	{
		return m_NewFuncTree;
	}
	
	inline void SetNewFuncTree(CSyntaxNode* pTree)
	{
		m_NewFuncTree = pTree;
	}
	
	inline CSyntaxNode* GetThreadFuncTree()
	{
		return m_ThreadFuncTree;
	}

	//add by yubin 2015/5/21,处理or时添加的函数
	void __OrPost(CSyntaxNode*pTree);

	//add by yubin 2015/5/21,在主函数的最后，添上以下三句话
	//$$firstTime: = 1; $$state_num: = 0; output("\n") and skip;
	void __AddLastStatments(CSyntaxNode*pTree);

	///add by yubin 2015/5/25,处理or时需要用到的全局变量加入到符号表中
	void __AddOrGloVarToSymbol();

private:
	/**
	* Get present tree of declaration statement: int x,y<==x
	* @param the syntax tree to be handled
	* @return void
	*/
	void __Declr(CSyntaxNode* pTree);
	/**
	* Get present tree of assignment statement: x<==e
	* @param the syntax tree to be handled
	* @return void
	*/
	void __Ass(CSyntaxNode* pTree);

	/**
	* Get present tree of unit assignment statement: x:=e
	* @param the syntax tree to be handled
	* @return void
	*/
	void __UAss(CSyntaxNode* pTree);

	/**
	* Get present tree of empty statement: empty
	* @param the syntax tree to be handled
	* @return void
	*/
	void __Emp(CSyntaxNode* pTree);

	/**
	* Get present tree of more statement: more
	* @param the syntax tree to be handled
	* @return void
	*/
	void __More(CSyntaxNode* pTree);

	/**
	* Get present tree of true statement: true
	* @param the syntax tree to be handled
	* @return void
	*/
	void __True(CSyntaxNode* pTree);

	/**
	* Get present tree of skip statement: skip
	* @param the syntax tree to be handled
	* @return void
	*/
	void __Skip(CSyntaxNode* pTree);

	/**
	* Get present tree of length statement: len(n);
	* @param the syntax tree to be handled
	* @return void
	*/
	void __Len(CSyntaxNode* pTree);

	/**
	* Get present tree of next statement: next p
	* @param the syntax tree to be handled
	* @return void
	*/
	void __Next(CSyntaxNode* pTree);

	/**
	* Get present tree of and statement: p and q
	* @param the syntax tree to be handled
	* @return void
	*/
	void __And(CSyntaxNode* pTree);

	/**
	* Get present tree of parallel statement: p||q
	* @param the syntax tree to be handled
	* @return void
	*/
	void __Pal(CSyntaxNode* pTree);

	/**
	* Get present tree of projection statement: {p1,p2,...}prj q
	* @param the syntax tree to be handled
	* @return void
	*/
	void __Prj(CSyntaxNode* pTree);

	/**
	* Get present tree of chop statement: p;q
	* @param the syntax tree to be handled
	* @return void
	*/
	void __Chop(CSyntaxNode* pTree);

	/**
	* Get present tree of if statement: if(b)then{p}else{q}
	* @param the syntax tree to be handled
	* @return void
	*/
	void __If(CSyntaxNode* pTree);

	/**
	* Get present tree of while statement: while(b){p}
	* @param the syntax tree to be handled
	* @return void
	*/
	void __While(CSyntaxNode* pTree);

	/**
	* Get present tree of for statement: for(p1,b,p2){q}
	* @param the syntax tree to be handled
	* @return void
	*/
	void __For(CSyntaxNode* pTree);

	/**
	* Get present tree of for statement: for n times do {p}
	* @param the syntax tree to be handled
	* @return void
	*/
	void __ExFor(CSyntaxNode* pTree);

	/**
	* Get present tree of switch statement: switch(e){case n1: (p1)...}
	* @param the syntax tree to be handled
	* @return void
	*/
	//void __Switch(CSyntaxNode* pTree);

	/**
	* Get present tree of frame statement: frame(c)
	* @param the syntax tree to be handled
	* @return void
	*/
	void __Frm(CSyntaxNode* pTree);

	/**
	* Get present tree of unframe statement: unframe(c)
	* @param the syntax tree to be handled
	* @return void
	*/
	void __UnFrm(CSyntaxNode* pTree);

	/**
	* Get present tree of or statement: p or q
	* @param the syntax tree to be handled
	* @return void
	*/
	void __Or(CSyntaxNode*pTree);

	/**
	* Get present tree of exist statement: exists(x,y,...){P}
	* @param the syntax tree to be handled
	* @return void
	*/
	void __Ext(CSyntaxNode* pTree);

	/**
	* check whether the parameter is RValue
	* @param the syntax tree to be handled
	* @return bool
	*/
	bool __isRValue(CSyntaxNode* pTree);

	/**
	* Struct definition
	* @param the syntax tree to be handled
	* @return void
	*/
	void __StructDef(CSyntaxNode* pTree);

	/**
	* 处理函数定义的参数部分
	* @param the syntax tree to be handled
	* @return void
	*/
	void __FuncDefPara(CSyntaxNode* pTree);

	/**
	* 处理函数定义的函数指针参数部分
	* @param the syntax tree to be handled
	* @return void
	*/
	void __FuncDefParaFuncP(CSyntaxNode* pTree);


	/**
	* Get present tree of function definition statement: fuction fname(e1,e2,...){P}
	* @param the syntax tree to be handled
	* @return void
	*/
	void __FuncDef(CSyntaxNode* pTree);

	/**
	* Get present tree of function call statement: fname(e1,e2,...)
	* @param the syntax tree to be handled
	* @return void
	*/
	void __FuncCall(CSyntaxNode* pTree, int flag = 1);

	/**
	* Get present tree of extern function call statement: extern fname(e1,e2,...)
	* @param the syntax tree to be handled
	* @return void
	*/
	void __EFuncCall(CSyntaxNode* pTree, int flag = 1);

	/**
	* Get present tree of await statement: await(b)
	* @param the syntax tree to be handled
	* @return void
	*/
	void __Await(CSyntaxNode*pTree);

	/**
	* Get present tree of final statement: fin(p)
	* @param the syntax tree to be handled
	* @return void
	*/
	void __Fin(CSyntaxNode* pTree);


	/**
	* Dealing with always statement
	* @param the syntax tree to be handled
	* @return void
	*/
	void __Alw(CSyntaxNode*pTree);


	/**
	* Get present tree of keep statement: keep(p)
	* @param the syntax tree to be handled
	* @return void
	*/
	void __Keep(CSyntaxNode*pTree);


	/**
	* Dealing with input statement
	* @param the syntax tree to be handled
	* @return void
	*/

	void __Input(CSyntaxNode* pTree);

	/**
	* Dealing with output statement
	* @param the syntax tree to be handled
	* @return void
	*/
	void __Output(CSyntaxNode* pTree);

	/**
	* Output a variable
	* @param the syntax tree to be handled
	* @return void
	*/
	//string __OutputVar(CSyntaxNode* pTree);

	/**
	* Output a string
	* @param the syntax tree to be handled
	* @return void
	*/
	string __OutputStr(CSyntaxNode* pTree);

	/**
	* Output the member of an array
	* @param the syntax tree to be handled
	* @return void
	*/
	//string __OutputArrayMember(CSyntaxNode* pTree);

	/**
	* Output the member of an array
	* @param the syntax tree to be handled
	* @return void
	*/
	//string __OutputArrayCast(CSyntaxNode* pTree);

	/**
	* Output the type cast statement
	* @param the syntax tree to be handled
	* @return void
	*/
	//string __OutputTypeCast(CSyntaxNode* pTree);

	/**
	* Merge the return tree to M_IRTree directly: return x
	* @param the syntax tree to be handled
	* @return void
	*/
	void __Return(CSyntaxNode* pTree);//2015-4-13,add by yubin,处理函数返回

	/**
	* Insert the present assignment to the present vector.
	* @param the syntax tree to be handled
	* @return void
	*/
	void __InsrtAss(CSyntaxNode*pTree);

	/*将当前状态赋值的链表加入到IRTree中
	* param1 当前状态的最后一个语句之后是否需要加MyPauseThread()
	* return void
	*/
	void __AdPrst2IR();

	///当得到区间长度后处理对应的unframe语句
	void __AddUnFrm();

	///统一处理keep，fin，alw
	void __KpFnAl();

	///释放frame变量
	void __FreeSymbl();

	/**
	* 获得节点后代中的所有标识符
	* @param pTree要遍历的节点，right存放获得的标识符
	*/
	///2015-03-10
	void __GetRightId(CSyntaxNode*pTree, vector<string> &right);

	/*节点类型为赋值时 获得赋值的左值和右值
	* @param pTree(赋值节点)  stru(存放标识符的结构体)
	*/
	///2015-03-12
	void __GetAssStruct(CSyntaxNode*pTree, Ass_Struct &stru);

	/// 杨凯 2015-07-1处理并行时用到
	void __GetAsssStruct(CSyntaxNode*pTree, Asss_Struct &stru);

	/*声明变量时 插入所有变量
	* type(声明类型)  pTree(声明节点) name (结构体类型名，可为空)
	*/
	///2015-03-13

	bool __SearchAndInsert(SymbolType type, CSyntaxNode *pTree, string name = "", bool IsPar = false, bool IsGlb = false, int AddrNum = 0);

	/*
	* 为获得的所有当前状态排序,声明并检查变量
	* 排序顺序: EX_ASS 声明 ASS_EXP
	* 排序规则：使用快排，比较左值是否包含右值
	*/
	///2015-03-13

	void __SortPrsnt();

	/**
	* 该函数用于对if语句和while语句的预处理
	* @param
	* @return void
	*/
	void __PreIfWhile();

	/**
	* 该函数用于将结构体定义插入到符号表中
	* @param pTree（结构体定义）
	* @return void
	*/
	void __InitStructDel(CSyntaxNode *pTree);


	///生成状态输出语句
	void __OutputState();


	/**
	* 处理赋值中有函数调用的情况,如果有函数掉用，处理后，将m_HasFunCal置为true
	* @param 赋值号右边的语法树
	* @param 是不是当前状态赋值
	* @return void
	*/
	void __AssFunc(CSyntaxNode* RAssTree, bool IsPAss = false);


	/**
	* 处理if条件中的函数调用,目前if条件中的函数调用必须是外部函数调用，如果是内部函数调用，会提示错误
	* @param if条件的语法树
	* @return void
	*/
	void __IfWhileCondFunc(CSyntaxNode* pTree);


	/**
	* 将变量声明加入到符号表中
	* @param 待加入的变量
	* @return 是否正确加入
	*/
	bool __InstDec2ST(CSyntaxNode*pTree);

	///清空m_UnFrm和m_PreUnFrm
	void __ClearUnFrm();
	/**
	* 判断此函数名是否是用户自定义函数
	* @param 函数调用语法树
	* @return bool
	*/
	bool isFuncExist(CSyntaxNode* pTree);


	//将一个int转为字符串
	string __Int2Str(int pN);


	/* 释放pVector占用的内存
	* @param1 要处理的vector
	* @return void
	*/
	void __DelVector(vector<Label_Relation*> pVector);


	/*yangkai 用于在构造strcmp函数
	*param1
	*param2
	*return CSyntaxNode*
	*
	*/
	CSyntaxNode* __GenStrCmp(string s1, string s2);

	/*yangkai 处理函数指针声明
	*param1 待处理语法树
	*return void
	*/
	void __FuncPtrDeclr(CSyntaxNode* pTree);

	/*yangkai 删除参数中的RValue
	*param1 待处理语法树
	*param2 Extern标记0或1
	*return void
	*/
	void __DelRValue(CSyntaxNode* pTree);

	/*yangkai 将结构体取成员表达式中的右值, 如a.b[i]中的a和i，存入vector中
	*param1 待处理语法树
	*param2 要存入的vector
	*return void
	*/
	void __GetRightInStructMember(CSyntaxNode* pTree, vector<string> &right);

	/*yangkai 对and和pal的语法树进行整理，将所有简单语句整理到一起
	*param1 待处理语法树
	*return void
	*/
	void __Trim(CSyntaxNode* pTree);

	/*yangkai 处理复杂的并行语句
	*param1 待处理语法树
	*return void
	*/
	void __ComplexPal(CSyntaxNode* pTree);

	/*yangkai 处理复杂的And语句
	*param1 待处理语法树
	*return void
	*/
	void __ComplexAnd(CSyntaxNode* pTree);

	/*yangkai 处理复杂的prj语句
	*param1 待处理语法树
	*return void
	*/
	void __ComplexPrj(CSyntaxNode* pTree);

	/*yangkai 构建结构体
	*param1 局部变量
	*param2 函数形参
	*return 新构造的结构体的语法树
	*/
	CSyntaxNode* __DefStruct(CSyntaxNode* pLocalVar, CSyntaxNode* pFuncFormalPar);

	/*yangkai 为结构体分配空间，并初始化其成员
	*param1 结构体语法树
	*return void
	*/
	void __InitStruct(CSyntaxNode* pTree);

	/*yangkai 根据语法树得到函数的形参列表
	*param1 待处理语法树
	*return 函数形参列表
	*/
	CSyntaxNode* __GetFuncFormalPar(CSyntaxNode* pTree);

	/*yangkai 该函数用于在线程函数中调用新函数时生成实参，前n-1个实参需要变为($$Si*)p->x的形式，
	其中$$Si是最新生成的结构体的名字；第n个参数需要变为($$Si*)p的形式，其中p是线程函数的参数名。
	如果当前处于新函数中，则$$Si要从pFormalPar的最后一个参数中获得，此时pFormalPar最后一个参数
	是一个结构类型，该结构体就是我们需要的结构体类型
	*param1 形参列表
	*return 函数实参列表
	*/
	CSyntaxNode* __GetFuncActualPar(CSyntaxNode* pFormalPar);

	/*yangkai 函数名，函数形参，函数体构造新的函数
	*param1 函数名
	*param2 函数形参
	*param3 函数体
	*return void
	*/
	void __DefFunc(const string pFuncName, CSyntaxNode* pFormalPar, CSyntaxNode* pFuncBody);

	/*yangkai 构造线程函数
	*param1 线程函数名
    *param2 线程函数调用的函数名
	*param2 线程函数所调用函数的实参
	*return void
	*/
	void __DefThreadFunc(string pThreadFuncName, string pFuncName, CSyntaxNode* funcActualPar);

	/*yangkai 给always语句构造线程函数，函数体框架是while(true){MyCreateThread(); MyPauseThread();}; return 0
	*param1 当前线程函数名
	*param2 要创建的线程调用的函数名
	*param2 要创建的线程调用的函数的实参
	*return void
	*/
	void __DefThreadFuncForAlw(string pThreadFuncName, string pFuncName, CSyntaxNode* funcActualPar);

	/*yangkai 给keep语句构造线程函数，函数体框架是while(true){MyCreateThread(); MyPauseThread();}; return 0
	*param1 当前线程函数名
	*param2 要创建的线程调用的函数名
	*param2 要创建的线程调用的函数的实参
	*return void
	*/
	void __DefThreadFuncForKeep(string pThreadFuncName, string pFuncName, CSyntaxNode* funcActualPar);

	/*yangkai 创建线程
	*param1 线程函数名
	*param2 线程函数参数
	*param3 线程父节点类型
	*return 返回创建线程的语句
	*/
	CSyntaxNode* __MyCreateThread(const string pFuncName, CSyntaxNode* pFuncActualPar, int pNodeType);

	/*yangkai 构造push语句
	*param1 执行语句
	*param2 语句优先级
	*return push语句
	*/
	CSyntaxNode* __ConstructPushStmt(CSyntaxNode* pStmt, int pPriority, bool pIsCond);

	/*yangkai 给pTree中的局部变量前加一重指针
	*param1 要操作的语法树
	*return void
	*/
	void __AddPointer(CSyntaxNode* pTree);

	/*yangkai 调用线程暂停函数
	*return 生成的函数调用语法树
	*/
	CSyntaxNode* __CallPauseThread();

	/*yangkai 处理在新函数中的函数调用，对调用的函数重命名并增加实参$$par;
	将原函数定义重命名后拷贝一份到新函数树上并增加$$Si类型的形参$$par;给每个形参前加*
	*要处理的函数调用语法树
	*return void
	*/
	void CMSV::__FuncCalInNewFunc(CSyntaxNode* pTree);

	/*yangkai 在并行语句处理中，在最后一个状态设置当前分支的empty或more标志
	 区间长度为More时默认为0，不用设置，区间长度为empty时设置为1，区间长度没有
	 规定时设置为2
	*return 生成的函数调用语法树
	*/
	CSyntaxNode* __SetNodeLength(int pFlag);

	/*yangkai 该函数的作用是为并行分支的最后一个if语句设置empty或more标志
	*param1 待处理的语法树
	*return bool
	*/
	bool __SetNodeLengthOfIfStat(CSyntaxNode* pTree);

	/*yangkai 该函数的作用是在新函数的最后一个while语句之后添加一个条件判断，条件成立则
	调用SETNODELENGTH(1)，有多个while嵌套的情况时只给最里层的while语句加条件判断
	*param1 待处理的语法树
	*param2 要添加的条件
	*return bool
	*/
	//bool __AddCondAtEndOfWhile(CSyntaxNode* pTree, CSyntaxNode* pCond);

	/*yangkai 处理非函数中的投影语句
	*param1 待处理的语法树
	*return void
	*/
	void __PrjNotInFunc(CSyntaxNode* pTree);

	/*yangkai 处理函数中的投影语句
	*param1 待处理的语法树
	*return void
	*/
	void __PrjInFunc(CSyntaxNode* pTree);

	/*yangkai 处理非函数中的并行语句
	*param1 待处理的语法树
	*return void
	*/
	void __PalNotInFunc(CSyntaxNode* pTree);
	
	/*yangkai 处理函数中的并行语句
	*param1 待处理的语法树
	*return void
	*/
	void __PalInFunc(CSyntaxNode* pTree);

	/*yangkai 处理非函数中的and语句
	*param1 待处理的语法树
	*return void
	*/
	void __AndNotInFunc(CSyntaxNode* pTree);

	/*yangkai 处理函数中的and语句
	*param1 待处理的语法树
	*return void
	*/
	void __AndInFunc(CSyntaxNode* pTree);

	/*yangkai 处理非函数中的always语句
	*param1 待处理的语法树
	*return void
	*/
	void __AlwNotInFunc(CSyntaxNode* pTree);

	/*yangkai 处理函数中的always语句
	*param1 待处理的语法树
	*return void
	*/
	void __AlwInFunc(CSyntaxNode* pTree);

	/*yangkai 处理非函数中的keep语句
	*param1 待处理的语法树
	*return void
	*/
	void __KeepNotInFunc(CSyntaxNode* pTree);

	/*yangkai 处理函数中的keep语句
	*param1 待处理的语法树
	*return void
	*/
	void __KeepInFunc(CSyntaxNode* pTree);

	/*yangkai 调用MyWaitForObject()
	*param1 待处理的语法树
	*return void
	*/
	void __MyWaitForObject();

	/*yangkai 调用free()
	*param1 待处理的语法树
	*return void
	*/
	void __MyFree(const string pVarName);

	/*yangkai 判断pTree是否是复杂语句(if, while, 
	复杂并行，复杂and，alw, keep, projection)
	*param1 语法树
	*return bool
	*/
	bool __IsComplexStmt(CSyntaxNode* pTree);

	/*yangkai 遇到复杂语句时删除之前最后一个状态的输出语句
	*return void
	*/
	void __DelLastStateOutput();

	/*yangkai 处理复杂并行，And，Prj，Alw和Keep中的输出，原理是根据
	当前状态的赋值语句输出赋值号左边的变量
	*param1 当前状态语句
	*return CSyntaxNode*
	*/
	CSyntaxNode* __OutputStateInNewFunc(CSyntaxNode* pPresent);

	/*yangkai 在处理复杂并行语句时得到要输出的变量名
	*param1 当前状态语句
	*return string
	*/
	string __GetOutputVarName(CSyntaxNode* pPresent);

	/*yangkai 判断一个变量是不是局部变量
	*param1 变量名
	*return bool
	*/
	bool __IsLocalVar(const string varName);

	/*yangkai 在模型图中添加一条边(建模时用)
	*param2 是否是最后一个状态
	*return void
	*/
	void __AddEdge(int pIsEmpty);

	/*yangkai 遇到复杂语句时删除之前最后一个生成边的语句
	*return void
	*/
	void __DelLastAddEdge();

private:
	/*
	  int type: 0 - warning    1 - error
	  string msg: massage
	*/
	void PrintErrorMassage(int type, string msg){
		if (type == 0){
			cout << "warning: " << msg << endl;
		}
		else if( type == 1){
			cout << "error: " << msg << endl;
		}
		else{
			cout << endl;
		}
	}

private:
	
	CSyntaxTree *m_Program = nullptr; // the syntax tree of program
	CSyntaxTree *m_PrgmFuture = nullptr; // the syntax tree of next state program
	vector <Node_Priority> m_PrgmPrsnt; // present 链表

	stack <CSyntaxNode*> m_UnFrm; //存储该状态需要处理unframe语句
	stack <CSyntaxNode*> m_PreUnFrm; //存储该下一状态需要处理unframe语句

	CSyntaxTree *m_IRTree = nullptr;

	CSyntaxTree *m_GlbVarTree = nullptr; //全局变量树
	CSyntaxTree *m_LocVarTree = nullptr; //局部变量树
	CSyntaxTree *m_FuncDecTree = nullptr;
	
	SymbolTable *m_FrmSymbl = nullptr; //frame符号表
	SymbolTable *m_NFrmSymbl = nullptr; //没有进行frame的变量的符号表
	SymbolTable *m_GlbSymbl = nullptr; //全局符号表
	map<string, SymbolTable*> struct_map; //存储已经定义过的结构体
	
	int m_Done;        // current interval terminates when m_Done=1
	int m_More;        // initial value:-1; exist next state when m_More=1; the final state when m_More=0
	int m_RdcOk;       // initial value:-1; program get present tree successfully when m_RdcOk=1; failed when m_RdOk=0;
	int m_FreeFrm;     // m_FreeFrm值为0则不删除frame变量，m_FreeFrm值为1则准备在下一状态删除。
	int m_InAndPalPrj; // 记录当前处理的语句是否在and或pal语句中
	int m_VisitedOr;   // 表示之前是否遇见过or分支
	
	map <string, int> m_existVarTimes; // 定义一个map，里面存储每个变量之前出现的次数
	string m_currFunc; // 用于处理存在量词时，表明当前处于哪个函数中，如果没在任何一个函数，此变量为空字符串
	// 用于调整内部函数调用时，状态的输出，等于1时，表示函数调用的最后一个状态，不输出全局变量
	// 等于2时，表示函数调用完之后再输出一个状态，此时不输出状态数
	int m_FuncFlag;
	int m_PreFlag;
	bool m_HasFunCal;
	// 每次处理函数定义时，将使用的函数指针存起来
	vector <string> m_FuncPNameVec;
	bool m_HasIfWhile;
	int m_LenNum;
	int m_ForNum; // 记录for times do语句的个数
	int m_NewStructNum;
	int m_NewFuncNum;
	CSyntaxNode *m_NewFuncTree = nullptr;
	CSyntaxNode *m_ThreadFuncTree = nullptr;
	set<string> m_LocalVarName;
	CSyntaxNode *m_FuncFormalPar = nullptr;
	bool m_IsComplexStmt; // 当前状态是否是复杂语句（if语句或while语句）
	string m_CurrentFunc; // 当前正在处理的函数名
	bool m_InWhile;       // 建模时用到
	int m_StatementNum;   // 给每个语句的位置编号，用于建模时找相同节点
	int m_InFunc;
};

#endif
