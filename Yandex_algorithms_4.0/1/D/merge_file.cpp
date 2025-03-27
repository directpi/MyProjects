#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
using namespace std;
void merge(vector<int>& ar1, vector<int>& res, size_t index1, size_t index2) {
    size_t left = index1;
    size_t mid = index1 + (index2 - index1) / 2;
    size_t right = mid + 1;
    size_t count = 0;
    while (left <= mid && right <= index2) {
        if (ar1[left] <= ar1[right]) {
            res[count] = ar1[left];
            ++left;
        } else {
            res[count] = ar1[right];
            ++right;
        }
        ++count;
    }
    while (left <= mid) {
        res[count] = ar1[left];
        ++left;
        ++count;
    }
    while (right <= index2) {
        res[count] = ar1[right];
        ++right;
        ++count;
    }
    for (size_t i = 0; i < count; ++i) {
        ar1[index1 + i] = res[i];
    }
}

void merge_sort(vector<int>& array, vector<int>& res, int index1, int index2) {
    if (index1 < index2) {
        if (index2 - index1 == 1) {
            if (array[index1] > array[index2]) {
                int tmp = 0;
                tmp = array[index1];
                array[index1] = array[index2];
                array[index2] = tmp;
            }
        } else {
            merge_sort(array, res, index1, index1 + (index2 - index1) / 2);
            merge_sort(array, res, index1 + (index2 - index1) / 2 + 1, index2);
            merge(array, res, index1, index2);
        }
    }
}
int main() {
    const size_t d = 1000000;
    int n;

    vector<int> ar1(d);

    vector<int> res(ar1.size());
    ifstream file("input.txt");
    if (file.is_open()) {
        string line;

        getline(file, line);
        n = atoi(line.c_str());
        ar1.resize(n);
        res.resize(n);
        while (getline(file, line)) {
            int i = 0;
            istringstream iss(line);
            string token;
            while (getline(iss, token, ' ')) {
                ar1[i] = atoi(token.c_str());
                ++i;
            }
        }
        merge_sort(ar1, res, 0, n - 1);
    } else {
        cout << "Не удалось открыть файл." << endl;
    }
    ofstream out("output.txt");
    if (out.is_open()) {
        for (int i = 0; i < n; ++i) {
            out << ar1[i] << " ";
        }
    }
    file.close();
    out.close();
}