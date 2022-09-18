#ifndef PARSERHPP
#define PARSERHPP

#include"lex.hpp"
#include"expression.hpp"

#include<vector>

class parser{
private:
	static bool haveError;
	std::vector<token>* tokens;
	void report(int line,std::string msg);
public:
	parser()=default;
	parser(decltype(tokens) tokenList);
	void setTokens(decltype(tokens) tokenList);
	expression* getExpression(int l,int r);
	bool error();
};

#endif