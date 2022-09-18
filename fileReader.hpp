#ifndef FILEREADERHPP
#define FILEREADERHPP

#define DELETE(ptr) delete ptr;ptr=nullptr

bool isAlpha(char ch);
bool isDigit(char ch);
bool isLower(char ch);
bool isUpper(char ch);
bool reachEnd(FILE* file);
void readLine(FILE* file,std::string& buffer);
std::string getString(std::string& str,int l,int r);
std::string getStringPlus(std::string& str,int l,int r);

#endif