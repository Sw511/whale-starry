#include "include/list.h"
#include <cstring>
#include <iostream>

using namespace std;
class String {
  friend ostream& operator<<(ostream& out, const String& s);
  friend istream& operator>>(istream& in, String& s);

 public:
  String(const char* str = "") {
    m_data = new char[strlen(str) + 1];
    strcpy(m_data, str);
  }
  String(const String& s) {
    m_data = new char[strlen(s.m_data) + 1];
    strcpy(m_data, s.m_data);
  }
  ~String() {
    delete[] m_data;
    m_data = NULL;
  }
  friend istream& operator>>(istream& in, String& s) {
    in >> s.m_data;
    return in;
  }
  friend ostream& operator<<(ostream& out, const String& s) {
    out << s.m_data;
    return out;
  }

 private:
  char* m_data;
};

int main() {
  list<String> mylist;
  String s("abc");
  String s1("abc1");
  String s2("abc2");
  String s3("abc3");
  // mylist.insert(mylist.begin(), s);
  mylist.push_back(s);
  mylist.push_back(s1);
  mylist.push_back(s2);
  mylist.push_back(s3);
  mylist.show();
  mylist.pop_front();
  mylist.show();
  cout << mylist.back() << endl;
  mylist.pop_back();
  mylist.show();
  // list<String> mylist;
  // String s("abc");
  // mylist.insert(mylist.begin(), s);
  // mylist.show_iterator();
  // mylist.erase(mylist.begin());
  // int a[5] = {1, 2, 3, 4, 5};
  // list<int> mylist;
  // for (int i = 0; i < 5; i++) {
  //   mylist.push_back(a[i]);
  // }

  // mylist.show();
  // mylist.pop_back();
  // mylist.show();
  // // cout << mylist.begin()._Mynode()->_Value << endl;
  // // cout << mylist.end()._Mynode()->_Value << endl;
  // // mylist.show_iterator();
  // 未曾使用适配器
  // #include <iostream>

  // using namespace std;

  // int main() {
  //   int a[5] = {1, 2, 3, 4, 5};
  //   list<int> mylist;

  //   // test push_front
  //   for (int i = 0; i < 5; i++) {
  //     mylist.push_front(a[i]);
  //   }
  //   mylist.show();
  //   for (int i = 6; i < 10; i++) {
  //     mylist.push_back(i);
  //   }
  //   mylist.show();
  //   mylist.show_iterator();
  //   // cout << mylist.front() << endl;
  //   // cout << mylist.back() << endl;
}