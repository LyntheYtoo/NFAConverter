#include "NFAtoDFA.h"

//FAstate 클래스 구현
FAstate::FAstate(string State, vector<string> NextStates) {	//상태 등록하는 생성자
	this->State = State;
	this->NextStates = NextStates;
}
FAstate::FAstate(string State, vector<string> NextStates, string EmptyOutput) {	//클로저 포함 상태 등록하는 생성자
	this->State = State;
	this->NextStates = NextStates;
	this->EmptyOutput = EmptyOutput;
}
void FAstate::registState(string State, vector<string> NextStates) {	//상태 등록
	this->State = State;
	this->NextStates = NextStates;
}
void FAstate::showState() {	//상태 프린트
	cout << State << "\t||";
	for (int i = 0; i < (int)NextStates.size(); i++)
	{
		
		cout << NextStates[i] << "\t";
		if (i != (int)NextStates.size() - 1) cout << "|";
	}
}
void FAstate::showStateComma() {	//상태 프린트 콤마 포함
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






//FAtable 클래스 구현
FAtable FAtable::convertDFA() {			//현재 테이블을 DFA로 바꾸는작업
	FAtable DFAtable;
	vector<string> SymList;				//DFA의 심볼 리스트들 Column
	string curSym;				//현재 State담는 변수
	FAstate* tempState;		//현재 심볼에 대한 FAstate 객체를 담는 포인터 변수

	string StartSym = "A";
	int Driver = 0;				//vector 를 도는 인덱스

	if (searchSym(StartSym)->getEmptyOutput() == "") {
		SymList.push_back(StartSym);		//StartSym이 Empty상태결과가 없으면 스타팅은 StartSym부터
	}
	else {
		SymList.push_back( Closure( *searchSym(StartSym) ) );
	}
	
	do {
		vector<string> NextSym(inputLen, "");		//입력심볼에 의해 변하는 다음 상태들 Row
		curSym = SymList[Driver];

		for (int L1index = 0; L1index < (int)curSym.length(); L1index++) {
			string curstr(1, curSym[L1index]);

			tempState = searchSym(curstr);		//현재 심볼에 대한 FAstate 객체를 뽑아냄

			for (int L2index = 0; L2index < (int)NextSym.size(); L2index++) {
				string curListAtom = BubbleSort(NextSym[L2index]);
				string newSymAtom;

				if (tempState->getNextStateLen() <= L2index) {		//NextState의 최대크기를 넘어가면 for문 종료
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
				//현재 NextSym들에 새로운 NextStates들을 머지한다.(머지하기 전에도 소트) 그리고 또 소트한다.
			}

		}
		for (int L2index = 0; L2index < (int)NextSym.size(); L2index++) {
			bool overlapFlag = true;											//중복되는지 체크플레그
			for (int L3index = 0; L3index < (int)SymList.size(); L3index++) {
				if (SymList[L3index] == NextSym[L2index] || NextSym[L2index] == "") {
					overlapFlag = false;
					break;
				}
			}
			if (overlapFlag) {										//겹치지 않으면 SymList에 추가 (DFA상태들 리스트)
				SymList.push_back(NextSym[L2index]);
			}
		}

		DFAtable.addState(curSym, NextSym);
		Driver++;

		
		
	} while ((int)SymList.size() > Driver);			//만약 더이상 찾아볼게 없으면 끝낸다.

	return DFAtable;
}
string FAtable::Closure(FAstate target) {		//Closure 함수
	string result = target.getState();

	if (target.getState() != target.getEmptyOutput()) {							//Emptyoutput이 자기 자신이면 넘긴다.
		for (int i = 0; i < (int)target.getEmptyOutput().length(); i++) {								//그렇지 않으면 Emptyoutput의 각각의 원소를 전부 Closure해서 result와 merge해나간다.
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
FAtable* FAtable::addState(string State, vector<string> NextStates) {		//상태 추가
	if ((int)NextStates.size() > inputLen)
		inputLen = (int)NextStates.size();

	States.push_back(new FAstate(State, NextStates));
	Length++;

	return this;
}
FAtable* FAtable::addState(string State, vector<string> NextStates, string EmptyOutput) {		//상태 추가, 클로저 포함
	if ((int)NextStates.size() > inputLen)
		inputLen = (int)NextStates.size();

	States.push_back(new FAstate(State, NextStates, EmptyOutput));
	Length++;

	return this;
}
void FAtable::putTableWithUI() {
	char StateName;			//상태 이름
	string inputTemp;			//입력 임시로 담아두는 변수
	int StateSymNum;
	int InputSymNum;
	bool checkFlag = true;

	while (checkFlag) {
		cout << "State심볼의 최대개수를 입력 :";
		cin >> StateSymNum;
		if (StateSymNum == 0) { cout << "제대로된 값을 입력해주세요" << endl; checkFlag = true; continue; }
		else { checkFlag = false; }

		cout << "input심볼의 최대개수를 입력 (인풋심볼은 숫자로 자동설정) :";
		cin >> InputSymNum;
		if (InputSymNum == 0) { cout << "제대로된 값을 입력해주세요" << endl; checkFlag = true; continue; }
		else { checkFlag = false; }

		cout << "\n\n";
		cout << "테이블을 입력해주세요 (공백은 '^'로 입력합니다) " << endl;
		cout << "(State심볼은 한글자만 써주세요) " << endl;
		cout << "(Start심볼은 'S'입니다) " << endl;
		cout << "\n\n";
		for (int i = 0; i < StateSymNum; i++) {
			vector<string> NextSym;		//입력심볼 결과
			cout << "Symbol :";
			cin >> StateName;

			string SN(1, StateName);

			for (int j = 0; j < InputSymNum; j++) {
				cout << j << "의 의한 결과값 :";
				cin >> inputTemp;
				inputTemp.erase(remove_if(inputTemp.begin(), inputTemp.end(), &IsStrRight), inputTemp.end());
				NextSym.push_back(inputTemp);
			}
			cout << "ε의 의한 결과값 :";
			cin >> inputTemp;
			inputTemp.erase(remove_if(inputTemp.begin(), inputTemp.end(), &IsStrRight), inputTemp.end());

			addState(SN, NextSym, inputTemp);
			cout << endl;
		}


	}
	
}

void FAtable::showAllStates() {			//테이블 전체 출력
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
void FAtable::showAllStatesComma() {			//콤마 포함 테이블 전체 출력

	cout << " " << "\t||";
	if (Length != 0) {
		for (int i = 0; i < inputLen; i++) {
			cout << i << " \t|";
		}
	}
	cout << "ε";
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





//전역함수
string strMerge(string str1, string str2) {			//머지 //문제점 머지가 제대로 안되고 요소안에 포함되있어도 계속 추가되서 나옴
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

string strMerge_neo(string str1, string str2) {			//머지 버전업
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

string BubbleSort(string target) {		//버블소트
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