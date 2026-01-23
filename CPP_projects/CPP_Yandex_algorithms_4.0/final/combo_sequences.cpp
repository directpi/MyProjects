#include <iostream>
#include <vector>
using namespace std;
int main() {
  long n;
  cin >> n;
  vector<size_t> a(n + 1);
  vector<size_t> b(n + 1);
  vector<size_t> c;
  size_t i = 1, j = 1;

  for (size_t k = 0; k <= n; ++k) {
    a[k] = k * k;
    b[k] = k * k * k;
  }
  size_t count = 1;
  size_t d = 1;
  c.push_back(1);
  while (count < n) {
    if (i * i <= j * j * j) {
      if (!c.empty() && i * i != c[count - 1]) {
        c.push_back(i * i);
        ++count;
      }
      ++i;
    } else {
      size_t jd = j * d;
      bool ajd_eq_bj = (jd <= static_cast<size_t>(n)) && (a[jd] == b[j]);
      bool ajd_ne_bj = !ajd_eq_bj;
      if (!c.empty() && j * j * j != c[count - 1] && ajd_ne_bj) {
        c.push_back(j * j * j);
        ++count;
      }
      if (ajd_eq_bj) ++d;
      ++j;
    }
  }
  size_t tmp = c.back();
  cout << tmp;
}