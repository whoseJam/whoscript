#ifndef FUNENVIRONMENTHPP
#define FUNENVIRONMENTHPP

#include<iostream>
#include<cstring>
#include<map>

#include"runtimeError.hpp"
#include"function.hpp"

class funEnvironment{
public:
	std::map<std::string,function*> fMap;
	void define(std::string& name,function* f,interpreter* i);
	function* find(std::string& name);
};

#endif