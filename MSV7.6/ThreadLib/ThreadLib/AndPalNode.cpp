#include "AllNode.h"

//and����ʵ��
AndNode::AndNode(HANDLE handle, DWORD id, Node * node, vector<Node*> vec, HANDLE eve) : Node(handle, id, node, vec, eve)
{
	newType = type = AND;
	//��and�ĺ��ӽ���е�KEEP���������
	/*vector<Node*> keepVec;
	vector<Node*>::iterator it = childs.begin();
	while (it != childs.end())
	{
		Node *node = *it;
		if (node->getNodeType() == KEEP)
		{
			keepVec.push_back(node);
			childs.erase(it);
			continue;
		}
		++it;
	}
	childs.insert(childs.end(), keepVec.begin(), keepVec.end());*/
}
AndNode::AndNode(Node *node) : AndNode(node->getThread(), node->getThreadID(), node->getFatherNode(), node->getChilds(),node->getEvent())
{

}
AndNode::~AndNode()
{

}

void AndNode::excute()
{
	if (isCorrectAndExit() || *pFalse)
	{
		newType = LEAF;
		return;
	}

	unsigned int i = 0;
	while (i<childs.size())
	{
		Node *child = childs[i];
		child->excute();
		//cout << "AND " << childs.size()<<"  "<<Id<<"  " <<child->getThreadID()<<" "<<this<< endl;
		if (child->getNodeType() != child->getNewNodeType())
		{
			childs[i] = getNewNode(child);
			continue;
		}
		++i;
	}
	//cout << "AND end" << endl;
}

bool AndNode::isCorrectAndExit()
{
	//cout << "and isexit() "<<Id << endl;
	vector<Node*>::iterator it = childs.begin();
	int emptyNum = 0, noneNum = 0, size = childs.size();
	int unknownNum = 0;//������䳤��ΪNONE,�ҽ�㲢δ��������ô����������������;�������Ҫ�������keep��alw��

	while (it != childs.end())
	{
		Node *node = *it;
		//cout << "AND begin  " <<node->getNodeType()<<"  "<<node->getThreadID()<<" "<<node<< endl;
		bool exited = node->isCorrectAndExit();
		if (node->getNodeLength() == EMPTY && node->getNodeType() == LEAF)
		{
			++emptyNum;
			hasEmpty = true;
		}
		else if (node->getNodeLength() == NONE)
			++noneNum;
		if (exited && node->getNodeLength() == EMPTY && node->getNodeType() == LEAF)
		{
			delete node;
			childs.erase(it);
			continue;
		}
		++it;
	}
	
	if (!hasEmpty)
	{
		if (noneNum == size)
			len = NONE;
		else
			len = MORE;
		return false;
	}

	if (emptyNum + noneNum == size)
	{
		len = EMPTY;
		return true;
	}
	
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(handle, FOREGROUND_RED | FOREGROUND_INTENSITY);
	cout << "FALSE!" << endl;
	SetConsoleTextAttribute(handle, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);
	//*pFalse = true;
	return false;
}

//and�����뺢��ʱ�������Ӳ��ڵ�һ����keep����λ��
void AndNode::insertChild(Node *child)
{
	childs.insert(getInsertPos(), child);
}

void AndNode::insertChild(vector<Node*> &child)
{
	childs.insert(getInsertPos(), child.begin(),child.end());
}

//����childs�ҵ�����λ��,����������һ����keep��㼴��
vector<Node*>::iterator AndNode::getInsertPos()
{
	vector<Node*>::reverse_iterator rit;
	for (rit = childs.rbegin(); rit != childs.rend(); ++rit)
		if ((*rit)->getNodeType() != KEEP)
			return ++(rit.base());
	
	return childs.begin();
}


//parallel����ʵ��
PalNode::PalNode(HANDLE handle, DWORD id, Node * node, vector<Node*> vec, HANDLE eve) : Node(handle, id, node, vec, eve)
{
	newType = type = PAL;
}

PalNode::PalNode(Node *node) : PalNode(node->getThread(), node->getThreadID(), node->getFatherNode(), node->getChilds(), node->getEvent())
{

}

PalNode::~PalNode()
{

}

void PalNode::excute()
{
	if (isCorrectAndExit() || *pFalse)
	{
		newType = LEAF;
		return;
	}

	vector<Node*>::iterator it = childs.begin();
	//cout << "Pal Start "<< childs.size() << endl;
	while (it != childs.end())
	{
		Node *child = *it;
		//cout << "child excute" << endl;
		child->excute();
		if (child->getNodeType() != child->getNewNodeType())
		{
			*it = getNewNode(child);
			continue;
		}
		++it;
	}
	
	//cout << "Pal End" << endl;

}

bool PalNode::isCorrectAndExit()
{
	vector<Node*>::iterator it = childs.begin();
	int exitNum = 0, emptyNum = 0, size = childs.size();
	while ( it != childs.end() )
	{
		Node *node = *it;
		if (node->isCorrectAndExit() && node->getNodeType() == LEAF)
		{
			if (node->getNodeLength() == EMPTY)
			{
				++emptyNum;
				hasEmpty = true;
			}
			delete node;
			childs.erase(it);
			continue;
		}
		++it;
	}

	if (len == EMPTY)
		return true;

	if (emptyNum == size)
	{
		len = EMPTY;
		return true;
	}
	return false;
}