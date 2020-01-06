#include "Node.h"
#include "function.h"
#include "modeling.h"

#include <sstream>
#include <algorithm>

extern map<DWORD, Node*> threadIDToNode;//全局映射表,线程到存储该线程节点的映射
extern bool isCopied;//代表该结点中的成员被copied到了另一个节点,设置为true的时候析构函数不会删除动态对象
extern vector<Node*> keepChilds;//keep结点在当前状态产生的孩子,需要确定父节点没有结束才能执行,否则删除

volatile bool flag = false;

int *stateNum = NULL;
void (*GlbOutput)() = NULL;
int (*IsOutput)() = NULL;

int *pFalse;//标志当前路径是否为false
Node *prjNode = NULL;
Node *changedNode = NULL;

Node  *threadRoot = NULL;

HANDLE manager = NULL;
HANDLE managerEvent = NULL;

vector<Element*> $$prsnt = vector<Element*>();

//对当前状态排序的依据，算法类中sort函数的参数之一
//排序的结果是倒序的
int Comp(const Element *a, const Element *b)
{
	if (a->priority > b->priority)
		return true;

	if (a->priority < b->priority) 
		return false;

	if (a->priority != 4) 
		return false;

	if (a->left.empty())
		return false;

	if (b->left.empty())
		return true;

	if (a->right.empty())
		return false;

	if (b->right.empty())
		return true;

	return (find(a->right.begin(), a->right.end(), b->left) != a->right.end());
}

void updateNode(Node *node)
{
	if (node == nullptr || node->getNodeType() == node->getNewNodeType() || node == threadRoot)
		return;
	
	vector<Node*> &childs = node->getFatherNode()->getChilds();
	int i = 0;
	for (; childs[i] != node; ++i);
	Node *temp = childs[i];
	do{
		temp = getNewNode(temp);
		temp->getFatherNode()->getChilds()[i] = temp;
		temp->excute();
	} while (temp->getNodeType() != temp->getNewNodeType());
}

void excuteKeepChilds()
{
	unsigned int i = 0;
	while (i < keepChilds.size())
	{
		Node *node = keepChilds[i];
		Node *andFather = node;
		while (andFather->getFatherNode() != NULL && andFather->getFatherNode()->getNodeType() == AND)//找到keepchilds所在的and的根,不然无法判断结束，因为它自己的父亲状态可能是NONE,也就是取决于其它结点
			andFather = andFather->getFatherNode();

		if (andFather->getNodeLength() == EMPTY)//如果父节点结束了keep的孩子就不执行
		{
			delete node;
			++i;
			continue;
		}

		node->getFatherNode()->getChilds().push_back(node);//将孩子加入到父节点中，参与以后的执行
		Node * temp = node->getFatherNode();
		while (temp != NULL && temp->getNodeLength() != MORE)
		{
			temp->setNodeLength(MORE);
			temp = temp->getFatherNode();
		}
		node->excute();
		updateNode(node);
		updateNode(node->getFatherNode());
		++i;
	}
	keepChilds.clear();
}


//主要分为3步:1.执行线程树,搜集排序使用的信息 2.排序 3.依次执行所有线程(肯定是叶子节点)
unsigned int WINAPI ManagerThread(LPVOID para)
{
	if (threadRoot == NULL)
	{
		cout << "threadRoot is NULL" << endl;
		return 0;
	}

	vector<Element*>::size_type preSize;
	DWORD id;
	bool hasExcuted = false;

	do{
		if (*pFalse || threadRoot->getNewNodeType() == LEAF || threadRoot->getNodeLength() == EMPTY)
		{
			break;
		}
		//输出上一状态

		if (GlbOutput != NULL && IsOutput != NULL && IsOutput())
		{
			cout << "State[" << *stateNum << "]"; 
			GlbOutput(); 
			cout << endl;
		}
		if (stateNum != NULL)
			++(*stateNum);
		//建模时增加的内容
		if (isModeling())
			if ((*pFalse = modelingCurrent()))//不要把赋值改为相等
				break;


		//cout << "Get Info" << threadRoot->getNewNodeType() << endl; //printIdNode();
		threadRoot->excute();//搜集信息
		changedNode = NULL;
		prjNode = NULL;
	SORT:
		//处理被改变的结点
		/*for (int i = 0; i < changedNode.size(); ++i)
		{
		Node *temp = changedNode[i];
		if (temp->getNodeType() != temp->getNewNodeType())
		updateNode(temp);
		else
		{
		temp->excute();
		updateNode(temp);
		}
		}
		changedNode.clear();*/
		//	cout << "prjNode excute" << endl;
		if (prjNode != NULL)
		{
			prjNode->excute();
			prjNode = NULL;
		}
		//cout << "Sort" << endl;
		//printPrsnt();
		//sort($$prsnt.begin(), $$prsnt.end(), Comp);//sort 排序
		preSize = $$prsnt.size();
		// cout << "excute" << endl;
		//printPrsnt();
		while (!$$prsnt.empty() && !*pFalse)//执行经过排序之后的线程
		{
			hasExcuted = true;
			//cout << "really ext Size:" << preSize << endl;
			Element* ele = $$prsnt[preSize - 1];
			//cout << "really ext ID:" << ele->threadId << endl;
			id = ele->threadID;
			$$prsnt.pop_back();
			preSize = $$prsnt.size();
			//cout << "really ext " << preSize << endl; 
			Node * tempNode = threadIDToNode[id];
			if (tempNode == NULL)
			{
				cout << "ThreadIdToNode does not have this thread: " << id << endl;
				continue;
			}
			//cout << "excute " << tempNode << endl; printIdNode(); printPrsnt();
			tempNode->excute();
			//cout << "excute end" << endl; printIdNode();
			updateNode(tempNode);
			if (changedNode != NULL)
			{
				updateNode(changedNode);
				changedNode = NULL;
			}
			delete ele;
			//cout << "end hehe " << $$prsnt.size() << endl; printIdNode();

			if (preSize < $$prsnt.size() || prjNode != NULL)//执行之后加入了新的排序信息，需要重新排序执行
				goto SORT;
		}

		if (*pFalse)
			break;

		threadRoot->isCorrectAndExit();

		if (!keepChilds.empty())
		{
			excuteKeepChilds();
			goto SORT;
		}

		//cout << "next state" << endl; printIdNode();

	} while (true);

	//清理全局变量
	for (map<DWORD, Node*>::iterator it = threadIDToNode.begin(); it != threadIDToNode.end(); ++it)
	{
		turn = USER;
		if (it->second == NULL || it->second == threadRoot)
			continue;
		//TerminateThread(it->second->getThread(), 0);
		//WaitForSingleObject(it->second->getThread(), INFINITE);

		//it->second->setTerm(true);
		flag = true;
		SetEvent(it->second->getEvent());
		WaitForSingleObject(it->second->getThread(), INFINITE);
		flag = false;
		CloseHandle(it->second->getEvent());
	}

	delete threadRoot;
	threadRoot = NULL;

	$$prsnt.clear();
	keepChilds.clear();
	threadIDToNode.clear();
	prjNode = NULL;
	if (*pFalse)
		*stateNum = -1;
	*pFalse = false;

	return 0;
}


//根据node的newType生成一个新类型的结点
//方法:创建新节点对象，将原结点对象的值复制，最后记得修改father结点中的成员:childs，以及删除旧结点
Node* getNewNode(Node *node)
{
	Node *newNode = NULL;
	threadIDToNode.erase(node->getThreadID());
	switch (node->getNewNodeType())
	{
	case LEAF: //变为叶子节点之前要把自己的孩子清空
	{
		vector<Node*> &vec = node->getChilds();
		for (vector<Node*>::iterator it = vec.begin(); it != vec.end(); ++it)
		{
			*it = NULL;
			delete (*it);
		}
		node->getChilds().clear();
		newNode = new LeafNode(node);
		break;
	}
	case AND: newNode = new AndNode(node); break;
	case PAL: newNode = new PalNode(node); break;
	case PRJ: newNode = new PrjNode(node); break;
	case KEEP: newNode = new KeepNode(node); break;
	case ALW: newNode = new AlwNode(node); break;
	default:
		cout << "ERROR: In getNewNode , undefined fathertype:" << node->getNewNodeType() << endl; exit(0);
	}

	isCopied = true;
	//cout << "delete " << node <<node->getNodeType()<< endl;
	//if (node != NULL)
	//delete node; 
	node = newNode;
	isCopied = false;
	return node;
}

extern "C" {
	//当一段并行开始的时候，初始化调度线程，初始化线程树的根节点
	void Init(int type)
	{
		manager = (HANDLE)_beginthreadex(NULL, 0, ManagerThread, NULL, CREATE_SUSPENDED, NULL);
		managerEvent = CreateEvent(NULL, FALSE, FALSE, TEXT(""));
		//cout << "Decl : Handle:" << manager << endl;
		HANDLE mainHandle = NULL;
		DuplicateHandle(GetCurrentProcess(), GetCurrentThread(), GetCurrentProcess(), &mainHandle, 0, FALSE, DUPLICATE_SAME_ACCESS);
		switch (type)
		{
		case 0: threadRoot = new AndNode(mainHandle, GetCurrentThreadId(), NULL);  break;
		case 1: threadRoot = new PalNode(mainHandle, GetCurrentThreadId(), NULL); break;
		case 2: threadRoot = new PrjNode(mainHandle, GetCurrentThreadId(), NULL); break;
			//case 3: threadRoot = new KeepNode(mainHandle, GetCurrentThreadId(), NULL); break;
			//case 4: threadRoot = new AlwNode(mainHandle, GetCurrentThreadId(), NULL);  break;
		default: cout << "ERROR: In Init , undefined fathertype" << endl; exit(0);
		}

		//threadRoot->setNodeType(ROOT);
		//threadRoot->setNewNodeType(ROOT);
		//cout << "Decl : Handle:" << mainHandle << endl;
	}
}

extern "C" {
	//And Pal Prj Keep Alw Manager
	//  0   1   2    3   4       5
	//根据函数入口地址和参数创建线程，并为此线程生成Node*类型的结点，加入线程树中
	void MyCreateThread(unsigned int (WINAPI addr)(LPVOID), LPVOID para, int type)
	{
		/*NODETYPE fatherType;
		switch (type)
		{
		case 0: fatherType = AND; break;
		case 1: fatherType = PAL; break;
		case 2: fatherType = PRJ; break;
		case 3: fatherType = KEEP; break;
		case 4: fatherType = ALW; break;
		case 5:    return;
		default: cout << "ERROR: In MyCreateThread , undefined fathertype" << endl; exit(0);
		}*/
		if (type == 5)
			return;
		if (threadRoot == NULL)
			Init(type);

		Node *father = threadIDToNode[GetCurrentThreadId()];
		father->setNewNodeType(NODETYPE(type));

		if (addr == NULL)
			return;

		//cout << "Begin Create" << endl;
		unsigned int childId;
		HANDLE childHandle = (HANDLE)_beginthreadex(NULL, 16 * 1024, addr, para, STACK_SIZE_PARAM_IS_A_RESERVATION | CREATE_SUSPENDED, &childId);
		if (childHandle == NULL)
		{
			cout << "Cannot create a thread , error :" << errno << endl;
			exit(0);
		}
		//cout << "Decl : Handle:" << childHandle << "  ID:" << childId << endl;
		Node *child = new LeafNode(childHandle, childId, father);
		father->getChilds().push_back(child);
		//cout << "end" << endl;
	}
}

extern "C" {
	//暂停一个线程，之后调度线程会阻塞掉它
	void MyPauseThread(void)
	{	
#ifdef BUG
		cerr << "MyPauseThread start."<< endl;
#endif

		auto it = threadIDToNode.find(GetCurrentThreadId());
		if (it == threadIDToNode.end())
			cerr << "MyPauseThread error" << endl;

		ResetEvent(it->second->getEvent());
		SetEvent(managerEvent);
		if (flag)
			_endthreadex(0);
		WaitForSingleObject(it->second->getEvent(), INFINITE);
		if (flag)
			_endthreadex(0);
	}
}

extern "C" {
	//这个函数只有主调线程最后会调用
	void MyWaitForObject(void)
	{
		static bool hasWaited = false;
		/*if (hasWaited)
		{
		MyPauseThread();
		return;
		}*/

		hasWaited = true;
		turn = MANAGER;
		ResumeThread(manager);
		WaitForSingleObject(manager, INFINITE);
		hasWaited = false;
		flag = false;
		CloseHandle(managerEvent);
		manager = NULL;
		if (isModeling())
			palEnd();

		return;
	}
}
extern "C" {
	void printPrsnt(void)//调试用，用来输出vector $$prsnt
	{
		for (unsigned int i = 0; i < $$prsnt.size(); ++i)
		{
			Element *ele = $$prsnt[i];
			cout << "priority: " << ele->priority;
			cout << " left: " << ele->left << endl;
			cout << "right:";
			for (unsigned int j = 0; j < ele->right.size(); ++j)
				cout << " " << ele->right[j];
			cout << endl;
		}
	}
}

extern "C" {
	void printIdNode(void)
	{
		for (map<DWORD, Node*>::iterator it = threadIDToNode.begin(); it != threadIDToNode.end(); ++it)
			cout << it->first << "  " << it->second << endl;
		cout << endl;
	}
}

extern "C" {
	//搜集信息
	void $$Push(char * left, char * right, int priority)
	{
		Element *ele = new Element();
		//memset(ele, 0, sizeof($$Element));
		//push的时候会复制结构体的内容，所以先push会比较快
		ele->left = string(left);
		string word(right);
		istringstream stream(word);
		//所有的右值是以空格隔开的，把他们拆开存入vector中
		while (stream >> word)
			ele->right.push_back(word);
		ele->priority = priority;
		ele->threadID = GetCurrentThreadId();

		$$prsnt.push_back(ele);
	}
}

extern "C" {
	//获得MSV中的全局变量的信息
	//参数1：状态数 参数2：全局变量输出函数
	void GetMsvVar(int *sNum, void(*output)(), int(*isout)())
	{
		stateNum = sNum;
		GlbOutput = output;
		IsOutput = isout;
		pFalse = new int;
		*pFalse = 0;
		//pFalse = pathFalse;
	}
}

//根据子节点的NODELENGTH 修改当前节点
//返回值用于决定setNodeLength是否需要继续循环下去
void setAndLen(Node *node, NODELENGTH nLen)
{
	while (node != NULL && node->getNodeType() == AND)
	{
		vector<Node*> &childs = node->getChilds();
		int none = 0, emp = 0;
		for (vector<Node*>::iterator it = childs.begin(); it != childs.end(); ++it)
			if ((*it)->getNodeLength() == NONE)
				++none;
			else if ((*it)->getNodeLength() == EMPTY && (*it)->getNodeType() == LEAF)
				++emp;

		if (emp > 0)
			node->setHasEmpty(true);

		if (none == childs.size() && !node->getHasEmpty())
		{
			node->setNodeLength(NONE);
			node = node->getFatherNode();
			continue;
		}

		emp += node->getHasEmpty() ? none : 0;

		if (emp == childs.size())
		{
			node->setNodeLength(EMPTY);
			node->setNewNodeType(LEAF);
			changedNode = node;
			return;
		}

		return;
	}
}

//根据子节点的NODELENGTH 修改当前节点
//返回值用于决定setNodeLength是否需要继续循环下去
void setPalLen(Node *node, NODELENGTH nLen)//PAL结点的孩子结点区间长度不可能为NONE,这个在MSVCopmlier中有控制
{
	vector<Node*> &childs = node->getChilds();
	int none = 0, emp = 0;
	for (vector<Node*>::iterator it = childs.begin(); it != childs.end(); ++it)
		if ((*it)->getNodeLength() == NONE)
			++none;
		else if ((*it)->getNodeLength() == EMPTY && (*it)->getNodeType() == LEAF)
			++emp;

	if (none == childs.size())
	{
		node->setNodeLength(NONE);
		return;
	}


	if (emp + none == childs.size())
	{
		node->setNodeLength(EMPTY);
		node->setNewNodeType(LEAF);
		changedNode = node;
	}

	return;
}

//根据子节点的NODELENGTH 修改当前节点
//将更改过的结点加入changedNode中
void setPrjLen(Node *father, Node *child, NODELENGTH nLen)//PRJ结点的孩子结点区间长度不可能为NONE,这个在MSVCopmlier中有控制
{
	vector<Node*> &childs = father->getChilds();
	vector<Node*>::iterator pos;
	int sum = 0;
	for (vector<Node*>::iterator it = childs.begin(); it != childs.end(); ++it)
	{
		if (*it == child)
			pos = it + 1;

		if ((*it)->getNodeLength() != MORE && (*it)->getNodeType() == LEAF)
			++sum;
	}

	if (sum == childs.size())//投影结点孩子的区间长度全部为empty
	{
		father->setNodeLength(EMPTY);
		father->setNewNodeType(LEAF);
		return;
	}

	if (childs[0] == child)//说明Q结点置为empty,
		return;

	if (pos == childs.end())
		return;
	if (!((PrjNode*)father)->getHasExtQ())
	{
		prjNode = father;//将下一个p的第一状态取出
	}

}

extern "C" {
	//设置线程树中结点的区间长度，由于more是默认的，因此参数值为enmpty或none
	//根据区间长度修改父节点的长度，投影为empty时要执行下一个p的第一个状态
	void setNodeLength(int type)
	{
		Node * node = threadIDToNode[GetCurrentThreadId()];
		//if (node->getNodeLength() != MORE)
		//return;
		node->setNodeLength(NODELENGTH(type));
		Node * temp = node->getFatherNode();
		if (temp != NULL)
		{
			switch (temp->getNodeType())
			{
			case AND:  setAndLen(temp, NODELENGTH(type)); break;
			case PAL:  setPalLen(temp, NODELENGTH(type)); break;
			case PRJ: setPrjLen(temp, node, NODELENGTH(type)); break;
			default:  break;
			}
		}

		SetEvent(managerEvent);
		_endthreadex(0);
	}
}
