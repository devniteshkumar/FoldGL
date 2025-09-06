#pragma once

#include <string>
#include <vector>
#include <memory>
#include <map>
#include <array>

namespace pdb {

// Forward declarations
class Atom;
class Residue;  
class Chain;
class Helix;
class Strand;
class Connection;
class Model;
class Reader;

// Type aliases
using Matrix = std::array<std::array<double, 4>, 4>;

// Enums
enum class ResidueType {
    Unknown = 0,
    Coil = 1,
    Helix = 2, 
    Strand = 3
};

// Utility functions
std::string parseString(const std::string& str);
int parseInt(const std::string& str);
double parseFloat(const std::string& str);
Matrix identity();

} // namespace pdb
