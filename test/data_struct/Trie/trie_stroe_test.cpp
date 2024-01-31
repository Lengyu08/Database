#include <future>
#include <thread>
#include <iostream>

#include "../../../include/data_struct/Trie/trie_stroe.hpp"

/// 一个特殊的类型，会阻塞移动构造函数和移动赋值运算符。在 TrieStore 的测试中使用。
class MoveBlocked {
public:
    bool waited{false};    // 表示对象是否已经等待过。
    std::future<int> wait; // 用于阻塞移动构造和移动赋值的 future 对象。

    // 构造函数，接受一个 std::future<int> 参数并将其移动构造到成员变量 wait 中。
    explicit MoveBlocked(std::future<int> input_wait) :
        wait(std::move(input_wait)) {
    }

    // 禁用拷贝构造函数，确保对象无法通过拷贝构造。
    MoveBlocked(const MoveBlocked &) = delete;

    // 移动构造函数，阻塞直到传递进来的 future 完成，然后将其标记为已等待。
    MoveBlocked(MoveBlocked &&that) noexcept {
        if (!that.waited) {
            that.wait.get();
        }
        that.waited = waited = true;
    }

    // 禁用拷贝赋值运算符，确保对象无法通过拷贝赋值。
    auto operator=(const MoveBlocked &) -> MoveBlocked & = delete;

    // 移动赋值运算符，阻塞直到传递进来的 future 完成，然后将其标记为已等待。
    auto operator=(MoveBlocked &&that) noexcept -> MoveBlocked & {
        if (!that.waited) {
            that.wait.get();
        }
        that.waited = waited = true;
        return *this;
    }
};

auto main(int argc, char **argv) -> int {
    auto trie_store = TrieStore();

    std::shared_ptr<int> test_count = std::make_shared<int>(1);

    std::cout << "TestTrieStore: " << (*test_count)++ << std::endl;

    return 0;
}