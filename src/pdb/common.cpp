#include "pdb/common.hpp"
#include <algorithm>
#include <cctype>
#include <stdexcept>

namespace pdb {

std::string parseString(const std::string& str) {
    // Trim whitespace from both ends
    std::string result = str;
    result.erase(result.begin(), 
                 std::find_if(result.begin(), result.end(), 
                             [](unsigned char ch) { return !std::isspace(ch); }));
    result.erase(std::find_if(result.rbegin(), result.rend(), 
                             [](unsigned char ch) { return !std::isspace(ch); }).base(),
                 result.end());
    return result;
}

int parseInt(const std::string& str) {
    std::string trimmed = parseString(str);
    if (trimmed.empty()) {
        return 0;
    }
    
    try {
        return std::stoi(trimmed);
    } catch (const std::exception&) {
        return 0; // Follow Go behavior of returning 0 on parse error
    }
}

double parseFloat(const std::string& str) {
    std::string trimmed = parseString(str);
    if (trimmed.empty()) {
        return 0.0;
    }
    
    try {
        return std::stod(trimmed);
    } catch (const std::exception&) {
        return 0.0; // Follow Go behavior of returning 0 on parse error
    }
}

Matrix identity() {
    Matrix m{};
    m[0] = {1, 0, 0, 0};
    m[1] = {0, 1, 0, 0};
    m[2] = {0, 0, 1, 0};
    m[3] = {0, 0, 0, 1};
    return m;
}

} // namespace pdb
