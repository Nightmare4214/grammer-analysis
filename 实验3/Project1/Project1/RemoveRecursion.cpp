#include "RemoveRecursion.h"
//����
void RemoveRecursion::input(int n) {
	while (n--) {
		string inputGrammer;
		cin >> inputGrammer;
		int index = inputGrammer.find('-');
		string left = inputGrammer.substr(0, index);
		//�ӵ�һ��-,�ƶ���>��һ��
		index += 2;
		int id;
		if (!grammerDictionary[left]) {
			grammerVector.push_back(Grammer(left));
			grammerDictionary[left] = id = grammerVector.size();
			--id;
		}
		for (int i = index; i < inputGrammer.size(); ++i) {
			if (inputGrammer[i] == '|') {
				grammerVector[id].insertRight(inputGrammer.substr(index, i - index));
				index = i + 1;
			}
		}
		grammerVector[id].insertRight(inputGrammer.substr(index));
	}
}
//����grammer_id��ֱ����ݹ�
void RemoveRecursion::removeDirectRecursion(int grammer_id)
{
	if (grammer_id < 0)return;
	string first = grammerVector[grammer_id].left;
	//ԭ������ʽ
	set<string> temp = grammerVector[grammer_id].right;
	string vn = first + "'";
	while (grammerDictionary[vn])vn += "'";
	bool flag = true;
	int id = 0;
	//��������ʽ�ж���û��ֱ����ݹ�
	for (set<string>::iterator it = temp.begin(); it != temp.end(); ++it) {
		if (it->find(first) == 0) {
			grammerVector.push_back(Grammer(vn));
			grammerDictionary[vn] = id = grammerVector.size();
			--id;
			flag = false;
			break;
		}
	}
	//û��ֱ����ݹ�
	if (flag)return;
	//�µĲ���ʽ
	vector<string> temp_production;
	//A'�Ĳ���ʽ
	set<string>& production = grammerVector[id].right;
	production.insert(Epsilon);
	//ԭ������ʽ������
	set<string>& origin = grammerVector[grammer_id].right;
	while (!origin.empty()) {
		string s = *(origin.begin());
		//A->Aa1
		if (s.find(first) == 0) {
			production.insert(s.substr(1) + vn);
		}
		else {
			//A->b
			temp_production.push_back(s + vn);
		}
		origin.erase(origin.begin());
	}
	for (const string &t : temp_production)origin.insert(t);
}
void RemoveRecursion::removeGene(int grammer_id)
{
	if (grammer_id < 0)return;
	while (true) {
		bool flag = true;
		for (set<string>::iterator it_i = grammerVector[grammer_id].right.begin(); it_i != grammerVector[grammer_id].right.end(); ++it_i) {
			string s1 = *it_i;
			string s2;
			//�Ƿ���ȡ�˹���������
			flag = false;
			set<string>::iterator it_j = it_i;
			++it_j;
			for (; it_j != grammerVector[grammer_id].right.end(); ++it_j) {
				s2 = *it_j;
				if (s1[0] == s2[0]) {
					flag = true;
					break;
				}
			}
			if (!flag)continue;
			int index = 0;
			while (index < s1.size() && index < s2.size() && s1[index] == s2[index])++index;
			//����������
			string pre = s1.substr(0, index);
			int len = pre.size();
			string vn = grammerVector[grammer_id].left + "'";
			while (grammerDictionary[vn]) {
				vn += "'";
			}
			grammerVector.push_back(Grammer(vn));
			int id;
			grammerDictionary[vn] = id = grammerVector.size();
			--id;
			set<string>& vn_right = grammerVector[id].right;
			vector<string> production;
			//�����µ�right����վɵ�
			while (!grammerVector[grammer_id].right.empty()) {
				string first = *(grammerVector[grammer_id].right.begin());
				if (first.find(pre) == 0) {
					string t = first.substr(len);
					if (t == "") {
						vn_right.insert(Epsilon);
					}
					else {
						vn_right.insert(t);
					}
				}
				else {
					production.push_back(first);
				}
				grammerVector[grammer_id].right.erase(grammerVector[grammer_id].right.begin());
			}
			for (const string &s : production)grammerVector[grammer_id].right.insert(s);
			grammerVector[grammer_id].right.insert(pre + vn);
			break;
		}
		if (!flag)break;
	}
}
//���������ݹ�
void RemoveRecursion::removeIndirectRecursion()
{
	for (int i = grammerVector.size() - 1; i >= 0; --i) {
		removeGene(i);
		for (int j = grammerVector.size() - 1; j > i; --j) {
			//�µĲ���ʽ
			vector<string> production;
			//���ã�ֱ��ʹ���������set
			set<string>& right_i = grammerVector[i].right;
			set<string>& right_j = grammerVector[j].right;
			//���j����ߵķ��ս��
			string first = grammerVector[j].left;
			for (set<string>::iterator it_i = right_i.begin(); it_i != right_i.end(); ++it_i) {
				if (it_i->find(first) != 0)continue;
				for (set<string>::iterator it_j = right_j.begin(); it_j != right_j.end(); ++it_j) {
					production.push_back(*it_j + it_i->substr(first.size()));
				}
			}
			int nn = right_i.size();
			//������ʽ�����µĲ���ʽvector���棬�����ԭ���Ĳ���ʽ
			while (nn--) {
				if (right_i.begin()->find(first) != 0) {
					production.push_back(*(right_i.begin()));
				}
				right_i.erase(right_i.begin());
			}
			for (const string &t : production)right_i.insert(t);
		}
		removeGene(i);
		removeDirectRecursion(i);
	}
}
void RemoveRecursion::dfs(int x) {
	if (visit[x])return;
	visit[x] = true;
	for (set<string>::iterator it = grammerVector[x].right.begin(); it != grammerVector[x].right.end(); ++it) {
		string s = *it;
		for (int i = 0; i < s.size(); ++i) {
			if (isupper(s[i])) {
				int next = i + 1;
				while (next != s.size() && s[next] == '\'') {
					++next;
				}
				int id = grammerDictionary[s.substr(i, next - i)] - 1;
				i = next - 1;
				/*int id = grammerDictionary[s.substr(i, 1)] - 1;
				if (i != s.size() - 1 && s[i + 1] == '\'') {
					id = grammerDictionary[s.substr(i, 2)] - 1;
					++i;
				}*/
				if (id < 0)continue;
				dfs(id);
			}
		}
	}
}
//��
void RemoveRecursion::simplify() {
	memset(visit, 0, sizeof(visit));
	dfs(grammerDictionary[start] - 1);
	vector<Grammer> temp;
	grammerDictionary.clear();
	int cnt = 0;
	for (int i = 0; i < grammerVector.size(); ++i) {
		if (visit[i]) {
			temp.push_back(grammerVector[i]);
			++cnt;
			grammerDictionary[grammerVector[i].left] = cnt;
		}
	}
	grammerVector.clear();
	grammerVector = temp;
}
//���
void RemoveRecursion::print() {
	for (Grammer g : grammerVector)g.display();
}
//���ԭ����
void RemoveRecursion::printOrigin()
{
	for (Grammer g : grammerVector)g.displayOrigin();
}
//������ݹ�
void RemoveRecursion::remove(int n) {
	input(n);
	removeIndirectRecursion();
	simplify();
}
//#include<iostream>
//#include "RemoveRecursion.h"
//using namespace std;
//int main() {
//	int n;
//	cout << "������G[S]�Ĳ���ʽ����" << endl;
//	cin >> n;
//	RemoveRecursion r;
//	r.remove(n);
//	cout << endl;
//	r.print();
//	return 0;
//}