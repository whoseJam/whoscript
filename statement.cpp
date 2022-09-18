#ifndef STATEMENTCPP
#define STATEMENTCPP

#include<cmath>

#define BREAK_FLAG 0
#define CONTINUE_FLAG 1

#include"statement.hpp"
#include"interpreter.hpp"

statement::statement(int line){
	this->line=line;
}

printStatement::printStatement(std::vector<expression*> exp,int line):statement(line){
	this->exp=exp;
}

statement* printStatement::readStatement(parser* p,interpreter* i){
	std::vector<expression*> exp;
	int line=i->getCurrentLine();
	//current -> print
	while(true){
		i->next();
		int l=i->getCurrent(),top=0;
		while(i->atTail()==false&&i->meetSemicolon()==false){
			if(i->meetLeftParen())top++;
			if(i->meetRightParen())top--;
			if(top==0&&i->meetComma())break;
			i->next();
		}
		if(i->atTail()){
			i->report(line,"missing ;");
			return new statement(line);
		}
		int r=i->getCurrent()-1;
		exp.push_back(p->getExpression(l,r));
		if(i->meetSemicolon())break;
	}
	i->next();
	return new printStatement(exp,line);
}

void printStatement::execute(interpreter* i){
	if(exp.size()==0)std::cout<<"nil";
	else{
		for(int k=0;k<exp.size();k++){
			var* tmp=exp[k]->getValue(i);
			tmp->output();
			if(tmp->instant)DELETE(tmp);
		}
	}
}

declarationStatement::declarationStatement(std::vector<std::string> varName,std::vector<expression*> exp,int line):statement(line){
	this->varName=varName;
	this->exp=exp;
}

statement* declarationStatement::readStatement(parser* p,interpreter* i){
	//current -> var
	std::vector<std::string> varName;
	std::vector<expression*> exp;
	int line=i->getCurrentLine();
	while(true){
		i->next();
		int l=i->getCurrent(),top=0;
		while(i->atTail()==false&&i->meetSemicolon()==false&&i->meetEqual()==false){
			if(i->meetLeftParen())top++;
			if(i->meetRightParen())top--;
			if(top==0&&i->meetComma())break;
			i->next();
		}
		if(i->atTail()){
			i->report(line,"missing ;");
			return new statement(line);
		}
		if(i->meetSemicolon()||i->meetComma()){
			int r=i->getCurrent()-1;
			if(l<r){i->report(line,"Too many variables");return new statement(line);}
			if(l>r){i->report(line,"variable not found");return new statement(line);}
			varName.push_back(i->getCertainLexeme(l));
			exp.push_back(nullptr);
			if(i->meetSemicolon())break;
		}
		if(i->meetEqual()){
			int r=i->getCurrent()-1;
			if(l<r){i->report(line,"too many variables");return new statement(line);}
			if(l>r){i->report(line,"Variable not found");return new statement(line);}
			i->prev();
			std::string tName=i->getCurrentLexeme();
			i->next();i->next();l=i->getCurrent();top=0;
			while(i->atTail()==false&&i->meetSemicolon()==false){
				if(i->meetLeftParen())top++;
				if(i->meetRightParen())top--;
				if(top==0&&i->meetComma())break;
				i->next();
			}
			if(i->atTail()){
				i->report(line,"missing ;");
				return new statement(line);
			}
			r=i->getCurrent()-1;
			varName.push_back(tName);
			exp.push_back(p->getExpression(l,r));
			if(i->meetSemicolon())break;
		}
	}
	i->next();
	return new declarationStatement(varName,exp,line);
}

void declarationStatement::execute(interpreter* i){
	for(int k=0;k<exp.size();k++){
		if(exp[k]==nullptr){
			i->define(varName[k],new nil(false));
		}else{
			var* b=exp[k]->getValue(i);
			if(b->instant)i->define(varName[k],b);
			else{
				var* tmp=nullptr;
				switch(b->type){
					case NUMBER_v:tmp=new number(((number*)b)->value,false);break;
					case STRING_v:tmp=new string(((string*)b)->value,false);break;
					case BOOL:tmp=new boolean(((boolean*)b)->value,false);break;
					case NIL:tmp=new nil(false);break;
					default:
						std::cout<<"what is b's type (declarationStatement)\n";
						break;
				}
				i->define(varName[k],tmp);
			}
		}
	}
}

blockStatement::blockStatement(decltype(statements) s,int line):statement(line){
	this->statements=s;
}

statement* blockStatement::readStatement(parser* p,interpreter* i){
	//current -> {
	i->addDepth(1);
	int line=i->getCurrentLine();std::vector<statement*> tmp;i->next();
	while(i->atTail()==false&&i->meetRightBrace()==false){
		statement* s=(i->getStatement(p));
		if(s!=nullptr)tmp.push_back(s);
	}
	//current -> }
	if(i->atTail()){
		i->report(line,"missing }");
		return new statement(line);
	}
	i->next();
	i->addDepth(-1);
	return new blockStatement(tmp,line);
}

void blockStatement::execute(interpreter* i){
	i->enterEnvironment();
	for(auto it:this->statements){
		i->setCurrentStatementLine(it->line);
		it->execute(i);
	}
	i->quitEnvironment();
}

void blockStatement::executeFromFunction(interpreter* i){
	for(auto it:this->statements){
		i->setCurrentStatementLine(it->line);
		it->execute(i);
	}
}

expressionStatement::expressionStatement(expression* exp,int line):statement(line){
	this->exp=exp;
}

statement* expressionStatement::readStatement(parser* p,interpreter* i){
	int line=i->getCurrentLine(),l=i->getCurrent();
	while(i->atTail()==false&&i->meetSemicolon()==false){
		i->next();
	}
	if(i->atTail()){
		i->report(line,"missing ;");
		return new statement(line);
	}
	int r=i->getCurrent()-1;
	i->next();
	return new expressionStatement(p->getExpression(l,r),line);
}

void expressionStatement::execute(interpreter* i){
	var* tmp=this->exp->getValue(i);
	if(tmp->instant==true)DELETE(tmp);
}

ifStatement::ifStatement(expression* condition,statement* thenBranch,statement* elseBranch,int line):statement(line){
	this->condition=condition;
	this->thenBranch=thenBranch;
	this->elseBranch=elseBranch;
}

statement* ifStatement::readStatement(parser* p,interpreter* i){
	// -> if
	int line=i->getCurrentLine();
	while(i->atTail()==false&&i->meetLeftParen()==false)
		i->next();
	if(i->atTail()){
		i->report(line,"expect '(' after 'if'");
		return new statement(line);
	}
	i->next();int l=i->getCurrent();
	int top=1;
	while(i->atTail()==false){
		if(i->meetLeftParen())top++;
		if(i->meetRightParen())top--;
		if(top==0)break;
		i->next();
	}	//i-> )
	if(i->atTail()){
		i->report(line,"expect ')' after condition");
		return new statement(line);
	}
	int r=i->getCurrent()-1;
	expression* exp=p->getExpression(l,r);i->next();
	statement* thenBranch=i->getStatement(p);
	if(i->getCurrentType()==KEYWORD&&i->getCurrentLexeme()=="else"){
		i->next();
		statement* elseBranch=i->getStatement(p);
		return new ifStatement(exp,thenBranch,elseBranch,line);
	}
	return new ifStatement(exp,thenBranch,nullptr,line);
}

void ifStatement::execute(interpreter* i){
	var* value=this->condition->getValue(i);bool flg=true;
	if(value->type==BOOL){
		flg=((boolean*)value)->value;
	}
	else if(value->type==NUMBER_v){
		flg=fabs(((number*)value)->value)>1e-8;
	}
	else runtimeError::report(this->line,"invalid condition");

	i->enterEnvironment();

	if(flg)this->thenBranch->execute(i);
	else if(this->elseBranch!=nullptr)this->elseBranch->execute(i);
	if(value->instant)DELETE(value);

	i->quitEnvironment();
}

whileStatement::whileStatement(expression* condition,statement* body,int line):statement(line){
	this->condition=condition;
	this->body=body;
}

statement* whileStatement::readStatement(parser* p,interpreter* i){
	// i -> while
	int line=i->getCurrentLine();
	i->next();
	if(i->meetLeftParen()==false){
		i->report(line,"expect '(' after 'while'");
		return new statement(line);
	}
	// i -> (
	i->next();int l=i->getCurrent();
	int top=1;
	while(i->atTail()==false){
		if(i->meetLeftParen())top++;
		if(i->meetRightParen())top--;
		if(top==0)break;
		i->next();
	}
	if(i->atTail()){
		i->report(line,"expect ')' after condition");
		return new statement(line);
	}
	// i -> )
	int r=i->getCurrent()-1;
	i->next();
	expression* condition=nullptr;
	if(l<=r)condition=p->getExpression(l,r);
	return new whileStatement(
		condition,
		i->getStatement(p),
		line
	);
}

void whileStatement::execute(interpreter* i){
	var* checker=(this->condition==nullptr?
		new boolean(true,true):
		this->condition->getValue(i)
	);
	bool breakQuit=false;
	while(checker->isTrue()){
		if(checker->instant)DELETE(checker);
		try{
			body->execute(i);
		}catch(int flag){
			if(flag==BREAK_FLAG){
				breakQuit=true;
				break;
			}
			if(flag==CONTINUE_FLAG){
			}
		}
		checker=(this->condition==nullptr?
			new boolean(true,true):
			this->condition->getValue(i)
		);
	}
	if(breakQuit==false&&checker->instant)DELETE(checker);
}

forStatement::forStatement(statement* initializer,expression* condition,expression*iterator,statement* body,int line):statement(line){
	this->initializer=initializer;
	this->condition=condition;
	this->iterator=iterator;
	this->body=body;
}

statement* forStatement::readStatement(parser* p,interpreter* i){
	// i -> for
	int line=i->getCurrentLine();
	i->next();
	if(i->meetLeftParen()==false){
		i->report(line,"expect '(' after 'for'");
	}
	i->next();
	statement* initializer=i->getStatement(p);
	int l=i->getCurrent();
	while(i->atTail()==false&&i->meetSemicolon()==false)
		i->next();
	if(i->atTail()){
		i->report(line,"expect ';' in for loop");
		return new statement(line);
	}
	// i -> ;
	int r=i->getCurrent()-1;
	expression* condition=nullptr;
	if(l<=r)condition=p->getExpression(l,r);
	i->next();int top=1;l=i->getCurrent();
	while(i->atTail()==false){
		if(i->meetLeftParen())top++;
		if(i->meetRightParen())top--;
		if(top==0)break;
		i->next();
	}
	// i -> )
	r=i->getCurrent()-1;
	expression* iterator=nullptr;
	if(l<=r)iterator=p->getExpression(l,r);
	i->next();
	statement* body=i->getStatement(p);
	return new forStatement(
		initializer,
		condition,
		iterator,
		body,
		line
	);
}

void forStatement::execute(interpreter* i){
	i->enterEnvironment();
	this->initializer->execute(i);
	var* checker=(this->condition==nullptr?
		new boolean(true,true):
		this->condition->getValue(i));
	bool breakQuit=false;
	while(checker->isTrue()){
		if(checker->instant)DELETE(checker);
		try{
			this->body->execute(i);
		}
		catch(int flag){
			if(flag==BREAK_FLAG){
				breakQuit=true;
				break;
			}
			if(flag==CONTINUE_FLAG){
			}
		}
		if(this->iterator!=nullptr){
			var* tmp=this->iterator->getValue(i);
			if(tmp->instant)DELETE(tmp);
		}
		checker=(this->condition==nullptr?
			new boolean(true,true):
			this->condition->getValue(i));
	}
	if(breakQuit==false&&checker->instant)DELETE(checker);
	i->quitEnvironment();
}

emptyStatement::emptyStatement(int line):statement(line){
	std::cout<<"make empty statement\n";
}
statement* emptyStatement::readStatement(parser* p,interpreter* i){
	//i -> ;
	int line=i->getCurrentLine();i->next();
	return new emptyStatement(line);
}
void emptyStatement::execute(interpreter* i){
}

returnStatement::returnStatement(expression* ret,int line):statement(line){
	this->ret=ret;
	this->line=line;
}

statement* returnStatement::readStatement(parser* p,interpreter* i){
	// i -> return
	int line=i->getCurrentLine();
	i->next();
	int l=i->getCurrent();
	while(i->atTail()==false&&i->meetSemicolon()==false)
		i->next();
	if(i->atTail()){
		i->report(line,"missing ;");
		return new statement(line);
	}
	int r=i->getCurrent()-1;
	i->next();
	return new returnStatement(
		p->getExpression(l,r),
		line
	);
}

void returnStatement::execute(interpreter* i){
	var* retvalue=ret->getValue(i);
	throw retvalue;
}

breakStatement::breakStatement(int line):statement(line){
}

statement* breakStatement::readStatement(parser* p,interpreter* i){
	int line=i->getCurrentLine();
	i->next();i->next();
	return new breakStatement(line);
}

void breakStatement::execute(interpreter* i){
	throw BREAK_FLAG;
}

continueStatement::continueStatement(int line):statement(line){
}

statement* continueStatement::readStatement(parser* p,interpreter* i){
	int line=i->getCurrentLine();
	i->next();i->next();
	return new continueStatement(line);
}

void continueStatement::execute(interpreter* i){
	throw CONTINUE_FLAG;
}

#endif