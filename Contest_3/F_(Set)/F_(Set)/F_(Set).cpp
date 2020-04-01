#include <iostream>
#include <set>
#include <map>
#include <string>
#include <vector>
#include <assert.h>

using namespace std;


#ifndef _DEBUG
#define assert(cond) ;
#endif


const int maxCount = 100000;
typedef long long elem_t;


int M = 0;

class Num_Sets {
public:
	void Add(elem_t e, int s) {
		assert(s >= 0);

		sets[s].insert(e);

		elemSets[e].insert(s);
	}


	void Delete(elem_t e, int s) {
		assert(s >= 0);

		sets[s].erase(e);

		elemSets[e].erase(s);
	}


	void Clear(int s) {
		assert(s >= 0);

		set<elem_t>::iterator curElem = sets[s].begin();
		for (int i = 0; i < sets[s].size(); ++i) {
			elemSets[*curElem].erase(s);
			++curElem;
		}
		assert(curElem == sets[s].end());

		sets[s].clear();
	}


	void ListSet(int s) {
		assert(s >= 0);

		int size = sets[s].size();
		if (size == 0) {
			cout << -1 << '\n';
		} else {
			set<elem_t>::iterator curElem = sets[s].begin();

			for (int i = 0; i < size; ++i) {
				cout << *curElem << ' ';
				++curElem;
			}
			assert(curElem == sets[s].end());

			cout << '\n';
		}
	}


	void ListSetsOf(elem_t e) {
		map<elem_t, set<int>>::iterator foundElem = elemSets.find(e);
		if (foundElem == elemSets.end() || foundElem->second.size() == 0) {
			cout << -1 << '\n';

		} else {
			set<int>::iterator curSet = foundElem->second.begin();
			for (int i = 0; i < foundElem->second.size(); ++i) {
				cout << *curSet << ' ';
				++curSet;
			}
			cout << '\n';
		}
	}

private:
	set<elem_t> sets[maxCount + 1];
	map<elem_t, set<int>> elemSets;
};



void ParseRequest(Num_Sets& sets) {
	string input;
	cin >> input;

	elem_t e = 0;
	int s = 0;
	switch (input[0]) {
	case 'A':
		cin >> e >> s;
		sets.Add(e, s);
		break;

	case 'D':
		cin >> e >> s;
		sets.Delete(e, s);
		break;

	case 'C':
		cin >> s;
		sets.Clear(s);
		break;

	case 'L':
		if (input.compare("LISTSET") == 0) {
			cin >> s;
			sets.ListSet(s);
		} else {
			assert(input.compare("LISTSETSOF") == 0);
			cin >> e;
			sets.ListSetsOf(e);
		}
		break;

	default:
		assert(0);
	}
}


int main() {
	ios_base::sync_with_stdio(false);
	cin.tie(NULL);

	long long N = 0;
	int K = 0;
	cin >> N >> M >> K;

	Num_Sets sets;
	for (int i = 0; i < K; ++i) {
		ParseRequest(sets);
	}

	return 0;
}


// Hallelujah!