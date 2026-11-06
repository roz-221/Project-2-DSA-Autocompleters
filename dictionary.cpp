#include "dictionary.h"

#include <algorithm>
#include <cctype>
#include <fstream>
#include <sstream>
#include <unordered_set>
 
namespace dict {
 
std::string normalize(const std::string& s) {
    std::string r = s;
    for (char& c : r) c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
    return r;
}
 
bool isValidWord(const std::string& w) {
    if (w.empty()) return false;
    for (char c : w)
        if (c < 'a' || c > 'z') return false;
    return true;
}
 
std::vector<std::string> rankEntries(std::vector<std::pair<std::string, long long>> entries) {
    for (auto& e : entries) e.first = normalize(e.first);
 
    std::sort(entries.begin(), entries.end(), [](const auto& a, const auto& b) {
        if (a.second != b.second) return a.second > b.second;
        return a.first < b.first;
    });
 
    std::unordered_set<std::string> seen;
    std::vector<std::string> ranked;
    ranked.reserve(entries.size());
    for (auto& e : entries) {
        // First occurrence after sorting is the highest-frequency one.
        if (isValidWord(e.first) && seen.insert(e.first).second)
            ranked.push_back(std::move(e.first));
    }
    return ranked;
}
 
std::vector<std::string> loadRanked(const std::string& path) {
    std::ifstream in(path);
    std::vector<std::pair<std::string, long long>> entries;
    std::vector<bool> hasCount;
    std::string line;
    while (std::getline(in, line)) {
        std::istringstream iss(line);
        std::string w;
        if (!(iss >> w)) continue;
        long long c = 0;
        bool has = static_cast<bool>(iss >> c);
        entries.emplace_back(w, c);
        hasCount.push_back(has);
    }
    const long long total = static_cast<long long>(entries.size());
    for (size_t i = 0; i < entries.size(); i++)
        if (!hasCount[i]) entries[i].second = total - static_cast<long long>(i);
 
    return rankEntries(std::move(entries));
}
 
namespace {
 
// Minimal quote-aware CSV splitter (handles "..." fields and CRLF endings).
std::vector<std::string> splitCsvLine(const std::string& line) {
    std::vector<std::string> fields;
    std::string cur;
    bool inQuotes = false;
    for (char ch : line) {
        if (ch == '"') inQuotes = !inQuotes;
        else if (ch == ',' && !inQuotes) { fields.push_back(cur); cur.clear(); }
        else if (ch != '\r') cur.push_back(ch);
    }
    fields.push_back(cur);
    return fields;
}
 
// Keeps digits only, so "26,251,832,128" or " 42 " both parse.
long long parseCount(const std::string& s) {
    long long v = 0;
    for (char ch : s)
        if (ch >= '0' && ch <= '9') v = v * 10 + (ch - '0');
    return v;
}
 
}  // namespace
 
std::vector<std::string> loadFrequencyCsv(const std::string& path) {
    std::ifstream in(path);
    std::vector<std::pair<std::string, long long>> entries;
    std::string line;
    std::size_t wordCol = 1, freqCol = 2;
    bool first = true;
 
    while (std::getline(in, line)) {
        if (first && line.size() >= 3 && line.compare(0, 3, "\xEF\xBB\xBF") == 0)
            line.erase(0, 3);  // strip UTF-8 BOM
 
        auto f = splitCsvLine(line);
 
        if (first) {
            first = false;
            bool header = false;
            for (std::size_t i = 0; i < f.size(); ++i) {
                std::string name = normalize(f[i]);
                if (name.find("word") != std::string::npos && name.find("stem") == std::string::npos) {
                    wordCol = i; header = true;
                } else if (name.find("freq") != std::string::npos) {
                    freqCol = i; header = true;
                }
            }
            if (header) continue;
        }
 
        if (f.size() <= std::max(wordCol, freqCol)) continue;
        entries.emplace_back(f[wordCol], parseCount(f[freqCol]));
    }
    return rankEntries(std::move(entries));
}
 
}