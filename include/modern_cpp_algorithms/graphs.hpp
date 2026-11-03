#pragma once

#include <algorithm>
#include <limits>
#include <queue>
#include <stdexcept>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

namespace modern_cpp_algorithms {

template <typename Vertex, typename Weight = int>
class graph {
 public:
  struct edge {
    Vertex destination;
    Weight weight;
  };

 private:
  std::unordered_map<Vertex, std::vector<edge>> adjacency_;

 public:
  void add_vertex(const Vertex& vertex) { adjacency_.try_emplace(vertex); }

  void add_edge(const Vertex& source, const Vertex& destination,
                Weight weight = Weight{1}) {
    adjacency_[source].push_back({destination, weight});
    adjacency_.try_emplace(destination);
  }

  const std::vector<edge>& neighbors(const Vertex& vertex) const {
    const auto found = adjacency_.find(vertex);
    if (found == adjacency_.end()) {
      throw std::out_of_range("neighbors() called for an unknown vertex");
    }
    return found->second;
  }

  bool contains(const Vertex& vertex) const {
    return adjacency_.find(vertex) != adjacency_.end();
  }
};

template <typename Vertex, typename Weight>
std::vector<Vertex> breadth_first_search(const graph<Vertex, Weight>& graph,
                                          const Vertex& start) {
  if (!graph.contains(start)) {
    return {};
  }
  std::vector<Vertex> order;
  std::queue<Vertex> pending;
  std::unordered_set<Vertex> visited{start};
  pending.push(start);
  while (!pending.empty()) {
    const Vertex current = pending.front();
    pending.pop();
    order.push_back(current);
    for (const auto& edge : graph.neighbors(current)) {
      if (visited.insert(edge.destination).second) {
        pending.push(edge.destination);
      }
    }
  }
  return order;
}

template <typename Vertex, typename Weight>
std::vector<Vertex> depth_first_search(const graph<Vertex, Weight>& graph,
                                        const Vertex& start) {
  if (!graph.contains(start)) {
    return {};
  }
  std::vector<Vertex> order;
  std::vector<Vertex> pending{start};
  std::unordered_set<Vertex> visited;
  while (!pending.empty()) {
    const Vertex current = pending.back();
    pending.pop_back();
    if (!visited.insert(current).second) {
      continue;
    }
    order.push_back(current);
    const auto& edges = graph.neighbors(current);
    for (auto edge = edges.rbegin(); edge != edges.rend(); ++edge) {
      if (visited.find(edge->destination) == visited.end()) {
        pending.push_back(edge->destination);
      }
    }
  }
  return order;
}

template <typename Vertex, typename Weight>
std::unordered_map<Vertex, Weight> dijkstra(
    const graph<Vertex, Weight>& graph, const Vertex& start) {
  if (!graph.contains(start)) {
    return {};
  }
  using queue_item = std::pair<Weight, Vertex>;
  std::priority_queue<queue_item, std::vector<queue_item>,
                      std::greater<queue_item>>
      pending;
  std::unordered_map<Vertex, Weight> distance;
  distance[start] = Weight{};
  pending.push({Weight{}, start});
  while (!pending.empty()) {
    const auto [current_distance, current] = pending.top();
    pending.pop();
    if (current_distance != distance[current]) {
      continue;
    }
    for (const auto& edge : graph.neighbors(current)) {
      const Weight candidate = current_distance + edge.weight;
      const auto found = distance.find(edge.destination);
      if (found == distance.end() || candidate < found->second) {
        distance[edge.destination] = candidate;
        pending.push({candidate, edge.destination});
      }
    }
  }
  return distance;
}

template <typename Vertex, typename Weight>
std::vector<Vertex> topological_sort(const graph<Vertex, Weight>& graph,
                                      const std::vector<Vertex>& vertices) {
  std::unordered_map<Vertex, std::size_t> indegree;
  for (const auto& vertex : vertices) {
    indegree[vertex] = 0;
  }
  for (const auto& vertex : vertices) {
    for (const auto& edge : graph.neighbors(vertex)) {
      if (indegree.find(edge.destination) != indegree.end()) {
        ++indegree[edge.destination];
      }
    }
  }
  std::queue<Vertex> ready;
  for (const auto& [vertex, degree] : indegree) {
    if (degree == 0) {
      ready.push(vertex);
    }
  }
  std::vector<Vertex> order;
  while (!ready.empty()) {
    const Vertex current = ready.front();
    ready.pop();
    order.push_back(current);
    for (const auto& edge : graph.neighbors(current)) {
      auto found = indegree.find(edge.destination);
      if (found != indegree.end() && --found->second == 0) {
        ready.push(edge.destination);
      }
    }
  }
  if (order.size() != vertices.size()) {
    throw std::logic_error("topological_sort() requires an acyclic graph");
  }
  return order;
}

}  // namespace modern_cpp_algorithms
