#include "trie.h"

trie::trie() {nodes_.emplace_back();}

void trie::build(const std::vector<std::string>& rankedWords) {
    nodes_.clear();
    nodes_.emplace_back();
    words_ = rankedWords;

    for (std:: size_t w = 0; w < words_.size(); ++w) {
        const int wordIdx = static_cast<int>(w);
        int cur = 0;
 
        auto record = [&](int node) {
            Node& n = nodes_[node];
            if (n.topCount < K) n.top[n.topCount++] = wordIdx;
        };
 
        record(cur);  // root holds the global top K

        for (char c : words_[w]) {
            const int slot = c - 'a';
            if (nodes_[cur].child[slot] == -1) {
                const int fresh = static_cast<int>(nodes_.size());
                nodes_.emplace_back();               // may reallocate: use indices only
                nodes_[cur].child[slot] = fresh;
            }
            cur = nodes_[cur].child[slot];
            record(cur);
        }
    }
    nodes_.shrink_to_fit();
}

std::vector<std::string> trie::suggest(const std::string& prefix) const {
    int cur = 0;
    
    // Check each character in prefix
    for (char c : prefix) {
        if (c < 'a' || c > 'z') return {};
        cur = nodes_[cur].child[c - 'a'];
        if (cur == -1) return {};
    }

    // Return matches
    const Node& n = nodes_[cur];
    std::vector<std::string> result;
    result.reserve(n.topCount);
    for (std::uint8_t i = 0; i < n.topCount; ++i) result.push_back(words_[n.top[i]]);
    return result;
}