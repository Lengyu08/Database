#pragma once

#include <mutex>
#include <utility>
#include <optional>
#include <shared_mutex>

#include "./trie.hpp"

// 这个类用于保护 trie 返回的值, 它持有对根的引用, 以确保对值的引用在并发操作中不会失效
template <class T>
class ValueGuard {
private:
    Trie trie;
    const T &value;

public:
    ValueGuard(Trie input_trie, const T &input_value) :
        trie(std::move(input_trie)), value(input_value) {
    }
    // 重载 * 运算符，允许通过 ValueGuard 对象解引用获取持有的值的引用
    // 此函数返回对 ValueGuard 对象中值的常量引用，以便在使用 * 运算符时能够方便地获取值
    auto operator*() const -> const T & {
        return value;
    }
};

class TrieStore {
private:
    Trie trie;

    mutable std::shared_mutex lock;
public:
    // 此函数返回一个 ValueGuard 对象，该对象持有对 trie 中值的引用；如果键在 trie 中不存在，则返回 std::nullopt
    template <class T>
    auto Get(std::string_view key) -> std::optional<ValueGuard<T>> {
        auto value_ptr = trie.Get<T>(key);

        if (value_ptr == nullptr) {
            return std::nullopt;
        } else {
            return ValueGuard<T>(trie, *value_ptr);
        }
    }

    // 此函数将键值对插入到 trie 中；如果键已经存在于 trie 中，则将覆盖该值
    template <class T>
    void Put(std::string_view key, T value) {
        std::unique_lock<std::shared_mutex> lock(this->lock);

        trie.Put<T>(key, std::move(value));
    }

    // 此函数将从 trie 中删除键值对
    void Remove(std::string_view key) {
        std::unique_lock<std::shared_mutex> lock(this->lock);

        trie = trie.Remove(key);
    }
};
