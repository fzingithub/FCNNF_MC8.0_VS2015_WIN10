#include "Node.h"

//一个全局变量,用于在线程调度时判断轮到哪一类线程执行
volatile int turn = 0;

//全局映射表,线程到存储该线程节点的映射
map<DWORD, Node*> threadIDToNode;

//代表该结点中的成员被copied到了另一个节点,设置为true的时候析构函数不会删除动态对象
bool isCopied;
extern Node *threadRoot;
extern volatile bool flag;

Node::Node(HANDLE handle, DWORD id, Node * node, vector<Node*> vec, HANDLE eve): id(id), thread(handle), father(node), childs(vec), len(MORE), hasEmpty(false)
{
	if (threadIDToNode.insert({id, this}).second == false) {
		cerr << "ERROR: In Function Node ,Failed to insert element " << endl;
	}

	for (size_t i = 0; i < childs.size(); ++i)
		childs[i]->setFatherNode(this);

	if (eve == nullptr)
		eventHandle = CreateEvent(NULL, FALSE, FALSE, NULL);
	else
		eventHandle = eve;
}

Node::~Node()
{
#ifdef BUG
	cerr << "~Node start." << endl;
#endif

	if (!isCopied){
#ifdef BUG
		cerr << "~Node if start." << endl;
#endif
		for (auto it = childs.begin(); it != childs.end(); ++it){
			delete *it;
			*it = nullptr;
		}

#ifdef BUG
		cerr << "~Node if middle = 1." << endl;
#endif
		childs.clear();
		father = nullptr; // father->childs 还存有this

		if (!isExit() && this != threadRoot){
			//TerminateThread(thread, 0);
			//WaitForSingleObject(thread, INFINITE);
			flag = true;
			SetEvent(this->eventHandle);
			WaitForSingleObject(thread, INFINITE);
			flag = false;
		}

#ifdef BUG
		cerr << "~Node if middle = 2." << endl;
#endif

		threadIDToNode.erase(id);
		CloseHandle(thread);
		CloseHandle(eventHandle);
		thread = nullptr;
	}

#ifdef BUG
	cerr << "~Node end." << endl;
#endif
}

bool Node::isExit()
{
#ifdef BUG
	cerr << "Node isExit id = " << id << endl;
#endif
	if (this->id == 0)
		return true;

	DWORD code;
	GetExitCodeThread(thread, &code);
#ifdef BUG
	cerr << "Node isExit code = " << code << " rtn = " << (code != STILL_ACTIVE)  << endl;
#endif
	return (code != STILL_ACTIVE);
}
