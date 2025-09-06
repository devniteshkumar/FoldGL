#pragma once

#include "common.hpp"

namespace pdb {

class Helix {
public:
    // Constructor
    Helix() = default;
    
    // PDB record parsing
    static std::unique_ptr<Helix> parseHelix(const std::string& line);
    
    // Data members
    int serial{0};
    std::string helixID;
    std::string initResName;
    std::string initChainID;
    int initSeqNum{0};
    std::string initICode;
    std::string endResName;
    std::string endChainID;
    int endSeqNum{0};
    std::string endICode;
    int helixClass{0};
    int length{0};
};

class Strand {
public:
    // Constructor
    Strand() = default;
    
    // PDB record parsing
    static std::unique_ptr<Strand> parseStrand(const std::string& line);
    
    // Data members
    int strand{0};
    std::string sheetID;
    int numStrands{0};
    std::string initResName;
    std::string initChainID;
    int initSeqNum{0};
    std::string initICode;
    std::string endResName;
    std::string endChainID;
    int endSeqNum{0};
    std::string endICode;
    int sense{0};
    std::string curAtom;
    std::string curResName;
    std::string curChainId;
    int curResSeq{0};
    std::string curICode;
    std::string prevAtom;
    std::string prevResName;
    std::string prevChainId;
    int prevResSeq{0};
    std::string prevICode;
};

class Connection {
public:
    // Constructor
    Connection(int s1, int s2) : serial1(s1), serial2(s2) {}
    Connection() = default;
    
    // PDB record parsing
    static std::vector<std::unique_ptr<Connection>> parseConnections(const std::string& line);
    
    // Data members
    int serial1{0};
    int serial2{0};
};

} // namespace pdb
