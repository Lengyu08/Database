#include <iostream>    // 用于输入和输出流
#include <map>         // 哈希表
#include <memory>      // 智能指针
#include <string_view> // 字符串视图, 避免不必要的字符串拷贝
#include <utility>     // 包含一些实用工具
#include <vector>      // 向量数组

class TrieNode {
public:
    bool is_value_node{false};

    std::map<char, std::shared_ptr<const TrieNode>> trie_map;

    TrieNode() = default;

    virtual ~TrieNode() = default;

    explicit TrieNode(std::map<char, std::shared_ptr<const TrieNode>> input_map) :
        trie_map(std::move(input_map)) {
    }

    virtual auto Clone() const -> std::unique_ptr<TrieNode> {
        return std::make_unique<TrieNode>(trie_map);
    }
};

template <class T>
class TrieNodeWithValue : public TrieNode {
public:
    std::shared_ptr<T> trie_value;

    explicit TrieNodeWithValue(std::shared_ptr<T> input_value) :
        trie_value(std::move(input_value)) {
        this->is_value_node = true;
    }

    TrieNodeWithValue(std::map<char, std::shared_ptr<const TrieNode>> input_map, std::shared_ptr<T> input_value) :
        TrieNode(std::move(input_map)), trie_value(std::move(input_value)) {
        this->is_value_node = true;
    }

    auto Clone() const -> std::unique_ptr<TrieNode> override {
        return std::make_unique<TrieNodeWithValue<T>>(trie_map, trie_value);
    }
};

class Trie {
public:
    std::shared_ptr<const TrieNode> trie_root{nullptr};

    explicit Trie(std::shared_ptr<const TrieNode> input_root) :
        trie_root(std::move(input_root)) {
    }

public:
    Trie() = default;

    template <class T>
    auto Get(std::string_view key) const -> const T * {
        if (key.empty() || !trie_root) {
            return nullptr;
        }
        
        std::shared_ptr<TrieNode> current_root = trie_root->Clone();

        std::shared_ptr<TrieNode> node = current_root;

        for (const char &c : key) {
            if (node->trie_map.find(c) != node->trie_map.end()) {
                node = node->trie_map.at(c)->Clone(); // 切换到下一个节点
            } else {
                return nullptr;
            }
        }

        if (node->is_value_node) {
            auto value_node = std::dynamic_pointer_cast<TrieNodeWithValue<T>>(node);
            return value_node->trie_value.get();
        } else {
            return nullptr;
        }
    }

    template <class T>
    auto Put(std::string_view key, T value) const -> Trie {
        if (key.empty()) {
            return Trie(trie_root);
        }

        std::shared_ptr<TrieNode> current_root = trie_root ? trie_root->Clone() : std::make_shared<TrieNode>();

        std::shared_ptr<TrieNode> node = current_root;

        for (unsigned int i = 0; i < key.size(); i++) {
            if (i == key.size() - 1) {
                std::shared_ptr<T> value_ptr = std::make_shared<T>(value);
                std::shared_ptr<TrieNode> new_node = std::make_shared<TrieNodeWithValue<T>>(value_ptr);

                if (node->trie_map.find(key[i]) != node->trie_map.end()) {
                    for (const auto &pair : node->trie_map.at(key[i])->trie_map) {
                        std::cout << "connet the old map's char: " << pair.first << std::endl;
                        new_node->trie_map.emplace(pair.first, pair.second->Clone());
                    }
                    node->trie_map.erase(key[i]);
                }

                std::cout << node->is_value_node << std::endl;
                node->trie_map.emplace(key[i], new_node);
                node = new_node;
                std::cout << node->is_value_node << std::endl;
                break;
            }

            if (node->trie_map.find(key[i]) != node->trie_map.end()) {
                // node = node->trie_map.at(key[i])->Clone();
                std::shared_ptr<TrieNode> new_node = node->trie_map.at(key[i])->Clone();
                node->trie_map.erase(key[i]);
                node->trie_map.emplace(key[i], new_node);
                node = new_node;
            } else {
                // std::cout << "Input : " << key[i] << " in map is not found" << std::endl;
                std::shared_ptr<TrieNode> new_node = std::make_shared<TrieNode>();
                node->trie_map.emplace(key[i], new_node);
                node = new_node;
            }
        }

        return Trie(current_root);
    }

    auto Remove(std::string_view key) const -> Trie {
        if (key.empty()) {
            return Trie(trie_root);
        }

        std::shared_ptr<TrieNode> current_root = trie_root ? trie_root->Clone() : std::make_shared<TrieNode>();

        std::shared_ptr<TrieNode> node = current_root;

        for (unsigned int i = 0; i < key.size(); i++) {
            if (i == key.size() - 1) {
                std::shared_ptr<TrieNode> new_node = std::make_shared<TrieNode>();

                if (node->trie_map.find(key[i]) != node->trie_map.end()) {
                    for (const auto &pair : node->trie_map.at(key[i])->trie_map) {
                        std::cout << "connet the old map's char: " << pair.first << std::endl;
                        new_node->trie_map.emplace(pair.first, pair.second->Clone());
                    }
                    node->trie_map.erase(key[i]);
                }

                std::cout << node->is_value_node << std::endl;
                node->trie_map.emplace(key[i], new_node);
                node = new_node;
                std::cout << node->is_value_node << std::endl;
                break;
            }

            if (node->trie_map.find(key[i]) != node->trie_map.end()) {
                // node = node->trie_map.at(key[i])->Clone();
                std::shared_ptr<TrieNode> new_node = node->trie_map.at(key[i])->Clone();
                node->trie_map.erase(key[i]);
                node->trie_map.emplace(key[i], new_node);
                node = new_node;
            } else {
                // std::cout << "Input : " << key[i] << " in map is not found" << std::endl;
                std::shared_ptr<TrieNode> new_node = std::make_shared<TrieNode>();
                node->trie_map.emplace(key[i], new_node);
                node = new_node;
            }
        }

        return Trie(current_root);
    }

    auto printTrieNode(std::string key, std::shared_ptr<const TrieNode> current_root) -> void {
        std::shared_ptr<TrieNode> node = current_root->Clone();
        for (const char &c : key) {
            std::cout << "char: " << c;
            if (node->trie_map.find(c) != node->trie_map.end()) {
                node = node->trie_map.at(c)->Clone(); // 切换到下一个节点
            } else {
                std::cout << " is not found!" << std::endl;
                break;
            }

            if (node->is_value_node) {
                auto value_node = std::dynamic_pointer_cast<TrieNodeWithValue<std::string>>(node);
                std::cout << "'s value is " << *(value_node->trie_value) << std::endl;
            } else {
                std::cout << " is not value node" << std::endl;
            }
        }
    }

};

auto main(int argc, char **argv) -> int {
    auto trie = Trie();

    std::string key = "hello";
    std::cout << "Strat to put " << key << std::endl;
    trie = trie.Put(key, key);
    trie.printTrieNode("hello", trie.trie_root);

    std::cout << std::endl;

    key = "h";
    std::cout << "Strat to put " << key << std::endl;
    trie = trie.Put(key, key);
    trie.printTrieNode("hello", trie.trie_root);

    std::cout << std::endl;

    key = "he";
    std::cout << "Strat to put " << key << std::endl;
    trie = trie.Put(key, key);
    trie.printTrieNode("hello", trie.trie_root);

    std::string new_key = "new";
    std::cout << "Strat to put " << new_key << std::endl;
    trie = trie.Put(key, new_key);
    trie.printTrieNode("hello", trie.trie_root);

    std::cout << std::endl;

    std::cout << "Start to remove " << key << std::endl;
    trie = trie.Remove(key);
    trie.printTrieNode("hello", trie.trie_root); 

    std::cout << std::endl;

    uint32_t value = 123;
    std::cout << "Strat to put " << key << std::endl;
    trie = trie.Put(key, value);
    std::cout << *(trie.Get<uint32_t>("he")) << std::endl;
    if (trie.Get<uint32_t>("he") == nullptr) {
        std::cout << "he is nullptr" << std::endl;
    }
    std::cout << *(trie.Get<std::string>("hello")) << std::endl;

    return 0;
}
