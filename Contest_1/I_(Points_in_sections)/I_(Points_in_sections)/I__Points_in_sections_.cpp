#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <iostream>
#include <iostream>
#include <algorithm>

using namespace std;

enum point_type {
	section_left,
	section_right,
	single_point
};

struct point_t {
	int coordinate = 0;
	point_type type = {};
	int ind = 0;
};

void ParseSections(int n, point_t sections[]) {
	assert(n > 0);
	assert(sections != NULL);

	int left = 0, right = 0;
	for (int i = 0; i < n; ++i) {
		scanf("%d %d", &left, &right);
		if (left > right) {
			swap(left, right);
		}

		sections[2 * i].coordinate = left;
		sections[2 * i].type = section_left;
		sections[2 * i + 1].coordinate = right;
		sections[2 * i + 1].type = section_right;
	}
}


bool PointCmp(point_t point1, point_t point2) {
	if (point1.coordinate < point2.coordinate) {
		return true;

	} else if (point1.coordinate > point2.coordinate) {
		return false;

	} else {
		if (point1.type == point2.type) {
			return false;

		} else if (point1.type == section_left && point2.type == section_right) {
			return true;

		} else if (point1.type == section_right && point2.type == section_left) {
			return false;

		} else if (point1.type == single_point && point2.type == section_left) {
			return false;

		} else if (point1.type == single_point && point2.type == section_right) {
			return true;

		} else if (point1.type == section_left && point2.type == single_point) {
			return true;

		} else if (point1.type == section_right && point2.type == single_point) {
			return false;

		}
	}
}


void SortPoints(int n, int m, point_t allPoints[]) {
	assert(n > 0);
	assert(allPoints != NULL);

	sort(allPoints, allPoints + 2 * n + m, PointCmp);
}


void CountSectionsForPoints(int n, int m, point_t allPoints[], int output[]) {
	assert(n > 0);
	assert(m > 0);
	assert(allPoints != NULL);
	assert(output != NULL);

	int curNSections = 0;

	for (int i = 0; i < 2 * n + m; ++i) {
		if (allPoints[i].type == section_left) {
			++curNSections;

		} else if (allPoints[i].type == section_right) {
			--curNSections;

		} else {
			assert(allPoints[i].type == single_point);

			output[allPoints[i].ind] = curNSections;
		}
	}
}


void ParsePoints(int n, int m, point_t allPoints[]){
	assert(n > 0);
	assert(m > 0);
	assert(allPoints != NULL);

	for (int i = 2 * n; i < 2 * n + m; ++i) {
		scanf("%d", &allPoints[i].coordinate);
		allPoints[i].type = single_point;
		allPoints[i].ind = i - 2 * n;
	}
}


void OutputAns(int m, int ans[]) {
	assert(m > 0);
	assert(ans != NULL);

	for (int i = 0; i < m; ++i) {
		printf("%d ", ans[i]);
	}
}


int main() {
	const int maxN = 100000;
	const int maxM = 100000;

	int n = 0, m = 0;
	scanf("%d %d", &n, &m);

	point_t allPoints[2 * maxN + maxM] = {};
	int ans[maxM] = { 0 };
	ParseSections(n, allPoints);
	ParsePoints(n, m, allPoints);

	SortPoints(n, m, allPoints);

	CountSectionsForPoints(n, m, allPoints, ans);

	OutputAns(m, ans);

	return 0;
}