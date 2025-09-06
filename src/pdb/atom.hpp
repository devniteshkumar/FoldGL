#pragma once

#include "common.hpp"

namespace pdb {

class Atom {
public:
    // Constructor
    Atom() = default;
    
    // PDB record parsing
    static std::unique_ptr<Atom> parseAtom(const std::string& line);
    
    // Data members (matching PDB format)
    int serial{0};
    std::string name;
    std::string altLoc;
    std::string resName;
    std::string chainID;
    int resSeq{0};
    std::string iCode;
    double x{0.0}, y{0.0}, z{0.0};
    double occupancy{0.0};
    double tempFactor{0.0};
    std::string element;
    std::string charge;
};

} // namespace pdb
