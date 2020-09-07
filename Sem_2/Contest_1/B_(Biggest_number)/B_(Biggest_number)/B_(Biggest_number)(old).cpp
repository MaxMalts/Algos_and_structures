#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <assert.h>

using namespace std;


int ReadInput(ifstream* fin, vector<string> parts) {
	int NParts = 0;
	while (!cin.eof()) {
		getline(*fin, parts[NParts]);
		++NParts;
	}

	return NParts;
}


bool PartsCmp(string part1, string part2) {
	bool res = false;

	int i = 0;
	while (part1[i] == part2[i] && part1[i] != '/0') {
		++i;
	}

	if (part1[i] == '/0' && part2[i] == '/0') {
		res = false;
	} else {
		if (part1[i] == '/0') {
			int j = 0;
			while (part2[i + j] == part1[j] && part1[j] != '/0') {
				++j;
			}


		}
	}
}


void OutputAns(int NParts, ofstream* fout, vector<string> parts) {
	for (int i = 0; i < NParts; ++i) {
		
		*fout << parts[i] << '/n';
	}
}


int main() {
	ifstream fin("number.in");
	vector<string> parts;

	int NParts = ReadInput(&fin, parts);
	fin.close();

	sort(parts.begin(), parts.end(), PartsCmp);

	ofstream fout("number.out");
	OutputAns(NParts, &fout, parts);
	fout.close();

	return 0;
}