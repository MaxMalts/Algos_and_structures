#include <iostream>
#include <vector>
#include <assert.h>


int n = 0, m = 0, l = 0;
std::vector<std::vector<int>> graph;
std::vector<int> ans;


void ReadData() {
	std::cin >> n >> m;
	graph.resize(n);

	for (int i = 0; i < n; ++i) {
		int curNode = 0;
		std::cin >> curNode;
		while (curNode) {
			graph[i].push_back(curNode - 1);
			std::cin >> curNode;
		}
	}
}


bool FindChain(int curNode, std::vector<std::vector<int>>& graph, std::vector<bool>& passed,
	int& matchSize, std::vector<int>& foundEdges) {
	if (passed[curNode])
		return false;

	passed[curNode] = true;
	for (auto nextNode : graph[curNode]) {
		if (nextNode >= foundEdges.size())
			foundEdges.resize(nextNode + 1, -1);

		if (foundEdges[nextNode] == -1) {
			foundEdges[nextNode] = curNode;
			++matchSize;
			return true;

		} else {
			if (FindChain(foundEdges[nextNode], graph, passed, matchSize, foundEdges)) {
				foundEdges[nextNode] = curNode;
				return true;
			}
		}
	}

	return false;
}


int MaxMatch(std::vector<std::vector<int>> graph, std::vector<int>& foundEdges) {
	int matchSize = 0;
	foundEdges.clear();
	std::vector<bool> passed;
	for (int i = 0; i < graph.size(); ++i) {
		passed.assign(graph.size(), false);
		FindChain(i, graph, passed, matchSize, foundEdges);
	}

	return matchSize;
}


void WriteAns() {
	std::cout << l << '\n';
	for (int i = 0; i < ans.size(); ++i) {
		if (ans[i] != -1)
			std::cout << ans[i] + 1 << ' ' << i + 1 << '\n';
	}
}


int main() {
	ReadData();
	l = MaxMatch(graph, ans);
	WriteAns();
}