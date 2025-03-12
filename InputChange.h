//
//  InputChange.h
//  
//
//  Created by L. Dombrowski on 4/7/21

//
#ifndef INPUTCHANGE_H	
#define INPUTCHANGE_H

#include <string>
#include <vector>
#include <fstream>
#include <chrono>
#include "GlobalParameters.h"

using namespace std;
// using namespace GAGENTS;

struct parameters {

    bool active;
    string distribution;
    float value;
    float mean;
    float lowerBound;
    float upperBound;
    float shape1Alpha;
    float shape2Beta;
    float scaleHB;
};

struct parametersRegionTier {

    string region;
    bool active[10];
    string distribution[10];
    float value[10];
    float mean[10];
    float lowerBound[10];
    float upperBound[10];
    float shape1Alpha[10];
    float shape2Beta[10];
    float scaleHB[10];
};

struct parameterStrings {

    bool active;
    string distribution;
    int value;
    string lowerBound;
    string upperBound;
    string shape1Alpha;
    string shape2Beta;
    string scaleHB;
    string region;
    string tier[10];
};

// set up range for incidence and modify
void inputChangeIncidence(vector<vector<string>> regionDistrictSpecificInput, string regionDistrict, string disease, parameters incidence, parameters probRepresentGeoDist, bool randomFlag, int noBins, int binNumber, float alpha, bool inPlaceFlag, string scenario, string jobNo, bool wideOpenFlag);

// vary different disease parameters
void inputChangeDisease(vector<vector<string>> diseaseInput, string diseases, string disease, parameters infUninfRatio, parameters perInitInf, parameters prpIncPresent, parameters initTreatSuccess, parameters initTreatDeath, parameters retreatmentPer, parameters probPatientReturns, parameters preTestMDCDInf, parameters preTestMDCDUnInf, parameters preTestLtoF, parametersRegionTier* preTestLtoFRegion, parameters preTreatLtoF, parametersRegionTier* preTreatLtoFRegion, parameters treatLtoF, parametersRegionTier* treatLtoFRegion, parameters representDeathPer, parameters newTimeDeathRetreat, parameters newTimeLtoFRetreat, parameters newTimeUntreatCure, bool randomFlag, int noBins, int binNumber, float alpha, bool inPlaceFlag, string scenario, string jobNo, bool wideOpenFlag);

// modify facility specific parameters
void inputChangeFacility(vector<vector<string>> facilitySpecificInput, string facilitySpecific, string disease, parameters noMachines, parameters sampleWait, bool randomFlag, int noBins, int binNumber, float alpha, bool inPlaceFlag, string scenario, string jobNo, bool wideOpenFlag, int returnArray[]);

// copy facility parameters from one facility file and swap them in another facility file
void inputCopyFacility(vector<vector<string>> LastFacilitySpecificInput, vector<vector<string>> facilitySpecificInput, string facilitySpecific, string scenario, string jobNo);
      
// change test parameters
void inputChangeTest(vector<vector<string>> testInput, string test, string diseases, parameters maxReagent, parameters timeStepsPerCycle, parameters sampleWait, parameters sensitivity, parameters sensitivityPCR, parameters sensitivityXpert, parameters specificity, parameters specificityPCR, parameters specificityXpert, parameters MDSensitivity, parameters MDSpecificity, bool randomFlag, int noBins, int binNumber, float alpha, bool inPlaceFlag, int sWaitTime, string scenario, string jobNo, bool wideOpenFlag);

// update all global loss to follow up tier values
void updateGlobalTiers(float tier[], float value);

// update all regional loss to follow up tier values
void updateGlobalTegionalTiers(struct initTreatLtoFRegionTier* variable, float value);

// create a new value for that tier and then update all the region tier values
void updateGlobalTierValue(struct initTreatLtoFRegionTier* ltof, string region, int tier, float value, bool allFlag);

// initialize regional tier array
void initRegionalTier(string name, struct initTreatLtoFRegionTier* variable, float *tierValues);

// output regional tier values
void dumpTegionalTiers(struct initTreatLtoFRegionTier* variable, string header);

// get region names
string getRegionName(int regIndex);

// get the distribution value for the parameter
float getDistributionValue(struct parameters* p, float value, bool randomFlag, bool wideOpenFlag, int noBins, int binNumber, string variablName);

// get the distribution value for the parameter from array of parameter values
float getDistributionLtoFValue(struct parametersRegionTier* p, int tier, float value, bool randomFlag, bool wideOpenFlag, int noBins, int binNumber, string variablName);

// get the correct distribution for integer intervals
int getDistRandInt(string name, string distribution, float base, float topInterval, float botInterval, float alpha, float beta);

// get the correct distribution for floating point intervals
float getDistRandFloat(string name, string distribution, float base, float topInterval, float botInterval, float alpha, float beta, float scaleHB);

// generate a random number within the interval
int getRandInt(float interval, float alpha, float beta); 

// generate a random number within the interval
float getRandFloat(float interval, float alpha, float beta, float scalHB);

// calculate mean for beta distribution
float calcReshape(float originalValue, float upperBound, int lowerBound);

// calculate the value in the range for the beta distribution
float calcRandValue(float randValue, float upperBound, int lowerBound);

// calculate beta for beta distribution
float calcBeta(float alpha, float mean);

// calc variance from alpha and beta 
float calcVariance(float alpha, float beta);

// calc alpha from mean and variance 
float calcNewAlpha(float mean, float variance);

// calc beta from mean and variance 
float calcNewBeta(float mean, float variance);

// find closest matching district
string modifyName(string districtName);

// find closest matching district
string findDistrictName(string districtName, string regionDistrictArray[]);

// find district index
int findDistrictIndex(string districtName, string regionDistrictArray[]);

#endif // INPUTCHANGE_H


