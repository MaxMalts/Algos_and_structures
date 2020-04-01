#include <iostream>
#include <vector>
#include <assert.h>

using namespace std;


typedef int key_t;
typedef int value_t;

struct Node {
	key_t key = {};
	value_t value = {};
};

class Heap {
public:
	Heap() {
		Node firstNode = {};
		data.push_back(firstNode);
	}


	void AddElem(key_t key, value_t value = {}) {
		Node newNode = { key, value };
		data.push_back(newNode);

		int curPos = data.size() - 1;
		while (curPos > 1 && KeyCmp(data[curPos].key, data[curPos / 2].key) > 0) {
			swap(data[curPos], data[curPos / 2]);
			curPos /= 2;
		}
	}


	Node GetFirst() {
		if (data.size() > 1) {
			return data[1];

		} else {
			assert(0);
			return {};
		}
	}


	void RemoveFirst() {
		assert(data.size() > 1);

		swap(data[data.size() - 1], data[1]);
		data.pop_back();

		int curPos = 1;
		while (curPos * 2 <= data.size() - 1) {
			int leftChild = curPos * 2;
			int rightChild = curPos * 2 + 1;

			int maxChild = leftChild;
			if (rightChild <= data.size() - 1 && KeyCmp(data[rightChild].key, data[leftChild].key) > 0) {
				maxChild = rightChild;
			}
			if (KeyCmp(data[curPos].key, data[maxChild].key) >= 0) {
				break;
			}
			swap(data[curPos], data[maxChild]);
			curPos = maxChild;
		}
	}


	int Size() {
		return data.size() - 1;
	}

private:
	vector<Node> data;
	
	int KeyCmp(key_t key1, key_t key2) {
		if (key1 > key2) {
			return -1;
		} else if (key1 < key2) {
			return 1;
		} else {
			return 0;
		}
	}
};



enum command_name {
	insert_comm,
	getMin_comm,
	removeMin_comm
};

struct command_t {
	command_name command = {};
	int parameter = 0;
};


command_t ParseCommand() {
	char command[100] = "";
	scanf("%s", command);

	command_t res = {};
	switch (command[0]) {
	case 'i':
		res.command = insert_comm;
		scanf("%d", &res.parameter);
		break;

	case 'g':
		res.command = getMin_comm;
		scanf("%d", &res.parameter);
		break;

	case 'r':
		res.command = removeMin_comm;
		break;

	default:
		assert(0);
	}

	return res;
}


void HandleCommand(command_t command, Heap& heap, vector<command_t>& outpComm) {
	switch (command.command) {
	case insert_comm: {
		outpComm.push_back(command);
		heap.AddElem(command.parameter);
		break;
	}

	case getMin_comm: {
		while (heap.Size() > 0 && heap.GetFirst().key < command.parameter) {
			command_t newComm = { removeMin_comm, 0 };
			outpComm.push_back(newComm);
			heap.RemoveFirst();
		}

		if ((heap.Size() > 0 && heap.GetFirst().key > command.parameter) || heap.Size() == 0) {
			command_t newComm = { insert_comm, command.parameter };
			outpComm.push_back(newComm);
			heap.AddElem(command.parameter);
		}

		assert(heap.GetFirst().key == command.parameter);
		outpComm.push_back(command);
		break;
	}

	case removeMin_comm: {
		if (heap.Size() == 0) {
			command_t newComm = { insert_comm, 0 };
			outpComm.push_back(newComm);
			heap.AddElem(0);
		}

		assert(heap.Size() > 0);
		outpComm.push_back(command);
		heap.RemoveFirst();
		break;
	}

	default:
		assert(0);
	}
}


void OutputAnswer(vector<command_t> outpComm) {
	int m = outpComm.size();
	printf("%d\n", m);

	for (int i = 0; i < m; ++i) {
		switch (outpComm[i].command) {
		case insert_comm:
			printf("insert %d\n", outpComm[i].parameter);
			break;

		case getMin_comm:
			printf("getMin %d\n", outpComm[i].parameter);
			break;

		case removeMin_comm:
			printf("removeMin\n");
			break;

		default:
			assert(0);
		}
	}
}


int main() {
	int n = 0;
	scanf("%d", &n);

	Heap heap;
	vector<command_t> outpComm;

	command_t curCommand = {};
	for (int i = 0; i < n; ++i) {
		curCommand = ParseCommand();

		HandleCommand(curCommand, heap, outpComm);
	}

	OutputAnswer(outpComm);

	//fseek(stdin, SEEK_END, 0);
	//getchar();
	return 0;
}