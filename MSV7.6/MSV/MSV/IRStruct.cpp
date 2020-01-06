#include "IRStruct.h"
#include "IR.h"
#include <iostream>

 IRStruct::IRStruct(Module *module, IR *ir, string name, CSyntaxNode *pTree) 
	 :module(module), m_IR(ir), s_type(nullptr), s_isDefined(true), s_name(name), s_size(0), basicSize(1), poss(), types(), IsSign()
{
	auto uit = IR::udef.find(s_name);
	if (uit == IR::udef.end()){
		s_type = StructType::create(module->getContext(), s_name);
		IR::udef.insert({ name, s_type });
		uit = IR::udef.find(s_name);
	}
	else
		s_type = uit->second;

	///a loop to define the variable of struct
	CSyntaxNode *visit = pTree;
	while (s_isDefined && visit != nullptr){
		switch (visit->GetNType()){
		case AND_EXP:{
			if (visit->GetChild0()->GetNType() == UNSIGN_DECLARATION_STA)
				__Init(visit->GetChild0()->GetChild0(), false);
			else
				__Init(visit->GetChild0(), true);
			visit = visit->GetChild1();
			break;
		}
		case DECLARE_STA:{ // 结构体的最后一个元素声明
			__Init(visit, true);
			visit = nullptr;
			break;
		}
		default:
			cerr << "ERROR: invalid struct define statement." << endl;
			s_isDefined = false;
		}
	}
	s_type->setBody(types);
	if (uit != IR::udef.end())
		IR::udef.erase(uit);
}

/*
*Init the struct
*@param module(which the struct be defined)  pTree(the defination of struct)
*/
void IRStruct::__Init(CSyntaxNode *pTree, bool sign)
{
	if (pTree != nullptr && pTree->GetRType() == FUNCPTYPE){
		__ChangeFuncpTree(pTree->GetChild0());
	}

	Type *type = m_IR->GetType(pTree);
	if (type == nullptr){
		if (pTree->GetRType() != STRUCTTYPE){
			cerr << "ERROR : unknownn struct element type." << endl;
			s_isDefined = false;
			return;
		}
		
		string name = pTree->GetNName();
		
		if (pTree->GetChild0()->GetChild0()->GetNType() != POINTER_EXP && name == s_name){
			cerr << "ERROR: cannot use struct " << name << " to define itself." << endl;
			s_isDefined = false;
			return;
		}

		if (pTree->GetChild0()->GetChild0()->GetNType() != POINTER_EXP && name != s_name){
			cerr << "ERROR: reference undefined struct " << name << endl;
			s_isDefined = false;
			return;
		}

		auto uit = IR::udef.find(pTree->GetNName());
		if (uit != IR::udef.end())
			type = uit->second;
		else{
			type = StructType::create(module->getContext(), name);
			IR::udef.insert({ name, (StructType*)type });
		}
	}

	CSyntaxNode *visit = pTree->GetChild0();
	while (s_isDefined && visit != nullptr){
		VarInfo vInfo = m_IR->__GetVarInfo(type, visit);
		if (poss.insert({ vInfo.name, types.size() }).second == false){
			cout << "ERROR: element " << vInfo.name << " has already been defined in struct: " << s_name << endl;
			s_isDefined = false;
		}
		Type *t = vInfo.type;
		types.push_back(t);
		IsSign.push_back(IR::prsnt_sign);

		if (t != nullptr){
			IRStruct *s = nullptr;
			if (t->isStructTy() && (s = IR::GetIRStruct(t->getStructName())) != nullptr){
				int ssize = s->GetBasicSize();
				basicSize = ssize > basicSize ? ssize : basicSize;
			}
			else if (t->isIntegerTy(8))
				basicSize = 1 > basicSize ? 1 : basicSize;
			else if (t->isIntegerTy(16))
				basicSize = 2 > basicSize ? 2 : basicSize;
			else if (t->isIntegerTy(32) || t->isFloatTy())
				basicSize = 4 > basicSize ? 4 : basicSize;
			else if (t->isPointerTy())
				basicSize = 4 > basicSize ? 4 : basicSize;
			else if (t->isIntegerTy(64) || t->isDoubleTy())
				basicSize = 8 > basicSize ? 8 : basicSize;
		}
		if (pTree->GetRType() == FUNCPTYPE) // function pointer 节点的Child1是函数类型，不为空，所以要提前返回。
			return;
		visit = visit->GetChild1();
	}
}

/*
*change the Syntax tree of this function pointer
*Let it's name be child0 and parameter be child1 .
*Generating a return type by a variable named "Rvalue",if there has not ,the default type is void type
*The return type should at the beginning of the parameter
*@param pTree(function pointer's Syntax tree)
*/
void IRStruct::__ChangeFuncpTree(CSyntaxNode *pTree)
{
	CSyntaxNode *extNode = new CSyntaxNode(FORMAL_PARAMETER_EXP, new CSyntaxNode(IDENT_EXP, "$$Ext", nullptr, VOIDTYPE), INTTYPE);
	extNode->SetChild1(pTree->GetChild1());
	pTree->SetChild1(extNode); // 将$$Ext变量添加到形参列表的最开始

	bool existRValue = false;  // 语法树中是否找到变量RValue
	// 函数的形参列表
	CSyntaxNode *paraList = pTree->GetChild1(); 
	while (paraList->GetChild1() != nullptr){
		auto curPara = paraList->GetChild1();
		CSyntaxNode *temp = curPara->GetChild0();
		if (temp->GetNType() == FUNCP_PARAMETER_EXP)
			__ChangeFuncpTree(temp);

		bool isRValue = false;
		while (temp != nullptr){
			if (temp->GetNName() == "RValue"){
				isRValue = true;
				break;
			}
			temp = temp->GetChild0();
		}

		if (isRValue && (curPara->GetChild1() == nullptr)){
			CSyntaxNode *retNode = new CSyntaxNode(FUNC_RETURN_TYPE, paraList->GetChild1(), VOIDTYPE);
			paraList->SetChild1(NULL);

			retNode->SetChild1(pTree->GetChild1());
			pTree->SetChild1(retNode);

			existRValue = true;
			break;
		}
		else if (isRValue && (paraList->GetChild1()->GetChild1() != nullptr)){
			cerr << "ERROR: In function pointer " << pTree->GetNName() << " definition, RValue must be the last parameter." << endl;
			return;
		}

		paraList = paraList->GetChild1();
	}

	if (!existRValue){
		CSyntaxNode *retNode = new CSyntaxNode(FUNC_RETURN_TYPE, VOIDTYPE);
		retNode->SetChild1(pTree->GetChild1());
		pTree->SetChild1(retNode);
	}
}

/*
*Get the element's type by its name
*@param EleName(the element's name)
*return false if it's unsigned ,else return true
*if we cannot find it return false
*/
bool IRStruct::GetIsSigned(string EleName)
{
	int pos = GetElementPos(EleName);
	if (pos == -1)
		return false;
	else
		return IsSign.at(pos);
}

/*
*Get the element's type by its name
*@param EleName(the element's name)
*return its Type* if we find it , else return NULL
*/
Type* IRStruct::GetElementType(string EleName)
{
	int pos = GetElementPos(EleName);
	if (pos == -1)
		return nullptr;
	else
		return types.at(pos);
}

/*
*Get the element's position by its name
*@param EleName(the element's name)
*return its poision if we find it , else return -1
*/
int IRStruct::GetElementPos(string EleName)
{
	auto it = poss.find(EleName);
	if (it == poss.end())
		return -1;
	else
		return it->second;
}

// Calculate size of the struct
void IRStruct::CalculateSize()
{
	if (module == nullptr)
		return;
	s_size = m_IR->getDataLayout()->getTypeAllocSize(s_type);
}