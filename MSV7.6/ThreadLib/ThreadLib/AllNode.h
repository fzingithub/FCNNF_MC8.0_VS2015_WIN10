
#ifndef _ALLNODE_H
#define _ALLNODE_H

#include "ThreadNode.h"

extern vector<Node*> keepChilds;//keep结点在当前状态产生的孩子,需要确定父节点没有结束才能执行,否则删除

class AndNode :public Node
{
public:
	AndNode(HANDLE handle, DWORD id, Node * node, vector<Node*> vec = vector<Node*>(), HANDLE eve = NULL);
	AndNode(Node *node);
	~AndNode();

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
	PalNode(HANDLE handle, DWORD id, Node * node, vector<Node*> vec = vector<Node*>(), HANDLE eve = NULL);
	PalNode(Node *node);
	~PalNode();

	void excute();
	bool isCorrectAndExit();
};

class LeafNode :public Node
{
public:
	LeafNode(HANDLE handle, DWORD id, Node * node, vector<Node*> vec = vector<Node*>(), HANDLE eve = NULL);
	LeafNode(Node *node);
	~LeafNode();

	void excute();
	bool isCorrectAndExit();
};

class PrjNode :public Node
{//投影中childs[0]代表公式中的q
public:
	PrjNode(HANDLE handle, DWORD id, Node * node, vector<Node*> vec = vector<Node*>(), HANDLE eve = NULL);
	PrjNode(Node *node);
	~PrjNode();

	inline bool getHasExtQ()
	{
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
	KeepNode(HANDLE handle, DWORD id, Node * node, vector<Node*> vec = vector<Node*>(), HANDLE eve = NULL);
	KeepNode(Node *node);
	~KeepNode();

	void excute();
	bool isCorrectAndExit();
};

class AlwNode :public Node
{
public:
	AlwNode(HANDLE handle, DWORD id, Node * node, vector<Node*> vec = vector<Node*>(), HANDLE eve = NULL);
	AlwNode(Node *node);
	~AlwNode();

	void excute();
	bool isCorrectAndExit();
};


#endif