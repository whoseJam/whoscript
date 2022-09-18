#ifndef FUNCTIONHPP
#define FUNCTIONHPP

#include<iostream>
#include<cstring>

#include"statement.hpp"

class function{
public:
	int line;
	std::vector<std::string> arguments;
	std::string name;
	statement* body;
	void output();
	function(int line);
	function(std::vector<std::string>arguments,std::string name,statement* body,int line);
	static void readFunction(parser* p,interpreter* i);
	bool nestedFunction();
};

#endif