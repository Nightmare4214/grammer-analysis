#pragma once
#include<iostream>
#include<stack>
#include<set>
#include<fstream>
#include<string>
using namespace std;
class Analysis {
public:
	//1基本保留字、2标识符、3常量、4运算符、5分隔符
	set<string> keyword;
	//标识符
	set<string> identifier;
	//常数表
	set<string> constant;
	//检测括号是否匹配
	stack<char> match;
	char character;
	string token;
	//是否终止程序;
	bool jumpEnd;
	//是否终止匹配括号
	bool endMatch;
	ifstream fin;
	Analysis();
	//若character中的字符为空，则继续读字符，直到非空
	void getbe();
	//将token中字符串与character中的字符连接作为token中的新字符串
	void concatenation();
	//判断character中的字符是否为字母
	bool letter();
	//判断character中的字符是否为数字
	bool digit();
	//判断token中的字符串是否在保留字表中
	bool reserve();
	//扫描指针回退一个字符，同时将character置为空白
	void retract();
	//将标识符登记到符号表中(id=2),将常数登记到常数表(id=3)
	void buildlist(int id);
	//错误处理
	void error();
	bool isEnd();
	bool isOp(char getOp);
	//判断科学记数法，返回：3代表数字，4代表运算符，-1代表错误
	int isUnsignNum();
	//1基本保留字、2标识符、3常量、4运算符、5分隔符,-1代表错误,0代表识别了注释或者预处理指令,-2读到文件尾
	//词法分析
	int analysis();
	int getNext();
};