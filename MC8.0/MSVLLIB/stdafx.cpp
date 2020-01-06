
#include <iostream>
#include <list>
#include <vector>
#include <string>

using namespace std;

string GraphFileName;//定义建模图程序文件路径
int *currentOr=NULL;
int *lengthOr = NULL;
char *flagOr = NULL;
void(*MyLongJmpPtr)() = NULL;

int hasOr;//程序是否含有or语句

void handleSinglePath(const char* pVarsInfo, int isEmpty);
void handlePal(const char* pStatementNum, const char* pVarsInfo);

extern "C" {
	void GetExternalVars(int *pCurrentOr, int *pLengthOr, char *pFlagOr, void(*f)(void), char * outFile)
	{
		currentOr = pCurrentOr;
		lengthOr = pLengthOr;
		flagOr = pFlagOr;
		MyLongJmpPtr = f;

		//dot的文件名，不加后缀
		string s(outFile);
		int pos = s.find_last_of("\\");
		int pos1 = s.find_last_of("/");
		pos = pos > pos1 ? pos : pos1;
		GraphFileName = s.erase(0, pos + 1);
		//filename = filename.substr();
	}
}

extern "C" {
	void setModelOption(bool or )
	{
		hasOr = or ;
	}
}

int currentNode=0;
int lastNode=0;
int num = 0;
int emptyNode = -1;

int currentNode2Or[10000];

struct Edge
{
	int num;
	int isEmpty;
	int from;
	int to;
	string varsInfo;
};

struct Location
{
	int edgeNum;
	string statementNum;
	string varsInfo;
};

list<Edge> edgeList;
vector<Location> LocVect;

void AddBackEdge(string varsInfo, int n)
{
	int to = -1;
	list<Edge>::iterator iter = edgeList.begin();
	for (; iter != edgeList.end(); iter++)
	{
		if (iter->num == n)
		{
			to = iter->to;
		}
	}

	Edge newEdge = { num, 0, currentNode, to, varsInfo };

	num = num + 1;

	edgeList.push_back(newEdge);
}

int FindSameNode(const char* pStatementNum, const char* VarInfo)
{
    if (*currentOr == *lengthOr){
		vector<Location>::iterator itr = LocVect.begin();
		for (; itr != LocVect.end(); itr++){
			if (itr->statementNum == pStatementNum && itr->varsInfo == VarInfo){
				return itr->edgeNum;
			}
		}
	}
	return -1;
}

int HandleSameNode(const char* pStatementNum, const char* pVarInfo)
{
	int n = FindSameNode(pStatementNum, pVarInfo);

	if (n >= 0){
		AddBackEdge(pVarInfo, n);
		return 1;
	}
	else if (*currentOr == *lengthOr){
		Location newLoc = { num, pStatementNum, pVarInfo };
		LocVect.push_back(newLoc);
	}
	return 0;
}

void GenerateEdge(const char* pVarsInfo, int isEmpty)
{
	int to = lastNode + 1;
	
	if (*currentOr == *lengthOr)//只在新的分支之后添加新的边
	{
		if (isEmpty){
			if (emptyNode == -1){
				emptyNode = to;//第一次遇到empty节点时记录empty节点编号
			}
			else{
				to = emptyNode;
				lastNode--;
			} 
		}

		Edge newEdge = { num, isEmpty, currentNode, to, pVarsInfo };

		edgeList.push_back(newEdge);
		num = num + 1;
		currentNode = lastNode + 1;
		lastNode = lastNode + 1;
	}
}

void AddEdge(const char* pVarsInfo, const char* pStatementNum, int isEmpty);

void AddEdge(const char* pVarsInfo, int pStatementNum, int isEmpty)
{
	char statementNum[10] = { '\0' };
	sprintf(statementNum, "%d", pStatementNum);
	AddEdge(pVarsInfo, (const char*)statementNum, isEmpty);
}

void AddEdge(const char *pVarsInfo, const char *pStatementNum, int isEmpty)
{
	//如果没有or，转到singlePath处理
	if (!hasOr){
		handleSinglePath(pVarsInfo, isEmpty);
		return;
	}

	if (HandleSameNode(pStatementNum, pVarsInfo)){
		MyLongJmpPtr();
	}
	else{
		GenerateEdge(pVarsInfo, isEmpty);
	}
}

bool HandleSameNode4Pal(const char* pStatementNum, const char* pVarsInfo)
{
	int n = FindSameNode(pStatementNum, pVarsInfo);
	if (n >= 0){
		AddBackEdge(pVarsInfo, n);
		return 1;
	}
	else if (*currentOr==*lengthOr){
		Location newLoc = { num, pStatementNum, pVarsInfo };

		LocVect.push_back(newLoc);
		
	}
	return 0;
}

extern "C" {
	bool AddEdge4Pal(const char* pStatementNum, const char* pVarsInfo)
	{
		if (!hasOr) {
			handlePal(pStatementNum, pVarsInfo);
			return 0;
		}

		if (HandleSameNode4Pal(pStatementNum, pVarsInfo)) {
			return -1;
		}
		else {
			GenerateEdge(pVarsInfo, 0);
			return 0;
		}
	}
}

void SaveCurrentNode()
{
	currentNode2Or[*currentOr] = currentNode;
}

void UpdateCurrentNode()
{
	currentNode = currentNode2Or[*currentOr];
}

extern "C" {
	void Draw()
	{
		//打开或创建一个.dot文件
		FILE* fp = fopen(GraphFileName.c_str(), "w+");
		FILE* fp1 = fopen((GraphFileName + ".txt").c_str(), "w+");
		fprintf(fp, "digraph G{\n");
		//设定图的大小尺寸
		//GraphFile.Write("size=\"", strlen("size=\""));    //size="
		//CRect rect;

		//	GetWindowRect(&rect);	
		/*int width = rect.Width();
		int height = rect.Height();
		tempstr1.Format("%d", width);    //tepstr1 =1180
		tempstr2.Format("%d", height);   //tempstr2 =963
		tempstr1 += ",";
		tempstr1 += tempstr2;
		tempstr1 += "\";\n";   //tempstr1 =1180,963";\n
		GraphFile.Write(tempstr1, strlen(tempstr1));*/
		//add by YY 2013/08/21 9:17 设定图的大小尺寸

		fprintf(fp, "edge[fontcolor=red];\n");
		fprintf(fp, "node[width=0.4];\n");

		list <Edge>::iterator itr = edgeList.begin();

		/*for (; itr != edgeList.end(); itr++)
		{
			cout << "edgeNum=" << itr->num << "location="  <<"  varInfo" << itr->varsInfo.c_str() << endl;
		}
		cout << "*****************************************************" << endl;


		for (vector<Location>::iterator it = LocVect.begin(); it != LocVect.end(); ++it)
			cout << "edgeNum : " << it->edgeNum << "stateNUM: " << it->statementNum.c_str() << "varInfo:  " << it->varsInfo.c_str() << endl;*/

		list<Edge>::iterator iter = edgeList.begin();

		//list<Edge>::iterator iter = edgeList.begin();
		for (; iter != edgeList.end(); iter++)
		{
			fprintf(fp, "%d", iter->from);
			if (iter->from == 0)
			{
				fprintf(fp, "[shape=doublecircle];\n");
			}
			else
			{
				fprintf(fp, "[shape=circle];\n");
			}

			fprintf(fp, "%d", iter->from);
			fprintf(fp, "->");
			fprintf(fp, "%d", iter->to);

			fprintf(fp, "[label=\"");
			fprintf(fp1, "Edge %d : %s\n", iter->num, iter->varsInfo.c_str());
			fprintf(fp, "%d", iter->num);
			fprintf(fp, "\"];\n");

			if (iter->isEmpty)
			{
				fprintf(fp, "%d", (iter)->to);
				fprintf(fp, "[shape=point, width=0.2];\n");
			}
		}

		fprintf(fp, "}");
		fclose(fp);


		cout << "start drawing : " << GraphFileName.c_str() << endl;

		//system("dot -Tpdf -Gsize=100  \"C:\\msvc\\_MSVGraph.dot\" -o \"C:\\msvc\\_result.pdf\"");
		//system("C:\\msvc\\_result.pdf");
	}
}