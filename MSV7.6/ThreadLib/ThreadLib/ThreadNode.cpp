#include "ThreadNode.h"

volatile int turn = 0;//一个全局变量,用于在线程调度时判断轮到哪一类线程执行
map<DWORD, Node*> threadIDToNode;//全局映射表,线程到存储该线程节点的映射
bool isCopied;//代表该结点中的成员被copied到了另一个节点,设置为true的时候析构函数不会删除动态对象
extern Node* threadRoot;
extern volatile bool flag;
Node::Node(HANDLE handle, DWORD id, Node * node, vector<Node*> vec, HANDLE eve)
	: thread(handle), Id(id), father(node), childs(vec), len(MORE), hasEmpty(false)
{
	map<DWORD, Node*>::value_type ele = make_pair(Id, this);
	if (threadIDToNode.insert(ele).second == false)
	{
		cout << "Error:In Function Node ,Failed to insert element " << endl;
	}
	for (unsigned int i = 0; i < childs.size(); ++i)
		childs[i]->setFatherNode(this);

	if (eve == NULL)
		eventHandle = CreateEvent(NULL, FALSE, FALSE, NULL);
	else
		eventHandle = eve;
}

Node::~Node()
{
	if (!isCopied)
	{
		vector<Node *>::iterator it = childs.begin();
		for (it; it != childs.end(); ++it)
		{
			delete *it;
			*it = NULL;
		}
		childs.clear();
		father = NULL;
		if (!isExit() && this != threadRoot)
		{
			//TerminateThread(thread, 0);
			//WaitForSingleObject(thread, INFINITE);
			flag = true;
			SetEvent(this->eventHandle);
			WaitForSingleObject(thread, INFINITE);
			flag = false;
		}
		threadIDToNode.erase(Id);
		CloseHandle(thread);
		CloseHandle(eventHandle);
		thread = NULL;
	}
}

bool Node::isExit()
{
	if (this->Id == 0)
		return true;
	DWORD code;
	GetExitCodeThread(thread, &code);
	//cout << "code:" << code << endl;
	return (code != STILL_ACTIVE);
}
