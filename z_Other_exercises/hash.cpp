#include <iostream>
#include <string>
#include <vector>
using namespace std;
void sub_string(const vector<vector<int>> array, const string str, int q) {
    string str_a, str_b;
    for (int i = 0; i < q; ++i) {
        str_a = str.substr(array[1][i], array[0][i]);
        str_b = str.substr(array[2][i], array[0][i]);

        if (str_a == str_b) {
            cout << "yes"
                 << "\n";
        } else {
            cout << "no"
                 << "\n";
        }
    }
}
int main() {
    string str;
    int q;
    cin >> str;
    cin >> q;
    vector<vector<int>> array(3, vector<int>(q));
    for (int i = 0; i < q; ++i) {
        cin >> array[0][i] >> array[1][i] >> array[2][i];
    }
    sub_string(array, str, q);
}