#ifndef STATEMENTHPP
#define STATEMENTHPP

#include"expression.hpp"
#include"parser.hpp"

class interpreter;

class statement{
public:
	int line;
	statement(int line);
	~statement(){};
	virtual void execute(interpreter* i){};
};

class printStatement:public statement{
public:
	std::vector<expression*> exp;
	printStatement(std::vector<expression*>exp,int line);
	static statement* readStatement(parser* p,interpreter* i);
	void execute(interpreter* i);
};

class declarationStatement:public statement{
public:
	std::vector<std::string> varName;
	std::vector<expression*> exp;
	declarationStatement(std::vector<std::string> varName,std::vector<expression*> exp,int line);
	static statement* readStatement(parser* p,interpreter* i);
	void execute(interpreter* i);
};

class blockStatement:public statement{
public:
	std::vector<statement*> statements;
	blockStatement(decltype(statements) s,int line);
	static statement* readStatement(parser* p,interpreter* i);
	void execute(interpreter* i);
	void executeFromFunction(interpreter* i);
};

class expressionStatement:public statement{
public:
	expression* exp;
	expressionStatement(expression* exp,int line);
	static statement* readStatement(parser* p,interpreter* i);
	void execute(interpreter* i);
};

class ifStatement:public statement{
public:
	expression* condition;
	statement* thenBranch;
	statement* elseBranch;
	ifStatement(expression* condition,statement* thenBranch,statement* elseBranch,int line);
	static statement* readStatement(parser* p,interpreter* i);
	void execute(interpreter* i);
};

class whileStatement:public statement{
public:
	expression* condition;
	statement* body;
	whileStatement(expression* codition,statement* body,int line);
	static statement* readStatement(parser* p,interpreter* i);
	void execute(interpreter* i);
};

class forStatement:public statement{
public:
	statement* initializer;
	expression* condition;
	expression* iterator;
	statement* body;
	forStatement(statement* initializer,expression* condition,expression* iterator,statement* body,int line);
	static statement* readStatement(parser* p,interpreter* i);
	void execute(interpreter* i);
};

class emptyStatement:public statement{
public:
	emptyStatement(int line);
	static statement* readStatement(parser* p,interpreter* i);
	void execute(interpreter* i);
};

class returnStatement:public statement{
public:
	expression* ret;
	returnStatement(expression* ret,int line);
	static statement* readStatement(parser* p,interpreter* i);
	void execute(interpreter* i);
};

class breakStatement:public statement{
public:
	breakStatement(int line);
	static statement* readStatement(parser* p,interpreter* i);
	void execute(interpreter* i);
};

class continueStatement:public statement{
public:
	continueStatement(int line);
	static statement* readStatement(parser* p,interpreter* i);
	void execute(interpreter* i);
};

#endif