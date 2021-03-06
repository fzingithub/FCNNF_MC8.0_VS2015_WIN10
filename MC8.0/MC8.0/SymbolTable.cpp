#include "SymbolTable.h"
#include "function.h"

#include <string>
#include <iostream>

using namespace std;

/*
*是否包含符号
*@param string(也就是key)
*return bool
*/
bool SymbolTable::HasSymbol(string NName){
	return (m_SymbolMap.find(NName) != m_SymbolMap.end());
}

/*
* 向符号表中插入数据
* @param string (结点名) SymbolType(符号类型)  int(数组长度)
* return bool
*/
bool SymbolTable::InstFSymbl(string NName){
	Symbol *symbol = FindSymbol(NName);
	if (symbol != nullptr){
		symbol->SetNFrm(symbol->GetNFrm() + 1);
		return true;
	}
	return m_SymbolMap.insert({ NName, new Symbol(UNDEFINE) }).second;
}

/*
* 判断节点声明的符号类型
* @param RETURNTYPE
* return SymbolType
*/
SymbolType SymbolTable::GetSymbolType(RETURNTYPE type){
	switch (type){
	case VOIDTYPE:     			{ return SYMBOLVOID; }
	case BOOLTYPE:     			{ return SYMBOLBOOL; }
	case CHARTYPE:	 			{ return SYMBOLCHAR; }
	case UCHARTYPE:   			{ return SYMBOLUCHAR; }
	case SHORTTYPE:    			{ return SYMBOLSHORT; }
	case USHORTTYPE:			{ return SYMBOLUSHORT; }
	case INTTYPE:				{ return SYMBOLINT; }
	case UINTTYPE:				{ return SYMBOLUINT; }
	case LONGTYPE:				{ return SYMBOLLONG; }
	case ULONGTYPE:				{ return SYMBOLULONG; }
	case LLONGTYPE:				{ return SYMBOLLLONG; }
	case ULLONGTYPE:     		{ return SYMBOLULLONG; }
	case FLOATTYPE:     		{ return SYMBOLFLOAT; }
	case DOUBLETYPE:	 		{ return SYMBOLDOUBLE; }
	case STRUCTTYPE:   			{ return SYMBOLSTRUCT; }
	case LISTTYPE:		 		{ return SYMBOLLIST; }			
	case FUNCPTYPE:				{ return SYMBOLFUNCP;}
	case STRTYPE:				{ return SYMBOLSTR; }
	case FILETYPE:				{ return SYMBOLFILE; }
	case CRITICAL_SECTION_TYPE:	{return SYMBOLCRITICALSECTION; }
	case TIMECAPS_TYPE:			{return SYMBOLTIMECAPS; }				   
	default:
		return UNDEFINE;
	}
}

/*
* 查找符号
* @param string
* return Symbol*
*/
Symbol* SymbolTable::FindSymbol(string NName){
	auto it = m_SymbolMap.find(NName);
	if (it == m_SymbolMap.end())
		return nullptr;
	return it->second;
}

/*
* 删除符号表中所有元素
*/
void SymbolTable::Clear(){
	for (auto it = m_SymbolMap.begin(); it != m_SymbolMap.end(); it++)
		delete it->second;
	m_SymbolMap.clear();
}

/*
* 删除符号表中指定符号
* @param  string
*/
void SymbolTable::Delete(string NName){
	Symbol *symbol = FindSymbol(NName);
	if (symbol != nullptr){
		delete symbol;
		m_SymbolMap.erase(NName);
	}
}

/*
* 删除符号表中指定frame变量
* @param  string 待删除的变量
* return void
*/
bool SymbolTable::DelFrm(string NName){
	Symbol *s = FindSymbol(NName);
	if (s != nullptr){
		int i = s->GetNFrm();
		if (i == 0){
			delete s;
			m_SymbolMap.erase(NName);
		}
		else if (i > 0){
			s->SetNFrm(i - 1);
		}
		else{
			cerr << "Delete frame symbol error." << endl;
			return false;
		}
		return true;
	}
	//else{
	//	cerr << "Symbol " << NName << " does not exist!" << endl;
	//	return false;
	//}
}

/*
* 拷贝符号表
* @param 待拷贝的符号表
* return void
*/
void SymbolTable::CopyTable(SymbolTable *pTable){
	Clear();
	for (auto it = pTable->m_SymbolMap.begin(); it != pTable->m_SymbolMap.end(); it++){
		string name = it->first;
		Symbol *symbol = new Symbol();
		symbol->Copy(it->second);
		m_SymbolMap.insert({ name, symbol });
	}
}