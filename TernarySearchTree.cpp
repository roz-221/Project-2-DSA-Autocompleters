#include "TernarySearchTree.h"

using namespace std;

// store a word index in the node's top 10 list
void TernarySearchTree::record(Node* node, int wordIndex) {
    if (node->topCount < K) {
        node->top[node->topCount] = wordIndex;
        node->topCount++;
    }
}

// insert a word into the TST
TernarySearchTree::Node* TernarySearchTree::insert(
    Node* node, const string& word,
    size_t index, int wordIndex) {

    char c = word[index];

    if (node == nullptr) {
        node = new Node(c);
        nodeCount_++;
    }

    if (c < node->letter) {
        node->left = insert(node->left, word, index, wordIndex);
    }
    else if (c > node->letter) {
        node->right = insert(node->right, word, index, wordIndex);
    }
    else {
        // matching character, record the prefix
        record(node, wordIndex);

        if (index == word.size() - 1) {
            node->isWord = true;
        }
        else {
            node->middle = insert(
                node->middle, word, index + 1, wordIndex);
        }
    }

    return node;
}

// build tree using freq ranked words
void TernarySearchTree::build(
    const vector<string>& rankedWords) {

    clear(root_);
    root_ = nullptr;
    nodeCount_ = 0;

    words_ = rankedWords;
    rootTopCount_ = 0;

    for (size_t i = 0; i < words_.size(); i++) {
        const string& word = words_[i];

        if (word.empty()) {
            continue;
        }

        int wordIndex = static_cast<int>(i);

        // root prefix represents all words
        if (rootTopCount_ < K) {
            rootTop_[rootTopCount_] = wordIndex;
            rootTopCount_++;
        }

        root_ = insert(root_, word, 0, wordIndex);
    }
}

// find node representing the last prefix char
const TernarySearchTree::Node* TernarySearchTree::find(
    const Node* node, const string& prefix,
    size_t index) const {

    if (node == nullptr) {
        return nullptr;
    }

    char c = prefix[index];

    if (c < node->letter) {
        return find(node->left, prefix, index);
    }

    if (c > node->letter) {
        return find(node->right, prefix, index);
    }

    if (index == prefix.size() - 1) {
        return node;
    }

    return find(node->middle, prefix, index + 1);
}

// return top 10 suggestions for a prefix
vector<string> TernarySearchTree::suggest(
    const string& prefix) const {

    vector<string> results;

    if (prefix.empty()) {
        for (size_t i = 0; i < rootTopCount_; i++) {
            results.push_back(words_[rootTop_[i]]);
        }

        return results;
    }

    const Node* node = find(root_, prefix, 0);

    if (node == nullptr) {
        return results;
    }

    for (size_t i = 0; i < node->topCount; i++) {
        results.push_back(words_[node->top[i]]);
    }

    return results;
}


void TernarySearchTree::clear(Node* node) {
    if (node == nullptr) {
        return;
    }

    clear(node->left);
    clear(node->middle);
    clear(node->right);

    delete node;
}

TernarySearchTree::~TernarySearchTree() {
    clear(root_);
}
