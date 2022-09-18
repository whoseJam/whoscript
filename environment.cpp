#ifndef ENVIRONMENTCPP
#define ENVIRONMENTCPP

#include"environment.hpp"

environment::environment(){
	father=nullptr;
}

environment::environment(environment* father){
	this->father=father;
}

environment::~environment(){
	for(auto it:eMap){
		var* tmp=it.second;
		DELETE(tmp);
	}
}

void environment::define(std::string& name,var* value){
	// std::cout<<"define "<<name<<" as ";value->output();std::cout<<"\n";
	value->instant=false;
	eMap[name]=value;
	value->it=eMap.find(name);
}

var* environment::find(std::string& name){
	auto it=eMap.find(name);
	if(it!=eMap.end()){
		return it->second;
	}
	if(father!=nullptr)return father->find(name);
	return nullptr;
}

void environment::setFather(environment* father){
	this->father=father;
}

environment* environment::getFather(){
	return this->father;
}

#endif