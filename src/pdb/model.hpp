#pragma once

#include "common.hpp"
#include "atom.hpp"
#include "residue.hpp"
#include "chain.hpp"
#include "secondary_structure.hpp"
#include <iostream>

namespace pdb {

class Model {
public:
    // Constructor
    Model() = default;
    
    // Methods
    void removeChain(const std::string& chainID);
    
    // Data members
    std::vector<std::shared_ptr<Atom>> atoms;
    std::vector<std::shared_ptr<Atom>> hetAtoms;
    std::vector<std::unique_ptr<Connection>> connections;
    std::vector<std::unique_ptr<Helix>> helixes;
    std::vector<std::unique_ptr<Strand>> strands;
    std::vector<Matrix> bioMatrixes;
    std::vector<Matrix> symMatrixes;
    std::vector<std::shared_ptr<Residue>> residues;
    std::vector<std::shared_ptr<Chain>> chains;
};

class Reader {
public:
    // Constructor
    explicit Reader(std::istream& stream) : stream_(stream) {}
    
    // Reading methods
    std::vector<std::unique_ptr<Model>> readAll();
    std::unique_ptr<Model> read();
    
private:
    std::istream& stream_;
};

} // namespace pdb
