
#include "AllNode.h"

vector<Node*> keepChilds;

//keep和alw的父节点必定为and结点

//keep 结点
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

	//执行过后将产生的线程结点加入到全局变量中,如果可以执行,会将这些结点加入到keep的父节点中
	for (vector<Node*>::iterator it = childs.begin(); it != childs.end(); ++it)
		(*it)->setFatherNode(this->father);

	keepChilds.insert(keepChilds.end(),childs.begin(),childs.end());
	childs.clear();
}

bool KeepNode::isCorrectAndExit()
{
	return isExit();
}

//alw结点
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

	//将产生的线程结点加入到alw的父亲中
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