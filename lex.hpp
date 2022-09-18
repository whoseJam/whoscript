#ifndef LEXHPP
#define LEXHPP

#include<iostream>
#include<vector>
#include<map>

#include"fileReader.hpp"
#include"token.hpp"

class lex{
private:
	static bool haveError;
	static std::map<std::string,std::string> M;
	std::vector<token> tokenList;
	static std::map<std::string,std::string> getMap();
	void addToken(token newToken);
	void addString(std::string& code,int& current,int& line);
	void addNumber(std::string& code,int& current,int& line);
	void addIdentifier(std::string& code,int& current,int& line);
	void getToken(std::string& code,int& current,int& line);
	void report(int line,const char* msg);
	void report(int line,std::string& msg);
public:
	void scan(FILE *file);
	void output();
	std::vector<token>* getList();
	bool error();
};

#endif