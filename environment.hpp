#ifndef ENVIRONMENTHPP
#define ENVIRONMENTHPP

#include<iostream>
#include<map>

#include"fileReader.hpp"
#include"variable.hpp"

class environment{
private:
	std::map<std::string,var*>eMap;
	environment* father;
public:
	environment();
	environment(environment* father);
	~environment();
	void setFather(environment* father);
	void define(std::string& name,var* value);
	var* find(std::string& name);
	environment* getFather();
};

#endif