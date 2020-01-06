#ifndef SETTING_H_
#define SETTING_H_

//#define RELEASE_MC //发布时取消注释

#ifndef RELEASE_MC
#define PAUSE system("pause")
#else
#define PAUSE
#endif

#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>

using namespace std;

static const char *CLANGCMD = "clang ";

enum INPUTTYPE{
	M_FILE,
	FUNC_FILE,
	BC_FILE,
	OTHER_FILE
};

struct InputFile{
	string path;
	INPUTTYPE type;
};

//根据命令行参数对工程进行设置
class Setting
{
private:
	Setting() = delete;
	static string MSVLHOME;
	static string inPath;//待编译文件输入路径
	static vector<InputFile> inFiles;
	static bool isFunc; //是否是.func文件

	static string outPath;//可执行程序的输出路径
	static string outName;//输出文件的名字,与待编译文件名相同
	static string outBCName;
	static string outDotName;
	static bool genExe;//是否生成最终的exe文件
	
	static bool isOutputStatus;//是否对每个状态进行输出
	static int maxStatus;//输出的最大状态数
	static int statusInterval;//状态间隔
	static bool Modeling;//yangkai 建模

public:
	static void Setting::setInfo(int argc,char* argv[])
	{
		char *home = getenv("MSVL_HOME");
		if (home == NULL){
			MSVLHOME = "./";
		}
		else{
			MSVLHOME = home;
			int pos = MSVLHOME.find_first_of("/");
			while (pos != string::npos){
				MSVLHOME = MSVLHOME.replace(pos,1,"\\");
				pos = MSVLHOME.find_first_of("/");
			}
			if ((pos = MSVLHOME.find_last_of("\\")) != (MSVLHOME.length() - 1)){
				MSVLHOME = MSVLHOME + "\\";
			}
			MSVLHOME = MSVLHOME + "MC\\";
		}
		cout << MSVLHOME << endl;
		Modeling = false;

		for (int i = 1; i < argc; ++i){
			string s = argv[i];
			if (s == "-h" || s == "-help" || s == "--help")
				Usage(0);

			if (s == "-c")
				__setOutputType(false);
			else if (s == "-o"){
				if (++i < argc){
					string ss = argv[i];
					if (ss.find_first_of("-") == 0
						|| (ss.length() > 7 && ss.substr(0, 7) == "output=")
						|| (ss.length() > 9 && ss.substr(0, 9) == "maxstate=")
						|| (ss.length() > 4 && ss.substr(0, 4) == "gap="))
					{
						Error(ss + " can't appear after -o");
					}
					__setOutputPathAndName(ss);
				}
				else
					Error("lack of a argument after -o");
			}
			else if (s.length() > 7 && s.substr(0, 7) == "output=")
				__setIsOutput(s);
			else if (s.length() > 9 && s.substr(0, 9) == "maxstate=")
				__setMaxStatus(s);
			else if (s.length() > 4 && s.substr(0, 4) == "gap=")
				__setGap(s);
			else if (s == "-model" || s == "-m" || s == "-M")
				Modeling = true;
			else
				__setInputFile(s);
		}
		
		switch (inFiles.size()){
		case 0: Error("no input file to deal"); break;
		case 1: 
			if (inFiles[0].type == M_FILE){
				isFunc = false;
				inPath = inFiles[0].path;
			}
			else if (inFiles[0].type == FUNC_FILE){
				if (genExe)
					Error("a .func file cannot be generated a .exe file");
				else{
					inPath = inFiles[0].path;
					isFunc = true;
				}
			}
			else if (inFiles[0].type == BC_FILE){
				if (genExe)
					exit(system((CLANGCMD + MSVLHOME + "MSVLLIB.lib " + inFiles[0].path + " -o " + outPath + outName).data()));
				else
					exit(system(("COPY " + inFiles[0].path + " " + outPath + outName).data()));
			}
			break;
		default:
			string cmd = CLANGCMD + MSVLHOME + "MSVLLIB.lib  -o " + outPath + outName + " ";
			for (size_t i = 0; i < inFiles.size(); ++i){
				if (inFiles[i].type != BC_FILE)
					Error("too many input files and all of them are not .bc files");
				cmd += inFiles[i].path + " ";
			}
			exit(system(cmd.data()));
		}
	}

private:
	static void __setInputFile(string s){
		if (s.empty())
			Error("invalid argument: empty string");

		int pos = s.find_last_of(".");
		if (pos == string::npos || pos == s.length() - 1)
			Error(s + ": unable to identify input file type");
		
		string postfix = s.substr(pos  + 1,s.length() - pos - 1);
		if (postfix == "m")
			inFiles.push_back({ s, M_FILE });
		else if (postfix == "func")
			inFiles.push_back({ s, FUNC_FILE });
		else if (postfix == "bc")
			inFiles.push_back({ s, BC_FILE });
		else
			Error("unsupported data type");
	}

	static void __setIsOutput(string s){
		static bool isDone = false;
		if (isDone)
			Error("repeat to set the argument " + s);
		else
			isDone = true;

		s = s.substr(7, s.length() - 7); //output=true
		if (s == "true")
			isOutputStatus = true;
		else if (s == "false")
			isOutputStatus = false;
		else
			Error("invalid argument: " + s);
	}

	static void __setMaxStatus(string s){
		static bool isDone = false;
		if (isDone)
			Error("repeat to set the argument " + s);
		else
			isDone = true;

		s = s.substr(9, s.length() - 9);//maxstate=1000000
		int num;
		if (sscanf(s.data(), "%d", &num) != -1)
			maxStatus = num;
		else
			Error("invalid number : " + s);
	}

	static void __setGap(string s){
		static bool isDone = false;
		if (isDone)
			Error("repeat to set the argument " + s);
		else
			isDone = true;

		s = s.substr(4, s.length() - 4);//gap=1
		int num;
		if (sscanf(s.data(), "%d", &num) != -1)
			statusInterval = num;
		else
			Error("invalid positive integer : " + s);
	}

	static void Setting::__setOutputPathAndName(string s){
		static bool isDone = false;
		if (isDone)
			Error("repeat to set the argument " + s);
		else
			isDone = true;

		if (s.empty()) //Default
			return;

		// directory
		int pos = s.find_last_of('\\');

		if (pos == string::npos)
			outPath = ".\\";
		else if (pos == s.length() - 1)
			Error("invalid output path: " + s);
		else
			outPath = s.substr(0, pos);

		// file name
		s = s.substr(pos + 1, s.length() - pos - 1);
		pos = s.find_last_of(".");
		if (pos == 0)
			Error("invalid output name:" + s);
		if (pos == string::npos || pos != 0){
			string prefix = s.substr(0, pos);
			string postfix = s.substr(pos + 1, s.length() - pos - 1);
			if (postfix == "bc" || postfix == "exe"){
				outName = prefix + ".exe";
				outBCName = prefix + ".bc";
				outDotName = prefix + ".dot";
			}
			else{
				outName = s + ".exe";
				outBCName = s + ".bc";
				outDotName = s + ".dot";
			}
		}
	}

	/*
		bool type: 
			true: genExe = true 
			false: genExe = false
	*/
	static void __setOutputType(bool type){
		static bool isDone = false;
		if (isDone)
			Error("repeat to set the argument -c");
		else
			isDone = true;
		genExe = type;
	}

	static void Usage(int rtn = -1){
		cout
			<< "\n\nThe most common usage:\n\n"
			<< "    MC a.m -o a.exe | MC a.m -c -o a.bc | MC a.bc b.bc -o ab.exe\n\n"
			<< "Usage:\n    MC [status options] <input files> [output options]\n"
			<< "status options:\n"
			<< "    output=<true|false>: output status \n"
			<< "        MC output=true a.m -o a.exe\n"
			<< "    [note: The following two options only when output=true effectively]\n"
			<< "    maxstate=<positive number>: max status number\n"
			<< "        MC output=true maxstate=100000 a.m -o a[.exe]\n"
			<< "        MC output=true maxstate=100000 a.m -c -o a[.bc]\n"
			<< "    gap=<positive number>: the interval between two printed status\n"
			<< "        MC output=true gap=2\n" 
			<< "        MC output=true maxstate=100000 gap=10\n"
			<< "input files:\n"
			<< "    one .m file: MC a.m\n"
			<< "    one .func file: MC a.func\n"
			<< "    one or more .bc files: MC a.bc b.bc ...\n"
			<< "output options:\n"
			<< "    -o <path>: output file path \n"
			<< "        MC a.m -o a.exe\n"
			<< "    -c: compile input file and output .bc file \n"
			<< "        MC a.m -c -o a.bc\n\n" << endl;
		exit(rtn);
	}

	static void Error(string msg){
		cout << "ERROR: " << msg << endl;
		exit(-1);
	}

public:
	static string getHome(){
		return MSVLHOME;
	}

	static string getInPath(){
		return inPath;
	}

	static string getOutPath(){
		return outPath;
	}

	static string getOutName(){
		return outName;
	}

	static string getOutBCName(){
		return outBCName;
	}

	static string getOutDotName(){
		return outDotName;
	}

	static bool isFuncFile(){
		return isFunc;
	}

	static bool isGenExe(){
		return genExe;
	}

	static bool isOutput(){
		return isOutputStatus;
	}

	static int getMaxStatus(){
		return maxStatus;
	}

	static int getStatusInterval(){
		return statusInterval;
	}

	static bool IsModeling(){
		return Modeling;
	}

	static bool IsSimulation(){
		return !Modeling;
	}

	static string getOutFilePath(){
		return getOutPath() + getOutName();
	}

	static string getOutBCFilePath(){
		return getOutPath() + getOutBCName();
	}

	static string getExternalSPath(){
		return MSVLHOME + "external.s";
	}
};
#endif