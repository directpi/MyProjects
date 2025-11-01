#include <cmath>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
using namespace std;
int main() {
    string str;
    int q;
    size_t p = pow(10, 9) + 7;
    int x_ = 257;  // 10
    vector<size_t> h;
    vector<size_t> x;
    ifstream file("input.txt");
    ofstream out("output.txt");
    if (file.is_open()) {
        string line;
        getline(file, str);
        str = " " + str;
        int n = str.length();
        getline(file, line);
        q = atoi(line.c_str());
        vector<vector<int>> arr(q, vector<int>(3));
        int ii = 0;
        int j = 0;
        while (getline(file, line)) {
            // getline(file, line);
            istringstream iss(line);
            string token;
            while (getline(iss, token, ' ')) {
                arr[ii][j] = atoi(token.c_str());
                ++j;
            }
            ++ii;
            j = 0;
        }
        h.push_back(0);
        x.push_back(1);
        for (int i = 1; i < n + 1; i++) {
            h.push_back((h[i - 1] * x_ + str[i]) % p);
            x.push_back((x[i - 1] * x_) % p);
        }

        int from1 = 0, from2 = 0, len = 0;

        if (out.is_open()) {
            for (int k = 0; k < q; ++k) {
                if (arr[k][1] > arr[k][2]) {
                    from1 = arr[k][2];
                    from2 = arr[k][1];
                } else {
                    from2 = arr[k][2];
                    from1 = arr[k][1];
                }
                len = arr[k][0];
                if ((h[from1 + len] + h[from2] * x[len]) % p == (h[from2 + len] + h[from1] * x[len]) % p) {
                    out << "yes"
                        << "\n";
                } else {
                    out << "no"
                        << "\n";
                }
            }
        }
    } else {
        cout << "Не удалось открыть файл." << endl;
    }
    file.close();
    out.close();
}