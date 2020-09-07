#include <iostream>
#include <algorithm>
#include <vector>
#include <queue>
#include <assert.h>

using namespace std;


#pragma GCC optimize("O3")


struct Flight {
	int departure = 0;
	int destination = 0;
	int price = 0;
};


void ReadFlights(vector<vector<Flight>>& flightsFrom, int NFlights) {
	assert(NFlights > 0);

	for (int i = 0; i < NFlights; ++i) {
		int curDep, curDest, curPrice;
		cin >> curDep >> curDest >> curPrice;
		--curDep; --curDest;

		flightsFrom[curDep].push_back({curDep, curDest, curPrice});
	}
}



//int FindMinPrice(int curCity, vector<bool>& visited, vector<vector<Flight>>& flightsFrom, int& curPrice, int& minPrice, int nightsLeft, int destCity) {
//	assert(flightsFrom.size() > 0);
//	assert(nightsLeft >= 0);
//	assert(destCity > 0);
//
//	if (visited[curCity] || (nightsLeft == 0 && curCity != destCity)) {
//		return -1;
//	}
//	visited[curCity] = true;
//
//	if (curCity == destCity) {
//		minPrice = curPrice;
//		visited[curCity] = false;
//		return 0;
//	}
//
//	int res = -1;
//	for (Flight curFlight : flightsFrom[curCity]) {
//		if (minPrice == -1 || curPrice + curFlight.price < minPrice) {
//			curPrice += curFlight.price;
//			int curMinPrice = FindMinPrice(curFlight.destination, visited, flightsFrom, curPrice, minPrice, nightsLeft - 1, destCity);
//			curPrice -= curFlight.price;
//
//			if (curMinPrice != -1 && res == -1 ||
//				curMinPrice != -1 && curMinPrice + curFlight.price < res) {
//				res = curMinPrice + curFlight.price;
//			}
//		}
//	}
//
//	visited[curCity] = false;
//	return res;
//}


struct CityData {
	int ind = 0;
	int priceTo = 0;
	int nightsLeft = 0;
};

int FindMinPrice(vector<vector<Flight>>& flightsFrom, int depCity, int destCity, int NCities, int nightsLeft) {
	assert(flightsFrom.size() > 0);
	assert(NCities > 0);
	assert(nightsLeft > 0);
	assert(destCity > 0);

	vector<int> priceTo(NCities, INT_MAX);
	priceTo[depCity] = 0;

	queue<CityData> cities;
	cities.push({depCity, 0, nightsLeft});
	while (cities.size() > 0) {
		CityData curCity = cities.front();
		cities.pop();

		for (Flight curFlight : flightsFrom[curCity.ind]) {
			int curPrice = curCity.priceTo + curFlight.price;
			if (curPrice >= priceTo[curFlight.destination]) {
				continue;
			}
			priceTo[curFlight.destination] = curPrice;

			if (curCity.nightsLeft > 1) {
				cities.push({curFlight.destination, curPrice, curCity.nightsLeft - 1});
			}
		}
	}

	if (priceTo[destCity] == INT_MAX) {
		return -1;
	}
	return priceTo[destCity];
}



int main() {
	ios_base::sync_with_stdio(false);
	cin.tie(0);

	int N, M, K, S, F;
	cin >> N >> M >> K >> S >> F;
	--S; --F;

	vector<vector<Flight>> flightsFrom(N);
	ReadFlights(flightsFrom, M);

	//vector<bool> visited(N, false);
	//int curPrice = 0, minPrice = -1;
	int ans = FindMinPrice(flightsFrom, S, F, N, K);

	cout << ans;

	return 0;
}