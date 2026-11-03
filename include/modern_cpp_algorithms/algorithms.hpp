#pragma once

#include <algorithm>
#include <functional>
#include <iterator>
#include <ranges>
#include <type_traits>
#include <utility>
#include <vector>

namespace modern_cpp_algorithms {

template <typename It, typename T, typename Compare = std::less<>>
concept binary_searchable =
    std::forward_iterator<It> &&
    std::indirect_strict_weak_order<Compare, It, const T*>;

template <typename It, typename Compare = std::less<>>
concept sortable_random_access =
    std::random_access_iterator<It> &&
    std::indirect_strict_weak_order<Compare, It>;

template <std::forward_iterator ForwardIt, typename T,
          typename Compare = std::less<>>
  requires binary_searchable<ForwardIt, T, Compare>
constexpr bool binary_search(ForwardIt first, ForwardIt last, const T& value,
                             Compare compare = {}) {
  while (first != last) {
    const auto middle = std::next(first, std::distance(first, last) / 2);
    if (compare(*middle, value)) {
      first = std::next(middle);
    } else if (compare(value, *middle)) {
      last = middle;
    } else {
      return true;
    }
  }
  return false;
}

template <typename Range, typename T, typename Compare = std::less<>>
  requires std::ranges::range<Range> &&
           std::forward_iterator<std::ranges::iterator_t<Range>> &&
           binary_searchable<std::ranges::iterator_t<Range>, T, Compare>
constexpr bool binary_search(Range&& range, const T& value,
                             Compare compare = {}) {
  return binary_search(std::ranges::begin(range), std::ranges::end(range),
                       value, std::move(compare));
}

template <std::random_access_iterator RandomIt,
          typename Compare = std::less<>>
  requires sortable_random_access<RandomIt, Compare>
void merge_sort(RandomIt first, RandomIt last, Compare compare = {}) {
  const auto size = std::distance(first, last);
  if (size < 2) {
    return;
  }

  const auto middle = std::next(first, size / 2);
  merge_sort(first, middle, compare);
  merge_sort(middle, last, compare);

  using value_type = typename std::iterator_traits<RandomIt>::value_type;
  std::vector<value_type> merged;
  merged.reserve(static_cast<std::size_t>(size));
  std::merge(first, middle, middle, last, std::back_inserter(merged), compare);
  std::move(merged.begin(), merged.end(), first);
}

template <std::ranges::random_access_range Range,
          typename Compare = std::less<>>
  requires sortable_random_access<std::ranges::iterator_t<Range>, Compare>
void merge_sort(Range&& range, Compare compare = {}) {
  merge_sort(std::ranges::begin(range), std::ranges::end(range),
             std::move(compare));
}

template <std::random_access_iterator RandomIt,
          typename Compare = std::less<>>
  requires sortable_random_access<RandomIt, Compare>
void quick_sort(RandomIt first, RandomIt last, Compare compare = {}) {
  if (last - first < 2) {
    return;
  }

  const auto pivot = *(first + (last - first) / 2);
  auto lower = first;
  auto upper = last - 1;
  while (lower <= upper) {
    while (compare(*lower, pivot)) {
      ++lower;
    }
    while (compare(pivot, *upper)) {
      --upper;
    }
    if (lower <= upper) {
      std::iter_swap(lower, upper);
      ++lower;
      if (upper != first) {
        --upper;
      } else {
        break;
      }
    }
  }

  if (first < upper + 1) {
    quick_sort(first, upper + 1, compare);
  }
  quick_sort(lower, last, compare);
}

template <std::ranges::random_access_range Range,
          typename Compare = std::less<>>
  requires sortable_random_access<std::ranges::iterator_t<Range>, Compare>
void quick_sort(Range&& range, Compare compare = {}) {
  quick_sort(std::ranges::begin(range), std::ranges::end(range),
             std::move(compare));
}

}  // namespace modern_cpp_algorithms
