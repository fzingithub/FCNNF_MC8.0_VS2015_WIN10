#include "Node.h"

PrjNode::PrjNode(HANDLE handle, DWORD id, Node * node, vector<Node*> vec, HANDLE eve) : Node(handle, id, node, vec, eve)
{
	newType = type = PRJ;
	extQ = true;

}

PrjNode::PrjNode(Node *node) : PrjNode(node->getThread(), node->getThreadID(), node->getFatherNode(), node->getChilds(), node->getEvent())
{

}

PrjNode::~PrjNode()
{

}

void PrjNode::excute()
{
#ifdef BUG
	cerr << "PrjNode execute start." << endl;
#endif
	if (isCorrectAndExit() || *pFalse)
	{
		newType = LEAF;
		return;
	}

	if (childs.size()<=1)
		extQ = true;
	else
	{
		childs[1]->excute();
		while (childs[1]->getNodeType() != childs[1]->getNewNodeType())
		{
			childs[1] = getNewNode(childs[1]);
			childs[1]->excute();
		}
		while (childs[1]->getNodeLength() == EMPTY)
		{
			extQ = true;
			delete childs[1];
			childs.erase((++childs.begin()));
			if (childs.size() == 1)
				break;
			childs[1]->excute();
			while (childs[1]->getNodeType() != childs[1]->getNewNodeType())
			{
				childs[1] = getNewNode(childs[1]);
				childs[1]->excute();
			}
		}
	}
	hasExtQ = extQ;
	if (extQ && !childs[0]->isCorrectAndExit())//Ö´ÐÐQ
	{
		childs[0]->excute();
		while (childs[0]->getNodeType() != childs[0]->getNewNodeType())
		{
			childs[0] = getNewNode(childs[0]);
			childs[0]->excute();
		}
		extQ = false;
	}
}

bool PrjNode::isCorrectAndExit()
{
	vector<Node*>::iterator it = childs.begin()+1;
	int exitNum = 0;
	size_t size = childs.size();
	if (childs[0]->isExit())
		++exitNum;
	for (vector<Node*>::iterator it = ++childs.begin(); it != childs.end(); ++it)
	{
		if ((*it)->getNodeLength() == EMPTY && (*it)->getNodeType() == LEAF)
			++exitNum;
	}

	if (len == EMPTY)
		return true;

	if (exitNum == size)
	{
		len = EMPTY;
		return true;
	}
	return false;
}