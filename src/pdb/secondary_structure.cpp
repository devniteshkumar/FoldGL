#include "pdb/secondary_structure.hpp"

namespace pdb {

// Helix implementation
std::unique_ptr<Helix> Helix::parseHelix(const std::string& line) {
    if (line.length() < 76) {
        return nullptr;
    }
    
    auto helix = std::make_unique<Helix>();
    
    helix->serial = parseInt(line.substr(7, 3));
    helix->helixID = parseString(line.substr(11, 3));
    helix->initResName = parseString(line.substr(15, 3));
    helix->initChainID = parseString(line.substr(19, 1));
    helix->initSeqNum = parseInt(line.substr(21, 4));
    helix->initICode = parseString(line.substr(25, 1));
    helix->endResName = parseString(line.substr(27, 3));
    helix->endChainID = parseString(line.substr(31, 1));
    helix->endSeqNum = parseInt(line.substr(33, 4));
    helix->endICode = parseString(line.substr(37, 1));
    helix->helixClass = parseInt(line.substr(38, 2));
    helix->length = parseInt(line.substr(71, 5));
    
    return helix;
}

// Strand implementation  
std::unique_ptr<Strand> Strand::parseStrand(const std::string& line) {
    if (line.length() < 70) {
        return nullptr;
    }
    
    auto strand = std::make_unique<Strand>();
    
    strand->strand = parseInt(line.substr(7, 3));
    strand->sheetID = parseString(line.substr(11, 3));
    strand->numStrands = parseInt(line.substr(14, 2));
    strand->initResName = parseString(line.substr(17, 3));
    strand->initChainID = parseString(line.substr(21, 1));
    strand->initSeqNum = parseInt(line.substr(22, 4));
    strand->initICode = parseString(line.substr(26, 1));
    strand->endResName = parseString(line.substr(28, 3));
    strand->endChainID = parseString(line.substr(32, 1));
    strand->endSeqNum = parseInt(line.substr(33, 4));
    strand->endICode = parseString(line.substr(37, 1));
    strand->sense = parseInt(line.substr(38, 2));
    strand->curAtom = parseString(line.substr(41, 4));
    strand->curResName = parseString(line.substr(45, 3));
    strand->curChainId = parseString(line.substr(49, 1));
    strand->curResSeq = parseInt(line.substr(50, 4));
    strand->curICode = parseString(line.substr(54, 1));
    strand->prevAtom = parseString(line.substr(56, 4));
    strand->prevResName = parseString(line.substr(60, 3));
    strand->prevChainId = parseString(line.substr(64, 1));
    strand->prevResSeq = parseInt(line.substr(65, 4));
    strand->prevICode = parseString(line.substr(69, 1));
    
    return strand;
}

// Connection implementation
std::vector<std::unique_ptr<Connection>> Connection::parseConnections(const std::string& line) {
    std::vector<std::unique_ptr<Connection>> connections;
    
    if (line.length() < 31) {
        return connections;
    }
    
    int a = parseInt(line.substr(6, 5));
    std::vector<int> bonds = {
        parseInt(line.substr(11, 5)),
        parseInt(line.substr(16, 5)),
        parseInt(line.substr(21, 5)),
        parseInt(line.substr(26, 5))
    };
    
    for (int b : bonds) {
        if (b != 0) {
            connections.push_back(std::make_unique<Connection>(a, b));
        }
    }
    
    return connections;
}

} // namespace pdb
