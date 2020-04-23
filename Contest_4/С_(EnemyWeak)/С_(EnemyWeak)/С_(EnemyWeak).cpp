#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <assert.h>
#include <time.h>

using namespace std;


typedef int value_t;

struct Node {
	value_t value = {};

	value_t sum = 0;
};

class SegmTree {
public:

	SegmTree(int NValues) : NValues(NValues) {
		assert(NValues > 0);

		data = vector<Node>(4 * (NValues + 1), Node());
	}

	SegmTree(vector<value_t> values) {
		NValues = values.size();

		data = vector<Node>(4 * (NValues + 1));

		Create(values, 1, 0, NValues - 1);
	}

	SegmTree(ifstream& fin, int NValues) : NValues(NValues) {
		data = vector<Node>(4 * (NValues + 1));

		Create(fin, 1, 0, NValues - 1);
	}

	void Update(int index, value_t element);

	value_t SegmSum(int left, int right);


private:
	vector<Node> data;

	int NValues = 0;

	void Create(vector<value_t> values, int curNodeInd, int curLeft, int curRight);

	void Create(ifstream& fin, int curNodeInd, int curLeft, int curRight);

	void Update_rec(int index, value_t newVal, int curNodeInd, int curLeft, int curRight);

	value_t SegmSum_rec(int left, int right, int curNodeInd, int curLeft, int curRight);

};


void SegmTree::Create(vector<value_t> values, int curNodeInd, int curLeft, int curRight) {
	if (curLeft == curRight) {
		data[curNodeInd].value = values[curLeft];
		data[curNodeInd].sum = values[curLeft];

	} else {
		int curMiddle = (curLeft + curRight) / 2;
		Create(values, curNodeInd * 2, curLeft, curMiddle);
		Create(values, curNodeInd * 2 + 1, curMiddle + 1, curRight);

		data[curNodeInd].value = {};

		data[curNodeInd].sum = data[curNodeInd * 2].sum + data[curNodeInd * 2 + 1].sum;
	}
}


void SegmTree::Create(ifstream& fin, int curNodeInd, int curLeft, int curRight) {
	if (curLeft == curRight) {
		fin >> data[curNodeInd].value;
		data[curNodeInd].sum = data[curLeft].value;

	} else {
		int curMiddle = (curLeft + curRight) / 2;
		Create(fin, curNodeInd * 2, curLeft, curMiddle);
		Create(fin, curNodeInd * 2 + 1, curMiddle + 1, curRight);

		data[curNodeInd].value = {};

		data[curNodeInd].sum = data[curNodeInd * 2].sum + data[curNodeInd * 2 + 1].sum;
	}
}


void SegmTree::Update(int index, value_t newVal) {
	assert(index >= 0);

	Update_rec(index, newVal, 1, 0, NValues - 1);
}


void SegmTree::Update_rec(int index, value_t newVal, int curNodeInd, int curLeft, int curRight) {
	if (curLeft == curRight) {
		data[curNodeInd].value = newVal;
		data[curNodeInd].sum = newVal;

	} else {
		int curMiddle = (curLeft + curRight) / 2;
		if (index <= curMiddle) {
			Update_rec(index, newVal, curNodeInd * 2, curLeft, curMiddle);

		} else {
			Update_rec(index, newVal, curNodeInd * 2 + 1, curMiddle + 1, curRight);
		}

		data[curNodeInd].value = {};

		data[curNodeInd].sum = data[curNodeInd * 2].sum + data[curNodeInd * 2 + 1].sum;
	}
}


value_t SegmTree::SegmSum(int left, int right) {
	assert(left >= 0);
	assert(right >= left);
	assert(right < NValues);

	return SegmSum_rec(left, right, 1, 0, NValues - 1);
}


value_t SegmTree::SegmSum_rec(int left, int right, int curNodeInd, int curLeft, int curRight) {
	if (left > right) {
		return 0;
	}

	if (left == curLeft && right == curRight) {
		return data[curNodeInd].sum;
	}

	int curMiddle = (curLeft + curRight) / 2;
	value_t res = SegmSum_rec(left, min(right, curMiddle), curNodeInd * 2, curLeft, curMiddle) +
		SegmSum_rec(max(left, curMiddle + 1), right, curNodeInd * 2 + 1, curMiddle + 1, curRight);

	return res;
}


// End of segment tree realization




int main() {
	freopen("input.txt", "r", stdin);
	freopen("output.txt", "w", stdout);

	int n;
	cin >> n;

	vector<int> rightLess(n), leftGreater(n);

	vector<pair<int, int>> val__ind(n);
	for (int i = 0; i < n; ++i) {
		cin >> val__ind[i].first;
		val__ind[i].second = i;
	}
	sort(val__ind.begin(), val__ind.end());

	SegmTree tree(n);
	rightLess[0] = 0;
	tree.Update(val__ind[0].second, 1);
	for (int i = 1; i < n; ++i) {
		if (val__ind[i].second < n - 1) {
			rightLess[i] = tree.SegmSum(val__ind[i].second + 1, n - 1);

		} else {
			rightLess[i] = 0;
		}
		tree.Update(val__ind[i].second, 1);
	}

	tree = SegmTree(n);
	leftGreater[0] = 0;
	tree.Update(val__ind[n - 1].second, 1);
	for (int i = 1; i < n; ++i) {
		if (val__ind[n - 1 - i].second) {
			leftGreater[i] = tree.SegmSum(0, val__ind[n - 1 - i].second - 1);

		} else {
			leftGreater[i] = 0;
		}
		tree.Update(val__ind[n - 1 - i].second, 1);
	}

	long long ans = 0;
	for (int i = 1; i < n; ++i) {
		ans += (long long)leftGreater[n - 1 - i] * (long long)rightLess[i];
	}
	cout << ans;

	double time = static_cast<double>(clock()) / CLOCKS_PER_SEC;

	return 0;
}