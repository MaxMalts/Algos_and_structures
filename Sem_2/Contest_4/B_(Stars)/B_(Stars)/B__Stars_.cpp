#include <iostream>
#include <vector>
#include <assert.h>
#include <ctime>

using namespace std;


typedef int value_t;

class FanvicTree3D {
public:

	FanvicTree3D(int edgeLen) : edgeLen(edgeLen) {
		assert(edgeLen > 0);

		data = vector<vector<vector<value_t>>>(edgeLen, vector<vector<value_t>>(edgeLen, vector<value_t>(edgeLen, {})));
	}

	void Update(int x, int y, int z, value_t diff);

	int AreaSum(int x1, int y1, int z1, int x2, int y2, int z2);

private:

	vector<vector<vector<value_t>>> data;

	int edgeLen = 0;


	int AndFunc(int x) {
		return x & (x + 1);
	}

	int OrFunc(int x) {
		return x | (x + 1);
	}

	int Prefix(int x, int y, int z);
};


void FanvicTree3D::Update(int x, int y, int z, value_t diff) {
	assert(x >= 0 && x < edgeLen);
	assert(y >= 0 && y < edgeLen);
	assert(z >= 0 && z < edgeLen);

	for (int i = x; i < edgeLen; i = OrFunc(i)) {
		for (int j = y; j < edgeLen; j = OrFunc(j)) {
			for (int k = z; k < edgeLen; k = OrFunc(k)) {
				data[i][j][k] += diff;
			}
		}
	}
}


int FanvicTree3D::Prefix(int x, int y, int z) {
	int res = 0;

	for (int i = x; i >= 0; i = AndFunc(i) - 1) {
		for (int j = y; j >= 0; j = AndFunc(j) - 1) {
			for (int k = z; k >= 0; k = AndFunc(k) - 1) {
				res += data[i][j][k];
			}
		}
	}

	return res;
}


int FanvicTree3D::AreaSum(int x1, int y1, int z1, int x2, int y2, int z2) {
	if (x1 > x2) {
		swap(x1, x2);
	}
	if (y1 > y2) {
		swap(y1, y2);
	}
	if (z1 > z2) {
		swap(z1, z2);
	}
	assert(x1 >= 0 && x2 >= x1 && x2 < edgeLen);
	assert(y1 >= 0 && y2 >= y1 && y2 < edgeLen);
	assert(z1 >= 0 && z2 >= z1 && z2 < edgeLen);

	int bigCube = Prefix(x2, y2, z2);
	int smallCube = Prefix(x1 - 1, y1 - 1, z1 - 1);
	int subVal = Prefix(x1 - 1, y2, z2) + Prefix(x2, y1 - 1, z2) + Prefix(x2, y2, z1 - 1);
	int addVal = Prefix(x2, y1 - 1, z1 - 1) + Prefix(x1 - 1, y2, z1 - 1) + Prefix(x1 - 1, y1 - 1, z2);

	return bigCube - subVal + addVal - smallCube;
}

// End of fanvic tree realization




enum command_type {
	update_command = 1,
	count_command = 2,
	end_command = 3
};


int main() {
	ios_base::sync_with_stdio(false);
	cin.tie(0);

	int n = 0;
	cin >> n;

	FanvicTree3D starsTree(n);

	int curCommand = 0;
	cin >> curCommand;
	while (curCommand != end_command) {
		switch (curCommand) {
		case update_command:
		{
			int x, y, z, diff;
			cin >> x >> y >> z >> diff;

			starsTree.Update(x, y, z, diff);
			break;
		}
		case count_command:
		{
			int x1, y1, z1, x2, y2, z2;
			cin >> x1 >> y1 >> z1 >> x2 >> y2 >> z2;

			cout << starsTree.AreaSum(x1, y1, z1, x2, y2, z2) << '\n';
			break;
		}
		default:
			assert(0);
		}

		cin >> curCommand;
	}

	//double time = (double)clock() / CLOCKS_PER_SEC;
	return 0;
}