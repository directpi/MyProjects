#include <cmath>
#include <iostream>
#include <string>
#include <vector>
using namespace std;

void print_s(int i, const vector<vector<string>>& array, int n) {
    int c_empty = 0, not_empty = 0;
    cout << "**********"
         << "\n";
    cout << "Phase " << i << "\n";
    for (int k = 0; k < 10; ++k) {
        cout << "Bucket " << k << ": ";
        for (int w = 0; w < n; ++w) {
            if (array[k][w] == "" && c_empty == 0 && w == 0) {
                cout << "empty";
                ++c_empty;

            } else if (array[k][w] != "") {
                if (not_empty != 0 && w < (n) && w != 0) {
                    cout << ", ";
                    ++c_empty;
                }
                cout << array[k][w];
                ++not_empty;
            }
        }
        cout << "\n";
        c_empty = 0;
        not_empty = 0;
    }
}
void sort(vector<string>& strings, int n, int len) {
    int phase = 0;

    for (int i = len - 1; i >= 0; --i) {
        ++phase;
        vector<vector<string>> array(10, vector<string>(n));
        int c1 = 0, c2 = 0, c3 = 0, c4 = 0, c5 = 0, c6 = 0, c7 = 0, c8 = 0, c9 = 0, c0 = 0;
        for (int r = 0; r < n; ++r) {
            switch (strings[r][i]) {
                case '0':
                    array[0][c0] = strings[r];
                    ++c0;
                    break;
                case '1':
                    array[1][c1] = strings[r];
                    ++c1;
                    break;
                case '2':
                    array[2][c2] = strings[r];
                    ++c2;
                    break;
                case '3':
                    array[3][c3] = strings[r];
                    ++c3;
                    break;
                case '4':
                    array[4][c4] = strings[r];
                    ++c4;
                    break;
                case '5':
                    array[5][c5] = strings[r];
                    ++c5;
                    break;
                case '6':
                    array[6][c6] = strings[r];
                    ++c6;
                    break;
                case '7':
                    array[7][c7] = strings[r];
                    ++c7;
                    break;
                case '8':
                    array[8][c8] = strings[r];
                    ++c8;
                    break;
                case '9':
                    array[9][c9] = strings[r];
                    ++c9;
                    break;

                default:
                    break;
            }
        }
        int count = 0;
        for (int m = 0; m < 10; ++m) {
            for (int q = 0; q < n; ++q) {
                if (array[m][q] != "") {
                    strings[count] = array[m][q];
                    ++count;
                }
            }
        }
        print_s(phase, array, n);
    }
}

int main() {
    int n;
    cin >> n;
    int len;
    vector<string> strings;

    string str;

    for (int i = 0; i < n; ++i) {
        cin >> str;

        strings.push_back(str);
    }
    len = str.length();
    cout << "Initial array:"
         << "\n";
    for (int i = 0; i < n; ++i) {
        cout << strings[i];
        if (i < n - 1) cout << ", ";
    }
    cout << "\n";

    sort(strings, n, len);
    cout << "**********"
         << "\n"
         << "Sorted array:"
         << "\n";
    for (int h = 0; h < n; ++h) {
        cout << strings[h];
        if (h < n - 1) cout << ", ";
    }
}