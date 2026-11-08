#pragma once

#include <array>
#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>

class trie {
public: 
    static constexpr std::size_t K = 10; // K most common words
    trie();
    void build(const std::vector<std::string>& rankedWords);

    std::vector<std::string> suggest(const std::string& prefix) const;

    std::size_t nodeCount() const { return nodes_.size(); }
    std::size_t wordCount() const { return words_.size(); }
 
    // Approximate bytes used by the node array (excludes the word strings,
    // which any implementation has to store).
    std::size_t memoryCost() const { return nodes_.capacity() * sizeof(Node); }

private:
    struct Node {
        std::array<int, 26> child;      // index into nodes_, or -1
        std::array<int, K> top;         // indices into words_, best first
        std::uint8_t topCount = 0;      // how many entries of `top` are used
        Node() { child.fill(-1); }
    };
 
    std::vector<Node> nodes_;
    std::vector<std::string> words_;    // In ranked order
};

