#include "pdb/model.hpp"
#include <sstream>
#include <algorithm>

namespace pdb {

void Model::removeChain(const std::string& chainID) {
    // Remove atoms
    atoms.erase(std::remove_if(atoms.begin(), atoms.end(),
        [&chainID](const std::shared_ptr<Atom>& atom) {
            return atom->chainID == chainID;
        }), atoms.end());
    
    // Remove hetAtoms  
    hetAtoms.erase(std::remove_if(hetAtoms.begin(), hetAtoms.end(),
        [&chainID](const std::shared_ptr<Atom>& atom) {
            return atom->chainID == chainID;
        }), hetAtoms.end());
    
    // Remove residues
    residues.erase(std::remove_if(residues.begin(), residues.end(),
        [&chainID](const std::shared_ptr<Residue>& residue) {
            return residue->chainID == chainID;
        }), residues.end());
    
    // Remove chains
    chains.erase(std::remove_if(chains.begin(), chains.end(),
        [&chainID](const std::shared_ptr<Chain>& chain) {
            return chain->chainID == chainID;
        }), chains.end());
}

std::vector<std::unique_ptr<Model>> Reader::readAll() {
    std::vector<std::unique_ptr<Model>> models;
    
    while (true) {
        auto model = read();
        if (model) {
            models.push_back(std::move(model));
        } else {
            break; // End of stream or error
        }
    }
    
    return models;
}

std::unique_ptr<Model> Reader::read() {
    bool foundData = false;
    std::vector<std::shared_ptr<Atom>> atoms;
    std::vector<std::shared_ptr<Atom>> hetAtoms;
    std::vector<std::unique_ptr<Connection>> connections;
    std::vector<std::unique_ptr<Helix>> helixes;
    std::vector<std::unique_ptr<Strand>> strands;
    std::vector<Matrix> bioMatrixes;
    std::vector<Matrix> symMatrixes;
    
    Matrix currentMatrix = identity();
    std::string line;
    
    while (std::getline(stream_, line)) {
        // Check for model end
        if (foundData && line.substr(0, 6) == "ENDMDL") {
            break;
        }
        
        // Parse different record types
        if (line.substr(0, 6) == "ATOM  ") {
            auto atom = Atom::parseAtom(line);
            if (atom) {
                atoms.push_back(std::move(atom));
                foundData = true;
            }
        }
        else if (line.substr(0, 6) == "HETATM") {
            auto atom = Atom::parseAtom(line);
            if (atom) {
                hetAtoms.push_back(std::move(atom));
                foundData = true;
            }
        }
        else if (line.substr(0, 6) == "CONECT") {
            auto conns = Connection::parseConnections(line);
            for (auto& conn : conns) {
                connections.push_back(std::move(conn));
            }
            foundData = true;
        }
        else if (line.substr(0, 6) == "HELIX ") {
            auto helix = Helix::parseHelix(line);
            if (helix) {
                helixes.push_back(std::move(helix));
                foundData = true;
            }
        }
        else if (line.substr(0, 6) == "SHEET ") {
            auto strand = Strand::parseStrand(line);
            if (strand) {
                strands.push_back(std::move(strand));
                foundData = true;
            }
        }
        else if (line.length() > 23 && line.substr(0, 19) == "REMARK 350   BIOMT") {
            // Parse BIOMT transformation matrices
            if (line.length() >= 68) {
                int row = parseInt(line.substr(18, 1)) - 1;
                if (row >= 0 && row <= 2) {
                    currentMatrix[row][0] = parseFloat(line.substr(23, 10));
                    currentMatrix[row][1] = parseFloat(line.substr(33, 10));
                    currentMatrix[row][2] = parseFloat(line.substr(43, 10));
                    currentMatrix[row][3] = parseFloat(line.substr(53, 15));
                    if (row == 2) {
                        bioMatrixes.push_back(currentMatrix);
                        currentMatrix = identity();
                    }
                }
            }
        }
        else if (line.length() > 23 && line.substr(0, 19) == "REMARK 290   SMTRY") {
            // Parse SMTRY transformation matrices
            if (line.length() >= 68) {
                int row = parseInt(line.substr(18, 1)) - 1;
                if (row >= 0 && row <= 2) {
                    currentMatrix[row][0] = parseFloat(line.substr(23, 10));
                    currentMatrix[row][1] = parseFloat(line.substr(33, 10));
                    currentMatrix[row][2] = parseFloat(line.substr(43, 10));
                    currentMatrix[row][3] = parseFloat(line.substr(53, 15));
                    if (row == 2) {
                        symMatrixes.push_back(currentMatrix);
                        currentMatrix = identity();
                    }
                }
            }
        }
    }
    
    // If no data found, return nullptr
    if (!foundData) {
        return nullptr;
    }
    
    // Create model and build hierarchical structure
    auto model = std::make_unique<Model>();
    model->atoms = std::move(atoms);
    model->hetAtoms = std::move(hetAtoms);
    model->connections = std::move(connections);
    model->helixes = std::move(helixes);
    model->strands = std::move(strands);
    model->bioMatrixes = std::move(bioMatrixes);
    model->symMatrixes = std::move(symMatrixes);
    
    // Build residues and chains
    std::vector<std::unique_ptr<Residue>> residueList = 
        residuesForAtoms(model->atoms, model->helixes, model->strands);
    
    // Convert unique_ptr to shared_ptr for residues
    for (auto& residue : residueList) {
        model->residues.push_back(std::shared_ptr<Residue>(residue.release()));
    }
    
    // Build chains
    std::vector<std::unique_ptr<Chain>> chainList = 
        chainsForResidues(model->residues);
    
    // Convert unique_ptr to shared_ptr for chains  
    for (auto& chain : chainList) {
        model->chains.push_back(std::shared_ptr<Chain>(chain.release()));
    }
    
    return model;
}

} // namespace pdb
