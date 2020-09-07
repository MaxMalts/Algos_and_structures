#include <iostream>
#include <vector>
#include<assert.h>

using namespace std;


int main() {
	int N = 0, M = 0;
	cin >> N >> M;
	if (M < N) {
		swap(N, M);
	}

	int ans = 0;
	if (N == 1) {
		ans = M;

	} else if (N == 2) {
		ans = 4 * (((M - 1) / 4) + 1);
		if (M % 4 == 1) {
			ans -= 2;
		}

	} else {
		ans = (N * M) / 2 + (N * M) % 2;
	}


	cout << ans;
	return 0;
}