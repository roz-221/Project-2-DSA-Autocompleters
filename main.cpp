#include <chrono>
#include <algorithm>
#include <chrono>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <random>
#include <string>
#include <vector>

#include "trie.h"
//#include "[RADIX TREE HEADER]"

// Set up clock in milliseconds to measure time
using Clock = std::chrono::steady_clock;

static double timeSince(Clock::time_point t0) {
    return std::chrono::duration<double, std::milli>(Clock::now() - t0).count();
}

int main (int argc, char* argv[]) {
    std::vector<std::string> words;
    words = {"app", "apple", "apply", "application", "apt", "ape", "apricot", "april", "bat", "batch", "banana"};

    trie demo;
    demo.build(words);

    // Check to see if trie is working as intended
    std::cout << "Sanity check, \"ap\" ->";
    for (const auto& w : demo.suggest("ap")) {
        std::cout << " " << w;
    }
    std::cout << "\n\n";
    if (words.empty()) { 
        std::cerr << "No valid words.\n"; 
        return 1; 
    }
    
    // Checking prefix
    if (argc > 1) {
        std::string prefix = argv[1];
        std::cout << std::fixed << std::setprecision(2);
        std::cout << "== " << prefix << " ==\n"
              << "  words            : " << demo.wordCount() << "\n"
              << "  nodes            : " << demo.nodeCount() << "\n"
              << "  node memory      : " << demo.memoryCost() / (1024.0 * 1024.0) << " MiB\n";
    }

    return 0;
}