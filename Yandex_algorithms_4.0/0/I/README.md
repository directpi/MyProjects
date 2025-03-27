[К оглавлению](../../README.md)

## I. Правильная скобочная последовательность

Рассмотрим последовательность, состоящую из круглых, квадратных и фигурных скобок. Программа должна определить, является ли данная скобочная последовательность правильной. Пустая последовательность является правильной. Если A — правильная, то последовательности (A), [A], {A} — правильные. Если A и B — правильные последовательности, то последовательность AB — правильная.

Формат ввода
В единственной строке записана скобочная последовательность, содержащая не более 100000 скобок.

Формат вывода
Если данная последовательность правильная, то программа должна вывести строку "yes", иначе строку "no".

```cpp
#include <iostream>
#include <stack>
#include <string>
using namespace std;
bool check_string(const string& str, int n) {
    stack<char> st_brackets;

    for (int i = 0; i <= n; ++i) {
        char c = str[i];
        if (c == '(' || c == '{' || c == '[') {
            st_brackets.push(c);
        } else {
            if (st_brackets.empty()) {
                return false;
            }
            char top = st_brackets.top();
            if ((top == '(' && c == ')') || (top == '{' && c == '}') || (top == '[' && c == ']')) {
                st_brackets.pop();
            } else {
                return false;
            }
        }
    }

    return st_brackets.empty();
}

int main() {
    string str;
    getline(cin, str);
    if (check_string(str, str.size() - 1)) {
        cout << "yes";
    } else {
        cout << "no";
    }
}
```