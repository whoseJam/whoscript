#ifndef RUNTIMEERRORHPP
#define RUNTIMEERRORHPP

#include<iostream>
#include<cstring>

class runtimeError{
public:
	static void report(int line,std::string& msg);
	static void report(int line,const char* msg);
};

#endif