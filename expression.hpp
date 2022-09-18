#ifndef EXPRESSIONHPP
#define EXPRESSIONHPP

#include<iostream>
#include<cstring>
#include<vector>

#include"variable.hpp"
#include"token.hpp"
#include"fileReader.hpp"

class interpreter;

enum expressionType{
	LITERAL,
	UNARY,
	BINARY,
	GROUP,
	FUNCGROUP
};

class expression{
public:
	expressionType etype;
	expression(){};
	virtual ~expression(){};
	virtual void interpret(){};
	virtual std::string toString(){};
	virtual void output(){};
	virtual var* getValue(interpreter* i){};
};

class literal:public expression{
public:
	varType type;
	std::string value;
	std::string toString();
	void output();
	literal(varType type,std::string value);
	var* getValue(interpreter* i);
};

class unary:public expression{
public:
	expression* right;
	token oper;
	std::string toString();
	void output();
	unary(token oper,expression* right);
	var* getValue(interpreter* i);
};

class binary:public expression{
public:
	expression* left;
	expression* right;
	token oper;
	std::string toString();
	void output();
	binary(expression* left,token oper,expression* right);
	var* getValue(interpreter* i);
};

class binaryCircuit:public expression{
public:
	expression* left;
	expression* right;
	token oper;
	std::string toString();
	void output();
	binaryCircuit(expression* left,token oper,expression* right);
	var* getValue(interpreter* i);
};

class group:public expression{
public:
	expression* gexp;
	std::string toString();
	void output();
	group(expression* gexp);
	var* getValue(interpreter* i);
};

class funcGroup:public expression{
public:
	int line;
	std::vector<expression*> arguments;
	expression* fun;
	std::string toString();
	void output();
	funcGroup(std::vector<expression*> arguments,expression* fun,int line);
	var* getValue(interpreter* i);
};

#endif