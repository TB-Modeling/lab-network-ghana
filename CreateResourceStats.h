//
//  CreateResourceStats.h
//  
//
//  Created by L. Dombrowski on 4/29/20
//
// Split up CreateAgents into two files, CreateAgents and CreateStats
//
// Split out of CreateStats on 5/14/20

//
#ifndef CREATERESOURCESTATS_H
#define CREATERESOURCESTATS_H

#include "FacilitiesList.h"

using namespace std;

// count resources used at each facility for this timestep
void resourceStats(FacilitiesList* list, bool fileFlag, int timeStep);


#endif // CREATERESOURCESTATS_H


