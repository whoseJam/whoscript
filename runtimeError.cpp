#ifndef RUNTIMEERRORCPP
#define RUNTIMEERRORCPP

#include"runtimeError.hpp"

void runtimeError::report(int line,const char* msg){
	std::cout<<"[line "<<line<<"] Error: "<<msg<<"\n";
	exit(201802);
}

void runtimeError::report(int line,std::string& msg){
	std::cout<<"[line "<<line<<"] Error: "<<msg<<"\n";
	exit(201802);
}

#endif