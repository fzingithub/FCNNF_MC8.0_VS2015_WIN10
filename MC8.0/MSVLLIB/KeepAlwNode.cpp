
#include "Node.h"

vector<Node*> keepChilds;

//keep��alw�ĸ��ڵ�ض�Ϊand���

//keep ���
KeepNode::KeepNode(HANDLE handle, DWORD id, Node * node, vector<Node*> vec, HANDLE eve) : Node(handle, id, node, vec, eve)
{
	type = newType = KEEP;
	len = NONE;
	father->setNodeLength(NONE);
}

KeepNode::KeepNode(Node *node) : KeepNode(node->getThread(), node->getThreadID(), node->getFatherNode(), node->getChilds(), node->getEvent())
{

}

KeepNode::~KeepNode()
{

}

void KeepNode::excute()
{
#ifdef BUG
	cerr << "KeepNode execute start." << endl;
#endif

	if (isExit() || *pFalse)
		return;

	/*turn = USER;
	//cout << "Resume User" << endl;
	//cout << "Handle:" << thread << "  ID:" << Id << endl;
	ResumeThread(thread);
	while (turn != MANAGER)
	{
		if (isExit())
		{
			turn = MANAGER;
			break;
		}
		SwitchToThread();
	}
	//cout << "Suspend User" << endl;
	SuspendThread(thread);*/

	if (ResumeThread(thread) <= 0)
	{
		SetEvent(eventHandle);
	}
	WaitForSingleObject(managerEvent, INFINITE);

	//ִ�й��󽫲������߳̽����뵽ȫ�ֱ�����,�������ִ��,�Ὣ��Щ�����뵽keep�ĸ��ڵ���
	for (vector<Node*>::iterator it = childs.begin(); it != childs.end(); ++it)
		(*it)->setFatherNode(this->father);

	keepChilds.insert(keepChilds.end(),childs.begin(),childs.end());
	childs.clear();
}

bool KeepNode::isCorrectAndExit()
{
	return isExit();
}

//alw���
AlwNode::AlwNode(HANDLE handle, DWORD id, Node * node, vector<Node*> vec, HANDLE eve) : Node(handle, id, node, vec, eve)
{
	type = newType = ALW;
	len = NONE;
	father->setNodeLength(NONE);
}

AlwNode::AlwNode(Node *node) : AlwNode(node->getThread(), node->getThreadID(), node->getFatherNode(), node->getChilds(),node->getEvent())
{

}

AlwNode::~AlwNode()
{

}

void AlwNode::excute()
{
#ifdef BUG
	cerr << "AlwNode execute start." << endl;
#endif
	if (isExit() || *pFalse)
		return;

	/*turn = USER;
	ResumeThread(thread);
	while (turn != MANAGER)
	{
		if (isExit())
		{
			turn = MANAGER;
			break;
		}
		SwitchToThread();
	}
	SuspendThread(thread);*/
	
	if (ResumeThread(thread) <= 0)
	{
		SetEvent(eventHandle);
	}
	WaitForSingleObject(managerEvent, INFINITE);

	//���������߳̽����뵽alw�ĸ�����
	for (vector<Node*>::iterator it = childs.begin(); it != childs.end(); ++it)
		(*it)->setFatherNode(this->father);

	father->getChilds().insert(father->getChilds().end(), childs.begin(), childs.end());
	if (childs.size() > 0)
	{
		Node * temp = father;
		while (temp != NULL && temp->getNodeLength() != MORE)
		{
			temp->setNodeLength(MORE);
			temp = temp->getFatherNode();
		}
	}
	childs.clear();
}

bool AlwNode::isCorrectAndExit()
{
	return isExit();
}