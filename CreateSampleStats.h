//
//  CreateSampleStats.h
//  
//
//  Created by L. Dombrowski on 4/29/20
//
// Split up CreateAgents into two files, CreateAgents and CreateStats
//
// Split out of CreateStats on 5/14/20

//
#ifndef CREATESAMPLESTATS_H
#define CREATESAMPLESTATS_H

#include <string>
#include <vector>

#include "FacilitiesList.h"

using namespace std;

// output current agent stats
void sampleStats(FacilitiesList* list, vector<string> diseaseList, bool fileFlag, bool yearly, int timeStep);

// output current agent stats
void sampleInactiveStats(FacilitiesList* list, vector<string> diseaseList, bool fileFlag, bool yearly, int timeStep);



#endif // CREATESAMPLESTATS_H


