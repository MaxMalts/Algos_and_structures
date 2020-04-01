#define _CRT_DISABLE_PERFCRIT_LOCKS
#define CRT_DISABLE_PERFCRIT_LOCKS


#include <iostream>
#include <assert.h>

using namespace std;


struct node_value {
	int length = 0;
};

typedef int key_t;

typedef node_value value_t;

enum node_color {
	black_node,
	red_node
};

struct Node {
	key_t key = {};
	value_t value = {};
	node_color color = {};
	int subSize = 0;

	Node* parent = nullptr;
	Node* left = nullptr;
	Node* right = nullptr;
};

class BRTree {
public:
	Node* root = nullptr;


	BRTree() = default;

	void Insert(key_t key, value_t value = {});

	void Delete(key_t key);

	void DeleteNode(Node* inpNode);

	Node* FindNode(key_t key);

	Node* FindNextNode(Node* node);

	Node* FindPrevNode(Node* node);

	Node* GetMinNode(Node* node);

	Node* GetMaxNode(Node* node);

	Node* FindKthMin(int k, Node* curNode);

	Node* FindKthMax(int k, Node* curNode);

private:
	Node* Brother(Node* node, Node* parent = nullptr);

	Node* Grandparent(Node* node);

	Node* Uncle(Node* node);

	void RotateRight(Node* node);

	void RotateLeft(Node* node);

	void AppendNode(Node* node);

	void BalanceAppended(Node* node);

	void ReplaceChild(Node* node, Node* child);

	void Delete_case1(Node* node, Node* child);

	void Delete_case2(Node* node, Node* child);

	void Delete_case3(Node* node, Node* child);

	void Delete_case3_1(Node* node, Node* parent);

	void Delete_case3_2(Node* node, Node* parent);

	void Delete_case3_3(Node* node, Node* parent);

	void Delete_case3_4(Node* node, Node* parent);

	void Delete_case3_5(Node* node, Node* parent);

	void Delete_case3_6(Node* node, Node* parent);
};


Node* BRTree::Brother(Node* node, Node* parent) {
	if (node == nullptr && parent == nullptr) {
		return nullptr;
	} else if (node != nullptr) {
		parent = node->parent;
	}

	if (parent == nullptr) {
		return nullptr;
	}

	if (node == parent->left) {
		return parent->right;
	} else if (node == parent->right) {
		return parent->left;
	}

	return nullptr;
}


Node* BRTree::Grandparent(Node* node) {
	assert(node != nullptr);

	if ((node != nullptr) && (node->parent != nullptr)) {
		return node->parent->parent;
	} else {
		return nullptr;
	}
}


Node* BRTree::Uncle(Node* node) {
	assert(node != nullptr);

	struct Node* gpar = Grandparent(node);
	if (gpar == nullptr) {
		return nullptr;
	}
	if (node->parent == gpar->left) {
		return gpar->right;
	} else {
		return gpar->left;
	}
}


Node* BRTree::FindNode(key_t key) {
	Node* curNode = root;

	while (curNode != nullptr && curNode->key != key) {
		if (key < curNode->key) {
			curNode = curNode->left;
		} else {
			curNode = curNode->right;
		}
	}

	return curNode;
}


Node* BRTree::GetMinNode(Node* node) {
	assert(node != nullptr);

	Node* curNode = node;

	while (curNode->left != nullptr) {
		curNode = curNode->left;
	}

	return curNode;
}


Node* BRTree::GetMaxNode(Node* node) {
	assert(node != nullptr);

	Node* curNode = node;

	while (curNode->right != nullptr) {
		curNode = curNode->right;
	}

	return curNode;
}


Node* BRTree::FindNextNode(Node* node) {
	assert(node != nullptr);


	if (node->right == nullptr) {
		if (node->parent != nullptr && node->parent->key == node->key) {
			return node->parent;
		}

		Node* curNode = node;
		while (curNode->parent != nullptr && curNode != curNode->parent->left) {
			curNode = curNode->parent;
		}

		return curNode->parent;
	} else {
		return GetMinNode(node->right);
	}
}


Node* BRTree::FindPrevNode(Node* node) {
	assert(node != nullptr);


	if (node->left == nullptr) {
		if (node->parent != nullptr && node->parent->key == node->key) {
			return node->parent;
		}

		Node* curNode = node;
		while (curNode->parent != nullptr && curNode != curNode->parent->right) {
			curNode = curNode->parent;
		}

		return curNode->parent;
	} else {
		return GetMaxNode(node->left);
	}
}


void BRTree::RotateLeft(Node* node) {
	assert(node != nullptr);

	Node* pivot = node->right;

	pivot->parent = node->parent;
	if (node->parent != nullptr) {
		if (node->parent->left == node) {
			node->parent->left = pivot;
		} else {
			node->parent->right = pivot;
		}
	} else {
		root = pivot;
	}

	node->right = pivot->left;
	if (pivot->left != nullptr) {
		pivot->left->parent = node;
	}

	node->parent = pivot;
	pivot->left = node;

	int oldPivotSize = pivot->subSize;
	pivot->subSize = node->subSize;
	node->subSize -= oldPivotSize;
	if (node->right != nullptr) {
		node->subSize += node->right->subSize;
	}
}


void BRTree::RotateRight(Node* node) {
	assert(node != nullptr);

	struct Node* pivot = node->left;

	pivot->parent = node->parent;
	if (node->parent != nullptr) {
		if (node->parent->left == node) {
			node->parent->left = pivot;
		} else {
			node->parent->right = pivot;
		}
	} else {
		root = pivot;
	}

	node->left = pivot->right;
	if (pivot->right != nullptr) {
		pivot->right->parent = node;
	}

	pivot->right = node;
	node->parent = pivot;

	int oldPivotSize = pivot->subSize;
	pivot->subSize = node->subSize;
	node->subSize -= oldPivotSize;
	if (node->left != nullptr) {
		node->subSize += node->left->subSize;
	}
}


void BRTree::AppendNode(Node* node) {
	assert(node != nullptr);

	node->subSize = 1;

	if (root == nullptr) {
		root = node;;
		return;
	}

	Node* curNode = root;
	while (true) {
		++curNode->subSize;

		if (node->key < curNode->key) {
			if (curNode->left == nullptr) {
				curNode->left = node;
				node->parent = curNode;
				break;
			}
			curNode = curNode->left;
		} else {
			if (curNode->right == nullptr) {
				curNode->right = node;
				node->parent = curNode;
				break;
			}
			curNode = curNode->right;
		}
	}
}


void BRTree::BalanceAppended(Node* node) {
	assert(node != nullptr);

	if (node == root) {
		node->color = black_node;
		return;
	}
	if (node->parent->color == black_node) {
		return;
	}

	Node* uncle = Uncle(node);
	Node* grandp = Grandparent(node);
	if (uncle != nullptr && uncle->color == red_node) {
		node->parent->color = black_node;
		grandp->color = red_node;
		uncle->color = black_node;
		BalanceAppended(grandp);
		return;
	}

	if (node == node->parent->right && node->parent == grandp->left) {
		RotateLeft(node->parent);
		node = node->left;
	} else if (node == node->parent->left && node->parent == grandp->right) {
		RotateRight(node->parent);
		node = node->right;
	}

	grandp = Grandparent(node);
	node->parent->color = black_node;
	grandp->color = red_node;
	if (node == node->parent->left) {
		assert(node->parent == grandp->left);

		RotateRight(grandp);
	} else {
		assert(node == node->parent->right && node->parent == grandp->right);

		RotateLeft(grandp);
	}
	return;
}


void BRTree::Insert(key_t key, value_t value) {
	Node* newNode = new Node;
	newNode->key = key;
	newNode->value = value;
	newNode->color = red_node;

	AppendNode(newNode);

	BalanceAppended(newNode);
}


void BRTree::ReplaceChild(Node* node, Node* child) {
	assert(node != nullptr);
	assert(node->left == child || node->right == child);

	if (node->parent != nullptr) {
		if (node == node->parent->left) {
			node->parent->left = child;
		} else {
			assert(node == node->parent->right);

			node->parent->right = child;
		}
	} else {
		assert(root == node);

		root = child;
	}

	if (child != nullptr) {
		child->parent = node->parent;
	}
}


void BRTree::Delete_case1(Node* node, Node* child) {
	assert(node != nullptr);
	assert(node->color == red_node);
	assert(node->left == child || node->right == child);
	assert(child == nullptr || child->color == black_node);
	assert(node->parent != nullptr);

	ReplaceChild(node, child);
	delete node;
}


void BRTree::Delete_case2(Node* node, Node* child) {
	assert(node != nullptr);
	assert(node->color == black_node);
	assert(node->left == child || node->right == child);
	assert(child != nullptr && child->color == red_node);

	child->color = black_node;

	ReplaceChild(node, child);
	delete node;
}


void BRTree::Delete_case3(Node* node, Node* child) {
	assert(node != nullptr);
	assert(node->color == black_node);
	assert(node->left == child || node->right == child);
	assert(child == nullptr || child->color == black_node);

	Node* parent = node->parent;

	ReplaceChild(node, child);
	delete node;

	Delete_case3_1(child, parent);
}


void BRTree::Delete_case3_1(Node* node, Node* parent) {
	if (root == node) {
		assert(parent == nullptr);
		// Everything ok
	} else {
		assert(parent != nullptr);

		Delete_case3_2(node, parent);
	}
}


void BRTree::Delete_case3_2(Node* node, Node* parent) {
	assert(parent != nullptr);

	Node* brother = Brother(node, parent);
	assert(brother != nullptr);

	if (brother->color == red_node) {
		assert(parent->color == black_node);

		parent->color = red_node;
		brother->color = black_node;
		if (node == parent->left) {
			RotateLeft(parent);
		} else {
			assert(node == parent->right);

			RotateRight(parent);
		}

		Delete_case3_4(node, parent);
	} else {
		Delete_case3_3(node, parent);
	}
}


void BRTree::Delete_case3_3(Node* node, Node* parent) {
	assert(parent != nullptr);

	Node* brother = Brother(node, parent);
	assert(brother != nullptr);

	if (parent->color == black_node && brother->color == black_node &&
		(brother->left == nullptr || brother->left->color == black_node) &&
		(brother->right == nullptr || brother->right->color == black_node)) {

		brother->color = red_node;
		Delete_case3_1(parent, parent->parent);
	} else {
		Delete_case3_4(node, parent);
	}
}


void BRTree::Delete_case3_4(Node* node, Node* parent) {
	assert(parent != nullptr);

	Node* brother = Brother(node, parent);
	assert(brother != nullptr);

	if (parent->color == red_node && brother->color == black_node &&
		(brother->left == nullptr || brother->left->color == black_node) &&
		(brother->right == nullptr || brother->right->color == black_node)) {

		parent->color = black_node;
		brother->color = red_node;
	} else {
		Delete_case3_5(node, parent);
	}
}


void BRTree::Delete_case3_5(Node* node, Node* parent) {
	assert(parent != nullptr);

	Node* brother = Brother(node, parent);
	assert(brother != nullptr);

	if (brother->color == black_node) {
		if (node == parent->left &&
			(brother->left != nullptr && brother->left->color == red_node) &&
			(brother->right == nullptr || brother->right->color == black_node)) {

			brother->color = red_node;
			brother->left->color = black_node;
			RotateRight(brother);
		} else if (node == parent->right &&
			(brother->right != nullptr && brother->right->color == red_node) &&
			(brother->left == nullptr || brother->left->color == black_node)) {

			brother->color = red_node;
			brother->right->color = black_node;
			RotateLeft(brother);
		}
	}
	Delete_case3_6(node, parent);
}


void BRTree::Delete_case3_6(Node* node, Node* parent) {
	assert(parent != nullptr);

	Node* brother = Brother(node, parent);
	assert(brother != nullptr);
	assert(brother->color == black_node);

	swap(parent->color, brother->color);

	if (node == parent->left) {
		assert(brother->right != nullptr && brother->right->color == red_node);

		brother->right->color = black_node;
		RotateLeft(parent);
	} else {
		assert(node == parent->right);
		assert(brother->left != nullptr && brother->left->color == red_node);

		brother->left->color = black_node;
		RotateRight(parent);
	}
}


void BRTree::DeleteNode(Node* inpNode) {
	assert(inpNode != nullptr);

	Node* delNode = {};
	if (inpNode->right == nullptr) {
		delNode = inpNode;
	} else {
		delNode = GetMinNode(inpNode->right);
	}

	swap(inpNode->key, delNode->key);
	swap(inpNode->value, delNode->value);

	Node* curNode = delNode->parent;
	while (curNode != nullptr) {
		--curNode->subSize;
		curNode = curNode->parent;
	}

	Node* child = nullptr;
	if (delNode->left != nullptr) {
		assert(delNode->right == nullptr);

		child = delNode->left;
	} else {
		child = delNode->right;
	}

	if (delNode->color == red_node) {
		Delete_case1(delNode, child);
	} else if (child != nullptr && child->color == red_node) {
		Delete_case2(delNode, child);
	} else {
		Delete_case3(delNode, child);
	}
}


void BRTree::Delete(key_t key) {
	Node* node = FindNode(key);

	DeleteNode(node);
}


Node* BRTree::FindKthMin(int k, Node* curNode) {
	assert(k > 0);
	assert(curNode != nullptr);

	int curK = 1;
	if (curNode->left != nullptr) {
		curK += curNode->left->subSize;
	}

	if (k == curK) {
		return curNode;
	} else if (k < curK) {
		return FindKthMin(k, curNode->left);
	} else {
		return FindKthMin(k - curK, curNode->right);
	}
}


Node* BRTree::FindKthMax(int k, Node* curNode) {
	assert(k > 0);
	assert(curNode != nullptr);

	int curK = 1;
	if (curNode->right != nullptr) {
		curK += curNode->right->subSize;
	}

	if (k == curK) {
		return curNode;
	} else if (k < curK) {
		return FindKthMax(k, curNode->right);
	} else {
		return FindKthMax(k - curK, curNode->left);
	}
}

// End of red-black tree realization

enum event_types {
	bankrupt_event = 1,
	split_event = 2
};


BRTree ParseInitTree(FILE* fin, int n, long long& sqSum) {
	assert(fin != nullptr);
	assert(n > 0);
	assert(sqSum == 0);

	BRTree resTree;

	int curFacilityPos = 1;
	int curFacilityLen = 0;
	for (int i = 0; i < n; ++i) {
		fscanf(fin, "%d", &curFacilityLen);
		resTree.Insert(curFacilityPos, { curFacilityLen });
		sqSum += curFacilityLen * curFacilityLen;
		curFacilityPos += curFacilityLen;
	}

	return resTree;
}


void BankruptEvent(BRTree& tree, int facilityPos, long long& sqSum) {
	assert(tree.root != nullptr);
	assert(facilityPos >= 0);

	Node* curFacility = {};
	Node* nextFacility = {};
	Node* prevFacility = {};
	if (facilityPos == 1) {
		curFacility = tree.GetMinNode(tree.root);
		nextFacility = tree.FindNextNode(curFacility);
		sqSum = sqSum - static_cast<long long>(curFacility->value.length) * curFacility->value.length -
			static_cast<long long>(nextFacility->value.length) * nextFacility->value.length;

		int nextLen = nextFacility->value.length + curFacility->value.length;
		nextFacility->value.length = nextLen;
		nextFacility->key = 1;
		tree.DeleteNode(curFacility);

		sqSum += static_cast<long long>(nextLen) * nextLen;

	} else if (facilityPos == tree.root->subSize) {
		curFacility = tree.GetMaxNode(tree.root);
		prevFacility = tree.FindPrevNode(curFacility);
		sqSum = sqSum - static_cast<long long>(curFacility->value.length) * curFacility->value.length -
			static_cast<long long>(prevFacility->value.length) * prevFacility->value.length;

		int prevLen = prevFacility->value.length + curFacility->value.length;
		prevFacility->value.length = prevLen;
		tree.DeleteNode(curFacility);

		sqSum += static_cast<long long>(prevLen) * prevLen;

	} else {
		assert(facilityPos >= 1 && facilityPos <= tree.root->subSize);

		curFacility = tree.FindKthMin(facilityPos, tree.root);
		prevFacility = tree.FindPrevNode(curFacility);
		nextFacility = tree.FindNextNode(curFacility);
		assert(curFacility != nullptr);
		assert(prevFacility != nullptr);
		assert(nextFacility != nullptr);

		sqSum = sqSum - static_cast<long long>(curFacility->value.length) * curFacility->value.length -
			static_cast<long long>(prevFacility->value.length) * prevFacility->value.length -
			static_cast<long long>(nextFacility->value.length) * nextFacility->value.length;

		int prevLen = prevFacility->value.length + curFacility->value.length / 2;
		int nextLen = nextFacility->value.length + (curFacility->value.length + 1) / 2;
		prevFacility->value.length = prevLen;
		nextFacility->value.length = nextLen;
		nextFacility->key -= (curFacility->value.length + 1) / 2;
		tree.DeleteNode(curFacility);

		sqSum = sqSum + static_cast<long long>(prevLen) * prevLen +
			static_cast<long long>(nextLen) * nextLen;
	}
}


void SplitEvent(BRTree& tree, int facilityPos, long long& sqSum) {
	assert(tree.root != nullptr);
	assert(facilityPos >= 0);

	Node* curFacility = tree.FindKthMin(facilityPos, tree.root);
	
	sqSum -= static_cast<long long>(curFacility->value.length) * curFacility->value.length;
	
	int firstLen = curFacility->value.length / 2;
	int secondLen = (curFacility->value.length + 1) / 2;

	curFacility->value.length = firstLen;
	tree.Insert(curFacility->key + firstLen, { secondLen });

	sqSum = sqSum + static_cast<long long>(firstLen) * firstLen +
		static_cast<long long>(secondLen) * secondLen;
}


void HandleEvent(BRTree& tree, FILE* fin, long long& sqSum) {
	assert(tree.root != nullptr);
	assert(fin != nullptr);

	int eventType = 0;
	int facilityPos = 0;
	fscanf(fin, "%d %d", &eventType, &facilityPos);
	switch (eventType) {
	case bankrupt_event:
		BankruptEvent(tree, facilityPos, sqSum);
		break;
	case split_event:
		SplitEvent(tree, facilityPos, sqSum);
		break;
	default:
		assert(0);
	}

}


int main() {
	FILE* fin = fopen("river.in", "r");
	FILE* fout = fopen("river.out", "w");

	int n = 0;
	fscanf(fin, "%d%*d", &n);
	long long sqSum = 0;
	BRTree tree = ParseInitTree(fin, n, sqSum);
	fprintf(fout, "%lld\n", sqSum);

	int k = 0;
	fscanf(fin, "%d", &k);
	for (int i = 0; i < k; ++i) {
		HandleEvent(tree, fin, sqSum);
		fprintf(fout, "%lld\n", sqSum);
	}

	fclose(fin);
	fclose(fout);
	return 0;
}