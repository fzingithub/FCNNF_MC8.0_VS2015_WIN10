#ifndef _MODELING_H
#define _MODELING_H

//��ý�ģ��Ҫ�ı��� �ֱ�Ϊ����ƴ�ӵ��ַ������׵�ַ���ַ����ĳ��ȵĵ�ַ��ƴ�Ӻ����ĺ���ָ��
void GetModelVar(char** var, int* l, void(*catVar)());

//�жϱ������Ƿ��ڽ�ģ��ģʽ
bool isModeling();

//��Modeling.lib���ݽ�ģ��Ҫ����Ϣ�������ݵ�ǰ״̬�����µĽ����ߣ�����ֵ��������Ƿ������ǰ·��
bool modelingCurrent();

//����һ���µĳ���λ��
void addStatement(int );

//Modeling.lib �еĺ�������������ѭ��������ֵ��������Ƿ������ǰ·��
extern bool AddEdge4Pal(const char* pStatementNum, const char* pVarsInfo);

//Modeling.lib �еĺ�������������or�ĳ���ʱ��������ʱ���������ģ�������
void palEnd();
#endif