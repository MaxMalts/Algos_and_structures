#include <iostream>
#include <vector>
#include <algorithm>
#include <assert.h>

using namespace std;



struct Node {
	long long ind = 0;
	long long inpPlace = 0;
};


struct Edge {
	long long node1 = 0;
	long long node2 = 0;
};



void ReadGraph(vector<vector<Node>>& nodesData, vector<Edge>& edges, long long N, long long M) {
	assert(N >= 0);
	assert(M >= 0);

	nodesData = vector<vector<Node>>(N);

	long long v = 0, u = 0;
	for (long long i = 0; i < M; ++i) {
		cin >> v >> u;

		edges.push_back({--v, --u});

		nodesData[v].push_back({u, i});
		nodesData[u].push_back({v, i});
	}
}


long long CalcSubsize(vector<vector<Node>>& graph, vector<long long>& updated, vector<long long>& subsizes, long long prevNode, long long curNode) {
	assert(prevNode >= 0);
	assert(curNode >= 0);

	long long res = 1;
	for (long long i = 0; i < graph[curNode].size(); ++i) {
		Node nextNode = graph[curNode][i];
		if (nextNode.ind == prevNode) {
			continue;
		}

		updated.push_back(nextNode.inpPlace);
		res += CalcSubsize(graph, updated, subsizes, curNode, nextNode.ind);
	}

	subsizes[curNode] = res;
	return res;
}


vector<long long> CalcAns(vector<vector<Node>>& nodesData, vector<Edge>& edges, long long NNodes) {
	assert(NNodes >= 0);

	vector<long long> ans(NNodes, 0);

	vector<long long> subsize(NNodes, 0);
	vector<long long> updated;
	for (long long i = 0; i < NNodes; ++i) {
		if (subsize[i] != 0) {
			assert(subsize[i] > 0);

			continue;
		}

		long long rootSize = CalcSubsize(nodesData, updated, subsize, i, i);
		for (long long j = 0; j < updated.size(); ++j) {
			long long curNum = updated[j];
			long long node1 = edges[curNum].node1;
			long long node2 = edges[curNum].node2;

			long long minSize = min(subsize[node1], subsize[node2]);
			ans[curNum] = (rootSize - minSize) * minSize;
		}
		updated.clear();
	}

	return ans;
}


void HandleRequests(vector<long long> ans) {
	long long Q;
	cin >> Q;

	long long e = 0;
	for(long long i = 0; i < Q; ++i){
		cin >> e;
		cout << ans[--e] << '\n';
	}
}


int main() {
	ios_base::sync_with_stdio(false);
	cin.tie(0);

	long long N = 0, M = 0;
	cin >> N >> M;

	vector<vector<Node>> nodesData;
	vector<Edge> edges;
	ReadGraph(nodesData, edges, N, M);

	vector<long long> ans = CalcAns(nodesData, edges, N);

	HandleRequests(ans);

	return 0;
}