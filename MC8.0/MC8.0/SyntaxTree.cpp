/*******************************************************************************
*@file     SyntaxTree.cpp
*@author
*@remarks  Modified by
*@version  1.0
*@section  Brief
*Modified: 2013/04/22  16:21
*
*
*
*Copyright (C) ICTT.,XiDian univ
*All rights reserved.
*
*******************************************************************************/


#include "function.h"
//#include "MSV.h"
#include "SyntaxTree.h"
//#include "Graph.h"
#include <iostream>
#include<string>
#include <sstream>

using namespace std;
//add by wm 2013.04.18
//extern CGraph* g_Graph;


//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------
//定义全局变量

//用来保存__NfChopStar的辅助函数__NfChopStar_Next2Chop函数中转化后的nextchop树
CSyntaxNode *NextChopStarTree = new CSyntaxNode(TRUE_EXP, VOIDTYPE);

//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------


CSyntaxTree::CSyntaxTree()
{
	m_Root = NULL;

	//add by YY 2013/06/17 10:12
	m_NextTree = NULL;
	m_EmptyTree = NULL;
}

CSyntaxTree::CSyntaxTree(CSyntaxNode* pRoot)
{
	m_Root = pRoot;

	//add by YY 2013/06/17 10:12
	m_NextTree = NULL;
	m_EmptyTree = NULL;
}

//add by YY 2013/08/22 9:47 将pTree加到m_PrgmRemains树上
bool CSyntaxTree::MergeTree(CSyntaxNode *pTree, NODETYPE NType, RETURNTYPE RType)
{
	if (pTree == NULL)
	{
		cout << "CSyntaxTree::MergeTree error!" << endl;
		return false;
	}
	//  [/8/27/2013   by CJ]

	//优化速度 annotate by YY 2013/12/05 16:35
	/*	if (pTree->GetNType()==PROJECTION_STA && pTree->GetChild2()==NULL) //将ptree 变成chop
	{
	pTree->SetNType(CHOP_STA);
	CSyntaxNode *temp,*temp1;
	temp=pTree;
	while(temp->GetChild1()!=NULL)
	{
	temp->GetChild1()->SetNType(CHOP_STA);
	temp=temp->GetChild1();
	} //最底下的chop的 右孩子为空，用做孩子替换temp

	temp1= new CSyntaxNode();
	temp1->CopyTree(temp->GetChild0());
	temp->CopyTree(temp1);

	delete temp1;
	temp1=NULL;
	temp=NULL;
	}
	//end of added by cj
	*/
	//add by YY 2013/08/22 9:57 m_PrgmRemains一定不为空，m_Root有可能为空
	if (m_Root == NULL)
	{
		m_Root = pTree;//优化速度 YY 2013/12/05 16:36
		return true;
	}
	else
	{
		//add by YY 2013/08/22 9:49 不知道可不可以这样直接改变this的指向，减少copytree
		CSyntaxNode *temp = new CSyntaxNode(NType, RType);
		temp->SetChild0(m_Root);
		temp->SetChild1(pTree);//优化速度 YY 2013/12/05 16:37
		m_Root = temp;
		temp = NULL;
		return true;
	}
}

bool CSyntaxTree::MergeTree_2(CSyntaxNode *pTree, NODETYPE NType, RETURNTYPE RType)
{
	if (pTree == NULL)
	{
		cout << "CSyntaxTree::MergeTree error!" << endl;
		return false;
	}
	//  [/8/27/2013   by CJ]

	//优化速度 annotate by YY 2013/12/05 16:35
	/*	if (pTree->GetNType()==PROJECTION_STA && pTree->GetChild2()==NULL) //将ptree 变成chop
	{
	pTree->SetNType(CHOP_STA);
	CSyntaxNode *temp,*temp1;
	temp=pTree;
	while(temp->GetChild1()!=NULL)
	{
	temp->GetChild1()->SetNType(CHOP_STA);
	temp=temp->GetChild1();
	} //最底下的chop的 右孩子为空，用做孩子替换temp

	temp1= new CSyntaxNode();
	temp1->CopyTree(temp->GetChild0());
	temp->CopyTree(temp1);

	delete temp1;
	temp1=NULL;
	temp=NULL;
	}
	//end of added by cj
	*/
	//add by YY 2013/08/22 9:57 m_PrgmRemains一定不为空，m_Root有可能为空
	if (m_Root == NULL)
	{
		m_Root = pTree;//优化速度 YY 2013/12/05 16:36
		return true;
	}
	else
	{
		//add by YY 2013/08/22 9:49 不知道可不可以这样直接改变this的指向，减少copytree
		CSyntaxNode *temp = new CSyntaxNode(NType, RType);
		temp->SetChild1(m_Root);
		temp->SetChild0(pTree);//优化速度 YY 2013/12/05 16:37
		m_Root = temp;
		temp = NULL;
		return true;
	}
}
//======================================================================================
// 对树进行预处理
void CSyntaxTree::Pre(CSyntaxNode* pTree)
{
	if (pTree == NULL || this == NULL)   //待处理的树为空，直接返回，否则判断根结点类型
	{
		cout << " Pre error!" << endl;
		return;
	}
	else
	{
		//Dnf(pTree);//临时修改
		switch (pTree->GetNType())
		{
		case AND_EXP:        __PreAnd(pTree);      break;
		case OR_EXP:         __PreOr(pTree);       break;
		case LENGTH_STA:     __PreLen(pTree);      break;
		case SKIP_STA:       __PreSkip(pTree);     break;
		case MORE_STA:       __PreMore(pTree);     break;
		case NEGATION_EXP:   __PreNeg(pTree);      break;
		case NEXT_STA:       __PreNext(pTree);     break;
		case CHOP_STA:       __PreChop(pTree);     break;
		case PROJECTION_STA: __PrePrj(pTree);      break;
		case IMPLY_STA:      __PreImply(pTree);	   break;
		case IFF_STA:        __PreEq(pTree);       break;
		case W_NEXT_STA:     __PreWNext(pTree);    break;
		case SOMETIMES_STA:  __PreSomtims(pTree);  break;
		case CHOPSTAR_STA:   __PreChopstar(pTree); break;


			//add by YY[fixed] 2013/05/10 15:53
			//add by YY[fixed] 2013/05/22 11:14
		case AWAIT_STA:      __PreAwait(pTree);    break;
		case HALT_STA:       __PreHalt(pTree);     break;
		case FINAL_STA:      __PreFin(pTree);      break;
		case KEEP_STA:       __PreKeep(pTree);     break;
		case ALWAYS_STA:     __PreAlways(pTree);   break;

			//add by YY 2013/06/06 21:43
			//programming by WM 2013/06/06 21:47
		case UNITASSIGN_EXP: __PreUnAssign(pTree); break;
		case ASS_EQU_EXP:    __PreAssEQ(pTree);    break;


		default: break;
		}
	}
}


//add by YY 2013/05/13 11:04
CSyntaxTree::~CSyntaxTree()
{
	delete m_Root;
	m_Root = NULL;
}

//--------------------对树预处理时用到的小函数------------------------------
//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
//added by YK 2013/05/15 17:09
void CSyntaxTree::__PreAnd(CSyntaxNode *ptree)
{

	//化为统一格式，也就是将and移到左子树上 p and (q and r) -> (p and q) and r

	CSyntaxNode *temp = NULL;
	CSyntaxNode *temp1 = NULL;

	//add by YY[fixed] 2013/05/24 16:19
	//先对树进行and的整理
	TrimTree(ptree);

	//分别预处理第0棵子树和第1棵子树
	if (ptree->GetChild0() != NULL)
	{
		Pre(ptree->GetChild0());
	}
	if (ptree->GetChild1() != NULL)
	{
		Pre(ptree->GetChild1());
	}

	//add by YY 2013/06/06 21:33
	//Dnf(ptree);//这里不用Dnf
	//add by YY 2013/06/06 21:33

	if (ptree->GetNType() == OR_EXP)
	{
		__PreOr(ptree);
		return;
	}

	//p and p -> p
	if (ptree->GetChild0() != NULL && ptree->GetChild1() != NULL &&
		ptree->GetChild0()->CompareTree(ptree->GetChild1()))
	{
		temp = new CSyntaxNode();
		temp->CopyTree(ptree->GetChild0());
		ptree->CopyTree(temp);
		delete temp;
		temp = NULL;
		return;
	}
	//p and true -> p
	if (ptree->GetChild0() != NULL && ptree->GetChild1() != NULL &&
		ptree->GetChild1()->GetNType() == TRUE_EXP)
	{
		temp = new CSyntaxNode();
		temp->CopyTree(ptree->GetChild0());
		ptree->CopyTree(temp);
		delete temp;
		temp = NULL;
		return;
	}
	//true and p -> p
	if (ptree->GetChild0() != NULL&&ptree->GetChild1() != NULL&&
		ptree->GetChild0()->GetNType() == TRUE_EXP)
	{
		temp = new CSyntaxNode();
		temp->CopyTree(ptree->GetChild1());
		ptree->CopyTree(temp);
		delete temp;
		temp = NULL;
		return;
	}
	//p and false -> false
	if (ptree->GetChild0() != NULL && ptree->GetChild1() != NULL &&
		ptree->GetChild1()->GetNType() == FALSE_EXP)
	{
		temp = new CSyntaxNode(FALSE_EXP, VOIDTYPE);
		ptree->CopyTree(temp);
		delete temp;
		temp = NULL;
		return;
	}
	//false and p -> false
	if (ptree->GetChild0() != NULL && ptree->GetChild1() != NULL &&
		ptree->GetChild0()->GetNType() == FALSE_EXP)
	{
		temp = new CSyntaxNode(FALSE_EXP, VOIDTYPE);
		ptree->CopyTree(temp);
		delete temp;
		temp = NULL;
		return;
	}
	//p and !p -> false
	if (ptree->GetChild0() != NULL && ptree->GetChild1() != NULL &&
		ptree->GetChild1()->GetNType() == NEGATION_EXP)
	{
		if (ptree->GetChild1()->GetChild0() != NULL &&
			ptree->GetChild0()->CompareTree(ptree->GetChild1()->GetChild0()))
		{
			temp = new CSyntaxNode(FALSE_EXP, VOIDTYPE);
			ptree->CopyTree(temp);
			delete temp;
			temp = NULL;
		}
		return;
	}
	//!p and p -> false
	if (ptree->GetChild0() != NULL && ptree->GetChild1() != NULL &&
		ptree->GetChild0()->GetNType() == NEGATION_EXP)
	{
		if (ptree->GetChild0()->GetChild0() != NULL &&
			ptree->GetChild1()->CompareTree(ptree->GetChild0()->GetChild0()))
		{
			temp = new CSyntaxNode(FALSE_EXP, VOIDTYPE);
			ptree->CopyTree(temp);
			delete temp;
			temp = NULL;
		}
		return;
	}
	//empty and next p -> false
	if (ptree->GetChild0() != NULL && ptree->GetChild1() != NULL &&
		ptree->GetChild0()->GetNType() == EMPTY_EXP &&
		ptree->GetChild1()->GetNType() == NEXT_STA)
	{
		temp = new CSyntaxNode(FALSE_EXP, VOIDTYPE);
		ptree->CopyTree(temp);
		delete temp;
		temp = NULL;
		return;
	}
	//next p and empty -> false
	if (ptree->GetChild0() != NULL && ptree->GetChild1() != NULL &&
		ptree->GetChild0()->GetNType() == NEXT_STA &&
		ptree->GetChild1()->GetNType() == EMPTY_EXP)
	{

		temp = new CSyntaxNode(FALSE_EXP, VOIDTYPE);
		ptree->CopyTree(temp);
		delete temp;
		temp = NULL;
		return;
	}
	//左子树中包含next节点(必须是and的孩子),右子树为empty节点，这种情况化为false
	if (ptree->GetChild1() != NULL && ptree->GetChild1()->GetNType() == EMPTY_EXP)
	{
		bool has_next = false;
		temp1 = new CSyntaxNode();
		temp1->CopyTree(ptree->GetChild0());
		while (temp1 != NULL && temp1->GetNType() == AND_EXP &&
			temp1->GetChild0() != NULL && temp1->GetChild0()->GetNType() == AND_EXP)
		{
			if (temp1->GetChild1() != NULL && temp1->GetChild1()->GetNType() == NEXT_STA)
			{
				has_next = true;
				break;
			}
			temp = new CSyntaxNode();
			temp->CopyTree(temp1->GetChild0());
			temp1->CopyTree(temp);
			delete temp;
			temp = NULL;

		}
		if (has_next != true)   //add by YY[fixed] 2013/05/21 16:10  不是true的话，才继续判断
		{
			if (temp1 != NULL && temp1->GetNType() == AND_EXP &&
				temp1->GetChild0() != NULL && temp1->GetChild0()->GetNType() != AND_EXP)
			{
				if (temp1->GetChild0() != NULL && temp1->GetChild0()->GetNType() == NEXT_STA)
				{
					has_next = true;
				}
				if (temp1->GetChild1() != NULL && temp1->GetChild1()->GetNType() == NEXT_STA)
				{
					has_next = true;
				}
			}
		}
		if (has_next == true)
		{
			temp = new CSyntaxNode(FALSE_EXP, VOIDTYPE);
			ptree->CopyTree(temp);
			delete temp;
			temp = NULL;
		}
		delete temp1;
		temp1 = NULL;
		return;

	}
	//左子树中包含empty节点(必须是and的孩子),右子树是next节点
	if (ptree->GetChild1() != NULL && ptree->GetChild1()->GetNType() == NEXT_STA)
	{
		bool has_empty = false;
		temp1 = new CSyntaxNode();
		temp1->CopyTree(ptree->GetChild0());
		while (temp1 != NULL && temp1->GetNType() == AND_EXP &&
			temp1->GetChild0() != NULL && temp1->GetChild0()->GetNType() == AND_EXP)
		{
			if (temp1->GetChild1() != NULL && temp1->GetChild1()->GetNType() == EMPTY_EXP)
			{
				has_empty = true;
				break;
			}
			temp = new CSyntaxNode();
			temp->CopyTree(temp1->GetChild0());
			temp1->CopyTree(temp);
			delete temp;
			temp = NULL;
		}
		if (has_empty != true)   //add by YY[fixed] 2013/05/21 16:19 已经为true的话，就不用继续判断了
		{
			if (temp1 != NULL && temp1->GetNType() == AND_EXP &&
				temp1->GetChild0() != NULL && temp1->GetChild0()->GetNType() != AND_EXP)
			{
				if (temp1->GetChild0() != NULL && temp1->GetChild0()->GetNType() == EMPTY_EXP)
				{
					has_empty = true;
				}
				if (temp1->GetChild1() != NULL && temp1->GetChild1()->GetNType() == EMPTY_EXP)
				{
					has_empty = true;
				}
			}
		}
		if (has_empty == true)
		{
			temp = new CSyntaxNode(FALSE_EXP, VOIDTYPE);
			ptree->CopyTree(temp);
			delete temp;
			temp = NULL;

		}
		delete temp1;
		temp1 = NULL;
		return;
	}
	/*左子树是and，右子树是其他(不是and和or)的情况，因为刚开始已经进行了整理，
	所以不会出现右子树是and的情况，因此只需要判断右子树不是or即可。例如：
	(p and q) and p -> p and q
	*/
	if (ptree->GetChild0() != NULL && ptree->GetChild0()->GetNType() == AND_EXP &&
		ptree->GetChild1() != NULL && ptree->GetChild1()->GetNType() != OR_EXP)
	{
		if (__FindInTree(ptree->GetChild0(), ptree->GetChild1(), AND_EXP))
		{
			temp = new CSyntaxNode();
			temp->CopyTree(ptree->GetChild0());
			ptree->CopyTree(temp);
			delete temp;
			temp = NULL;
		}
		return;
	}

	//处理左子树是and，右子树是其他的情况，非在左边，
	//例如(!p and !q) and p -> false   (p and !q) and !p -> false
	if (ptree->GetChild0() != NULL && ptree->GetChild0()->GetNType() == AND_EXP)
	{
		if (ptree->GetChild1() != NULL)
		{
			if (ptree->GetChild1()->GetNType() != NEGATION_EXP)
			{
				if (__FindNegInTree(ptree->GetChild0(), ptree->GetChild1(), AND_EXP))
				{
					temp = new CSyntaxNode(FALSE_EXP, VOIDTYPE);
					ptree->CopyTree(temp);
					delete temp;
					temp = NULL;
				}
			}
			else
			{
				if (__FindInTree(ptree->GetChild0(), ptree->GetChild1()->GetChild0(), AND_EXP))
				{
					temp = new CSyntaxNode(FALSE_EXP, VOIDTYPE);
					ptree->CopyTree(temp);
					delete temp;
					temp = NULL;
				}
			}
		}
		return;
	}
}
//added by YK 2013/05/15 17:09
void   CSyntaxTree::__PreOr(CSyntaxNode*ptree)
{
	CSyntaxNode *temp = NULL;
	CSyntaxNode *temp1 = NULL;
	CSyntaxNode *temp2 = NULL;

	//化为统一格式
	//add by YY[fixed] 2013/05/24 16:24
	TrimTree(ptree);

	//分别预处理第0棵子树和第1棵子树
	if (ptree->GetChild0() != NULL)
	{
		Pre(ptree->GetChild0());
	}
	if (ptree->GetChild1() != NULL)
	{
		Pre(ptree->GetChild1());
	}

	//p or p -> p
	if (ptree->GetChild0() != NULL&&ptree->GetChild1() != NULL&&
		ptree->GetChild0()->CompareTree(ptree->GetChild1()))
	{
		temp = new CSyntaxNode();
		temp->CopyTree(ptree->GetChild0());
		ptree->CopyTree(temp);
		delete temp;
		temp = NULL;
	}
	//p or !p -> true
	else if (ptree->GetChild0() != NULL&&ptree->GetChild1() != NULL&&
		ptree->GetChild1()->GetNType() == NEGATION_EXP&&
		ptree->GetChild0()->CompareTree(ptree->GetChild1()->GetChild0()))
	{
		temp = new CSyntaxNode();
		temp = new CSyntaxNode(TRUE_EXP, VOIDTYPE);
		ptree->CopyTree(temp);
		delete temp;
		temp = NULL;
	}
	//!p or p -> true
	else if (ptree->GetChild0() != NULL&&ptree->GetChild1() != NULL&&
		ptree->GetChild0()->GetNType() == NEGATION_EXP&&
		ptree->GetChild0()->GetChild0()->CompareTree(ptree->GetChild1()))
	{
		temp = new CSyntaxNode();
		temp = new CSyntaxNode(TRUE_EXP, VOIDTYPE);
		ptree->CopyTree(temp);
		delete temp;
		temp = NULL;
	}
	//p or true -> true;       true or p-> true
	else if (ptree->GetChild0() != NULL
		&&
		ptree->GetChild1() != NULL
		&&
		(ptree->GetChild0()->GetNType() == TRUE_EXP || ptree->GetChild1()->GetNType() == TRUE_EXP))
	{

		temp = new CSyntaxNode();
		temp = new CSyntaxNode(TRUE_EXP, VOIDTYPE);
		ptree->CopyTree(temp);
		delete temp;
		temp = NULL;
	}
	//p or false -> p
	else if (ptree->GetChild0() != NULL&&ptree->GetChild1() != NULL&&
		ptree->GetChild1()->GetNType() == FALSE_EXP)
	{
		temp = new CSyntaxNode();
		temp->CopyTree(ptree->GetChild0());
		ptree->CopyTree(temp);
		delete temp;
		temp = NULL;
	}
	//false or p -> p
	else if (ptree->GetChild0() != NULL&&ptree->GetChild1() != NULL&&
		ptree->GetChild0()->GetNType() == FALSE_EXP)
	{
		temp = new CSyntaxNode();
		temp->CopyTree(ptree->GetChild1());
		ptree->CopyTree(temp);
		delete temp;
		temp = NULL;
	}


	//左子树是and，右子树是其他，不含非，例如(p and q) or p -> p
	if (ptree->GetChild0() != NULL && ptree->GetChild0()->GetNType() == AND_EXP &&
		ptree->GetChild1() != NULL && ptree->GetChild1()->GetNType() != AND_EXP)
	{
		if (__FindInTree(ptree->GetChild0(), ptree->GetChild1(), AND_EXP))
		{
			temp = new CSyntaxNode();
			temp->CopyTree(ptree->GetChild1());
			ptree->CopyTree(temp);
			delete temp;
			temp = NULL;
			return;
		}
	}
	//右子树是and，左子树是其他，不含非，例如p or (p and q) -> p
	if (ptree->GetChild1() != NULL && ptree->GetChild1()->GetNType() == AND_EXP &&
		ptree->GetChild0() != NULL && ptree->GetChild0()->GetNType() != AND_EXP)
	{
		if (__FindInTree(ptree->GetChild1(), ptree->GetChild0(), AND_EXP))
		{
			temp = new CSyntaxNode();
			temp->CopyTree(ptree->GetChild0());
			ptree->CopyTree(temp);
			delete temp;
			temp = NULL;
			return;
		}
	}
	//左子树是and，右子树是and，不含非，例如(p and q and r) or (p and q) -> p and q
	//(p and q) or (p and q and r) -> p and q
	if (ptree->GetChild0() != NULL && ptree->GetChild0()->GetNType() == AND_EXP &&
		ptree->GetChild1() != NULL && ptree->GetChild1()->GetNType() == AND_EXP)
	{
		bool find = true;
		temp1 = new CSyntaxNode();
		temp1->CopyTree(ptree->GetChild1());
		while (temp1 != NULL  &&  temp1->GetNType() == AND_EXP  &&
			temp1->GetChild0() != NULL  &&  temp1->GetChild0()->GetNType() == AND_EXP)
		{
			if (!__FindInTree(ptree->GetChild0(), temp1->GetChild1(), AND_EXP))
			{
				find = false;
				break;  //add by YY[fixed] 2013/05/27 10:20
			}
			temp = new CSyntaxNode();
			temp->CopyTree(temp1->GetChild0());
			temp1->CopyTree(temp);
			delete temp;
			temp = NULL;
		}
		if (find != false)    //add by YY[fixed] 2013/05/27 10:20
		{
			if (temp1 != NULL  &&  temp1->GetNType() == AND_EXP    //add by YY[fixed] 2013/05/21 15:14
				&&  temp1->GetChild0() != NULL   && temp1->GetChild1() != NULL)
			{
				if (!__FindInTree(ptree->GetChild0(), temp1->GetChild0(), AND_EXP))
					find = false;

				if (find != false)
				{
					if (!__FindInTree(ptree->GetChild0(), temp1->GetChild1(), AND_EXP))
						find = false;
				}
			}
		}
		if (find == true)
		{
			temp = new CSyntaxNode();
			temp->CopyTree(ptree->GetChild1());
			ptree->CopyTree(temp);
			delete temp;
			temp = NULL;
			return;
			delete temp1;
			temp1 = NULL;
		}
		else
		{

			find = true;
			temp2 = new CSyntaxNode();
			temp2->CopyTree(ptree->GetChild0());
			while (temp2 != NULL  &&  temp2->GetNType() == AND_EXP  &&
				temp2->GetChild0() != NULL  &&  temp2->GetChild0()->GetNType() == AND_EXP)
			{
				if (!__FindInTree(ptree->GetChild1(), temp2->GetChild1(), AND_EXP))
				{
					find = false;
					break;   //add by YY[fixed] 2013/05/27 10:21
				}
				temp = new CSyntaxNode();
				temp->CopyTree(temp2->GetChild0());
				temp2->CopyTree(temp);
				delete temp;
				temp = NULL;
			}
			if (find != false)   //add by YY[fixed] 2013/05/27 10:21
			{
				if (temp2 != NULL && temp2->GetNType() == AND_EXP && temp2->GetChild0() != NULL
					&& temp2->GetChild1() != NULL)
				{
					if (!__FindInTree(ptree->GetChild1(), temp2->GetChild0(), AND_EXP))
						find = false;

					//add by YY 2013/06/14 16:20 多加一个if判断 少了一次函数调用
					if (find != false)
					{
						if (!__FindInTree(ptree->GetChild1(), temp2->GetChild1(), AND_EXP))
							find = false;
					}
				}
			}
			if (find == true)
			{
				temp = new CSyntaxNode();
				temp->CopyTree(ptree->GetChild0());
				ptree->CopyTree(temp);
				delete temp;
				temp = NULL;
			}
			//add by YY 2013/06/14 16:29
			delete temp1;
			temp1 = NULL;

			delete temp2;
			temp2 = NULL;
		}

	}
	//左子树根节点是or，右子树根节点是其他，不考虑非，例如(p or q) or p -> p or q
	if (ptree->GetChild0() != NULL && ptree->GetChild0()->GetNType() == OR_EXP &&
		ptree->GetChild1() != NULL && ptree->GetChild1()->GetNType() != AND_EXP)
	{
		if (__FindInTree(ptree->GetChild0(), ptree->GetChild1(), OR_EXP))
		{
			temp = new CSyntaxNode();
			temp->CopyTree(ptree->GetChild0());
			ptree->CopyTree(temp);
			delete temp;
			temp = NULL;
		}
	}
	/*左子树根节点是or，右子树根节点是其他，左子树含有右子树的非，
	例如(!p or q) or p -> true, (p or q) or !p -> true*/

	if (ptree->GetChild0() != NULL && ptree->GetChild0()->GetNType() == OR_EXP &&
		ptree->GetChild1() != NULL && ptree->GetChild1()->GetNType() != AND_EXP)
	{
		if (ptree->GetChild1()->GetNType() != NEGATION_EXP)
		{
			if (__FindNegInTree(ptree->GetChild0(), ptree->GetChild1(), OR_EXP))
			{
				temp = new CSyntaxNode(TRUE_EXP, VOIDTYPE);
				ptree->CopyTree(temp);
				delete temp;
				temp = NULL;
			}
		}
		else
		{
			if (__FindInTree(ptree->GetChild0(), ptree->GetChild1()->GetChild0(), OR_EXP))
			{
				temp = new CSyntaxNode(TRUE_EXP, VOIDTYPE);
				ptree->CopyTree(temp);
				delete temp;
				temp = NULL;
			}
		}

	}

	//左子树根节点是or,右子树根节点是and,不考虑非,例如(p or q) or (m and q) -> p or q
	//只要在左边的or项中能找到右边and项中的某一项，则整个公式即为左边的整个or项
	if (ptree->GetChild0() != NULL && ptree->GetChild0()->GetNType() == OR_EXP &&
		ptree->GetChild1() != NULL && ptree->GetChild1()->GetNType() == AND_EXP)
	{
		bool find = false;
		temp1 = new CSyntaxNode();
		temp1->CopyTree(ptree->GetChild1());
		while (temp1 != NULL && temp1->GetNType() == AND_EXP &&
			temp1->GetChild0() != NULL && temp1->GetChild0()->GetNType() == AND_EXP)
		{
			if (__FindInTree(ptree->GetChild0(), temp1->GetChild1(), OR_EXP))
			{
				find = true;
				break;
			}
			temp = new CSyntaxNode();
			temp->CopyTree(temp1->GetChild0());
			temp1->CopyTree(temp);
			delete temp;
			temp = NULL;
		}
		if (find != true)     //add by YY[fixed] 2013/05/21 15:14
		{
			if (temp1 != NULL && temp1->GetNType() == AND_EXP && temp1->GetChild0() != NULL && temp1->GetChild1() != NULL)
			{
				if (__FindInTree(ptree->GetChild0(), temp1->GetChild0(), OR_EXP))
				{
					find = true;
				}
				if (find != true)  //add by YY[fixed] 2013/05/21 15:58  只要是true，就不用继续查找了
				{
					if (__FindInTree(ptree->GetChild0(), temp1->GetChild1(), OR_EXP))
					{
						find = true;
					}
				}
			}
		}
		if (find == true)
		{
			temp = new CSyntaxNode();
			temp->CopyTree(ptree->GetChild0());
			ptree->CopyTree(temp);
			delete temp;
			temp = NULL;
		}
		delete temp1;
		temp1 = NULL;
	}

	/*左子树根节点是or,右子树根节点是and,考虑非,例如(!p or !q) or (p and q) -> true
	(p or !q) or (!p and q) -> true, (p or !q) or ( p and q) 不处理,也就是说，
	对于右子树的每个合取项，如果在左子树中都能找到它的非，则整个公式为true,否则不处理*/
	if (ptree->GetChild0() != NULL && ptree->GetChild0()->GetNType() == OR_EXP &&
		ptree->GetChild1() != NULL && ptree->GetChild1()->GetNType() == AND_EXP)
	{
		bool find = true;
		temp1 = new CSyntaxNode();
		temp1->CopyTree(ptree->GetChild1());
		while (temp1 != NULL && temp1->GetNType() == AND_EXP &&
			temp1->GetChild0() != NULL && temp1->GetChild0()->GetNType() == AND_EXP)
		{
			if (temp1->GetChild1() != NULL)
			{
				if (temp1->GetChild1()->GetNType() != NEGATION_EXP)
				{
					if (!__FindNegInTree(ptree->GetChild0(), temp1->GetChild1(), OR_EXP))
					{
						find = false;
						break;
					}
				}
				else
				{
					if (!__FindInTree(ptree->GetChild0(), temp1->GetChild1()->GetChild0(), OR_EXP))
					{
						find = false;
						break;
					}
				}
			}

			temp = new CSyntaxNode();
			temp->CopyTree(temp1->GetChild0());
			temp1->CopyTree(temp);
			delete temp;
			temp = NULL;
		}
		if (find != false)  //add by YY[fixed] 2013/05/21 15:49
		{
			if (temp1 != NULL && temp1->GetNType() == AND_EXP)
			{
				if (temp1->GetChild0() != NULL)
				{
					if (temp1->GetChild0()->GetNType() != NEGATION_EXP)
					{
						if (!__FindNegInTree(ptree->GetChild0(), temp1->GetChild0(), OR_EXP))
						{
							find = false;
						}
					}
					else
					{
						if (!__FindInTree(ptree->GetChild0(), temp1->GetChild0()->GetChild0(), OR_EXP))
						{
							find = false;
						}
					}
				}
				if (find != false)   //add by YY[fixed] 2013/05/21 15:49   若已经为false，就没必要加继续判断了
				{
					if (temp1->GetChild1() != NULL)
					{
						if (temp1->GetChild1()->GetNType() != NEGATION_EXP)
						{
							if (!__FindNegInTree(ptree->GetChild0(), temp1->GetChild1(), OR_EXP))
							{
								find = false;
							}
						}
						else
						{
							if (!__FindInTree(ptree->GetChild0(), temp1->GetChild1()->GetChild0(), OR_EXP))
							{
								find = false;
							}
						}
					}
				}
			}
		}
		if (find == true)
		{
			temp = new CSyntaxNode(TRUE_EXP, VOIDTYPE);
			ptree->CopyTree(temp);
			delete temp;
			temp = NULL;
		}
		delete temp1;
		temp1 = NULL;
	}
}


void    CSyntaxTree::__PreLen(CSyntaxNode* pTree)
{
	if (pTree->GetChild0() == NULL)
	{
		cout << " __PreLen error! " << endl;
		return;
		//exit(-1);
	}
	//add by wm 2013/08/15  10:43
	else if (pTree->GetPreLen() == true)
	{
		return;
	}
	else
	{
		int n = pTree->GetChild0()->GetiValue();
		//-------------------len 0  -> empty---------------------------
		if (n == 0)
		{
			delete pTree->GetChild0();
			pTree->SetChild0(NULL);
			pTree->SetNType(EMPTY_EXP);
		}
		// -----------------len 1 ->  () empty--------------------------
		else if (n == 1)
		{
			pTree->SetNType(NEXT_STA);
			pTree->GetChild0()->SetNType(EMPTY_EXP);
			pTree->GetChild0()->SetiValue(0);
		}
		// ----------------len n -> ()len (n-1)-------------------------
		else if (n>1)
		{
			CSyntaxNode* temp = new CSyntaxNode(NEXT_STA, VOIDTYPE);
			temp->SetChild0(new CSyntaxNode());
			temp->GetChild0()->CopyTree(pTree);
			temp->GetChild0()->SetPreLen(true);

			temp->GetChild0()->GetChild0()->SetiValue(n - 1);
			pTree->CopyTree(temp);
			delete temp;
			temp = NULL;
		}
		else
		{
			cout << "Error! Length can't be negative!" << endl;
			return;
		}
	}
}

void	CSyntaxTree::__PreSkip(CSyntaxNode* pTree)
{
	//skip -> ()empty
	pTree->SetNType(NEXT_STA);
	pTree->SetChild0(new CSyntaxNode(EMPTY_EXP, VOIDTYPE));
}

// add by YY[fixed] 2013/05/09 9:25
//programming by WM 2013/05/09 9 27
void	CSyntaxTree::__PreMore(CSyntaxNode* pTree)
{
	//--------------more -> ()T--------------------
	//add by YY[fixed] 2013/05/06 16:43
	pTree->SetNType(NEXT_STA);
	pTree->SetChild0(new CSyntaxNode(TRUE_EXP, VOIDTYPE));
}

//add by YY 2013/05/09 10:18
//programming by WM 2013/05/09 10 19
void	CSyntaxTree::__PreNeg(CSyntaxNode *pTree)
{
	if (pTree->GetChild0() == NULL)
	{
		cout << " __PreNeg error! " << endl;
		return;
	}
	CSyntaxNode *temp = NULL, *temp2 = NULL;
	Pre(pTree->GetChild0());

	//!!p  ->  p
	if (pTree->GetChild0()->GetNType() == NEGATION_EXP)
	{
		temp = new CSyntaxNode();
		temp->CopyTree(pTree->GetChild0()->GetChild0());
		pTree->CopyTree(temp);
		delete temp;
		temp = NULL;
		return;
	}

	//!true  ->  false
	else if (pTree->GetChild0()->GetNType() == TRUE_EXP)
	{
		temp = new CSyntaxNode(FALSE_EXP, BOOLTYPE);
		pTree->CopyTree(temp);
		delete temp;
		temp = NULL;
		return;
	}

	//!false  -> true
	else if (pTree->GetChild0()->GetNType() == FALSE_EXP)
	{
		temp = new CSyntaxNode(TRUE_EXP, BOOLTYPE);
		pTree->CopyTree(temp);
		delete temp;
		return;
	}

	// !empty  -> next true
	else if (pTree->GetChild0()->GetNType() == EMPTY_EXP)
	{
		temp = new CSyntaxNode(NEXT_STA, BOOLTYPE);
		temp->SetChild0(new CSyntaxNode(TRUE_EXP, BOOLTYPE));
		pTree->CopyTree(temp);
		delete temp;
		return;
	}

	//!()true  -> empty
	else if (pTree->GetChild0()->GetNType() == NEXT_STA&&
		pTree->GetChild0()->GetChild0() != NULL&&
		pTree->GetChild0()->GetChild0()->GetNType() == TRUE_EXP)
	{
		temp = new CSyntaxNode(EMPTY_EXP, BOOLTYPE);
		pTree->CopyTree(temp);
		delete temp;
		return;
	}

	//![]p ->  <>!p
	else if (pTree->GetChild0()->GetNType() == ALWAYS_STA&&
		pTree->GetChild0()->GetChild0() != NULL)
	{
		temp = new CSyntaxNode(NEGATION_EXP, BOOLTYPE);
		temp->SetChild0(new CSyntaxNode());
		temp->GetChild0()->CopyTree(pTree->GetChild0()->GetChild0());
		//__PreNeg(temp);

		pTree->SetNType(SOMETIMES_STA);
		pTree->GetChild0()->CopyTree(temp);

		//add by Yk 2013/10/09 21:01
		__PreSomtims(pTree);

		delete temp;
		temp = NULL;
		return;
	}

	//!<> p  ->  []!p
	else if (pTree->GetChild0()->GetNType() == SOMETIMES_STA &&
		pTree->GetChild0()->GetChild0() != NULL)
	{
		temp = new CSyntaxNode(NEGATION_EXP, BOOLTYPE);
		temp->SetChild0(new CSyntaxNode());
		temp->GetChild0()->CopyTree(pTree->GetChild0()->GetChild0());
		__PreNeg(temp);

		pTree->SetNType(ALWAYS_STA);
		pTree->GetChild0()->CopyTree(temp);
		delete temp;
		temp = NULL;
		return;
	}

	//!(p and q and r)  化成 !p or !q or !r
	else if (pTree->GetChild0()->GetNType() == AND_EXP)
	{
		temp = pTree->GetChild0();


		while (temp->GetNType() == AND_EXP && temp->GetChild1() != NULL)
		{
			temp2 = new CSyntaxNode(NEGATION_EXP, BOOLTYPE);
			temp2->SetChild0(new CSyntaxNode());
			temp2->GetChild0()->CopyTree(temp->GetChild1());

			__PreNeg(temp2);

			temp->GetChild1()->CopyTree(temp2);
			delete temp2;
			temp2 = NULL;
			temp->SetNType(OR_EXP);
			if (temp->GetChild0() != NULL)
			{
				temp = temp->GetChild0();
			}
			else
			{
				cout << "memory error!" << endl;
				return;
			}
		}

		//!(p and q and r) 化成 !p or !q or !r
		temp2 = new CSyntaxNode(NEGATION_EXP, BOOLTYPE);
		temp2->SetChild0(new CSyntaxNode());
		temp2->GetChild0()->CopyTree(temp);

		__PreNeg(temp2);

		temp->CopyTree(temp2);

		delete temp2;
		temp2 = NULL;

		//将最终结果拷贝到pTree
		temp = new CSyntaxNode();
		temp->CopyTree(pTree->GetChild0());
		pTree->CopyTree(temp);
		delete temp;
		temp = NULL;
		return;
	}

	//!(p or q) 化成!p and !q
	else if (pTree->GetChild0()->GetNType() == OR_EXP)
	{
		temp = pTree->GetChild0();

		//!(p or q or r)  先化成 p and !q and !r
		while (temp->GetNType() == OR_EXP && temp->GetChild1() != NULL)
		{
			temp2 = new CSyntaxNode(NEGATION_EXP, BOOLTYPE);
			temp2->SetChild0(new CSyntaxNode());
			temp2->GetChild0()->CopyTree(temp->GetChild1());

			__PreNeg(temp2);

			temp->GetChild1()->CopyTree(temp2);
			delete temp2;
			temp2 = NULL;
			temp->SetNType(AND_EXP);
			if (temp->GetChild0() != NULL)
			{
				temp = temp->GetChild0();
			}
			else
			{
				cout << "memory error!" << endl;
				return;
			}

		}
		//!(p or q or r) 化成 !p and !q and !r
		temp2 = new CSyntaxNode(NEGATION_EXP, BOOLTYPE);
		temp2->SetChild0(new CSyntaxNode());
		temp2->GetChild0()->CopyTree(temp);
		__PreNeg(temp2);

		temp->CopyTree(temp2);
		delete temp2;
		temp2 = NULL;

		//将结果拷贝到pTree
		temp = new CSyntaxNode();
		temp->CopyTree(pTree->GetChild0());
		pTree->CopyTree(temp);
		delete temp;
		temp = NULL;
		return;
	}
}

void	CSyntaxTree::__PreNext(CSyntaxNode* pTree)
{
	//---------------()F -> F--------------
	if (pTree->GetChild0() == NULL)
	{
		cout << " __PreNext error! " << endl;
		return;
	}
	Pre(pTree->GetChild0());
	if (pTree->GetChild0()->GetNType() == FALSE_EXP)
	{
		delete pTree->GetChild0();
		pTree->SetChild0(NULL);
		pTree->SetNType(FALSE_EXP);
	}
}


//add by YY 2013/05/07 16:09
void	CSyntaxTree::__PreChop(CSyntaxNode *pTree)
{
	if (pTree->GetChild0() == NULL || pTree->GetChild1() == NULL)
	{
		cout << " __PreChop error! " << endl;
		return;
		//exit(-1);
	}
	CSyntaxNode *temp = NULL;

	//----------整理为统一格式，方便以后比较语法树是否相同 p;q;r  ->  p;(q;r)-----------
	//add by YY[fixed] 2013/05/24 16:25
	TrimTree(pTree);
	if (pTree->GetChild0() != NULL)
	{
		Pre(pTree->GetChild0());
	}

	if (pTree->GetChild1() != NULL)
	{
		Pre(pTree->GetChild1());
	}


	// ---------------false;p   ->  false    p;false  ->  false-----------
	if (pTree->GetChild0() != NULL && pTree->GetChild0()->GetNType() == FALSE_EXP &&
		pTree->GetChild1() != NULL
		||
		pTree->GetChild0() != NULL &&
		pTree->GetChild1() != NULL && pTree->GetChild1()->GetNType() == FALSE_EXP)
	{
		temp = new CSyntaxNode(FALSE_EXP, VOIDTYPE);
		pTree->CopyTree(temp);
	}
	//-------empty;p  ->  p-------------
	else if (pTree->GetChild0() != NULL &&
		pTree->GetChild0()->GetNType() == EMPTY_EXP &&
		pTree->GetChild1() != NULL)
	{
		temp = new CSyntaxNode();
		temp->CopyTree(pTree->GetChild1());
		pTree->CopyTree(temp);
	}
	//-----------true;true  ->  true-------------
	else if (pTree->GetChild0() != NULL && pTree->GetChild0()->GetNType() == TRUE_EXP &&
		pTree->GetChild1() != NULL && pTree->GetChild1()->GetNType() == TRUE_EXP)
	{
		temp = new CSyntaxNode(TRUE_EXP, VOIDTYPE);
		pTree->CopyTree(temp);
	}
	//--------------true;p  ->  <>p---------------
	/*	else if(pTree->GetChild0()!=NULL && pTree->GetChild0()->GetNType()== TRUE_EXP &&
	pTree->GetChild1()!= NULL)
	{
	temp=new CSyntaxNode(SOMETIMES_STA,VOIDTYPE);
	temp->SetChild0(new CSyntaxNode());
	temp->GetChild0()->CopyTree(pTree->GetChild1());
	if(temp->GetChild0()->GetNType()!=SOMETIMES_STA)
	{
	pTree->CopyTree(temp);
	}
	else
	{
	pTree->CopyTree(temp->GetChild0());
	}
	}*/
	//add by YY[fixed] 2013/05/07 16:10
	delete temp;
	temp = NULL;
}



//add by YY 2013/05/07 16:10
void	CSyntaxTree::__PrePrj(CSyntaxNode *pTree)
{
	if (pTree->GetChild0() == NULL)
	{
		cout << " __PrePrj error! " << endl;
		return;
		//exit(-1);
	}

	//针对于optional_projection部分的PROJECTION_STA的化简
	if (pTree->GetChild2() == NULL)
	{
		Pre(pTree->GetChild0());
		if (pTree->GetChild1() != NULL)
		{
			Pre(pTree->GetChild1());
		}
		return;
	}

	CSyntaxNode *temp = NULL;

	Pre(pTree->GetChild0());
	if (pTree->GetChild1() != NULL)
	{
		Pre(pTree->GetChild1());
	}
	Pre(pTree->GetChild2());

	//add by YY[fixed] 2013/05/08 16:55
	if (pTree->GetChild0() != NULL && pTree->GetChild0()->GetNType() == FALSE_EXP)
	{
		temp = new CSyntaxNode(FALSE_EXP, VOIDTYPE);
		pTree->CopyTree(temp);
	}
	else if (pTree->GetChild1() != NULL && pTree->GetChild1()->GetNType() == FALSE_EXP)
	{
		temp = new CSyntaxNode(FALSE_EXP, VOIDTYPE);
		pTree->CopyTree(temp);
	}
	else if (pTree->GetChild2() != NULL && pTree->GetChild2()->GetNType() == FALSE_EXP)
	{
		temp = new CSyntaxNode(FALSE_EXP, VOIDTYPE);
		pTree->CopyTree(temp);
	}

	// ---------- empty prj p -> p -------------
	// ---------- true prj p -> p -------------
	else if (pTree->GetChild0() != NULL && pTree->GetChild0()->GetNType() == EMPTY_EXP &&
		pTree->GetChild1() == NULL
		||
		pTree->GetChild0() != NULL && pTree->GetChild0()->GetNType() == TRUE_EXP &&
		pTree->GetChild1() == NULL)
	{
		temp = new CSyntaxNode();
		temp->CopyTree(pTree->GetChild2());
		pTree->CopyTree(temp);
	}
	//-------------p prj true -> p ----------------------------
	else if (pTree->GetChild0() != NULL && pTree->GetChild2() != NULL &&
		pTree->GetChild2()->GetNType() == TRUE_EXP)
	{
		temp = new CSyntaxNode();
		temp->CopyTree(pTree->GetChild0());
		pTree->CopyTree(temp);
	}

	//add by YY[fixed] 2013/05/07 16:25
	delete temp;
	temp = NULL;

}

void	CSyntaxTree::__PreImply(CSyntaxNode *pTree)
{
	if (pTree->GetChild0() == NULL || pTree->GetChild1() == NULL)
	{
		cout << " __PreImply error! " << endl;
		return;
		//exit(-1);
	}
	CSyntaxNode *pChild0 = pTree->GetChild0(),
		*pChild1 = pTree->GetChild1(),
		*temp = NULL;

	Pre(pChild0);
	Pre(pChild1);

	//------false->p  ->   true     p->true   ->   true---------------
	if (pChild0->GetNType() == FALSE_EXP || pChild1->GetNType() == TRUE_EXP)
	{
		temp = new CSyntaxNode(TRUE_EXP, BOOLTYPE);
		pTree->CopyTree(temp);
		delete temp;
		temp = NULL;
		return;
	}

	//---------------------true->p  ->   p   -------------------------
	else if (pChild0->GetNType() == TRUE_EXP)
	{
		temp = new CSyntaxNode();
		temp->CopyTree(pChild1);
		pTree->CopyTree(temp);
		delete temp;
		temp = NULL;
		return;
	}

	//------------------   p->false  ->  !p --------------------------
	else if (pChild1->GetNType() == FALSE_EXP)
	{
		delete pChild1;
		pChild1 = NULL;
		pTree->SetNType(NEGATION_EXP);
		pTree->SetChild1(NULL);
		__PreNeg(pTree);
		return;
	}

	//-----------------   p->q  ->  !p or q --------------------------
	else
	{
		pTree->SetNType(OR_EXP);
		temp = new CSyntaxNode(NEGATION_EXP, BOOLTYPE);
		temp->SetChild0(new CSyntaxNode());
		temp->GetChild0()->CopyTree(pChild0);
		pChild0->CopyTree(temp);
		delete temp;
		temp = NULL;
		__PreOr(pTree);
		return;
	}

}


//programming by WM 2013/05/09 17 08
//tested by YY 2013/05/09 17:08
void	CSyntaxTree::__PreEq(CSyntaxNode *pTree)
{
	if (pTree->GetChild0() == NULL || pTree->GetChild1() == NULL)
	{
		cout << " __PreEq error! " << endl;
		return;
		//exit(-1);
	}
	CSyntaxNode *pChild0 = pTree->GetChild0(),
		*pChild1 = pTree->GetChild1(),
		*temp1 = NULL;
	Pre(pChild0);
	Pre(pChild1);

	//------------------ p<->p   ->   true -----------------
	if (pChild0->CompareTree(pChild1))
	{
		temp1 = new CSyntaxNode(TRUE_EXP, BOOLTYPE);
		pTree->CopyTree(temp1);
		delete temp1;
		temp1 = NULL;
	}

	//----------------false<->p   ->  !p -------------------
	else if (pChild0->GetNType() == FALSE_EXP)
	{
		temp1 = new CSyntaxNode(NEGATION_EXP, BOOLTYPE);
		temp1->SetChild0(new CSyntaxNode());
		temp1->GetChild0()->CopyTree(pChild1);
		pTree->CopyTree(temp1);
		__PreNeg(pTree);
		delete temp1;
		temp1 = NULL;
	}

	//---------------- p<->false  ->  !p --------------------
	else if (pChild1->GetNType() == FALSE_EXP)
	{
		delete pChild1;
		pChild1 = NULL;
		pTree->SetChild1(NULL);
		pTree->SetNType(NEGATION_EXP);
		__PreNeg(pTree);
	}

	//---------------- true<->p   ->   p ---------------------
	else if (pChild0->GetNType() == TRUE_EXP)
	{
		temp1 = new CSyntaxNode();
		temp1->CopyTree(pChild1);
		pTree->CopyTree(temp1);
		delete temp1;
		temp1 = NULL;
	}

	//---------------- p<->true   ->   p----------------------
	else if (pChild1->GetNType() == TRUE_EXP)
	{
		temp1 = new CSyntaxNode();
		temp1->CopyTree(pChild0);
		pTree->CopyTree(temp1);
		delete temp1;
		temp1 = NULL;
	}
	//--------------- p<->!p   ->   false------------------
	//--------------- !p<->p   ->   false------------------
	else if (pChild1->GetNType() == NEGATION_EXP  &&
		pChild0->CompareTree(pChild1->GetChild0()) ||
		pChild0->GetNType() == NEGATION_EXP &&
		pChild1->CompareTree(pChild0->GetChild0()))
	{
		temp1 = new CSyntaxNode(FALSE_EXP, BOOLTYPE);
		pTree->CopyTree(temp1);
		delete temp1;
		temp1 = NULL;
		return;
	}
	//---------- p<->q   ->  (p and q) or (!p and !q)
	else
	{
		//!p
		temp1 = new CSyntaxNode(NEGATION_EXP, BOOLTYPE);
		temp1->SetChild0(new CSyntaxNode());
		temp1->GetChild0()->CopyTree(pChild0);

		//!q
		CSyntaxNode* temp2 = new CSyntaxNode(NEGATION_EXP, BOOLTYPE);
		temp2->SetChild0(new CSyntaxNode());
		temp2->GetChild0()->CopyTree(pChild1);

		//p and q
		CSyntaxNode* temp3 = new CSyntaxNode(AND_EXP, BOOLTYPE);
		temp3->SetChild0(new CSyntaxNode());
		temp3->GetChild0()->CopyTree(pChild0);
		temp3->SetChild1(new CSyntaxNode());
		temp3->GetChild1()->CopyTree(pChild1);

		//!p and !q
		CSyntaxNode *temp4 = new CSyntaxNode(AND_EXP, BOOLTYPE);
		temp4->SetChild0(new CSyntaxNode());
		temp4->GetChild0()->CopyTree(temp1);
		temp4->SetChild1(new CSyntaxNode());
		temp4->GetChild1()->CopyTree(temp2);

		//(p and q) or (!p and !q)
		pTree->SetNType(OR_EXP);
		pChild0->CopyTree(temp3);
		pChild1->CopyTree(temp4);

		__PreOr(pTree);

		delete temp1;
		temp1 = NULL;
		delete temp2;
		temp2 = NULL;
		delete temp3;
		temp3 = NULL;
		delete temp4;
		temp4 = NULL;
	}
}

//tested by YY 2013/05/10 10:01
void	CSyntaxTree::__PreWNext(CSyntaxNode* pTree)
{
	// (.) p  -> empty or ()p
	// (.) false -> empty

	// (.) true  -> true
	//add by YY[fixed] 2013/05/10 9:59
	if (pTree->GetChild0() == NULL)
	{
		cout << " __PreWNext error! " << endl;
		return;
		//exit(-1);
	}
	Pre(pTree->GetChild0());
	if (pTree->GetChild0()->GetNType() == TRUE_EXP)
	{
		delete pTree->GetChild0();
		pTree->SetChild0(NULL);
		pTree->SetNType(TRUE_EXP);
		return;
	}
	if (pTree->GetChild0()->GetNType() == FALSE_EXP)
	{
		delete pTree->GetChild0();
		pTree->SetChild0(NULL);
		pTree->SetNType(EMPTY_EXP);
		return;
	}
	else
	{
		CSyntaxNode *temp = NULL;
		temp = new CSyntaxNode();
		temp->CopyTree(pTree->GetChild0());

		delete pTree->GetChild0();
		pTree->SetChild0(new CSyntaxNode(EMPTY_EXP, VOIDTYPE));
		pTree->SetChild1(new CSyntaxNode(NEXT_STA, VOIDTYPE));
		pTree->GetChild1()->SetChild0(new CSyntaxNode());
		pTree->GetChild1()->GetChild0()->CopyTree(temp);
		pTree->SetNType(OR_EXP);
		delete temp;
		temp = NULL;
		//add by YY[fixed] 2013/05/06 17:40
		/*
		delete pTree->GetChild0();
		pTree->SetChild0(new CSyntaxNode(EMPTY_EXP,VOIDTYPE));
		pTree->SetChild1(new CSyntaxNode(NEXT_STA,VOIDTYPE));
		pTree->GetChild1()->SetChild0(new CSyntaxNode());
		pTree->GetChild1()->GetChild0()->CopyTree(temp);
		delete temp;
		temp=NULL;
		*/
	}
}

/*   annotated by YY   2013-5-22
void	CSyntaxTree::__PreAlways(CSyntaxNode* pTree)
{
if(pTree->GetChild0()==NULL)
{
cout<<" __PreAlways error! "<<endl;
return;
//exit(-1);
}

CSyntaxNode *temp = NULL;
Pre(pTree->GetChild0());
// [][]p ->  []p
if(pTree->GetChild0()->GetNType()==ALWAYS_STA)
{
temp=new CSyntaxNode();
temp->CopyTree(pTree->GetChild0());
pTree->CopyTree(temp);
delete temp;
temp=NULL;
}
//[]false  ->false
else if(pTree->GetChild0()->GetNType()==FALSE_EXP)
{
temp=new CSyntaxNode(FALSE_EXP,VOIDTYPE);
pTree->CopyTree(temp);
delete temp;
temp=NULL;
}
// []true  ->true
else if(pTree->GetChild0()->GetNType()==TRUE_EXP)
{
temp=new CSyntaxNode(TRUE_EXP,VOIDTYPE);
pTree->CopyTree(temp);
delete temp;
temp=NULL;
}

//[]p ==> p and empty || p and ()[]p
else                                                                            //add by YY[fixed] 2013/05/13 15:35
{
temp=new CSyntaxNode(OR_EXP, VOIDTYPE);
CSyntaxNode *temp1=new CSyntaxNode(AND_EXP, VOIDTYPE);
CSyntaxNode *temp2=new CSyntaxNode(AND_EXP, VOIDTYPE);

temp1->SetChild0(new CSyntaxNode());
temp1->GetChild0()->CopyTree(pTree->GetChild0());
temp1->SetChild1(new CSyntaxNode(EMPTY_EXP, VOIDTYPE));

temp2->SetChild0(new CSyntaxNode());
temp2->GetChild0()->CopyTree(pTree->GetChild0());
temp2->SetChild1(new CSyntaxNode(NEXT_STA, VOIDTYPE));
temp2->GetChild1()->SetChild0(new CSyntaxNode());
temp2->GetChild1()->GetChild0()->CopyTree(pTree);

temp->SetChild0(new CSyntaxNode());
temp->GetChild0()->CopyTree(temp1);
temp->SetChild1(new CSyntaxNode());
temp->GetChild1()->CopyTree(temp2);
pTree->CopyTree(temp);

__PreOr(pTree);

delete temp;
temp=NULL;
delete temp1;
temp1=NULL;
delete temp2;
temp2=NULL;

}
} */

void	CSyntaxTree::__PreSomtims(CSyntaxNode* pTree)
{
	if (pTree->GetChild0() == NULL)
	{
		cout << " __PreSomtims error! " << endl;
		return;
		//exit(-1);
	}
	CSyntaxNode *temp = NULL;
	Pre(pTree->GetChild0());
	// <><>p ->  <> p
	if (pTree->GetChild0()->GetNType() == SOMETIMES_STA)
	{
		temp = new CSyntaxNode();
		temp->CopyTree(pTree->GetChild0());
		pTree->CopyTree(temp);
		delete temp;
		temp = NULL;
	}
	//<>false  ->false
	else if (pTree->GetChild0()->GetNType() == FALSE_EXP)
	{
		temp = new CSyntaxNode(FALSE_EXP, VOIDTYPE);
		pTree->CopyTree(temp);
		delete temp;
		temp = NULL;
	}
	// <>true  ->true
	else if (pTree->GetChild0()->GetNType() == TRUE_EXP)
	{
		temp = new CSyntaxNode(TRUE_EXP, VOIDTYPE);
		pTree->CopyTree(temp);
		delete temp;
		temp = NULL;
	}
	//<>p -> true;p
	else
	{
		temp = new CSyntaxNode(CHOP_STA, VOIDTYPE);
		temp->SetChild0(new CSyntaxNode(TRUE_EXP, VOIDTYPE));
		temp->SetChild1(new CSyntaxNode());
		temp->GetChild1()->CopyTree(pTree->GetChild0());
		pTree->CopyTree(temp);
		delete temp;
		temp = NULL;
	}
}

void	CSyntaxTree::__PreChopstar(CSyntaxNode* pTree)
{
	//add by YY[fixed] 2013/05/06 17:39
	//	CSyntaxNode *temp=pTree->GetChild0();
	//p## -> p#
	if (pTree->GetChild0() == NULL)
	{
		cout << " __PreChopstar error! " << endl;
		return;
		//exit(-1);
	}

	Pre(pTree->GetChild0());
	if (pTree->GetChild0()->GetNType() == CHOPSTAR_STA)
	{
		CSyntaxNode *temp = new CSyntaxNode();
		temp->CopyTree(pTree->GetChild0());
		pTree->CopyTree(temp);
		delete temp;
		temp = NULL;
		//add by YY[fixed] 2013/05/06 17:40
		/*
		CSyntaxNode *temp=pTree->GetChild0();
		pTree->SetChild0(pTree->GetChild0()->GetChild0());
		delete temp;
		temp=NULL;
		*/
	}
}

//programming by WM 2013/05/09 17 37
//tested by YY 2013/05/09 17:37
void	CSyntaxTree::__PreParallel(CSyntaxNode *pTree)
{
	if (pTree->GetChild0() == NULL || pTree->GetChild1() == NULL)
	{
		cout << " __PreParallel error! " << endl;
		return;
	}

	Pre(pTree->GetChild0());
	Pre(pTree->GetChild1());

	//p||q  ->  p and (q;true) or (p;true) and q
	CSyntaxNode *temp1 = new CSyntaxNode(AND_EXP, VOIDTYPE),
		*temp2 = new CSyntaxNode(AND_EXP, VOIDTYPE);

	//p and (q;true)
	temp1->SetChild0(new CSyntaxNode());
	temp1->GetChild0()->CopyTree(pTree->GetChild0());
	temp1->SetChild1(new CSyntaxNode(CHOP_STA, VOIDTYPE));
	temp1->GetChild1()->SetChild0(new CSyntaxNode());
	temp1->GetChild1()->GetChild0()->CopyTree(pTree->GetChild1());
	temp1->GetChild1()->SetChild1(new CSyntaxNode(TRUE_EXP, VOIDTYPE));

	//(p;true) and q;
	temp2->SetChild0(new CSyntaxNode(CHOP_STA, VOIDTYPE));
	temp2->GetChild0()->SetChild0(new CSyntaxNode());
	temp2->GetChild0()->GetChild0()->CopyTree(pTree->GetChild0());
	temp2->GetChild0()->SetChild1(new CSyntaxNode(TRUE_EXP, VOIDTYPE));
	temp2->SetChild1(new CSyntaxNode());
	temp2->GetChild1()->CopyTree(pTree->GetChild1());

	pTree->SetNType(OR_EXP);
	pTree->GetChild0()->CopyTree(temp1);
	pTree->GetChild1()->CopyTree(temp2);

	__PreOr(pTree);

	delete temp1;
	temp1 = NULL;
	delete temp2;
	temp2 = NULL;

}

//------------------------------------------------------------------------------
//programming by YY 2013/05/24 10:14
void	CSyntaxTree::__PreAlways(CSyntaxNode*pTree)
{
	CSyntaxNode*temp = NULL;

	if (pTree->GetChild0() == NULL)
	{
		cout << " __PreAlways error! " << endl;
		return;
	}

	else
	{
		Pre(pTree->GetChild0());
		// [][]p ->  []p
		if (pTree->GetChild0()->GetNType() == ALWAYS_STA)
		{
			temp = new CSyntaxNode();
			temp->CopyTree(pTree->GetChild0());
			pTree->CopyTree(temp);
			delete temp;
			temp = NULL;
		}
		//[]false  ->false
		else if (pTree->GetChild0()->GetNType() == FALSE_EXP)
		{
			temp = new CSyntaxNode(FALSE_EXP, VOIDTYPE);
			pTree->CopyTree(temp);
			delete temp;
			temp = NULL;
		}
		// []true  ->true
		else if (pTree->GetChild0()->GetNType() == TRUE_EXP)
		{
			temp = new CSyntaxNode(TRUE_EXP, VOIDTYPE);
			pTree->CopyTree(temp);
			delete temp;
			temp = NULL;
		}
	}

}

//programming by YY 2013/05/10 16:35
//await(b) ==>b && empty  or  ~b && ()await(b)
//只是pre最明显的case
void CSyntaxTree::__PreAwait(CSyntaxNode *pTree)
{
	if (pTree->GetChild0() == NULL)
	{
		cout << " __PreAwait error! " << endl;
		return;
		//exit(-1);
	}
	Pre(pTree->GetChild0());

	NODETYPE NType = pTree->GetChild0()->GetNType();

	//---------- await(true) ->  empty -------------------
	if (NType == TRUE_EXP)
	{
		delete pTree->GetChild0();
		pTree->SetChild0(NULL);
		pTree->SetNType(EMPTY_EXP);
	}
	//--------- await(next true) ->  false----------------
	else if (NType == NEXT_STA)
	{
		if (pTree->GetChild0()->GetChild0() == NULL)
		{
			cout << " __PreAwait error! " << endl;
			return;
			// exit(-1);
		}
		if (pTree->GetChild0()->GetChild0()->GetNType() == TRUE_EXP)
		{
			delete pTree->GetChild0();
			pTree->SetChild0(NULL);
			pTree->SetNType(FALSE_EXP);
		}
	}
}

//programming by YY 2013/05/10 16:39
//halt(b) ==>b && empty  or  ~b && ()halt(b)
//只是pre最明显的case
void CSyntaxTree::__PreHalt(CSyntaxNode *pTree)
{
	if (pTree->GetChild0() == NULL)
	{
		cout << " __PreHalt error! " << endl;
		return;
		//exit(-1);
	}

	//add by wm 2013/05/21  17:46
	Pre(pTree->GetChild0());
	NODETYPE NType = pTree->GetChild0()->GetNType();

	//---------- halt(true) ->  empty -------------------
	if (NType == TRUE_EXP)
	{
		delete pTree->GetChild0();
		pTree->SetChild0(NULL);
		pTree->SetNType(EMPTY_EXP);
	}

	//--------- halt(next true) ->  false----------------
	else if (NType == NEXT_STA)
	{
		if (pTree->GetChild0()->GetChild0() == NULL)
		{
			cout << " __PreHalt error! " << endl;
			return;
			//	exit(-1);
		}
		if (pTree->GetChild0()->GetChild0()->GetNType() == TRUE_EXP)
		{
			delete pTree->GetChild0();
			pTree->SetChild0(NULL);
			pTree->SetNType(FALSE_EXP);
		}
	}

}


//programming by YY 2013/05/10 16:49
//keep(b) ==> empty or b&&()keep(b)
//只是pre最明显的case
void CSyntaxTree::__PreKeep(CSyntaxNode *pTree)
{
	if (pTree->GetChild0() == NULL)
	{
		cout << " __PreKeep error! " << endl;
		return;
		//exit(-1);
	}

	//add by wm 2013/05/21  17:50
	Pre(pTree->GetChild0());
	NODETYPE NType = pTree->GetChild0()->GetNType();

	//---------- keep(false)  ->  empty -----------------
	if (NType == FALSE_EXP)
	{
		delete pTree->GetChild0();
		pTree->SetChild0(NULL);
		pTree->SetNType(EMPTY_EXP);
	}
	//---------- keep(empty) ->  empty -------------------
	else if (NType == EMPTY_EXP)
	{
		delete pTree->GetChild0();
		pTree->SetChild0(NULL);
		pTree->SetNType(EMPTY_EXP);
	}
}

//programming by YY 2013/05/10 17:04
//fin(b) ==> b && empty or ()fin(b)
//只是pre最明显的case
void CSyntaxTree::__PreFin(CSyntaxNode *pTree)
{

}






//======================================================================================
//programming by CJ 2013/05/13 14:51
//add by YY 2013/05/13 14:51
//对树进行NF
void CSyntaxTree::NF(CSyntaxNode *pTree)
{
	if (pTree == NULL || this == NULL)
	{
		cout << " NF error! " << endl;
		return;
	}

	//先判断pTree 是否为 状态公式
	if (__IsStatFormula(pTree))
	{

		//先化成DNF,再NFStateFormula,再DNF
		//add by YY 2013/05/14 9:17
		//__Reduce(pTree);
		Dnf(pTree);
		__NfStatFormula(pTree);

		//add by YY 2013/06/07 11:47
		//programming by WM 2013/06/07 11:47
		/*if(pTree->GetChild0()->GetChild0()->GetNType()==OR_EXP)//yangkai 2014-1-2 present部分不用化为析取范式
		{
		//Dnf(pTree);//临时修改
		//TrimTree(pTree);//临时修改
		}*/
	}

	//add by YY 2013/05/13 17:22
	//这里应该还要多加几层判断，针对u and empty, u and ()p这些情况，直接已经是NF
	//add by YY 2013/05/14 17:41
	//这个判断可以加到__NfAnd函数里面去
	else
	{
		switch (pTree->GetNType())
		{

		case  TRUE_EXP:				__NfTrue(pTree);	 break;

		case  AND_EXP:				__NfAnd(pTree);		 break;

		case  OR_EXP:				__NfOr(pTree);		 break;

		case  PROJECTION_STA:		__NfPrj(pTree);		 break;

		case  NEGATION_EXP:			__NfNeg(pTree);		 break;

		case  CHOP_STA:				__NfChop(pTree);	 break;

		case  CHOPSTAR_STA:			__NfChopStar(pTree); break;


			//add by YY 2013/05/14 17:47
		case SOMETIMES_STA:         __NfSomtims(pTree);  break;

			//add by YY 2013/05/22 11:17
		case HALT_STA:              __NfHalt(pTree);     break;
		case AWAIT_STA:             __NfAwait(pTree);	 break;
		case KEEP_STA:				__NfKeep(pTree);     break;
		case FINAL_STA:				__NfFinal(pTree);    break;
		case ALWAYS_STA:			__NfAlwys(pTree);    break;


		}
	}
}


//x<==1  --> x<==1 && e || x<==1 && ()true
void    CSyntaxTree::__NfStatFormula(CSyntaxNode *pTree)
{
	CSyntaxNode *temp = new CSyntaxNode(OR_EXP, VOIDTYPE);
	CSyntaxNode *temp1 = new CSyntaxNode(AND_EXP, VOIDTYPE);
	CSyntaxNode *temp2 = new CSyntaxNode(AND_EXP, VOIDTYPE);

	temp1->SetChild0(new CSyntaxNode());
	temp1->GetChild0()->CopyTree(pTree);
	temp1->SetChild1(new CSyntaxNode(EMPTY_EXP, VOIDTYPE));

	temp2->SetChild0(new CSyntaxNode());
	temp2->GetChild0()->CopyTree(pTree);
	temp2->SetChild1(new CSyntaxNode(NEXT_STA, VOIDTYPE));
	temp2->GetChild1()->SetChild0(new CSyntaxNode(TRUE_EXP, VOIDTYPE));

	temp->SetChild0(new CSyntaxNode());
	temp->GetChild0()->CopyTree(temp1);
	temp->SetChild1(new CSyntaxNode());
	temp->GetChild1()->CopyTree(temp2);

	pTree->CopyTree(temp);

	delete temp;
	temp = NULL;
	delete temp1;
	temp1 = NULL;
	delete temp2;
	temp2 = NULL;
}

//add by wm 2013/05/21  18:51
void	CSyntaxTree::__NfAlwys(CSyntaxNode*pTree)
{
	if (pTree->GetChild0() == NULL)
	{
		cout << "__NFAlwys error!" << endl;
		return;
	}

	//------------always p -> p and empty or p and ()always p------------
	CSyntaxNode*temp1 = new CSyntaxNode(AND_EXP, VOIDTYPE),
		*temp2 = new CSyntaxNode(AND_EXP, VOIDTYPE);

	//temp1: p and empty
	temp1->SetChild0(new CSyntaxNode());
	temp1->GetChild0()->CopyTree(pTree->GetChild0());
	temp1->SetChild1(new CSyntaxNode(EMPTY_EXP, VOIDTYPE));

	//temp2:p and  ()always p
	temp2->SetChild0(new CSyntaxNode());
	temp2->GetChild0()->CopyTree(pTree->GetChild0());
	temp2->SetChild1(new CSyntaxNode(NEXT_STA, VOIDTYPE));
	temp2->GetChild1()->SetChild0(new CSyntaxNode());
	temp2->GetChild1()->GetChild0()->CopyTree(pTree);

	pTree->SetNType(OR_EXP);
	pTree->GetChild0()->CopyTree(temp1);
	pTree->SetChild1(new CSyntaxNode());
	pTree->GetChild1()->CopyTree(temp2);

	NF(pTree);

	delete temp1;
	temp1 = NULL;
	delete temp2;
	temp2 = NULL;

}
//add by wm 2013/05/21  18:57
void	CSyntaxTree::__NfKeep(CSyntaxNode*pTree)
{
	if (pTree->GetChild0() == NULL)
	{
		cout << "__NFKeep error!" << endl;
		return;
	}
	//------------keep(b) ->  empty or b and ()keep(b) ------------
	CSyntaxNode*temp = new CSyntaxNode(AND_EXP, VOIDTYPE);

	//temp:b and ()keep(b)
	temp->SetChild0(new CSyntaxNode());
	temp->GetChild0()->CopyTree(pTree->GetChild0());
	temp->SetChild1(new CSyntaxNode(NEXT_STA, VOIDTYPE));
	temp->GetChild1()->SetChild0(new CSyntaxNode());
	temp->GetChild1()->GetChild0()->CopyTree(pTree);

	pTree->SetNType(OR_EXP);
	delete pTree->GetChild0();
	pTree->SetChild0(new CSyntaxNode(EMPTY_EXP, VOIDTYPE));
	pTree->SetChild1(new CSyntaxNode());
	pTree->GetChild1()->CopyTree(temp);

	NF(pTree);

	delete temp;
	temp = NULL;

}

//add by wm 2013/05/21  18:59
void	CSyntaxTree::__NfFinal(CSyntaxNode*pTree)
{
	if (pTree->GetChild0() == NULL)
	{
		cout << "__NFFinal error!" << endl;
		return;
	}
	//------------fin(b) -> b and empty or ()fin(b)------------
	CSyntaxNode*temp1 = new CSyntaxNode(AND_EXP, VOIDTYPE),
		*temp2 = new CSyntaxNode(NEXT_STA, VOIDTYPE);

	//temp1: b and empty
	temp1->SetChild0(new CSyntaxNode());
	temp1->GetChild0()->CopyTree(pTree->GetChild0());
	temp1->SetChild1(new CSyntaxNode(EMPTY_EXP, VOIDTYPE));

	//temp2: ()fin(b)
	temp2->SetChild0(new CSyntaxNode());
	temp2->GetChild0()->CopyTree(pTree);

	pTree->SetNType(OR_EXP);
	pTree->GetChild0()->CopyTree(temp1);
	pTree->SetChild1(new CSyntaxNode());
	pTree->GetChild1()->CopyTree(temp2);

	NF(pTree);

	delete temp1;
	temp1 = NULL;
	delete temp2;
	temp2 = NULL;


}
//add by wm 2013/05/21  19:05
void	CSyntaxTree::__NfHalt(CSyntaxNode*pTree)
{
	if (pTree->GetChild0() == NULL)
	{
		cout << "__NFAlwys error!" << endl;
		return;
	}

	//------------halt (b) -> b and empty or !b and ()halt(b)------------
	CSyntaxNode*temp1 = new CSyntaxNode(AND_EXP, VOIDTYPE),
		*temp2 = new CSyntaxNode(AND_EXP, VOIDTYPE);

	//temp1: b and empty
	temp1->SetChild0(new CSyntaxNode());
	temp1->GetChild0()->CopyTree(pTree->GetChild0());
	temp1->SetChild1(new CSyntaxNode(EMPTY_EXP, VOIDTYPE));

	//temp2:!b and  ()halt(b)
	temp2->SetChild0(new CSyntaxNode(NEGATION_EXP, BOOLTYPE));
	temp2->GetChild0()->SetChild0(new CSyntaxNode());
	temp2->GetChild0()->GetChild0()->CopyTree(pTree->GetChild0());
	temp2->SetChild1(new CSyntaxNode(NEXT_STA, VOIDTYPE));
	temp2->GetChild1()->SetChild0(new CSyntaxNode());
	temp2->GetChild1()->GetChild0()->CopyTree(pTree);

	pTree->SetNType(OR_EXP);
	pTree->GetChild0()->CopyTree(temp1);
	pTree->SetChild1(new CSyntaxNode());
	pTree->GetChild1()->CopyTree(temp2);

	delete temp1;
	temp1 = NULL;
	delete temp2;
	temp2 = NULL;

}
void CSyntaxTree::__NfAwait(CSyntaxNode*pTree)
{
	if (pTree->GetChild0() == NULL)
	{
		cout << "__NFAwait error!" << endl;
		return;
	}

	//------------await (b) -> b and empty or !b and ()await(b)------------
	CSyntaxNode*temp1 = new CSyntaxNode(AND_EXP, VOIDTYPE),
		*temp2 = new CSyntaxNode(AND_EXP, VOIDTYPE);

	//temp1: b and empty
	temp1->SetChild0(new CSyntaxNode());
	temp1->GetChild0()->CopyTree(pTree->GetChild0());
	temp1->SetChild1(new CSyntaxNode(EMPTY_EXP, VOIDTYPE));

	//temp2:!b and  ()await(b)
	temp2->SetChild0(new CSyntaxNode(NEGATION_EXP, BOOLTYPE));
	temp2->GetChild0()->SetChild0(new CSyntaxNode());
	temp2->GetChild0()->GetChild0()->CopyTree(pTree->GetChild0());
	temp2->SetChild1(new CSyntaxNode(NEXT_STA, VOIDTYPE));
	temp2->GetChild1()->SetChild0(new CSyntaxNode());
	temp2->GetChild1()->GetChild0()->CopyTree(pTree);

	pTree->SetNType(OR_EXP);
	pTree->GetChild0()->CopyTree(temp1);
	pTree->SetChild1(new CSyntaxNode());
	pTree->GetChild1()->CopyTree(temp2);

	delete temp1;
	temp1 = NULL;
	delete temp2;
	temp2 = NULL;
}

//programming by YY 2013/05/23 17:54
//在SyntaxTree.cpp最顶部定义全局变量NextChopStarTree，初始化为TRUE_EXP (内存原因)
//存储用or连接的Next_chop部分
void CSyntaxTree::__NfChopStar(CSyntaxNode *pTree)
{
	CSyntaxNode *temp1 = NULL, *temp2 = NULL, *temp3 = NULL;

	//用来保存待化简的chopstar树的复本  2013-5-25 add by YY
	CSyntaxNode *pTree_Copy = new CSyntaxNode();
	pTree_Copy->CopyTree(pTree);


	if (pTree->GetChild0() == NULL)
	{
		cout << " __NfChopStar error! " << endl;
		return;
	}

	NF(pTree->GetChild0());

	//将所有的Next部分化为next_chop部分，存储在全局变量NextChopStarTree中
	__NfChopStar_Next2Chop(pTree->GetChild0(), NextChopStarTree, pTree_Copy);

	//如果NextChopStarTree仍旧为TRUE_EXP，表明p#的p中不含有next部分，p#直接化成empty即可
	if (NextChopStarTree->GetNType() == TRUE_EXP)
	{
		temp1 = new CSyntaxNode(EMPTY_EXP, VOIDTYPE);
		pTree->CopyTree(temp1);

		//add by YY 2013/05/23 17:52  注意此处的内存 NextChopStarTree为全局变量
		delete temp1;
		temp1 = new CSyntaxNode(TRUE_EXP, VOIDTYPE);
		NextChopStarTree->CopyTree(temp1);
		delete temp1;
		temp1 = NULL;
		delete pTree_Copy;
		pTree_Copy = NULL;
		return;
	}
	//如果NextChopStarTree不再是为TRUE_EXP，表明p#的p中含有next部分，
	//再增加一个or节点连接NextChopStarTree和empty即可
	else
	{
		temp1 = new CSyntaxNode(OR_EXP, VOIDTYPE);
		temp1->SetChild0(new CSyntaxNode());
		temp1->GetChild0()->CopyTree(NextChopStarTree);
		temp1->SetChild1(new CSyntaxNode(EMPTY_EXP, VOIDTYPE));

		pTree->CopyTree(temp1);

		//add by YY 2013/05/23 17:52  注意此处的内存 NextChopStarTree为全局变量
		delete temp1;
		temp1 = new CSyntaxNode(TRUE_EXP, VOIDTYPE);
		NextChopStarTree->CopyTree(temp1);
		delete temp1;
		temp1 = NULL;
		delete pTree_Copy;
		pTree_Copy = NULL;
		return;
	}
}

//programming by YY 2013/05/23 17:54
//专门用于将所有的包含next的部分转化为next_chop
//所有的next_chop统一存储在全局变量NextChopStarTree中
//empty部分则不考虑
void CSyntaxTree::__NfChopStar_Next2Chop(CSyntaxNode *pTree, CSyntaxNode *NextChopTree, CSyntaxNode *OriginTree)
{
	CSyntaxNode *temp = NULL, *temp1 = NULL;
	if (pTree == NULL)
	{
		cout << " __NfChopStar_Next2Chop error!" << endl;
		return;
	}
	//()p
	if (pTree->GetNType() == NEXT_STA)
	{
		temp = new CSyntaxNode(NEXT_STA, VOIDTYPE);
		temp->SetChild0(new CSyntaxNode());

		temp1 = new CSyntaxNode(CHOP_STA, VOIDTYPE);
		temp1->SetChild0(new CSyntaxNode());
		temp1->GetChild0()->CopyTree(pTree->GetChild0());
		temp1->SetChild1(new CSyntaxNode());
		temp1->GetChild1()->CopyTree(OriginTree);

		temp->GetChild0()->CopyTree(temp1);

		delete temp1;
		temp1 = NULL;
	}
	//p and ()q
	else if (pTree->GetNType() == AND_EXP &&
		pTree->GetChild0() != NULL   &&
		pTree->GetChild1() != NULL   &&
		__IsStatFormula(pTree->GetChild0()) &&
		pTree->GetChild1()->GetNType() == NEXT_STA)
	{
		temp = new CSyntaxNode(AND_EXP, VOIDTYPE);
		temp->SetChild0(new CSyntaxNode());
		temp->GetChild0()->CopyTree(pTree->GetChild0());
		temp->SetChild1(new CSyntaxNode(NEXT_STA, VOIDTYPE));
		temp->GetChild1()->SetChild0(new CSyntaxNode());

		temp1 = new CSyntaxNode(CHOP_STA, VOIDTYPE);
		temp1->SetChild0(new CSyntaxNode());
		temp1->GetChild0()->CopyTree(pTree->GetChild1()->GetChild0());
		temp1->SetChild1(new CSyntaxNode());
		temp1->GetChild1()->CopyTree(OriginTree);

		temp->GetChild1()->GetChild0()->CopyTree(temp1);

		delete temp1;
		temp1 = NULL;
	}
	//()q and p
	else if (pTree->GetNType() == AND_EXP &&
		pTree->GetChild0() != NULL   &&
		pTree->GetChild1() != NULL   &&
		__IsStatFormula(pTree->GetChild1()) &&
		pTree->GetChild0()->GetNType() == NEXT_STA)
	{
		temp = new CSyntaxNode(AND_EXP, VOIDTYPE);
		temp->SetChild0(new CSyntaxNode());
		temp->GetChild0()->CopyTree(pTree->GetChild1());
		temp->SetChild1(new CSyntaxNode(NEXT_STA, VOIDTYPE));
		temp->GetChild1()->SetChild0(new CSyntaxNode());

		temp1 = new CSyntaxNode(CHOP_STA, VOIDTYPE);
		temp1->SetChild0(new CSyntaxNode());
		temp1->GetChild0()->CopyTree(pTree->GetChild0()->GetChild0());
		temp1->SetChild1(new CSyntaxNode());
		temp1->GetChild1()->CopyTree(OriginTree);

		temp->GetChild1()->GetChild0()->CopyTree(temp1);

		delete temp1;
		temp1 = NULL;
	}
	//temp部分用来存储以上三种next情况时，所化成的nextchop部分
	if (temp != NULL)
	{
		if (NextChopTree->GetNType() == TRUE_EXP)
		{
			NextChopTree->CopyTree(temp);
			delete temp;
			temp = NULL;
			return;
		}
		else
		{
			temp1 = new CSyntaxNode(OR_EXP, VOIDTYPE);
			temp1->SetChild0(new CSyntaxNode());
			temp1->GetChild0()->CopyTree(NextChopTree);
			temp1->SetChild1(new CSyntaxNode());
			temp1->GetChild1()->CopyTree(temp);

			NextChopTree->CopyTree(temp1);
			delete temp;
			temp = NULL;
			delete temp1;
			temp1 = NULL;
			return;
		}
	}
	if (pTree->GetChild0() != NULL)
	{
		__NfChopStar_Next2Chop(pTree->GetChild0(), NextChopTree, OriginTree);
	}
	if (pTree->GetChild1() != NULL)
	{
		__NfChopStar_Next2Chop(pTree->GetChild1(), NextChopTree, OriginTree);
	}
}


//add by YY 2013/05/20 10:30
void CSyntaxTree::__NfOr(CSyntaxNode *pTree)
{
	if (pTree->GetChild0() == NULL || pTree->GetChild1() == NULL)
	{
		cout << " __NfOr error!" << endl;
		return;
	}
	NF(pTree->GetChild0());
	NF(pTree->GetChild1());
}

//anotated by YY 2013/06/04 21:55

//add by YY 2013/05/14 17:40
void CSyntaxTree::__NfSomtims(CSyntaxNode *pTree)
{

	if (pTree->GetChild0() == NULL)
	{
		cout << "__NFSomtims error!" << endl;
		return;
	}

	//------------som p -> p or ()som p------------

	//temp1: p   temp2: ()som p
	CSyntaxNode*temp1 = new CSyntaxNode(),
		*temp2 = new CSyntaxNode(NEXT_STA, VOIDTYPE);
	temp1->CopyTree(pTree->GetChild0());

	temp2->SetChild0(new CSyntaxNode());
	temp2->GetChild0()->CopyTree(pTree);

	pTree->SetNType(OR_EXP);
	pTree->GetChild0()->CopyTree(temp1);
	pTree->SetChild1(new CSyntaxNode());
	pTree->GetChild1()->CopyTree(temp2);

	//对p进行NF，()som p已经是NF形式
	NF(pTree->GetChild0());
	delete temp1;
	temp1 = NULL;
	delete temp2;
	temp2 = NULL;
}

//add by YY 2013/05/14 11:42
//programming by CJ 2013/05/14 11:42
void	CSyntaxTree::__NfTrue(CSyntaxNode *pTree)
{
	pTree->SetNType(OR_EXP);
	pTree->SetChild0(new CSyntaxNode(EMPTY_EXP, VOIDTYPE));
	pTree->SetChild1(new CSyntaxNode(NEXT_STA, VOIDTYPE));
	pTree->GetChild1()->SetChild0(new CSyntaxNode(TRUE_EXP, VOIDTYPE));
}





//add by YY 2013/05/15 9:53
//programming by YY 2013/05/15 9:53
void	CSyntaxTree::__NfChop(CSyntaxNode *pTree)
{
	if (pTree->GetChild0() == NULL || pTree->GetChild1() == NULL)
	{
		cout << " __NfChop error!" << endl;
		return;
	}

	CSyntaxNode *temp = NULL, *temp1 = NULL, *temp2 = NULL;
	//add by YY 2013/06/04 22:16
	//programming by WM 2013/06/04 22:16
	//------------------------false;p --> false  -------------------------------
	//------------------------p; false -->false  -------------------------------
	//added by wm 2013/05/30 15:40
	if (pTree->GetChild0()->GetNType() == FALSE_EXP ||
		pTree->GetChild1()->GetNType() == FALSE_EXP)
	{
		delete pTree->GetChild0();
		pTree->SetChild0(NULL);
		delete pTree->GetChild1();
		pTree->SetChild1(NULL);
		pTree->SetNType(FALSE_EXP);
		return;
	}
	//------------------------------empty;q --> q --------------------------------------
	if (pTree->GetChild0()->GetNType() == EMPTY_EXP)
	{
		temp = new CSyntaxNode();
		temp->CopyTree(pTree->GetChild1());
		pTree->CopyTree(temp);
		NF(pTree);
		delete temp;
		temp = NULL;
		return;
	}
	//------------------------------()p;q --> ()(p;q)-----------------------------------
	else if (pTree->GetChild0()->GetNType() == NEXT_STA && pTree->GetChild0()->GetChild0() != NULL)
	{
		temp1 = new CSyntaxNode(CHOP_STA, VOIDTYPE);
		temp1->SetChild0(new CSyntaxNode());
		temp1->SetChild1(new CSyntaxNode());
		temp1->GetChild0()->CopyTree(pTree->GetChild0()->GetChild0());
		temp1->GetChild1()->CopyTree(pTree->GetChild1());

		temp = new CSyntaxNode(NEXT_STA, VOIDTYPE);
		temp->SetChild0(new CSyntaxNode());
		temp->GetChild0()->CopyTree(temp1);

		pTree->CopyTree(temp);

		delete temp;
		temp = NULL;
		delete temp1;
		temp1 = NULL;
	}
	//------------------------------w && p;q --> NF(w && CHOP(p;q))---------------------
	else if (pTree->GetChild0()->GetNType() == AND_EXP	     &&
		pTree->GetChild0()->GetChild0() != NULL			 &&
		pTree->GetChild0()->GetChild1() != NULL			 &&
		__IsStatFormula(pTree->GetChild0()->GetChild0()) &&
		!__IsStatFormula(pTree->GetChild0()->GetChild1()))
	{
		temp1 = new CSyntaxNode(CHOP_STA, VOIDTYPE);
		temp1->SetChild0(new CSyntaxNode());
		temp1->SetChild1(new CSyntaxNode());
		temp1->GetChild0()->CopyTree(pTree->GetChild0()->GetChild1());
		temp1->GetChild1()->CopyTree(pTree->GetChild1());
		__NfChop(temp1);

		temp = new CSyntaxNode(AND_EXP, VOIDTYPE);
		temp->SetChild0(new CSyntaxNode());
		temp->SetChild1(new CSyntaxNode());
		temp->GetChild0()->CopyTree(pTree->GetChild0()->GetChild0());
		temp->GetChild1()->CopyTree(temp1);

		pTree->CopyTree(temp);
		NF(pTree);

		delete temp;
		temp = NULL;
		delete temp1;
		temp1 = NULL;
		return;
	}
	//------------------------------p && w;q --> NF(w && CHOP(p;q))---------------------
	else if (pTree->GetChild0()->GetNType() == AND_EXP		  &&
		pTree->GetChild0()->GetChild0() != NULL			  &&
		pTree->GetChild0()->GetChild1() != NULL			  &&
		!__IsStatFormula(pTree->GetChild0()->GetChild0()) &&
		__IsStatFormula(pTree->GetChild0()->GetChild1()))
	{
		temp1 = new CSyntaxNode(CHOP_STA, VOIDTYPE);
		temp1->SetChild0(new CSyntaxNode());
		temp1->SetChild1(new CSyntaxNode());
		temp1->GetChild0()->CopyTree(pTree->GetChild0()->GetChild0());
		temp1->GetChild1()->CopyTree(pTree->GetChild1());
		__NfChop(temp1);

		temp = new CSyntaxNode(AND_EXP, VOIDTYPE);
		temp->SetChild0(new CSyntaxNode());
		temp->SetChild1(new CSyntaxNode());
		temp->GetChild0()->CopyTree(pTree->GetChild0()->GetChild1());
		temp->GetChild1()->CopyTree(temp1);

		pTree->CopyTree(temp);
		NF(pTree);

		delete temp;
		temp = NULL;
		delete temp1;
		temp1 = NULL;
		return;
	}
	//------------------------------p1||p2;q --> CHOP(p1;q)||CHOP(p2;q))---------------------
	else if (pTree->GetChild0()->GetNType() == OR_EXP &&
		pTree->GetChild0()->GetChild0() != NULL  &&
		pTree->GetChild0()->GetChild1() != NULL)
	{
		temp1 = new CSyntaxNode(CHOP_STA, VOIDTYPE);
		temp1->SetChild0(new CSyntaxNode());
		temp1->SetChild1(new CSyntaxNode());
		temp1->GetChild0()->CopyTree(pTree->GetChild0()->GetChild0());
		temp1->GetChild1()->CopyTree(pTree->GetChild1());
		__NfChop(temp1);

		temp2 = new CSyntaxNode(CHOP_STA, VOIDTYPE);
		temp2->SetChild0(new CSyntaxNode());
		temp2->SetChild1(new CSyntaxNode());
		temp2->GetChild0()->CopyTree(pTree->GetChild0()->GetChild1());
		temp2->GetChild1()->CopyTree(pTree->GetChild1());
		__NfChop(temp2);

		temp = new CSyntaxNode(OR_EXP, VOIDTYPE);
		temp->SetChild0(new CSyntaxNode());
		temp->SetChild1(new CSyntaxNode());
		temp->GetChild0()->CopyTree(temp1);
		temp->GetChild1()->CopyTree(temp2);

		pTree->CopyTree(temp);

		__PreOr(pTree);

		delete temp;
		temp = NULL;
		delete temp1;
		temp1 = NULL;
		delete temp2;
		temp2 = NULL;
	}
	else
	{
		NF(pTree->GetChild0());
		__NfChop(pTree);
		return;
	}
}


//add by YY 2013/05/15 11:06
//programming by YY 2013/05/20 10:04
void	CSyntaxTree::__NfPrj(CSyntaxNode *pTree)
{
	if (pTree->GetChild0() == NULL || this == NULL)
	{
		cout << "__NfPrj error!" << endl;
		return;

	}
	CSyntaxNode *temp = NULL, *temp1 = NULL, *temp2 = NULL;

	//add by YY 2013/05/16 9:50     这条case可有可没有
	//----------------------- e prj e --> e ---------------------------------------
	if (pTree->GetChild0() != NULL && pTree->GetChild0()->GetNType() == EMPTY_EXP &&
		pTree->GetChild2() != NULL && pTree->GetChild2()->GetNType() == EMPTY_EXP &&
		pTree->GetChild1() == NULL)
	{
		temp = new CSyntaxNode(EMPTY_EXP, VOIDTYPE);
		pTree->CopyTree(temp);
		delete temp;
		temp = NULL;
		return;
	}
	//add by YY 2013/05/16 9:50     这条case可有可没有
	//-----------------------e prj Q -->NF(Q) -----------------------------------------------
	else if (pTree->GetChild0() != NULL && pTree->GetChild0()->GetNType() == EMPTY_EXP &&
		pTree->GetChild1() == NULL &&
		pTree->GetChild2() != NULL && pTree->GetChild2()->GetNType() != EMPTY_EXP)

	{
		temp = new CSyntaxNode();
		temp->CopyTree(pTree->GetChild2());
		pTree->CopyTree(temp);
		NF(pTree);
		delete temp;
		temp = NULL;
		return;
	}
	//---------------------- P prj e --> NF(p) ---------------------------------------------
	else if (pTree->GetChild0() != NULL && pTree->GetChild0()->GetNType() != EMPTY_EXP &&
		pTree->GetChild2() != NULL && pTree->GetChild2()->GetNType() == EMPTY_EXP &&
		pTree->GetChild1() == NULL)
	{
		temp = new CSyntaxNode();
		temp->CopyTree(pTree->GetChild0());
		pTree->CopyTree(temp);
		NF(pTree);
		delete temp;
		temp = NULL;
		return;
	}
	//-----------------------()P prj ()Q --> ()(P;Q) ---------------------------------------
	else if (pTree->GetChild0() != NULL && pTree->GetChild0()->GetNType() == NEXT_STA &&
		pTree->GetChild2() != NULL && pTree->GetChild2()->GetNType() == NEXT_STA &&
		pTree->GetChild1() == NULL)
	{
		temp = new CSyntaxNode(NEXT_STA, VOIDTYPE);
		temp->SetChild0(new CSyntaxNode(CHOP_STA, VOIDTYPE));
		temp->GetChild0()->SetChild0(new CSyntaxNode());
		temp->GetChild0()->SetChild1(new CSyntaxNode());
		temp->GetChild0()->GetChild0()->CopyTree(pTree->GetChild0()->GetChild0());
		temp->GetChild0()->GetChild1()->CopyTree(pTree->GetChild2()->GetChild0());
		pTree->CopyTree(temp);
		delete temp;
		temp = NULL;
		return;
	}
	//-------------------( ()p1,...,pm ) prj ()Q- --> ()(p1;((p2,...,pm) prj Q)) -----------
	else if (pTree->GetChild0() != NULL && pTree->GetChild0()->GetNType() == NEXT_STA &&
		pTree->GetChild2() != NULL && pTree->GetChild2()->GetNType() == NEXT_STA &&
		pTree->GetChild1() != NULL)
	{
		//(p2,...,pm) prj Q)
		temp = new CSyntaxNode();
		temp->CopyTree(pTree->GetChild1());
		temp->SetChild2(new CSyntaxNode());
		temp->GetChild2()->CopyTree(pTree->GetChild2()->GetChild0());


		//()(p1;((p2,...,pm) prj Q))
		temp1 = new CSyntaxNode(NEXT_STA, VOIDTYPE);
		temp1->SetChild0(new CSyntaxNode(CHOP_STA, VOIDTYPE));
		temp1->GetChild0()->SetChild0(new CSyntaxNode());
		temp1->GetChild0()->SetChild1(new CSyntaxNode());
		temp1->GetChild0()->GetChild0()->CopyTree(pTree->GetChild0()->GetChild0());
		temp1->GetChild0()->GetChild1()->CopyTree(temp);

		pTree->CopyTree(temp1);

		delete temp;
		temp = NULL;
		delete temp1;
		temp1 = NULL;
		return;

	}
	//----------------------------(e,p2,...,pm) prj Q --> PRJ((p2,...,pm) prj Q) -----------
	else if (pTree->GetChild0() != NULL && pTree->GetChild0()->GetNType() == EMPTY_EXP &&
		pTree->GetChild1() != NULL && pTree->GetChild2() != NULL)
	{
		temp = new CSyntaxNode();
		temp->CopyTree(pTree->GetChild1());
		temp->SetChild2(new CSyntaxNode());
		temp->GetChild2()->CopyTree(pTree->GetChild2());

		pTree->CopyTree(temp);
		__NfPrj(pTree);

		delete temp;
		temp = NULL;
		return;
	}

	//-------------------------(p1,...,pm) prj e --> (p1;(p2;(...;pm)))---------------------
	else if (pTree->GetChild2() != NULL && pTree->GetChild2()->GetNType() == EMPTY_EXP &&
		pTree->GetChild0() != NULL && pTree->GetChild1() != NULL)
	{
		//(p1,p2)  prj Q  -->  p1;p2
		if (pTree->GetChild1()->GetChild1() == NULL)
		{
			temp = new CSyntaxNode(CHOP_STA, VOIDTYPE);
			temp->SetChild0(new CSyntaxNode());
			temp->SetChild1(new CSyntaxNode());
			temp->GetChild0()->CopyTree(pTree->GetChild0());
			temp->GetChild1()->CopyTree(pTree->GetChild1()->GetChild0());

			pTree->CopyTree(temp);
			__NfChop(pTree);
			return;
		}
		else
		{
			temp = new CSyntaxNode(CHOP_STA, VOIDTYPE);
			temp->SetChild0(new CSyntaxNode());
			temp1 = pTree;

			//保留chop树指针
			temp2 = temp;
			temp->GetChild0()->CopyTree(temp1->GetChild0());
			temp1 = temp1->GetChild1();
			while (temp1 != NULL)
			{

				if (temp1->GetChild1()->GetChild1() != NULL)
				{
					temp->SetChild1(new CSyntaxNode(CHOP_STA, VOIDTYPE));
					temp = temp->GetChild1();
					temp->SetChild0(new CSyntaxNode());
					temp->GetChild0()->CopyTree(temp1->GetChild0());
					temp1 = temp1->GetChild1();
				}
				else
				{
					temp->SetChild1(new CSyntaxNode(CHOP_STA, VOIDTYPE));
					temp = temp->GetChild1();
					temp->SetChild0(new CSyntaxNode());
					temp->SetChild1(new CSyntaxNode());
					temp->GetChild0()->CopyTree(temp1->GetChild0());
					temp->GetChild1()->CopyTree(temp1->GetChild1()->GetChild0());
					break;

				}

			}
			pTree->CopyTree(temp2);
			__NfChop(pTree);
			return;
		}


	}
	//------------------(w && p1,...,pm) prj Q --> NF(w && PRJ((p1,...,pm) prj Q))----------
	else if (pTree->GetChild0() != NULL && pTree->GetChild0()->GetNType() == AND_EXP &&
		pTree->GetChild0()->GetChild0() != NULL &&  __IsStatFormula(pTree->GetChild0()->GetChild0()) &&
		pTree->GetChild0()->GetChild1() != NULL && !__IsStatFormula(pTree->GetChild0()->GetChild1()) &&
		/*pTree->GetChild1()!=NULL &&*/ pTree->GetChild2() != NULL)
	{
		//(p1,...,pm) prj Q
		temp = new CSyntaxNode(PROJECTION_STA, VOIDTYPE);
		temp->SetChild0(new CSyntaxNode());
		temp->GetChild0()->CopyTree(pTree->GetChild0()->GetChild1());
		if (pTree->GetChild1() != NULL)
		{
			temp->SetChild1(new CSyntaxNode());
			temp->GetChild1()->CopyTree(pTree->GetChild1());
		}
		temp->SetChild2(new CSyntaxNode());
		temp->GetChild2()->CopyTree(pTree->GetChild2());
		__NfPrj(temp);

		//w && (p1,...,pm) prj Q
		temp1 = new CSyntaxNode(AND_EXP, VOIDTYPE);
		temp1->SetChild0(new CSyntaxNode());
		temp1->GetChild0()->CopyTree(pTree->GetChild0()->GetChild0());
		temp1->SetChild1(new CSyntaxNode());
		temp1->GetChild1()->CopyTree(temp);
		pTree->CopyTree(temp1);
		NF(pTree);

		delete temp;
		temp = NULL;

		delete temp1;
		temp1 = NULL;
		return;

	}
	//------------------(p1 && w,...,pm) prj Q --> NF(w && PRJ((p1,...,pm) prj Q))----------
	else if (pTree->GetChild0() != NULL && pTree->GetChild0()->GetNType() == AND_EXP &&
		pTree->GetChild0()->GetChild0() != NULL && !__IsStatFormula(pTree->GetChild0()->GetChild0()) &&
		pTree->GetChild0()->GetChild1() != NULL && __IsStatFormula(pTree->GetChild0()->GetChild1()) &&
		/*pTree->GetChild1()!=NULL &&*/ pTree->GetChild2() != NULL)
	{
		//(p1,...,pm) prj Q
		temp = new CSyntaxNode(PROJECTION_STA, VOIDTYPE);
		temp->SetChild0(new CSyntaxNode());
		temp->GetChild0()->CopyTree(pTree->GetChild0()->GetChild0());
		if (pTree->GetChild1() != NULL)
		{
			temp->SetChild1(new CSyntaxNode());
			temp->GetChild1()->CopyTree(pTree->GetChild1());
		}
		temp->SetChild2(new CSyntaxNode());
		temp->GetChild2()->CopyTree(pTree->GetChild2());
		__NfPrj(temp);

		//w && (p1,...,pm) prj Q
		temp1 = new CSyntaxNode(AND_EXP, VOIDTYPE);
		temp1->SetChild0(new CSyntaxNode());
		temp1->GetChild0()->CopyTree(pTree->GetChild0()->GetChild1());
		temp1->SetChild1(new CSyntaxNode());
		temp1->GetChild1()->CopyTree(temp);

		pTree->CopyTree(temp1);
		NF(pTree);

		delete temp;
		temp = NULL;
		delete temp1;
		temp1 = NULL;
		return;
	}
	//------------------(p1 ,...,pm) prj (w && Q) --> NF(w && PRJ((p1,...,pm) prj Q))----------
	else if (pTree->GetChild2() != NULL && pTree->GetChild2()->GetNType() == AND_EXP &&
		pTree->GetChild2()->GetChild0() != NULL && __IsStatFormula(pTree->GetChild2()->GetChild0()) &&
		pTree->GetChild2()->GetChild1() != NULL && !__IsStatFormula(pTree->GetChild2()->GetChild1()) &&
		pTree->GetChild0() != NULL /*&& pTree->GetChild1()!=NULL*/)
	{
		//(p1,...,pm) prj Q
		temp = new CSyntaxNode(PROJECTION_STA, VOIDTYPE);
		temp->SetChild0(new CSyntaxNode());
		temp->GetChild0()->CopyTree(pTree->GetChild0());
		if (pTree->GetChild1() != NULL)
		{
			temp->SetChild1(new CSyntaxNode());
			temp->GetChild1()->CopyTree(pTree->GetChild1());
		}
		temp->SetChild2(new CSyntaxNode());
		temp->GetChild2()->CopyTree(pTree->GetChild2()->GetChild1());
		__NfPrj(temp);

		//w && (p1,...,pm) prj Q
		temp1 = new CSyntaxNode(AND_EXP, VOIDTYPE);
		temp1->SetChild0(new CSyntaxNode());
		temp1->GetChild0()->CopyTree(pTree->GetChild2()->GetChild0());
		temp1->SetChild1(new CSyntaxNode());
		temp1->GetChild1()->CopyTree(temp);

		pTree->CopyTree(temp1);
		NF(pTree);

		delete temp;
		temp = NULL;
		delete temp1;
		temp1 = NULL;
		return;
	}
	//------------------(p1 ,...,pm) prj (Q && w) --> NF(w && PRJ((p1,...,pm) prj Q))----------
	else if (pTree->GetChild2() != NULL && pTree->GetChild2()->GetNType() == AND_EXP &&
		pTree->GetChild2()->GetChild0() != NULL && !__IsStatFormula(pTree->GetChild2()->GetChild0()) &&
		pTree->GetChild2()->GetChild1() != NULL && __IsStatFormula(pTree->GetChild2()->GetChild1()) &&
		pTree->GetChild0() != NULL /*&& pTree->GetChild1()!=NULL*/)
	{
		//(p1,...,pm) prj Q
		temp = new CSyntaxNode(PROJECTION_STA, VOIDTYPE);
		temp->SetChild0(new CSyntaxNode());
		temp->GetChild0()->CopyTree(pTree->GetChild0());
		if (pTree->GetChild1() != NULL)
		{
			temp->SetChild1(new CSyntaxNode());
			temp->GetChild1()->CopyTree(pTree->GetChild1());
		}
		temp->SetChild2(new CSyntaxNode());
		temp->GetChild2()->CopyTree(pTree->GetChild2()->GetChild0());
		__NfPrj(temp);

		//w && (p1,...,pm) prj Q
		temp1 = new CSyntaxNode(AND_EXP, VOIDTYPE);
		temp1->SetChild0(new CSyntaxNode());
		temp1->GetChild0()->CopyTree(pTree->GetChild2()->GetChild1());
		temp1->SetChild1(new CSyntaxNode());
		temp1->GetChild1()->CopyTree(temp);

		pTree->CopyTree(temp1);
		NF(pTree);

		delete temp;
		temp = NULL;
		delete temp1;
		temp1 = NULL;
		return;
	}
	//---(p1 || p1',...,pm) prj Q --> PRJ((p1,...,pm) prj Q)) || PRJ((p1',...,pm) prj Q))---
	else if (pTree->GetChild0() != NULL  && pTree->GetChild0()->GetNType() == OR_EXP &&
		pTree->GetChild0()->GetChild0() != NULL && pTree->GetChild0()->GetChild1() != NULL &&
		/*pTree->GetChild1()!=NULL &&*/ pTree->GetChild2() != NULL)
	{
		temp1 = new CSyntaxNode(PROJECTION_STA, VOIDTYPE);
		temp1->SetChild0(new CSyntaxNode());
		temp1->GetChild0()->CopyTree(pTree->GetChild0()->GetChild0());
		if (pTree->GetChild1() != NULL)
		{
			temp1->SetChild1(new CSyntaxNode());
			temp1->GetChild1()->CopyTree(pTree->GetChild1());
		}
		temp1->SetChild2(new CSyntaxNode());
		temp1->GetChild2()->CopyTree(pTree->GetChild2());
		__NfPrj(temp1);

		temp2 = new CSyntaxNode(PROJECTION_STA, VOIDTYPE);
		temp2->SetChild0(new CSyntaxNode());
		temp2->GetChild0()->CopyTree(pTree->GetChild0()->GetChild1());
		if (pTree->GetChild1() != NULL)
		{
			temp2->SetChild1(new CSyntaxNode());
			temp2->GetChild1()->CopyTree(pTree->GetChild1());
		}
		temp2->SetChild2(new CSyntaxNode());
		temp2->GetChild2()->CopyTree(pTree->GetChild2());
		__NfPrj(temp2);

		temp = new CSyntaxNode(OR_EXP, VOIDTYPE);
		temp->SetChild0(new CSyntaxNode());
		temp->SetChild1(new CSyntaxNode());
		temp->GetChild0()->CopyTree(temp1);
		temp->GetChild1()->CopyTree(temp2);

		pTree->CopyTree(temp);
		delete temp1;
		temp1 = NULL;
		delete temp2;
		temp2 = NULL;
		return;
	}
	//---(p1,...,pm) prj (Q||Q') --> PRJ((p1,...,pm) prj Q)) || PRJ((p1,...,pm) prj Q'))---
	else if (pTree->GetChild2() != NULL  && pTree->GetChild2()->GetNType() == OR_EXP &&
		pTree->GetChild2()->GetChild0() != NULL && pTree->GetChild2()->GetChild1() != NULL &&
		pTree->GetChild0() != NULL /*&& pTree->GetChild1()!=NULL*/)
	{
		temp1 = new CSyntaxNode(PROJECTION_STA, VOIDTYPE);
		temp1->SetChild0(new CSyntaxNode());
		temp1->GetChild0()->CopyTree(pTree->GetChild0());
		if (pTree->GetChild1() != NULL)
		{
			temp1->SetChild1(new CSyntaxNode());
			temp1->GetChild1()->CopyTree(pTree->GetChild1());
		}
		temp1->SetChild2(new CSyntaxNode());
		temp1->GetChild2()->CopyTree(pTree->GetChild2()->GetChild0());
		__NfPrj(temp1);

		temp2 = new CSyntaxNode(PROJECTION_STA, VOIDTYPE);
		temp2->SetChild0(new CSyntaxNode());
		temp2->GetChild0()->CopyTree(pTree->GetChild0());
		if (pTree->GetChild1() != NULL)
		{
			temp2->SetChild1(new CSyntaxNode());
			temp2->GetChild1()->CopyTree(pTree->GetChild1());
		}
		temp2->SetChild2(new CSyntaxNode());
		temp2->GetChild2()->CopyTree(pTree->GetChild2()->GetChild1());
		__NfPrj(temp2);

		temp = new CSyntaxNode(OR_EXP, VOIDTYPE);
		temp->SetChild0(new CSyntaxNode());
		temp->SetChild1(new CSyntaxNode());
		temp->GetChild0()->CopyTree(temp1);
		temp->GetChild1()->CopyTree(temp2);

		pTree->CopyTree(temp);
		delete temp1;
		temp1 = NULL;
		delete temp2;
		temp2 = NULL;
		return;
	}
	//----------------------return:PRJ((NF(p1),...,pm) prj NF(Q))---------------------------
	else
	{
		NF(pTree->GetChild0());
		if (pTree->GetChild2() == NULL)
		{
			cout << "__NfPrj child2 can't be NULL!" << endl;
			return;
		}
		else
		{
			NF(pTree->GetChild2());
			__NfPrj(pTree);
		}

		return;
	}

}

//add by YY 2013/06/06 21:44
//------------------------------------------------------------------------------
//add by wm 2013/06/06  19:21
void CSyntaxTree::__PreUnAssign(CSyntaxNode*pTree)
{
	if (pTree->GetChild0() == NULL || pTree->GetChild1() == NULL)
	{
		cout << "__PreUnAssign error!" << endl;
		return;
	}

	//x:=y  -> ()(x<==(-) (y) and empty)
	CSyntaxNode*temp1 = new CSyntaxNode(),
		*temp2 = new CSyntaxNode();
	//temp1: x
	temp1->CopyTree(pTree->GetChild0());
	//temp2: y
	temp2->CopyTree(pTree->GetChild1());

	delete pTree->GetChild0();
	delete pTree->GetChild1();
	pTree->SetChild1(NULL);
	pTree->SetNType(NEXT_STA);
	pTree->SetChild0(new CSyntaxNode(AND_EXP, VOIDTYPE));
	pTree->GetChild0()->SetChild0(new CSyntaxNode(ASS_EQU_EXP, VOIDTYPE));
	pTree->GetChild0()->GetChild0()->SetChild0(new CSyntaxNode());
	pTree->GetChild0()->GetChild0()->GetChild0()->CopyTree(temp1);
	pTree->GetChild0()->GetChild0()->SetChild1(new CSyntaxNode(PREVIOUS_STA, VOIDTYPE));
	pTree->GetChild0()->GetChild0()->GetChild1()->SetChild0(new CSyntaxNode());
	pTree->GetChild0()->GetChild0()->GetChild1()->GetChild0()->CopyTree(temp2);

	pTree->GetChild0()->SetChild1(new CSyntaxNode(EMPTY_EXP, VOIDTYPE));
	delete temp1;
	temp1 = NULL;
	delete temp2;
	temp2 = NULL;
}
//------------------------------------------------------------------------------
//programming by wm 2013/06/06  19:37
void CSyntaxTree::__PreAssEQ(CSyntaxNode*pTree)
{
	if (pTree->GetChild0() == NULL || pTree->GetChild1() == NULL)
	{
		cout << "__PreAssEQ error!" << endl;
		return;
	}
	//()x<==y  ->  ()(x<==pre y)
	if (pTree->GetChild0()->GetNType() == NEXT_STA)
	{
		if (pTree->GetChild0()->GetChild0() == NULL)
		{
			cout << "__PreAssEQ error!" << endl;
			return;
		}
		CSyntaxNode *temp1 = new CSyntaxNode(),
			*temp2 = new CSyntaxNode();
		//temp1: x
		temp1->CopyTree(pTree->GetChild0()->GetChild0());
		//temp2: y
		temp2->CopyTree(pTree->GetChild1());

		delete pTree->GetChild0();
		delete pTree->GetChild1();
		pTree->SetChild1(NULL);
		pTree->SetNType(NEXT_STA);
		pTree->SetChild0(new CSyntaxNode(ASS_EQU_EXP, VOIDTYPE));
		pTree->GetChild0()->SetChild0(new CSyntaxNode());
		pTree->GetChild0()->GetChild0()->CopyTree(temp1);
		pTree->GetChild0()->SetChild1(new CSyntaxNode(PREVIOUS_STA, VOIDTYPE));
		pTree->GetChild0()->GetChild1()->SetChild0(new CSyntaxNode());
		pTree->GetChild0()->GetChild1()->GetChild0()->CopyTree(temp2);
		delete temp1;
		temp1 = NULL;
		delete temp2;
		temp2 = NULL;
	}
}
//add by YY[fixed] 2013/05/14 11:42
//programming by CJ 2013/05/14 11:41
bool	CSyntaxTree::__IsStatFormula(CSyntaxNode *pTree)
{
	if (pTree == NULL)
	{
		cout << " __IsFormula error!" << endl;
		return false;
		//exit(-1);
	}
	else if (pTree->GetNType() == EMPTY_EXP || pTree->GetNType() == NEXT_STA || pTree->GetNType() == MORE_STA || pTree->GetNType() == ALWAYS_STA
		|| pTree->GetNType() == SKIP_STA || pTree->GetNType() == LENGTH_STA || pTree->GetNType() == SOMETIMES_STA
		|| pTree->GetNType() == ALWAYS_STA || pTree->GetNType() == PROJECTION_STA || pTree->GetNType() == CHOP_STA
		|| pTree->GetNType() == CHOPSTAR_STA || pTree->GetNType() == FINAL_STA || pTree->GetNType() == FALSE_EXP
		|| pTree->GetNType() == HALT_STA || pTree->GetNType() == AWAIT_STA || pTree->GetNType() == TRUE_EXP
		|| pTree->GetNType() == KEEP_STA || pTree->GetNType() == UNITASSIGN_EXP || pTree->GetNType() == WHILE_DO_STA
		|| pTree->GetNType() == IF_ELSE_EXP)
		//add by wm 2013/08/19  11:57
	{
		return false;
	}

	if (pTree->GetChild0() != NULL)
	{
		//Modified by wm 2013/05/14  21:50
		if (__IsStatFormula(pTree->GetChild0()) == false)
		{
			return false;
		}

	}

	if (pTree->GetChild1() != NULL)
	{
		//Modified by wm 2013/05/14  21:50
		if (__IsStatFormula(pTree->GetChild1()) == false)
		{
			return false;
		}
	}

	if (pTree->GetChild2() != NULL)
	{
		//Modified by wm 2013/05/14  21:50
		if (__IsStatFormula(pTree->GetChild2()) == false)
		{
			return false;
		}
	}

	return true;

}

//add by YY[fixed] 2013/05/14 11:38
//programming by CJ 2013/05/14 11:38
void	CSyntaxTree::Dnf(CSyntaxNode *pTree)
{
	DnfNeg(pTree);
	DnfAnd(pTree);
	return;
}

//add by YY[fixed] 2013/05/14 11:38
//programming by CJ 2013/05/14 11:38
void	CSyntaxTree::DnfNeg(CSyntaxNode *pTree)
{
	CSyntaxNode *temp1 = NULL, *temp2 = NULL;
	if (pTree == NULL)
	{
		cout << " DnfNeg error!" << endl;
		return;
	}
	if (pTree->GetNType() == AND_EXP || pTree->GetNType() == OR_EXP)
	{
		DnfNeg(pTree->GetChild0());
		DnfNeg(pTree->GetChild1());
		return;
	}
	//!(p and q) --> !p or !q
	if (pTree->GetNType() == NEGATION_EXP && pTree->GetChild0()->GetNType() == AND_EXP)
	{
		temp1 = new CSyntaxNode(NEGATION_EXP, BOOLTYPE);
		temp1->SetChild0(new CSyntaxNode());
		temp1->GetChild0()->CopyTree(pTree->GetChild0()->GetChild0());

		temp2 = new CSyntaxNode(NEGATION_EXP, BOOLTYPE);
		temp2->SetChild0(new CSyntaxNode());
		temp2->GetChild0()->CopyTree(pTree->GetChild0()->GetChild1());

		pTree->SetNType(OR_EXP);
		pTree->GetChild0()->CopyTree(temp1);

		pTree->SetChild1(new CSyntaxNode());
		pTree->GetChild1()->CopyTree(temp2);

		delete temp1;
		delete temp2;
		temp1 = NULL;
		temp2 = NULL;

		DnfNeg(pTree);
		return;
	}
	//!(p or q) --> !p and !q
	if (pTree->GetNType() == NEGATION_EXP && pTree->GetChild0()->GetNType() == OR_EXP)
	{
		temp1 = new CSyntaxNode(NEGATION_EXP, BOOLTYPE);
		temp1->SetChild0(new CSyntaxNode());
		temp1->GetChild0()->CopyTree(pTree->GetChild0()->GetChild0());

		temp2 = new CSyntaxNode(NEGATION_EXP, BOOLTYPE);
		temp2->SetChild0(new CSyntaxNode());
		temp2->GetChild0()->CopyTree(pTree->GetChild0()->GetChild1());

		pTree->SetNType(AND_EXP);
		pTree->GetChild0()->CopyTree(temp1);

		pTree->SetChild1(new CSyntaxNode());
		pTree->GetChild1()->CopyTree(temp2);

		delete temp1;
		delete temp2;
		temp1 = NULL;
		temp2 = NULL;

		DnfNeg(pTree);
		return;
	}

	//Modified by wm 2013/05/14  16:39
	//pTree: ~~p
	if (pTree->GetNType() == NEGATION_EXP && pTree->GetChild0() != NULL &&
		pTree->GetChild0()->GetNType() == NEGATION_EXP)
	{
		temp1 = new CSyntaxNode();
		if (pTree->GetChild0()->GetChild0() != NULL)
		{
			temp1->CopyTree(pTree->GetChild0()->GetChild0());

		}
		else
		{
			cout << "__DNFNeg_ error!" << endl;
		}

		pTree->CopyTree(temp1);

		DnfNeg(pTree);
		delete temp1;
		temp1 = NULL;
	}
}

//add by YY[fixed] 2013/05/14 11:38
//programming by CJ 2013/05/14 11:38
void CSyntaxTree::DnfAnd(CSyntaxNode* pTree)
{
	CSyntaxNode *temp1 = NULL, *temp2 = NULL, *temp = NULL;
	if (pTree == NULL)
	{
		cout << " _DNF_And error!" << endl;
		return;
	}

	//add by YY 2013/06/14 15:54
	// (p1 or p2) and q => p1 and q or p2 and q 之后还会调用DnfAnd函数
	if (pTree->GetNType() == OR_EXP)
	{
		DnfAnd(pTree->GetChild0());
		DnfAnd(pTree->GetChild1());
		return;
	}

	if (pTree->GetNType() == AND_EXP)
	{

		DnfAnd(pTree->GetChild0());
		DnfAnd(pTree->GetChild1());
		// (p1 or p2) and q => p1 and q or p2 and q
		if (pTree->GetChild0() != NULL && pTree->GetChild0()->GetNType() == OR_EXP)
		{
			temp1 = new CSyntaxNode(AND_EXP, VOIDTYPE);
			temp1->SetChild0(new CSyntaxNode());
			temp1->GetChild0()->CopyTree(pTree->GetChild0()->GetChild0());

			temp1->SetChild1(new CSyntaxNode());
			temp1->GetChild1()->CopyTree(pTree->GetChild1());

			temp2 = new CSyntaxNode(AND_EXP, VOIDTYPE);
			temp2->SetChild0(new CSyntaxNode());
			temp2->GetChild0()->CopyTree(pTree->GetChild0()->GetChild1());

			temp2->SetChild1(new CSyntaxNode());
			temp2->GetChild1()->CopyTree(pTree->GetChild1());

			temp = new CSyntaxNode(OR_EXP, VOIDTYPE);
			temp->SetChild0(new CSyntaxNode());
			temp->SetChild1(new CSyntaxNode());
			temp->GetChild0()->CopyTree(temp1);
			temp->GetChild1()->CopyTree(temp2);
			pTree->CopyTree(temp);

			delete temp1;
			delete temp2;
			delete temp;
			temp1 = NULL;
			temp2 = NULL;
			temp = NULL;

			DnfAnd(pTree);

			//add by YY[fixed] 2013/05/14 17:13
			return;
		}
		// p and (q1 or q2) => p and q1 or p and q2
		if (pTree->GetChild1() != NULL && pTree->GetChild1()->GetNType() == OR_EXP)
		{
			temp1 = new CSyntaxNode(AND_EXP, VOIDTYPE);
			temp1->SetChild0(new CSyntaxNode());
			temp1->GetChild0()->CopyTree(pTree->GetChild0());

			temp1->SetChild1(new CSyntaxNode());
			temp1->GetChild1()->CopyTree(pTree->GetChild1()->GetChild0());

			temp2 = new CSyntaxNode(AND_EXP, VOIDTYPE);
			temp2->SetChild0(new CSyntaxNode());
			temp2->GetChild0()->CopyTree(pTree->GetChild0());

			temp2->SetChild1(new CSyntaxNode());
			temp2->GetChild1()->CopyTree(pTree->GetChild1()->GetChild1());

			temp = new CSyntaxNode(OR_EXP, VOIDTYPE);
			temp->SetChild0(new CSyntaxNode());
			temp->SetChild1(new CSyntaxNode());
			temp->GetChild0()->CopyTree(temp1);
			temp->GetChild1()->CopyTree(temp2);
			pTree->CopyTree(temp);

			delete temp1;
			delete temp2;
			delete temp;
			temp1 = NULL;
			temp2 = NULL;
			temp = NULL;

			DnfAnd(pTree);

			//add by YY[fixed] 2013/05/14 17:13
			return;
		}
	}
}

//------------------------------------------------------------------------------
//add by YY 2013/05/21 11:42
//专用于pre_and和pre_or中
//programming by YK 2013/05/24 9:23
//add by YY 2013/06/14 15:32
//Eg: (p and q and r and s and t)中寻找s
bool CSyntaxTree::__FindInTree(CSyntaxNode * ptree, CSyntaxNode* target, NODETYPE ntype)
{
	if (target != NULL && ptree->GetChild0() != NULL &&
		target->CompareTree(ptree->GetChild0()))
	{
		return true;
	}
	else if (target != NULL && ptree->GetChild1() != NULL &&
		target->CompareTree(ptree->GetChild1()))
	{
		return true;
	}

	if (ptree->GetChild0() != NULL && ptree->GetChild0()->GetNType() == ntype)
	{
		if (__FindInTree(ptree->GetChild0(), target, ntype))
		{
			return true;
		}
	}
	if (ptree->GetChild1() != NULL && ptree->GetChild1()->GetNType() == ntype)
	{
		if (__FindInTree(ptree->GetChild1(), target, ntype))
		{
			return true;
		}
	}
	if (ptree->GetChild2() != NULL && ptree->GetChild2()->GetNType() == ntype)
	{
		if (__FindInTree(ptree->GetChild2(), target, ntype))
		{
			return true;
		}
	}
	return false;
}
//------------------------------------------------------------------------------
//programming by YK 2013/05/24 9:24
//在ptree中找target的非
bool CSyntaxTree::__FindNegInTree(CSyntaxNode * ptree, CSyntaxNode* target, NODETYPE ntype)
{
	if (target != NULL && ptree->GetChild0() != NULL &&
		ptree->GetChild0()->GetNType() == NEGATION_EXP &&
		ptree->GetChild0()->GetChild0() != NULL &&
		target->CompareTree(ptree->GetChild0()->GetChild0()))
	{
		return true;
	}
	else if (target != NULL && ptree->GetChild1() != NULL &&
		ptree->GetChild1()->GetNType() == NEGATION_EXP &&
		ptree->GetChild1()->GetChild0() != NULL &&
		target->CompareTree(ptree->GetChild1()->GetChild0()))
	{
		return true;
	}

	if (ptree->GetChild0() != NULL && ptree->GetChild0()->GetNType() == ntype)
	{
		if (__FindNegInTree(ptree->GetChild0(), target, ntype))
		{
			return true;
		}
	}
	if (ptree->GetChild1() != NULL && ptree->GetChild1()->GetNType() == ntype)
	{
		if (__FindNegInTree(ptree->GetChild1(), target, ntype))
		{
			return true;
		}
	}
	if (ptree->GetChild2() != NULL && ptree->GetChild2()->GetNType() == ntype)
	{
		if (__FindNegInTree(ptree->GetChild2(), target, ntype))
		{
			return true;
		}
	}
	return false;

}

//added by YK 2013/05/16 17:20
//modified by YY 2013/05/27 11:51
//要把empty或者是next部分放在程序語法樹的最后，
//p && next true && q   '  p && q && next true
//------------------------------------------------------------------------------
//added by yangkai 2013/05/16 17:20
void CSyntaxTree::SortTree(CSyntaxNode* ptree)
{
	CSyntaxNode *temp = NULL;
	CSyntaxNode *tempPresent = NULL;
	CSyntaxNode *tempPrePresent = NULL;
	CSyntaxNode *tempNext = NULL;
	CSyntaxNode *tempPreNext = NULL;
	CSyntaxNode *presenttree = NULL;
	CSyntaxNode *emptytree = NULL;
	CSyntaxNode *nexttree = NULL;
	if (ptree == NULL)
	{
		return;
	}
	TrimTree(ptree);//先对树中的and，or，chop进行整理
	if (ptree->GetNType() != AND_EXP)
	{
		if (ptree->GetChild0() != NULL)
			SortTree(ptree->GetChild0());
		if (ptree->GetChild1() != NULL)
			SortTree(ptree->GetChild1());
		if (ptree->GetChild2() != NULL)
			SortTree(ptree->GetChild2());
		return;
	}
	else
	{
		/*		//先对树进行整理，如p and (q and r) -> (p and q) and r
		while(ptree->GetChild1()!=NULL && ptree->GetChild1()->GetNType()==AND_EXP)
		{
		temp=new CSyntaxNode(AND_EXP,VOIDTYPE);//没有考虑返回值类型
		temp->SetChild0(new CSyntaxNode());
		temp->GetChild0()->CopyTree(ptree->GetChild0());
		temp->SetChild1(new CSyntaxNode());
		temp->GetChild1()->CopyTree(ptree->GetChild1()->GetChild0());
		temp1=new CSyntaxNode();
		temp1->CopyTree(ptree->GetChild1()->GetChild1());
		ptree->GetChild0()->CopyTree(temp);
		ptree->GetChild1()->CopyTree(temp1);
		delete temp;
		temp=NULL;
		delete temp1;
		temp1=NULL;
		}

		*///Modified by yk 2013/05/24  11:46

		//Modified by wm 2013/06/09  11:12
		//将present，empty，next分别加到相应的树中去
		temp = ptree;
		while (temp->GetNType() == AND_EXP)
		{
			//Modified by wm 2013/06/09  11:20
			if (temp->GetChild0() == NULL || temp->GetChild1() == NULL)
			{
				cout << "SortTree error!" << endl;
				delete presenttree;
				presenttree = NULL;
				delete emptytree;
				emptytree = NULL;
				delete nexttree;
				nexttree = NULL;
				return;
			}
			if (temp->GetChild1()->GetNType() == EMPTY_EXP)
			{
				//empty只加一次
				if (emptytree == NULL)
				{
					emptytree = new CSyntaxNode(EMPTY_EXP, VOIDTYPE);
				}
			}
			else if (temp->GetChild1()->GetNType() == NEXT_STA)
			{
				//Modified by wm 2013/06/09  11:06
				/*	if(nexttree==NULL)
				{
				nexttree=new CSyntaxNode();
				nexttree->CopyTree(ptree->GetChild1()->GetChild0());
				}
				else
				{
				nexttree->MergeTree(ptree->GetChild1()->GetChild0(),AND_EXP);
				}
				*/
				if (temp->GetChild1()->GetChild0() == NULL)
				{
					cout << "SortTree error!" << endl;
					delete presenttree;
					presenttree = NULL;
					delete emptytree;
					emptytree = NULL;
					delete nexttree;
					nexttree = NULL;
					return;
				}
				if (nexttree == NULL)
				{
					nexttree = new CSyntaxNode(AND_EXP, VOIDTYPE);
					tempNext = nexttree;
				}

				tempPreNext = tempNext;
				tempNext = __MergeTree(tempNext, temp->GetChild1()->GetChild0(), AND_EXP, VOIDTYPE);

			}
			else
			{
				//Modified by wm 2013/06/09  11:10
				/*	if(presenttree==NULL)
				{
				presenttree=new CSyntaxNode();
				presenttree->CopyTree(ptree->GetChild1());
				}
				else
				{
				presenttree->MergeTree(ptree->GetChild1(),AND_EXP);
				}*/

				if (presenttree == NULL)
				{
					presenttree = new CSyntaxNode(AND_EXP, VOIDTYPE, VOIDTYPE);
					tempPresent = presenttree;
				}
				tempPrePresent = tempPresent;
				tempPresent = __MergeTree(tempPresent, temp->GetChild1(), AND_EXP, VOIDTYPE);
			}
			//temp重新指向它的左子树
			/*	temp=new CSyntaxNode();
			temp->CopyTree(ptree->GetChild0());
			ptree->CopyTree(temp);
			delete temp;
			temp=NULL;*/  //annotated by wm 2013.06.09

			//temp指向它的左子树
			temp = temp->GetChild0();
		}

		//add by wm 2013/06/09  11:25
		if (temp->GetNType() == EMPTY_EXP)
		{
			if (emptytree == NULL)
			{
				emptytree = new CSyntaxNode(EMPTY_EXP, VOIDTYPE);
			}
		}
		else if (temp->GetNType() == NEXT_STA)
		{
			if (temp->GetChild0() == NULL)
			{
				cout << "SortTree error!" << endl;
				delete presenttree;
				presenttree = NULL;
				delete emptytree;
				emptytree = NULL;
				delete nexttree;
				nexttree = NULL;
				return;
			}
			if (nexttree == NULL)
			{
				nexttree = new CSyntaxNode();
				nexttree->CopyTree(temp->GetChild0());
			}
			else
			{
				tempNext->CopyTree(temp->GetChild0());
			}
		}
		else
		{
			if (presenttree == NULL)
			{
				presenttree = new CSyntaxNode();
				presenttree->CopyTree(temp);
			}
			else
			{
				tempPresent->CopyTree(temp);
			}
		}

		__DeletePost(tempPrePresent, AND_EXP);
		__DeletePost(tempPreNext, AND_EXP);

		if (nexttree != NULL)
		{
			Pre(nexttree);
		}

		//add by wm 2013/06/09  14:31
		//将整理结果存放到ptree里边
		if (presenttree != NULL)
		{
			if (emptytree != NULL)
			{
				delete ptree->GetChild0();
				delete ptree->GetChild1();
				ptree->SetNType(AND_EXP);
				ptree->SetChild0(new CSyntaxNode());
				ptree->GetChild0()->CopyTree(presenttree);
				ptree->SetChild1(new CSyntaxNode(EMPTY_EXP, VOIDTYPE));
			}
			else if (nexttree != NULL)
			{
				delete ptree->GetChild0();
				delete ptree->GetChild1();
				ptree->SetChild0(new CSyntaxNode());
				ptree->GetChild0()->CopyTree(presenttree);
				ptree->SetChild1(new CSyntaxNode(NEXT_STA, VOIDTYPE));
				ptree->GetChild1()->SetChild0(new CSyntaxNode());
				ptree->GetChild1()->GetChild0()->CopyTree(nexttree);
			}
			else
			{
				ptree->CopyTree(presenttree);
			}
		}
		else
		{
			if (emptytree != NULL)
			{
				ptree->CopyTree(emptytree);
			}
			else if (nexttree != NULL)
			{
				delete ptree->GetChild0();
				delete ptree->GetChild1();
				ptree->SetChild1(NULL);
				ptree->SetNType(NEXT_STA);
				ptree->SetChild0(new CSyntaxNode());
				ptree->GetChild0()->CopyTree(nexttree);
			}
		}

		//annotated by wm 2013.06.09
		/*	if(temp!=NULL&&temp->GetNType()==AND_EXP)
		{
		//把左子树加到相应的树上去
		if (temp->GetChild0()->GetNType()==EMPTY_EXP)
		{
		//empty只加一次
		if(emptytree==NULL)
		{
		emptytree=new CSyntaxNode();
		emptytree->CopyTree(ptree->GetChild0());
		}
		}
		else if(ptree->GetChild0()->GetNType()==NEXT_STA)
		{
		if(nexttree==NULL)
		{
		nexttree=new CSyntaxNode();
		nexttree->CopyTree(ptree->GetChild0()->GetChild0());
		}
		else
		{
		nexttree->MergeTree(ptree->GetChild0()->GetChild0(),AND_EXP);
		}
		}
		else
		{
		if(presenttree==NULL)
		{
		presenttree=new CSyntaxNode();
		presenttree->CopyTree(ptree->GetChild0());
		}
		else
		{
		presenttree->MergeTree(ptree->GetChild0(),AND_EXP);
		}
		}
		//把右子树加到相应的树上去
		if (ptree->GetChild1()->GetNType()==EMPTY_EXP)
		{
		//empty只加一次
		if(emptytree==NULL)
		{
		emptytree=new CSyntaxNode();
		emptytree->CopyTree(ptree->GetChild1());
		}
		}
		else if(ptree->GetChild1()->GetNType()==NEXT_STA)
		{
		if(nexttree==NULL)
		{
		nexttree=new CSyntaxNode();
		nexttree->CopyTree(ptree->GetChild1()->GetChild0());
		}
		else
		{
		nexttree->MergeTree(ptree->GetChild1()->GetChild0(),AND_EXP);/////////还没有实现
		}
		}
		else
		{
		if(presenttree==NULL)
		{
		presenttree=new CSyntaxNode();
		presenttree->CopyTree(ptree->GetChild1());
		}
		else
		{
		presenttree->MergeTree(ptree->GetChild1(),AND_EXP);
		}
		}

		}

		//前面只是将next节点的子树都合取起来，这里给合取起来的整个树加上next根节点
		if(nexttree!=NULL)
		{
		//Pre(nexttree);//////////////////////
		temp=new CSyntaxNode(NEXT_STA,VOIDTYPE);
		temp->SetChild0(new CSyntaxNode());
		temp->GetChild0()->CopyTree(nexttree);
		nexttree->CopyTree(temp);
		delete temp;
		temp=NULL;
		}

		if(presenttree!=NULL)
		{
		if(emptytree!=NULL)
		{
		presenttree->MergeTree(emptytree,AND_EXP);
		ptree->CopyTree(presenttree);
		return;
		}
		else if(nexttree!=NULL)
		{
		presenttree->MergeTree(nexttree,AND_EXP);
		ptree->CopyTree(presenttree);
		return;
		}
		else
		{
		ptree->CopyTree(presenttree);
		return;
		}
		}
		else
		{
		if(emptytree!=NULL)
		ptree->CopyTree(emptytree);
		else if(nexttree!=NULL)
		ptree->CopyTree(nexttree);
		}*/

	}
	//Pre(ptree);
	delete presenttree;
	presenttree = NULL;
	delete emptytree;
	emptytree = NULL;
	delete nexttree;
	nexttree = NULL;
}

//add by YY 2013/06/13 10:53
//programming by WM 2013/06/13 10:53
//------------------------------------------------------------------------------
//add by wm 2013/06/09  10:52
CSyntaxNode* CSyntaxTree::__MergeTree(CSyntaxNode*pTree1, CSyntaxNode*pTree2, NODETYPE NType, RETURNTYPE RType)
{
	if (pTree1 == NULL)
	{
		cout << "__MergeTree error!" << endl;
		return NULL;
	}
	CSyntaxNode*temp = pTree1;

	//找到左右子树均为空的NType节点，temp记录位置
	while (temp->GetChild0() != NULL)
	{
		temp = temp->GetChild0();
	}

	//给该结点左子树添加NType节点
	temp->SetChild0(new CSyntaxNode(NType, RType));

	//将pTree2加到找到的NType节点的右子树上
	temp->SetChild1(new CSyntaxNode());
	temp->GetChild1()->CopyTree(pTree2);

	temp = temp->GetChild0();
	return temp;
}

//------------------------------------------------------------------------------
//add by wm 2013/06/09  10:55
void CSyntaxTree::__DeletePost(CSyntaxNode*pTree, NODETYPE NType)
{
	if (pTree == NULL)//Modified by wm 2013/08/07  16:53
	{
		return;
	}
	if (pTree->GetNType() != NType)
	{
		return;
	}
	CSyntaxNode*temp = pTree;

	/*	//有可能ptree为empty，所以需要加上判断temp->GetNType()是否为NType
	if (temp->GetChild0()==NULL)
	{
	if(temp->GetNType()==NType)
	{
	pTree->SetNType(FALSE_EXP);
	}
	temp=NULL;
	return;
	}*///annotated by wm 2013/06/09  14:51
	while (temp->GetChild0()->GetNType() == NType &&
		temp->GetChild0()->GetChild0() != NULL)
	{
		temp = temp->GetChild0();
	}
	if (temp->GetChild0()->GetNType() != NType)
	{
		temp = NULL;
		return;
	}
	CSyntaxNode *temp2 = new CSyntaxNode();
	temp2->CopyTree(temp->GetChild1());
	temp->CopyTree(temp2);

	delete temp2;
	temp2 = NULL;
	temp = NULL;
}


//add by YY 2013/05/24 14:52
//用于树的整理，即将and , or整理为靠左排列  chop靠右排列
void CSyntaxTree::TrimTree(CSyntaxNode *pTree)
{
	if (pTree == NULL)
	{
		cout << " TrimTree error!" << endl;
		return;
	}

	CSyntaxNode *temp = NULL, *temp1 = NULL;

	//and的整理: p1 and (p2 and p3) --> (p1 and p2) and p3
	while (pTree->GetNType() == AND_EXP &&
		pTree->GetChild0() != NULL   &&
		pTree->GetChild1() != NULL   &&
		pTree->GetChild1()->GetNType() == AND_EXP)
	{
		//p1 and p2
		temp1 = new CSyntaxNode(AND_EXP, VOIDTYPE);
		temp1->SetChild0(new CSyntaxNode());
		temp1->GetChild0()->CopyTree(pTree->GetChild0());
		temp1->SetChild1(new CSyntaxNode());
		temp1->GetChild1()->CopyTree(pTree->GetChild1()->GetChild0());

		//(p1 and p2) and p3
		temp = new CSyntaxNode(AND_EXP, VOIDTYPE);
		temp->SetChild0(new CSyntaxNode());
		temp->GetChild0()->CopyTree(temp1);
		temp->SetChild1(new CSyntaxNode());
		temp->GetChild1()->CopyTree(pTree->GetChild1()->GetChild1());

		pTree->CopyTree(temp);

		delete temp;
		temp = NULL;
		delete temp1;
		temp1 = NULL;
	}
	//or的整理: p1 or (p2 or p3) --> (p1 or p2) or p3
	while (pTree->GetNType() == OR_EXP  &&
		pTree->GetChild0() != NULL   &&
		pTree->GetChild1() != NULL   &&
		pTree->GetChild1()->GetNType() == OR_EXP)
	{
		//p1 or p2
		temp1 = new CSyntaxNode(OR_EXP, VOIDTYPE);
		temp1->SetChild0(new CSyntaxNode());
		temp1->GetChild0()->CopyTree(pTree->GetChild0());
		temp1->SetChild1(new CSyntaxNode());
		temp1->GetChild1()->CopyTree(pTree->GetChild1()->GetChild0());

		//(p1 or p2) or p3
		temp = new CSyntaxNode(OR_EXP, VOIDTYPE);
		temp->SetChild0(new CSyntaxNode());
		temp->GetChild0()->CopyTree(temp1);
		temp->SetChild1(new CSyntaxNode());
		temp->GetChild1()->CopyTree(pTree->GetChild1()->GetChild1());

		pTree->CopyTree(temp);

		delete temp;
		temp = NULL;
		delete temp1;
		temp1 = NULL;
	}
	//chop的整理: (p1;p2) ; p3 --> p1 ; (p2;p3)
	while (pTree->GetNType() == CHOP_STA  &&
		pTree->GetChild0() != NULL     &&
		pTree->GetChild1() != NULL     &&
		pTree->GetChild0()->GetNType() == CHOP_STA)
	{
		//p2 ; p3
		temp1 = new CSyntaxNode(CHOP_STA, VOIDTYPE);
		temp1->SetChild0(new CSyntaxNode());
		temp1->GetChild0()->CopyTree(pTree->GetChild0()->GetChild1());
		temp1->SetChild1(new CSyntaxNode());
		temp1->GetChild1()->CopyTree(pTree->GetChild1());

		//(p1;p2) ; p3
		temp = new CSyntaxNode(CHOP_STA, VOIDTYPE);
		temp->SetChild0(new CSyntaxNode());
		temp->GetChild0()->CopyTree(pTree->GetChild0()->GetChild0());
		temp->SetChild1(new CSyntaxNode());
		temp->GetChild1()->CopyTree(temp1);

		pTree->CopyTree(temp);

		delete temp;
		temp = NULL;
		delete temp1;
		temp1 = NULL;
	}
	if (pTree->GetChild0() != NULL)
	{
		TrimTree(pTree->GetChild0());
	}
	if (pTree->GetChild1() != NULL)
	{
		TrimTree(pTree->GetChild1());
	}
	if (pTree->GetChild2() != NULL)
	{
		TrimTree(pTree->GetChild2());
	}
}

//--------------------------------------------------------------------------------------
//add by YY 2013/06/04 21:43
//programming by WM 2013/06/04 21:53
void CSyntaxTree::SetAllPreFlag(bool PreFlag, CSyntaxNode * pTree)
{
	if (pTree == NULL)
	{
		return;
	}
	if (pTree->GetNType() == LENGTH_STA)
	{
		pTree->SetPreLen(PreFlag);
	}
	else
	{
		if (pTree->GetChild0() != NULL)
		{
			SetAllPreFlag(PreFlag, pTree->GetChild0());
		}
		if (pTree->GetChild1() != NULL)
		{
			SetAllPreFlag(PreFlag, pTree->GetChild1());
		}
		if (pTree->GetChild2() != NULL)
		{
			SetAllPreFlag(PreFlag, pTree->GetChild2());
		}
	}

}

//add by YY 2013/06/04 21:53
//programming by WM 2013/06/04 21:53
void	CSyntaxTree::__NfNeg(CSyntaxNode*pTree)
{
	if (pTree->GetChild0() == NULL)
	{
		cout << "__NfNeg error!" << endl;
		return;
	}

	//!true -> false
	if (pTree->GetChild0()->GetNType() == TRUE_EXP)
	{
		delete pTree->GetChild0();
		pTree->SetChild0(NULL);
		pTree->SetNType(FALSE_EXP);
		return;
	}

	//将~p中的p先化为NF的形式
	NF(pTree->GetChild0());

	if (pTree->GetChild0()->GetNType() == FALSE_EXP)
	{
		delete pTree->GetChild0();
		pTree->SetChild0(NULL);
		pTree->SetNType(TRUE_EXP);

		__NfTrue(pTree);
		return;
	}

	CSyntaxNode*m_NegEmpTree = new CSyntaxNode(AND_EXP, VOIDTYPE);

	m_EmptyTree = new CSyntaxNode(OR_EXP, VOIDTYPE);
	m_NextTree = new CSyntaxNode(OR_EXP, VOIDTYPE);

	//	SortTree(pTree->GetChild0());//临时修改

	//将程序语法树非的子树分成empty子树和next子树，
	//分别存储到m_EmptyTree和m_NextTree中
	__SprEmpNext(pTree->GetChild0());
	__DeleteOr(m_EmptyTree);
	__DeleteOr(m_NextTree);

	//将m_NextTree化成完全NF形式
	__Conf(m_NextTree);

	//树中只有empty树，没有next树
	if (m_NextTree->GetNType() == FALSE_EXP)
	{
		//树中的empty树只有empty
		if (m_EmptyTree->GetNType() == EMPTY_EXP)
		{
			delete pTree->GetChild0();
			pTree->SetNType(NEXT_STA);
			pTree->SetChild0(new CSyntaxNode(TRUE_EXP, VOIDTYPE));
		}
		else
		{
			__NegEmpTree(m_EmptyTree, m_NegEmpTree);

			if (m_NegEmpTree->GetNType() == AND_EXP)
			{
				pTree->SetNType(OR_EXP);
				delete pTree->GetChild0();
				pTree->SetChild0(new CSyntaxNode());
				pTree->GetChild0()->CopyTree(m_NegEmpTree);
				pTree->SetChild1(new CSyntaxNode(NEXT_STA, VOIDTYPE));
				pTree->GetChild1()->SetChild0(new CSyntaxNode(TRUE_EXP, VOIDTYPE));
			}
		}

	}

	//树中有next树
	else
	{
		__NegNextTree(m_NextTree);

		//树中没有empty树
		if (m_EmptyTree->GetNType() == FALSE_EXP)
		{
			m_NegEmpTree->SetNType(EMPTY_EXP);
		}
		//树中的empty树，只有empty
		else if (m_EmptyTree->GetNType() == EMPTY_EXP)
		{
			pTree->CopyTree(m_NextTree);
			delete m_NegEmpTree;
			m_NegEmpTree = NULL;
			delete m_EmptyTree;
			m_EmptyTree = NULL;
			delete m_NextTree;
			m_NextTree = NULL;
			return;
		}

		//树中有empty树
		else
		{
			__NegEmpTree(m_EmptyTree, m_NegEmpTree);

		}
		pTree->SetNType(OR_EXP);
		delete pTree->GetChild0();
		pTree->SetChild0(new CSyntaxNode());
		pTree->GetChild0()->CopyTree(m_NegEmpTree);
		pTree->SetChild1(new CSyntaxNode());
		pTree->GetChild1()->CopyTree(m_NextTree);
	}

	delete m_NegEmpTree;
	m_NegEmpTree = NULL;
	delete m_EmptyTree;
	m_EmptyTree = NULL;
	delete m_NextTree;
	m_NextTree = NULL;

}

//------------------------------------------------------------------------------
//add by wm 2013/05/17  9:20
void	CSyntaxTree::__Conf(CSyntaxNode *pTree)
{
	//pTree的形式为p and ()q 则直接调用__ConfNext(pTree);
	if (pTree->GetNType() == AND_EXP)
	{
		if (pTree->GetChild0() == NULL || pTree->GetChild1() == NULL)
		{
			cout << "__Conf() error!" << endl;
			return;
		}
		if (pTree->GetChild1()->GetNType() == NEXT_STA)
		{
			__ConfNext(pTree);
		}
	}

	//or的形式为 p or q or r or ... or m
	else if (pTree->GetNType() == OR_EXP)
	{
		if (pTree->GetChild0() == NULL || pTree->GetChild1() == NULL)
		{
			cout << "__Conf error!" << endl;
			return;
		}
		__Conf(pTree->GetChild0());
		__ConfNOr(pTree->GetChild1(), pTree->GetChild0());
		CSyntaxNode*temp = new CSyntaxNode();
		temp->CopyTree(pTree->GetChild0());
		pTree->CopyTree(temp);
		delete temp;
		temp = NULL;
	}

}

//------------------------------------------------------------------------------
//add by wm 2013/05/16  17:55
// p and ()q   ->  p and ()q  or  !p  and ()false
void CSyntaxTree::__ConfNext(CSyntaxNode* pTree)
{

	CSyntaxNode *temp = NULL;

	//pTree是and的形式就必须有左右子树，否则输出错误信息
	if (pTree->GetChild0() == NULL || pTree->GetChild1() == NULL ||
		pTree->GetChild1()->GetChild0() == NULL)
	{
		cout << "__ConfNext() error!" << endl;
		return;
	}
	//在这里我们默认pTree已经是p and () q的形式，而不会是next r and q的形式

	//Modified by wm 2013/05/31  10:34
	//当pTree形式为p and next true时，由于我们会对其取非，这里我们可将其完全NF化
	//为!p and ()false的形式。
	if (pTree->GetChild1()->GetChild0()->GetNType() == TRUE_EXP)
	{
		temp = new CSyntaxNode();
		temp->CopyTree(pTree->GetChild0());
		delete pTree->GetChild0();
		pTree->SetChild0(new CSyntaxNode(NEGATION_EXP, BOOLTYPE));
		pTree->GetChild0()->SetChild0(new CSyntaxNode());
		pTree->GetChild0()->GetChild0()->CopyTree(temp);

		//		Dnf(pTree->GetChild0());//临时修改

		pTree->GetChild1()->GetChild0()->SetNType(FALSE_EXP);
		delete temp;
		temp = NULL;
		return;
	}
	if (pTree->GetChild0()->GetNType() == TRUE_EXP)
	{
		return;
	}
	temp = new CSyntaxNode();
	temp->CopyTree(pTree);

	//pTree左子树:p and ()q
	pTree->SetNType(OR_EXP);
	pTree->GetChild0()->CopyTree(temp);

	//pTree右子树:!p and ()false
	delete pTree->GetChild1();
	pTree->SetChild1(new CSyntaxNode(AND_EXP, VOIDTYPE));
	pTree->GetChild1()->SetChild0(new CSyntaxNode(NEGATION_EXP, BOOLTYPE));
	pTree->GetChild1()->GetChild0()->SetChild0(new CSyntaxNode());
	pTree->GetChild1()->GetChild0()->GetChild0()->CopyTree(temp->GetChild0());
	//	Dnf(pTree->GetChild1()->GetChild0());//临时修改

	pTree->GetChild1()->SetChild1(new CSyntaxNode(NEXT_STA, VOIDTYPE));
	pTree->GetChild1()->GetChild1()->SetChild0(new CSyntaxNode(FALSE_EXP, VOIDTYPE));

	delete temp;
	temp = NULL;
}
//------------------------------------------------------------------------------
//add by wm 2013/05/22  11:10
void CSyntaxTree::__ConfOr(CSyntaxNode*pTree, CSyntaxNode*pConfTree)
{
	//pConfTree:m and ()m1 , pTree: p and ()p1
	if (pConfTree == NULL || pTree == NULL)
	{
		cout << "__ConfOr error!" << endl;
		return;
	}

	CSyntaxNode* temp1 = NULL,
		*temp2 = NULL,
		*temp3 = NULL,
		*temp4 = NULL;

	//p and ()p1 or m and ()m1  -> p and m and ()(p1 or m1) or !p and m and ()m1

	if (pTree->GetChild0() == NULL || pTree->GetChild1() == NULL ||
		pTree->GetChild1()->GetChild0() == NULL || pConfTree->GetChild0() == NULL ||
		pConfTree->GetChild1() == NULL || pConfTree->GetChild1()->GetChild0() == NULL)
	{
		cout << "__ConfOr error!" << endl;
		return;
	}

	//p and ()true or m and ()m1化为!p and m and ()m1
	if (pTree->GetChild1()->GetChild0()->GetNType() == TRUE_EXP)
	{
		temp1 = new CSyntaxNode();
		temp1->CopyTree(pConfTree->GetChild0());
		delete pConfTree->GetChild0();
		pConfTree->SetChild0(new CSyntaxNode(AND_EXP, VOIDTYPE));
		pConfTree->GetChild0()->SetChild0(new CSyntaxNode(NEGATION_EXP, BOOLTYPE));
		pConfTree->GetChild0()->GetChild0()->SetChild0(new CSyntaxNode());
		pConfTree->GetChild0()->GetChild0()->GetChild0()->CopyTree(pTree->GetChild0());
		pConfTree->GetChild0()->SetChild1(new CSyntaxNode());
		pConfTree->GetChild0()->GetChild1()->CopyTree(temp1);

		__Reduce(pConfTree);
		delete temp1;
		temp1 = NULL;
		return;
	}

	//temp2:p and m
	temp2 = new CSyntaxNode(AND_EXP, VOIDTYPE);
	temp2->SetChild0(new CSyntaxNode());
	temp2->GetChild0()->CopyTree(pTree->GetChild0());
	temp2->SetChild1(new CSyntaxNode());
	temp2->GetChild1()->CopyTree(pConfTree->GetChild0());

	__Reduce(temp2);

	//temp1: p1 or m1
	if (temp2->GetNType() != FALSE_EXP)
	{
		temp1 = new CSyntaxNode(OR_EXP, VOIDTYPE);
		temp1->SetChild0(new CSyntaxNode());
		temp1->GetChild0()->CopyTree(pTree->GetChild1()->GetChild0());
		temp1->SetChild1(new CSyntaxNode());
		temp1->GetChild1()->CopyTree(pConfTree->GetChild1()->GetChild0());

		__Reduce(temp1);
	}

	//temp3: !p and m
	temp3 = new CSyntaxNode(AND_EXP, VOIDTYPE);
	temp3->SetChild0(new CSyntaxNode(NEGATION_EXP, BOOLTYPE));
	temp3->GetChild0()->SetChild0(new CSyntaxNode());
	temp3->GetChild0()->GetChild0()->CopyTree(pTree->GetChild0());
	temp3->SetChild1(new CSyntaxNode());
	temp3->GetChild1()->CopyTree(pConfTree->GetChild0());

	__Reduce(temp3);

	//temp4:m1
	if (temp3->GetNType() != FALSE_EXP)
	{
		temp4 = new CSyntaxNode();
		temp4->CopyTree(pConfTree->GetChild1()->GetChild0());
	}

	//p and ()p1 or m and ()m1  -> p and m and ()(p1 or m1) or !p and m and ()m1
	delete pConfTree->GetChild1();
	pConfTree->SetChild1(NULL);

	//p and m 为 false,则化为!p and m and ()m1
	if (temp2->GetNType() == FALSE_EXP)
	{
		pConfTree->GetChild0()->CopyTree(temp3);
		pConfTree->SetChild1(new CSyntaxNode(NEXT_STA, VOIDTYPE));
		pConfTree->GetChild1()->SetChild0(new CSyntaxNode());
		pConfTree->GetChild1()->GetChild0()->CopyTree(temp4);
	}

	//!p and m 为false,则化为p and m and ()(p1 or m1)
	else if (temp3->GetNType() == FALSE_EXP)
	{
		pConfTree->GetChild0()->CopyTree(temp2);
		pConfTree->SetChild1(new CSyntaxNode(NEXT_STA, VOIDTYPE));
		pConfTree->GetChild1()->SetChild0(new CSyntaxNode());
		pConfTree->GetChild1()->GetChild0()->CopyTree(temp1);
	}

	//化为p and m and ()(p1 or m1) or !p and m and ()m1
	else
	{
		pConfTree->SetNType(OR_EXP);
		delete pConfTree->GetChild0();
		pConfTree->SetChild0(new CSyntaxNode(AND_EXP, VOIDTYPE));
		pConfTree->GetChild0()->SetChild0(new CSyntaxNode());
		pConfTree->GetChild0()->GetChild0()->CopyTree(temp2);
		pConfTree->GetChild0()->SetChild1(new CSyntaxNode(NEXT_STA, VOIDTYPE));
		pConfTree->GetChild0()->GetChild1()->SetChild0(new CSyntaxNode());
		pConfTree->GetChild0()->GetChild1()->GetChild0()->CopyTree(temp1);

		pConfTree->SetChild1(new CSyntaxNode(AND_EXP, VOIDTYPE));
		pConfTree->GetChild1()->SetChild0(new CSyntaxNode());
		pConfTree->GetChild1()->GetChild0()->CopyTree(temp3);
		pConfTree->GetChild1()->SetChild1(new CSyntaxNode(NEXT_STA, VOIDTYPE));
		pConfTree->GetChild1()->GetChild1()->SetChild0(new CSyntaxNode());
		pConfTree->GetChild1()->GetChild1()->GetChild0()->CopyTree(temp4);

	}

	delete temp1;
	temp1 = NULL;
	delete temp2;
	temp2 = NULL;
	delete temp3;
	temp3 = NULL;
	delete temp4;
	temp4 = NULL;

}
//------------------------------------------------------------------------------
//add by wm 2013/05/23  11:11
void CSyntaxTree::__ConfNOr(CSyntaxNode *pTree, CSyntaxNode *pConfTree)
{
	if (pTree == NULL || pConfTree == NULL)
	{
		cout << "__ConfNOr error!" << endl;
		return;
	}
	if (pConfTree->GetNType() != OR_EXP)
	{
		__ConfOr(pTree, pConfTree);
		return;
	}
	else
	{
		__ConfNOr(pTree, pConfTree->GetChild0());
		__ConfNOr(pTree, pConfTree->GetChild1());
	}

}

//------------------------------------------------------------------------------
//add by wm 2013/05/23  15:32
void CSyntaxTree::__NegEmpTree(CSyntaxNode*pTree, CSyntaxNode*pNegTree)
{
	CSyntaxNode*temp = pTree,
		*Ntemp = pNegTree;

	//p and empty or q and empty or ...取非得到  (!p and !q and ...) and empty
	pNegTree->SetChild1(new CSyntaxNode(EMPTY_EXP, VOIDTYPE));

	while (temp->GetNType() == OR_EXP)
	{
		if (pTree->GetChild1() == NULL || pTree->GetChild1()->GetChild0() == NULL ||
			pTree->GetChild0() == NULL)
		{
			cout << "__NegEmpTree error!" << endl;
			return;
		}
		Ntemp->SetChild0(new CSyntaxNode(AND_EXP, VOIDTYPE));
		Ntemp->GetChild0()->SetChild1(new CSyntaxNode(NEGATION_EXP, BOOLTYPE));
		Ntemp->GetChild0()->GetChild1()->SetChild0(new CSyntaxNode());
		Ntemp->GetChild0()->GetChild1()->GetChild0()
			->CopyTree(temp->GetChild1()->GetChild0());

		Ntemp = Ntemp->GetChild0();
		temp = temp->GetChild0();
	}

	if (temp->GetNType() == AND_EXP)
	{
		if (temp->GetChild0() == NULL)
		{
			cout << "__NegEmpTree error!" << endl;
			return;
		}
		Ntemp->SetChild0(new CSyntaxNode(NEGATION_EXP, BOOLTYPE));
		Ntemp->GetChild0()->SetChild0(new CSyntaxNode());
		Ntemp->GetChild0()->GetChild0()->CopyTree(temp->GetChild0());
	}


	//	Dnf(pNegTree);//临时修改
	TrimTree(pNegTree);
	__Reduce(pNegTree);

	temp = NULL;
	Ntemp = NULL;
}
//------------------------------------------------------------------------------
//add by wm 2013/05/23  15:40
void CSyntaxTree::__NegNextTree(CSyntaxNode*pTree)
{
	CSyntaxNode*temp = NULL;

	//p and ()q取非化成p and ()!q
	if (pTree->GetNType() == AND_EXP)
	{
		if (pTree->GetChild0() == NULL || pTree->GetChild1() == NULL ||
			pTree->GetChild1()->GetChild0() == NULL)
		{
			cout << "__NegEmpTree error!" << endl;
			return;
		}

		//p and ()false 取非化为p and ()true
		if (pTree->GetChild1()->GetChild0()->GetNType() == FALSE_EXP)
		{
			temp = new CSyntaxNode(TRUE_EXP, VOIDTYPE);
		}

		//p and ()true 取非化为false
		else if (pTree->GetChild1()->GetChild0()->GetNType() == TRUE_EXP)
		{
			delete pTree->GetChild0();
			pTree->SetChild0(NULL);
			delete pTree->GetChild1();
			pTree->SetChild1(NULL);
			pTree->SetNType(FALSE_EXP);
			return;

		}
		else
		{
			temp = new CSyntaxNode(NEGATION_EXP, BOOLTYPE);
			temp->SetChild0(new CSyntaxNode());
			temp->GetChild0()->CopyTree(pTree->GetChild1()->GetChild0());
		}

		//true and ()q 取非化为()!q
		if (pTree->GetChild0()->GetNType() == TRUE_EXP)
		{
			//Modified by wm 2013/06/03  17:34
			pTree->SetNType(NEXT_STA);
			delete pTree->GetChild0();
			delete pTree->GetChild1();
			pTree->SetChild1(NULL);
			pTree->SetChild0(new CSyntaxNode());
			pTree->GetChild0()->CopyTree(temp);
		}
		else
		{
			pTree->GetChild1()->GetChild0()->CopyTree(temp);
		}


		delete temp;
		temp = NULL;
	}
	else if (pTree->GetNType() == OR_EXP)
	{
		if (pTree->GetChild0() == NULL || pTree->GetChild1() == NULL)
		{
			cout << "__NegNextTree error!" << endl;
			return;
		}
		__NegNextTree(pTree->GetChild0());
		__NegNextTree(pTree->GetChild1());

	}
}

//------------------------------------------------------------------------------
//add by wm 2013/05/15  15:33
//将pTree并到m_EmptyTree树上，此时得到的语法树并不是最终形式的语法树，会有最后一
//级的or节点没有左右子树，在将所有的pTree加到m_EmptyTree以后，我们必须调用函数
//__DeleteOr(m_EmptyTree)对语法树进行整理，去掉最后一级or，才能得到最终的语法树。
void CSyntaxTree::__Merge2EmpTree(CSyntaxNode*pTree)
{
	if (m_EmptyTree == NULL)
	{
		return;
	}
	CSyntaxNode*temp = m_EmptyTree;

	//找到左右子树均为空的or节点，temp记录位置
	while (temp->GetChild0() != NULL)
	{
		temp = temp->GetChild0();
	}

	//给该结点左子树添加or节点
	temp->SetChild0(new CSyntaxNode(OR_EXP, VOIDTYPE));

	//将pTree加到找到的or节点的右子树上
	temp->SetChild1(new CSyntaxNode());
	temp->GetChild1()->CopyTree(pTree);
	temp = NULL;

}
//------------------------------------------------------------------------------
//add by wm 2013/05/15  19:11
//将pTree并到m_NextTree树上，此时得到的语法树并不是最终形式的语法树，会有最后一
//级的or节点没有左右子树，在将所有的pTree加到m_NextTree以后，我们必须调用函数
//__DeleteOr(m_NextTree)对语法树进行整理，去掉最后一级or，才能得到最终的语法树。
void CSyntaxTree::__Merge2NextTree(CSyntaxNode*pTree)
{
	if (m_EmptyTree == NULL)
	{
		return;
	}
	CSyntaxNode*temp = m_NextTree;

	//找到左右子树均为空的or节点，temp记录位置
	while (temp->GetChild0() != NULL)
	{
		temp = temp->GetChild0();
	}

	//给该结点左子树添加or节点
	temp->SetChild0(new CSyntaxNode(OR_EXP, VOIDTYPE));
	//将pTree加到找到的or节点的右子树上
	temp->SetChild1(new CSyntaxNode());
	temp->GetChild1()->CopyTree(pTree);
	temp = NULL;
}

//------------------------------------------------------------------------------
//add by wm 2013/05/16  10:00
void CSyntaxTree::__DeleteOr(CSyntaxNode *pTree)
{
	CSyntaxNode*temp = pTree,
		*temp2 = new CSyntaxNode();

	//Modified by wm 2013/06/03  15:25
	//有可能ptree为empty，所以需要加上判断temp->GetNType()是否为OR_EXP
	if (temp->GetChild0() == NULL)
	{
		if (temp->GetNType() == OR_EXP)
		{
			pTree->SetNType(FALSE_EXP);
		}
		temp = NULL;
		return;
	}
	while (temp->GetChild0()->GetChild0() != NULL)
	{
		temp = temp->GetChild0();
	}
	temp2->CopyTree(temp->GetChild1());
	temp->CopyTree(temp2);

	delete temp2;
	temp2 = NULL;
	temp = NULL;
}
//------------------------------------------------------------------------------
//add by wm 2013/05/16  10:47
void CSyntaxTree::__SprEmpNext(CSyntaxNode* pTree)
{
	CSyntaxNode*temp = NULL;
	//如果pTree为p and empty或者p and next q的形式
	if (pTree->GetNType() == AND_EXP)
	{

		if (pTree->GetChild1() == NULL)
		{
			cout << "__SprEmpNext() error!" << endl;
			return;
		}
		else
		{
			//pTree是p and empty的形式，将pTree加入到m_EmptyTree里边
			if (pTree->GetChild1()->GetNType() == EMPTY_EXP)
			{
				__Merge2EmpTree(pTree);
				return;
			}
			//pTree是p and next q的形式，将pTree加入到m_NextTree里边
			else if (pTree->GetChild1()->GetNType() == NEXT_STA)
			{
				if (pTree->GetChild1()->GetChild0() == NULL)
				{
					cout << "__SprEmpNext() error!" << endl;
					return;
				}

				__Merge2NextTree(pTree);

				return;
			}
		}
	}

	//如果pTree是empty形式，则m_EmptyTree就为empty
	else if (pTree->GetNType() == EMPTY_EXP)
	{

		temp = new CSyntaxNode(EMPTY_EXP, VOIDTYPE);
		m_EmptyTree->CopyTree(temp);
		delete temp;
		temp = NULL;
		return;
	}

	//pTree是next q的形式，将pTree加入到m_NextTree里边
	else if (pTree->GetNType() == NEXT_STA)
	{
		if (pTree->GetChild0() == NULL)
		{
			cout << "__SprEmpNext() error!" << endl;
			return;
		}
		else
		{
			//将pTree统一格式，化为true and ()q
			temp = new CSyntaxNode();
			temp->CopyTree(pTree->GetChild0());
			pTree->SetNType(AND_EXP);
			delete pTree->GetChild0();
			pTree->SetChild0(new CSyntaxNode(TRUE_EXP, VOIDTYPE));
			pTree->SetChild1(new CSyntaxNode(NEXT_STA, VOIDTYPE));
			pTree->GetChild1()->SetChild0(new CSyntaxNode());
			pTree->GetChild1()->GetChild0()->CopyTree(temp);
			__Merge2NextTree(pTree);
			delete temp;
			temp = NULL;
			return;
		}
	}

	//如果pTree是p or q 的形式，那么就分别判断pTree的左右子树该加到哪棵树上
	else if (pTree->GetNType() == OR_EXP)
	{
		if (pTree->GetChild0() == NULL || pTree->GetChild1() == NULL)
		{
			cout << "__SprEmpNext() error!" << endl;
			return;
		}
		else
		{
			__SprEmpNext(pTree->GetChild0());
			__SprEmpNext(pTree->GetChild1());
		}
	}
}
//--------------------------------------------------------------------------------------
//add by YY 2013/06/04 21:58
//programming by YK 2013/06/04 21:58
//------------------------------------------------------------------------------
//add by YY 2013/05/20 10:05
//modified by yangkai 2013/05/28 16:57
//------------------------------------------------------------------------------
//add by YY 2013/05/20 10:05
//modified by yangkai 2013/05/28 16:57
void CSyntaxTree::__NfAnd(CSyntaxNode *ptree)
{
	CSyntaxNode *temp = NULL, *temp1 = NULL, *temp2 = NULL, *temp3 = NULL;
	if (ptree->GetChild0() == NULL || ptree->GetChild1() == NULL)
	{
		cout << " __NfAnd error!" << endl;
		return;
	}
	//先进行判断是否为 u and e或者 u and ()q的情况
	//add by YY 2013/05/20 10:54
	if (__IsStatFormula(ptree->GetChild0()) &&
		(ptree->GetChild1()->GetNType() == EMPTY_EXP || ptree->GetChild1()->GetNType() == NEXT_STA))
	{
		return;
	}
	else if (__IsStatFormula(ptree->GetChild1()) &&
		(ptree->GetChild0()->GetNType() == EMPTY_EXP || ptree->GetChild0()->GetNType() == NEXT_STA))
	{
		temp = new CSyntaxNode(AND_EXP, VOIDTYPE);
		temp->SetChild0(new CSyntaxNode());
		temp->SetChild1(new CSyntaxNode());
		temp->GetChild0()->CopyTree(ptree->GetChild1());
		temp->GetChild1()->CopyTree(ptree->GetChild0());
		ptree->CopyTree(temp);
		delete temp;
		temp = NULL;
		return;
	}

	if (ptree->GetChild0() != NULL)
	{
		NF(ptree->GetChild0());
	}
	if (ptree->GetChild1() != NULL)
	{
		NF(ptree->GetChild1());
	}
	//--------------------(p1 or p2) and Q --> AND(p1,Q) or AND(p2,Q)------------------------
	if (ptree->GetChild0()->GetNType() == OR_EXP
		&&  ptree->GetChild0()->GetChild0() != NULL
		&&  ptree->GetChild0()->GetChild1() != NULL)
	{
		//AND(p1,Q)
		temp1 = new CSyntaxNode(AND_EXP, VOIDTYPE);
		temp1->SetChild0(new CSyntaxNode());
		temp1->GetChild0()->CopyTree(ptree->GetChild0()->GetChild0());
		temp1->SetChild1(new CSyntaxNode());
		temp1->GetChild1()->CopyTree(ptree->GetChild1());

		//__TrimTree(temp1);

		//		__Reduce(temp1);//临时修改
		//		SortTree(temp1);//临时修改
		NF(temp1);

		//AND(p2,Q)
		temp2 = new CSyntaxNode(AND_EXP, VOIDTYPE);
		temp2->SetChild0(new CSyntaxNode());
		temp2->GetChild0()->CopyTree(ptree->GetChild0()->GetChild1());
		temp2->SetChild1(new CSyntaxNode());
		temp2->GetChild1()->CopyTree(ptree->GetChild1());
		//__TrimTree(temp2);


		//		__Reduce(temp2);//临时修改
		//		SortTree(temp2);//临时修改
		NF(temp2);

		//AND(p1,Q) or AND(p2,Q)
		temp = new CSyntaxNode(OR_EXP, VOIDTYPE);
		temp->SetChild0(new CSyntaxNode());
		temp->GetChild0()->CopyTree(temp1);
		temp->SetChild1(new CSyntaxNode());
		temp->GetChild1()->CopyTree(temp2);

		ptree->CopyTree(temp);


		delete temp1;
		temp1 = NULL;
		delete temp2;
		temp2 = NULL;
		delete temp;
		temp = NULL;
		return;

	}
	//--------------------P and (q1 or q2) --> AND(P,q1) or AND(P,q2)------------------------
	else if (ptree->GetChild1()->GetNType() == OR_EXP
		&&  ptree->GetChild1()->GetChild0() != NULL
		&&  ptree->GetChild1()->GetChild1() != NULL)
	{
		//AND(P,q1)
		temp1 = new CSyntaxNode(AND_EXP, VOIDTYPE);
		temp1->SetChild0(new CSyntaxNode());
		temp1->GetChild0()->CopyTree(ptree->GetChild0());
		temp1->SetChild1(new CSyntaxNode());
		temp1->GetChild1()->CopyTree(ptree->GetChild1()->GetChild0());
		//__TrimTree(temp1);
		//		__Reduce(temp1);//临时修改
		//		SortTree(temp1);//临时修改
		NF(temp1);

		//AND(P,q2)
		temp2 = new CSyntaxNode(AND_EXP, VOIDTYPE);
		temp2->SetChild0(new CSyntaxNode());
		temp2->GetChild0()->CopyTree(ptree->GetChild0());
		temp2->SetChild1(new CSyntaxNode());
		temp2->GetChild1()->CopyTree(ptree->GetChild1()->GetChild1());
		//__TrimTree(temp2);
		//		__Reduce(temp2);//临时修改
		//		SortTree(temp2);//临时修改
		NF(temp2);

		//AND(P,q1) or AND(P,q2)
		temp = new CSyntaxNode(OR_EXP, VOIDTYPE);
		temp->SetChild0(new CSyntaxNode());
		temp->GetChild0()->CopyTree(temp1);
		temp->SetChild1(new CSyntaxNode());
		temp->GetChild1()->CopyTree(temp2);


		ptree->CopyTree(temp);


		delete temp1;
		temp1 = NULL;
		delete temp2;
		temp2 = NULL;
		delete temp;
		temp = NULL;
		return;
	}
	//false and p -> false
	//p and false ->false
	if (ptree->GetChild0()->GetNType() == FALSE_EXP || ptree->GetChild1()->GetNType() == FALSE_EXP)
	{
		temp = new CSyntaxNode(FALSE_EXP, VOIDTYPE);
		ptree->CopyTree(temp);
		delete temp;
		temp = NULL;
	}

	//(p and empty) and (q and empty) -> (p and q) and empty
	if (ptree->GetChild0() != NULL && ptree->GetChild1() != NULL &&
		ptree->GetChild0()->GetNType() == AND_EXP &&
		ptree->GetChild1()->GetNType() == AND_EXP &&
		ptree->GetChild0()->GetChild1() != NULL &&
		ptree->GetChild1()->GetChild1() != NULL &&
		ptree->GetChild0()->GetChild1()->GetNType() == EMPTY_EXP &&
		ptree->GetChild1()->GetChild1()->GetNType() == EMPTY_EXP)
	{
		temp = new CSyntaxNode(AND_EXP, VOIDTYPE);
		temp->SetChild0(new CSyntaxNode());
		temp->GetChild0()->CopyTree(ptree->GetChild0()->GetChild0());
		temp->SetChild1(new CSyntaxNode());
		temp->GetChild1()->CopyTree(ptree->GetChild1()->GetChild0());
		//__TrimTree(temp);
		//		__Reduce(temp);//对 p and q 进行精简 //临时修改
		//		SortTree(temp);//临时修改
		if (temp->GetNType() != FALSE_EXP)
		{
			temp1 = new CSyntaxNode(AND_EXP, VOIDTYPE);
			temp1->SetChild0(new CSyntaxNode());
			temp1->GetChild0()->CopyTree(temp);
			temp1->SetChild1(new CSyntaxNode(EMPTY_EXP, VOIDTYPE));
		}
		else
		{
			ptree->CopyTree(temp);
		}
		ptree->CopyTree(temp1);
		delete temp;
		temp = NULL;
		delete temp1;
		temp1 = NULL;
		return;
	}
	//empty and (q and empty) -> q and empty
	else	if (ptree->GetChild0() != NULL && ptree->GetChild0()->GetNType() == EMPTY_EXP &&
		ptree->GetChild1() != NULL && ptree->GetChild1()->GetNType() == AND_EXP &&
		ptree->GetChild1()->GetChild1() != NULL &&
		ptree->GetChild1()->GetChild1()->GetNType() == EMPTY_EXP)
	{
		if (ptree->GetChild1()->GetChild0() != NULL)
		{
			temp = new CSyntaxNode();
			temp->CopyTree(ptree->GetChild1()->GetChild0());
		}


		temp1 = new CSyntaxNode(AND_EXP, VOIDTYPE);
		temp1->SetChild0(new CSyntaxNode());
		temp1->GetChild0()->CopyTree(temp);
		temp1->SetChild1(new CSyntaxNode(EMPTY_EXP, VOIDTYPE));


		ptree->CopyTree(temp1);
		delete temp;
		temp = NULL;
		delete temp1;
		temp1 = NULL;
		return;
	}
	//(p and empty) and empty -> p and empty
	else if (ptree->GetChild0() != NULL && ptree->GetChild0()->GetNType() == AND_EXP &&
		ptree->GetChild0()->GetChild1() != NULL &&
		ptree->GetChild0()->GetChild1()->GetNType() == EMPTY_EXP &&
		ptree->GetChild1() != NULL &&
		ptree->GetChild1()->GetNType() == EMPTY_EXP)
	{
		if (ptree->GetChild0()->GetChild0() != NULL)
		{
			temp = new CSyntaxNode();
			temp->CopyTree(ptree->GetChild0()->GetChild0());
		}

		temp1 = new CSyntaxNode(AND_EXP, VOIDTYPE);
		temp1->SetChild0(new CSyntaxNode());
		temp1->GetChild0()->CopyTree(temp);
		temp1->SetChild1(new CSyntaxNode(EMPTY_EXP, VOIDTYPE));

		ptree->CopyTree(temp1);
		delete temp;
		temp = NULL;
		delete temp1;
		temp1 = NULL;
		return;
	}
	//()p and ()q -> ()(p and q)
	else if (ptree->GetChild0() != NULL && ptree->GetChild0()->GetNType() == NEXT_STA &&
		ptree->GetChild1() != NULL && ptree->GetChild1()->GetNType() == NEXT_STA)
	{
		temp1 = new CSyntaxNode(AND_EXP, VOIDTYPE);
		temp1->SetChild0(new CSyntaxNode());
		temp1->GetChild0()->CopyTree(ptree->GetChild0()->GetChild0());
		temp1->SetChild1(new CSyntaxNode());
		temp1->GetChild1()->CopyTree(ptree->GetChild1()->GetChild0());
		__Reduce(temp1);
		if (temp1->GetNType() != FALSE_EXP)
		{
			temp = new CSyntaxNode(NEXT_STA, VOIDTYPE);
			temp->SetChild0(new CSyntaxNode());
			temp->GetChild0()->CopyTree(temp1);
			ptree->CopyTree(temp);
			delete temp;
			temp = NULL;
			delete temp1;
			temp1 = NULL;
		}
		else
		{
			ptree->CopyTree(temp1);
		}
	}


	//(p1 and ()q1) and (p2 and ()q2) -> p1 and p2 and ()(q1 and q2)
	else if (ptree->GetChild0() != NULL && ptree->GetChild0()->GetNType() == AND_EXP &&
		ptree->GetChild1() != NULL && ptree->GetChild1()->GetNType() == AND_EXP &&
		ptree->GetChild0()->GetChild1() != NULL &&
		ptree->GetChild0()->GetChild1()->GetNType() == NEXT_STA &&
		ptree->GetChild1()->GetChild1() != NULL &&
		ptree->GetChild1()->GetChild1()->GetNType() == NEXT_STA)
	{
		if (ptree->GetChild0()->GetChild0() != NULL && ptree->GetChild1()->GetChild0() != NULL)
		{
			//temp=p1 and p2
			temp = new CSyntaxNode(AND_EXP, VOIDTYPE);
			temp->SetChild0(new CSyntaxNode());
			temp->GetChild0()->CopyTree(ptree->GetChild0()->GetChild0());
			temp->SetChild1(new CSyntaxNode());
			temp->GetChild1()->CopyTree(ptree->GetChild1()->GetChild0());
			//__TrimTree(temp);
			//			__Reduce(temp);//临时修改
			//			SortTree(temp);//临时修改
			if (temp->GetNType() != FALSE_EXP)
			{
				//temp1=q1 and q2
				temp1 = new CSyntaxNode(AND_EXP, VOIDTYPE);
				if (ptree->GetChild0()->GetChild1()->GetChild0() != NULL)
				{
					temp1->SetChild0(new CSyntaxNode());
					temp1->GetChild0()->CopyTree(ptree->GetChild0()->GetChild1()->GetChild0());
				}
				if (ptree->GetChild1()->GetChild1()->GetChild0() != NULL)
				{
					temp1->SetChild1(new CSyntaxNode());
					temp1->GetChild1()->CopyTree(ptree->GetChild1()->GetChild1()->GetChild0());
				}
				//__TrimTree(temp1);
				//				__Reduce(temp1);//临时修改
				//				SortTree(temp1);//临时修改
				if (temp1->GetNType() != FALSE_EXP)
				{
					//temp2=()(q1 and q2)
					temp2 = new CSyntaxNode(NEXT_STA, VOIDTYPE);
					temp2->SetChild0(new CSyntaxNode());
					temp2->GetChild0()->CopyTree(temp1);
					//temp3=(p1 and p2 and ()(q1 and q2))
					temp3 = new CSyntaxNode(AND_EXP, VOIDTYPE);
					temp3->SetChild0(new CSyntaxNode());
					temp3->GetChild0()->CopyTree(temp);
					temp3->SetChild1(new CSyntaxNode());
					temp3->GetChild1()->CopyTree(temp2);
					ptree->CopyTree(temp3);
				}
				else
				{
					ptree->CopyTree(temp1);
				}
			}
			else
			{
				ptree->CopyTree(temp);
			}
		}
		delete temp;
		temp = NULL;
		delete temp1;
		temp1 = NULL;
		delete temp2;
		temp2 = NULL;
		delete temp3;
		temp3 = NULL;
		return;
	}
	//()q1 and (p and ()q2) -> p and ()(q1 and q2)
	else if (ptree->GetChild0() != NULL && ptree->GetChild0()->GetNType() == NEXT_STA &&
		ptree->GetChild1() != NULL && ptree->GetChild1()->GetNType() == AND_EXP &&
		ptree->GetChild1()->GetChild1() != NULL &&
		ptree->GetChild1()->GetChild1()->GetNType() == NEXT_STA)
	{
		if (ptree->GetChild1()->GetChild0() != NULL)
		{
			//temp=p
			temp = new CSyntaxNode();
			temp->CopyTree(ptree->GetChild1()->GetChild0());


			//temp1=q1 and q2
			temp1 = new CSyntaxNode(AND_EXP, VOIDTYPE);
			if (ptree->GetChild0()->GetChild0() != NULL)
			{
				temp1->SetChild0(new CSyntaxNode);
				temp1->GetChild0()->CopyTree(ptree->GetChild0()->GetChild0());
			}
			if (ptree->GetChild1()->GetChild1()->GetChild0() != NULL)
			{
				temp1->SetChild1(new CSyntaxNode);
				temp1->GetChild1()->CopyTree(ptree->GetChild1()->GetChild1()->GetChild0());
			}
			//			__Reduce(temp1);//临时修改
			//   		SortTree(temp1);//临时修改
			if (temp1->GetNType() != FALSE_EXP)
			{
				//temp2=()(q1 and q2)
				temp2 = new CSyntaxNode(NEXT_STA, VOIDTYPE);
				temp2->SetChild0(new CSyntaxNode());
				temp2->GetChild0()->CopyTree(temp1);
				//temp3=p and (q1 and q2)
				temp3 = new CSyntaxNode(AND_EXP, VOIDTYPE);
				temp3->SetChild0(new CSyntaxNode());
				temp3->GetChild0()->CopyTree(temp);
				temp3->SetChild1(new CSyntaxNode());
				temp3->GetChild1()->CopyTree(temp2);
				ptree->CopyTree(temp3);
			}
			else
			{
				ptree->CopyTree(temp1);
			}

		}
		delete temp;
		temp = NULL;
		delete temp1;
		temp1 = NULL;
		delete temp2;
		temp2 = NULL;
		delete temp3;
		temp3 = NULL;
		return;
	}
	//(p and ()q1) and ()q2 -> p and ()(q1 and q2)
	else if (ptree->GetChild0() != NULL && ptree->GetChild0()->GetNType() == AND_EXP &&
		ptree->GetChild0()->GetChild1() != NULL &&
		ptree->GetChild0()->GetChild1()->GetNType() == NEXT_STA &&
		ptree->GetChild1() != NULL && ptree->GetChild1()->GetNType() == NEXT_STA)
	{
		if (ptree->GetChild0()->GetChild0() != NULL)
		{
			//temp=p
			temp = new CSyntaxNode();
			temp->CopyTree(ptree->GetChild0()->GetChild0());
			//temp1=q1 and q2
			temp1 = new CSyntaxNode(AND_EXP, VOIDTYPE);
			if (ptree->GetChild0()->GetChild1()->GetChild0() != NULL)
			{
				temp1->SetChild0(new CSyntaxNode());
				temp1->GetChild0()->CopyTree(ptree->GetChild0()->GetChild1()->GetChild0());
			}
			if (ptree->GetChild1()->GetChild0() != NULL)
			{
				temp1->SetChild1(new CSyntaxNode());
				temp1->GetChild1()->CopyTree(ptree->GetChild1()->GetChild0());
			}
			//__TrimTree(temp1);
			//			__Reduce(temp1);//临时修改
			//			SortTree(temp1);//临时修改
			if (temp1->GetNType() != FALSE_EXP)
			{
				//temp2=()(q1 and q2)
				temp2 = new CSyntaxNode(NEXT_STA, VOIDTYPE);
				temp2->SetChild0(new CSyntaxNode());
				temp2->GetChild0()->CopyTree(temp1);
				//temp3=p and ()(q1 and q2)
				temp3 = new CSyntaxNode(AND_EXP, VOIDTYPE);
				temp3->SetChild0(new CSyntaxNode());
				temp3->GetChild0()->CopyTree(temp);
				temp3->SetChild1(new CSyntaxNode());
				temp3->GetChild1()->CopyTree(temp2);
				ptree->CopyTree(temp3);
			}
			else
			{
				ptree->CopyTree(temp1);
			}
		}
		delete temp;
		temp = NULL;
		delete temp1;
		temp1 = NULL;
		delete temp2;
		temp2 = NULL;
		delete temp3;
		temp3 = NULL;
		return;
	}
	//左右子树同时含有next部分和empty部分
	else
	{
		if (__HasEmpty(ptree->GetChild0()) && __HasNext(ptree->GetChild1()) ||
			__HasEmpty(ptree->GetChild1()) && __HasNext(ptree->GetChild0()))
		{
			temp = new CSyntaxNode(FALSE_EXP, VOIDTYPE);
			ptree->CopyTree(temp);
			delete temp;
			temp = NULL;
			return;
		}
	}
}


//------------------------------------------------------------------------------
//added by yangkai 2013/05/29 10:44
void CSyntaxTree::__Reduce(CSyntaxNode *ptree)
{
	/*	if(ptree==NULL)
	{
	return;
	}
	else
	{
	//Dnf(ptree);//临时修改
	switch(ptree->GetNType())
	{
	case AND_EXP:__ReduceAnd(ptree); break;
	case OR_EXP:__ReduceOr(ptree);   break;
	default:
	if (ptree->GetChild0()!=NULL)
	{
	__Reduce(ptree->GetChild0());
	}
	if (ptree->GetChild1()!=NULL)
	{
	__Reduce(ptree->GetChild1());
	}

	break;
	}
	}*/
}
//------------------------------------------------------------------------------
//added by yangkai 2013/05/29 10:44
void CSyntaxTree::__ReduceAnd(CSyntaxNode *ptree)
{

	//化为统一格式，也就是将and移到左子树上 p and (q and r) -> (p and q) and r

	CSyntaxNode *temp = NULL;
	CSyntaxNode *temp1 = NULL;
	while (ptree->GetChild1() != NULL && ptree->GetChild1()->GetNType() == AND_EXP)
	{
		temp = new CSyntaxNode(AND_EXP, VOIDTYPE);
		temp->SetChild0(new CSyntaxNode());
		temp->GetChild0()->CopyTree(ptree->GetChild0());
		temp->SetChild1(new CSyntaxNode());
		temp->GetChild1()->CopyTree(ptree->GetChild1()->GetChild0());
		temp1 = new CSyntaxNode();
		temp1->CopyTree(ptree->GetChild1()->GetChild1());
		ptree->GetChild0()->CopyTree(temp);
		ptree->GetChild1()->CopyTree(temp1);
		delete temp;
		temp = NULL;
		delete temp1;
		temp1 = NULL;
	}

	//分别预处理第0棵子树和第1棵子树
	if (ptree->GetChild0() != NULL)
	{
		__Reduce(ptree->GetChild0());
	}
	if (ptree->GetChild1() != NULL)
	{
		__Reduce(ptree->GetChild1());
	}
	//p and p -> p
	if (ptree->GetChild0() != NULL&&ptree->GetChild1() != NULL&&
		ptree->GetChild0()->CompareTree(ptree->GetChild1()))
	{
		temp = new CSyntaxNode();
		temp->CopyTree(ptree->GetChild0());
		ptree->CopyTree(temp);
		delete temp;
		temp = NULL;
		return;
	}
	//p and true -> p
	if (ptree->GetChild0() != NULL&&ptree->GetChild1() != NULL&&
		ptree->GetChild1()->GetNType() == TRUE_EXP)
	{
		temp = new CSyntaxNode();
		temp->CopyTree(ptree->GetChild0());
		ptree->CopyTree(temp);
		delete temp;
		temp = NULL;
		return;
	}
	//true and p -> p
	if (ptree->GetChild0() != NULL&&ptree->GetChild1() != NULL&&
		ptree->GetChild0()->GetNType() == TRUE_EXP)
	{
		temp = new CSyntaxNode();
		temp->CopyTree(ptree->GetChild1());
		ptree->CopyTree(temp);
		delete temp;
		temp = NULL;
		return;
	}
	//p and false -> false
	if (ptree->GetChild0() != NULL&&ptree->GetChild1() != NULL&&
		ptree->GetChild1()->GetNType() == FALSE_EXP)
	{
		temp = new CSyntaxNode(FALSE_EXP, VOIDTYPE);
		ptree->CopyTree(temp);
		delete temp;
		temp = NULL;
		return;
	}
	//false and p -> false
	if (ptree->GetChild0() != NULL&&ptree->GetChild1() != NULL&&
		ptree->GetChild0()->GetNType() == FALSE_EXP)
	{
		temp = new CSyntaxNode(FALSE_EXP, VOIDTYPE);
		ptree->CopyTree(temp);
		delete temp;
		temp = NULL;
		return;
	}
	//p and !p -> false
	if (ptree->GetChild0() != NULL&&ptree->GetChild1() != NULL&&
		ptree->GetChild1()->GetNType() == NEGATION_EXP)
	{
		if (ptree->GetChild1()->GetChild0() != NULL
			&&ptree->GetChild0()->CompareTree(ptree->GetChild1()->GetChild0()))
		{
			temp = new CSyntaxNode(FALSE_EXP, VOIDTYPE);
			ptree->CopyTree(temp);
			delete temp;
			temp = NULL;

		}
		return;
	}
	//!p and p -> false
	if (ptree->GetChild0() != NULL&&ptree->GetChild1() != NULL&&
		ptree->GetChild0()->GetNType() == NEGATION_EXP)
	{
		if (ptree->GetChild0()->GetChild0() != NULL
			&&ptree->GetChild1()->CompareTree(ptree->GetChild0()->GetChild0()))
		{
			temp = new CSyntaxNode(FALSE_EXP, VOIDTYPE);
			ptree->CopyTree(temp);
			delete temp;
			temp = NULL;

		}
		return;
	}
	//empty and next p -> false
	if (ptree->GetChild0() != NULL&&ptree->GetChild1() != NULL&&
		ptree->GetChild0()->GetNType() == EMPTY_EXP&&
		ptree->GetChild1()->GetNType() == NEXT_STA)
	{
		temp = new CSyntaxNode(FALSE_EXP, VOIDTYPE);
		ptree->CopyTree(temp);
		delete temp;
		temp = NULL;
		return;
	}
	//next p and empty -> false
	if (ptree->GetChild0() != NULL&&ptree->GetChild1() != NULL&&
		ptree->GetChild0()->GetNType() == NEXT_STA&&
		ptree->GetChild1()->GetNType() == EMPTY_EXP)
	{

		temp = new CSyntaxNode(FALSE_EXP, VOIDTYPE);
		ptree->CopyTree(temp);
		delete temp;
		temp = NULL;
		return;
	}
	//左子树中包含next节点(必须是and的孩子),右子树为empty节点，这种情况化为false
	if (ptree->GetChild1() != NULL&&ptree->GetChild1()->GetNType() == EMPTY_EXP)
	{
		bool has_next = false;
		temp1 = new CSyntaxNode();
		temp1->CopyTree(ptree->GetChild0());
		while (temp1 != NULL&&temp1->GetNType() == AND_EXP&&
			temp1->GetChild0() != NULL&&temp1->GetChild0()->GetNType() == AND_EXP)
		{
			if (temp1->GetChild1() != NULL&&temp1->GetChild1()->GetNType() == NEXT_STA)
			{
				has_next = true;
				break;
			}
			temp = new CSyntaxNode();
			temp->CopyTree(temp1->GetChild0());
			temp1->CopyTree(temp);
			delete temp;
			temp = NULL;

		}
		if (temp1 != NULL&&temp1->GetNType() == AND_EXP&&
			temp1->GetChild0() != NULL&&temp1->GetChild0()->GetNType() != AND_EXP)
		{
			if (temp1->GetChild0() != NULL&&temp1->GetChild0()->GetNType() == NEXT_STA)
				has_next = true;
			if (temp1->GetChild1() != NULL&&temp1->GetChild1()->GetNType() == NEXT_STA)
				has_next = true;
		}
		if (has_next == true)
		{
			temp = new CSyntaxNode(FALSE_EXP, VOIDTYPE);
			ptree->CopyTree(temp);
			delete temp;
			temp = NULL;

		}
		delete temp1;
		temp1 = NULL;
		return;

	}
	//左子树中包含empty节点(必须是and的孩子),右子树是next节点
	if (ptree->GetChild1() != NULL&&ptree->GetChild1()->GetNType() == NEXT_STA)
	{
		bool has_empty = false;
		temp1 = new CSyntaxNode();
		temp1->CopyTree(ptree->GetChild0());
		while (temp1 != NULL&&temp1->GetNType() == AND_EXP&&
			temp1->GetChild0() != NULL&&temp1->GetChild0()->GetNType() == AND_EXP)
		{
			if (temp1->GetChild1() != NULL&&temp1->GetChild1()->GetNType() == EMPTY_EXP)
			{
				has_empty = true;
				break;
			}
			temp = new CSyntaxNode();
			temp->CopyTree(temp1->GetChild0());
			temp1->CopyTree(temp);
			delete temp;
			temp = NULL;
		}
		if (temp1 != NULL&&temp1->GetNType() == AND_EXP&&
			temp1->GetChild0() != NULL&&temp1->GetChild0()->GetNType() != AND_EXP)
		{
			if (temp1->GetChild0() != NULL&&temp1->GetChild0()->GetNType() == EMPTY_EXP)
				has_empty = true;
			if (temp1->GetChild1() != NULL&&temp1->GetChild1()->GetNType() == EMPTY_EXP)
				has_empty = true;
		}
		if (has_empty == true)
		{
			temp = new CSyntaxNode(FALSE_EXP, VOIDTYPE);
			ptree->CopyTree(temp);
			delete temp;
			temp = NULL;

		}
		delete temp1;
		temp1 = NULL;
		return;

	}
	/*左子树是and，右子树是其他(不是and和or)的情况，因为刚开始已经进行了整理，
	所以不会出现右子树是and的情况，因此只需要判断右子树不是or即可。例如：
	(p and q) and p -> p and q
	*/
	if (ptree->GetChild0() != NULL&&ptree->GetChild0()->GetNType() == AND_EXP&&
		ptree->GetChild1() != NULL&&ptree->GetChild1()->GetNType() != OR_EXP)
	{
		if (__FindInTree(ptree->GetChild0(), ptree->GetChild1(), AND_EXP))
		{
			temp = new CSyntaxNode();
			temp->CopyTree(ptree->GetChild0());
			ptree->CopyTree(temp);
			delete temp;
			temp = NULL;

		}
		return;
	}

	//处理左子树是and，右子树是其他的情况，含有非，例如(!p and !q) and p -> false
	if (ptree->GetChild0() != NULL&&ptree->GetChild0()->GetNType() == AND_EXP)
	{
		if (ptree->GetChild1() != NULL)
		{
			if (ptree->GetChild1()->GetNType() != NEGATION_EXP)
			{
				if (__FindNegInTree(ptree->GetChild0(), ptree->GetChild1(), AND_EXP))
				{
					temp = new CSyntaxNode(FALSE_EXP, VOIDTYPE);
					ptree->CopyTree(temp);
					delete temp;
					temp = NULL;
				}
			}
			else
			{
				if (__FindInTree(ptree->GetChild0(), ptree->GetChild1()->GetChild0(), AND_EXP))
				{
					temp = new CSyntaxNode(FALSE_EXP, VOIDTYPE);
					ptree->CopyTree(temp);
					delete temp;
					temp = NULL;
				}
			}
		}
		return;
	}
}
//------------------------------------------------------------------------------
//added by yangkai 2013/05/29 10:44
void CSyntaxTree::__ReduceOr(CSyntaxNode* ptree)
{
	CSyntaxNode *temp = NULL;
	CSyntaxNode *temp1 = NULL;
	CSyntaxNode *temp2 = NULL;

	//化为统一格式
	while (ptree->GetChild1() != NULL && ptree->GetChild1()->GetNType() == OR_EXP)
	{
		temp = new CSyntaxNode(OR_EXP, VOIDTYPE);//没有考虑返回值类型
		temp->SetChild0(new CSyntaxNode());
		temp->GetChild0()->CopyTree(ptree->GetChild0());
		temp->SetChild1(new CSyntaxNode());
		temp->GetChild1()->CopyTree(ptree->GetChild1()->GetChild0());
		temp1 = new CSyntaxNode();
		temp1->CopyTree(ptree->GetChild1()->GetChild1());
		ptree->GetChild0()->CopyTree(temp);
		ptree->GetChild1()->CopyTree(temp1);
		delete temp;
		temp = NULL;
		delete temp1;
		temp1 = NULL;
	}
	//分别预处理第0棵子树和第1棵子树
	if (ptree->GetChild0() != NULL)
	{
		__Reduce(ptree->GetChild0());
	}
	if (ptree->GetChild1() != NULL)
	{
		__Reduce(ptree->GetChild1());
	}

	//p or p -> p
	if (ptree->GetChild0() != NULL&&ptree->GetChild1() != NULL&&
		ptree->GetChild0()->CompareTree(ptree->GetChild1()))
	{
		temp = new CSyntaxNode();
		temp->CopyTree(ptree->GetChild0());
		ptree->CopyTree(temp);
		delete temp;
		temp = NULL;
		return;
	}
	//p or !p -> true
	else if (ptree->GetChild0() != NULL&&ptree->GetChild1() != NULL&&
		ptree->GetChild1()->GetNType() == NEGATION_EXP&&
		ptree->GetChild0()->CompareTree(ptree->GetChild1()->GetChild0()))
	{
		temp = new CSyntaxNode();
		temp = new CSyntaxNode(TRUE_EXP, VOIDTYPE);
		ptree->CopyTree(temp);
		delete temp;
		temp = NULL;
		return;
	}
	//p or true -> true; true or p-> true
	else if (ptree->GetChild0() != NULL&&ptree->GetChild1() != NULL &&
		(ptree->GetChild0()->GetNType() == TRUE_EXP ||
		ptree->GetChild1()->GetNType() == TRUE_EXP))
	{

		temp = new CSyntaxNode();
		temp = new CSyntaxNode(TRUE_EXP, VOIDTYPE);
		ptree->CopyTree(temp);
		delete temp;
		temp = NULL;
		return;
	}
	//p or false -> p
	else if (ptree->GetChild0() != NULL&&ptree->GetChild1() != NULL&&
		ptree->GetChild1()->GetNType() == FALSE_EXP)
	{
		temp = new CSyntaxNode();
		temp->CopyTree(ptree->GetChild0());
		ptree->CopyTree(temp);
		delete temp;
		temp = NULL;
		return;
	}
	//false or p -> p
	else if (ptree->GetChild0() != NULL&&ptree->GetChild1() != NULL&&
		ptree->GetChild0()->GetNType() == FALSE_EXP)
	{
		temp = new CSyntaxNode();
		temp->CopyTree(ptree->GetChild1());
		ptree->CopyTree(temp);
		delete temp;
		temp = NULL;
		return;
	}

	//!p or p -> true
	else if (ptree->GetChild0() != NULL&&ptree->GetChild1() != NULL&&
		ptree->GetChild0()->GetNType() == NEGATION_EXP&&
		ptree->GetChild0()->GetChild0()->CompareTree(ptree->GetChild1()))
	{
		temp = new CSyntaxNode();
		temp = new CSyntaxNode(TRUE_EXP, VOIDTYPE);
		ptree->CopyTree(temp);
		delete temp;
		temp = NULL;
		return;
	}
	//左子树是and，右子树是其他，不含非，例如(p and q) or p -> p
	if (ptree->GetChild0() != NULL&&ptree->GetChild0()->GetNType() == AND_EXP&&
		ptree->GetChild1() != NULL&&ptree->GetChild1()->GetNType() != AND_EXP)
	{
		if (__FindInTree(ptree->GetChild0(), ptree->GetChild1(), AND_EXP))
		{
			temp = new CSyntaxNode();
			temp->CopyTree(ptree->GetChild1());
			ptree->CopyTree(temp);
			delete temp;
			temp = NULL;

		}
		return;
	}
	//右子树是and，左子树是其他，不含非，例如p or (p and q) -> p
	if (ptree->GetChild1() != NULL&&ptree->GetChild1()->GetNType() == AND_EXP&&
		ptree->GetChild0() != NULL&&ptree->GetChild0()->GetNType() != AND_EXP)
	{
		if (__FindInTree(ptree->GetChild1(), ptree->GetChild0(), AND_EXP))
		{
			temp = new CSyntaxNode();
			temp->CopyTree(ptree->GetChild0());
			ptree->CopyTree(temp);
			delete temp;
			temp = NULL;

		}
		return;
	}
	//左子树是and，右子树是and，不含非，例如(p and q and r) or (p and q) -> p and q
	//(p and q) or (p and q and r) -> p and q
	if (ptree->GetChild0() != NULL&&ptree->GetChild0()->GetNType() == AND_EXP&&
		ptree->GetChild1() != NULL&&ptree->GetChild1()->GetNType() == AND_EXP)
	{
		bool find = true;
		temp1 = new CSyntaxNode();
		temp1->CopyTree(ptree->GetChild1());
		while (temp1 != NULL&&temp1->GetNType() == AND_EXP
			&&temp1->GetChild0() != NULL&&temp1->GetChild0()->GetNType() == AND_EXP)
		{
			if (!__FindInTree(ptree->GetChild0(), temp1->GetChild1(), AND_EXP))
			{
				find = false;
			}
			temp = new CSyntaxNode();
			temp->CopyTree(temp1->GetChild0());
			temp1->CopyTree(temp);
			delete temp;
			temp = NULL;
		}
		if (temp1 != NULL&&temp1->GetNType() == AND_EXP)
		{
			if (!__FindInTree(ptree->GetChild0(), temp1->GetChild0(), AND_EXP))
				find = false;
			if (!__FindInTree(ptree->GetChild0(), temp1->GetChild1(), AND_EXP))
				find = false;
		}
		if (find == true)
		{
			temp = new CSyntaxNode();
			temp->CopyTree(ptree->GetChild1());
			ptree->CopyTree(temp);
			delete temp;
			temp = NULL;
			return;
		}
		else
		{

			find = true;
			temp2 = new CSyntaxNode();
			temp2->CopyTree(ptree->GetChild0());
			while (temp2 != NULL&&temp2->GetNType() == AND_EXP&&
				temp2->GetChild0() != NULL&&temp2->GetChild0()->GetNType() == AND_EXP)
			{
				if (!__FindInTree(ptree->GetChild1(), temp2->GetChild1(), AND_EXP))
					find = false;
				temp = new CSyntaxNode();
				temp->CopyTree(temp2->GetChild0());
				temp2->CopyTree(temp);
				delete temp;
				temp = NULL;
			}
			if (temp2 != NULL&&temp2->GetNType() == AND_EXP)
			{
				if (!__FindInTree(ptree->GetChild1(), temp2->GetChild0(), AND_EXP))
					find = false;
				if (!__FindInTree(ptree->GetChild1(), temp2->GetChild1(), AND_EXP))
					find = false;
			}
			if (find == true)
			{
				temp = new CSyntaxNode();
				temp->CopyTree(ptree->GetChild0());
				ptree->CopyTree(temp);
				delete temp;
				temp = NULL;
			}
			delete temp2;
			temp2 = NULL;
		}
		delete temp1;
		temp1 = NULL;
		return;
	}

	//左子树根节点是or，右子树根节点是其他，不考虑非，例如(p or q) or p -> p or q
	if (ptree->GetChild0() != NULL&&ptree->GetChild0()->GetNType() == OR_EXP&&
		ptree->GetChild1() != NULL&&ptree->GetChild1()->GetNType() != AND_EXP)
	{
		if (__FindInTree(ptree->GetChild0(), ptree->GetChild1(), OR_EXP))
		{
			temp = new CSyntaxNode();
			temp->CopyTree(ptree->GetChild0());
			ptree->CopyTree(temp);
			delete temp;
			temp = NULL;
		}
		return;
	}

	/*左子树根节点是or，右子树根节点是其他，左子树含有右子树的非，
	例如(!p or q) or p -> true, (p or q) or !p -> true*/

	if (ptree->GetChild0() != NULL&&ptree->GetChild0()->GetNType() == OR_EXP&&
		ptree->GetChild1() != NULL&&ptree->GetChild1()->GetNType() != AND_EXP)
	{
		if (ptree->GetChild1()->GetNType() != NEGATION_EXP)
		{
			if (__FindNegInTree(ptree->GetChild0(), ptree->GetChild1(), OR_EXP))
			{
				temp = new CSyntaxNode(TRUE_EXP, VOIDTYPE);
				ptree->CopyTree(temp);
				delete temp;
				temp = NULL;
			}
		}
		else
		{
			if (__FindInTree(ptree->GetChild0(), ptree->GetChild1()->GetChild0(), OR_EXP))
			{
				temp = new CSyntaxNode(TRUE_EXP, VOIDTYPE);
				ptree->CopyTree(temp);
				delete temp;
				temp = NULL;
			}
		}
		return;

	}

	//左子树根节点是or,右子树根节点是and,不考虑非,例如(p or q) or (p and q) -> p or q
	if (ptree->GetChild0() != NULL&&ptree->GetChild0()->GetNType() == OR_EXP&&
		ptree->GetChild1() != NULL&&ptree->GetChild1()->GetNType() == AND_EXP)
	{
		bool find = false;
		temp1 = new CSyntaxNode();
		temp1->CopyTree(ptree->GetChild1());
		while (temp1 != NULL&&temp1->GetNType() == AND_EXP&&
			temp1->GetChild0() != NULL&&temp1->GetChild0()->GetNType() == AND_EXP)
		{
			if (__FindInTree(ptree->GetChild0(), temp1->GetChild1(), OR_EXP))
			{
				find = true;
				break;
			}
			temp = new CSyntaxNode();
			temp->CopyTree(temp1->GetChild0());
			temp1->CopyTree(temp);
			delete temp;
			temp = NULL;
		}
		if (temp1 != NULL&&temp1->GetNType() == AND_EXP)
		{

			if (__FindInTree(ptree->GetChild0(), temp1->GetChild0(), OR_EXP))
			{
				find = true;
			}
			if (__FindInTree(ptree->GetChild0(), temp1->GetChild1(), OR_EXP))
			{
				find = true;
			}
		}
		if (find == true)
		{
			temp = new CSyntaxNode();
			temp->CopyTree(ptree->GetChild0());
			ptree->CopyTree(temp);
			delete temp;
			temp = NULL;
		}
		delete temp1;
		temp1 = NULL;
		return;
	}
	/*左子树根节点是or,右子树根节点是and,考虑非,例如(!p or !q) or (p and q) -> true
	(p or !q) or (!p and q) -> true, (p or !q) or ( p and q) 不处理,也就是说，对于
	右子树的每个合取项，如果在左子树中都能找到它的非，则整个公式为true,否则不处理*/
	if (ptree->GetChild0() != NULL&&ptree->GetChild0()->GetNType() == OR_EXP&&
		ptree->GetChild1() != NULL&&ptree->GetChild1()->GetNType() == AND_EXP)
	{
		bool find = true;
		temp1 = new CSyntaxNode();
		temp1->CopyTree(ptree->GetChild1());
		while (temp1 != NULL&&temp1->GetNType() == AND_EXP&&
			temp1->GetChild0() != NULL&&temp1->GetChild0()->GetNType() == AND_EXP)
		{
			if (temp1->GetChild1() != NULL)
			{
				if (temp1->GetChild1()->GetNType() != NEGATION_EXP)
				{
					if (!__FindNegInTree(ptree->GetChild0(), temp1->GetChild1(), OR_EXP))
					{
						find = false;
						break;
					}
				}
				else
				{
					if (!__FindInTree(ptree->GetChild0(), temp1->GetChild1()->GetChild0(), OR_EXP))
					{
						find = false;
						break;
					}
				}
			}

			temp = new CSyntaxNode();
			temp->CopyTree(temp1->GetChild0());
			temp1->CopyTree(temp);
			delete temp;
			temp = NULL;
		}
		if (temp1 != NULL&&temp1->GetNType() == AND_EXP)
		{
			if (temp1->GetChild0() != NULL)
			{
				if (temp1->GetChild0()->GetNType() != NEGATION_EXP)
				{
					if (!__FindNegInTree(ptree->GetChild0(), temp1->GetChild0(), OR_EXP))
					{
						find = false;
					}
				}
				else
				{
					if (!__FindInTree(ptree->GetChild0(), temp1->GetChild0()->GetChild0(), OR_EXP))
					{
						find = false;
					}
				}
			}
			if (temp1->GetChild1() != NULL)
			{
				if (temp1->GetChild1()->GetNType() != NEGATION_EXP)
				{
					if (!__FindNegInTree(ptree->GetChild0(), temp1->GetChild1(), OR_EXP))
					{
						find = false;
					}
				}
				else
				{
					if (!__FindInTree(ptree->GetChild0(), temp1->GetChild1()->GetChild0(), OR_EXP))
					{
						find = false;
					}
				}
			}
		}
		if (find == true)
		{
			temp = new CSyntaxNode(TRUE_EXP, VOIDTYPE);
			ptree->CopyTree(temp);
			delete temp;
			temp = NULL;
		}
		delete temp1;
		temp1 = NULL;
		return;
	}
	/*左子树根节点是or,右子树根节点是and或其他,如(p and q) or (p and r) or q -> q or (p and r)*/
	if (ptree->GetChild0() != NULL && ptree->GetChild0()->GetNType() == OR_EXP)
	{
		temp = new CSyntaxNode();
		temp = ptree->GetChild0();
		while (temp != NULL && temp->GetNType() == OR_EXP)
		{
			if (__FindInTree(temp->GetChild1(), ptree->GetChild1()))
			{
				temp->GetChild1()->CopyTree(ptree->GetChild1());
				temp1 = new CSyntaxNode();
				temp1->CopyTree(ptree->GetChild0());
				ptree->CopyTree(temp1);
				__Reduce(ptree);
				temp = NULL;
				delete temp1;
				temp1 = NULL;
				return;
			}
			temp = temp->GetChild0();
		}
		if (temp != NULL && temp->GetNType() != OR_EXP)
		{
			if (__FindInTree(temp, ptree->GetChild1()))
			{
				temp->CopyTree(ptree->GetChild1());
				temp1 = new CSyntaxNode();
				temp1->CopyTree(ptree->GetChild0());
				ptree->CopyTree(temp1);
				__Reduce(ptree);
				temp = NULL;
				delete temp1;
				temp1 = NULL;
				return;
			}
		}
		return;
	}
}

//add by YY 2013/06/05 9:27
//programming by YK 2013/06/05 9:27
//------------------------------------------------------------------------------
bool CSyntaxTree::__HasEmpty(CSyntaxNode* ptree)
{
	if (ptree == NULL)
	{
		return false;
	}
	if (ptree->GetNType() == EMPTY_EXP)
	{
		return true;
	}
	else if (ptree->GetNType() == AND_EXP &&
		ptree->GetChild1()->GetNType() == EMPTY_EXP)
	{
		return true;
	}
	return false;
}
//------------------------------------------------------------------------------
bool CSyntaxTree::__HasNext(CSyntaxNode* ptree)
{
	if (ptree == NULL)
	{
		return false;
	}
	if (ptree->GetNType() == NEXT_STA)
	{
		return true;
	}
	else if (ptree->GetNType() == AND_EXP &&
		ptree->GetChild1()->GetNType() == NEXT_STA)
	{
		return true;
	}
	return false;
}

//------------------------------------------------------------------------------
bool CSyntaxTree::__FindInTree(CSyntaxNode* ptree, CSyntaxNode* target)
{

	CSyntaxNode* temp = NULL;
	if (ptree == NULL || target == NULL)
	{
		cout << "__FindInTree() error!" << endl;
		return false;
	}
	if (ptree->GetNType() != AND_EXP && target->GetNType() != AND_EXP)
	{
		if (target->CompareTree(ptree))
		{
			return true;
		}
	}
	if (ptree->GetNType() == AND_EXP && target->GetNType() != AND_EXP)
	{
		if (__FindInTree(ptree, target, AND_EXP))
		{
			return true;
		}
	}
	if (ptree->GetNType() == AND_EXP && target->GetNType() == AND_EXP)
	{
		temp = target;
		bool find = true;
		while (temp != NULL && temp->GetNType() == AND_EXP)
		{

			if (temp->GetChild1() != NULL && !__FindInTree(ptree, temp->GetChild1(), AND_EXP))
			{
				find = false;
				break;
			}
			temp = temp->GetChild0();
		}
		if (temp->GetNType() != AND_EXP && !__FindInTree(ptree, temp, AND_EXP))
		{
			find = false;
		}
		if (find == true)
		{
			return true;
		}
	}
	temp = NULL;
	return false;
}
//------------------------------------------------------------------------------
/*void CSyntaxTree::AddFin(CSyntaxNode* pTree,CGraphDraw* graph,int &k)
{
if(pTree==NULL)
{
cout<<"AddFin error"<<endl;
return;
}
if(pTree->GetNType()==OR_EXP || pTree->GetNType()==AND_EXP)
{
AddFin(pTree->GetChild0(),graph,k);
AddFin(pTree->GetChild1(),graph,k);
}
else if(pTree->GetNType()==CHOP_STA)
{
if(__NeedFin(pTree->GetChild0()))//需要加fin标记
{
if(!__HasFin(pTree->GetChild0(),graph))//是否已经加过fin标记
{
CSyntaxNode* temp=NULL;
temp=new CSyntaxNode(AND_EXP,VOIDTYPE);
temp->SetChild0(new CSyntaxNode());
temp->GetChild0()->CopyTree(pTree->GetChild0());
temp->SetChild1(new CSyntaxNode(FINAL_STA,VOIDTYPE));
//将k转为string类型的s begin
string s;
stringstream ss;
ss<<k;
s=ss.str();
//end
temp->GetChild1()->SetChild0(new CSyntaxNode(AUX_IDENT_EXP,s,NULL,VOIDTYPE));
pTree->GetChild0()->CopyTree(temp);
graph->GetCurrent()->FinSetInsert(k);
k++;
delete temp;
temp=NULL;
}
}
}
}
//------------------------------------------------------------------------------
bool CSyntaxTree::__NeedFin(CSyntaxNode* pTree)
{
//skip,len等都会Pre为()p的形式
if(pTree->GetNType()==NEXT_STA)
{
return false;
}
else if(pTree->GetNType()==AND_EXP)
{
if(!__NeedFin(pTree->GetChild0()) || !__NeedFin(pTree->GetChild1()))
{
return false;
}
}
return true;
}*/
//------------------------------------------------------------------------------
/*bool CSyntaxTree::__HasFin(CSyntaxNode* pTree, CGraphDraw* graph)
{
if(pTree->GetNType()==FINAL_STA && pTree->GetChild0()->GetNType()==AUX_IDENT_EXP)
{
graph->GetCurrent()->FinSetInsert(atoi(pTree->GetChild0()->GetNName().c_str()));
return true;
}
bool hasfin=false;
if(pTree->GetChild0()!=NULL)
{
if(__HasFin(pTree->GetChild0(),graph))
{
hasfin=true;
}
}
if(pTree->GetChild1()!=NULL)
{
if(__HasFin(pTree->GetChild1(),graph))
{
hasfin=true;
}
}
if(pTree->GetChild2()!=NULL)
{
if(__HasFin(pTree->GetChild2(),graph))
{
hasfin=true;
}
}
return hasfin;
}
*/
//------------------------------------------------------------------------------
//add by wm 2013/08/15  16:28
bool CSyntaxTree::CompareFinTree(CSyntaxNode* pTree)
{
	CSyntaxNode *temp1, *temp2;
	//先将顶点中的fin(p)结构删除掉，再进行比较begin
	temp1 = new CSyntaxNode();
	temp2 = new CSyntaxNode();
	temp1->CopyTree(this->GetRoot());
	temp2->CopyTree(pTree);
	DeleteFin(temp1);
	DeleteFin(temp2);

	__Reduce(temp1);
	__Reduce(temp2);
	if (temp1->CompareTree(temp2))
	{
		delete temp1;
		delete temp2;
		temp1 = NULL;
		temp2 = NULL;
		return true;
	}
	else
	{
		delete temp1;
		delete temp2;
		temp1 = NULL;
		temp2 = NULL;

		return false;
	}

}

//------------------------------------------------------------------------------
//add by wm 2013/08/15  16:28
void CSyntaxTree::DeleteFin(CSyntaxNode* pTree)
{
	CSyntaxNode *temp1 = NULL;
	if (pTree->GetNType() == CHOP_STA)
		__ChopDelFin(pTree);
	else if (pTree->GetNType() == AND_EXP)
	{
		if (pTree->GetChild0() == NULL || pTree->GetChild1() == NULL)
		{
			cout << "DeleteFin error!" << endl;
			return;
		}
		temp1 = pTree;
		while (temp1->GetNType() == AND_EXP)
		{
			if (temp1->GetChild1()->GetNType() == CHOP_STA)
				__ChopDelFin(temp1->GetChild1());
			temp1 = temp1->GetChild0();
		}
		if (temp1->GetNType() == CHOP_STA)
			__ChopDelFin(temp1);
		temp1 = NULL;

	}
}
//------------------------------------------------------------------------------
//add by wm 2013/08/15  18:19
void CSyntaxTree::__ChopDelFin(CSyntaxNode*pTree)
{
	CSyntaxNode *temp1 = NULL;

	if (pTree->GetChild0() == NULL || pTree->GetChild1() == NULL)
	{
		cout << "__ChopDelFin error!" << endl;
		return;
	}
	if (pTree->GetChild0()->GetNType() == FINAL_STA)
	{
		if (pTree->GetChild0()->GetChild0() == NULL)
		{
			cout << "__ChopDelFin error!" << endl;
			return;
		}

	}
	else if (pTree->GetChild0()->GetNType() == AND_EXP)
	{
		if (pTree->GetChild0()->GetChild0() == NULL || pTree->GetChild0()->GetChild1() == NULL)
		{
			cout << "__ChopDelFin error!" << endl;
			return;
		}
		if (pTree->GetChild0()->GetChild1()->GetNType() == FINAL_STA)
		{
			if (pTree->GetChild0()->GetChild1()->GetChild0() == NULL)
			{
				cout << "__ChopDelFin error!" << endl;
				return;
			}
		}

	}
}









