// #include <chrono>
#include <ctime>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <utility>
#include <vector>
#define NMAX 1000000
using namespace std;
// auto start = chrono::steady_clock::now();
void q_sort(std::vector<long>& arr, int index1, int index2) {
    if (index1 > index2) {
        return;
    }
    long p = arr[(rand() % (index2 - index1 + 1)) + index1];
    // arr[index1 + (index2 - index1) / 2]
    int equal = index1;
    int now = index1;
    int grater = index1;

    while (now <= index2) {
        long tmp = 0;
        if (arr[now] < p) {
            tmp = arr[now];
            arr[now] = arr[grater];
            arr[grater] = arr[equal];
            arr[equal] = tmp;
            ++now;
            ++equal;
            ++grater;
        } else if (arr[now] == p) {
            tmp = arr[now];
            arr[now] = arr[grater];
            arr[grater] = tmp;
            ++now;
            ++grater;

        } else {
            ++now;
        }
    }
    if (index1 <= equal - 1) {
        q_sort(arr, index1, equal - 1);
    }
    if (grater < index2) {
        q_sort(arr, grater, index2);
    }
}
int main() {
    srand(time(NULL));
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, l, r;
    std::vector<long> arr(NMAX);
    ifstream file("input.txt");
    if (file.is_open()) {
        string line;

        getline(file, line);
        n = atoi(line.c_str());
        arr.resize(n);
        while (getline(file, line)) {
            int i = 0;
            istringstream iss(line);
            string token;
            while (getline(iss, token, ' ')) {
                arr[i] = atoi(token.c_str());
                ++i;
            }
        }
        l = 0;
        r = n - 1;
        q_sort(arr, l, r);

    } else {
        cout << "Не удалось открыть файл." << endl;
    }
    ofstream out("output.txt");
    if (out.is_open()) {
        for (int i = 0; i < n; ++i) {
            out << arr[i] << " ";
        }
    }
    file.close();
    out.close();
    // cout << "Done"
    //      << "\n";
    // auto end = chrono::steady_clock::now();
    // auto diff = end - start;
    // cout << chrono::duration<double, milli>(diff).count() << " ms" << endl;
}