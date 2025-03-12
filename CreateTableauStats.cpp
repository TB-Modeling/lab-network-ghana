//
//  CreateTableauStats.cpp
// 
//
//  Created by L. Dombrowski on 4/29/20

//
// Split out of CreateStats on 5/14/20

#include <vector>
#include <string>
#include <cstring>
#include <sys/stat.h>
#include <errno.h>

#include "CreateResourceStats.h"
#include "GlobalParameters.h"
#include "FacilitiesList.h"

using namespace std;






// create stats for use in tableau 
void tableauStats(FacilitiesList* list, vector<string> diseaseList, vector<Test*> testList, bool fileFlag, bool invdividualFlag, int timeStep, int year) {

    Diagnosis* d;
    Testing* t;
    vector<facilityAttributes*> fa;
    vector<Patient*> p;
    vector<Patient*> pI;
    vector<Sample*> samples;
    vector<Sample*> samplesInactive;
    vector<Test*> tests;
    ofstream myfile;
    ofstream myfile1;

    int count = 0;
    int facilityCountP = 0;
    int countF = 0;
    int facilityPop = 0;
    int status;
    int lagTime;
    int timeSteps;
    int errNo;
    float sampleTime;
    //float longitude;
    //float latitude;

    int diseaseCount;
    int diseaseIndex = 0;
    int testCount;
    int testStorageCount;
    int testIndex = 0;
    int samplesSize = 0;
    int sampleIndex = 0;
    string diseaseNameTest;
    string diseaseName;
    string testName;
    string firstTestName = "";
    string secondTestName = "";
    string subDirBase;

    // keep patient statistics by region and district
    struct geographicalStats {
        string name;
        string facilityType;
        string region;
        string district;
        string facID;
        int countP = 0;
        int countPU = 0;
        int countPI = 0;
        int countPT = 0;
        int countPR = 0;
        int countPUT = 0;
        int countPC = 0;
        int countPDI = 0;
        int countPDU = 0;
        int countPO = 0;
        int countPUI = 0;
        int countPUINR = 0;
        int countPUIUT = 0;
        int countPTPos = 0;
        int countPFNeg = 0;
        int countPTNeg = 0;
        int countPFPos = 0;
        int countPMDTPos = 0;
        int countPMDFNeg = 0;
        int countPMDTNeg = 0;
        int countPMDFPos = 0;
        int countPwdTPos = 0;
        int countPwdFNeg = 0;
        int countPwdTNeg = 0;
        int countPwdFPos = 0;
        int countPwdMDTPos = 0;
        int countPwdMDFNeg = 0;
        int countPwdMDTNeg = 0;
        int countPwdMDFPos = 0;
        int timeToDrugs = 0;
        int noPatientsGettingDrugs = 0;
        int infectionDays = 0;
        int noPatientsInfectionDays = 0;
        int pop = 0;
        int withTB = 0;
        int avgTimeToDrugs = 0;
        int avgInfectionDays = 0;
        int infected = 0;
        int uninfected = 0;
        int sampleCount = 0;
        float sampleTotalTime = 0;
        float prevalence = 0;
        float yll = 0;

        // a vector of patient container vectors
        vector< vector<Patient*> > patientContainer;
        int totalTests = 0;
    };


    // find out how many diseases to save stats for
    // use this as an count to create arrays for each of the diseases being run
    diseaseCount = diseaseList.size();
    testCount = testList.size();

    // keep track of the data at the test level for true/false
    // assume at most 2 tests plus lost to followup plus patients whose samples haven't been tested yet
    // testIndex = 0 is where first test statistics are stored
    // testIndex = 1 is where second test statistics are stored
    // testIndex = 2 is where Lost to Follow up statistics are stored, also statistics that are a combination of first and second tests are stored here
    // testIndex = 3 is where tests that haven't been processed yet are stored
    testStorageCount = 4;

    geographicalStats total;
    geographicalStats diseaseTotal[diseaseCount];

    // keep patient statistics for each disease for each facility
    int patientCount[diseaseCount];

    vector<Facility*> facilities = list->getListPtr();

    countF = facilities.size();

    // std::cout << "stats: agents no. of facilities =  " << countF << "\n";

    // get statistical array for each facility
    geographicalStats facilityStats[diseaseCount][testStorageCount][countF];

    // initialize total structure
    total.pop = 0;
    total.countP = 0;
    total.countPU = 0;
    total.countPI = 0;
    total.countPT = 0;
    total.countPR = 0;
    total.countPUT = 0;
    total.countPC = 0;
    total.countPDI = 0;
    total.countPDU = 0;
    total.countPO = 0;
    total.countPUI = 0;
    total.countPUINR = 0;
    total.countPUIUT = 0;
    total.countPTPos = 0;
    total.countPFNeg = 0;
    total.countPTNeg = 0;
    total.countPFPos = 0;
    total.countPMDTPos = 0;
    total.countPMDFNeg = 0;
    total.countPMDTNeg = 0;
    total.countPMDFPos = 0;
    total.countPwdTPos = 0;
    total.countPwdFNeg = 0;
    total.countPwdTNeg = 0;
    total.countPwdFPos = 0;
    total.countPwdMDTPos = 0;
    total.countPwdMDFNeg = 0;
    total.countPwdMDTNeg = 0;
    total.countPwdMDFPos = 0;
    total.timeToDrugs = 0;
    total.noPatientsGettingDrugs = 0;
    total.infectionDays = 0;
    total.noPatientsInfectionDays = 0;
    total.sampleTotalTime = 0;
    total.withTB = 0;
    total.avgTimeToDrugs = 0;
    total.avgInfectionDays = 0;
    total.sampleCount = 0;
    total.infected = 0;
    total.uninfected = 0;
    total.prevalence = 0;
    total.yll = 0;
    total.totalTests = 0;

    if (statSubDir.length() > 0) {
        subDirBase = "./stats/" + statSubScenarioDir + "/job-" + statSubDir;
    }
    else {
        subDirBase = "./stats/" + statSubScenarioDir + "/job-" + to_string(1);
    }
  
    // convert to char array
    char subArray[subDirBase.length() +1];
    // copying the contents of the
    // string to char array
    strcpy(subArray, subDirBase.c_str());

    errno = 0;
            
    errNo = mkdir(subArray, 0777);

    // check if directory is created or currently exists 
    if ((errNo != 0) && (errno != EEXIST)) {

        //std::cout << "unable to create directory = " << subArray << " errNO = " << errNo << " errno = " << errno << " EEXIST = " << EEXIST << "\n"; 
    }

    // if fileFlag is true, output to a file
    if (fileFlag) {

        myfile.open(subDirBase + "/Tableau-stats.csv");

        // output time stamp in file and other parameters that we varied for this run
        //time_t rawtime;
        //struct tm* timeinfo;
        //char buffer[80];

        //time(&rawtime);
        //timeinfo = localtime(&rawtime);

        //strftime(buffer, sizeof(buffer), "%d-%m-%Y %H:%M:%S", timeinfo);
        //std::string str(buffer);

        //std::cout << str << "\n";
        //myfile << "Date" << ", " << ", " << ", " << ", " << ", " << ", " << ", " << ", " << "\n";
        //myfile << str << "," << "," << "," << "," << "," << "," << "," << "," << "\n";

    }

    // dump statistics for each patient - only do it to file
    if (invdividualFlag) {

        myfile1.open(subDirBase + "/TableauPatientsFacility.csv");
    }

    // loop through each facility and the number of Patients and Samples at each facility
    for (auto facility : facilities) {

        //std::cout << "stats: inside facility loop fac =  " << facility->getName() << "\n";

        // get the up name for this facility
        d = facility->getDiagnosisPtr();
        t = facility->getTestingPtr();
        p = d->getPatientPtr();
        pI = d->getInactivePatientPtr();
        samples = t->getSamplePtr();
        samplesInactive = t->getInactiveSamplePtr();
        fa = facility->getFacilityAttributesPtr();
        tests = t->getTeststPtr();
        facilityPop = facility->getPopulation();
        //longitude = facility->getLongitude();
        //latitude = facility->getLatitude();
        total.pop = total.pop + facilityPop;

        // get patient stats
        facilityCountP = p.size();
        total.countP = total.countP + facilityCountP;

        // update facility statistics
        for (int w = 0; w < diseaseCount; w++) {

            for (int v = 0; v < testStorageCount; v++) {
                facilityStats[w][v][count].name = facility->getName();
                facilityStats[w][v][count].facilityType = facility->getType();
                facilityStats[w][v][count].region = facility->getRegion();
                facilityStats[w][v][count].district = facility->getDistrict();
                facilityStats[w][v][count].facID = facility->getID();
                facilityStats[w][v][count].pop = facilityPop;
                facilityStats[w][v][count].countP = 0;
                facilityStats[w][v][count].countPU = 0;
                facilityStats[w][v][count].countPI = 0;
                facilityStats[w][v][count].countPT = 0;
                facilityStats[w][v][count].countPR = 0;
                facilityStats[w][v][count].countPUT = 0;
                facilityStats[w][v][count].countPC = 0;
                facilityStats[w][v][count].countPDI = 0;
                facilityStats[w][v][count].countPDU = 0;
                facilityStats[w][v][count].countPO = 0;
                facilityStats[w][v][count].countPUI = 0;
                facilityStats[w][v][count].countPUINR = 0;
                facilityStats[w][v][count].countPUIUT = 0;
                facilityStats[w][v][count].countPTPos = 0;
                facilityStats[w][v][count].countPFNeg = 0;
                facilityStats[w][v][count].countPTNeg = 0;
                facilityStats[w][v][count].countPFPos = 0;
                facilityStats[w][v][count].countPMDTPos = 0;
                facilityStats[w][v][count].countPMDFNeg = 0;
                facilityStats[w][v][count].countPMDTNeg = 0;
                facilityStats[w][v][count].countPMDFPos = 0;
                facilityStats[w][v][count].countPwdTPos = 0;
                facilityStats[w][v][count].countPwdFNeg = 0;
                facilityStats[w][v][count].countPwdTNeg = 0;
                facilityStats[w][v][count].countPwdFPos = 0;
                facilityStats[w][v][count].countPwdMDTPos = 0;
                facilityStats[w][v][count].countPwdMDFNeg = 0;
                facilityStats[w][v][count].countPwdMDTNeg = 0;
                facilityStats[w][v][count].countPwdMDFPos = 0;
                facilityStats[w][v][count].timeToDrugs = 0;
                facilityStats[w][v][count].noPatientsGettingDrugs = 0;
                facilityStats[w][v][count].infectionDays = 0;
                facilityStats[w][v][count].noPatientsInfectionDays = 0;
                facilityStats[w][v][count].sampleTotalTime = 0;
                facilityStats[w][v][count].withTB = 0;
                facilityStats[w][v][count].avgTimeToDrugs = 0;
                facilityStats[w][v][count].avgInfectionDays = 0;
                facilityStats[w][v][count].sampleCount = 0;
                facilityStats[w][v][count].infected = 0;
                facilityStats[w][v][count].uninfected = 0;
                facilityStats[w][v][count].prevalence = 0;
                facilityStats[w][v][count].yll = 0;
                facilityStats[w][v][count].totalTests = 0;
            }
            
            patientCount[w] = 0;
        }
        //std::cout << "stats: inside facility loop after initialization  " << "\n";

        if (invdividualFlag) {

            myfile1 << "-----------------------------------------------------------------------------------  " << "\n";
            myfile1 << "stats: fac =  " << facilityStats[0][0][count].name << ", patients = " << facilityCountP << "\n";
            myfile1 << " status " << ", disease  " << ", test " << ", age " << ", alive " << ", infected " << ", time from onset " << ", time in treatment" << ", time not in treatment " << "\n";
        }

        for (auto pat : p) {

            //std::cout << "stats: inside patient loop  disease name = " << pat->getDisease() << ", diseaseCount = " << diseaseCount << "\n";

            // store stats by disease
            diseaseName = pat->getDisease();

            // get the index for the disease
            diseaseIndex = 0;

            for (int w = 0; w < diseaseCount; w++) {

                if (diseaseName == diseaseList[w]) {
                    diseaseIndex = w;

                    // increase the patient count for this disease for this facility
                    patientCount[w]++;
                }
            }

           // std::cout << "stats: inside patient loop disease name  " << diseaseName << ", index = " << diseaseIndex << "\n";

            // store stats as a subcategory of test
            testName = pat->getSampleTest();
            
            // get the index for the disease
            testIndex = 0;

            if (testName == "LtoF") {

                testIndex = 2;
            }
            else if (testName == "") {

                testIndex = 3;
            }
            else if (testName == "none") {

                testIndex = 3;
            }
            else
            {
                // make first test encountered index 0
                if (firstTestName == "") {

                    // get the each test name associated with this disease
                    for (int testIndex = 0; testIndex < testCount; testIndex++) {

                        if (diseaseName == testList[testIndex]->getDiseaseName()) {

                            if (firstTestName == "") {
                                firstTestName = testList[testIndex]->getTestName();
                            }
                            else {
                                secondTestName = testList[testIndex]->getTestName();
                            }

                        }
                    }
                }
                // anything that matches firstTestName is index 0, everything else is index 1
                if (testName == firstTestName) {

                    testIndex = 0;
                }
                else {

                    testIndex = 1;
                }
            }
           //std::cout << "test name =  " << testName << ", first test name =  " << firstTestName << ", second test name =  " << secondTestName << ", test index =  " << testIndex << "\n";

            // 0 - away from facility - hasn't returned for results
            // 1 - in-house
            // 2 - sample results returned, undergoing treatment
            // 3 - sample results returned, treatment failure
            // 4 - sample results returned, no treatment
            // 5 - no sample results returned, no treatment
            // 6 - sample results returned, no treatment LtoF
            // 7 - patient dead
            // 8 - patient cured
            // 9 - patient uninfected - returned sample
            // 10 - patient uninfected - didn't return for sample
            // 11 - patient uninfected - under treatment
            status = pat->getStatus();

            //std::cout << "stats:patient test Name  =  " << testName << ", status = " << status << "\n";

            switch (status) {
            case 0:                     // 0 - away from facility - hasn't returned for results
                total.countPU++;
                diseaseTotal[diseaseIndex].countPU++;
                facilityStats[diseaseIndex][testIndex][count].countPU++;
                break;
            case 1:                     // 1 - in-house
                total.countPI++;
                diseaseTotal[diseaseIndex].countPI++;
                facilityStats[diseaseIndex][testIndex][count].countPI++;
                break;
            case 2:                     // 2 - sample results returned, undergoing treatment
                total.countPT++;
                diseaseTotal[diseaseIndex].countPT++;
                facilityStats[diseaseIndex][testIndex][count].countPT++;
                if (pat->getTestPos()) {
                    // will yield True Positve and True Negative
                    if (pat->getInfected()) {
                        total.countPwdTPos++;
                        diseaseTotal[diseaseIndex].countPwdTPos++;
                        facilityStats[diseaseIndex][testIndex][count].countPwdTPos++;

                        // add these to MD true-postive count as well
                        total.countPwdMDTPos++;
                        diseaseTotal[diseaseIndex].countPwdMDTPos++;
                        facilityStats[diseaseIndex][testIndex][count].countPwdMDTPos++;
                    }
                    else {
                        total.countPwdTNeg++;
                        diseaseTotal[diseaseIndex].countPwdTNeg++;
                        facilityStats[diseaseIndex][testIndex][count].countPwdTNeg++;

                        // MD sensitivitiy and specificity on applies to True Negative and False Negative
                        if (pat->getMDPos()) {
                            total.countPwdMDTNeg++;
                            diseaseTotal[diseaseIndex].countPwdMDTNeg++;
                            facilityStats[diseaseIndex][testIndex][count].countPwdMDTNeg++;
                        }
                        else {
                            total.countPwdMDFPos++;
                            diseaseTotal[diseaseIndex].countPwdMDFPos++;
                            facilityStats[diseaseIndex][testIndex][count].countPwdMDFPos++;
                        }
                    }
                }
                else {
                    // will yield False Negative and False Positive
                    if (pat->getInfected()) {
                        total.countPwdFNeg++;
                        diseaseTotal[diseaseIndex].countPwdFNeg++;
                        facilityStats[diseaseIndex][testIndex][count].countPwdFNeg++;

                        // MD sensitivitiy and specificity on applies to True Negative and False Negative
                        if (pat->getMDPos()) {
                            total.countPwdMDTPos++;
                            diseaseTotal[diseaseIndex].countPwdMDTPos++;
                            facilityStats[diseaseIndex][testIndex][count].countPwdMDTPos++;
                        }
                        else {
                            total.countPwdMDFNeg++;
                            diseaseTotal[diseaseIndex].countPwdMDFNeg++;
                            facilityStats[diseaseIndex][testIndex][count].countPwdMDFNeg++;
                        }
                    }
                    else {
                        total.countPwdFPos++;
                        diseaseTotal[diseaseIndex].countPwdFPos++;
                        facilityStats[diseaseIndex][testIndex][count].countPwdFPos++;
                    }
                }
                
                // store time in treatment
                timeSteps = pat->getTimeStepsInTreatment() + 14;
                total.infectionDays = total.infectionDays + timeSteps;
                diseaseTotal[diseaseIndex].infectionDays = diseaseTotal[diseaseIndex].infectionDays + timeSteps;
                facilityStats[diseaseIndex][testIndex][count].infectionDays = facilityStats[diseaseIndex][testIndex][count].infectionDays + timeSteps;
                total.noPatientsInfectionDays++;
                diseaseTotal[diseaseIndex].noPatientsInfectionDays++;
                facilityStats[diseaseIndex][testIndex][count].noPatientsInfectionDays++;
                // time from sending sample out to getting drugs
                timeSteps = pat->getResultsReturnedTime() - pat->getCreateTime();
                if (timeSteps > 1) {
                    // patient never received results - don't count them
                    total.timeToDrugs = total.timeToDrugs + timeSteps;
                    diseaseTotal[diseaseIndex].timeToDrugs = diseaseTotal[diseaseIndex].timeToDrugs + timeSteps;
                    facilityStats[diseaseIndex][testIndex][count].timeToDrugs = facilityStats[diseaseIndex][testIndex][count].timeToDrugs + timeSteps;
                    total.noPatientsGettingDrugs++;
                    diseaseTotal[diseaseIndex].noPatientsGettingDrugs++;
                    facilityStats[diseaseIndex][testIndex][count].noPatientsGettingDrugs++;
                }
                break;
            case 3:                     // 3 - sample results returned, treatment failure
                total.countPR++;
                diseaseTotal[diseaseIndex].countPR++;
                facilityStats[diseaseIndex][testIndex][count].countPR++;
                if (pat->getTestPos()) {
                    // will yield True Positve and True Negative
                    if (pat->getInfected()) {
                        total.countPwdTPos++;
                        diseaseTotal[diseaseIndex].countPwdTPos++;
                        facilityStats[diseaseIndex][testIndex][count].countPwdTPos++;

                        // add these to MD true-postive count as well
                        total.countPwdMDTPos++;
                        diseaseTotal[diseaseIndex].countPwdMDTPos++;
                        facilityStats[diseaseIndex][testIndex][count].countPwdMDTPos++;
                    }
                    else {
                        total.countPwdTNeg++;
                        diseaseTotal[diseaseIndex].countPwdTNeg++;
                        facilityStats[diseaseIndex][testIndex][count].countPwdTNeg++;

                        // MD sensitivitiy and specificity on applies to True Negative and False Negative
                        if (pat->getMDPos()) {
                            total.countPwdMDTNeg++;
                            diseaseTotal[diseaseIndex].countPwdMDTNeg++;
                            facilityStats[diseaseIndex][testIndex][count].countPwdMDTNeg++;
                        }
                        else {
                            total.countPwdMDFPos++;
                            diseaseTotal[diseaseIndex].countPwdMDFPos++;
                            facilityStats[diseaseIndex][testIndex][count].countPwdMDFPos++;
                        }
                    }
                }
                else {
                    // will yield False Negative and False Positive
                    if (pat->getInfected()) {
                        total.countPwdFNeg++;
                        diseaseTotal[diseaseIndex].countPwdFNeg++;
                        facilityStats[diseaseIndex][testIndex][count].countPwdFNeg++;

                        // MD sensitivitiy and specificity on applies to True Negative and False Negative
                        if (pat->getMDPos()) {
                            total.countPwdMDTPos++;
                            diseaseTotal[diseaseIndex].countPwdMDTPos++;
                            facilityStats[diseaseIndex][testIndex][count].countPwdMDTPos++;
                        }
                        else {
                            total.countPwdMDFNeg++;
                            diseaseTotal[diseaseIndex].countPwdMDFNeg++;
                            facilityStats[diseaseIndex][testIndex][count].countPwdMDFNeg++;
                        }
                    }
                    else {
                        total.countPwdFPos++;
                        diseaseTotal[diseaseIndex].countPwdFPos++;
                        facilityStats[diseaseIndex][testIndex][count].countPwdFPos++;
                    }
                }

                // store time in treatment
                timeSteps = pat->getTimeStepsInTreatment() + 14;
                total.infectionDays = total.infectionDays + timeSteps;
                diseaseTotal[diseaseIndex].infectionDays = diseaseTotal[diseaseIndex].infectionDays + timeSteps;
                facilityStats[diseaseIndex][testIndex][count].infectionDays = facilityStats[diseaseIndex][testIndex][count].infectionDays + timeSteps;
                total.noPatientsInfectionDays++;
                diseaseTotal[diseaseIndex].noPatientsInfectionDays++;
                facilityStats[diseaseIndex][testIndex][count].noPatientsInfectionDays++;
                // time from sending sample out to getting drugs
                timeSteps = pat->getResultsReturnedTime() - pat->getCreateTime();
                if (timeSteps > 1) {
                    // patient never received results - don't count them
                    total.timeToDrugs = total.timeToDrugs + timeSteps;
                    diseaseTotal[diseaseIndex].timeToDrugs = diseaseTotal[diseaseIndex].timeToDrugs + timeSteps;
                    facilityStats[diseaseIndex][testIndex][count].timeToDrugs = facilityStats[diseaseIndex][testIndex][count].timeToDrugs + timeSteps;
                    total.noPatientsGettingDrugs++;
                    diseaseTotal[diseaseIndex].noPatientsGettingDrugs++;
                    facilityStats[diseaseIndex][testIndex][count].noPatientsGettingDrugs++;
                }
                break;
            case 4:                     // 4 - sample results returned, no treatment
                total.countPUT++;
                diseaseTotal[diseaseIndex].countPUT++;
                facilityStats[diseaseIndex][testIndex][count].countPUT++;
                if (pat->getTestPos()) {
                    // will yield True Positve and True Negative
                    if (pat->getInfected()) {
                        total.countPwdTPos++;
                        diseaseTotal[diseaseIndex].countPwdTPos++;
                        facilityStats[diseaseIndex][testIndex][count].countPwdTPos++;

                        // add these to MD true-postive count as well
                        total.countPwdMDTPos++;
                        diseaseTotal[diseaseIndex].countPwdMDTPos++;
                        facilityStats[diseaseIndex][testIndex][count].countPwdMDTPos++;
                    }
                    else {
                        total.countPwdTNeg++;
                        diseaseTotal[diseaseIndex].countPwdTNeg++;
                        facilityStats[diseaseIndex][testIndex][count].countPwdTNeg++;

                        // MD sensitivitiy and specificity on applies to True Negative and False Negative
                        if (pat->getMDPos()) {
                            total.countPwdMDTNeg++;
                            diseaseTotal[diseaseIndex].countPwdMDTNeg++;
                            facilityStats[diseaseIndex][testIndex][count].countPwdMDTNeg++;
                        }
                        else {
                            total.countPwdMDFPos++;
                            diseaseTotal[diseaseIndex].countPwdMDFPos++;
                            facilityStats[diseaseIndex][testIndex][count].countPwdMDFPos++;
                        }
                    }
                }
                else {
                    // will yield False Negative and False Positive
                    if (pat->getInfected()) {
                        total.countPwdFNeg++;
                        diseaseTotal[diseaseIndex].countPwdFNeg++;
                        facilityStats[diseaseIndex][testIndex][count].countPwdFNeg++;

                        // MD sensitivitiy and specificity on applies to True Negative and False Negative
                        if (pat->getMDPos()) {
                            total.countPwdMDTPos++;
                            diseaseTotal[diseaseIndex].countPwdMDTPos++;
                            facilityStats[diseaseIndex][testIndex][count].countPwdMDTPos++;
                        }
                        else {
                            total.countPwdMDFNeg++;
                            diseaseTotal[diseaseIndex].countPwdMDFNeg++;
                            facilityStats[diseaseIndex][testIndex][count].countPwdMDFNeg++;
                        }
                    }
                    else {
                        total.countPwdFPos++;
                        diseaseTotal[diseaseIndex].countPwdFPos++;
                        facilityStats[diseaseIndex][testIndex][count].countPwdFPos++;
                    }
                }
                break;
            case 5:                     // 4 - no sample results returned, no treatment
                total.countPUT++;
                diseaseTotal[diseaseIndex].countPUT++;
                facilityStats[diseaseIndex][testIndex][count].countPUT++;
                if (pat->getTestPos()) {
                    // will yield True Positve and True Negative
                    if (pat->getInfected()) {
                        total.countPwdTPos++;
                        diseaseTotal[diseaseIndex].countPwdTPos++;
                        facilityStats[diseaseIndex][testIndex][count].countPwdTPos++;

                        // add these to MD true-postive count as well
                        total.countPwdMDTPos++;
                        diseaseTotal[diseaseIndex].countPwdMDTPos++;
                        facilityStats[diseaseIndex][testIndex][count].countPwdMDTPos++;
                    }
                    else {
                        total.countPwdTNeg++;
                        diseaseTotal[diseaseIndex].countPwdTNeg++;
                        facilityStats[diseaseIndex][testIndex][count].countPwdTNeg++;

                        // MD sensitivitiy and specificity on applies to True Negative and False Negative
                        if (pat->getMDPos()) {
                            total.countPwdMDTNeg++;
                            diseaseTotal[diseaseIndex].countPwdMDTNeg++;
                            facilityStats[diseaseIndex][testIndex][count].countPwdMDTNeg++;
                        }
                        else {
                            total.countPwdMDFPos++;
                            diseaseTotal[diseaseIndex].countPwdMDFPos++;
                            facilityStats[diseaseIndex][testIndex][count].countPwdMDFPos++;
                        }
                    }
                }
                else {
                    // will yield False Negative and False Positive
                    if (pat->getInfected()) {
                        total.countPwdFNeg++;
                        diseaseTotal[diseaseIndex].countPwdFNeg++;
                        facilityStats[diseaseIndex][testIndex][count].countPwdFNeg++;

                        // MD sensitivitiy and specificity on applies to True Negative and False Negative
                        if (pat->getMDPos()) {
                            total.countPwdMDTPos++;
                            diseaseTotal[diseaseIndex].countPwdMDTPos++;
                            facilityStats[diseaseIndex][testIndex][count].countPwdMDTPos++;
                        }
                        else {
                            total.countPwdMDFNeg++;
                            diseaseTotal[diseaseIndex].countPwdMDFNeg++;
                            facilityStats[diseaseIndex][testIndex][count].countPwdMDFNeg++;
                        }
                    }
                    else {
                        total.countPwdFPos++;
                        diseaseTotal[diseaseIndex].countPwdFPos++;
                        facilityStats[diseaseIndex][testIndex][count].countPwdFPos++;
                    }
                }
                break;
            case 6:                     // 4 - sample results returned, no treatment - loss to follow up
                total.countPUT++;
                diseaseTotal[diseaseIndex].countPUT++;
                facilityStats[diseaseIndex][testIndex][count].countPUT++;
                if (pat->getTestPos()) {
                    // will yield True Positve and True Negative
                    if (pat->getInfected()) {
                        total.countPwdTPos++;
                        diseaseTotal[diseaseIndex].countPwdTPos++;
                        facilityStats[diseaseIndex][testIndex][count].countPwdTPos++;

                        // add these to MD true-postive count as well
                        total.countPwdMDTPos++;
                        diseaseTotal[diseaseIndex].countPwdMDTPos++;
                        facilityStats[diseaseIndex][testIndex][count].countPwdMDTPos++;
                    }
                    else {
                        total.countPwdTNeg++;
                        diseaseTotal[diseaseIndex].countPwdTNeg++;
                        facilityStats[diseaseIndex][testIndex][count].countPwdTNeg++;

                        // MD sensitivitiy and specificity on applies to True Negative and False Negative
                        if (pat->getMDPos()) {
                            total.countPwdMDTNeg++;
                            diseaseTotal[diseaseIndex].countPwdMDTNeg++;
                            facilityStats[diseaseIndex][testIndex][count].countPwdMDTNeg++;
                        }
                        else {
                            total.countPwdMDFPos++;
                            diseaseTotal[diseaseIndex].countPwdMDFPos++;
                            facilityStats[diseaseIndex][testIndex][count].countPwdMDFPos++;
                        }
                    }
                }
                else {
                    // will yield False Negative and False Positive
                    if (pat->getInfected()) {
                        total.countPwdFNeg++;
                        diseaseTotal[diseaseIndex].countPwdFNeg++;
                        facilityStats[diseaseIndex][testIndex][count].countPwdFNeg++;

                        // MD sensitivitiy and specificity on applies to True Negative and False Negative
                        if (pat->getMDPos()) {
                            total.countPwdMDTPos++;
                            diseaseTotal[diseaseIndex].countPwdMDTPos++;
                            facilityStats[diseaseIndex][testIndex][count].countPwdMDTPos++;
                        }
                        else {
                            total.countPwdMDFNeg++;
                            diseaseTotal[diseaseIndex].countPwdMDFNeg++;
                            facilityStats[diseaseIndex][testIndex][count].countPwdMDFNeg++;
                        }
                    }
                    else {
                        total.countPwdFPos++;
                        diseaseTotal[diseaseIndex].countPwdFPos++;
                        facilityStats[diseaseIndex][testIndex][count].countPwdFPos++;
                    }
                }
                break;
            case 7:                     // 5 - patient dead
                if (pat->getInfected()) {
                    total.countPDI++;
                    diseaseTotal[diseaseIndex].countPDI++;
                    facilityStats[diseaseIndex][testIndex][count].countPDI++;
                    if (pat->getTestPos()) {
                        // will yield True Positve and True Negative
                        if (pat->getInfected()) {
                            total.countPwdTPos++;
                            diseaseTotal[diseaseIndex].countPwdTPos++;
                            facilityStats[diseaseIndex][testIndex][count].countPwdTPos++;

                            // add these to MD true-postive count as well
                            total.countPwdMDTPos++;
                            diseaseTotal[diseaseIndex].countPwdMDTPos++;
                            facilityStats[diseaseIndex][testIndex][count].countPwdMDTPos++;
                        }
                        else {
                            total.countPwdTNeg++;
                            diseaseTotal[diseaseIndex].countPwdTNeg++;
                            facilityStats[diseaseIndex][testIndex][count].countPwdTNeg++;

                            // MD sensitivitiy and specificity on applies to True Negative and False Negative
                            if (pat->getMDPos()) {
                                total.countPwdMDTNeg++;
                                diseaseTotal[diseaseIndex].countPwdMDTNeg++;
                                facilityStats[diseaseIndex][testIndex][count].countPwdMDTNeg++;
                            }
                            else {
                                total.countPwdMDFPos++;
                                diseaseTotal[diseaseIndex].countPwdMDFPos++;
                                facilityStats[diseaseIndex][testIndex][count].countPwdMDFPos++;
                            }
                        }
                    }
                    else {
                        // will yield False Negative and False Positive
                        if (pat->getInfected()) {
                            total.countPwdFNeg++;
                            diseaseTotal[diseaseIndex].countPwdFNeg++;
                            facilityStats[diseaseIndex][testIndex][count].countPwdFNeg++;

                            // MD sensitivitiy and specificity on applies to True Negative and False Negative
                            if (pat->getMDPos()) {
                                total.countPwdMDTPos++;
                                diseaseTotal[diseaseIndex].countPwdMDTPos++;
                                facilityStats[diseaseIndex][testIndex][count].countPwdMDTPos++;
                            }
                            else {
                                total.countPwdMDFNeg++;
                                diseaseTotal[diseaseIndex].countPwdMDFNeg++;
                                facilityStats[diseaseIndex][testIndex][count].countPwdMDFNeg++;
                            }
                        }
                        else {
                            total.countPwdFPos++;
                            diseaseTotal[diseaseIndex].countPwdFPos++;
                            facilityStats[diseaseIndex][testIndex][count].countPwdFPos++;
                        }
                    }
                    // store time in treatment
                    timeSteps = pat->getTimeStepsInTreatment() + 14;
                    total.infectionDays = total.infectionDays + timeSteps;
                    diseaseTotal[diseaseIndex].infectionDays = diseaseTotal[diseaseIndex].infectionDays + timeSteps;
                    facilityStats[diseaseIndex][testIndex][count].infectionDays = facilityStats[diseaseIndex][testIndex][count].infectionDays + timeSteps;
                    total.noPatientsInfectionDays++;
                    diseaseTotal[diseaseIndex].noPatientsInfectionDays++;
                    facilityStats[diseaseIndex][testIndex][count].noPatientsInfectionDays++;
                    // time from sending sample out to getting drugs
                    timeSteps = pat->getResultsReturnedTime() - pat->getCreateTime();
                    if (timeSteps > 1) {
                        // patient never received results - don't count them
                        total.timeToDrugs = total.timeToDrugs + timeSteps;
                        diseaseTotal[diseaseIndex].timeToDrugs = diseaseTotal[diseaseIndex].timeToDrugs + timeSteps;
                        facilityStats[diseaseIndex][testIndex][count].timeToDrugs = facilityStats[diseaseIndex][testIndex][count].timeToDrugs + timeSteps;
                        total.noPatientsGettingDrugs++;
                        diseaseTotal[diseaseIndex].noPatientsGettingDrugs++;
                        facilityStats[diseaseIndex][testIndex][count].noPatientsGettingDrugs++;
                    }
                }
                else {
                    total.countPDU++;
                    diseaseTotal[diseaseIndex].countPDU++;
                    facilityStats[diseaseIndex][testIndex][count].countPDU++;
                }
                break;
            case 8:                         // 6 - patient cured
                total.countPC++;
                diseaseTotal[diseaseIndex].countPC++;
                facilityStats[diseaseIndex][testIndex][count].countPC++;

                if (pat->getTestPos()) {
                    // will yield True Positve and True Negative
                    if (pat->getInfected()) {
                        total.countPwdTPos++;
                        diseaseTotal[diseaseIndex].countPwdTPos++;
                        facilityStats[diseaseIndex][testIndex][count].countPwdTPos++;

                        // add these to MD true-postive count as well
                        total.countPwdMDTPos++;
                        diseaseTotal[diseaseIndex].countPwdMDTPos++;
                        facilityStats[diseaseIndex][testIndex][count].countPwdMDTPos++;
                    }
                    else {
                        total.countPwdTNeg++;
                        diseaseTotal[diseaseIndex].countPwdTNeg++;
                        facilityStats[diseaseIndex][testIndex][count].countPwdTNeg++;

                        // MD sensitivitiy and specificity on applies to True Negative and False Negative
                        if (pat->getMDPos()) {
                            total.countPwdMDTNeg++;
                            diseaseTotal[diseaseIndex].countPwdMDTNeg++;
                            facilityStats[diseaseIndex][testIndex][count].countPwdMDTNeg++;
                        }
                        else {
                            total.countPwdMDFPos++;
                            diseaseTotal[diseaseIndex].countPwdMDFPos++;
                            facilityStats[diseaseIndex][testIndex][count].countPwdMDFPos++;
                        }
                    }
                }
                else {
                    // will yield False Negative and False Positive
                    if (pat->getInfected()) {
                        total.countPwdFNeg++;
                        diseaseTotal[diseaseIndex].countPwdFNeg++;
                        facilityStats[diseaseIndex][testIndex][count].countPwdFNeg++;

                        // MD sensitivitiy and specificity on applies to True Negative and False Negative
                        if (pat->getMDPos()) {
                            total.countPwdMDTPos++;
                            diseaseTotal[diseaseIndex].countPwdMDTPos++;
                            facilityStats[diseaseIndex][testIndex][count].countPwdMDTPos++;
                        }
                        else {
                            total.countPwdMDFNeg++;
                            diseaseTotal[diseaseIndex].countPwdMDFNeg++;
                            facilityStats[diseaseIndex][testIndex][count].countPwdMDFNeg++;
                        }
                    }
                    else {
                        total.countPwdFPos++;
                        diseaseTotal[diseaseIndex].countPwdFPos++;
                        facilityStats[diseaseIndex][testIndex][count].countPwdFPos++;
                    }
                }

                // time from sending sample out to getting drugs
                timeSteps = pat->getResultsReturnedTime() - pat->getCreateTime();
                if (timeSteps > 1) {
                    // patient never received results - don't count them
                    total.timeToDrugs = total.timeToDrugs + timeSteps;
                    diseaseTotal[diseaseIndex].timeToDrugs = diseaseTotal[diseaseIndex].timeToDrugs + timeSteps;
                    facilityStats[diseaseIndex][testIndex][count].timeToDrugs = facilityStats[diseaseIndex][testIndex][count].timeToDrugs + timeSteps;
                    total.noPatientsGettingDrugs++;
                    diseaseTotal[diseaseIndex].noPatientsGettingDrugs++;
                    facilityStats[diseaseIndex][testIndex][count].noPatientsGettingDrugs++;
                }
                break;
            case 9:                    // 7 - patient uninfected - returned sample 
                total.countPUI++;
                diseaseTotal[diseaseIndex].countPUI++;
                facilityStats[diseaseIndex][testIndex][count].countPUI++;
                break;
            case 10:                    // 8 - patient uninfected - didn't return for sampl 
                total.countPUINR++;
                diseaseTotal[diseaseIndex].countPUINR++;
                facilityStats[diseaseIndex][testIndex][count].countPUINR++;
                break;
            case 11:                      // 9 - patient uninfected - under treatment
                total.countPUIUT++;
                diseaseTotal[diseaseIndex].countPUIUT++;
                facilityStats[diseaseIndex][testIndex][count].countPUIUT++;
                break;
            default:
                total.countPO++;
                diseaseTotal[diseaseIndex].countPO++;
                facilityStats[diseaseIndex][testIndex][count].countPO++;

            }

            // track all True Positive, False Negative, True Negative and False Positive values
            if (pat->getTestPos()) {
                // will yield True Positve and True Negative
                if (pat->getInfected()) {
                    if (testIndex < 2) {
                        // don't add in lost to follow up or sample results no read yet
                        total.countPTPos++;
                        diseaseTotal[diseaseIndex].countPTPos++;
                    }
                    facilityStats[diseaseIndex][testIndex][count].countPTPos++;

                    // add these to the MD True-positive count as well
                    if (testIndex < 2) {
                            // don't add in lost to follow up or sample results no read yet
                            total.countPMDTPos++;
                            diseaseTotal[diseaseIndex].countPMDTPos++;
                        }
                        facilityStats[diseaseIndex][testIndex][count].countPMDTPos++;
                }
                else {
                    if (testIndex < 2) {
                        // don't add in lost to follow up or sample results no read yet
                        total.countPTNeg++;
                        diseaseTotal[diseaseIndex].countPTNeg++;
                    }
                    facilityStats[diseaseIndex][testIndex][count].countPTNeg++;

                    // MD sensitivitiy and specificity on applies to True Negative and False Negative
                    if (pat->getMDPos()) {
                        if (testIndex < 2) {
                            // don't add in lost to follow up or sample results no read yet
                            total.countPMDTNeg++;
                            diseaseTotal[diseaseIndex].countPMDTNeg++;
                        }
                        facilityStats[diseaseIndex][testIndex][count].countPMDTNeg++;
                    }
                    else {
                        if (testIndex < 2) {
                            // don't add in lost to follow up or sample results no read yet
                            total.countPMDFPos++;
                            diseaseTotal[diseaseIndex].countPMDFPos++;
                        }
                        facilityStats[diseaseIndex][testIndex][count].countPMDFPos++;
                    }
                }
            }
            else {
                // will yield False Negative and False Positive
                if (pat->getInfected()) {
                    if (testIndex < 2) {
                        // don't add in lost to follow up or sample results no read yet
                        total.countPFNeg++;
                        diseaseTotal[diseaseIndex].countPFNeg++;
                    }
                    facilityStats[diseaseIndex][testIndex][count].countPFNeg++;

                    // MD sensitivitiy and specificity on applies to True Negative and False Negative
                    if (pat->getMDPos()) {
                        if (testIndex < 2) {
                            // don't add in lost to follow up or sample results no read yet
                            total.countPMDTPos++;
                            diseaseTotal[diseaseIndex].countPMDTPos++;
                        }
                        facilityStats[diseaseIndex][testIndex][count].countPMDTPos++;
                    }
                    else {
                        if (testIndex < 2) {
                            // don't add in lost to follow up or sample results no read yet
                            total.countPMDFNeg++;
                            diseaseTotal[diseaseIndex].countPMDFNeg++;
                        }
                        facilityStats[diseaseIndex][testIndex][count].countPMDFNeg++;
                    }
                }
                else {
                    if (testIndex < 2) {
                        // don't add in lost to follow up or sample results no read yet
                        total.countPFPos++;
                        diseaseTotal[diseaseIndex].countPFPos++;
                    }
                    facilityStats[diseaseIndex][testIndex][count].countPFPos++;
                }
            }
            
            // count infected and uninfected patients for each  test type
            if (pat->getInfected()) {
                total.infected++;
                diseaseTotal[diseaseIndex].infected++;
                facilityStats[diseaseIndex][testIndex][count].infected++;
            }
            else {
                total.uninfected++;
                diseaseTotal[diseaseIndex].uninfected++;
                facilityStats[diseaseIndex][testIndex][count].uninfected++;
            }

            // dump statistics for each patient - only do it to file
            if (invdividualFlag) {
                myfile1 << status << "," << pat->getDisease() << "," << pat->getSampleTest() << "," << pat->getAge() << "," << pat->isAlive() << "," << pat->getInfected() << "," << pat->getTimeFromOnset() << "," << "," << pat->getTimeStepsInTreatment() << "," << pat->getTimeStepsNotInTreatment() << "\n";
            }

            // count dead people from old age
            // age = pat->getAge();
            //if (!pat->isAlive()) {
            //    countPD++;
            //}
        }

        // if for some reason you don't want to include the inactive patients, set this to false
        if (true) {

            //std::cout << "stats: before inactive patients " << "\n";

            // now loop through the inactive patients
            for (auto pat : pI) {

                // store stats by disease
                diseaseName = pat->getDisease();

                // get the index for the disease
                diseaseIndex = 0;

                for (int w = 0; w < diseaseCount; w++) {

                    if (diseaseName == diseaseList[w]) {
                        diseaseIndex = w;

                        // increase the sample count for this disease for this facility
                        patientCount[w]++;
                    }
                }

                // store stats as a subcategory of test
                testName = pat->getSampleTest();

                // get the index for the disease
                testIndex = 0;

                if (testName == "LtoF") {

                    testIndex = 2;
                }
                else if (testName == "") {

                    testIndex = 3;
                }
                else if (testName == "none") {

                    testIndex = 3;
                }
                else
                {
                    // make first test encountered index 0
                    if (firstTestName == "") {

                        // get the each test name associated with this disease
                        for (int testIndex = 0; testIndex < testCount; testIndex++) {

                            if (diseaseName == testList[testIndex]->getDiseaseName()) {

                                if (firstTestName == "") {
                                    firstTestName = testList[testIndex]->getTestName();
                                }
                                else {
                                    secondTestName = testList[testIndex]->getTestName();
                                }

                            }
                        }
                    }

                    // anything that matches firstTestName is index 0, everything else is index 1
                    if (testName == firstTestName) {

                        testIndex = 0;
                    }
                    else {

                        testIndex = 1;
                    }
                }
                
                // 0 - away from facility - hasn't returned for results
                // 1 - in-house
                // 2 - sample results returned, undergoing treatment
                // 3 - sample results returned, treatment failure
                // 4 - sample results returned, no treatment
                // 5 - no sample results returned, no treatment
                // 6 - sample results returned, no treatment - loss to follow up
                // 7 - patient dead
                // 8 - patient cured
                // 9 - patient uninfected - returned sample
                // 10 - patient uninfected - didn't return for sample
                // 11 - patient uninfected - under treatment
                status = pat->getStatus();
                //std::cout << "stats:patient test Name  =  " << testName << ", status = " << status << "\n";

                switch (status) {
                case 0:                     // 0 - away from facility - hasn't returned for results
                    total.countPU++;
                    diseaseTotal[diseaseIndex].countPU++;
                    facilityStats[diseaseIndex][testIndex][count].countPU++;
                    break;
                case 1:                     // 1 - in-house
                    total.countPI++;
                    diseaseTotal[diseaseIndex].countPI++;
                    facilityStats[diseaseIndex][testIndex][count].countPI++;
                    break;
                case 2:                     // 2 - sample results returned, undergoing treatment
                    total.countPT++;
                    diseaseTotal[diseaseIndex].countPT++;
                    facilityStats[diseaseIndex][testIndex][count].countPT++;
                    if (pat->getTestPos()) {
                        // will yield True Positve and True Negative
                        if (pat->getInfected()) {
                            total.countPwdTPos++;
                            diseaseTotal[diseaseIndex].countPwdTPos++;
                            facilityStats[diseaseIndex][testIndex][count].countPwdTPos++;

                            // add these to MD true-postive count as well
                            total.countPwdMDTPos++;
                            diseaseTotal[diseaseIndex].countPwdMDTPos++;
                            facilityStats[diseaseIndex][testIndex][count].countPwdMDTPos++;
                        }
                        else {
                            total.countPwdTNeg++;
                            diseaseTotal[diseaseIndex].countPwdTNeg++;
                            facilityStats[diseaseIndex][testIndex][count].countPwdTNeg++;

                            // MD sensitivitiy and specificity on applies to True Negative and False Negative
                            if (pat->getMDPos()) {
                                total.countPwdMDTNeg++;
                                diseaseTotal[diseaseIndex].countPwdMDTNeg++;
                                facilityStats[diseaseIndex][testIndex][count].countPwdMDTNeg++;
                            }
                            else {
                                total.countPwdMDFPos++;
                                diseaseTotal[diseaseIndex].countPwdMDFPos++;
                                facilityStats[diseaseIndex][testIndex][count].countPwdMDFPos++;
                            }
                        }
                    }
                    else {
                        // will yield False Negative and False Positive
                        if (pat->getInfected()) {
                            total.countPwdFNeg++;
                            diseaseTotal[diseaseIndex].countPwdFNeg++;
                            facilityStats[diseaseIndex][testIndex][count].countPwdFNeg++;

                            // MD sensitivitiy and specificity on applies to True Negative and False Negative
                            if (pat->getMDPos()) {
                                total.countPwdMDTPos++;
                                diseaseTotal[diseaseIndex].countPwdMDTPos++;
                                facilityStats[diseaseIndex][testIndex][count].countPwdMDTPos++;
                            }
                            else {
                                total.countPwdMDFNeg++;
                                diseaseTotal[diseaseIndex].countPwdMDFNeg++;
                                facilityStats[diseaseIndex][testIndex][count].countPwdMDFNeg++;
                            }
                        }
                        else {
                            total.countPwdFPos++;
                            diseaseTotal[diseaseIndex].countPwdFPos++;
                            facilityStats[diseaseIndex][testIndex][count].countPwdFPos++;
                        }
                    }

                    // store time in treatment
                    timeSteps = pat->getTimeStepsInTreatment() + 14;
                    total.infectionDays = total.infectionDays + timeSteps;
                    diseaseTotal[diseaseIndex].infectionDays = diseaseTotal[diseaseIndex].infectionDays + timeSteps;
                    facilityStats[diseaseIndex][testIndex][count].infectionDays = facilityStats[diseaseIndex][testIndex][count].infectionDays + timeSteps;
                    total.noPatientsInfectionDays++;
                    diseaseTotal[diseaseIndex].noPatientsInfectionDays++;
                    facilityStats[diseaseIndex][testIndex][count].noPatientsInfectionDays++;
                    // time from sending sample out to getting drugs
                    timeSteps = pat->getResultsReturnedTime() - pat->getCreateTime();
                    if (timeSteps > 1) {
                        // patient never received results - don't count them
                        total.timeToDrugs = total.timeToDrugs + timeSteps;
                        diseaseTotal[diseaseIndex].timeToDrugs = diseaseTotal[diseaseIndex].timeToDrugs + timeSteps;
                        facilityStats[diseaseIndex][testIndex][count].timeToDrugs = facilityStats[diseaseIndex][testIndex][count].timeToDrugs + timeSteps;
                        total.noPatientsGettingDrugs++;
                        diseaseTotal[diseaseIndex].noPatientsGettingDrugs++;
                        facilityStats[diseaseIndex][testIndex][count].noPatientsGettingDrugs++;
                    }
                case 3:                     // 3 - sample results returned, treatment failure
                    total.countPR++;
                    diseaseTotal[diseaseIndex].countPR++;
                    facilityStats[diseaseIndex][testIndex][count].countPR++;
                    if (pat->getTestPos()) {
                        // will yield True Positve and True Negative
                        if (pat->getInfected()) {
                            total.countPwdTPos++;
                            diseaseTotal[diseaseIndex].countPwdTPos++;
                            facilityStats[diseaseIndex][testIndex][count].countPwdTPos++;

                            // add these to MD true-postive count as well
                            total.countPwdMDTPos++;
                            diseaseTotal[diseaseIndex].countPwdMDTPos++;
                            facilityStats[diseaseIndex][testIndex][count].countPwdMDTPos++;
                        }
                        else {
                            total.countPwdTNeg++;
                            diseaseTotal[diseaseIndex].countPwdTNeg++;
                            facilityStats[diseaseIndex][testIndex][count].countPwdTNeg++;

                            // MD sensitivitiy and specificity on applies to True Negative and False Negative
                            if (pat->getMDPos()) {
                                total.countPwdMDTNeg++;
                                diseaseTotal[diseaseIndex].countPwdMDTNeg++;
                                facilityStats[diseaseIndex][testIndex][count].countPwdMDTNeg++;
                            }
                            else {
                                total.countPwdMDFPos++;
                                diseaseTotal[diseaseIndex].countPwdMDFPos++;
                                facilityStats[diseaseIndex][testIndex][count].countPwdMDFPos++;
                            }
                        }
                    }
                    else {
                        // will yield False Negative and False Positive
                        if (pat->getInfected()) {
                            total.countPwdFNeg++;
                            diseaseTotal[diseaseIndex].countPwdFNeg++;
                            facilityStats[diseaseIndex][testIndex][count].countPwdFNeg++;

                            // MD sensitivitiy and specificity on applies to True Negative and False Negative
                            if (pat->getMDPos()) {
                                total.countPwdMDTPos++;
                                diseaseTotal[diseaseIndex].countPwdMDTPos++;
                                facilityStats[diseaseIndex][testIndex][count].countPwdMDTPos++;
                            }
                            else {
                                total.countPwdMDFNeg++;
                                diseaseTotal[diseaseIndex].countPwdMDFNeg++;
                                facilityStats[diseaseIndex][testIndex][count].countPwdMDFNeg++;
                            }
                        }
                        else {
                            total.countPwdFPos++;
                            diseaseTotal[diseaseIndex].countPwdFPos++;
                            facilityStats[diseaseIndex][testIndex][count].countPwdFPos++;
                        }
                    }

                    // store time in treatment
                    timeSteps = pat->getTimeStepsInTreatment() + 14;
                    total.infectionDays = total.infectionDays + timeSteps;
                    diseaseTotal[diseaseIndex].infectionDays = diseaseTotal[diseaseIndex].infectionDays + timeSteps;
                    facilityStats[diseaseIndex][testIndex][count].infectionDays = facilityStats[diseaseIndex][testIndex][count].infectionDays + timeSteps;
                    total.noPatientsInfectionDays++;
                    diseaseTotal[diseaseIndex].noPatientsInfectionDays++;
                    facilityStats[diseaseIndex][testIndex][count].noPatientsInfectionDays++;
                    // time from sending sample out to getting drugs
                    timeSteps = pat->getResultsReturnedTime() - pat->getCreateTime();
                    if (timeSteps > 1) {
                        // patient never received results - don't count them
                        total.timeToDrugs = total.timeToDrugs + timeSteps;
                        diseaseTotal[diseaseIndex].timeToDrugs = diseaseTotal[diseaseIndex].timeToDrugs + timeSteps;
                        facilityStats[diseaseIndex][testIndex][count].timeToDrugs = facilityStats[diseaseIndex][testIndex][count].timeToDrugs + timeSteps;
                        total.noPatientsGettingDrugs++;
                        diseaseTotal[diseaseIndex].noPatientsGettingDrugs++;
                        facilityStats[diseaseIndex][testIndex][count].noPatientsGettingDrugs++;
                    }
                    break;
                case 4:                     // 4 - sample results returned, no treatment
                    total.countPUT++;
                    diseaseTotal[diseaseIndex].countPUT++;
                    facilityStats[diseaseIndex][testIndex][count].countPUT++;
                    if (pat->getTestPos()) {
                        // will yield True Positve and True Negative
                        if (pat->getInfected()) {
                            total.countPwdTPos++;
                            diseaseTotal[diseaseIndex].countPwdTPos++;
                            facilityStats[diseaseIndex][testIndex][count].countPwdTPos++;

                            // add these to MD true-postive count as well
                            total.countPwdMDTPos++;
                            diseaseTotal[diseaseIndex].countPwdMDTPos++;
                            facilityStats[diseaseIndex][testIndex][count].countPwdMDTPos++;
                        }
                        else {
                            total.countPwdTNeg++;
                            diseaseTotal[diseaseIndex].countPwdTNeg++;
                            facilityStats[diseaseIndex][testIndex][count].countPwdTNeg++;

                            // MD sensitivitiy and specificity on applies to True Negative and False Negative
                            if (pat->getMDPos()) {
                                total.countPwdMDTNeg++;
                                diseaseTotal[diseaseIndex].countPwdMDTNeg++;
                                facilityStats[diseaseIndex][testIndex][count].countPwdMDTNeg++;
                            }
                            else {
                                total.countPwdMDFPos++;
                                diseaseTotal[diseaseIndex].countPwdMDFPos++;
                                facilityStats[diseaseIndex][testIndex][count].countPwdMDFPos++;
                            }
                        }
                    }
                    else {
                        // will yield False Negative and False Positive
                        if (pat->getInfected()) {
                            total.countPwdFNeg++;
                            diseaseTotal[diseaseIndex].countPwdFNeg++;
                            facilityStats[diseaseIndex][testIndex][count].countPwdFNeg++;

                            // MD sensitivitiy and specificity on applies to True Negative and False Negative
                            if (pat->getMDPos()) {
                                total.countPwdMDTPos++;
                                diseaseTotal[diseaseIndex].countPwdMDTPos++;
                                facilityStats[diseaseIndex][testIndex][count].countPwdMDTPos++;
                            }
                            else {
                                total.countPwdMDFNeg++;
                                diseaseTotal[diseaseIndex].countPwdMDFNeg++;
                                facilityStats[diseaseIndex][testIndex][count].countPwdMDFNeg++;
                            }
                        }
                        else {
                            total.countPwdFPos++;
                            diseaseTotal[diseaseIndex].countPwdFPos++;
                            facilityStats[diseaseIndex][testIndex][count].countPwdFPos++;
                        }
                    }

                    break;
                case 5:                     // 5 - snoample results returned, no treatment
                    total.countPUT++;
                    diseaseTotal[diseaseIndex].countPUT++;
                    facilityStats[diseaseIndex][testIndex][count].countPUT++;
                    if (pat->getTestPos()) {
                        // will yield True Positve and True Negative
                        if (pat->getInfected()) {
                            total.countPwdTPos++;
                            diseaseTotal[diseaseIndex].countPwdTPos++;
                            facilityStats[diseaseIndex][testIndex][count].countPwdTPos++;

                            // add these to MD true-postive count as well
                            total.countPwdMDTPos++;
                            diseaseTotal[diseaseIndex].countPwdMDTPos++;
                            facilityStats[diseaseIndex][testIndex][count].countPwdMDTPos++;
                        }
                        else {
                            total.countPwdTNeg++;
                            diseaseTotal[diseaseIndex].countPwdTNeg++;
                            facilityStats[diseaseIndex][testIndex][count].countPwdTNeg++;

                            // MD sensitivitiy and specificity on applies to True Negative and False Negative
                            if (pat->getMDPos()) {
                                total.countPwdMDTNeg++;
                                diseaseTotal[diseaseIndex].countPwdMDTNeg++;
                                facilityStats[diseaseIndex][testIndex][count].countPwdMDTNeg++;
                            }
                            else {
                                total.countPwdMDFPos++;
                                diseaseTotal[diseaseIndex].countPwdMDFPos++;
                                facilityStats[diseaseIndex][testIndex][count].countPwdMDFPos++;
                            }
                        }
                    }
                    else {
                        // will yield False Negative and False Positive
                        if (pat->getInfected()) {
                            total.countPwdFNeg++;
                            diseaseTotal[diseaseIndex].countPwdFNeg++;
                            facilityStats[diseaseIndex][testIndex][count].countPwdFNeg++;

                            // MD sensitivitiy and specificity on applies to True Negative and False Negative
                            if (pat->getMDPos()) {
                                total.countPwdMDTPos++;
                                diseaseTotal[diseaseIndex].countPwdMDTPos++;
                                facilityStats[diseaseIndex][testIndex][count].countPwdMDTPos++;
                            }
                            else {
                                total.countPwdMDFNeg++;
                                diseaseTotal[diseaseIndex].countPwdMDFNeg++;
                                facilityStats[diseaseIndex][testIndex][count].countPwdMDFNeg++;
                            }
                        }
                        else {
                            total.countPwdFPos++;
                            diseaseTotal[diseaseIndex].countPwdFPos++;
                            facilityStats[diseaseIndex][testIndex][count].countPwdFPos++;
                        }
                    }

                    break;
                case 6:                     // 4 - sample results returned, no treatment - loss to follow up
                    total.countPUT++;
                    diseaseTotal[diseaseIndex].countPUT++;
                    facilityStats[diseaseIndex][testIndex][count].countPUT++;
                    if (pat->getTestPos()) {
                        // will yield True Positve and True Negative
                        if (pat->getInfected()) {
                            total.countPwdTPos++;
                            diseaseTotal[diseaseIndex].countPwdTPos++;
                            facilityStats[diseaseIndex][testIndex][count].countPwdTPos++;

                            // add these to MD true-postive count as well
                            total.countPwdMDTPos++;
                            diseaseTotal[diseaseIndex].countPwdMDTPos++;
                            facilityStats[diseaseIndex][testIndex][count].countPwdMDTPos++;
                        }
                        else {
                            total.countPwdTNeg++;
                            diseaseTotal[diseaseIndex].countPwdTNeg++;
                            facilityStats[diseaseIndex][testIndex][count].countPwdTNeg++;

                            // MD sensitivitiy and specificity on applies to True Negative and False Negative
                            if (pat->getMDPos()) {
                                total.countPwdMDTNeg++;
                                diseaseTotal[diseaseIndex].countPwdMDTNeg++;
                                facilityStats[diseaseIndex][testIndex][count].countPwdMDTNeg++;
                            }
                            else {
                                total.countPwdMDFPos++;
                                diseaseTotal[diseaseIndex].countPwdMDFPos++;
                                facilityStats[diseaseIndex][testIndex][count].countPwdMDFPos++;
                            }
                        }
                    }
                    else {
                        // will yield False Negative and False Positive
                        if (pat->getInfected()) {
                            total.countPwdFNeg++;
                            diseaseTotal[diseaseIndex].countPwdFNeg++;
                            facilityStats[diseaseIndex][testIndex][count].countPwdFNeg++;

                            // MD sensitivitiy and specificity on applies to True Negative and False Negative
                            if (pat->getMDPos()) {
                                total.countPwdMDTPos++;
                                diseaseTotal[diseaseIndex].countPwdMDTPos++;
                                facilityStats[diseaseIndex][testIndex][count].countPwdMDTPos++;
                            }
                            else {
                                total.countPwdMDFNeg++;
                                diseaseTotal[diseaseIndex].countPwdMDFNeg++;
                                facilityStats[diseaseIndex][testIndex][count].countPwdMDFNeg++;
                            }
                        }
                        else {
                            total.countPwdFPos++;
                            diseaseTotal[diseaseIndex].countPwdFPos++;
                            facilityStats[diseaseIndex][testIndex][count].countPwdFPos++;
                        }
                    }

                    break;
                case 7:                     // patient dead
                    if (pat->getInfected()) {
                        total.countPDI++;
                        diseaseTotal[diseaseIndex].countPDI++;
                        facilityStats[diseaseIndex][testIndex][count].countPDI++;
                        if (pat->getTestPos()) {
                            // will yield True Positve and True Negative
                            if (pat->getInfected()) {
                                total.countPwdTPos++;
                                diseaseTotal[diseaseIndex].countPwdTPos++;
                                facilityStats[diseaseIndex][testIndex][count].countPwdTPos++;

                                // add these to MD true-postive count as well
                                total.countPwdMDTPos++;
                                diseaseTotal[diseaseIndex].countPwdMDTPos++;
                                facilityStats[diseaseIndex][testIndex][count].countPwdMDTPos++;
                            }
                            else {
                                total.countPwdTNeg++;
                                diseaseTotal[diseaseIndex].countPwdTNeg++;
                                facilityStats[diseaseIndex][testIndex][count].countPwdTNeg++;

                                // MD sensitivitiy and specificity on applies to True Negative and False Negative
                                if (pat->getMDPos()) {
                                    total.countPwdMDTNeg++;
                                    diseaseTotal[diseaseIndex].countPwdMDTNeg++;
                                    facilityStats[diseaseIndex][testIndex][count].countPwdMDTNeg++;
                                }
                                else {
                                    total.countPwdMDFPos++;
                                    diseaseTotal[diseaseIndex].countPwdMDFPos++;
                                    facilityStats[diseaseIndex][testIndex][count].countPwdMDFPos++;
                                }
                            }
                        }
                        else {
                            // will yield False Negative and False Positive
                            if (pat->getInfected()) {
                                total.countPwdFNeg++;
                                diseaseTotal[diseaseIndex].countPwdFNeg++;
                                facilityStats[diseaseIndex][testIndex][count].countPwdFNeg++;

                                // MD sensitivitiy and specificity on applies to True Negative and False Negative
                                if (pat->getMDPos()) {
                                    total.countPwdMDTPos++;
                                    diseaseTotal[diseaseIndex].countPwdMDTPos++;
                                    facilityStats[diseaseIndex][testIndex][count].countPwdMDTPos++;
                                }
                                else {
                                    total.countPwdMDFNeg++;
                                    diseaseTotal[diseaseIndex].countPwdMDFNeg++;
                                    facilityStats[diseaseIndex][testIndex][count].countPwdMDFNeg++;
                                }
                            }
                            else {
                                total.countPwdFPos++;
                                diseaseTotal[diseaseIndex].countPwdFPos++;
                                facilityStats[diseaseIndex][testIndex][count].countPwdFPos++;
                            }
                        }

                        // store time in treatment
                        timeSteps = pat->getTimeStepsInTreatment() + 14;
                        total.infectionDays = total.infectionDays + timeSteps;
                        diseaseTotal[diseaseIndex].infectionDays = diseaseTotal[diseaseIndex].infectionDays + timeSteps;
                        facilityStats[diseaseIndex][testIndex][count].infectionDays = facilityStats[diseaseIndex][testIndex][count].infectionDays + timeSteps;
                        total.noPatientsInfectionDays++;
                        diseaseTotal[diseaseIndex].noPatientsInfectionDays++;
                        facilityStats[diseaseIndex][testIndex][count].noPatientsInfectionDays++;
                        // time from sending sample out to getting drugs
                        timeSteps = pat->getResultsReturnedTime() - pat->getCreateTime();
                        if (timeSteps > 1) {
                            // patient never received results - don't count them
                            total.timeToDrugs = total.timeToDrugs + timeSteps;
                            diseaseTotal[diseaseIndex].timeToDrugs = diseaseTotal[diseaseIndex].timeToDrugs + timeSteps;
                            facilityStats[diseaseIndex][testIndex][count].timeToDrugs = facilityStats[diseaseIndex][testIndex][count].timeToDrugs + timeSteps;
                            total.noPatientsGettingDrugs++;
                            diseaseTotal[diseaseIndex].noPatientsGettingDrugs++;
                            facilityStats[diseaseIndex][testIndex][count].noPatientsGettingDrugs++;
                        }
                    }
                    else {
                        total.countPDU++;
                        diseaseTotal[diseaseIndex].countPDU++;
                        facilityStats[diseaseIndex][testIndex][count].countPDU++;
                    }
                    break;
                case 8:                     // patient cured
                    total.countPC++;
                    diseaseTotal[diseaseIndex].countPC++;
                    facilityStats[diseaseIndex][testIndex][count].countPC++;

                    if (pat->getTestPos()) {
                        // will yield True Positve and True Negative
                        if (pat->getInfected()) {
                            total.countPwdTPos++;
                            diseaseTotal[diseaseIndex].countPwdTPos++;
                            facilityStats[diseaseIndex][testIndex][count].countPwdTPos++;

                            // add these to MD true-postive count as well
                            total.countPwdMDTPos++;
                            diseaseTotal[diseaseIndex].countPwdMDTPos++;
                            facilityStats[diseaseIndex][testIndex][count].countPwdMDTPos++;
                        }
                        else {
                            total.countPwdTNeg++;
                            diseaseTotal[diseaseIndex].countPwdTNeg++;
                            facilityStats[diseaseIndex][testIndex][count].countPwdTNeg++;

                            // MD sensitivitiy and specificity on applies to True Negative and False Negative
                            if (pat->getMDPos()) {
                                total.countPwdMDTNeg++;
                                diseaseTotal[diseaseIndex].countPwdMDTNeg++;
                                facilityStats[diseaseIndex][testIndex][count].countPwdMDTNeg++;
                            }
                            else {
                                total.countPwdMDFPos++;
                                diseaseTotal[diseaseIndex].countPwdMDFPos++;
                                facilityStats[diseaseIndex][testIndex][count].countPwdMDFPos++;
                            }
                        }
                    }
                    else {
                        // will yield False Negative and False Positive
                        if (pat->getInfected()) {
                            total.countPwdFNeg++;
                            diseaseTotal[diseaseIndex].countPwdFNeg++;
                            facilityStats[diseaseIndex][testIndex][count].countPwdFNeg++;

                            // MD sensitivitiy and specificity on applies to True Negative and False Negative
                            if (pat->getMDPos()) {
                                total.countPwdMDTPos++;
                                diseaseTotal[diseaseIndex].countPwdMDTPos++;
                                facilityStats[diseaseIndex][testIndex][count].countPwdMDTPos++;
                            }
                            else {
                                total.countPwdMDFNeg++;
                                diseaseTotal[diseaseIndex].countPwdMDFNeg++;
                                facilityStats[diseaseIndex][testIndex][count].countPwdMDFNeg++;
                            }
                        }
                        else {
                            total.countPwdFPos++;
                            diseaseTotal[diseaseIndex].countPwdFPos++;
                            facilityStats[diseaseIndex][testIndex][count].countPwdFPos++;
                        }
                    }

                    // time from sending sample out to getting drugs
                    timeSteps = pat->getResultsReturnedTime() - pat->getCreateTime();
                    if (timeSteps > 1) {
                        // patient never received results - don't count them
                        total.timeToDrugs = total.timeToDrugs + timeSteps;
                        diseaseTotal[diseaseIndex].timeToDrugs = diseaseTotal[diseaseIndex].timeToDrugs + timeSteps;
                        facilityStats[diseaseIndex][testIndex][count].timeToDrugs = facilityStats[diseaseIndex][testIndex][count].timeToDrugs + timeSteps;
                        total.noPatientsGettingDrugs++;
                        diseaseTotal[diseaseIndex].noPatientsGettingDrugs++;
                        facilityStats[diseaseIndex][testIndex][count].noPatientsGettingDrugs++;
                    }
                    break;
                case 9:                     // 7 - patient uninfected - returned sample
                    total.countPUI++;
                    diseaseTotal[diseaseIndex].countPUI++;
                    facilityStats[diseaseIndex][testIndex][count].countPUI++;
                    break;
                case 10:                     // 8 - patient uninfected - didn't return for sample
                    total.countPUINR++;
                    diseaseTotal[diseaseIndex].countPUINR++;
                    facilityStats[diseaseIndex][testIndex][count].countPUINR++;
                    break;
                case 11:                     // 9 - patient uninfected - under treatment
                    total.countPUIUT++;
                    diseaseTotal[diseaseIndex].countPUIUT++;
                    facilityStats[diseaseIndex][testIndex][count].countPUIUT++;
                    break;
                default:
                    total.countPO++;
                    diseaseTotal[diseaseIndex].countPO++;
                    facilityStats[diseaseIndex][testIndex][count].countPO++;

                }
                
                // track all True Positive, False Negative, True Negative and False Positive values
                if (pat->getTestPos()) {
                    // will yield True Positve and True Negative
                    if (pat->getInfected()) {
                        if (testIndex < 2) {
                            // don't add in lost to follow up or sample results no read yet
                            total.countPTPos++;
                            diseaseTotal[diseaseIndex].countPTPos++;
                        }
                        facilityStats[diseaseIndex][testIndex][count].countPTPos++;

                        // add these to the MD true-positives as well
                        if (testIndex < 2) {
                            // don't add in lost to follow up or sample results no read yet
                            total.countPMDTPos++;
                            diseaseTotal[diseaseIndex].countPMDTPos++;
                        }
                        facilityStats[diseaseIndex][testIndex][count].countPMDTPos++;
                    }
                    else {
                        if (testIndex < 2) {
                            // don't add in lost to follow up or sample results no read yet
                            total.countPTNeg++;
                            diseaseTotal[diseaseIndex].countPTNeg++;
                        }
                        facilityStats[diseaseIndex][testIndex][count].countPTNeg++;

                        // MD sensitivitiy and specificity on applies to True Negative and False Negative
                        if (pat->getMDPos()) {
                            if (testIndex < 2) {
                                // don't add in lost to follow up or sample results no read yet
                                total.countPMDTNeg++;
                                diseaseTotal[diseaseIndex].countPMDTNeg++;
                            }
                            facilityStats[diseaseIndex][testIndex][count].countPMDTNeg++;
                        }
                        else {
                            if (testIndex < 2) {
                                // don't add in lost to follow up or sample results no read yet
                                total.countPMDFPos++;
                                diseaseTotal[diseaseIndex].countPMDFPos++;
                            }
                            facilityStats[diseaseIndex][testIndex][count].countPMDFPos++;
                        }
                    }
                }
                else {
                    // will yield False Negative and False Positive
                    if (pat->getInfected()) {
                        if (testIndex < 2) {
                            // don't add in lost to follow up or sample results no read yet
                            total.countPFNeg++;
                            diseaseTotal[diseaseIndex].countPFNeg++;
                        }
                        facilityStats[diseaseIndex][testIndex][count].countPFNeg++;

                        // MD sensitivitiy and specificity on applies to True Negative and False Negative
                        if (pat->getMDPos()) {
                            if (testIndex < 2) {
                                // don't add in lost to follow up or sample results no read yet
                                total.countPMDTPos++;
                                diseaseTotal[diseaseIndex].countPMDTPos++;
                            }
                            facilityStats[diseaseIndex][testIndex][count].countPMDTPos++;
                        }
                        else {
                            if (testIndex < 2) {
                                // don't add in lost to follow up or sample results no read yet
                                total.countPMDFNeg++;
                                diseaseTotal[diseaseIndex].countPMDFNeg++;
                            }
                            facilityStats[diseaseIndex][testIndex][count].countPMDFNeg++;
                        }
                    }
                    else {
                        if (testIndex < 2) {
                            // don't add in lost to follow up or sample results no read yet
                            total.countPFPos++;
                            diseaseTotal[diseaseIndex].countPFPos++;
                        }
                        facilityStats[diseaseIndex][testIndex][count].countPFPos++;
                    }
                }

                // count infected and uninfected patients for each  test type
                if (pat->getInfected()) {
                    total.infected++;
                    diseaseTotal[diseaseIndex].infected++;
                    facilityStats[diseaseIndex][testIndex][count].infected++;
                }
                else {
                    total.uninfected++;
                    diseaseTotal[diseaseIndex].uninfected++;
                    facilityStats[diseaseIndex][testIndex][count].uninfected++;
                }

                // dump statistics for each patient - only do it to file
                if (invdividualFlag) {
                    myfile1 << status << "," << pat->getDisease() << "," << pat->getSampleTest() << "," << pat->getAge() << "," << pat->isAlive() << "," << pat->getInfected() << "," << pat->getTimeFromOnset() << "," << "," << pat->getTimeStepsInTreatment() << "," << pat->getTimeStepsNotInTreatment() << "\n";
                }

                // dump statistics for each patient - only do it to file
                if (invdividualFlag) {

                    // get time since patient has turned inactive
                    lagTime = timeStep - pat->getTimestepInactive();

                    myfile1 << status << "," << pat->getDisease() << "," << pat->getSampleTest() << "," << pat->getAge() << "," << pat->isAlive() << "," << pat->getInfected() << "," << pat->getTimeFromOnset() + lagTime << "," << "," << pat->getTimeStepsInTreatment() << "," << pat->getTimeStepsNotInTreatment() + lagTime << "\n";
                }

                // count dead people from old age
                // age = pat->getAge();
                //if (!pat->isAlive()) {
                //    countPD++;
                //}
            }

            // get patient stats
            facilityCountP = facilityCountP + pI.size();
            total.countP = total.countP + pI.size();
        }

        //std::cout << "stats: after inactive patients sample size = " << samples.size() << "\n";

        // if facility has diagnosis - find out the average time from sample creation to test results return
        if (facility->diagnosisAvailable()) {

            // loop through samples at facility and get the average time from sample creation to test results returned
            samplesSize = samples.size();

            for (sampleIndex = 0; sampleIndex < samplesSize; sampleIndex++) {

                // make sure sample is returned
                if (samples[sampleIndex]->getStatus() > 4) {

                    // make sure sample originated here to count it
                    if(samples[sampleIndex]->getOrigFacility() == facility->getName()) {

                        // get the index for the disease
                        diseaseIndex = 0;

                        for (int w = 0; w < diseaseCount; w++) {

                            if (samples[sampleIndex]->getDisease() == diseaseList[w]) {
                                diseaseIndex = w;
                            }
                        }

                        //std::cout << "stats: inside status if statement status = " << samples[sampleIndex]->getStatus() << "\n";

                        facilityStats[diseaseIndex][2][count].sampleCount++;

                        facilityStats[diseaseIndex][2][count].sampleTotalTime = facilityStats[diseaseIndex][2][count].sampleTotalTime + (samples[sampleIndex]->getResultsReturnedToOrigFacilityTime() - samples[sampleIndex]->getCreateTime());

                        //std::cout << "stats: sample lab time loop  status > 4 sample time = " << facilityStats[diseaseIndex][2][count].sampleTotalTime << " , orig time = " << samples[sampleIndex]->getResultsReturnedToOrigFacilityTime() << ", create time = " << samples[sampleIndex]->getCreateTime() << "\n";
                    }
                }
            }

            // now add in the inactive samples
            samplesSize = samplesInactive.size();

            for (sampleIndex = 0; sampleIndex < samplesSize; sampleIndex++) {

                // make sure sample is returned
                if (samplesInactive[sampleIndex]->getStatus() > 4) {

                    // make sure sample originated here to count it
                    if(samplesInactive[sampleIndex]->getOrigFacility() == facility->getName()) {

                        // get the index for the disease
                        diseaseIndex = 0;

                        for (int w = 0; w < diseaseCount; w++) {

                            if (samplesInactive[sampleIndex]->getDisease() == diseaseList[w]) {
                                diseaseIndex = w;
                            }
                        }

                        //std::cout << "stats: inside status if statement status = " << samples[sampleIndex]->getStatus() << "\n";

                        facilityStats[diseaseIndex][2][count].sampleCount++;

                        facilityStats[diseaseIndex][2][count].sampleTotalTime = facilityStats[diseaseIndex][2][count].sampleTotalTime + (samplesInactive[sampleIndex]->getResultsReturnedToOrigFacilityTime() - samplesInactive[sampleIndex]->getCreateTime());

                        //std::cout << "stats: sample lab time loop  status > 4 sample time = " << facilityStats[diseaseIndex][2][count].sampleTotalTime << " , orig time = " << samplesInactive[sampleIndex]->getResultsReturnedToOrigFacilityTime() << ", create time = " << samplesInactive[sampleIndex]->getCreateTime() << "\n";
                    }
                }
            }


            //for (int w = 0; w < diseaseCount; w++) {
            //
            //    if (sampleCount[w] > 0) {
            //        facilityStats[w][2][count].sampleTotalTime = facilityStats[w][2][count].sampleTotalTime / facilityStats[w][2][count].sampleCount;
            //
            //        //std::cout << "stats: after sample lab time  sampleTotalTime =  " << facilityStats[w][2][count].sampleTotalTime << "\n";
            //    }
            //}
        }


        // loop through each disease in model run and update statistics
        // store disease specific totals in Loss to Follow up test results (index = 2)
        for (diseaseIndex = 0; diseaseIndex < diseaseCount; diseaseIndex++) {

            // update the patient count for each disease for this facility
            facilityStats[diseaseIndex][2][count].countP = patientCount[diseaseIndex];
            //std::cout << "stats:facilityCountP =  " << facilityCountP << ", patientCount 0  = " << patientCount[diseaseIndex] << "\n";

            // loop through tests to count up resources used
            facilityStats[diseaseIndex][2][count].totalTests = 0;

            if (facility->testingAvailable(diseaseList[diseaseIndex],"Xpert")) {

                // loop through tests to count up resources used
                for (auto test : tests) {

                    if (test->getDiseaseName() == diseaseList[diseaseIndex]) {
                        //std::cout << "stats: fac disease total tests =  " << facilityStats[diseaseIndex][count].totalTests << ", test - total tests = " << test->getTotalTests() << ",     test name = " << test->getTestName() << "\n";

                        facilityStats[diseaseIndex][2][count].totalTests = facilityStats[diseaseIndex][2][count].totalTests + test->getTotalTests();
                    }
                }
            }
            total.totalTests = total.totalTests + facilityStats[diseaseIndex][2][count].totalTests;

            // added up other stats accross all tests
            facilityStats[diseaseIndex][2][count].pop = facilityStats[diseaseIndex][0][count].pop + facilityStats[diseaseIndex][1][count].pop + facilityStats[diseaseIndex][2][count].pop;
            facilityStats[diseaseIndex][2][count].countPDI = facilityStats[diseaseIndex][0][count].countPDI + facilityStats[diseaseIndex][1][count].countPDI + facilityStats[diseaseIndex][2][count].countPDI;
            facilityStats[diseaseIndex][2][count].countPC = facilityStats[diseaseIndex][0][count].countPC + facilityStats[diseaseIndex][1][count].countPC + facilityStats[diseaseIndex][2][count].countPC;
            facilityStats[diseaseIndex][2][count].totalTests = facilityStats[diseaseIndex][0][count].totalTests + facilityStats[diseaseIndex][1][count].totalTests + facilityStats[diseaseIndex][2][count].totalTests;
            facilityStats[diseaseIndex][2][count].infectionDays = facilityStats[diseaseIndex][0][count].infectionDays + facilityStats[diseaseIndex][1][count].infectionDays + facilityStats[diseaseIndex][2][count].infectionDays;
            facilityStats[diseaseIndex][2][count].noPatientsInfectionDays = facilityStats[diseaseIndex][0][count].noPatientsInfectionDays + facilityStats[diseaseIndex][1][count].noPatientsInfectionDays + facilityStats[diseaseIndex][2][count].noPatientsInfectionDays;
            facilityStats[diseaseIndex][2][count].noPatientsGettingDrugs = facilityStats[diseaseIndex][0][count].noPatientsGettingDrugs + facilityStats[diseaseIndex][1][count].noPatientsGettingDrugs + facilityStats[diseaseIndex][2][count].noPatientsGettingDrugs;
            facilityStats[diseaseIndex][2][count].noPatientsGettingDrugs = facilityStats[diseaseIndex][0][count].noPatientsGettingDrugs + facilityStats[diseaseIndex][1][count].noPatientsGettingDrugs + facilityStats[diseaseIndex][2][count].noPatientsGettingDrugs;
            facilityStats[diseaseIndex][2][count].countPT = facilityStats[diseaseIndex][0][count].countPT + facilityStats[diseaseIndex][1][count].countPT + facilityStats[diseaseIndex][2][count].countPT;
            facilityStats[diseaseIndex][2][count].countPR = facilityStats[diseaseIndex][0][count].countPR + facilityStats[diseaseIndex][1][count].countPR + facilityStats[diseaseIndex][2][count].countPR;
            facilityStats[diseaseIndex][2][count].countPUT = facilityStats[diseaseIndex][0][count].countPUT + facilityStats[diseaseIndex][1][count].countPUT + facilityStats[diseaseIndex][2][count].countPUT;
            facilityStats[diseaseIndex][2][count].pop = facilityStats[diseaseIndex][0][count].pop + facilityStats[diseaseIndex][1][count].pop + facilityStats[diseaseIndex][2][count].pop;

            //std::cout << "stats:cured 0 =  " << facilityStats[diseaseIndex][0][count].countPC << ", 1 = " << facilityStats[diseaseIndex][1][count].countPC << ", 2 = " << facilityStats[diseaseIndex][2][count].countPC << "\n";

            // patients with TB = in treatment + treatment failure + pos test no treat + no test no treat + loss to followup + pat dead + pat cured 
            // add in counts from all tests
            facilityStats[diseaseIndex][2][count].withTB = facilityStats[diseaseIndex][2][count].countPT + facilityStats[diseaseIndex][2][count].countPR + facilityStats[diseaseIndex][2][count].countPUT + facilityStats[diseaseIndex][2][count].countPDI + facilityStats[diseaseIndex][2][count].countPC;;

            // prevalence = no. with TB * (100,000/population)                           (no. with TB/pop = prevalence/100000)
            // total pop stored in all test subcategories - choose 1
            if (facilityStats[diseaseIndex][2][count].pop == 0) {
                facilityStats[diseaseIndex][2][count].prevalence = 0;
            }
            else {
                facilityStats[diseaseIndex][2][count].prevalence = float((facilityStats[diseaseIndex][2][count].withTB) * 100000) / float(facilityStats[diseaseIndex][2][count].pop);
            }

            // DALY YLL
            facilityStats[diseaseIndex][2][count].yll = (facilityStats[diseaseIndex][0][count].countPDI + facilityStats[diseaseIndex][1][count].countPDI + facilityStats[diseaseIndex][2][count].countPDI) * 5.035733;

            // average time to drugs = (time step recieved results - time step sample created)/infected patients that received treatement
            if (facilityStats[diseaseIndex][2][count].noPatientsGettingDrugs > 1) {
                facilityStats[diseaseIndex][2][count].avgTimeToDrugs = facilityStats[diseaseIndex][2][count].timeToDrugs / facilityStats[diseaseIndex][2][count].noPatientsGettingDrugs;
            }
            else {
                facilityStats[diseaseIndex][2][count].avgTimeToDrugs = 0;
            }

            // average time of infection = time step infected/infected patients
            if (facilityStats[diseaseIndex][2][count].noPatientsInfectionDays > 1) {
                facilityStats[diseaseIndex][2][count].avgInfectionDays = facilityStats[diseaseIndex][2][count].infectionDays / facilityStats[diseaseIndex][2][count].noPatientsInfectionDays;
            }
            else {
                facilityStats[diseaseIndex][2][count].avgInfectionDays = 0;
            }



            //std::cout << "stats: avgTimeToDrugs =  " << avgTimeToDrugs << ", time to drugs = " << facilityStats[diseaseIndex][count].timeToDrugs << ", patients = " << facilityStats[diseaseIndex][count].noPatientsGettingDrugs << "\n";
            //std::cout << "stats: avgInfectionDays =  " << avgTimeToDrugs << ", infection days = " << facilityStats[diseaseIndex][count].infectionDays << ", patients = " << facilityStats[diseaseIndex][count].noPatientsInfectionDays << "\n";

        }
        //std::cout << "stats: after disease index loop count  =  " << count << "\n";

        count = count + 1;
    }

    diseaseNameTest = "";

    if (fileFlag) {
        
        // loop through each facility and the number of Patients and Samples at each facility
        for (int fcount = 0; fcount < countF; fcount++) {

            // loop through each disease in model run and update statistics
            // store disease specific totals in Loss to Follow up test results (index = 2)
            for (diseaseIndex = 0; diseaseIndex < diseaseCount; diseaseIndex++) {
             
                // get the each test name associated with this disease
                for (int testIndex = 0; testIndex < testCount; testIndex++) {

                    if (diseaseList[diseaseIndex] == testList[testIndex]->getDiseaseName()) {

                        if (firstTestName == "") {
                            firstTestName = testList[testIndex]->getTestName();
                        }
                        else {
                            secondTestName = testList[testIndex]->getTestName();
                        }

                    }
                }

               // output header if the disease name changes
                if (diseaseNameTest != diseaseList[diseaseIndex]) {

                    myfile << " ID " << ", Year " << ", Catchment " << ", No. of Patients " << ", Disease " << ", Calculated Prevalence ";
                    myfile << ", YLL" << ", Patients with Disease " << ", Dead from disease " << ", Cured " << ", No. of Tests " << ", " << firstTestName << " True-Pos " << ", " << firstTestName << " False-Neg " << ", " << firstTestName << " True-Neg " << "," << firstTestName << " False-Pos " << ", " << firstTestName << " MD True-Pos ";
                    myfile << ", " << firstTestName << " MD False-Neg " << ", " << firstTestName << " MD True-Neg " << ", " << firstTestName << " MD False-Pos " << ", " << secondTestName << " True-Pos " << ", " << secondTestName << " False-Neg " << "," << secondTestName << " True-Neg " << "," << secondTestName << " False-Pos " << "," << secondTestName << " MD True-Pos ";
                    myfile << "," << secondTestName << " MD False-Neg " << "," << secondTestName << " MD True-Neg " << "," << secondTestName << " MD False-Pos " << ", LtoF infected " << ", LtoF uninfected " << ", Avg. time to drugs " << ", Total infectious days " << ", Avg. infectious days " << ", Avg. Sample Lab time " << "\n";
                      
                    // update current disease name
                    diseaseNameTest = diseaseList[diseaseIndex];
                }

                // calculate sample lab trip time
                sampleTime = 0;
                if (facilityStats[diseaseIndex][2][fcount].sampleCount > 0) {

                    sampleTime = facilityStats[diseaseIndex][2][fcount].sampleTotalTime / facilityStats[diseaseIndex][2][fcount].sampleCount;

                    // update totals for Sample lab time at the same time
                    total.sampleTotalTime = total.sampleTotalTime + facilityStats[diseaseIndex][2][fcount].sampleTotalTime;
                    total.sampleCount = total.sampleCount + facilityStats[diseaseIndex][2][fcount].sampleCount;
                }

                // dump statistics for each facility to a file
                myfile << facilityStats[diseaseIndex][2][fcount].facID << "," << year << "," << facilityStats[diseaseIndex][2][fcount].pop;
                myfile << "," << facilityStats[diseaseIndex][2][fcount].countP << "," << diseaseList[diseaseIndex];
                myfile << "," << facilityStats[diseaseIndex][2][fcount].prevalence << "," << facilityStats[diseaseIndex][2][fcount].yll << "," << facilityStats[diseaseIndex][2][fcount].withTB << "," << facilityStats[diseaseIndex][2][fcount].countPDI << "," << facilityStats[diseaseIndex][2][fcount].countPC;
                myfile << "," << facilityStats[diseaseIndex][2][fcount].totalTests << "," << facilityStats[diseaseIndex][0][fcount].countPTPos << "," << facilityStats[diseaseIndex][0][fcount].countPFNeg;
                myfile << "," << facilityStats[diseaseIndex][0][fcount].countPTNeg << "," << facilityStats[diseaseIndex][0][fcount].countPFPos << "," << facilityStats[diseaseIndex][0][fcount].countPMDTPos;
                myfile << "," << facilityStats[diseaseIndex][0][fcount].countPMDFNeg << "," << facilityStats[diseaseIndex][0][fcount].countPMDTNeg << "," << facilityStats[diseaseIndex][0][fcount].countPMDFPos;
                myfile << "," << facilityStats[diseaseIndex][1][fcount].countPTPos << "," << facilityStats[diseaseIndex][1][fcount].countPFNeg;
                myfile << "," << facilityStats[diseaseIndex][1][fcount].countPTNeg << "," << facilityStats[diseaseIndex][1][fcount].countPFPos << "," << facilityStats[diseaseIndex][1][fcount].countPMDTPos;
                myfile << "," << facilityStats[diseaseIndex][1][fcount].countPMDFNeg << "," << facilityStats[diseaseIndex][1][fcount].countPMDTNeg << "," << facilityStats[diseaseIndex][1][fcount].countPMDFPos;
                myfile << "," << facilityStats[diseaseIndex][2][fcount].infected << "," << facilityStats[diseaseIndex][2][fcount].uninfected;
                myfile << "," << facilityStats[diseaseIndex][2][fcount].avgTimeToDrugs << "," << facilityStats[diseaseIndex][2][fcount].infectionDays << "," << facilityStats[diseaseIndex][2][fcount].avgInfectionDays;
                myfile << "," << sampleTime << "\n";
            }

            count = count + 1;
        }
    }

    // DALY calculation is yyl = (no. of patients dead from disease / .03) * (1 - e^-(.03*life expectancy))
    //                           (no. of patients dead from disease / .03) * (1 - e^-(.03*63))
    //                           (no. of patients dead from disease) * 5.035733
    total.yll = total.countPDI * 5.035733;

    if (fileFlag) {
        myfile << "Facility Count" << ",Year" <<  ", Population " << ",No. of Patients " << ", disease " << ", Calculated Prevalence " << ", DALY YLL " << ", Patients with Disease  ";
        myfile << ", Dead from disease " << ", Cured " << ", No. of Tests " << ", True-Pos " << ", False-Neg " << ", True-Neg " << ", False-Pos " << ", MD True-Pos " << ", MD False-Neg " << ", MD True-Neg " << ", MD False-Pos ";
        myfile << ", Total Infected " << ", Total Uninfected " << ", Avg. time to drugs " << ", Total infectious days " << ", Avg. infectious days "  << ", Avg. Sample Lab time " << "\n";
        
       
        // patients with TB = in treatment + treatment failure + pos test no treat + no test no treat + loss to followup + pat dead + pat cured
        total.withTB = total.countPT + total.countPR + total.countPUT + total.countPDI + total.countPC;

        total.prevalence = (float(total.withTB) * 100000)/ float(total.pop);

        // average time to drugs = (time step recieved results - time step sample created)/infected patients that received treatement
        if (total.noPatientsGettingDrugs > 1) {
            total.avgTimeToDrugs = total.timeToDrugs / total.noPatientsGettingDrugs;
        }
        else {
            total.avgTimeToDrugs = 0;
        }
        
        // average time of infection = time step infected/infected patients
        if (total.noPatientsInfectionDays > 1) {
            total.avgInfectionDays = total.infectionDays / total.noPatientsInfectionDays;
        }
        else {
            total.avgInfectionDays = 0;
        }

        // calculate sample lab trip time
        sampleTime = 0;
        if (total.sampleCount > 0) {

            sampleTime = total.sampleTotalTime / total.sampleCount;
        }

        myfile << countF << "," << year << "," << total.pop << "," << total.countP << "," << "" << "," << total.prevalence << "," << total.yll << "," << total.withTB;
        myfile  << "," << total.countPDI << "," << total.countPC << "," << total.totalTests;
        myfile << "," << total.countPTPos << "," << total.countPFNeg << "," << total.countPTNeg << "," << total.countPFPos << "," << total.countPMDTPos << "," << total.countPMDFNeg << "," << total.countPMDTNeg << "," << total.countPMDFPos;
        myfile << "," << total.infected << "," << total.uninfected << "," << total.avgTimeToDrugs << "," << total.infectionDays << "," << total.avgInfectionDays << "," << sampleTime << "\n";

        //myfile << "," << facilityStats[diseaseIndex][count].totalTests << "," << facilityStats[diseaseIndex][count].countPUT;
        //myfile << "," << facilityStats[diseaseIndex][count].countPUIUT << "," << facilityStats[diseaseIndex][count].countPO << "," << facilityStats[diseaseIndex][count].countPUI;
        //myfile << "," << facilityStats[diseaseIndex][count].countPUINR << "," << facilityStats[diseaseIndex][count].countPDU << "\n";

        myfile.close();

    }
    else {
        std::cout << "stats: fac =  " << countF << ", population = " << total.pop << ", patients = " << total.countP << ",      DALY YLL = " << total.yll << "\n";
        std::cout << "stats: patient" << " Untested-out = " << total.countPU << " Untested-in =  " << total.countPI << ", under treat = " << total.countPT << ", failed treat = " << total.countPR << ", untreated = " << total.countPUT << ", cured = " << total.countPC << ", uninfec-treat = " << total.countPUIUT << ", Other = " << total.countPO << "\n";
        std::cout << "               Non-infected = " << total.countPUI << ", Non-infected, not returned = " << total.countPUINR << ", dead - infected = " << total.countPDI << ", dead - uninfected = " << total.countPDU << "\n";

    }

    if (invdividualFlag) {

        myfile1 << "-----------------------------------------------------------------------------------  " << "\n";
        myfile1 << "stats: fac =  " << countF << ", population = " << total.pop << ", patients = " << total.countP << ",      DALY YLL = " << total.yll << "\n";
        myfile1 << "stats: patient" << " Untested-out = " << total.countPU << " Untested-in =  " << total.countPI << ", under treat = " << total.countPT << ", failed treat = " << total.countPR << ", untreated = " << total.countPUT << ", cured = " << total.countPC << ", uninfec-treat = " << total.countPUIUT << ", Other = " << total.countPO << "\n";
        myfile1 << "               Non-infected = " << total.countPUI << ", Non-infected, not returned = " << total.countPUINR << ", dead - infected = " << total.countPDI << ", dead - uninfected = " << total.countPDU << "\n";
        myfile1.close();
    }
}



