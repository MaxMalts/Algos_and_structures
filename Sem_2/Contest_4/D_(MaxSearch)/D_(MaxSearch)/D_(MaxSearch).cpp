#include <iostream>
#include <vector>
#include <cmath>
#include <fstream>
#include <assert.h>

using namespace std;


#ifndef _DEBUG
#define assert(cond) ;
#endif


int Log2(int x) {

	int res = 0;
	while (x > 1) {
		x >>= 1;
		++res;
	}

	return res;
}


typedef int value_t;

struct Node {
	value_t maxValue = {};
	int index = 0;
};

class SparceTable {
public:

	SparceTable(vector<value_t> elements) {
		NValues = elements.size();

		table = vector<vector<Node>>(NValues, vector<Node>(height));
		
		for (int i = 0; i < NValues; ++i) {
			table[i][0].maxValue = elements[i];
			table[i][0].index = i;
		}

		ConstructNext();

		PrepareLogs2();
	}


	SparceTable(istream& fin, int NValues) : NValues(NValues) {
		height = Log2(NValues) + 1;

		table = vector<vector<Node>>(NValues, vector<Node>(height));

		for (int i = 0; i < NValues; ++i) {
			cin >> table[i][0].maxValue;
			table[i][0].index = i;
		}

		ConstructNext();

		//log2_x = vector<int>(NValues + 1, 0);
		PrepareLogs2();
	}


	int SegmMaxIndex(int left, int right) {
		assert(left >= 0);
		assert(right >= left);
		assert(right < NValues);

		int len = right - left + 1;

		/*if (log2_x[len] == 0) {
			log2_x[len] = floor(log2(len));
		}*/

		int log2_len = log2_x[len];

		if (table[left][log2_len].maxValue >= table[right - (1 << log2_len) + 1][log2_len].maxValue) {
			return table[left][log2_len].index;
		} else {
			return table[right - (1 << log2_len) + 1][log2_len].index;
		}
	}


private:
	int height = 0;

	vector<vector<Node>> table;

	int NValues = 0;

	vector<int> log2_x;


	void ConstructNext() {

		for (int j = 1; j < height; ++j) {
			for (int i = 0; i + (1 << j) <= NValues; ++i) {

				if (table[i][j - 1].maxValue >= table[i + (1 << (j - 1))][j - 1].maxValue) {
					table[i][j].maxValue = table[i][j - 1].maxValue;
					table[i][j].index = table[i][j - 1].index;

				} else {
					table[i][j].maxValue = table[i + (1 << (j - 1))][j - 1].maxValue;
					table[i][j].index = table[i + (1 << (j - 1))][j - 1].index;
				}
			}
		}
	}
	
	void PrepareLogs2() {
		log2_x = vector<int>(NValues + 1);

		for (int i = 0; i <= NValues; ++i) {
			log2_x[i] = Log2(i);
		}
	}
};




void HandleRequest(SparceTable& elemTable) {
	int left = 0, right = 0;
	cin >> left >> right;

	int ans = elemTable.SegmMaxIndex(left - 1, right - 1) + 1;
	cout << ans << ' ';
}


int main() {
	freopen("index-max.in", "r", stdin);
	freopen("index-max.out", "w", stdout);

	ios_base::sync_with_stdio(false);
	cin.tie(0);

	int N = 0, K = 0;
	cin >> N;

	SparceTable elemTable(cin, N);

	cin >> K;
	for (int i = 0; i < K; ++i) {
		HandleRequest(elemTable);
	}

	return 0;
}