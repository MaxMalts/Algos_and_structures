#include <iostream>
#include <string>
#include <vector>
#include <stack>
#include <cmath>
#include <fstream>
#include <assert.h>

#pragma GCC optimize(3)
#pragma GCC optimize("unroll-loops")

using namespace std;


#ifndef _DEBUG
#define assert(cond) ;
#endif


long long Log2(long long x) {

	long long res = 0;
	while (x > 1) {
		x >>= 1;
		++res;
	}

	return res;
}


typedef long long value_t;

struct Node {
	value_t maxValue = {};
	long long index = 0;
};

class SparceTable {
public:

	SparceTable(vector<value_t>& elements) {
		NValues = elements.size();
		height = Log2(NValues) + 1;

		if (table.empty()) {
			table = vector<vector<Node>>(NValues, vector<Node>(height));
		}

		for (long long i = 0; i < NValues; ++i) {
			table[i][0].maxValue = elements[i];
			table[i][0].index = i;
		}

		ConstructNext();

		PrepareLogs2(NValues);
	}


	SparceTable(istream& fin, long long NValues) : NValues(NValues) {
		height = Log2(NValues) + 1;

		table = vector<vector<Node>>(NValues, vector<Node>(height));

		for (long long i = 0; i < NValues; ++i) {
			cin >> table[i][0].maxValue;
			table[i][0].index = i;
		}

		ConstructNext();

		//log2_x = vector<long long>(NValues + 1, 0);
		PrepareLogs2(NValues);
	}


	long long SegmMinIndex(long long left, long long right) {
		assert(left >= 0);
		assert(right >= left);
		assert(right < NValues);

		long long len = right - left + 1;

		/*if (log2_x[len] == 0) {
			log2_x[len] = floor(log2(len));
		}*/

		long long log2_len = log2_x[len];

		if (table[left][log2_len].maxValue <= table[right - (1 << log2_len) + 1][log2_len].maxValue) {
			return table[left][log2_len].index;
		} else {
			return table[right - (1 << log2_len) + 1][log2_len].index;
		}
	}


private:
	long long height = 0;

	static vector<vector<Node>> table;

	long long NValues = 0;

	static vector<long long> log2_x;

	void ConstructNext() {

		for (long long j = 1; j < height; ++j) {
			for (long long i = 0; i + (1 << j) <= NValues; ++i) {

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

	static void PrepareLogs2(long long NValues) {
		if (log2_x.empty()) {
			log2_x = vector<long long>(NValues + 1);

			for (long long i = 0; i <= NValues; ++i) {
				log2_x[i] = Log2(i);
			}
		}
	}
};

vector<vector<Node>> SparceTable::table;
vector<long long> SparceTable::log2_x;

// End of Sparce Table realization



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
		SparceTable lcpSparceTable(lcp);
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


int main() {
	ios_base::sync_with_stdio(false);
	std::cin.tie(0);

	long long n = 0, m = 0;
	std::cin >> n >> m;
	assert(m > 0 && m <= 10);

	std::string str(n, 0);
	for (long long i = 0; i < n; ++i) {
		long long curNum = 0;
		std::cin >> curNum;
		assert(curNum >= 0 && curNum <= m);

		str.at(i) = '$' + 1 + static_cast<char>(curNum);
	}

	std::vector<long long> sufArray;
	std::vector<long long> lcp;
	str.append(1, '$');
	CreateSufArray(str, sufArray, lcp);


	struct StackData {

		StackData(const long long& length, const long long& nEntries, const long long& id)
			: length(length), nEntries(nEntries), id(id) {}

		long long length = 0, nEntries = 0, id = 0;
	};

	std::stack<StackData> stack;
	long long maxLen = n, maxEntries = 1, subseqBegInd = 0;
	for (long long i = 0; i < n; i++) {
		long long curEntries = 1;
		while (!stack.empty() && lcp.at(i + 1) <= stack.top().length) {
			StackData curData = stack.top();
			stack.pop();

			curEntries += curData.nEntries;
			if (curEntries * curData.length > maxEntries * maxLen) {
				maxEntries = curEntries;
				maxLen = curData.length;
				subseqBegInd = sufArray.at(curData.id + 1);
			}
		}

		stack.emplace(lcp.at(i + 1), curEntries, i);
	}

	std::cout << maxLen * maxEntries << '\n' << maxLen << '\n';
	for (long long i = subseqBegInd; i < subseqBegInd + maxLen; ++i) {
		long long curNum = static_cast<long long>(str.at(i) - '$' - 1);
		std::cout << curNum << ' ';
	}

	return 0;
}