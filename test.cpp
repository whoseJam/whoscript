#include<typeinfo>
#include<iostream>
#include"compractice.hpp"

lex tokenReader;
parser treeBuilder;
interpreter executor;

int main(int argc,char** args){
	if(argc==1){
		std::cout<<"Usage: test <filepath>\n";
		return 0;
	}
	FILE* f=fopen(args[1],"r");
	if(f==nullptr){
		std::cout<<"file not found!\n";
		return 0;
	}
	tokenReader.scan(f);
	if(tokenReader.error()){
		std::cout<<"compile failed!\n";
		return 0;
	}
	treeBuilder.setTokens(tokenReader.getList());
	executor.setTokens(tokenReader.getList());
	executor.initStatement(&treeBuilder);
	if(treeBuilder.error()||executor.error()){
		std::cout<<"compile failed!\n";
		return 0;
	}
	executor.setGlobalEnvironment();
	std::cout<<"compile successfully\n";
	std::cout<<"start to run:\n\n";
	executor.run();
	return 0;
}