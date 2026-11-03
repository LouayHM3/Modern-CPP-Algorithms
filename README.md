# Modern CPP Algorithms

Personal implementations of classic algorithms using C++17, with focused unit
tests and explicit complexity analysis.

## Included algorithms

- Binary search for sorted forward ranges.
- Merge sort with a comparator.
- In-place quick sort with a comparator.

See [docs/complexity.md](docs/complexity.md) for time and space analysis.

## Build and test

Requirements: CMake 3.16+ and a C++17 compiler.

```powershell
cmake -S . -B build
cmake --build build --config Release
ctest --test-dir build -C Release --output-on-failure
```

The library is header-only and can be consumed through the
`modern_cpp_algorithms` CMake target.

## Repository layout

```text
include/modern_cpp_algorithms/  Header-only implementations
tests/                          Unit tests
docs/                           Complexity analysis
```

## License

Personal educational work. Add a license before publishing if reuse terms are
important.
