#include "modern_cpp_algorithms/algorithms.hpp"
#include "modern_cpp_algorithms/data_structures.hpp"
#include "modern_cpp_algorithms/graphs.hpp"

#include <cassert>
#include <stdexcept>
#include <string>
#include <vector>
#include <array>

namespace {

void test_binary_search() {
  const std::vector<int> values{1, 3, 5, 7, 9};
  assert(modern_cpp_algorithms::binary_search(values.begin(), values.end(), 1));
  assert(modern_cpp_algorithms::binary_search(values.begin(), values.end(), 9));
  assert(!modern_cpp_algorithms::binary_search(values.begin(), values.end(), 4));
  assert(!modern_cpp_algorithms::binary_search(values.begin(), values.begin(), 1));
  assert(modern_cpp_algorithms::binary_search(values, 7));
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

  std::array<int, 4> range_values{4, 1, 3, 2};
  modern_cpp_algorithms::quick_sort(range_values);
  assert((range_values == std::array<int, 4>{1, 2, 3, 4}));
}

void test_avl_tree() {
  modern_cpp_algorithms::avl_tree<int> values;
  assert(values.insert(4));
  assert(values.insert(2));
  assert(values.insert(6));
  assert(values.insert(1));
  assert(values.insert(3));
  assert(!values.insert(2));
  assert(values.contains(3));
  assert(values.erase(2));
  assert(!values.contains(2));
  assert((values.inorder() == std::vector<int>{1, 3, 4, 6}));
  assert(values.size() == 4);
}

void test_hash_table() {
  modern_cpp_algorithms::hash_table<std::string, int> values(2);
  assert(values.insert("one", 1));
  assert(!values.insert("one", 2));
  values["two"] = 2;
  assert(values.find("one") != nullptr && *values.find("one") == 1);
  assert(values.contains("two"));
  assert(values.erase("one"));
  assert(!values.contains("one"));
  assert(values.size() == 1);
}

void test_heap() {
  modern_cpp_algorithms::binary_heap<int> max_heap;
  for (const int value : {3, 1, 5, 2}) {
    max_heap.push(value);
  }
  assert(max_heap.top() == 5);
  max_heap.pop();
  assert(max_heap.top() == 3);

  modern_cpp_algorithms::binary_heap<int, std::greater<int>> min_heap;
  for (const int value : {3, 1, 5, 2}) {
    min_heap.push(value);
  }
  assert(min_heap.top() == 1);
}

void test_graph_algorithms() {
  modern_cpp_algorithms::graph<int> graph;
  graph.add_edge(1, 2);
  graph.add_edge(1, 3);
  graph.add_edge(2, 4);
  graph.add_edge(3, 4);
  assert((modern_cpp_algorithms::breadth_first_search(graph, 1) ==
          std::vector<int>{1, 2, 3, 4}));
  assert((modern_cpp_algorithms::depth_first_search(graph, 1) ==
          std::vector<int>{1, 2, 4, 3}));

  modern_cpp_algorithms::graph<std::string, int> weighted;
  weighted.add_edge("A", "B", 4);
  weighted.add_edge("A", "C", 1);
  weighted.add_edge("C", "B", 2);
  const auto distances =
      modern_cpp_algorithms::dijkstra(weighted, std::string("A"));
  assert(distances.at("B") == 3);

  const std::vector<int> vertices{1, 2, 3, 4};
  const auto sorted = modern_cpp_algorithms::topological_sort(graph, vertices);
  assert(sorted.front() == 1);
  assert(sorted.back() == 4);

  graph.add_edge(4, 1);
  bool cycle_detected = false;
  try {
    (void)modern_cpp_algorithms::topological_sort(graph, vertices);
  } catch (const std::logic_error&) {
    cycle_detected = true;
  }
  assert(cycle_detected);
}

}  // namespace

int main() {
  test_binary_search();
  test_merge_sort();
  test_quick_sort();
  test_avl_tree();
  test_hash_table();
  test_heap();
  test_graph_algorithms();
}
