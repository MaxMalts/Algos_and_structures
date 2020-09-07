#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <assert.h>
//#include <time.h>

using namespace std;


typedef int value_t;

struct Node {
	value_t value = {};

	int NZeros = 0;
	//value_t maxElem = 0;
	//int maxIndex = 0;
};

class SegmTree {
public:

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

	int KthZeroIndex(int k, int left, int right);

	//int SegmMaxIndex(int left, int right);

	int CountZerosSegm(int left, int right);

private:
	vector<Node> data;

	struct NodeInfo {
		int index = 0;
		int left = 0;
		int right = 0;
	};


	int NValues = 0;

	void Create(vector<value_t> values, int curNodeInd, int curLeft, int curRight);

	void Create(ifstream& fin, int curNodeInd, int curLeft, int curRight);

	void Update_rec(int index, value_t newVal, int curNodeInd, int curLeft, int curRight);

	void GetNeededNodes(vector<NodeInfo>& neededNodes, int left, int right, int curNodeInd, int curLeft, int curRight);

	int GetNeededIndex(int k, int curNodeInd, int curLeft, int curRight);

	int KthZeroIndex_rec(int k, int left, int right, int curNodeInd, int curLeft, int curRight, int& curK);

	//int SegmMaxIndex_rec(int left, int right, int& res, value_t& foundMax, int curNodeInd, int curLeft, int curRight);

	int CountZerosSegm_rec(int left, int right, int curNodeInd, int curleft, int curRight);
};


void SegmTree::Create(vector<value_t> values, int curNodeInd, int curLeft, int curRight) {
	if (curLeft == curRight) {
		data[curNodeInd].value = values[curLeft];
		data[curNodeInd].NZeros = (values[curLeft] == 0) ? 1 : 0;
		//data[curNodeInd].maxElem = values[curLeft];
		//data[curNodeInd].maxIndex = curLeft;

	} else {
		int curMiddle = (curLeft + curRight) / 2;
		Create(values, curNodeInd * 2, curLeft, curMiddle);
		Create(values, curNodeInd * 2 + 1, curMiddle + 1, curRight);

		data[curNodeInd].value = {};

		data[curNodeInd].NZeros = data[curNodeInd * 2].NZeros + data[curNodeInd * 2 + 1].NZeros;

		//data[curNodeInd].maxElem = max(data[curNodeInd * 2].maxElem, data[curNodeInd * 2 + 1].maxElem);
		//if (data[curNodeInd].maxElem == data[curNodeInd * 2].maxElem) {
		//	data[curNodeInd].maxIndex = data[curNodeInd * 2].maxIndex;
		//} else {
		//	assert(data[curNodeInd].maxElem == data[curNodeInd * 2 + 1].maxElem);

		//	data[curNodeInd].maxIndex = data[curNodeInd * 2 + 1].maxIndex;
		//}
	}
}


void SegmTree::Create(ifstream& fin, int curNodeInd, int curLeft, int curRight) {
	if (curLeft == curRight) {
		fin >> data[curNodeInd].value;
		data[curNodeInd].NZeros = (data[curNodeInd].value == 0) ? 1 : 0;
		//data[curNodeInd].maxElem = data[curNodeInd].value;
		//data[curNodeInd].maxIndex = curLeft;

	} else {
		int curMiddle = (curLeft + curRight) / 2;
		Create(fin, curNodeInd * 2, curLeft, curMiddle);
		Create(fin, curNodeInd * 2 + 1, curMiddle + 1, curRight);

		data[curNodeInd].value = {};

		data[curNodeInd].NZeros = data[curNodeInd * 2].NZeros + data[curNodeInd * 2 + 1].NZeros;

		//data[curNodeInd].maxElem = max(data[curNodeInd * 2].maxElem, data[curNodeInd * 2 + 1].maxElem);
		//if (data[curNodeInd].maxElem == data[curNodeInd * 2].maxElem) {
		//	data[curNodeInd].maxIndex = data[curNodeInd * 2].maxIndex;
		//} else {
		//	assert(data[curNodeInd].maxElem == data[curNodeInd * 2 + 1].maxElem);

		//	data[curNodeInd].maxIndex = data[curNodeInd * 2 + 1].maxIndex;
		//}
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
		//data[curNodeInd].maxElem = newVal;

	} else {
		int curMiddle = (curLeft + curRight) / 2;
		if (index <= curMiddle) {
			Update_rec(index, newVal, curNodeInd * 2, curLeft, curMiddle);

		} else {
			Update_rec(index, newVal, curNodeInd * 2 + 1, curMiddle + 1, curRight);
		}

		data[curNodeInd].value = {};

		data[curNodeInd].NZeros = data[curNodeInd * 2].NZeros + data[curNodeInd * 2 + 1].NZeros;

		//data[curNodeInd].maxElem = max(data[curNodeInd * 2].maxElem, data[curNodeInd * 2 + 1].maxElem);
		//if (data[curNodeInd].maxElem == data[curNodeInd * 2].maxElem) {
		//	data[curNodeInd].maxIndex = data[curNodeInd * 2].maxIndex;
		//} else {
		//	assert(data[curNodeInd].maxElem == data[curNodeInd * 2 + 1].maxElem);

		//	data[curNodeInd].maxIndex = data[curNodeInd * 2 + 1].maxIndex;
		//}
	}
}


int SegmTree::KthZeroIndex(int k, int left, int right) {
	assert(k > 0);
	assert(left >= 0);
	assert(right >= left);
	assert(right <= NValues - 1);

	vector<NodeInfo> neededNodes;
	GetNeededNodes(neededNodes, left, right, 1, 0, NValues - 1);

	int restK = k;
	bool exists = false;
	int res = 0;
	for (int i = 0; i < neededNodes.size(); ++i) {
		if (data[neededNodes[i].index].NZeros >= restK) {
			exists = true;
			res = GetNeededIndex(restK, neededNodes[i].index, neededNodes[i].left, neededNodes[i].right);
			break;

		} else {
			restK -= data[neededNodes[i].index].NZeros;
		}
	}
	
	if (exists) {
		return res;
	} else {
		return -1;
	}
}


void SegmTree::GetNeededNodes(vector<NodeInfo>& neededNodes, int left, int right, int curNodeInd, int curLeft, int curRight) {
	assert(left >= 0);
	assert(curNodeInd > 0);
	assert(curLeft >= 0);
	assert(curRight >= curLeft);
	assert(curRight < NValues);

	if (left > right) {
		return;
	}

	if (left == curLeft && right == curRight) {
		neededNodes.push_back( {curNodeInd, curLeft, curRight} );
		return;
	}

	int curMiddle = (curLeft + curRight) / 2;
	GetNeededNodes(neededNodes, left, min(right, curMiddle), curNodeInd * 2, curLeft, curMiddle);
	GetNeededNodes(neededNodes, max(left, curMiddle + 1), right, curNodeInd * 2 + 1, curMiddle + 1, curRight);
}


int SegmTree::GetNeededIndex(int k, int curNodeInd, int curLeft, int curRight) {
	assert(k > 0);
	assert(k <= data[curNodeInd].NZeros);
	assert(curNodeInd > 0);
	assert(curLeft >= 0);
	assert(curRight >= curLeft);
	assert(curRight < NValues);

	if (curLeft == curRight) {
		assert(k == 1);
		assert(data[curNodeInd].value == 0);

		return curLeft;
	}

	int curMiddle = (curLeft + curRight) / 2;
	int leftZeros = data[curNodeInd * 2].NZeros;
	if (leftZeros >= k) {
		return GetNeededIndex(k, curNodeInd * 2, curLeft, curMiddle);
	} else {
		return GetNeededIndex(k - leftZeros, curNodeInd * 2 + 1, curMiddle + 1, curRight);
	}
}


//int SegmTree::KthZeroIndex_rec(int k, int left, int right, int curNodeInd, int curLeft, int curRight, int& curK) {
//	if (left > right) {
//		return -1;
//	}
//
//	if (curLeft == curRight) {
//		assert(data[curNodeInd].NZeros == 1 || data[curNodeInd].NZeros == 0);
//
//		if (data[curNodeInd].NZeros > curK) {
//			curK = data[curNodeInd].NZeros;
//		}
//
//		if (k == 1 && data[curNodeInd].NZeros == 1) {
//			return curLeft;
//		} else {
//			return -1;
//		}
//	}
//
//	if (left == curLeft && right == curRight) {
//		if (data[curNodeInd].NZeros > curK) {
//			curK = data[curNodeInd].NZeros;
//		}
//
//		if (k > data[curNodeInd].NZeros) {
//			return -1;
//		} else {
//			int curMiddle = (curLeft + curRight) / 2;
//
//			if (k <= data[curNodeInd * 2].NZeros) {
//				return KthZeroIndex_rec(k, left, curMiddle, curNodeInd * 2, curLeft, curMiddle, curK);
//			} else {
//				return KthZeroIndex_rec(k - data[curNodeInd * 2].NZeros, curMiddle + 1, right,
//					                    curNodeInd * 2 + 1, curMiddle + 1, curRight, curK);
//			}
//		}
//	}
//
//	int curMiddle = (curLeft + curRight) / 2;
//	curK = 0;
//	int leftZeroInd = KthZeroIndex_rec(k, left, min(right, curMiddle), curNodeInd * 2, curLeft, curMiddle, curK);
//	if (leftZeroInd == -1) {
//		return KthZeroIndex_rec(k - curK, max(left, curMiddle + 1), right, curNodeInd * 2 + 1, curMiddle + 1, curRight, curK);
//	} else {
//		return leftZeroInd;
//	}
//}


//int SegmTree::SegmMaxIndex(int left, int right) {
//	assert(left >= 0);
//	assert(right >= left);
//	assert(right <= NValues - 1);
//
//	int res = 0;
//	value_t foundMax = 0;
//	SegmMaxIndex_rec(left, right, res, foundMax, 1, 0, NValues - 1);
//
//	return res;
//}
//
//
//value_t SegmTree::SegmMaxIndex_rec(int left, int right, int& foundInd, value_t& foundMax, int curNodeInd, int curLeft, int curRight) {
//	if (left > right) {
//		return 0;
//	}
//
//	if (left == curLeft && right == curRight) {
//		if (data[curNodeInd].maxElem >= foundMax) {
//			foundInd = data[curNodeInd].maxIndex;
//			foundMax = data[curNodeInd].maxElem;
//		}
//		return data[curNodeInd].maxElem;
//	}
//
//	int curMiddle = (curLeft + curRight) / 2;
//	value_t res = max(SegmMaxIndex_rec(left, min(right, curMiddle), foundInd, foundMax, curNodeInd * 2, curLeft, curMiddle),
//		SegmMaxIndex_rec(max(left, curMiddle + 1), right, foundInd, foundMax, curNodeInd * 2 + 1, curMiddle + 1, curRight));
//
//	return res;
//}


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

// End of segment tree realization





void ParseCommand(SegmTree& elemTree){
	char curCommand = 0;
	cin >> curCommand;

	switch (curCommand) {
	case 's': {
		int left = 0, right = 0, k = 0;
		cin >> left >> right >> k;

		int ans = elemTree.KthZeroIndex(k, left - 1, right - 1) + 1;
		cout << ((ans == 0) ? -1 : ans) << '\n';
		break;
	}

	case 'u': {
		int index = 0, value = 0;
		cin >> index >> value;

		elemTree.Update(index - 1, value);
		break;
	}

	default: {
		assert(0);
	}
	}
}


int main() {
	//freopen("input.txt", "r", stdin);
	//freopen("output.txt", "w", stdout);

	ios_base::sync_with_stdio(false);
	cin.tie(0);

	int N = 0, M = 0;
	cin >> N;

	//vector<int> elements(N);
	//for (int i = 0; i < N; ++i) {
	//	cin >> elements[i];
	//}
	ifstream fin(stdin);
	SegmTree elemTree(fin, N);

	cin >> M;
	for (int i = 0; i < M; ++i) {
		ParseCommand(elemTree);
	}

	//double time = static_cast<double>(clock()) / CLOCKS_PER_SEC;

	return 0;
}