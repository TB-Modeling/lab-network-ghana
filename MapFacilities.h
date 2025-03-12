//
//  MapFacilities.h
//  
//
//  Created by L. Dombrowski on 4/29/20
//
//  Split up CreateAgents into two files, CreateAgents and CreateStats
//
//  Split off of CreateStats.h on 5/14/20
//


//
#ifndef MAPFACILITIES_H
#define MAPFACILITIES_H

#include "FacilitiesList.h"

using namespace std;

// output data for created facilities
void mapFacilities(FacilitiesList* list, vector<string> diseaseList, bool fileFlag);

#endif // MAPFACILITIES_H


