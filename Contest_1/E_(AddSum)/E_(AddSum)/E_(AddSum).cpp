#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <iostream>

unsigned int a, b;
unsigned int cur = 0;
unsigned int nextRand() {
	
	cur = cur * a + b;

	return cur >> 8;
}


void DoAdds(int m, long long arr[]) {
	assert(arr != NULL);

	for (int i = 0; i < m; ++i) {
		unsigned int add = nextRand();
		unsigned int l = nextRand();
		unsigned int r = nextRand();
		if (l > r)
			std::swap(l, r);

		arr[l] += add;
		arr[r + 1] -= add;
	}
}


unsigned int DoSums(int q, int sums[]) {
	assert(sums != NULL);

	unsigned int sum = 0;

	unsigned int l = 0;
	unsigned int r = 0;

	for (int i = 0; i < q; ++i) {
		l = nextRand();
		r = nextRand();
		if (l > r)
			std::swap(l, r);

		if (l > 0)
			sum += sums[r] - sums[l - 1];
		else
			sum += sums[r];
	}

	return sum;
}


int main() {
	const int n = 16777216;
	int m = 0, q = 0;
	scanf("%d %d\n%d %d", &m, &q, &a, &b);

	long long arr[n + 1] = { 0 };

	DoAdds(m, arr);

	int curSum = 0;
	int partSum = 0;
	int sums[n] = {};
	for (int i = 0; i < n; ++i) {
		curSum += arr[i];
		partSum += curSum;
		sums[i] = partSum;
	}

	unsigned int sum = DoSums(q, sums);

	printf("%u", sum);

	return 0;
}