#include <iostream>
#include <vector>
#include <unordered_map>
#include <string>

#pragma GCC optimize(3)
#pragma GCC optimize("unroll-loops")


class SufAutomata {
public:

	SufAutomata(std::string& str) {
		NewState();
		link.at(0) = -1;
		nWays.at(0) = 1;

		for (int i = 0; i < str.size(); ++i) {
			AddChar(str.at(i));

			std::cout << sumWays << '\n';
		}
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

	std::string str;
	std::cin >> str;

	SufAutomata sufAutomata(str);

	return 0;
}