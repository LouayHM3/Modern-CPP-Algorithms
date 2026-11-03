#include "modern_cpp_algorithms/algorithms.hpp"

#include <cassert>
#include <string>
#include <vector>

namespace {

void test_binary_search() {
  const std::vector<int> values{1, 3, 5, 7, 9};
  assert(modern_cpp_algorithms::binary_search(values.begin(), values.end(), 1));
  assert(modern_cpp_algorithms::binary_search(values.begin(), values.end(), 9));
  assert(!modern_cpp_algorithms::binary_search(values.begin(), values.end(), 4));
  assert(!modern_cpp_algorithms::binary_search(values.begin(), values.begin(), 1));
}

void test_merge_sort() {
  std::vector<int> values{4, 1, 3, 1, 2};
  modern_cpp_algorithms::merge_sort(values.begin(), values.end());
  assert((values == std::vector<int>{1, 1, 2, 3, 4}));

  std::vector<std::string> words{"pear", "apple", "orange"};
  modern_cpp_algorithms::merge_sort(words.begin(), words.end(),
                                    [](const auto& left, const auto& right) {
                                      return left.size() < right.size();
                                    });
  assert((words == std::vector<std::string>{"pear", "apple", "orange"}));
}

void test_quick_sort() {
  std::vector<int> values{5, 2, 8, 2, 0, -1, 9};
  modern_cpp_algorithms::quick_sort(values.begin(), values.end());
  assert((values == std::vector<int>{-1, 0, 2, 2, 5, 8, 9}));
}

}  // namespace

int main() {
  test_binary_search();
  test_merge_sort();
  test_quick_sort();
}
