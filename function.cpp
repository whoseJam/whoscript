#ifndef FUNCTIONCPP
#define FUNCTIONCPP

#include"function.hpp"
#include"interpreter.hpp"

function::function(int line){
    this->line=line;
}

function::function(std::vector<std::string>arguments,std::string name,statement* body,int line){
    this->arguments=arguments;
    this->name=name;
    this->body=body;
    this->line=line;
}

void function::readFunction(parser* p,interpreter* i){
	int line=i->getCurrentLine();
	if(i->getDepth()!=0){
		i->report(line,"function must be declared in global scope");
		return;
	}
	i->next();
	if(i->getCurrentType()!=IDENTIFIER){
		i->report(line,"function need a name");
		return;
	}
    std::string fName=i->getCurrentLexeme();
	i->next();
	if(i->meetLeftParen()==false){
		i->report(line,"expect '(' after function");
		return;
	}
	i->next();
	std::vector<std::string> arguments;
	while(true){
		if(i->meetRightParen())break;
		if(i->getCurrentType()!=IDENTIFIER){
			i->report(line,"error in function statement");
			return;
		}
		arguments.push_back(i->getCurrentLexeme());
		i->next();
		if(i->meetComma()==false&&i->meetRightParen()==false){
			i->report(line,"error in function statement");
			return;
		}
		if(i->meetComma())
			i->next();
	}
	i->next();
	statement* body=i->getStatement(p);
	i->define(
		fName,
		new function(arguments,fName,body,line)
	);
}

void function::output(){
	std::cout<<"fun "<<name<<"(";bool bef=false;
	for(int i=0;i<arguments.size();i++){
		if(bef)std::cout<<",";
		std::cout<<arguments[i];
	}std::cout<<")";
}

#endif