#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stack>

using namespace std;

struct rect_t {
	int pos = 0;
	int height = 0;
};


int main() {
	int N = 0;
	scanf("%d", &N);

	stack<rect_t> rectsData;
	rectsData.push({ 0, -1 });

	int h_i = 0;
	int prevPos = 0;
	int prevHeight = 0;
	long long area = 0;
	long long maxArea = 0;
	for (int i = 1; i <= N + 1; ++i) {
		if (i <= N) {
			scanf("%d", &h_i);
		} else {
			h_i = 0;
		}

		prevPos = i;
		while (h_i <= rectsData.top().height) {
			prevPos = rectsData.top().pos;
			prevHeight = rectsData.top().height;
			rectsData.pop();

			area = (long long)prevHeight * (i - prevPos);

			if (area > maxArea) {
				maxArea = area;
			}
		}

		rectsData.push({ prevPos, h_i });
	}

	printf("%lld", maxArea);

	return 0;
}