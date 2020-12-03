#include <iostream>
#include <string>
#include <vector>
#include <assert.h>



void SufArray(std::string str, std::vector<int>& res) {

	const int maxCharCode = CHAR_MAX;
	const int strLen = str.length();

	res.resize(strLen);
	std::vector<int> nClassElems(maxCharCode + 1), c(strLen);
	for (int i = 0; i < strLen; ++i) {
		++nClassElems.at(str.at(i));
	}
	for (int i = 1; i < nClassElems.size(); ++i) {
		nClassElems.at(i) += nClassElems.at(i - 1);
	}
	for (int i = 0; i < strLen; ++i) {
		res.at(--nClassElems.at(str.at(i))) = i;
	}

	int nClasses = 1;
	for (int i = 1; i < strLen; ++i) {
		if (str.at(res.at(i)) != str.at(res.at(i - 1))) {
			++nClasses;
		}

		c.at(res.at(i)) = nClasses - 1;
	}


	std::vector<int> secondSort(strLen), newC(strLen);
	int curLen = 0;
	for (int k = 0; (curLen = (1 << k)) < strLen; ++k) {
		for (int i = 0; i < strLen; ++i) {
			secondSort.at(i) = (res.at(i) + strLen - curLen) % strLen;
		}

		nClassElems.assign(nClasses + 1, 0);
		for (int i = 0; i < strLen; ++i) {
			++nClassElems.at(c.at(secondSort.at(i)));
		}
		for (int i = 1; i < nClasses; ++i) {
			nClassElems.at(i) += nClassElems.at(i - 1);
		}
		for (int i = strLen - 1; i >= 0; --i) {
			res.at(--nClassElems.at(c.at(secondSort.at(i)))) = secondSort.at(i);
		}

		newC.at(res.at(0)) = 0;
		nClasses = 1;
		for (int i = 1; i < strLen; ++i) {
			int curMid = (res.at(i) + curLen) % strLen;
			int prevMid = (res.at(i - 1) + curLen) % strLen;
			if (c.at(res.at(i)) != c.at(res.at(i - 1)) || c.at(curMid) != c.at(prevMid)) {
				++nClasses;
			}

			newC.at(res.at(i)) = nClasses - 1;
		}
		c = newC;
	}
}


int main() {
	std::string str;
	std::cin >> str;
	str.append(1, '$');

	std::vector<int> ans;
	SufArray(str, ans);

	for (int i = 1; i < ans.size(); ++i) {
		std::cout << ans.at(i) + 1 << ' ';
	}

	return 0;
}