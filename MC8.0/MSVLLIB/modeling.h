#ifndef _MODELING_H
#define _MODELING_H

#define VARINFOLEN 100000

extern "C" {
	//��ý�ģ��Ҫ�ı��� �ֱ�Ϊ����ƴ�ӵ��ַ������׵�ַ���ַ����ĳ��ȵĵ�ַ��ƴ�Ӻ����ĺ���ָ��
	void GetModelVar(char (*var)[VARINFOLEN], int* l, void(*catVar)(void));

	//�жϱ������Ƿ��ڽ�ģ��ģʽ
	int isModeling(void);

	//��Modeling.lib���ݽ�ģ��Ҫ����Ϣ�������ݵ�ǰ״̬�����µĽ����ߣ�����ֵ��������Ƿ������ǰ·��
	int modelingCurrent(void);

	//����һ���µĳ���λ��
	void addStatement(int);

	//Modeling.lib �еĺ�������������ѭ��������ֵ��������Ƿ������ǰ·��
	extern int AddEdge4Pal(const char *pStatementNum, const char *pVarsInfo);

	//Modeling.lib �еĺ�������������or�ĳ���ʱ��������ʱ���������ģ�������
	void palEnd(void);
}

#endif