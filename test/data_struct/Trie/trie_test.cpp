#include <cassert>  // 用于断言
#include <iostream> // 用于输入和输出流

#include "../../../include/data_struct/Trie/trie.hpp"

using Integer = std::unique_ptr<uint32_t>;

template <class T>
auto TestTrie(const std::string &key, const T &value, std::shared_ptr<int> test_count) -> void {
    std::cout << "TestTrie: " << (*test_count)++ << std::endl;

    auto trie = Trie();

    trie = trie.Put<T>(key, value);

    auto get_value = trie.Get<T>(key);

    assert(get_value != nullptr && *get_value == value);
}

auto main(int argc, char **argv) -> int {
    auto trie = Trie();

    std::shared_ptr<int> test_count = std::make_shared<int>(1);

    std::string key;
    std::string string_value;
    std::uint32_t uint32_value;
    std::uint64_t uint64_value;

    // Test 1 Basic Test
    key = "hello", string_value = "hello";
    TestTrie<std::string>(key, string_value, test_count);

    // Tets 2 Get Remove Put Mix Test
    std::cout << "TestTrie: " << (*test_count)++ << std::endl;

    key = "test", uint32_value = 1, trie = trie.Put<std::uint32_t>(key, uint32_value);
    key = "tes", uint32_value = 2, trie = trie.Put<std::uint32_t>(key, uint32_value);
    key = "t", string_value = "t", trie = trie.Put<std::string>(key, string_value);

    key = "tes";
    trie = trie.Remove(key);

    key = "test";
    assert(trie.Get<std::uint32_t>(key) != nullptr);
    assert(*trie.Get<std::uint32_t>(key) == 1);

    key = "t";
    assert(trie.Get<std::string>(key) != nullptr);
    assert(*trie.Get<std::string>(key) == key);

    trie = Trie();

    assert(trie.Get<std::string>(key) == nullptr);

    // Tets 3 unique_ptr Test write-on-copy
    std::cout << "TestTrie: " << (*test_count)++ << std::endl;

    trie = trie.Put<Integer>("test", std::make_unique<uint32_t>(233));
    trie.Get<Integer>("test");
    assert(trie.Get<Integer>("tes") == nullptr);

    return 0;
}