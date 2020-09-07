#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <assert.h>

using namespace std;


typedef int value_t;

struct Node {
	value_t value = {};

	int NZeros = 0;
	value_t maxElem = 0;
	int maxIndex = 0;
	value_t sum = 0;
};

class SegmTree {
public:

	SegmTree(int NValues, value_t value) : NValues(NValues) {
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

	int KthZeroIndex(int k, int left, int right);

	int SegmMaxIndex(int left, int right);

	int CountZerosSegm(int left, int right);

private:
	vector<Node> data;

	int NValues = 0;

	void Create(vector<value_t> values, int curNodeInd, int curLeft, int curRight);

	void Create(ifstream& fin, int curNodeInd, int curLeft, int curRight);

	void Update_rec(int index, value_t newVal, int curNodeInd, int curLeft, int curRight);

	value_t SegmSum_rec(int left, int right, int curNodeInd, int curLeft, int curRight);

	int KthZeroIndex_rec(int k, int left, int right, int curNodeInd, int curLeft, int curRight);

	int SegmMaxIndex_rec(int left, int right, int& res, value_t& foundMax, int curNodeInd, int curLeft, int curRight);

	int CountZerosSegm_rec(int left, int right, int curNodeInd, int curleft, int curRight);
};


void SegmTree::Create(vector<value_t> values, int curNodeInd, int curLeft, int curRight) {
	if (curLeft == curRight) {
		data[curNodeInd].value = values[curLeft];
		data[curNodeInd].NZeros = (values[curLeft] == 0) ? 1 : 0;
		data[curNodeInd].maxElem = values[curLeft];
		data[curNodeInd].maxIndex = curLeft;
		data[curNodeInd].sum = values[curLeft];

	} else {
		int curMiddle = (curLeft + curRight) / 2;
		Create(values, curNodeInd * 2, curLeft, curMiddle);
		Create(values, curNodeInd * 2 + 1, curMiddle + 1, curRight);

		data[curNodeInd].value = {};

		data[curNodeInd].NZeros = data[curNodeInd * 2].NZeros + data[curNodeInd * 2 + 1].NZeros;

		data[curNodeInd].maxElem = max(data[curNodeInd * 2].maxElem, data[curNodeInd * 2 + 1].maxElem);
		if (data[curNodeInd].maxElem == data[curNodeInd * 2].maxElem) {
			data[curNodeInd].maxIndex = data[curNodeInd * 2].maxIndex;
		} else {
			assert(data[curNodeInd].maxElem == data[curNodeInd * 2 + 1].maxElem);

			data[curNodeInd].maxIndex = data[curNodeInd * 2 + 1].maxIndex;
		}

		data[curNodeInd].sum = data[curNodeInd * 2].sum + data[curNodeInd * 2 + 1].sum;
	}
}


void SegmTree::Create(ifstream& fin, int curNodeInd, int curLeft, int curRight) {
	if (curLeft == curRight) {
		fin >> data[curNodeInd].value;
		data[curNodeInd].NZeros = (data[curLeft].value == 0) ? 1 : 0;
		data[curNodeInd].maxElem = data[curNodeInd].value;
		data[curNodeInd].maxIndex = curLeft;
		data[curNodeInd].sum = data[curLeft].value;

	} else {
		int curMiddle = (curLeft + curRight) / 2;
		Create(fin, curNodeInd * 2, curLeft, curMiddle);
		Create(fin, curNodeInd * 2 + 1, curMiddle + 1, curRight);

		data[curNodeInd].value = {};

		data[curNodeInd].NZeros = data[curNodeInd * 2].NZeros + data[curNodeInd * 2 + 1].NZeros;

		data[curNodeInd].maxElem = max(data[curNodeInd * 2].maxElem, data[curNodeInd * 2 + 1].maxElem);
		if (data[curNodeInd].maxElem == data[curNodeInd * 2].maxElem) {
			data[curNodeInd].maxIndex = data[curNodeInd * 2].maxIndex;
		} else {
			assert(data[curNodeInd].maxElem == data[curNodeInd * 2 + 1].maxElem);

			data[curNodeInd].maxIndex = data[curNodeInd * 2 + 1].maxIndex;
		}

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
		data[curNodeInd].NZeros = (newVal == 0) ? 1 : 0;
		data[curNodeInd].maxElem = newVal;
		data[curNodeInd].sum = newVal;

	} else {
		int curMiddle = (curLeft + curRight) / 2;
		if (index <= curMiddle) {
			Update_rec(index, newVal, curNodeInd * 2, curLeft, curMiddle);

		} else {
			Update_rec(index, newVal, curNodeInd * 2 + 1, curMiddle + 1, curRight);
		}

		data[curNodeInd].value = {};

		data[curNodeInd].NZeros = data[curNodeInd * 2].NZeros + data[curNodeInd * 2 + 1].NZeros;

		data[curNodeInd].maxElem = max(data[curNodeInd * 2].maxElem, data[curNodeInd * 2 + 1].maxElem);
		if (data[curNodeInd].maxElem == data[curNodeInd * 2].maxElem) {
			data[curNodeInd].maxIndex = data[curNodeInd * 2].maxIndex;
		} else {
			assert(data[curNodeInd].maxElem == data[curNodeInd * 2 + 1].maxElem);

			data[curNodeInd].maxIndex = data[curNodeInd * 2 + 1].maxIndex;
		}

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


int SegmTree::KthZeroIndex(int k, int left, int right) {
	assert(k > 0);
	assert(left >= 0);
	assert(right >= left);
	assert(right <= NValues - 1);

	return KthZeroIndex_rec(k, left, right, 1, 0, NValues - 1);
}


int SegmTree::KthZeroIndex_rec(int k, int left, int right, int curNodeInd, int curLeft, int curRight) {
	if (left > right) {
		return -1;
	}

	if (curLeft == curRight) {
		assert(data[curNodeInd].NZeros == 1 || data[curNodeInd].NZeros == 0);

		if (k == 1 && data[curNodeInd].NZeros == 1) {
			return curLeft;
		} else {
			return -1;
		}
	}

	int curMiddle = (curLeft + curRight) / 2;
	int leftZeros = CountZerosSegm_rec(left, min(right, curMiddle), curNodeInd, curLeft, curRight);
	if (leftZeros >= k) {
		return KthZeroIndex_rec(k, left, min(right, curMiddle), curNodeInd * 2, curLeft, curMiddle);

	} else {
		return KthZeroIndex_rec(k - leftZeros, max(left, curMiddle + 1), right,
			curNodeInd * 2 + 1, curMiddle + 1, curRight);
	}
}


int SegmTree::SegmMaxIndex(int left, int right) {
	assert(left >= 0);
	assert(right >= left);
	assert(right <= NValues - 1);

	int res = 0;
	value_t foundMax = 0;
	SegmMaxIndex_rec(left, right, res, foundMax, 1, 0, NValues - 1);

	return res;
}


value_t SegmTree::SegmMaxIndex_rec(int left, int right, int& foundInd, value_t& foundMax, int curNodeInd, int curLeft, int curRight) {
	if (left > right) {
		return 0;
	}

	if (left == curLeft && right == curRight) {
		if (data[curNodeInd].maxElem >= foundMax) {
			foundInd = data[curNodeInd].maxIndex;
			foundMax = data[curNodeInd].maxElem;
		}
		return data[curNodeInd].maxElem;
	}

	int curMiddle = (curLeft + curRight) / 2;
	value_t res = max(SegmMaxIndex_rec(left, min(right, curMiddle), foundInd, foundMax, curNodeInd * 2, curLeft, curMiddle),
		SegmMaxIndex_rec(max(left, curMiddle + 1), right, foundInd, foundMax, curNodeInd * 2 + 1, curMiddle + 1, curRight));

	return res;
}


int SegmTree::CountZerosSegm(int left, int right) {
	assert(left >= 0);
	assert(right >= left);

	return CountZerosSegm_rec(left, right, 1, 0, NValues - 1);
}


int SegmTree::CountZerosSegm_rec(int left, int right, int curNodeInd, int curLeft, int curRight) {
	if (left > right) {
		return 0;
	}

	if (left == curLeft && right == curRight) {
		return data[curNodeInd].NZeros;
	}

	int curMiddle = (curLeft + curRight) / 2;
	int res = CountZerosSegm_rec(left, min(right, curMiddle), curNodeInd * 2, curLeft, curMiddle) +
		CountZerosSegm_rec(max(left, curMiddle + 1), right, curNodeInd * 2 + 1, curMiddle + 1, curRight);

	return res;
}


int main() {
	vector<int> test = { 3, 0, 5, 7, 0 };
	SegmTree tree(test);
	tree.SegmSum(1, 2);
	tree.SegmSum(1, 3);
	tree.SegmSum(0, 1);
	tree.SegmSum(1, 4);
	tree.SegmSum(1, 1);
	tree.Update(1, 6);
	tree.SegmSum(1, 2);
	tree.SegmSum(2, 5);



	return 0;
}