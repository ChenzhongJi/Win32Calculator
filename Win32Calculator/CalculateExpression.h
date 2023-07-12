#pragma once
#include<iostream>
#include<algorithm>
#include<stack>
#include<stdlib.h>
#include<vector>
#include<string>
using namespace std;
string format(string str) {
	for (int i = 0; i < str.length(); i++) {
		if (str[i] == '-') {
			if (i == 0) {
				str.insert(0, 1, '0');
			}
			else if (str[i - 1] == '(') {
				str.insert(i, 1, '0');
			}
		}

	}
	return str;
}
int prior(char c) {
	if (c == '+' || c == '-') {
		return 1;
	}
	else if (c == '*' || c == '/' || c == '%') {
		return 2;
	}
	else {
		return 0;
	}

}
vector<string> hz(string str) {
	vector<string> vs;
	stack<char> st;
	for (int i = 0; i < str.length(); i++) {
		string tmp = "";
		switch (str[i]) {
		case '+':
		case '-':
		case '*':
		case '/':
		case '%':
			if (st.empty() || st.top() == '(') {
				st.push(str[i]);
			}
			else {
				while (!st.empty() && prior(st.top()) >= prior(str[i])) {
					tmp += st.top();
					vs.push_back(tmp);
					st.pop();
					tmp = "";
				}
				st.push(str[i]);
			}
			break;
		case '(':
			st.push(str[i]);
			break;
		case ')':
			while (st.top() != '(') {
				tmp += st.top();
				vs.push_back(tmp);
				st.pop();
				tmp = "";
			}
			st.pop();
			break;
		default:
			if ((str[i] >= '0' && str[i] <= '9')) {
				tmp += str[i];
				while (i + 1 < str.size() && str[i + 1] >= '0' && str[i + 1] <= '9' || str[i + 1] == '.')
				{
					tmp += str[i + 1];//若是连续数字
					++i;
				}
				vs.push_back(tmp);
			}
		}
	}
	while (!st.empty()) {
		string tmp = "";
		tmp += st.top();
		vs.push_back(tmp);
		st.pop();
	}
	return vs;
}
double result(vector<string> bh) {
	stack<double> st;
	double num, op1, op2;
	for (int i = 0; i < bh.size(); i++) {
		string tmp = bh[i];
		if (tmp[0] >= '0' && tmp[0] <= '9') {
			num = atof(tmp.c_str());
			st.push(num);
		}
		else if (bh[i] == "+")
		{
			op2 = st.top();
			st.pop();
			op1 = st.top();
			st.pop();
			st.push(op1 + op2);
		}
		else if (bh[i] == "-")
		{
			op2 = st.top();
			st.pop();
			op1 = st.top();
			st.pop();
			st.push(op1 - op2);
		}
		else if (bh[i] == "*")
		{
			op2 = st.top();
			st.pop();
			op1 = st.top();
			st.pop();
			st.push(op1 * op2);
		}
		else if (bh[i] == "/")
		{
			op2 = st.top();
			st.pop();
			op1 = st.top();
			st.pop();
			st.push(op1 / op2);
		}
		else if (bh[i] == "%")
		{
			op2 = st.top();
			st.pop();
			op1 = st.top();
			st.pop();
			if (int(op1) != op1 || int(op2) != op2 || op2 == 0)
			{
				throw "An Error Exists in REM!";
			}
			st.push((int)op1 % (int)op2);
		}
	}
	return st.top();
}

string removeLastZero(string str)
{
	string numstr = str;
	if (NULL == numstr.find('.'))
		return numstr;

	int length = numstr.length();
	for (int i = length - 1; i > 0; --i)
	{
		if ('\0' == numstr[i])
		{
			continue;
		}
		else if ('0' == numstr[i])
		{
			numstr[i] = '\0';
		}
		else if ('.' == numstr[i])// 小数点之后全为零
		{
			numstr[i] = '\0';
			break;
		}
		else// 小数点后有非零数字
		{
			break;
		}
	}
	return numstr;
}

string solve(string str) {
	str = format(str);
	vector<string> bh = hz(str);
	double k = 0;
	try
	{
		k = result(bh);
	}
	catch (const char* msg)
	{
		return msg;
	}
	if (int(k) == k)
	{
		return to_string((int(k)));
	}
	return removeLastZero(to_string(k));
}

