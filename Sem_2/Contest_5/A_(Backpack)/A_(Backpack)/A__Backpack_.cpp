//#include <iostream>
//#include <vector>
//#include <assert.h>
//#include <ctime>
//
//using namespace std;
//
//
//
//int newClocks = 0;
//
//struct Item {
//	int weight = 0;
//	int cost = 0;
//};
//
//
//vector<Item> ReadItems(int NItems) {
//	vector<Item> res(NItems);
//
//	for (int i = 0; i < NItems; ++i) {
//		cin >> res[i].weight;
//	}
//	for (int i = 0; i < NItems; ++i) {
//		cin >> res[i].cost;
//	}
//
//	return res;
//}
//
//
//pair<int, vector<int>*> CalcPutedItems(vector<Item> items, int curInd, int maxWeight) {
//	if (curInd < 0 || maxWeight <= 0) {
//		int curClocks = clock();
//		vector<int>* curBranch = new vector<int>;
//		curClocks = clock() - curClocks;
//		newClocks += curClocks;
//
//		pair<int, vector<int>*> res(0, curBranch);
//
//		return res;
//	}
//
//	pair<int, vector<int>*> curNotAdded = CalcPutedItems(items, curInd - 1, maxWeight);
//	int costNotAdded = curNotAdded.first;
//
//	pair<int, vector<int>*> curAdded;
//	int costAdded = -1;
//	if (maxWeight >= items[curInd].weight) {
//		curAdded = CalcPutedItems(items, curInd - 1, maxWeight - items[curInd].weight);
//		costAdded = curAdded.first + items[curInd].cost;
//	}
//
//	pair<int, vector<int>*> res = curNotAdded;
//	if (costNotAdded > costAdded) {
//		/*if (costAdded > -1) {
//			delete curAdded.second;
//		}*/
//
//	} else {
//		//delete curNotAdded.second;
//
//		res = curAdded;
//		res.first = costAdded;
//		res.second->push_back(curInd);
//	}
//
//	return res;
//}
//
//
//void PrintAnswer(vector<Item> items, int maxWeight) {
//	assert(items.size() > 0);
//	assert(maxWeight > 0);
//
//	pair<int, vector<int>*> ans = CalcPutedItems(items, items.size() - 1, maxWeight);
//
//	cout << ans.second->size() << '\n';
//	for (int i = 0; i < ans.second->size(); ++i) {
//		cout << (*ans.second)[i] + 1 << ' ';
//	}
//}
//
//
//int main() {
//	std::ios_base::sync_with_stdio(false);
//	cin.tie(0);
//
//	int N = 0, M = 0;
//	cin >> N >> M;
//
//	vector<Item> items = ReadItems(N);
//
//	PrintAnswer(items, M);
//
//
//	double totalTime = (double)clock() / CLOCKS_PER_SEC;
//	double newTime = (double)newClocks / CLOCKS_PER_SEC;
//	return 0;
//}




#include <iostream>
#include <vector>
#include <algorithm>
#include <assert.h>

using namespace std;


struct Item {
	int weight = 0;
	int cost = 0;
};


vector<Item> ReadItems(int NItems) {
	vector<Item> res(NItems + 1);

	for (int i = 1; i <= NItems; ++i) {
		cin >> res[i].weight;
	}
	for (int i = 1; i <= NItems; ++i) {
		cin >> res[i].cost;
	}

	return res;
}


vector<vector<int>> GenerateTable(vector<Item> items, int maxWeight) {
	assert(items.size() > 1);
	assert(maxWeight > 0);

	vector<vector<int >> res(items.size(), vector<int>(maxWeight + 1, 0));
	
	for (int curInd = 1; curInd < items.size(); ++curInd) {
		for (int curWeight = 1; curWeight <= maxWeight; ++curWeight) {
			if (items[curInd].weight > curWeight) {
				res[curInd][curWeight] = res[curInd - 1][curWeight];
			} else {
				res[curInd][curWeight] = max(res[curInd - 1][curWeight],
					      res[curInd - 1][curWeight - items[curInd].weight] + items[curInd].cost);
			}
		}
	}

	return res;
}


void GetAnswer_rec(vector<int>& ans, vector<vector<int>> table, vector<Item> items, int curInd, int curWeight) {
	assert(curInd >= 0);
	assert(curWeight >= 0);

	if (table[curInd][curWeight] == 0) {
		return;
	}
	if (table[curInd][curWeight] == table[curInd - 1][curWeight]) {
		GetAnswer_rec(ans, table, items, curInd - 1, curWeight);
	} else {
		GetAnswer_rec(ans, table, items, curInd - 1, curWeight - items[curInd].weight);
		ans.push_back(curInd);
	}
}


vector<int> GetAnswer(vector<vector<int>> table, vector<Item> items, int maxWeight) {
	assert(table.size() > 0);
	assert(items.size() > 1);

	vector<int> ans;
	GetAnswer_rec(ans, table, items, items.size() - 1, maxWeight);

	return ans;
}


void PrintAnswer(vector<int> ans) {
	cout << ans.size() << '\n';
	for (int i = 0; i < ans.size(); ++i) {
		cout << ans[i] << ' ';
	}
}


int main() {
	std::ios_base::sync_with_stdio(false);
	cin.tie(0);

	int N = 0, M = 0;
	cin >> N >> M;

	vector<Item> items = ReadItems(N);

	vector<vector<int>> table = GenerateTable(items, M);

	vector<int>ans = GetAnswer(table, items, M);

	PrintAnswer(ans);

	return 0;
}