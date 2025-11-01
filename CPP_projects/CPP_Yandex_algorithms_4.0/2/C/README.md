[К оглавлению](../../README.md)

## C. Z-функция

Ограничение времени	2 секунды
Ограничение памяти	256Mb
Ввод	стандартный ввод или input.txt
Вывод	стандартный вывод или output.txt

Дана непустая строка S, длина которой N не превышает 106. Будем считать, что элементы строки нумеруются от 0 до N-1.

Вычислите z-функцию z[i] для всех i от 0 до N-1. z[i] определяется как максимальная длина подстроки, начинающейся с позиции i и совпадающей с префиксом всей строки. z[0] = 0

Формат ввода
Одна строка длины N, 0 < N ≤ 106, состоящая из прописных латинских букв.

Формат вывода
Выведите N чисел — значения z-функции для каждой позиции, разделённые пробелом.

```cpp
#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
using namespace std;

void z_function(string str) {
    int n = (int)str.length();
    vector<int> z(n);
    for (int i = 1, l = 0, r = 0; i < n; ++i) {
        if (i <= r) z[i] = min(r - i + 1, z[i - l]);

        while (i + z[i] < n && str[z[i]] == str[i + z[i]]) ++z[i];
        if (i + z[i] - 1 > r) l = i, r = i + z[i] - 1;
    }
    for (int k = 0; k < n; ++k) {
        cout << z[k] << "\n";
    }
}

int main() {
    string str;
    vector<int> z(string str);
    getline(cin, str);
    z_function(str);
}
```