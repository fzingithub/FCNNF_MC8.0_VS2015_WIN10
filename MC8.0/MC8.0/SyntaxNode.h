#ifndef SYNTAXNODE_H_INCLUDED
#define SYNTAXNODE_H_INCLUDED

#include "mctypes.h"

#include <string>
#include <vector>

using namespace std;

struct VALUE{
	VALUE():iValue(0),fValue(0.0),cValue(0),sValue(),ST(){}
	int iValue;
	float fValue;
	char cValue;
	string sValue;
	vector<string>  ST;
};

/**
*  程序结点类，用来存储结点数据
*/
class CSyntaxNode
{
public:
	///无参构造函数
	CSyntaxNode();
	/**
	* 构造函数
	* @param1 结点类型
	* @param2 返回值类型
	* @return
	*/
	CSyntaxNode(NODETYPE pnodetype, RETURNTYPE preturntype);
	/**
	* 构造函数
	* @param1 结点类型
	* @param2 结点的child0结点
	* @param3 返回值类型
	* @return
	*/
	CSyntaxNode(NODETYPE pnodetype, CSyntaxNode * p0, RETURNTYPE preturntype);
	/**
	* 构造函数
	* @param1 结点类型
	* @param2 结点的child0结点
	* @param3 结点的child1结点
	* @param4 返回值类型
	* @return
	*/
	CSyntaxNode(NODETYPE pnodetype, CSyntaxNode * p0, CSyntaxNode * p1, RETURNTYPE preturntype);
	/**
	* 构造函数
	* @param1 结点类型
	* @param2 结点的child0结点
	* @param3 结点的child1结点
	* @param4 结点的child2结点
	* @param5 返回值类型
	* @return
	*/
	CSyntaxNode(NODETYPE pnodetype, CSyntaxNode * p0, CSyntaxNode * p1, CSyntaxNode * p2, RETURNTYPE preturntype);

	/**
	* 构造函数
	* @param1 结点类型
	* @param2 结点整形值
	* @param3 返回值类型
	* @return
	*/
	CSyntaxNode(NODETYPE pnodetype, int piValue, RETURNTYPE preturntype);

	/**
	* 构造函数
	* @param1 结点类型
	* @param2 结点的浮点值
	* @param3 返回值类型
	* @return
	*/
	CSyntaxNode(NODETYPE pnodetype, float pfValue, RETURNTYPE preturntype);
	/**
	* 构造函数
	* @param1 结点类型
	* @param2 结点的字符值
	* @param3 返回值类型
	* @return
	*/
	CSyntaxNode(NODETYPE pnodetype, char pcValue, RETURNTYPE preturntype);
	/**
	* 构造函数
	* @param1 结点类型
	* @param2 结点的字符串值
	* @param3 返回值类型
	* @return
	*/
	CSyntaxNode(NODETYPE pnodetype, const std::string &psValue, RETURNTYPE preturntype);
	/**
	* 构造函数
	* @param1 结点类型
	* @param2 结构体名
	* @param3 变量名
	* @param4 返回值类型
	* @return
	*/
	CSyntaxNode(NODETYPE pnodetype, const std::string &pstructname, const std::string pname, RETURNTYPE preturntype);

	/**
	* 构造函数
	* @param1 结点类型
	* @param2 变量名
	* @param3 结点的child0结点
	* @param4 返回值类型
	* @return
	*/
	CSyntaxNode(NODETYPE pnodetype, const std::string &pname, CSyntaxNode * p0, RETURNTYPE preturntype);
	/**
	* 构造函数
	* @param1 结点类型
	* @param2 变量名
	* @param3 结点的child0结点
	* @param4 结点的child1结点
	* @param5 返回值类型
	* @return
	*/
	CSyntaxNode(NODETYPE pndoetype, const std::string &pname, CSyntaxNode * p0, CSyntaxNode * p1, RETURNTYPE preturntype);
	/**
	* 构造函数
	* @param1 结点类型
	* @param2 变量名
	* @param3 结点的child0结点
	* @param4 结点的child1结点
	* @param5 结点的child2结点
	* @param6 返回值类型
	* @return
	*/
	CSyntaxNode(NODETYPE pnodetype, const std::string &pname, CSyntaxNode * p0, CSyntaxNode * p1, CSyntaxNode * p2, RETURNTYPE preturntype);
	/**
	* 构造函数
	* @param1 结点类型
	* @param2 结点整型值
	* @param3 结点的child0结点
	* @param4 结点的child1结点
	* @param5 结点的child2结点
	* @param6 返回值类型
	* @return
	*/
	CSyntaxNode(NODETYPE pnodetype, int piValue, CSyntaxNode * p0, CSyntaxNode * p1, CSyntaxNode * p2, RETURNTYPE preturntype);//add by yubin 2014-6-5

	/**
	* 构造函数
	* @param1 结点类型
	* @param2 返回值类型
	* @param3 非frame符号表
	* @param4 frame符号表
	* @return
	*/
	CSyntaxNode(NODETYPE pnodetype, RETURNTYPE preturntype, vector<string> &ST);

	/**
	* 构造函数
	* @param1 结点类型
	* @param2 返回值类型
	* @param3 非frame符号表
	* @param4 frame符号表
	* @return
	*/
	CSyntaxNode(NODETYPE pnodetype, RETURNTYPE preturntype, string &psValue, vector<string> &ST);

	/**
	* 判断函数是否重名
	* @param 要查找的函数名
	* @return bool
	*/
	bool FindSameName(std::string pname);

	/**
	* 在一棵树中查找另一棵树是否存在
	* @param 查找树 目标树 节点类型
	* @return bool
	*/
	bool FindInTree(CSyntaxNode * ptree, CSyntaxNode* target, NODETYPE ntype);

	/**
	* 拷贝结点
	* @param 待拷贝的结点
	* @return void
	*/
	void CopyNode(CSyntaxNode* pnode);

	/**
	* 拷贝树
	* @param 待拷贝的树的根结点
	* @return void
	*/
	void CopyTree(CSyntaxNode* pnode);

	/**
	* 比较结点是否相同
	* @param 待比较的结点
	* @return bool
	*/
	bool CompareNode(CSyntaxNode* pnode);

	/**
	* 比较树是否相同
	* @param 待比较的结点
	* @return bool
	*/
	bool CompareTree(CSyntaxNode* pnode);


	/**
	* 将树转化为string形式以供输出
	* @param 待比较的结点
	* @return bool
	*/
	std::string TreeToString();

	///得到记载变量名字的节点
	CSyntaxNode* GetNameNode(int &nValue);
	CSyntaxNode* GetNameNode();
	//得到变量名
	std::string GetNName(){
		return m_NodeName;
	}
	//设置变量名
	void  SetNName(const std::string NName){
		m_NodeName = NName;
	}
	//得到结点类型
	NODETYPE GetNType(){
		return m_NodeType;
	}
	//设置结点类型
	void SetNType(NODETYPE NType){
		m_NodeType = NType;
	}
	//得到返回类型
	RETURNTYPE GetRType(){
		return m_ReturnType;
	}
	//设置返回类型
	void SetRType(RETURNTYPE RType){
		m_ReturnType = RType;
	}
	//得到孩子结点0
	CSyntaxNode* GetChild0(){
		return m_child[0];
	}
	//设置孩子结点0
	void SetChild0(CSyntaxNode * Child){
		m_child[0] = Child;
	}
	//得到孩子结点1
	CSyntaxNode* GetChild1(){
		return m_child[1];
	}
	//设置孩子结点1
	void SetChild1(CSyntaxNode * Child){
		m_child[1] = Child;
	}
	//得到孩子结点2
	CSyntaxNode* GetChild2(){
		return m_child[2];
	}
	//设置孩子结点2
	void SetChild2(CSyntaxNode * Child){
		m_child[2] = Child;
	}
	//设置整型值
	void SetiValue(int ivalue){
		m_Value.iValue = ivalue;
	}
	//得到整形值
	int GetiValue(){
		return m_Value.iValue;
	}
	//设置浮点型值
	void SetfValue(float fvalue){
		m_Value.fValue = fvalue;
	}
	//得到浮点型值
	float GetfValue(){
		return m_Value.fValue;
	}

	//设置字符串值
	void SetsValue(std::string svalue){
		m_Value.sValue = svalue;
	}
	//得到字符串值
	std::string GetsValue(){
		return m_Value.sValue;
	}
	//设置字符值
	void SetcValue(char cvalue){
		m_Value.cValue = cvalue;
	}
	//得到字符值
	char GetcValue(){
		return m_Value.cValue;
	}
	//设置符号表
	void SetST(vector<string> &ST){
		m_Value.ST = ST;
	}
	//得到符号表
	vector<string> GetST(){
		return m_Value.ST;
	}
	//得到PreLen的标志位
	bool GetPreLen(){
		return m_PreLen;
	}
	//设置PreLen的标志位
	void SetPreLen(bool PreLen){
		m_PreLen = PreLen;
	}

	// 用于exists，统一将结点的名字换成$_函数名_块名_变量名
	void replaceExistName(std::string origNam, std::string funcNam, std::string blockNam);

	// 统一将结点的名字前面加上$$,用于or的前期处理
	void replaceOrName();

	virtual ~CSyntaxNode();

private:
	VALUE		m_Value;		// 值
	string	    m_NodeName;		// 变量名
	NODETYPE	m_NodeType;		// 结点类型
	RETURNTYPE	m_ReturnType;	// 返回类型
	bool		m_PreLen;		// 是否对Len进行Pre的标志位
	CSyntaxNode	*m_child[3];	// 孩子结点
};

#endif