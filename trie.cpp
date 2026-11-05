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

        // NEED TO CREATE CHAR LOOP
    }
    nodes_.shrink_to_fit();
}