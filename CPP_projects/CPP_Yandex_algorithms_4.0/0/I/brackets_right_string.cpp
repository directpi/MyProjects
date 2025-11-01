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