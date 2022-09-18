#ifndef TOKENHPP
#define TOKENHPP

#include<cstring>
/*
token type:
type			lexeme		literal
LEFT_PAREN		(
RIGHT_PAREN		)
LEFT_BRACE		{
RIGHT_BRACE		}
COMMA			,
DOT				.
MINUS			-
PLUS			+
SEMICOLON		;
STAR			*
BANG_EQUAL		!=
BANG			!
EQUAL_EQUAL		==
EQUAL			=
GREATER_EQUAL	>=
GREATER			>
LESS_EQUAL		<=
LESS			<
SLASH			/
STRING 						a string
NUMBER						a number(in form of string)
IDENTIFIER		a name

KEYWORD			and
				class
				else
				false
				true
				for
				fun
				if
				nil
				or
				print
				return
				super
				this
				var
				while
				break
				continue
*/
enum tokenType{
	LEFT_PAREN,RIGHT_PAREN,LEFT_BRACE,RIGHT_BRACE,
	COMMA,DOT,MINUS,PLUS,SEMICOLON,STAR,
	BANG_EQUAL,BANG,EQUAL_EQUAL,EQUAL,
	GREATER_EQUAL,GREATER,LESS_EQUAL,LESS,
	SLASH,STRING,NUMBER,IDENTIFIER,KEYWORD
};

class token{
private:
	tokenType type;
	std::string lexeme;
	std::string literal;
	int line;
public:
	token(){};
	token(tokenType type,int line){
		this->type=type;
		this->lexeme=this->literal="";
		this->line=line;
	}
	token(tokenType type,std::string lexeme,std::string literal,int line){
		this->type=type;
		this->lexeme=lexeme;
		this->literal=literal;
		this->line=line;
	}
	std::string getLexeme(){
		return lexeme;
	}
	tokenType getType(){
		return type;
	}
	std::string getLiteral(){
		return literal;
	}
	std::string toString(){
		return "{"+lexeme+" "+literal+"}";
	}
	int getLine(){
		return line;
	}
};


#endif