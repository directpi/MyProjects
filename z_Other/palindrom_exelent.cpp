#include <algorithm>
#include <iostream>
#include <string>

int main() {
  std::string s;
  std::getline(std::cin, s);

  std::erase(s, ' ');  // С++20
  // s.erase(std::remove(s.begin(), s.end(), ' '), s.end()); // С++17

  if (std::equal(s.begin(), s.begin() + s.size() / 2, s.rbegin())) {
    std::cout << "YES\n";
  } else {
    std::cout << "NO\n";
  }
}