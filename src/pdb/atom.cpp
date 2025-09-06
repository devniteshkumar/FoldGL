#include "pdb/atom.hpp"

namespace pdb {

std::unique_ptr<Atom> Atom::parseAtom(const std::string& line) {
    if (line.length() < 80) {
        return nullptr; // Insufficient data
    }
    
    auto atom = std::make_unique<Atom>();
    
    // Parse according to PDB format specification
    atom->serial = parseInt(line.substr(6, 5));
    atom->name = parseString(line.substr(12, 4));
    atom->altLoc = parseString(line.substr(16, 1));
    atom->resName = parseString(line.substr(17, 3));
    atom->chainID = parseString(line.substr(21, 1));
    atom->resSeq = parseInt(line.substr(22, 4));
    atom->iCode = parseString(line.substr(26, 1));
    
    // Coordinates
    atom->x = parseFloat(line.substr(30, 8));
    atom->y = parseFloat(line.substr(38, 8));
    atom->z = parseFloat(line.substr(46, 8));
    
    // Occupancy and temperature factor
    atom->occupancy = parseFloat(line.substr(54, 6));
    atom->tempFactor = parseFloat(line.substr(60, 6));
    
    // Element and charge (check bounds)
    if (line.length() >= 78) {
        atom->element = parseString(line.substr(76, 2));
    }
    if (line.length() >= 80) {
        atom->charge = parseString(line.substr(78, 2));
    }
    
    return atom;
}

} // namespace pdb
