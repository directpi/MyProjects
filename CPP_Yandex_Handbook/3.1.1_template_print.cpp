#include <array>
#include <deque>
#include <forward_list>
#include <iostream>
#include <list>
#include <string>
#include <vector>
template <typename T>
void Print(const T& data, const std::string& str) {
  size_t ii = 0;
  for (const auto& i : data) {
    std::cout << i;
    if (ii < data.size() - 1) {
      std::cout << str;
      ++ii;
    }
  }
  std::cout << "\n";
}
int main() {
  std::vector<int> data = {1, 2, 3, 4};
  Print(data, ", ");  // 1, 2, 3, 4
}