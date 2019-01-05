#pragma once
#include "Grammer.h"
#include "analysis.h"
#include<vector>
#include<stack>
class LL {
private:
	const char Epsilon = '$';
	const string start = "S";
	//first集
	map<string, set<char> > first;
	//follow集
	map<string, set<char> > follow;
	//产生式左边->编号
	map<string, int> grammerDictionary;
	//所有的文法
	vector<Grammer> grammerVector;
	//终结符
	vector<char> vt;
	//文法左边->(终结符->产生式)
	map<string, map<char, string> > table;
	bool visit[500];
	void dfs(int x);
	//求first集
	void getFirst();
	//求follow集
	void getFollow();
	//将s1的follow加到s2
	void append(const string &s1, const string &s2);
	//判断产生式s的first集有没有字符ch
	bool check_first(const string &s, char ch);
	//判断s的follow集有没有ch
	bool check_follow(const string &s, char ch);
	//求LL(1)表
	void getTable();
	stack<string> str;
public:
	LL() {}
	LL(int n);
	LL(vector<Grammer> grammerVector,
		map<string, int> grammerDictionary);
	//输入
	void input(int n);
	//打印first集
	void printFirst();
	//打印follow集
	void printFollow();
	//打印LL(1)表
	void printTable();
	//获得LL(1)分析表
	void init();
	//token转类型
	string change(string a,int id) {
		if (a == "main")return "H";
		if (a == "while")return "U";
		if (a == "if")return "J";
		if (a == "else")return "K";
		if (id == 2)return "D";
		if (id == 3)return "N";
		if (a == "<=" || a == "<" || a == ">" || a == ">=" || a == "==" || a == "!=")return "Q";
		return a;
	}
	bool isSame(string x, string a, int id) {
		/*
		dictionary["H"] = "main";
	dictionary["U"] = "while";
	dictionary["J"] = "if";
	dictionary["K"] = "else";
	dictionary["D"] = "ID";
	dictionary["N"] = "NUM";
	dictionary["Q"] = "<=|<|>|>=|==|!=";
		*/
		if (x == "H"&&a == "main")return true;
		if (x == "U"&&a == "while")return true;
		if (x == "J"&&a == "if")return true;
		if (x == "K"&&a == "else")return true;
		if (x == "D"&&id == 2)return true;
		if (x == "N"&&id==3)return true;
		if (x == "Q" && (a == "<=" || a == "<" || a == ">" || a == ">=" || a == "==" || a == "!="))return true;
		return x == a;
	}
	bool grammerAnalysis(string filename = "test.txt");
};