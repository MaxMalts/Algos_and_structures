#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <assert.h>



class OffsetString : public std::string {
public:

	OffsetString() = default;

	OffsetString(const long long offset) : offset(offset) {}

	OffsetString(std::string& str, const long long offset = 0) : std::string(str), offset(offset % str.length()) {}

	OffsetString(const OffsetString& other) : std::string(other), offset(other.offset) {}


	char& operator[](const long long ind) {
		return std::string::operator[]((offset + ind) % this->length());
	}

	const char& operator[](const long long ind) const {
		return std::string::operator[]((offset + ind) % this->length());
	}

private:

	long long offset = 0;
};


struct Node {

	Node() = default;

	Node(const bool isRoot) : isRoot(isRoot) {}

	bool isRoot = false;

	std::unordered_map<char, Node*> go;
	std::unordered_set<long long> waitingDnas;
};


struct DNA {
	DNA(const OffsetString& str, Node* node) : str(str), waitingNode(node) {}

	OffsetString str;
	Node* waitingNode = nullptr;
	size_t curPos = 0;
};


std::vector<DNA> dnas;
Node rootNode(true);


bool CanDecode(const long long ind) {

	DNA& curDna = dnas[ind];
	Node* oldNode = curDna.waitingNode;

	Node* curNode = oldNode;
	while (curDna.curPos < curDna.str.length()) {
		char curCh = curDna.str[curDna.curPos];

		if (curNode->go.count(curCh) > 0) {
			++curDna.curPos;
			curNode = curNode->go[curCh];

		} else if (!curNode->isRoot && curNode->go.empty()) {
			curNode = &rootNode;

		} else {
			break;
		}
	}
	oldNode->waitingDnas.erase(ind);

	if (curDna.curPos == curDna.str.length() && curNode->go.empty())
		return true;

	curDna.waitingNode = curNode;

	if (curDna.curPos < curDna.str.length())
		curNode->waitingDnas.insert(ind);

	return false;
}


long long InsertDna(const OffsetString& str) {

	long long curInd = dnas.size();

	dnas.push_back(DNA(str, &rootNode));
	rootNode.waitingDnas.insert(curInd);

	if (CanDecode(curInd))
		return curInd;
	return -1;
}


void InsertGene(const OffsetString& str, std::vector<long long>& canDecode) {

	std::vector<long long> needToHandle;

	Node* curNode = &rootNode;
	for (long long i = 0; i < str.length(); ++i) {

		if (curNode->go.count(str[i]) == 0) {
			curNode->go.insert({ str[i], new Node });

			for (long long curInd : curNode->waitingDnas)
				if (dnas[curInd].str[dnas[curInd].curPos] == str[i])
					needToHandle.push_back(curInd);
		}

		curNode = curNode->go[str[i]];
	}

	for (long long i = 0; i < needToHandle.size(); ++i)
		if (CanDecode(needToHandle[i]))
			canDecode.push_back(needToHandle[i]);
}



int main() {
	long long n = 0;
	std::cin >> n;

	long long prevSize = 0;
	for (long long i = 0; i < n; ++i) {
		char curComm = 0;
		std::cin >> curComm;

		OffsetString curStr(prevSize);
		std::cin >> curStr;

		switch (curComm) {
		case '+': {
			std::vector<long long> curAns;
			InsertGene(curStr, curAns);

			std::cout << curAns.size() << ' ';
			for (long long j = 0; j < curAns.size(); ++j)
				std::cout << curAns[j] + 1 << ' ';

			prevSize = curAns.size();
			break;
		}

		case '?': {
			long long curAns = InsertDna(curStr);

			if (curAns != -1) {
				std::cout << "1 " << curAns + 1;
				prevSize = 1;
			} else {
				std::cout << '0';
				prevSize = 0;
			}

			break;
		}

		default:
			assert(0);
		}
		std::cout << '\n';
	}

	return 0;
}