#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

void ReadArr(FILE* fin, int a[100000], int N) {
	assert(fin != NULL);
	assert(a != NULL);
	assert(N > 0);

	for (int i = 0; i < N; ++i) {
		fscanf(fin, "%d", &a[i]);
	}
}


int merge(int arrSize, int leftSize, int rightSize, int arr[], int left[], int right[]) {
	assert(arr != NULL);
	assert(left != NULL);
	assert(right != NULL);
	assert(rightSize > 0);
	assert(leftSize > 0);
	assert(arrSize > 0);

	int i = 0, j = 0, count = 0;
	while (i < leftSize || j < rightSize) {
		if (i == leftSize) {
			arr[i + j] = right[j];
			j++;
		} else if (j == rightSize) {
			arr[i + j] = left[i];
			i++;
		} else if (left[i] <= right[j]) {
			arr[i + j] = left[i];
			i++;
		} else {
			arr[i + j] = right[j];
			count += leftSize - i;
			j++;
		}
	}
	return count;
}


void CopyArrayRng(int* dest, int* src, int first, int last) {
	assert(dest != NULL);
	assert(src != NULL);
	assert(first >= 0);
	assert(last >= first);

	for (int i = first; i <= last; ++i) {
		dest[i - first] = src[i];
	}
}


int CountInversions(int arrSize, int arr[]) {
	assert(arrSize > 0);
	assert(arr != NULL);

	if (arrSize < 2) {
		return 0;
	}

	int leftSize = (arrSize + 1) / 2;
	int rightSize = arrSize - leftSize;
	int* left = new int[leftSize];
	int* right = new int[rightSize];
	CopyArrayRng(left, arr, 0, leftSize - 1);
	CopyArrayRng(right, arr, leftSize, arrSize - 1);

	int res = CountInversions(leftSize, left) + CountInversions(rightSize, right) +
		merge(arrSize, leftSize, rightSize, arr, left, right);

	delete[](left);
	delete[](right);
	return res;
}



int main() {
	FILE* fin = fopen("inverse.in", "r");
	int N = 0;
	fscanf(fin, "%d", &N);

	int a[100000] = {};
	ReadArr(fin, a, N);
	fclose(fin);

	int ans = CountInversions(N, a);

	FILE* fout = fopen("inverse.out", "w");
	fprintf(fout, "%d", ans);
	fclose(fout);

	return 0;
}