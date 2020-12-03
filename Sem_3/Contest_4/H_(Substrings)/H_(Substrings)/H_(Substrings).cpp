#include <iostream>
#include <vector>
#include <unordered_map>
#include <set>
#include <string>
#include <algorithm>
#include <cctype>
#include <assert.h>

#pragma GCC optimize(3)
#pragma GCC optimize("unroll-loops")


class SufAutomata {
public:

	SufAutomata() {
		NewState();
		link.at(0) = -1;
		nWays.at(0) = 1;
	}


	//bool SubstrExists(const std::string& str) {
	//	int curState = 0;
	//	for (int i = 0; i < str.size(); ++i) {
	//		if (-1 == edges.at(curState).at(str[i])) {
	//			return false;
	//		}

	//		curState = edges.at(curState).at(str[i]);
	//	}

	//	return true;
	//}


	void AddWord(std::string word) {
		word.append(1, nWords++);

		AddText(word);
	}


	std::string LongestCommonSubstr() {
		int curSpecial = 0;
		std::vector<bool> visited(edges.size(), false);
		std::vector<bool> candidates(edges.size());
		Attainability_DFS(0, curSpecial, visited, candidates);

		std::string curStr, res;
		Candidate_DFS(0, candidates, curStr, res);

		return res;
	}


private:

	void AddText(const std::string& text) {
		for (int i = 0; i < text.size(); ++i) {
			AddChar(text.at(i));
		}
	}


	int NewState() {
		link.push_back(0);
		len.push_back(0);
		nWays.push_back(0);
		attainability.push_back(std::set<int>());
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


	void Attainability_DFS(int curState, int& curSpecial, std::vector<bool>& visited, std::vector<bool>& candidates) {

		for (int i = 0; i < edges.at(curState).size(); ++i) {
			int nextState = edges.at(curState).at(i);
			if (nextState != -1) {
				if (visited.at(nextState)) {
					if (i < nWords) {
						curSpecial = i;
						attainability.at(curState).insert(i);

					} else {
						attainability.at(curState).insert(attainability.at(nextState).begin(), 
						                                  attainability.at(nextState).end());
					}

				} else {
					Attainability_DFS(nextState, curSpecial, visited, candidates);
					if (i < nWords) {
						curSpecial = i;
						attainability.at(curState).insert(i);

					} else {
						attainability.at(curState).insert(attainability.at(nextState).begin(), 
						                                  attainability.at(nextState).end());
					}
				}
			}
		}
		visited.at(curState) = true;

		if (nWords == attainability.at(curState).size()) {
			candidates.at(curState) = true;
		}
	}


	void Candidate_DFS(int curState, std::vector<bool>& candidates, std::string& curStr, std::string& res) {
		if (candidates.at(curState) && curStr.size() > res.size()) {
			res = curStr;
		}

		for (int i = 0; i < edges.at(curState).size(); ++i) {
			int nextState = edges.at(curState).at(i);
			if (nextState != -1 && i >= nWords) {
				curStr.push_back(i);
				Candidate_DFS(nextState, candidates, curStr, res);
				curStr.pop_back();
			}
		}
	}


	int last = 0;
	std::vector<std::vector<int>> edges;
	std::vector<int> link, len, nWays;
	std::vector<std::set<int>> attainability;
	int nStates = 0;

	long long sumWays = 0;

	int nWords = 0;
};


int main() {
	std::cin.tie(0);
	std::ios_base::sync_with_stdio(false);

	int K = 0;
	std::cin >> K;

	SufAutomata sufAutomata;

	for (int i = 0; i < K; ++i) {
		std::string curStr;
		std::cin >> curStr;

		sufAutomata.AddWord(curStr);
	}

	std::cout << sufAutomata.LongestCommonSubstr();

	return 0;
}