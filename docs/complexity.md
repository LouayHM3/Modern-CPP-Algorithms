# Complexity analysis

| Algorithm | Best | Average | Worst | Extra space | Notes |
| --- | ---: | ---: | ---: | ---: | --- |
| `binary_search` | O(1) | O(log n) | O(log n) | O(1) | Requires a sorted range |
| `merge_sort` | O(n log n) | O(n log n) | O(n log n) | O(n) | Stable when the comparator treats equivalent values consistently |
| `quick_sort` | O(n log n) | O(n log n) | O(n²) | O(log n) average | In-place partitioning; midpoint pivot |

The analysis describes asymptotic behavior for the implementations in
`include/modern_cpp_algorithms/algorithms.hpp`. It does not replace benchmarks:
constant factors, cache behavior, iterator categories, and input distributions
can materially affect real workloads.
