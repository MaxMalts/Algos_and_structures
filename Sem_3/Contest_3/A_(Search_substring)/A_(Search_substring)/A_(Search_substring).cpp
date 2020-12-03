//#include <iostream>
//#include <vector>
//#include <string>
//#include <assert.h>
//
//
//template<long long base, long long exp>
//struct Power {
//	static const long long res = base * Power<base, exp - 1>::res;
//};
//
//template<long long base>
//struct Power<base, 0> {
//	static const long long res = 1;
//};
//
//
//std::vector<long long> powP(50000);
//
//
//long long PrefixHash(const std::string str, const long long end, const long long p) {
//	assert(str.size() > 0);
//	assert(end >= 0 && end < str.size());
//	assert(p > 0);
//
//	long long res = 0;
//	for (long long i = 0; i <= end; ++i)
//		res = res * p + str[i];
//
//	return res;
//}
//
//
//void FindSubstrings(const std::string T, const std::string S, const long long THash,
//                    const std::vector<long long> prefHashes, const long long p) {
//
//	assert(T.size() > 0);
//	assert(S.size() > 0);
//	assert(prefHashes.size() == S.size());
//
//	for (long long i = 0; i <= S.size() - T.size(); ++i) {
//		long long beg = i, end = i + T.size();
//		long long curSHash = prefHashes[end - 1] - (beg > 0 ? prefHashes[beg - 1] * powP[p, end - beg] : 0);
//		if (THash == curSHash)
//			std::cout << i << ' ';
//	}
//}
//
//
//int main() {
//
//	const long long p = 2;
//	powP[0] = 1;
//	for (int i = 1; i < 50000; ++i)
//		powP[i] = powP[i - 1] * p;
//
//	std::string S, T;
//
//	std::cin >> S >> T;
//
//	std::vector<long long> prefHashes(S.size());
//	for (long long i = 0; i < S.size(); ++i)
//		prefHashes[i] = PrefixHash(S, i, p);
//	long long THash = PrefixHash(T, T.size() - 1, p);
//
//	FindSubstrings(T, S, THash, prefHashes, p);
//
//	return 0;
//}



#include <iostream>
#include <string>



int main() {
	std::string S, T;
	std::cin >> S >> T;

	for (int i = 0; i <= S.size() - T.size(); ++i)
		if (S.compare(i, T.size(), T) == 0)
			std::cout << i << " ";

	return 0;
}