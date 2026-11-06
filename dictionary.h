#pragma once
// Shared dictionary loading/cleaning. Both the trie and the radix tree should
// be built from the same output of these functions so comparison is fair
 
#include <string>
#include <utility>
#include <vector>
 
namespace dict {
// Lowercases everything.
std::string normalize(const std::string& s);
 
// True if non-empty and every character is alphabetic.
bool isValidWord(const std::string& w);
 
// Takes (word, frequency) pairs and returns the cleaned word list:
std::vector<std::string> rankEntries(std::vector<std::pair<std::string, long long>> entries);
 
// Reads a file of lines "word" or "word<whitespace>count".
// Lines without a count are ranked by position (earlier = more common).
std::vector<std::string> loadRanked(const std::string& path);
 
// Reads the file valid_words_sorted_by_frequency.csv
// Only the word and frequency columns are read.
std::vector<std::string> loadFrequencyCsv(const std::string& path);
 
}