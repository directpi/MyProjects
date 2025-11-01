#include <iostream>
#include <map>
#include <set>
#include <string>

using namespace std;

int main() {
  int words_count = 0;
  string word;
  map<char, int> letter_count;
  while (cin >> word) {
    set<char> letters(word.begin(), word.end());
    ++words_count;
    for (char ch : letters) {
      ++letter_count[ch];
    }
  }
  for (auto [ch, n] : letter_count) {
    if (n == words_count) {
      cout << ch;
    }
  }
}