#include "Setting.h"
#include "function.h"
#include "SyntaxNode.h"
#include "SyntaxTree.h"
#include "MSVCompiler.h"
#include "content.h"

#include <iostream>
#include <windows.h>

using namespace std;

extern FILE *yyin;
extern FILE *yyout;
extern int yyparse();

extern int lineno;
extern int g_nErrors;
extern int g_HasErr;

extern vector<string> includefile;
extern CSyntaxNode    *g_syntaxTree;
extern CSyntaxNode    *function_tree;
extern CSyntaxNode    *struct_tree;
extern CSyntaxNode    *extern_tree;
extern CSyntaxNode    *g_CopyFuncTree;
extern CMAP           g_StructNameMap;

void __IncludeFile();
void __AsyncCom();

bool g_InFunc = 0;
bool g_InNewFunc = 0;
bool g_InStruct = 0;

int main(int argc, char* argv[])
{
	DWORD start_time = GetTickCount();
#ifdef RELEASE_MC
	Setting::setInfo(argc, argv);
#endif

	// 在读入文件之前，先修改InPath文件，将与or相关的内容加入到文本中
	if (Setting::IsSimulation()){
		//yyin = fopen((Setting::getHome() + "needAdd.m").data(), "r");
		yyin = getMFile(1);
	}
	else if (Setting::IsModeling()){
		//yyin = fopen((Setting::getHome() + "needAddforModeling.m").data(), "r");
		yyin = getMFile(2);
	}

	if (yyin == nullptr){
		cerr << "Error: The file needAdd.m does not exist" << endl;
		PAUSE;
		return -1;
	}
	yyparse(); // Call the parser  调用词法，语法分析器，分别生成程序语法树（由g_syntaxTree指向它）和性质语法树（由g_propositionTree指向它）
	if (g_nErrors){ //如果有错误,即错误数不为0
		errorSummary();
		PAUSE;
		return -1;
	}

	CMSV *Cmplr = new CMSV();
	g_syntaxTree->replaceOrName();
	function_tree->replaceOrName();

	CSyntaxNode *tmpGloVarTree1 = g_syntaxTree->GetChild0(); // 将chop左边的添加的全局变量的定义拷贝到tmpGloVarTree上
	CSyntaxNode *tmpGloVarTree2 = g_syntaxTree->GetChild1()->GetChild1(); // 将右边的while循环拷贝到tmpGloVarTree2

	g_syntaxTree->SetChild0(nullptr);
	g_syntaxTree->GetChild1()->SetChild1(nullptr);
	delete g_syntaxTree;
	g_syntaxTree = nullptr;

	Cmplr->SetProgram(tmpGloVarTree2);
	Cmplr->__AddOrGloVarToSymbol(); // 处理or时需要用到的全局变量加入到符号表中
	Cmplr->SetPreFlag(1);           // 不输出
	Cmplr->Pre();                   // 这样就可以将tmpGloVarTree2的内容存到IRTree上

	CSyntaxNode *IRtree1 = new CSyntaxNode();
	IRtree1->CopyTree(Cmplr->GetIRTree()->GetRoot());

	CSyntaxNode *tmp_function_tree = new CSyntaxNode();
	tmp_function_tree->CopyTree(function_tree);
	delete function_tree;
	function_tree = nullptr;

	yyin = fopen(Setting::getInPath().data(), "r");
	if (yyin == nullptr){
		cerr << "ERROR: " << Setting::getInPath() << ": No such file" << endl;
		PAUSE;
		return -1;
	}

	//每次语法分析时都要将classarray清空，否则越来越大
	//classarray.RemoveAll();
	lineno = 1;
	g_StructNameMap.insert("Chn"); //防止不识别异步通信的结构体
	yyparse(); // Call the parser  调用词法，语法分析器，分别生成程序语法树（由g_syntaxTree指向它）和性质语法树（由g_propositionTree指向它）

	if (g_nErrors){
		errorSummary();
		PAUSE;
		return -1;
	}
	g_StructNameMap.clear();

	if (!Setting::isFuncFile())
		__AsyncCom();

	__IncludeFile();

	if (function_tree != nullptr){
		CSyntaxNode *tmp = function_tree;
		while (tmp->GetChild2() != nullptr){
			tmp = tmp->GetChild2();
		}
		tmp->SetChild2(tmp_function_tree);
	}
	else{
		function_tree = tmp_function_tree;
	}
	tmp_function_tree = nullptr;

	//add by yubin 2015/5/21,在主函数的最后，添上以下三句话
	//$$firstTime: = 1; $$state_num: = 0; output("\n") and skip;
	Cmplr->SetProgram(g_syntaxTree);//将正式的msvl程序转成IR树,参数为1，表示不修改IRTree
	Cmplr->SetPreFlag(0);//输出
	//先对frame中的程序进行预处理
	if (Setting::IsSimulation()){
		Cmplr->Simulate();
	}
	else if (Setting::IsModeling()){
		Cmplr->Model();
	}

	CSyntaxNode *IRtree2 = Cmplr->GetIRTree()->GetRoot();
	Cmplr->__AddLastStatments(IRtree2);

	//2015-5-21,add by yubin,后期处理和or相关的操作，主要是调整IRTree
	//调试的其他程序的时候，可以先把下面一句话注释掉
	CSyntaxNode *tmp_IRTree = new CSyntaxNode(CHOP_STA, VOIDTYPE);
	tmp_IRTree->SetChild0(IRtree1);
	tmp_IRTree->SetChild1(IRtree2);
	Cmplr->GetIRTree()->SetRoot(tmp_IRTree);
	Cmplr->__OrPost(tmpGloVarTree1);

	//将函数语法树拷贝一份
	g_CopyFuncTree = new CSyntaxNode();
	g_CopyFuncTree->CopyTree(function_tree);
	//add by yubin 2015-4-13,预处理函数语法树
	CSyntaxTree* IRtree = Cmplr->GetIRTree();
	Cmplr->SetPreFlag(0);//输出

	if (function_tree != nullptr){
		g_InFunc = true;
		Cmplr->SetProgram(function_tree);//转正式的msvl程序之前，先转函数定义的树，转好之后的IR结构，仍然保存在function_tree上
		Cmplr->Pre();
		Cmplr->SetIRTree(function_tree);
		g_InFunc = false;
	}

	CSyntaxNode *newFuncTree = nullptr;
	while (Cmplr->GetNewFuncTree()){
		CSyntaxNode* tempTree = Cmplr->GetNewFuncTree();
		Cmplr->SetNewFuncTree(nullptr);
		g_InNewFunc = 1;
		g_InFunc = 1;
		Cmplr->SetProgram(tempTree);
		Cmplr->Pre();
		//Cmplr->SetIRTree(tempTree);
		if (newFuncTree != nullptr){
			CSyntaxNode* visit = newFuncTree;
			while (visit->GetChild2() != nullptr){
				visit = visit->GetChild2();
			}
			visit->SetChild2(tempTree);
		}
		else{
			newFuncTree = tempTree;
		}
		g_InNewFunc = 0;
		g_InFunc = 0;
	}

	//将m_ThreadFuncTree加到m_NewFuncTree上
	if (newFuncTree != nullptr){
		CSyntaxNode* visit = newFuncTree;
		while (visit->GetChild2() != nullptr){
			visit = visit->GetChild2();
		}
		visit->SetChild2(Cmplr->GetThreadFuncTree());
	}

	//将function_tree和m_NewFuncTree合起来
	if (function_tree != nullptr){
		CSyntaxNode* visit = function_tree;
		while (visit->GetChild2() != nullptr){
			visit = visit->GetChild2();
		}
		visit->SetChild2(newFuncTree);
	}

	delete g_CopyFuncTree; //删除函数树的拷贝

	IR *m_IR = new IR();
	m_IR->Trslt2IR(Cmplr->GetGlbVar(), function_tree, IRtree);

	DWORD end_time = GetTickCount();
	cout << "The run time is: " << (end_time - start_time) << "ms" << endl;
	PAUSE;
	return 0;
}

//处理include文件
void __IncludeFile(){
	CSyntaxNode *temp1 = g_syntaxTree;
	CSyntaxNode *temp2 = function_tree;
	CSyntaxNode *temp3 = struct_tree;
	g_syntaxTree = function_tree = struct_tree = nullptr;

	set<string> parsed;//已经分析过的文件
	for (size_t i = 0; i < includefile.size(); ++i){
		if (parsed.count(includefile[i]) != 0)
			continue;
		parsed.insert(includefile[i]);
		yyin = fopen((includefile[i]).data(), "r");
		if (yyin == nullptr){
			cerr << "ERROR: The file " << includefile[i] << " does not exist." << endl;
			PAUSE;
			exit(-1);
		}
		yyparse();
		if (g_nErrors){
			cerr << "In file " << includefile[i] << endl;
			errorSummary();
			PAUSE;
			exit(-1);
		}
		
		delete g_syntaxTree;
		delete function_tree;
		delete struct_tree;
	}
	g_syntaxTree = temp1;
	function_tree = temp2;
	struct_tree = temp3;
}

//异步通信分析其对应的文件
void __AsyncCom(){
	CSyntaxNode *temp1 = g_syntaxTree;
	CSyntaxNode *temp2 = function_tree;
	CSyntaxNode *temp3 = struct_tree;
	g_syntaxTree = function_tree = struct_tree = nullptr;

	//yyin = fopen((Setting::getHome() + "async_com.m").data(), "r");
	yyin = getMFile(3);
	if (yyin == nullptr){
		cerr << "The file async_com.m does not exist." << endl;
		PAUSE;
		exit(-1);
	}
	lineno = 1;
	yyparse();
	if (g_nErrors){
		cerr << "In file async_com.m " << endl;
		errorSummary();
		PAUSE;
		exit(-1);
	}

	CSyntaxNode *visit = function_tree;
	while (visit->GetChild2() != nullptr){
		visit = visit->GetChild2();
	}
	visit->SetChild2(temp2);

	visit = struct_tree;
	while (visit->GetChild1() != nullptr){
		visit = visit->GetChild1();
	}
	visit->SetChild1(temp3);
	
	g_syntaxTree = temp1;
}