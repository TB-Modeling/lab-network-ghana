//
//  InputChange.cpp
// 
//
//  Created by L. Dombrowski on 4/7/21


#include <iostream>
#include <random>
#include <sys/stat.h>
#include <errno.h>
#include <boost/math/distributions/beta.hpp>
#include <boost/math/distributions.hpp>
//#include </lib/boost/math/distributions/beta.hpp>
//#include </lib/boost/math/distributions.hpp>
//#include <beta.hpp>
//#include <distributions.hpp>
#include "InputChange.h"
#include "GlobalParameters.h"


using namespace std;

// set up range for incidence  in regions/districts and modify
void inputChangeIncidence(vector<vector<string>> regionDistrictSpecificInput, string regionDistrict, string currentDisease, parameters incidence, parameters probRepresentGeoDist, bool randomFlag, int noBins, int binNumber, float alpha, bool inPlaceFlag, string scenario, string jobNo, bool wideOpenFlag){

    int noNewData;
    //int currentJobNo = 0;
    //int randInt;
    float randFloat;
    float topInterval;
    float botInterval;
    float base;
    float interval;
    bool debug = false;
    bool debug2 = false;

    ofstream myfile;

    /*struct parameters {

        bool active;
        string distribution;
        float value;
        float mean;
        float lowerBound;
        float higerBound;
        float shape1Alpha;
        float shape2Beta;
        float scaleHB;
    };*/


    struct regionDistrictStruct {
        int ID;
        string name;
        string disease;
        int testing;
        float incidence;
        float perIncidencePresenting;
    };

    if (debug) std::cout << "input-change: inside change incidence" << "\n";

    //currentJobNo = stoi(jobNo);

    noNewData = regionDistrictSpecificInput.size();

    regionDistrictStruct* newData = new regionDistrictStruct[noNewData];

    if (debug) std::cout << "input-change: inside create aggregate file size = " << noNewData  << ", disease = " << currentDisease << ", incidence flag = " << incidence.active << "\n";

    // change incidence 
    if (incidence.active) {

        // skip the first row, it contains column headers
        // format is : ID, name, disease, testing, incidence
        for (auto rdIndex = 1u; rdIndex < regionDistrictSpecificInput.size(); rdIndex++) {

            if (debug2) {
                std::cout << regionDistrictSpecificInput[rdIndex][0] << ' ' << regionDistrictSpecificInput[rdIndex][1] << ' ' << regionDistrictSpecificInput[rdIndex][2] << ' ' << regionDistrictSpecificInput[rdIndex][4] << '\n';
            }

            newData[rdIndex-1].ID = stoi(regionDistrictSpecificInput[rdIndex][0]);
            newData[rdIndex-1].name = regionDistrictSpecificInput[rdIndex][1];
            newData[rdIndex-1].disease = regionDistrictSpecificInput[rdIndex][2];
            newData[rdIndex-1].testing = stoi(regionDistrictSpecificInput[rdIndex][3]);
            newData[rdIndex-1].incidence = stof(regionDistrictSpecificInput[rdIndex][4]);

            if (debug2) {
                std::cout << newData[rdIndex-1].ID << ' ' << newData[rdIndex-1].name << ' ' << newData[rdIndex-1].disease << ' ' << newData[rdIndex-1].testing << ' ' << newData[rdIndex-1].incidence << '\n';
            }
        }
     
        // adjust for missing header row
        noNewData = noNewData -1;

        if (debug) std::cout << "input-change: inside input change incidence  " << regionDistrict << " no of data = " << noNewData << "\n";
        
        // seed random number generator - moved inside randInt and randFloat routines
        //unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
       // std::mt19937 generator(seed);
        if (debug) std::cout << "input-change: inside input change incidence  after seed generator " << "\n";

        // now add newData to agData
        for (int i = 0; i < noNewData; i++) {
    
            // only make changes for current Disease
            if (newData[i].disease == currentDisease) {

                // incidence +- 50%
                if (newData[i].incidence  > 0) {
        
                    if (incidence.upperBound > 0) {
                    
                        topInterval = incidence.upperBound;
                        botInterval = incidence.lowerBound;
                        base = incidence.mean;
                    }
                    else if (wideOpenFlag) {
                
                        topInterval = newData[i].incidence*10;
                        botInterval = newData[i].incidence;
                        base = newData[i].incidence*5;
                    }
                    else {
                
                        topInterval = newData[i].incidence + newData[i].incidence*.5;
                        botInterval = newData[i].incidence - newData[i].incidence*.5;
                        base = newData[i].incidence;
                    }

                    interval = topInterval - botInterval;

                    if (randomFlag) {
                    
                        randFloat = getDistRandFloat( "incidence", incidence.distribution, base, topInterval, botInterval, incidence.shape1Alpha, incidence.shape2Beta, incidence.scaleHB);
                    }
                    else {

                        randFloat = interval/noBins * binNumber;
                    }
        
                    //std::cout << "input-change: inside input change incidence = " << newData[i].incidence  << " randFloat = "<< randFloat <<  "\n";
            
                    if (incidence.distribution == "Beta") {

                        newData[i].incidence = randFloat;
                    }
                    else {

                        newData[i].incidence = botInterval + randFloat;
                    }

                    //std::cout << "input-change: inside input change incidence new incidence for "<< i << ", incidence 0 = " << newData[i].incidence0 <<  "\n";
                }

            }
        }

        // open aggregate file and output values
        myfile.open("./stats/" + scenario + "/job-" + jobNo + "/input/region-district-specific-data/region-district-specific-data.csv");

        myfile << " " << ",Area - region or district" << ", disease " << ",testing " << ",incid pppd 2018 ";
        myfile << ",incid pppd 2019 " << ",incid pppd 2020 " << ",incid pppd 2021 " << ",incid pppd 2022 " << ",incid pppd 2023 " << ",incid pppd 2024 " << ",incid pppd 2025 ";
        myfile << ",incid pppd 2026 " << ",incid pppd 2027 " << "\n";

        // now add newData to agData
        for (int i = 0; i < noNewData; i++) {
        

            myfile  << newData[i].ID << "," << newData[i].name << "," << newData[i].disease << "," << newData[i].testing;
            myfile << "," << newData[i].incidence << "\n";
        }

        myfile.close();
    }

    if (probRepresentGeoDist.active) {

        // seed random number generator - moved inside randInt and randFloat routines
        //unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
        //std::mt19937 generator(seed);

        // adjust ratio +- 50%
        if (probRepresentGeoDist.upperBound > 0) {

            topInterval = probRepresentGeoDist.upperBound;
            botInterval = probRepresentGeoDist.lowerBound;
            base = probRepresentGeoDist.mean;
        }
        else if (wideOpenFlag) {

            topInterval = probRepresentGeoDist.value * 2;
            botInterval = probRepresentGeoDist.value / 2;
            base = probRepresentGeoDist.value;
        }
        else {

            topInterval = probRepresentGeoDist.value + probRepresentGeoDist.value * .5;
            botInterval = probRepresentGeoDist.value - probRepresentGeoDist.value * .5;
            base = probRepresentGeoDist.value;
        }


        interval = topInterval - botInterval;

        if (randomFlag) {

            randFloat = getDistRandFloat("probRepresentGeoDist", probRepresentGeoDist.distribution, base, topInterval, botInterval, probRepresentGeoDist.shape1Alpha, probRepresentGeoDist.shape2Beta, probRepresentGeoDist.scaleHB);
        }
        else {

            randFloat = interval / noBins * binNumber;
        }

        if (probRepresentGeoDist.distribution == "Beta") {

            pRepresentation = randFloat;
        }
        else {

            pRepresentation = botInterval + randFloat;
        }

        // debug - true
        if (debug) std::cout << "input-change: pRepresentation = " << pRepresentation << " botInterval = " << botInterval << " randFloat = " << randFloat << "\n";
    }


    if (debug) std::cout << "input-change: leaving aggregate file" <<  "\n";


}

// vary different disease parameters
void inputChangeDisease(vector<vector<string>> diseaseInput, string diseases, string currentDisease, parameters infUninfRatio, parameters perInitInf, parameters perIncPresenting, parameters initTreatSuccess, parameters initTreatDeath, parameters retreatmentPer, parameters probReturn, parameters preTestMDCDInf, parameters preTestMDCDUnInf, parameters preTestLtoF, parametersRegionTier* preTestLtoFRegion, parameters preTreatLtoF, parametersRegionTier* preTreatLtoFRegion, parameters treatLtoF, parametersRegionTier* treatLtoFRegion, parameters representDeathPer, parameters newTimeDeathRetreat, parameters newTimeLtoFRepresent, parameters newTimeUntreatCure, bool randomFlag, int noBins, int binNumber, float alpha, bool inPlaceFlag, string scenario, string jobNo, bool wideOpenFlag){

    int index = 0;
    int noNewData;
    int noNewDataDisease = 0;
    int noNewDataLtoF = 0;
    int randInt;
    string tierName; 
    float randFloat;
    float topInterval;
    float botInterval;
    float base;
    float interval;
    float relapsePer;
    float initTreatProb;
    float distValue;
    string itemName;
    //float initLtoFRatio;
    // this becomes true if it is necessary to output individual loss to follow up values at the regional level
    //bool outputRegionTierLtoF = false;
    bool debug = false;
    bool debug2 = false;

    ofstream myfile;

    struct diseaseStruct {
        string name;
        float mortality;
        float base;
        float incidence;
        string incAdjType;
        float incAdjFactor;
        float prpIncidencePresenting;
        float patientsRatioUninfInf;
        float perPatientPresentInf;
        float transmission;
        int priority;
        float moribidity;
        float preTestMDCDInfPer;
        float preTestMDCDUnInfPer;
        float preTestLtoFPer;
        float preTestLtoFPerTier[10];
        initTreatLtoFRegionTier preTestLtoFTierRegion[20];
        float preTestLtoFDeathPer;
        float space1;
        float space2;
        float probReturn;
        float perLostSamples;
        float patWithTreatAfterDiag;
        int initTreatTime;
        float initTreatPer;
        float initTreatDeathPer;
        float initTreatDeathPerIP;
        float preTreatLtoFPer;
        float preTreatLtoFPerTier[10];
        initTreatLtoFRegionTier preTreatLtoFTierRegion[20];
        float treatLtoFPer;
        float treatLtoFPerTier[10];
        initTreatLtoFRegionTier treatLtoFTierRegion[20];
        float treatLtoFPerIP;
        int treatFailTime;
        float relapseSuccessPer;
        float probNonInfTreat;
        float drugResistPer;
        int onsetToPresentTime;
        float untreatedDiePer;
        float representDeathPer;
        int timeTreatedRepresent;
        int timeDeathRetreat;
        int timeLtoFRepresent;
        int timeUntreatCure;
        float space3;
        float space4;
    };

    struct ltofStruct {

        string disease;
        initTreatLtoFRegionTier preTestLtoFTierRegion[20]; 
        initTreatLtoFRegionTier preTreatLtoFTierRegion[20];
        initTreatLtoFRegionTier treatLtoFTierRegion[20];

    };


    noNewData = diseaseInput.size();

    diseaseStruct* newData = new diseaseStruct[noNewData];
    diseaseStruct* newDataLtoF = new diseaseStruct[noNewData];
    

    if (debug) {

        std::cout << "input-change: inside change disease input = " << noNewData << ", disease = " << currentDisease << "\n";
        std::cout << "input-change-disease: newTimeDeathRetreat = " << newTimeDeathRetreat.active << ", newTimeLtoFRepresent = " << newTimeLtoFRepresent.active << ", newTimeUntreatCure = " << newTimeUntreatCure.active << "\n";
    }

    // skip the first row, it contains column headers
    // format is : disease, mortality, incidence
    // seperate disease lines that start with disease and disease-tier and put the in different arrays
    for (auto rdIndex = 1u; rdIndex < diseaseInput.size(); rdIndex++) {

        // debug2 - true
        if (debug2) {
            std::cout << diseaseInput[rdIndex][0] << ' ' << diseaseInput[rdIndex][1] << ' ' << diseaseInput[rdIndex][2] << ' ' << diseaseInput[rdIndex][4] << ' ' << diseaseInput[rdIndex][5];
            std::cout << ' ' << diseaseInput[rdIndex][6] << ' ' << diseaseInput[rdIndex][7] << ' ' << diseaseInput[rdIndex][8] << ' ' << diseaseInput[rdIndex][9];
            std::cout << ' ' << diseaseInput[rdIndex][10] << ' ' << diseaseInput[rdIndex][11] << ' ' << diseaseInput[rdIndex][12] << ' ' << diseaseInput[rdIndex][13]  << '\n';
        }

        index = 0;

        if (diseaseInput[rdIndex][index].find("-tier") < diseaseInput[rdIndex][index].length()) {

            if (debug) std::cout << "input-change: correctly found disease-tier " << diseaseInput[rdIndex][index] << "\n";
            
            // format disease-tier - stores loss to follow up tier values
            newDataLtoF[noNewDataLtoF].name = diseaseInput[rdIndex][index];
            index = index + 1;
            newDataLtoF[noNewDataLtoF].mortality = stof(diseaseInput[rdIndex][index]);
            index = index + 1;
            newDataLtoF[noNewDataLtoF].incidence = stof(diseaseInput[rdIndex][index]);
            index = index + 1;
            newDataLtoF[noNewDataLtoF].incAdjType = diseaseInput[rdIndex][index];
            index = index + 1;
            newDataLtoF[noNewDataLtoF].incAdjFactor = stof(diseaseInput[rdIndex][index]);
            index = index + 1;
            newDataLtoF[noNewDataLtoF].prpIncidencePresenting = stof(diseaseInput[rdIndex][index]);
            index = index + 1;
            newDataLtoF[noNewDataLtoF].patientsRatioUninfInf = stof(diseaseInput[rdIndex][index]);
            index = index + 1;
            newDataLtoF[noNewDataLtoF].perPatientPresentInf = stof(diseaseInput[rdIndex][index]);
            index = index + 1;
            newDataLtoF[noNewDataLtoF].transmission = stof(diseaseInput[rdIndex][index]);
            index = index + 1;
            newDataLtoF[noNewDataLtoF].priority = stoi(diseaseInput[rdIndex][index]);
            index = index + 1;
            newDataLtoF[noNewDataLtoF].moribidity = stof(diseaseInput[rdIndex][index]);
            index = index + 1;
            newDataLtoF[noNewDataLtoF].preTestMDCDInfPer = stof(diseaseInput[rdIndex][index]);
            index = index + 1;
            newDataLtoF[noNewDataLtoF].preTestMDCDUnInfPer = stof(diseaseInput[rdIndex][index]);
            index = index + 1;
            newDataLtoF[noNewDataLtoF].preTestLtoFPer = stof(diseaseInput[rdIndex][index]);
            index = index + 1;
            for (int i = 0; i < 10; i++) {
                newDataLtoF[noNewDataLtoF].preTestLtoFPerTier[i] = stof(diseaseInput[rdIndex][index]);
                index = index + 1;
            }
            newDataLtoF[noNewDataLtoF].preTestLtoFDeathPer = stof(diseaseInput[rdIndex][index]);
            index = index + 1;
            newDataLtoF[noNewDataLtoF].space1 = stof(diseaseInput[rdIndex][index]);
            index = index + 1;
            newDataLtoF[noNewDataLtoF].space2 = stof(diseaseInput[rdIndex][index]);
            index = index + 1;
            newDataLtoF[noNewDataLtoF].probReturn = stof(diseaseInput[rdIndex][index]);
            index = index + 1;
            newDataLtoF[noNewDataLtoF].perLostSamples = stof(diseaseInput[rdIndex][index]);
            index = index + 1;
            newDataLtoF[noNewDataLtoF].patWithTreatAfterDiag = stof(diseaseInput[rdIndex][index]);
            index = index + 1;
            newDataLtoF[noNewDataLtoF].initTreatTime = stoi(diseaseInput[rdIndex][index]);
            index = index + 1;
            newDataLtoF[noNewDataLtoF].initTreatPer = stof(diseaseInput[rdIndex][index]);
            index = index + 1;
            newDataLtoF[noNewDataLtoF].initTreatDeathPer = stof(diseaseInput[rdIndex][index]);
            //newDataLtoF[noNewDataLtoF].initTreatDeathPerIP = newDataLtoF[noNewDataLtoF - 1].initTreatDeathPer;
            newDataLtoF[noNewDataLtoF].initTreatDeathPerIP = newDataLtoF[noNewDataLtoF].initTreatDeathPer;
            index = index + 1;
            newDataLtoF[noNewDataLtoF].preTreatLtoFPer = stof(diseaseInput[rdIndex][index]);
            index = index + 1;
            for (int i = 0; i < 10; i++) {
                newDataLtoF[noNewDataLtoF].preTreatLtoFPerTier[i] = stof(diseaseInput[rdIndex][index]);
                index = index + 1;
            }
            newDataLtoF[noNewDataLtoF].treatLtoFPer = stof(diseaseInput[rdIndex][index]);
            index = index + 1;
            for (int i = 0; i < 10; i++) {
                newDataLtoF[noNewDataLtoF].treatLtoFPerTier[i] = stof(diseaseInput[rdIndex][index]);
                index = index + 1;
            }
            //newDataLtoF[noNewDataLtoF].treatLtoFPerIP = newDataLtoF[noNewDataLtoF - 1].treatLtoFPer; 
            newDataLtoF[noNewDataLtoF].treatLtoFPerIP = newDataLtoF[noNewDataLtoF].treatLtoFPer;
            newDataLtoF[noNewDataLtoF].treatFailTime = stoi(diseaseInput[rdIndex][index]);
            index = index + 1;
            newDataLtoF[noNewDataLtoF].relapseSuccessPer = stof(diseaseInput[rdIndex][index]);
            index = index + 1;
            newDataLtoF[noNewDataLtoF].probNonInfTreat = stof(diseaseInput[rdIndex][index]);
            index = index + 1;
            newDataLtoF[noNewDataLtoF].drugResistPer = stof(diseaseInput[rdIndex][index]);
            index = index + 1;
            newDataLtoF[noNewDataLtoF].onsetToPresentTime = stoi(diseaseInput[rdIndex][index]);
            index = index + 1;
            newDataLtoF[noNewDataLtoF].untreatedDiePer = stof(diseaseInput[rdIndex][index]);
            index = index + 1;
            newDataLtoF[noNewDataLtoF].representDeathPer = stof(diseaseInput[rdIndex][index]);
            index = index + 1;
            newDataLtoF[noNewDataLtoF].timeDeathRetreat = stoi(diseaseInput[rdIndex][index]);
            index = index + 1;
            newDataLtoF[noNewDataLtoF].timeLtoFRepresent = stoi(diseaseInput[rdIndex][index]);
            index = index + 1;
            newDataLtoF[noNewDataLtoF].timeUntreatCure = stoi(diseaseInput[rdIndex][index]);
            index = index + 1;
            newDataLtoF[noNewDataLtoF].space3 = stof(diseaseInput[rdIndex][index]);
            index = index + 1;
            newDataLtoF[noNewDataLtoF].space4 = stof(diseaseInput[rdIndex][index]);

            noNewDataLtoF++;
        }
        else {

            if (debug) std::cout << "input-change: correctly found disease " << diseaseInput[rdIndex][index] << "\n";
            
            // store disease parameters in seperate array
            newData[noNewDataDisease].name = diseaseInput[rdIndex][index];
            index = index + 1;
            newData[noNewDataDisease].mortality = stof(diseaseInput[rdIndex][index]);
            index = index + 1;
            newData[noNewDataDisease].incidence = stof(diseaseInput[rdIndex][index]);
            index = index + 1;
            newData[noNewDataDisease].incAdjType = diseaseInput[rdIndex][index];
            index = index + 1;
            newData[noNewDataDisease].incAdjFactor = stof(diseaseInput[rdIndex][index]);
            index = index + 1;
            newData[noNewDataDisease].prpIncidencePresenting = stof(diseaseInput[rdIndex][index]);
            index = index + 1;
            newData[noNewDataDisease].patientsRatioUninfInf = stof(diseaseInput[rdIndex][index]);
            index = index + 1;
            newData[noNewDataDisease].perPatientPresentInf = stof(diseaseInput[rdIndex][index]);
            index = index + 1;
            newData[noNewDataDisease].transmission = stof(diseaseInput[rdIndex][index]);
            index = index + 1;
            newData[noNewDataDisease].priority = stoi(diseaseInput[rdIndex][index]);
            index = index + 1;
            newData[noNewDataDisease].moribidity = stof(diseaseInput[rdIndex][index]);
            index = index + 1;
            newData[noNewDataDisease].preTestMDCDInfPer = stof(diseaseInput[rdIndex][index]);
            index = index + 1;
            newData[noNewDataDisease].preTestMDCDUnInfPer = stof(diseaseInput[rdIndex][index]);
            index = index + 1;
            newData[noNewDataDisease].preTestLtoFPer = stof(diseaseInput[rdIndex][index]);
            index = index + 1;
            for (int i = 0; i < 10; i++) {
                newData[noNewDataDisease].preTestLtoFPerTier[i] = stof(diseaseInput[rdIndex][index]);
                index = index + 1;
            }
            newData[noNewDataDisease].preTestLtoFDeathPer = stof(diseaseInput[rdIndex][index]);
            index = index + 1;
            newData[noNewDataDisease].space1 = stof(diseaseInput[rdIndex][index]);
            index = index + 1;
            newData[noNewDataDisease].space2 = stof(diseaseInput[rdIndex][index]);
            index = index + 1;
            newData[noNewDataDisease].probReturn = stof(diseaseInput[rdIndex][index]);
            index = index + 1;
            newData[noNewDataDisease].perLostSamples = stof(diseaseInput[rdIndex][index]);
            index = index + 1;
            newData[noNewDataDisease].patWithTreatAfterDiag = stof(diseaseInput[rdIndex][index]);
            index = index + 1;
            newData[noNewDataDisease].initTreatTime = stoi(diseaseInput[rdIndex][index]);
            index = index + 1;
            newData[noNewDataDisease].initTreatPer = stof(diseaseInput[rdIndex][index]);
            index = index + 1;
            newData[noNewDataDisease].initTreatDeathPer = stof(diseaseInput[rdIndex][index]);
            //newData[noNewDataDisease].initTreatDeathPerIP = newData[noNewDataDisease - 1].initTreatDeathPer;
            newData[noNewDataDisease].initTreatDeathPerIP = newData[noNewDataDisease].initTreatDeathPer;
            index = index + 1;
            newData[noNewDataDisease].preTreatLtoFPer = stof(diseaseInput[rdIndex][index]);
            index = index + 1;
            for (int i = 0; i < 10; i++) {
                newData[noNewDataDisease].preTreatLtoFPerTier[i] = stof(diseaseInput[rdIndex][index]);
                index = index + 1;
            }
            newData[noNewDataDisease].treatLtoFPer = stof(diseaseInput[rdIndex][index]);
            index = index + 1;
            for (int i = 0; i < 10; i++) {
                newData[noNewDataDisease].treatLtoFPerTier[i] = stof(diseaseInput[rdIndex][index]);
                index = index + 1;
            }
            //newData[noNewDataDisease].treatLtoFPerIP = newData[noNewDataDisease - 1].treatLtoFPer; 
            newData[noNewDataDisease].treatLtoFPerIP = newData[noNewDataDisease].treatLtoFPer;
            newData[noNewDataDisease].treatFailTime = stoi(diseaseInput[rdIndex][index]);
            index = index + 1;
            newData[noNewDataDisease].relapseSuccessPer = stof(diseaseInput[rdIndex][index]);
            index = index + 1;
            newData[noNewDataDisease].probNonInfTreat = stof(diseaseInput[rdIndex][index]);
            index = index + 1;
            newData[noNewDataDisease].drugResistPer = stof(diseaseInput[rdIndex][index]);
            index = index + 1;
            newData[noNewDataDisease].onsetToPresentTime = stoi(diseaseInput[rdIndex][index]);
            index = index + 1;
            newData[noNewDataDisease].untreatedDiePer = stof(diseaseInput[rdIndex][index]);
            index = index + 1;
            newData[noNewDataDisease].representDeathPer = stof(diseaseInput[rdIndex][index]);
            index = index + 1;
            newData[noNewDataDisease].timeDeathRetreat = stoi(diseaseInput[rdIndex][index]);
            index = index + 1;
            newData[noNewDataDisease].timeLtoFRepresent = stoi(diseaseInput[rdIndex][index]);
            index = index + 1;
            newData[noNewDataDisease].timeUntreatCure = stoi(diseaseInput[rdIndex][index]);
            index = index + 1;
            newData[noNewDataDisease].space3 = stof(diseaseInput[rdIndex][index]);
            index = index + 1;
            newData[noNewDataDisease].space4 = stof(diseaseInput[rdIndex][index]);

            if (debug) {

                std::cout << "input - change: after init loop disease = " << newData[noNewDataDisease].name << '\n';
                std::cout << "input - change: after init loop initTreatPer = " << newData[noNewDataDisease].initTreatPer << " treatLtoFPer = " << newData[noNewDataDisease].treatLtoFPer << '\n';
            }
            noNewDataDisease++;
        }

        if (debug2) {
            std::cout << newData[rdIndex - 1].name << ' ' << newData[rdIndex - 1].mortality << ' ' << newData[rdIndex - 1].incidence;
            std::cout << ' ' << newData[rdIndex - 1].incAdjType << ' ' << newData[rdIndex - 1].incAdjFactor;
            std::cout << ' ' << newData[rdIndex - 1].representDeathPer << ' ' << newData[rdIndex - 1].initTreatTime << '\n';
        }
    }


    // adjust for no header row
    noNewData = noNewDataDisease;

    // newData now has entries made up of diseases and disease-tier information.  The disease-tier information is used for loss to follow up 
    // values for pre-test, pre-treat and treatment loss to follow-up.  The disease-tier information is further broken down by region.
    // initialize each region diseaseLtoF tier values to the global tier values set in newData array for disease
    ltofStruct* diseaseLtoF = new ltofStruct[noNewData];

    for (int disIndex = 0; disIndex < noNewData; disIndex++) {

        // initialize all loss to follow up arrays for each disease
        diseaseLtoF[disIndex].disease = newData[disIndex].name;

        for (int regIndex = 0; regIndex < 20; regIndex++) {

            // get the region name
            diseaseLtoF[disIndex].preTestLtoFTierRegion[regIndex].regionName = regDist.regions[regIndex];
            diseaseLtoF[disIndex].preTreatLtoFTierRegion[regIndex].regionName = regDist.regions[regIndex];
            diseaseLtoF[disIndex].treatLtoFTierRegion[regIndex].regionName = regDist.regions[regIndex];

            for (int tierIndex = 0; tierIndex < 10; tierIndex++) {

                // copy the loss to follow up tiers
                diseaseLtoF[disIndex].preTestLtoFTierRegion[regIndex].tier[tierIndex] = newData[disIndex].preTestLtoFPerTier[tierIndex];
                diseaseLtoF[disIndex].preTreatLtoFTierRegion[regIndex].tier[tierIndex] = newData[disIndex].preTreatLtoFPerTier[tierIndex];
                diseaseLtoF[disIndex].treatLtoFTierRegion[regIndex].tier[tierIndex] = newData[disIndex].treatLtoFPerTier[tierIndex];
            }
        }
    }

    // debug = true
    if (debug) {

        for (int disIndex = 0; disIndex < noNewData; disIndex++) {

            std::cout << "input - change: after init diseaseLtoF disease = " << newData[disIndex].name << '\n';

            std::cout << "input - change: after init preTestLtoFPer = " << newData[disIndex].preTestLtoFPer << '\n';
            dumpTegionalTiers(diseaseLtoF[disIndex].preTestLtoFTierRegion, "after init diseaseLtoF pretestLtof");
            std::cout << "input - change: after init preTreatLtoFPer = " << newData[disIndex].preTreatLtoFPer << '\n';
            dumpTegionalTiers(diseaseLtoF[disIndex].preTreatLtoFTierRegion, "after init diseaseLtoF pretreatLtof");
            std::cout << "input - change: after init treatLtoFPer = " <<newData[disIndex].treatLtoFPer << '\n';
            dumpTegionalTiers(diseaseLtoF[disIndex].treatLtoFTierRegion, "after init diseaseLtoF treatLtof");
        }
    }

    //debug - true
    if (debug) std::cout << "input-change: inside input change disease  " << diseases << " no of data = " << noNewData << " noNewDataLtof = " << noNewDataLtoF << "\n";

    // seed random number generator - moved inside randInt and randFloat routines
    //unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    //std::mt19937 generator(seed);


    // remember initTreatPer + initTreatDeathPer + relapse/retreatemnt = 100 
    // relapse/retreatmentPer is what is leftover after adding initTreatPer + initTreatDeathPer
    // changing any of these values will cause a change in the other values
    // changing initTreatPer will cause a proportionate change in initTreatDeathPer + relapse/retreatemnt
    // changing initTreatDeathPer,  relapse/retreatemnt will cause a change in initTreatPer





    // newData holds 1 element for each row in the disease.csv file
    // newDataLtoF holds the loss to follow up tier values for 1 region for a given disease 
    // there are two types of rows
    // first column - disease = contains different disease parameters including national loss to follow up values by tier
    // first column - disease-tier = contains disease loss to follow up values by tier and empty values for other columns -
    //               except indicence.adjustment.typ contains the region that this rows loss to follow up tier values apply


    // handle first column - disease-tier - update the loss to follow up variables
    for (int i = 0; i < noNewDataLtoF; i++) {

        // first extract disease name - also subtract off 2 to account for arrays starting at 0 instead of 1 and end of line character
        tierName = newDataLtoF[i].name.substr(0, newDataLtoF[i].name.length() - newDataLtoF[i].name.find("-tier") - 2);

        // debug - true
        if (debug) std::cout << "input-change: disease =  " << currentDisease << " newDataLtoF.name = " << newDataLtoF[i].name << " no. newDataLtoF = " << noNewDataLtoF << "\n";

        // loop through diseases to find the one that you need to update those ltof tiers for
        for (int disIndex = 0; disIndex < noNewData; disIndex++) {
            
            // debug - true
            if (debug) std::cout << "inside dis loop: disease dis[i].name =  " << diseaseLtoF[disIndex].disease << " tierName = " << tierName << "\n";

            // only adjust for disease that matches &&&
            if (tierName == diseaseLtoF[disIndex].disease) {

                for (int regIndex = 0; regIndex < 20; regIndex++) {

                    // debug - true
                    if (debug) std::cout << "inside loop: disease =  " << diseaseLtoF[disIndex].disease << " regionName = " << diseaseLtoF[disIndex].preTestLtoFTierRegion[regIndex].regionName << " incAdjType = " << newDataLtoF[i].incAdjType << "\n";
                    
                    // make sure region names are all upper case
                    transform(newDataLtoF[i].incAdjType.begin(), newDataLtoF[i].incAdjType.end(), newDataLtoF[i].incAdjType.begin(), ::toupper);

                    // initialize all loss to follow up arrays for each disease
                    if (diseaseLtoF[disIndex].preTestLtoFTierRegion[regIndex].regionName == newDataLtoF[i].incAdjType) {

                        // debug - true
                        if (debug) std::cout << "inside loop: disease =  " << diseaseLtoF[disIndex].disease << " regionName = " << diseaseLtoF[disIndex].preTestLtoFTierRegion[regIndex].regionName << " incAdjType = " << newDataLtoF[i].incAdjType << "\n";

                        // store away tier values
                        for (int tierIndex = 0; tierIndex < 10; tierIndex++) {

                            // copy the loss to follow up tiers
                            diseaseLtoF[disIndex].preTestLtoFTierRegion[regIndex].tier[tierIndex] = newDataLtoF[i].preTestLtoFPerTier[tierIndex];
                            diseaseLtoF[disIndex].preTreatLtoFTierRegion[regIndex].tier[tierIndex] = newDataLtoF[i].preTreatLtoFPerTier[tierIndex];
                            diseaseLtoF[disIndex].treatLtoFTierRegion[regIndex].tier[tierIndex] = newDataLtoF[i].treatLtoFPerTier[tierIndex];
                        }
                    }
                }
            }
        }
    }

    // debug = true
    if (debug) {

        for (int disIndex = 0; disIndex < noNewData; disIndex++) {

            std::cout << "input - change: after disease-tier diseaseLtoF disease = " << newData[disIndex].name << '\n';

            dumpTegionalTiers(diseaseLtoF[disIndex].preTestLtoFTierRegion, "after disease-tier diseaseLtoF pretestLtof");
            dumpTegionalTiers(diseaseLtoF[disIndex].preTreatLtoFTierRegion, "after disease-tier diseaseLtoF pretreatLtof");
            dumpTegionalTiers(diseaseLtoF[disIndex].treatLtoFTierRegion, "after disease-tier diseaseLtoF treatLtof");
        }
    }


    // diseaseLtoF now contains the regional values for all tiers for loss to follow up
    // these can be changed by preTestLtoF at global basis for all tiers all regions
    //                         preTestLtoF.all.tier at a tier level for all regions
    //                         preTestLtoF.region.tier at a tier level for a specific region
    //                         for Ghana it can also be broken down into geographic zones, which will affect more than one region
    // this also works for preTreatLtoF and treatLtoF values


    // handle first column - disease
    for (int i = 0; i < noNewData; i++) {
    
        // debug - true
        if (debug) std::cout << "input-change: disease =  " << currentDisease << " newData.name = " << newData[i].name << " no. newData = " << noNewData << "\n";
        
        // only adjust for currentDisease
        if (newData[i].name == currentDisease) {

            if (infUninfRatio.active) {

                //distValue = getDistributionValue(&infUninfRatio, infUninfRatio.mean, randomFlag, wideOpenFlag, noBins, binNumber, "infUninfRatio");
                if (debug) std::cout << "input-change: infUninfRatio = " << distValue << "\n";

                // seed random number generator - moved inside randInt and randFloat routines
                //unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
                //std::mt19937 generator(seed);

                // adjust ratio +- 50%
                if (infUninfRatio.upperBound > 0) {

                    topInterval = infUninfRatio.upperBound;
                    botInterval = infUninfRatio.lowerBound;
                    base = infUninfRatio.mean;
                }
                else if (wideOpenFlag) {

                    topInterval = infUninfRatio.value * 2;
                    botInterval = infUninfRatio.value / 2;
                    base = infUninfRatio.value;
                }
                else {

                    topInterval = infUninfRatio.value + infUninfRatio.value * .5;
                    botInterval = infUninfRatio.value - infUninfRatio.value * .5;
                    base = infUninfRatio.value;
                }


                interval = topInterval - botInterval;

                if (randomFlag) {

                    randFloat = getDistRandFloat("infUninfRatio", infUninfRatio.distribution, base, topInterval, botInterval, infUninfRatio.shape1Alpha, infUninfRatio.shape2Beta, infUninfRatio.scaleHB);
                }
                else {

                    randFloat = interval / noBins * binNumber;
                }

                if (infUninfRatio.distribution == "Beta") {

                    newData[i].patientsRatioUninfInf = randFloat;
                }
                else {

                    newData[i].patientsRatioUninfInf = botInterval + randFloat;
                }

                if (debug) std::cout << "input-change: updated infected-uninfected ratio = " << newData[i].patientsRatioUninfInf << " botInterval = " << botInterval << " randFloat = " << randFloat << "\n";
            }


            
            if (perInitInf.active) {

                //distValue = getDistributionValue(&perInitInf, perInitInf.mean, randomFlag, wideOpenFlag, noBins, binNumber, "perInitInf");
                if (debug) std::cout << "input-change: getDistributionValue = " << distValue << "\n";

                // seed random number generator - moved inside randInt and randFloat routines
                //unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
                //std::mt19937 generator(seed);

                // adjust ratio +- 50%
                if (perInitInf.upperBound > 0) {

                    topInterval = perInitInf.upperBound;
                    botInterval = perInitInf.lowerBound;
                    base = perInitInf.mean;
                }
                else if (wideOpenFlag) {

                    topInterval = perInitInf.value * 2;
                    botInterval = perInitInf.value / 2;
                    base = perInitInf.value;
                }
                else {

                    topInterval = perInitInf.value + perInitInf.value * .5;
                    botInterval = perInitInf.value - perInitInf.value * .5;
                    base = perInitInf.value;
                }


                interval = topInterval - botInterval;

                if (randomFlag) {

                    randFloat = getDistRandFloat("perInitInf", perInitInf.distribution, base, topInterval, botInterval, perInitInf.shape1Alpha, perInitInf.shape2Beta, perInitInf.scaleHB);
                }
                else {

                    randFloat = interval / noBins * binNumber;
                }

                if (perInitInf.distribution == "Beta") {

                    newData[i].patientsRatioUninfInf = 1 / randFloat;
                }
                else {

                    newData[i].patientsRatioUninfInf = 1 / (botInterval + randFloat);
                }

                // debug - true
                if (debug) std::cout << "input-change: updated infected-uninfected ratio = " << newData[i].patientsRatioUninfInf << " botInterval = " << botInterval << " randFloat = " << randFloat << "\n";
            }


            if (perIncPresenting.active) {

                // seed random number generator - moved inside randInt and randFloat routines
                //unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
                //std::mt19937 generator(seed);

                if (debug) std::cout << "inside perIncPresenting parameter change upper bound = " << perIncPresenting.upperBound << " lower bound = " << perIncPresenting.lowerBound << " mean = " << perIncPresenting.mean << " value = " << perIncPresenting.value << "\n";

                // adjust ratio +- 50%
                if (perIncPresenting.upperBound > 0) {

                    topInterval = perIncPresenting.upperBound;
                    botInterval = perIncPresenting.lowerBound;
                    base = perIncPresenting.mean;
                }
                else if (wideOpenFlag) {

                    topInterval = perIncPresenting.value * 2;
                    botInterval = perIncPresenting.value / 2;
                    base = perIncPresenting.value;
                }
                else {

                    topInterval = perIncPresenting.value + perIncPresenting.value * .5;
                    botInterval = perIncPresenting.value - perIncPresenting.value * .5;
                    base = perIncPresenting.value;
                }

                interval = topInterval - botInterval;

                if (debug) std::cout << "inside perIncPresenting parameter change distribution = " << perIncPresenting.distribution << " shape1Alpha = " << perIncPresenting.shape1Alpha << " shape2Beta = " << perIncPresenting.shape2Beta << "\n";

                if (randomFlag) {

                    randFloat = getDistRandFloat("perIncPresenting", perIncPresenting.distribution, base, topInterval, botInterval, perIncPresenting.shape1Alpha, perIncPresenting.shape2Beta, perIncPresenting.scaleHB);
                }
                else {

                    randFloat = interval / noBins * binNumber;
                }

                if (perIncPresenting.distribution == "Beta") {

                    newData[i].prpIncidencePresenting = randFloat;
                }
                else {

                    newData[i].prpIncidencePresenting = botInterval + randFloat;
                }

                //incAdjustment = incAdjustment / perIncPresenting.mean;

                // debug - true
                if (debug) std::cout << "input-change: updated proportion incidence presenting factor = " << newData[i].prpIncidencePresenting << " botInterval = " << botInterval << " randFloat = " << randFloat << "\n";
            }


            relapsePer = 100 - newData[i].initTreatPer - newData[i].initTreatDeathPer - newData[i].treatLtoFPer;
            //initDeathRatio = newData[i].initTreatDeathPer/(100 - relapsePer - newData[i].initTreatPer);
            //initLtoFRatio = newData[i].preTreatLtoFPer/(100 - relapsePer - newData[i].initTreatPer);;



            // there are 4 possible outcomes for treatment : cure, ltof, death and relapse
            // there are 3 possible changes in input parameter values initTreatSuccess, treatLtoF, initTreatDeath
            // the percentages of the 4 treatments must add up 100
            // first check initTreatSuccess. If that changes, calculate the new initTreatSuccess.
            // subtract that amount from 100%.  If there is any percentage remaining, check treatLtoF
            // treatLtoF percentage is the percentage of the remaining value.  if treatLtoF is 33% and initTreatSuccess if 85% then treatLtoF is 1/3 of the remaining value or 5%.
            // if that needs to change, subtract it from the remaining percentage.  
            // If there is an percentage remaining after that, check initTreatDeath.
            // initTreatDeath percentage is the percentage of the remaining value.  if initTreatDeath is 90% and the remaining percentage is 10% then initTreatDeath is 90% of the remaining value or 9%.
            // If that needs to change, subtract it from the remaining percentage.
            // Anything left over is initTreatRelapse.

            // example 1:
        
            // initTreatProb = 100%

            // initTreatSuccess varies and is 90%.  That leave 10% probability available for the other 3 states
            // initTreatProb = 10%

            // treatLtoF varies and is 33 %. That leaves 6.7% probability for the other 2 states

            // initTreatDeath varies and is 90 %. That leaves a .67% probability for relapse

            // example 2:

            // initTreatProb = 100%

            // initTreatSuccess varies and is 95%.  That leave 5% probability available for the other 3 states
            // initTreatProb = 5%

            // treatLtoF varies and is 33 %.  It gets changed to 40% and the probability of 3% is left for the other two states.

            // initTreatDeath varies and is 90 %. It gets changed to 83% and that leaves a .17% probability for relapse
            
            /*Dr.Schroederand I met with DHIMS team 11-13-2023.Here are some interesting statistics they came up with for patients under treatment.

                Died 7 %

                LToF 4.1 %

                Failed .7 %

                Not Evaluated 1.3 % (patients that they had not results for – they differentiated these from LToF)

                Cured = 100 – Died – LtoF – Failed – Not Evaluated = 86.9*/


            initTreatProb = 100;

            // percentage of patients cured after treatment
            if (initTreatSuccess.active) {
         
                if (debug) std::cout << "input-change: inside input change init treat success newData[i].initTreatPer = " << newData[i].initTreatPer << " initTreatProb = " << initTreatProb << "\n";
                
                // for example if sensitivity is .7 on a range of 0-1 then take 1-.7 to get .3.  vary .7 by .3/2 in each direction to keep sensitivity in the 0-1 interval  
                if (newData[i].initTreatPer > 0) {

                    //std::cout << "input-change: inside input change init treat success newData[i].initTreatPer = " << newData[i].initTreatPer <<  "\n";
                
                    if (initTreatSuccess.upperBound > 0) {
                    
                        topInterval = initTreatSuccess.upperBound * 100;
                        botInterval = initTreatSuccess.lowerBound * 100;
                        base = initTreatSuccess.mean * 100;
                    }
                    else if (wideOpenFlag) {
                     
                        topInterval = 100;
                        botInterval = 50;
                        base = 50;
                    }
                    else {
                    
                        topInterval = newData[i].initTreatPer + (100 - newData[i].initTreatPer)/2;
                        botInterval = newData[i].initTreatPer - (100 - newData[i].initTreatPer)/2;
                        base = newData[i].initTreatPer;
                    }
            
                    interval = topInterval - botInterval;

                    // debug - true
                    if (debug) std::cout << "input-change: inside input change init treat success top interval = " << topInterval  << " botInterval "<< botInterval << ", interval = " << interval <<  "\n";
                
                    if (randomFlag) {
                    
                        randFloat = getDistRandFloat( "initTreatSuccess", initTreatSuccess.distribution, base, topInterval, botInterval, initTreatSuccess.shape1Alpha, initTreatSuccess.shape2Beta, initTreatSuccess.scaleHB);
                    }
                    else {

                        randFloat = interval/noBins * binNumber;
                        //std::cout << "input-change:  init treat success orig = " << newData[i].initTreatPer <<  ", interval = " << interval << ", randFloat = " << randFloat << ", value = " << botInterval+randFloat <<  "\n";
                    }
                
                    // debug - true
                    if (debug) std::cout << "input-change: inside input change init treat success per = " << newData[i].initTreatPer  << " randFloat "<< randFloat << ", botInterval = " << botInterval <<  "\n";
                
                    
                    if (initTreatSuccess.distribution == "Beta") {

                        newData[i].initTreatPer = randFloat*100;
                    }
                    else {

                        newData[i].initTreatPer = botInterval + randFloat;
                    }
                }
            }

            // reduce initTreatProb by initTreatPer value
            initTreatProb = initTreatProb - newData[i].initTreatPer;

            if (debug) std::cout << "input-change: inside input change after init treat success newData[i].initTreatPer = " << newData[i].initTreatPer << " initTreatProb = " << initTreatProb << "\n";

            // debug - true
            if (debug) dumpTegionalTiers(newData[i].treatLtoFTierRegion, "before regions treatLtof");

                
            if (treatLtoF.active) {

                if (debug) std::cout << "input-change: inside input change init death per = " << newData[i].treatLtoFPer << " lowerBound " << treatLtoF.lowerBound << ", upperBound = " << treatLtoF.upperBound << "\n";
                if (debug) std::cout << "input-change: inside input change init treat ltof newData[i].treatLtoFPer = " << newData[i].treatLtoFPer << " initTreatProb = " << initTreatProb << "\n";

                // loss to follow up percentage +- 50%
                if (treatLtoF.upperBound > 0) {

                    topInterval = treatLtoF.upperBound * 100;
                    botInterval = treatLtoF.lowerBound * 100;
                    base = treatLtoF.mean * 100;
                }
                else if (wideOpenFlag) {

                    topInterval = newData[i].treatLtoFPer + newData[i].treatLtoFPer;
                    botInterval = newData[i].treatLtoFPer - newData[i].treatLtoFPer;
                    base = newData[i].treatLtoFPer;
                }
                else {

                    topInterval = newData[i].treatLtoFPer + newData[i].treatLtoFPer / 2;
                    botInterval = newData[i].treatLtoFPer - newData[i].treatLtoFPer / 2;
                    base = newData[i].treatLtoFPer;
                }

                interval = topInterval - botInterval;

                if (interval > 0) {

                    if (randomFlag) {

                        randFloat = getDistRandFloat("treatLtoF", treatLtoF.distribution, base, topInterval, botInterval, treatLtoF.shape1Alpha, treatLtoF.shape2Beta, treatLtoF.scaleHB);
                    }
                    else {

                        randFloat = interval / noBins * binNumber;
                        //std::cout << "input-change:  init ltof orig = " << newData[i].treatLtoF <<  ", interval = " << interval << ", randFloat = " << randFloat << ", value = " << botInterval+randInt <<  "\n";
                    }

                    if (debug) std::cout << "input-change: inside input change init death per = " << newData[i].treatLtoFPer << " randFloat " << randFloat << ", botInterval = " << botInterval << "\n";       

                    if (treatLtoF.distribution == "Beta") {

                        newData[i].treatLtoFPer = initTreatProb * randFloat;
                        newData[i].treatLtoFPerIP = randFloat * 100;
                    }
                    else {

                        newData[i].treatLtoFPer = initTreatProb * (botInterval + randFloat) / 100;
                        newData[i].treatLtoFPerIP = (botInterval + randFloat);
                    }
                }
                else {

                    newData[i].treatLtoFPer = initTreatProb * (newData[i].treatLtoFPer / 100);

                    newData[i].treatLtoFPerIP = (newData[i].treatLtoFPer);
                }
                initTreatProb = initTreatProb - newData[i].treatLtoFPer;

                // this represents a global change for all the ptreatLtoF tiers so update tiers
                updateGlobalTiers(newData[i].treatLtoFPerTier, newData[i].treatLtoFPer);

                updateGlobalTegionalTiers(diseaseLtoF[i].treatLtoFTierRegion, newData[i].treatLtoFPer);

                updateGlobalTegionalTiers(newData[i].treatLtoFTierRegion, newData[i].treatLtoFPer);
            }
            else {

                // reduce initTreatProb by default value
                newData[i].treatLtoFPer = initTreatProb * (newData[i].treatLtoFPer/100);
                newData[i].treatLtoFPerIP = newData[i].treatLtoFPer;
                initTreatProb = initTreatProb - newData[i].treatLtoFPer;

                // this represents a global change for all the treatLtoF tiers so update tiers
                updateGlobalTiers(newData[i].treatLtoFPerTier, newData[i].treatLtoFPer);

                updateGlobalTegionalTiers(diseaseLtoF[i].treatLtoFTierRegion, newData[i].treatLtoFPer);

                updateGlobalTegionalTiers(newData[i].treatLtoFTierRegion, newData[i].treatLtoFPer);
            }
          
            //&&&&
            // treatLtoF.   (all, norther, middlebelt, southern or region)
            for (int disIndex = 0; disIndex < noNewData; disIndex++) {

                // only update loss to follow up values for current disease
                if (currentDisease == diseaseLtoF[disIndex].disease) {

                    for (int regIndex = 0; regIndex < 20; regIndex++) {

                        if (debug) std::cout << "found TreatLtoF. region = " << (treatLtoFRegion + regIndex)->region << "\n";

                        if ((treatLtoFRegion + regIndex)->region == "all") {

                            if (debug) std::cout << "found PreTreatLtoF.all = " << (treatLtoFRegion + regIndex)->region << " active - 3 = " << (treatLtoFRegion + regIndex)->active[2] << ", 4 = " << (treatLtoFRegion + regIndex)->active[3] << ", 5 = " << (treatLtoFRegion + regIndex)->active[4] << ", 6 = " << (treatLtoFRegion + regIndex)->active[5] << ", 7 = " << (treatLtoFRegion + regIndex)->active[6] << ", 8 = " << (treatLtoFRegion + regIndex)->active[7] << "\n";

                            // check active flags for each tier.
                            for (int actIndex = 0; actIndex < 10; actIndex++) {

                                if ((treatLtoFRegion + regIndex)->active[actIndex] == 1) {

                                    // get the distribution value for the active parameter from array of parameter values
                                    distValue = getDistributionLtoFValue((treatLtoFRegion + regIndex), actIndex, newData[i].treatLtoFPerTier[actIndex], randomFlag, wideOpenFlag, noBins, binNumber, "treatLtoF");

                                    // create a new value for that tier and then update all the region tier values
                                    updateGlobalTierValue(diseaseLtoF[disIndex].treatLtoFTierRegion, "all", actIndex, distValue, true);
                                }
                            }
                        }
                        else if ((treatLtoFRegion + regIndex)->region == "northern") {

                            if (debug) std::cout << "found PreTreatLtoF.region = " << (treatLtoFRegion + regIndex)->region << " active - 3 = " << (treatLtoFRegion + regIndex)->active[2] << ",4 = " << (treatLtoFRegion + regIndex)->active[3] << ", 5 = " << (treatLtoFRegion + regIndex)->active[4] << ", 6 = " << (treatLtoFRegion + regIndex)->active[5] << ", 7 = " << (treatLtoFRegion + regIndex)->active[6] << ", 8 = " << (treatLtoFRegion + regIndex)->active[7] << "\n";

                            //check active flags for each tier.
                            for (int actIndex = 0; actIndex < 10; actIndex++) {

                                if ((treatLtoFRegion + regIndex)->active[actIndex] == 1) {

                                    // get the distribution value for the active parameter from array of parameter values
                                    distValue = getDistributionLtoFValue((treatLtoFRegion + regIndex), actIndex, newData[i].treatLtoFPerTier[actIndex], randomFlag, wideOpenFlag, noBins, binNumber, "treatLtoF");

                                    // create a new value for that tier and then update all the region tier values
                                    updateGlobalTierValue(diseaseLtoF[disIndex].treatLtoFTierRegion, "NORTHERN", actIndex, distValue, false);
                                    updateGlobalTierValue(diseaseLtoF[disIndex].treatLtoFTierRegion, "NORTH EAST", actIndex, distValue, false);
                                    updateGlobalTierValue(diseaseLtoF[disIndex].treatLtoFTierRegion, "SAVANNAH", actIndex, distValue, false);
                                    updateGlobalTierValue(diseaseLtoF[disIndex].treatLtoFTierRegion, "UPPER EAST", actIndex, distValue, false);
                                    updateGlobalTierValue(diseaseLtoF[disIndex].treatLtoFTierRegion, "UPPER WEST", actIndex, distValue, false);
                                }
                            }
                        }
                        else if ((treatLtoFRegion + regIndex)->region == "middlebelt") {

                            if (debug) std::cout << "found PreTreatLtoF.region = " << (treatLtoFRegion + regIndex)->region << " active - 3 = " << (treatLtoFRegion + regIndex)->active[2] << ", 4 = " << (treatLtoFRegion + regIndex)->active[3] << ", 5 = " << (treatLtoFRegion + regIndex)->active[4] << ", 6 = " << (treatLtoFRegion + regIndex)->active[5] << ", 7 = " << (treatLtoFRegion + regIndex)->active[6] << ", 8 = " << (treatLtoFRegion + regIndex)->active[7] << "\n";

                            //check active flags for each tier.
                            for (int actIndex = 0; actIndex < 10; actIndex++) {

                                if ((treatLtoFRegion + regIndex)->active[actIndex] == 1) {

                                    // get the distribution value for the active parameter from array of parameter values
                                    distValue = getDistributionLtoFValue((treatLtoFRegion + regIndex), actIndex, newData[i].treatLtoFPerTier[actIndex], randomFlag, wideOpenFlag, noBins, binNumber, "treatLtoF");

                                    // create a new value for that tier and then update all the region tier values
                                    updateGlobalTierValue(diseaseLtoF[disIndex].treatLtoFTierRegion, "AHAFO", actIndex, distValue, false);
                                    updateGlobalTierValue(diseaseLtoF[disIndex].treatLtoFTierRegion, "ASHANTI", actIndex, distValue, false);
                                    updateGlobalTierValue(diseaseLtoF[disIndex].treatLtoFTierRegion, "BONO", actIndex, distValue, false);
                                    updateGlobalTierValue(diseaseLtoF[disIndex].treatLtoFTierRegion, "BONO EAST", actIndex, distValue, false);
                                    updateGlobalTierValue(diseaseLtoF[disIndex].treatLtoFTierRegion, "EASTERN", actIndex, distValue, false);
                                }
                            }
                        }
                        else if ((treatLtoFRegion + regIndex)->region == "southern") {

                            if (debug) std::cout << "found PreTreatLtoF.region = " << (treatLtoFRegion + regIndex)->region << " active - 3 = " << (treatLtoFRegion + regIndex)->active[2] << ", 4 = " << (treatLtoFRegion + regIndex)->active[3] << ", 5 = " << (treatLtoFRegion + regIndex)->active[4] << ", 6 = " << (treatLtoFRegion + regIndex)->active[5] << ", 7 = " << (treatLtoFRegion + regIndex)->active[6] << ", 8 = " << (treatLtoFRegion + regIndex)->active[7] << "\n";

                            //check active flags for each tier.
                            for (int actIndex = 0; actIndex < 10; actIndex++) {

                                if ((treatLtoFRegion + regIndex)->active[actIndex] == 1) {

                                    // get the distribution value for the active parameter from array of parameter values
                                    distValue = getDistributionLtoFValue((treatLtoFRegion + regIndex), actIndex, newData[i].treatLtoFPerTier[actIndex], randomFlag, wideOpenFlag, noBins, binNumber, "treatLtoF");

                                    // create a new value for that tier and then update all the region tier values
                                    updateGlobalTierValue(diseaseLtoF[disIndex].treatLtoFTierRegion, "CENTRAL", actIndex, distValue, false);
                                    updateGlobalTierValue(diseaseLtoF[disIndex].treatLtoFTierRegion, "GREATER ACCRA", actIndex, distValue, false);
                                    updateGlobalTierValue(diseaseLtoF[disIndex].treatLtoFTierRegion, "OTI", actIndex, distValue, false);
                                    updateGlobalTierValue(diseaseLtoF[disIndex].treatLtoFTierRegion, "VOLTA", actIndex, distValue, false);
                                    updateGlobalTierValue(diseaseLtoF[disIndex].treatLtoFTierRegion, "WESTERN", actIndex, distValue, false);
                                    updateGlobalTierValue(diseaseLtoF[disIndex].treatLtoFTierRegion, "WESTERN NORTH", actIndex, distValue, false);
                                }
                            }
                        }
                        else {

                            if (debug) std::cout << "found TreatLtoF.region = " << (treatLtoFRegion + regIndex)->region << " active - 3 = " << (treatLtoFRegion + regIndex)->active[2] << ", 4 = " << (treatLtoFRegion + regIndex)->active[3] << ", 5 = " << (treatLtoFRegion + regIndex)->active[4] << ", 6 = " << (treatLtoFRegion + regIndex)->active[5] << ", 7 = " << (treatLtoFRegion + regIndex)->active[6] << "\n";

                            if ((treatLtoFRegion + regIndex)->region == "OTI") {

                                //check active flags for each tier.
                                for (int actIndex = 0; actIndex < 10; actIndex++) {

                                    if ((treatLtoFRegion + regIndex)->active[actIndex] == 1) {

                                        // get the distribution value for the active parameter from array of parameter values
                                        distValue = getDistributionLtoFValue((treatLtoFRegion + regIndex), actIndex, newData[i].preTreatLtoFPerTier[actIndex], randomFlag, wideOpenFlag, noBins, binNumber, "treatLtoF");

                                        // create a new value for that tier and then update all the region tier values
                                        updateGlobalTierValue(diseaseLtoF[disIndex].treatLtoFTierRegion, (treatLtoFRegion + regIndex)->region, actIndex, distValue, false);
                                    }
                                }
                            }
                        }
                    }
                }
            }
            // debug = true
            if (debug) {

                for (int disIndex = 0; disIndex < noNewData; disIndex++) {

                    std::cout << "input - change: after treatLtoF. = " << newData[disIndex].name << '\n';

                    dumpTegionalTiers(diseaseLtoF[disIndex].treatLtoFTierRegion, "after treatLtoF.");
                }
            }

            if (debug) std::cout << "input-change: inside input change after init treat ltof newData[i].treatLtoFPer = " << newData[i].treatLtoFPer << " initTreatProb = " << initTreatProb << "\n";
            
            //Percentage of patients that die during treatment
            if (initTreatDeath.active) {

                if (debug) std::cout << "input-change: inside input change init treat death newData[i].initTreatDeathPer = " << newData[i].initTreatDeathPer << " initTreatProb = " << initTreatProb << "\n";
                
                // death rate +- 50 %
                if (initTreatDeath.upperBound > 0) {
                    
                    topInterval = initTreatDeath.upperBound * 100;
                    botInterval = initTreatDeath.lowerBound * 100;
                    base = initTreatDeath.mean * 100;
                }
                else if (wideOpenFlag) {
                     
                    topInterval = newData[i].initTreatDeathPer + newData[i].initTreatDeathPer;
                    botInterval = newData[i].initTreatDeathPer - newData[i].initTreatDeathPer;
                    base = newData[i].initTreatDeathPer;
                }
                else {
                
                    topInterval = newData[i].initTreatDeathPer + newData[i].initTreatDeathPer/2;
                    botInterval = newData[i].initTreatDeathPer - newData[i].initTreatDeathPer/2;
                    base = newData[i].initTreatDeathPer;
                }

                
                interval = topInterval - botInterval;

                if (debug) std::cout << "calculated alpha = " << initTreatDeath.shape1Alpha <<  ", topInterval = " << topInterval << ", botInterval = " << botInterval <<  "\n";


                if (interval > 0) {
            
                    if (randomFlag) {
                    
                        randFloat = getDistRandFloat( "initTreatDeath", initTreatDeath.distribution, base, topInterval, botInterval, initTreatDeath.shape1Alpha, initTreatDeath.shape2Beta, initTreatDeath.scaleHB);

                        //std::cout << "randFloat = " << randFloat <<  "\n";
                    }
                    else {

                        randFloat = interval/noBins * binNumber;
                        //std::cout << "input-change:  init death treat orig = " << newData[i].initTreatDeathPer <<  ", interval = " << interval << ", randFloat = " << ", value = " << botInterval+randFloat << randFloat <<  "\n";
                    }
            
                    if (debug) std::cout << "input-change: inside input change pre test death per = " << newData[i].initTreatDeathPer  << " randFloat "<< randFloat << ", botInterval = " << botInterval <<  "\n";

                    if (initTreatDeath.distribution == "Beta") {

                        newData[i].initTreatDeathPer = initTreatProb * randFloat;
                        newData[i].initTreatDeathPerIP = randFloat * 100;
                    }
                    else {

                        newData[i].initTreatDeathPer = initTreatProb * (botInterval + randFloat) / 100;
                        newData[i].initTreatDeathPerIP = (botInterval + randFloat);
                    }
                }
                else {

                    newData[i].initTreatDeathPer = initTreatProb * (newData[i].initTreatDeathPer/100);

                    newData[i].initTreatDeathPerIP = newData[i].initTreatDeathPer;
                }

                initTreatProb = initTreatProb - newData[i].initTreatDeathPer;
            }
            else {

                // reduce initTreatProb by default value
                newData[i].initTreatDeathPer = initTreatProb * (newData[i].initTreatDeathPer/100);
                newData[i].initTreatDeathPerIP = newData[i].initTreatDeathPer;
                initTreatProb = initTreatProb - newData[i].initTreatDeathPer;
            }
            
            
            if (debug) std::cout << "input-change: inside input change after init treat death newData[i].initTreatDeathPer = " << newData[i].initTreatDeathPer << " initTreatProb = " << initTreatProb << "\n";

            // if anythng left over it is the percentage of patients going to relapse therapy
            // if not, adjust porportionally
            if (initTreatProb > 0) {

                relapsePer = initTreatProb;
            }
            else {

                relapsePer = 0;
            }


            // debug - true
            if (debug) dumpTegionalTiers(newData[i].preTreatLtoFTierRegion, "before regions preTreatLtof");


            if (preTreatLtoF.active) {
            
                // loss to follow up percentage +- 50%
                if (preTreatLtoF.upperBound > 0) {
                    
                    topInterval = preTreatLtoF.upperBound * 100;
                    botInterval = preTreatLtoF.lowerBound * 100;
                    base = preTreatLtoF.mean * 100;
                }
                else if (wideOpenFlag) {
                  
                    topInterval = newData[i].preTreatLtoFPer + newData[i].preTreatLtoFPer;
                    botInterval = newData[i].preTreatLtoFPer - newData[i].preTreatLtoFPer;
                    base = newData[i].preTreatLtoFPer;
                }
                else {
                
                    topInterval = newData[i].preTreatLtoFPer + newData[i].preTreatLtoFPer/2;
                    botInterval = newData[i].preTreatLtoFPer - newData[i].preTreatLtoFPer/2;
                    base = newData[i].preTreatLtoFPer;
                }

                interval = topInterval - botInterval;

                if (interval > 0) {

                    if (randomFlag) {

                        randFloat = getDistRandFloat("preTreatLtoF", preTreatLtoF.distribution, base, topInterval, botInterval, preTreatLtoF.shape1Alpha, preTreatLtoF.shape2Beta, preTreatLtoF.scaleHB);
                    }
                    else {

                        randFloat = interval / noBins * binNumber;
                        //std::cout << "input-change:  init ltof orig = " << newData[i].preTreatLtoF <<  ", interval = " << interval << ", randFloat = " << randFloat << ", value = " << botInterval+randInt <<  "\n";
                    }

                    if (debug) std::cout << "input-change: inside input change pre treat ltoF per = " << newData[i].preTreatLtoFPer << " randFloat " << randFloat << ", botInterval = " << botInterval << "\n";

                    if (preTreatLtoF.distribution == "Beta") {

                        newData[i].preTreatLtoFPer = randFloat * 100;
                    }
                    else {

                        newData[i].preTreatLtoFPer = botInterval + randFloat;
                    }

                    // this represents a global change for all the preTreatLtoF tiers so update tiers
                    updateGlobalTiers(newData[i].preTreatLtoFPerTier, newData[i].preTreatLtoFPer);

                    updateGlobalTegionalTiers(diseaseLtoF[i].preTreatLtoFTierRegion, newData[i].preTreatLtoFPer);

                    updateGlobalTegionalTiers(newData[i].preTreatLtoFTierRegion, newData[i].preTreatLtoFPer);
                }
            }

            //&&&
            
            // preTreatLtoF.   (all, norther, middlebelt, southern or region) &&&&
            for (int disIndex = 0; disIndex < noNewData; disIndex++) {

                // only update loss to follow up values for current disease
                if (currentDisease == diseaseLtoF[disIndex].disease) {

                    for (int regIndex = 0; regIndex < 20; regIndex++) {

                        if (debug) std::cout << "found PreTreatLtoF. region = " << (preTreatLtoFRegion + regIndex)->region << "\n";
                        
                        if ((preTreatLtoFRegion + regIndex)->region == "all") {

                            if (debug) std::cout << "found PreTreatLtoF.all = " << (preTreatLtoFRegion + regIndex)->region << " active - 3 = " << (preTreatLtoFRegion + regIndex)->active[2] << ", 4 = " << (preTreatLtoFRegion + regIndex)->active[3] << ", 5 = " << (preTreatLtoFRegion + regIndex)->active[4] << ", 6 = " << (preTreatLtoFRegion + regIndex)->active[5] << ", 7 = " << (preTreatLtoFRegion + regIndex)->active[6] << ", 8 = " << (preTreatLtoFRegion + regIndex)->active[7] << "\n";

                            // check active flags for each tier.
                            for (int actIndex = 0; actIndex < 10; actIndex++) {

                                if ((preTreatLtoFRegion + regIndex)->active[actIndex] == 1) {

                                    // get the distribution value for the active parameter from array of parameter values
                                    distValue = getDistributionLtoFValue((preTreatLtoFRegion + regIndex), actIndex, newData[i].preTreatLtoFPerTier[actIndex], randomFlag, wideOpenFlag, noBins, binNumber, "preTreatLtoF");

                                    // create a new value for that tier and then update all the region tier values
                                    updateGlobalTierValue(diseaseLtoF[disIndex].preTreatLtoFTierRegion, "all", actIndex, distValue, true);
                                }
                            }
                        }
                        else if ((preTreatLtoFRegion + regIndex)->region == "northern") {

                            if (debug) std::cout << "found PreTreatLtoF.region = " << (preTreatLtoFRegion + regIndex)->region << " active - 3 = " << (preTreatLtoFRegion + regIndex)->active[2] << ",4 = " << (preTreatLtoFRegion + regIndex)->active[3] << ", 5 = " << (preTreatLtoFRegion + regIndex)->active[4] << ", 6 = " << (preTreatLtoFRegion + regIndex)->active[5] << ", 7 = " << (preTreatLtoFRegion + regIndex)->active[6] << ", 8 = " << (preTreatLtoFRegion + regIndex)->active[7] << "\n";

                            //check active flags for each tier.
                            for (int actIndex = 0; actIndex < 10; actIndex++) {

                                if ((preTreatLtoFRegion + regIndex)->active[actIndex] == 1) {

                                    // get the distribution value for the active parameter from array of parameter values
                                    distValue = getDistributionLtoFValue((preTreatLtoFRegion + regIndex), actIndex, newData[i].preTreatLtoFPerTier[actIndex], randomFlag, wideOpenFlag, noBins, binNumber, "preTreatLtoF");

                                    // create a new value for that tier and then update all the region tier values
                                    updateGlobalTierValue(diseaseLtoF[disIndex].preTreatLtoFTierRegion, "NORTHERN", actIndex, distValue, false);
                                    updateGlobalTierValue(diseaseLtoF[disIndex].preTreatLtoFTierRegion, "NORTH EAST", actIndex, distValue, false);
                                    updateGlobalTierValue(diseaseLtoF[disIndex].preTreatLtoFTierRegion, "SAVANNAH", actIndex, distValue, false);
                                    updateGlobalTierValue(diseaseLtoF[disIndex].preTreatLtoFTierRegion, "UPPER EAST", actIndex, distValue, false);
                                    updateGlobalTierValue(diseaseLtoF[disIndex].preTreatLtoFTierRegion, "UPPER WEST", actIndex, distValue, false);
                                }
                            }
                        }
                        else if ((preTreatLtoFRegion + regIndex)->region == "middlebelt") {

                            if (debug) std::cout << "found PreTreatLtoF.region = " << (preTreatLtoFRegion + regIndex)->region << " active - 3 = " << (preTreatLtoFRegion + regIndex)->active[2] << ", 4 = " << (preTreatLtoFRegion + regIndex)->active[3] << ", 5 = " << (preTreatLtoFRegion + regIndex)->active[4] << ", 6 = " << (preTreatLtoFRegion + regIndex)->active[5] << ", 7 = " << (preTreatLtoFRegion + regIndex)->active[6] << ", 8 = " << (preTreatLtoFRegion + regIndex)->active[7] << "\n";

                            //check active flags for each tier.
                            for (int actIndex = 0; actIndex < 10; actIndex++) {

                                if ((preTreatLtoFRegion + regIndex)->active[actIndex] == 1) {

                                    // get the distribution value for the active parameter from array of parameter values
                                    distValue = getDistributionLtoFValue((preTreatLtoFRegion + regIndex), actIndex, newData[i].preTreatLtoFPerTier[actIndex], randomFlag, wideOpenFlag, noBins, binNumber, "preTreatLtoF");

                                    // create a new value for that tier and then update all the region tier values
                                    updateGlobalTierValue(diseaseLtoF[disIndex].preTreatLtoFTierRegion, "AHAFO", actIndex, distValue, false);
                                    updateGlobalTierValue(diseaseLtoF[disIndex].preTreatLtoFTierRegion, "ASHANTI", actIndex, distValue, false);
                                    updateGlobalTierValue(diseaseLtoF[disIndex].preTreatLtoFTierRegion, "BONO", actIndex, distValue, false);
                                    updateGlobalTierValue(diseaseLtoF[disIndex].preTreatLtoFTierRegion, "BONO EAST", actIndex, distValue, false);
                                    updateGlobalTierValue(diseaseLtoF[disIndex].preTreatLtoFTierRegion, "EASTERN", actIndex, distValue, false);
                                }
                            }
                        }
                        else if ((preTreatLtoFRegion + regIndex)->region == "southern") {

                            if (debug) std::cout << "found PreTreatLtoF.region = " << (preTreatLtoFRegion + regIndex)->region << " active - 3 = " << (preTreatLtoFRegion + regIndex)->active[2] << ", 4 = " << (preTreatLtoFRegion + regIndex)->active[3] << ", 5 = " << (preTreatLtoFRegion + regIndex)->active[4] << ", 6 = " << (preTreatLtoFRegion + regIndex)->active[5] << ", 7 = " << (preTreatLtoFRegion + regIndex)->active[6] << ", 8 = " << (preTreatLtoFRegion + regIndex)->active[7] << "\n";

                            //check active flags for each tier.
                            for (int actIndex = 0; actIndex < 10; actIndex++) {

                                if ((preTreatLtoFRegion + regIndex)->active[actIndex] == 1) {

                                    // get the distribution value for the active parameter from array of parameter values
                                    distValue = getDistributionLtoFValue((preTreatLtoFRegion + regIndex), actIndex, newData[i].preTreatLtoFPerTier[actIndex], randomFlag, wideOpenFlag, noBins, binNumber, "preTreatLtoF");

                                    // create a new value for that tier and then update all the region tier values
                                    updateGlobalTierValue(diseaseLtoF[disIndex].preTreatLtoFTierRegion, "CENTRAL", actIndex, distValue, false);
                                    updateGlobalTierValue(diseaseLtoF[disIndex].preTreatLtoFTierRegion, "GREATER ACCRA", actIndex, distValue, false);
                                    updateGlobalTierValue(diseaseLtoF[disIndex].preTreatLtoFTierRegion, "OTI", actIndex, distValue, false);
                                    updateGlobalTierValue(diseaseLtoF[disIndex].preTreatLtoFTierRegion, "VOLTA", actIndex, distValue, false);
                                    updateGlobalTierValue(diseaseLtoF[disIndex].preTreatLtoFTierRegion, "WESTERN", actIndex, distValue, false);
                                    updateGlobalTierValue(diseaseLtoF[disIndex].preTreatLtoFTierRegion, "WESTERN NORTH", actIndex, distValue, false);
                                }
                            }
                        }
                        else {

                            if (debug) std::cout << "found PreTreatLtoF.region = " << (preTreatLtoFRegion + regIndex)->region << " active - 3 = " << (preTreatLtoFRegion + regIndex)->active[2] << ", 4 = " << (preTreatLtoFRegion + regIndex)->active[3] << ", 5 = " << (preTreatLtoFRegion + regIndex)->active[4] << ", 6 = " << (preTreatLtoFRegion + regIndex)->active[5] << ", 7 = " << (preTreatLtoFRegion + regIndex)->active[6] << "\n";

                            if ((preTreatLtoFRegion + regIndex)->region == "OTI") {

                                //check active flags for each tier.
                                for (int actIndex = 0; actIndex < 10; actIndex++) {

                                    if ((preTreatLtoFRegion + regIndex)->active[actIndex] == 1) {

                                        // get the distribution value for the active parameter from array of parameter values
                                        distValue = getDistributionLtoFValue((preTreatLtoFRegion + regIndex), actIndex, newData[i].preTreatLtoFPerTier[actIndex], randomFlag, wideOpenFlag, noBins, binNumber, "preTreatLtoF");

                                        // create a new value for that tier and then update all the region tier values
                                        updateGlobalTierValue(diseaseLtoF[disIndex].preTreatLtoFTierRegion, (preTreatLtoFRegion + regIndex)->region, actIndex, distValue, false);
                                    }
                                }
                            }
                        }
                    }
                }
            }
            // debug = true
            if (debug) {

                for (int disIndex = 0; disIndex < noNewData; disIndex++) {

                    std::cout << "input - change: after preTreatLtoF. = " << newData[disIndex].name << '\n';

                    dumpTegionalTiers(diseaseLtoF[disIndex].preTreatLtoFTierRegion, "after preTreatLtoF.");
                }
            }


            // relapse success percentage 
            if (retreatmentPer.active) {

            
                // for example if relapse success rate is .7 on a range of 0-1 then take 1-.7 to get .3.  vary .7 by .3/2 in each direction to keep relapse percentage in the 0-1 interval    
                if (newData[i].relapseSuccessPer > 0) {

                    if (retreatmentPer.upperBound > 0) {
                    
                        topInterval = retreatmentPer.upperBound * 100;
                        botInterval = retreatmentPer.lowerBound * 100;
                        base = retreatmentPer.mean * 100;
                    }
                    else if (wideOpenFlag) {
                    
                        topInterval = 100;
                        botInterval = newData[i].relapseSuccessPer - (100 - newData[i].relapseSuccessPer);
                        base = newData[i].relapseSuccessPer;
                    }
                    else {
                    
                        topInterval = newData[i].relapseSuccessPer + (100 - newData[i].relapseSuccessPer)/2;
                        botInterval = newData[i].relapseSuccessPer - (100 - newData[i].relapseSuccessPer)/2;
                        base = newData[i].relapseSuccessPer;
                    }
            
                    interval = topInterval - botInterval;

                    if (randomFlag) {
                    
                        randFloat = getDistRandFloat("retreatmentPer", retreatmentPer.distribution, base, topInterval, botInterval, retreatmentPer.shape1Alpha, retreatmentPer.shape2Beta, retreatmentPer.scaleHB);
                    }
                    else {

                        randFloat = interval/noBins * binNumber;
                        //std::cout << "input-change:  init relapse orig = " << newData[i].relapseSuccessPer <<  ", interval = " << interval << ", randFloat = " << randFloat << ", value = " << botInterval+randInt << "\n";
                    }

                    if (debug) std::cout << "input-change: inside input change relapse success per = " << newData[i].relapseSuccessPer  << " randFloat "<< randFloat << ", botInterval = " << botInterval <<  "\n";
                
                    if (retreatmentPer.distribution == "Beta") {

                        newData[i].relapseSuccessPer = randFloat * 100;
                    }
                    else {

                        newData[i].relapseSuccessPer = botInterval + randFloat;
                    }
                }
            }


            // probability patient returns percentage 
            if (probReturn.active) {
         
                // for example if probability of patient return rate is .7 on a range of 0-1 then take 1-.7 to get .3.  vary .7 by .3/2 in each direction to keep relapse percentage in the 0-1 interval    
                if (newData[i].probReturn > 0) {

                    if (probReturn.upperBound > 0) {
                    
                        topInterval = probReturn.upperBound * 100;
                        botInterval = probReturn.lowerBound * 100;
                        base = probReturn.mean * 100;
                    }
                    else if (wideOpenFlag) {

                        topInterval = 100;
                        botInterval = 0;
                        base = 50;
                    }
                    else {

                        topInterval = newData[i].probReturn + (100 - newData[i].probReturn)/2;
                        botInterval = newData[i].probReturn - (100 - newData[i].probReturn)/2;
                        base = newData[i].probReturn;
                    }
            
                    interval = topInterval - botInterval;

                    if (randomFlag) {
                    
                        randFloat = getDistRandFloat("probReturn", probReturn.distribution, base, topInterval, botInterval, probReturn.shape1Alpha, probReturn.shape2Beta, probReturn.scaleHB);
                    }
                    else {

                        randFloat = interval/noBins * binNumber;
                        //std::cout << "input-change:  init probReturn orig = " << newData[i].probReturn <<  ", interval = " << interval << ", randFloat = " << randFloat << ", value = " << botInterval+randInt << "\n";
                    }

                    if (debug) std::cout << "input-change: inside input change probility patient return = " << newData[i].probReturn  << " randFloat "<< randFloat << ", botInterval = " << botInterval <<  "\n";
                
                    if (probReturn.distribution == "Beta") {

                        newData[i].probReturn = randFloat * 100;
                    }
                    else {

                        newData[i].probReturn = botInterval + randFloat;
                    }
                }
            }

            // preTestLtoF, preTreatLtoF and treatLtoF have values for each tier in each region. The format for storage for each disease is storage for 10 tiers for each region in the country
            // This section of code deals with the 3 possible cases of modifying those values:
            // 1 - a global value is set and this value is translated into all the loss to follow up variables for every tier in every region
            // 2 - global tier by tier values are set and the same for all the tiers in each region
            // 3 - individual tier values for each region are set
            // no. 1 is controlled with the PreTestLtoF, PreTreatLtoF, and TreatLtoF variables.  Values for these variable will be the same for all tiers in all regions
            // no. 2 is controlled with PreTestLtoF.all.3, This will set the preTestsLtoF value for tier3 in all regions th the input value.  The same is true for PreTreatLtoF.all.4 and TreatLtoF.all.5
            // no. 3 is controled with PreTreatLtoF.northern.3.  This will set the preTestLtoF value for tier 3 in all the regions in the norhtern zone to the input value.  The same is true for PreTreatLtoF.middlebelt.5 and TreatLtoF.southern.7.
            // override order is no. 3 over no. 2 and no. 2 over no. 1

            // probability patient sample lost to pre-diagnostic LtoF percentage 
            if (preTestLtoF.active) {
         
                // for example if probability of pre-diagnostic sample loss rate is .7 on a range of 0-1 then take 1-.7 to get .3.  vary .7 by .3/2 in each direction to keep relapse percentage in the 0-1 interval    
                if (newData[i].preTestLtoFPer > 0) {

                    if (preTestLtoF.upperBound > 0) {
                    
                        topInterval = preTestLtoF.upperBound * 100;
                        botInterval = preTestLtoF.lowerBound * 100;
                        base = preTestLtoF.mean * 100;
                    }
                    else if (wideOpenFlag) {

                        topInterval = 100;
                        botInterval = 0;
                        base = 50;
                    }
                    else {

                        topInterval = newData[i].preTestLtoFPer + (100 - newData[i].preTestLtoFPer)/2;
                        botInterval = newData[i].preTestLtoFPer - (100 - newData[i].preTestLtoFPer)/2;
                        base = newData[i].preTestLtoFPer;
                    }
            
                    interval = topInterval - botInterval;

                    if (randomFlag) {
                    
                        randFloat = getDistRandFloat("preTestLtoF", preTestLtoF.distribution, base, topInterval, botInterval, preTestLtoF.shape1Alpha, preTestLtoF.shape2Beta, preTestLtoF.scaleHB);
                    }
                    else {

                        randFloat = interval/noBins * binNumber;
                        //std::cout << "input-change:  init probReturn orig = " << newData[i].probReturn <<  ", interval = " << interval << ", randFloat = " << randFloat << ", value = " << botInterval+randInt << "\n";
                    }

                    // debug - true
                    if (debug) std::cout << "input-change: inside input change pre-test ltfu percentage = " << newData[i].preTestLtoFPer  << " randFloat "<< randFloat << ", botInterval = " << botInterval <<  "\n";
                
                    if (preTestLtoF.distribution == "Beta") {

                        newData[i].preTestLtoFPer = randFloat * 100;
                    }
                    else {

                        newData[i].preTestLtoFPer = botInterval + randFloat;
                    }

                    // this represents a global change for all the preTestLtoF tiers so update tiers
                    // diseaseLtoF[disIndex].preTestLtoFTierRegion
                    updateGlobalTiers(newData[i].preTestLtoFPerTier, newData[i].preTestLtoFPer);

                    updateGlobalTegionalTiers(diseaseLtoF[i].preTestLtoFTierRegion, newData[i].preTestLtoFPer);

                    updateGlobalTegionalTiers(newData[i].preTestLtoFTierRegion, newData[i].preTestLtoFPer);
                }
            }

            // preTestLtoF.   (all, norther, middlebelt, southern or region) &&&&
            for (int disIndex = 0; disIndex < noNewData; disIndex++) {

                // only update loss to follow up values for current disease
                if (currentDisease == diseaseLtoF[disIndex].disease) {
                    
                    for (int regIndex = 0; regIndex < 20; regIndex++) {


                        if (debug) std::cout << "found PreTestLtoF region = " << (preTestLtoFRegion + regIndex)->region << "\n";

                        if ((preTestLtoFRegion + regIndex)->region == "all") {

                            if (debug) std::cout << "found PreTestLtoF.all = " << (preTestLtoFRegion + regIndex)->region << " active - 3 = " << (preTestLtoFRegion + regIndex)->active[2] << ", 4 = " << (preTestLtoFRegion + regIndex)->active[3] << ", 5 = " << (preTestLtoFRegion + regIndex)->active[4] << ", 6 = " << (preTestLtoFRegion + regIndex)->active[5] << ", 7 = " << (preTestLtoFRegion + regIndex)->active[6] << "\n";
                            
                            // check active flags for each tier.
                            for (int actIndex = 0; actIndex < 10; actIndex++) {

                                if ((preTestLtoFRegion + regIndex)->active[actIndex] == 1) {

                                    // get the distribution value for the active parameter from array of parameter values
                                    distValue = getDistributionLtoFValue((preTestLtoFRegion + regIndex), actIndex, newData[i].preTestLtoFPerTier[actIndex], randomFlag, wideOpenFlag, noBins, binNumber, "preTestLtoF");

                                    // create a new value for that tier and then update all the region tier values
                                    updateGlobalTierValue(diseaseLtoF[disIndex].preTestLtoFTierRegion, "all", actIndex, distValue, true);
                                }
                            }
                        }
                        else if ((preTestLtoFRegion + regIndex)->region == "northern") {

                            if (debug) std::cout << "found PreTestLtoF.region = " << (preTestLtoFRegion + regIndex)->region << " active -  3 = " << (preTestLtoFRegion + regIndex)->active[2] << ", 4 = " << (preTestLtoFRegion + regIndex)->active[3] << ", 5 = " << (preTestLtoFRegion + regIndex)->active[4] << ", 6 = " << (preTestLtoFRegion + regIndex)->active[5] << ", 7 = " << (preTestLtoFRegion + regIndex)->active[6] << "\n";

                            //check active flags for each tier.
                            for (int actIndex = 0; actIndex < 10; actIndex++) {

                                if ((preTestLtoFRegion + regIndex)->active[actIndex] == 1) {
   
                                    // get the distribution value for the active parameter from array of parameter values
                                    distValue = getDistributionLtoFValue((preTestLtoFRegion + regIndex), actIndex, newData[i].preTestLtoFPerTier[actIndex], randomFlag, wideOpenFlag, noBins, binNumber, "preTestLtoF");

                                    // create a new value for that tier and then update all the region tier values
                                    updateGlobalTierValue(diseaseLtoF[disIndex].preTestLtoFTierRegion, "NORTHERN", actIndex, distValue, false);
                                    updateGlobalTierValue(diseaseLtoF[disIndex].preTestLtoFTierRegion, "NORTH EAST", actIndex, distValue, false);
                                    updateGlobalTierValue(diseaseLtoF[disIndex].preTestLtoFTierRegion, "SAVANNAH", actIndex, distValue, false);
                                    updateGlobalTierValue(diseaseLtoF[disIndex].preTestLtoFTierRegion, "UPPER EAST", actIndex, distValue, false);
                                    updateGlobalTierValue(diseaseLtoF[disIndex].preTestLtoFTierRegion, "UPPER WEST", actIndex, distValue, false);
                                }
                            }
                        }
                        else if ((preTestLtoFRegion + regIndex)->region == "middlebelt") {

                            if (debug) std::cout << "found PreTestLtoF.region = " << (preTestLtoFRegion + regIndex)->region << " active -  3 = " << (preTestLtoFRegion + regIndex)->active[2] << ", 4 = " << (preTestLtoFRegion + regIndex)->active[3] << ", 5 = " << (preTestLtoFRegion + regIndex)->active[4] << ", 6 = " << (preTestLtoFRegion + regIndex)->active[5] << ", 7 = " << (preTestLtoFRegion + regIndex)->active[6] << "\n";

                            //check active flags for each tier.
                            for (int actIndex = 0; actIndex < 10; actIndex++) {

                                if ((preTestLtoFRegion + regIndex)->active[actIndex] == 1) {
                                
                                    // get the distribution value for the active parameter from array of parameter values
                                    distValue = getDistributionLtoFValue((preTestLtoFRegion + regIndex), actIndex, newData[i].preTestLtoFPerTier[actIndex], randomFlag, wideOpenFlag, noBins, binNumber, "preTestLtoF");
        
                                    // create a new value for that tier and then update all the region tier values
                                    updateGlobalTierValue(diseaseLtoF[disIndex].preTestLtoFTierRegion, "AHAFO", actIndex, distValue, false);
                                    updateGlobalTierValue(diseaseLtoF[disIndex].preTestLtoFTierRegion, "ASHANTI", actIndex, distValue, false);
                                    updateGlobalTierValue(diseaseLtoF[disIndex].preTestLtoFTierRegion, "BONO", actIndex, distValue, false);
                                    updateGlobalTierValue(diseaseLtoF[disIndex].preTestLtoFTierRegion, "BONO EAST", actIndex, distValue, false);
                                    updateGlobalTierValue(diseaseLtoF[disIndex].preTestLtoFTierRegion, "EASTERN", actIndex, distValue, false);
                                }
                            }
                        }
                        else if ((preTestLtoFRegion + regIndex)->region == "southern") {

                            if (debug) std::cout << "found PreTestLtoF.region = " << (preTestLtoFRegion + regIndex)->region << " active -  3 = " << (preTestLtoFRegion + regIndex)->active[2] << ", 4 = " << (preTestLtoFRegion + regIndex)->active[3] << ", 5 = " << (preTestLtoFRegion + regIndex)->active[4] << ", 6 = " << (preTestLtoFRegion + regIndex)->active[5] << ", 7 = " << (preTestLtoFRegion + regIndex)->active[6] << "\n";

                            //check active flags for each tier.
                            for (int actIndex = 0; actIndex < 10; actIndex++) {

                                if ((preTestLtoFRegion + regIndex)->active[actIndex] == 1) {

                                      // get the distribution value for the active parameter from array of parameter values
                                      distValue = getDistributionLtoFValue((preTestLtoFRegion + regIndex), actIndex, newData[i].preTestLtoFPerTier[actIndex], randomFlag, wideOpenFlag, noBins, binNumber, "preTestLtoF");
          
                                      // create a new value for that tier and then update all the region tier values
                                      updateGlobalTierValue(diseaseLtoF[disIndex].preTestLtoFTierRegion, "CENTRAL", actIndex, distValue, false);
                                      updateGlobalTierValue(diseaseLtoF[disIndex].preTestLtoFTierRegion, "GREATER ACCRA", actIndex, distValue, false);
                                      updateGlobalTierValue(diseaseLtoF[disIndex].preTestLtoFTierRegion, "OTI", actIndex, distValue, false);
                                      updateGlobalTierValue(diseaseLtoF[disIndex].preTestLtoFTierRegion, "VOLTA", actIndex, distValue, false);
                                      updateGlobalTierValue(diseaseLtoF[disIndex].preTestLtoFTierRegion, "WESTERN", actIndex, distValue, false);
                                      updateGlobalTierValue(diseaseLtoF[disIndex].preTestLtoFTierRegion, "WESTERN NORTH", actIndex, distValue, false);
                                }
                            }
                        }
                        else {

                            if (debug) std::cout << "found PreTestLtoF.region = " << (preTestLtoFRegion + regIndex)->region << " active -  3 = " << (preTestLtoFRegion + regIndex)->active[2] << ", 4 = " << (preTestLtoFRegion + regIndex)->active[3] << ", 5 = " << (preTestLtoFRegion + regIndex)->active[4] << ", 6 = " << (preTestLtoFRegion + regIndex)->active[5] << ", 7 = " << (preTestLtoFRegion + regIndex)->active[6] << "\n";

                            if ((preTestLtoFRegion + regIndex)->region == "OTI") {

                                //check active flags for each tier.
                                for (int actIndex = 0; actIndex < 10; actIndex++) {

                                    if ((preTestLtoFRegion + regIndex)->active[actIndex] == 1) {

                                        // get the distribution value for the active parameter from array of parameter values
                                        distValue = getDistributionLtoFValue((preTestLtoFRegion + regIndex), actIndex, newData[i].preTestLtoFPerTier[actIndex], randomFlag, wideOpenFlag, noBins, binNumber, "preTestLtoF");

                                        // create a new value for that tier and then update all the region tier values
                                        updateGlobalTierValue(diseaseLtoF[disIndex].preTestLtoFTierRegion, (preTestLtoFRegion + regIndex)->region, actIndex, distValue, false);
                                    }
                                }
                            }
                        }
                    }
                }
            }
            // debug = true
            if (debug) {

                for (int disIndex = 0; disIndex < noNewData; disIndex++) {

                    std::cout << "input - change: after preTestLtoF. = " << newData[disIndex].name << '\n';

                    dumpTegionalTiers(diseaseLtoF[disIndex].preTestLtoFTierRegion, "after preTestLtoF.");
                }
            }
            //// percentage representation time for pre-test untreated or LtoF patients
            //if (testRepresentation.active) {
         
            //    // for example if sensitivity is .7 on a range of 0-1 then take 1-.7 to get .3.  vary .7 by .3/2 in each direction to keep sensitivity in the 0-1 interval  
            //    if (newData[i].untestedRepresentTime > 0) {

            //        //std::cout << "input-change: inside input change init treat success newData[i].initTreatPer = " << newData[i].initTreatPer <<  "\n";
            //    
            //        if (testRepresentation.upperBound > 0) {
            //        
            //            topInterval = testRepresentation.upperBound;
            //            botInterval = testRepresentation.lowerBound;
            //            base = testRepresentation.mean;
            //        }
            //        else if (wideOpenFlag) {
            //         
            //            topInterval = 90;
            //            botInterval = 10;
            //            base = 50;
            //        }
            //        else {
            //        
            //            topInterval = newData[i].untestedRepresentTime + (100 - newData[i].untestedRepresentTime)/2;
            //            botInterval = newData[i].untestedRepresentTime - (100 - newData[i].untestedRepresentTime)/2;
            //            base = newData[i].untestedRepresentTime;
            //        }
            //
            //        interval = topInterval - botInterval;

            //        if (debug) std::cout << "input-change: inside input change pre-test representation time success top interval = " << topInterval  << " botInterval "<< botInterval << ", interval = " << interval <<  "\n";
            //    
            //        if (randomFlag) {
            //        
            //            randInt = getDistRandInt( "testRepresentation", testRepresentation.distribution, base, topInterval, botInterval, testRepresentation.shape1Alpha, testRepresentation.shape2Beta);
            //        }
            //        else {

            //            randInt = interval/noBins * binNumber;
            //            //std::cout << "input-change:  init treat success orig = " << newData[i].initTreatPer <<  ", interval = " << interval << ", randInt = " << randInt << ", value = " << botInterval+randInt <<  "\n";
            //        }
            //    
            //        if (debug) std::cout << "input-change: inside input change init treat success time = " << newData[i].initTreatPer  << " randint "<< randInt << ", botInterval = " << botInterval <<  "\n";
            //    
            //        newData[i].untestedRepresentTime = botInterval + randInt;
            //    }
            //}


            if (representDeathPer.active) {
         
                // for example if sensitivity is .7 on a range of 0-1 then take 1-.7 to get .3.  vary .7 by .3/2 in each direction to keep sensitivity in the 0-1 interval  
                if (newData[i].representDeathPer > 0) {

                    if (debug) std::cout << "input-change: inside input change init treat success newData[i].representDeathPer = " << newData[i].representDeathPer <<  "\n";
                
                    if (representDeathPer.upperBound > 0) {
                    
                        topInterval = representDeathPer.upperBound * 100;
                        botInterval = representDeathPer.lowerBound * 100;
                        base = representDeathPer.mean * 100;
                    }
                    else if (wideOpenFlag) {
                     
                        topInterval = 90;
                        botInterval = 10;
                        base = 50;
                    }
                    else {
                    
                        topInterval = newData[i].representDeathPer + (100 - newData[i].representDeathPer)/2;
                        botInterval = newData[i].representDeathPer - (100 - newData[i].representDeathPer)/2;
                        base = newData[i].representDeathPer;
                    }
            
                    interval = topInterval - botInterval;

                    if (debug) std::cout << "input-change: inside input change pre-test representation percentage success top interval = " << topInterval  << " botInterval "<< botInterval << ", interval = " << interval <<  "\n";
                
                    if (randomFlag) {
                    
                        randFloat = getDistRandFloat("representDeathPer", representDeathPer.distribution, base, topInterval, botInterval, representDeathPer.shape1Alpha, representDeathPer.shape2Beta, representDeathPer.scaleHB);
                    }
                    else {

                        randFloat = interval/noBins * binNumber;
                        //std::cout << "input-change:  init treat success orig = " << newData[i].representDeathPer <<  ", interval = " << interval << ", randFloat = " << randFloat << ", value = " << botInterval+randFloat <<  "\n";
                    }
                
                    if (debug) std::cout << "input-change: inside input change untestedRepresent per = " << newData[i].representDeathPer << " randFloat "<< randFloat << ", botInterval = " << botInterval <<  "\n";
                
                    
                    if (representDeathPer.distribution == "Beta") {

                        newData[i].representDeathPer = randFloat * 100;
                    }
                    else {

                        newData[i].representDeathPer = botInterval + randFloat;
                    }
                }
            }

            if (newTimeDeathRetreat.active) {
                
                // for example if sensitivity is .7 on a range of 0-1 then take 1-.7 to get .3.  vary .7 by .3/2 in each direction to keep sensitivity in the 0-1 interval  
                if (newData[i].timeDeathRetreat > 0) {

                    if (debug) std::cout << "input-change: inside input change init treat success newData[i].timeDeathRetreat = " << newData[i].timeDeathRetreat <<  "\n";

                    if (newTimeDeathRetreat.upperBound > 0) {

                        topInterval = newTimeDeathRetreat.upperBound;
                        botInterval = newTimeDeathRetreat.lowerBound;
                        base = newTimeDeathRetreat.mean;
                    }
                    else if (wideOpenFlag) {

                        topInterval = 90;
                        botInterval = 10;
                        base = 50;
                    }
                    else {

                        topInterval = newData[i].timeDeathRetreat + (100 - newData[i].timeDeathRetreat) / 2;
                        botInterval = newData[i].timeDeathRetreat - (100 - newData[i].timeDeathRetreat) / 2;
                        base = newData[i].timeDeathRetreat;
                    }

                    interval = topInterval - botInterval;

                    if (debug) std::cout << "input-change: inside input change new time death retreat success top interval = " << topInterval << " botInterval " << botInterval << ", interval = " << interval << "\n";

                    if (randomFlag) {

                        randFloat = getDistRandFloat("newTimeDeathRetreat", newTimeDeathRetreat.distribution, base, topInterval, botInterval, newTimeDeathRetreat.shape1Alpha, newTimeDeathRetreat.shape2Beta, newTimeDeathRetreat.scaleHB);
                    }
                    else {

                        randFloat = interval / noBins * binNumber;
                        //std::cout << "input-change:  init treat success orig = " << newData[i].timeDeathRetreat <<  ", interval = " << interval << ", randFloat = " << randFloat << ", value = " << botInterval+randFloat <<  "\n";
                    }

                    if (debug) std::cout << "input-change: inside input change new time death retreatment = " << newData[i].timeDeathRetreat << " randFloat " << randFloat << ", botInterval = " << botInterval << "\n";

                    if (newTimeDeathRetreat.distribution == "Beta") {

                        newData[i].timeDeathRetreat = randFloat;
                    }
                    else {

                        newData[i].timeDeathRetreat = botInterval + randFloat;
                    }
                }
            }


            if (newTimeLtoFRepresent.active) {
                
                // for example if sensitivity is .7 on a range of 0-1 then take 1-.7 to get .3.  vary .7 by .3/2 in each direction to keep sensitivity in the 0-1 interval  
                if (newData[i].timeLtoFRepresent > 0) {

                    if (debug) std::cout << "input-change: inside input change init treat success newData[i].timeLtoFRepresent = " << newData[i].timeLtoFRepresent << "\n";

                    if (newTimeLtoFRepresent.upperBound > 0) {

                        topInterval = newTimeLtoFRepresent.upperBound;
                        botInterval = newTimeLtoFRepresent.lowerBound;
                        base = newTimeLtoFRepresent.mean;
                    }
                    else if (wideOpenFlag) {

                        topInterval = 90;
                        botInterval = 10;
                        base = 50;
                    }
                    else {

                        topInterval = newData[i].timeLtoFRepresent + (100 - newData[i].timeLtoFRepresent) / 2;
                        botInterval = newData[i].timeLtoFRepresent - (100 - newData[i].timeLtoFRepresent) / 2;
                        base = newData[i].timeLtoFRepresent;
                    }

                    interval = topInterval - botInterval;

                    if (debug) std::cout << "input-change: inside input change new time ltof represent top interval = " << topInterval << " botInterval " << botInterval << ", interval = " << interval << "\n";

                    if (randomFlag) {

                        randFloat = getDistRandFloat("newTimeLtoFRepresent", newTimeLtoFRepresent.distribution, base, topInterval, botInterval, newTimeLtoFRepresent.shape1Alpha, newTimeLtoFRepresent.shape2Beta, newTimeLtoFRepresent.scaleHB);
                    }
                    else {

                        randFloat = interval / noBins * binNumber;
                        //std::cout << "input-change:  timeLtoFRepresent = " << newData[i].timeLtoFRepresent <<  ", interval = " << interval << ", randFloat = " << randFloat << ", value = " << botInterval+randFloat <<  "\n";
                    }

                    if (debug) std::cout << "input-change: inside input change time ltof re-presentation = " << newData[i].timeLtoFRepresent << " randFloat " << randFloat << ", botInterval = " << botInterval << "\n";

                    if (newTimeLtoFRepresent.distribution == "Beta") {

                        newData[i].timeLtoFRepresent = randFloat;
                    }
                    else {

                        newData[i].timeLtoFRepresent = botInterval + randFloat;
                    }
                }
            }


            if (newTimeUntreatCure.active) {
   
                // for example if sensitivity is .7 on a range of 0-1 then take 1-.7 to get .3.  vary .7 by .3/2 in each direction to keep sensitivity in the 0-1 interval  
                if (newData[i].timeUntreatCure > 0) {

                    if (debug) std::cout << "input-change: inside input change init treat success newData[i].timeUntreatCure = " << newData[i].timeUntreatCure <<  "\n";

                    if (newTimeUntreatCure.upperBound > 0) {

                        topInterval = newTimeUntreatCure.upperBound;
                        botInterval = newTimeUntreatCure.lowerBound;
                        base = newTimeUntreatCure.mean;
                    }
                    else if (wideOpenFlag) {

                        topInterval = 90;
                        botInterval = 10;
                        base = 50;
                    }
                    else {

                        topInterval = newData[i].timeUntreatCure + (100 - newData[i].timeUntreatCure) / 2;
                        botInterval = newData[i].timeUntreatCure - (100 - newData[i].timeUntreatCure) / 2;
                        base = newData[i].timeUntreatCure;
                    }

                    interval = topInterval - botInterval;

                    if (debug) std::cout << "input-change: inside input change time untreated cure top interval = " << topInterval << " botInterval " << botInterval << ", interval = " << interval << "\n";

                    if (randomFlag) {

                        randFloat = getDistRandFloat("newTimeUntreatCure", newTimeUntreatCure.distribution, base, topInterval, botInterval, newTimeUntreatCure.shape1Alpha, newTimeUntreatCure.shape2Beta, newTimeUntreatCure.scaleHB);
                    }
                    else {

                        randFloat = interval / noBins * binNumber;
                        //std::cout << "input-change:  timeUntreatCure = " << newData[i].timeUntreatCure <<  ", interval = " << interval << ", randFloat = " << randFloat << ", value = " << botInterval+randFloat <<  "\n";
                    }

                    if (debug) std::cout << "input-change: inside input change time for untreated cure = " << newData[i].timeUntreatCure << " randFloat " << randFloat << ", botInterval = " << botInterval << "\n";

                    if (newTimeUntreatCure.distribution == "Beta") {

                        newData[i].timeUntreatCure = randFloat;
                    }
                    else {

                        newData[i].timeUntreatCure = botInterval + randFloat;
                    }
                }
            }

            // probability patient of MD clinical diagnosis use before sample send out
            //std::cout << "input-change: inside input change pre-test MD CD infected active = " << preTestMDCDInf.active << " distribution = " << preTestMDCDInf.distribution <<"\n";
            //std::cout << "input-change: inside input change pre-test MD CD uninfected active = " << preTestMDCDUnInf.active << " distribution = " << preTestMDCDUnInf.distribution <<"\n";

            if (preTestMDCDInf.active) {

                if (preTestMDCDInf.distribution == "States") {

                    // fixed no. states (3) - 0%, 50% and 100%
                    std::mt19937 generator(std::chrono::system_clock::now().time_since_epoch().count());
                    std::uniform_int_distribution<> distribution(1, preTestMDCDInf.mean);

                    randInt = distribution(generator);

                    if (debug) std::cout << "input-change: inside input change pre-test MD CD mean = " << preTestMDCDInf.mean << " randint = " << randInt << "\n";

                    switch (randInt) {

                    case 1:
                        newData[i].preTestMDCDInfPer = preTestMDCDInf.lowerBound;
                        break;
                    case 2:
                        newData[i].preTestMDCDInfPer = preTestMDCDInf.upperBound;
                        break;
                    case 3:
                        newData[i].preTestMDCDInfPer = preTestMDCDInf.shape1Alpha;
                        break;
                    default:
                        newData[i].preTestMDCDInfPer = preTestMDCDInf.lowerBound;
                        break;
                    }
                }
                else {

                    // Uniform or beta distribution
                    if (debug) std::cout << "input-change: inside input change pre-test MD CD newData[i].preTestMDCDInfPer = " << newData[i].preTestMDCDInfPer << "\n";

                    if (preTestMDCDInf.upperBound > 0) {

                        topInterval = preTestMDCDInf.upperBound;
                        botInterval = preTestMDCDInf.lowerBound;
                        base = preTestMDCDInf.mean;
                    }
                    else if (wideOpenFlag) {

                        topInterval = 99;
                        botInterval = 0;
                        base = 50;
                    }
                    else {

                        topInterval = newData[i].preTestMDCDInfPer + (100 - newData[i].preTestMDCDInfPer) / 2;
                        botInterval = newData[i].preTestMDCDInfPer - (100 - newData[i].preTestMDCDInfPer) / 2;
                        base = newData[i].preTestMDCDInfPer;
                    }

                    interval = topInterval - botInterval;

                    if (debug) std::cout << "input-change: inside input change pre-test MD CD infected top interval = " << topInterval << " botInterval " << botInterval << ", interval = " << interval << "\n";

                    if (randomFlag) {

                        randFloat = getDistRandFloat("preTestMDCDInfPer", preTestMDCDInf.distribution, base, topInterval, botInterval, preTestMDCDInf.shape1Alpha, preTestMDCDInf.shape2Beta, preTestMDCDInf.scaleHB);
                    }
                    else {

                        randFloat = interval / noBins * binNumber;
                        std::cout << "input-change:  pre-test MD CD Per = " << newData[i].preTestMDCDInfPer <<  ", interval = " << interval << ", randFloat = " << randFloat << ", value = " << botInterval+randFloat <<  "\n";
                    }

                    if (debug) std::cout << "input-change: inside input change per. pre-test MD CD = " << newData[i].preTestMDCDInfPer << " randFloat " << randFloat << ", botInterval = " << botInterval << "\n";

                    if (preTestMDCDInf.distribution == "Beta") {

                        newData[i].preTestMDCDInfPer = randFloat;
                    }
                    else {

                        newData[i].preTestMDCDInfPer = botInterval + randFloat;
                    }
                }

                // debug - true
                if (debug) std::cout << "input-change: inside input change pre-test MD CD percentage = " << newData[i].preTestMDCDInfPer << "\n";
            }
            if (preTestMDCDUnInf.active) {

                if (preTestMDCDUnInf.distribution == "States") {

                    // fixed no. states (3) - 0%, 50% and 100%
                    std::mt19937 generator(std::chrono::system_clock::now().time_since_epoch().count());
                    std::uniform_int_distribution<> distribution(1, preTestMDCDUnInf.mean);

                    randInt = distribution(generator);

                    if (debug) std::cout << "input-change: inside input change pre-test MD CD mean = " << preTestMDCDUnInf.mean << " randint = " << randInt << "\n";

                    switch (randInt) {

                    case 1:
                        newData[i].preTestMDCDUnInfPer = preTestMDCDUnInf.lowerBound;
                        break;
                    case 2:
                        newData[i].preTestMDCDUnInfPer = preTestMDCDUnInf.upperBound;
                        break;
                    case 3:
                        newData[i].preTestMDCDUnInfPer = preTestMDCDUnInf.shape1Alpha;
                        break;
                    default:
                        newData[i].preTestMDCDUnInfPer = preTestMDCDUnInf.lowerBound;
                        break;
                    }
                }
                else {

                    // Uniform or beta distribution
                    if (debug) std::cout << "input-change: inside input change pre-test MD CD newData[i].preTestMDCDUnInfPer = " << newData[i].preTestMDCDUnInfPer << "\n";

                    if (preTestMDCDUnInf.upperBound > 0) {

                        topInterval = preTestMDCDUnInf.upperBound;
                        botInterval = preTestMDCDUnInf.lowerBound;
                        base = preTestMDCDUnInf.mean;
                    }
                    else if (wideOpenFlag) {

                        topInterval = 99;
                        botInterval = 0;
                        base = 50;
                    }
                    else {

                        topInterval = newData[i].preTestMDCDUnInfPer + (100 - newData[i].preTestMDCDUnInfPer) / 2;
                        botInterval = newData[i].preTestMDCDUnInfPer - (100 - newData[i].preTestMDCDUnInfPer) / 2;
                        base = newData[i].preTestMDCDUnInfPer;
                    }

                    interval = topInterval - botInterval;

                    if (debug) std::cout << "input-change: inside input change pre-test MD CD top interval = " << topInterval << " botInterval " << botInterval << ", interval = " << interval << "\n";

                    if (randomFlag) {

                        randFloat = getDistRandFloat("preTestMDCDUnInfPer", preTestMDCDUnInf.distribution, base, topInterval, botInterval, preTestMDCDUnInf.shape1Alpha, preTestMDCDUnInf.shape2Beta, preTestMDCDUnInf.scaleHB);
                    }
                    else {

                        randFloat = interval / noBins * binNumber;
                        std::cout << "input-change:  pre-test MD CD Per = " << newData[i].preTestMDCDUnInfPer << ", interval = " << interval << ", randFloat = " << randFloat << ", value = " << botInterval + randFloat << "\n";
                    }

                    if (debug) std::cout << "input-change: inside input change per. pre-test MD CD = " << newData[i].preTestMDCDUnInfPer << " randFloat " << randFloat << ", botInterval = " << botInterval << "\n";

                    if (preTestMDCDUnInf.distribution == "Beta") {

                        newData[i].preTestMDCDUnInfPer = randFloat;
                    }
                    else {

                        newData[i].preTestMDCDUnInfPer = botInterval + randFloat;
                    }
                }

                // debug - true
                if (debug) std::cout << "input-change: inside input change treatLtoFPer = " << newData[i].treatLtoFPer << "\n";

            }

            // now adjust incidence for any incidence adjustment factor
            //if (newData[i].prpIncidencePresenting > 1) {

            //    // not really and incidence adjustment factor - just a new incidence
            //    //newData[i].incidence = newData[i].incidence * incAdjustment;
            //    newData[i].incidence = newData[i].prpIncidencePresenting;
            //}

            if (debug) std::cout << "input-change: change incidence = " << newData[i].incidence << ", prpIncidencePresenting = " << newData[i].prpIncidencePresenting << "\n";
        }
        if (debug) {

            std::cout << "input-change: inside input disease bottom of loop " << "\n";
            std::cout << "input-change: initTreatPer " << newData[i].initTreatPer << " initTreatDeathPer " << newData[i].initTreatDeathPer << " relapsePer " << relapsePer << "\n";
            std::cout << "input-change: adjusted initTreatPer " << newData[i].initTreatPer << " initTreatDeathPer " << newData[i].initTreatDeathPer << " relapsePer " << relapsePer << "\n";
        }
    }       // bottom of newData loop
    
    // debug = true
    if (debug) {

        for (int disIndex = 0; disIndex < noNewData; disIndex++) {

            std::cout << "input - change: after newData loop disease = " << newData[disIndex].name << '\n';

            dumpTegionalTiers(diseaseLtoF[disIndex].preTestLtoFTierRegion, "after disease-tier diseaseLtoF pretestLtof");
            dumpTegionalTiers(diseaseLtoF[disIndex].preTreatLtoFTierRegion, "after disease-tier diseaseLtoF pretreatLtof");
            dumpTegionalTiers(diseaseLtoF[disIndex].treatLtoFTierRegion, "after disease-tier diseaseLtoF treatLtof");
        }
    }

    //&&&
    // if region tier loss to follow up values were programmed, then update them before output
    // line format disease-tier xxxx region xxxxx preTestLtoF xxxxx preTreatLtoF xxxxx treatLtoF xxxxx
    //if (outputRegionTierLtoF) {

    //    for (int newIndex = 0; newIndex < noNewData; newIndex++) {

    //        for (int regIndex = 0; regIndex < 20; regIndex++) {

    //            newData[newIndex].preTestLtoFTierRegion[regIndex].regionNameTier = itemName;
    //            
    //            if (newData[newIndex].name == newData[newIndex].preTestLtoFTierRegion[regIndex].regionNameTier) {

    //                // update the loss to follow up values in this line - we know one of them changed so update them all
    //                for (int ltofIndex = 0; ltofIndex < 10; ltofIndex++) {

    //                    newData[newIndex].preTestLtoFPerTier[ltofIndex] = newData[newIndex].preTestLtoFTierRegion[regIndex].tier[ltofIndex];
    //                    newData[newIndex].preTreatLtoFPerTier[ltofIndex] = newData[newIndex].preTreatLtoFTierRegion[regIndex].tier[ltofIndex];
    //                    newData[newIndex].treatLtoFPerTier[ltofIndex] = newData[newIndex].treatLtoFTierRegion[regIndex].tier[ltofIndex];
    //                }
    //            }
    //        }
    //    }
    //}
    //cooties
    for (int newIndex = 0; newIndex < noNewData; newIndex++) {

        for (int regIndex = 0; regIndex < 20; regIndex++) {


            newData[newIndex].preTestLtoFTierRegion[regIndex].regionName = diseaseLtoF[newIndex].preTestLtoFTierRegion[regIndex].regionName;
            newData[newIndex].preTreatLtoFTierRegion[regIndex].regionName = diseaseLtoF[newIndex].preTreatLtoFTierRegion[regIndex].regionName;
            newData[newIndex].treatLtoFTierRegion[regIndex].regionName = diseaseLtoF[newIndex].treatLtoFTierRegion[regIndex].regionName;

            // update the loss to follow up values in this line - we know one of them changed so update them all
            for (int ltofIndex = 0; ltofIndex < 10; ltofIndex++) {

                newData[newIndex].preTestLtoFTierRegion[regIndex].tier[ltofIndex] = diseaseLtoF[newIndex].preTestLtoFTierRegion[regIndex].tier[ltofIndex];
                newData[newIndex].preTreatLtoFTierRegion[regIndex].tier[ltofIndex] = diseaseLtoF[newIndex].preTreatLtoFTierRegion[regIndex].tier[ltofIndex];
                newData[newIndex].treatLtoFTierRegion[regIndex].tier[ltofIndex] = diseaseLtoF[newIndex].treatLtoFTierRegion[regIndex].tier[ltofIndex];
            }
        }
    }
       



    // dump values
    if (debug) {

        for (int i = 0; i < noNewData; i++) {

            std::cout << "input-change: name " << newData[i].name << " init death per " << newData[i].initTreatDeathPer << " init death per IP " << newData[i].initTreatDeathPerIP;
            std::cout << " treatltof " << newData[i].treatLtoFPer <<  "treatltofIP " << newData[i].treatLtoFPerIP << "\n";
        }
    }

    // open aggregate file and output values
    myfile.open("./stats/" + scenario + "/job-" + jobNo + "/input/diseases/diseases.csv");

    myfile << "Name" << ",Mortality" << ",incidence.per.100000" << ",incidence.adjustment.type" << ",incidence.adjusment.factor" << ",prp.incidence.presenting" << ",patients.ratio.uninf.inf" << ",per.patients.present.inf";
    myfile << ",Transmission" << ",Priority" << ",Morbidity" << ",preTest MD clinical diagnosis infected" << ",preTest MD clinical diagnosis uninfected";
    myfile << ",preTestLtoF per" << ",preTestLtoF per Tier1" << ",preTestLtoF per Tier2" << ",preTestLtoF per Tier3" << ",preTestLtoF per Tier4" << ",preTestLtoF per Tier5";
    myfile << ",preTestLtoF per Tier6" << ",preTestLtoF per Tier7" << ",preTestLtoF per Tier8" << ",preTestLtoF per Tier9" << ",preTestLtoF per Tier10";
    myfile << ",preTestLtoF reduction per" << ",space" << ",space" << ",prob patient returns" << ",per samples lost";
    myfile << ",No. of patients that receive treatmet after Diagnosis %" << ",Initial treat time (timesteps)";
    myfile << ",Initial treat success %" << ",Initial treat death %";
    myfile << ",pre-treat LtoF %" << ",pre-treat LtoF % Tier1" << ",pre-treat LtoF % Tier2" << ",pre-treat LtoF % Tier3" << ",pre-treat LtoF % Tier4" << ",pre-treat LtoF % Tier5";
    myfile << ",pre-treat LtoF % Tier6" << ",pre-treat LtoF % Tier7" << ",pre-treat LtoF % Tier8" << ",pre-treat LtoF % Tier9" << ",pre-treat LtoF % Tier10";
    myfile << ",treatment LtoF %" << ",treatment LtoF % Tier1" << ",treatment LtoF % Tier2" << ",treatment LtoF % Tier3" << ",treatment LtoF % Tier4" << ",treatment LtoF % Tier5";
    myfile << ",treatment LtoF % Tier6" << ",treatment LtoF % Tier7" << ",treatment LtoF % Tier8" << ",treatment LtoF % Tier9" << ",treatment LtoF % Tier10";
    myfile << ",Treat failure time (timesteps)" << ",Relapse success %";
    myfile << ",prob non-infected treated" << ",percent drug resistant" << ",avg time onset to presentation" << ",percent untreated die";
    myfile << ",percent ltof that die before representing";
    myfile << ",max.time.death.retreated" << ",max.time.ltof.represent" << ",max.time.untreat.cure";
    myfile << ",space" << ",space" << ",Cost tx" << ",Benefit Tx";
    myfile << ",Loss to followup P" << ",Loss to followup SP" << "\n";

    // create disease rows
    for (int i = 0; i < noNewData; i++) {

        myfile << newData[i].name << "," << newData[i].mortality;
        myfile << "," << newData[i].incidence << "," << newData[i].incAdjType << "," << newData[i].incAdjFactor << "," << newData[i].prpIncidencePresenting << "," << newData[i].patientsRatioUninfInf << "," << newData[i].perPatientPresentInf;
        myfile << "," << newData[i].transmission << "," << newData[i].priority << "," << newData[i].moribidity << "," << newData[i].preTestMDCDInfPer << "," << newData[i].preTestMDCDUnInfPer;
        myfile << "," << newData[i].preTestLtoFPer << "," << newData[i].preTestLtoFPerTier[0] << "," << newData[i].preTestLtoFPerTier[1] << "," << newData[i].preTestLtoFPerTier[2];
        myfile << "," << newData[i].preTestLtoFPerTier[3] << "," << newData[i].preTestLtoFPerTier[4] << "," << newData[i].preTestLtoFPerTier[5] << "," << newData[i].preTestLtoFPerTier[6];
        myfile << "," << newData[i].preTestLtoFPerTier[7] << "," << newData[i].preTestLtoFPerTier[8] << "," << newData[i].preTestLtoFPerTier[9];
        myfile << "," << newData[i].preTestLtoFDeathPer << "," << newData[i].space1 << "," << newData[i].space2 << "," << newData[i].probReturn << "," << newData[i].perLostSamples;
        myfile << "," << newData[i].patWithTreatAfterDiag << "," << newData[i].initTreatTime << "," << newData[i].initTreatPer << "," << newData[i].initTreatDeathPerIP;
        myfile << "," << newData[i].preTreatLtoFPer << "," << newData[i].preTreatLtoFPerTier[0] << "," << newData[i].preTreatLtoFPerTier[1] << "," << newData[i].preTreatLtoFPerTier[2];
        myfile << "," << newData[i].preTreatLtoFPerTier[3] << "," << newData[i].preTreatLtoFPerTier[4] << "," << newData[i].preTreatLtoFPerTier[5] << "," << newData[i].preTreatLtoFPerTier[6];
        myfile << "," << newData[i].preTreatLtoFPerTier[7] << "," << newData[i].preTreatLtoFPerTier[8] << "," << newData[i].preTreatLtoFPerTier[9];
        myfile << "," << newData[i].treatLtoFPerIP << "," << newData[i].treatLtoFPerTier[0] << "," << newData[i].treatLtoFPerTier[1] << "," << newData[i].treatLtoFPerTier[2];
        myfile << "," << newData[i].treatLtoFPerTier[3] << "," << newData[i].treatLtoFPerTier[4] << "," << newData[i].treatLtoFPerTier[5] << "," << newData[i].treatLtoFPerTier[6];
        myfile << "," << newData[i].treatLtoFPerTier[7] << "," << newData[i].treatLtoFPerTier[8] << "," << newData[i].treatLtoFPerTier[9];
        myfile << "," << newData[i].treatFailTime << "," << newData[i].relapseSuccessPer;
        myfile << "," << newData[i].probNonInfTreat << "," << newData[i].drugResistPer;
        myfile << "," << newData[i].onsetToPresentTime << "," << newData[i].untreatedDiePer;
        myfile << "," << newData[i].representDeathPer;
        myfile << "," << newData[i].timeDeathRetreat << "," << newData[i].timeLtoFRepresent << "," << newData[i].timeUntreatCure;
        myfile << "," << newData[i].space3 << "," << newData[i].space3 << "\n";
    }

    // create disease-tier rows for loss to follow up
    // swap out old loss to follow up values with new ones calculated  &&&
    for (int i = 0; i < noNewDataLtoF; i++) {
        
        for (int disIndex = 0; disIndex < noNewData; disIndex++) {

            // first extract disease name - also subtract off 2 to account for arrays starting at 0 instead of 1 and end of line character
            tierName = newDataLtoF[i].name.substr(0, newDataLtoF[i].name.length() - newDataLtoF[i].name.find("-tier") - 2);

            if (tierName == diseaseLtoF[disIndex].disease) {

                if (debug) std::cout << "output: newDataLtoF[i].name= " << tierName << " diseaseLtoF = " << diseaseLtoF[disIndex].disease << "\n";

                // now make sure region matches to output the loss to follow up values
                for (int regIndex = 0; regIndex < 20; regIndex++) {

                    if (newDataLtoF[i].incAdjType == diseaseLtoF[disIndex].preTestLtoFTierRegion[regIndex].regionName) {

                        if (debug) std::cout << "region = " << newDataLtoF[i].incAdjType << " diseaseLtoF region = " << diseaseLtoF[disIndex].preTestLtoFTierRegion[regIndex].regionName << "\n";
                        
                        myfile << newDataLtoF[i].name << "," << newDataLtoF[i].mortality;
                        myfile << "," << newDataLtoF[i].incidence << "," << newDataLtoF[i].incAdjType << "," << newDataLtoF[i].incAdjFactor << "," << newDataLtoF[i].prpIncidencePresenting << "," << newDataLtoF[i].patientsRatioUninfInf << "," << newDataLtoF[i].perPatientPresentInf;
                        myfile << "," << newDataLtoF[i].transmission << "," << newDataLtoF[i].priority << "," << newDataLtoF[i].moribidity << "," << newDataLtoF[i].preTestMDCDInfPer << "," << newDataLtoF[i].preTestMDCDUnInfPer;
                        myfile << "," << newDataLtoF[i].preTestLtoFPer << "," << diseaseLtoF[disIndex].preTestLtoFTierRegion[regIndex].tier[0];
                        myfile << "," << diseaseLtoF[disIndex].preTestLtoFTierRegion[regIndex].tier[1];
                        myfile << "," << diseaseLtoF[disIndex].preTestLtoFTierRegion[regIndex].tier[2];
                        myfile << "," << diseaseLtoF[disIndex].preTestLtoFTierRegion[regIndex].tier[3];
                        myfile << "," << diseaseLtoF[disIndex].preTestLtoFTierRegion[regIndex].tier[4];
                        myfile << "," << diseaseLtoF[disIndex].preTestLtoFTierRegion[regIndex].tier[5];
                        myfile << "," << diseaseLtoF[disIndex].preTestLtoFTierRegion[regIndex].tier[6];
                        myfile << "," << diseaseLtoF[disIndex].preTestLtoFTierRegion[regIndex].tier[7];
                        myfile << "," << diseaseLtoF[disIndex].preTestLtoFTierRegion[regIndex].tier[8];
                        myfile << "," << diseaseLtoF[disIndex].preTestLtoFTierRegion[regIndex].tier[9];
                        myfile << "," << newDataLtoF[i].preTestLtoFDeathPer << "," << newDataLtoF[i].space1 << "," << newDataLtoF[i].space2 << "," << newDataLtoF[i].probReturn << "," << newDataLtoF[i].perLostSamples;
                        myfile << "," << newDataLtoF[i].patWithTreatAfterDiag << "," << newDataLtoF[i].initTreatTime << "," << newDataLtoF[i].initTreatPer << "," << newDataLtoF[i].initTreatDeathPerIP;
                        myfile << "," << newDataLtoF[i].preTreatLtoFPer << "," << diseaseLtoF[disIndex].preTreatLtoFTierRegion[regIndex].tier[0];
                        myfile << "," << diseaseLtoF[disIndex].preTreatLtoFTierRegion[regIndex].tier[1];
                        myfile << "," << diseaseLtoF[disIndex].preTreatLtoFTierRegion[regIndex].tier[2];
                        myfile << "," << diseaseLtoF[disIndex].preTreatLtoFTierRegion[regIndex].tier[3];
                        myfile << "," << diseaseLtoF[disIndex].preTreatLtoFTierRegion[regIndex].tier[4];
                        myfile << "," << diseaseLtoF[disIndex].preTreatLtoFTierRegion[regIndex].tier[5];
                        myfile << "," << diseaseLtoF[disIndex].preTreatLtoFTierRegion[regIndex].tier[6];
                        myfile << "," << diseaseLtoF[disIndex].preTreatLtoFTierRegion[regIndex].tier[7];
                        myfile << "," << diseaseLtoF[disIndex].preTreatLtoFTierRegion[regIndex].tier[8];
                        myfile << "," << diseaseLtoF[disIndex].preTreatLtoFTierRegion[regIndex].tier[9];
                        myfile << "," << newDataLtoF[i].treatLtoFPerIP << "," << diseaseLtoF[disIndex].treatLtoFTierRegion[regIndex].tier[0];
                        myfile << "," << diseaseLtoF[disIndex].treatLtoFTierRegion[regIndex].tier[1];
                        myfile << "," << diseaseLtoF[disIndex].treatLtoFTierRegion[regIndex].tier[2];
                        myfile << "," << diseaseLtoF[disIndex].treatLtoFTierRegion[regIndex].tier[3];
                        myfile << "," << diseaseLtoF[disIndex].treatLtoFTierRegion[regIndex].tier[4];
                        myfile << "," << diseaseLtoF[disIndex].treatLtoFTierRegion[regIndex].tier[5];
                        myfile << "," << diseaseLtoF[disIndex].treatLtoFTierRegion[regIndex].tier[6];
                        myfile << "," << diseaseLtoF[disIndex].treatLtoFTierRegion[regIndex].tier[7];
                        myfile << "," << diseaseLtoF[disIndex].treatLtoFTierRegion[regIndex].tier[8];
                        myfile << "," << diseaseLtoF[disIndex].treatLtoFTierRegion[regIndex].tier[9];
                        myfile << "," << newDataLtoF[i].treatFailTime << "," << newDataLtoF[i].relapseSuccessPer;
                        myfile << "," << newDataLtoF[i].probNonInfTreat << "," << newDataLtoF[i].drugResistPer;
                        myfile << "," << newDataLtoF[i].onsetToPresentTime << "," << newDataLtoF[i].untreatedDiePer;
                        myfile << "," << newDataLtoF[i].representDeathPer;
                        myfile << "," << newDataLtoF[i].timeDeathRetreat << "," << newDataLtoF[i].timeLtoFRepresent << "," << newDataLtoF[i].timeUntreatCure;
                        myfile << "," << newDataLtoF[i].space3 << "," << newDataLtoF[i].space3 << "\n";
                    }
                }
            }
        }
    }

    myfile.close();
    myfile.close();

    if (debug) std::cout << "input-change: leaving disease file" <<  "\n"; 
}

// modify facility specific parameters
void inputChangeFacility(vector<vector<string>> facilitySpecificInput, string facilitySpecific, string currentDisease, parameters noMachines, parameters batchWait, bool randomFlag, int noBins, int binNumber, float alpha, bool inPlaceFlag, string scenario, string jobNo, bool wideOpenFlag, int returnArray[]){

    int noNewData;
    int randInt = 0;
    int randInt2 = 0;
    float topInterval;
    float topInterval2;
    float botInterval = 0;
    float botInterval2 = 0;
    float base;
    float interval;
    float interval2;
    bool debug = false;
    bool debug2 = false;

    ofstream myfile;

    struct facilityStruct {
        string number;
        string name;
        int noDiseases;
        string disease;
        int batchWait;
        int returnTime;
        int noTests;
        string test1Name;
        int test1Testing;
        int test1Priority;
        int noMachinesTest1;
        int maxRegentsTest1;
        int timeStepsInCycleTest1;
        string test2Name;
        int test2Testing;
        int test2Priority;
        int noMachinesTest2;
        int maxRegentsTest2;
        int timeStepsInCycleTest2;
        string district;
    };

    noNewData = facilitySpecificInput.size();

    facilityStruct* newData = new facilityStruct[noNewData];

    if (debug) std::cout << "input-change: inside change facility" << "\n";
    
    if (debug) std::cout << "input-change: inside change facility input = " << noNewData  << ", disease = " << currentDisease <<  "\n";

    // skip the first row, it contains column headers
    // format is : id, Name, no. of diseases,disease, Batch wait time for sample send out, Return time for sample results, No. of tests, test1 name, Testing test1,
    //          priority of test, no. of machines1, max reagents per cycle1, timestep in reagent cycle1, test2 name, testing test2, no. of Machines, max reagents per cycle2,
    //          timestep in reagent cycle2, , ,District
    for (auto rdIndex = 1u; rdIndex < facilitySpecificInput.size(); rdIndex++) {

        if (debug2) {
            std::cout << facilitySpecificInput[rdIndex][0] << ' ' << facilitySpecificInput[rdIndex][1] << ' ' << facilitySpecificInput[rdIndex][2] << ' ' << facilitySpecificInput[rdIndex][4] << ' ' << facilitySpecificInput[rdIndex][5];
            std::cout << ' ' << facilitySpecificInput[rdIndex][6] << ' ' << facilitySpecificInput[rdIndex][7] << ' ' << facilitySpecificInput[rdIndex][8] << ' ' << facilitySpecificInput[rdIndex][9];
            std::cout << ' ' << facilitySpecificInput[rdIndex][10] << ' ' << facilitySpecificInput[rdIndex][11] << ' ' << facilitySpecificInput[rdIndex][12] << ' ' << facilitySpecificInput[rdIndex][13]  << '\n';
        }

        newData[rdIndex-1].number = facilitySpecificInput[rdIndex][0];
        newData[rdIndex-1].name = facilitySpecificInput[rdIndex][1];
        newData[rdIndex-1].noDiseases = stoi(facilitySpecificInput[rdIndex][2]);
        newData[rdIndex-1].disease = facilitySpecificInput[rdIndex][3];
        newData[rdIndex-1].batchWait = stoi(facilitySpecificInput[rdIndex][4]);
        newData[rdIndex-1].returnTime = stoi(facilitySpecificInput[rdIndex][5]);
        newData[rdIndex-1].noTests = stoi(facilitySpecificInput[rdIndex][6]);
        newData[rdIndex-1].test1Name = facilitySpecificInput[rdIndex][7];
        newData[rdIndex-1].test1Testing = stoi(facilitySpecificInput[rdIndex][8]);
        newData[rdIndex-1].test1Priority = stoi(facilitySpecificInput[rdIndex][9]);
        newData[rdIndex-1].noMachinesTest1 = stoi(facilitySpecificInput[rdIndex][10]);
        newData[rdIndex-1].maxRegentsTest1 = stoi(facilitySpecificInput[rdIndex][11]);
        newData[rdIndex-1].timeStepsInCycleTest1 = stoi(facilitySpecificInput[rdIndex][12]);
        newData[rdIndex-1].test2Name = facilitySpecificInput[rdIndex][13];
        newData[rdIndex-1].test2Testing = stoi(facilitySpecificInput[rdIndex][14]);
        newData[rdIndex-1].test2Priority = stoi(facilitySpecificInput[rdIndex][15]);
        newData[rdIndex-1].noMachinesTest2 = stoi(facilitySpecificInput[rdIndex][16]);
        newData[rdIndex-1].maxRegentsTest2 = stoi(facilitySpecificInput[rdIndex][17]);
        newData[rdIndex-1].timeStepsInCycleTest2 = stoi(facilitySpecificInput[rdIndex][18]);
        newData[rdIndex-1].district = facilitySpecificInput[rdIndex][21];

        if (debug2) {
            std::cout << newData[rdIndex-1].name << ' ' << newData[rdIndex-1].noDiseases << ' ' << newData[rdIndex-1].disease << ' ' << newData[rdIndex-1].batchWait;
            std::cout << ' ' << newData[rdIndex-1].returnTime << ' ' << newData[rdIndex-1].noTests << ' ' << newData[rdIndex-1].test1Name;
            std::cout << ' ' << newData[rdIndex-1].test1Testing << ' ' << newData[rdIndex-1].test1Priority << ' ' << newData[rdIndex-1].noMachinesTest1;
            std::cout << ' ' << newData[rdIndex-1].maxRegentsTest1 << ' ' << newData[rdIndex-1].timeStepsInCycleTest1 << ' ' << newData[rdIndex-1].test2Name;
            std::cout << ' ' << newData[rdIndex-1].test2Testing << ' ' << newData[rdIndex-1].test2Priority;
            std::cout << ' ' << newData[rdIndex-1].noMachinesTest2 << ' ' << newData[rdIndex-1].maxRegentsTest2 << ' ' << newData[rdIndex-1].timeStepsInCycleTest2<< '\n';
        }
    }
     
    // adjust for no header row
    noNewData = noNewData - 1;

    if (debug) std::cout << "input-change: inside input change incidence  " << facilitySpecific << " no of data = " << noNewData << "\n";

    // seed random number generator - moved into randInt and randFloat routines
    //unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    //std::mt19937 generator(seed);

    for (int i = 0; i < noNewData; i++) {

        // how many machines at testing facilities
        //if (noMachines.active) {
        if (false) {
    
            // no of machines 1 to 4
            if (noMachines.upperBound > 0) {
                    
                 topInterval = noMachines.upperBound;
                 botInterval = noMachines.lowerBound;
                 base = noMachines.mean;
            }
            else if (wideOpenFlag) {

                topInterval = 4;
                botInterval = 0;
                base = newData[i].noMachinesTest1;
            }
            else {

                topInterval = 4;
                botInterval = 1;
                base = newData[i].noMachinesTest1;
            }

            interval = topInterval - botInterval;

            if (debug) std::cout << "input-change:  no machines before reshape i = " <<  i << ", " <<  newData[i].noMachinesTest1 <<  ", topInterval = " << topInterval << ", botInterval = " << botInterval <<  "\n";
            
            if (randomFlag) {
        
                randInt = getDistRandInt("noMachines", noMachines.distribution, base, topInterval, botInterval, noMachines.shape1Alpha, noMachines.shape2Beta);
            }
            else {

                randInt = interval/noBins * binNumber;
                std::cout << "input-change:  no machines orig = " << botInterval <<  ", interval = " << interval << ", randInt = " << randInt << ", value = " << botInterval+randInt <<  "\n";
            }

            if (debug) {
            
                std::cout << "input-change:  no. machines for test1 randint  = " <<  randInt << "\n";
                std::cout << "input-change:  no. machines before reshape i = " <<  i << ", " <<  newData[i].noMachinesTest1 <<  ", topInterval = " << topInterval << ", botInterval = " << botInterval <<  "\n";
            }

            // no of machines 1 to 4
            if (noMachines.upperBound > 0) {
                    
                 topInterval2 = noMachines.upperBound;
                 botInterval2 = noMachines.lowerBound;
                 base = noMachines.mean;
            }
            else if (wideOpenFlag) {

                topInterval2 = 4;
                botInterval2 = 0;
                base = newData[i].noMachinesTest2;
            }
            else {

                topInterval2 = 4;
                botInterval2 = 1;
                base = newData[i].noMachinesTest2;
            }
            
            interval2 = topInterval2 - botInterval2;

            if (randomFlag) {

                randInt2 = getDistRandInt("noMachines", noMachines.distribution, base, topInterval2, botInterval2, noMachines.shape1Alpha, noMachines.shape2Beta);
            }
            else {

                randInt2 = interval2/noBins * binNumber;
                std::cout << "input-change:  no machines orig = " << botInterval <<  ", interval = " << interval << ", randInt = " << randInt << ", value = " << botInterval+randInt <<  "\n";
            }

            if (debug) std::cout << "input-change:  no. machines for test2 randint  = " <<  randInt2 << "\n";
        }
    
        // how often do a batch of samples get sent out = currently 2 time steps for away testing
        if (batchWait.active) {

            // batchWait = 1 to 7 timestemps
            if (batchWait.upperBound > 0) {
                    
                 topInterval = batchWait.upperBound;
                 botInterval = batchWait.lowerBound;
                 base = batchWait.mean;
            }
            else if (wideOpenFlag) {

                topInterval = 18;
                botInterval = 1;
                base = newData[i].batchWait;
            }
            else {

                topInterval = 5;
                botInterval = 1;
                base = newData[i].batchWait;
            }

            interval = 6;

            if (randomFlag) {
                
                randInt = getDistRandInt("batchWait", batchWait.distribution, base, topInterval, botInterval, batchWait.shape1Alpha, batchWait.shape2Beta);
            }
            else {

                randInt = interval/noBins * binNumber;
                if (debug2) std::cout << "input-change:  sample wait orig = " << newData[i].batchWait <<  ", interval = " << interval << ", randInt = " << randInt << ", value = " << botInterval+randInt <<  "\n";
            }
                
            if (debug2) std::cout << "input-change: inside input change sample wait randInt =  " <<  randInt << "\n";

            newData[i].batchWait = botInterval + randInt;
        }

        for (int i = 0; i < noNewData; i++) {
   
            // only make changes for current disease
            if (newData[i].disease  == currentDisease) {
            
                if (noMachines.active) {
                
                    newData[i].noMachinesTest1 = botInterval + randInt;
                    newData[i].noMachinesTest2 = botInterval2 + randInt2;
                }
            }
        }

        if (debug) std::cout << "input-change:  bottom of noNewData loop i = " << i <<   "\n";
    }

    if (debug) std::cout << "input-change:  after loop before output facility-specific-data"  <<  "\n";

    // open aggregate file and output values
    myfile.open("./stats/" + scenario + "/job-" + jobNo + "/input/facility-specific-data/facility-specific-data.csv");

    myfile << "" << ",Name" << ",no. of diseases" << ",disease" << ",Batch wait time for sample send out";
    myfile << ",Return time for sample results" << ",No. of tests" << ",test1 name" << ",Testing test1" << ",Priority 1" << ",no. of machines1";
    myfile << ",max reagents per cycle1" << ",timestep in reagent cycle1";
    myfile << ",test2 name" << ",testing test2" << ",Priority 2" << ",no. of Machines" << ",max reagents per cycle2" << ",timestep in reagent cycle2";
    myfile << "," << "," << ",District" <<  "\n";

    // now add newData to agData
    for (int i = 0; i < noNewData; i++) {
        
        myfile << newData[i].number << "," << newData[i].name<< "," << newData[i].noDiseases;
        myfile << "," << newData[i].disease << "," << newData[i].batchWait << "," << newData[i].returnTime << "," << newData[i].noTests;
        myfile << "," << newData[i].test1Name << "," << newData[i].test1Testing << "," << newData[i].test1Priority << "," << newData[i].noMachinesTest1;
        myfile << "," << newData[i].maxRegentsTest1 << "," << newData[i].timeStepsInCycleTest1 << "," << newData[i].test2Name << "," << newData[i].test2Testing;
        myfile << "," << newData[i].test2Priority << "," << newData[i].noMachinesTest2 << "," << newData[i].maxRegentsTest2 << "," << newData[i].timeStepsInCycleTest2;
        myfile << "," << "," << "," << newData[i].district << "\n";
    }
    myfile.close();
    
    if (debug) std::cout << "input-change: leaving facility file" <<  "\n";

    // patient return time in case we need to change it in test.csv
    returnArray[0] = newData[0].batchWait;

}




// copy facility parameters from one facility file and swap them in another facility file
void inputCopyFacility(vector<vector<string>> lastFacilitySpecificInput, vector<vector<string>> facilitySpecificInput, string facilitySpecific, string scenario, string jobNo ){

    int noNewData;
    int noOldData;
    bool debug = false;
    bool debug2 = false;

    ofstream myfile;

    struct facilityStruct {
        string number;
        string name;
        int noDiseases;
        string disease;
        int batchWait;
        int returnTime;
        int noTests;
        string test1Name;
        int test1Testing;
        int test1Priority;
        int noMachinesTest1;
        int maxRegentsTest1;
        int timeStepsInCycleTest1;
        string test2Name;
        int test2Testing;
        int test2Priority;
        int noMachinesTest2;
        int maxRegentsTest2;
        int timeStepsInCycleTest2;
        string district;
    };

    noNewData = facilitySpecificInput.size();
    noOldData = lastFacilitySpecificInput.size();

    facilityStruct* newData = new facilityStruct[noNewData];
    facilityStruct* oldData = new facilityStruct[noOldData];

    if (debug) std::cout << "inside copy facility:  input = " << noNewData  <<  "\n";

    // skip the first row, it contains column headers
    // format is : id, Name, no. of diseases,disease, Batch wait time for sample send out, Return time for sample results, No. of tests, test1 name, Testing test1,
    //          priority of test, no. of machines1, max reagents per cycle1, timestep in reagent cycle1, test2 name, testing test2, no. of Machines, max reagents per cycle2,
    //          timestep in reagent cycle2, , ,District
    for (auto rdIndex = 1u; rdIndex < facilitySpecificInput.size(); rdIndex++) {

        if (debug2) {
            std::cout << facilitySpecificInput[rdIndex][0] << ' ' << facilitySpecificInput[rdIndex][1] << ' ' << facilitySpecificInput[rdIndex][2] << ' ' << facilitySpecificInput[rdIndex][4] << ' ' << facilitySpecificInput[rdIndex][5];
            std::cout << ' ' << facilitySpecificInput[rdIndex][6] << ' ' << facilitySpecificInput[rdIndex][7] << ' ' << facilitySpecificInput[rdIndex][8] << ' ' << facilitySpecificInput[rdIndex][9];
            std::cout << ' ' << facilitySpecificInput[rdIndex][10] << ' ' << facilitySpecificInput[rdIndex][11] << ' ' << facilitySpecificInput[rdIndex][12] << ' ' << facilitySpecificInput[rdIndex][13]  << '\n';
        }

        newData[rdIndex-1].number = facilitySpecificInput[rdIndex][0];
        newData[rdIndex-1].name = facilitySpecificInput[rdIndex][1];
        newData[rdIndex-1].noDiseases = stoi(facilitySpecificInput[rdIndex][2]);
        newData[rdIndex-1].disease = facilitySpecificInput[rdIndex][3];
        newData[rdIndex-1].batchWait = stoi(facilitySpecificInput[rdIndex][4]);
        newData[rdIndex-1].returnTime = stoi(facilitySpecificInput[rdIndex][5]);
        newData[rdIndex-1].noTests = stoi(facilitySpecificInput[rdIndex][6]);
        newData[rdIndex-1].test1Name = facilitySpecificInput[rdIndex][7];
        newData[rdIndex-1].test1Testing = stoi(facilitySpecificInput[rdIndex][8]);
        newData[rdIndex-1].test1Priority = stoi(facilitySpecificInput[rdIndex][9]);
        newData[rdIndex-1].noMachinesTest1 = stoi(facilitySpecificInput[rdIndex][10]);
        newData[rdIndex-1].maxRegentsTest1 = stoi(facilitySpecificInput[rdIndex][11]);
        newData[rdIndex-1].timeStepsInCycleTest1 = stoi(facilitySpecificInput[rdIndex][12]);
        newData[rdIndex-1].test2Name = facilitySpecificInput[rdIndex][13];
        newData[rdIndex-1].test2Testing = stoi(facilitySpecificInput[rdIndex][14]);
        newData[rdIndex-1].test2Priority = stoi(facilitySpecificInput[rdIndex][15]);
        newData[rdIndex-1].noMachinesTest2 = stoi(facilitySpecificInput[rdIndex][16]);
        newData[rdIndex-1].maxRegentsTest2 = stoi(facilitySpecificInput[rdIndex][17]);
        newData[rdIndex-1].timeStepsInCycleTest2 = stoi(facilitySpecificInput[rdIndex][18]);
        newData[rdIndex-1].district = facilitySpecificInput[rdIndex][21];

        if (debug2) {
            std::cout << newData[rdIndex-1].name << ' ' << newData[rdIndex-1].noDiseases << ' ' << newData[rdIndex-1].disease << ' ' << newData[rdIndex-1].batchWait;
            std::cout << ' ' << newData[rdIndex-1].returnTime << ' ' << newData[rdIndex-1].noTests << ' ' << newData[rdIndex-1].test1Name;
            std::cout << ' ' << newData[rdIndex-1].test1Testing << ' ' << newData[rdIndex-1].test1Priority << ' ' << newData[rdIndex-1].noMachinesTest1;
            std::cout << ' ' << newData[rdIndex-1].maxRegentsTest1 << ' ' << newData[rdIndex-1].timeStepsInCycleTest1 << ' ' << newData[rdIndex-1].test2Name;
            std::cout << ' ' << newData[rdIndex-1].test2Testing << ' ' << newData[rdIndex-1].test2Priority;
            std::cout << ' ' << newData[rdIndex-1].noMachinesTest2 << ' ' << newData[rdIndex-1].maxRegentsTest2 << ' ' << newData[rdIndex-1].timeStepsInCycleTest2<< '\n';
        }
    }
     
    // adjust for no header row
    noNewData = noNewData - 1;

    if (debug) std::cout << "facility-copy: after read new data  " << "\n";

    // same thing for old data
    for (auto rdIndex = 1u; rdIndex < lastFacilitySpecificInput.size(); rdIndex++) {

        if (debug2) {
            std::cout << lastFacilitySpecificInput[rdIndex][0] << ' ' << lastFacilitySpecificInput[rdIndex][1] << ' ' << lastFacilitySpecificInput[rdIndex][2] << ' ' << lastFacilitySpecificInput[rdIndex][4] << ' ' << facilitySpecificInput[rdIndex][5];
            std::cout << ' ' << lastFacilitySpecificInput[rdIndex][6] << ' ' << lastFacilitySpecificInput[rdIndex][7] << ' ' << lastFacilitySpecificInput[rdIndex][8] << ' ' << lastFacilitySpecificInput[rdIndex][9];
            std::cout << ' ' << lastFacilitySpecificInput[rdIndex][10] << ' ' << lastFacilitySpecificInput[rdIndex][11] << ' ' << lastFacilitySpecificInput[rdIndex][12] << ' ' << lastFacilitySpecificInput[rdIndex][13]  << '\n';
        }

        oldData[rdIndex-1].number = lastFacilitySpecificInput[rdIndex][0];
        oldData[rdIndex-1].name = lastFacilitySpecificInput[rdIndex][1];
        oldData[rdIndex-1].noDiseases = stoi(lastFacilitySpecificInput[rdIndex][2]);
        oldData[rdIndex-1].disease = lastFacilitySpecificInput[rdIndex][3];
        oldData[rdIndex-1].batchWait = stoi(lastFacilitySpecificInput[rdIndex][4]);
        oldData[rdIndex-1].returnTime = stoi(lastFacilitySpecificInput[rdIndex][5]);
        oldData[rdIndex-1].noTests = stoi(lastFacilitySpecificInput[rdIndex][6]);
        oldData[rdIndex-1].test1Name = lastFacilitySpecificInput[rdIndex][7];
        oldData[rdIndex-1].test1Testing = stoi(lastFacilitySpecificInput[rdIndex][8]);
        oldData[rdIndex-1].test1Priority = stoi(lastFacilitySpecificInput[rdIndex][9]);
        oldData[rdIndex-1].noMachinesTest1 = stoi(lastFacilitySpecificInput[rdIndex][10]);
        oldData[rdIndex-1].maxRegentsTest1 = stoi(lastFacilitySpecificInput[rdIndex][11]);
        oldData[rdIndex-1].timeStepsInCycleTest1 = stoi(lastFacilitySpecificInput[rdIndex][12]);
        oldData[rdIndex-1].test2Name = lastFacilitySpecificInput[rdIndex][13];
        oldData[rdIndex-1].test2Testing = stoi(lastFacilitySpecificInput[rdIndex][14]);
        oldData[rdIndex-1].test2Priority = stoi(lastFacilitySpecificInput[rdIndex][15]);
        oldData[rdIndex-1].noMachinesTest2 = stoi(lastFacilitySpecificInput[rdIndex][16]);
        oldData[rdIndex-1].maxRegentsTest2 = stoi(lastFacilitySpecificInput[rdIndex][17]);
        oldData[rdIndex-1].timeStepsInCycleTest2 = stoi(lastFacilitySpecificInput[rdIndex][18]);
        oldData[rdIndex-1].district = lastFacilitySpecificInput[rdIndex][21];

        if (debug2) {
            std::cout << oldData[rdIndex-1].name << ' ' << oldData[rdIndex-1].noDiseases << ' ' << oldData[rdIndex-1].disease << ' ' << oldData[rdIndex-1].batchWait;
            std::cout << ' ' << oldData[rdIndex-1].returnTime << ' ' << oldData[rdIndex-1].noTests << ' ' << oldData[rdIndex-1].test1Name;
            std::cout << ' ' << oldData[rdIndex-1].test1Testing << ' ' << oldData[rdIndex-1].test1Priority << ' ' << oldData[rdIndex-1].noMachinesTest1;
            std::cout << ' ' << oldData[rdIndex-1].maxRegentsTest1 << ' ' << oldData[rdIndex-1].timeStepsInCycleTest1 << ' ' << oldData[rdIndex-1].test2Name;
            std::cout << ' ' << oldData[rdIndex-1].test2Testing << ' ' << oldData[rdIndex-1].test2Priority;
            std::cout << ' ' << oldData[rdIndex-1].noMachinesTest2 << ' ' << oldData[rdIndex-1].maxRegentsTest2 << ' ' << oldData[rdIndex-1].timeStepsInCycleTest2<< '\n';
        }
    }
     
    // adjust for no header row
    noOldData = noOldData - 1;

    if (debug) std::cout << "facility-copy: after read old data "  << "\n";

    for (int i = 0; i < noNewData; i++) {

        // copy certain columns from lastFacilitySpecificDatata to facilitySpecificData
        // copy the testing priority and whether a test is done here
        newData[i].test1Testing = oldData[i].test1Testing;
        newData[i].test1Priority = oldData[i].test1Priority;
        newData[i].test2Testing = oldData[i].test2Testing;
        newData[i].test2Priority = oldData[i].test2Priority;

        if (debug) std::cout << "facility-copy:  bottom of noNewData loop i = " << i <<   "\n";
    }

    if (debug) std::cout << "facility-copy:  after loop before output facility-specific-data"  <<  "\n";

    // open aggregate file and output values
    myfile.open("./stats/" + scenario + "/job-" + jobNo + "/input/facility-specific-data/facility-specific-data.csv");

    myfile << "" << ",Name" << ",no. of diseases" << ",disease" << ",Batch wait time for sample send out";
    myfile << ",Return time for sample results" << ",No. of tests" << ",test1 name" << ",Testing test1" << ",Priority 1" << ",no. of machines1";
    myfile << ",max reagents per cycle1" << ",timestep in reagent cycle1";
    myfile << ",test2 name" << ",testing test2" << ",Priority 2" << ",no. of Machines" << ",max reagents per cycle2" << ",timestep in reagent cycle2";
    myfile << "," << "," << ",District" <<  "\n";

    // now add newData to agData
    for (int i = 0; i < noNewData; i++) {
        
        myfile << newData[i].number << "," << newData[i].name<< "," << newData[i].noDiseases;
        myfile << "," << newData[i].disease << "," << newData[i].batchWait << "," << newData[i].returnTime << "," << newData[i].noTests;
        myfile << "," << newData[i].test1Name << "," << newData[i].test1Testing << "," << newData[i].test1Priority << "," << newData[i].noMachinesTest1;
        myfile << "," << newData[i].maxRegentsTest1 << "," << newData[i].timeStepsInCycleTest1 << "," << newData[i].test2Name << "," << newData[i].test2Testing;
        myfile << "," << newData[i].test2Priority << "," << newData[i].noMachinesTest2 << "," << newData[i].maxRegentsTest2 << "," << newData[i].timeStepsInCycleTest2;
        myfile << "," << "," << "," << newData[i].district << "\n";
    }
    myfile.close();
    
    if (debug) std::cout << "facility-copy: leaving facility copy file" <<  "\n";
}


      
// change test parameters
void inputChangeTest(vector<vector<string>> testInput, string tests, string currentDisease, parameters maxReagent, parameters timeStepsPerCycle, parameters sampleWait, parameters sensitivity, parameters sensitivityPCR, parameters sensitivityXpert, parameters specificity, parameters specificityPCR, parameters specificityXpert, parameters MDSensitivity, parameters MDSpecificity, bool randomFlag, int noBins, int binNumber, float alpha, bool inPlaceFlag, int bWait, string scenario, string jobNo, bool wideOpenFlag){

    int noNewData;
    int randInt;
    float base;
    float randFloat;
    float topInterval;
    float botInterval;
    float interval;
    bool debug = false;
    bool debug2 = false;

    ofstream myfile;

    struct testStruct {
        string name;
        string disease;
        int priority;
        int batchWait;
        int patientReturnTime;
        int maxTestPerMachine;
        int noUsesPerTimeStep;
        int maxTestsPerDay;
        int testThisCycle;
        int maxTests;
        int reagentCycle;
        float sensitivity;
        float sensitivityPCR;
        float sensitivityXpert;
        float specificity;
        float specificityPCR;
        float specificityXpert;
        float sensitivityMD;
        float specificityMD;
        float sensitivityMDWithTest;
        float specificityMDWithTest;
    };

    noNewData = testInput.size();

    testStruct* newData = new testStruct[noNewData];

    // debug - true
    if (debug) std::cout << "input-change: inside change test input = " << noNewData  << ", disease = " << currentDisease << "\n";

    // skip the first row, it contains column headers
    // format is : name, disease, batch wait time, patient return time, maxTestPerMachine, noUsesPerTimeStep, maxTestsPerDay, testThisCycle, maxTests, reagentCycle, testPriority, sensitivity, specificity;
    //       sensitivityMD, specificityMD, sensitivityMDWithTest, specificityMDWithTest, tier0, tier1, tier2, tier3, tier4, tier5, tier4, tier7, tier8, tier9
    for (auto rdIndex = 1u; rdIndex < testInput.size(); rdIndex++) {

        if (debug2) {
            std::cout << testInput[rdIndex][0] << ' ' << testInput[rdIndex][1] << ' ' << testInput[rdIndex][2] << ' ' << testInput[rdIndex][4] << ' ' << testInput[rdIndex][5];
            std::cout << ' ' << testInput[rdIndex][6] << ' ' << testInput[rdIndex][7] << ' ' << testInput[rdIndex][8] << ' ' << testInput[rdIndex][9];
            std::cout << ' ' << testInput[rdIndex][10] << ' ' << testInput[rdIndex][11] << ' ' << testInput[rdIndex][12] << ' ' << testInput[rdIndex][13]  << '\n';
        }

        newData[rdIndex-1].name = testInput[rdIndex][0];
        newData[rdIndex-1].disease = testInput[rdIndex][1];
        newData[rdIndex-1].priority = stoi(testInput[rdIndex][2]);
        newData[rdIndex-1].batchWait = stoi(testInput[rdIndex][3]);
        newData[rdIndex-1].patientReturnTime = stoi(testInput[rdIndex][4]);
        newData[rdIndex-1].maxTestPerMachine = stoi(testInput[rdIndex][5]);
        newData[rdIndex-1].noUsesPerTimeStep = stoi(testInput[rdIndex][6]);
        newData[rdIndex-1].maxTestsPerDay = stoi(testInput[rdIndex][7]);
        newData[rdIndex-1].testThisCycle = stoi(testInput[rdIndex][8]);
        newData[rdIndex-1].maxTests = stoi(testInput[rdIndex][9]);
        newData[rdIndex-1].reagentCycle = stoi(testInput[rdIndex][10]);
        newData[rdIndex-1].sensitivity = stof(testInput[rdIndex][11]);
        newData[rdIndex-1].specificity = stof(testInput[rdIndex][12]);
        newData[rdIndex-1].sensitivityMD = stof(testInput[rdIndex][13]);
        newData[rdIndex-1].specificityMD = stof(testInput[rdIndex][14]);
        newData[rdIndex-1].sensitivityMDWithTest = stof(testInput[rdIndex][15]);
        newData[rdIndex-1].specificityMDWithTest = stof(testInput[rdIndex][16]);

        // debug2 - true
        if (debug2) {
            std::cout << newData[rdIndex-1].name << ' ' << newData[rdIndex-1].disease << ' ' << newData[rdIndex-1].priority;
            std::cout << ' ' << newData[rdIndex-1].batchWait << ' ' << newData[rdIndex-1].patientReturnTime << ' ' << newData[rdIndex-1].maxTestPerMachine << ' ' << newData[rdIndex-1].noUsesPerTimeStep;
            std::cout << ' ' << newData[rdIndex-1].maxTestsPerDay << ' ' << newData[rdIndex-1].testThisCycle << ' ' << newData[rdIndex-1].maxTests;
            std::cout << ' ' << newData[rdIndex-1].reagentCycle << ' ' << newData[rdIndex-1].sensitivity << ' ' << newData[rdIndex-1].specificity << '\n';
        }
    }
     
    // adjust for no header row
    noNewData = noNewData - 1;

    if (debug) std::cout << "input-change: inside input change test parameters  " << tests << " no of data = " << noNewData << "\n";

    // seed random number generator - moved into randInt and randFloat routines
    //unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    //std::mt19937 generator(seed);

     // now add newData to agData
    for (int i = 0; i < noNewData; i++) {
    
        // only change if matches current Disease
        if (newData[i].disease  == currentDisease) {

            // debug2 - true
            if (debug2) {
                std::cout << "maxReagent.active = " << maxReagent.active <<  ",timeStepsPerCycle.active = " << timeStepsPerCycle.active << ",sampleWait.active = " << sampleWait.active;
                std::cout << "sensitivity.active = " << sensitivity.active << "sensitivityPCR.active = " << sensitivityPCR.active << "sensitivityXpert.active = " << sensitivityXpert.active;
                std::cout << ",specificity.active = " << specificity.active << ",specificityPCR.active = " << specificityPCR.active << ",specificityXpert.active = " << specificityXpert.active;
                std::cout << ",MDSensitivity.active = " << MDSensitivity.active;
                std::cout << "MDSpecificity.active = " << MDSpecificity.active << '\n';
            }
        
            // how many reagents in a cycle =  no. of time steps in cycle * max tests per machine * no. of machines * no. of times a machine can be used pre time step  (Xpert = 560, smear 672)
            if (maxReagent.active) {

                // maxTests +- 50%
                if (maxReagent.upperBound > 0) {
                    
                    topInterval = maxReagent.upperBound;
                    botInterval = maxReagent.lowerBound;
                    base = maxReagent.mean;
                }
                else if (wideOpenFlag) {

                    topInterval = 5000;
                    botInterval = 0;
                    base = newData[i].maxTests;
                }
                else {

                    topInterval = newData[i].maxTests + newData[i].maxTests/2;
                    botInterval = newData[i].maxTests - newData[i].maxTests/2;
                    base = newData[i].maxTests;
                }

                interval = topInterval - botInterval;

                if (randomFlag) {
                
                    randInt = getDistRandInt("maxReagent", maxReagent.distribution, base, topInterval, botInterval, maxReagent.shape1Alpha, maxReagent.shape2Beta);
                }
                else {

                    randInt = interval/noBins * binNumber;
                    if (debug2) std::cout << "input-change:  max reagent orig = " << newData[i].maxTests <<  ", interval = " << interval << ", randInt = " << randInt << ", value = " << botInterval+randInt <<  "\n";
                }

                if (debug2) std::cout << "input-change: inside input change max reagent randInt =  " <<  randInt << "\n";
            
                newData[i].maxTests = botInterval + randInt;
            }
    
            // How often to reagents get restocked - current baseline is 28 +- 14 time steps
            if (timeStepsPerCycle.active) {

                // reagentCycle +- 50%
                if (timeStepsPerCycle.upperBound > 0) {
                    
                    topInterval = timeStepsPerCycle.upperBound;
                    botInterval = timeStepsPerCycle.lowerBound;
                    base = timeStepsPerCycle.mean;
                }
                else if (wideOpenFlag) {

                    topInterval = 60;
                    botInterval = 7;
                    base = newData[i].reagentCycle;
                }
                else {

                    topInterval = newData[i].reagentCycle + newData[i].reagentCycle/2;
                    botInterval = newData[i].reagentCycle - newData[i].reagentCycle/2;
                    base = newData[i].reagentCycle;
                }

                interval = topInterval - botInterval;

                if (randomFlag) {
                
                    randInt = getDistRandInt("timeStepsPerCycle", timeStepsPerCycle.distribution, base, topInterval, botInterval, timeStepsPerCycle.shape1Alpha, timeStepsPerCycle.shape2Beta);
                }
                else {

                    randInt = interval/noBins * binNumber;
                    if (debug2) std::cout << "input-change:  reagent cycle orig = " << newData[i].reagentCycle <<  ", interval = " << interval << ", randInt = " << randInt << ", value = " << botInterval+randInt <<  "\n";
                }
                // debug2 - true
                if (debug2) std::cout << "input-change: inside input change reagent cycle randInt =  " <<  randInt << "\n";

                newData[i].reagentCycle = botInterval + randInt;
            }
    
            // how often do a batch of samples get sent out = currently 2 time steps for away testing
            if (sampleWait.active) {

                newData[i].batchWait = bWait;

                // debug2 - true
                if (debug2) std::cout << "input-change: inside input change sample wait patient wait time =  " <<  newData[i].batchWait << " i = " << i <<  "\n";
            }

            // test sensitivity = 0 - 100 %
            if (sensitivity.active) {

                // for example if sensitivity is .7 on a range of 0-1 then take 1-.7 to get .3.  vary .7 by .3/2 in each direction to keep sensitivity in the 0-1 interval
                if (sensitivity.upperBound > 0) {
                    
                    topInterval = sensitivity.upperBound;
                    botInterval = sensitivity.lowerBound;
                    base = sensitivity.mean;
                }
                else if (wideOpenFlag) {

                    topInterval = 100;
                    botInterval = newData[i].sensitivity - (100 - newData[i].sensitivity);
                    base = newData[i].sensitivity;
                }
                else {

                    //topInterval = newData[i].sensitivity + (100 - newData[i].sensitivity)/2;
                    topInterval = newData[i].sensitivity + int((100 - newData[i].sensitivity)*1.5);
                    botInterval = newData[i].sensitivity - (100 - newData[i].sensitivity)/2;
                    base = newData[i].sensitivity;
                }

                interval = topInterval - botInterval;

                if (interval > 0) {
            
                    if (randomFlag) {
                    
                        randFloat = getDistRandFloat("sensitivity", sensitivity.distribution, base, topInterval, botInterval, sensitivity.shape1Alpha, sensitivity.shape2Beta, sensitivity.scaleHB);
                    }
                    else {

                        randFloat = interval/noBins * binNumber;
                    }
                    if (debug2) std::cout << "input-change:  sensitivity orig = " << newData[i].sensitivity <<  ", interval = " << interval << ", randFloat = " << randFloat << ", value = " << botInterval+randInt <<  "\n";
                }
                else {
                    randFloat = 0;
                }
            
                // debug - true
                if (debug) std::cout << "input-change: inside input change sensitivity = " << newData[i].sensitivity <<  ", randFloat =  " <<  randFloat << " botInterval =  " <<  botInterval <<  "\n";

                if (sensitivity.distribution == "Beta") {

                    newData[i].sensitivity = randFloat;
                }
                else {

                    newData[i].sensitivity = botInterval + randFloat;
                }
            }
    
            // test sensitivityPCR = will override global test sensitivity
            if ((sensitivityPCR.active) && (newData[i].name == "PCR")) {

                if (debug) std::cout << "input-change: inside input change sensitivityPCR test = " << newData[i].name << "\n";
                
                // for example if sensitivityPCR is .7 on a range of 0-1 then take 1-.7 to get .3.  vary .7 by .3/2 in each direction to keep sensitivityPCR in the 0-1 interval
                if (sensitivityPCR.upperBound > 0) {

                    topInterval = sensitivityPCR.upperBound;
                    botInterval = sensitivityPCR.lowerBound;
                    base = sensitivityPCR.mean;
                }
                else if (wideOpenFlag) {

                    topInterval = 100;
                    botInterval = newData[i].sensitivityPCR - (100 - newData[i].sensitivityPCR);
                    base = newData[i].sensitivityPCR;
                }
                else {

                    //topInterval = newData[i].sensitivityPCR + (100 - newData[i].sensitivityPCR)/2;
                    topInterval = newData[i].sensitivityPCR + int((100 - newData[i].sensitivityPCR) * 1.5);
                    botInterval = newData[i].sensitivityPCR - (100 - newData[i].sensitivityPCR) / 2;
                    base = newData[i].sensitivityPCR;
                }

                interval = topInterval - botInterval;

                if (interval > 0) {

                    if (randomFlag) {

                        randFloat = getDistRandFloat("sensitivityPCR", sensitivityPCR.distribution, base, topInterval, botInterval, sensitivityPCR.shape1Alpha, sensitivityPCR.shape2Beta, sensitivityPCR.scaleHB);
                    }
                    else {

                        randFloat = interval / noBins * binNumber;
                    }
                    if (debug2) std::cout << "input-change:  sensitivityPCR orig = " << newData[i].sensitivityPCR << ", interval = " << interval << ", randFloat = " << randFloat << ", value = " << botInterval + randInt << "\n";
                }
                else {
                    randFloat = 0;
                }

                // debug - true
                if (debug) std::cout << "input-change: inside input change sensitivityPCR = " << newData[i].sensitivityPCR << ", randFloat =  " << randFloat << " botInterval =  " << botInterval << "\n";

                if (sensitivityPCR.distribution == "Beta") {

                    newData[i].sensitivity = randFloat;
                }
                else {

                    newData[i].sensitivity = botInterval + randFloat;
                }
            }

            // test sensitivityXpert = will override global test sensitivity
            if ((sensitivityXpert.active) && (newData[i].name == "Xpert")) {

                // for example if sensitivityXpert is .7 on a range of 0-1 then take 1-.7 to get .3.  vary .7 by .3/2 in each direction to keep sensitivityXpert in the 0-1 interval
                if (sensitivityXpert.upperBound > 0) {

                    topInterval = sensitivityXpert.upperBound;
                    botInterval = sensitivityXpert.lowerBound;
                    base = sensitivityXpert.mean;
                }
                else if (wideOpenFlag) {

                    topInterval = 100;
                    botInterval = newData[i].sensitivityXpert - (100 - newData[i].sensitivityXpert);
                    base = newData[i].sensitivityXpert;
                }
                else {

                    //topInterval = newData[i].sensitivityXpert + (100 - newData[i].sensitivityXpert)/2;
                    topInterval = newData[i].sensitivityXpert + int((100 - newData[i].sensitivityXpert) * 1.5);
                    botInterval = newData[i].sensitivityXpert - (100 - newData[i].sensitivityXpert) / 2;
                    base = newData[i].sensitivityXpert;
                }

                interval = topInterval - botInterval;

                if (interval > 0) {

                    if (randomFlag) {

                        randFloat = getDistRandFloat("sensitivityXpert", sensitivityXpert.distribution, base, topInterval, botInterval, sensitivityXpert.shape1Alpha, sensitivityXpert.shape2Beta, sensitivityXpert.scaleHB);
                    }
                    else {

                        randFloat = interval / noBins * binNumber;
                    }
                    if (debug2) std::cout << "input-change:  sensitivityXpert orig = " << newData[i].sensitivityXpert << ", interval = " << interval << ", randFloat = " << randFloat << ", value = " << botInterval + randInt << "\n";
                }
                else {
                    randFloat = 0;
                }

                // debug - true
                if (debug) std::cout << "input-change: inside input change sensitivityXpert = " << newData[i].sensitivityXpert << ", randFloat =  " << randFloat << " botInterval =  " << botInterval << "\n";

                if (sensitivityXpert.distribution == "Beta") {

                    newData[i].sensitivity = randFloat;
                }
                else {

                    newData[i].sensitivity = botInterval + randFloat;
                }
            }

            // test specficity = 0 - 100 %
            if (specificity.active) {

                // for example if specificity is .7 on a range of 0-1 then take 1-.7 to get .3.  vary .7 by .3/2 in each direction to keep specificity in the 0-1 interval
                if (debug) std::cout << "input-change: inside input change specificity = " <<  "\n";

                if (specificity.upperBound > 0) {
                    
                    topInterval = specificity.upperBound;
                    botInterval = specificity.lowerBound;
                    base = specificity.mean;
                }
                else if (wideOpenFlag) {

                    topInterval = 100;
                    botInterval = newData[i].specificity - (100 - newData[i].specificity);
                    base = newData[i].specificity;
                }
                else {

                    topInterval = newData[i].specificity + (100 - newData[i].specificity)/2;
                    botInterval = newData[i].specificity - (100 - newData[i].specificity)/2;
                    base = newData[i].specificity;
                }

                interval = topInterval - botInterval;

                 if (debug) std::cout << "input-change:  specificity orig = " << newData[i].specificity <<  ", interval = " << interval << ", topInterval = " << topInterval << ", botInterval = " << botInterval <<  "\n";

                if (interval > 0) {
                
                    if (randomFlag) {

                        randFloat = getDistRandFloat("specificity", specificity.distribution, base, topInterval, botInterval, specificity.shape1Alpha, specificity.shape2Beta, specificity.scaleHB);
                    }
                    else {

                        randFloat = interval/noBins * binNumber;
                    }
                    if (debug2) std::cout << "input-change:  specificity orig = " << newData[i].specificity <<  ", interval = " << interval << ", randFloat = " << randFloat << ", value = " << botInterval+randInt <<  "\n";
                }
                else {
                    randFloat = 0;
                }
            
                //debug
                if (debug) std::cout << "input-change: inside input change specificity = " << newData[i].specificity <<  ", randFloat =  " <<  randFloat << " botInterval =  " <<  botInterval <<  "\n";

                if (specificity.distribution == "Beta") {

                    newData[i].specificity = randFloat;
                }
                else {

                    newData[i].specificity = botInterval + randFloat;
                }
            }
    
            // test specficity for PCR tests, will override global specificity for tests
            if ((specificityPCR.active) && (newData[i].name == "PCR")){

                // for example if specificity is .7 on a range of 0-1 then take 1-.7 to get .3.  vary .7 by .3/2 in each direction to keep specificity in the 0-1 interval
                if (debug) std::cout << "input-change: inside input change specificityPCR = " << "\n";

                if (specificityPCR.upperBound > 0) {

                    topInterval = specificityPCR.upperBound;
                    botInterval = specificityPCR.lowerBound;
                    base = specificityPCR.mean;
                }
                else if (wideOpenFlag) {

                    topInterval = 100;
                    botInterval = newData[i].specificityPCR - (100 - newData[i].specificityPCR);
                    base = newData[i].specificityPCR;
                }
                else {

                    topInterval = newData[i].specificityPCR + (100 - newData[i].specificityPCR) / 2;
                    botInterval = newData[i].specificityPCR - (100 - newData[i].specificityPCR) / 2;
                    base = newData[i].specificityPCR;
                }

                interval = topInterval - botInterval;

                if (debug) std::cout << "input-change:  specificityPCR orig = " << newData[i].specificityPCR << ", interval = " << interval << ", topInterval = " << topInterval << ", botInterval = " << botInterval << "\n";

                if (interval > 0) {

                    if (randomFlag) {

                        randFloat = getDistRandFloat("specificityPCR", specificityPCR.distribution, base, topInterval, botInterval, specificityPCR.shape1Alpha, specificityPCR.shape2Beta, specificityPCR.scaleHB);
                    }
                    else {

                        randFloat = interval / noBins * binNumber;
                    }
                    if (debug2) std::cout << "input-change:  specificityPCR orig = " << newData[i].specificityPCR << ", interval = " << interval << ", randFloat = " << randFloat << ", value = " << botInterval + randInt << "\n";
                }
                else {
                    randFloat = 0;
                }

                //debug
                if (debug) std::cout << "input-change: inside input change specificityPCR = " << newData[i].specificityPCR << ", randFloat =  " << randFloat << " botInterval =  " << botInterval << "\n";

                if (specificityPCR.distribution == "Beta") {

                    newData[i].specificity = randFloat;
                }
                else {

                    newData[i].specificity = botInterval + randFloat;
                }
            }

            // test specficity for Xpert tests, will override global specificity for tests
            if ((specificityXpert.active) && (newData[i].name == "Xpert")) {

                // for example if specificity is .7 on a range of 0-1 then take 1-.7 to get .3.  vary .7 by .3/2 in each direction to keep specificity in the 0-1 interval
                if (debug) std::cout << "input-change: inside input change specificityXpert = " << "\n";

                if (specificityXpert.upperBound > 0) {

                    topInterval = specificityXpert.upperBound;
                    botInterval = specificityXpert.lowerBound;
                    base = specificityXpert.mean;
                }
                else if (wideOpenFlag) {

                    topInterval = 100;
                    botInterval = newData[i].specificityXpert - (100 - newData[i].specificityXpert);
                    base = newData[i].specificityXpert;
                }
                else {

                    topInterval = newData[i].specificityXpert + (100 - newData[i].specificityXpert) / 2;
                    botInterval = newData[i].specificityXpert - (100 - newData[i].specificityXpert) / 2;
                    base = newData[i].specificityXpert;
                }

                interval = topInterval - botInterval;

                if (debug) std::cout << "input-change:  specificityXpert orig = " << newData[i].specificityXpert << ", interval = " << interval << ", topInterval = " << topInterval << ", botInterval = " << botInterval << "\n";

                if (interval > 0) {

                    if (randomFlag) {

                        randFloat = getDistRandFloat("specificityXpert", specificityXpert.distribution, base, topInterval, botInterval, specificityXpert.shape1Alpha, specificityXpert.shape2Beta, specificityXpert.scaleHB);
                    }
                    else {

                        randFloat = interval / noBins * binNumber;
                    }
                    if (debug2) std::cout << "input-change:  specificityXpert orig = " << newData[i].specificityXpert << ", interval = " << interval << ", randFloat = " << randFloat << ", value = " << botInterval + randInt << "\n";
                }
                else {
                    randFloat = 0;
                }

                //debug
                if (debug) std::cout << "input-change: inside input change specificityXpert = " << newData[i].specificityXpert << ", randFloat =  " << randFloat << " botInterval =  " << botInterval << "\n";

                if (specificityXpert.distribution == "Beta") {

                    newData[i].specificity = randFloat;
                }
                else {

                    newData[i].specificity = botInterval + randFloat;
                }
            }


            // MD override of Test results sensitivity = 0 - 100 %
            if (MDSensitivity.active) {

                // for example if sensitivity is .7 on a range of 0-1 then take 1-.7 to get .3.  vary .7 by .3/2 in each direction to keep sensitivity in the 0-1 interval
                
                if (MDSensitivity.upperBound > 0) {
                    
                    topInterval = MDSensitivity.upperBound;
                    botInterval = MDSensitivity.lowerBound;
                    base = MDSensitivity.mean;
                }
                else if (wideOpenFlag) {

                    topInterval = 100;
                    botInterval = newData[i].sensitivityMDWithTest - (100 - newData[i].sensitivityMDWithTest);
                    base = newData[i].sensitivityMDWithTest;
                }
                else {

                    topInterval = newData[i].sensitivityMDWithTest + (100 - newData[i].sensitivityMDWithTest)/2;
                    botInterval = newData[i].sensitivityMDWithTest - (100 - newData[i].sensitivityMDWithTest)/2;
                    base = newData[i].sensitivityMDWithTest;
                }
        
                interval = topInterval - botInterval;

                if (interval > 0) {
            
                     if (randomFlag) {
                     
                        randFloat = getDistRandFloat("MDSensitivity", MDSensitivity.distribution, base, topInterval, botInterval, MDSensitivity.shape1Alpha, MDSensitivity.shape2Beta, MDSensitivity.scaleHB);
                    }
                    else {

                        randFloat = interval/noBins * binNumber;
                    }
                }
                else {
                    randFloat = 0;
                }
            
                if (debug2) std::cout << "input-change: inside input change sensitivityMDWithTest randFloat =  " <<  randFloat << " interval =  " <<  interval << "\n";

                if (MDSensitivity.distribution == "Beta") {

                    newData[i].sensitivityMDWithTest = randFloat;
                }
                else {

                    newData[i].sensitivityMDWithTest = botInterval + randFloat;
                }
            }


            // MD override of Test results specificity = 0 - 100 %
            if (MDSpecificity.active) {

                // for example if specificity is .7 on a range of 0-1 then take 1-.7 to get .3.  vary .7 by .3/2 in each direction to keep specificity in the 0-1 interval
                if (MDSpecificity.upperBound > 0) {
                    
                    topInterval = MDSpecificity.upperBound;
                    botInterval = MDSpecificity.lowerBound;
                    base = MDSpecificity.mean;
                }
                else if (wideOpenFlag) {

                    topInterval = 100;
                    botInterval = newData[i].specificityMDWithTest - (100 - newData[i].specificityMDWithTest);
                    base = newData[i].specificityMDWithTest;
                }
                else {

                    topInterval = newData[i].specificityMDWithTest + (100 - newData[i].specificityMDWithTest)/2;
                    botInterval = newData[i].specificityMDWithTest - (100 - newData[i].specificityMDWithTest)/2;
                    base = newData[i].specificityMDWithTest;
                }
        
                interval = topInterval - botInterval;

                if (interval > 0) {
            
                     if (randomFlag) {
                     
                        randFloat = getDistRandFloat("MDSpecificity", MDSpecificity.distribution, base, topInterval, botInterval, MDSpecificity.shape1Alpha, MDSpecificity.shape2Beta, MDSpecificity.scaleHB);
                    }
                    else {

                        randFloat = interval/noBins * binNumber;
                    }
                }
                else {
                    randFloat = 0;
                }
            
                if (debug2) std::cout << "input-change: inside input change specificityMDWithTest randFloat =  " <<  randFloat << " interval =  " <<  interval << "\n";

                if (MDSpecificity.distribution == "Beta") {

                    newData[i].specificityMDWithTest = randFloat;
                }
                else {

                    newData[i].specificityMDWithTest = botInterval + randFloat;
                }
            }
        }
    }

    // open aggregate file and output values
    myfile.open("./stats/" + scenario + "/job-" + jobNo + "/input/tests/tests.csv");

    myfile << "Test name" << ",Disease" << ",Priority" << ",Batch Wait Time" << ",Patient Return Time" << ",Max test per machine" << ",No. times machine can be use per time step";
    myfile << ",Max tests per day" << ",Tests this time step" << ",Max tests in cycle" << ",Reagent cycle in timesteps" << ",Sensitivity";
    myfile << ",Specificity" << ",MD Sensitivity" << ",MD Specificity" << ",MDwithTestSn" << ",MDwithTestSp" << "\n";

    // now add newData to agData
    for (int i = 0; i < noNewData; i++) {
        
        myfile << newData[i].name << "," << newData[i].disease << "," << newData[i].priority ;
        myfile << "," << newData[i].batchWait << "," << newData[i].patientReturnTime << "," << newData[i].maxTestPerMachine << "," << newData[i].noUsesPerTimeStep;
        myfile  << "," << newData[i].maxTestsPerDay << "," << newData[i].testThisCycle << "," << newData[i].maxTests << "," << newData[i].reagentCycle;
        myfile << "," << newData[i].sensitivity << "," << newData[i].specificity;
        myfile << "," << newData[i].sensitivityMD << "," << newData[i].specificityMD << "," << newData[i].sensitivityMDWithTest;
        myfile << "," << newData[i].specificityMDWithTest << "\n";
    }
    myfile.close();

    if (debug) std::cout << "input-change: leaving test file" <<  "\n";
}


// update global tier values
void updateGlobalTiers(float tier[], float value) {

    for (int tierIndex = 0; tierIndex < 10; tierIndex++) {

        tier[tierIndex] = value;
    }
}

// update regional tier values
void updateGlobalTegionalTiers(struct initTreatLtoFRegionTier* variable, float value) {

    for (int regIndex = 0; regIndex < 20; regIndex++) {

        for (int tierIndex = 0; tierIndex < 10; tierIndex++) {

                    (variable + regIndex)->tier[tierIndex] = value;
        }
    }
}



// create a new value for that tier and then update all the region tier values
void updateGlobalTierValue(struct initTreatLtoFRegionTier* ltof, string region, int tier, float value, bool allFlag) {

    /*struct initTreatLtoFRegionTier {
	string regionNameTier;
	string regionName;
	float tier[10];
};*/

    for (int regIndex = 0; regIndex < 20; regIndex++) {

        if (allFlag) {

            // adjust tier for all regions
            (ltof + regIndex)->tier[tier] = value;
        }
        else if ((ltof + regIndex)->regionName == region) {

            // only adjust for this region
            (ltof + regIndex)->tier[tier] = value;
        }
    }
    
}


// initialize regional tier array
void initRegionalTier(string name, struct initTreatLtoFRegionTier* variable, float *tierValues) {

    string itemName;
    
    // create disease-tier 
    itemName = name + "-tier";

    // initialize treatLtoF region area to treatLtoF global value
    for (int regIndex = 0; regIndex < 20; regIndex++) {

        (variable + regIndex)->regionNameTier = itemName;

        (variable + regIndex)->regionName = regDist.regions[regIndex];

        for (int ltofIndex = 0; ltofIndex < 10; ltofIndex++) {

            (variable + regIndex)->tier[ltofIndex] = tierValues[ltofIndex];
        }
    }
}



// output regional tier values
void dumpTegionalTiers(struct initTreatLtoFRegionTier* variable, string header) {

    for (int regIndex = 0; regIndex < 20; regIndex++) {

        std::cout << "input-change: " << header << ", region = " << (variable + regIndex)->regionName << ", regionTier = " << (variable + regIndex)->regionNameTier;
        std::cout << " tier2 " << (variable + regIndex)->tier[1] << " tier3 " << (variable + regIndex)->tier[2] << ", tier4 = " << (variable + regIndex)->tier[3] << " tier5 = " << (variable + regIndex)->tier[4];
        std::cout << " tier6 " << (variable + regIndex)->tier[5] << ", tier7 = " << (variable + regIndex)->tier[6] << ", tier8 = " << (variable + regIndex)->tier[7] << "\n";
    }
}



// function to get region names 
string getRegionName(int regIndex) {

    string region = "";
    
    region = regDist.regions[regIndex];

    return region;
}


// get the distribution value for the parameter
//*parameter p   newData[i].preTestLtoFPer, randomFlag, "preTestLtoF"
float getDistributionValue(struct parameters* p, float value, bool randomFlag, bool wideOpenFlag, int noBins, int binNumber, string variableName) {

    float topInterval = 0;
    float botInterval = 0;
    float interval = 0;
    float base;
    float newValue = 0;
    float randFloat;
    bool debug = true;

    if (debug) std::cout << "getDistributionValue: " << variableName << " value = " << p->mean << ", UB = " << p->upperBound << ", LB = " << p->lowerBound << "\n";
    
    // for example if probability of pre-diagnostic sample loss rate is .7 on a range of 0-1 then take 1-.7 to get .3.  vary .7 by .3/2 in each direction to keep relapse percentage in the 0-1 interval    

    if (p->upperBound > 0) {

        topInterval = p->upperBound * 100;
        botInterval = p->lowerBound * 100;
        base = p->mean * 100;
    }
    else if (wideOpenFlag) {

        topInterval = 100;
        botInterval = 0;
        base = 50;
    }
    else {

        topInterval = value + (100 - value) / 2;
        botInterval = value - (100 - value) / 2;
        base = value;
    }

    interval = topInterval - botInterval;

    if (randomFlag) {

        randFloat = getDistRandFloat(variableName, p->distribution, base, topInterval, botInterval, p->shape1Alpha, p->shape2Beta, p->scaleHB);
    }
    else {

        randFloat = interval / noBins * binNumber;
            
    }

    // debug - true
    if (debug) std::cout << "input-change: inside input change dist calc " << variableName << " = " << value << " randFloat " << randFloat << ", botInterval = " << botInterval << "\n";

    if (p->distribution == "Beta") {

        newValue = randFloat * 100;
    }
    else {

        newValue = botInterval + randFloat;
    }

    // debug - true
    if (debug) std::cout << "input-change: inside input change  " << variableName << " = " << value << " randFloat " << randFloat << ", botInterval = " << botInterval << "\n";

    return newValue;
}


// get the distribution value for the parameter from array of parameter values
//newData[i].preTestLtoFPer, randomFlag, "preTestLtoF"
float getDistributionLtoFValue(struct parametersRegionTier* p, int tier, float value, bool randomFlag, bool wideOpenFlag, int noBins, int binNumber, string variableName) {

    float topInterval = 0;
    float botInterval = 0;
    float interval = 0;
    float base;
    float newValue = 0;
    float randFloat;
    bool debug = false;

    // for example if probability of pre-diagnostic sample loss rate is .7 on a range of 0-1 then take 1-.7 to get .3.  vary .7 by .3/2 in each direction to keep relapse percentage in the 0-1 interval    
    if (value > 0) {

        if (p->upperBound[tier] > 0) {

            topInterval = p->upperBound[tier] * 100;
            botInterval = p->lowerBound[tier] * 100;
            base = p->mean[tier] * 100;
        }
        else if (wideOpenFlag) {

            topInterval = 100;
            botInterval = 0;
            base = 50;
        }
        else {

            topInterval = value + (100 - value) / 2;
            botInterval = value - (100 - value) / 2;
            base = value;
        }

        interval = topInterval - botInterval;

        if (randomFlag) {

            randFloat = getDistRandFloat(variableName, p->distribution[tier], base, topInterval, botInterval, p->shape1Alpha[tier], p->shape2Beta[tier], p->scaleHB[tier]);
        }
        else {

            randFloat = interval / noBins * binNumber;
            //std::cout << "input-change:  init preTestLtoF orig = " << value <<  ", interval = " << interval << ", randFloat = " << randFloat << ", value = " << botInterval+randInt << "\n";
        }

        // debug - true
        if (debug) std::cout << "input-change: inside input change pre-test ltfu percentage = " << value << " randFloat " << randFloat << ", botInterval = " << botInterval << "\n";

        if (p->distribution[tier] == "Beta") {

            newValue = randFloat * 100;
        }
        else {

            newValue = botInterval + randFloat;
        }
    }

    return newValue;
}



// get the correct distribution for integer intervals
int getDistRandInt(string name, string distribution, float base, float topInterval, float botInterval, float alpha, float beta){

    int randInt = 0;
    float randFloat = 0;
    float interval = 0;
    //float beta;
    //float reshape;
    bool debug = false;
    //ofstream myfile;

    //std::cout << "input-change: getDistRandInt  name = " << name << " topInterval = " << topInterval << " botInterval = " << botInterval << " alpha = " << alpha << " beta = " << beta << "\n";
    
    //if (distribution == "Beta") {

    //    myfile.open( "./betaStatsInt.csv");

    //    myfile << "base" << ",bot.interval" << ",top.interval" << ",alpha" << ",beta" << ",reshape" << ",randInt" << "\n";
    // }

    if (topInterval > botInterval) {
    
        interval = topInterval - botInterval;
    }
    else {

        std::cout << "input-change: getDistRandInt top interval value is less then lower interval value name = " << name <<  "\n";
    }
    
    // find value based on distribution
    if (distribution == "Beta") {
                    
        //reshape = calcReshape(base, topInterval, botInterval);
        //beta = calcBeta(alpha, reshape);

        //for (int w = 0; w < 2000; w++) {
            randInt = getRandInt(interval, alpha, beta);
            //myfile << base << "," << botInterval << "," << topInterval << "," << alpha << "," << beta << "," << reshape << "," << randInt << "\n";
        //}

        if (debug) std::cout << "input-change: getDistRandInt beta = " << randInt <<  "\n";
    }
    else if (distribution == "Uniform") {
                        
        unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
        std::mt19937 generator(seed);

        std::uniform_real_distribution<> distribution(0, 1);

        randFloat = distribution(generator);

        randInt = (int) interval * randFloat;

        if (debug) std::cout << "input-change: getDistRandInt uniform = " << randInt <<  "\n";
    }
    else {

        // default to uniform
        unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
        std::mt19937 generator(seed);

        std::uniform_real_distribution<> distribution(0, 1);

        randFloat = distribution(generator);
                            
        randInt = (int) interval * randFloat;
    }
    //if (distribution == "beta") myfile.close();
    //std::cout << "input-change: getDistRandInt " << name << " mean = " << base << " value = " << botInterval + randInt << " botInterval = " << botInterval << " top interval = " << topInterval << " randInt = " << randInt << "\n";

    return randInt;
}



// get the correct distribution for floating point intervals
float getDistRandFloat(string name, string distribution, float base, float topInterval, float botInterval, float alpha, float beta, float scaleHB){

    float randFloat = 0;
    float interval = 0;
    //float beta;
    //float reshape;
    bool debug = false;
    //ofstream myfile; 

    //std::cout << "input-change: getDistRandFloat  name = " << name << " distribution = " << distribution << " topInterval = " << topInterval << " botInterval = " << botInterval << " alpha = " << alpha << " beta = " << beta << "\n";
    
    //if (distribution == "Beta") {

    //    myfile.open( "./betaStatsFloat.csv");

    //    myfile << "base" << ",bot.interval" << ",top.interval" << ",alpha" << ",beta" << ",reshape" << ",randFloat" << "\n";
    //}

    if (topInterval > botInterval) {
    
        interval = topInterval - botInterval;
    }
    else {

        std::cout << "input-change: getDistRandFloat top interval value is less then lower interval value" <<  "\n";
    }
    
    // find value based on distribution
    if (distribution == "Beta") {
                    
        //reshape = calcReshape(base, topInterval, botInterval);
        //beta = calcBeta(alpha, reshape);

        //for (int w = 0; w < 2000; w++) {
            randFloat = getRandFloat(interval, alpha, beta, scaleHB);
            //myfile << base << "," << botInterval << "," << topInterval << "," << alpha << "," << beta << "," << randFloat << "\n";
        //}

        if (debug) std::cout << "input-change: getDistRandfloat Beta name = " << name << " alpha = " << alpha << " beta = " << beta << " interval = " << interval << " randfloat = " << randFloat << "\n";
    }
    else if (distribution == "Uniform") {
                        
        unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
        std::mt19937 generator(seed);

        std::uniform_real_distribution<> distribution(0, 1);

        randFloat = distribution(generator);

        randFloat = interval * randFloat;

        if (debug) std::cout << "input-change: getDistRandfloat Uniform name = " << name << " alpha = " << alpha << " beta = " << beta << " interval = " << interval << " randfloat = " << randFloat <<  "\n";
    }
    else {

        // default to uniform
        unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
        std::mt19937 generator(seed);

        std::uniform_real_distribution<> distribution(0, 1);

        randFloat = distribution(generator);
                            
        randFloat = interval * randFloat;
    }
    //if (distribution == "beta") myfile.close();

    if (debug) std::cout << "input-change: getDistRandFloat " << name << " dist = " << distribution << " mean = " << base << " value = " << botInterval + randFloat << " botInterval = " << botInterval << " top interval = " << topInterval << " randFloat = " << randFloat << "\n";
    
    return randFloat;
}



int getRandInt(float interval, float alpha, float beta){

    int randIntFromDist;
    int limit = 1;
    float randFloat;
    float randFromDist;
    bool fileOutput = false;
    bool debug = false;
    ofstream myfile;

    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::mt19937 generator(seed);
    
    // if checking output to file
    if (fileOutput) {

        myfile.open("./stats/multIntRuns.csv");
        limit = 10000;
    }
    
    if (debug) std::cout << "insideRandIntinterval = " << interval << ", alpha = " << alpha << ", with beta = " << beta << " ";
    
    //std::uniform_int_distribution<int> distribution(0, interval);
    std::uniform_real_distribution<> distribution(0, 1);

    for (int i = 0; i < limit; i++) {

        randFloat = distribution(generator);

        boost::math::beta_distribution<> mybeta(alpha, beta);
        randFromDist = quantile(mybeta, randFloat);

        if (fileOutput) myfile << randFloat << "," << randFromDist;

        //randIntFromDist = int  (randFromDist * interval);
        randIntFromDist = int  (randFromDist);

        if (fileOutput) myfile << "," << randIntFromDist << "\n";
    }
    if (fileOutput) myfile.close();

    if (debug) std::cout << "interval = " << interval << ", randFloat = " << randFloat << ", with beta = " << randIntFromDist << "\n";
    //std::cout <<  ", alpha = " << alpha << ", beta = " << beta << "\n";

    return randIntFromDist;
}


float getRandFloat(float interval, float alpha, float beta, float scaleHB){

    int limit = 1;
    float randFloat;
    float randFromDist;
    bool fileOutput = false;
    bool debug = false;
    ofstream myfile;

    // generate a random number between 0 and 1
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::mt19937 generator(seed);

     // if checking output to file
    if (fileOutput) {

        myfile.open("./stats/multFloatRuns.csv");
        limit = 10000;
        myfile << "interval" << ",randFloat" << ",randFromDist" << ",interval value" << "\n";;
    }
    //debug - true
    if (debug) std::cout << "insideRandFloat interval = " << interval << ", alpha = " << alpha << ", with beta = " << beta << " ";

    //std::uniform_int_distribution<int> distribution(0, interval * factor);
    //std::uniform_int_distribution<int> distribution(0, 1);
    std::uniform_real_distribution<> distribution(0, 1);
    boost::math::beta_distribution<> mybeta(alpha, beta);

    for (int i = 0; i < limit; i++) {

        randFloat = distribution(generator);

        // map this number to a beta distribution
        randFromDist = quantile(mybeta, randFloat) * scaleHB;

        if (fileOutput) myfile << interval << "," << randFloat << "," << randFromDist;
        
        // map the beta distribution to a value in the interval
        //randFromDist = randFromDist * interval;

        if (fileOutput) myfile << "," << randFromDist << "\n";
    }
    if (fileOutput) myfile.close();

    if (debug) std::cout << ", randFloat = " << randFloat << ", with beta = " << randFromDist <<  "\n";

    return randFromDist;

}



float calcReshape(float originalValue, float upperBound, int lowerBound){

    // determing mean (reshape) for an interval to calculate the variance, alpha and beta for
    // a beta distribution   mean = (originalValue - lowerBound)/(upperBound - lowerBound)
    // normalized to 0-1 distribution
    // for a 50% variance in values, most cases the mean of reshape will be 0.5, unless we're modeling 
    // an asymmetric distribution (where the most likely value is not the midpoint).

    // if original value is 0, then can only have values on positive side.
    // find distance between upper-bound and original value and divide by 2

    float interval;
    float reshape = .5;
    
    interval = upperBound - lowerBound;

    if (interval > 0) {

        // check to see if original value is less than lowerBound
        if (originalValue == 0) {

            reshape = ((upperBound - originalValue)/2)/interval;
        }
        else {

            reshape = (originalValue - lowerBound)/interval;
        }
    }

    //std::cout << "reshape = " << reshape << ", originalValue = " << originalValue << ", interval = " << interval <<  "\n";


    return reshape;

}



float calcRandValue(float randValue, float upperBound, int lowerBound){

    // determine value in range from the random value
    // randValue is between 0-1
    // value = lowerBound + randValue * (upperBound - lowerBound)

    float interval;
    float value;
    
    interval = upperBound - lowerBound;
    
    // default to midway of interval
    value = lowerBound + interval/2;
    
    
    if (interval > 0) {
    
        value = lowerBound + randValue * interval;
    }

    //std::cout << "value = " << value << ", randValue = " << randValue << ", lowerBound = " << lowerBound << ", interval = " << interval <<  "\n";


    return value;

}



// calculate beta for beta distribution
float calcBeta(float alpha, float mean) { 

    // default beta to 4 (similar to alpha = 4)

    float beta = 4;

    if (mean > 0) {

        beta = (alpha/mean) - alpha;
    }

    return beta;  
    
}



// calc variance from alpha and beta 
float calcVariance(float alpha, float beta){

    // determing variance from alpha and beta 
    // varience = (alpha * beta)/[(alpha + beta)^2*(alpha + beta + 1)
    //

    float variance = 0; 

    if ((alpha > 0) && (beta > 0)) {
    
        variance = (alpha * beta)/(pow(alpha + beta,2) * (alpha + beta + 1));
    }

    //std::cout << "variance = " << variance << ", alpha = " << alpha << ", beta = " << beta <<  "\n";


    return variance;

}

// calc alpha from mean and variance 
float calcNewAlpha(float mean, float variance){

    // calculate alpha for beta variance 
    // alpha = mean * (((mean*(1-mean))/variance)-1) 
    //

    float alpha = 0; 

    if (variance > 0) {
    
        alpha = mean * (((mean * (1 - mean))/variance)-1);
    }

    //std::cout << "alpha = " << alpha << ", mean = " << mean << ", variance = " << variance <<  "\n";


    return alpha;

}


// calc beta from mean and variance 
float calcNewBeta(float mean, float variance){

    // calculate alpha for beta variance 
    // float = (1-mean) * (((mean*(1-mean))/variance)-1) 
    //

    float beta = 0; 

    if (variance > 0) {
    
        beta = (1-mean) * (((mean * (1 - mean))/variance)-1);
    }

    //std::cout << "beta = " << beta << ", mean = " << mean << ", variance = " << variance <<  "\n";


    return beta;

}

// modify district name
string modifyName(string name) {

    int municipalIndex = 0;
    int metro1Index = 0;
    int metro2Index = 0;
    int metro3Index = 0;
    string newName = "";
    bool debug = false;

    //debug - true
    if (debug) std::cout << "inside modifyName = " << name << "\n";

    // first we will turn original district words into upper case
    /*for (size_t i = 0; i < name.size(); i++)
    {
        newName[i] = toupper(name[i]);
    }*/
    transform(name.begin(), name.end(), name.begin(), ::toupper);

    newName = name;

    //debug - true
    if (debug) std::cout << " after all upper = " << newName;

    // now we will remove extra words from district name like "MUNIIPAL", "METROPOLITAN", "METROPOLIS", "METRP"
    // remove MUNICIPAL from all districts to handle both cases - will return -1 if didn't find MUNICIPAL string
    municipalIndex = newName.find("MUNICIPAL");
    metro1Index = newName.find("METROPOLITAN");
    metro2Index = newName.find("METROPOLIS");

    if (municipalIndex > -1) newName = newName.substr(0, municipalIndex - 1);
    if (metro1Index > -1) newName = newName.substr(0, metro1Index - 1);
    if (metro2Index > -1) newName = newName.substr(0, metro2Index - 1);

    //debug - true
    if (debug) std::cout << " after MUN = " << newName;

    // check to make sure "METRO" handled after "METROPOLITAN" and "METROPOLIS"
    metro3Index = newName.find("METRO");
    if (metro2Index > -1) newName = newName.substr(0, metro3Index - 1);


    // now we will remove all "-" and spaces
    newName.erase(std::remove(newName.begin(), newName.end(), '-'), newName.end());
    newName.erase(std::remove(newName.begin(), newName.end(), ' '), newName.end());

    //debug - true
    if (debug) std::cout << " after space- = " << newName;

    // now we will remove all spaces and the last characher in case the district ends in a "I" or "E"
    newName = newName.substr(0, newName.size() - 1);

    //debug - true
    if (debug) std::cout << " leaving modify name = " << newName << "\n";

    return newName;

}


// find closest matching district
string findDistrictName(string districtName, string regionDistrictArray[]) {

    string arrayDistrictName = "";
    string compactName = "";
    string arrayCompactName = "";
    bool foundDistrict = false;
    bool debug = false;


    // compact district name
    compactName = modifyName(districtName);

    //debug - true
    if (debug) std::cout << " inside findDistrictName = " << districtName << " compact name = " << compactName << "\n";

    // now compact district names in array and see if any match
    for (int rdIndex = 0; rdIndex < noElementsDistrictRegionArray; rdIndex++) {

        if (!foundDistrict) {

            arrayCompactName = modifyName(regionDistrictArray[rdIndex]);

            if (compactName == arrayCompactName) {

                arrayDistrictName = regionDistrictArray[rdIndex];

                foundDistrict = true;
            }
        }

    }
    
    if (!foundDistrict) {

        arrayDistrictName = districtName;

        std::cout << "findDistrictName - couldn't find = " << districtName << "\n";

    }

    // debug - true
    if (debug) std::cout << "leaving findDistrictName = " << districtName << " found district = "  << foundDistrict << " array name = " << arrayDistrictName << "\n";

    return arrayDistrictName;

}


// find district index
int findDistrictIndex(string districtName, string regionDistrictArray[]) {

    int districtIndex = -1;

    for (int rdIndex = 0; rdIndex < noElementsDistrictRegionArray; rdIndex++) {

    }

    return districtIndex;

}
