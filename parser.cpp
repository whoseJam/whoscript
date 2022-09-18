#ifndef PARSERCPP
#define PARSERCPP

#include"parser.hpp"

bool parser::haveError=false;

void parser::report(int line,std::string msg){
	haveError=true;
	std::cout<<"[line "<<line<<"] Error: "<<msg<<"\n";
}

parser::parser(decltype(tokens) tokenList){
	this->tokens=tokenList;
}

void parser::setTokens(decltype(tokens) tokenList){
	this->tokens=tokenList;
}

#define NOT_FOUND -2137

expression* parser::getExpression(int l,int r){
	if(l>r){
		report((*tokens)[r].getLine(),"arguments missing");
		return new literal(NIL,"nil");
	}
	if(l==r){
		if((*tokens)[l].getType()==NUMBER)
			return new literal(NUMBER_v,(*tokens)[l].getLiteral());
		if((*tokens)[l].getType()==STRING)
			return new literal(STRING_v,(*tokens)[l].getLiteral());
		if((*tokens)[l].getType()==IDENTIFIER)
			return new literal(IDENTIFIER_v,(*tokens)[l].getLexeme());
		if((*tokens)[l].getType()==KEYWORD){
			if((*tokens)[l].getLexeme()=="true")return new literal(BOOL,"true");
			if((*tokens)[l].getLexeme()=="false")return new literal(BOOL,"false");
			if((*tokens)[l].getLexeme()=="nil")return new literal(NIL,"");
		}
		report((*tokens)[l].getLine(),"WTF is this token???");
		return new literal(NIL,"nil");
	}
	
	int top=0;
	for(int i=l;i<=r;i++){
		if((*tokens)[i].getType()==LEFT_PAREN)top++;
		if((*tokens)[i].getType()==RIGHT_PAREN)top--;
	}if(top!=0){
		report((*tokens)[l].getLine(),"paren not match");
	}

	int target=NOT_FOUND;
	for(int i=l;i<=r;i++){
		token& t=(*tokens)[i];
		if((*tokens)[i].getType()==LEFT_PAREN)top++;
		if((*tokens)[i].getType()==RIGHT_PAREN)top--;
		if(top==0&&(t.getType()==EQUAL)){
			target=i;
			break;
		}
	}if(target!=NOT_FOUND){
		return new binary(
			getExpression(l,target-1),
			(*tokens)[target],
			getExpression(target+1,r)
		);
	}

	for(int i=l;i<=r;i++){
		token& t=(*tokens)[i];
		if(t.getType()==LEFT_PAREN)top++;
		if(t.getType()==RIGHT_PAREN)top--;
		if(top==0&&(t.getType()==KEYWORD&&t.getLexeme()=="or")){
			target=i;
			break;
		}
	}if(target!=NOT_FOUND){
		return new binaryCircuit(
			getExpression(l,target-1),
			(*tokens)[target],
			getExpression(target+1,r)
		);
	}

	for(int i=l;i<=r;i++){
		token& t=(*tokens)[i];
		if(t.getType()==LEFT_PAREN)top++;
		if(t.getType()==RIGHT_PAREN)top--;
		if(top==0&&(t.getType()==KEYWORD&&t.getLexeme()=="and")){
			target=i;
			break;
		}
	}if(target!=NOT_FOUND){
		return new binaryCircuit(
			getExpression(l,target-1),
			(*tokens)[target],
			getExpression(target+1,r)
		);
	}

	for(int i=r;i>=l;i--){
		token& t=(*tokens)[i];
		if((*tokens)[i].getType()==LEFT_PAREN)top++;
		if((*tokens)[i].getType()==RIGHT_PAREN)top--;
		if(top==0&&(t.getType()==EQUAL_EQUAL||t.getType()==BANG_EQUAL)){
			target=i;
			break;
		}
	}if(target!=NOT_FOUND)
		return new binary(
			getExpression(l,target-1),
			(*tokens)[target],
			getExpression(target+1,r));
	
	for(int i=r;i>=l;i--){
		token& t=(*tokens)[i];
		if((*tokens)[i].getType()==LEFT_PAREN)top++;
		if((*tokens)[i].getType()==RIGHT_PAREN)top--;
		if(top==0&&(t.getType()==GREATER||t.getType()==LESS
		||t.getType()==GREATER_EQUAL||t.getType()==LESS_EQUAL)){
			target=i;
			break;
		}
	}if(target!=NOT_FOUND)
		return new binary(
			getExpression(l,target-1),
			(*tokens)[target],
			getExpression(target+1,r)
		);
	
	for(int i=r;i>=l;i--){
		token& t=(*tokens)[i];
		if((*tokens)[i].getType()==LEFT_PAREN)top++;
		if((*tokens)[i].getType()==RIGHT_PAREN)top--;
		if(top==0&&(t.getType()==PLUS||t.getType()==MINUS)){
			target=i;
			break;
		}
	}if(target!=NOT_FOUND){
		if(target==l&&(*tokens)[target].getType()==MINUS)
			return new unary((*tokens)[target],getExpression(target+1,r));
		return new binary(
			getExpression(l,target-1),
			(*tokens)[target],
			getExpression(target+1,r)
		);
	}

	for(int i=r;i>=l;i--){
		token& t=(*tokens)[i];
		if((*tokens)[i].getType()==LEFT_PAREN)top++;
		if((*tokens)[i].getType()==RIGHT_PAREN)top--;
		if(top==0&&(t.getType()==STAR||t.getType()==SLASH)){
			target=i;
			break;
		}
	}if(target!=NOT_FOUND){
		return new binary(
			getExpression(l,target-1),
			(*tokens)[target],
			getExpression(target+1,r)
		);
	}

	for(int i=l;i<=r;i++){
		token& t=(*tokens)[i];
		if((*tokens)[i].getType()==LEFT_PAREN)top++;
		if((*tokens)[i].getType()==RIGHT_PAREN)top--;
		if(top==0&&t.getType()==BANG){
			target=i;
			break;
		}
	}if(target!=NOT_FOUND){
		return new unary(
			(*tokens)[target],
			getExpression(target+1,r)
		);
	}

	if((*tokens)[l].getType()==LEFT_PAREN&&
	(*tokens)[r].getType()==RIGHT_PAREN){
		return new group(getExpression(l+1,r-1));
	}

	if((*tokens)[l].getType()==IDENTIFIER&&
	(*tokens)[r].getType()==RIGHT_PAREN){
		int top=0;
		for(int i=r;i>=l;i--){
			if((*tokens)[i].getType()==RIGHT_PAREN)top++;
			if((*tokens)[i].getType()==LEFT_PAREN)top--;
			if(top==0){
				target=i;break;
			}
		}
		std::vector<expression*>args;int lst=target+1;top=0;
		for(int i=target+1;i<r;i++){
			if((*tokens)[i].getType()==LEFT_PAREN)top++;
			if((*tokens)[i].getType()==RIGHT_PAREN)top--;
			if(top==0&&(*tokens)[i].getType()==COMMA){
				args.push_back(
					getExpression(lst,i-1)
				);
				lst=i+1;
			}
		}
		if(lst<=r-1)args.push_back(getExpression(lst,r-1));
		return new funcGroup(args,
			getExpression(l,target-1),
			(*tokens)[target].getLine()
		);
	}

	report((*tokens)[l].getLine(),"how do you come here??");
	return new literal(NIL,"nil"); 
}

bool parser::error(){
	return haveError;
}

#endif