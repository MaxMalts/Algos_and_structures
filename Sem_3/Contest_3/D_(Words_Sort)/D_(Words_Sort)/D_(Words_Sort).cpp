#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <assert.h>



struct Word {
	char* pos = nullptr;
	int len = 0;
};


class Data {
public:

	void Read() {
		char* cursor = str;

		*cursor = getchar();
		if (*cursor != '.')
			words.push_back({ cursor, 1 });

		*(++cursor) = getchar();
		while (*cursor != EOF && *cursor != 0 && *cursor != '\n') {

			if (*cursor != '.' && *(cursor - 1) != '.')
				++(words[words.size() - 1].len);
			else if (*cursor != '.' && *(cursor - 1) == '.')
				words.push_back({ cursor, 1 });

			*(++cursor) = getchar();
		}
		*cursor = '\0';

		sortedWords = words;
	}


	void Sort() {
		if (words.size() == 0)
			return;

		std::sort(sortedWords.begin(), sortedWords.end(), WordsCmp);
	}


	void Print() {
		if (words.size() == 0) {
			printf("%s", str);
			return;
		}

		int curDotsLen = words[0].pos - str;
		fwrite(str, 1, curDotsLen, stdout);

		for (int i = 0; i < sortedWords.size() - 1; ++i) {
			fwrite(sortedWords[i].pos, 1, sortedWords[i].len, stdout);

			curDotsLen = words[i + 1].pos - words[i].pos - words[i].len;
			fwrite(words[i].pos + words[i].len, 1, curDotsLen, stdout);
		}

		fwrite(sortedWords[sortedWords.size() - 1].pos, 1, sortedWords[sortedWords.size() - 1].len, stdout);

		printf("%s", words[words.size() - 1].pos + words[words.size() - 1].len);
	}


private:

	static bool WordsCmp(const Word& word1, const Word& word2) {
		int minLen = std::min(word1.len, word2.len);
		int cmp = strncmp(word1.pos, word2.pos, minLen);

		if (cmp == 0)
			return word1.len < word2.len;

		return cmp < 0;
	}

	char str[1000001] = "";
	std::vector<Word> words;
	std::vector<Word> sortedWords;
};


int main() {
	Data test;
	test.Read();
	test.Sort();
	test.Print();

	return 0;
}