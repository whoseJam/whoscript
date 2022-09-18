#ifndef INTERPRETER
#define INTERPRETER

#include"interpreter.hpp"

void interpreter::report(int line,std::string& msg){
	haveError=true;
	std::cout<<"[line "<<line<<"] Error: "<<msg<<"\n";
	std::cout<<"compile error\n";
	exit(201802);
}

void interpreter::report(int line,const char* msg){
	haveError=true;
	std::cout<<"[line "<<line<<"] Error: "<<msg<<"\n";
	std::cout<<"compile error\n";
	exit(201802);
}

interpreter::interpreter(decltype(tokens) tokenList){
	this->tokens=tokenList;
}

void interpreter::setTokens(decltype(tokens) tokenList){
	this->tokens=tokenList;
}

void interpreter::addStatement(statement* s){
	statements.push_back(s);
}

void interpreter::initStatement(parser* p){
	current=0;
	while(!atTail()){
		statement* s=getStatement(p);
		if(s!=nullptr)statements.push_back(s);
	}
}

void interpreter::setGlobalEnvironment(){
	innerEnvironment=new environment;
}

statement* interpreter::getStatement(parser* p){
#ifdef DEBUG
	std::cout<<"getStatement:"<<(*tokens)[current].getLexeme()<<" interpreter.cpp ";
#endif
	if((*tokens)[current].getType()==KEYWORD){
		if((*tokens)[current].getLexeme()=="print"){
		#ifdef DEBUG
			std::cout<<"KEYWORD-print\n";
		#endif
			return printStatement::readStatement(p,this);
		}
		if((*tokens)[current].getLexeme()=="var"){
		#ifdef DEBUG
			std::cout<<"KEYWORD-var\n";
		#endif
			return declarationStatement::readStatement(p,this);
		}
		if((*tokens)[current].getLexeme()=="if"){
		#ifdef DEBUG
			std::cout<<"KEYWORD-if\n";
		#endif
			return ifStatement::readStatement(p,this);
		}
		if((*tokens)[current].getLexeme()=="while"){
		#ifdef DEBUG
			std::cout<<"KEYWORD-while\n";
		#endif
			return whileStatement::readStatement(p,this);
		}
		if((*tokens)[current].getLexeme()=="for"){
		#ifdef DEBUG
			std::cout<<"KEYWORD-for\n";
		#endif
			return forStatement::readStatement(p,this);
		}
		if((*tokens)[current].getLexeme()=="fun"){
		#ifdef DEBUG
			std::cout<<"KEYWORD-fun\n";
		#endif
			function::readFunction(p,this);
			return nullptr;
		}
		if((*tokens)[current].getLexeme()=="return"){
		#ifdef DEBUG
			std::cout<<"KEYWORD-return\n";
		#endif
			return returnStatement::readStatement(p,this);
		}
		if((*tokens)[current].getLexeme()=="break"){
		#ifdef DEBUG
			std::cout<<"KEYWORD-break\n";
		#endif
			return breakStatement::readStatement(p,this);
		}
		if((*tokens)[current].getLexeme()=="continue"){
		#ifdef DEBUG
			std::cout<<"KEYWORD-continue\n";
		#endif
			return continueStatement::readStatement(p,this);
		}
	}
	else if((*tokens)[current].getType()==LEFT_BRACE){
	#ifdef DEBUG
		std::cout<<"block\n";
	#endif
		return blockStatement::readStatement(p,this);
	}
	else if((*tokens)[current].getType()==SEMICOLON){
	#ifdef DEBUG
		std::cout<<"empty\n";
	#endif
		return emptyStatement::readStatement(p,this);
	}
	else{
	#ifdef DEBUG
		std::cout<<"expression\n";
	#endif
		return expressionStatement::readStatement(p,this);
	}
}

bool interpreter::atTail(){
	return current>=tokens->size();
}

void interpreter::next(){
	current++;
}

void interpreter::prev(){
	current--;
}

bool interpreter::error(){
	return haveError;
}

bool interpreter::meetSemicolon(){
	return (*tokens)[current].getType()==SEMICOLON;
}

bool interpreter::meetEqual(){
	return (*tokens)[current].getType()==EQUAL;
}

bool interpreter::meetLeftParen(){
	return (*tokens)[current].getType()==LEFT_PAREN;
}

bool interpreter::meetRightParen(){
	return (*tokens)[current].getType()==RIGHT_PAREN;
}

bool interpreter::meetLeftBrace(){
	return (*tokens)[current].getType()==LEFT_BRACE;
}

bool interpreter::meetRightBrace(){
	return (*tokens)[current].getType()==RIGHT_BRACE;
}

bool interpreter::meetComma(){
	return (*tokens)[current].getType()==COMMA;
}

int interpreter::getCurrent(){
	return current;
}

int interpreter::getCurrentLine(){
	return (*tokens)[current].getLine();
}

std::string interpreter::getCurrentLexeme(){
	return (*tokens)[current].getLexeme();
}
std::string interpreter::getCertainLexeme(int target){
	return (*tokens)[target].getLexeme();
}

tokenType interpreter::getCurrentType(){
	return (*tokens)[current].getType();
}

void interpreter::define(std::string& name,var* value){
	// std::cout<<"define "<<name<<" ";value->output();std::cout<<"\n";
	innerEnvironment->define(name,value);
}

void interpreter::define(std::string& name,function* fun){
	functionEnvironment.define(name,fun,this);
}

void interpreter::enterEnvironment(){
	environment* tmp=innerEnvironment;
	innerEnvironment=new environment(tmp);
}

void interpreter::quitEnvironment(){
	environment* tmp=innerEnvironment;
	innerEnvironment=innerEnvironment->getFather();
	DELETE(tmp);
}

var* interpreter::find(std::string& name){
	environment* tmp=innerEnvironment;
	var* ans=innerEnvironment->find(name);
	return ans;
}

function* interpreter::findFun(std::string& name){
	return functionEnvironment.find(name);
}

int interpreter::getCurrentStatementLine(){
	return currentStatementLine;
}

void interpreter::setCurrentStatementLine(int line){
	currentStatementLine=line;
}

void interpreter::run(){
	for(auto it:statements){
		currentStatementLine=it->line;
		it->execute(this);
	}
}

void interpreter::addDepth(int del){
	Depth+=del;
}

int interpreter::getDepth(){
	return Depth;
}

#endif