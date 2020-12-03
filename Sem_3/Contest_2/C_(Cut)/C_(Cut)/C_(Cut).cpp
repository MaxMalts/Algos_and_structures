#include <iostream>
#include <vector>
#include <queue>
#include <assert.h>


struct Edge {
	long long from = 0;
	long long to = 0;
	long long capacity = 0;
	long long flow = 0;
};


struct Graph {

	Graph(const long long nVertices)
		: edgesIndByVert(std::vector<std::vector<long long>>(nVertices)), nVertices(nVertices) {}

	std::vector<Edge> edges;
	std::vector<std::vector<long long>> edgesIndByVert;

	long long nVertices = 0;
};


void ReadData(Graph& graph, const long long nEdges) {
	assert(nEdges >= 0);

	long long curSrc = 0, curDest = 0, curCapacity = 0;
	for (long long i = 0; i < nEdges; ++i) {
		std::cin >> curSrc >> curDest >> curCapacity;
		--curSrc;
		--curDest;

		graph.edges.push_back(Edge{ curSrc, curDest, curCapacity, 0 });
		graph.edgesIndByVert[curSrc].push_back(graph.edges.size() - 1);

		graph.edges.push_back(Edge{ curDest, curSrc, curCapacity, 0 });
		graph.edgesIndByVert[curDest].push_back(graph.edges.size() - 1);
	}
}


long long FindPath(Graph& graph, const long long source, const long long target) {
	assert(source >= 0);
	assert(target >= 0);

	std::queue<long long> queue;
	queue.push(source);

	std::vector<long long> prevEdge(graph.nVertices, 0);

	std::vector<long long> vertFlow(graph.nVertices, 0);

	prevEdge[target] = -1;

	long long curVert = 0;
	vertFlow[source] = LLONG_MAX;
	while (prevEdge[target] == -1 && queue.size() > 0) {
		curVert = queue.front();
		queue.pop();
		for (long long curEdgeInd : graph.edgesIndByVert[curVert]) {
			Edge& curEdge = graph.edges[curEdgeInd];

			if (curEdge.capacity - curEdge.flow > 0) {
				if (vertFlow[curEdge.to] == 0) {
					queue.push(curEdge.to);
					prevEdge[curEdge.to] = curEdgeInd;

					if (curEdge.capacity - curEdge.flow < vertFlow[curVert])
						vertFlow[curEdge.to] = curEdge.capacity - curEdge.flow;
					else
						vertFlow[curEdge.to] = vertFlow[curVert];
				}

				if (curEdge.to == target) {
					assert(prevEdge[target] != -1);
					break;
				}
			}
		}
	}

	if (prevEdge[target] == -1)
		return 0;

	curVert = target;
	while (curVert != source) {
		long long prevEdgeInd = prevEdge[curVert];
		graph.edges[prevEdge[curVert]].flow += vertFlow[target];
		graph.edges[prevEdgeInd % 2 == 0 ? prevEdgeInd + 1 : prevEdgeInd - 1].flow -= vertFlow[target];
		curVert = graph.edges[prevEdgeInd].from;
	}

	return vertFlow[target];
}


int MaxFlow(Graph& graph, const long long source, const long long target) {
	assert(source >= 0);
	assert(target >= 0);

	long long maxFlow = 0;
	long long curFlow = 0;
	do {
		curFlow = FindPath(graph, source, target);
		maxFlow += curFlow;
	} while (curFlow > 0);

	return maxFlow;
}


bool FindResPath(Graph& graph, const long long source, const long long target, std::vector<bool>& visited) {
	assert(source >= 0);
	assert(target >= 0);

	std::queue<long long> queue;
	queue.push(source);

	std::vector<long long> prevVert(graph.nVertices, 0);

	prevVert[target] = -1;

	long long curVert = 0;
	visited[source] = true;
	while (prevVert[target] == -1 && queue.size() > 0) {
		curVert = queue.front();
		queue.pop();
		for (long long curEdgeInd : graph.edgesIndByVert[curVert]) {
			Edge& curEdge = graph.edges[curEdgeInd];

			if (curEdge.capacity - curEdge.flow > 0) {
				if (!visited[curEdge.to]) {
					queue.push(curEdge.to);
					prevVert[curEdge.to] = curEdgeInd;
					visited[curEdge.to] = true;
				}

				if (curEdge.to == target) {
					assert(prevVert[target] != -1);
					break;
				}
			}
		}
	}

	if (prevVert[target] == -1)
		return false;
	else
		return true;
}


void MinCut(Graph& graph, int source, int target, std::vector<int>& vertices) {
	assert(source >= 0);
	assert(target >= 0);
	assert(vertices.size() == 0);

	std::vector<bool> visited(graph.nVertices, 0);
	FindResPath(graph, source, target, visited);

	for (int edgeInd = 0; edgeInd < graph.edges.size(); ++edgeInd) {
		if (visited[graph.edges[edgeInd].from] && !visited[graph.edges[edgeInd].to]) {
			vertices.push_back(edgeInd / 2);
		}
	}
}


int main() {
	long long nVertices = 0, nEdges = 0;
	std::cin >> nVertices >> nEdges;

	Graph graph(nVertices);
	ReadData(graph, nEdges);

	long long maxFlow = MaxFlow(graph, 0, nVertices - 1);

	std::vector<int> ans;
	MinCut(graph, 0, nVertices - 1, ans);

	std::cout << ans.size() << ' ' << maxFlow << '\n';
	for (int i = 0; i < ans.size(); ++i)
		std::cout << ans[i] + 1 << ' ';

	return 0;
}