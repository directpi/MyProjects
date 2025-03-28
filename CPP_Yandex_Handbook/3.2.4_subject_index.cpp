#include<iostream>
#include<string>
#include<map>
#include<set>

using namespace std;

int main() {
  map <int , set <string>> subject_index;
  int n;
  cin >> n;
  for (int i=0; i<n; ++i){
    string word;//
    int page; 
    cin >> word >> page;
    subject_index[page].insert(word);
  }
 for (const auto& [p , ws] : subject_index) {
   cout << p;
   for (const auto& w : ws)  {
     cout << " " << w;
   }
   cout << "\n";
 }
}