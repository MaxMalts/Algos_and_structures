#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <iostream>

using namespace std;


typedef int elem_t;
typedef int add_info_t;

enum column_type {
	column_normal,
	column_head,
	column_tail
};

const int columnMaxHeight = 10;

struct column_t {
	column_type type = column_normal;
	elem_t value = {};
	add_info_t addInfo = {};
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


void AddElement(skiplist_t* slist, elem_t val, add_info_t addInfo = {}) {
	assert(slist != NULL);

	column_t* nearColumns[columnMaxHeight] = {};
	slist_pos_t addPos = PositionByValue(slist, val, nearColumns);

	int newHeight = RandHeight(columnMaxHeight);

	column_t* newColumn = CreateColumn();
	newColumn->type = column_normal;
	newColumn->value = val;
	newColumn->addInfo = addInfo;
	newColumn->height = newHeight;

	for (int i = 0; i < newHeight; ++i) {
		column_t* oldNext = nearColumns[i]->nextColumn[i]; 
		nearColumns[i]->nextColumn[i] = newColumn;
		newColumn->nextColumn[i] = oldNext;
	}

	++slist->size;
}


bool ElementExists(skiplist_t* slist, elem_t val, column_t** foundColumn = NULL) {
	assert(slist != NULL);

	*foundColumn = PositionByValue(slist, val).column->nextColumn[0];

	return (*foundColumn)->value == val;
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




skiplist_t ReadFirstArray() {
	int N = 0;
	cin >> N;

	skiplist_t points = SkiplistConstructor();
	column_t* foundColumn = NULL;
	int curPoint = 0;
	for (int i = 0; i < N; ++i) {
		cin >> curPoint;
		//scanf("%d", &curPoint);

		if (ElementExists(&points, curPoint, &foundColumn)) {
			++(foundColumn->addInfo);
		} else {
			AddElement(&points, curPoint, 1);
		}
	}

	return points;
}


void ParseSecondArray(skiplist_t points) {
	int m = 0;
	//scanf("%d\n", &m);
	cin >> m;

	column_t* foundColumn = NULL;
	int curPoint = 0;
	for (int i = 0; i < m; ++i) {
		//scanf("%d ", &curPoint);
		cin >> curPoint;

		if (ElementExists(&points, curPoint, &foundColumn)) {
			//printf("%d ", foundColumn->addInfo);
			cout << (foundColumn->addInfo) << " ";
		} else {
			//printf("0 ");
			cout << "0 ";
		}
	}
}


int main() {
	skiplist_t points = ReadFirstArray();

	ParseSecondArray(points);

	fseek(stdin, SEEK_END, 0);
	getchar();
	return 0;
}