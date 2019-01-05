#pragma once
#include "Grammer.h"
#include<cctype>
#include<vector>
#include<map>
class RemoveRecursion {
private:
	bool visit[300];
	//G[S]
	const string start = "S";
	//空字ε
	const string Epsilon = "$";
	void dfs(int x);
	//消除grammer_id的直接左递归
	void removeDirectRecursion(int grammer_id);
	//提取grammer_id的公共左因子
	void removeGene(int grammer_id);
	//消除间接左递归
	void removeIndirectRecursion();
	//简化
	void simplify();
public:
	//输入
	void input(int n);
	//产生式左边->编号
	map<string, int> grammerDictionary;
	//所有的文法
	vector<Grammer> grammerVector;
	//输出
	void print();
	//输出原本的
	void printOrigin();
	//消除左递归
	void remove(int n);
};