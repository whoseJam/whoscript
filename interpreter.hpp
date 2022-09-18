#ifndef INTERPRETERHPP
#define INTERPRETERHPP

#include<vector>

#include"parser.hpp"
#include"function.hpp"
#include"statement.hpp"
#include"expression.hpp"
#include"environment.hpp"
#include"funEnvironment.hpp"

class interpreter{
private:
	int Depth;
	bool haveError;
	int current,currentStatementLine;
	std::vector<token>* tokens;
	std::vector<statement*> statements;
	environment* innerEnvironment;
	funEnvironment functionEnvironment;
public:
	std::string lstInfo;
	interpreter()=default;
	interpreter(decltype(tokens) tokenList);
	void report(int line,const char* msg);
	void report(int line,std::string& msg);
	void setTokens(decltype(tokens) tokenList);
	void addStatement(statement* s);
	void setGlobalEnvironment();
	statement* getStatement(parser* p);
	void initStatement(parser* p);
	bool atTail();
	void next();
	void prev();
	bool error();
	bool meetSemicolon();
	bool meetEqual();
	bool meetLeftParen();
	bool meetRightParen();
	bool meetLeftBrace();
	bool meetRightBrace();
	bool meetComma();
	int getCurrent();
	int getCurrentLine();
	int getCurrentStatementLine();
	void setCurrentStatementLine(int line);
	std::string getCertainLexeme(int target);
	std::string getCurrentLexeme();
	tokenType getCurrentType();
	void define(std::string& name,var* value);
	void define(std::string& name,function* fun);
	void enterEnvironment();
	void quitEnvironment();
	void addDepth(int del);
	int getDepth();
	var* find(std::string& name);
	function* findFun(std::string& name);
	void run();
};

#endif