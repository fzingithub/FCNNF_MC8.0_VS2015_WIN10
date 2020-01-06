
#ifndef _THREADNODE_H
#define _THREADNODE_H

#include <iostream>
#include <vector>
#include <map>
#include <windows.h>
#include <process.h>

using namespace std;

#define EXITCODE 0
#define USER 0
#define MANAGER 1

enum NODELENGTH//结点当前的区间长度
{
	MORE,//长度大于等于1
	EMPTY,//区间长度为0
	NONE//长度由其它结点决定
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
	Node(HANDLE handle, DWORD id, Node * node, vector<Node*> vec = vector<Node*>(), HANDLE eve = NULL);
	virtual ~Node();

	inline HANDLE getThread()
	{
		return thread;
	}
	inline void setThread(HANDLE handle)
	{
		thread = handle;
	}
	inline HANDLE getEvent()
	{
		return eventHandle;
	}
	inline DWORD getThreadID()
	{
		return Id;
	}
	inline void setThreadID(DWORD threadId)
	{
		Id = threadId;
	}
	inline Node *getFatherNode()
	{
		return father;
	}
	inline void setFatherNode(Node * node)
	{
		father = node;
	}
	inline NODETYPE getNodeType()
	{
		return type;
	}
	inline void setNodeType(NODETYPE nType)
	{
		type = nType;
	}
	inline NODETYPE getNewNodeType()
	{
		return newType;
	}
	inline void setNewNodeType(NODETYPE nType)
	{
		newType = nType;
	}
	inline  void addChild(Node* node)
	{
		childs.push_back(node);
	}
	inline vector<Node*>& getChilds()
	{
		return childs;
	}

	inline void setNodeLength(NODELENGTH nType)
	{
		len = nType;
	}
	inline NODELENGTH getNodeLength()
	{
		return len;
	}

	inline bool getHasEmpty()
	{
		return hasEmpty;
	}

	inline void setHasEmpty(bool empty)
	{
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
	HANDLE thread;//结点所代表的线程
	HANDLE eventHandle;
	DWORD Id;//当前线程的ID
	Node* father;//父节点
	vector<Node*> childs;//孩子节点的集合
	NODETYPE type;//结点类型
	NODETYPE newType;//运行时被改变的结点类型
	NODELENGTH len;//结点的区间长度
	bool hasEmpty;//是否有到达empty的孩子
};

extern volatile int turn;//一个全局变量,用于在线程调度时判断轮到哪一类线程执行
extern map<DWORD, Node*> threadIDToNode;//全局映射表,线程到存储该线程节点的映射
extern bool isCopied;//代表该结点中的成员被copied到了另一个节点,设置为true的时候析构函数不会删除动态对象

extern Node* getNewNode(Node *node);
extern bool *pFalse;//判断当前路径是否为false
extern HANDLE managerEvent;
#endif