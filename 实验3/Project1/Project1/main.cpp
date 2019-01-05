#include "LL.h"
#include "RemoveRecursion.h"
#include "analysis.h"
int main() {
	/*
14
S->H(){L}|H(){}
L->LG|G
G->C|I|W
C->E;|;
I->J(E)GKG
W->U(E)G
E->V=E|B
V->D|D[E]
B->AQA|A
A->AOT|T
O->+|-
T->TMF|F
M->*|/
F->(E)|V|N
	*/
	int n;
	cout << "请输入G[S]的产生式数量" << endl;
	cin >> n;
	RemoveRecursion a;
	a.remove(n);
	cout << endl;
	a.print();
	LL t(a.grammerVector, a.grammerDictionary);
	/*LL t(n);*/
	cout << endl;
	t.init();
	cout << endl;
	t.printFirst();
	cout << endl;
	t.printFollow();
	cout << endl;
	t.printTable();
	bool result = t.grammerAnalysis();
	cout << endl;
	if (result) {
		cout << "成功" << endl;
	}
	else {
		cout << "失败" << endl;
	}
	return 0;
}