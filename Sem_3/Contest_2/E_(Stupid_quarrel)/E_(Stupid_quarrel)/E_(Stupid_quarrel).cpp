#include <iostream>
#include <vector>
#include <assert.h>


struct Edge {
	int from = 0;
	int to = 0;
	int capacity = 0;
	int flow = 0;
};


struct Graph {

	Graph(const int nVertices) : edges(std::vector<std::vector<Edge>>(nVertices)) {}

	std::vector<std::vector<Edge>> edges;
};


void UpdateFlow(Graph& graph, Edge& edge, int changes) {
	edge.flow += changes;
	int dest = edge.to;
	graph.edges[dest][edge.from].flow -= changes;
}


void ReadData(Graph& graph, const int nEdges) {
	assert(nEdges >= 0);

	for (int i = 0; i < nEdges; ++i) {
		int curSrc, curDest;
		std::cin >> curSrc >> curDest;
		--curSrc;
		--curDest;

		graph.edges[curSrc].push_back(Edge{ static_cast<int>(graph.edges[curDest].size()), curDest, 1, 0 });
		graph.edges[curDest].push_back(Edge{ static_cast<int>(graph.edges[curSrc].size()) - 1, curSrc, 0, 0 });
	}
}


bool Improve(Graph& graph, std::vector<bool>& visited, int curVert, int target) {
	if (curVert == target)
		return true;

	visited[curVert] = true;
	for (Edge& curEdge : graph.edges[curVert]) {
		if (curEdge.flow < curEdge.capacity && !visited[curEdge.to]) {
			if (Improve(graph, visited, curEdge.to, target)) {
				UpdateFlow(graph, curEdge, 1);
				return true;
			}
		}
	}

	return false;
}


void OutputAns(Graph& graph, std::vector<bool>& visited, int curVert, int target) {
	if (curVert == target) {
		std::cout << target + 1 << '\n';
		return;
	}

	visited[curVert] = true;
	for (Edge& curEdge : graph.edges[curVert]) {
		if (curEdge.flow == 1 && !visited[curEdge.to]) {
			UpdateFlow(graph, curEdge, -1);
			std::cout << curVert + 1 << " ";
			OutputAns(graph, visited, curEdge.to, target);
			break;
		}
	}
}


void StartTask(Graph& graph, int source, int target) {
	assert(source >= 0);
	assert(target >= 0);

	std::vector<bool> visited(graph.edges.size());
	Improve(graph, visited, source, target);

	visited.assign(graph.edges.size(), false);
	if (Improve(graph, visited, source, target)) {
		std::cout << "YES\n";

		visited.assign(graph.edges.size(), 0);
		OutputAns(graph, visited, source, target);

		visited.assign(graph.edges.size(), 0);
		OutputAns(graph, visited, source, target);

	} else {
		std::cout << "NO";
	}
}


int main() {
	int nVertices, nEdges, s, t;
	std::cin >> nVertices >> nEdges >> s >> t;

	Graph graph(nVertices);
	ReadData(graph, nEdges);

	StartTask(graph, s - 1, t - 1);

	return 0;
}