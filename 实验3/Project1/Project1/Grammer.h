#pragma once
#include<iostream>
#include<map>
#include<string>
#include<set>
using namespace std;
//�ķ�
class Grammer {
public:
	string left;
	set<string> right;
	map<string, string> dictionary;
	Grammer(const string &left);
	//���Ӳ���ʽ
	void insertRight(const string &production);
	//���
	void display();
	void displayOrigin();
};