
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

//�Ѽ���Ϣ
void $$Push(char * left, char * right, int priority);

//��һ�β��п�ʼ��ʱ�򣬳�ʼ�������̣߳���ʼ���߳����ĸ��ڵ�
void Init(int type);

//And Pal Prj Keep Alw Manager
//  0   1   2    3   4       5
//���ݺ�����ڵ�ַ�Ͳ��������̣߳���Ϊ���߳�����Node*���͵Ľ�㣬�����߳�����
void MyCreateThread(unsigned int (WINAPI addr)(LPVOID), LPVOID para, int type);

//��ͣһ���̣߳�֮������̻߳���������
//Ϊ��ģ������һ��������������������λ�ñ��
void MyPauseThread();

//�������ֻ�������߳��������
void MyWaitForObject();

//���MSV�е�ȫ�ֱ�������Ϣ
//����1��״̬�� ����2��ȫ�ֱ����������
//void GetMsvVar(int * sNum, void(*output)(),bool *pathFalse);
void GetMsvVar(int * sNum, void(*output)());

void printPrsnt();

void printIdNode();


#endif