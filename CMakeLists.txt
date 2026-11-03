cmake_minimum_required(VERSION 3.16)
project(ModernCPPAlgorithms VERSION 0.1.0 LANGUAGES CXX)

include(CTest)

add_library(modern_cpp_algorithms INTERFACE)
target_include_directories(modern_cpp_algorithms INTERFACE include)
target_compile_features(modern_cpp_algorithms INTERFACE cxx_std_20)

if(BUILD_TESTING)
  add_executable(modern_cpp_algorithms_tests tests/test_algorithms.cpp)
  target_link_libraries(modern_cpp_algorithms_tests PRIVATE modern_cpp_algorithms)
  if(MSVC)
    target_compile_options(modern_cpp_algorithms_tests PRIVATE /W4 /permissive-)
  else()
    target_compile_options(modern_cpp_algorithms_tests PRIVATE
      -Wall -Wextra -Wpedantic -Wconversion)
  endif()
  add_test(NAME modern_cpp_algorithms_tests COMMAND modern_cpp_algorithms_tests)
endif()
