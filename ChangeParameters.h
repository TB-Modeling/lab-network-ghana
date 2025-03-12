//
//  ChangeParameters.h
//  
//
//  Created by L. Dombrowski on 9/13/21

//
#ifndef CHANGEPARAMETERS_H
#define CHANGEPARAMETERS_H

#include <string>

#include "Facility.h"
#include "FacilitiesList.h"
#include "Test.h"

using namespace std;

// check parameter_range.csv file in ./input/input-change-parameters and change input parameters as necessary
void changeParameters(string scenario, string jobNo, string facSpecFile);

// read input parameters into variable array
void readParameterIntoArray(vector<string > parameterInput, parameters* variable);

// read input parameters into variable array
void readParameterRegionTierIntoArray(vector<string > parameterInput, string region, int tier, struct parametersRegionTier* variable);

// dump variables
void dumpParameter(parameters* variable, string name);

// dump variables
void dumpParameterRegionTier(struct parametersRegionTier* variable, string name);
	
	// copy ./input/ paramaters from one directory to another
void copyInput(string source, string destination, int scenarioNo, string jobNo, string popBinFile, string diseaseFile, string facilitiesFile, scenarioParam fileNames[], string  parameterFlagFile, string filenameFile, string regDistFile, string populationFile, string regionDistrictSpecificFile, string runSpecificFile, string testFile);


#endif // CHANGEPARAMETERS_H


