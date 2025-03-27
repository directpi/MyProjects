[К оглавлению](../../README.md)

## D. Анаграмма?

Задано две строки, нужно проверить, является ли одна анаграммой другой. Анаграммой называется строка, полученная из другой перестановкой букв.

Формат ввода
Строки состоят из строчных латинских букв, их длина не превосходит 100000. Каждая записана в отдельной строке.

Формат вывода
Выведите "YES" если одна из строк является анаграммой другой и "NO" в противном случае.

```cpp
#include <algorithm>
#include <iostream>

int main() {
    std::string str1;
    std::string str2;

    std::getline(std::cin, str1);
    std::getline(std::cin, str2);
    std::sort(str1.begin(), str1.end());
    std::sort(str2.begin(), str2.end());
    if (str1 == str2) {
        std::cout << "YES";
    } else {
        std::cout << "NO";
    }
}
```