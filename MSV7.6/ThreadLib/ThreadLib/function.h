
#ifndef _FUNCTION_H
#define _FUNCTION_H

#include "AllNode.h"

extern int * stateNum;
extern void(*GlbOutput)();

struct $$Element
{
	DWORD threadId;
	string left;
	vector<string> right;
	int priority;//the num is smaller , the priority is higher
};

//搜集信息
void $$Push(char * left, char * right, int priority);

//当一段并行开始的时候，初始化调度线程，初始化线程树的根节点
void Init(int type);

//And Pal Prj Keep Alw Manager
//  0   1   2    3   4       5
//根据函数入口地址和参数创建线程，并为此线程生成Node*类型的结点，加入线程树中
void MyCreateThread(unsigned int (WINAPI addr)(LPVOID), LPVOID para, int type);

//暂停一个线程，之后调度线程会阻塞掉它
//为建模增加了一个参数，用来代表程序的位置编号
void MyPauseThread();

//这个函数只有主调线程最后会调用
void MyWaitForObject();

//获得MSV中的全局变量的信息
//参数1：状态数 参数2：全局变量输出函数
//void GetMsvVar(int * sNum, void(*output)(),bool *pathFalse);
void GetMsvVar(int * sNum, void(*output)());

void printPrsnt();

void printIdNode();


#endif