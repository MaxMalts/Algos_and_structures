#include <iostream>
#include <vector>
#include <assert.h>


int TimeBetweenPoints(std::pair<int, int> firstPoint, std::pair<int, int> secondPoint) {
	return abs(firstPoint.first - secondPoint.first) + abs(firstPoint.second - secondPoint.second);
}

class Order {
public:
	int startTime = 0;
	std::pair<int, int> startPoint;
	std::pair<int, int> endPoint;
	int endTime = 0;

	Order(int startTime, int startPointX, int startPointY, int endPointX, int endPointY)
		: startTime(startTime),
		startPoint(startPointX, startPointY),
		endPoint(endPointX, endPointY),
		endTime(startTime + TimeBetweenPoints(startPoint, endPoint)) {}
};


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


int MaxMatch(std::vector<std::vector<int>> graph) {
	int matchSize = 0;
	std::vector<int> foundEdges;
	std::vector<bool> passed;
	for (int i = 0; i < graph.size(); ++i) {
		passed.assign(graph.size(), false);
		FindChain(i, graph, passed, matchSize, foundEdges);
	}

	return matchSize;
}


int M = 0;
std::vector<Order> orders;
std::vector<std::vector<int>> bipartGraph;  // First part includes the right nodes of each order and the second
                                            // part - the left nodes (each order is assitiated with 2 nodes)
std::vector<int> foundEdges;

void ReadData() {
	std::cin >> M;

	for (int i = 0; i < M; ++i) {
		int startHour = 0, startMinute = 0, startPointX = 0, startPointY = 0, endPointX = 0, endPointY = 0;
		std::cin >> startHour;
		std::cin.get();
		std::cin >> startMinute;
		std::cin >> startPointX >> startPointY >> endPointX >> endPointY;

		orders.push_back(Order(startHour * 60 + startMinute, startPointX, startPointY, endPointX, endPointY));
	}
}


void GraphFromOrders() {
	bipartGraph.resize(M);
	for (int i = 0; i < M; ++i) {
		for (int j = i + 1; j < M; ++j) {
			if (orders[i].endTime + TimeBetweenPoints(orders[i].endPoint, orders[j].startPoint) < orders[j].startTime) {
				bipartGraph[i].push_back(j);
			}
		}
	}
}


int Calculate() {
	GraphFromOrders();

	int res = M - MaxMatch(bipartGraph);
	return res;
}


int main() {
	ReadData();
	int ans = Calculate();
	std::cout << ans;
}