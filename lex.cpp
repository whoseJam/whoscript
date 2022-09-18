
#ifndef LEXCPP
#define LEXCPP

#include"lex.hpp"

std::string EMPTY=""; 

bool lex::haveError=false;

std::vector<token>* lex::getList(){
	return &tokenList;
}

decltype(lex::M) lex::getMap(){
	decltype(lex::M) tmp;
	tmp.insert(std::make_pair<std::string,std::string>("and","AND"));
	tmp.insert(std::make_pair<std::string,std::string>("class","CLASS"));
	tmp.insert(std::make_pair<std::string,std::string>("else","ELSE"));
	tmp.insert(std::make_pair<std::string,std::string>("false","FALSE"));
	tmp.insert(std::make_pair<std::string,std::string>("true","TRUE"));
	tmp.insert(std::make_pair<std::string,std::string>("for","FOR"));
	tmp.insert(std::make_pair<std::string,std::string>("fun","FUN"));
	tmp.insert(std::make_pair<std::string,std::string>("if","IF"));
	tmp.insert(std::make_pair<std::string,std::string>("nil","NIL"));
	tmp.insert(std::make_pair<std::string,std::string>("or","OR"));
	tmp.insert(std::make_pair<std::string,std::string>("print","PRINT"));
	tmp.insert(std::make_pair<std::string,std::string>("return","RETURN"));
	tmp.insert(std::make_pair<std::string,std::string>("super","SUPER"));
	tmp.insert(std::make_pair<std::string,std::string>("this","THIS"));
	tmp.insert(std::make_pair<std::string,std::string>("var","VAR"));
	tmp.insert(std::make_pair<std::string,std::string>("while","WHILE"));
	tmp.insert(std::make_pair<std::string,std::string>("break","BREAK"));
	tmp.insert(std::make_pair<std::string,std::string>("continue","CONTINUE"));
	return tmp;
}
decltype(lex::M) lex::M=lex::getMap();

void lex::report(int line,const char* msg){
	haveError=true;
	std::cout<<"[line "<<line<<"] Error: "<<msg<<"\n";
}

void lex::report(int line,std::string& msg){
	haveError=true;
	std::cout<<"[line "<<line<<"] Error: "<<msg<<"\n";
}

void lex::scan(FILE *file){
	std::string code;
	{	std::string line;
		while(reachEnd(file)==false){
			readLine(file,line);
			code=code+line+'\n';
		}
	}
	int current=0,line=1,len=code.length();
	while(current<len){
		getToken(code,current,line);
	}
}

void lex::addToken(token newToken){
	tokenList.push_back(newToken);
}

void lex::addString(std::string& code,int& current,int& line){
	int l=current;
	while(code[current]!='"'&&current<code.length()){
		if(code[current]=='\n')line++;
		current++;
	}
	if(current==code.length()){
		report(line,"Unterminated string\n");
		return;
	}
	int r=current-1;
	addToken(token(STRING,"",getStringPlus(code,l,r),line));
	current++;
}

void lex::addNumber(std::string& code,int& current,int& line){
	int l=current-1;
	while(isDigit(code[current]))current++;
	if(code[current]=='.'&&isDigit(code[current+1])){
		current++;
		while(isDigit(code[current]))current++;
	}
	int r=current-1;
	addToken(token(NUMBER,"",getString(code,l,r),line));
}

void lex::addIdentifier(std::string& code,int& current,int& line){
	int l=current-1;
	while(isAlpha(code[current]))current++;
	int r=current-1;
	std::string tmp=getString(code,l,r);
	auto it=M.find(tmp);
	if(it!=M.end()){
		addToken(token(KEYWORD,(*it).first,"",line));
	}else addToken(token(IDENTIFIER,tmp,"",line));
}

void lex::getToken(std::string& code,int& current,int& line){
	char ch=code[current];current++;
	switch(ch){
		case '(':addToken(token(LEFT_PAREN,"(","",line));break;
		case ')':addToken(token(RIGHT_PAREN,")","",line));break;
		case '{':addToken(token(LEFT_BRACE,"{","",line));break;
		case '}':addToken(token(RIGHT_BRACE,"}","",line));break;
		case ',':addToken(token(COMMA,",","",line));break;
		case '.':addToken(token(DOT,".","",line));break;
		case '-':addToken(token(MINUS,"-","",line));break;
		case '+':addToken(token(PLUS,"+","",line));break;
		case ';':addToken(token(SEMICOLON,";","",line));break;
		case '*':addToken(token(STAR,"*","",line));break;

		case '!':
			if(code[current]=='=')addToken(token(BANG_EQUAL,"!=","",line)),current++;
			else addToken(token(BANG,"!","",line));break;
		case '=':
			if(code[current]=='=')addToken(token(EQUAL_EQUAL,"==","",line)),current++;
			else addToken(token(EQUAL,"=","",line));break;
		case '>':
			if(code[current]=='=')addToken(token(GREATER_EQUAL,">=","",line)),current++;
			else addToken(token(GREATER,">","",line));break;
		case '<':
			if(code[current]=='=')addToken(token(LESS_EQUAL,"<=","",line)),current++;
			else addToken(token(LESS,"<","",line));break;
		
		case '/':
			if(code[current]=='/'){
				while(code[current]!='\n'&&current<code.length())current++;
			}else addToken(token(SLASH,"/","",line));
			break;
		
		case ' ':
		case '\r':
		case '\t':
			break;
		case '\n':line++;break;

		case '"':addString(code,current,line);break;
		default:
			if(isDigit(ch)){
				addNumber(code,current,line);
			}else if(isAlpha(ch)){
				addIdentifier(code,current,line);
			}
			else report(line,"Unexpected character.");
			break;
	}
}

void lex::output(){
	for(auto it:tokenList){
		std::cout<<it.toString()<<"\n";
	}
}

bool lex::error(){
	return haveError;
}

#endif