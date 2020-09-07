#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <vector>

using namespace std;

typedef int elem_t;

enum column_type {
	column_normal,
	column_head,
	column_tail
};

const int columnMaxHeight = 10;

struct column_t {
	column_type type = column_normal;
	elem_t value = {};
	column_t* nextColumn[columnMaxHeight] = {};
	int height = 0;
};

struct skiplist_t {
	column_t* head = NULL;
	int size = 0;
};




column_t* CreateColumn() {

	column_t* column = (column_t*)calloc(1, sizeof(column_t));

	return column;
}


void EraseColumn(column_t* column) {
	assert(column != NULL);

	free(column);
}


int ColumnCmp(column_t* column1, column_t* column2) {
	assert(column1 != NULL);
	assert(column2 != NULL);

	int res = 0;

	if (column1->type == column_head && column2->type == column_head) {
		res = 0;
	} else if (column1->type == column_head && column2->type != column_head) {
		res = -1;
	} else if (column1->type == column_tail && column2->type == column_tail) {
		res = 0;
	} else if (column1->type == column_tail && column2->type != column_tail) {
		res = 1;
	} else if (column1->type == column_normal && column2->type == column_head) {
		res = 1;
	} else if (column1->type == column_normal && column2->type == column_tail) {
		res = -1;
	} else if (column1->type == column_normal && column2->type == column_normal) {
		if (column1->value < column2->value) {
			res = -1;
		} else if (column1->value == column2->value) {
			res = 0;
		} else {
			res = 1;
		}
	}

	return res;
}


int RandHeight(int maxHeight = columnMaxHeight) {
	assert(maxHeight > 0);

	int res = 1;
	for (int i = 1; i < maxHeight; ++i) {
		if (rand() % 2) {
			break;
		}
		++res;
	}

	return res;
}




skiplist_t SkiplistConstructor() {
	skiplist_t slist = {};
	slist.size = 0;

	column_t* head = CreateColumn();
	head->type = column_head;
	head->height = columnMaxHeight;

	column_t* tail = CreateColumn();
	tail->type = column_tail;
	tail->height = columnMaxHeight;

	for (int i = 0; i < columnMaxHeight; ++i) {
		head->nextColumn[i] = tail;
	}

	slist.head = head;

	return slist;
}


struct slist_pos_t {
	column_t* column = NULL;
	int height = 0;
};

slist_pos_t PositionByValue(skiplist_t* slist, elem_t val, column_t* nearColumns[columnMaxHeight] = NULL) {
	assert(slist != NULL);

	column_t* tempColumn = CreateColumn();
	tempColumn->type = column_normal;
	tempColumn->value = val - 1;

	slist_pos_t curPos = { slist->head, columnMaxHeight - 1 };

	while (true) {
		if (ColumnCmp(tempColumn, curPos.column->nextColumn[curPos.height]) < 0) {
			if (nearColumns != NULL) {
				nearColumns[curPos.height] = curPos.column;
			}

			if (curPos.height > 0) {
				--curPos.height;
			} else {
				assert(curPos.height == 0);
				break;
			}

		} else {
			curPos.column = curPos.column->nextColumn[curPos.height];
		}
	}

	EraseColumn(tempColumn);
	return curPos;
}


void AddElement(skiplist_t* slist, elem_t val) {
	assert(slist != NULL);

	column_t* nearColumns[columnMaxHeight] = {};
	slist_pos_t addPos = PositionByValue(slist, val, nearColumns);

	int newHeight = RandHeight(columnMaxHeight);

	column_t* newColumn = CreateColumn();
	newColumn->type = column_normal;
	newColumn->value = val;
	newColumn->height = newHeight;

	for (int i = 0; i < newHeight; ++i) {
		column_t* oldNext = nearColumns[i]->nextColumn[i];
		nearColumns[i]->nextColumn[i] = newColumn;
		newColumn->nextColumn[i] = oldNext;
	}

	++slist->size;
}


bool ElementExists(skiplist_t* slist, elem_t val) {
	assert(slist != NULL);

	column_t* elemColumn = PositionByValue(slist, val).column->nextColumn[0];

	return (elemColumn->value == val);
}


void DeleteElement(skiplist_t* slist, elem_t val) {
	assert(slist != NULL);

	column_t* nearColumns[columnMaxHeight] = {};
	column_t* elemColumn = PositionByValue(slist, val, nearColumns).column->nextColumn[0];

	for (int i = 0; i < elemColumn->height; ++i) {
		nearColumns[i]->nextColumn[i] = elemColumn->nextColumn[i];
	}

	EraseColumn(elemColumn);

	--slist->size;
}


elem_t ValueByIndex(skiplist_t* slist, int index) {
	assert(slist != NULL);
	assert(index >= 0);
	assert(index < slist->size);

	column_t* curColumn = slist->head->nextColumn[0];
	for (int i = 0; i < index; ++i) {
		curColumn = curColumn->nextColumn[0];

		assert(curColumn != NULL);
	}

	return curColumn->value;
}
// End of skiplist realization



const int NColors = 3;
const int MaxN = 200000;

skiplist_t* ParseTShirts(int n) {
	assert(n > 0);

	int p[MaxN] = {};
	int a[MaxN] = {};
	int* pCursor = p;
	int* aCursor = a;

	for (int i = 0; i < n; ++i) {
		scanf("%d", pCursor++);
	}
	for (int i = 0; i < n; ++i) {
		scanf("%d", aCursor++);
	}


	static skiplist_t pricesForColor[3] = { SkiplistConstructor(), SkiplistConstructor(), SkiplistConstructor() };

	pCursor = p;
	aCursor = a;
	int p_i = 0, a_i = 0, b_i = 0;
	for (int i = 0; i < n; ++i) {
		p_i = *pCursor;
		a_i = *aCursor;
		scanf("%d", &b_i);

		for (int j = 0; j < NColors; ++j) {
			if (a_i == j + 1 || b_i == j + 1) {
				AddElement(&pricesForColor[j], p_i);
			}
		}

		++pCursor;
		++aCursor;
	}

	return pricesForColor;
}


void ServeBuyers(skiplist_t pricesForColor[3], int m) {
	assert(m > 0);

	int favColor_i = 0;
	for (int i = 0; i < m; ++i) {
		scanf("%d", &favColor_i);

		if (pricesForColor[favColor_i - 1].size == 0) {
			printf("-1 ");

		} else {
			int curPrice = ValueByIndex(&pricesForColor[favColor_i - 1], 0);
			printf("%d ", curPrice);

			for (int j = 0; j < NColors; ++j) {
				if (ElementExists(&pricesForColor[j], curPrice)) {
					DeleteElement(&pricesForColor[j], curPrice);
				}
			}
		}
	}
}


int main() {
	int n = 0, m = 0;

	scanf("%d", &n);
	skiplist_t* pricesForColor = ParseTShirts(n);

	scanf("%d", &m);
	ServeBuyers(pricesForColor, m);

	getchar();
	getchar();
	return 0;
}