#pragma once

#include <algorithm>
#include <functional>
#include <initializer_list>
#include <list>
#include <memory>
#include <stdexcept>
#include <utility>
#include <vector>

namespace modern_cpp_algorithms {

template <typename T, typename Compare = std::less<T>>
class avl_tree {
  struct node {
    T value;
    std::unique_ptr<node> left;
    std::unique_ptr<node> right;
    int height = 1;

    explicit node(const T& value) : value(value) {}
    explicit node(T&& value) : value(std::move(value)) {}
  };

  std::unique_ptr<node> root_;
  std::size_t size_ = 0;
  Compare compare_;

  static int height(const std::unique_ptr<node>& current) {
    return current == nullptr ? 0 : current->height;
  }

  static void update_height(node& current) {
    current.height = 1 + std::max(height(current.left), height(current.right));
  }

  static int balance_factor(const node& current) {
    return height(current.left) - height(current.right);
  }

  static std::unique_ptr<node> rotate_right(std::unique_ptr<node> current) {
    auto pivot = std::move(current->left);
    current->left = std::move(pivot->right);
    update_height(*current);
    pivot->right = std::move(current);
    update_height(*pivot);
    return pivot;
  }

  static std::unique_ptr<node> rotate_left(std::unique_ptr<node> current) {
    auto pivot = std::move(current->right);
    current->right = std::move(pivot->left);
    update_height(*current);
    pivot->left = std::move(current);
    update_height(*pivot);
    return pivot;
  }

  static std::unique_ptr<node> rebalance(std::unique_ptr<node> current) {
    update_height(*current);
    const int balance = balance_factor(*current);
    if (balance > 1) {
      if (balance_factor(*current->left) < 0) {
        current->left = rotate_left(std::move(current->left));
      }
      return rotate_right(std::move(current));
    }
    if (balance < -1) {
      if (balance_factor(*current->right) > 0) {
        current->right = rotate_right(std::move(current->right));
      }
      return rotate_left(std::move(current));
    }
    return current;
  }

  std::unique_ptr<node> insert(std::unique_ptr<node> current, const T& value,
                               bool& inserted) {
    if (current == nullptr) {
      inserted = true;
      return std::make_unique<node>(value);
    }
    if (compare_(value, current->value)) {
      current->left = insert(std::move(current->left), value, inserted);
    } else if (compare_(current->value, value)) {
      current->right = insert(std::move(current->right), value, inserted);
    } else {
      return current;
    }
    return rebalance(std::move(current));
  }

  static std::unique_ptr<node> remove_min(std::unique_ptr<node> current,
                                          std::unique_ptr<node>& minimum) {
    if (current->left == nullptr) {
      minimum = std::move(current);
      return std::move(minimum->right);
    }
    current->left = remove_min(std::move(current->left), minimum);
    return rebalance(std::move(current));
  }

  std::unique_ptr<node> erase(std::unique_ptr<node> current, const T& value,
                              bool& erased) {
    if (current == nullptr) {
      return nullptr;
    }
    if (compare_(value, current->value)) {
      current->left = erase(std::move(current->left), value, erased);
    } else if (compare_(current->value, value)) {
      current->right = erase(std::move(current->right), value, erased);
    } else {
      erased = true;
      if (current->left == nullptr) {
        return std::move(current->right);
      }
      if (current->right == nullptr) {
        return std::move(current->left);
      }
      std::unique_ptr<node> successor;
      current->right = remove_min(std::move(current->right), successor);
      successor->left = std::move(current->left);
      successor->right = std::move(current->right);
      return rebalance(std::move(successor));
    }
    return rebalance(std::move(current));
  }

  static void append_inorder(const std::unique_ptr<node>& current,
                             std::vector<T>& values) {
    if (current == nullptr) {
      return;
    }
    append_inorder(current->left, values);
    values.push_back(current->value);
    append_inorder(current->right, values);
  }

 public:
  bool insert(const T& value) {
    bool inserted = false;
    root_ = insert(std::move(root_), value, inserted);
    if (inserted) {
      ++size_;
    }
    return inserted;
  }

  bool erase(const T& value) {
    bool erased = false;
    root_ = erase(std::move(root_), value, erased);
    if (erased) {
      --size_;
    }
    return erased;
  }

  bool contains(const T& value) const {
    const node* current = root_.get();
    while (current != nullptr) {
      if (compare_(value, current->value)) {
        current = current->left.get();
      } else if (compare_(current->value, value)) {
        current = current->right.get();
      } else {
        return true;
      }
    }
    return false;
  }

  std::size_t size() const { return size_; }
  bool empty() const { return size_ == 0; }

  std::vector<T> inorder() const {
    std::vector<T> values;
    values.reserve(size_);
    append_inorder(root_, values);
    return values;
  }
};

template <typename T, typename Compare = std::less<T>>
class binary_heap {
  std::vector<T> values_;
  Compare compare_;

 public:
  bool empty() const { return values_.empty(); }
  std::size_t size() const { return values_.size(); }
  const T& top() const {
    if (empty()) {
      throw std::out_of_range("top() called on an empty heap");
    }
    return values_.front();
  }

  void push(const T& value) {
    values_.push_back(value);
    std::push_heap(values_.begin(), values_.end(), compare_);
  }

  void push(T&& value) {
    values_.push_back(std::move(value));
    std::push_heap(values_.begin(), values_.end(), compare_);
  }

  void pop() {
    if (empty()) {
      throw std::out_of_range("pop() called on an empty heap");
    }
    std::pop_heap(values_.begin(), values_.end(), compare_);
    values_.pop_back();
  }
};

template <typename Key, typename Value, typename Hash = std::hash<Key>,
          typename KeyEqual = std::equal_to<Key>>
class hash_table {
  using entry = std::pair<const Key, Value>;
  std::vector<std::list<entry>> buckets_;
  std::size_t size_ = 0;
  Hash hash_;
  KeyEqual equal_;

  std::size_t bucket_for(const Key& key) const {
    return hash_(key) % buckets_.size();
  }

  void rehash(std::size_t bucket_count) {
    std::vector<std::list<entry>> replacement(bucket_count);
    for (auto& bucket : buckets_) {
      while (!bucket.empty()) {
        auto item = bucket.begin();
        const std::size_t destination = hash_(item->first) % bucket_count;
        replacement[destination].splice(replacement[destination].end(), bucket,
                                        item);
      }
    }
    buckets_ = std::move(replacement);
  }

  void grow_if_needed() {
    if (size_ + 1 > buckets_.size() * 2) {
      rehash(buckets_.size() * 2);
    }
  }

 public:
  explicit hash_table(std::size_t bucket_count = 16)
      : buckets_(bucket_count == 0 ? 1 : bucket_count) {}

  Value& operator[](const Key& key) {
    auto& bucket = buckets_[bucket_for(key)];
    for (auto& item : bucket) {
      if (equal_(item.first, key)) {
        return item.second;
      }
    }
    grow_if_needed();
    auto& target_bucket = buckets_[bucket_for(key)];
    target_bucket.emplace_back(key, Value{});
    ++size_;
    return target_bucket.back().second;
  }

  bool insert(const Key& key, const Value& value) {
    auto& bucket = buckets_[bucket_for(key)];
    for (const auto& item : bucket) {
      if (equal_(item.first, key)) {
        return false;
      }
    }
    grow_if_needed();
    buckets_[bucket_for(key)].emplace_back(key, value);
    ++size_;
    return true;
  }

  Value* find(const Key& key) {
    auto& bucket = buckets_[bucket_for(key)];
    for (auto& item : bucket) {
      if (equal_(item.first, key)) {
        return &item.second;
      }
    }
    return nullptr;
  }

  const Value* find(const Key& key) const {
    const auto& bucket = buckets_[bucket_for(key)];
    for (const auto& item : bucket) {
      if (equal_(item.first, key)) {
        return &item.second;
      }
    }
    return nullptr;
  }

  bool contains(const Key& key) const { return find(key) != nullptr; }

  bool erase(const Key& key) {
    auto& bucket = buckets_[bucket_for(key)];
    for (auto item = bucket.begin(); item != bucket.end(); ++item) {
      if (equal_(item->first, key)) {
        bucket.erase(item);
        --size_;
        return true;
      }
    }
    return false;
  }

  std::size_t size() const { return size_; }
  bool empty() const { return size_ == 0; }
};

}  // namespace modern_cpp_algorithms
