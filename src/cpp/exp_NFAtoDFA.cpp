#include "NFAtoDFA.h"

int main() {
	FAtable NFAtable;
	/*
	vector<string> Astate;
	vector<string> Bstate;
	vector<string> Cstate;
	vector<string> Dstate;
	vector<string> Fstate;

	Astate.push_back("BC"); Astate.push_back("BD");
	Bstate.push_back("BC"); Bstate.push_back("BD");
	Cstate.push_back("F"); Cstate.push_back("");
	Dstate.push_back(""); Dstate.push_back("F");
	Fstate.push_back("F"); Fstate.push_back("F");

	NFAtable.addState("A", Astate);
	NFAtable.addState("B", Bstate);
	NFAtable.addState("C", Cstate);
	NFAtable.addState("D", Dstate);
	NFAtable.addState("F", Fstate);
	*/
	
	vector<string> Astate;
	vector<string> Bstate;
	vector<string> Cstate;

	Astate.push_back(""); Astate.push_back("B"); Astate.push_back("C");
	Bstate.push_back("A"); Bstate.push_back("C"); Bstate.push_back("AB");
	Cstate.push_back(""); Cstate.push_back(""); Cstate.push_back("");

	NFAtable.addState("A", Astate, "BC")->addState("B", Bstate)->addState("C", Cstate);
	
	//NFAtable.putTableWithUI();

	NFAtable.showAllStatesComma();

	cout << endl;
	cout << endl;

	FAtable DFAtable = NFAtable.convertDFA();
	DFAtable.showAllStates();

}