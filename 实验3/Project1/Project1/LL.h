#pragma once
#include "Grammer.h"
#include "analysis.h"
#include<vector>
#include<stack>
class LL {
private:
	const char Epsilon = '$';
	const string start = "S";
	//first��
	map<string, set<char> > first;
	//follow��
	map<string, set<char> > follow;
	//����ʽ���->���
	map<string, int> grammerDictionary;
	//���е��ķ�
	vector<Grammer> grammerVector;
	//�ս��
	vector<char> vt;
	//�ķ����->(�ս��->����ʽ)
	map<string, map<char, string> > table;
	bool visit[500];
	void dfs(int x);
	//��first��
	void getFirst();
	//��follow��
	void getFollow();
	//��s1��follow�ӵ�s2
	void append(const string &s1, const string &s2);
	//�жϲ���ʽs��first����û���ַ�ch
	bool check_first(const string &s, char ch);
	//�ж�s��follow����û��ch
	bool check_follow(const string &s, char ch);
	//��LL(1)��
	void getTable();
	stack<string> str;
public:
	LL() {}
	LL(int n);
	LL(vector<Grammer> grammerVector,
		map<string, int> grammerDictionary);
	//����
	void input(int n);
	//��ӡfirst��
	void printFirst();
	//��ӡfollow��
	void printFollow();
	//��ӡLL(1)��
	void printTable();
	//���LL(1)������
	void init();
	//tokenת����
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