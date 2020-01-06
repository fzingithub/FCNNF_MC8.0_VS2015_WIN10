#ifndef _THREADNODE_H
#define _THREADNODE_H

//#define BUG

#include <iostream>
#include <vector>
#include <map>
#include <windows.h>
#include <process.h>

using namespace std;

#define EXITCODE 0
#define USER 0
#define MANAGER 1

//结点当前的区间长度
enum NODELENGTH 
{
	MORE = 0,	// 长度大于等于1
	EMPTY = 1,	// 区间长度为0
	NONE= 2		// 长度由其它结点决定
};

enum NODETYPE
{
	AND,
	PAL,
	PRJ,
	KEEP,
	ALW,
	LEAF,
	ROOT
};

class Node{
public:
	Node(){}
	Node(HANDLE handle, DWORD id, Node *node, vector<Node*> vec = vector<Node*>(), HANDLE eve = nullptr);
	virtual ~Node();

	HANDLE getThread(){
		return thread;
	}
	
	void setThread(HANDLE handle){
		thread = handle;
	}
	
	HANDLE getEvent(){
		return eventHandle;
	}

	DWORD getThreadID(){
		return id;
	}
	
	void setThreadID(DWORD threadId){
		id = threadId;
	}

	Node* getFatherNode(){
		return father;
	}
	
	void setFatherNode(Node * node){
		father = node;
	}
	
	NODETYPE getNodeType(){
		return type;
	}

	void setNodeType(NODETYPE nType){
		type = nType;
	}

	NODETYPE getNewNodeType(){
		return newType;
	}

	void setNewNodeType(NODETYPE nType){
		newType = nType;
	}

	void addChild(Node* node){
		childs.push_back(node);
	}
	
	vector<Node*>& getChilds(){
		return childs;
	}

	void setNodeLength(NODELENGTH nType){
		len = nType;
	}

	NODELENGTH getNodeLength(){
		return len;
	}

	bool getHasEmpty(){
		return hasEmpty;
	}

	void setHasEmpty(bool empty){
		hasEmpty = empty;
	}

	//判断结点中的线程是否已经结束
	bool isExit();

	//执行当前结点,不同的结点有不同的实现方式
	virtual void excute() = 0;

	//判断结点是否正确的执行,有问题直接exit,全部结束返回true,否则返回false
	//在判断的同时将已经结束的子节点删除
	virtual bool isCorrectAndExit() = 0;


protected:
	DWORD  id = 0;                  // 当前线程的ID
	HANDLE thread = nullptr;        // 结点所代表的线程
	HANDLE eventHandle = nullptr;
	
	Node       *father = nullptr;   // 父节点
	vector<Node*> childs; // 孩子节点的集合
	NODETYPE   type;      // 结点类型
	NODETYPE   newType;   // 运行时被改变的结点类型
	NODELENGTH len;       // 结点的区间长度
	bool       hasEmpty;  // 是否有到达empty的孩子
};

extern volatile int turn;//一个全局变量,用于在线程调度时判断轮到哪一类线程执行
extern map<DWORD, Node*> threadIDToNode;//全局映射表,线程到存储该线程节点的映射
extern bool isCopied;//代表该结点中的成员被copied到了另一个节点,设置为true的时候析构函数不会删除动态对象

extern Node* getNewNode(Node *node);
extern int *pFalse;//判断当前路径是否为false
extern HANDLE managerEvent;
extern vector<Node*> keepChilds;//keep结点在当前状态产生的孩子,需要确定父节点没有结束才能执行,否则删除

class AndNode :public Node
{
public:
	AndNode(HANDLE handle, DWORD id, Node *node, vector<Node*> vec = vector<Node*>(), HANDLE eve = nullptr);
	AndNode(Node *node);
	~AndNode() {};

	void excute();
	bool isCorrectAndExit();
	void insertChild(Node *child);//and结点插入孩子时，将孩子插在第一个非keep结点的位置
	void insertChild(vector<Node*> &child);
private:
	vector<Node*>::iterator getInsertPos();//根据childs找到插入位置,逆向查找最后一个非keep结点即可
};

class PalNode :public Node
{
public:
	PalNode(HANDLE handle, DWORD id, Node * node, vector<Node*> vec = vector<Node*>(), HANDLE eve = nullptr);
	PalNode(Node *node);
	~PalNode();

	void excute();
	bool isCorrectAndExit();
};

class LeafNode :public Node
{
public:
	LeafNode(HANDLE handle, DWORD id, Node * node, vector<Node*> vec = vector<Node*>(), HANDLE eve = nullptr);
	LeafNode(Node *node);
	~LeafNode();

	void excute();
	bool isCorrectAndExit();
};

class PrjNode :public Node
{//投影中childs[0]代表公式中的q
public:
	PrjNode(HANDLE handle, DWORD id, Node * node, vector<Node*> vec = vector<Node*>(), HANDLE eve = nullptr);
	PrjNode(Node *node);
	~PrjNode();

	bool getHasExtQ(){
		return hasExtQ;
	}

	void excute();

	bool isCorrectAndExit();

private:
	bool extQ;//是否执行Q
	bool hasExtQ;//当前状态是否已经加入Q
};

class KeepNode :public Node
{
public:
	KeepNode(HANDLE handle, DWORD id, Node * node, vector<Node*> vec = vector<Node*>(), HANDLE eve = nullptr);
	KeepNode(Node *node);
	~KeepNode();

	void excute();
	bool isCorrectAndExit();
};

class AlwNode :public Node
{
public:
	AlwNode(HANDLE handle, DWORD id, Node * node, vector<Node*> vec = vector<Node*>(), HANDLE eve = nullptr);
	AlwNode(Node *node);
	~AlwNode();

	void excute();
	bool isCorrectAndExit();
};

#endif