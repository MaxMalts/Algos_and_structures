#include <iostream>
#include <map>
#include <algorithm>

using namespace std;


map<int, int> ReadFirstArray() {
	int N = 0;
	cin >> N;

	map<int, int> points;
	int curPoint = 0;
	for (int i = 0; i < N; ++i) {
		cin >> curPoint;
		//scanf("%d", &curPoint);

		map<int, int>::iterator found = points.find(curPoint);
		if (found != points.end()) {
			++found->second;
		} else {
			points.insert({ curPoint, 1 });
		}
	}

	return points;
}


void ParseSecondArray(map<int, int> points) {
	int m = 0;
	//scanf("%d\n", &m);
	cin >> m;

	int curPoint = 0;
	for (int i = 0; i < m; ++i) {
		//scanf("%d ", &curPoint);
		cin >> curPoint;

		map<int, int>::iterator found = points.find(curPoint);
		if (found != points.end()) {
			//printf("%d ", foundColumn->addInfo);
			cout << found->second << " ";
		} else {
			//printf("0 ");
			cout << "0 ";
		}
	}
}


int main() {
	map<int, int> points = ReadFirstArray();

	ParseSecondArray(points);

	//fseek(stdin, SEEK_END, 0);
	//getchar();
	return 0;
}