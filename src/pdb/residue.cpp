#include "pdb/residue.hpp"
#include "pdb/secondary_structure.hpp"

namespace pdb {

std::unique_ptr<Residue> Residue::create(const std::vector<std::shared_ptr<Atom>>& atoms) {
    if (atoms.empty()) {
        return nullptr;
    }
    
    auto residue = std::make_unique<Residue>();
    residue->atoms = atoms;
    residue->resName = atoms[0]->resName;
    residue->chainID = atoms[0]->chainID;
    residue->resSeq = atoms[0]->resSeq;
    residue->type = ResidueType::Coil;
    residue->buildAtomMap();
    
    return residue;
}

void Residue::buildAtomMap() {
    atomsByName.clear();
    for (const auto& atom : atoms) {
        atomsByName[atom->name] = atom;
    }
}

std::vector<std::unique_ptr<Residue>> 
residuesForAtoms(const std::vector<std::shared_ptr<Atom>>& atoms,
                 const std::vector<std::unique_ptr<Helix>>& helixes,
                 const std::vector<std::unique_ptr<Strand>>& strands) {
    
    std::vector<std::unique_ptr<Residue>> residues;
    std::vector<std::shared_ptr<Atom>> group;
    int previous = -1;
    
    // Group atoms by residue sequence number
    for (const auto& atom : atoms) {
        int value = atom->resSeq;
        if (value != previous && !group.empty()) {
            auto residue = Residue::create(group);
            if (residue) {
                residues.push_back(std::move(residue));
            }
            group.clear();
        }
        group.push_back(atom);
        previous = value;
    }
    
    // Handle the last group
    if (!group.empty()) {
        auto residue = Residue::create(group);
        if (residue) {
            residues.push_back(std::move(residue));
        }
    }
    
    // Assign secondary structure types based on helixes
    for (auto& residue : residues) {
        for (const auto& helix : helixes) {
            if (residue->chainID == helix->initChainID && 
                residue->resSeq >= helix->initSeqNum && 
                residue->resSeq <= helix->endSeqNum) {
                residue->type = ResidueType::Helix;
            }
        }
        
        // Strands override helixes if both are present
        for (const auto& strand : strands) {
            if (residue->chainID == strand->initChainID && 
                residue->resSeq >= strand->initSeqNum && 
                residue->resSeq <= strand->endSeqNum) {
                residue->type = ResidueType::Strand;
            }
        }
    }
    
    return residues;
}

} // namespace pdb
