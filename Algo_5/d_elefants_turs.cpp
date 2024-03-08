#include <iostream>
#include <vector>
using namespace std;

int main() {
    char c;
    int count = 0;
    vector<vector<int>> desk(10, vector<int>(10));
    for (int k = 0; k < 10; ++k) {
        desk[0][k] = -1;
        desk[k][0] = -1;
        desk[9][k] = -1;
        desk[k][9] = -1;
    }
    for (int i = 1; i < 9; ++i) {
        for (int j = 1; j < 9; ++j) {
            cin >> c;
            if (c == '*') {
                desk[i][j] = 0;  //ничего
            } else if (c == 'B') {
                desk[i][j] = 1;  // слон
            } else if (c == 'R') {
                desk[i][j] = 2;  // ладья
            }
        }
    }
    int n = 0, m = 0, f = 0;
    for (int i = 1; i < 9; ++i) {
        for (int j = 1; j < 9; ++j) {
            if (desk[i][j] == 2) {
                n = i;
                m = j;
                f = 1;
                while (f) {
                    --n;
                    if (desk[n][m] != -1 && desk[n][m] != 1 && desk[n][m] != 2) {
                        desk[n][m] = 5;
                    } else {
                        f = 0;
                    }
                }
                n = i;
                m = j;
                f = 1;
                while (f) {
                    ++n;
                    if (desk[n][m] != -1 && desk[n][m] != 1 && desk[n][m] != 2) {
                        desk[n][m] = 5;
                    } else {
                        f = 0;
                    }
                }
                n = i;
                m = j;
                f = 1;
                while (f) {
                    ++m;
                    if (desk[n][m] != -1 && desk[n][m] != 1 && desk[n][m] != 2) {
                        desk[n][m] = 5;
                    } else {
                        f = 0;
                    }
                }
                n = i;
                m = j;
                f = 1;
                while (f) {
                    --m;
                    if (desk[n][m] != -1 && desk[n][m] != 1 && desk[n][m] != 2) {
                        desk[n][m] = 5;
                    } else {
                        f = 0;
                    }
                }
            }
            if (desk[i][j] == 1) {
                n = i;
                m = j;
                f = 1;
                while (f) {
                    --n;
                    --m;
                    if (desk[n][m] != -1 && desk[n][m] != 1 && desk[n][m] != 2) {
                        desk[n][m] = 5;
                    } else {
                        f = 0;
                    }
                }
                n = i;
                m = j;
                f = 1;
                while (f) {
                    ++n;
                    --m;
                    if (desk[n][m] != -1 && desk[n][m] != 1 && desk[n][m] != 2) {
                        desk[n][m] = 5;
                    } else {
                        f = 0;
                    }
                }
                n = i;
                m = j;
                f = 1;
                while (f) {
                    ++m;
                    --n;
                    if (desk[n][m] != -1 && desk[n][m] != 1 && desk[n][m] != 2) {
                        desk[n][m] = 5;
                    } else {
                        f = 0;
                    }
                }
                n = i;
                m = j;
                f = 1;
                while (f) {
                    ++m;
                    ++n;
                    if (desk[n][m] != -1 && desk[n][m] != 1 && desk[n][m] != 2) {
                        desk[n][m] = 5;
                    } else {
                        f = 0;
                    }
                }
            }
        }
    }
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            if (desk[i][j] == 0) {
                ++count;
            }
        }
    }
    printf("%d\n", count);
}
