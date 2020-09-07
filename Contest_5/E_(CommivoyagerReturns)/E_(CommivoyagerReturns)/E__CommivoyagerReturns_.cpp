#include <iostream>
#include <vector>
#include <assert.h>

using namespace std;


#pragma GCC optimize("O3")

typedef pair<long long, int> minPrice_prevPlanet;


long long FindWay(vector<vector<minPrice_prevPlanet>>& table, vector<vector<int>>& prices,
	int curMask, int curPlanet, int NPlanets) {
	assert(curPlanet >= 0);
	assert(NPlanets > 0);

	if (table[curMask][curPlanet].first != -1) {
		return table[curMask][curPlanet].first;
	}

	table[curMask][curPlanet].first = 2147483647;
	int prevMask = curMask & ~(1 << curPlanet);
	for (int i = 0; i < NPlanets; ++i) {
		if ((prevMask & (1 << i)) == 0) {
			continue;
		}

		long long curMinPrice = FindWay(table, prices, prevMask, i, NPlanets) + prices[i][curPlanet];
		if (curMinPrice < table[curMask][curPlanet].first) {
			table[curMask][curPlanet].first = curMinPrice;
			table[curMask][curPlanet].second = i;
		}
	}

	return table[curMask][curPlanet].first;
}


void PrintWay(vector<vector<minPrice_prevPlanet>>& table, int curMask, int curPlanet) {
	assert(curPlanet >= 0);

	int prevPlanet = table[curMask][curPlanet].second;
	int prevMask = curMask & ~(1 << curPlanet);
	if (prevPlanet != curPlanet) {
		PrintWay(table, prevMask, prevPlanet);
	}
	cout << curPlanet + 1 << ' ';
}


void ReadPrices(vector<vector<int>>& prices, int NPlanets) {
	assert(NPlanets > 0);

	for (int i = 0; i < NPlanets; ++i) {
		for (int j = 0; j < NPlanets; ++j) {
			cin >> prices[i][j];
		}
	}
}


void InitTable(vector<vector<minPrice_prevPlanet>>& table, int NMasks, int NPlanets) {
	assert(NPlanets > 0);

	for (int i = 0; i < NMasks; ++i) {
		for (int j = 0; j < NPlanets; ++j) {
			table[i][j].first = -1;
		}
	}

	for (int i = 0; i < NPlanets; ++i) {
		table[1 << i][i].first = 0;
		table[1 << i][i].second = i;
	}
}


int main() {
	int n = 0;
	cin >> n;

	int NMasks = 1 << n;

	vector<vector<int>> prices(n, vector<int>(n, 0));
	ReadPrices(prices, n);

	vector<vector<minPrice_prevPlanet>> table(NMasks, vector<minPrice_prevPlanet>(n));
	InitTable(table, NMasks, n);


	long long minPrice = FindWay(table, prices, NMasks - 1, 0, n);
	int lastStep = 0;
	for (int i = 1; i < n; ++i) {
		long long curMinPrice = FindWay(table, prices, NMasks - 1, i, n);

		if (curMinPrice < minPrice) {
			minPrice = curMinPrice;
			lastStep = i;
		}
	}

	cout << minPrice << "\n";
	PrintWay(table, NMasks - 1, lastStep);

	return 0;
}