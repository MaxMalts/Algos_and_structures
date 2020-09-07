#include <iostream>
#include <vector>
#include <assert.h>


enum CellStatus{
	cell_free,
	cell_paved
};

long long n = 0, m = 0, a = 0, b = 0;
long long freeCells = 0, pavedCells = 0;
std::vector<std::vector<CellStatus>> board;


void ReadData() {
	std::cin >> n >> m >> a >> b;

	board.assign(n, std::vector<CellStatus>(m));
	for (long long curRow = 0; curRow < n; ++curRow) {
		for (long long curColumn = 0; curColumn < m; ++curColumn) {
			char curCell = 0;
			std::cin >> curCell;
			switch (curCell) {
			case '*':
				board[curRow][curColumn] = cell_free;
				++freeCells;
				break;

			case '.':
				board[curRow][curColumn] = cell_paved;
				++pavedCells;
				break;
			
			default:
				assert(0);
			}
		}
	}

	assert(freeCells + pavedCells == n * m);
}


std::vector<std::vector<long long>> GraphFromBoard() {
	std::vector<std::vector<long long>> res(n * m);

	for (long long curRow = 0; curRow < n; ++curRow) {
		for (long long curColumn = 0; curColumn < m; ++curColumn) {
			if ((curRow + curColumn) % 2)
				continue;

			if (board[curRow][curColumn] == cell_free) {
				if (curColumn - 1 >= 0 && board[curRow][curColumn - 1] == cell_free) {
					res[m * curRow + curColumn].push_back(m * curRow + (curColumn - 1));
				}

				if (curRow + 1 < n && board[curRow + 1][curColumn] == cell_free) {
					res[m * curRow + curColumn].push_back(m * (curRow + 1) + curColumn);
				}

				if (curColumn + 1 < m && board[curRow][curColumn + 1] == cell_free) {
					res[m * curRow + curColumn].push_back(m * curRow + (curColumn + 1));
				}

				if (curRow - 1 >= 0 && board[curRow - 1][curColumn] == cell_free) {
					res[m * curRow + curColumn].push_back(m * (curRow - 1) + curColumn);
				}
			}
		}
	}

	return res;
}


bool FindChain(long long curNode, std::vector<std::vector<long long>>& graph, std::vector<bool>& passed,
	long long& matchSize, std::vector<long long>& foundEdges) {
	if (passed[curNode])
		return false;

	passed[curNode] = true;
	for (auto nextNode : graph[curNode]) {
		if (nextNode >= foundEdges.size())
			foundEdges.resize(nextNode + 1, -1);

		if (foundEdges[nextNode] == -1) {
			foundEdges[nextNode] = curNode;
			++matchSize;
			return true;

		} else {
			if (FindChain(foundEdges[nextNode], graph, passed, matchSize, foundEdges)) {
				foundEdges[nextNode] = curNode;
				return true;
			}
		}
	}

	return false;
}


long long MaxMatch(std::vector<std::vector<long long>> graph) {
	long long matchSize = 0;
	std::vector<long long> foundEdges;
	std::vector<bool> passed;
	for (long long i = 0; i < graph.size(); ++i) {
		passed.assign(graph.size(), false);
		FindChain(i, graph, passed, matchSize, foundEdges);
	}

	return matchSize;
}


long long Calculate() {
	if (a >= 2 * b) {
		return b * freeCells;
	}

	std::vector<std::vector<long long>> graph = GraphFromBoard();
	long long doubleCount = MaxMatch(graph);

	return doubleCount * a + (freeCells - 2 * doubleCount) * b;
}


int main() {
	freopen("dominoes.in", "r", stdin);
	freopen("dominoes.out", "w", stdout);

	ReadData();
	long long ans = Calculate();
	std::cout << ans;
}