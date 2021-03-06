#ifndef SYMBOLTABLE_H_
#define SYMBOLTABLE_H_

#include "SyntaxNode.h"
#include "Symbol.h"
#include "mctypes.h"

#include <map>
#include <string>

using namespace std;

class SymbolTable
{
public:

	// 构造函数
	SymbolTable() :m_SymbolMap(){};

	/*
	*是否包含符号
	*@param string(也就是key)
	*return bool
	*/
	bool HasSymbol(string NName);

	/*
	* 向符号表中插入数据
	* @param string (结点名) SymbolType(符号类型)  int(数组长度)
	* return bool
	*/
	bool InstSymbol(string NName, SymbolType type, string SName, bool isArray = false){
		return m_SymbolMap.insert({ NName, new Symbol(type, isArray, SName) }).second;
	}

	/*
	* 向符号表中插入数据
	* @param string (结点名) SymbolType(符号类型)  int(数组长度)
	* return bool
	*/
	bool InstSymbol(string NName, Symbol *symbol){
		return m_SymbolMap.insert({ NName, symbol }).second;
	}

	/*
	* 向符号表中插入frame变量
	* @param string (结点名) SymbolType(符号类型)
	* return bool
	*/
	bool InstFSymbl(string NName);

	/*
	* 查找符号
	* @param string
	* return Symbol*
	*/
	Symbol* FindSymbol(string NName);

	/*
	* 拷贝符号表
	* @param 待拷贝的符号表
	* return void
	*/
	void CopyTable(SymbolTable *pTable);

	/*
	* 删除符号表中所有元素
	*/
	void Clear();

	/*
	* 删除符号表中指定符号
	* @param  string
	*/
	void Delete(string NName);

	/*
	* 删除符号表中指定frame变量
	* @param  string 待删除的变量
	* return 是否成功删除
	*/
	bool DelFrm(string NName);

	map<string,Symbol*>* GetSymbolMap(){
		return &m_SymbolMap;
	}

	/*
	* 检查此类型是否为指针
	*@param 符号类型
	*return bool
	*/
	static bool IsPointer(SymbolType type){
		if (type == SYMBOLNULL || type == SYMBOLPOINTER)
			return true;
		else
			return false;
	}

	/*
	* 判断节点声明的符号类型
	* @param RETURNTYPE
	* return SymbolType
	*/
	static SymbolType GetSymbolType(RETURNTYPE type);
private:
	map<string,Symbol*> m_SymbolMap;
};
#endif