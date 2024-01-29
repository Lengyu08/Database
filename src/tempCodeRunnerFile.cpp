#include <string_view>

#include <map>      	// 哈希表
#include <memory>   	// 智能指针
#include <utility>  	// 包含一些实用工具
#include <iostream> 	// 用于输入和输出流

// TrieNode 是 Trie 中的一个节点。
class TrieNode {
public:
    // 指示节点是否具有值
    bool is_value_node{false};

    // 下一个子节点的映射
    std::map<char, std::shared_ptr<const TrieNode>> trie_map;

    // 编译器使用默认的构造函数实现, 而不是显式地提供自己的实现
    TrieNode() = default;

    // 虚函数析构, 便于子类特殊处理内存
    virtual ~TrieNode() = default;

    /**
     * Note :
     * * (1) explicit 禁止构造函数隐式类型转换
     * * (2) : 成员初始化列表
     * * (3) std::move() 拷贝, input_map 参数会在构造函数结束时被销毁
     */
    explicit TrieNode(std::map<char, std::shared_ptr<const TrieNode>> input_map) :
        trie_map(std::move(input_map)) {
    }

    /**
     * Function : Clone 返回此 TrieNode 的副本 如果 TrieNode 具有值, 则将复制该值
     *
     * Note :
     * * (1) 如果要将 unique_ptr 转换为 shared_ptr, 可以使用 std::shared_ptr<T>(std::move(ptr))
     * * (2) 不能使用复制构造函数来克隆节点, 因为它不知道 TrieNode 是否包含值。
     * * (3)
     * Return : 指向 TrieNode 的 unique_ptr
     */
    virtual auto Clone() const -> std::unique_ptr<TrieNode> {
        return std::make_unique<TrieNode>(trie_map);
    }
};

// TrieNodeWithValue 是具有与之关联的值类型 T 的 TrieNode。
template <class T>
class TrieNodeWithValue : public TrieNode {
public:
    std::shared_ptr<T> trie_value;

    explicit TrieNodeWithValue(std::shared_ptr<T> input_value) :
        trie_value(std::move(input_value)) {
        this -> is_value_node = true;
    }

    TrieNodeWithValue(std::map<char, std::shared_ptr<const TrieNode>> input_map, std::shared_ptr<T> input_value) :
        TrieNode(std::move(input_map)), trie_value(std::move(input_value)) {
        this -> is_value_node = true;
    }

    /**
     * Function : Clone 返回此 TrieNode 的副本 如果 TrieNode 具有值, 则将复制该值
     * Note :
     * * (1) 如果要将 unique_ptr 转换为 shared_ptr, 可以使用 std::shared_ptr<T>(std::move(ptr))
     * * (2) 不能使用复制构造函数来克隆节点, 因为它不知道 TrieNode 是否包含值。
     * * (3) 继承自 TrieNode 的 Clone() 函数, override 代表重写虚函数
     * Return : 指向 TrieNode 的 unique_ptr
     */
    auto Clone() const -> std::unique_ptr<TrieNode> override {
        return std::make_unique<TrieNodeWithValue<T>>(trie_map, trie_value);
    }
};

auto main(int argc, char **argv) -> int {
    // 创建一个不带值的 TrieNode
    auto root = std::make_unique<TrieNode>();

    std::cout << "Hello, Trie!" << std::endl;
    return 0;
}
