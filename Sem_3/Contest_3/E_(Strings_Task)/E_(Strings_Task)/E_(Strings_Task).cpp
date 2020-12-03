#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <queue>


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
};


class Trie {
public:

	Trie() : rootNode(new Node) {}

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


	Node* Jump(Node* node, const char c) {
		while (node->go.count(c) == 0 && node != rootNode)
			node = node->sufLink;

		if (node->go.count(c) > 0)
			return node->go[c];

		return rootNode;
	}


	std::vector<int> TermIds(Node* curNode) {
		std::vector<int> res;

		do {
			if (curNode->IsTerminate())
				res.insert(res.end(), curNode->wordIds.begin(), curNode->wordIds.end());
			curNode = curNode->sufLink;
		} while (curNode != rootNode);

		return res;
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

private:

	Node* rootNode = nullptr;
	int lastWordId = 0;
};



int main() {
	std::string text;
	std::cin >> text;

	int n = 0;
	std::cin >> n;

	Trie trie;

	std::vector<std::string> vocab(n);
	for (int i = 0; i < n; ++i) {
		std::cin >> vocab[i];
		trie.AddWord(vocab[i]);
	}

	trie.CreateSuflinks();

	std::unordered_map<int, std::vector<int>> entries = trie.FindEntries(text);

	for (int i = 0; i < n; ++i) {
		if (entries.count(i) == 0) {
			std::cout << '0';
			
		} else {
			std::cout << entries[i].size();
			for (int pos : entries[i])
				std::cout << ' ' << (pos + 2 - vocab[i].size());
		}
		std::cout << '\n';
	}

	return 0;
}