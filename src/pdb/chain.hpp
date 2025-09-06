#pragma once

#include "common.hpp"
#include "residue.hpp"

namespace pdb {

class Chain {
public:
    // Constructor
    Chain() = default;
    
    // Factory method
    static std::unique_ptr<Chain> create(const std::vector<std::shared_ptr<Residue>>& residues);
    
    // Data members
    std::string chainID;
    std::vector<std::shared_ptr<Residue>> residues;
};

// Factory function for creating chains from residues
std::vector<std::unique_ptr<Chain>> 
chainsForResidues(const std::vector<std::shared_ptr<Residue>>& residues);

} // namespace pdb
