#include "AllNode.h"
#include "function.h"
#include <sstream>
#include <algorithm>
#include "modeling.h"
//#include <istringstream>

//extern int turn;//һ��ȫ�ֱ���,�������̵߳���ʱ�ж��ֵ���һ���߳�ִ��
extern map<DWORD, Node*> threadIDToNode;//ȫ��ӳ���,�̵߳��洢���߳̽ڵ��ӳ��
extern bool isCopied;//����ý���еĳ�Ա��copied������һ���ڵ�,����Ϊtrue��ʱ��������������ɾ����̬����
extern vector<Node*> keepChilds;//keep����ڵ�ǰ״̬�����ĺ���,��Ҫȷ�����ڵ�û�н�������ִ��,����ɾ��

volatile bool flag = false;
int * stateNum = NULL;
void(*GlbOutput)() = NULL;
bool(*IsOutput)() = NULL;
bool *pFalse;//��־��ǰ·���Ƿ�Ϊfalse
Node *prjNode = NULL;
Node *changedNode = NULL;
HANDLE manager = NULL;
Node* threadRoot = NULL;
HANDLE managerEvent = NULL;
vector<$$Element*> $$prsnt = vector<$$Element*>();

//�Ե�ǰ״̬��������ݣ��㷨����sort�����Ĳ���֮һ
//����Ľ���ǵ����
bool Comp(const $$Element *a, const $$Element *b)
{
	if (a->priority > b->priority) return true;
	if (a->priority < b->priority) return false;
	if (a->priority != 4) return false;
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

void updateNode(Node * node)
{
	//cout << "Update!" << endl;
	if (node == NULL || node->getNodeType() == node->getNewNodeType() || node == threadRoot)
		return;
	//cout << "Update1!" << endl;
	vector<Node*> &childs = node->getFatherNode()->getChilds();
	int i = 0;
	for (; childs[i] != node; ++i);
	Node * temp = childs[i];
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
		while (andFather->getFatherNode() != NULL&&andFather->getFatherNode()->getNodeType() == AND)//�ҵ�keepchilds���ڵ�and�ĸ�,��Ȼ�޷��жϽ�������Ϊ���Լ��ĸ���״̬������NONE,Ҳ����ȡ�����������
			andFather = andFather->getFatherNode();

		if (andFather->getNodeLength() == EMPTY)//������ڵ������keep�ĺ��ӾͲ�ִ��
		{
			delete node;
			++i;
			continue;
		}

		node->getFatherNode()->getChilds().push_back(node);//�����Ӽ��뵽���ڵ��У������Ժ��ִ��
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

//��Ҫ��Ϊ3��:1.ִ���߳���,�Ѽ�����ʹ�õ���Ϣ 2.���� 3.����ִ�������߳�(�϶���Ҷ�ӽڵ�)
unsigned int WINAPI ManagerThread(LPVOID para)
{
	if (threadRoot == NULL)
	{
		cout << "threadRoot is NULL" << endl;
		return 0;
	}

	vector<$$Element*>::size_type preSize;
	DWORD id;
	bool hasExcuted = false;
	do{
		if (*pFalse || threadRoot->getNewNodeType() == LEAF || threadRoot->getNodeLength() == EMPTY)
		{
			break;
		}
		//�����һ״̬

		if (GlbOutput != NULL && IsOutput != NULL && IsOutput())
		{
			cout << "State[" << *stateNum << "]";
			GlbOutput();
			cout << endl;
		}
		if (stateNum != NULL)
			++(*stateNum);
		//��ģʱ���ӵ�����
		if (isModeling())
			if ((*pFalse = modelingCurrent()))//��Ҫ�Ѹ�ֵ��Ϊ���
				break;


		//cout << "Get Info" << threadRoot->getNewNodeType() << endl; printIdNode();
		threadRoot->excute();//�Ѽ���Ϣ
		//cout << "End Get Info" <<endl ;
		changedNode = NULL;
		prjNode = NULL;
	SORT:
		//�����ı�Ľ��
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
		//sort($$prsnt.begin(), $$prsnt.end(), Comp);//sort ����
		preSize = $$prsnt.size();
		// cout << "excute" << endl;
		//printPrsnt();
		while (!$$prsnt.empty() && !*pFalse)//ִ�о�������֮����߳�
		{
			hasExcuted = true;
			//cout << "really ext Size:" << preSize << endl;
			$$Element* ele = $$prsnt[preSize - 1];
			//cout << "really ext ID:" << ele->threadId << endl;
			id = ele->threadId;
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

			if (preSize < $$prsnt.size() || prjNode != NULL)//ִ��֮��������µ�������Ϣ����Ҫ��������ִ��
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

	//����ȫ�ֱ���
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


//����node��newType����һ�������͵Ľ��
//����:�����½ڵ���󣬽�ԭ�������ֵ���ƣ����ǵ��޸�father����еĳ�Ա:childs���Լ�ɾ���ɽ��
Node* getNewNode(Node *node)
{
	Node *newNode = NULL;
	threadIDToNode.erase(node->getThreadID());
	switch (node->getNewNodeType())
	{
	case LEAF: //��ΪҶ�ӽڵ�֮ǰҪ���Լ��ĺ������
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

//��һ�β��п�ʼ��ʱ�򣬳�ʼ�������̣߳���ʼ���߳����ĸ��ڵ�
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

//And Pal Prj Keep Alw Manager
//  0   1   2    3   4       5
//���ݺ�����ڵ�ַ�Ͳ��������̣߳���Ϊ���߳�����Node*���͵Ľ�㣬�����߳�����
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
}

//��ͣһ���̣߳�֮������̻߳���������
void MyPauseThread()
{
	/*//cout << "User Pause" << endl ;
	turn = MANAGER;
	while (turn != USER)
	{
		SwitchToThread();
	}
	if (exited)
	{
		turn = MANAGER;
		ExitThread(0);
	}
	fprintf(fp, "pause end %d \n", GetCurrentThreadId());
	*/
		//printf( "pause begin %d\n", GetCurrentThreadId());	
	auto it = threadIDToNode.find(GetCurrentThreadId());
	if (it == threadIDToNode.end())
		cout << "MyPauseThread error" << endl;
	ResetEvent(it->second->getEvent());
	SetEvent(managerEvent);
	if (flag)
		_endthreadex(0);
	WaitForSingleObject(it->second->getEvent(), INFINITE);
	if (flag)
		_endthreadex(0);
}

//�������ֻ�������߳��������
void MyWaitForObject()
{
	static bool hasWaited = false;
	/*if (hasWaited)
	{
	MyPauseThread();
	return;
	}*/

	hasWaited = true;
	turn = MANAGER;
	//cout << "Resume  Manger!"<<endl;
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

void printPrsnt()//�����ã��������vector $$prsnt
{
	for (unsigned int i = 0; i < $$prsnt.size(); ++i)
	{
		$$Element * ele = $$prsnt[i];
		cout << "priority: " << ele->priority;
		cout << " left: " << ele->left << endl;
		cout << "right:";
		for (unsigned int j = 0; j < ele->right.size(); ++j)
			cout << " " << ele->right[j];
		cout << endl;
	}
}

void printIdNode()
{
	for (map<DWORD, Node*>::iterator it = threadIDToNode.begin(); it != threadIDToNode.end(); ++it)
		cout << it->first << "  " << it->second << endl;
	cout << endl;
}

//�Ѽ���Ϣ
void $$Push(char * left, char * right, int priority)
{
	$$Element *ele = new $$Element();
	//memset(ele, 0, sizeof($$Element));
	//push��ʱ��Ḵ�ƽṹ������ݣ�������push��ȽϿ�
	ele->left = string(left);
	string word(right);
	istringstream stream(word);
	//���е���ֵ���Կո�����ģ������ǲ𿪴���vector��
	while (stream >> word)
		ele->right.push_back(word);
	ele->priority = priority;
	ele->threadId = GetCurrentThreadId();
	//cout << "Push left:  "<<left<<" right :"<< right << "id:" << ele->threadId << endl;
	//cout <<  "id:" << ele->threadId << endl;
	$$prsnt.push_back(ele);
}


//���MSV�е�ȫ�ֱ�������Ϣ
//����1��״̬�� ����2��ȫ�ֱ����������
void GetMsvVar(int * sNum, void(*output)(), bool(*isout)())
{
	stateNum = sNum;
	GlbOutput = output;
	IsOutput = isout;
	pFalse = (bool*)malloc(sizeof(bool));
	*pFalse = false;
	//pFalse = pathFalse;
}

//�����ӽڵ��NODELENGTH �޸ĵ�ǰ�ڵ�
//����ֵ���ھ���setNodeLength�Ƿ���Ҫ����ѭ����ȥ
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

//�����ӽڵ��NODELENGTH �޸ĵ�ǰ�ڵ�
//����ֵ���ھ���setNodeLength�Ƿ���Ҫ����ѭ����ȥ
void setPalLen(Node *node, NODELENGTH nLen)//PAL���ĺ��ӽ�����䳤�Ȳ�����ΪNONE,�����MSVCopmlier���п���
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

//�����ӽڵ��NODELENGTH �޸ĵ�ǰ�ڵ�
//�����Ĺ��Ľ�����changedNode��
void setPrjLen(Node *father, Node *child, NODELENGTH nLen)//PRJ���ĺ��ӽ�����䳤�Ȳ�����ΪNONE,�����MSVCopmlier���п���
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

	if (sum == childs.size())//ͶӰ��㺢�ӵ����䳤��ȫ��Ϊempty
	{
		father->setNodeLength(EMPTY);
		father->setNewNodeType(LEAF);
		return;
	}

	if (childs[0] == child)//˵��Q�����Ϊempty,
		return;

	if (pos == childs.end())
		return;
	if (!((PrjNode*)father)->getHasExtQ())
	{
		prjNode = father;//����һ��p�ĵ�һ״̬ȡ��
	}

}

//�����߳����н������䳤�ȣ�����more��Ĭ�ϵģ���˲���ֵΪenmpty��none
//�������䳤���޸ĸ��ڵ�ĳ��ȣ�ͶӰΪemptyʱҪִ����һ��p�ĵ�һ��״̬
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