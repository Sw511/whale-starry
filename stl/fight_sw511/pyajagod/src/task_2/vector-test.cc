#include "include/vector.h"
#include <iostream>

using namespace std;

int main() {
  vector<int> v;
  // 测试空间不足的情况
  v.insert(v.end(), 2, 1);
}