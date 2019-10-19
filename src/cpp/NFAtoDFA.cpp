#include "NFAtoDFA.h"

//FAstate Ŭ���� ����
FAstate::FAstate(string State, vector<string> NextStates) {	//���� ����ϴ� ������
	this->State = State;
	this->NextStates = NextStates;
}
FAstate::FAstate(string State, vector<string> NextStates, string EmptyOutput) {	//Ŭ���� ���� ���� ����ϴ� ������
	this->State = State;
	this->NextStates = NextStates;
	this->EmptyOutput = EmptyOutput;
}
void FAstate::registState(string State, vector<string> NextStates) {	//���� ���
	this->State = State;
	this->NextStates = NextStates;
}
void FAstate::showState() {	//���� ����Ʈ
	cout << State << "\t||";
	for (int i = 0; i < (int)NextStates.size(); i++)
	{
		
		cout << NextStates[i] << "\t";
		if (i != (int)NextStates.size() - 1) cout << "|";
	}
}
void FAstate::showStateComma() {	//���� ����Ʈ �޸� ����
	cout << State << "\t||";
	vector<string>::iterator it;
	for (it = NextStates.begin(); it < NextStates.end(); it++) {
		for (int i = 0; i < (int)it->length(); i++) {
			cout << (*it)[i];
			if (i != (int)it->length() - 1)	cout << ",";
		}
		cout << "\t|";
	}
	for (int i = 0; i < (int)EmptyOutput.length(); i++) {
		cout << EmptyOutput[i];
		if (i != (int)EmptyOutput.length() - 1)	cout << ",";
	}
}
string FAstate::getEmptyOutput() { return EmptyOutput; }
void FAstate::setEmptyOutput(string EmptyOutput) { this->EmptyOutput = EmptyOutput; }






//FAtable Ŭ���� ����
FAtable FAtable::convertDFA() {			//���� ���̺��� DFA�� �ٲٴ��۾�
	FAtable DFAtable;
	vector<string> SymList;				//DFA�� �ɺ� ����Ʈ�� Column
	string curSym;				//���� State��� ����
	FAstate* tempState;		//���� �ɺ��� ���� FAstate ��ü�� ��� ������ ����

	string StartSym = "A";
	int Driver = 0;				//vector �� ���� �ε���

	if (searchSym(StartSym)->getEmptyOutput() == "") {
		SymList.push_back(StartSym);		//StartSym�� Empty���°���� ������ ��Ÿ���� StartSym����
	}
	else {
		SymList.push_back( Closure( *searchSym(StartSym) ) );
	}
	
	do {
		vector<string> NextSym(inputLen, "");		//�Է½ɺ��� ���� ���ϴ� ���� ���µ� Row
		curSym = SymList[Driver];

		for (int L1index = 0; L1index < (int)curSym.length(); L1index++) {
			string curstr(1, curSym[L1index]);

			tempState = searchSym(curstr);		//���� �ɺ��� ���� FAstate ��ü�� �̾Ƴ�

			for (int L2index = 0; L2index < (int)NextSym.size(); L2index++) {
				string curListAtom = BubbleSort(NextSym[L2index]);
				string newSymAtom;

				if (tempState->getNextStateLen() <= L2index) {		//NextState�� �ִ�ũ�⸦ �Ѿ�� for�� ����
					break;
				}
				else {
					newSymAtom = BubbleSort(tempState->getNextStates()[L2index]);
					for (int L3index = 0; L3index < (int)newSymAtom.length(); L3index++) {
						string temp(1, newSymAtom[L3index]);
						newSymAtom = BubbleSort(strMerge_neo(newSymAtom, Closure(*searchSym(temp) ) ) );
					}
				}

				NextSym[L2index] = BubbleSort(strMerge_neo(curListAtom, newSymAtom));
				//���� NextSym�鿡 ���ο� NextStates���� �����Ѵ�.(�����ϱ� ������ ��Ʈ) �׸��� �� ��Ʈ�Ѵ�.
			}

		}
		for (int L2index = 0; L2index < (int)NextSym.size(); L2index++) {
			bool overlapFlag = true;											//�ߺ��Ǵ��� üũ�÷���
			for (int L3index = 0; L3index < (int)SymList.size(); L3index++) {
				if (SymList[L3index] == NextSym[L2index] || NextSym[L2index] == "") {
					overlapFlag = false;
					break;
				}
			}
			if (overlapFlag) {										//��ġ�� ������ SymList�� �߰� (DFA���µ� ����Ʈ)
				SymList.push_back(NextSym[L2index]);
			}
		}

		DFAtable.addState(curSym, NextSym);
		Driver++;

		
		
	} while ((int)SymList.size() > Driver);			//���� ���̻� ã�ƺ��� ������ ������.

	return DFAtable;
}
string FAtable::Closure(FAstate target) {		//Closure �Լ�
	string result = target.getState();

	if (target.getState() != target.getEmptyOutput()) {							//Emptyoutput�� �ڱ� �ڽ��̸� �ѱ��.
		for (int i = 0; i < (int)target.getEmptyOutput().length(); i++) {								//�׷��� ������ Emptyoutput�� ������ ���Ҹ� ���� Closure�ؼ� result�� merge�س�����.
			string temp(1, target.getEmptyOutput()[i]);
			result = BubbleSort(strMerge_neo(BubbleSort(result), BubbleSort(Closure(*searchSym(temp)))));
		}
	}
	return result;
}

FAstate* FAtable::searchSym(string Key) {
	for (int i = 0; i < Length; i++) {
		if ((States[i])->getState() == Key)
			return States[i];
	}
	return NULL;
}
FAtable* FAtable::addState(string State, vector<string> NextStates) {		//���� �߰�
	if ((int)NextStates.size() > inputLen)
		inputLen = (int)NextStates.size();

	States.push_back(new FAstate(State, NextStates));
	Length++;

	return this;
}
FAtable* FAtable::addState(string State, vector<string> NextStates, string EmptyOutput) {		//���� �߰�, Ŭ���� ����
	if ((int)NextStates.size() > inputLen)
		inputLen = (int)NextStates.size();

	States.push_back(new FAstate(State, NextStates, EmptyOutput));
	Length++;

	return this;
}
void FAtable::putTableWithUI() {
	char StateName;			//���� �̸�
	string inputTemp;			//�Է� �ӽ÷� ��Ƶδ� ����
	int StateSymNum;
	int InputSymNum;
	bool checkFlag = true;

	while (checkFlag) {
		cout << "State�ɺ��� �ִ밳���� �Է� :";
		cin >> StateSymNum;
		if (StateSymNum == 0) { cout << "����ε� ���� �Է����ּ���" << endl; checkFlag = true; continue; }
		else { checkFlag = false; }

		cout << "input�ɺ��� �ִ밳���� �Է� (��ǲ�ɺ��� ���ڷ� �ڵ�����) :";
		cin >> InputSymNum;
		if (InputSymNum == 0) { cout << "����ε� ���� �Է����ּ���" << endl; checkFlag = true; continue; }
		else { checkFlag = false; }

		cout << "\n\n";
		cout << "���̺��� �Է����ּ��� (������ '^'�� �Է��մϴ�) " << endl;
		cout << "(State�ɺ��� �ѱ��ڸ� ���ּ���) " << endl;
		cout << "(Start�ɺ��� 'S'�Դϴ�) " << endl;
		cout << "\n\n";
		for (int i = 0; i < StateSymNum; i++) {
			vector<string> NextSym;		//�Է½ɺ� ���
			cout << "Symbol :";
			cin >> StateName;

			string SN(1, StateName);

			for (int j = 0; j < InputSymNum; j++) {
				cout << j << "�� ���� ����� :";
				cin >> inputTemp;
				inputTemp.erase(remove_if(inputTemp.begin(), inputTemp.end(), &IsStrRight), inputTemp.end());
				NextSym.push_back(inputTemp);
			}
			cout << "���� ���� ����� :";
			cin >> inputTemp;
			inputTemp.erase(remove_if(inputTemp.begin(), inputTemp.end(), &IsStrRight), inputTemp.end());

			addState(SN, NextSym, inputTemp);
			cout << endl;
		}


	}
	
}

void FAtable::showAllStates() {			//���̺� ��ü ���
	cout << " " << "\t||";
	if (Length != 0) {
		for (int i = 0; i < inputLen; i++) {
			cout << i << " \t";
			if (i != inputLen - 1) cout << "|";
		}
		cout << endl;
	}
	for (int i = 0; i < inputLen; i++) {
		cout << "-----------";
	}
	cout << endl;
	vector<FAstate*>::iterator it;
	for (it = States.begin(); it < States.end(); it++) {
		(*it)->showState();
		cout << endl;
	}
}
void FAtable::showAllStatesComma() {			//�޸� ���� ���̺� ��ü ���

	cout << " " << "\t||";
	if (Length != 0) {
		for (int i = 0; i < inputLen; i++) {
			cout << i << " \t|";
		}
	}
	cout << "��";
	cout << endl;
	for (int i = 0; i < (inputLen + 1); i++) {
		cout << "----------";
	}
	cout << endl;

	vector<FAstate*>::iterator it;
	for (it = States.begin(); it < States.end(); it++) {
		(*it)->showStateComma();
		cout << endl;
	}
}





//�����Լ�
string strMerge(string str1, string str2) {			//���� //������ ������ ����� �ȵǰ� ��Ҿȿ� ���Ե��־ ��� �߰��Ǽ� ����
	int str1len = str1.length();
	int str2len = str2.length();
	int index;
	int endflag;

	index = 0;
	while (true) {
		if (str1[index] != str2[index]) {
			endflag = 0;
			break;
		}

		index++;
		if (index >= str2len) {
			endflag = 1;
			break;
		}
		if (index >= str1len) {
			endflag = 2;
			break;
		}
		
	}
	switch (endflag) {
	case 0:
		str1.append(str2.substr(index, str2len - index));
	case 1:
		return str1;
	case 2:
		return str2;
	}

}

string strMerge_neo(string str1, string str2) {			//���� ������
	int str1len = str1.length();
	int str2len = str2.length();
	int index;

	if (str1.find(str2) != string::npos) { return str1;	}
	else if (str2.find(str1) != string::npos) { return str2; }

	index = 0;
	while (true) {
		if (str1[index] != str2[index]) {	break;	}
		index++;

	}
	str1.append(str2.substr(index, str2len - index));
	return str1;

}

string BubbleSort(string target) {		//�����Ʈ
	char temp;
	int targetLen = target.length();
	string result;

	for (int i = 0; i < targetLen - 1; i++) {
		for (int j = 0; j < targetLen - 1 - i; j++) {
			if ((int)target[j] > (int)target[j + 1]) {
				temp = target[j + 1];
				target[j + 1] = target[j];
				target[j] = temp;
			}
		}
	}

	result = target;
	return result;
}

bool IsStrRight(char c)
{
	if (ispunct(c) || isspace(c)) return true;
	else return false;
}