#include <iostream>
#include <vector>
#include <unordered_map>
#include <string>
#include <algorithm>
#include <cctype>
#include <assert.h>

#pragma GCC optimize(3)
#pragma GCC optimize("unroll-loops")


class SufAutomata {
public:

	SufAutomata(const std::string& text = std::string()) {
		NewState();
		link.at(0) = -1;
		nWays.at(0) = 1;

		AddText(text);
	}


	void AddText(const std::string& text) {
		for (int i = 0; i < text.size(); ++i) {
			AddChar(text.at(i));
		}
	}


	bool SubstrExists(const std::string& str) {
		int curState = 0;
		for (int i = 0; i < str.size(); ++i) {
			if (-1 == edges.at(curState).at(str[i])) {
				return false;
			}

			curState = edges.at(curState).at(str[i]);
		}

		return true;
	}


private:

	int NewState() {
		link.push_back(0);
		len.push_back(0);
		nWays.push_back(0);
		edges.push_back(std::vector<int>(CHAR_MAX + 1, -1));

		return nStates++;
	}


	int CloneState(int state) {
		int newState = NewState();
		edges.at(newState) = edges.at(state);
		link.at(newState) = link.at(state);

		return newState;
	}


	void AddChar(char ch) {
		int newState = NewState();
		len.at(newState) = len.at(last) + 1;

		int prevState = last;
		while (prevState >= 0 && -1 == edges.at(prevState).at(ch)) {
			edges.at(prevState).at(ch) = newState;
			nWays.at(newState) += nWays.at(prevState);
			sumWays += nWays.at(prevState);
			prevState = link.at(prevState);
		}

		if (prevState != -1) {
			int chState = edges.at(prevState).at(ch);

			if (len.at(prevState) + 1 == len.at(chState)) {
				link.at(newState) = chState;

			} else {
				int clonedState = CloneState(chState);
				len.at(clonedState) = len.at(prevState) + 1;
				link.at(newState) = clonedState;
				link.at(chState) = clonedState;

				while (prevState >= 0 && edges.at(prevState).at(ch) == chState) {
					edges.at(prevState).at(ch) = clonedState;
					nWays.at(clonedState) += nWays.at(prevState);
					nWays.at(chState) -= nWays.at(prevState);
					prevState = link.at(prevState);
				}
			}
		}
		last = newState;
	}


	int last = 0;
	std::vector<std::vector<int>> edges;
	std::vector<int> link, len, nWays;
	int nStates = 0;
	long long sumWays = 0;
};


int main() {
	std::cin.tie(0);
	std::ios_base::sync_with_stdio(false);

	SufAutomata sufAutomata;

	char curCommand = 0;
	std::cin >> curCommand;
	while (!std::cin.eof()) {
		std::string str;
		std::cin >> str;
		std::transform(str.begin(), str.end(), str.begin(), std::tolower);

		switch (curCommand) {
		case '?':
			std::cout << (sufAutomata.SubstrExists(str) ? "YES" : "NO") << '\n';
			break;

		case 'A':
			sufAutomata.AddText(str);
			break;

		default:
			assert(false);
		}

		std::cin >> curCommand;
	}

	return 0;
}