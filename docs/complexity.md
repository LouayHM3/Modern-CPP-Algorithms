# Complexity analysis

| Algorithm | Best | Average | Worst | Extra space | Notes |
| --- | ---: | ---: | ---: | ---: | --- |
| `binary_search` | O(1) | O(log n) | O(log n) | O(1) | Requires a sorted range |
| `merge_sort` | O(n log n) | O(n log n) | O(n log n) | O(n) | Stable when the comparator treats equivalent values consistently |
| `quick_sort` | O(n log n) | O(n log n) | O(n²) | O(log n) average | In-place partitioning; midpoint pivot |
| `avl_tree` lookup/insert/erase | O(log n) | O(log n) | O(log n) | O(n) | Self-balancing binary search tree |
| `hash_table` lookup/insert/erase | O(1) | O(1) | O(n) | O(n) | Separate chaining |
| `binary_heap` top/push/pop | O(1)/O(log n)/O(log n) | Same | Same | O(n) | Comparator determines the heap root |
| BFS / DFS | O(V + E) | O(V + E) | O(V + E) | O(V) | Traverses reachable vertices |
| Dijkstra | O((V + E) log V) | O((V + E) log V) | O((V + E) log V) | O(V) | Requires non-negative edge weights |
| Topological sort | O(V + E) | O(V + E) | O(V + E) | O(V) | Throws when the graph contains a cycle |

The analysis describes asymptotic behavior for the implementations in
`include/modern_cpp_algorithms/algorithms.hpp`. It does not replace benchmarks:
constant factors, cache behavior, iterator categories, and input distributions
can materially affect real workloads.
