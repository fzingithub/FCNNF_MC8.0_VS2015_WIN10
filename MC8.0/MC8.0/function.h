#ifndef FUNCTION_H_INCLUDED
#define FUNCTION_H_INCLUDED

#include <stdarg.h>

void errorSummary();
void error(const char *cFormat, ...);


#include <iostream>
#include <string>
#include <map>

using namespace std;

class CMAP
{
private:
	map<string, string> m_structName;
public:
	CMAP(){};
	~CMAP(){
		clear();
	}

	bool find(const string pStructName){
		auto it = m_structName.find(pStructName);
		if (it == m_structName.end())
			return false;
		else
			return true;
	}

	bool insert(const string pStructName){
		return m_structName.insert({ pStructName, pStructName }).second;
	}

	void clear(){
		m_structName.clear();
	}
};

#endif