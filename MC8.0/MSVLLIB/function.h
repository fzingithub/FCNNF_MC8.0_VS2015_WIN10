
#ifndef _FUNCTION_H
#define _FUNCTION_H

#include "Node.h"

extern int *stateNum;
extern void(*GlbOutput)(void);

struct Element
{
	DWORD threadID;
	string left;
	vector<string> right;
	int priority;//the num is smaller , the priority is higher
};

extern "C" {
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
	void MyPauseThread(void);

	//�������ֻ�������߳��������
	void MyWaitForObject(void);

	//���MSV�е�ȫ�ֱ�������Ϣ
	//����1��״̬�� ����2��ȫ�ֱ����������
	//void GetMsvVar(int * sNum, void(*output)(),bool *pathFalse);
	void GetMsvVar(int *sNum, void(*output)(), int(*isout)());

	void setNodeLength(int type);

	void printPrsnt(void);
	void printIdNode(void);
}

#endif