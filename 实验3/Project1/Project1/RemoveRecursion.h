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
	//���֦�
	const string Epsilon = "$";
	void dfs(int x);
	//����grammer_id��ֱ����ݹ�
	void removeDirectRecursion(int grammer_id);
	//��ȡgrammer_id�Ĺ���������
	void removeGene(int grammer_id);
	//���������ݹ�
	void removeIndirectRecursion();
	//��
	void simplify();
public:
	//����
	void input(int n);
	//����ʽ���->���
	map<string, int> grammerDictionary;
	//���е��ķ�
	vector<Grammer> grammerVector;
	//���
	void print();
	//���ԭ����
	void printOrigin();
	//������ݹ�
	void remove(int n);
};