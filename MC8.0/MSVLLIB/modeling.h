#ifndef _MODELING_H
#define _MODELING_H

#define VARINFOLEN 100000

extern "C" {
	//获得建模需要的变量 分别为变量拼接的字符串的首地址，字符串的长度的地址，拼接函数的函数指针
	void GetModelVar(char (*var)[VARINFOLEN], int* l, void(*catVar)(void));

	//判断编译器是否处于建模的模式
	int isModeling(void);

	//向Modeling.lib传递建模必要的信息，并根据当前状态产生新的结点与边，返回值用来辨别是否结束当前路径
	int modelingCurrent(void);

	//增加一个新的程序位置
	void addStatement(int);

	//Modeling.lib 中的函数，用来处理循环，返回值用来辨别是否结束当前路径
	extern int AddEdge4Pal(const char *pStatementNum, const char *pVarsInfo);

	//Modeling.lib 中的函数，当处于无or的程序时，结束的时候清除并行模块的数据
	void palEnd(void);
}

#endif