#include <iostream>
# include <unordered_map>
# include <vector>
# include <string>
# include <queue>



class Node {
public:

	Node(Node* fail_node = nullptr) : fail(fail_node), term(nullptr), out(-1) {}


	Node* PrefLink(const char c) {
		if (prefLinks.count(c) > 0)
			return prefLinks[c];
		else
			return nullptr;
	}


	bool isTerminal() {
		return (out >= 0);
	}


	std::unordered_map<char, Node*> prefLinks;
	Node* fail;  // Предыдущее состояние для функции отката. Только для root равно nullptr.
	Node* term; // Ближайшее терминальное состояние. Если отстутствует - nullptr
	int out;
};

class AhoCorasick {
public:
	typedef void (*Callback) (const char* substr);
	Node root;
	std::vector<std::string> words;
	Node* current_state;

	void addString(const char* const str)
	{
		Node* current_node = &root;
		for (const char* cp = str; *cp; ++cp) {
			Node* child_node = current_node->PrefLink(*cp);
			if (!child_node) {
				child_node = new Node(&root);
				current_node->prefLinks[*cp] = child_node;
			}
			current_node = child_node;
		}
		current_node->out = words.size();
		words.push_back(str);
	}

	void init() {
		std::queue<Node*> q;
		q.push(&root);
		while (!q.empty()) {
			Node* current_node = q.front();
			q.pop();
			for (std::unordered_map<const char, Node*>::const_iterator iter = current_node->prefLinks.cbegin();
				iter != current_node->prefLinks.cend(); ++iter)
			{
				const char symbol = iter->first;
				Node* child = iter->second;

				// Defining .fail for the childnode
				Node* temp_node = current_node->fail;
				while (temp_node) {
					Node* fail_candidate = temp_node->PrefLink(symbol);
					if (fail_candidate) {
						child->fail = fail_candidate;
						break;
					}
					temp_node = temp_node->fail;
				}

				// Defining .term for the childnode using .term of current node
				if (child->fail->isTerminal()) {
					child->term = child->fail;
				} else {
					child->term = child->fail->term;
				}
				q.push(child);
			}
		}
	}

	void step(const char c) {
		while (current_state) {
			Node* candidate = current_state->PrefLink(c);
			if (candidate) {
				current_state = candidate;
				return;
			}
			current_state = current_state->fail;
		}
		current_state = &root;
	}

	void printTermsForCurrentState(Callback callback) const {
		if (current_state->isTerminal()) {
			callback(words[current_state->out].c_str());
		}
		Node* temp_node = current_state->term;
		while (temp_node) {
			callback(words[temp_node->out].c_str());
			temp_node = temp_node->term;
		}
	}

	void search(const char* str, Callback callback) {
		current_state = &root;
		for (; *str; ++str) {
			step(*str);
			printTermsForCurrentState(callback);
		}
	}
};

void print(const char* str) {
	std::cout << "found substring " << str << "\n";
}

int main() {
	AhoCorasick ak;

	ak.addString("test");
	ak.addString("rok");
	ak.addString("roka");
	ak.addString("strok");
	ak.addString("t");

	ak.init();

	ak.search("testovaya_stroka!", print);

	std::cin.get();

	return 0;
}