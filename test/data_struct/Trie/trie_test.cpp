#include <iostream>    // 用于输入和输出流

#include "../../../include/data_struct/Trie/trie.hpp"

template <class T>
auto TestTrie(std::string &key, T value) {
    std::cout << "K : " << key << " V : " << value << std::endl;

    auto trie = Trie();

    trie = trie.Put(key, value);

    if (trie.Get<T>(key) == nullptr) {
        std::cout << "Null" << std::endl;
    } else {
        std::cout << "Value : " << *(trie.Get<T>(key)) << std::endl;
    }
}

auto main(int argc, char **argv) -> int {
    std::string key;
    std::string string_value;
    std::uint32_t uint32_value = 0;
    std::uint64_t uint64_value = 0;

    key = "hello", string_value = "hello";
    TestTrie(key, string_value);

    key = "", string_value = "empty";
    TestTrie(key, string_value);

    return 0;
}