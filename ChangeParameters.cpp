//
//  ChangeParameters.cpp
//  mdrtb
//
//  Created by Trinity on 5/19/16.
//  Copyright © 2016 Trinity. All rights reserved.
//

#include <iostream>
#include <string>
#include <cstring>
#include <vector>
#include <random>
#include <sys/stat.h>
#include <unistd.h>


using namespace std;


#include "InputOutput.h"
#include "InputChange.h"
#include "ChangeParameters.h"
#include "GlobalParameters.h"
#include "CreateAgents.h"

// check priorRangesAndDistributions.csv file in ./input/input-change-parameters and change input parameters as necessary
void changeParameters(string scenario, string jobNo, string facilitySpecificFile) {
    
    
    //int b=atoi(argv[2]);
    // cout<<"runing scenario "<<a<<"to"<<b<<endl;

    int noChanges;
    int noBins = 0;
    int binNumber = 0;
    int dIndex = 0;
    int randInt;
    int tempTier;
    // create integer array to pass batchWait time and patientReturnTime variable between facilitySpecific.csv and test.csv files
    int returnArray[1] = {0};
    string tempString;
    //string facilitySpecificFile;
    // randomFlag controls whether to use bins for random numbers or beta distribution for random numbers
    bool randomFlag = true;
    bool inPlaceFlag = false;
    bool debug = false;
    // open beta parameters wide open
    bool wideOpenFlag = false;
    bool dumpVariables = false;

    // alpha is set to 4 and beta is calculated in each routine based on interval values
    float alpha = 4;

    // input files
    string temp;
    string yearDir;
    string fileDir;
    string subDirBase;
    string statsType;
    string jobNoDir = scenario + "/" + "job-" + jobNo;
    string facilityProjectDir = "./facility-project/";
    string defInputDir = "./stats/" + jobNoDir + "/input/";
    string inputDir = "./stats/" + jobNoDir + "/input/";
    string diseaseDir = "diseases/";
    string testDir = "tests/";
    string facilitiesDir = "facilities/";
    string populationDir = "population/";
    string popBinDir = "catchment/";
    string facilitySpecificDir = "facility-specific-data/";
    string regionDistrictSpecificDir = "region-district-specific-data/";
    string runSpecificDir = "run-specific-data/";
    string parameterDir = "input-change-parameters/";
    string filenameDir = "input-file-names/";
    string diseaseFile = "diseases.csv";
    string testFile = "tests.csv";
    string facilitiesFile = "facilities.csv";
    string facilitiesFileNoTop = "facilities-no-top-layer.csv";
    string populationFile = "pop_1km_array_2015.csv";
    string popBinFile = "Facility-Catchment-Population-Distribution.csv";
    /*switch (stoi(scenario)) {
        case 1:
        case 4:
        case 7:
            facilitySpecificFile = "facility-specific-data-tier1-2-3-xpert-only-by-tier.csv";
            break;
        case 2:
        case 5:
        case 8:
            facilitySpecificFile = "facility-specific-data-tier1-2-3-4-xpert-only-by-tier.csv";
            break;
        case 3:
        case 6:
        case 9:
            facilitySpecificFile = "facility-specific-data-current-model-smear-testing1234-xpert-testing.csv";
            break;
        case 10:
            facilitySpecificFile = "facility-specific-data-tier1-7-xpert-only-by-tier.csv";
            break;
        default :
            facilitySpecificFile = "facility-specific-data-current-model-smear-testing1234-xpert-testing.csv";
            break;
    }*/
    string regionDistrictSpecificFile = "region-district-specific-data.csv";
    string runSpecificFile = "run-specific-data.csv";
    //string parameterFlagFile = "parameter-range.csv";
    string parameterFlagFile = "priorRangesAndDistributions.csv";
    string filenameFile = "input-file-names.csv";

    string diseaseInput = defInputDir + diseaseDir + diseaseFile;
    string testInput = defInputDir + testDir + testFile;
    string facilitiesInput = defInputDir + facilitiesDir + facilitiesFile;
    string facilitiesInputNoTop = defInputDir + facilitiesDir + facilitiesFileNoTop;
    string populationInput = defInputDir + populationDir + populationFile;
    string popBinInput = defInputDir + popBinDir + popBinFile;
    string facilitySpecificInput = defInputDir + facilitySpecificDir + facilitySpecificFile;
    string regionDistrictSpecificInput = defInputDir + regionDistrictSpecificDir + regionDistrictSpecificFile;
    string runSpecificInput = defInputDir + runSpecificDir + runSpecificFile;
    string parameterInput = inputDir + parameterDir + parameterFlagFile;
    string filenameInput = inputDir + filenameDir + filenameFile;

    if (debug) std::cout << "defInputDir = " << defInputDir << ",inputDir = " << inputDir << " scenario =" << scenario << " fac spec file = " << facilitySpecificFile << "\n";
    
    // command line parameters should be 
    // 1 = no. of bins to divide changing parameter
    // 2 = which bin to get value for changing parameterif
    // if changing parameter is .5 +- .2 then the .4 values will be broken into no. of bins and the value will correspond
    // to the bin number - so if no. of bins is 10 and bin number is 3 the new parameter would be (lower end of range) +
    // (range/no.of bins)*bin number or .3 + (.4/10)*.3 = 4.2

    // std::cout << "number of arguments = " << argc << ", " << argv[0] << ", " << argv[1] << "\n";

    // check if using set no. of bins or beta distribution
 
    //std::cout << "number of bins = " << noBins << ", bin number = " << binNumber << ", randomFlag = " << randomFlag <<  "\n";


    // Region-district-specific-data/Region-district-specific-data.csv
    //     Incidence per person per day Years – 0,1,2,3,4,5,6,7,8,9 – range (+- 50%), calculate for each district/facility
    // diseases/diseases.csv
    //     % Initial treatment success
    //     % Initial treatment death
    //     % Initial treatment LtoF
    //     % Retreatment success
    // facility-specific-data/facility-specific-data.csv
    //     Number of machines
    //     Patient return time for test results
    // Tests/Tests.csv
	//     Max reagents per cycle
    //     Time steps in cycle
	//     Sample wait time for batch shipment
	//     Sensitivity (confidence intervals – keep in range of 0-1) check Isabella
    //     Specificity
    //     MD with test Sensitivity
    //     MD with test Specificity

    if (debug) {
    
        char tmp[256];
        getcwd(tmp, 256);
        std::cout << "Current path is " << tmp << '\n'; 
    }
    
    // read in different tests parameters
    // header Param	Param.description	Disease	Dist	Active	mean	LB	HB	shape1.alpha	shape2.beta	scale.HB
    vector<vector<string>> parameterFlags = readDataInput(parameterInput);

    if (parameterFlags.size() < 2) {
        std::cout << "In ChangeParameters - problem reading parameter flag file = " << parameterInput << "\n";
    }
        
    if (debug) {
        std::cout << "after read parameter flag file\n";
    }

    // in inputChange.h
    //struct parameters {

    //    bool active;
    //    string distribution;
    //    float mean;
    //    float lowerBound;
    //    float higerBound;
    //    float shape1Alpha;
    //    float shape2Beta;
    //    float scaleHB;
    //};

    struct inputChange {
        string disease;
        parameters incidence;
        parameters probRepresentGeoDist;
        parameters perIncPresenting;
        parameters infUninfRatio;
        parameters perInitInf;
        parameters initTreatSuccess;
        parameters initTreatDeath;
        parameters retreatmentPer;
        parameters representationLtoFTime;
        parameters noMachines;
        parameters maxReagent;
        parameters timeStepsPerCycle;
        parameters sampleWait;
        parameters sensitivity;
        parameters sensitivityPCR;
        parameters sensitivityXpert;
        parameters specificity;
        parameters specificityPCR;
        parameters specificityXpert;
        parameters MDSensitivity;
        parameters MDSpecificity;
        parameters probPatientReturns;
        bool preTestLtoFFlag;
        parameters preTestLtoF;
        parametersRegionTier preTestLtoFRegion[20];
        bool preTreatLtoFFlag;
        parameters preTreatLtoF;
        parametersRegionTier preTreatLtoFRegion[20];
        bool treatLtoFFlag;
        parameters treatLtoF;
        parametersRegionTier treatLtoFRegion[20];
        parameters perRepresentLtoFDeath;
        parameters timeDeathRetreat;
        parameters timeLtoFRepresent;
        parameters timeUntreatCure;
        parameters MDClinicDiag;
        parameters preTestMDClinicDiagInf;
        parameters preTestMDClinicDiagUnInf;
    };

    noChanges = parameterFlags.size();

    inputChange* newData = new inputChange[maxNoDiseases];


    // set up diseases
    newData[0].disease = "TB";
    newData[1].disease = "HIV";
    newData[2].disease = "HCV";
    newData[3].disease = "Measles";
    newData[4].disease = "Yellow Fever";
    newData[5].disease = "Meningitis";
    newData[6].disease = "Disease1";
    newData[7].disease = "Disease2";
    
    
    // intialize them to all off
    for (int i = 0; i < maxNoDiseases; i++) {

        newData[i].incidence.active = false;
        newData[i].probRepresentGeoDist.active = false;
        newData[i].perIncPresenting.active = false;
        newData[i].infUninfRatio.active = false;
        newData[i].perInitInf.active = false;
        newData[i].initTreatSuccess.active = false;
        newData[i].initTreatDeath.active = false;
        newData[i].retreatmentPer.active = false;
        newData[i].representationLtoFTime.active = false;
        newData[i].noMachines.active = false;
        newData[i].maxReagent.active = false;
        newData[i].timeStepsPerCycle.active = false;
        newData[i].sampleWait.active = false;
        newData[i].sensitivity.active = false;
        newData[i].sensitivityPCR.active = false;
        newData[i].sensitivityXpert.active = false;
        newData[i].specificity.active = false;
        newData[i].specificityPCR.active = false;
        newData[i].specificityXpert.active = false;
        newData[i].MDSensitivity.active = false;
        newData[i].MDSpecificity.active = false;
        newData[i].probPatientReturns.active = false;
        newData[i].preTestLtoFFlag = false;
        newData[i].preTestLtoF.active = false;
        for (int j = 0; j < 20; j++) {

            if (j == 0) newData[i].preTestLtoFRegion[j].region = "all";
            else if (j == 1) newData[i].preTestLtoFRegion[j].region = "northern";
            else if (j == 2) newData[i].preTestLtoFRegion[j].region = "middlebelt";
            else if (j == 3) newData[i].preTestLtoFRegion[j].region = "southern";
            else newData[i].preTestLtoFRegion[j].region = getRegionName(j-4);

            for (int k = 0; k < 10; k++) {
                newData[i].preTestLtoFRegion[j].active[k] = false;
            }
        }
        newData[i].preTreatLtoFFlag = false;
        newData[i].preTreatLtoF.active = false;
        for (int j = 0; j < 20; j++) {

            if (j == 0) newData[i].preTreatLtoFRegion[j].region = "all";
            else if (j == 1) newData[i].preTreatLtoFRegion[j].region = "northern";
            else if (j == 2) newData[i].preTreatLtoFRegion[j].region = "middlebelt";
            else if (j == 3) newData[i].preTreatLtoFRegion[j].region = "southern";
            else newData[i].preTreatLtoFRegion[j].region = getRegionName(j-4);

            for (int k = 0; k < 10; k++) {
                newData[i].preTreatLtoFRegion[j].active[k] = false;
            }
        }
        newData[i].treatLtoFFlag = false;
        newData[i].treatLtoF.active = false;
        for (int j = 0; j < 20; j++) {

            if (j == 0) newData[i].treatLtoFRegion[j].region = "all";
            else if (j == 1) newData[i].treatLtoFRegion[j].region = "northern";
            else if (j == 2) newData[i].treatLtoFRegion[j].region = "middlebelt";
            else if (j == 3) newData[i].treatLtoFRegion[j].region = "southern";
            else newData[i].treatLtoFRegion[j].region = getRegionName(j-4);

            for (int k = 0; k < 10; k++) {
                newData[i].treatLtoFRegion[j].active[k] = false;
            }
        }
        newData[i].perRepresentLtoFDeath.active = false;
        newData[i].timeDeathRetreat.active = false;
        newData[i].timeLtoFRepresent.active = false;
        newData[i].timeUntreatCure.active = false;
        newData[i].MDClinicDiag.active = false;
        newData[i].preTestMDClinicDiagInf.active = false;
        newData[i].preTestMDClinicDiagUnInf.active = false;
    }


    // skip the first row, it contains column headers
    // format is : disease, incidence, ncidenceAdjustment, initTreatSuccess, initTreatDeath, retreatment, noMachines, maxReagent
    //       timeStepsPerCycle, sampleWait, sensitivity, specificity, MDSensitivity, MDSpecificity, probPatientReturns, preTestLtoF, preTreatLtoF
    // the format is : disease parameter, active, distribution, param1, param2, param2
    for (auto rdIndex = 1u; rdIndex < parameterFlags.size(); rdIndex++) {

        // debug - true
        if (debug) {
            std::cout << parameterFlags[rdIndex][0] << ' ' << parameterFlags[rdIndex][1] << ' ' << parameterFlags[rdIndex][2];
            std::cout << ' ' << parameterFlags[rdIndex][4] << ' ' << parameterFlags[rdIndex][5] << ' ' << parameterFlags[rdIndex][6] << '\n';
        }

        // find disease index
        for (int i = 0; i < maxNoDiseases; i++) {
        
            if (parameterFlags[rdIndex][2] == newData[i].disease) {
           
                dIndex = i;
                break;
            }
        }
        
        // debug - true
        if (debug) std::cout << "indexo0 = " << parameterFlags[rdIndex][0] << '\n';

        // now update the correct parameter values
        if (parameterFlags[rdIndex][0] == "incidence") {
            
            readParameterIntoArray(parameterFlags[rdIndex], &newData[dIndex].incidence);

            if ((dumpVariables) && (parameterFlags[rdIndex][4] == "TRUE")) dumpParameter(&newData[dIndex].incidence, "incidence");
        }

        // now update the correct parameter values
        if (parameterFlags[rdIndex][0] == "probGeoDist") {

            readParameterIntoArray(parameterFlags[rdIndex], &newData[dIndex].probRepresentGeoDist);

            if ((dumpVariables) && (parameterFlags[rdIndex][4] == "TRUE")) dumpParameter(&newData[dIndex].probRepresentGeoDist, "probRepresentGeoDist");
        }

        // now update the correct parameter values
        if (parameterFlags[rdIndex][0] == "prpIncPresenting") {

            readParameterIntoArray(parameterFlags[rdIndex], &newData[dIndex].perIncPresenting);

            if ((dumpVariables) && (parameterFlags[rdIndex][4] == "TRUE")) dumpParameter(&newData[dIndex].perIncPresenting, "prpIncPresenting");
        }

        // now update the correct parameter values
        if (parameterFlags[rdIndex][0] == "infUninfRatio") {

            readParameterIntoArray(parameterFlags[rdIndex], &newData[dIndex].infUninfRatio);

            if ((dumpVariables) && (parameterFlags[rdIndex][4] == "TRUE")) dumpParameter(&newData[dIndex].infUninfRatio, "infUninfRatio");
        }

        // now update the correct parameter values
        if (parameterFlags[rdIndex][0] == "perInitInf") {

            readParameterIntoArray(parameterFlags[rdIndex], &newData[dIndex].perInitInf);

            if ((dumpVariables) && (parameterFlags[rdIndex][4] == "TRUE")) dumpParameter(&newData[dIndex].perInitInf, "perInitInf");
        }


        // now update the correct parameter values
        if (parameterFlags[rdIndex][0] == "initTreatSuccess") {

            readParameterIntoArray(parameterFlags[rdIndex], &newData[dIndex].initTreatSuccess);

            if ((dumpVariables) && (parameterFlags[rdIndex][4] == "TRUE")) dumpParameter(&newData[dIndex].initTreatSuccess, "initTreatSuccess");
        }

        // now update the correct parameter values
        if (parameterFlags[rdIndex][0] == "initTreatDeath") {

            readParameterIntoArray(parameterFlags[rdIndex], &newData[dIndex].initTreatDeath);

            if ((dumpVariables) && (parameterFlags[rdIndex][4] == "TRUE")) dumpParameter(&newData[dIndex].initTreatDeath, "initTreatDeath");
        }

        // now update the correct parameter values
        if (parameterFlags[rdIndex][0] == "retreatmentPer") {

            readParameterIntoArray(parameterFlags[rdIndex], &newData[dIndex].retreatmentPer);

            if ((dumpVariables) && (parameterFlags[rdIndex][4] == "TRUE")) dumpParameter(&newData[dIndex].retreatmentPer, "retreatmentPer");
        }
        

        // now update the correct parameter values
        if (parameterFlags[rdIndex][0] == "representationLtoFTime") {

            readParameterIntoArray(parameterFlags[rdIndex], &newData[dIndex].representationLtoFTime);

            if ((dumpVariables) && (parameterFlags[rdIndex][4] == "TRUE")) dumpParameter(&newData[dIndex].representationLtoFTime, "representationLtoFTime");
        }
         
        // now update the correct parameter values
        if (parameterFlags[rdIndex][0] == "noMachines") {

            readParameterIntoArray(parameterFlags[rdIndex], &newData[dIndex].noMachines);

            if ((dumpVariables) && (parameterFlags[rdIndex][4] == "TRUE")) dumpParameter(&newData[dIndex].noMachines, "noMachines");
        }

        // now update the correct parameter values
        if (parameterFlags[rdIndex][0] == "maxReagent") {

            readParameterIntoArray(parameterFlags[rdIndex], &newData[dIndex].maxReagent);

            if ((dumpVariables) && (parameterFlags[rdIndex][4] == "TRUE")) dumpParameter(&newData[dIndex].maxReagent, "maxReagent");
        }

        // now update the correct parameter values
        if (parameterFlags[rdIndex][0] == "timeStepsPerCycle") {

            readParameterIntoArray(parameterFlags[rdIndex], &newData[dIndex].timeStepsPerCycle);

            if ((dumpVariables) && (parameterFlags[rdIndex][4] == "TRUE")) dumpParameter(&newData[dIndex].timeStepsPerCycle, "timeStepsPerCycle");
        }

        // now update the correct parameter values
        if (parameterFlags[rdIndex][0] == "sampleWait") {

            readParameterIntoArray(parameterFlags[rdIndex], &newData[dIndex].sampleWait);

            if ((dumpVariables) && (parameterFlags[rdIndex][4] == "TRUE")) dumpParameter(&newData[dIndex].sampleWait, "sampleWait");
        }

        // now update the correct parameter values
        if (parameterFlags[rdIndex][0] == "sensitivity") {

            readParameterIntoArray(parameterFlags[rdIndex], &newData[dIndex].sensitivity);

            // dumpVariables
            if ((dumpVariables) && (parameterFlags[rdIndex][4] == "TRUE")) dumpParameter(&newData[dIndex].sensitivity, "sensitivity");
        }

        // now update the correct parameter values
        if (parameterFlags[rdIndex][0] == "sensitivity.PCR") {

            readParameterIntoArray(parameterFlags[rdIndex], &newData[dIndex].sensitivityPCR);

            // dumpVariables
            if ((dumpVariables) && (parameterFlags[rdIndex][4] == "TRUE")) dumpParameter(&newData[dIndex].sensitivityPCR, "sensitivityPCR");
        }

        // now update the correct parameter values
        if (parameterFlags[rdIndex][0] == "sensitivity.Xpert") {

            readParameterIntoArray(parameterFlags[rdIndex], &newData[dIndex].sensitivityXpert);

            // dumpVariables
            if ((dumpVariables) && (parameterFlags[rdIndex][4] == "TRUE")) dumpParameter(&newData[dIndex].sensitivityXpert, "sensitivityXpert");
        }

        // now update the correct parameter values
        if (parameterFlags[rdIndex][0] == "specificity") {

            readParameterIntoArray(parameterFlags[rdIndex], &newData[dIndex].specificity);

            // dumpVariables
            if ((dumpVariables) && (parameterFlags[rdIndex][4] == "TRUE")) dumpParameter(&newData[dIndex].specificity, "specificity");
        }

        // now update the correct parameter values
        if (parameterFlags[rdIndex][0] == "specificity.PCR") {

            readParameterIntoArray(parameterFlags[rdIndex], &newData[dIndex].specificityPCR);

            // dumpVariables
            if ((dumpVariables) && (parameterFlags[rdIndex][4] == "TRUE")) dumpParameter(&newData[dIndex].specificityPCR, "specificityPCR");
        }

        // now update the correct parameter values
        if (parameterFlags[rdIndex][0] == "specificity.Xpert") {

            readParameterIntoArray(parameterFlags[rdIndex], &newData[dIndex].specificityXpert);

            // dumpVariables
            if ((dumpVariables) && (parameterFlags[rdIndex][4] == "TRUE")) dumpParameter(&newData[dIndex].specificityXpert, "specificityXpert");
        }

        // now update the correct parameter values
        if (parameterFlags[rdIndex][0] == "MDSensitivity") {

            readParameterIntoArray(parameterFlags[rdIndex], &newData[dIndex].MDSensitivity);

            if ((dumpVariables) && (parameterFlags[rdIndex][4] == "TRUE")) dumpParameter(&newData[dIndex].MDSensitivity, "MDSensitivity");
        }

        // now update the correct parameter values
        if (parameterFlags[rdIndex][0] == "MDSpecificity") {

            readParameterIntoArray(parameterFlags[rdIndex], &newData[dIndex].MDSpecificity);

            if ((dumpVariables) && (parameterFlags[rdIndex][4] == "TRUE")) dumpParameter(&newData[dIndex].MDSpecificity, "MDSpecificity");
        }

        // now update the correct parameter values
        if (parameterFlags[rdIndex][0] == "ProbilityPatientReturns") {

            readParameterIntoArray(parameterFlags[rdIndex], &newData[dIndex].probPatientReturns);

            if ((dumpVariables) && (parameterFlags[rdIndex][4] == "TRUE")) dumpParameter(&newData[dIndex].probPatientReturns, "probPatientReturns");
        }
        
        // now update the correct parameter values
        if (parameterFlags[rdIndex][0] == "PreTestLtoF") {

            if (parameterFlags[rdIndex][4] == "TRUE") newData[dIndex].preTestLtoFFlag = true;

            readParameterIntoArray(parameterFlags[rdIndex], &newData[dIndex].preTestLtoF);

            if ((dumpVariables) && (parameterFlags[rdIndex][4] == "TRUE")) dumpParameter(&newData[dIndex].preTestLtoF, "preTestLtoF");
        }
        
        // now update the correct parameter values
        tempString = parameterFlags[rdIndex][0].substr(0, 12);
        if (tempString == "PreTestLtoF.") {

            // for Ghana
            // all - all regions
            // northern zone - regions: Northern, North East, Savannah, Upper East, Upper West
            // middleBelt zone - regions: Ahafo, Ashanti, Bono, Bono East, Eastern
            // southern zone - regions: Central, Greater Accra, Oti, Volta, Western, Western North

            tempTier = parameterFlags[rdIndex][0].find(".", 12);
            tempString = parameterFlags[rdIndex][0].substr(12, tempTier - 12);
            tempTier = stoi(parameterFlags[rdIndex][0].substr(tempTier + 1, 1));

            if (parameterFlags[rdIndex][4] == "TRUE") newData[dIndex].preTestLtoFFlag = true;
                
            //std::cout << "after read parameter PreTestLtoF. " << tempString << "\n";

            readParameterRegionTierIntoArray(parameterFlags[rdIndex], tempString, tempTier, newData[dIndex].preTestLtoFRegion);

            //std::cout << "after read parameter - PreTestLtoF zone " << tempString << "\n";
        }

        // now update the correct parameter values
        if (parameterFlags[rdIndex][0] == "PreTreatLtoF") {

            if (parameterFlags[rdIndex][4] == "TRUE") newData[dIndex].preTreatLtoFFlag = true;
            
            readParameterIntoArray(parameterFlags[rdIndex], &newData[dIndex].preTreatLtoF);

            if ((dumpVariables) && (parameterFlags[rdIndex][4] == "TRUE")) dumpParameter(&newData[dIndex].preTreatLtoF, "preTreatLtoF");
        }

        // now update the correct parameter values
        tempString = parameterFlags[rdIndex][0].substr(0, 13);
        if (tempString == "PreTreatLtoF.") {

            // for Ghana
            // all - all regions
            // northern zone - regions: Northern, North East, Savannah, Upper East, Upper West
            // middleBelt zone - regions: Ahafo, Ashanti, Bono, Bono East, Eastern
            // southern zone - regions: Central, Greater Accra, Oti, Volta, Western, Western North

            tempTier = parameterFlags[rdIndex][0].find(".", 13);
            tempString = parameterFlags[rdIndex][0].substr(13, tempTier - 13);
            tempTier = stoi(parameterFlags[rdIndex][0].substr(tempTier + 1, 1));

            if (parameterFlags[rdIndex][4] == "TRUE") newData[dIndex].preTreatLtoFFlag = true;

            //std::cout << "after read parameter PreTreatLtoF." << tempString << "\n";

            readParameterRegionTierIntoArray(parameterFlags[rdIndex], tempString, tempTier, newData[dIndex].preTreatLtoFRegion);

            //std::cout << "after read parameter - PreTreatLtoF zone " << tempString << "\n";
        }


        // now update the correct parameter values
        if (parameterFlags[rdIndex][0] == "TreatLtoF") {

            if (parameterFlags[rdIndex][4] == "TRUE") newData[dIndex].treatLtoFFlag = true;
            
            readParameterIntoArray(parameterFlags[rdIndex], &newData[dIndex].treatLtoF);

            if ((dumpVariables) && (parameterFlags[rdIndex][4] == "TRUE")) dumpParameter(&newData[dIndex].treatLtoF, "treatLtoF");
        }

        // now update the correct parameter values
        tempString = parameterFlags[rdIndex][0].substr(0, 10);
        if (tempString == "TreatLtoF.") {

            // for Ghana
            // all - all regions
            // northern zone - regions: Northern, North East, Savannah, Upper East, Upper West
            // middleBelt zone - regions: Ahafo, Ashanti, Bono, Bono East, Eastern
            // southern zone - regions: Central, Greater Accra, Oti, Volta, Western, Western North

            tempTier = parameterFlags[rdIndex][0].find(".", 10);
            tempString = parameterFlags[rdIndex][0].substr(10, tempTier - 10);
            tempTier = stoi(parameterFlags[rdIndex][0].substr(tempTier + 1, 1));

            if (parameterFlags[rdIndex][4] == "TRUE") newData[dIndex].treatLtoFFlag = true;

            //std::cout << "after read parameter TreatLtoF." << tempString << "\n";

            readParameterRegionTierIntoArray(parameterFlags[rdIndex], tempString, tempTier, newData[dIndex].treatLtoFRegion);

            //std::cout << "after read parameter - PreTreatLtoF zone " << tempString << "\n";
        }
        
        

        // now update the correct parameter values
        if (parameterFlags[rdIndex][0] == "representationDeathPer") {

            readParameterIntoArray(parameterFlags[rdIndex], &newData[dIndex].perRepresentLtoFDeath);

            if ((dumpVariables) && (parameterFlags[rdIndex][4] == "TRUE")) dumpParameter(&newData[dIndex].perRepresentLtoFDeath, "perRepresentLtoFDeath");
        }

        // now update the correct parameter values
        if (parameterFlags[rdIndex][0] == "timeDeathRetreat") {

            readParameterIntoArray(parameterFlags[rdIndex], &newData[dIndex].timeDeathRetreat);

            if ((dumpVariables) && (parameterFlags[rdIndex][4] == "TRUE")) dumpParameter(&newData[dIndex].timeDeathRetreat, "timeDeathRetreat");
            //std::cout << "inside timeDeathRetreat " << newData[dIndex].timeDeathRetreat.active << ' ' << newData[dIndex].timeDeathRetreat.mean << ' ' << newData[dIndex].timeDeathRetreat.upperBound << '\n';
        }

        // now update the correct parameter values
        if (parameterFlags[rdIndex][0] == "timeLtoFRepresent") {

            readParameterIntoArray(parameterFlags[rdIndex], &newData[dIndex].timeLtoFRepresent);

            if ((dumpVariables) && (parameterFlags[rdIndex][4] == "TRUE")) dumpParameter(&newData[dIndex].timeLtoFRepresent, "timeLtoFRepresent");
            //std::cout << "inside timeLtoFRepresent " << newData[dIndex].timeLtoFRepresent.active << ' ' << newData[dIndex].timeLtoFRepresent.mean << ' ' << newData[dIndex].timeLtoFRepresent.upperBound << '\n';
        }

        // now update the correct parameter values
        if (parameterFlags[rdIndex][0] == "timeUntreatCure") {

            readParameterIntoArray(parameterFlags[rdIndex], &newData[dIndex].timeUntreatCure);

            if ((dumpVariables) && (parameterFlags[rdIndex][4] == "TRUE")) dumpParameter(&newData[dIndex].timeUntreatCure, "timeUntreatCure");
            //std::cout << "inside timeUntreatCure " << newData[dIndex].timeUntreatCure.active << ' ' << newData[dIndex].timeUntreatCure.mean << ' ' << newData[dIndex].timeUntreatCure.upperBound << '\n';
        }


        // now update the correct parameter values
        /*if (parameterFlags[rdIndex][0] == "MDClinicDiag") {

            readParameterIntoArray(parameterFlags[rdIndex], &newData[dIndex].MDClinicDiag);

            if ((dumpVariables) && (parameterFlags[rdIndex][4] == "TRUE")) dumpParameter(&newData[dIndex].MDClinicDiag, "MDClinicDiag");
        }*/

        //now update the correct parameter values
        if (parameterFlags[rdIndex][0] == "pClinicalDiagUpfrontForInfectedPatients") {

            readParameterIntoArray(parameterFlags[rdIndex], &newData[dIndex].preTestMDClinicDiagInf);

            if ((dumpVariables) && (parameterFlags[rdIndex][4] == "TRUE")) dumpParameter(&newData[dIndex].preTestMDClinicDiagInf, "preTestMDClinicDiagInf");
            //std::cout << "inside pClinicalDiagUpfrontForInfectedPatients " << newData[dIndex].preTestMDClinicDiagInf.active << ' ' << newData[dIndex].preTestMDClinicDiagInf.mean << ' ' << newData[dIndex].preTestMDClinicDiagInf.upperBound << '\n';
        }

        //now update the correct parameter values
        if (parameterFlags[rdIndex][0] == "pClinicalDiagUpfrontForUninfectedPatients") {

            readParameterIntoArray(parameterFlags[rdIndex], &newData[dIndex].preTestMDClinicDiagUnInf);

            if ((dumpVariables) && (parameterFlags[rdIndex][4] == "TRUE")) dumpParameter(&newData[dIndex].preTestMDClinicDiagUnInf, "preTestMDClinicDiagUnInf");
            //std::cout << "inside pClinicalDiagUpfrontForUninfectedPatients " << newData[dIndex].preTestMDClinicDiagUnInf.active << ' ' << newData[dIndex].preTestMDClinicDiagUnInf.mean << ' ' << newData[dIndex].preTestMDClinicDiagUnInf.upperBound << '\n';
        }
    }
    if (newData[dIndex].preTestLtoFFlag) {

        if (debug) dumpParameterRegionTier(newData[dIndex].preTestLtoFRegion, "preTestLtoFRegion");
    }
    if (newData[dIndex].preTreatLtoFFlag) {

        if (debug) dumpParameterRegionTier(newData[dIndex].preTreatLtoFRegion, "preTreatLtoFRegion");
    }
    if (newData[dIndex].treatLtoFFlag) {

        if (debug) dumpParameterRegionTier(newData[dIndex].treatLtoFRegion, "treatLtoFRegion");
    }
    for (int i = 0; i < maxNoDiseases; i++) {

        // debug - true, false
        if (debug) {
            std::cout << newData[i].disease << ' ' << newData[i].incidence.active << ' ' << newData[i].probRepresentGeoDist.active;
            std::cout << ' ' << newData[i].perIncPresenting.active << ' ' << newData[i].infUninfRatio.active << ' ' << newData[i].perInitInf.active << ' ' << newData[i].initTreatSuccess.active;
            std::cout << ' ' << newData[i].initTreatDeath.active << ' ' << newData[i].retreatmentPer.active  << ' ' << newData[i].representationLtoFTime.active;
            std::cout << ' ' << newData[i].noMachines.active << ' ' << newData[i].maxReagent.active;
            std::cout << ' ' << newData[i].timeStepsPerCycle.active << ' ' << newData[i].sampleWait.active << ' ' << newData[i].sensitivity.active;
            std::cout << ' ' << newData[i].specificity.active << ' ' << newData[i].MDSensitivity.active << ' ' << newData[i].MDSpecificity.active;
            std::cout << ' ' << newData[i].probPatientReturns.active << ' ' << newData[i].preTestLtoFFlag << ' ' << newData[i].preTreatLtoFFlag;
            std::cout << ' ' << newData[i].treatLtoFFlag << ' ' << newData[i].perRepresentLtoFDeath.active << '\n';
        }
    }
    

    // adjust for header row
    noChanges = noChanges -1;

    if (debug) std::cout << "inside change input parameters" <<   "\n";

    for (int rdIndex = 0; rdIndex < maxNoDiseases; rdIndex++) {

        if (newData[rdIndex].incidence.active || newData[rdIndex].probRepresentGeoDist.active) {
    
            // debug - true
            if (debug) {
                std::cout << " before incidence change disease = " << newData[rdIndex].disease <<  " incidence flag = " << newData[rdIndex].incidence.active;
                std::cout <<  " probRepresentGeoDist flag = " << newData[rdIndex].probRepresentGeoDist.active << '\n';
            }

            if (debug) {
            
                std::cout << "read region-distric-specific-data = " << regionDistrictSpecificInput << "\n";
            }

            // read in different tests parameters
            vector<vector<string>> regionDistrict = readDataInput(regionDistrictSpecificInput);
            if (regionDistrict.size() < 2) {
                std::cout << "In ChangeParameters - problem reading region district specific file\n";
            }
        
            if (debug) {
                std::cout << "after read region-distric-specific-data\n";
            }

            // set up range for incidence and modify
            inputChangeIncidence(regionDistrict, regionDistrictSpecificInput, newData[rdIndex].disease, newData[rdIndex].incidence, newData[rdIndex].probRepresentGeoDist, randomFlag, noBins, binNumber, alpha, inPlaceFlag, scenario, jobNo, wideOpenFlag);
        }

                
        if (newData[rdIndex].initTreatSuccess.active || newData[rdIndex].initTreatDeath.active || newData[rdIndex].retreatmentPer.active || newData[rdIndex].probPatientReturns.active || newData[rdIndex].preTestMDClinicDiagInf.active || newData[rdIndex].preTestMDClinicDiagUnInf.active || newData[rdIndex].preTestLtoFFlag || newData[rdIndex].preTreatLtoFFlag || newData[rdIndex].perRepresentLtoFDeath.active || newData[rdIndex].treatLtoFFlag || newData[rdIndex].timeDeathRetreat.active || newData[rdIndex].timeLtoFRepresent.active || newData[rdIndex].timeUntreatCure.active || newData[rdIndex].perIncPresenting.active || newData[rdIndex].infUninfRatio.active || newData[rdIndex].perInitInf.active) {

            // debug - true
            if (debug) {
                std::cout << "before disease change disease = " << newData[rdIndex].disease <<  " initTreatSuccess flag = " << newData[rdIndex].initTreatSuccess.active <<  " initTreatDeath flag = " << newData[rdIndex].initTreatDeath.active;
                std::cout << " test retreatment flag = " << newData[rdIndex].retreatmentPer.active <<  " test representation flag = " << newData[rdIndex].representationLtoFTime.active;
                std::cout << " probPatientReturns flag = " << newData[rdIndex].probPatientReturns.active << " preTestLtoF flag = " << newData[rdIndex].preTestLtoFFlag << " preTreatLtoF flag = " << newData[rdIndex].preTreatLtoFFlag << " treatLtoF flag = " << newData[rdIndex].treatLtoFFlag;
                std::cout << " perRepresentLtoFDeath flag = " << newData[rdIndex].perRepresentLtoFDeath.active << " timeDeathRetreat flag = " << newData[rdIndex].timeDeathRetreat.active << " timeLtoFRepresent flag = " << newData[rdIndex].timeLtoFRepresent.active;
                std::cout << " timeUntreatCure flag = " << newData[rdIndex].timeUntreatCure.active << '\n';
            }
            
            // read in different disease parameters
            vector<vector<string>> diseases = readDataInput(diseaseInput);
            if (diseases.size() < 2) {
                std::cout << "In ChangeParameters - problem reading disease file\n";;
            }

            // debug - true
            if (debug) {

                std::cout << "after read Disease-Objects!\n";

                std::cout << "preTestLtoFRegion ";
                for (int regIndex = 0; regIndex < 20; regIndex++) {

                    std::cout << " region = " << newData[rdIndex].preTestLtoFRegion[regIndex].region << " active -";
                    
                    for (int tierIndex = 0; tierIndex < 10; tierIndex++) {

                        std::cout << " i = " << tierIndex << " , " << newData[rdIndex].preTestLtoFRegion[regIndex].active[tierIndex];
                    }
                    std::cout << "\n";
                }
                
                std::cout << "preTreatLtoFRegion ";
                for (int regIndex = 0; regIndex < 20; regIndex++) {

                    std::cout << " region = " << newData[rdIndex].preTreatLtoFRegion[regIndex].region << "active -";;

                    for (int tierIndex = 0; tierIndex < 10; tierIndex++) {

                        std::cout << " i = " << tierIndex << " , " << newData[rdIndex].preTreatLtoFRegion[regIndex].active[tierIndex];
                    }
                    std::cout << "\n";
                }

                std::cout << "treatLtoFRegion ";
                for (int regIndex = 0; regIndex < 20; regIndex++) {

                    std::cout << " region = " << newData[rdIndex].treatLtoFRegion[regIndex].region << "active -";;

                    for (int tierIndex = 0; tierIndex < 10; tierIndex++) {

                        std::cout << " i = " << tierIndex << " , " << newData[rdIndex].treatLtoFRegion[regIndex].active[tierIndex];
                    }
                    std::cout << "\n";
                }
            }

            // vary different disease parameters
            inputChangeDisease(diseases, diseaseInput,  newData[rdIndex].disease, newData[rdIndex].infUninfRatio, newData[rdIndex].perInitInf, newData[rdIndex].perIncPresenting, newData[rdIndex].initTreatSuccess, newData[rdIndex].initTreatDeath, newData[rdIndex].retreatmentPer, newData[rdIndex].probPatientReturns, newData[rdIndex].preTestMDClinicDiagInf, newData[rdIndex].preTestMDClinicDiagUnInf, newData[rdIndex].preTestLtoF, newData[rdIndex].preTestLtoFRegion, newData[rdIndex].preTreatLtoF, newData[rdIndex].preTreatLtoFRegion, newData[rdIndex].treatLtoF, newData[rdIndex].treatLtoFRegion, newData[rdIndex].perRepresentLtoFDeath, newData[rdIndex].timeDeathRetreat, newData[rdIndex].timeLtoFRepresent, newData[rdIndex].timeUntreatCure, randomFlag, noBins, binNumber, alpha, inPlaceFlag, scenario, jobNo, wideOpenFlag);
        }

   
        if (newData[rdIndex].noMachines.active || newData[rdIndex].sampleWait.active) {
    
            if (debug) {
                std::cout << " before Facility-Specific-Disease-Data change disease = " << newData[rdIndex].disease;
                std::cout <<  " noMachines flag = " << newData[rdIndex].noMachines.active << '\n';
            }

            // read in disease specific information for each facility from file
            vector<vector<string>> facilitySpecific = readDataInput(facilitySpecificInput);
            if (facilitySpecific.size() < 2) {
                std::cout << "No facility specific file, using defaults.\n";
            }

            if (debug) {
                std::cout << "after read Facility-Specific-Disease-Data\n";
            }

            // modify facility specific parameters
            inputChangeFacility(facilitySpecific, facilitySpecificInput, newData[rdIndex].disease, newData[rdIndex].noMachines,  newData[rdIndex].sampleWait, randomFlag, noBins, binNumber, alpha, inPlaceFlag, scenario, jobNo, wideOpenFlag, returnArray);
        }

        if (newData[rdIndex].maxReagent.active || newData[rdIndex].timeStepsPerCycle.active || newData[rdIndex].sampleWait.active || newData[rdIndex].sensitivity.active || newData[rdIndex].sensitivityPCR.active || newData[rdIndex].sensitivityXpert.active || newData[rdIndex].specificity.active || newData[rdIndex].specificityPCR.active || newData[rdIndex].specificityXpert.active || newData[rdIndex].MDSensitivity.active || newData[rdIndex].MDSpecificity.active) {

            // debug - true
            if (debug) {
                std::cout << " before Test change disease = " << newData[rdIndex].disease <<  " maxReagent flag = " << newData[rdIndex].maxReagent.active;
                std::cout <<  " timeStepsPerCycle flag = " << newData[rdIndex].timeStepsPerCycle.active;
                std::cout << " sampleWait flag = " << newData[rdIndex].sampleWait.active;
                std::cout << " sensitivity flag = " << newData[rdIndex].sensitivity.active << " sensitivityPCR flag = " << newData[rdIndex].sensitivityPCR.active << " sensitivityXpert flag = " << newData[rdIndex].sensitivityXpert.active;
                std::cout << " specificity flag = " << newData[rdIndex].specificity.active << " specificityPCR flag = " << newData[rdIndex].specificityPCR.active << " specificityXpert flag = " << newData[rdIndex].specificityXpert.active;
                std::cout << " MDSensitivity flag = " << newData[rdIndex].MDSensitivity.active;
                std::cout <<  " MDSpecificity flag = " << newData[rdIndex].MDSpecificity.active << '\n';
            }

            // read in different tests parameters
            vector<vector<string>> tests = readDataInput(testInput);

            if (tests.size() < 2) {
                std::cout << "In ChangeParameters - problem reading tests file\n";;
            }
    
            if (debug) {
                std::cout << "after read Test-Objects!\n";
            }
            
            if (debug)  std::cout << "return array 1 = " << returnArray[0] <<  "\n";

            // change test parameters
            inputChangeTest(tests, testInput, newData[rdIndex].disease, newData[rdIndex].maxReagent, newData[rdIndex].timeStepsPerCycle, newData[rdIndex].sampleWait, newData[rdIndex].sensitivity, newData[rdIndex].sensitivityPCR, newData[rdIndex].sensitivityXpert, newData[rdIndex].specificity, newData[rdIndex].specificityPCR, newData[rdIndex].specificityXpert, newData[rdIndex].MDSensitivity, newData[rdIndex].MDSpecificity, randomFlag, noBins, binNumber, alpha, inPlaceFlag, returnArray[0], scenario, jobNo, wideOpenFlag);
        }

        // update MD override logic - turn off for now
        //if (newData[rdIndex].MDClinicDiag.active) {
        if (false) {

            // can only have a fixed number of states
            if (newData[rdIndex].MDClinicDiag.distribution == "States") {
            
                // rotate through the no. of states, maximum of 3

                // get random number between 0 and newData[rdIndex].MDClinicDiag.value = use global seed value
                // unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
                std::mt19937 generator(seed);
                std::uniform_int_distribution<> distribution(1,newData[rdIndex].MDClinicDiag.mean);

                randInt = distribution(generator);

                switch (randInt) {

                    case 1: 
                        MDClinicDiagText = newData[rdIndex].MDClinicDiag.lowerBound;
                        MDClinicDiagOption = 1;
                        break;
                    case 2: 
                        MDClinicDiagText = newData[rdIndex].MDClinicDiag.upperBound;
                        MDClinicDiagOption = 2;
                        break;
                    case 3:
                        MDClinicDiagText = newData[rdIndex].MDClinicDiag.shape1Alpha;
                        MDClinicDiagOption = 3;
                        break;
                    default: 
                        MDClinicDiagText = "RegionOnly";
                        MDClinicDiagOption = 1;
                        break;
                }
            }
        }
    }

    //std::cout << "Leaving ChangeParameters " <<   "\n";
    
}




// copy ./input/ paramaters from one directory to another
void readParameterIntoArray(vector<string > parameterInput, parameters* variable) {

    //struct parameters {

    //    bool active;
    //    string distribution;
    //    float mean;
    //    float lowerBound;
    //    float higerBound;
    //    float shape1Alpha;
    //    float shape2Beta;
    //    float scaleHP;
    //};

    bool debug = false;

    if (debug) {

        std::cout << "inside readParameterIntoArray parameterInput[3] = " << parameterInput[3] << " parameterInput[4] = " << parameterInput[4] << " parameterInput[5] = " << parameterInput[5];
        std::cout << " parameterInput[6] = " << parameterInput[6] << " parameterInput[7] = " << parameterInput[7] << " parameterInput[8] = " << parameterInput[8];
        std::cout << " parameterInput[9] = " << parameterInput[9] << " parameterInput[10] = " << parameterInput[10] << "\n";
    }
    
    variable->distribution = parameterInput[3];
    if (parameterInput[4] == "FALSE") {

        variable->active = false;

        variable->mean = 0;
        variable->lowerBound = 0;
        variable->upperBound = 0;
        variable->shape1Alpha = 0;
        variable->shape2Beta = 0;
        variable->scaleHB = 0;
    }
    else {

        variable->active = true;

        variable->mean = stof(parameterInput[5]);

        if (parameterInput[3] == "Beta") {

            variable->lowerBound = stof(parameterInput[6]);
            variable->upperBound = stof(parameterInput[7]);
            variable->shape1Alpha = stof(parameterInput[8]);
            variable->shape2Beta = stof(parameterInput[9]);
            variable->scaleHB = stof(parameterInput[10]);
        }
        else if (parameterInput[3] == "Uniform") {

            variable->lowerBound = stof(parameterInput[6]);
            variable->upperBound = stof(parameterInput[7]);
            variable->shape1Alpha = 0;
            variable->shape2Beta = 0;
            variable->scaleHB = 0;
        }
        else if (parameterInput[3] == "States") {

            variable->lowerBound = stof(parameterInput[6]);
            variable->upperBound = stof(parameterInput[7]);
            variable->shape1Alpha = stof(parameterInput[8]);
            variable->shape2Beta = stof(parameterInput[9]);
            variable->scaleHB = stof(parameterInput[10]);
        }
    }
    
    if (debug) std::cout << "inside readParameterIntoArray variable->upperBound = " << variable->upperBound << "\n";
}

// copy ./input/ paramaters from one directory to another
void readParameterRegionTierIntoArray(vector<string > parameterInput, string region, int tier,  struct parametersRegionTier* variable) {

    //struct parametersRegionTier {

    //    string region;
    //    bool active[10];
    //    string distribution[10];
    //    float value[10];
    //    float mean[10];
    //    float lowerBound[10];
    //    float upperBound[10];
    //    float shape1Alpha[10];
    //    float shape2Beta[10];
    //    float scaleHB[10];
    //};

    float shape1Alpha;
    float shape2Beta;
    float scaleHB;
    bool debug = false;

    if (false) {

        std::cout << "inside readParameterIntoArray " << "\n";
        std::cout << " parameterInput[0] = " << parameterInput[0] << " parameterInput[1] = " << parameterInput[1] << " parameterInput[2] = " << parameterInput[2];
        std::cout << " parameterInput[3] = " << parameterInput[3] << " parameterInput[4] = " << parameterInput[4] << " parameterInput[5] = " << parameterInput[5];
        std::cout << " parameterInput[6] = " << parameterInput[6] << " parameterInput[7] = " << parameterInput[7] << " parameterInput[8] = " << parameterInput[8];
        std::cout << " parameterInput[9] = " << parameterInput[9] << " parameterInput[10] = " << parameterInput[10] << "\n";
    }

    // adjust tier number to reflect arrays start at 0 instead of 1
    tier = tier - 1;

    if (parameterInput[4] == "FALSE") {

        if (debug) std::cout << " False branch of if statement " << "\n";

        variable->region = "";
        
        for (int i = 0; i < 10; i++) {
            
            variable->active[i] = false;
            variable->distribution[i] = parameterInput[3];
            variable->mean[i] = 0;
            variable->lowerBound[i] = 0;
            variable->upperBound[i] = 0;
            variable->shape1Alpha[i] = 0;
            variable->shape2Beta[i] = 0;
            variable->scaleHB[i] = 0;
        }
    }
    else {

        if (debug) std::cout << " True branch of if statement region " << region << " tier = " << tier << "\n";

        shape1Alpha = stof(parameterInput[8]);
        shape2Beta = stof(parameterInput[9]);
        scaleHB = stof(parameterInput[10]);

        if (parameterInput[3] == "Uniform") {

            shape1Alpha = 0;
            shape2Beta = 0;
            scaleHB = 0;
        }

        // update variable array depending on region
        // if region is all, update the tier values for all regions to input values
        // if region is a region, only update the tier values for that tier in that region
        
        for (int i = 0; i < 20; i++) {

            if ((variable + i)->region == region) {

                // add region name and tier value
                //(variable+i)->region = region;

                (variable + i)->active[tier] = true;
                (variable + i)->mean[tier] = stof(parameterInput[5]);
                (variable + i)->distribution[tier] = parameterInput[3];
                (variable + i)->lowerBound[tier] = stof(parameterInput[6]);
                (variable + i)->upperBound[tier] = stof(parameterInput[7]);
                (variable + i)->shape1Alpha[tier] = shape1Alpha;
                (variable + i)->shape2Beta[tier] = shape2Beta;
                (variable + i)->scaleHB[tier] = scaleHB;
            }
        }

        if (debug) std::cout << " True branch of if statement distribution " << parameterInput[3] << "\n";

  
    }

    if (debug) std::cout << "inside readParameterRegionTierIntoArray variable->upperBound = " << variable->upperBound << "\n";
}


// dump variables
void dumpParameter(parameters* variable, string name) {

    //struct parameters {

    //    bool active;
    //    string distribution;
    //    float mean;
    //    float lowerBound;
    //    float higerBound;
    //    float shape1Alpha;
    //    float shape2Beta;
    //    float scaleHP;
    //};


    std::cout << "dumpParameter " << name << " ," << variable->distribution << " ," << variable->active << " mean = " << variable->mean;
    std::cout << " lowerBound = " << variable->lowerBound << " upperBound = " << variable->upperBound << " shape1Alpha = " << variable->shape1Alpha;
    std::cout << " shape2Beta = " << variable->shape2Beta << " scaleHB = " << variable->scaleHB << "\n";

}

// dump variables
void dumpParameterRegionTier(struct parametersRegionTier* variable, string name) {

    //struct parametersRegionTier {

    //    string region;
    //    bool active[10];
    //    string distribution[10];
    //    float value[10];
    //    float mean[10];
    //    float lowerBound[10];
    //    float upperBound[10];
    //    float shape1Alpha[10];
    //    float shape2Beta[10];
    //    float scaleHB[10];
    //};

    std::cout << "dumpParameterRegionTier " << name <<  "\n";

    // 20 regions
    for (int i = 0; i < 20; i++) {

        std::cout << " region " << (variable+i)->region << "\n";

        std::cout << " active - ";

        for (int j = 0; j < 10; j++) {

            std::cout << ", " << j << " = " << (variable + i)->active[j];
        }
        std::cout << "\n";

        std::cout << "mean - ";

        for (int j = 0; j < 10; j++) {

            std::cout << ", " << j << " = " << (variable + i)->mean[j];
        }
        std::cout << "\n";
    }

}



// copy ./input/ paramaters from one directory to another
void copyInput(string source, string dest, int scenarioNo, string jobNo, string popBinFile, string diseaseFile, string facilitiesFile, scenarioParam fileNames[], string  parameterFlagFile, string filenameFile, string regDistFile, string populationFile, string regionDistrictSpecificFile, string runSpecificFile, string testFile) {
   

    // assume source directory exists.  Create destination directories if they don't exist, then copy files to new directories
    int errNo = 0;
    string inputDir = "/input/";
    string jobNoSource = "/job-" + jobNo;
    string jobNoDest = "/job-" + jobNo;
    bool debug = false;

    // add job number to path if source isn't "defaults"
    
    if (source == "./defaults") {

        jobNoSource = "";
    }

    // input directories
    string popBinDir = "catchment/";
    string diseaseDir = "diseases/";
    string facilitiesDir = "facilities/";
    string facilitySpecificDir = "facility-specific-data/";
    string parameterDir = "input-change-parameters/";
    string filenameDir = "input-file-names/";
    string populationDir = "population/";
    string regionDistrictSpecificDir = "region-district-specific-data/";
    string runSpecificDir = "run-specific-data/";
    string testDir = "tests/";

    // input files
    //string popBinFile = "Facility-Catchment-Population-Distribution.csv";
    //string diseaseFile = "diseases.csv";
    string diseaseTBUntreatMortFile = "survivalMortality_untreatedTB.csv";
    //string facilitiesFile = "facilities.csv";
    //string facilitiesFile = "case-reg-703-with-dist.csv";
    //string facilitySpecificFile1 = "facility-specific-data-tier1-2-3-xpert-only-by-tier.csv";
    //string facilitySpecificFile2 = "facility-specific-data-tier1-2-3-4-xpert-only-by-tier.csv";
    //string facilitySpecificFile2 = "facility-specific-data-current-model-169-plus-96-dist-hosp-xpert-testing.csv";
    //string facilitySpecificFile3 = "facility-specific-data-current-model-smear-testing1234-xpert-testing.csv";
    //string facilitySpecificFile4 = "facility-specific-data-current-model-smear-xpert-testing.csv";
    //string facilitySpecificFile5 = "facility-specific-data-tier1-7-xpert-only-by-tier.csv";
    //string facilitySpecificFile6 = "facility-specific-data-current-model-169-smear-testing1234-xpert-testing.csv";
    //string facilitySpecificFile7 = "facility-specific-data-current-model-169-29-smear-testing1234-xpert-testing.csv";
    //string parameterFlagFile = "parameter-range.csv";
    //string parameterFlagFile = "priorRangesAndDistributions.csv";
    //string filenameFile = "input-file-names.csv";
    // populationFile = "pop_1km_array_2015.csv";
    //string regionDistrictSpecificFile = "region-district-specific-data.csv";
    //string runSpecificFile = "run-specific-data.csv";
    //string testFile = "tests.csv";
    
    // which facility-specific-data.csv file to use depends on the scenario
    //if (source == "./defaults") {

    //    switch (scenarioNo) {
    //        case 1:
    //            // Xpert at tiers 1, 2, 3 only - no smear
    //            facilitySpecificFileInput = "facility-specific-data-tier1-2-3-xpert-only-by-tier.csv";
    //            std::cout << "Running scenario Xpert at tiers 1, 2, 3 " << "\n";
    //            break;
    //        case 2:
    //            // Xpert at tiers 1, 2, 3, 4 only - no smear
    //            facilitySpecificFileInput = "facility-specific-data-tier1-2-3-4-xpert-only-by-tier.csv";
    //            std::cout << "Running scenario Xpert at tiers 1, 2, 3, 4 " << "\n";
    //            break;
    //        case 3:
    //            // Xpert at tiers 1, 2, 3 and some 4 only - smear at tier 4
    //            facilitySpecificFileInput = "facility-specific-data-current-model-smear-testing1234-xpert-testing.csv";
    //            std::cout << "Running scenario Xpert at tiers 1, 2, 3, some 4, smear at tier 4, 5, 6 " << "\n";
    //            break;
    //        case 4:
    //            // Xpert at tier 1, 2, 3 and some 4 only only - smear at tiers 4, 5, 6
    //            facilitySpecificFileInput = "facility-specific-data-tier1-2-3-xpert-only-by-tier.csv";
    //            std::cout << "Running scenario Xpert at tiers 1, 2, 3  " << "\n";
    //            break;
    //        case 5:
    //            // Xpert at tiers 1, 2, 3 only - no smear
    //            facilitySpecificFileInput = "facility-specific-data-tier1-2-3-4-xpert-only-by-tier.csv";
    //            std::cout << "Running scenario Xpert at tiers 1, 2, 3, 4 " << "\n";
    //            break;
    //        case 6:
    //            // Xpert at tiers 1, 2, 3, 4 only - no smear
    //            facilitySpecificFileInput = "facility-specific-data-current-model-smear-testing1234-xpert-testing.csv";
    //            std::cout << "Running scenario Xpert at tiers 1, 2, 3, some 4, smear at tier 4, 5, 6 " << "\n";
    //            break;
    //        case 7:
    //            // Xpert at tiers 1, 2, 3 and some 4 only - smear at tier 4
    //            facilitySpecificFileInput = "facility-specific-data-tier1-2-3-xpert-only-by-tier.csv";
    //            std::cout << "Running scenario Xpert at tiers 1, 2, 3  " << "\n";
    //            break;
    //        case 8:
    //            // Xpert at tier 1, 2, 3 and some 4 only only - smear at tiers 4, 5, 6
    //            facilitySpecificFileInput = "facility-specific-data-tier1-2-3-4-xpert-only-by-tier.csv";
    //            std::cout << "Running scenario Xpert at tiers 1, 2, 3, 4 " << "\n";
    //            break;
    //        case 9:
    //            // Xpert at tier 1, 2, 3 and some 4 only only - smear at tiers 4, 5, 6
    //            facilitySpecificFileInput = "facility-specific-data-current-model-smear-testing1234-xpert-testing.csv";
    //            std::cout << "Running scenario Xpert at tiers 1, 2, 3, some 4, smear at tier 4, 5, 6 " << "\n";
    //            break;
    //        default :
    //            // Xpert at tiers 1, 2, 3 and some 4 only - smear at tier 4
    //            facilitySpecificFileInput = "facility-specific-data-current-model-smear-testing1234-xpert-testing.csv";
    //            std::cout << "Running scenario Xpert at tiers 1, 2, 3, some 4, smear at tier 4 " << "\n";
    //    }
    //}
    // debug - true
    if (debug) {
    
        std::cout << "inside copy input parameters source = " << source << " destination = " << dest << " scenario = " << scenarioNo;
        std::cout << " jobNo = " << jobNo << " jobNoSource = " << jobNoSource << ", jobNoDest = " << jobNoDest << ", facilitySpecificFile1 = " << fileNames[1].facilitySpecificFile << "\n";
    }

    string popBinInput = source + jobNoSource + inputDir + popBinDir + popBinFile;
    string diseaseInput = source + jobNoSource + inputDir + diseaseDir + diseaseFile;
    string diseaseTBUntreatMortInput = source + jobNoSource + inputDir + diseaseDir + diseaseTBUntreatMortFile;
    string facilitiesInput = source + jobNoSource + inputDir + facilitiesDir + facilitiesFile;
    string facilitySpecificInput1 = source + jobNoSource + inputDir + facilitySpecificDir + fileNames[1].facilitySpecificFile;
    string facilitySpecificInput2 = source + jobNoSource + inputDir + facilitySpecificDir + fileNames[2].facilitySpecificFile;
    string facilitySpecificInput3 = source + jobNoSource + inputDir + facilitySpecificDir + fileNames[3].facilitySpecificFile;
    string facilitySpecificInput4 = source + jobNoSource + inputDir + facilitySpecificDir + fileNames[4].facilitySpecificFile;
    string facilitySpecificInput5 = source + jobNoSource + inputDir + facilitySpecificDir + fileNames[5].facilitySpecificFile;
    string facilitySpecificInput6 = source + jobNoSource + inputDir + facilitySpecificDir + fileNames[6].facilitySpecificFile;
    string facilitySpecificInput7 = source + jobNoSource + inputDir + facilitySpecificDir + fileNames[7].facilitySpecificFile;
    string facilitySpecificInput8 = source + jobNoSource + inputDir + facilitySpecificDir + fileNames[8].facilitySpecificFile;
    string facilitySpecificInput9 = source + jobNoSource + inputDir + facilitySpecificDir + fileNames[9].facilitySpecificFile;
    string facilitySpecificInput10 = source + jobNoSource + inputDir + facilitySpecificDir + fileNames[10].facilitySpecificFile;
    string facilitySpecificInput11 = source + jobNoSource + inputDir + facilitySpecificDir + fileNames[11].facilitySpecificFile;
    string parameterInput = source + jobNoSource + inputDir + parameterDir + parameterFlagFile;
    string filenameInput = source + jobNoSource + inputDir + filenameDir + filenameFile;
    string regDistInput = source + jobNoSource + inputDir + filenameDir + regDistFile;
    string populationInput = source + jobNoSource + inputDir + populationDir + populationFile;
    string regionDistrictSpecificInput = source + jobNoSource + inputDir + regionDistrictSpecificDir + regionDistrictSpecificFile;
    string runSpecificInput = source + jobNoSource + inputDir + runSpecificDir + runSpecificFile;
    string testInput = source + jobNoSource + inputDir + testDir + testFile;

    if (debug)  {

        std::cout << "  source files = " << popBinInput << ", " << diseaseInput << ", " << diseaseTBUntreatMortInput << ", " << facilitiesInput << ", " << facilitySpecificInput1;
        std::cout << ", " << parameterInput << ", " << populationInput << ", " << regionDistrictSpecificInput << ", " << runSpecificInput << ", " << filenameInput << ", " << regDistInput << "\n";
    }
   
    string destDirBase = dest;
    string destDirBaseJobNo = dest + jobNoDest;
    string destDirBaseInput = dest + jobNoDest + inputDir;
    string popBinInputDest = dest + jobNoDest + inputDir + popBinDir;
    string diseaseInputDest = dest + jobNoDest + inputDir + diseaseDir;
    string diseaseTBUntreatMortInputDest = dest + jobNoDest + inputDir + diseaseDir;
    string facilitiesInputDest = dest + jobNoDest + inputDir + facilitiesDir;
    string facilitySpecificInputDest1 = dest + jobNoDest + inputDir + facilitySpecificDir;
    string facilitySpecificInputDest2 = dest + jobNoDest + inputDir + facilitySpecificDir;
    string facilitySpecificInputDest3 = dest + jobNoDest + inputDir + facilitySpecificDir;
    string facilitySpecificInputDest4 = dest + jobNoDest + inputDir + facilitySpecificDir;
    string facilitySpecificInputDest5 = dest + jobNoDest + inputDir + facilitySpecificDir;
    string facilitySpecificInputDest6 = dest + jobNoDest + inputDir + facilitySpecificDir;
    string facilitySpecificInputDest7 = dest + jobNoDest + inputDir + facilitySpecificDir;
    string facilitySpecificInputDest8 = dest + jobNoDest + inputDir + facilitySpecificDir;
    string facilitySpecificInputDest9 = dest + jobNoDest + inputDir + facilitySpecificDir;
    string facilitySpecificInputDest10 = dest + jobNoDest + inputDir + facilitySpecificDir;
    string facilitySpecificInputDest11 = dest + jobNoDest + inputDir + facilitySpecificDir;
    string parameterInputDest = dest + jobNoDest + inputDir + parameterDir;
    string filenameInputDest1 = dest + jobNoDest + inputDir + filenameDir;
    string filenameInputDest2 = dest + jobNoDest + inputDir + filenameDir;
    string populationInputDest = dest + jobNoDest + inputDir + populationDir;
    string regionDistrictSpecificInputDest = dest + jobNoDest + inputDir + regionDistrictSpecificDir;
    string runSpecificInputDest = dest + jobNoDest + inputDir + runSpecificDir;
    string testInputDest = dest + jobNoDest + inputDir + testDir;

    // debug - true
    if (debug)  {
        std::cout << "  dest dir = " << destDirBase << ", " << destDirBaseJobNo << ", " << destDirBaseInput << ", " << popBinInputDest;
        std::cout << ", " << diseaseInputDest << ", " << diseaseTBUntreatMortInputDest << ", " << facilitiesInputDest << ", " << facilitySpecificInputDest1 << ", " << parameterInputDest << ", " << filenameInputDest1 << "\n";
    }


    // convert to char array
    char subArrayBase[dest.length() +1];
    char subArrayJobNo[destDirBaseJobNo.length() +1];
    char subArrayInput[destDirBaseInput.length() +1];
    char subArrayPopBin[popBinInputDest.length() +1];
    char subArrayDisease[diseaseInputDest.length() +1];
    char subArrayFacilities[facilitiesInputDest.length() +1];
    char subArrayFacSpecific[facilitySpecificInputDest1.length() +1];
    char subArrayParam[parameterInputDest.length() +1];
    char subArrayFilename[filenameInputDest1.length() + 1];
    char subArrayPop[populationInputDest.length() +1];
    char subArrayRegDist[regionDistrictSpecificInputDest.length() +1];
    char subArrayRun[runSpecificInputDest.length() +1];
    char subArrayTest[testInputDest.length() +1];


    // copying the contents of the string to char array
    strcpy(subArrayBase, dest.c_str());
    strcpy(subArrayJobNo, destDirBaseJobNo.c_str());
    strcpy(subArrayInput, destDirBaseInput.c_str());
    strcpy(subArrayPopBin, popBinInputDest.c_str());
    strcpy(subArrayDisease, diseaseInputDest.c_str());
    strcpy(subArrayFacilities, facilitiesInputDest.c_str());
    strcpy(subArrayFacSpecific, facilitySpecificInputDest1.c_str());
    strcpy(subArrayParam, parameterInputDest.c_str());
    strcpy(subArrayFilename, filenameInputDest1.c_str());
    strcpy(subArrayPop, populationInputDest.c_str());
    strcpy(subArrayRegDist, regionDistrictSpecificInputDest.c_str());
    strcpy(subArrayRun, runSpecificInputDest.c_str());
    strcpy(subArrayTest, testInputDest.c_str());

    if (debug)  std::cout << "inside copy input parameters before make directory" << "\n";

    errNo = mkdir(subArrayBase, 0777);
    errNo = mkdir(subArrayJobNo, 0777);
    errNo = mkdir(subArrayInput, 0777);
    errNo = mkdir(subArrayPopBin, 0777);
    errNo = mkdir(subArrayDisease, 0777);
    errNo = mkdir(subArrayFacilities, 0777);
    errNo = mkdir(subArrayFacSpecific, 0777);
    errNo = mkdir(subArrayParam, 0777);
    errNo = mkdir(subArrayFilename, 0777);
    errNo = mkdir(subArrayPop, 0777);
    errNo = mkdir(subArrayRegDist, 0777);
    errNo = mkdir(subArrayRun, 0777);
    errNo = mkdir(subArrayTest, 0777);

    if (debug)  std::cout << "inside copy input parameters after make directory errNo = " << errNo << "\n";
    
    // now add file names to destination string
    destDirBaseInput = dest + jobNoDest + "/input/";
    popBinInputDest = dest + jobNoDest + inputDir + popBinDir + popBinFile;
    diseaseInputDest = dest + jobNoDest + inputDir + diseaseDir + diseaseFile;
    diseaseTBUntreatMortInputDest = dest + jobNoDest + inputDir + diseaseDir + diseaseTBUntreatMortFile;
    facilitiesInputDest = dest + jobNoDest + inputDir + facilitiesDir + facilitiesFile;
    facilitySpecificInputDest1 = dest + jobNoDest + inputDir + facilitySpecificDir + fileNames[1].facilitySpecificFile;
    facilitySpecificInputDest2 = dest + jobNoDest + inputDir + facilitySpecificDir + fileNames[2].facilitySpecificFile;
    facilitySpecificInputDest3 = dest + jobNoDest + inputDir + facilitySpecificDir + fileNames[3].facilitySpecificFile;
    facilitySpecificInputDest4 = dest + jobNoDest + inputDir + facilitySpecificDir + fileNames[4].facilitySpecificFile;
    facilitySpecificInputDest5 = dest + jobNoDest + inputDir + facilitySpecificDir + fileNames[5].facilitySpecificFile;
    facilitySpecificInputDest6 = dest + jobNoDest + inputDir + facilitySpecificDir + fileNames[6].facilitySpecificFile;
    facilitySpecificInputDest7 = dest + jobNoDest + inputDir + facilitySpecificDir + fileNames[7].facilitySpecificFile;
    facilitySpecificInputDest8 = dest + jobNoDest + inputDir + facilitySpecificDir + fileNames[8].facilitySpecificFile;
    facilitySpecificInputDest9 = dest + jobNoDest + inputDir + facilitySpecificDir + fileNames[9].facilitySpecificFile;
    facilitySpecificInputDest10 = dest + jobNoDest + inputDir + facilitySpecificDir + fileNames[10].facilitySpecificFile;
    facilitySpecificInputDest11 = dest + jobNoDest + inputDir + facilitySpecificDir + fileNames[11].facilitySpecificFile;
    parameterInputDest = dest + jobNoDest + inputDir + parameterDir + parameterFlagFile;
    filenameInputDest1 = dest + jobNoDest + inputDir + filenameDir + filenameFile;
    filenameInputDest2 = dest + jobNoDest + inputDir + filenameDir + regDistFile;
    populationInputDest = dest + jobNoDest + inputDir + populationDir + populationFile;
    regionDistrictSpecificInputDest = dest + jobNoDest + inputDir + regionDistrictSpecificDir + regionDistrictSpecificFile;
    runSpecificInputDest = dest + jobNoDest + inputDir + runSpecificDir + runSpecificFile;
    testInputDest = dest + jobNoDest + inputDir + testDir + testFile;


     if (debug)  {
        std::cout << "  dest files = " << popBinInputDest << ", " << diseaseInputDest << ", " << diseaseTBUntreatMortInputDest << ", " << facilitiesInputDest << ", " << facilitySpecificInputDest1;
        std::cout << ", " << parameterInputDest << ", " << populationInputDest << ", " << regionDistrictSpecificInputDest << ", " << runSpecificInputDest << ", " << filenameInputDest1 << ", " << filenameInputDest2 << "\n";
    }
   

    // now copy the source files into the dest subdirectories
    std::ifstream  src1(populationInput, std::ios::binary);
    std::ifstream  src2(diseaseInput, std::ios::binary);
    std::ifstream  src3(testInput, std::ios::binary);
    std::ifstream  src4(facilitiesInput, std::ios::binary);
    std::ifstream  src5(facilitySpecificInput1, std::ios::binary);
    std::ifstream  src5b(facilitySpecificInput2, std::ios::binary);
    std::ifstream  src5c(facilitySpecificInput3, std::ios::binary);
    std::ifstream  src5d(facilitySpecificInput4, std::ios::binary);
    std::ifstream  src5e(facilitySpecificInput5, std::ios::binary);
    std::ifstream  src5f(facilitySpecificInput6, std::ios::binary);
    std::ifstream  src5g(facilitySpecificInput7, std::ios::binary);
    std::ifstream  src5h(facilitySpecificInput8, std::ios::binary);
    std::ifstream  src5i(facilitySpecificInput9, std::ios::binary);
    std::ifstream  src5j(facilitySpecificInput10, std::ios::binary);
    std::ifstream  src5k(facilitySpecificInput11, std::ios::binary);
    std::ifstream  src6(regionDistrictSpecificInput, std::ios::binary);
    std::ifstream  src7(runSpecificInput, std::ios::binary);
    std::ifstream  src8(popBinInput, std::ios::binary);
    std::ifstream  src9(parameterInput, std::ios::binary);
    std::ifstream  src10a(filenameInput, std::ios::binary);
    std::ifstream  src10b(regDistInput, std::ios::binary);
    std::ifstream  src11(diseaseTBUntreatMortInput, std::ios::binary);

    std::ofstream  dst1(populationInputDest,   std::ios::binary);
    std::ofstream  dst2(diseaseInputDest,   std::ios::binary);
    std::ofstream  dst3(testInputDest,   std::ios::binary);
    std::ofstream  dst4(facilitiesInputDest,   std::ios::binary);
    std::ofstream  dst5(facilitySpecificInputDest1,  std::ios::binary);
    std::ofstream  dst5b(facilitySpecificInputDest2, std::ios::binary);
    std::ofstream  dst5c(facilitySpecificInputDest3, std::ios::binary);
    std::ofstream  dst5d(facilitySpecificInputDest4, std::ios::binary);
    std::ofstream  dst5e(facilitySpecificInputDest5, std::ios::binary);
    std::ofstream  dst5f(facilitySpecificInputDest6, std::ios::binary);
    std::ofstream  dst5g(facilitySpecificInputDest7, std::ios::binary);
    std::ofstream  dst5h(facilitySpecificInputDest8, std::ios::binary);
    std::ofstream  dst5i(facilitySpecificInputDest9, std::ios::binary);
    std::ofstream  dst5j(facilitySpecificInputDest10, std::ios::binary);
    std::ofstream  dst5k(facilitySpecificInputDest11, std::ios::binary);
    std::ofstream  dst6(regionDistrictSpecificInputDest,   std::ios::binary);
    std::ofstream  dst7(runSpecificInputDest,   std::ios::binary);
    std::ofstream  dst8(popBinInputDest,   std::ios::binary);
    std::ofstream  dst9(parameterInputDest,   std::ios::binary);
    std::ofstream  dst10a(filenameInputDest1, std::ios::binary);
    std::ofstream  dst10b(filenameInputDest2, std::ios::binary);
    std::ofstream  dst11(diseaseTBUntreatMortInputDest, std::ios::binary);

    if (debug)  std::cout << "inside copy input parameters before copy individual files" << "\n";
    
    dst1 << src1.rdbuf();
    dst2 << src2.rdbuf();
    dst3 << src3.rdbuf();
    dst4 << src4.rdbuf();
    dst5 << src5.rdbuf();
    dst5b << src5b.rdbuf();
    dst5c << src5c.rdbuf();
    dst5d << src5d.rdbuf();
    dst5e << src5e.rdbuf();
    dst5f << src5f.rdbuf();
    dst5g << src5g.rdbuf();
    dst5h << src5h.rdbuf();
    dst5i << src5i.rdbuf();
    dst5j << src5j.rdbuf();
    dst5k << src5k.rdbuf();
    dst6 << src6.rdbuf();
    dst7 << src7.rdbuf();
    dst8 << src8.rdbuf();
    dst9 << src9.rdbuf();
    dst10a << src10a.rdbuf();
    dst10b << src10b.rdbuf();
    dst11 << src11.rdbuf();

    if (debug)  std::cout << "inside copy input parameters after copy individual files" << "\n";

    // make sure copied files are closed
    dst1.close();
    dst2.close();
    dst3.close();
    dst4.close();
    dst5.close();
    dst5b.close();
    dst5c.close();
    dst5d.close();
    dst5e.close();
    dst5f.close();
    dst5g.close();
    dst5h.close();
    dst5i.close();
    dst5j.close();
    dst5k.close();
    dst6.close();
    dst7.close();
    dst8.close();
    dst9.close();
    dst10a.close();
    dst10b.close();
    dst11.close();

    // debug - true
    if (debug)  std::cout << "leaving copy input parameters" << "\n";
}

