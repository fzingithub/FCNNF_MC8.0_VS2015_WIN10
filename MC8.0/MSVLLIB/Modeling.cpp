#include "modeling.h"
#include <stdio.h>
#include <vector>
#include <sstream>
#include <algorithm>
using namespace std;

//��ǰ���ı�ţ��ɲ����������̵߳ĵ�ǰ�������ɣ�ʹ��addStatement����Ϊ�����ӱ��
vector<int> statements;

extern int *stateNum;

//ָ��ƴ�Ӷ��ɵı����ַ�����ָ��
char (*pVarInfo)[VARINFOLEN] = NULL;

//ָ��ƴ�Ӷ��ɵı����ַ������ȵ�ָ��
int *pLen = NULL;

//ƴ���ַ����ĺ���ָ��
void(*catVarValue)() = NULL;

extern "C" {
	//����һ���µĳ���λ��
	void addStatement(int num)
	{
		statements.push_back(num);
	}
}

extern "C" {
	//��ý�ģ��Ҫ�ı��� �ֱ�Ϊ����ƴ�ӵ��ַ������׵�ַ���ַ����ĳ��ȵĵ�ַ��ƴ�Ӻ����ĺ���ָ��
	void GetModelVar(char (*var)[VARINFOLEN], int* l, void(*catVar)(void)){
		pVarInfo = var;
		pLen = l;
		catVarValue = catVar;
	}
}

extern "C" {
	//�жϱ������Ƿ��ڽ�ģ��ģʽ
	int isModeling()
	{
		return ((pVarInfo != NULL) && (pLen != NULL) && (catVarValue != NULL));
	}
}

extern "C" {
	//��Modeling.lib���ݽ�ģ��Ҫ����Ϣ�������ݵ�ǰ״̬�����µĽ����ߣ�����ֵ��������Ƿ������ǰ·��������true��˵���ҵ�ѭ����������ǰ·��
	int modelingCurrent()
	{
		//��vectorƴ��Ϊ����λ�õ��ַ���
		sort(statements.begin(), statements.end());
		string program_pos;
		//stringstream ss;
		char buff[10] = "\0";
		for (unsigned int i = 0; i < statements.size(); ++i)
		{
			sprintf(buff, "%d", statements[i]);
			program_pos += string(buff);
			program_pos += "#";
			memset(buff, 0, 10);
		}
		statements.clear();

		catVarValue();
		//����Modeling.lib�Ľӿ�
		//printf("statenum %d param: %s  %s\n", *stateNum, program_pos.c_str(), *pVarInfo);
		bool res = AddEdge4Pal(program_pos.c_str(), *pVarInfo);
		//printf("end of threadlib \n");
		//��ǰ״̬��������ƴ�ӳɵ��ַ�����Ϊ�գ��´�ƴ��ʹ��(��Ϊ����ַ�����ȫ�ֱ���)
		memset(*pVarInfo, 0, *pLen);
		*pLen = 0;

		return res;
	}
}