//
//  FacilitiesList.cpp
//  
//
//  Created by L. Dombrowski on 2/06/2020.
//

#include <iostream>
#include <vector>
#include <algorithm>
#include "FacilitiesList.h"


using namespace std;


FacilitiesList::FacilitiesList() {
};

void FacilitiesList::addFacility(Facility* ptr) {
    // Add another step to addFacility; push the Facility name and index into the
    // _facilityPtr vector into a binary search tree map for faster searching

    const auto next_index = _facilityPtr.size();
    _facilityPtr.push_back(ptr);
    _index_map.insert({ptr->getName(), next_index});
}

Facility* FacilitiesList::getPtr(string name) {

    // look up Facility pointer from string;
    // std::cout << "getPtr name " << name << "\n";
    // std::cout << "getPtr facilitylist ptr vector size " << _facilityPtr.size() << "\n";

    // Previous code: O(n) search through the _facilityPtr vector for the correct Facility pointer.

    /*
     *    for (auto i = 0u; i < _facilityPtr.size(); i++) {
     *
     *        // if facility name matches - store pointer in up and closest pointer variables
     *        if (name == _facilityPtr[i]->getName()) {
     *            // std::cout << "found match " << _facilityPtr[i]->getName() << "," << name << "\n";
     *            return _facilityPtr[i];
     *        }
     *    }
     */

    // New code: O(log n) search through the _index_map for the proper index into the 
    // _facilityPtr vector for the correct Facility pointer.

    const auto it = _index_map.find(name);
    if (it != _index_map.cend()) {
        return _facilityPtr[ it->second ];
    }
    
    std::cout << "couldn't find ptr to match name string for Facility = " << name << "." << "\n";

    return NULL;
}

FacilitiesList::~FacilitiesList() {
    //cout<<"Removing facilitiesList object "<<_id<<endl;
}


