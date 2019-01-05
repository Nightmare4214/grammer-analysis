#include"analysis.h"
Analysis::Analysis()
{
	//������
	string reserved[] = { "try", "catch", "class", "signed", "unsigned", "char", "int", "short", "long", "bool", "long long", "size_t",
	"enum", "false", "true", "float", "double", "long double", "string", "typedef", "void", "struct", "define", "const", "delete", "dynamic_cast",
	"static", "static_cast", "switch", "case", "break", "default", "for", "while", "do", "continue", "if", "else", "goto", "explicit",
	"export", "extern", "new", "operator", "private", "public", "protected", "register", "return", "sizeof", "throw", "template", "this", "typeid",
	"friend", "inline", "using", "namespace", "union", "virtual", "reinterpret_cast", "typename", "volatile", "wchar_t", "main" };
	//������/�ؼ��ֵļ���
	keyword.insert(reserved, reserved + 65);
	endMatch = false;
	jumpEnd = false;
}
//��character�е��ַ�Ϊ�գ���������ַ���ֱ���ǿ�
void Analysis::getbe() {
	while (character == ' ' || character == '\r' || character == '\n' || character == '\t') {
		character = fin.get();
	}
}
//��token���ַ�����character�е��ַ�������Ϊtoken�е����ַ���
void Analysis::concatenation() {
	token += character;
}
//�ж�character�е��ַ��Ƿ�Ϊ��ĸ
bool Analysis::letter() {
	return character >= 'a'&&character <= 'z' || character >= 'A'&&character <= 'Z';
}
//�ж�character�е��ַ��Ƿ�Ϊ����
bool Analysis::digit() {
	return character >= '0'&&character <= '9';
}
//�ж�token�е��ַ����Ƿ��ڱ����ֱ���
bool Analysis::reserve() {
	return keyword.find(token) == keyword.end();
}
//ɨ��ָ�����һ���ַ���ͬʱ��character��Ϊ�հ�
void Analysis::retract() {
	fin.seekg(-1, ios::cur);
	character = ' ';
}
//����ʶ���Ǽǵ����ű���(id=2),�������Ǽǵ�������(id=3)
void Analysis::buildlist(int id) {
	if (id == 2) {
		identifier.insert(token);
	}
	else {
		constant.insert(token);
	}
}
//������
void Analysis::error() {
	cout << "error:" << token << endl;
}
bool Analysis::isEnd() {
	return character == ' ' || character == '\r' || character == '\n' || character == '\t' || character == '(' || character == ')' || character == '{' || character == '}' || character == '[' || character == ']' || character == ';' || character == ',' || character == EOF;
}
bool Analysis::isOp(char getOp) {
	return getOp == '+' || getOp == '-' || getOp == '*' || getOp == '/';
}
//�жϿ�ѧ�����������أ�3�������֣�4�����������-1�������
int Analysis::isUnsignNum()
{
	//��ͷС�����ж�
	if (character == '.')
	{
		concatenation();
		character = fin.get();
		if (character == '_' || letter()) {
			retract();

			return 4;
		}
	}
	//��ȡ��������E����
	while (!fin.eof() && digit()) {
		concatenation();
		character = fin.get();
	}
	if (character == '.')//����С���㣬����С������
	{
		do {
			concatenation();
			character = fin.get();
		} while (!fin.eof() && digit());//��ȡС�����ֵ�����
		if (character == 'E' || character == 'e')//�������E
		{
			concatenation();
			character = fin.get();
			if (character == '+' || character == '-')//E����ֻ�ܸ�+��-
			{
				concatenation();
				character = fin.get();
				//��ȡE,E+,E-��������ֲ���
				while (!fin.eof() && digit()) {
					concatenation();
					character = fin.get();
				}
			}
			else if (!fin.eof() && digit())
			{
				//��ȡE,E+,E-��������ֲ���
				while (!fin.eof() && digit()) {
					concatenation();
					character = fin.get();
				}
			}
			else
			{
				while (!isEnd() && !isOp(character))//��û������ƥ��ʧ�ܣ�error()
				{
					concatenation();
					character = fin.get();
				}
				retract();
				return -1;
			}
		}
	}
	else if (character == 'E' || character == 'e')//û�г���С����,��ȡ�������ֵ�E
	{
		concatenation();
		character = fin.get();
		if (character == '+' || character == '-')//E����ֻ�ܸ�+��-
		{
			concatenation();
			character = fin.get();
			//��ȡE,E+,E-��������ֲ���
			while (!fin.eof() && digit()) {
				concatenation();
				character = fin.get();
			}
		}
		else if (!fin.eof() && digit())
		{
			//��ȡE,E+,E-��������ֲ���
			while (!fin.eof() && digit()) {
				concatenation();
				character = fin.get();
			}
		}
		else
		{
			while (!isEnd() && !isOp(character))//��û������ƥ��ʧ�ܣ�error()
			{
				concatenation();
				character = fin.get();
			}
			retract();
			return -1;
		}
	}
	else if (character == 'x' || character == 'X') {
		concatenation();
		character = fin.get();
		while (!fin.eof() && (digit() || character >= 'a'&&character <= 'f' || character >= 'A'&&character <= 'Z')) {
			concatenation();
			character = fin.get();
		}
	}
	//ƥ��������жϸ��޷��Ŷ�ȡ�Ƿ����
	if (isEnd() || isOp(character))
	{
		retract();
		buildlist(3);
		return 3;
	}
	else
	{
		while (!isEnd() && !isOp(character))//��û������ƥ��ʧ�ܣ�error()
		{
			concatenation();
			character = fin.get();
		}
		retract();
		return -1;
	}
}
//1���������֡�2��ʶ����3������4�������5�ָ���,-1�������,0����ʶ����ע�ͻ���Ԥ����ָ��,-2�����ļ�β
//�ʷ�����
int Analysis::analysis() {
	token = "";
	character = fin.get();
	getbe();
	bool finded;
	bool changed;
	if (character == EOF)
		return -2;
	if (letter() || character == '_') {
		while (letter() || digit() || character == '_') {
			concatenation();
			character = fin.get();
		}
		if (reserve()) {
			//��ʶ��
			buildlist(2);
			retract();
			//cout << "(2,\"" + token + "\")" << endl;
			return 2;
		}
		else {
			//������
			//cout << "(1,\"" + token + "\")" << endl;
			retract();
			return 1;
		}
		if (character == '.') {
			token = ".";
			//cout << "(4,\"" + token + "\")" << endl;
			return 4;
		}
		else
			retract();
	}
	else if (digit() || character == '.') {
		int t = isUnsignNum();
		if (t == 3) {
			//����
			//cout << "(3,\"" << token << "\")" << endl;
			return 3;
		}
		else if (t == -1) {
			//����
			error();
			return -1;
		}
		else if (t == 4) {
			//�Զ���ʽ���ʳ�Ա.
			//cout << "(4,\"" + token + "\")" << endl;
			return 4;
		}
	}
	else
	{
		switch (character)
		{
		case '+':
			concatenation();
			character = fin.get();
			if (character == '+') {
				//����++
				concatenation();
				//cout << "(4,\"" << token << "\")" << endl;
			}
			else if (character == '=') {
				//�Ժ͸�ֵ+=
				concatenation();
				//cout << "(4,\"" << token << "\")" << endl;
			}
			else {
				//�Ӻ�+
				retract();
				//cout << "(4,\"" << token << "\")" << endl;
			}
			return 4;
			break;
		case '-':
			concatenation();
			character = fin.get();
			if (character == '-') {
				//�Լ�--
				concatenation();
				//cout << "(4,\"" << token << "\")" << endl;
				return 4;
			}
			else if (character == '>') {
				//��ָ�뷽ʽ���ʳ�Ա->
				concatenation();
				//cout << "(4,\"" << token << "\")" << endl;
				return 4;
			}
			else if (character == '=') {
				//�Բֵ-=
				concatenation();
				//cout << "(4,\"" << token << "\")" << endl;
				return 4;
			}
			else if (digit())
			{
				char getOp = ' ';
				if (fin.tellg() >= 3) {
					fin.seekg(-3, ios::cur);
					getOp = fin.get();
					fin.seekg(2, ios::cur);
				}
				//�����ǰ�������ַ��������ǵȺŻ�����������߿ո���ʶ��Ϊ����;
				if (isOp(getOp) || getOp == '=' || getOp == '\n' || getOp == '\t' || getOp == ' ')
				{
					int t = isUnsignNum();
					if (t == 3) {
						//����
						//cout << "(3,\"" << token << "\")" << endl;
						return 3;
					}
					else if (t == -1) {
						//����
						error();
						return -1;
					}
					else if (t == 4) {
						//�Զ���ʽ���ʳ�Ա
						//cout << "(4,\"" + token + "\")" << endl;
						return 4;
					}
				}
			}
			else {
				//����-
				retract();
				//cout << "(4,\"" + token + "\")" << endl;
				return 4;
			}
			break;
		case '*':
			concatenation();
			character = fin.get();
			if (character == '=') {
				//�Ի���ֵ*=
				concatenation();
				//cout << "(4,\"" << token << "\")" << endl;
			}
			else {
				//�˺�/ȡָ��ָ���ֵ*
				retract();
				//cout << "(4,\"" << token << "\")" << endl;
			}
			return 4;
			break;
		case '%':
			concatenation();
			character = fin.get();
			if (character == '=') {
				//��ȡ������ֵ%=
				concatenation();
				//cout << "(4,\"" << token << "\")" << endl;
			}
			else {
				//ȡ��%
				retract();
				//cout << "(4,\"" << token << "\")" << endl;
			}
			return 4;
			break;
		case '/':
			concatenation();
			character = fin.get();
			if (character == '=') {
				//���̸�ֵ/=
				concatenation();
				//cout << "(4,\"" << token << "\")" << endl;
				return 4;
			}
			else if (character == '/') {
				//����ע��
				character = fin.get();
				while (!fin.eof() && character != '\n') {
					character = fin.get();
				}
				return 0;
			}
			else if (character == '*') {
				//����ע��
				character = fin.get();
				bool finded = false;
				while (!fin.eof()) {
					if (character == '*') {
						finded = true;
					}
					else if (character == '/'&&finded) {
						break;
					}
					else {
						finded = false;
					}
					character = fin.get();
				}
				// /**/��ƥ��
				if (!finded) {
					cout << "/**/��ƥ��" << endl;
					return -1;
				}
				return 0;
				break;
			}
			else {
				//����/
				retract();
				//cout << "(4,\"" << token << "\")" << endl;
				return 4;
			}
			break;
		case '<':
			concatenation();
			character = fin.get();
			if (character == '=') {
				//С�ڵ��ڹ�ϵ<=
				concatenation();
				//cout << "(4,\"" << token << "\")" << endl;
			}
			else if (character == '<') {
				concatenation();
				character = fin.get();
				if (character == '=') {
					//�Ա������Ƹ�ֵ<<=
					concatenation();
					//cout << "(4,\"" << token << "\")" << endl;
				}
				else {
					//��������<<
					retract();
					//cout << "(4,\"" << token << "\")" << endl;
				}
			}
			else {
				//С�ڹ�ϵ<
				retract();
				//cout << "(4,\"" << token << "\")" << endl;
			}
			return 4;
			break;
		case '>':
			concatenation();
			character = fin.get();
			if (character == '=') {
				//���ڵ��ڹ�ϵ>=
				concatenation();
				//cout << "(4,\"" << token << "\")" << endl;
			}
			else if (character == '>') {
				concatenation();
				character = fin.get();
				if (character == '=') {
					//�Ա������Ƹ�ֵ>>=
					concatenation();
					//cout << "(4,\"" << token << "\")" << endl;
				}
				else {
					//��������>>
					retract();
					//cout << "(4,\"" << token << "\")" << endl;
				}
			}
			else {
				//���ڹ�ϵ>
				retract();
				//cout << "(4,\"" << token << "\")" << endl;
			}
			return 4;
			break;
		case '=':
			concatenation();
			character = fin.get();
			if (character == '=') {
				//���ڹ�ϵ==
				concatenation();
				//cout << "(4,\"" << token << "\")" << endl;
			}
			else {
				//ֱ�Ӹ�ֵ=
				retract();
				//cout << "(4,\"" << token << "\")" << endl;
			}
			return 4;
			break;
		case '&':
			concatenation();
			character = fin.get();
			if (character == '&') {
				//�߼�AND &&
				concatenation();
				//cout << "(4,\"" << token << "\")" << endl;
			}
			else if (character == '=') {
				//�Ա���AND��ֵ &=
				concatenation();
				//cout << "(4,\"" << token << "\")" << endl;
			}
			else {
				//����AND/ȡ�����ĵ�ַ &
				retract();
				//cout << "(4,\"" << token << "\")" << endl;
			}
			return 4;
			break;
		case '|':
			concatenation();
			character = fin.get();
			if (character == '|') {
				//�߼�OR ||
				concatenation();
				//cout << "(4,\"" << token << "\")" << endl;
			}
			else if (character == '=') {
				//�Ա���OR��ֵ |=
				concatenation();
				//cout << "(4,\"" << token << "\")" << endl;
			}
			else {
				//����OR |
				retract();
				//cout << "(4,\"" << token << "\")" << endl;
			}
			return 4;
			break;
		case '^':
			concatenation();
			character = fin.get();
			if (character == '=') {
				//�Ա���XOR��ֵ ^=
				concatenation();
				//cout << "(4,\"" << token << "\")" << endl;
			}
			else {
				//����XOR ^
				retract();
				//cout << "(4,\"" << token << "\")" << endl;
			}
			return 4;
			break;
		case '!':
			concatenation();
			character = fin.get();
			if (character == '=') {
				//�����ڹ�ϵ!=
				concatenation();
				//cout << "(4,\"" << token << "\")" << endl;
			}
			else {
				//�߼���!
				retract();
				//cout << "(4,\"" << token << "\")" << endl;
			}
			return 4;
			break;
		case '~':
			//��λȡ��~
			//cout << "(4,\"" << token << "\")" << endl;
			return 4;
			break;
		case '.':
			//�Զ���ʽ���ʳ�Ա
			//cout << "(4,\"" << token << "\")" << endl;
			return 4;
			break;
		case '#':
			concatenation();
			character = fin.get();
			while (!fin.eof() && character != '\n') {
				concatenation();
				character = fin.get();
			}
			//Ԥ����ָ��
			cout << "Ԥ����ָ��" + token << endl;
			return 0;
			break;
		case '[':
		case '(':
		case '{':
			if (!endMatch) {
				match.push(character);
			}
			token = character;
			//cout << "(5,\"" << character << "\")" << endl;
			return 5;
			break;
		case ')':
			if (endMatch) {
				token = character;
				//cout << "(5,\"" << character << "\")" << endl;
				return 5;
			}
			else if (!match.empty() && match.top() == '(') {
				match.pop();
				token = character;
				//cout << "(5,\"" << character << "\")" << endl;
				return 5;
			}
			else {
				cout << "()��ƥ��" << endl;
				jumpEnd = true;
				endMatch = true;
				return -1;
			}
			break;
		case '}':
			if (endMatch) {
				token = character;
				//cout << "(5,\"" << character << "\")" << endl;
				return 5;
			}
			else if (!match.empty() && match.top() == '{') {
				match.pop();
				token = character;
				//cout << "(5,\"" << character << "\")" << endl;
				return 5;
			}
			else {
				cout << "{}��ƥ��" << endl;
				jumpEnd = true;
				endMatch = true;
				return -1;
			}
			break;
		case ']':
			if (endMatch) {
				token = character;
				//cout << "(5,\"" << character << "\")" << endl;
				return 5;
			}
			else if (!match.empty() && match.top() == '[') {
				match.pop();
				token = character;
				//cout << "(5,\"" << character << "\")" << endl;
				return 5;
			}
			else {
				cout << "[]��ƥ��" << endl;
				jumpEnd = true;
				endMatch = true;
				return -1;
			}
			break;
		case '\'':
			//�ҵ�'
			finded = false;
			//ת��
			changed = false;
			concatenation();
			character = fin.get();
			while (!fin.eof()) {
				if (changed) {
					changed = false;
					concatenation();
					character = fin.get();
				}
				else if (character == '\n') {
					cout << "�ַ��в����л���" << endl;
					return -1;
				}
				else if (character == '\'') {
					finded = true;
					concatenation();
					buildlist(3);
					//�ַ�����
					//cout << "(4,\"" << token << "\")" << endl;
					return 4;
					break;
				}
				else if (character == '\\') {
					changed = true;
					concatenation();
					character = fin.get();
					if (!fin.eof() && character == '\r') {
						character = fin.get();
						if (!fin.eof() && character == '\n') {
							token = token.substr(0, token.length() - 1);
							changed = false;
							character = fin.get();
						}
						else {
							retract();
							retract();
						}
					}
				}
				else {
					concatenation();
					character = fin.get();
				}

			}
			if (!finded) {
				cout << "''��ƥ��" << endl;
				return -1;
			}
			break;
		case '"':
			//�ҵ�'
			finded = false;
			//ת��
			changed = false;
			concatenation();
			character = fin.get();
			while (!fin.eof()) {
				if (changed) {
					changed = false;
					concatenation();
					character = fin.get();
				}
				else if (character == '\n') {
					cout << "�ַ����в����л���" << endl;
					return -1;
				}
				else if (character == '"') {
					finded = true;
					concatenation();
					buildlist(3);
					//�ַ�������
					//cout << "(4,\"" << token << "\")" << endl;
					return 4;
					break;
				}
				else if (character == '\\') {
					changed = true;
					concatenation();
					character = fin.get();
					if (!fin.eof() && character == '\r') {
						character = fin.get();
						if (!fin.eof() && character == '\n') {
							token = token.substr(0, token.length() - 1);
							changed = false;
							character = fin.get();
						}
						else {
							retract();
							retract();
						}
					}
				}
				else {
					concatenation();
					character = fin.get();
				}

			}
			if (!finded) {
				cout << "\"\"��ƥ��" << endl;
				return -1;
			}
			break;
		case ':':
			concatenation();
			character = fin.get();
			if (character == ':') {
				//���������::
				//cout << "(4,\"" << token << "\")" << endl;
				return 4;
			}
			else {
				retract();
				// :
				//cout << "(4,\"" << token << "\")" << endl;
				return 4;
			}
		case ',':
		case ';':
		case '`':
			token = character;
			//cout << "(5,\"" << character << "\")" << endl;
			return 5;
			break;
		default:
			error();
			return -1;
			break;
		}
	}
	return -1;
}
int Analysis::getNext()
{
	int id;
	if (fin.peek() == EOF) {
		token = '#';
		return -2;
	}
	while (fin.peek() != EOF)
	{
		if (jumpEnd) {
			break;
		}
		id = analysis();
		if (id > 0)return id;
	}
	if (id == -2) {
		token = '#';
		return -2;
	}
	if (!match.empty()) {
		cout << "���Ų�ƥ��" << endl;
		return -1;
	}
	return -1;
}
/*
#include<iostream>
#include"analysis.h"
using namespace std;
int main()
{
	Analysis r;
	ifstream &fin = r.fin;
	fin.open("test.txt", ios::binary | ios::in);
	if (!fin)
	{
		cout << "�ļ���ʧ��" << endl;
	}
	else
	{
		while (1) {
			int id = r.getNext();
			if (id == -2) {
				cout << "(-2,\"#\")" << endl;
				break;
			}
			if (id > 0)printf("(%d,\"%s\")\n", id, r.token.c_str());
		}
	}
	return 0;
}
*/