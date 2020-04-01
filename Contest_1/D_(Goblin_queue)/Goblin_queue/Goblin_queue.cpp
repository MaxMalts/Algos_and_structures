#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

struct list_node {
	int value = 0;
	list_node* next = NULL;
};

struct list_t {
	list_node* head = NULL;
	list_node* middle = NULL;
	list_node* tail = NULL;
	int size = 0;
};



list_node* CreateNode(int val = 0) {
	list_node* node = (list_node*)calloc(1, sizeof(list_node));
	node->value = val;

	return node;
}


void DeleteNode(list_node* node) {
	assert(node != NULL);

	free(node);
}


void AddToEnd(list_t* list, int val) {
	assert(list != NULL);

	list_node* newNode = CreateNode(val);

	if (list->size == 0) {
		assert(list->head == NULL);
		assert(list->middle == NULL);
		assert(list->tail == NULL);

		list->head = newNode;
		list->middle = newNode;
		list->tail = newNode;
		++list->size;

	} else {
		assert(list->size > 0);

		list->tail->next = newNode;
		list->tail = newNode;
		++list->size;

		if (list->size % 2 == 1) {
			list->middle = list->middle->next;
		}
	}
}


void AddToMiddle(list_t* list, int val) {
	assert(list != NULL);

	if (list->size < 2) {
		assert(list->size >= 0);

		AddToEnd(list, val);

	} else {
		list_node* nextMiddle = list->middle->next;
		list->middle->next = CreateNode(val);
		list->middle->next->next = nextMiddle;
		++list->size;

		if (list->size % 2 == 1) {
			list->middle = list->middle->next;
		}
	}
}


int PassFirst(list_t* list) {
	assert(list != NULL);

	assert(list->size > 0);

	int res = list->head->value;

	list_node* oldHead = list->head;
	list->head = oldHead->next;

	DeleteNode(oldHead);
	--list->size;

	if (list->size == 0) {
		list->head = NULL;
		list->middle = NULL;
		list->tail = NULL;

	} else if (list->size == 1) {
		assert(list->head == list->tail);

		list->middle = list->head;

	} else if (list->size % 2 == 1) {
		list->middle = list->middle->next;
	}

	return res;
}


void ParseString(FILE* fin, FILE* fout, list_t* queue) {
	assert(fin != NULL);
	assert(fout != NULL);

	int curGob = 0;
	char curCh = fgetc(fin);
	fgetc(fin);
	
	switch (curCh) {
	case '+': {
		fscanf(fin, "%d%*c", &curGob);
		AddToEnd(queue, curGob);
		break;
	}

	case '*': {
		fscanf(fin, "%d%*c", &curGob);
		AddToMiddle(queue, curGob);
		break;
	}

	case '-': {
		curGob = PassFirst(queue);
		fprintf(fout, "%d\n", curGob);
		break;
	}

	default:
		assert(0);
	}
}

int main() {
	FILE* fin = stdin;
	FILE* fout = stdout;

	int N = 0;
	fscanf(fin, "%d%*c", &N);

	list_t queue = {};
	for (int i = 0; i < N; ++i) {
		ParseString(fin, fout, &queue);
	}

	fclose(fin);
	fclose(fout);

	return 0;
}