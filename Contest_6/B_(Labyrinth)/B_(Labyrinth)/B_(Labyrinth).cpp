#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <assert.h>

using namespace std;



struct Cell_Info {
	int x = 0;
	int y = 0;
	int leftLeft = 0;   // left - direction, Left - still available
	int rightLeft = 0;
};


void HandleCell(vector<vector<char>>& labyrinth, vector<vector<pair<int, int>>>& left_rightLeft,
				queue<Cell_Info>& needToHandle, int x, int y, int left, int right, int& ans) {
	assert(x > 0);
	assert(y > 0);

	char cellType = labyrinth[x][y];
	switch(cellType){
	case '.': {
		labyrinth[x][y] = '+';
		left_rightLeft[x][y] = {left, right};
		needToHandle.push({x, y, left, right});

		++ans;
		break;
	}

	case '+': {
		if (left_rightLeft[x][y].first < left || left_rightLeft[x][y].second < right) {

			left_rightLeft[x][y].first = max(left_rightLeft[x][y].first, left);
			left_rightLeft[x][y].second = max(left_rightLeft[x][y].second, right);
			needToHandle.push({x, y, left, right}); \
		}
		break;
	}

	default:
		assert(cellType == '*');
	}
}


int GetAns(vector<vector<char>>& labyrinth, int NRows, int NColumns, int startRow, int startColumn,
		   int leftClicks, int rightClicks) {
	assert(NRows > 0);
	assert(NColumns > 0);
	assert(startRow > 0);
	assert(startColumn > 0);

	vector<vector<pair<int, int>>> left_rightLeft(NColumns + 1, vector<pair<int, int>>(NRows + 1, {0, 0}));

	int ans = 1;
	labyrinth[startColumn][startRow] = '+';
	left_rightLeft[startColumn][startRow] = {leftClicks, rightClicks};

	queue<Cell_Info> needToHandle;
	needToHandle.push({startColumn, startRow, leftClicks, rightClicks});

	int newX = 0, newY = 0, newLeft = 0, newRight = 0;
	while (!needToHandle.empty()) {
		Cell_Info curCell = needToHandle.front();
		needToHandle.pop();

		if (curCell.x > 1 && curCell.leftLeft > 0) {
			newX = curCell.x - 1;
			newY = curCell.y;
			newLeft = curCell.leftLeft - 1;
			newRight = curCell.rightLeft;

			HandleCell(labyrinth, left_rightLeft, needToHandle, newX, newY, newLeft, newRight, ans);
		}
		if (curCell.x + 1 <= NColumns && curCell.rightLeft > 0) {
			newX = curCell.x + 1;
			newY = curCell.y;
			newLeft = curCell.leftLeft;
			newRight = curCell.rightLeft - 1;

			HandleCell(labyrinth, left_rightLeft, needToHandle, newX, newY, newLeft, newRight, ans);
		}
		if (curCell.y > 1) {
			newX = curCell.x;
			newY = curCell.y - 1;
			newLeft = curCell.leftLeft;
			newRight = curCell.rightLeft;

			HandleCell(labyrinth, left_rightLeft, needToHandle, newX, newY, newLeft, newRight, ans);
		}
		if (curCell.y + 1 <= NRows) {
			newX = curCell.x;
			newY = curCell.y + 1;
			newLeft = curCell.leftLeft;
			newRight = curCell.rightLeft;

			HandleCell(labyrinth, left_rightLeft, needToHandle, newX, newY, newLeft, newRight, ans);
		}
	}

	return ans;
}


int main() {
	int n, m, r, c, x, y;
	cin >> n >> m >> r >> c >> x >> y;

	vector<vector<char>> labyrinth(m + 1, vector<char>(n + 1, 0));
	for (int i = 1; i <= n; ++i) {
		for (int j = 1; j <= m; ++j) {
			cin >> labyrinth[j][i];
		}
	}


	int ans = GetAns(labyrinth, n, m, r, c, x, y);
	
	cout << ans;

	return 0;
}