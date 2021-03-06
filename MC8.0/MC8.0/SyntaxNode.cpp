#include "SyntaxNode.h"
#include "function.h"

#include <sstream>
#include <stdlib.h>
#include <iostream>

using namespace std;

extern bool g_InStruct;
extern void ReplaceESC(string& s);
extern string ReturnESC(char c);

CSyntaxNode::CSyntaxNode() 
	:m_Value(), m_NodeName(""), m_NodeType(TRUE_EXP), m_ReturnType(VOIDTYPE), m_PreLen()
{
	m_child[0] = nullptr;
	m_child[1] = nullptr;
	m_child[2] = nullptr;
}

CSyntaxNode::CSyntaxNode(NODETYPE pnodetype, RETURNTYPE preturntype) 
	:m_Value(), m_NodeName(""), m_NodeType(pnodetype), m_ReturnType(preturntype), m_PreLen()
{
	m_child[0] = nullptr;
	m_child[1] = nullptr;
	m_child[2] = nullptr;
}

CSyntaxNode::CSyntaxNode(NODETYPE pnodetype, CSyntaxNode *p0, RETURNTYPE preturntype) 
	:m_Value(), m_NodeName(""), m_NodeType(pnodetype), m_ReturnType(preturntype), m_PreLen()
{
	m_child[0] = p0;
	m_child[1] = nullptr;
	m_child[2] = nullptr;
}

CSyntaxNode::CSyntaxNode(NODETYPE pnodetype, CSyntaxNode *p0, CSyntaxNode *p1, RETURNTYPE preturntype)
	:m_Value(), m_NodeName(""), m_NodeType(pnodetype), m_ReturnType(preturntype), m_PreLen()
{
	m_child[0] = p0;
	m_child[1] = p1;
	m_child[2] = nullptr;
}

CSyntaxNode::CSyntaxNode(NODETYPE pnodetype, CSyntaxNode *p0, CSyntaxNode *p1, CSyntaxNode *p2, RETURNTYPE preturntype)
	:m_Value(), m_NodeName(""), m_NodeType(pnodetype), m_ReturnType(preturntype), m_PreLen()
{
	m_child[0] = p0;
	m_child[1] = p1;
	m_child[2] = p2;
}

CSyntaxNode::CSyntaxNode(NODETYPE pnodetype, int piValue, RETURNTYPE preturntype)
	:m_Value(), m_NodeName(""), m_NodeType(pnodetype), m_ReturnType(preturntype), m_PreLen()
{
	m_Value.iValue = piValue;
	m_child[0] = nullptr;
	m_child[1] = nullptr;
	m_child[2] = nullptr;
}

CSyntaxNode::CSyntaxNode(NODETYPE pnodetype, float pfValue, RETURNTYPE preturntype)
	:m_Value(), m_NodeName(""), m_NodeType(pnodetype), m_ReturnType(preturntype), m_PreLen()
{
	m_Value.fValue = pfValue;
	m_child[0] = nullptr;
	m_child[1] = nullptr;
	m_child[2] = nullptr;
}

CSyntaxNode::CSyntaxNode(NODETYPE pnodetype, char pcValue, RETURNTYPE preturntype)
	:m_Value(), m_NodeName(""), m_NodeType(pnodetype), m_ReturnType(preturntype), m_PreLen()
{
	m_Value.cValue = pcValue;
	m_child[0] = nullptr;
	m_child[1] = nullptr;
	m_child[2] = nullptr;
}

CSyntaxNode::CSyntaxNode(NODETYPE pnodetype, const string &psValue, RETURNTYPE preturntype)
	:m_Value(), m_NodeName(""), m_NodeType(pnodetype), m_ReturnType(preturntype), m_PreLen()
{
	m_Value.sValue = psValue;

	m_child[0] = nullptr;
	m_child[1] = nullptr;
	m_child[2] = nullptr;

	if (preturntype == STRUCT_NAME_TYPE){
		m_NodeName = psValue;
		m_Value.sValue = "";
	}
}

CSyntaxNode::CSyntaxNode(NODETYPE pnodetype, const string &pname, CSyntaxNode * p0, RETURNTYPE preturntype)
	:m_Value(), m_NodeName(pname), m_NodeType(pnodetype), m_ReturnType(preturntype), m_PreLen()
{
	m_child[0] = p0;
	m_child[1] = nullptr;
	m_child[2] = nullptr;
}

CSyntaxNode::CSyntaxNode(NODETYPE pnodetype, const string &pname, CSyntaxNode * p0, CSyntaxNode * p1, RETURNTYPE preturntype)
	:m_Value(), m_NodeName(pname), m_NodeType(pnodetype), m_ReturnType(preturntype), m_PreLen()
{
	m_child[0] = p0;
	m_child[1] = p1;
	m_child[2] = nullptr;
}

CSyntaxNode::CSyntaxNode(NODETYPE pnodetype, const string &pname, CSyntaxNode * p0, CSyntaxNode * p1, CSyntaxNode * p2, RETURNTYPE preturntype)
	:m_Value(), m_NodeName(pname), m_NodeType(pnodetype), m_ReturnType(preturntype), m_PreLen()
{
	m_child[0] = p0;
	m_child[1] = p1;
	m_child[2] = p2;
}

CSyntaxNode::CSyntaxNode(NODETYPE pnodetype, int piValue, CSyntaxNode * p0, CSyntaxNode * p1, CSyntaxNode * p2, RETURNTYPE preturntype)
	:m_Value(), m_NodeName(""), m_NodeType(pnodetype), m_ReturnType(preturntype), m_PreLen()
{
	m_Value.iValue = piValue;
	m_child[0] = p0;
	m_child[1] = p1;
	m_child[2] = p2;
}

CSyntaxNode::CSyntaxNode(NODETYPE pnodetype, RETURNTYPE preturntype, vector<string> &ST)
	:m_Value(), m_NodeName(""), m_NodeType(pnodetype), m_ReturnType(preturntype), m_PreLen()
{
	m_Value.ST = ST;
	m_child[0] = nullptr;
	m_child[1] = nullptr;
	m_child[2] = nullptr;
}

CSyntaxNode::CSyntaxNode(NODETYPE pnodetype, RETURNTYPE preturntype, string&psValue, vector<string> &ST)
	:m_Value(), m_NodeName(""), m_NodeType(pnodetype), m_ReturnType(preturntype), m_PreLen()
{
	m_Value.sValue = psValue;
	m_Value.ST = ST;
	m_child[0] = nullptr;
	m_child[1] = nullptr;
	m_child[2] = nullptr;
}

void CSyntaxNode::CopyNode(CSyntaxNode* pnode){
	m_NodeType = pnode->m_NodeType;
	m_ReturnType = pnode->m_ReturnType;
	m_NodeName = pnode->m_NodeName;
	m_Value.iValue = pnode->m_Value.iValue;
	m_Value.fValue = pnode->m_Value.fValue;
	m_Value.cValue = pnode->m_Value.cValue;
	m_Value.sValue = pnode->m_Value.sValue;
	m_Value.ST = pnode->m_Value.ST;
}

//注意copytree函数的参数不可以是调用该函数的对象
void CSyntaxNode::CopyTree(CSyntaxNode *pnode)
{
	if (pnode == nullptr)
		return;
	else{
		if (pnode->m_child[0] != nullptr){
			delete m_child[0];
			m_child[0] = new CSyntaxNode();
			m_child[0]->CopyTree(pnode->m_child[0]);
		}
		else{
			delete m_child[0];
			m_child[0] = nullptr;
		}

		if (pnode->m_child[1] != nullptr){
			delete m_child[1];
			m_child[1] = new CSyntaxNode();
			m_child[1]->CopyTree(pnode->m_child[1]);
		}
		else{
			delete m_child[1];
			m_child[1] = nullptr;
		}

		if (pnode->m_child[2] != nullptr){
			delete m_child[2];
			m_child[2] = new CSyntaxNode();
			m_child[2]->CopyTree(pnode->m_child[2]);
		}
		else{
			delete m_child[2];
			m_child[2] = nullptr;
		}
		CopyNode(pnode);
	}
}

bool CSyntaxNode::CompareNode(CSyntaxNode *pnode)
{
	if (this == nullptr || pnode == nullptr){
		return false;
	}
	else{ //除了m_child指针，各个节点变量都相等  2013-5-2 add by YY[fixed]
		if (m_NodeType == pnode->m_NodeType  &&
			m_ReturnType == pnode->m_ReturnType  &&
			m_NodeName == pnode->m_NodeName  &&
			m_Value.iValue == pnode->m_Value.iValue &&
			m_Value.cValue == pnode->m_Value.cValue  &&
			m_Value.fValue == pnode->m_Value.fValue &&
			m_Value.sValue == pnode->m_Value.sValue&&
			m_Value.ST == pnode->m_Value.ST)
		{
			return true;
		}
		else{
			return false;
		}
	}
}

/*
比较两棵树是否相同
包含p and q and r与r and p and q的比较，以及p or q or r 与q or r or p
*/
bool CSyntaxNode::CompareTree(CSyntaxNode *ptree)
{
	if (this == nullptr || ptree == nullptr)
		return false;
	
	CSyntaxNode *temp = nullptr;
	CSyntaxNode *temp1 = nullptr;
	CSyntaxNode *temp2 = nullptr;

	if (!CompareNode(ptree)){
		return false;
	}//p and q and r与r and p and q
	else if (m_NodeType == AND_EXP && ptree->m_NodeType == AND_EXP){
		temp1 = new CSyntaxNode();
		temp2 = new CSyntaxNode();
		temp1->CopyTree(this);
		temp2->CopyTree(ptree);
		while (temp2->m_NodeType == AND_EXP && temp2->m_child[0] != NULL && temp2->m_child[0]->m_NodeType == AND_EXP){
			if (!(FindInTree(this, temp2->m_child[1], AND_EXP))){
				return false;
			}
			temp = new CSyntaxNode();
			temp->CopyTree(temp2->m_child[0]);
			temp2->CopyTree(temp);
			delete temp;
			temp = NULL;
		}
		if (temp2->m_NodeType == AND_EXP && temp2->m_child[0] != NULL && temp2->m_child[0]->m_NodeType != AND_EXP){
			if (!(FindInTree(this, temp2->m_child[0], AND_EXP)))
				return false;
			if (!(FindInTree(this, temp2->m_child[1], AND_EXP)))
				return false;
		}
		delete temp2;
		temp2 = NULL;

		while (temp1->m_NodeType == AND_EXP && temp1->m_child[0] != NULL && temp1->m_child[0]->m_NodeType == AND_EXP){
			if (!(FindInTree(ptree, temp1->m_child[1], AND_EXP))){
				return false;
			}
			temp = new CSyntaxNode();
			temp->CopyTree(temp1->m_child[0]);
			temp1->CopyTree(temp);
			delete temp;
			temp = NULL;
		}

		if (temp1->m_NodeType == AND_EXP && temp1->m_child[0] != NULL &&temp1->m_child[0]->m_NodeType != AND_EXP){
			if (!(FindInTree(ptree, temp1->m_child[0], AND_EXP)))
				return false;
			if (!(FindInTree(ptree, temp1->m_child[1], AND_EXP)))
				return false;
		}
		delete temp1;
		temp1 = NULL;
	}//p or q or r 与q or r or p
	else if (m_NodeType == OR_EXP && ptree->m_NodeType == OR_EXP){
		temp1 = new CSyntaxNode();
		temp2 = new CSyntaxNode();
		temp1->CopyTree(this);
		temp2->CopyTree(ptree);
		while (temp2->m_NodeType == OR_EXP && temp2->m_child[0] != NULL && temp2->m_child[0]->m_NodeType == OR_EXP){
			if (!(FindInTree(this, temp2->m_child[1], OR_EXP))){
				return false;
			}
			temp = new CSyntaxNode();
			temp->CopyTree(temp2->m_child[0]);
			temp2->CopyTree(temp);
			delete temp;
			temp = NULL;
		}
		if (temp2->m_NodeType == OR_EXP && temp2->m_child[0] != NULL && temp2->m_child[0]->m_NodeType != OR_EXP){
			if (!(FindInTree(this, temp2->m_child[0], OR_EXP)))
				return false;
			if (!(FindInTree(this, temp2->m_child[1], OR_EXP)))
				return false;
		}
		delete temp2;
		temp2 = NULL;
		while (temp1->m_NodeType == OR_EXP && temp1->m_child[0] != NULL && temp1->m_child[0]->m_NodeType == OR_EXP){
			if (!(FindInTree(ptree, temp1->m_child[1], OR_EXP))){
				return false;
			}
			temp = new CSyntaxNode();
			temp->CopyTree(temp1->m_child[0]);
			temp1->CopyTree(temp);
			delete temp;
			temp = NULL;
		}
		if (temp1->m_NodeType == OR_EXP && temp1->m_child[0] != NULL && temp1->m_child[0]->m_NodeType != OR_EXP){
			if (!(FindInTree(ptree, temp1->m_child[0], OR_EXP)))
				return false;
			if (!(FindInTree(ptree, temp1->m_child[1], OR_EXP)))
				return false;
		}
		delete temp1;
		temp1 = NULL;
	}
	else{
		if (m_child[0] != NULL /*&& ptree->m_child[0]!=NULL*/){
			if (!(m_child[0]->CompareTree(ptree->m_child[0])))
				return false;
		}
		if (m_child[1] != NULL /*&& ptree->m_child[1]!=NULL*/){
			if (!(m_child[1]->CompareTree(ptree->m_child[1])))
				return false;
		}
		if (ptree->m_child[2] != NULL /*&& ptree->GetChild2()!=NULL*/){
			if (!(m_child[2]->CompareTree(ptree->m_child[2])))
				return false;
		}
	}
	return true;
}

//在ptree中查找target是否存在，专用于CompareTree函数中
bool CSyntaxNode::FindInTree(CSyntaxNode * ptree, CSyntaxNode* target, NODETYPE ntype){
	if (target != nullptr && ptree->GetChild0() != nullptr && target->CompareTree(ptree->GetChild0())){
		return true;
	}
	else if (target != nullptr && ptree->GetChild1() != nullptr && target->CompareTree(ptree->GetChild1())){
		return true;
	}

	if (ptree->GetChild0() != nullptr && ptree->GetChild0()->GetNType() == ntype){
		if (FindInTree(ptree->GetChild0(), target, ntype))
			return true;
	}
	if (ptree->GetChild1() != nullptr && ptree->GetChild1()->GetNType() == ntype){
		if (FindInTree(ptree->GetChild1(), target, ntype))
			return true;
	}
	if (ptree->GetChild2() != nullptr && ptree->GetChild2()->GetNType() == ntype){
		if (FindInTree(ptree->GetChild2(), target, ntype)){
			return true;
		}
	}
	return false;
}

bool CSyntaxNode::FindSameName(string pname){
	if (!this->GetNName().compare(pname))
		return true;
	else
		return false;
}

CSyntaxNode* CSyntaxNode::GetNameNode(int &nPointer){
	CSyntaxNode *visit = this;
	while (visit != nullptr && visit->GetNType() != IDENT_EXP){
		if (visit->GetNType() == POINTER_EXP){
			nPointer++;
		}
		visit = visit->GetChild0();
	}
	return visit;
}

CSyntaxNode* CSyntaxNode::GetNameNode(){
	CSyntaxNode *visit = this;
	while (visit != nullptr && visit->GetNType() != IDENT_EXP){
		visit = visit->GetChild0();
	}
	return visit;
}

// 用于exists， 如果节点的名字等于origNam，如果是在函数中，将结点的名字换成#函数名_变量名_块名，如果是全局的，将节点名字换成#变量名_块名
void CSyntaxNode::replaceExistName(string origNam, string funcNam, string blockNam){
	if (m_NodeName == origNam){
		if (funcNam == ""){
			m_NodeName = "$" + m_NodeName + "_" + blockNam;
		}
		else{
			m_NodeName = "$" + funcNam + "_" + m_NodeName + "_" + blockNam;
		}
	}
	if (m_child[0] != nullptr){
		GetChild0()->replaceExistName(origNam, funcNam, blockNam);
	}
	if (m_child[1] != nullptr){
		GetChild1()->replaceExistName(origNam, funcNam, blockNam);
	}
	if (m_child[2] != nullptr){
		GetChild2()->replaceExistName(origNam, funcNam, blockNam);
	}
}

// 统一将结点的名字前面加上$$
void CSyntaxNode::replaceOrName(){
	m_NodeName = "$$" + m_NodeName;
	if (m_child[0] != nullptr){
		GetChild0()->replaceOrName();
	}
	if (m_child[1] != nullptr){
		GetChild1()->replaceOrName();
	}
	if (m_child[2] != nullptr){
		GetChild2()->replaceOrName();
	}
}
/*
//为了进行测试输出做准备
string CSyntaxNode::TreeToString()
{
	if (NULL == this)//如果待转换的语法树为空
	{
		return "";//则返回空字符串
	}
	string Stree = "";

	switch (m_NodeType)//根据待转换语法树节点的类型
	{
	case PARALLEL_STA:
	{
		Stree += m_child[0]->TreeToString();
		Stree += " || ";
		Stree += m_child[1]->TreeToString();
		break;
	}
	case NEXT_STA:
	{
		Stree += "next (";
		Stree += m_child[0]->TreeToString();
		Stree += ")";
		break;
	}
	case EMPTY_EXP:// empty
		Stree += "empty";
		break;
	case SKIP_STA://skip
		Stree += "skip";
		break;

	case DECLARE_STA://如果是变量声明语句
	{
		switch (m_ReturnType)//根据节点的返回类型
		{
		case INTTYPE:
			Stree += "int ";
			Stree += m_child[0]->TreeToString();//把声明的变量序列转换为可以输出的字符串
			break;
		case FLOATTYPE:
			Stree += "float ";
			Stree += m_child[0]->TreeToString();//把声明的变量序列转换为可以输出的字符串
			break;
		case CHARTYPE:
			Stree += "char ";
			Stree += m_child[0]->TreeToString();//把声明的变量序列转换为可以输出的字符串
			break;
		case UINTTYPE:
			Stree += "unsigned int ";
			Stree += m_child[0]->TreeToString();
			break;
		case UCHARTYPE:
			Stree += "unsigned char ";
			Stree += m_child[0]->TreeToString();
			break;
		case STRUCTTYPE:
			Stree += "struct " + m_NodeName + " ";
			Stree += m_child[0]->TreeToString();//把声明的变量序列转换为可以输出的字符串
			break;
		case VOIDTYPE:
			Stree += "void ";
			Stree += m_child[0]->TreeToString();
			break;
		case STRTYPE:
			Stree += "string ";
			Stree += m_child[0]->TreeToString();
			break;
		}

		break;
	}
	case DOT_EXP:
	{
		Stree += m_child[0]->TreeToString();
		Stree += ".";
		Stree += m_child[1]->TreeToString();
		break;
	}
	case ARROW_EXP:
	{
		Stree += m_child[0]->TreeToString();
		Stree += "->";
		Stree += m_child[1]->TreeToString();
		break;
	}
	case OPTION_EXP:
	{
		Stree += m_child[0]->TreeToString();
		CSyntaxNode* visit = m_child[1];

		while (visit != NULL)
		{
			Stree += ",";
			Stree += visit->GetChild0()->TreeToString();
			visit = visit->GetChild1();
		}
		break;
	}
	//add by YY 2013/06/09 9:34
	case ARRAY_DECLARE_STA:
	case ARRAY_EXP://LIST_VALUE
	{
		Stree += m_child[0]->TreeToString();
		Stree += "[" + m_child[1]->TreeToString() + "]";
		break;
	}
	case IDENT_EXP://identifier
	{
		//add by YY [fixed] 2013/05/07 10:45
		//以下两种赋值形式都可以：将string类型赋值给cstring
		Stree += m_NodeName.data();
		break;
	}
	case ADDRESS_EXP:
	{
		Stree += '&';
		Stree += m_child[0]->TreeToString();
		break;
	}
	case POINTER_EXP://*()
	{
		Stree += '*';
		Stree += m_child[0]->TreeToString();
		break;
	}

	case STR_EXP://string
	{
		Stree += "\"";
		string temp = m_Value.sValue.c_str();
		ReplaceESC(temp);
		Stree += temp;
		Stree += "\"";
		break;
	}

	case INTEGER_EXP:// interger
	{ stringstream ss;
	ss << m_Value.iValue;
	Stree += ss.str();
	ss.clear();
	break;
	}
	case FLOATLITERAL_EXP:// float
	{
		stringstream ss;
		ss << m_Value.fValue;
		Stree += ss.str();
		ss.clear();
		break;
	}
	case CHARLITERAL_EXP:// char
	{
		Stree += '\'';
		string tmp = ReturnESC(m_Value.cValue);
		if (tmp == "")
		{
			Stree += m_Value.cValue;
		}
		else
		{
			Stree += tmp;
		}
		Stree += '\'';
		break;
	}

	case ASS_EQU_EXP: //"<=="
		Stree += m_child[0]->TreeToString();
		Stree += "=";
		Stree += m_child[1]->TreeToString();
		break;
	case EX_ASS_EXP://"<=="
		Stree += m_child[0]->TreeToString();
		Stree += "=";
		Stree += m_child[1]->TreeToString();
		break;
	case EQU_EXP:
		Stree += m_child[0]->TreeToString();
		Stree += "==";
		Stree += m_child[1]->TreeToString();
		break;
	case UNITASSIGN_EXP://:=
		Stree += m_child[0]->TreeToString();
		Stree += ":=";
		Stree += m_child[1]->TreeToString();
		break;
		//add by YY[fixed] 2013/05/24 16:11
	case CHOP_STA://";"
		Stree += m_child[0]->TreeToString();
		Stree += ";\n";
		Stree += m_child[1]->TreeToString();
		break;
	case NEGATION_EXP:
		Stree += "!";
		if (!(m_child[0]->m_NodeType == IDENT_EXP
			|| m_child[0]->m_NodeType == INTEGER_EXP
			|| m_child[0]->m_NodeType == TRUE_EXP
			|| m_child[0]->m_NodeType == FALSE_EXP))
		{
			Stree += "(";
			Stree += m_child[0]->TreeToString();
			Stree += ")";
		}
		else
			Stree += m_child[0]->TreeToString();
		break;
	case IF_ELSE_STA://"if_else"
	{
		Stree += "if (";
		Stree += m_child[0]->TreeToString();
		Stree += "){\n";
		Stree += m_child[1]->TreeToString();
		Stree += ";\n";
		if (m_child[2] != NULL)
		{
			Stree += "} else {";
			Stree += m_child[2]->TreeToString();
			Stree += ";\n";
		}
		Stree += "}";
		Stree += "\n";
		break;
	}
	//a>b? a: b
	case IF_ELSE_EXP:
	{
		Stree += "(";
		Stree += "(";
		Stree += m_child[0]->TreeToString();
		Stree += ") ?";
		Stree += m_child[1]->TreeToString();
		Stree += " : ";
		Stree += m_child[2]->TreeToString();
		Stree += " ";

		Stree += ")";
		break;
	}
	//Annotated by YY  2013/06/09 9:30
	case WHILE_DO_STA://"while_do"
		Stree += "while(";
		Stree += m_child[0]->TreeToString();
		Stree += "){";
		Stree += m_child[1]->TreeToString();
		Stree += ";\n";
		Stree += "}";

		Stree += "\n";
		break;
	case FOR_TIMES_STA://for
		Stree += "for ";
		Stree += m_child[0]->TreeToString();
		Stree += " times do{";
		Stree += m_child[1]->TreeToString();
		Stree += "}";
		break;
	case AND_EXP:
		if (!g_InStruct)
		{
			Stree += m_child[0]->TreeToString();
			Stree += " and ";
			Stree += m_child[1]->TreeToString();
		}
		else
		{
			Stree += m_child[0]->TreeToString();
			Stree += ";\n";
			Stree += m_child[1]->TreeToString();
		}
		break;
	case OR_EXP:
		Stree += m_child[0]->TreeToString();
		Stree += " or ";
		Stree += m_child[1]->TreeToString();
		break;
	case GT_EXP://">"
		Stree += m_child[0]->TreeToString();
		Stree += ">";
		Stree += m_child[1]->TreeToString();
		break;
	case LT_EXP://"<"
		Stree += m_child[0]->TreeToString();
		Stree += "<";
		Stree += m_child[1]->TreeToString();
		break;
	case GE_EXP://">="
		Stree += m_child[0]->TreeToString();
		Stree += ">=";
		Stree += m_child[1]->TreeToString();
		break;
	case LE_EXP://"<="
		Stree += m_child[0]->TreeToString();
		Stree += "<=";
		Stree += m_child[1]->TreeToString();
		break;
	case NE_EXP://"!="
		Stree += "(" + m_child[0]->TreeToString() + ")";
		Stree += "!=";
		Stree += "(" + m_child[1]->TreeToString() + ")";
		break;
	case ADD_EXP://"+"
		Stree += "(" + m_child[0]->TreeToString() + ")";
		Stree += "+";
		Stree += "(" + m_child[1]->TreeToString() + ")";
		break;
	case SUB_EXP://"-"
		Stree += "(" + m_child[0]->TreeToString() + ")";
		Stree += "-";
		Stree += "(" + m_child[1]->TreeToString() + ")";
		break;
	case UNARY_SUB_EXP:
		Stree += '-';
		if (!(m_child[0]->m_NodeType == IDENT_EXP || m_child[0]->m_NodeType == INTEGER_EXP || m_child[0]->m_NodeType == TRUE_EXP || m_child[0]->m_NodeType == FALSE_EXP))
		{
			Stree += "(";
			Stree += m_child[0]->TreeToString();
			Stree += ")";
		}
		else
			Stree += m_child[0]->TreeToString();
		break;
	case MUL_EXP://"*"
	{
		Stree += "(";
		if (m_child[0]->m_NodeType == ADD_EXP || m_child[0]->m_NodeType == SUB_EXP)
		{
			Stree += "(";
			Stree += m_child[0]->TreeToString();
			Stree += ")";
		}
		else
			Stree += m_child[0]->TreeToString();

		Stree += '*';

		if (m_child[1]->m_NodeType == ADD_EXP || m_child[1]->m_NodeType == SUB_EXP)
		{
			Stree += "(";
			Stree += m_child[1]->TreeToString();
			Stree += ")";
		}
		else
			Stree += m_child[1]->TreeToString();
		Stree += ")";
		break;
	}
	case DIV_EXP://"/"
	{
		Stree += "(";
		if (m_child[0]->m_NodeType == ADD_EXP || m_child[0]->m_NodeType == SUB_EXP)
		{
			Stree += "(";
			Stree += m_child[0]->TreeToString();
			Stree += ")";
		}
		else
			Stree += m_child[0]->TreeToString();

		Stree += "/ ";

		if (m_child[1]->m_NodeType == ADD_EXP || m_child[1]->m_NodeType == SUB_EXP)
		{
			Stree += "(";
			Stree += m_child[1]->TreeToString();
			Stree += ")";
		}
		else
			Stree += m_child[1]->TreeToString();
		Stree += ")";
		break;
	}

	case MOD_EXP://"%"
	{
		Stree += "(";
		if (m_child[0]->m_NodeType == ADD_EXP || m_child[0]->m_NodeType == SUB_EXP)
		{
			Stree += "(";
			Stree += m_child[0]->TreeToString();
			Stree += ")";
		}
		else
			Stree += m_child[0]->TreeToString();

		Stree += '%';

		if (m_child[1]->m_NodeType == ADD_EXP || m_child[1]->m_NodeType == SUB_EXP)
		{
			Stree += "(";
			Stree += m_child[1]->TreeToString();
			Stree += ")";
		}
		else
			Stree += m_child[1]->TreeToString();
		Stree += ")";
		break;
	}

	case TRUE_EXP://"true"
		Stree += "true";
		break;
	case FALSE_EXP://"false"
		Stree += "false";
		break;
	case REQUEST_STA://程序中的input()语句
	{
		Stree += m_Value.sValue;
		break;
	}
	case DISPLAY_STA://程序中的output()语句
	{
		Stree += "output(";
		Stree += m_child[0]->TreeToString();
		Stree += ")";
		break;
	}
	case STATE_OUTPUT_STA://每个状态的输出语句
	{
		Stree += "StateOutput(";
		vector<string>::iterator iter;
		for (iter = m_Value.ST.begin(); iter != m_Value.ST.end(); iter++)
		{
			Stree += *iter;
			Stree += ",";
		}
		Stree += ")\n";
		break;
	}
	case FUNCTION_STA:
	case EXT_FUNCTION_STA:
	{
		if (this->GetChild0()->GetNName() == "$$f1")
		{
			int a=1;
		}

		if (m_ReturnType == FUNCPTYPE)
		{
			Stree += "(";
			Stree += m_child[0]->TreeToString();
			Stree += ")";
			Stree += "(";
			CSyntaxNode* par = m_child[1];
			while (par != NULL)
			{
				Stree += par->TreeToString();
				par = par->m_child[1];
				if (par != NULL)
				{
					Stree += ",";
				}
			}
			Stree += ")";
		}
		else
		{
			//函数调用语句
			Stree += m_child[0]->TreeToString();
			Stree += "(" + m_child[1]->TreeToString() + ")";
		}
		break;
	}
	case STRUCT_DEFINE_STA:
	{
		Stree += "struct " + m_NodeName + "{\n";
		Stree += m_child[0]->TreeToString();
		Stree += ";\n}\n";
		break;
	}
	case THREAD_FUNC:
	{
		Stree += m_NodeName;
		Stree += "(" + m_child[0]->TreeToString() + ")";
		Stree += "{\n" + m_child[1]->TreeToString() + "\n}\n";
		Stree += m_child[2]->TreeToString();
		break;
	}
	case FUNCTION_DEFINE_STA:
	{
		if (m_child[0]->GetChild0() != NULL)
		{
			Stree += m_child[0]->GetChild0()->TreeToString();
		}
		else
		{
			Stree += "void ";
		}
		Stree += m_NodeName;
		Stree += "(" + m_child[0]->GetChild1()->TreeToString() + ")";
		Stree += "{\n" + m_child[1]->TreeToString() + "\n}\n";
		Stree += m_child[2]->TreeToString();
		break;
	}
	case RETURN_STA:
	{
		Stree += "return ";
		//modified by yubin 2015/4/17,以前return的情况只能是变量，现在也可以是表达式，因此输出的时候，输出child0
		Stree += m_child[0]->TreeToString();
		Stree += ";";
		break;
	}
	case DEC_VAR_LIST://参数表达式
	{
		CSyntaxNode* par = this;
		while (par != NULL)
		{
			Stree += par->m_child[0]->TreeToString();
			par = par->m_child[1];
			if (par != NULL)
			{
				Stree += ",";
			}
		}
		break;
	}

	case FUNCP_PARAMETER_EXP:
	{
		if (m_child[0]->m_child[0] != NULL)
		{
			Stree += m_child[0]->m_child[0]->TreeToString();
		}
		else
		{
			Stree += "void ";
		}
		Stree += "(*";
		Stree += m_NodeName;
		Stree += ")";
		Stree += "(";
		Stree += m_child[0]->m_child[1]->TreeToString();
		Stree += ")";
		break;
	}
	case FORMAL_PARAMETER_EXP://参数表达式
	{
		CSyntaxNode* par = this;
		while (par != NULL)
		{

			switch (par->m_ReturnType)
			{
			case INTTYPE:
			{
				Stree += "int ";
				break;
			}
			case FLOATTYPE:
			{
				Stree += "float ";
				break;
			}
			case CHARTYPE:
			{
				Stree += "char ";
				break;
			}
			case UINTTYPE:
			{
				Stree += "unsigned int ";
				break;
			}
			case UCHARTYPE:
			{
				Stree += "unsigned char ";
				break;
			}
			case VOIDTYPE:
			{
				Stree += "void ";
				break;
			}
			case STRUCTTYPE:
			{
				Stree += par->GetNName() + " ";
				break;
			}
			default:
				break;
			}


			Stree += par->m_child[0]->TreeToString();
			par = par->m_child[1];
			if (par != NULL)
			{
				Stree += ",";
			}
		}
		break;
	}
	case TYPE_CAST_STA:{
		//要转换为的类型
		Stree += "(";
		Stree += m_child[0]->TreeToString();
		Stree += ")";
		Stree += m_child[1]->TreeToString();
		break;
	}
	case CAST_TYPE:{
		switch (m_ReturnType){
		case STRUCTTYPE:
			Stree += m_NodeName;
			break;
		case VOIDTYPE:
			Stree += "void";
			break;
		case INTTYPE:
			Stree += "int";
			break;
		case CHARTYPE:
			Stree += "char";
			break;
		case FLOATTYPE:
			Stree += "float";
			break;
		default:
			//cout << "unknown type!" << endl;
			break;
		}

		Stree += m_child[0]->TreeToString();
		break;
	}
	case NULL_EXP:{
		Stree += "NULL";
		break;
	}
	case LST_EXP:{
		Stree += "(" + m_child[0]->TreeToString() + ")";
		Stree += "<<";
		Stree += "(" + m_child[1]->TreeToString() + ")";
		break;
	}
	case RST_EXP:{
		Stree += "(" + m_child[0]->TreeToString() + ")";
		Stree += ">>";
		Stree += "(" + m_child[1]->TreeToString() + ")";
		break;
	}
	case BAN_EXP:{
		Stree += "(" + m_child[0]->TreeToString() + ")";
		Stree += "&";
		Stree += "(" + m_child[1]->TreeToString() + ")";
		break;
	}
	case BOR_EXP:{
		Stree += "(" + m_child[0]->TreeToString() + ")";
		Stree += "|";
		Stree += "(" + m_child[1]->TreeToString() + ")";
		break;
	}
	case CON_EXP:{
		Stree += "(" + m_child[0]->TreeToString() + ")";
		Stree += "^";
		Stree += "(" + m_child[1]->TreeToString() + ")";
		break;
	}
	case XOR_EXP:{
		Stree += "(" + m_child[0]->TreeToString() + ")";
		Stree += "^";
		Stree += "(" + m_child[1]->TreeToString() + ")";
		break;
	}
	case BNE_EXP:{
		Stree += "~";
		Stree += "(" + m_child[0]->TreeToString() + ")";
		break;
	}
	case SWITCH_STA:{
		Stree += "switch(";
		Stree += m_child[0]->TreeToString();
		Stree += ")";
		Stree += "{" + m_child[1]->TreeToString() + "}";
		break;
	}
	case CASE_STA:{
		Stree += "case ";
		Stree += m_child[0]->TreeToString();
		Stree += ":";
		Stree += m_child[1]->TreeToString();
		if (m_child[2]->GetRType() == BREAKTYPE)
		{
			Stree += ";break;";
		}
		if (m_child[2]->GetChild0())
		{
			Stree += m_child[2]->GetChild0()->TreeToString();
		}
		break;
	}
	case CREATE_THREAD:{
		Stree += "CreateThread";
		Stree += "(";
		Stree += m_NodeName + ", " + m_child[0]->TreeToString() + ", ";
		stringstream ss;
		ss << m_Value.iValue;
		Stree += ss.str();
		ss.clear();
		Stree += ")";
		break;
	}
	case SIZEOF_EXP:{
		Stree += "sizeof";
		Stree += "(";
		switch (m_ReturnType)
		{
		case STRUCTTYPE:
			Stree += m_NodeName;
			break;
		case VOIDTYPE:
			Stree += "void";
			break;
		case INTTYPE:
			Stree += "int";
			break;
		case CHARTYPE:
			Stree += "char";
			break;
		case FLOATTYPE:
			Stree += "float";
			break;
		default:
			//cout << "unknown type (1609)" << endl;
			break;
		}
		Stree += m_child[0]->TreeToString();
		break;
	}
	case NODELENGTH:{
		Stree += "SetNodeLength(";
		stringstream ss;
		ss << m_Value.iValue;
		Stree += ss.str();
		ss.clear();
		Stree += ")";
		break;
	}
	case LABEL_STA:{
		Stree += m_NodeName;
		Stree += ":\n";
		break;
	}
	}
	return Stree;
}*/

CSyntaxNode::~CSyntaxNode(){
	if (m_child[0] != nullptr){
		delete m_child[0];
		m_child[0] = nullptr;
	}
	if (m_child[1] != nullptr){
		delete m_child[1];
		m_child[1] = nullptr;
	}
	if (m_child[2] != nullptr){
		delete m_child[2];
		m_child[2] = nullptr;
	}
}



//为了进行测试输出做准备  2013-5-2 add by YY[fixed]
//------------------------------------------------------------------------------------------------------------------------
string CSyntaxNode::TreeToString()
{
	if (NULL == this)//如果待转换的语法树为空
	{
		return "";//则返回空字符串
	}
	string Stree = "";

	switch (m_NodeType)//根据待转换语法树节点的类型
	{
	case PARALLEL_STA:
	{
		Stree += m_child[0]->TreeToString();
		Stree += " || ";
		Stree += m_child[1]->TreeToString();
		break;
	}
	case NEXT_STA:
	{
		Stree += "next (";
		Stree += m_child[0]->TreeToString();
		Stree += ")";
		break;
	}
	case EMPTY_EXP:// empty
		Stree += "empty";
		break;
	case SKIP_STA://skip
		Stree += "skip";
		break;

	case DECLARE_STA://如果是变量声明语句
	{
		switch (m_ReturnType)//根据节点的返回类型
		{
		case INTTYPE:
			Stree += "int ";
			Stree += m_child[0]->TreeToString();//把声明的变量序列转换为可以输出的字符串
			break;
		case FLOATTYPE:
			Stree += "float ";
			Stree += m_child[0]->TreeToString();//把声明的变量序列转换为可以输出的字符串
			break;
		case CHARTYPE:
			Stree += "char ";
			Stree += m_child[0]->TreeToString();//把声明的变量序列转换为可以输出的字符串
			break;
		case UINTTYPE:
			Stree += "unsigned int ";
			Stree += m_child[0]->TreeToString();
			break;
		case UCHARTYPE:
			Stree += "unsigned char ";
			Stree += m_child[0]->TreeToString();
			break;
		case STRUCTTYPE:
			Stree += "struct " + m_NodeName + " ";
			Stree += m_child[0]->TreeToString();//把声明的变量序列转换为可以输出的字符串
			break;

		case VOIDTYPE:
			Stree += "void ";
			Stree += m_child[0]->TreeToString();
			break;
		case STRTYPE:
			Stree += "string ";
			Stree += m_child[0]->TreeToString();
			break;
		default:
			Stree += "struct " + m_NodeName + " ";
			Stree += m_child[0]->TreeToString();//把声明的变量序列转换为可以输出的字符串
			break;
		}

		break;
	}
	case DOT_EXP:
	{
		Stree += m_child[0]->TreeToString();
		Stree += ".";
		Stree += m_child[1]->TreeToString();
		break;
	}
	case ARROW_EXP:
	{
		Stree += m_child[0]->TreeToString();
		Stree += "->";
		Stree += m_child[1]->TreeToString();
		break;
	}
	case OPTION_EXP:
	{
		Stree += m_child[0]->TreeToString();
		CSyntaxNode* visit = m_child[1];

		while (visit != NULL)
		{
			Stree += ",";
			Stree += visit->GetChild0()->TreeToString();
			visit = visit->GetChild1();
		}
		break;
	}
	//add by YY 2013/06/09 9:34
	case ARRAY_DECLARE_STA:
	case ARRAY_EXP://LIST_VALUE
	{
		Stree += "(" + m_child[0]->TreeToString() + ")";
		Stree += "[" + m_child[1]->TreeToString() + "]";
		break;
	}
	case IDENT_EXP://identifier
	{
		//add by YY [fixed] 2013/05/07 10:45
		//以下两种赋值形式都可以：将string类型赋值给cstring
		Stree += m_NodeName.data();
		break;
	}
	case ADDRESS_EXP:
	{
		Stree += '&';
		Stree += m_child[0]->TreeToString();
		break;
	}
	case POINTER_EXP://*()
	{
		Stree += '*';
		Stree += '(' + m_child[0]->TreeToString() + ')';
		break;
	}

	case STR_EXP://string
	{
		Stree += "\"";
		string temp = m_Value.sValue.c_str();
		ReplaceESC(temp);
		Stree += temp;
		Stree += "\"";
		break;
	}

	case INTEGER_EXP:// interger
	{ stringstream ss;
	ss << m_Value.iValue;
	Stree += ss.str();
	ss.clear();
	break;
	}
	case FLOATLITERAL_EXP:// float
	{
		stringstream ss;
		ss << m_Value.fValue;
		Stree += ss.str();
		ss.clear();
		break;
	}
	case CHARLITERAL_EXP:// char
	{
		Stree += '\'';
		string tmp = ReturnESC(m_Value.cValue);
		if (tmp == "")
		{
			Stree += m_Value.cValue;
		}
		else
		{
			Stree += tmp;
		}
		Stree += '\'';
		break;
	}

	case ASS_EQU_EXP: //"<=="
		Stree += m_child[0]->TreeToString();
		Stree += "=";
		Stree += m_child[1]->TreeToString();
		break;
	case EX_ASS_EXP://"<=="
		Stree += m_child[0]->TreeToString();
		Stree += "=";
		Stree += m_child[1]->TreeToString();
		break;
	case EQU_EXP:
		Stree += m_child[0]->TreeToString();
		Stree += "==";
		Stree += m_child[1]->TreeToString();
		break;
	case UNITASSIGN_EXP://:=
		Stree += m_child[0]->TreeToString();
		Stree += ":=";
		Stree += m_child[1]->TreeToString();
		break;
		//add by YY[fixed] 2013/05/24 16:11
	case CHOP_STA://";"
		Stree += m_child[0]->TreeToString();
		Stree += ";\n";
		Stree += m_child[1]->TreeToString();
		break;
	case NEGATION_EXP:
		Stree += "!";
		if (!(m_child[0]->m_NodeType == IDENT_EXP
			|| m_child[0]->m_NodeType == INTEGER_EXP
			|| m_child[0]->m_NodeType == TRUE_EXP
			|| m_child[0]->m_NodeType == FALSE_EXP))
		{
			Stree += "(";
			Stree += m_child[0]->TreeToString();
			Stree += ")";
		}
		else
			Stree += m_child[0]->TreeToString();
		break;
	case IF_ELSE_STA://"if_else"
	{
		Stree += "if (";
		Stree += m_child[0]->TreeToString();
		Stree += "){\n";
		Stree += m_child[1]->TreeToString();
		Stree += ";\n";
		if (m_child[2] != NULL)
		{
			Stree += "} else {";
			Stree += m_child[2]->TreeToString();
			Stree += ";\n";
		}
		Stree += "}";
		Stree += "\n";
		break;
	}
	//a>b? a: b
	case IF_ELSE_EXP:
	{
		Stree += "(";
		Stree += "(";
		Stree += m_child[0]->TreeToString();
		Stree += ") ?";
		Stree += m_child[1]->TreeToString();
		Stree += " : ";
		Stree += m_child[2]->TreeToString();
		Stree += " ";

		Stree += ")";
		break;
	}
	//Annotated by YY  2013/06/09 9:30
	case WHILE_DO_STA://"while_do"
		Stree += "while(";
		Stree += m_child[0]->TreeToString();
		Stree += "){";
		Stree += m_child[1]->TreeToString();
		Stree += ";\n";
		Stree += "}";

		Stree += "\n";
		break;
	case FOR_TIMES_STA://for
		Stree += "for ";
		Stree += m_child[0]->TreeToString();
		Stree += " times do{";
		Stree += m_child[1]->TreeToString();
		Stree += "}";
		break;
	case AND_EXP:
		if (!g_InStruct)
		{
			Stree += m_child[0]->TreeToString();
			Stree += " and ";
			Stree += m_child[1]->TreeToString();
		}
		else
		{
			Stree += m_child[0]->TreeToString();
			Stree += ";\n";
			Stree += m_child[1]->TreeToString();
		}
		break;
	case OR_EXP:
		Stree += m_child[0]->TreeToString();
		Stree += " or ";
		Stree += m_child[1]->TreeToString();
		break;
	case GT_EXP://">"
		Stree += m_child[0]->TreeToString();
		Stree += ">";
		Stree += m_child[1]->TreeToString();
		break;
	case LT_EXP://"<"
		Stree += m_child[0]->TreeToString();
		Stree += "<";
		Stree += m_child[1]->TreeToString();
		break;
	case GE_EXP://">="
		Stree += m_child[0]->TreeToString();
		Stree += ">=";
		Stree += m_child[1]->TreeToString();
		break;
	case LE_EXP://"<="
		Stree += m_child[0]->TreeToString();
		Stree += "<=";
		Stree += m_child[1]->TreeToString();
		break;
	case NE_EXP://"!="
		Stree += "(" + m_child[0]->TreeToString() + ")";
		Stree += "!=";
		Stree += "(" + m_child[1]->TreeToString() + ")";
		break;
	case ADD_EXP://"+"
		Stree += "(" + m_child[0]->TreeToString() + ")";
		Stree += "+";
		Stree += "(" + m_child[1]->TreeToString() + ")";
		break;
	case SUB_EXP://"-"
		Stree += "(" + m_child[0]->TreeToString() + ")";
		Stree += "-";
		Stree += "(" + m_child[1]->TreeToString() + ")";
		break;
	case UNARY_SUB_EXP:
		Stree += '-';
		if (!(m_child[0]->m_NodeType == IDENT_EXP || m_child[0]->m_NodeType == INTEGER_EXP || m_child[0]->m_NodeType == TRUE_EXP || m_child[0]->m_NodeType == FALSE_EXP))
		{
			Stree += "(";
			Stree += m_child[0]->TreeToString();
			Stree += ")";
		}
		else
			Stree += m_child[0]->TreeToString();
		break;
	case MUL_EXP://"*"
	{
		Stree += "(";
		if (m_child[0]->m_NodeType == ADD_EXP || m_child[0]->m_NodeType == SUB_EXP)
		{
			Stree += "(";
			Stree += m_child[0]->TreeToString();
			Stree += ")";
		}
		else
			Stree += m_child[0]->TreeToString();

		Stree += '*';

		if (m_child[1]->m_NodeType == ADD_EXP || m_child[1]->m_NodeType == SUB_EXP)
		{
			Stree += "(";
			Stree += m_child[1]->TreeToString();
			Stree += ")";
		}
		else
			Stree += m_child[1]->TreeToString();
		Stree += ")";
		break;
	}
	case DIV_EXP://"/"
	{
		Stree += "(";
		if (m_child[0]->m_NodeType == ADD_EXP || m_child[0]->m_NodeType == SUB_EXP)
		{
			Stree += "(";
			Stree += m_child[0]->TreeToString();
			Stree += ")";
		}
		else
			Stree += m_child[0]->TreeToString();

		Stree += "/ ";

		if (m_child[1]->m_NodeType == ADD_EXP || m_child[1]->m_NodeType == SUB_EXP)
		{
			Stree += "(";
			Stree += m_child[1]->TreeToString();
			Stree += ")";
		}
		else
			Stree += m_child[1]->TreeToString();
		Stree += ")";
		break;
	}

	case MOD_EXP://"%"
	{
		Stree += "(";
		if (m_child[0]->m_NodeType == ADD_EXP || m_child[0]->m_NodeType == SUB_EXP)
		{
			Stree += "(";
			Stree += m_child[0]->TreeToString();
			Stree += ")";
		}
		else
			Stree += m_child[0]->TreeToString();

		Stree += '%';

		if (m_child[1]->m_NodeType == ADD_EXP || m_child[1]->m_NodeType == SUB_EXP)
		{
			Stree += "(";
			Stree += m_child[1]->TreeToString();
			Stree += ")";
		}
		else
			Stree += m_child[1]->TreeToString();
		Stree += ")";
		break;
	}

	case TRUE_EXP://"true"
		Stree += "true";
		break;
	case FALSE_EXP://"false"
		Stree += "false";
		break;
	case REQUEST_STA://程序中的input()语句
	{
		Stree += m_Value.sValue;
		break;
	}
	case DISPLAY_STA://程序中的output()语句
	{
		Stree += "output(";
		Stree += m_child[0]->TreeToString();
		Stree += ")";
		break;
	}
	case STATE_OUTPUT_STA://每个状态的输出语句
	{
		Stree += "StateOutput(";
		vector<string>::iterator iter;
		for (iter = m_Value.ST.begin(); iter != m_Value.ST.end(); iter++)
		{
			Stree += *iter;
			Stree += ",";
		}
		Stree += ")\n";
		break;
	}
	case FUNCTION_STA:
	{
		if (m_ReturnType == FUNCPTYPE)
		{
			Stree += "(";
			Stree += m_child[0]->TreeToString();
			Stree += ")";
			Stree += "(";
			CSyntaxNode* par = m_child[1];
			while (par != NULL)
			{
				Stree += par->TreeToString();
				par = par->m_child[1];
				if (par != NULL)
				{
					Stree += ",";
				}
			}
			Stree += ")";
		}
		else
		{
			//函数调用语句
			Stree += m_child[0]->TreeToString();
			Stree += "(" + m_child[1]->TreeToString() + ")";
		}
		break;
	}
	case EXT_FUNCTION_STA:
	{
		//函数调用语句
		Stree += m_child[0]->m_child[0]->TreeToString();
		Stree += "(" + m_child[0]->m_child[1]->TreeToString() + ")";
		break;
	}
	case STRUCT_DEFINE_STA:
	{
		Stree += "struct " + m_NodeName + "{\n";
		Stree += m_child[0]->TreeToString();
		Stree += ";\n}\n";
		Stree += m_child[1]->TreeToString();
		break;
	}
	case THREAD_FUNC:
	{
		Stree += m_NodeName;
		Stree += "(" + m_child[0]->TreeToString() + ")";
		Stree += "{\n" + m_child[1]->TreeToString() + "\n}\n";
		Stree += m_child[2]->TreeToString();
		break;
	}
	case FUNCTION_DEFINE_STA:
	{
		/*if (m_child[0]->GetChild0() != NULL)
		{
		Stree += m_child[0]->GetChild0()->TreeToString();
		}
		else
		{
		Stree += "void ";
		}*/
		Stree += m_NodeName;
		Stree += "(" + m_child[0]->TreeToString() + ")";
		Stree += "{\n" + m_child[1]->TreeToString() + "\n}\n";
		Stree += "\n\n";
		Stree += m_child[2]->TreeToString();
		break;
	}
	case RETURN_STA:
	{
		Stree += "return ";
		//modified by yubin 2015/4/17,以前return的情况只能是变量，现在也可以是表达式，因此输出的时候，输出child0
		Stree += m_child[0]->TreeToString();
		Stree += ";";
		break;
	}
	case DEC_VAR_LIST://参数表达式
	{
		CSyntaxNode* par = this;
		while (par != NULL)
		{
			Stree += par->m_child[0]->TreeToString();
			par = par->m_child[1];
			if (par != NULL)
			{
				Stree += ",";
			}
		}
		break;
	}

	case FUNCP_PARAMETER_EXP:
	{
		if (m_child[0]->m_child[0] != NULL)
		{
			Stree += m_child[0]->m_child[0]->TreeToString();
		}
		else
		{
			Stree += "void ";
		}
		Stree += "(*";
		Stree += m_NodeName;
		Stree += ")";
		Stree += "(";
		Stree += m_child[0]->m_child[1]->TreeToString();
		Stree += ")";
		break;
	}
	case FORMAL_PARAMETER_EXP://参数表达式
	{
		CSyntaxNode* par = this;
		while (par != NULL)
		{

			switch (par->m_ReturnType)
			{
			case INTTYPE:
			{
				Stree += "int ";
				break;
			}
			case FLOATTYPE:
			{
				Stree += "float ";
				break;
			}
			case CHARTYPE:
			{
				Stree += "char ";
				break;
			}
			case UINTTYPE:
			{
				Stree += "unsigned int ";
				break;
			}
			case UCHARTYPE:
			{
				Stree += "unsigned char ";
				break;
			}
			case VOIDTYPE:
			{
				Stree += "void ";
				break;
			}
			case STRUCTTYPE:
			{
				Stree += par->GetNName() + " ";
				break;
			}
			default:
				break;
			}


			Stree += par->m_child[0]->TreeToString();
			par = par->m_child[1];
			if (par != NULL)
			{
				Stree += ",";
			}
		}
		break;
	}

	case TYPE_CAST_STA:
	{
		//要转换为的类型
		Stree += "(";
		Stree += m_child[0]->TreeToString();
		Stree += ")";

		Stree += m_child[1]->TreeToString();

		break;
	}

	case CAST_TYPE:
	{
		switch (m_ReturnType)
		{
		case STRUCTTYPE:
			Stree += m_NodeName;
			break;
		case VOIDTYPE:
			Stree += "void";
			break;
		case INTTYPE:
			Stree += "int";
			break;
		case CHARTYPE:
			Stree += "char";
			break;
		case FLOATTYPE:
			Stree += "float";
			break;
		case FUNCPTYPE:
			Stree += "func p";
			break;
		default:
			//cout << "unknown type!" << endl;
			break;
		}

		Stree += m_child[0]->TreeToString();
		break;
	}

	case NULL_EXP:
	{
		Stree += "NULL";
		break;
	}
	case LST_EXP:
	{
		Stree += "(" + m_child[0]->TreeToString() + ")";
		Stree += "<<";
		Stree += "(" + m_child[1]->TreeToString() + ")";
		break;
	}
	case RST_EXP:
	{
		Stree += "(" + m_child[0]->TreeToString() + ")";
		Stree += ">>";
		Stree += "(" + m_child[1]->TreeToString() + ")";
		break;
	}
	case BAN_EXP:
	{
		Stree += "(" + m_child[0]->TreeToString() + ")";
		Stree += "&";
		Stree += "(" + m_child[1]->TreeToString() + ")";
		break;
	}
	case BOR_EXP:
	{
		Stree += "(" + m_child[0]->TreeToString() + ")";
		Stree += "|";
		Stree += "(" + m_child[1]->TreeToString() + ")";
		break;
	}
	case CON_EXP:
	{
		Stree += "(" + m_child[0]->TreeToString() + ")";
		Stree += "^";
		Stree += "(" + m_child[1]->TreeToString() + ")";
		break;
	}
	case XOR_EXP:
	{
		Stree += "(" + m_child[0]->TreeToString() + ")";
		Stree += "^";
		Stree += "(" + m_child[1]->TreeToString() + ")";
		break;
	}
	case BNE_EXP:
	{
		Stree += "~";
		Stree += "(" + m_child[0]->TreeToString() + ")";
		break;
	}
	case CREATE_THREAD:
	{
		Stree += "CreateThread";
		Stree += "(";
		Stree += m_NodeName + ", " + m_child[0]->TreeToString() + ", ";
		stringstream ss;
		ss << m_Value.iValue;
		Stree += ss.str();
		ss.clear();
		Stree += ")";
		break;
	}
	case SIZEOF_EXP:
	{
		Stree += "sizeof";
		Stree += "(";
		switch (m_ReturnType)
		{
		case STRUCTTYPE:
			Stree += m_NodeName;
			break;
		case VOIDTYPE:
			Stree += "void";
			break;
		case INTTYPE:
			Stree += "int";
			break;
		case CHARTYPE:
			Stree += "char";
			break;
		case FLOATTYPE:
			Stree += "float";
			break;
		case LONGTYPE:
			Stree += "long";
			break;
		case LLONGTYPE:
			Stree += "long long";
			break;
		default:
			cout << "unknown type (1609)" << endl;
			break;
		}
		Stree += m_child[0]->TreeToString();
		break;
	}
	case NODELENGTH:
	{
		Stree += "SetNodeLength(";
		stringstream ss;
		ss << m_Value.iValue;
		Stree += ss.str();
		ss.clear();
		Stree += ")";
		break;
	}
	case LABEL_STA:
	{
		Stree += m_NodeName;
		Stree += ":\n";
		break;
	}
	case FRAME_STA:
	{
		Stree += m_child[1]->TreeToString();
		break;
	}
	case FUNC_RETURN_TYPE:
	{
		Stree += "returntype,";
		Stree += m_child[1]->TreeToString();
		break;
	}
	case ELEMENT_LIST:
	{
		Stree += "{" + m_child[0]->TreeToString() + "}";
		break;
	}
	case ELEMENT:
	{
		Stree += m_child[0]->TreeToString();
		if (m_child[1] != NULL)
		{
			Stree += "," + m_child[1]->TreeToString();
		}

		break;
	}
	//2010-07-15 end summer
	}

	return Stree;
}