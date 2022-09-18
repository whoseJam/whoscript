#ifndef VARIABLECPP
#define VARIABLECPP

#include"variable.hpp"
#include"function.hpp"

#include<cmath>

bool Operator::flag=false;
Operator* Operator::instance=nullptr;

number::number(bool instant){
	this->instant=instant;
	this->type=NUMBER_v;
}
number::number(double value,bool instant){
	this->instant=instant;
	this->type=NUMBER_v;
	this->value=value;
}
bool number::isValue(){
	return true;
}
void number::output(){
	std::cout<<value;
#ifdef DEBUG
	if(instant)std::cout<<"(i)";
#endif
}
bool number::isTrue(){
	return fabs(value)>1e-8;
}
var* number::copy(){
	return new number(value,true);
}

string::string(bool instant){
	this->instant=instant;
	this->type=STRING_v;
}
string::string(std::string value,bool instant){
	this->instant=instant;
	this->type=STRING_v;
	this->value=value;
}
bool string::isValue(){
	return false;
}
void string::output(){
	std::cout<<value;
}
bool string::isTrue(){
	return false;
}
var* string::copy(){
	return new string(value,true);
}

boolean::boolean(bool instant){
	this->instant=instant;
	this->type=BOOL;
}
boolean::boolean(bool value,bool instant){
	this->instant=instant;
	this->type=BOOL;
	this->value=value;
}
bool boolean::isValue(){
	return true;
}
void boolean::output(){
	if(value)std::cout<<"true";
	else std::cout<<"false";
#ifdef DEBUG
	if(instant)std::cout<<"(i)";
#endif
}
bool boolean::isTrue(){
	return value;
}
var* boolean::copy(){
	return new boolean(value,true);
}

nil::nil(bool instant){
	this->instant=instant;
	this->type=NIL;
}
bool nil::isValue(){
	return false;
}
void nil::output(){
	std::cout<<"nil";
#ifdef DEBUG
	if(instant)std::cout<<"(i)";
#endif
}
bool nil::isTrue(){
	return false;
}
var* nil::copy(){
	return new nil(true);
}

funVar::funVar(bool instant){
	this->instant=instant;
	this->type=FUN_v;
}
funVar::funVar(function* fun,bool instant){
	this->fun=fun;
	this->instant=instant;
	this->type=FUN_v;
}
bool funVar::isTrue(){
	return false;
}
bool funVar::isValue(){
	return false;
}
void funVar::output(){
	fun->output();
#ifdef DEBUG
	if(instant)std::cout<<"(i)";
#endif
}
var* funVar::copy(){
	return new funVar(fun,true);
}

Operator* Operator::getOper(){
	if(flag==false){
		flag=true;
		instance=new Operator();
	}
	return instance;
}

double getVarValue(var* a){
	if(a->type==BOOL)return ((boolean*)a)->value?1:0;
	if(a->type==NUMBER_v)return ((number*)a)->value;
	std::cout<<"error type\n";
	return 0;
}

var* Operator::PLUS(var* a,var* b,int line){
	if(a->type==NIL||b->type==NIL){
		runtimeError::report(line,"+ meet a variable that cannot plus");
	}
	bool haveString=(a->type==STRING_v)|(b->type==STRING_v);
	if(haveString){
		if(a->type!=STRING_v||b->type!=STRING_v){
			runtimeError::report(line,"+ meet a variable that cannot be plus");
		}
		std::string s=((string*)a)->value+((string*)b)->value;
		return new string(s,true);
	}
	double va=getVarValue(a),vb=getVarValue(b);
	return new number(va+vb,true);
}

var* Operator::MINUS(var* a,var* b,int line){
	if(a->type==STRING_v||a->type==NIL||b->type==STRING_v||b->type==NIL){
		runtimeError::report(line,"- meet two variables that cannot minus");
	}
	double va=getVarValue(a),vb=getVarValue(b);
	return new number(va-vb,true);
}

var* Operator::MINUS(var* a,int line){
	if(a->type==STRING_v||a->type==NIL){
		runtimeError::report(line,"- meet a variable that cannot be negative");
	}
	double v=getVarValue(a);
	return new number(-v,true);
}

var* Operator::STAR(var* a,var* b,int line){
	if(a->type==STRING_v||a->type==NIL||b->type==STRING_v||b->type==NIL){
		runtimeError::report(line,"* meet two variables that cannot be multiplied");
	}
	double va=getVarValue(a),vb=getVarValue(b);
	return new number(va*vb,true);
}

var* Operator::SLASH(var* a,var* b,int line){
	if(a->type==STRING_v||a->type==NIL||b->type==STRING_v||b->type==NIL){
		runtimeError::report(line,"/ meet two variables that cannot be divided");
	}
	double va=getVarValue(a),vb=getVarValue(b);
	return new number(va/vb,true);
}

var* Operator::BANG(var* a,int line){
	if(a->type==STRING_v||a->type==NIL){
		runtimeError::report(line,"! meet a variable that cannot be reversed");
	}
	double v=getVarValue(a);
	if(fabs(v)>1e-8)return new boolean(false,true);
	return new boolean(true,true);
}

var* Operator::EQUAL_EQUAL(var* a,var* b,int line){
	if(a->type==NIL&&b->type==NIL)return new boolean(true,true);
	if(a->type==STRING_v&&b->type==STRING_v)return new boolean(true,true);
	if((a->type==BOOL||a->type==NUMBER_v)&&
	(b->type==BOOL||b->type==NUMBER_v)){
		if(fabs(getVarValue(a)-getVarValue(b))<1e-8)return new boolean(true,true);
	}
	return new boolean(false,true);
}

var* Operator::BANG_EQUAL(var* a,var* b,int line){
	var* value=EQUAL_EQUAL(a,b,line);
	boolean* tvalue=(boolean*)&value;
	if(tvalue->value==true)return new boolean(false,true);
	return new boolean(true,true);
}

var* Operator::GREATER(var* a,var* b,int line){
	if(a->isValue()&&b->isValue()){
		if(getVarValue(a)>getVarValue(b))return new boolean(true,true);
		return new boolean(false,true);
	}
	else if(a->type==STRING_v&&b->type==STRING_v){
		if(((string*)a)->value>((string*)b)->value)return new boolean(true,true);
		return new boolean(false,true);
	}
	else if(a->type==NIL&&b->type==NIL){
		return new boolean(false,true);
	}
	runtimeError::report(line,"> meet two variables that cannot be compared");
}

var* Operator::GREATER_EQUAL(var* a,var* b,int line){
	if(a->isValue()&&b->isValue()){
		if(getVarValue(a)>=getVarValue(b))return new boolean(true,true);
		return new boolean(false,true);
	}
	else if(a->type==STRING_v&&b->type==STRING_v){
		if(((string*)a)->value>=((string*)b)->value)return new boolean(true,true);
		return new boolean(false,true);
	}
	else if(a->type==NIL&&b->type==NIL){
		return new boolean(true,true);
	}
	runtimeError::report(line,">= meet two variables that cannot be compared");
}

var* Operator::LESS(var* a,var* b,int line){
	if(a->isValue()&&b->isValue()){
		if(getVarValue(a)<getVarValue(b))return new boolean(true,true);
		return new boolean(false,true);
	}
	else if(a->type==STRING_v&&b->type==STRING_v){
		if(((string*)a)->value<((string*)b)->value)return new boolean(true,true);
		return new boolean(false,true);
	}
	else if(a->type==NIL&&b->type==NIL){
		return new boolean(false,true);
	}
	runtimeError::report(line,"< meet two variables that cannot be compared");
}

var* Operator::LESS_EQUAL(var* a,var* b,int line){
	if(a->isValue()&&b->isValue()){
		if(getVarValue(a)<=getVarValue(b))return new boolean(true,true);
		return new boolean(false,true);
	}
	else if(a->type==STRING_v&&b->type==STRING_v){
		if(((string*)a)->value<=((string*)b)->value)return new boolean(true,true);
		return new boolean(false,true);
	}
	else if(a->type==NIL&&b->type==NIL){
		return new boolean(true,true);
	}
	runtimeError::report(line,"<= meet two variables that cannot be compared");
}

var* Operator::EQUAL(var* a,var* b,int line){
	if(a->instant==true)
		runtimeError::report(line,"the left of = is an instant variable");
	if(b->instant==true){
		(*(a->it)).second=b;
		b->instant=false;
		a->instant=true;
		b->it=a->it;
	}
	else{
		var* tmp=nullptr;
		switch(b->type){
			case NUMBER_v:tmp=new number(((number*)b)->value,false);break;
			case STRING_v:tmp=new string(((string*)b)->value,false);break;
			case BOOL:tmp=new boolean(((boolean*)b)->value,false);break;
			case NIL:tmp=new nil(false);break;
			default:
				std::cout<<"what is b's type (EQUAL)\n";
				break;
		}
		tmp->it=a->it;
		var* del=(*(a->it)).second;
		(*(a->it)).second=tmp;
		del->instant=true;
	}
	return b;
}

#endif