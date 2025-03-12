//
//  CreateTableauStats.h
//  
//
//  Created by L. Dombrowski on 4/29/20
//
// Split up CreateAgents into two files, CreateAgents and CreateStats
//
// Split out of CreateStats on 5/14/20

//
#ifndef CREATETABLEAUSTATS_H
#define CREATETABLEAUSTATS_H

#include <string>
#include <vector>

#include "FacilitiesList.h"
#include "Test.h"

using namespace std;

// output stats for Tableau
void tableauStats(FacilitiesList* list, vector<string> diseaseList, vector<Test*> tests,  bool fileFlag, bool invdividualFlag, int timeStep, int year);

#endif // CREATETABLEAUSTATS_H


