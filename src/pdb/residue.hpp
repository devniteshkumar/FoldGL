#pragma once

#include "common.hpp"
#include "atom.hpp"

namespace pdb {

class Residue {
public:
    // Constructor
    Residue() = default;
    
    // Factory method
    static std::unique_ptr<Residue> create(const std::vector<std::shared_ptr<Atom>>& atoms);
    
    // Data members
    std::string resName;
    std::string chainID;
    int resSeq{0};
    std::vector<std::shared_ptr<Atom>> atoms;
    std::map<std::string, std::shared_ptr<Atom>> atomsByName;
    ResidueType type{ResidueType::Coil};
    
private:
    void buildAtomMap();
};

// Factory function for creating residues from atoms
std::vector<std::unique_ptr<Residue>> 
residuesForAtoms(const std::vector<std::shared_ptr<Atom>>& atoms,
                 const std::vector<std::unique_ptr<Helix>>& helixes,
                 const std::vector<std::unique_ptr<Strand>>& strands);

} // namespace pdb
