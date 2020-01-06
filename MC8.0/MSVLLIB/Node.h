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

//��㵱ǰ�����䳤��
enum NODELENGTH 
{
	MORE = 0,	// ���ȴ��ڵ���1
	EMPTY = 1,	// ���䳤��Ϊ0
	NONE= 2		// ����������������
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

	//�жϽ���е��߳��Ƿ��Ѿ�����
	bool isExit();

	//ִ�е�ǰ���,��ͬ�Ľ���в�ͬ��ʵ�ַ�ʽ
	virtual void excute() = 0;

	//�жϽ���Ƿ���ȷ��ִ��,������ֱ��exit,ȫ����������true,���򷵻�false
	//���жϵ�ͬʱ���Ѿ��������ӽڵ�ɾ��
	virtual bool isCorrectAndExit() = 0;


protected:
	DWORD  id = 0;                  // ��ǰ�̵߳�ID
	HANDLE thread = nullptr;        // �����������߳�
	HANDLE eventHandle = nullptr;
	
	Node       *father = nullptr;   // ���ڵ�
	vector<Node*> childs; // ���ӽڵ�ļ���
	NODETYPE   type;      // �������
	NODETYPE   newType;   // ����ʱ���ı�Ľ������
	NODELENGTH len;       // �������䳤��
	bool       hasEmpty;  // �Ƿ��е���empty�ĺ���
};

extern volatile int turn;//һ��ȫ�ֱ���,�������̵߳���ʱ�ж��ֵ���һ���߳�ִ��
extern map<DWORD, Node*> threadIDToNode;//ȫ��ӳ���,�̵߳��洢���߳̽ڵ��ӳ��
extern bool isCopied;//����ý���еĳ�Ա��copied������һ���ڵ�,����Ϊtrue��ʱ��������������ɾ����̬����

extern Node* getNewNode(Node *node);
extern int *pFalse;//�жϵ�ǰ·���Ƿ�Ϊfalse
extern HANDLE managerEvent;
extern vector<Node*> keepChilds;//keep����ڵ�ǰ״̬�����ĺ���,��Ҫȷ�����ڵ�û�н�������ִ��,����ɾ��

class AndNode :public Node
{
public:
	AndNode(HANDLE handle, DWORD id, Node *node, vector<Node*> vec = vector<Node*>(), HANDLE eve = nullptr);
	AndNode(Node *node);
	~AndNode() {};

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
{//ͶӰ��childs[0]����ʽ�е�q
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
	bool extQ;//�Ƿ�ִ��Q
	bool hasExtQ;//��ǰ״̬�Ƿ��Ѿ�����Q
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