#include <iostream>
#include <vector>
#include <time.h>

using namespace std;


class BITree {
private:
	vector<int> arr;
	vector<int> inarr;
public:
	BITree(vector<int>& arr) : arr(arr), inarr(arr.size(), 0) {
		arr[0] = arr[0] == 0;
		for (int i = 1; i < arr.size(); ++i) {
			arr[i] = arr[i - 1] + int(arr[i] == 0);
		}

		for (int i = 0; i < inarr.size(); ++i) {
			int idx = i & (i + 1);
			inarr[i] = arr[i];
			inarr[i] -= idx ? arr[idx - 1] : 0;
		}
	}

	int prefix(int pos) {
		int ans = 0;
		for (int i = pos; i >= 0; i = (i & (i + 1)) - 1)
			ans += inarr[i];

		return ans;
	}

	int segment(int l, int r) {
		return prefix(r) - prefix(l - 1);
	}

	void update(int pos, int val) {
		int old = arr[pos];
		arr[pos] = val;

		for (int i = pos; i < inarr.size(); i = i | (i + 1))
			inarr[i] += int(val == 0) - int(old == 0);
	}

	int index(int l, int r, int k) {
		if (segment(l, r) < k)
			return -1;

		while (l < r) {
			int mid = (l + r) / 2;
			int count = segment(l, mid);

			if (k > count) {
				l = mid + 1;
				k -= count;
			} else r = mid;
		}

		return l;
	}
};

int main1() {
	freopen("input.txt", "r", stdin);
	freopen("output.txt", "w", stdout);

	ios_base::sync_with_stdio(false);
	cin.tie(0);

	int n, m;
	cin >> n;

	vector<int> arr(n);
	for (auto& i : arr)
		cin >> i;

	BITree tree(arr);

	cin >> m;
	while (m--) {
		char c;
		cin >> c;

		if (c == 'u') {
			int pos, val;
			cin >> pos >> val;
			tree.update(--pos, val);
		}

		if (c == 's') {
			int l, r, k;
			cin >> l >> r >> k;
			int ans = tree.index(--l, --r, k);
			cout << (ans == -1 ? ans : ans + 1) << ' ';
		}
	}

	double time = static_cast<double>(clock()) / CLOCKS_PER_SEC;
	
	return 0;
}