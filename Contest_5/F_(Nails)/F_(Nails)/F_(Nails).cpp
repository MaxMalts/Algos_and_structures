#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <assert.h>

using namespace std;


int main() {
	int N = 0;
	cin >> N;

	vector<int> nails(N);
	for (int i = 0; i < N; ++i) {
		cin >> nails[i];
	}
	sort(nails.begin(), nails.end());

	vector<int> ans(N + 1);
	ans[0] = 0;
	ans[1] = 2147483647;
	for (int i = 2; i <= N; ++i) {
		ans[i] = min(ans[i - 1], ans[i - 2]) + abs(nails[i - 1] - nails[i - 2]);
	}

	cout << ans[N];

	return 0;
}