#include "AllNode.h"

//叶子结点的实现
LeafNode::LeafNode(HANDLE handle, DWORD id, Node * node, vector<Node*> vec, HANDLE eve) : Node(handle, id, node, vec, eve)
{
	newType = type = LEAF;
}

LeafNode::LeafNode(Node *node) : LeafNode(node->getThread(), node->getThreadID(), node->getFatherNode(), node->getChilds(), node->getEvent())
{
}

LeafNode::~LeafNode()
{
}

void LeafNode::excute()
{
	//cout << "LEAF begin  "<<Id << endl;
	if (isExit() || *pFalse || len)
		return;
	//cout << "LEAF Begin" << endl;
	/*
	turn = USER;
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
	if (SuspendThread(thread) == -1)
	cout << "suspend error!" << endl;
	//cout << "LEAF End" << endl;
	*/
	/*if (firstTime)
	{
	ResumeThread(thread);
	firstTime = false;
	}
	else
	{
	SetEvent(eventHandle);
	}*/
	//cout << "LEAF Begin " << Id<< endl;
	if (ResumeThread(thread) <= 0)
	{
		//cout << "set event " << Id << "   "<<i << endl;
		SetEvent(eventHandle);
	}
	//cout << "set event end " << Id << endl;
	WaitForSingleObject(managerEvent, INFINITE);
	//cout << "LEAF End " << Id << endl;
	return;
}

bool LeafNode::isCorrectAndExit()
{
	return isExit() || len;
}