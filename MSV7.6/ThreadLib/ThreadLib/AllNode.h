
#ifndef _ALLNODE_H
#define _ALLNODE_H

#include "ThreadNode.h"

extern vector<Node*> keepChilds;//keep����ڵ�ǰ״̬�����ĺ���,��Ҫȷ�����ڵ�û�н�������ִ��,����ɾ��

class AndNode :public Node
{
public:
	AndNode(HANDLE handle, DWORD id, Node * node, vector<Node*> vec = vector<Node*>(), HANDLE eve = NULL);
	AndNode(Node *node);
	~AndNode();

	void excute();
	bool isCorrectAndExit();
	void insertChild(Node *child);//and�����뺢��ʱ�������Ӳ��ڵ�һ����keep����λ��
	void insertChild(vector<Node*> &child);
private:
	vector<Node*>::iterator getInsertPos();//����childs�ҵ�����λ��,����������һ����keep��㼴��
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
{//ͶӰ��childs[0]����ʽ�е�q
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
	bool extQ;//�Ƿ�ִ��Q
	bool hasExtQ;//��ǰ״̬�Ƿ��Ѿ�����Q
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