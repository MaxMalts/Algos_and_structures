#include <iostream>
#include <assert.h>

using namespace std;

typedef int key_t;

enum node_color {
	black_node,
	red_node
};

struct Node {
	key_t key = {};
	node_color color = {};

	Node* parent = nullptr;
	Node* left = nullptr;
	Node* right = nullptr;
};

class BRTree {
public:
	Node* root = nullptr;


	BRTree() = default;

	void Insert(key_t key);

	void Delete(key_t key);

	bool ElemExists(key_t key);

	key_t FindNextElem(key_t key);

	bool NextExists(key_t key);

private:
	Node* FindNode(key_t key);

	Node* FindNextNode(Node* node);

	Node* Brother(Node* node, Node* parent = nullptr);

	Node* Grandparent(Node* node);

	Node* Uncle(Node* node);

	Node* LeftestNode(Node* node);

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


bool BRTree::ElemExists(key_t key) {
	if (FindNode(key) != nullptr) {
		return true;
	} else {
		return false;
	}
}


key_t BRTree::FindNextElem(key_t key) {
	Node* inpNode = FindNode(key);
	assert(inpNode != nullptr);
	
	Node* nextNode = FindNextNode(inpNode);
	assert(nextNode != nullptr);

	return nextNode->key;
}


Node* BRTree::LeftestNode(Node* node) {
	assert(node != nullptr);

	Node* curNode = node;

	while (curNode->left != nullptr) {
		curNode = curNode->left;
	}

	return curNode;
}


bool BRTree::NextExists(key_t key) {
	Node* inpNode = FindNode(key);
	assert(inpNode != nullptr);

	if (FindNextNode(inpNode) != nullptr) {
		return true;
	} else {
		return false;
	}
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
		return LeftestNode(node->right);
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
}


void BRTree::AppendNode(Node* node) {
	assert(node != nullptr);

	if (root == nullptr) {
		root = node;
		return;
	}

	Node* curNode = root;

	while (true) {
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


void BRTree::Insert(key_t key) {
	Node* newNode = new Node;
	newNode->key = key;
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


void BRTree::Delete(key_t key) {
	Node* inpNode = FindNode(key);
	assert(inpNode != nullptr);

	Node* delNode = {};
	if (inpNode->right == nullptr) {
		delNode = inpNode;
	} else {
		delNode = LeftestNode(inpNode->right);
	}
	swap(inpNode->key, delNode->key);

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

// End of black-red tree realization



void AddCommand(BRTree& tree, char& lastComm, int& lastOutp) {
	int param = 0;
	scanf("%d%*c", &param);

	if (lastComm == '?') {
		param = (param + lastOutp) % 1000000000;
	}

	if (!tree.ElemExists(param)) {
		tree.Insert(param);
	}

	lastComm = '+';
}


void NextCommand(BRTree& tree, char& lastComm, int& lastOutp) {
	int param = 0;
	scanf("%d%*c", &param);

	if (tree.ElemExists(param)) {
		printf("%d\n", param);

		lastComm = '?';
		lastOutp = param;
		return;
	}
	tree.Insert(param);

	if (!tree.NextExists(param)) {
		printf("-1\n");
		lastOutp = -1;
	} else {
		key_t nextElem = tree.FindNextElem(param);
		printf("%d\n", nextElem);
		lastOutp = nextElem;
	}

	tree.Delete(param);

	lastComm = '?';
}


void ParseCommand(BRTree& tree, char& lastComm, int& lastOutp) {
	char curCommand = 0;
	scanf("%c ", &curCommand);

	switch (curCommand) {
	case '+':
		AddCommand(tree, lastComm, lastOutp);
		break;
	case '?':
		NextCommand(tree, lastComm, lastOutp);
		break;
	default:
		assert(0);
	}
}


int main() {
	int n = 0;
	scanf("%d%*c", &n);

	BRTree tree;
	char lastComm = 0;
	int lastOutp = 0;
	for (int i = 0; i < n; ++i) {
		ParseCommand(tree, lastComm, lastOutp);
	}

	return 0;
}