#include <iostream>
#include <list>
#include <vector>
#include <map>
#include <string.h>
#include <string>

using namespace std;

/*
当MSVl程序不含有or语句时，使用此文件中函数进行建模
这样会使得建模速度更快
当程序无并行语句(详见ThreadLib)时，只需要在while处寻找循环，使用的全局变量为whileInfo
当程序执行到并行语句中，每个状态的变量都需要比较，使用的全局变量为palInfo
*/

// < function编号+while的编号, <变量信息，对应的edgenum> >
map<unsigned int, map<string, int>* > whileInfo;

// < 程序语句的编号， <变量信息，对应的edgenum> >
map<string, map<string, int>* > palInfo;

//建模已经有的边的个数
static int edgeNum = 0;

//从external.cpp中导入
extern char $$varsInfo[100000];//将所用变量及值拼接起来的缓存
extern int $$tempLen;

extern string GraphFileName;//定义建模图程序文件路径

//画一个从from到to的循环
void draw(int from,int to);

//while开始时调用此函数，若有相同信息存入，则找到循环，否则将信息存入whileInfo
//while2IR中插入此函数的调用
void whileBegin(unsigned int functionNum, unsigned int whileNum)
{
	if ((functionNum >> 16) != 0 || (whileNum >> 16) != 0)
	{
		cout << "Modeling.lib -- whileBegin : functionNum or whileNum is bigger than 65535!" << endl;
		exit(0);
	}

	whileNum = (functionNum << 16) | whileNum;

	map<unsigned int, map<string, int>* >::iterator it1 = whileInfo.find(whileNum);
	//未找到while编号对应的信息
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
	memset($$varsInfo,0,$$tempLen+1);
	$$tempLen = 0;
	map<string, int>::iterator it2 = m->find(info);
	//没有找到信息，则插入该信息，否则找到循环，然后画图
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
		draw(edgeNum-1, it2->second);
	}
}

//编号为whileNum的while结束了，删除对应信息
//while2IR中插入此函数的调用
void whileEnd(unsigned int functionNum, unsigned int whileNum)
{
	if ((functionNum >> 16) != 0 || (whileNum >> 16) != 0)
	{
		cout << "Modeling.lib -- whileBegin : functionNum or whileNum is bigger than 65535!" << endl;
		exit(0);
	}

	whileNum = (functionNum << 16) | whileNum;

	map<unsigned int, map<string, int>* >::iterator it1 = whileInfo.find(whileNum);
	//未找到while编号对应的信息
	if (it1 != whileInfo.end())
	{
		delete it1->second;
		whileInfo.erase(it1);
	}
}

//打印边上的信息
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

//处理并行部分
void handlePal(const char* pStatementNum, const char* pVarsInfo)
{
	handleSinglePath(pVarsInfo, false);

	map<string, map<string, int>* >::iterator it1 = palInfo.find(pStatementNum);
	//未找到程序编号对应的信息
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
	//没有找到信息，则插入该信息，否则找到循环，然后画图
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

//并行结束, 清除palInfo,在ThreadLib中线程结束时调用
void palEnd()
{
	for (auto it = palInfo.begin(); it != palInfo.end(); ++it)
		delete it->second;
	palInfo.clear();
}

//根据起点和终点的间距决定画实线还是虚线
void drawLine(FILE *fp,int from ,int to)
{
	if (from == 0)
		fprintf(fp, "%d[shape=doublecircle];\n", from);
	else
		fprintf(fp, "%d[shape=circle];\n", from);

	if (from + 6 <= to)
	{
		//画点
		fprintf(fp, "%d[shape=circle];\n", from+1);
		fprintf(fp, "%d[label=\"...\",fontsize=35,color=white];\n", from+2);
		fprintf(fp, "%d[shape=circle];\n", to);
		//画边
		fprintf(fp, "%d->%d[label=\" %d \"];\n", from, from + 1, from);
		fprintf(fp, "%d->%d[label=\" %d \",style=dashed];\n", from+1, from + 2, from+1);
		fprintf(fp, "%d->%d[label=\" %d \",style=dashed];\n", from + 2, to, to-1);

		return;
	}

	for (; from < to; ++from)
	{
		fprintf(fp, "%d->%d[label=\" %d \"];\n", from, from + 1, from);//边
		fprintf(fp, "%d[shape=circle];\n", from + 1);//点
	}

}

//写dot文件
void draw(int from, int to)
{
	FILE* fp = fopen(GraphFileName.c_str(), "w+");
	//FILE* fp1 = fopen((GraphFileName + ".txt").c_str(), "w+");

	fprintf(fp, "digraph G{\n");
	fprintf(fp, "edge[fontcolor=red];\n");
	fprintf(fp, "node[width=0.4];\n");

	if (from == to && from == -1)
	{
		printf("无循环: %d\n", edgeNum);
		drawLine(fp,0,edgeNum-1);
		fprintf(fp, "%d[shape=point, width=0.2];\n", edgeNum);//empty结点
		fprintf(fp, "%d->%d[label=\" %d \"];\n", edgeNum - 1, edgeNum, edgeNum - 1);
	}
	else
	{
		printf("发现循环 ; %d -> %d\n", from, to);
		drawLine(fp, 0, to);
		fprintf(fp, "%d->%d[label=\" %d \"];\n", to, to + 1, to);//边
		drawLine(fp, to+1, from);
		fprintf(fp, "%d->%d[label=\" %d \"];\n", from, to, from);//边
	}
	fprintf(fp, "}");
	fclose(fp);

	exit(0);
}