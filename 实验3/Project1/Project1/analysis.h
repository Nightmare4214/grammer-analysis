#pragma once
#include<iostream>
#include<stack>
#include<set>
#include<fstream>
#include<string>
using namespace std;
class Analysis {
public:
	//1���������֡�2��ʶ����3������4�������5�ָ���
	set<string> keyword;
	//��ʶ��
	set<string> identifier;
	//������
	set<string> constant;
	//��������Ƿ�ƥ��
	stack<char> match;
	char character;
	string token;
	//�Ƿ���ֹ����;
	bool jumpEnd;
	//�Ƿ���ֹƥ������
	bool endMatch;
	ifstream fin;
	Analysis();
	//��character�е��ַ�Ϊ�գ���������ַ���ֱ���ǿ�
	void getbe();
	//��token���ַ�����character�е��ַ�������Ϊtoken�е����ַ���
	void concatenation();
	//�ж�character�е��ַ��Ƿ�Ϊ��ĸ
	bool letter();
	//�ж�character�е��ַ��Ƿ�Ϊ����
	bool digit();
	//�ж�token�е��ַ����Ƿ��ڱ����ֱ���
	bool reserve();
	//ɨ��ָ�����һ���ַ���ͬʱ��character��Ϊ�հ�
	void retract();
	//����ʶ���Ǽǵ����ű���(id=2),�������Ǽǵ�������(id=3)
	void buildlist(int id);
	//������
	void error();
	bool isEnd();
	bool isOp(char getOp);
	//�жϿ�ѧ�����������أ�3�������֣�4�����������-1�������
	int isUnsignNum();
	//1���������֡�2��ʶ����3������4�������5�ָ���,-1�������,0����ʶ����ע�ͻ���Ԥ����ָ��,-2�����ļ�β
	//�ʷ�����
	int analysis();
	int getNext();
};