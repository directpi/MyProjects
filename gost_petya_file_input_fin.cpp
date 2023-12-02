#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
using namespace std;
int main() {
    string str1, str2;
    int x_ = 257;  // 10
    long p = pow(10, 9) + 7;
    string tmp;
    ifstream file("input.txt");
    if (file.is_open()) {
        string line;
        int n;
        getline(file, line);
        n = atoi(line.c_str());
        vector<int> res(n);
        vector<long long int> h1;
        vector<long long int> h2(n + 1);
        vector<long long int> x;
        h1.push_back(0);
        x.push_back(1);
        getline(file, line);
        int i = 0;
        istringstream iss(line);
        string token;
        while (getline(iss, token, ' ')) {
            res[i] = atoi(token.c_str());
            ++i;
        }
        for (int i = 1; i < n + 1; i++) {
            h1.push_back((h1[i - 1] * x_ + res[i - 1]) % p);
            h2[i] = ((h2[i - 1] * x_ + res[n - i]) % p);
            x.push_back((x[i - 1] * x_) % p);
        }
        int len = res.size();
        long long int target_h, c_p = 0;
        for (int i = n / 2; i > 0; --i) {
            target_h = h1[i];
            c_p = (h2[len - i] - (h2[len - (i + i)] * x[(len - i) - (len - (i + i))])) % p;
            if (c_p == target_h) {
                cout << n - i << " ";
            }
            c_p = p + (h2[len - i] - (h2[len - (i + i)] * x[(len - i) - (len - (i + i))])) % p;
            if (c_p == target_h) {
                cout << n - i << " ";
            }
        }
        cout << n << "\n";
    } else {
        cout << "Не удалось открыть файл." << endl;
    }
    file.close();
}