#ifndef SYMBOL_H_
#define SYMBOL_H_

#include "mctypes.h"

#include <string>
#include <vector>
using std::string;
using std::vector;

RETURNTYPE GetUnsignedType(RETURNTYPE type);

class CSyntaxNode;

class Symbol
{
public:
	//构造函数
	Symbol();

	/*
	*构造函数
	*@param 符号类型
	*/
	Symbol(SymbolType type);

	/*
	* 构造函数
	*@param 符号类型
	*@param 数组长度
	*/
	Symbol(SymbolType type, bool isArray);

	/*
	* 构造函数
	*@param 符号类型
	*@param 结构体类型名
	*/
	Symbol(SymbolType type, string name);

	/*
	* 构造函数
	*@param 符号类型
	*@param 结构体类型名
	*@param 指针重数
	*/
	Symbol(SymbolType type,int AddrNum, string name);

	/*
	* 构造函数
	*@param 符号类型
	*@param 结构体类型名
	*@param 被frame次数
	*/
	Symbol(SymbolType type, string name, int nFrm);

	/*
	* 构造函数
	*@param 符号类型
	*@param 数组长度
	*@param 结构体类型名
	*/
	Symbol(SymbolType type, bool isArray, string name);

	/*
	* 构造函数
	*@param 符号类型
	*@param 是否是数组
	*@param 结构体类型名
	*@param 数组长度
	*/
	Symbol(SymbolType type, bool isArray, string name, vector<int> &ArrayNum);

	/*
	* 构造函数
	*@param 符号类型
	*@param 是否是数组
	*@param 结构体类型名
	*@param 数组长度
	*@param 指针重数
	*/
	Symbol(SymbolType type, bool isArray, string name, vector<int> &ArrayNum, int AddrNum);

	/*
	* 拷贝符号函数
	*@param 待拷贝的符号
	*return void
	*/
	void Copy(Symbol *psb);

	/*
	* 输出符号转换成字符串
	*@param 变量名
	*return void
	*/
	string PrntSym(string name,string nmspc="");

	/*
	* 将符号转成变量定义语法树
	*@param 变量名
	*return 生成的语法树
	*/
	CSyntaxNode* GetDecTree(string& name,int nValue);

	// 获得符号类型
	SymbolType GetSymbolType(){
		return m_SymbolType;
	}

	// 获得数组长度
	bool IsArray(){
		return m_isArray;
	}

	//设置符号类型
	void SetSymbolType(SymbolType type){
		m_SymbolType = type;
	}

	//获得结构体类型名
	string GetSName(){
		return  m_name;
	}

	// 设置结构体类型名
	void SetSName(string name){
		m_name = name;
	}

	//设置数组长度
	void SetIsArray(bool isArray){
		m_isArray = isArray;
	}

	// 设置变量frame的次数
	void SetNFrm(int num){
		m_nFrm = num;
	}

	///得到变量frame的次数
	int GetNFrm(){
		return m_nFrm;
	}

	void SetArrayNum(vector <int>* n){
		m_ArrayNum = *n;
	}

	vector<int>& GetArrayNum(){
		return m_ArrayNum;
	}

	void SetAddrNum(int AddrNum){
		m_AddrNum = AddrNum;
	}

	int GetAddrNum(){
		return m_AddrNum;
	}

private:
	// 根据节点类型得到返回值类型
	RETURNTYPE GetReturnType();

	/*
	* 输出符号转换成字符串
	*@param1 根据不同类型对应的printf里边的类型，如int对应"%d"
	*@param2 变量名
	*return void
	*/
	string __PrntFixSym(string flag, string name,string nmspc="");

private:
	SymbolType   m_SymbolType;
	bool		 m_isArray;    // 是否为数组
	int          m_nFrm;       // 变量frame的次数
	string		 m_name;       // 结构体类型名
	vector<int>  m_ArrayNum;
	int          m_AddrNum;
};
#endif