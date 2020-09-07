#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <assert.h>

using namespace std;



int main() {
	int N = 0, K = 0;
	cin >> N >> K;

	string str;
	cin >> str;

	int ans = 0;
	for (int curCh = 0; curCh < N; ++curCh) {
		int lim = K;
		for (int i = 0; curCh - i >= 0 && curCh + i < N; ++i) {
			if (str[curCh - i] != str[curCh + i]) {
				if (lim > 0) {
					--lim;
				} else {
					break;
				}
			}

			++ans;
		}

		lim = K;
		for (int i = 0; curCh - i - 1 >= 0 && curCh + i < N; ++i) {
			if (str[curCh - i - 1] != str[curCh + i]) {
				if (lim > 0) {
					--lim;
				} else {
					break;
				}
			}

			++ans;
		}
	}

	cout << ans;

	return 0;
}