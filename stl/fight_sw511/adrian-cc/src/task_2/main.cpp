#include <chrono>
#include <iostream>
#include <vector>

constexpr int TEST_SIZE = 10000;  // 每轮测试元素个数

struct TestObject {
  int a;
  std::string b;

  TestObject() : a(0), b("") {}
  TestObject(int a_, const std::string& b_) : a(a_), b(b_) {}
};

int main() {
  // 测试 push_back
  {
    std::vector<TestObject> vec;
    auto start_time = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < TEST_SIZE; ++i) {
      vec.push_back(TestObject(i, "test"));
    }
    auto end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end_time - start_time;
    std::cout << "push_back 时间: " << elapsed.count() << "s\n";
  }

  // 测试 emplace_back
  {
    std::vector<TestObject> vec;
    auto start_time = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < TEST_SIZE; ++i) {
      vec.emplace_back(i, "test");
    }
    auto end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end_time - start_time;
    std::cout << "emplace_back 时间: " << elapsed.count() << "s\n";
  }

  // 测试 insert at end
  {
    std::vector<TestObject> vec;
    auto start_time = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < TEST_SIZE; ++i) {
      vec.insert(vec.end(), TestObject(i, "test"));
    }
    auto end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end_time - start_time;
    std::cout << "insert at end 时间: " << elapsed.count() << "s\n";
  }

  // 测试 insert at beginning
  {
    std::vector<TestObject> vec;
    auto start_time = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < TEST_SIZE; ++i) {
      vec.insert(vec.begin(), TestObject(i, "test"));
    }
    auto end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end_time - start_time;
    std::cout << "insert at beginning 时间: " << elapsed.count() << "s\n";
  }

  return 0;
}
