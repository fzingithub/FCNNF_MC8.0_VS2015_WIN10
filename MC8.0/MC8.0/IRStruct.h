#ifndef IRSTRUCT_H_
#define IRSTRUCT_H_

#include <map>
#include <vector>

#include "SyntaxNode.h"
#include "SyntaxTree.h"

#include "llvm/ADT/SmallVector.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Value.h"
#include "llvm/IR/Type.h"

using namespace llvm;
using namespace std;

class IR;
class IRStruct
{
public:
	IRStruct(Module *module, IR *m_IR, string name, CSyntaxNode *pTree);
	/* 只用于生成系统库函数中的结构体*/
	IRStruct(StructType *type, map<string,int> &name_pos, vector<bool> &sign, int size, int basicSize)
		:module(nullptr), m_IR(nullptr), s_type(type), s_isDefined(true), s_name(""), s_size(size), basicSize(basicSize), poss(name_pos), types(), IsSign(sign){}

	~IRStruct(){}

	string GetStructName(){
		return s_name;
	}
	bool isDefined(){
		return s_isDefined;
	}
	StructType* GetStructType(){
		return s_type;
	}
	int GetStructSize(){
		return s_size;
	}
	int GetBasicSize(){
		return basicSize;
	}

	/*
	*Get the element's type by its name
	*@param EleName(the element's name)
	*return false if it's unsigned ,else return true
	*if we cannot find it return false
	*/
	bool GetIsSigned(string EleName);

	/*
	*Get the element's type by its name
	*@param EleName(the element's name)
	*return its Type* if we find it , else return NULL
	*/
	Type* GetElementType(string EleName);

	/*
	 *Get the element's position by its name
	 *@param EleName(the element's name)
	 *return its poision if we find it , else return -1
	*/
	int GetElementPos(string EleName);

	void CalculateSize();
private:
	/*
	*Init the struct
	*@param module(which the struct be defined)  pTree(the defination of struct)
	*/
	void __Init(CSyntaxNode *pTree, bool sign);

	/*
	*change the Syntax tree of this function pointer
	*Let it's name be child0 and parameter be child1 . 
	*Generating a return type by a variable named "Rvalue",if there has not ,the default type is void type
	*The return type should at the beginning of the parameter
	*@param pTree(function pointer's Syntax tree) 
	*/
	void __ChangeFuncpTree(CSyntaxNode *pTree);

private:
	Module *module;
	IR *m_IR;
	StructType *s_type;              // struct's type
	bool s_isDefined;                // whether this struct has been defined successfully 
	string s_name;                   // struct's name
	int s_size;                      // struct's size (Byte)
	int basicSize;                   // basic type's size (Byte)
	map<string,int> poss;            // struct member's name -> member's position
	vector<Type*> types;             // struct member's type in order
	vector<bool> IsSign;             // struct member's sign
};
#endif