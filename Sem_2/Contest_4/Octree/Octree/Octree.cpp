#include <iostream>
#include <vector>
#include <algorithm>
#include <assert.h>

using namespace std;



typedef int value_t;

class Cube {
public:
	value_t value = {};

	Cube* leftBotBack = nullptr;
	Cube* rightBotBack = nullptr;
	Cube* rightBotFront = nullptr;
	Cube* leftBotFront = nullptr;
	Cube* leftTopBack = nullptr;
	Cube* rightTopBack = nullptr;
	Cube* rightTopFront = nullptr;
	Cube* leftTopFront = nullptr;
};

class Octree {
public:

	Octree(int maxCoord) : edgeLen(maxCoord) {
		InitCubes(root, 0, 0, 0, maxCoord - 1, maxCoord - 1, maxCoord - 1);
	}

	void Update(int x, int y, int z, value_t diff);

	value_t AreaSum(int x1, int y1, int z1, int x2, int y2, int z2);

private:

	int edgeLen = 0;

	Cube* root = nullptr;


	void Update_rec(int x, int y, int z, value_t diff,
		Cube* curCube, int curX1, int curY1, int curZ1,
		int curX2, int curY2, int curZ2);

	void InitCubes(Cube*& curCube, int curX1, int curY1, int curZ1, int curX2, int curY2, int curZ2);

	value_t AreaSum_rec(int x1, int y1, int z1, int x2, int y2, int z2,
		Cube* curCube, int curX1, int curY1, int curZ1,
		int curX2, int curY2, int curZ2);
};


void Octree::InitCubes(Cube*& curCube, int curX1, int curY1, int curZ1, int curX2, int curY2, int curZ2) {
	assert(curX1 >= 0);
	assert(curY1 >= 0);
	assert(curZ1 >= 0);
	assert(curX1 <= edgeLen);
	assert(curY1 <= edgeLen);
	assert(curZ1 <= edgeLen);

	if (curX1 > curX2 || curY1 > curY2 || curZ1 > curZ2) {
		// curCube already nullptr
		return;
	}

	curCube = new Cube;
	if (curX1 == curX2 && curY1 == curY2 && curZ1 == curZ2) {
		return;
	}

	int midX = (curX1 + curX2) / 2;
	int midY = (curY1 + curY2) / 2;
	int midZ = (curZ1 + curZ2) / 2;

	InitCubes(curCube->leftBotBack, curX1, curY1, curZ1, midX, midY, midZ);
	InitCubes(curCube->rightBotBack, curX1, midY + 1, curZ1, midX, curY2, midZ);
	InitCubes(curCube->rightBotFront, midX + 1, midY + 1, curZ1, curX2, curY2, midZ);
	InitCubes(curCube->leftBotFront, midX + 1, curY1, curZ1, curX2, midY, midZ);
	InitCubes(curCube->leftTopBack, curX1, curY1, midZ + 1, midX, midY, curZ2);
	InitCubes(curCube->rightTopBack, curX1, midY + 1, midZ + 1, midX, curY2, curZ2);
	InitCubes(curCube->rightTopFront, midX + 1, midY + 1, midZ + 1, curX2, curY2, curZ2);
	InitCubes(curCube->leftTopFront, midX + 1, curY1, midZ + 1, curX2, midY, curZ2);
}


void Octree::Update(int x, int y, int z, value_t diff) {
	assert(x >= 0 && x < edgeLen);
	assert(y >= 0 && y < edgeLen);
	assert(z >= 0 && z < edgeLen);

	Update_rec(x, y, z, diff, root, 0, 0, 0, edgeLen - 1, edgeLen - 1, edgeLen - 1);
}


void Octree::Update_rec(int x, int y, int z, value_t diff,
	Cube* curCube, int curX1, int curY1, int curZ1,
	int curX2, int curY2, int curZ2) {
	assert(x >= 0 && x < edgeLen);
	assert(y >= 0 && y < edgeLen);
	assert(z >= 0 && z < edgeLen);
	assert(curX1 >= 0);
	assert(curY1 >= 0);
	assert(curZ1 >= 0);
	assert(curX1 <= edgeLen);
	assert(curY1 <= edgeLen);
	assert(curZ1 <= edgeLen);

	if (curX1 > curX2 || curY1 > curY2 || curZ1 > curZ2) {
		return;
	}

	if (curX1 == curX2 && curY1 == curY2 && curZ1 == curZ2) {
		assert(x == curX1 && y == curY1 && z == curZ1);

		curCube->value += diff;
		return;
	}

	int midX = (curX1 + curX2) / 2;
	int midY = (curY1 + curY2) / 2;
	int midZ = (curZ1 + curZ2) / 2;

	if (x <= midX) {
		if (y <= midY) {
			if (z <= midZ) {
				Update_rec(x, y, z, diff, curCube->leftBotBack, curX1, curY1, curZ1, midX, midY, midZ);
			} else {
				Update_rec(x, y, z, diff, curCube->leftTopBack, curX1, curY1, midZ + 1, midX, midY, curZ2);
			}

		} else {
			if (z <= midZ) {
				Update_rec(x, y, z, diff, curCube->rightBotBack, curX1, midY + 1, curZ1, midX, curY2, midZ);
			} else {
				Update_rec(x, y, z, diff, curCube->rightTopBack, curX1, midY + 1, midZ + 1, midX, curY2, curZ2);
			}
		}

	} else {
		if (y <= midY) {
			if (z <= midZ) {
				Update_rec(x, y, z, diff, curCube->leftBotFront, midX + 1, curY1, curZ1, curX2, midY, midZ);
			} else {
				Update_rec(x, y, z, diff, curCube->leftTopFront, midX + 1, curY1, midZ + 1, curX2, midY, curZ2);
			}

		} else {
			if (z <= midZ) {
				Update_rec(x, y, z, diff, curCube->rightBotFront, midX + 1, midY + 1, curZ1, curX2, curY2, midZ);
			} else {
				Update_rec(x, y, z, diff, curCube->rightTopFront, midX + 1, midY + 1, midZ + 1, curX2, curY2, curZ2);
			}
		}
	}

	curCube->value += diff;
}


value_t Octree::AreaSum(int x1, int y1, int z1, int x2, int y2, int z2) {
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

	return AreaSum_rec(x1, y1, z1, x2, y2, z2, root, 0, 0, 0, edgeLen - 1, edgeLen - 1, edgeLen - 1);
}


value_t Octree::AreaSum_rec(int x1, int y1, int z1, int x2, int y2, int z2,
	Cube* curCube, int curX1, int curY1, int curZ1,
	int curX2, int curY2, int curZ2) {
	assert(curX1 >= 0);
	assert(curY1 >= 0);
	assert(curZ1 >= 0);
	assert(curX1 <= edgeLen);
	assert(curY1 <= edgeLen);
	assert(curZ1 <= edgeLen);

	if (x1 > x2 || y1 > y2 || z1 > z2) {
		return {};
	}

	if (x1 == curX1 && y1 == curY1 && z1 == curZ1 &&
		x2 == curX2 && y2 == curY2 && z2 == curZ2) {
		return curCube->value;
	}

	int midX = (curX1 + curX2) / 2;
	int midY = (curY1 + curY2) / 2;
	int midZ = (curZ1 + curZ2) / 2;

	return AreaSum_rec(x1, y1, z1, min(x2, midX), min(y2, midY), min(z2, midZ),
		curCube->leftBotBack, curX1, curY1, curZ1, midX, midY, midZ) +
		AreaSum_rec(x1, max(y1, midY + 1), z1, min(x2, midX), y2, min(z2, midZ),
			curCube->rightBotBack, curX1, midY + 1, curZ1, midX, curY2, midZ) +
		AreaSum_rec(max(x1, midX + 1), max(y1, midY + 1), z1, x2, y2, min(z2, midZ),
			curCube->rightBotFront, midX + 1, midY + 1, curZ1, curX2, curY2, midZ) +
		AreaSum_rec(max(x1, midX + 1), y1, z1, x2, min(y2, midY), min(z2, midZ),
			curCube->leftBotFront, midX + 1, curY1, curZ1, curX2, midY, midZ) +

		AreaSum_rec(x1, y1, max(z1, midZ + 1), min(x2, midX), min(y2, midY), z2,
			curCube->leftTopBack, curX1, curY1, midZ + 1, midX, midY, curZ2) +
		AreaSum_rec(x1, max(y1, midY + 1), max(z1, midZ + 1), min(x2, midX), y2, z2,
			curCube->rightTopBack, curX1, midY + 1, midZ + 1, midX, curY2, curZ2) +
		AreaSum_rec(max(x1, midX + 1), max(y1, midY + 1), max(z1, midZ + 1), x2, y2, z2,
			curCube->rightTopFront, midX + 1, midY + 1, midZ + 1, curX2, curY2, curZ2) +
		AreaSum_rec(max(x1, midX + 1), y1, max(z1, midZ + 1), x2, min(y2, midY), z2,
			curCube->leftTopFront, midX + 1, curY1, midZ + 1, curX2, midY, curZ2);
}


int main() {
	Octree tree(3);

	tree.Update(0, 0, 0, 5);
	tree.Update(0, 1, 1, -2);
	tree.Update(0, 2, 2, 4);
	tree.Update(1, 2, 2, 20);
	tree.Update(2, 2, 1, 1);

	tree.AreaSum(0, 0, 0, 0, 1, 0); // 5
	tree.AreaSum(0, 1, 0, 0, 0, 0); // 5
	tree.AreaSum(2, 2, 2, 1, 1, 1); // 21
	tree.AreaSum(0, 1, 2, 0, 0, 2); // 0

	return 0;
}