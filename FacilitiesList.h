//
//  FacilitiesList.h
//  
//
//  Created by L. Dombrowski on 2/06/2020.
//

#ifndef FACILITIESLIST_H
#define FACILITIESLIST_H


#include <string>
#include <vector>
#include <map>

#include "Facility.h"

// class Testing;
// class Diagnosis;

using namespace  std;


class FacilitiesList {
private:

    // list of pointers for existing Facilities
    vector<Facility*> _facilityPtr;

    // As Facilities are added once at the beginning of the program, this
    // map will stay valid as long as FacilitiesList is valid.
    //
    // map; mapping facility names to indexes into _facilityPtr
    map<string, size_t> _index_map;


public:
    FacilitiesList();
    ~FacilitiesList();

    // get pointer for Facility list
    inline vector<Facility*> getListPtr() { return _facilityPtr; };

    // return number of facilities stored
    inline int getSize() { return _facilityPtr.size(); };

    // get Facility pointer from Facility name
    Facility* getPtr(string facility);

    // add facility to list
    // Moved out of inline and into FacilitiesList.cpp, as there are
    // more steps to do than before.
    void addFacility(Facility* ptr);

    // clear the vector and the map
    inline void clear() { _facilityPtr.clear(); _index_map.clear(); };


};

#endif /* defined(FacilitiesList_H) */
