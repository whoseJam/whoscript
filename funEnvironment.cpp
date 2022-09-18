#ifndef FUNENVIRONMENTCPP
#define FUNENVIRONMENTCPP

#include"funEnvironment.hpp"
#include"interpreter.hpp"

void funEnvironment::define(std::string& name,function* f,interpreter* i){
	auto it=fMap.find(name);
	if(it!=fMap.end()){
		i->report(f->line,"redefined");
	}
	else{
		fMap[name]=f;
	}
}

function* funEnvironment::find(std::string& name){
	auto it=fMap.find(name);
	if(it==fMap.end()){
		return nullptr;
		// std::string msg="cannot find a function named ["+name+"]";
		// runtimeError::report(0,msg);
	}
	else return it->second;
}

#endif