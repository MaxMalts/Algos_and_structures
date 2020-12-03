#include <iostream>
#include <vector>
#include <queue>
#include <assert.h>

#define INT_MAX 2147483647

void ReadData(std::vector<std::vector<int>>& graph, const int nEdges) {
    assert(nEdges > 0);

    int curSrc = 0, curDest = 0, curCapacity;
    for (int i = 0; i < nEdges; ++i) {
        std::cin >> curSrc >> curDest >> curCapacity;
        curSrc--;
        curDest--;
        assert(curSrc >= 0 && curSrc < graph.size());
        assert(curDest >= 0 && curSrc < graph.size());
		assert(curSrc != curDest);
        assert(curCapacity > 0);

        graph[curSrc][curDest] = curCapacity;
    }
}


int FindPath(const std::vector<std::vector<int>>& graph, std::vector<std::vector<int>>& flow,
             const int source, const int target) {

	assert(source >= 0 && source < graph.size());
	assert(target >= 0 && target < graph.size());
	assert(source != target);
	assert(flow.size() >= graph.size());


	std::queue<int> queue;
	queue.push(source);
    
	std::vector<int> prevVert(graph.size(), 0);

	std::vector<int> vertFlow(graph.size(), 0);
	
	prevVert[target] = -1;

    int curVert = 0;
	int minFlow = 0;
    vertFlow[source] = INT_MAX;
    while (prevVert[target] == -1 && queue.size() > 0) {
		curVert = queue.front();
		queue.pop();
		for (int i = 0; i < graph.size(); ++i) {
			if ((graph[curVert][i] - flow[curVert][i]) > 0 && vertFlow[i] == 0) {
				queue.push(i);
				prevVert[i] = curVert;

				if (graph[curVert][i] - flow[curVert][i] < vertFlow[curVert])
					vertFlow[i] = graph[curVert][i] - flow[curVert][i];
				else
					vertFlow[i] = vertFlow[curVert];
			}
		}
    }

    if (prevVert[target] == -1)
		return 0;

    curVert = target;
    while (curVert != source) {
        flow[prevVert[curVert]][curVert] += vertFlow[target];
		if (graph[curVert][prevVert[curVert]] > 0)
        curVert = prevVert[curVert];
    }

    return vertFlow[target];
}


int MaxFlow(const std::vector<std::vector<int>>& graph, const int source, const int target) {
	assert(source >= 0 && source < graph.size());
	assert(target >= 0 && target < graph.size());
	assert(source != target);

	std::vector<std::vector<int>> flow(graph.size(), std::vector<int>(graph.size(), 0));

    int res = 0, curFlow = 0;
    do {
        curFlow = FindPath(graph, flow, source, target);
        res += curFlow;
    } while (curFlow > 0);

    return res;
}


int main() {
    int nVertices = 0, nEdges = 0;
    std::cin >> nVertices >> nEdges;

	std::vector<std::vector<int>> graph(nVertices, std::vector<int>(nVertices, 0));
	ReadData(graph, nEdges);

	int ans = MaxFlow(graph, 0, nVertices - 1);
	std::cout << ans;
    
    return 0;
}