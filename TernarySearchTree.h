#pragma once

#include <array>
#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>

class TernarySearchTree {
public:
    static constexpr std::size_t K = 10;

    TernarySearchTree() = default;

    TernarySearchTree(const TernarySearchTree&) = delete;
    TernarySearchTree& operator=(const TernarySearchTree&) = delete;

    ~TernarySearchTree();

    void build(const std::vector<std::string>& rankedWords);

    std::vector<std::string> suggest(const std::string& prefix) const;

    std::size_t nodeCount() const {
        return nodeCount_;
    }

    std::size_t wordCount() const {
        return words_.size();
    }

    std::size_t memoryCost() const {
        return nodeCount_ * sizeof(Node);
    }

private:
    struct Node {
        char letter;
        bool isWord = false;

        Node* left = nullptr;
        Node* middle = nullptr;
        Node* right = nullptr;

        std::array<int, K> top{};
        std::uint8_t topCount = 0;

        explicit Node(char c) : letter(c) {}
    };

    Node* root_ = nullptr;

    std::vector<std::string> words_;
    std::array<int, K> rootTop_{};
    std::uint8_t rootTopCount_ = 0;

    std::size_t nodeCount_ = 0;

    Node* insert(Node* node, const std::string& word,
                 std::size_t index, int wordIndex);

    const Node* find(const Node* node,
                     const std::string& prefix,
                     std::size_t index) const;

    void record(Node* node, int wordIndex);

    void clear(Node* node);
};
