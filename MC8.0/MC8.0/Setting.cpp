#include "Setting.h"

string Setting::MSVLHOME = "";
string Setting::inPath = "_MSVInput.m";
vector<InputFile> Setting::inFiles;
bool Setting::isFunc = false;

string Setting::outPath = "";
string Setting::outName = "a.exe";
string Setting::outBCName = "a.bc";
string Setting::outDotName = "a.graph.dot";
bool Setting::genExe = true;

bool Setting::isOutputStatus = false;
int Setting::maxStatus = 100;
int Setting::statusInterval = 10;

bool Setting::Modeling = false;