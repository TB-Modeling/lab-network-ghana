//
//  CreateParameterStats.h
//  
//
//  Created by L. Dombrowski on 5/7/21
//
// Split up CreateAgents into two files, CreateAgents and CreateStats
//
// output input parameters used for this model run

//
#ifndef CREATEPARAMETERSTATS_H
#define CREATEPARAMETERSTATS_H

#include <vector>

#include "FacilitiesList.h"
#include "Disease.h"
#include "Test.h"

using namespace std;

// count resources used at each facility for this timestep
void parameterStats( vector<Disease*> dis, vector<Test*> tes, FacilitiesList* list);

// find infectious days for active patient
void activePatientInfDays(Disease* dis, FacilitiesList* list, Stats* stats);

#endif // CREATEPARAMETERSTATS_H


