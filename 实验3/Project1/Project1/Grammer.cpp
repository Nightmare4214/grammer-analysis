#include "Grammer.h"
#include<cctype>
/*
文法
14
S->H(){L}|H(){}
L->LG|G
G->C|I|W
C->E;|;
I->J(E)G|J(E)GKG
W->U(E)G
E->V=E|B
V->D|D[E]
B->AQA|A
A->AOT|T
O->+|-
T->TMF|F
M->*|/
F->(E)|V|N

原->简写
program:S
main:H
statement_list:L
statement:G
expression_stmt:C
if_stmt:I
if:J
else:K
while_stmt:W
while:U
expression:E
simple_expression:B
additive_expression:A
relop:R
addop:O
term:T
mulop:M
factor:F
var:V
ID:D
NUM:N
<=|<|>|>=|==|!=:Q

简写->原
S:program
H:main
L:statement_list
G:statement
C:expression_stmt
I:if_stmt
J:if
K:else
W:while_stmt
U:while
E:expression
B:simple_expression
A:additive_expression
R:relop
O:addop
T:term
M:mulop
F:factor
V:var
D:ID
N:NUM
Q:<=|<|>|>=|==|!=
*/
Grammer::Grammer(const string & left) :left(left) {
	dictionary["S"] = "program";
	dictionary["L"] = "statement_list";
	dictionary["G"] = "statement";
	dictionary["C"] = "expression_stmt";
	dictionary["I"] = "if_stmt";
	dictionary["W"] = "while_stmt";
	dictionary["E"] = "expression";
	dictionary["B"] = "simple_expression";
	dictionary["A"] = "additive_expression";
	dictionary["R"] = "relop";
	dictionary["O"] = "addop";
	dictionary["T"] = "term";
	dictionary["M"] = "mulop";
	dictionary["F"] = "factor";
	dictionary["V"] = "var";
	dictionary["H"] = "main";
	dictionary["U"] = "while";
	dictionary["J"] = "if";
	dictionary["K"] = "else";
	dictionary["D"] = "ID";
	dictionary["N"] = "NUM";
	dictionary["Q"] = "<=|<|>|>=|==|!=";
}
//增加产生式
void Grammer::insertRight(const string & production)
{
	right.insert(production);
}
//输出
void Grammer::display()
{
	cout << left << "->";
	for (set<string>::iterator it = right.begin(); it != right.end(); ++it) {
		if (it == right.begin())cout << *it;
		else cout << '|' << *it;
	}
	cout << endl;
}

void Grammer::displayOrigin()
{
	string temp_left = left.substr(0, 1);
	if (dictionary[temp_left] == "")cout << left;
	else cout << dictionary[temp_left] << left.substr(1);
	cout << " ->";
	for (set<string>::iterator it = right.begin(); it != right.end(); ++it) {
		if (it != right.begin())cout << '|';
		string s = *it;
		for (int i = 0; i < s.size(); ++i) {
			string tt = s.substr(i, 1);
			if (dictionary[tt] == "")cout << tt;
			else cout << dictionary[tt];
			cout << ' ';
			//if (isupper(s[i]))cout << dictionary[s.substr(i, 1)];
			//else cout << s[i];
		}
	}
	cout << endl;
}