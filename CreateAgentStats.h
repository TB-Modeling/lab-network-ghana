//
//  CreateAgentStats.h
//  
//
//  Created by L. Dombrowski on 4/29/20
//
// Split up CreateAgents into two files, CreateAgents and CreateStats
//
// Split outof CreateStats 5-14-20

//
#ifndef CREATEAGENTSTATS_H
#define CREATEAGENTSTATS_H

#include <vector>
#include <string>

#include "FacilitiesList.h"
#include "Patient.h"

using namespace std;

// output current agent stats
void agentStats(FacilitiesList* list, vector<string> diseaseList, vector<Disease*> dis, bool fileFlag, bool invdividualFlag,  bool invdividualSamplingFlag, bool yearly, int timeStep);

// dump patient and sample info
void dumpPatientInfo(Patient* pat, int timeStep);

#endif // CREATEAGENTSTATS_H


