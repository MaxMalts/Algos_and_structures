#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

const int maxN = 2147483647;
const int maxM = 100000;

enum request_status_t {
	status_declined,
	status_allocated,
	status_removed
};

struct Segment {
	Segment* prev, * next;
	bool free;
	int segmBeg = 0, segmEnd = 0, heapPos = 0;

	Segment(Segment* prev, Segment* next, bool free, int segmBeg, int segmEnd, int heapPos) {
		this->prev = prev;
		this->next = next;
		this->free = free;
		this->segmBeg = segmBeg;
		this->segmEnd = segmEnd;
		this->heapPos = heapPos;
		if (prev) {
			prev->next = this;
		}
		if (next) {
			next->prev = this;
		}
	}

	void remove() {
		if (prev) {
			prev->next = next;
		}
		if (next) {
			next->prev = prev;
		}
	}
};


void Swap(int ind1, int ind2);

struct Heap {
	int heapSize = 0;
	Segment* data[maxM] = {};

	bool heapCmp(int ind1, int ind2) {
		return data[ind1]->segmEnd - data[ind1]->segmBeg > data[ind2]->segmEnd - data[ind2]->segmBeg;
	}

	void heapify(int ind) {
		while (true) {
			int ind1 = ind;
			if (((ind << 1) + 1 < heapSize) && heapCmp((ind << 1) + 1, ind1)) {
				ind1 = (ind << 1) + 1;
			}
			if (((ind << 1) + 2 < heapSize) && heapCmp((ind << 1) + 2, ind1)) {
				ind1 = (ind << 1) + 2;
			}
			if (ind == ind1) {
				return;
			}
			Swap(ind, ind1);
			ind = ind1;
		}
	}

	void lift(int ind) {
		while (ind && heapCmp(ind, (ind - 1) >> 1)) {
			Swap(ind, (ind - 1) >> 1);
			ind = (ind - 1) >> 1;
		}
	}

	void remove(int ind) {
		Swap(ind, heapSize - 1);
		--heapSize;
		if (ind >= heapSize)
			return;
		lift(ind);
		heapify(ind);
	}

	void add(Segment* a) {
		a->heapPos = heapSize;
		data[heapSize] = a;
		lift(heapSize++);
	}
};

int n = 0, m = 0, curReqInd = 0;
request_status_t requestStatus[maxM] = {};
Heap segmHeap;
Segment* reqSegment[maxM] = {};


void Swap(int ind1, int ind2) {
	Segment* t = segmHeap.data[ind1];
	segmHeap.data[ind1] = segmHeap.data[ind2];
	segmHeap.data[ind2] = t;
	segmHeap.data[ind1]->heapPos = ind1;
	segmHeap.data[ind2]->heapPos = ind2;
}

void Allocate(int K) {
	Segment* segm = segmHeap.data[0];
	if ((segmHeap.heapSize == 0) || (segm->segmEnd - segm->segmBeg < K)) {
		requestStatus[curReqInd++] = status_declined;
		printf("-1\n");
		return;
	}

	requestStatus[curReqInd++] = status_allocated;
	reqSegment[curReqInd - 1] = new Segment(segm->prev, segm, false, segm->segmBeg, segm->segmBeg + K, -1);
	printf("%d\n", segm->segmBeg + 1);
	segm->segmBeg += K;
	if (segm->segmBeg < segm->segmEnd) {
		segmHeap.heapify(segm->heapPos);
	} else {
		segm->remove();
		segmHeap.remove(0);
		delete segm;
	}
}

void Free(int T) {
	--T;
	requestStatus[curReqInd++] = status_removed;
	if (requestStatus[T] == status_declined) {
		return;
	}

	requestStatus[T] = status_removed;
	Segment* segm = reqSegment[T];
	Segment* prevSegm = segm->prev;
	Segment* nextSegm = segm->next;
	bool prevAvailable = prevSegm && prevSegm->free;
	bool nextAvailable = nextSegm && nextSegm->free;
	if (!prevAvailable && !nextAvailable) {
		segm->free = true;
		segmHeap.add(segm);
		return;
	}
	if (!prevAvailable) {
		nextSegm->segmBeg = segm->segmBeg;
		segmHeap.lift(nextSegm->heapPos);
		segm->remove();
		delete segm;
		return;
	}
	if (!nextAvailable) {
		prevSegm->segmEnd = segm->segmEnd;
		segmHeap.lift(prevSegm->heapPos);
		segm->remove();
		delete segm;
		return;
	}
	prevSegm->segmEnd = nextSegm->segmEnd;
	segmHeap.lift(prevSegm->heapPos);
	segm->remove();
	delete segm;
	segmHeap.remove(nextSegm->heapPos);
	nextSegm->remove();
	delete nextSegm;
}

int main() {
	scanf("%d%d", &n, &m);
	segmHeap.heapSize = 1;
	segmHeap.data[0] = new Segment(0, 0, true, 0, n, 0);
	for (int i = 0; i < m; i++) {
		int K = 0;
		scanf("%d", &K);

		if (K > 0) {
			Allocate(K);
		} else {
			Free(-K);
		}
	}

	return 0;
}