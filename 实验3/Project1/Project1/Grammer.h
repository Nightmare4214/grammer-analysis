#pragma once
#include<iostream>
#include<map>
#include<string>
#include<set>
using namespace std;
//文法
class Grammer {
public:
	string left;
	set<string> right;
	map<string, string> dictionary;
	Grammer(const string &left);
	//增加产生式
	void insertRight(const string &production);
	//输出
	void display();
	void displayOrigin();
};