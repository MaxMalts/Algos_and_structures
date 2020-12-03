#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <assert.h>



enum Color {
	undefined,
	red,
	green,
	blue,
};

struct Node {

	Node() = default;

	Node(const char prevCh, Node* parent) : prevChar(prevCh), parent(parent) {}

	bool IsTerminate() {
		return !wordIds.empty();
	}

	std::unordered_map<char, Node*> go;
	char prevChar = 0;
	std::vector<int> wordIds;
	Node* sufLink = nullptr;
	Node* parent = nullptr;

	Color color = undefined;
};


class Trie {
public:

	Trie(const std::vector<char>& alphabet) : alphabet(alphabet), rootNode(new Node) {}

	void AddWord(const std::string& word) {
		Node* curNode = rootNode;
		for (int i = 0; i < word.length(); ++i) {
			char curCh = word[i];
			if (curNode->go.count(curCh) == 0)
				curNode->go.insert(std::pair<char, Node* >(curCh, new Node(curCh, curNode)));

			curNode = curNode->go[curCh];
		}

		curNode->wordIds.push_back(lastWordId++);
	}


	void CreateSuflinks() {
		std::queue<Node*> pendingNodes;;

		rootNode->sufLink = rootNode;
		for (auto rootChild : rootNode->go)
			pendingNodes.push(rootChild.second);

		while (!pendingNodes.empty()) {
			Node* curNode = pendingNodes.front();
			pendingNodes.pop();

			for (auto child : curNode->go)
				pendingNodes.push(child.second);

			if (curNode->parent == rootNode) {
				curNode->sufLink = rootNode;
				continue;
			}

			Node* tempNode = curNode->parent->sufLink;
			while (tempNode->go.count(curNode->prevChar) == 0 && tempNode != rootNode)
				tempNode = tempNode->sufLink;

			if (tempNode->go.count(curNode->prevChar) > 0)
				curNode->sufLink = tempNode->go[curNode->prevChar];
			else
				curNode->sufLink = rootNode;
		}
	}


	std::unordered_map<int, std::vector<int>> FindEntries(const std::string& text) {
		std::unordered_map<int, std::vector<int>> res;
		Node* curNode = rootNode;

		for (size_t i = 0; i < text.size(); ++i) {
			char curCh = text[i];
			curNode = Jump(curNode, curCh);
			std::vector<int> wordIds = TermIds(curNode);
			for (int wordId : wordIds)
				res[wordId].push_back(i);
		}

		return res;
	}


	bool SafeInfWordExists() {
		return SafeInfWordExistsDfs(rootNode);
	}



private:

	Node* Jump(Node* node, const char c) {
		while (node->go.count(c) == 0 && node != rootNode)
			node = node->sufLink;

		if (node->go.count(c) > 0)
			return node->go[c];

		return rootNode;
	}


	std::vector<int> TermIds(Node* node) {
		std::vector<int> res;

		do {
			if (node->IsTerminate())
				res.insert(res.end(), node->wordIds.begin(), node->wordIds.end());
			node = node->sufLink;
		} while (node != rootNode);

		return res;
	}


	Node* TermLink(Node* node) {
		while (node->wordIds.empty() && node != rootNode)
			node = node->sufLink;

		return node;
	}


	bool SafeInfWordExistsDfs(Node* curNode) {
		assert(curNode != nullptr);

		switch (curNode->color) {
		case red:
			return true;

		case blue:
			return false;
		}
		curNode->color = red;

		for (int i = 0; i < alphabet.size(); ++i) {
			Node* jumpNode = Jump(curNode, alphabet[i]);
			if (TermLink(jumpNode) == rootNode && SafeInfWordExistsDfs(jumpNode))
				return true;
		}
		curNode->color = blue;

		return false;
	}


	std::vector<char> alphabet;

	Node* rootNode = nullptr;
	int lastWordId = 0;
};



int main() {
	int N = 0;
	std::cin >> N;

	Trie trie({ '0', '1' });

	std::vector<std::string> vocab(N);
	for (int i = 0; i < N; ++i) {
		std::cin >> vocab[i];
		trie.AddWord(vocab[i]);
	}

	trie.CreateSuflinks();

	if (trie.SafeInfWordExists())
		std::cout << "TAK";
	else
		std::cout << "NIE";

	return 0;
}