#include "pdb/chain.hpp"

namespace pdb {

std::unique_ptr<Chain> Chain::create(const std::vector<std::shared_ptr<Residue>>& residues) {
    if (residues.empty()) {
        return nullptr;
    }
    
    auto chain = std::make_unique<Chain>();
    chain->chainID = residues[0]->chainID;
    chain->residues = residues;
    
    return chain;
}

std::vector<std::unique_ptr<Chain>> 
chainsForResidues(const std::vector<std::shared_ptr<Residue>>& residues) {
    std::vector<std::unique_ptr<Chain>> chains;
    std::vector<std::shared_ptr<Residue>> group;
    std::string previous = "";
    
    // Group residues by chain ID
    for (const auto& residue : residues) {
        std::string value = residue->chainID;
        if (value != previous && !group.empty()) {
            auto chain = Chain::create(group);
            if (chain) {
                chains.push_back(std::move(chain));
            }
            group.clear();
        }
        group.push_back(residue);
        previous = value;
    }
    
    // Handle the last group
    if (!group.empty()) {
        auto chain = Chain::create(group);
        if (chain) {
            chains.push_back(std::move(chain));
        }
    }
    
    return chains;
}

} // namespace pdb
