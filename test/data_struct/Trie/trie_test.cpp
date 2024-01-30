#include <future>   // 用于异步编程
#include <cassert>  // 用于断言
#include <iostream> // 用于输入和输出流

#include "../../../include/data_struct/Trie/trie.hpp"

class MoveBlocked {
public:
    // 构造函数，接受一个 std::future<int> 参数，使用 std::move 进行移动构造
    explicit MoveBlocked(std::future<int> wait) :
        wait_(std::move(wait)) {
    }

    // 禁用拷贝构造函数
    MoveBlocked(const MoveBlocked &) = delete;

    // 移动构造函数，使用 noexcept 保证不抛出异常
    MoveBlocked(MoveBlocked &&that) noexcept {
        // 如果之前未等待过，此时等待 std::future 的结果
        if (!that.waited_) {
            that.wait_.get();
        }
        // 标记为已等待
        that.waited_ = waited_ = true;
    }

    // 禁用拷贝赋值运算符
    auto operator=(const MoveBlocked &) -> MoveBlocked & = delete;

    // 移动赋值运算符，使用 noexcept 保证不抛出异常
    auto operator=(MoveBlocked &&that) noexcept -> MoveBlocked & {
        // 如果之前未等待过，此时等待 std::future 的结果
        if (!that.waited_) {
            that.wait_.get();
        }
        // 标记为已等待
        that.waited_ = waited_ = true;
        return *this;
    }

    // 表示是否已经等待过
    bool waited_{false};

    // 存储 std::future<int> 对象
    std::future<int> wait_;
};

template <class T>
auto TestTrie(const std::string &key, const T &value, std::shared_ptr<int> test_count) -> void {
    std::cout << "TestTrie: " << (*test_count)++ << std::endl;

    auto trie = Trie();

    trie = trie.Put<T>(key, value);

    auto get_value = trie.Get<T>(key);

    assert(get_value != nullptr && *get_value == value);
}

int main(int argc, char **argv) {
    auto trie = Trie();

    std::shared_ptr<int> test_count = std::make_shared<int>(1);

    std::string key;
    std::string string_value;
    std::uint32_t uint32_value;
    std::uint64_t uint64_value;
    std::unique_ptr<u_int32_t> unique_ptr_uint32_value;

    // Test 1
    key = "hello", string_value = "hello";
    TestTrie<std::string>(key, string_value, test_count);

    // Test 2
    key = "", string_value = "empty";
    TestTrie<std::string>(key, string_value, test_count);

    // Tets 3
    key = "hello", uint32_value = 1;
    trie.Put<std::uint32_t>(key, uint32_value);
    key = "hell";
    trie.Put<std::uint32_t>(key, uint32_value);
    trie.Remove(key);

    std::cout << "TestTrie: " << (*test_count)++ << std::endl;
    assert(trie.Get<std::uint32_t>(key) == nullptr);
    key = "hello";
    assert(trie.Get<std::uint32_t>(key) != nullptr);

    // Tets 4
    return 0;
}