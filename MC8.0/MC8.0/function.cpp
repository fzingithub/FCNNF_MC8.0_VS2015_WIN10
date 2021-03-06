// stdafx.cpp : source file that includes just the standard includes
//	MSV.pch will be the pre-compiled header
//	stdafx.obj will contain the pre-compiled type information

#include "function.h"
#include <iostream>
using namespace std;

extern int g_nErrors;
extern int lineno;

void error(const char *cFormat, ...)
{
	va_list args;
	++g_nErrors;
	cerr << "\nline " << lineno << ": ";
	va_start(args, cFormat);
	printf( cFormat, args);
	va_end(args);
	cerr << endl;
}

void errorSummary()  //错误概况
{
	cerr << g_nErrors << " error(s) were found." << endl;
}

void yyerror(const char *msg)
{
	error(msg);
}

extern "C" int yywrap(void)
{
	return 1;
}
