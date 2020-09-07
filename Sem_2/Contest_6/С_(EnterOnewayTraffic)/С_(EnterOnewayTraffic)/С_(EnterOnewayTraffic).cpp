#include <iostream>
#include <vector>
#include <algorithm>
#include <assert.h>

using namespace std;

#pragma GCC optimize("unroll-loops")
#pragma GCC optimize ("O3")

//#define assert(cond)  



struct Road {
	int depCity = 0;
	int destCity = 0;
};


const int NMax = 1000;
int timer = 0;
vector<int> tin(NMax + 1), fup(NMax + 1);

bool IsBiconnected_rec(vector<vector<Road*>>& roadByCity, vector<vector<vector<Road*>>>& roadsByCities,
					   vector<bool>& visited, int curCity, int curParent) {

	visited[curCity] = true;
	tin[curCity] = fup[curCity] = timer++;
	for (Road* curRoad : roadByCity[curCity]) {
		int city1 = curRoad->depCity;
		int city2 = curRoad->destCity;
		if (city1 > city2) {
			swap(city1, city2);
		}

		if (curCity == curRoad->destCity) {
			swap(curRoad->depCity, curRoad->destCity);
		}
		
		int to = curRoad->destCity;
		if (to == curParent) {
			continue;
		}
		if (visited[to])
			fup[curCity] = min(fup[curCity], tin[to]);
		else {
			if (!IsBiconnected_rec(roadByCity, roadsByCities, visited, to, curCity)) {
				return false;
			}
			fup[curCity] = min(fup[curCity], fup[to]);
			if (fup[to] > tin[curCity] && roadsByCities[city1][city2].size() < 2) {
				return false;
			}
		}		
	}

	return true;
}

bool IsBiconnected(vector<vector<Road*>>& roadByCity, vector<vector<vector<Road*>>>& roadsByCities, int nCities) {
	assert(roadByCity.size() > 0);
	assert(roadsByCities.size() > 0);
	assert(nCities > 0);

	vector<bool> visited(nCities + 1, false);

	for (int i = 1; i <= nCities; ++i) {
		if (!visited[i]) {
			if (!IsBiconnected_rec(roadByCity, roadsByCities, visited, i, -1)) {
				return false;
			}
		}
	}

	return true;
}


bool OrientRoads(vector<vector<Road*>>& roadByCity, vector<vector<vector<Road*>>>& roadsByCities,
				 vector<bool>& visited, int curCity) {
	assert(roadByCity.size() > 0);
	assert(roadsByCities.size() > 0);
	assert(visited.size() > 0);
	assert(curCity > 0);
	assert(!visited[curCity]);

	visited[curCity] = true;

	for (Road* curRoad : roadByCity[curCity]) {
		int city1 = curRoad->depCity;
		int city2 = curRoad->destCity;
		if (city1 > city2) {
			swap(city1, city2);
		}
		
		if (curCity == curRoad->destCity) {
			swap(curRoad->depCity, curRoad->destCity);
		}
		assert(curCity == curRoad->depCity);
		assert(curCity != curRoad->destCity);

		assert(roadsByCities[city1][city2].size() > 0);
		if (visited[curRoad->destCity]) {
			if (roadsByCities[city1][city2].size() >= 2) {
				if (roadsByCities[city1][city2][0]->depCity == roadsByCities[city1][city2][1]->depCity) {
					swap(roadsByCities[city1][city2][1]->depCity, roadsByCities[city1][city2][1]->destCity);
				}
			} else {
				swap(curRoad->depCity, curRoad->destCity);
			}

		} else {
			if (roadsByCities[city1][city2].size() >= 2) {
				if (roadsByCities[city1][city2][0]->depCity == roadsByCities[city1][city2][1]->depCity) {
					swap(roadsByCities[city1][city2][1]->depCity, roadsByCities[city1][city2][1]->destCity);
				}
			}
			OrientRoads(roadByCity, roadsByCities, visited, curRoad->destCity);
		}
	}

	//visited[curCity] = false;

	return true;
}


int main() {
	ios_base::sync_with_stdio(false);
	cin.tie(0);

	int N = 0, M = 0;
	cin >> N >> M;
	
	vector<Road> roadByInd(M + 1);
	vector<vector<Road*>> roadsByCity(N + 1);
	vector<vector<vector<Road*>>> roadsByCities(N + 1, vector<vector<Road*>>(N + 1));
	for (int i = 0; i < M; ++i) {
		int city1 = 0, city2 = 0;
		cin >> city1 >> city2;
		if (city1 > city2) {
			swap(city1, city2);
		}

		roadsByCities[city1][city2].push_back(&roadByInd[i]);

		roadByInd[i].depCity = city1;
		roadByInd[i].destCity = city2;

		if (roadsByCities[city1][city2].size() == 1) {
			roadsByCity[city1].push_back(&roadByInd[i]);
			roadsByCity[city2].push_back(&roadByInd[i]);
		}
	}
	cout << '\n';
	if (!IsBiconnected(roadsByCity, roadsByCities, N)) {
		cout << 0;

	} else {
		vector<bool> visited(N + 1, false);
		bool exists = OrientRoads(roadsByCity, roadsByCities, visited, 1);

		for (int i = 0; i < M; ++i) {
			cout << roadByInd[i].depCity << ' ' << roadByInd[i].destCity << '\n';
		}
	}

	return 0;
}