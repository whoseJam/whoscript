#ifndef EXPRESSIONCPP
#define EXPRESSIONCPP

#include"expression.hpp"
#include"interpreter.hpp"

std::string literal::toString(){
	return value;
}

void literal::output(){
	std::cout<<value;
}

literal::literal(varType type,std::string value){
	this->type=type;
	this->value=value;
	this->etype=LITERAL;
}

var* literal::getValue(interpreter* i){
	if(this->type==IDENTIFIER_v){
		var* tmp=i->find(this->value);
		// std::cout<<"value="<<this->value<<"\n";
		if(tmp==nullptr){
			function*  tmp=i->findFun(this->value);
			if(tmp==nullptr){
				runtimeError::report(i->getCurrentStatementLine(),"variable not found");
				return new nil(true);
			}
			return new funVar(tmp,true);
		}
		return tmp;
	}
	if(this->type==STRING_v)return new string(this->value,true);
	if(this->type==NUMBER_v)return new number(atof(this->value.c_str()),true);
	if(this->type==BOOL)return new boolean(this->value=="true"?true:false,true);
	return new nil(true);
}

std::string unary::toString(){
	return "("+oper.getLexeme()+" "+right->toString()+")";
}

void unary::output(){
	std::cout<<"("<<oper.getLexeme()<<" ";
	right->output();std::cout<<")";
}

unary::unary(token oper,expression* right){
	this->oper=oper;
	this->right=right;
	this->etype=UNARY;
}

var* unary::getValue(interpreter* i){
	var* rvar=this->right->getValue(i);
	var* tvar=nullptr;
	if(this->oper.getType()==MINUS)
		tvar=Operator::getOper()->MINUS(rvar,oper.getLine());
	if(this->oper.getType()==BANG)
		tvar=Operator::getOper()->BANG(rvar,oper.getLine());
	if(rvar->instant)DELETE(rvar);
	if(tvar==nullptr)return new nil(true);
	return tvar;
}

std::string binary::toString(){
	return "("+oper.getLexeme()+" "+left->toString()+" "+right->toString()+")";
}

void binary::output(){
	std::cout<<"("<<oper.getLexeme()<<" ";
	left->output();std::cout<<" ";
	right->output();std::cout<<")";
}

binary::binary(expression* left,token oper,expression* right){
	this->left=left;
	this->right=right;
	this->oper=oper;
	this->etype=BINARY;
}

var* binary::getValue(interpreter* i){
	var* lvar=this->left->getValue(i);
	var* rvar=this->right->getValue(i);
	var* tvar=nullptr;
	switch (this->oper.getType()){
		case MINUS:tvar=Operator::getOper()->MINUS(lvar,rvar,oper.getLine());break;
		case PLUS:tvar=Operator::getOper()->PLUS(lvar,rvar,oper.getLine());break;
		case STAR:tvar=Operator::getOper()->STAR(lvar,rvar,oper.getLine());break;
		case BANG_EQUAL:tvar=Operator::getOper()->BANG_EQUAL(lvar,rvar,oper.getLine());break;
		case EQUAL_EQUAL:tvar=Operator::getOper()->EQUAL_EQUAL(lvar,rvar,oper.getLine());break;
		case GREATER_EQUAL:tvar=Operator::getOper()->GREATER_EQUAL(lvar,rvar,oper.getLine());break;
		case GREATER:tvar=Operator::getOper()->GREATER(lvar,rvar,oper.getLine());break;
		case LESS_EQUAL:tvar=Operator::getOper()->LESS_EQUAL(lvar,rvar,oper.getLine());break;
		case LESS:tvar=Operator::getOper()->LESS(lvar,rvar,oper.getLine());break;
		case SLASH:tvar=Operator::getOper()->SLASH(lvar,rvar,oper.getLine());break;
		case EQUAL:tvar=Operator::getOper()->EQUAL(lvar,rvar,oper.getLine());break;
	}
	// std::cout<<this->oper.getLexeme()<<" ";
	// lvar->output();std::cout<<" ";
	// rvar->output();std::cout<<" ";
	// tvar->output();std::cout<<" expression.cpp\n\n";
	if(lvar->instant)DELETE(lvar);
	if(rvar->instant)DELETE(rvar);
	if(tvar==nullptr)return new nil(true);
	return tvar;
}

std::string binaryCircuit::toString(){
	return "("+oper.getLexeme()+" "+left->toString()+" "+right->toString()+")";
}
void binaryCircuit::output(){
	std::cout<<"("<<oper.getLexeme()<<" ";
	left->output();std::cout<<" ";
	right->output();std::cout<<")";
}
binaryCircuit::binaryCircuit(expression* left,token oper,expression* right){
	this->left=left;
	this->right=right;
	this->oper=oper;
}
var* binaryCircuit::getValue(interpreter* i){
	var* lvar=this->left->getValue(i);
	if(this->oper.getLexeme()=="and"){
		if(lvar->isTrue()==false){
			if(lvar->instant)DELETE(lvar);
			return new boolean(false,true);
		}
		var* rvar=this->right->getValue(i);
		bool flg=rvar->isTrue();
		if(lvar->instant)DELETE(lvar);
		if(rvar->instant)DELETE(rvar);
		return new boolean(flg,true);
	}
	if(this->oper.getLexeme()=="or"){
		if(lvar->isTrue()==true){
			if(lvar->instant)DELETE(lvar);
			return new boolean(true,true);
		}
		var* rvar=this->right->getValue(i);
		bool flg=rvar->isTrue();
		if(lvar->instant)DELETE(lvar);
		if(rvar->instant)DELETE(rvar);
		return new boolean(flg,true);
	}
}

std::string group::toString(){
	return "(group "+gexp->toString()+")";
}

void group::output(){
	std::cout<<"(group ";gexp->output();std::cout<<")";
}

group::group(expression* gexp){
	this->gexp=gexp;
	this->etype=GROUP;
}

var* group::getValue(interpreter* i){
	return this->gexp->getValue(i);
}

std::string funcGroup::toString(){
	std::string ans="(fun ";bool bef=false;
	for(int i=0;i<arguments.size();i++){
		if(bef)ans=ans+" , "+arguments[i]->toString();
		else ans=ans+arguments[i]->toString();
		bef=true;
	}
	ans=ans+")";
	return ans;
}

void funcGroup::output(){
	std::cout<<"(fun ";bool bef=false;
	for(int i=0;i<arguments.size();i++){
		if(bef)std::cout<<" , ";
		arguments[i]->output();
		bef=true;
	}
	std::cout<<")";
}

funcGroup::funcGroup(std::vector<expression*> arguments,expression* fun,int line){
	this->arguments=arguments;
	this->line=line;
	this->fun=fun;
}

var* funcGroup::getValue(interpreter* i){
	var* myFun=fun->getValue(i);
	if(myFun->type!=FUN_v){
		runtimeError::report(line,"function error:FUN_v missing");
		return new nil(true);
	}
	function* f=((funVar*)myFun)->fun;
	if(f->arguments.size()!=arguments.size()){
		runtimeError::report(line,"function error:length of arguments not match");
		return new nil(true);
	}
	i->enterEnvironment();
	for(int k=0;k<arguments.size();k++){
		var* tmp=arguments[k]->getValue(i);
		if(tmp->instant)i->define(f->arguments[k],tmp);
		else i->define(f->arguments[k],tmp->copy());
	}
	var* retvalue=nullptr;
	try{
		((blockStatement*)f->body)->executeFromFunction(i);
		retvalue=new nil(true);
	}catch(var* ret){
		retvalue=ret;
		// std::cout<<"name="<<f->arguments[0]<<"\n";
		// std::cout<<"i catch ret:\n";
		// ret->output();std::cout<<"\n\n";
	}
	i->quitEnvironment();
	return retvalue;
}

#endif