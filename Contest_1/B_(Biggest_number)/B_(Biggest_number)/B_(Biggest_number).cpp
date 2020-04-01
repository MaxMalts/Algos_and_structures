#include <stdio.h>
#include <list>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <string>


using namespace std;

int StrCmp(string str1, string str2) {
	int lenSum = str1.length() + str2.length();
	string cnct1 = str1 + str2;
	string cnct2 = str2 + str1;

	for (int i = 0; i < lenSum; ++i) {
		if (cnct1[i] != cnct2[i]) {
			return cnct2[i] - cnct1[i];
		}
	}

	return 0;
}


void AddPart(list<string>& parts, string buf) {
	list<string>::iterator cur = parts.begin();

	while ((cur != parts.end()) && (StrCmp(*cur, buf) < 0)) {
		cur++;
	}
	parts.insert(cur, buf);
}


list<string> ParseParts(ifstream* fin) {
	list<string> parts;

	string curPart = "";
	getline(*fin, curPart);
	parts.push_back(curPart);
	while (getline(*fin, curPart)) {
		AddPart(parts, curPart);
	}

	return parts;
}


void OutputAnswer(ofstream* fout, list<string> parts) {
	list<string>::iterator iter = parts.begin();
	for (iter; iter != parts.end(); ++iter) {
		*fout << *iter;
	}
}


int main() {
	ifstream fin("number.in");
	list<string> parts = ParseParts(&fin);
	fin.close();

	ofstream fout("number.out");
	OutputAnswer(&fout, parts);
	fout.close();

	return 0;
}