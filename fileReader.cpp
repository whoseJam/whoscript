
#ifndef FILEREADERCPP
#define FILEREADERCPP

#include<iostream>
#include"fileReader.hpp"

bool isAlpha(char ch){
	return isLower(ch)||isUpper(ch)||ch=='_';
}
bool isDigit(char ch){
	return '0'<=ch&&ch<='9';
}
bool isLower(char ch){
	return 'a'<=ch&&ch<='z';
}
bool isUpper(char ch){
	return 'A'<=ch&&ch<='Z';
}

bool reachEnd(FILE* file){
	char ch=fgetc(file);
	if(ch==EOF)return true;
	fseek(file,-1,SEEK_CUR);
	return false;
}

void readLine(FILE* file,std::string& buffer){
	buffer.clear();
	char ch=fgetc(file);
	while(ch!='\n'&&ch!=EOF){
		buffer.push_back(ch);
		ch=fgetc(file);
	}
}

std::string getString(std::string& str,int l,int r){
	std::string ans;
	for(int i=l;i<=r;i++)
		ans.push_back(str[i]);
	return ans;
}

std::string getStringPlus(std::string& str,int l,int r){
	std::string ans;
	for(int i=l;i<=r;i++){
		if(str[i]=='\\'&&i+1<=r){
			switch(str[i+1]){
				case 'n':ans.push_back('\n');break;
				case 'r':ans.push_back('\r');break;
				case 't':ans.push_back('\t');break;
			}
			i++;
		}
		else ans.push_back(str[i]);
	}
	return ans;
}

#endif