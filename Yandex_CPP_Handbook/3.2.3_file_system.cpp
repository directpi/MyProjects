#include <iostream>
#include <set>
#include <string>

using namespace std;

int main() {
  set<string> not_empty_dirs;
  string path_name;
  while (getline(cin, path_name)) {
    for (size_t i = 0; i != path_name.size(); ++i) {
      if (path_name[i] == '/') {
        not_empty_dirs.insert(path_name.substr(0, i + 1));
      }
    }
  }
  for (auto my_dir : not_empty_dirs) {
    cout << my_dir << "\n";
  }
}