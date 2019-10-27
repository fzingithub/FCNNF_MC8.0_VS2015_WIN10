#include "modeling.h"
#include <stdio.h>
#include <vector>
#include <sstream>
#include <algorithm>
using namespace std;


//当前语句的编号，由并行中所有线程的当前语句编号组成，使用addStatement函数为其增加编号
vector<int> statements;
extern int * stateNum;
//指向拼接而成的变量字符串的指针
char (*pVarInfo)[100000] = NULL;
//指向拼接而成的变量字符串长度的指针
int* pLen = NULL;
//拼接字符串的函数指针
void(*catVarValue)() = NULL;

//增加一个新的程序位置
void addStatement(int num)
{
	statements.push_back(num);
}

//获得建模需要的变量 分别为变量拼接的字符串的首地址，字符串的长度的地址，拼接函数的函数指针
void GetModelVar(char (*var)[100000], int* l, void(*catVar)())
{
	pVarInfo = var;
	pLen = l;
	catVarValue = catVar;
}

//判断编译器是否处于建模的模式
bool isModeling()
{
	return ((pVarInfo!=NULL) && (pLen!=NULL) && (catVarValue!=NULL));
}

//向Modeling.lib传递建模必要的信息，并根据当前状态产生新的结点与边，返回值用来辨别是否结束当前路径，返回true则说明找到循环，结束当前路径
bool modelingCurrent()
{
	//将vector拼接为程序位置的字符串
	sort(statements.begin(), statements.end());
	string program_pos;
	//stringstream ss;
	char buff[10] = "\0";
	for (unsigned int i = 0; i < statements.size(); ++i)
	{
		sprintf(buff,"%d", statements[i]);
		program_pos += string(buff);
		program_pos += "#";
		memset(buff,0,10);
	}
	statements.clear();

	catVarValue();
	//调用Modeling.lib的接口
	//printf("statenum %d param: %s  %s\n", *stateNum, program_pos.c_str(), *pVarInfo);
	bool res = AddEdge4Pal(program_pos.c_str(), *pVarInfo);
	//printf("end of threadlib \n");
	//当前状态结束，将拼接成的字符串置为空，下次拼接使用(因为这个字符串是全局变量)
	memset(*pVarInfo,0,*pLen);
	*pLen = 0;
	
	return res;
}