#undef _DEBUG

#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <fstream>
#include <assert.h>

#pragma GCC optimize(3)
#pragma GCC optimize("unroll-loops")

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


typedef long long value_t;

struct Node {
	value_t maxValue = {};
	int index = 0;
};

class SparceTable {
public:

	SparceTable(vector<value_t>& elements) {
		NValues = elements.size();
		height = Log2(NValues) + 1;

		if (table.empty()) {
			table = vector<vector<Node>>(NValues, vector<Node>(height));
		}

		for (int i = 0; i < NValues; ++i) {
			table[i][0].maxValue = elements[i];
			table[i][0].index = i;
		}

		ConstructNext();

		PrepareLogs2(NValues);
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
		PrepareLogs2(NValues);
	}


	int SegmMinIndex(int left, int right) {
		assert(left >= 0);
		assert(right >= left);
		assert(right < NValues);

		int len = right - left + 1;

		/*if (log2_x[len] == 0) {
			log2_x[len] = floor(log2(len));
		}*/

		int log2_len = log2_x[len];

		if (table[left][log2_len].maxValue <= table[right - (1 << log2_len) + 1][log2_len].maxValue) {
			return table[left][log2_len].index;
		} else {
			return table[right - (1 << log2_len) + 1][log2_len].index;
		}
	}


private:
	int height = 0;

	static vector<vector<Node>> table;

	int NValues = 0;

	static vector<int> log2_x;

	void ConstructNext() {

		for (int j = 1; j < height; ++j) {
			for (int i = 0; i + (1 << j) <= NValues; ++i) {

				if (table[i][j - 1].maxValue <= table[i + (1 << (j - 1))][j - 1].maxValue) {
					table[i][j].maxValue = table[i][j - 1].maxValue;
					table[i][j].index = table[i][j - 1].index;

				} else {
					table[i][j].maxValue = table[i + (1 << (j - 1))][j - 1].maxValue;
					table[i][j].index = table[i + (1 << (j - 1))][j - 1].index;
				}
			}
		}
	}

	static void PrepareLogs2(int NValues) {
		if (log2_x.empty()) {
			log2_x = vector<int>(NValues + 1);

			for (int i = 0; i <= NValues; ++i) {
				log2_x[i] = Log2(i);
			}
		}
	}
};

vector<vector<Node>> SparceTable::table;
vector<int> SparceTable::log2_x;

// End of Sparce Table realization



//long long FindMin(std::vector<long long>& arr, long long begin, long long end) {
//	assert(begin <= end);
//
//	long long res = arr.at(begin);
//	for (long long i = begin + 1; i <= end; ++i) {
//		if (arr.at(i) < res) {
//			res = arr.at(i);
//		}
//	}
//
//	return res;
//}


void CreateSufArray(const std::string& str, std::vector<long long>& sufArray, std::vector<long long>& lcp) {
	assert(str.size() > 0);

	const long long maxCharCode = CHAR_MAX;
	const long long strLen = str.length();

	sufArray.resize(strLen);
	std::vector<long long> nClassElems(maxCharCode + 1), c(strLen);
	for (long long i = 0; i < strLen; ++i) {
		++nClassElems.at(str.at(i));
	}
	for (long long i = 1; i < nClassElems.size(); ++i) {
		nClassElems.at(i) += nClassElems.at(i - 1);
	}
	for (long long i = 0; i < strLen; ++i) {
		sufArray.at(--nClassElems.at(str.at(i))) = i;
	}

	long long nClasses = 1;
	for (long long i = 1; i < strLen; ++i) {
		if (str.at(sufArray.at(i)) != str.at(sufArray.at(i - 1))) {
			++nClasses;
		}

		c.at(sufArray.at(i)) = nClasses - 1;
	}


	std::vector<long long> secondSort(strLen), newC(strLen);
	long long curLen = 0;
	lcp.assign(str.size(), 0);
	std::vector<long long> newLcp(strLen), where(strLen), lastInEquivC(strLen), firstInEquivC(strLen);
	for (long long k = 0; (curLen = (1LL << k)) < strLen; ++k) {
		for (long long i = 0; i < strLen; ++i) {
			where.at(sufArray.at(i)) = i;
			lastInEquivC.at(c.at(sufArray.at(i))) = i;
			firstInEquivC.at(c.at(sufArray.at(strLen - 1 - i))) = strLen - 1 - i;
		}


		for (long long i = 0; i < strLen; ++i) {
			secondSort.at(i) = (sufArray.at(i) + strLen - curLen) % strLen;
		}

		nClassElems.assign(nClasses + 1, 0);
		for (long long i = 0; i < strLen; ++i) {
			++nClassElems.at(c.at(secondSort.at(i)));
		}
		for (long long i = 1; i < nClasses; ++i) {
			nClassElems.at(i) += nClassElems.at(i - 1);
		}
		for (long long i = strLen - 1; i >= 0; --i) {
			sufArray.at(--nClassElems.at(c.at(secondSort.at(i)))) = secondSort.at(i);
		}

		newC.at(sufArray.at(0)) = 0;
		nClasses = 1;
		for (long long i = 1; i < strLen; ++i) {
			long long curMid = (sufArray.at(i) + curLen) % strLen;
			long long prevMid = (sufArray.at(i - 1) + curLen) % strLen;
			if (c.at(sufArray.at(i)) != c.at(sufArray.at(i - 1)) || c.at(curMid) != c.at(prevMid)) {
				++nClasses;
			}

			newC.at(sufArray.at(i)) = nClasses - 1;
		}


		std::vector<long long>& oldC = c;
		SparceTable lcpSparceTable (lcp);
		for (long long i = 0; i < strLen - 1; ++i) {
			long long curSuf = sufArray.at(i);
			long long nextSuf = sufArray.at(i + 1);

			if (oldC.at(curSuf) != oldC.at(nextSuf)) {
				newLcp.at(i) = lcp.at(lastInEquivC.at(oldC.at(curSuf)));

			} else {
				long long curMid = (curSuf + curLen) % strLen;
				long long nextMid = (nextSuf + curLen) % strLen;
				newLcp.at(i) = curLen +
					lcp.at(lcpSparceTable.SegmMinIndex(firstInEquivC.at(oldC.at(curMid)),
					                                   lastInEquivC.at(oldC.at(nextMid)) - 1));
			}
		}
		lcp = newLcp;
		c = newC;
	}
}


std::string KthSubstring(const std::string& str, const long long k,
                         const std::vector<long long>& sufArray, const std::vector<long long>& lcp) {
	assert(str.size() > 0);
	assert(k > 0);

	long long strLen = str.size() - 1;
	std::string res;

	long long sufArrayCursor = 1;
	long long i = k;
	while (sufArrayCursor < sufArray.size()) {
		if (i <= strLen - sufArray.at(sufArrayCursor) - lcp.at(sufArrayCursor - 1)) {
			res = str.substr(sufArray.at(sufArrayCursor), lcp.at(sufArrayCursor - 1) + i);
			return res;
		}

		i -= strLen - sufArray.at(sufArrayCursor) - lcp.at(sufArrayCursor - 1);
		++sufArrayCursor;
	}

	res = str.substr(sufArray.at(sufArray.size() - 1));
	res.pop_back();
	return res;
}


int main() {
	ios_base::sync_with_stdio(false);
	std::cin.tie(0);

	std::string str;
	long long k = 0;
	std::cin >> str >> k;
	str.append(1, '$');

	std::vector<long long> sufArray;
	std::vector<long long> lcp;
	CreateSufArray(str, sufArray, lcp);

	//for (long long i = 0; i < sufArray.size(); ++i) {
	//	prlong longf("%-10s%d\n", &str.c_str()[sufArray[i]], lcp[i]);
	//}

	std::string ans = KthSubstring(str, k, sufArray, lcp);

	std::cout << ans;

	return 0;
}