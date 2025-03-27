#include <iostream>
using namespace std;
void d_swap(int* arr, int i, int j) {
    int s = arr[i];
    arr[i] = arr[j];
    arr[j] = s;
}
bool permutation(int* arr, int n) {
    int j = n - 2;
    while (j != -1 && arr[j] >= arr[j + 1]) {
        j--;
    }
    if (j == -1) {
        return false;
    }
    int k = n - 1;
    while (arr[j] >= arr[k]) {
        k--;
    }
    d_swap(arr, j, k);
    int l = j + 1, r = n - 1;
    while (l < r) {
        d_swap(arr, l++, r--);
    }
    return true;
}
void out_print(int* arr, int n) {
    for (int i = 0; i < n; i++) {
        cout << arr[i];
    }
    cout << "\n";
}
int main() {
    int n;
    cin >> n;
    int arr[n];
    for (int i = 0; i < n; i++) arr[i] = i + 1;
    out_print(arr, n);
    while (permutation(arr, n)) {
        out_print(arr, n);
    }
}