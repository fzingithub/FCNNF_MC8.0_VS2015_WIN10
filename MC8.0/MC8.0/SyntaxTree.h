#ifndef SYNTAXTREE_INCLUDED
#define SYNTAXTREE_INCLUDED

#include <string>
#include "SyntaxNode.h"
//#include "SymbolTable.h"
//#include "GraphNode.h"
//#include "GraphDraw.h"


//class CGraphDraw;

using namespace std;

/**
* 语法树类，存储程序与性质语法树，进行语法树的预处理与化NF工作
*/

class CSyntaxTree
{
public:
	///构造函数
	CSyntaxTree();
	/**
	* 构造函数
	* @param 语法树
	* @return
	*/
	CSyntaxTree(CSyntaxNode* pRoot);
	/**
	* 合并语法树
	* @param1 待合并的语法树
	* @param2 合并结点的结点类型
	* @param3 合并结点的返回值类型
	* @return 是否合并成功，成功返回true，失败返回false
	*/
	bool MergeTree(CSyntaxNode*pTree, NODETYPE NType=AND_EXP, RETURNTYPE RType=VOIDTYPE);


	/**
	* yangkai
	* 合并语法树,和函数MergeTree的相反，将新添加的树作为孩子0
	* @param1 待合并的语法树
	* @param2 合并结点的结点类型
	* @param3 合并结点的返回值类型
	* @return 是否合并成功，成功返回true，失败返回false
	*/
	bool MergeTree_2(CSyntaxNode*pTree, NODETYPE NType = AND_EXP, RETURNTYPE RType = VOIDTYPE);

	/**
	* 对语法树进行预处理
	* @param 待处理的语法树
	* @return void
	*/
	void Pre(CSyntaxNode*);

	/**
	* 对图中节点加fin标记
	* @param 节点对应的公式
	* @param 加fin标记的节点
	* @return void
	*/
	//	void AddFin(CSyntaxNode*,CGraphDraw*,int&);

	/**
	* 对语法树进行NF
	* @param 待处理的语法树
	* @return void
	*/
	void NF(CSyntaxNode *pTree);

	///获得语法树根节点指针
	inline CSyntaxNode* GetRoot()
	{
		return m_Root;
	}

	/**
	* 设置语法书树根节点指针
	* @param  语法树根节点
	* @return void
	*/
	inline void SetRoot(CSyntaxNode *pRoot)
	{
		m_Root = pRoot;
	}

	/**
	* 将一个公式化成析取范式
	* @param 待处理的语法树根节点
	* @return void
	*/
	void Dnf(CSyntaxNode *ptree);

	/**
	* 将一个公式化成析取范式所用到的and处理小函数
	* @param 待处理的语法树根节点
	* @return void
	*/
	void DnfAnd(CSyntaxNode * ptree);
	/**
	* 将一个公式化成析取范式所用到的neg处理小函数
	* @param 待处理的语法树根节点
	* @return void
	*/
	void DnfNeg(CSyntaxNode * ptree);

	/**
	* 将一棵语法树中的empty部分和next部分整理到最后，如
	p and empty and q -> p and q and empty
	next p and next q -> next(p and q)
	next p and q and next r -> q and next(p and r)
	* @param 要整理的语法树
	* @return void
	*/
	void SortTree(CSyntaxNode *);

	/**
	* 用于树的整理，即将and , or整理为靠左排列  chop靠右排列
	* @param 需要整理的语法树
	* @return void
	*/
	void TrimTree(CSyntaxNode *);

	/**
	* 设置树中的所有的Pre标志位为指定的标志true或者false
	* @param1 需要设置成的标志
	* @param2 要处理的树
	* return void
	*/
	void SetAllPreFlag(bool PreFlag, CSyntaxNode * pTree);

	/**
	* 比较树是否fin等价
	* @param 待比较的结点
	* @return bool
	*/
	bool CompareFinTree(CSyntaxNode* pTree);

	/**
	* 删除fin标记
	* @param 待删除fin标记的树
	* @return bool
	*/
	void DeleteFin(CSyntaxNode* pTree);

	/**
	* 删除一个分支chop前的fin标记
	* @param 待删除fin标记的树
	* @return bool
	*/
	void __ChopDelFin(CSyntaxNode*pTree);

	///析构函数
	virtual ~CSyntaxTree();

private:

	/**
	*对树中的以and和or为根的子树进行精简，相当于PreAnd和PreOr
	*@param 要处理的树
	*return void
	*/
	void __Reduce(CSyntaxNode*);

	/**
	* 将两棵树用给定类型的结点合并，并放到第一棵树上，但此时的语法树并不是规范
	的语法树，需要调用 __DeletePost()来将其规范化
	* @param1 需要合并的第一棵语法树
	* @param2 需要合并的第二棵语法树
	* @param3 以该结点类型的结点合并两棵树
	* @param4 以该返回值类型的结点合并两棵树
	* @return 返回最后一级给定类型结点的位置
	*/
	CSyntaxNode* __MergeTree(CSyntaxNode*, CSyntaxNode*, NODETYPE, RETURNTYPE);

	/**
	* 整理合并以后的不规范的语法树，删除最后一级的不该有的结点，将其规范化
	* @param1 需要删除最后一级结点整理的语法树
	* @param2 需要删除的最后一级结点语法树的结点类型
	* @return void
	*/
	void __DeletePost(CSyntaxNode*, NODETYPE);

	/**
	* 判断当前公式中是否已经加过fin标记，有的话将其编号加入该公式对应节点的
	标记集合，并返回fin标记个数
	* @param1 需要判断的公式的语法树
	* @return 是否加入了fin标记，加入返回true，未加入返回false
	*/
	//	bool __HasFin(CSyntaxNode*,CGraphDraw*);

	/**
	* 判断语法树中是否需要加入fin标记
	* @param 需要判断的公式的语法树
	* @return 需要加fin标记返回true，不需要加fin标记返回false
	*/
	bool __NeedFin(CSyntaxNode* pTree);



	/**
	* 对and进行预处理
	* @param 待处理的语法树
	* @return void
	*/
	void    __PreAnd(CSyntaxNode*);


	/**
	* 对or进行预处理
	* @param 待处理的语法树
	* @return void
	*/
	void    __PreOr(CSyntaxNode*);


	/**
	* 对len进行预处理
	* @param 待处理的语法树
	* @return void
	*/
	void    __PreLen(CSyntaxNode*);


	/**
	* 对skip进行预处理
	* @param 待处理的语法树
	* @return void
	*/
	void	__PreSkip(CSyntaxNode*);


	/**
	* 对more进行预处理
	* @param 待处理的语法树
	* @return void
	*/
	void	__PreMore(CSyntaxNode*);


	/**
	* 对非进行预处理
	* @param 待处理的语法树
	* @return void
	*/
	void	__PreNeg(CSyntaxNode*);


	/**
	* 对next进行预处理
	* @param 待处理的语法树
	* @return void
	*/
	void	__PreNext(CSyntaxNode*);


	/**
	* 对;进行预处理
	* @param 待处理的语法树
	* @return void
	*/
	void	__PreChop(CSyntaxNode*);


	/**
	* 对prj进行预处理
	* @param 待处理的语法树
	* @return void
	*/
	void	__PrePrj(CSyntaxNode*);

	/**
	* 对蕴含进行预处理
	* @param 待处理的语法树
	* @return void
	*/
	void	__PreImply(CSyntaxNode*);


	/**
	* 对等价关系进行预处理
	* @param 待处理的语法树
	* @return void
	*/
	void	__PreEq(CSyntaxNode*);


	/**
	* 对weak next进行预处理
	* @param 待处理的语法树
	* @return void
	*/
	void	__PreWNext(CSyntaxNode*);
	/**
	* 对always进行预处理
	* @param 待处理的语法树
	* @return void
	*/
	void	__PreAlways(CSyntaxNode*);
	/**
	* 对sometimes进行预处理
	* @param 待处理的语法树
	* @return void
	*/
	void	__PreSomtims(CSyntaxNode*);
	/**
	* 对chopstar进行预处理
	* @param 待处理的语法树
	* @return void
	*/
	void	__PreChopstar(CSyntaxNode*);

	/**
	* 对并行进行预处理
	* @param 待处理的语法树
	* @return void
	*/
	void	__PreParallel(CSyntaxNode*);


	/**
	* 对await进行预处理
	* @param 待处理的语法树
	* @return void
	*/
	void	__PreAwait(CSyntaxNode*);


	/**
	* 对halt进行预处理
	* @param 待处理的语法树
	* @return void
	*/
	void    __PreHalt(CSyntaxNode*);


	/**
	* 对Keep进行预处理
	* @param 待处理的语法树
	* @return void
	*/
	void    __PreKeep(CSyntaxNode*);


	/**
	* 对Fin进行预处理
	* @param 待处理的语法树
	* @return void
	*/
	void    __PreFin(CSyntaxNode*);

	/**
	* 对:=进行预处理
	* @param 待处理的语法树
	* @return void
	*/
	void    __PreUnAssign(CSyntaxNode*);
	/**
	* 对=进行预处理,将 ()x=y 化成()(x=(-)y)的形式
	* @param 待处理的语法树
	* @return void
	*/
	void    __PreAssEQ(CSyntaxNode*);

	/**
	* 将状态公式化成NF形式
	* @param 待化成NF形式的语法树
	* @return void
	*/
	void    __NfStatFormula(CSyntaxNode*);
	/**
	* 将always化成NF形式
	* @param 待化成NF形式的语法树
	* @return void
	*/
	void	__NfAlwys(CSyntaxNode*);
	/**
	* 将keep化成NF形式
	* @param 待化成NF形式的语法树
	* @return void
	*/
	void	__NfKeep(CSyntaxNode*);
	/**
	* 将final化成NF形式
	* @param 待化成NF形式的语法树
	* @return void
	*/
	void	__NfFinal(CSyntaxNode*);

	/**
	* 将halt化成NF形式
	* @param 待化成NF形式的语法树
	* @return void
	*/
	void	__NfHalt(CSyntaxNode*);

	/**
	* 将await化成NF形式
	* @param 待化成NF形式的语法树
	* @return void
	*/
	void	__NfAwait(CSyntaxNode*);

	/**
	* 将chopstar化成NF形式
	* @param 待化成NF形式的语法树
	* @return void
	*/

	void	__NfChopStar(CSyntaxNode*);
	/**
	* 将and化成NF形式
	* @param 待化成NF形式的语法树
	* @return void
	*/
	void	__NfAnd(CSyntaxNode*);
	/**
	* 将or化成NF形式
	* @param 待化成NF形式的语法树
	* @return void
	*/
	void	__NfOr(CSyntaxNode*);
	/**
	* 将true化成NF形式
	* @param 待化成NF形式的语法树
	* @return void
	*/
	void	__NfTrue(CSyntaxNode*);
	/**
	* 将somtimes化成NF形式
	* @param 待化成NF形式的语法树
	* @return void
	*/
	void __NfSomtims(CSyntaxNode*);
	/**
	* 将;化成NF形式
	* @param 待化成NF形式的语法树
	* @return void
	*/
	void	__NfChop(CSyntaxNode*);
	/**
	* 将非化成NF形式
	* @param 待化成NF形式的语法树
	* @return void
	*/
	void	__NfNeg(CSyntaxNode*);
	/**
	* 将prj化成NF形式
	* @param 待化成NF形式的语法树
	* @return void
	*/
	void	__NfPrj(CSyntaxNode*);

	/**
	* 判断一个语法树表示的公式是否是状态公式
	* @param 待判断的语法树
	* @return void
	*/
	bool	__IsStatFormula(CSyntaxNode*);

	/**
	* 将一个公式化成完全NF形式
	* @param 待处理的语法树
	* @return void
	*/
	void	__Conf(CSyntaxNode *pTree);

	/**
	* 将一个p and next q形式的公式化成完全NF形式
	* @param 待处理的语法树
	* @return void
	*/
	void    __ConfNext(CSyntaxNode *pTree);
	/**
	* 将带有Or的NF形式的语法树化为完全NF形式
	* @param1 需要化为完全NF形式的语法树的or的未化成完全NF部分
	* @param2 需要化为完全NF形式的语法树的or的已化成完全NF部分
	* @return void
	*/
	void   __ConfOr(CSyntaxNode *, CSyntaxNode*);
	/**
	* 将带有n个Or的NF形式的语法树化为完全NF形式
	* @param1 需要化为完全NF形式的语法树的or的未化成完全NF部分
	* @param2 需要化为完全NF形式的语法树的or的已化成完全NF部分
	* @return void
	*/
	void   __ConfNOr(CSyntaxNode *pTree, CSyntaxNode *pConfTree);
	/**
	* 对emptyTree取非
	* @param1 需要取非的emptyTree树
	* @param2 emptyTree取非以后的树
	* @return void
	*/
	void   __NegEmpTree(CSyntaxNode*, CSyntaxNode*);
	/**
	* 对NextTree取非
	* @param 取非的NextTree树
	* @return void
	*/
	void   __NegNextTree(CSyntaxNode*);

	/**
	* 将语法树并到m_EmptyTree上
	* @param 需要并到m_EmptyTree上的语法树
	* @return void
	*/
	void   __Merge2EmpTree(CSyntaxNode*);
	/**
	* 将语法树并到m_NextTree上
	* @param 需要并到m_NextTree上的语法树
	* @return void
	*/
	void   __Merge2NextTree(CSyntaxNode*);
	/**
	* 删除m_EmptyTree或m_NextTree上的最后一级的or
	* @param 需要删除最后一级or的语法树
	* @return void
	*/
	void   __DeleteOr(CSyntaxNode*);
	/**
	* 将NF后的语法树，分为有empty和有next的两种情况，分别存放到两棵语法树上
	* @param 需要分离的语法树
	* @return void
	*/
	void   __SprEmpNext(CSyntaxNode*);


	/**
	*对树中的以and为根的子树进行精简，相当于PreAnd
	*@param 要处理的树
	*return void
	*/
	void __ReduceAnd(CSyntaxNode*);
	/**
	*对树中的以or为根的子树进行精简，相当于PreOr
	*@param 要处理的树
	*return void
	*/
	void __ReduceOr(CSyntaxNode*);

	/**
	*查找Nf形式的一个析取项中是否包含empty部分，只被__NfAnd调用
	*@param 要查找的树
	*return 如果包含empty部分返回true，否则返回false
	*/
	bool __HasEmpty(CSyntaxNode*);
	/**
	*查找Nf形式的一个析取项中是否包含next部分，只被__NfAnd调用
	*@param 要查找的树
	*return 包含next部分返回true，不包含返回false
	*/
	bool __HasNext(CSyntaxNode*);
	/**
	* 在一棵树中找目标子树
	* @param1 待查找的树
	* @param2 要查找的树
	* @return 找到要查找的树返回true，没有返回false
	*/
	bool __FindInTree(CSyntaxNode*, CSyntaxNode*);

	/**
	* 在一棵树中找目标子树
	* @param1 待查找的树
	* @param2 要查找的树
	* @param2 根节点类型
	* @return 找到要查找的树返回true，没有返回false
	*/
	bool __FindInTree(CSyntaxNode *, CSyntaxNode*, NODETYPE);


	/**
	* 在一棵树中找目标子树的非
	* @param1 待查找的树
	* @param2 要查找的树
	* @param2 根节点类型
	* @return 找到返回true，没找到返回false
	*/
	bool __FindNegInTree(CSyntaxNode *, CSyntaxNode*, NODETYPE);


	/**
	* 被__NfChopStar函数调用，实现next形式化为nextchop形式
	* @param1 待化简节点
	* @param2 用来存储nextchop部分的全局变量NextChopStarTree
	* @param3 没有经过任何操作的最原始的chopstar树
	* @return void
	*/
	void __NfChopStar_Next2Chop(CSyntaxNode *, CSyntaxNode *, CSyntaxNode *);




private:  //2013-4-27 add by YY[fixed]

	///语法树根结点
	CSyntaxNode *m_Root;
	///包含Next的语法树
	CSyntaxNode *m_NextTree;
	///empty的语法树
	CSyntaxNode *m_EmptyTree;
};

//end wm





#endif // SYNTAXTREE_INCLUDED
