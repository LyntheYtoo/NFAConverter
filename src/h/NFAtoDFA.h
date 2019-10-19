#pragma once
#include <iostream>
#include <string>
#include <algorithm>
#include <vector>

using namespace std;



class FAstate {
private:
	string State;
	vector<string> NextStates;
	string EmptyOutput = "";		//emptyÀÎÇ²ÀÏ¶§ÀÇ ¾Æ¿ôÇ²
public:
	FAstate(string State, vector<string> NextStates);
	FAstate(string State, vector<string> NextStates, string EmptyOutput);
	vector<string> getNextStates() { return NextStates; }
	string getState() { return State; }
	void registState(string State, vector<string> NextStates);

	void showState();
	void showStateComma();
	int getNextStateLen() { return (int)NextStates.size(); }
	string getEmptyOutput();
	void setEmptyOutput(string EmptyOutput);
};


class FAtable {
private:
	vector<FAstate*> States;
	int Length = 0;
	int inputLen = 0;

public:
	FAtable convertDFA();
	FAstate* searchSym(string Key);
	string Closure(FAstate target);
	FAtable* addState(string State, vector<string> NextStates);
	FAtable* addState(string State, vector<string> NextStates, string EmptyOutput);
	void putTableWithUI();
	void showAllStates();
	void showAllStatesComma();
	vector<FAstate*> getStates() { return States; }
	int getLength() { return Length; }
};





//Àü¿ªÇÔ¼ö
string strMerge(string str1, string str2);
string strMerge_neo(string str1, string str2);
string BubbleSort(string target);
bool IsStrRight(char c);