#ifndef VARIABLE
#define VARIABLE

#include"runtimeError.hpp"
#include"fileReader.hpp"

#include<iostream>
#include<cstring>
#include<map>

class function;

enum varType{
	NUMBER_v,
	STRING_v,
	IDENTIFIER_v,
	FUN_v,
	BOOL,
	NIL
};

class var{
public:
	bool instant;
	std::map<std::string,var*>::iterator it;
	varType type;
	virtual bool isTrue(){};
	virtual bool isValue(){};
	virtual void output(){};
	virtual var* copy(){};
};

class number:public var{
public:
	double value;
	number(bool instant);
	number(double value,bool instant);
	bool isTrue();
	bool isValue();
	void output();
	var* copy();
};

class string:public var{
public:
	std::string value;
	string(bool instant);
	string(std::string value,bool instant);
	bool isTrue();
	bool isValue();
	void output();
	var* copy();
};

class boolean:public var{
public:
	bool value;
	boolean(bool instant);
	boolean(bool value,bool instant);
	bool isTrue();
	bool isValue();
	void output();
	var* copy();
};

class nil:public var{
public:
	nil(bool instant);
	bool isTrue();
	bool isValue();
	void output();
	var* copy();
};

class funVar:public var{
public:
	function* fun;
	funVar(bool instant);
	funVar(function* fun,bool instant);
	bool isTrue();
	bool isValue();
	void output();
	var* copy();
};

class Operator{
private:
	static bool flag;
	static Operator* instance;
	Operator(){};
public:
	static Operator* getOper();
	var* PLUS(var* a,var* b,int line);
	var* MINUS(var* a,var* b,int line);
	var* MINUS(var* a,int line);
	var* STAR(var* a,var* b,int line);
	var* SLASH(var* a,var* b,int line);
	var* BANG(var* a,int line);
	var* EQUAL_EQUAL(var* a,var* b,int line);
	var* BANG_EQUAL(var* a,var* b,int line);
	var* GREATER(var* a,var* b,int line);
	var* GREATER_EQUAL(var* a,var* b,int line);
	var* LESS(var* a,var* b,int line);
	var* LESS_EQUAL(var* a,var* b,int line);
	var* EQUAL(var* a,var* b,int line);
};

#endif