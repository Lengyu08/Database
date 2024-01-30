#include <cassert>  // 用于断言
#include <iostream> // 用于输入和输出流

#include "../../../include/data_struct/Trie/trie.hpp"

template <class T>
auto TestTrie(const std::string &key, const T &value, const T &assert_value, std::shared_ptr<int> test_count) -> void {
    std::cout << "TestTrie: " << (*test_count)++ << std::endl;

    auto trie = Trie();

    trie = trie.Put(key, value);

    assert(trie.Get<T>(key) != nullptr && *trie.Get<T>(key) == assert_value);
}

int main(int argc, char **argv) {
    std::shared_ptr<int> test_count = std::make_shared<int>(0);

    std::string key;
    std::string string_value;
    std::uint32_t uint32_value;
    std::uint64_t uint64_value;

    key = "hello", string_value = "hello";
    TestTrie<std::string>(key, string_value, string_value, test_count);

    key = "", string_value = "empty";
    TestTrie<std::string>(key, string_value, string_value, test_count);

    return 0;
}