
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

enum NODELENGTH//��㵱ǰ�����䳤��
{
	MORE,//���ȴ��ڵ���1
	EMPTY,//���䳤��Ϊ0
	NONE//����������������
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

	//�жϽ���е��߳��Ƿ��Ѿ�����
	bool isExit();

	//ִ�е�ǰ���,��ͬ�Ľ���в�ͬ��ʵ�ַ�ʽ
	virtual void excute() = 0;
	//�жϽ���Ƿ���ȷ��ִ��,������ֱ��exit,ȫ����������true,���򷵻�false
	//���жϵ�ͬʱ���Ѿ��������ӽڵ�ɾ��
	virtual bool isCorrectAndExit() = 0;



protected:
	HANDLE thread;//�����������߳�
	HANDLE eventHandle;
	DWORD Id;//��ǰ�̵߳�ID
	Node* father;//���ڵ�
	vector<Node*> childs;//���ӽڵ�ļ���
	NODETYPE type;//�������
	NODETYPE newType;//����ʱ���ı�Ľ������
	NODELENGTH len;//�������䳤��
	bool hasEmpty;//�Ƿ��е���empty�ĺ���
};

extern volatile int turn;//һ��ȫ�ֱ���,�������̵߳���ʱ�ж��ֵ���һ���߳�ִ��
extern map<DWORD, Node*> threadIDToNode;//ȫ��ӳ���,�̵߳��洢���߳̽ڵ��ӳ��
extern bool isCopied;//����ý���еĳ�Ա��copied������һ���ڵ�,����Ϊtrue��ʱ��������������ɾ����̬����

extern Node* getNewNode(Node *node);
extern bool *pFalse;//�жϵ�ǰ·���Ƿ�Ϊfalse
extern HANDLE managerEvent;
#endif