#include "Symbol.h"
#include "function.h"
#include "SyntaxNode.h"

#include <iostream>
#include <string>
#include <sstream>

using namespace std;

int g_namecount=0;

Symbol::Symbol():m_SymbolType(UNDEFINE), m_isArray(false), m_nFrm(0), m_name(""), m_ArrayNum(), m_AddrNum(0){}

Symbol::Symbol(SymbolType type):m_SymbolType(type), m_isArray(false), m_nFrm(0), m_name(""), m_ArrayNum(), m_AddrNum(0){}

Symbol::Symbol(SymbolType type, bool isArray):m_SymbolType(type), m_isArray(isArray), m_nFrm(0), m_name(""), m_ArrayNum(), m_AddrNum(0){}

Symbol::Symbol(SymbolType type, string name):m_SymbolType(type), m_isArray(false), m_nFrm(0), m_name(name), m_ArrayNum(), m_AddrNum(0){}

Symbol::Symbol(SymbolType type, int AddrNum, string name) :m_SymbolType(type), m_isArray(false), m_nFrm(0), m_name(name), m_ArrayNum(), m_AddrNum(AddrNum){}

Symbol::Symbol(SymbolType type, string name, int nFrm) :m_SymbolType(type), m_isArray(false), m_nFrm(nFrm), m_name(name), m_ArrayNum(), m_AddrNum(0){}

Symbol::Symbol(SymbolType type, bool isArray, string name) : m_SymbolType(type), m_isArray(isArray), m_nFrm(0), m_name(name), m_ArrayNum(0), m_AddrNum(0){}

Symbol::Symbol(SymbolType type, bool isArray, string name, vector<int> &ArrayNum) : m_SymbolType(type), m_isArray(isArray), m_nFrm(0), m_name(name), m_ArrayNum(ArrayNum), m_AddrNum(0){}

Symbol::Symbol(SymbolType type, bool isArray, string name, vector <int> &ArrayNum, int AddrNum) :m_SymbolType(type), m_isArray(isArray), m_nFrm(0), m_name(name), m_ArrayNum(ArrayNum), m_AddrNum(AddrNum){}


/*
* 拷贝符号函数 考虑数据区域的copy  复制变量
*@param 待拷贝的符号
*return void
*/
void Symbol::Copy(Symbol *pSymbl)
{
	if (pSymbl == nullptr)
		return;

	m_SymbolType = pSymbl->GetSymbolType();
	m_isArray = pSymbl->IsArray();
	m_nFrm = pSymbl->GetNFrm();
	m_ArrayNum = pSymbl->GetArrayNum();
	m_name = pSymbl->GetSName();
	m_AddrNum = pSymbl->GetAddrNum();
}


/*
* 输出符号转换成字符串
*@param 根据不同类型对应的printf里边的类型，如int对应"%d"
*return void
*/
string Symbol::__PrntFixSym(string flag, string name, string nmspc)
{
	string result = "printf(";
	if (!m_isArray){
		result += "\"" + name + "=" + flag + " \","+ nmspc+name;
	}
	else{
		if (m_ArrayNum.empty())
			return "";
		
		auto iter = m_ArrayNum.begin();
		if (iter + 1 == m_ArrayNum.end()){
			result += "\"" + name + "[";
			result += to_string(*iter) + "]";
			result += "= [";

			string tmp = "";
			for (int i = 0; i < *iter; i++){
				result += flag + ",";
				tmp += "," +nmspc+ name + "[";
				tmp += to_string(i);
				tmp += "]";
			}
			result = result.substr(0, (result.length() - 1));
			result += "]\"";
			result += tmp;
		}
		else{
			auto iter2 = m_ArrayNum.begin() + 1;

			result += "\"" + name + "[";
			result += to_string(*iter2) + ",";
			result += to_string(*iter2) +"]";
			result += "= [";

			string tmp = "";
			for (int i = 0; i < *iter; i++){
				result += "[";
				for (int j = 0; j < *iter2; j++){
					result+= flag + ",";
					tmp += "," + nmspc + name;
					tmp += "[" + to_string(i) + "]";
					tmp += "[" + to_string(j) + "]";
				}
				result = result.substr(0, (result.length() - 1));
				result += "],";
				
			}
			result = result.substr(0, (result.length() - 1));
			result += "]\"";
			result += tmp;
		}
	}
	result += ");\n";
	return result;
}

//输出符号转换成字符串
string Symbol::PrntSym(string name, string nmspc)
{
	switch (m_SymbolType){
	case UNDEFINE:				{ return ""; }
	case SYMBOLVOID:			{ return ""; }
	case SYMBOLBOOL:			{ return __PrntFixSym("%d", name, nmspc); }
	case SYMBOLCHAR:			{ return __PrntFixSym("%c", name, nmspc); }
	case SYMBOLUCHAR:			{ return __PrntFixSym("%c", name, nmspc); }
	case SYMBOLSHORT:			{ return __PrntFixSym("%d", name, nmspc); }
	case SYMBOLUSHORT:			{ return __PrntFixSym("%u", name, nmspc); }
	case SYMBOLINT:				{ return __PrntFixSym("%d", name, nmspc); }
	case SYMBOLUINT:			{ return __PrntFixSym("%u", name, nmspc); }
	case SYMBOLLONG:			{ return __PrntFixSym("%ld", name, nmspc); }
	case SYMBOLULONG:			{ return __PrntFixSym("%lu", name, nmspc); }
	case SYMBOLLLONG:			{ return __PrntFixSym("%lld", name, nmspc); } // lld I64d
	case SYMBOLULLONG:			{ return __PrntFixSym("%llu", name, nmspc); } // llu I64u
	case SYMBOLFLOAT:			{ return __PrntFixSym("%f", name, nmspc); }
	case SYMBOLDOUBLE:			{ return __PrntFixSym("%f", name, nmspc); }
	case SYMBOLPOINTER:
	case SYMBOLFUNCP:			{ return __PrntFixSym("%p", name, nmspc); }
	//case SYMBOLCHARP:			{ return __PrntFixSym("%s", name, nmspc); }
	//case SYMBOLUCHARP:		{ return __PrntFixSym("%s", name, nmspc); }
	case SYMBOLCRITICALSECTION: { return __PrntFixSym("%p", name, nmspc); }
	default: 
		return "";
	}
}

// 根据节点类型得到返回值类型
RETURNTYPE Symbol::GetReturnType()
{
	switch (m_SymbolType){
	case UNDEFINE:     			{ return UNKNOWNTYPE; }
	case SYMBOLVOID:			{ return VOIDTYPE; }
	case SYMBOLBOOL:			{ return BOOLTYPE; }
	case SYMBOLCHAR:			{ return CHARTYPE; }
	//case SYMBOLCHARP:			{ return CHARTYPE;}
	case SYMBOLUCHAR:			{ return UCHARTYPE; }
	//case SYMBOLUCHARP:		{ return UCHARTYPE; }
	case SYMBOLSHORT:     		{ return SHORTTYPE; }
	case SYMBOLUSHORT:	 		{ return USHORTTYPE; }
	case SYMBOLINT:    			{ return INTTYPE; }
	case SYMBOLUINT:			{ return UINTTYPE; }
	case SYMBOLLONG:     		{ return LONGTYPE; }
	case SYMBOLULONG:			{ return ULONGTYPE; }
	case SYMBOLLLONG:     		{ return LLONGTYPE; }
	case SYMBOLULLONG:			{ return ULLONGTYPE; }
	case SYMBOLFLOAT:     		{ return FLOATTYPE; }
	case SYMBOLDOUBLE:			{ return DOUBLETYPE; }
	case SYMBOLSTRUCT:			{ return STRUCTTYPE; }
	case SYMBOLPOINTER:			{ return POINTERTYPE; };
	case SYMBOLLIST:			{ return LISTTYPE; }
	case SYMBOLFUNCP:			{ return FUNCPTYPE; }
	case SYMBOLFILE:			{ return FILETYPE; }
	case SYMBOLSTR:				{ return STRTYPE; }
	case SYMBOLCRITICALSECTION:	{ return CRITICAL_SECTION_TYPE; }
	case SYMBOLTIMECAPS:		{ return TIMECAPS_TYPE; }
	default:
		return UNKNOWNTYPE;
	}
}

/*
* 将符号转成变量定义语法树
*@param 变量名
*return 生成的语法树
*/
CSyntaxNode* Symbol::GetDecTree(string &name, int nValue)
{
	RETURNTYPE RType = GetReturnType();
	if (RType == UNKNOWNTYPE || name == "")
		return nullptr;
	
	CSyntaxNode *decNode = new CSyntaxNode(DECLARE_STA, m_name, nullptr, RType);
	CSyntaxNode *visit = new CSyntaxNode(DEC_VAR_LIST, VOIDTYPE);
	decNode->SetChild0(visit);

	for (int i = 0; i < m_AddrNum - nValue; i++){
		visit->SetChild0(new CSyntaxNode(POINTER_EXP, VOIDTYPE));
		visit = visit->GetChild0();		
	}

	name += "$" + to_string(g_namecount);
	g_namecount++;

	visit->SetChild0(new CSyntaxNode(IDENT_EXP, name, nullptr, VOIDTYPE));

	return decNode;	
}

RETURNTYPE GetUnsignedType(RETURNTYPE type){
	RETURNTYPE result = type;
	switch (type){
	case CHARTYPE:
		result = UCHARTYPE;
		break;
	case SHORTTYPE:
		result = USHORTTYPE;
		break;
	case INTTYPE:
		result = UINTTYPE;
		break;
	case LONGTYPE:
		result = ULONGTYPE;
		break;
	case LLONGTYPE:
		result = ULLONGTYPE;
		break;
	default:
		cout << "WANNING: RETURNTYPE: " << type << " doesn't have unsigned type." << endl;
	}
	return type;
}