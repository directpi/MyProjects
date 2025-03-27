#include <iostream>
#include <string>
#include <vector>

int main() {
    std::vector<std::string> words;

    size_t words_count;
    std::cin >> words_count;

    // Размер вектора остаётся нулевым, меняется только резерв:
    words.reserve(words_count);

    for (size_t i = 0; i != words_count; ++i) {
        std::string word;
        std::cin >> word;
        // Все добавления будут дешёвыми, без реаллокаций:
        words.push_back(word);
    }
}