#include "LL.h"
LL::LL(int n)
{
	input(n);
}
LL::LL(vector<Grammer> grammerVector, map<string, int> grammerDictionary) :
	grammerVector(grammerVector),
	grammerDictionary(grammerDictionary) {}
//输入
void LL::input(int n)
{
	while (n--) {
		string inputGrammer;
		cin >> inputGrammer;
		int index = inputGrammer.find('-');
		string left = inputGrammer.substr(0, index);
		//从第一个-,移动到>后一个
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

void LL::dfs(int x)
{
	if (visit[x])return;
	visit[x] = true;
	string left = grammerVector[x].left;
	set<string> right = grammerVector[x].right;
	//遍历产生式
	for (set<string>::iterator it = right.begin(); it != right.end(); ++it) {
		string s = *it;
		bool flag = true;
		int i = 0;
		for (; i < s.size(); ++i) {
			//终结符
			if (!isupper(s[i]) && s[i] != '\'') {
				first[left].insert(s[i]);
				break;
			}
			if (isupper(s[i])) {
				int next = i + 1;
				string temp;
				while (next != s.size() && s[next] == '\'') {
					++next;
				}
				temp = s.substr(i, next - i);
				i = next - 1;
				int id = grammerDictionary[temp] - 1;
				if (id < 0) {
					first[left].insert(temp[0]);
					break;
				}
				//A->Ba1a2，将B的first先求出来，加入A
				dfs(id);
				set<char> temp_first = first[temp];
				flag = true;
				for (set<char>::iterator it = temp_first.begin(); it != temp_first.end(); ++it) {
					//B的first去掉Epsilon加入A
					if (*it == Epsilon)flag = false;
					else first[left].insert(*it);
				}
				if (flag)break;
			}

		}
		//A->B1B2B3..Bn全都是空字，则把空字加入first
		if (i == s.size() && !flag) {
			first[left].insert(Epsilon);
		}
	}
}
//求first集
void LL::getFirst()
{
	memset(visit, false, sizeof(visit));
	for (int i = 0; i < grammerVector.size(); ++i) {
		dfs(i);
	}
}
//将s1的follow加到s2
void LL::append(const string & s1, const string & s2)
{
	set<char> from = follow[s1];
	for (set<char>::iterator it = from.begin(); it != from.end(); ++it) {
		follow[s2].insert(*it);
	}
}
//求follow集
void LL::getFollow()
{
	follow[start].insert('#');
	while (true) {
		//是否变大
		bool extend = false;
		//遍历文法
		for (int i = 0; i < grammerVector.size(); ++i) {
			string left = grammerVector[i].left;
			set<string> right = grammerVector[i].right;
			//遍历产生式
			for (set<string>::iterator it = right.begin(); it != right.end(); ++it) {
				//用于判断β是否为空（为空把A的follow加到B）
				bool flag = true;
				const string s = *it;
				//逆序遍历
				for (int j = s.size() - 1; j >= 0; --j) {
					int pre_j = j;
					//获取下一个
					string str;
					//如果是'，说明是非终结符
					if (s[j] == '\'') {
						int next = j - 1;
						while (next >= 0 && s[next] == '\'')--next;
						str = s.substr(next, j - next + 1);
						j = next;
					}
					else str = s.substr(j, 1);
					int id = grammerDictionary[str] - 1;
					//说明是终结符
					if (id < 0) {
						//后面不为空
						flag = false;
						continue;
					}
					//后面为空，把A的follow加到B
					if (flag) {
						int pre_size = follow[str].size();
						append(left, str);
						int now_size = follow[str].size();
						if (now_size > pre_size)extend = true;
					}
					//说明str到不了空字
					if (first[str].find(Epsilon) == first[str].end())flag = false;
					//将first集加入B
					for (int k = pre_j + 1; k < s.size(); ++k) {
						if (isupper(s[k])) {
							int next = k + 1;
							while (next != s.size() && s[next] == '\'')++next;
							//下一个
							string f = s.substr(k, next - k);
							k = next - 1;
							int id = grammerDictionary[f] - 1;
							//终结符
							if (id < 0) {
								int pre_size = follow[str].size();
								follow[str].insert(f[0]);
								int now_size = follow[str].size();
								if (now_size > pre_size)extend = true;
								break;
							}
							//下一个的first集
							set<char> first_set = first[f];
							int pre_size = follow[str].size();
							for (set<char>::iterator it = first_set.begin(); it != first_set.end(); ++it) {
								//去掉Epsilon的first集加入B
								if (*it != Epsilon)follow[str].insert(*it);
							}
							int now_size = follow[str].size();
							if (now_size > pre_size)extend = true;
							if (first[f].find(Epsilon) == first[f].end())break;
						}
						else {//终结符
							int pre_size = follow[str].size();
							follow[str].insert(s[k]);
							int now_size = follow[str].size();
							if (now_size > pre_size)extend = true;
							break;
						}
					}
				}
			}
		}
		if (!extend)break;
	}
}
//打印first集
void LL::printFirst()
{
	for (Grammer g : grammerVector) {
		cout << "FIRST(" + g.left + "):{";
		set<char> st = first[g.left];
		for (set<char>::iterator it = st.begin(); it != st.end(); ++it) {
			if (it != st.begin())cout << ',';
			cout << *it;
		}
		cout << '}' << endl;
	}
}
//打印follow集
void LL::printFollow()
{
	for (Grammer g : grammerVector) {
		cout << "Follow(" + g.left + "):{";
		set<char> st = follow[g.left];
		for (set<char>::iterator it = st.begin(); it != st.end(); ++it) {
			if (it != st.begin())cout << ',';
			cout << *it;
		}
		cout << '}' << endl;
	}
}
//判断产生式s的first集有没有字符ch
bool LL::check_first(const string & s, char ch)
{
	bool hasEpsilon = true;
	int i = 0;
	for (; i < s.size(); ++i) {
		//终结符
		if (!isupper(s[i]) && s[i] != '\'') {
			return s[i] == ch;
		}
		else if (isupper(s[i])) {
			string temp;
			int next = i + 1;
			while (next != s.size() && s[next] == '\'')++next;
			temp = s.substr(i, next - i);
			i = next - 1;
			int id = grammerDictionary[temp] - 1;
			//说明是终结符
			if (id < 0) {
				return ch == temp[0];
			}
			set<char> dic = first[temp];
			hasEpsilon = false;
			//遍历first集，如果有空字，对应A->B1B2...Bi 的B1..Bi-1全空的情况
			for (set<char>::iterator it = dic.begin(); it != dic.end(); ++it) {
				if (*it == Epsilon)hasEpsilon = true;
				else if (*it == ch)return true;
			}
			if (!hasEpsilon)break;
		}
	}
	if (i == s.size() && hasEpsilon&&ch == Epsilon)return true;
	return false;
}
//判断s的follow集有没有ch
bool LL::check_follow(const string & s, char ch)
{
	return follow[s].find(ch) != follow[s].end();
}
//求LL(1)表
void LL::getTable()
{
	bool visited[500];
	memset(visited, false, sizeof(visited));
	//获得所有终结符
	for (int i = 0; i < grammerVector.size(); ++i) {
		set<string> right = grammerVector[i].right;
		for (set<string>::iterator it = right.begin(); it != right.end(); ++it) {
			string s = *it;
			for (int i = 0; i < s.size(); ++i) {
				//终结符直接加
				if (!isupper(s[i]) && s[i] != '\'') {
					if (s[i] == Epsilon)continue;
					if (!visited[s[i]]) {
						visited[s[i]] = true;
						vt.push_back(s[i]);
					}
				}
				else if (isupper(s[i])) {
					string temp;
					int next = i + 1;
					while (next != s.size() && s[next] == '\'')++next;
					temp = s.substr(i, next - i);
					i = next - 1;
					int id = grammerDictionary[temp] - 1;
					//终结符
					if (id < 0) {
						if (temp[0] == Epsilon)continue;
						if (!visited[temp[0]]) {
							visited[temp[0]] = true;
							vt.push_back(temp[0]);
						}
					}
				}
			}
		}
	}
	vt.push_back('#');
	for (int i = 0; i < grammerVector.size(); ++i) {
		string left = grammerVector[i].left;
		set<string> right = grammerVector[i].right;
		//遍历产生式
		for (set<string>::iterator it = right.begin(); it != right.end(); ++it) {
			//对于每个终结符，如果在first里，则在table[left][vt[j]]填入*it
			for (int j = 0; j < vt.size(); ++j) {
				if (check_first(*it, vt[j])) {
					if (table[left][vt[j]] != "")
						cout << left << "的first " << *it << " 和 " << table[left][vt[j]] << " 冲突" << endl;
					table[left][vt[j]] = *it;
				}
			}
			if (check_first(*it, Epsilon)) {
				set<char> f = follow[left];
				for (set<char>::iterator it_j = f.begin(); it_j != f.end(); ++it_j) {
					if (table[left][*it_j] != "")
						cout << left << " 的follow " << *it << " 和 " << table[left][*it_j] << " 冲突" << endl;
					table[left][*it_j] = *it;
				}
			}
		}
	}
}
//打印LL(1)表
void LL::printTable()
{
	cout << "" << '\t';
	for (const char &c : vt) {
		cout << c << '\t';
	}
	cout << endl;
	for (Grammer g : grammerVector) {
		cout << g.left << '\t';
		for (const char &c : vt)cout << table[g.left][c] << '\t';
		cout << endl;
	}
}
//获得LL(1)分析表
void LL::init()
{
	getFirst();
	getFollow();
	getTable();
}
bool LL::grammerAnalysis(string filename)
{
	str = stack<string>();
	str.push("#");
	str.push(start);
	bool end = false;
	Analysis r;
	ifstream &fin = r.fin;
	int id;
	fin.open(filename, ios::binary | ios::in);
	if (!fin)
	{
		cout << "文件打开失败" << endl;
		return false;
	}
	id = r.getNext();
	string x, a = r.token;
	a = change(a, id);
	if (id == -2)end = true;
	while (!str.empty()) {
		x = str.top();
		str.pop();
		//如果是终结符
		if (!isupper(x[0])) {
			if (x == a) {
				if (!end) {
					id = r.getNext();
					a = r.token;
					a = change(a, id);
					if (id == -2)end = true;
				}
			}
			else return false;
		}
		else {
			//终结符
			if (!grammerDictionary[x]) {
				if (x == a) {
					if (!end) {
						id = r.getNext();
						a = r.token;
						a = change(a, id);
						if (id == -2)end = true;
					}
				}
				else return false;
			}
			else {
				//逆序压入栈
				string production = table[x][a[0]];
				if (production == "") {
					cout << "错误" << endl;
					return false;
				}
				for (int i = production.size() - 1; i >= 0; --i) {
					if (production[i] == Epsilon)continue;
					if (production[i] != '\'')str.push(production.substr(i, 1));
					else {
						int next = i - 1;
						while (production[next] == '\''&&next >= 1)--next;
						str.push(production.substr(next, i - next + 1));
						i = next;
					}
				}
				//cout << x << "->" << production << endl;
				Grammer temp(x);
				temp.insertRight(production);
				temp.displayOrigin();
			}
		}
	}
	return str.empty();
}
//#include "LL.h"
//#include "RemoveRecursion.h"
//int main() {
//	int n;
//	cout << "请输入G[S]的产生式数量" << endl;
//	cin >> n;
//	RemoveRecursion r;
//	r.remove(n);
//	cout << endl;
//	r.print();
//	LL t(r.grammerVector, r.grammerDictionary);
//	cout << endl;
//	t.init();
//	cout << endl;
//	t.printFirst();
//	cout << endl;
//	t.printFollow();
//	cout << endl;
//	t.printTable();
//	return 0;
//}