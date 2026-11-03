# Modern CPP Algorithms

Personal implementations of classic algorithms using C++20, with focused unit
tests and explicit complexity analysis.

## Included algorithms

- Binary search for sorted forward ranges.
- Merge sort with a comparator.
- In-place quick sort with a comparator.
- AVL tree with insertion, removal, lookup, and in-order traversal.
- Separate-chaining hash table.
- Comparator-based binary heap.
- Graph traversal (BFS and DFS), Dijkstra shortest paths, and topological sort.

See [docs/complexity.md](docs/complexity.md) for time and space analysis.

## Build and test

Requirements: CMake 3.16+ and a C++20 compiler.

```powershell
cmake -S . -B build
cmake --build build --config Release
ctest --test-dir build -C Release --output-on-failure
```

The library is header-only and can be consumed through the
`modern_cpp_algorithms` CMake target.

## Data structure and graph headers

```cpp
#include "modern_cpp_algorithms/data_structures.hpp"
#include "modern_cpp_algorithms/graphs.hpp"
```

`dijkstra` expects non-negative edge weights. `topological_sort` accepts the
graph and the set of vertices to order and throws `std::logic_error` when a
cycle is found.

The sorting and searching algorithms provide constrained iterator overloads and
C++20 range overloads. The AVL tree owns its nodes with `std::unique_ptr`, so
tree ownership is explicit and automatic.

## Repository layout

```text
include/modern_cpp_algorithms/  Header-only implementations
tests/                          Unit tests
docs/                           Complexity analysis
```

## License

Personal educational work. Add a license before publishing if reuse terms are
important.
