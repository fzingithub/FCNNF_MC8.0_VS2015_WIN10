
#include <iostream>
#include <list>
#include <vector>
#include <map>
#include <string.h>
#include <string>

using namespace std;

/*
��MSVl���򲻺���or���ʱ��ʹ�ô��ļ��к������н�ģ
������ʹ�ý�ģ�ٶȸ���
�������޲������(���ThreadLib)ʱ��ֻ��Ҫ��while��Ѱ��ѭ����ʹ�õ�ȫ�ֱ���ΪwhileInfo
������ִ�е���������У�ÿ��״̬�ı�������Ҫ�Ƚϣ�ʹ�õ�ȫ�ֱ���ΪpalInfo
*/

// < function���+while�ı��, <������Ϣ����Ӧ��edgenum> >
map<unsigned int, map<string, int>* > whileInfo;

// < �������ı�ţ� <������Ϣ����Ӧ��edgenum> >
map<string, map<string, int>* > palInfo;

//��ģ�Ѿ��еıߵĸ���
static int edgeNum = 0;

extern "C" {
	//��external.c�е���
	extern char $$varsInfo[100000];//�����ñ�����ֵƴ�������Ļ���
	extern int  $$tempLen;
}


extern string GraphFileName;//���彨ģͼ�����ļ�·��

//��һ����from��to��ѭ��
void draw(int from,int to);

extern "C" {
	//while��ʼʱ���ô˺�����������ͬ��Ϣ���룬���ҵ�ѭ����������Ϣ����whileInfo
	//while2IR�в���˺����ĵ���
	void whileBegin(unsigned int functionNum, unsigned int whileNum)
	{
		if ((functionNum >> 16) != 0 || (whileNum >> 16) != 0)
		{
			cout << "whileBegin : functionNum or whileNum is bigger than 65535!" << endl;
			exit(0);
		}

		whileNum = (functionNum << 16) | whileNum;

		map<unsigned int, map<string, int>* >::iterator it1 = whileInfo.find(whileNum);
		//δ�ҵ�while��Ŷ�Ӧ����Ϣ
		if (it1 == whileInfo.end())
		{
			auto p = whileInfo.insert(make_pair(whileNum, new map<string, int>()));
			if (!p.second)
			{
				cout << "Modeling.lib -- whileBegin : insert whileNum error!" << endl;
				exit(0);
			}
			it1 = p.first;
		}

		map<string, int>* m = it1->second;
		string info($$varsInfo);
		memset($$varsInfo, 0, $$tempLen + 1);
		$$tempLen = 0;
		map<string, int>::iterator it2 = m->find(info);
		//û���ҵ���Ϣ����������Ϣ�������ҵ�ѭ����Ȼ��ͼ
		if (it2 == m->end())
		{
			if (m->insert(make_pair(info, edgeNum)).second == false)
			{
				cout << "Modeling.lib -- whileBegin : insert varInfo error!" << endl;
				exit(0);
			}
		}
		else
		{
			cout << "whileBegin : " << whileNum << endl;
			draw(edgeNum - 1, it2->second);
		}
	}
}

extern "C" {
	//���ΪwhileNum��while�����ˣ�ɾ����Ӧ��Ϣ
	//while2IR�в���˺����ĵ���
	void whileEnd(unsigned int functionNum, unsigned int whileNum)
	{
		if ((functionNum >> 16) != 0 || (whileNum >> 16) != 0)
		{
			cout << "Modeling.lib -- whileBegin : functionNum or whileNum is bigger than 65535!" << endl;
			exit(0);
		}

		whileNum = (functionNum << 16) | whileNum;

		map<unsigned int, map<string, int>* >::iterator it1 = whileInfo.find(whileNum);
		//δ�ҵ�while��Ŷ�Ӧ����Ϣ
		if (it1 != whileInfo.end())
		{
			delete it1->second;
			whileInfo.erase(it1);
		}
	}
}


//��ӡ���ϵ���Ϣ
void handleSinglePath(const char* pVarsInfo, int isEmpty)
{
	static FILE *fp = fopen((GraphFileName + ".txt").c_str(), "w+");
	fprintf(fp, "Edge %d : %s\n", edgeNum, pVarsInfo);
	++edgeNum;
	if (isEmpty)
	{
		draw(-1, -1);
	}
}

//�����в���
void handlePal(const char* pStatementNum, const char* pVarsInfo)
{
	handleSinglePath(pVarsInfo, false);

	map<string, map<string, int>* >::iterator it1 = palInfo.find(pStatementNum);
	//δ�ҵ������Ŷ�Ӧ����Ϣ
	if (it1 == palInfo.end())
	{
		auto p = palInfo.insert(make_pair(pStatementNum, new map<string, int>()));
		if (!p.second)
		{
			cout << "Modeling.lib -- handlePal : insert pStatementNum error!" << endl;
			exit(0);
		}
		it1 = p.first;
	}

	map<string, int>* m = it1->second;
	string info(pVarsInfo);
	map<string, int>::iterator it2 = m->find(info);
	//û���ҵ���Ϣ����������Ϣ�������ҵ�ѭ����Ȼ��ͼ
	if (it2 == m->end())
	{
		if (m->insert(make_pair(info, edgeNum)).second == false)
		{
			cout << "Modeling.lib -- handlePal : insert varInfo error!" << endl;
			exit(0);
		}
	}
	else
	{
		draw(edgeNum-1, it2->second);
	}

}

extern "C" {
	//���н���, ���palInfo,��ThreadLib���߳̽���ʱ����
	void palEnd(void)
	{
		for (auto it = palInfo.begin(); it != palInfo.end(); ++it)
			delete it->second;
		palInfo.clear();
	}
}

//���������յ�ļ�������ʵ�߻�������
void drawLine(FILE *fp,int from ,int to)
{
	if (from == 0)
		fprintf(fp, "%d[shape=doublecircle];\n", from);
	else
		fprintf(fp, "%d[shape=circle];\n", from);

	if (from + 6 <= to)
	{
		//����
		fprintf(fp, "%d[shape=circle];\n", from+1);
		fprintf(fp, "%d[label=\"...\",fontsize=35,color=white];\n", from+2);
		fprintf(fp, "%d[shape=circle];\n", to);
		//����
		fprintf(fp, "%d->%d[label=\" %d \"];\n", from, from + 1, from);
		fprintf(fp, "%d->%d[label=\" %d \",style=dashed];\n", from+1, from + 2, from+1);
		fprintf(fp, "%d->%d[label=\" %d \",style=dashed];\n", from + 2, to, to-1);

		return;
	}

	for (; from < to; ++from)
	{
		fprintf(fp, "%d->%d[label=\" %d \"];\n", from, from + 1, from);//��
		fprintf(fp, "%d[shape=circle];\n", from + 1);//��
	}

}

//дdot�ļ�
void draw(int from, int to)
{
	FILE* fp = fopen(GraphFileName.c_str(), "w+");
	//FILE* fp1 = fopen((GraphFileName + ".txt").c_str(), "w+");

	fprintf(fp, "digraph G{\n");
	fprintf(fp, "edge[fontcolor=red];\n");
	fprintf(fp, "node[width=0.4];\n");

	if (from == to && from == -1)
	{
		printf("��ѭ��: %d\n", edgeNum);
		drawLine(fp,0,edgeNum-1);
		fprintf(fp, "%d[shape=point, width=0.2];\n", edgeNum);//empty���
		fprintf(fp, "%d->%d[label=\" %d \"];\n", edgeNum - 1, edgeNum, edgeNum - 1);
	}
	else
	{
		printf("����ѭ�� ; %d -> %d\n", from, to);
		drawLine(fp, 0, to);
		fprintf(fp, "%d->%d[label=\" %d \"];\n", to, to + 1, to);//��
		drawLine(fp, to+1, from);
		fprintf(fp, "%d->%d[label=\" %d \"];\n", from, to, from);//��
	}
	fprintf(fp, "}");
	fclose(fp);

	exit(0);
}