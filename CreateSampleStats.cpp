//
//  CreateSampleStats.cpp
// 
//
//  Created by L. Dombrowski on 4/29/20

//
// Split out of CreateStats on 5/14/20


#include <string>
#include <vector>
#include <cstring>
#include <sys/stat.h>
#include <errno.h> 

#include "CreateSampleStats.h"
#include "GlobalParameters.h"
#include "FacilitiesList.h"
using namespace std;




// count how many Samples currently in existence 
void sampleStats(FacilitiesList* list, vector<string> diseaseList, bool fileFlag, bool yearly, int timeStep) {

    Diagnosis* d;
    Testing* t;
    vector<Patient*> p;
    vector<Sample*> s;
    ofstream myfile;
    ofstream myfile1;
    ofstream myfile2;
    ofstream myfile3;
    ofstream myfile4;

    int countF = 0;
    int facilityCountS = 0;
    int count = 0;
    int status;
    int diseaseCount;
    int diseaseIndex;
    int errNo;
    string diseaseName;
    string subDirBase;
    string subDir;
    string subDirYear;
    string test1Name = "";
    string test2Name = "";
    string test3Name = "";
    string test4Name = "";
    bool debug = false;

    // keep patient statistics by region and district
    struct geographicalStats {
        string name;
        int countS = 0;
        int countSN = 0;
        int countSB = 0;
        int countST = 0;
        int countSW = 0;
        int countSL = 0;
        int countSRT = 0;
        float countSR = 0;
        int countSRL = 0;
        int countSO = 0;
        int test1Count = 0;
        int test2Count = 0;
        int test3Count = 0;
        int test4Count = 0;
        float sampleTurnAroundTime = 0;
    };

    // find out how many diseases to save stats for
    // use this as an count to create arrays for each of the diseases being run
    diseaseCount = diseaseList.size();

    geographicalStats total;
    geographicalStats totalRegion;
    geographicalStats totalDistrict;
    geographicalStats diseaseTotal[diseaseCount];
    geographicalStats diseaseTotalRegion[diseaseCount];
    geographicalStats diseaseTotalDistrict[diseaseCount];

    // keep sample statistics by region and district
    geographicalStats regions[diseaseCount][25];
    geographicalStats districts[diseaseCount][270];

    int regionCount[diseaseCount];
    int districtCount[diseaseCount];
    int sampleCount[diseaseCount];
    string region;
    string district;
    bool regionNotFound;
    bool districtNotFound;

    for (int i = 0; i < diseaseCount; i++) {

        regionCount[i] = 0;
        districtCount[i] = 0;
    }

    vector<Facility*> facilities = list->getListPtr();

    countF = facilities.size();

    // get statistical array for each facility
    geographicalStats facilityStats[diseaseCount][countF];

    // initialize all region data to 0
    for (int w = 0; w < diseaseCount; w++) {

        for (int i = 0; i < 25; i++) {

            regions[w][i].name = "";
            regions[w][i].countS = 0;
            regions[w][i].countSN = 0;
            regions[w][i].countSB = 0;
            regions[w][i].countST = 0;
            regions[w][i].countSW = 0;
            regions[w][i].countSL = 0;
            regions[w][i].countSRT = 0;
            regions[w][i].countSR = 0;
            regions[w][i].countSRL = 0;
            regions[w][i].countSO = 0;
            regions[w][i].test1Count = 0;
            regions[w][i].test2Count = 0;
            regions[w][i].test3Count = 0;
            regions[w][i].test4Count = 0;
            regions[w][i].sampleTurnAroundTime = 0;

        }


        // initialize all district data to 0
        for (int i = 0; i < 270; i++) {

            districts[w][i].name = "";
            districts[w][i].countS = 0;
            districts[w][i].countSB = 0;
            districts[w][i].countSN = 0;
            districts[w][i].countST = 0;
            districts[w][i].countSW = 0;
            districts[w][i].countSL = 0;
            districts[w][i].countSRT = 0;
            districts[w][i].countSR = 0;
            districts[w][i].countSRL = 0;
            districts[w][i].countSO = 0;
            districts[w][i].test1Count = 0;
            districts[w][i].test2Count = 0;
            districts[w][i].test3Count = 0;
            districts[w][i].test4Count = 0;
            districts[w][i].sampleTurnAroundTime = 0;

        }

        // reset region and district totals
        diseaseTotal[w].countS = 0;
        diseaseTotal[w].countSN = 0;
        diseaseTotal[w].countSB = 0;
        diseaseTotal[w].countST = 0;
        diseaseTotal[w].countSW = 0;
        diseaseTotal[w].countSL = 0;
        diseaseTotal[w].countSRT = 0;
        diseaseTotal[w].countSR = 0;
        diseaseTotal[w].countSRL = 0;
        diseaseTotal[w].countSO = 0;
        diseaseTotal[w].test1Count = 0;
        diseaseTotal[w].test2Count = 0;
        diseaseTotal[w].test3Count = 0;
        diseaseTotal[w].test4Count = 0;
        diseaseTotal[w].sampleTurnAroundTime = 0;
        diseaseTotalRegion[w].countS = 0;
        diseaseTotalRegion[w].countSN = 0;
        diseaseTotalRegion[w].countSB = 0;
        diseaseTotalRegion[w].countST = 0;
        diseaseTotalRegion[w].countSW = 0;
        diseaseTotalRegion[w].countSL = 0;
        diseaseTotalRegion[w].countSRT = 0;
        diseaseTotalRegion[w].countSR = 0;
        diseaseTotalRegion[w].countSRL = 0;
        diseaseTotalRegion[w].countSO = 0;
        diseaseTotalRegion[w].test1Count = 0;
        diseaseTotalRegion[w].test2Count = 0;
        diseaseTotalRegion[w].test3Count = 0;
        diseaseTotalRegion[w].test4Count = 0;
        diseaseTotalRegion[w].sampleTurnAroundTime = 0;
        diseaseTotalDistrict[w].countS = 0;
        diseaseTotalDistrict[w].countSN = 0;
        diseaseTotalDistrict[w].countSB = 0;
        diseaseTotalDistrict[w].countST = 0;
        diseaseTotalDistrict[w].countSW = 0;
        diseaseTotalDistrict[w].countSL = 0;
        diseaseTotalDistrict[w].countSRT = 0;
        diseaseTotalDistrict[w].countSR = 0;
        diseaseTotalDistrict[w].countSRL = 0;
        diseaseTotalDistrict[w].countSO = 0;
        diseaseTotalDistrict[w].test1Count = 0;
        diseaseTotalDistrict[w].test2Count = 0;
        diseaseTotalDistrict[w].test3Count = 0;
        diseaseTotalDistrict[w].test4Count = 0;
        diseaseTotalDistrict[w].sampleTurnAroundTime = 0;
    }

    // reset region and district totals
    totalRegion.countS = 0;
    totalRegion.countSN = 0;
    totalRegion.countSB = 0;
    totalRegion.countST = 0;
    totalRegion.countSW = 0;
    totalRegion.countSL = 0;
    totalRegion.countSRT = 0;
    totalRegion.countSR = 0;
    totalRegion.countSRL = 0;
    totalRegion.countSO = 0;
    totalRegion.test1Count = 0;
    totalRegion.test2Count = 0;
    totalRegion.test3Count = 0;
    totalRegion.test4Count = 0;
    totalRegion.sampleTurnAroundTime = 0;
    totalDistrict.countS = 0;
    totalDistrict.countSN = 0;
    totalDistrict.countSB = 0;
    totalDistrict.countST = 0;
    totalDistrict.countSW = 0;
    totalDistrict.countSL = 0;
    totalDistrict.countSRT = 0;
    totalDistrict.countSR = 0;
    totalDistrict.countSRL = 0;
    totalDistrict.countSO = 0;
    totalDistrict.test1Count = 0;
    totalDistrict.test2Count = 0;
    totalDistrict.test3Count = 0;
    totalDistrict.test4Count = 0;
    totalDistrict.sampleTurnAroundTime = 0;


    // if fileFlag is true, output to a file
    if (fileFlag) {

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

        if (yearly) {

            subDirYear = "year-" + to_string(currentYearInRun-1);
            
            subDir = subDirBase + "/" + subDirYear;
        }
        else {
            
            subDir = subDirBase + "/total";
        }

        // convert to char array
        char subArray4[subDir.length() +1];
        // copying the contents of the
        // string to char array
        strcpy(subArray4, subDir.c_str());
            
        // check if directory is created or not 
        if (mkdir(subArray4,0777) != 0) {
                
            // std::cout << "unable to create directory = " << subArray4 << "\n"; 
        }

        if (false) std::cout << "inside create sample stats subDirBase " << subDirBase << ", statSubScenarioDir = " << statSubScenarioDir  << ", subDir = " << subDir << "\n";

        myfile.open(subDir + "/SamplesStats.csv");
        myfile1.open(subDir + "/SamplesStatsReturned.txt");
        myfile2.open(subDir + "/SamplesStats-facilities.csv");
        myfile3.open(subDir + "/SamplesStats-districts.csv");
        myfile4.open(subDir + "/SamplesStats-regions.csv");

        myfile << "facility" << ",samples" << ",disease" << ",New" << ",New.in.batch" << ",In.Transit" << ",Wait" << ",Lost" << ",Returned-transit" << ",Returned" << ",Re/Lost";
        myfile << ",Other" << ",avg.return.time" << ",test1.name" << ",test1.count" << ",tes2.name" << ",test2.count" << ",test3.name" << ",test3.count" << ",test4.name" << ",test4.count" << "\n";
        
        myfile1 << "stats.samples.patient.ptr_" << ",orig.fac_" << ",testing.fac_" << "\n";

        myfile2 << "facility" << ",samples" << ",disease" << ",New" << ",New.in.batch" << ",In.Transit" << ",Wait" << ",Lost" << ",Returned-transit" << ",Returned" << ",Re/Lost";
        myfile2 << ",Other" << ",avg.return.time" << ",test1.name" << ",test1.count" << ",test2.name" << ",test2.count" << ",test3.name" << ",test3.count" << ",test4.name" << ",test4.count" << "\n";
        
        myfile3 << "facility" << ",samples" << ",disease" << ",New" << ",New.in.batch" << ",In.Transit" << ",Wait" << ",Lost" << ",Returned-transit" << ",Returned" << ",Re/Lost";
        myfile3 << ",Other" << ",avg.return.time" << ",test1.name" << ",test1.count" << ",test2.name" << ",test2.count" << ",test3.name" << ",test3.count" << ",test4.name" << ",test4.count" << "\n";
        
        myfile4 << "facility" << ",samples" << ",disease" << ",New" << ",New.in.batch" << ",In.Transit" << ",Wait" << ",Lost" << ",Returned-transit" << ",Returned" << ",Re/Lost";
        myfile4 << ",Other" << ",avg.return.time" << ",test1.name" << ",test1.count" << ",test2.name" << ",test2.count" << ",test3.name" << ",test3.count" << ",test4.name" << ",test4.count" << "\n";
    }

    // loop through each facility and the number of Patients and Samples at each facility
    for (auto facility : facilities) {

        // get the up name for this facility
        d = facility->getDiagnosisPtr();
        t = facility->getTestingPtr();
        p = d->getPatientPtr();
        s = t->getSamplePtr();
        region = facility->getRegion();
        district = facility->getDistrict();

        // get sample stats
        facilityCountS = s.size();
        total.countS = total.countS + facilityCountS;

        // update facility statistics
        for (int w = 0; w < diseaseCount; w++) {
            facilityStats[w][count].name = facility->getName();
            facilityStats[w][count].countS = 0;
            facilityStats[w][count].countSN = 0;
            facilityStats[w][count].countSB = 0;
            facilityStats[w][count].countST = 0;
            facilityStats[w][count].countSW = 0;
            facilityStats[w][count].countSL = 0;
            facilityStats[w][count].countSRT = 0;
            facilityStats[w][count].countSR = 0;
            facilityStats[w][count].countSRL = 0;
            facilityStats[w][count].countSO = 0;
            facilityStats[w][count].test1Count = 0;
            facilityStats[w][count].test2Count = 0;
            facilityStats[w][count].test3Count = 0;
            facilityStats[w][count].test4Count = 0;
            facilityStats[w][count].sampleTurnAroundTime = 0;

            sampleCount[w] = 0;
        }

        if (fileFlag) {
            //std::cout << "stats: samples facility count =  " << count << ", facility name = " << facility->getName() << "\n";
        }

        //get sample stats
        for (auto samp : s) {

            // store stats by disease
            diseaseName = samp->getDisease();

            // get the index for the disease
            diseaseIndex = 0;

            for (int w = 0; w < diseaseCount; w++) {

                if (diseaseName == diseaseList[w]) {
                    diseaseIndex = w;

                    // increase the sample count for this disease for this facility
                    sampleCount[w]++;
                }
            }

            // sample status 
            // 0 - SampCreatedUnTest;		// created but untested
            // 1 - SampCreatedUnTestBatch;	// created, untested waiting to be sent out in batch mode
            // 2 - SampUnTestInTransit;		// in transit
            // 3 - SampWaitForTest;			// waiting for test
            // 4 - SampLostExpired;			// lost/expired
            // 5 - SampTestedInTransit;		// results in transit back to orig facility
            // 6 - SampReturnedAvail;		// results returned - available
            // 7 - SampReturnedLostExpired;	// loss to follow up
            // 8 - SampLtoF;				// loss to follow up
            // 9 - SampPatDead              // patient dead	
            status = samp->getStatus();

            switch (status) {
            case 0:
                total.countSN++;
                diseaseTotal[diseaseIndex].countSN++;
                facilityStats[diseaseIndex][count].countSN++;
                break;
            case 1:
                total.countSB++;
                diseaseTotal[diseaseIndex].countSB++;
                facilityStats[diseaseIndex][count].countSB++;
                break;
            case 2:
                total.countST++;
                diseaseTotal[diseaseIndex].countST++;
                facilityStats[diseaseIndex][count].countST++;
                break;
            case 3:
                total.countSW++;
                diseaseTotal[diseaseIndex].countSW++;
                facilityStats[diseaseIndex][count].countSW++;
                break;
            case 4:
                total.countSL++;
                diseaseTotal[diseaseIndex].countSL++;
                facilityStats[diseaseIndex][count].countSL++;
                break;
            case 5:
                total.countSRT++;
                diseaseTotal[diseaseIndex].countSRT++;
                facilityStats[diseaseIndex][count].countSRT++;
                break;
            case 6:
                total.countSR++;
                diseaseTotal[diseaseIndex].countSR++;
                facilityStats[diseaseIndex][count].countSR++;

                myfile1 << samp->getPatientPtr() << "," << samp->getOrigFacility() << "," << samp->getTestingFacility() << "\n";

                total.sampleTurnAroundTime = total.sampleTurnAroundTime + (samp->getResultsReturnedToOrigFacilityTime() - samp->getCreateTime());
                diseaseTotal[diseaseIndex].sampleTurnAroundTime = diseaseTotal[diseaseIndex].sampleTurnAroundTime  + (samp->getResultsReturnedToOrigFacilityTime() - samp->getCreateTime()) ;
                facilityStats[diseaseIndex][count].sampleTurnAroundTime = facilityStats[diseaseIndex][count].sampleTurnAroundTime + (samp->getResultsReturnedToOrigFacilityTime() - samp->getCreateTime());
                break;
            case 7:
                total.countSRL++;
                diseaseTotal[diseaseIndex].countSRL++;
                facilityStats[diseaseIndex][count].countSRL++;
                break;
            default:
                total.countSO++;
                diseaseTotal[diseaseIndex].countSO++;
                facilityStats[diseaseIndex][count].countSO++;

            }

            // record count of tests for Samples
            if (test1Name == "") {
                test1Name = samp->getTest(); 
                total.test1Count++;
                diseaseTotal[diseaseIndex].test1Count++;
                facilityStats[diseaseIndex][count].test1Count++;
            }
            else if (test1Name == samp->getTest()) {
                total.test1Count++;
                diseaseTotal[diseaseIndex].test1Count++;
                facilityStats[diseaseIndex][count].test1Count++;
            }
            else if (test2Name == "") {
                test2Name = samp->getTest(); 
                total.test2Count++;
                diseaseTotal[diseaseIndex].test2Count++;
                facilityStats[diseaseIndex][count].test2Count++;
            }
            else if (test2Name == samp->getTest()) {
                total.test2Count++;
                diseaseTotal[diseaseIndex].test2Count++;
                facilityStats[diseaseIndex][count].test2Count++;
            }
            else if (test3Name == "") {
                test3Name = samp->getTest(); 
                total.test3Count++;
                diseaseTotal[diseaseIndex].test3Count++;
                facilityStats[diseaseIndex][count].test3Count++; 
            }
            else if (test3Name == samp->getTest()) {
                total.test3Count++;
                diseaseTotal[diseaseIndex].test3Count++;
                facilityStats[diseaseIndex][count].test3Count++;
            }
            else if (test4Name == "") {
                test4Name = samp->getTest();
                total.test4Count++;
                diseaseTotal[diseaseIndex].test4Count++;
                facilityStats[diseaseIndex][count].test4Count++;
            }
            else if (test4Name == samp->getTest()) {
                total.test4Count++;
                diseaseTotal[diseaseIndex].test4Count++;
                facilityStats[diseaseIndex][count].test4Count++;
            }
            //std::cout << "tests names = " << test1Name << "," << test2Name << "," << test3Name << "," << test4Name << ", sample test name = " << samp->getTest() << "\n";
            //if (samp->getTest() == "") std::cout << "tests status = " << status << "\n";
        }

        // loop through each disease in model run and update statistics
        for (diseaseIndex = 0; diseaseIndex < diseaseCount; diseaseIndex++) {

            // update the sample count for each disease for this facility
            facilityStats[diseaseIndex][count].countS = sampleCount[diseaseIndex];


            // output sample stats only to a file
            if (fileFlag) {

                myfile << facilityStats[diseaseIndex][count].name << "," << facilityStats[diseaseIndex][count].countS << "," << diseaseList[diseaseIndex] << "," << facilityStats[diseaseIndex][count].countSN;
                myfile << "," << facilityStats[diseaseIndex][count].countSB << "," << facilityStats[diseaseIndex][count].countST << "," << facilityStats[diseaseIndex][count].countSW << "," << facilityStats[diseaseIndex][count].countSL;
                myfile << "," << facilityStats[diseaseIndex][count].countSRT << "," << facilityStats[diseaseIndex][count].countSR << "," << facilityStats[diseaseIndex][count].countSRL << "," << facilityStats[diseaseIndex][count].countSO;
                if (facilityStats[diseaseIndex][count].countSR < 1) {
                    myfile << "," << "0";
                }
                else {
                    myfile << "," << facilityStats[diseaseIndex][count].sampleTurnAroundTime/facilityStats[diseaseIndex][count].countSR;
                }
                myfile << "," << test1Name << "," << facilityStats[diseaseIndex][count].test1Count << "," <<  test2Name << "," << facilityStats[diseaseIndex][count].test2Count;
                myfile << "," << test3Name << "," << facilityStats[diseaseIndex][count].test3Count << "," <<  test4Name << "," << facilityStats[diseaseIndex][count].test4Count <<  "\n";

                // output to facilities file
                myfile2 << facilityStats[diseaseIndex][count].name << "," << facilityStats[diseaseIndex][count].countS << "," << diseaseList[diseaseIndex] << "," << facilityStats[diseaseIndex][count].countSN;
                myfile2 << "," << facilityStats[diseaseIndex][count].countSB << "," << facilityStats[diseaseIndex][count].countST << "," << facilityStats[diseaseIndex][count].countSW << "," << facilityStats[diseaseIndex][count].countSL;
                myfile2 << "," << facilityStats[diseaseIndex][count].countSRT << "," << facilityStats[diseaseIndex][count].countSR << "," << facilityStats[diseaseIndex][count].countSRL << "," << facilityStats[diseaseIndex][count].countSO;
                if (facilityStats[diseaseIndex][count].countSR < 1) {
                    myfile2 << "," << "0";
                }
                else {
                    myfile2 << "," << facilityStats[diseaseIndex][count].sampleTurnAroundTime/facilityStats[diseaseIndex][count].countSR;
                }
                myfile2 << "," << test1Name << "," << facilityStats[diseaseIndex][count].test1Count << "," <<  test2Name << "," << facilityStats[diseaseIndex][count].test2Count;
                myfile2 << "," << test3Name << "," << facilityStats[diseaseIndex][count].test3Count << "," <<  test4Name << "," << facilityStats[diseaseIndex][count].test4Count <<  "\n";
            }

            // now update stats for regions - only compile if output is to file
            if (fileFlag) {
                regionNotFound = true;

                for (int i = 0; i < regionCount[diseaseIndex]; i++) {

                    if (region == regions[diseaseIndex][i].name) {
                        if (debug) std::cout << "stats: samples found region = " << region << "\n";

                        //update statistics for that region
                        regionNotFound = false;

                        regions[diseaseIndex][i].countS = regions[diseaseIndex][i].countS + facilityStats[diseaseIndex][count].countS;
                        regions[diseaseIndex][i].countSN = regions[diseaseIndex][i].countSN + facilityStats[diseaseIndex][count].countSN;
                        regions[diseaseIndex][i].countSB = regions[diseaseIndex][i].countSB + facilityStats[diseaseIndex][count].countSB;
                        regions[diseaseIndex][i].countST = regions[diseaseIndex][i].countST + facilityStats[diseaseIndex][count].countST;
                        regions[diseaseIndex][i].countSW = regions[diseaseIndex][i].countSW + facilityStats[diseaseIndex][count].countSW;
                        regions[diseaseIndex][i].countSL = regions[diseaseIndex][i].countSL + facilityStats[diseaseIndex][count].countSL;
                        regions[diseaseIndex][i].countSRT = regions[diseaseIndex][i].countSRT + facilityStats[diseaseIndex][count].countSRT;
                        regions[diseaseIndex][i].countSR = regions[diseaseIndex][i].countSR + facilityStats[diseaseIndex][count].countSR;
                        regions[diseaseIndex][i].countSRL = regions[diseaseIndex][i].countSRL + facilityStats[diseaseIndex][count].countSRL;
                        regions[diseaseIndex][i].countSO = regions[diseaseIndex][i].countSO + facilityStats[diseaseIndex][count].countSO;
                        regions[diseaseIndex][i].test1Count = regions[diseaseIndex][i].test1Count + facilityStats[diseaseIndex][count].test1Count;
                        regions[diseaseIndex][i].test2Count = regions[diseaseIndex][i].test2Count + facilityStats[diseaseIndex][count].test2Count;
                        regions[diseaseIndex][i].test3Count = regions[diseaseIndex][i].test3Count + facilityStats[diseaseIndex][count].test3Count;
                        regions[diseaseIndex][i].test4Count = regions[diseaseIndex][i].test4Count + facilityStats[diseaseIndex][count].test4Count;
                        regions[diseaseIndex][i].sampleTurnAroundTime = regions[diseaseIndex][i].sampleTurnAroundTime + facilityStats[diseaseIndex][count].sampleTurnAroundTime;

                    }
                }
                if (regionNotFound) {
                    if (debug) std::cout << "stats: samples didn't find region = " << region << "\n";

                    // add it to the region array
                    regions[diseaseIndex][regionCount[diseaseIndex]].name = region;
                    regions[diseaseIndex][regionCount[diseaseIndex]].countS = facilityStats[diseaseIndex][count].countS;
                    regions[diseaseIndex][regionCount[diseaseIndex]].countSN = facilityStats[diseaseIndex][count].countSN;
                    regions[diseaseIndex][regionCount[diseaseIndex]].countSB = facilityStats[diseaseIndex][count].countSB;
                    regions[diseaseIndex][regionCount[diseaseIndex]].countST = facilityStats[diseaseIndex][count].countST;
                    regions[diseaseIndex][regionCount[diseaseIndex]].countSW = facilityStats[diseaseIndex][count].countSW;
                    regions[diseaseIndex][regionCount[diseaseIndex]].countSL = facilityStats[diseaseIndex][count].countSL;
                    regions[diseaseIndex][regionCount[diseaseIndex]].countSRT = facilityStats[diseaseIndex][count].countSRT;
                    regions[diseaseIndex][regionCount[diseaseIndex]].countSR = facilityStats[diseaseIndex][count].countSR;
                    regions[diseaseIndex][regionCount[diseaseIndex]].countSRL = facilityStats[diseaseIndex][count].countSRL;
                    regions[diseaseIndex][regionCount[diseaseIndex]].countSO = facilityStats[diseaseIndex][count].countSO;
                    regions[diseaseIndex][regionCount[diseaseIndex]].test1Count = facilityStats[diseaseIndex][count].test1Count;
                    regions[diseaseIndex][regionCount[diseaseIndex]].test2Count = facilityStats[diseaseIndex][count].test2Count;
                    regions[diseaseIndex][regionCount[diseaseIndex]].test3Count = facilityStats[diseaseIndex][count].test3Count;
                    regions[diseaseIndex][regionCount[diseaseIndex]].test4Count = facilityStats[diseaseIndex][count].test4Count;
                    regions[diseaseIndex][regionCount[diseaseIndex]].sampleTurnAroundTime = facilityStats[diseaseIndex][count].sampleTurnAroundTime;

                    regionCount[diseaseIndex] = regionCount[diseaseIndex] + 1;

                }

                // now update stats for districts
                districtNotFound = true;
                for (int i = 0; i < districtCount[diseaseIndex]; i++) {

                    if (district == districts[diseaseIndex][i].name) {
                        if (debug) std::cout << "stats: samples found district = " << district << "\n";

                        //update statistics for that region
                        districtNotFound = false;

                        districts[diseaseIndex][i].countS = districts[diseaseIndex][i].countS + facilityStats[diseaseIndex][count].countS;
                        districts[diseaseIndex][i].countSN = districts[diseaseIndex][i].countSN + facilityStats[diseaseIndex][count].countSN;
                        districts[diseaseIndex][i].countSB = districts[diseaseIndex][i].countSB + facilityStats[diseaseIndex][count].countSB;
                        districts[diseaseIndex][i].countST = districts[diseaseIndex][i].countST + facilityStats[diseaseIndex][count].countST;
                        districts[diseaseIndex][i].countSW = districts[diseaseIndex][i].countSW + facilityStats[diseaseIndex][count].countSW;
                        districts[diseaseIndex][i].countSL = districts[diseaseIndex][i].countSL + facilityStats[diseaseIndex][count].countSL;
                        districts[diseaseIndex][i].countSRT = districts[diseaseIndex][i].countSRT + facilityStats[diseaseIndex][count].countSRT;
                        districts[diseaseIndex][i].countSR = districts[diseaseIndex][i].countSR + facilityStats[diseaseIndex][count].countSR;
                        districts[diseaseIndex][i].countSRL = districts[diseaseIndex][i].countSRL + facilityStats[diseaseIndex][count].countSRL;
                        districts[diseaseIndex][i].countSO = districts[diseaseIndex][i].countSO + facilityStats[diseaseIndex][count].countSO;
                        districts[diseaseIndex][i].test1Count = districts[diseaseIndex][i].test1Count + facilityStats[diseaseIndex][count].test1Count;
                        districts[diseaseIndex][i].test2Count = districts[diseaseIndex][i].test2Count + facilityStats[diseaseIndex][count].test2Count;
                        districts[diseaseIndex][i].test3Count = districts[diseaseIndex][i].test3Count + facilityStats[diseaseIndex][count].test3Count;
                        districts[diseaseIndex][i].test4Count = districts[diseaseIndex][i].test4Count + facilityStats[diseaseIndex][count].test4Count;
                        districts[diseaseIndex][i].sampleTurnAroundTime = districts[diseaseIndex][i].sampleTurnAroundTime + facilityStats[diseaseIndex][count].sampleTurnAroundTime;

                    }
                }
                if (districtNotFound) {
                    if (debug) std::cout << "stats: samples didn't find district = " << district << ", districtCount = " << districtCount[diseaseCount] << "\n";

                    // add it to the district array
                    districts[diseaseIndex][districtCount[diseaseIndex]].name = district;
                    districts[diseaseIndex][districtCount[diseaseIndex]].countS = facilityStats[diseaseIndex][count].countS;
                    districts[diseaseIndex][districtCount[diseaseIndex]].countSN = facilityStats[diseaseIndex][count].countSN;
                    districts[diseaseIndex][districtCount[diseaseIndex]].countSB = facilityStats[diseaseIndex][count].countSB;
                    districts[diseaseIndex][districtCount[diseaseIndex]].countST = facilityStats[diseaseIndex][count].countST;
                    districts[diseaseIndex][districtCount[diseaseIndex]].countSW = facilityStats[diseaseIndex][count].countSW;
                    districts[diseaseIndex][districtCount[diseaseIndex]].countSL = facilityStats[diseaseIndex][count].countSL;
                    districts[diseaseIndex][districtCount[diseaseIndex]].countSRT = facilityStats[diseaseIndex][count].countSRT;
                    districts[diseaseIndex][districtCount[diseaseIndex]].countSR = facilityStats[diseaseIndex][count].countSR;
                    districts[diseaseIndex][districtCount[diseaseIndex]].countSRL = facilityStats[diseaseIndex][count].countSRL;
                    districts[diseaseIndex][districtCount[diseaseIndex]].countSO = facilityStats[diseaseIndex][count].countSO;
                    districts[diseaseIndex][districtCount[diseaseIndex]].test1Count = facilityStats[diseaseIndex][count].test1Count;
                    districts[diseaseIndex][districtCount[diseaseIndex]].test2Count = facilityStats[diseaseIndex][count].test2Count;
                    districts[diseaseIndex][districtCount[diseaseIndex]].test3Count = facilityStats[diseaseIndex][count].test3Count;
                    districts[diseaseIndex][districtCount[diseaseIndex]].test4Count = facilityStats[diseaseIndex][count].test4Count;
                    districts[diseaseIndex][districtCount[diseaseIndex]].sampleTurnAroundTime = facilityStats[diseaseIndex][count].sampleTurnAroundTime;

                    districtCount[diseaseIndex] = districtCount[diseaseIndex] + 1;

                }
            }
        }
        count = count + 1;
    }

    // dump statistics for each region and district to a file
    if (fileFlag) {
        if (debug) std::cout << "stats: samples region  " << "\n";
        myfile << "-----------------------------------------------------------------------------------  " << "\n";

        for (int w = 0; w < diseaseCount; w++) {

            if (debug) std::cout << "stats: samples region count = " << regionCount[w] << ", disease = " << diseaseList[w] << "\n";

            myfile << "region" << ",samples" << ",disease" << ",New" << ",New.in.batch" <<",In.Transit" << ",Wait" << ",Lost" << ",Returned-transit" << ",Returned" << ",Re/Lost" << ",Other";
            myfile << ",avg.return.time" << ",test1.name" << ",test1.count" << ",test2.name" << ",test2.count" << ",test3.name" << ",test3.count" << ",test4.name" << ",test4.count" << "\n";

            for (int i = 0; i < regionCount[w]; i++) {

                myfile << regions[w][i].name << "," << regions[w][i].countS << "," << diseaseList[w] << "," << regions[w][i].countSN << "," << regions[w][i].countSB << "," << regions[w][i].countST << "," << regions[w][i].countSW;
                myfile << "," << regions[w][i].countSL << "," << regions[w][i].countSRT << "," << regions[w][i].countSR << "," << regions[w][i].countSRL << "," << regions[w][i].countSO;
                if (regions[w][i].countSR < 1) {
                    myfile << "," << "0";
                }
                else {
                    myfile << "," << regions[w][i].sampleTurnAroundTime/regions[w][i].countSR;
                }
                myfile << "," << test1Name << "," << regions[w][i].test1Count << "," << test2Name << "," << regions[w][i].test2Count;
                myfile << "," << test3Name << "," << regions[w][i].test3Count << "," << test4Name << "," << regions[w][i].test4Count << "\n";

                // output to regions file
                myfile4 << regions[w][i].name << "," << regions[w][i].countS << "," << diseaseList[w] << "," << regions[w][i].countSN << "," << regions[w][i].countSB << "," << regions[w][i].countST << "," << regions[w][i].countSW;
                myfile4 << "," << regions[w][i].countSL << "," << regions[w][i].countSRT << "," << regions[w][i].countSR << "," << regions[w][i].countSRL << "," << regions[w][i].countSO;
                if (regions[w][i].countSR < 1) {
                    myfile4 << "," << "0";
                }
                else {
                    myfile4 << "," << regions[w][i].sampleTurnAroundTime/regions[w][i].countSR;
                }
                myfile4 << "," << test1Name << "," << regions[w][i].test1Count << "," << test2Name << "," << regions[w][i].test2Count;
                myfile4 << "," << test3Name << "," << regions[w][i].test3Count << "," << test4Name << "," << regions[w][i].test4Count << "\n";


                diseaseTotalRegion[w].countS = diseaseTotalRegion[w].countS + regions[w][i].countS;
                diseaseTotalRegion[w].countSN = diseaseTotalRegion[w].countSN + regions[w][i].countSN;
                diseaseTotalRegion[w].countSB = diseaseTotalRegion[w].countSB + regions[w][i].countSB;
                diseaseTotalRegion[w].countST = diseaseTotalRegion[w].countST + regions[w][i].countST;
                diseaseTotalRegion[w].countSW = diseaseTotalRegion[w].countSW + regions[w][i].countSW;
                diseaseTotalRegion[w].countSL = diseaseTotalRegion[w].countSL + regions[w][i].countSL;
                diseaseTotalRegion[w].countSRT = diseaseTotalRegion[w].countSRT + regions[w][i].countSRT;
                diseaseTotalRegion[w].countSR = diseaseTotalRegion[w].countSR + regions[w][i].countSR;
                diseaseTotalRegion[w].countSRL = diseaseTotalRegion[w].countSRL + regions[w][i].countSRL;
                diseaseTotalRegion[w].countSO = diseaseTotalRegion[w].countSO + regions[w][i].countSO;
                diseaseTotalRegion[w].test1Count = diseaseTotalRegion[w].test1Count + regions[w][i].test1Count;
                diseaseTotalRegion[w].test2Count = diseaseTotalRegion[w].test2Count + regions[w][i].test2Count;
                diseaseTotalRegion[w].test3Count = diseaseTotalRegion[w].test3Count + regions[w][i].test3Count;
                diseaseTotalRegion[w].test4Count = diseaseTotalRegion[w].test4Count + regions[w][i].test4Count;
                diseaseTotalRegion[w].sampleTurnAroundTime = diseaseTotalRegion[w].sampleTurnAroundTime + regions[w][i].sampleTurnAroundTime;
                totalRegion.countS = totalRegion.countS + regions[w][i].countS;
                totalRegion.countSN = totalRegion.countSN + regions[w][i].countSN;
                totalRegion.countSB = totalRegion.countSB + regions[w][i].countSB;
                totalRegion.countST = totalRegion.countST + regions[w][i].countST;
                totalRegion.countSW = totalRegion.countSW + regions[w][i].countSW;
                totalRegion.countSL = totalRegion.countSL + regions[w][i].countSL;
                totalRegion.countSRT = totalRegion.countSRT + regions[w][i].countSRT;
                totalRegion.countSR = totalRegion.countSR + regions[w][i].countSR;
                totalRegion.countSRL = totalRegion.countSRL + regions[w][i].countSRL;
                totalRegion.countSO = totalRegion.countSO + regions[w][i].countSO;
                totalRegion.test1Count = totalRegion.test1Count + regions[w][i].test1Count;
                totalRegion.test2Count = totalRegion.test2Count + regions[w][i].test2Count;
                totalRegion.test3Count = totalRegion.test3Count + regions[w][i].test3Count;
                totalRegion.test4Count = totalRegion.test4Count + regions[w][i].test4Count;
                totalRegion.sampleTurnAroundTime = totalRegion.sampleTurnAroundTime + regions[w][i].sampleTurnAroundTime;
            }

            // update total sample count
            diseaseTotal[w].countS = diseaseTotalRegion[w].countS;

            myfile << "-----------------------------------------------------------------------------------  " << "\n";
            myfile << "regions" << ",Samples" << ",Disease" << ",New" << ",New.in.batch" << ",In.Transit" << ",Wait" << ",Lost" << ",Returned-transit" << ",Returned" << ",Re/Lost" << ",Other";
            myfile << ",avg.return.time" << ",test1.name" << ",test1.count" << ",test2.name" << ",test2.count" << ",test3.name" << ",test3.count" << ",test4.name" << ",test4.count" << "\n";

            myfile << "Totals," << diseaseTotalRegion[w].countS << "," << diseaseList[w] << "," << diseaseTotalRegion[w].countSN << "," << diseaseTotalRegion[w].countSB << "," << diseaseTotalRegion[w].countST << "," << diseaseTotalRegion[w].countSW;
            myfile << "," << diseaseTotalRegion[w].countSL << "," << diseaseTotalRegion[w].countSRT << "," << diseaseTotalRegion[w].countSR << "," << diseaseTotalRegion[w].countSRL << "," << diseaseTotalRegion[w].countSO;
            if (diseaseTotalRegion[w].countSR  < 1) {
                myfile << "," <<  "0";
            }
            else {
                myfile << "," << diseaseTotalRegion[w].sampleTurnAroundTime/diseaseTotalRegion[w].countSR;
            }

            myfile << "," << test1Name << "," << diseaseTotalRegion[w].test1Count << "," << test2Name << "," << diseaseTotalRegion[w].test2Count;
            myfile << "," << test3Name << "," << diseaseTotalRegion[w].test3Count << "," << test4Name << "," << diseaseTotalRegion[w].test4Count << "\n";

            myfile << "-----------------------------------------------------------------------------------  " << "\n";
        }

        myfile << "-----------------------------------------------------------------------------------  " << "\n";
        myfile << "regions" << ",Samples" << "," << ",New" << ",New.in.batch" << ",In.Transit" << ",Wait" << ",Lost" << ",Returned-transit" << ",Returned" << ",Re/Lost" << ",Other";
        myfile << ",avg.return.time" << ",test1.name" << ",test1.count" << ",test2.name" << ",test2.count" << ",test3.name" << ",test3.count" << ",test4.name" << ",test4.count" << "\n";

        myfile << "Totals," << totalRegion.countS << "," << "," << totalRegion.countSN  << "," << totalRegion.countSB << ", " << totalRegion.countST << "," << totalRegion.countSW << "," << totalRegion.countSL << "," << totalRegion.countSRT;
        myfile << "," << totalRegion.countSR << "," << totalRegion.countSRL << "," << totalRegion.countSO;
        if (totalRegion.countSR  < 1) {
            myfile << "," <<  "0";
        }
        else {
            myfile << "," << totalRegion.sampleTurnAroundTime/totalRegion.countSR;
        }
        myfile << "," << test1Name << "," << totalRegion.test1Count << "," << test2Name << "," << totalRegion.test2Count;
        myfile << "," << test3Name << "," << totalRegion.test3Count << "," << test4Name << "," << totalRegion.test4Count << "\n";

        myfile << "-----------------------------------------------------------------------------------  " << "\n";

        for (int w = 0; w < diseaseCount; w++) {

            if (debug) std::cout << "stats: samples  district count = " << districtCount[w] << ", disease = " << diseaseList[w] << "\n";

            myfile << "stats.district" << ",samples" << ",disease" << ",New" << ",New.in.batch" << ",In.Transit" << ",Wait" <<  ",Lost" <<  ",Returned-transit" << ",Returned" << ",Re/Lost" << ",Other";
            myfile << ",avg.return.time" << ",test1.name" << ",test1.count" << ",test2.name" << ",test2.count" << ",test3.name" << ",test3.count" << ",test4.name" << ",test4.count" << "\n";

            for (int i = 0; i < districtCount[w]; i++) {

                myfile << districts[w][i].name << "," << districts[w][i].countS << "," << diseaseList[w] << "," << districts[w][i].countSN << "," << districts[w][i].countSB << "," << districts[w][i].countST << "," << districts[w][i].countSW;
                myfile << "," << districts[w][i].countSL << "," << districts[w][i].countSRT << "," << districts[w][i].countSR << "," << districts[w][i].countSRL << "," << districts[w][i].countSO;
                if (districts[w][i].countSR  < 1) {
                    myfile << "," <<  "0";
                }
                else {
                    myfile << "," << districts[w][i].sampleTurnAroundTime/districts[w][i].countSR;
                }
                myfile << "," << test1Name << "," << districts[w][i].test1Count << "," << test2Name << "," << districts[w][i].test2Count;
                myfile << "," << test3Name << "," << districts[w][i].test3Count << "," << test4Name << "," << districts[w][i].test4Count << "\n";

                // output to district file
                myfile3 << districts[w][i].name << "," << districts[w][i].countS << "," << diseaseList[w] << "," << districts[w][i].countSN << "," << districts[w][i].countSB << "," << districts[w][i].countST << "," << districts[w][i].countSW;
                myfile3 << "," << districts[w][i].countSL << "," << districts[w][i].countSRT << "," << districts[w][i].countSR << "," << districts[w][i].countSRL << "," << districts[w][i].countSO;
                if (districts[w][i].countSR  < 1) {
                    myfile3 << "," <<  "0";
                }
                else {
                    myfile3 << "," << districts[w][i].sampleTurnAroundTime/districts[w][i].countSR;
                }
                myfile3 << "," << test1Name << "," << districts[w][i].test1Count << "," << test2Name << "," << districts[w][i].test2Count;
                myfile3 << "," << test3Name << "," << districts[w][i].test3Count << "," << test4Name << "," << districts[w][i].test4Count << "\n";


                diseaseTotalDistrict[w].countS = diseaseTotalDistrict[w].countS + districts[w][i].countS;
                diseaseTotalDistrict[w].countSN = diseaseTotalDistrict[w].countSN + districts[w][i].countSN;
                diseaseTotalDistrict[w].countSB = diseaseTotalDistrict[w].countSB + districts[w][i].countSB;
                diseaseTotalDistrict[w].countST = diseaseTotalDistrict[w].countST + districts[w][i].countST;
                diseaseTotalDistrict[w].countSW = diseaseTotalDistrict[w].countSW + districts[w][i].countSW;
                diseaseTotalDistrict[w].countSL = diseaseTotalDistrict[w].countSL + districts[w][i].countSL;
                diseaseTotalDistrict[w].countSRT = diseaseTotalDistrict[w].countSRT + districts[w][i].countSRT;
                diseaseTotalDistrict[w].countSR = diseaseTotalDistrict[w].countSR + districts[w][i].countSR;
                diseaseTotalDistrict[w].countSRL = diseaseTotalDistrict[w].countSRL + districts[w][i].countSRL;
                diseaseTotalDistrict[w].countSO = diseaseTotalDistrict[w].countSO + districts[w][i].countSO;
                diseaseTotalDistrict[w].test1Count = diseaseTotalDistrict[w].test1Count + districts[w][i].test1Count;
                diseaseTotalDistrict[w].test2Count = diseaseTotalDistrict[w].test2Count + districts[w][i].test2Count;
                diseaseTotalDistrict[w].test3Count = diseaseTotalDistrict[w].test3Count + districts[w][i].test3Count;
                diseaseTotalDistrict[w].test4Count = diseaseTotalDistrict[w].test4Count + districts[w][i].test4Count;
                diseaseTotalDistrict[w].sampleTurnAroundTime = diseaseTotalDistrict[w].sampleTurnAroundTime + districts[w][i].sampleTurnAroundTime;
                totalDistrict.countS = totalDistrict.countS + districts[w][i].countS;
                totalDistrict.countSN = totalDistrict.countSN + districts[w][i].countSN;
                totalDistrict.countSB = totalDistrict.countSB + districts[w][i].countSB;
                totalDistrict.countST = totalDistrict.countST + districts[w][i].countST;
                totalDistrict.countSW = totalDistrict.countSW + districts[w][i].countSW;
                totalDistrict.countSL = totalDistrict.countSL + districts[w][i].countSL;
                totalDistrict.countSRT = totalDistrict.countSRT + districts[w][i].countSRT;
                totalDistrict.countSR = totalDistrict.countSR + districts[w][i].countSR;
                totalDistrict.countSRL = totalDistrict.countSRL + districts[w][i].countSRL;
                totalDistrict.countSO = totalDistrict.countSO + districts[w][i].countSO;
                totalDistrict.test1Count = totalDistrict.test1Count + districts[w][i].test1Count;
                totalDistrict.test2Count = totalDistrict.test2Count + districts[w][i].test2Count;
                totalDistrict.test3Count = totalDistrict.test3Count + districts[w][i].test3Count;
                totalDistrict.test4Count = totalDistrict.test4Count + districts[w][i].test4Count;
                totalDistrict.sampleTurnAroundTime = totalDistrict.sampleTurnAroundTime + districts[w][i].sampleTurnAroundTime;
            }

            myfile << "-----------------------------------------------------------------------------------  " << "\n";
            myfile << "Districts" << ",Samples" << ",Disease" << ",New" << ",New.in.batch" <<  ",In.Transit" <<  ",Wait" << ",Lost" << ",Returned-transit" << ",Returned" << ",Re/Lost" << ",Other";
            myfile << ",avg.return.time" << ",test1.name" << ",test1.count" << ",test2.name" << ",test2.count" << ",test3.name" << ",test3.count" << ",test4.name" << ",test4.count" << "\n";

            myfile << "Totals," << diseaseTotalDistrict[w].countS << "," << diseaseList[w] << "," << diseaseTotalDistrict[w].countSN << "," << diseaseTotalDistrict[w].countSB << "," << diseaseTotalDistrict[w].countST << "," << diseaseTotalDistrict[w].countSW;
            myfile << "," << diseaseTotalDistrict[w].countSL << "," << diseaseTotalDistrict[w].countSRT << "," << diseaseTotalDistrict[w].countSR << "," << diseaseTotalDistrict[w].countSRL << "," << diseaseTotalDistrict[w].countSO;
            if (diseaseTotalDistrict[w].countSR  < 1) {
                myfile << "," <<  "0";
            }
            else {
                myfile << "," << diseaseTotalDistrict[w].sampleTurnAroundTime/diseaseTotalDistrict[w].countSR;
            }
            myfile << "," << test1Name << "," << diseaseTotalDistrict[w].test1Count << "," << test2Name << "," << diseaseTotalDistrict[w].test2Count;
            myfile << "," << test3Name << "," << diseaseTotalDistrict[w].test3Count << "," << test4Name << "," << diseaseTotalDistrict[w].test4Count << "\n";


            myfile << "-----------------------------------------------------------------------------------  " << "\n";
        }

        myfile << "-----------------------------------------------------------------------------------  " << "\n";
        myfile << "Districts" << ",Samples" << "," << ",New" << ",New.in.batch" << ",In.Transit" << ",Wait" << ",Lost" << ",Returned-transit" << ",Returned" << ",Re/Lost" << ",Other";
        myfile << ",avg.return.time" << ",test1.name" << ",test1.count" << ",test2.name" << ",test2.count" << ",test3.name" << ",test3.count" << ",test4.name" << ",test4.count" << "\n";
        
        myfile << "Totals," << totalDistrict.countS << "," << "," << totalDistrict.countSN << "," << totalDistrict.countSB << "," << totalDistrict.countST << "," << totalDistrict.countSW << "," << totalDistrict.countSL << "," << totalDistrict.countSRT;
        myfile << "," << totalDistrict.countSR << "," << totalDistrict.countSRL << "," << totalDistrict.countSO;
        if (totalDistrict.countSR  < 1) {
            myfile << "," <<  "0";
        }
        else {
            myfile << "," << totalDistrict.sampleTurnAroundTime/totalDistrict.countSR;
        }
        myfile << "," << test1Name << "," << totalDistrict.test1Count << "," << test2Name << "," << totalDistrict.test2Count;
        myfile << "," << test3Name << "," << totalDistrict.test3Count << "," << test4Name << "," << totalDistrict.test4Count << "\n";

        myfile << "-----------------------------------------------------------------------------------  " << "\n";
    }


    if (fileFlag) {

        for (int w = 0; w < diseaseCount; w++) {

            myfile << "facility.count" << ",samples" << ",disease" << ",New" << ",New.in.batch" << ",In.Transit" << ",Wait" << ",Lost" << ",Returned-transit" << ",Returned" << ",Re/Lost" << ",Other";
            myfile << ",avg.return.time" << ",test1.name" << ",test1.count" << ",test2.name" << ",test2.count" << ",test3.name" << ",test3.count" << ",test4.name" << ",test4.count" << "\n";
        
            myfile << countF << "," << diseaseTotal[w].countS << "," << diseaseList[w] << "," << diseaseTotal[w].countSN << "," << diseaseTotal[w].countSB << "," << diseaseTotal[w].countST << "," << diseaseTotal[w].countSW << "," << diseaseTotal[w].countSL << "," << diseaseTotal[w].countSRT;
            myfile << "," << diseaseTotal[w].countSR << "," << diseaseTotal[w].countSRL << "," << diseaseTotal[w].countSO;
            if (diseaseTotal[w].countSR  < 1) {
                myfile << "," <<  "0";
            }
            else {
                myfile << "," << diseaseTotal[w].sampleTurnAroundTime/diseaseTotal[w].countSR;
            }
            myfile << "," << test1Name << "," << diseaseTotal[w].test1Count << "," << test2Name << "," << diseaseTotal[w].test2Count;
            myfile << "," << test3Name << "," << diseaseTotal[w].test3Count << "," << test4Name << "," << diseaseTotal[w].test4Count << "\n";

            myfile << "-----------------------------------------------------------------------------------  " << "\n";
        }


        myfile << "facility.count" << ",samples" << "," << ",New" << ",New.in.batch" << ",In.Transit" << ",Wait" << ",Lost" << ",Returned-transit" << ",Returned" << ",Re/Lost" << ",Other";
        myfile << ",avg.return.time" << ",test1.name" << ",test1.count" << ",test2.name" << ",test2.count" << ",test3.name" << ",test3.count" << ",test4.name" << ",test4.count" << "\n";
        
        myfile << countF << "," << total.countS << "," << "," << total.countSN << "," << total.countSB << "," << total.countST << "," << total.countSW << "," << total.countSL << "," << total.countSRT;
        myfile << "," << total.countSR << "," << total.countSRL << "," << total.countSO;
        if (total.countSR  < 1) {
            myfile << "," <<  "0";
        }
        else {
            myfile << "," << total.sampleTurnAroundTime/total.countSR;
        }
        myfile << "," << test1Name << "," << total.test1Count << "," << test2Name << "," << total.test2Count;
        myfile << "," << test3Name << "," << total.test3Count << "," << test4Name << "," << total.test4Count << "\n";


        myfile.close();
        myfile1.close();
    }
    else {
        std::cout << "stats: fac =  " << countF << ", samples = " << total.countS << "\n";
        std::cout << "stats: sample" << "  New = " << total.countSN << "  New waiting in batch = " << total.countSB << " In Transit =  " << total.countST << ", Wait = " << total.countSW << ", Lost = " << total.countSL << ", Returned -transit = " << total.countSRT;
        std::cout << ", Returned = " << total.countSR << ", Re/Lost = " << total.countSRL << ", Other = " << total.countSO << ",avg. return time = " << total.sampleTurnAroundTime/total.countSR << "," << test1Name << " = " << total.test1Count;
        std::cout << "," << test2Name << " = " << total.test2Count <<"," << test3Name << " = " << total.test3Count <<"," << test4Name << " = " << total.test4Count << "\n";
    
    }
}


// count how many Samples currently in existence 
void sampleInactiveStats(FacilitiesList* list, vector<string> diseaseList, bool fileFlag, bool yearly, int timeStep) {

    Diagnosis* d;
    Testing* t;
    vector<Patient*> p;
    vector<Sample*> s;
    ofstream myfile;
    ofstream myfile1;

    int countF = 0;
    int facilityCountS = 0;
    int count = 0;
    int status;
    int diseaseCount;
    int diseaseIndex;
    int errNo;
    string diseaseName;
    string subDirBase;
    string subDir;
    string subDirYear;
    string test1Name;
    string test2Name;
    string test3Name;
    string test4Name;
    bool debug = false;

    // keep patient statistics by region and district
    struct geographicalStats {
        string name;
        string name2;
        int countS = 0;
        int countSN = 0;
        int countSB = 0;
        int countST = 0;
        int countSW = 0;
        int countSL = 0;
        int countSRT = 0;
        float countSR = 0;
        int countSRL = 0;
        int countSL2F = 0;
        int countSO = 0;
        int test1Count = 0;
        int test2Count = 0;
        int test3Count = 0;
        int test4Count = 0;
        float sampleReturnTime = 0;
    };

    if (debug) std::cout << "stats: inside inactive sample " << "\n";

    // find out how many diseases to save stats for
    // use this as an count to create arrays for each of the diseases being run
    diseaseCount = diseaseList.size();

    geographicalStats total;
    geographicalStats totalRegion;
    geographicalStats totalDistrict;
    geographicalStats diseaseTotal[diseaseCount];
    geographicalStats diseaseTotalRegion[diseaseCount];
    geographicalStats diseaseTotalDistrict[diseaseCount];

    // keep sample statistics by region and district
    geographicalStats regions[diseaseCount][25];
    geographicalStats districts[diseaseCount][270];

    int regionCount[diseaseCount];
    int districtCount[diseaseCount];
    int sampleCount[diseaseCount];
    string region;
    string district;
    bool regionNotFound;
    bool districtNotFound;

    for (int i = 0; i < diseaseCount; i++) {

        regionCount[i] = 0;
        districtCount[i] = 0;
    }

    vector<Facility*> facilities = list->getListPtr();

    countF = facilities.size();

    // get statistical array for each facility
    geographicalStats facilityStats[diseaseCount][countF];

    // initialize all region data to 0
    for (int w = 0; w < diseaseCount; w++) {

        for (int i = 0; i < 25; i++) {

            regions[w][i].name = "";
            regions[w][i].name2 = "";
            regions[w][i].countS = 0;
            regions[w][i].countSN = 0;
            regions[w][i].countSB = 0;
            regions[w][i].countST = 0;
            regions[w][i].countSW = 0;
            regions[w][i].countSL = 0;
            regions[w][i].countSRT = 0;
            regions[w][i].countSR = 0;
            regions[w][i].countSRL = 0;
            regions[w][i].countSL2F = 0;
            regions[w][i].countSO = 0;
            regions[w][i].test1Count = 0;
            regions[w][i].test2Count = 0;
            regions[w][i].test3Count = 0;
            regions[w][i].test4Count = 0;
            regions[w][i].sampleReturnTime = 0;

        }


        // initialize all district data to 0
        for (int i = 0; i < 270; i++) {

            districts[w][i].name = "";
            districts[w][i].name2 = "";
            districts[w][i].countS = 0;
            districts[w][i].countSN = 0;
            districts[w][i].countSB = 0;
            districts[w][i].countST = 0;
            districts[w][i].countSW = 0;
            districts[w][i].countSL = 0;
            districts[w][i].countSRT = 0;
            districts[w][i].countSR = 0;
            districts[w][i].countSRL = 0;
            districts[w][i].countSL2F = 0;
            districts[w][i].countSO = 0;
            districts[w][i].test1Count = 0;
            districts[w][i].test2Count = 0;
            districts[w][i].test3Count = 0;
            districts[w][i].test4Count = 0;
            districts[w][i].sampleReturnTime = 0;

        }

        // reset region and district totals
        diseaseTotal[w].countS = 0;
        diseaseTotal[w].countSN = 0;
        diseaseTotal[w].countSB = 0;
        diseaseTotal[w].countST = 0;
        diseaseTotal[w].countSW = 0;
        diseaseTotal[w].countSL = 0;
        diseaseTotal[w].countSRT = 0;
        diseaseTotal[w].countSR = 0;
        diseaseTotal[w].countSRL = 0;
        diseaseTotal[w].countSL2F = 0;
        diseaseTotal[w].countSO = 0;
        diseaseTotal[w].test1Count = 0;
        diseaseTotal[w].test2Count = 0;
        diseaseTotal[w].test3Count = 0;
        diseaseTotal[w].test4Count = 0;
        diseaseTotal[w].sampleReturnTime = 0;
        diseaseTotalRegion[w].countS = 0;
        diseaseTotalRegion[w].countSN = 0;
        diseaseTotalRegion[w].countSB = 0;
        diseaseTotalRegion[w].countST = 0;
        diseaseTotalRegion[w].countSW = 0;
        diseaseTotalRegion[w].countSL = 0;
        diseaseTotalRegion[w].countSRT = 0;
        diseaseTotalRegion[w].countSR = 0;
        diseaseTotalRegion[w].countSRL = 0;
        diseaseTotalRegion[w].countSL2F = 0;
        diseaseTotalRegion[w].countSO = 0;
        diseaseTotalRegion[w].test1Count = 0;
        diseaseTotalRegion[w].test2Count = 0;
        diseaseTotalRegion[w].test3Count = 0;
        diseaseTotalRegion[w].test4Count = 0;
        diseaseTotalRegion[w].sampleReturnTime = 0;
        diseaseTotalDistrict[w].countS = 0;
        diseaseTotalDistrict[w].countSN = 0;
        diseaseTotalDistrict[w].countSB = 0;
        diseaseTotalDistrict[w].countST = 0;
        diseaseTotalDistrict[w].countSW = 0;
        diseaseTotalDistrict[w].countSL = 0;
        diseaseTotalDistrict[w].countSRT = 0;
        diseaseTotalDistrict[w].countSR = 0;
        diseaseTotalDistrict[w].countSRL = 0;
        diseaseTotalDistrict[w].countSL2F = 0;
        diseaseTotalDistrict[w].countSO = 0;
        diseaseTotalDistrict[w].test1Count = 0;
        diseaseTotalDistrict[w].test2Count = 0;
        diseaseTotalDistrict[w].test3Count = 0;
        diseaseTotalDistrict[w].test4Count = 0;
        diseaseTotalDistrict[w].sampleReturnTime = 0;
    }

    // reset region and district totals
    totalRegion.countS = 0;
    totalRegion.countSN = 0;
    totalRegion.countSB = 0;
    totalRegion.countST = 0;
    totalRegion.countSW = 0;
    totalRegion.countSL = 0;
    totalRegion.countSRT = 0;
    totalRegion.countSR = 0;
    totalRegion.countSRL = 0;
    totalRegion.countSL2F = 0;
    totalRegion.countSO = 0;
    totalRegion.test1Count = 0;
    totalRegion.test2Count = 0;
    totalRegion.test3Count = 0;
    totalRegion.test4Count = 0;
    totalRegion.sampleReturnTime = 0;
    totalDistrict.countS = 0;
    totalDistrict.countSN = 0;
    totalDistrict.countSB = 0;
    totalDistrict.countST = 0;
    totalDistrict.countSW = 0;
    totalDistrict.countSL = 0;
    totalDistrict.countSRT = 0;
    totalDistrict.countSR = 0;
    totalDistrict.countSRL = 0;
    totalDistrict.countSL2F = 0;
    totalDistrict.countSO = 0;
    totalDistrict.test1Count = 0;
    totalDistrict.test2Count = 0;
    totalDistrict.test3Count = 0;
    totalDistrict.test4Count = 0;
    totalDistrict.sampleReturnTime = 0;

    if (debug) std::cout << "stats: inside inactive sample before file creation " <<  "\n";

    // if fileFlag is true, output to a file
    if (fileFlag) {

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

        if (yearly) {

            subDirYear = "year-" + to_string(currentYearInRun-1);
            
            subDir = subDirBase + "/" + subDirYear;
        }
        else {
            
            subDir = subDirBase + "/total";
         }
  
         // convert to char array
         char subArray4[subDir.length() +1];
         // copying the contents of the
         // string to char array
         strcpy(subArray4, subDir.c_str());
            
         // check if directory is created or not 
         if (mkdir(subArray4,0777) != 0) {
                
             // std::cout << "unable to create directory = " << subArray4 << "\n"; 
         }

        myfile.open(subDir + "/SamplesInactiveStats.csv");
        myfile1.open(subDir + "/SamplesInactiveStatsReturned.txt");

        myfile << "facility" <<  ",samples" <<  ",for.disease" << ",New"  << ",New.in.batch" << ",In.Transit" << ",Wait" << ",Lost" <<  ",Returned-transit" << ",Returned" << ",Re/Lost" << ",LtoF" << ",Other";
        myfile << ",avg.return.time" << ",test1.name" << ",test1.count" << ",test2.name" << ",test2.count" << ",test3.name" << ",test3.count" << ",test4.name" << ",test4.count" << "\n";
    }
    // loop through each facility and the number of Patients and Samples at each facility
    for (auto facility : facilities) {

        // get the up name for this facility
        d = facility->getDiagnosisPtr();
        t = facility->getTestingPtr();
        p = d->getPatientPtr();
        s = t->getInactiveSamplePtr();
        region = facility->getRegion();
        district = facility->getDistrict();

        // get sample stats
        facilityCountS = s.size();
        total.countS = total.countS + facilityCountS;

        // update facility statistics
        for (int w = 0; w < diseaseCount; w++) {
            facilityStats[w][count].name = facility->getName();
            facilityStats[w][count].countS = 0;
            facilityStats[w][count].countSN = 0;
            facilityStats[w][count].countSB = 0;
            facilityStats[w][count].countST = 0;
            facilityStats[w][count].countSW = 0;
            facilityStats[w][count].countSL = 0;
            facilityStats[w][count].countSRT = 0;
            facilityStats[w][count].countSR = 0;
            facilityStats[w][count].countSRL = 0;
            facilityStats[w][count].countSL2F = 0;
            facilityStats[w][count].countSO = 0;
            facilityStats[w][count].test1Count = 0;
            facilityStats[w][count].test2Count = 0;
            facilityStats[w][count].test3Count = 0;
            facilityStats[w][count].test4Count = 0;
            facilityStats[w][count].sampleReturnTime = 0;

            sampleCount[w] = 0;
        }

        if (fileFlag) {
            //std::cout << "stats: samples facility count =  " << count << ", facility name = " << facility->getName() << "\n";
        }

        if (debug) std::cout << "stats: inside inactive sample before sample loop name = " << facility->getName() <<  "\n";

        //get sample stats
        for (auto samp : s) {

            // store stats by disease
            diseaseName = samp->getDisease();

            // get the index for the disease
            diseaseIndex = 0;

            for (int w = 0; w < diseaseCount; w++) {

                if (diseaseName == diseaseList[w]) {
                    diseaseIndex = w;

                    // increase the sample count for this disease for this facility
                    sampleCount[w]++;
                }
            }

            // sample status
            // 0 - SampCreatedUnTest;		// created but untested
            // 1 - SampCreatedUnTestBatch;	// created, untested waiting to be sent out in batch mode
            // 2 - SampUnTestInTransit;		// in transit
            // 3 - SampWaitForTest;			// waiting for test
            // 4 - SampLostExpired;			// lost/expired
            // 5 - SampTestedInTransit;		// results in transit back to orig facility
            // 6 - SampReturnedAvail;		// results returned - available
            // 7 - SampReturnedLostExpired;	// loss to follow up
            // 8 - SampLtoF;				// loss to follow up
            // 9 - SampPatDead              // patient dead	
            status = samp->getStatus();

            switch (status) {
            case 0:
                total.countSN++;
                diseaseTotal[diseaseIndex].countSN++;
                facilityStats[diseaseIndex][count].countSN++;
                break;
            case 1:
                total.countSB++;
                diseaseTotal[diseaseIndex].countSB++;
                facilityStats[diseaseIndex][count].countSB++;
                break;
            case 2:
                total.countST++;
                diseaseTotal[diseaseIndex].countST++;
                facilityStats[diseaseIndex][count].countST++;
                break;
            case 3:
                total.countSW++;
                diseaseTotal[diseaseIndex].countSW++;
                facilityStats[diseaseIndex][count].countSW++;
                break;
            case 4:
                total.countSL++;
                diseaseTotal[diseaseIndex].countSL++;
                facilityStats[diseaseIndex][count].countSL++;
                break;
            case 5:
                total.countSRT++;
                diseaseTotal[diseaseIndex].countSRT++;
                facilityStats[diseaseIndex][count].countSRT++;
                break;
            case 6:
                total.countSR++;
                diseaseTotal[diseaseIndex].countSR++;
                facilityStats[diseaseIndex][count].countSR++;
                if (fileFlag) {
                    myfile1 << "stats.samples.patient.ptr_" << samp->getPatientPtr() << ",orig.fac_" << samp->getOrigFacility() << ",testing.fac_" << samp->getTestingFacility() << "\n";
                }
                total.sampleReturnTime = total.sampleReturnTime + (samp->getResultsReturnedToOrigFacilityTime() - samp->getCreateTime());
                diseaseTotal[diseaseIndex].sampleReturnTime =  diseaseTotal[diseaseIndex].sampleReturnTime   + (samp->getResultsReturnedToOrigFacilityTime() - samp->getCreateTime());
                facilityStats[diseaseIndex][count].sampleReturnTime = facilityStats[diseaseIndex][count].sampleReturnTime  + (samp->getResultsReturnedToOrigFacilityTime() - samp->getCreateTime());
                break;
            case 7:
                total.countSRL++;
                diseaseTotal[diseaseIndex].countSRL++;
                facilityStats[diseaseIndex][count].countSRL++;
                break;
            case 8:
                total.countSL2F++;
                diseaseTotal[diseaseIndex].countSL2F++;
                facilityStats[diseaseIndex][count].countSL2F++;
                break;
            default:
                total.countSO++;
                diseaseTotal[diseaseIndex].countSO++;
                facilityStats[diseaseIndex][count].countSO++;

            }

            // record count of tests for Samples
            if (test1Name == "") {
                test1Name = samp->getTest(); 
                total.test1Count++;
                diseaseTotal[diseaseIndex].test1Count++;
                facilityStats[diseaseIndex][count].test1Count++;
            }
            else if (test1Name == samp->getTest()) {
                total.test1Count++;
                diseaseTotal[diseaseIndex].test1Count++;
                facilityStats[diseaseIndex][count].test1Count++;
            }
            else if (test2Name == "") {
                test2Name = samp->getTest(); 
                total.test2Count++;
                diseaseTotal[diseaseIndex].test2Count++;
                facilityStats[diseaseIndex][count].test2Count++;
            }
            else if (test2Name == samp->getTest()) {
                total.test2Count++;
                diseaseTotal[diseaseIndex].test2Count++;
                facilityStats[diseaseIndex][count].test2Count++;
            }
            else if (test3Name == "") {
                test3Name = samp->getTest(); 
                total.test3Count++;
                diseaseTotal[diseaseIndex].test3Count++;
                facilityStats[diseaseIndex][count].test3Count++; 
            }
            else if (test3Name == samp->getTest()) {
                total.test3Count++;
                diseaseTotal[diseaseIndex].test3Count++;
                facilityStats[diseaseIndex][count].test3Count++;
            }
            else if (test4Name == "") {
                test4Name = samp->getTest();
                total.test4Count++;
                diseaseTotal[diseaseIndex].test4Count++;
                facilityStats[diseaseIndex][count].test4Count++;
            }
            else if (test4Name == samp->getTest()) {
                total.test4Count++;
                diseaseTotal[diseaseIndex].test4Count++;
                facilityStats[diseaseIndex][count].test4Count++;
            }
            //std::cout << "tests status = " << status << "\n";
        }

        // loop through each disease in model run and update statistics
        for (diseaseIndex = 0; diseaseIndex < diseaseCount; diseaseIndex++) {

            // update the sample count for each disease for this facility
            facilityStats[diseaseIndex][count].countS = sampleCount[diseaseIndex];

            if (debug) std::cout << "stats: inside inactive sample before sample loop count = " << sampleCount[diseaseIndex] <<  "\n";

            // output sample stats only to a file
            if (fileFlag) {

                myfile << facilityStats[diseaseIndex][count].name << "," << facilityStats[diseaseIndex][count].countS << "," << diseaseList[diseaseIndex] << "," << facilityStats[diseaseIndex][count].countSN;
                myfile << "," << facilityStats[diseaseIndex][count].countSB << "," << facilityStats[diseaseIndex][count].countST << "," << facilityStats[diseaseIndex][count].countSW << "," << facilityStats[diseaseIndex][count].countSL;
                myfile  << "," << facilityStats[diseaseIndex][count].countSRT << "," << facilityStats[diseaseIndex][count].countSR << "," << facilityStats[diseaseIndex][count].countSRL << "," << facilityStats[diseaseIndex][count].countSL2F << "," << facilityStats[diseaseIndex][count].countSO;
                if (facilityStats[diseaseIndex][count].countSR  < 1) {
                    myfile << "," << "0";
                }
                else {
                    myfile << "," << facilityStats[diseaseIndex][count].sampleReturnTime/facilityStats[diseaseIndex][count].countSR;
                }
                myfile << "," << test1Name << "," << facilityStats[diseaseIndex][count].test1Count << "," << test2Name << "," << facilityStats[diseaseIndex][count].test2Count;
                myfile << "," << test3Name << "," << facilityStats[diseaseIndex][count].test3Count << "," << test4Name << "," << facilityStats[diseaseIndex][count].test4Count << "\n";

                
            }

            // now update stats for regions - only compile if output is to file
            if (fileFlag) {
                regionNotFound = true;

                for (int i = 0; i < regionCount[diseaseIndex]; i++) {

                    if (region == regions[diseaseIndex][i].name) {
                        if (debug) std::cout << "stats: samples found region = " << region << ", i = " << i << ", index = " << regionCount[diseaseIndex] << "\n";

                        //update statistics for that region
                        regionNotFound = false;

                        regions[diseaseIndex][i].countS = regions[diseaseIndex][i].countS + facilityStats[diseaseIndex][count].countS;
                        regions[diseaseIndex][i].countSN = regions[diseaseIndex][i].countSN + facilityStats[diseaseIndex][count].countSN;
                        regions[diseaseIndex][i].countSB = regions[diseaseIndex][i].countSB + facilityStats[diseaseIndex][count].countSB;
                        regions[diseaseIndex][i].countST = regions[diseaseIndex][i].countST + facilityStats[diseaseIndex][count].countST;
                        regions[diseaseIndex][i].countSW = regions[diseaseIndex][i].countSW + facilityStats[diseaseIndex][count].countSW;
                        regions[diseaseIndex][i].countSL = regions[diseaseIndex][i].countSL + facilityStats[diseaseIndex][count].countSL;
                        regions[diseaseIndex][i].countSRT = regions[diseaseIndex][i].countSRT + facilityStats[diseaseIndex][count].countSRT;
                        regions[diseaseIndex][i].countSR = regions[diseaseIndex][i].countSR + facilityStats[diseaseIndex][count].countSR;
                        regions[diseaseIndex][i].countSRL = regions[diseaseIndex][i].countSRL + facilityStats[diseaseIndex][count].countSRL;
                        regions[diseaseIndex][i].countSL2F = regions[diseaseIndex][i].countSL2F + facilityStats[diseaseIndex][count].countSL2F;
                        regions[diseaseIndex][i].countSO = regions[diseaseIndex][i].countSO + facilityStats[diseaseIndex][count].countSO;
                        regions[diseaseIndex][i].test1Count = regions[diseaseIndex][i].test1Count + facilityStats[diseaseIndex][count].test1Count;
                        regions[diseaseIndex][i].test2Count = regions[diseaseIndex][i].test2Count + facilityStats[diseaseIndex][count].test2Count;
                        regions[diseaseIndex][i].test3Count = regions[diseaseIndex][i].test3Count + facilityStats[diseaseIndex][count].test3Count;
                        regions[diseaseIndex][i].test4Count = regions[diseaseIndex][i].test4Count + facilityStats[diseaseIndex][count].test4Count;
                        regions[diseaseIndex][i].sampleReturnTime = regions[diseaseIndex][i].sampleReturnTime + facilityStats[diseaseIndex][count].sampleReturnTime;

                        if (debug) std::cout << "stats: samples found region = " << region << "\n";
                    }
                }
                if (regionNotFound) {
                    if (debug) std::cout << "stats: samples didn't find region = " << region <<  ", index = " << diseaseIndex << "\n";

                    // add it to the region array
                    regions[diseaseIndex][regionCount[diseaseIndex]].name = region;
                    if (debug) std::cout << "stats: samples didn't find region 10 = " <<  "\n";
                    regions[diseaseIndex][regionCount[diseaseIndex]].countS = facilityStats[diseaseIndex][count].countS;
                    if (debug) std::cout << "stats: samples didn't find region 11 = " <<  "\n";
                    regions[diseaseIndex][regionCount[diseaseIndex]].countSN = facilityStats[diseaseIndex][count].countSN;
                    regions[diseaseIndex][regionCount[diseaseIndex]].countSB = facilityStats[diseaseIndex][count].countSB;
                    regions[diseaseIndex][regionCount[diseaseIndex]].countST = facilityStats[diseaseIndex][count].countST;
                    if (debug) std::cout << "stats: samples didn't find region 12 = " <<  "\n";
                    regions[diseaseIndex][regionCount[diseaseIndex]].countSW = facilityStats[diseaseIndex][count].countSW;
                    regions[diseaseIndex][regionCount[diseaseIndex]].countSL = facilityStats[diseaseIndex][count].countSL;
                    if (debug) std::cout << "stats: samples didn't find region 13 = " <<  "\n";
                    regions[diseaseIndex][regionCount[diseaseIndex]].countSRT = facilityStats[diseaseIndex][count].countSRT;
                    regions[diseaseIndex][regionCount[diseaseIndex]].countSR = facilityStats[diseaseIndex][count].countSR;
                    if (debug) std::cout << "stats: samples didn't find region 14 = " <<  "\n";
                    regions[diseaseIndex][regionCount[diseaseIndex]].countSRL = facilityStats[diseaseIndex][count].countSRL;
                    regions[diseaseIndex][regionCount[diseaseIndex]].countSL2F = facilityStats[diseaseIndex][count].countSL2F;
                    if (debug) std::cout << "stats: samples didn't find region 15 = " <<  "\n";
                    regions[diseaseIndex][regionCount[diseaseIndex]].countSO = facilityStats[diseaseIndex][count].countSO;
                    regions[diseaseIndex][regionCount[diseaseIndex]].test1Count = facilityStats[diseaseIndex][count].test1Count;
                    regions[diseaseIndex][regionCount[diseaseIndex]].test2Count = facilityStats[diseaseIndex][count].test2Count;
                    regions[diseaseIndex][regionCount[diseaseIndex]].test3Count = facilityStats[diseaseIndex][count].test3Count;
                    regions[diseaseIndex][regionCount[diseaseIndex]].test4Count = facilityStats[diseaseIndex][count].test4Count;
                    regions[diseaseIndex][regionCount[diseaseIndex]].sampleReturnTime = facilityStats[diseaseIndex][count].sampleReturnTime;
                    
                    regionCount[diseaseIndex] = regionCount[diseaseIndex] + 1;

                    if (debug) std::cout << "stats: samples didn't find region 2 = " << region << "\n";
                }

                // now update stats for districts
                districtNotFound = true;
                for (int i = 0; i < districtCount[diseaseIndex]; i++) {

                    if (district == districts[diseaseIndex][i].name) {
                        if (debug) std::cout << "stats: samples found district = " << district << "\n";

                        //update statistics for that region
                        districtNotFound = false;

                        districts[diseaseIndex][i].countS = districts[diseaseIndex][i].countS + facilityStats[diseaseIndex][count].countS;
                        districts[diseaseIndex][i].countSN = districts[diseaseIndex][i].countSN + facilityStats[diseaseIndex][count].countSN;
                        districts[diseaseIndex][i].countSB = districts[diseaseIndex][i].countSB + facilityStats[diseaseIndex][count].countSB;
                        districts[diseaseIndex][i].countST = districts[diseaseIndex][i].countST + facilityStats[diseaseIndex][count].countST;
                        districts[diseaseIndex][i].countSW = districts[diseaseIndex][i].countSW + facilityStats[diseaseIndex][count].countSW;
                        districts[diseaseIndex][i].countSL = districts[diseaseIndex][i].countSL + facilityStats[diseaseIndex][count].countSL;
                        districts[diseaseIndex][i].countSRT = districts[diseaseIndex][i].countSRT + facilityStats[diseaseIndex][count].countSRT;
                        districts[diseaseIndex][i].countSR = districts[diseaseIndex][i].countSR + facilityStats[diseaseIndex][count].countSR;
                        districts[diseaseIndex][i].countSRL = districts[diseaseIndex][i].countSRL + facilityStats[diseaseIndex][count].countSRL;
                        districts[diseaseIndex][i].countSL2F = districts[diseaseIndex][i].countSL2F + facilityStats[diseaseIndex][count].countSL2F;
                        districts[diseaseIndex][i].countSO = districts[diseaseIndex][i].countSO + facilityStats[diseaseIndex][count].countSO;
                        districts[diseaseIndex][i].test1Count = districts[diseaseIndex][i].test1Count + facilityStats[diseaseIndex][count].test1Count;
                        districts[diseaseIndex][i].test2Count = districts[diseaseIndex][i].test2Count + facilityStats[diseaseIndex][count].test2Count;
                        districts[diseaseIndex][i].test3Count = districts[diseaseIndex][i].test3Count + facilityStats[diseaseIndex][count].test3Count;
                        districts[diseaseIndex][i].test4Count = districts[diseaseIndex][i].test4Count + facilityStats[diseaseIndex][count].test4Count;
                        districts[diseaseIndex][i].sampleReturnTime = districts[diseaseIndex][i].sampleReturnTime + facilityStats[diseaseIndex][count].sampleReturnTime;

                    }
                }
                if (districtNotFound) {
                    if (debug) std::cout << "stats: samples didn't find district = " << district << ", districtCount = " << districtCount[diseaseCount] << "\n";

                    // add it to the district array
                    districts[diseaseIndex][districtCount[diseaseIndex]].name = district;
                    districts[diseaseIndex][districtCount[diseaseIndex]].name2 = region;
                    districts[diseaseIndex][districtCount[diseaseIndex]].countS = facilityStats[diseaseIndex][count].countS;
                    districts[diseaseIndex][districtCount[diseaseIndex]].countSN = facilityStats[diseaseIndex][count].countSN;
                    districts[diseaseIndex][districtCount[diseaseIndex]].countSB = facilityStats[diseaseIndex][count].countSB;
                    districts[diseaseIndex][districtCount[diseaseIndex]].countST = facilityStats[diseaseIndex][count].countST;
                    districts[diseaseIndex][districtCount[diseaseIndex]].countSW = facilityStats[diseaseIndex][count].countSW;
                    districts[diseaseIndex][districtCount[diseaseIndex]].countSL = facilityStats[diseaseIndex][count].countSL;
                    districts[diseaseIndex][districtCount[diseaseIndex]].countSRT = facilityStats[diseaseIndex][count].countSRT;
                    districts[diseaseIndex][districtCount[diseaseIndex]].countSR = facilityStats[diseaseIndex][count].countSR;
                    districts[diseaseIndex][districtCount[diseaseIndex]].countSRL = facilityStats[diseaseIndex][count].countSRL;
                    districts[diseaseIndex][districtCount[diseaseIndex]].countSL2F = facilityStats[diseaseIndex][count].countSL2F;
                    districts[diseaseIndex][districtCount[diseaseIndex]].countSO = facilityStats[diseaseIndex][count].countSO;
                    districts[diseaseIndex][districtCount[diseaseIndex]].test1Count = facilityStats[diseaseIndex][count].test1Count;
                    districts[diseaseIndex][districtCount[diseaseIndex]].test2Count = facilityStats[diseaseIndex][count].test2Count;
                    districts[diseaseIndex][districtCount[diseaseIndex]].test3Count = facilityStats[diseaseIndex][count].test3Count;
                    districts[diseaseIndex][districtCount[diseaseIndex]].test4Count = facilityStats[diseaseIndex][count].test4Count;
                    districts[diseaseIndex][districtCount[diseaseIndex]].sampleReturnTime = facilityStats[diseaseIndex][count].sampleReturnTime;

                    districtCount[diseaseIndex] = districtCount[diseaseIndex] + 1;

                }
            }
        }
        count = count + 1;
    }

    if (debug) std::cout << "stats: inside inactive sample before dump stats" <<  "\n";

    // dump statistics for each region and district to a file
    if (fileFlag) {
        if (debug) std::cout << "stats: samples region  " << "\n";
        myfile << "-----------------------------------------------------------------------------------  " << "\n";
        myfile << "region" <<  ",samples" <<  ",disease" << ",New" << ",New.in.batch" << ",In.Transit" << ",Wait" << ",Lost" << ",Returned-transit" << ",Returned" << ",Re/Lost" << ",LtoF" << ",Other";
        myfile << ",avg.return.time" << ",test1.name" << ",test1.count" << ",test2.name" << ",test2.count" << ",test3.name" << ",test3.count" << ",test4.name" << ",test4.count" << "\n";

        for (int w = 0; w < diseaseCount; w++) {

            if (debug) std::cout << "stats: samples region count = " << regionCount[w] << ", disease = " << diseaseList[w] << "\n";

            for (int i = 0; i < regionCount[w]; i++) {

                myfile << regions[w][i].name << "," << regions[w][i].countS << "," << diseaseList[w] << "," << regions[w][i].countSN << "," << regions[w][i].countSB << "," << regions[w][i].countST << "," << regions[w][i].countSW;
                myfile  << "," << regions[w][i].countSL << "," << regions[w][i].countSRT << "," << regions[w][i].countSR << "," << regions[w][i].countSRL << "," << regions[w][i].countSL2F << "," << regions[w][i].countSO;
                if (regions[w][i].countSR  < 1) {
                    myfile << "," << "0";
                }
                else {
                    myfile << "," << regions[w][i].sampleReturnTime/regions[w][i].countSR;
                }
                myfile << "," << test1Name << "," << regions[w][i].test1Count << "," << test2Name << "," << regions[w][i].test2Count;
                myfile << "," << test3Name << "," << regions[w][i].test3Count << "," << test4Name << "," << regions[w][i].test4Count << "\n";


                diseaseTotalRegion[w].countS = diseaseTotalRegion[w].countS + regions[w][i].countS;
                diseaseTotalRegion[w].countSN = diseaseTotalRegion[w].countSN + regions[w][i].countSN;
                diseaseTotalRegion[w].countSB = diseaseTotalRegion[w].countSB + regions[w][i].countSB;
                diseaseTotalRegion[w].countST = diseaseTotalRegion[w].countST + regions[w][i].countST;
                diseaseTotalRegion[w].countSW = diseaseTotalRegion[w].countSW + regions[w][i].countSW;
                diseaseTotalRegion[w].countSL = diseaseTotalRegion[w].countSL + regions[w][i].countSL;
                diseaseTotalRegion[w].countSRT = diseaseTotalRegion[w].countSRT + regions[w][i].countSRT;
                diseaseTotalRegion[w].countSR = diseaseTotalRegion[w].countSR + regions[w][i].countSR;
                diseaseTotalRegion[w].countSRL = diseaseTotalRegion[w].countSRL + regions[w][i].countSRL;
                diseaseTotalRegion[w].countSL2F = diseaseTotalRegion[w].countSL2F + regions[w][i].countSL2F;
                diseaseTotalRegion[w].countSO = diseaseTotalRegion[w].countSO + regions[w][i].countSO;
                diseaseTotalRegion[w].test1Count = diseaseTotalRegion[w].test1Count + regions[w][i].test1Count;
                diseaseTotalRegion[w].test2Count = diseaseTotalRegion[w].test2Count + regions[w][i].test2Count;
                diseaseTotalRegion[w].test3Count = diseaseTotalRegion[w].test3Count + regions[w][i].test3Count;
                diseaseTotalRegion[w].test4Count = diseaseTotalRegion[w].test4Count + regions[w][i].test4Count;
                diseaseTotalRegion[w].sampleReturnTime = diseaseTotalRegion[w].sampleReturnTime + regions[w][i].sampleReturnTime;
                totalRegion.countS = totalRegion.countS + regions[w][i].countS;
                totalRegion.countSN = totalRegion.countSN + regions[w][i].countSN;
                totalRegion.countSB = totalRegion.countSB + regions[w][i].countSB;
                totalRegion.countST = totalRegion.countST + regions[w][i].countST;
                totalRegion.countSW = totalRegion.countSW + regions[w][i].countSW;
                totalRegion.countSL = totalRegion.countSL + regions[w][i].countSL;
                totalRegion.countSRT = totalRegion.countSRT + regions[w][i].countSRT;
                totalRegion.countSR = totalRegion.countSR + regions[w][i].countSR;
                totalRegion.countSRL = totalRegion.countSRL + regions[w][i].countSRL;
                totalRegion.countSL2F = totalRegion.countSL2F + regions[w][i].countSL2F;
                totalRegion.countSO = totalRegion.countSO + regions[w][i].countSO;
                totalRegion.test1Count = totalRegion.test1Count + regions[w][i].test1Count;
                totalRegion.test2Count = totalRegion.test2Count + regions[w][i].test2Count;
                totalRegion.test3Count = totalRegion.test3Count + regions[w][i].test3Count;
                totalRegion.test4Count = totalRegion.test4Count + regions[w][i].test4Count;
                totalRegion.sampleReturnTime = totalRegion.sampleReturnTime + regions[w][i].sampleReturnTime;
            }

            // update total count
            diseaseTotal[w].countS = diseaseTotalRegion[w].countS;

            myfile << "-----------------------------------------------------------------------------------  " << "\n";
            myfile << "total.region.samples " << ",disease " << ",New " << ",New.in.batch" << ",In.Transit" << ",Wait" << ",Lost" << ",Returned-transit" << ",Returned" << ",Re/Lost" << ",LtoF" << ",Other";
            myfile << ",avg.return.time" << ",test1.name" << ",test1.count" << ",test2.name" << ",test2.count" << ",test3.name" << ",test3.count" << ",test4.name" << ",test4.count" << "\n";
           
            myfile << diseaseTotalRegion[w].countS << "," << diseaseList[w] << "," << diseaseTotalRegion[w].countSN << "," << diseaseTotalRegion[w].countSB << "," << diseaseTotalRegion[w].countST << "," << diseaseTotalRegion[w].countSW;
            myfile  << "," << diseaseTotalRegion[w].countSL << "," << diseaseTotalRegion[w].countSRT << "," << diseaseTotalRegion[w].countSR << "," << diseaseTotalRegion[w].countSRL << "," << diseaseTotalRegion[w].countSL2F << "," << diseaseTotalRegion[w].countSO;
            if (diseaseTotalRegion[w].countSR  < 1) {
                myfile << "," << "0";
            }
            else {
                myfile << "," << diseaseTotalRegion[w].sampleReturnTime/diseaseTotalRegion[w].countSR;
            }
            myfile << "," << test1Name << "," << diseaseTotalRegion[w].test1Count << "," << test2Name << "," << diseaseTotalRegion[w].test2Count;
            myfile << "," << test3Name << "," << diseaseTotalRegion[w].test3Count << "," << test4Name << "," << diseaseTotalRegion[w].test4Count << "\n";

            myfile << "-----------------------------------------------------------------------------------  " << "\n";
        }
        

        myfile << "-----------------------------------------------------------------------------------  " << "\n";
        myfile << "stats.total.region.samples" << ",New" << ",New.in.batch" <<  ",In.Transit" <<  ",Wait" <<  ",Lost" << ",Returned-transit" << ",Returned" <<  ",Re/Lost" <<  ",LtoF" <<  ",Other";
        myfile << ",avg.return.time" << ",test1.name" << ",test1.count" << ",test2.name" << ",test2.count" << ",test3.name" << ",test3.count" << ",test4.name" << ",test4.count" << "\n";
        
        myfile << totalRegion.countS << "," << totalRegion.countSN << "," << totalRegion.countSB << "," << totalRegion.countST << "," << totalRegion.countSW << "," << totalRegion.countSL;
        myfile << "," << totalRegion.countSRT << "," << totalRegion.countSR << "," << totalRegion.countSRL << "," << totalRegion.countSL2F << "," << totalRegion.countSO;
        if (totalRegion.countSR  < 1) {
             myfile << "," << "0";
            }
        else {
             myfile << "," << totalRegion.sampleReturnTime/totalRegion.countSR;
        }
        myfile << "," << test1Name << "," << totalRegion.test1Count << "," << test2Name << "," << totalRegion.test2Count;
        myfile << "," << test3Name << "," << totalRegion.test3Count << "," << test4Name << "," << totalRegion.test4Count << "\n";

        myfile << "-----------------------------------------------------------------------------------  " << "\n";

        for (int w = 0; w < diseaseCount; w++) {

            if (debug) std::cout << "stats: samples  district count = " << districtCount[w] << ", disease = " << diseaseList[w] << "\n";
            myfile << "district" << " region" << ",samples" <<  ",disease" << ",New" << ",New.in.batch" << ",In.Transit" <<  ",Wait" <<  ",Lost" <<  ",Returned-transit" <<  ",Returned" <<  ",Re/Lost" << ",LtoF" <<  ",Other";
            myfile << ",avg.return.time" << ",test1.name" << ",test1.count" << ",test2.name" << ",test2.count" << ",test3.name" << ",test3.count" << ",test4.name" << ",test4.count" << "\n";


            for (int i = 0; i < districtCount[w]; i++) {

                myfile << districts[w][i].name << "," << districts[w][i].name2 << "," << districts[w][i].countS << "," << diseaseList[w] << "," << districts[w][i].countSN << "," << districts[w][i].countSB << "," << districts[w][i].countST << "," << districts[w][i].countSW;
                myfile << "," << districts[w][i].countSL << "," << districts[w][i].countSRT << "," << districts[w][i].countSR << "," << districts[w][i].countSRL << "," << districts[w][i].countSL2F << "," << districts[w][i].countSO;
                if (districts[w][i].countSR  < 1) {
                    myfile << "," << "0";
                }
                else {
                    myfile << "," << districts[w][i].sampleReturnTime/districts[w][i].countSR;
                }
                myfile << "," << test1Name << "," << districts[w][i].test1Count << "," << test2Name << "," << districts[w][i].test2Count;
                myfile << "," << test3Name << "," << districts[w][i].test3Count << "," << test4Name << "," << districts[w][i].test4Count << "\n";


                diseaseTotalDistrict[w].countS = diseaseTotalDistrict[w].countS + districts[w][i].countS;
                diseaseTotalDistrict[w].countSN = diseaseTotalDistrict[w].countSN + districts[w][i].countSN;
                diseaseTotalDistrict[w].countSB = diseaseTotalDistrict[w].countSB + districts[w][i].countSB;
                diseaseTotalDistrict[w].countST = diseaseTotalDistrict[w].countST + districts[w][i].countST;
                diseaseTotalDistrict[w].countSW = diseaseTotalDistrict[w].countSW + districts[w][i].countSW;
                diseaseTotalDistrict[w].countSL = diseaseTotalDistrict[w].countSL + districts[w][i].countSL;
                diseaseTotalDistrict[w].countSRT = diseaseTotalDistrict[w].countSRT + districts[w][i].countSRT;
                diseaseTotalDistrict[w].countSR = diseaseTotalDistrict[w].countSR + districts[w][i].countSR;
                diseaseTotalDistrict[w].countSRL = diseaseTotalDistrict[w].countSRL + districts[w][i].countSRL;
                diseaseTotalDistrict[w].countSL2F = diseaseTotalDistrict[w].countSL2F + districts[w][i].countSL2F;
                diseaseTotalDistrict[w].countSO = diseaseTotalDistrict[w].countSO + districts[w][i].countSO;
                diseaseTotalDistrict[w].test1Count = diseaseTotalDistrict[w].test1Count + districts[w][i].test1Count;
                diseaseTotalDistrict[w].test2Count = diseaseTotalDistrict[w].test2Count + districts[w][i].test2Count;
                diseaseTotalDistrict[w].test3Count = diseaseTotalDistrict[w].test3Count + districts[w][i].test3Count;
                diseaseTotalDistrict[w].test4Count = diseaseTotalDistrict[w].test4Count + districts[w][i].test4Count;
                diseaseTotalDistrict[w].sampleReturnTime = diseaseTotalDistrict[w].sampleReturnTime + districts[w][i].sampleReturnTime;
                totalDistrict.countS = totalDistrict.countS + districts[w][i].countS;
                totalDistrict.countSN = totalDistrict.countSN + districts[w][i].countSN;
                totalDistrict.countSB = totalDistrict.countSB + districts[w][i].countSB;
                totalDistrict.countST = totalDistrict.countST + districts[w][i].countST;
                totalDistrict.countSW = totalDistrict.countSW + districts[w][i].countSW;
                totalDistrict.countSL = totalDistrict.countSL + districts[w][i].countSL;
                totalDistrict.countSRT = totalDistrict.countSRT + districts[w][i].countSRT;
                totalDistrict.countSR = totalDistrict.countSR + districts[w][i].countSR;
                totalDistrict.countSRL = totalDistrict.countSRL + districts[w][i].countSRL;
                totalDistrict.countSL2F = totalDistrict.countSL2F + districts[w][i].countSL2F;
                totalDistrict.countSO = totalDistrict.countSO + districts[w][i].countSO;
                totalDistrict.test1Count = totalDistrict.test1Count + districts[w][i].test1Count;
                totalDistrict.test2Count = totalDistrict.test2Count + districts[w][i].test2Count;
                totalDistrict.test3Count = totalDistrict.test3Count + districts[w][i].test3Count;
                totalDistrict.test4Count = totalDistrict.test4Count + districts[w][i].test4Count;
                totalDistrict.sampleReturnTime = totalDistrict.sampleReturnTime + districts[w][i].sampleReturnTime;
            }

            myfile << "-----------------------------------------------------------------------------------  " << "\n";
            myfile << "total.district.samples" <<  ",disease" << ",New" << ",New.in.batch" << ",In.Transit" << ",Wait" << ",Lost" << ",Returned-transit" << ",Returned" << ",Re/Lost" << ",LtoF" << ",Other";
            myfile << ",avg.return.time" << ",test1.name" << ",test1.count" << ",test2.name" << ",test2.count" << ",test3.name" << ",test3.count" << ",test4.name" << ",test4.count" << "\n";

            myfile << diseaseTotalDistrict[w].countS << "," << diseaseList[w] << "," << diseaseTotalDistrict[w].countSN << "," << diseaseTotalDistrict[w].countSB << "," << diseaseTotalDistrict[w].countST << "," << diseaseTotalDistrict[w].countSW;
            myfile << "," << diseaseTotalDistrict[w].countSL << "," << diseaseTotalDistrict[w].countSRT << "," << diseaseTotalDistrict[w].countSR << "," << diseaseTotalDistrict[w].countSRL << "," << diseaseTotalDistrict[w].countSL2F;
            myfile << "," << diseaseTotalDistrict[w].countSO;
            if (diseaseTotalDistrict[w].countSR  < 1) {
                myfile << "," << "0";
            }
            else {
                myfile << "," << diseaseTotalDistrict[w].sampleReturnTime/diseaseTotalDistrict[w].countSR;
            }
            myfile << "," << test1Name << "," << diseaseTotalDistrict[w].test1Count << "," << test2Name << "," << diseaseTotalDistrict[w].test2Count;
            myfile << "," << test3Name << "," << diseaseTotalDistrict[w].test3Count << "," << test4Name << "," << diseaseTotalDistrict[w].test4Count << "\n";

            myfile << "-----------------------------------------------------------------------------------  " << "\n";
        }

        myfile << "-----------------------------------------------------------------------------------  " << "\n";
        myfile << "total.district.samples" <<  ",New" << ",New.in.batch" << ",In.Transit" << ",Wait" << ",Lost" << ",Returned-transit" << ",Returned" << ",Re/Lost" << ",LtoF" << ",Other";
        myfile << ",avg.return.time" << ",test1.name" << ",test1.count" << ",test2.name" << ",test2.count" << ",test3.name" << ",test3.count" << ",test4.name" << ",test4.count" << "\n";
        
        myfile << totalDistrict.countS << "," << totalDistrict.countSN << "," << totalDistrict.countSB << "," << totalDistrict.countST << "," << totalDistrict.countSW << "," << totalDistrict.countSL << "," << totalDistrict.countSRT;
        myfile << "," << totalDistrict.countSR << "," << totalDistrict.countSRL << "," << totalDistrict.countSL2F << "," << totalDistrict.countSO;
        if (totalDistrict.countSR  < 1) {
            myfile << "," << "0";
        }
        else {
            myfile << "," << totalDistrict.sampleReturnTime/totalDistrict.countSR;
        }
        myfile << "," << test1Name << "," << totalDistrict.test1Count << "," << test2Name << "," << totalDistrict.test2Count;
        myfile << "," << test3Name << "," << totalDistrict.test3Count << "," << test4Name << "," << totalDistrict.test4Count << "\n";

        myfile << "-----------------------------------------------------------------------------------  " << "\n";
    }

    if (debug) std::cout << "stats: inside inactive sample before file final output " <<  "\n";

    if (fileFlag) {

        for (int w = 0; w < diseaseCount; w++) {

            myfile << "facility.count" << ",disease" << ",samples" << ",New" << ",New.in.batch" << ",In.Transit" << ",Wait" << ",Lost" << ",Returned-transit" << ",Returned" << ",Re/Lost" << ",Other";
            myfile << ",avg.return.time" << ",test1.name" << ",test1.count" << ",test2.name" << ",test2.count" << ",test3.name" << ",test3.count" << ",test4.name" << ",test4.count" << "\n";
        
            myfile << countF << "," <<  diseaseList[w] << "," << diseaseTotal[w].countS << "," << diseaseTotal[w].countSN << "," << diseaseTotal[w].countSB << "," << diseaseTotal[w].countST << "," << diseaseTotal[w].countSW << "," << diseaseTotal[w].countSL << "," << diseaseTotal[w].countSRT;
            myfile << "," << diseaseTotal[w].countSR << "," << diseaseTotal[w].countSRL << "," << diseaseTotal[w].countSO;
            if (diseaseTotal[w].countSR  < 1) {
                myfile << "," <<  "0";
            }
            else {
                myfile << "," << diseaseTotal[w].sampleReturnTime/diseaseTotal[w].countSR;
            }
            myfile << "," << test1Name << "," << diseaseTotal[w].test1Count << "," << test2Name << "," << diseaseTotal[w].test2Count;
            myfile << "," << test3Name << "," << diseaseTotal[w].test3Count << "," << test4Name << "," << diseaseTotal[w].test4Count << "\n";

            myfile << "-----------------------------------------------------------------------------------  " << "\n";
        }


        myfile << "facility.count" << ",samples" << ",New" << ",New.in.batch" << ",In.Transit" << ",Wait" << ",Lost" << ",Returned-transit" << ",Returned" << ",Re/Lost" << ",LtoF" << ",Other";
        myfile << ",avg.return.time" << ",test1.name" << ",test1.count" << ",test2.name" << ",test2.count" << ",test3.name" << ",test3.count" << ",test4.name" << ",test4.count" << "\n";
        
        myfile << countF << "," << total.countS << " ," << total.countSN << " ," << total.countSB << "," << total.countST << "," << total.countSW << "," << total.countSL << "," << total.countSRT << "," << total.countSR;
        myfile << "," << total.countSRL << "," << total.countSL2F << "," << total.countSO;
        if (total.countSR  < 1) {
            myfile << "," << "0";
        }
        else {
            myfile << "," << total.sampleReturnTime/total.countSR;
        }
        myfile << "," << test1Name << "," << total.test1Count << "," << test2Name << "," << total.test2Count;
        myfile << "," << test3Name << "," << total.test3Count << "," << test4Name << "," << total.test4Count << "\n";


        myfile.close();
        myfile1.close();
    }
    else {
        std::cout << "stats: fac =  " << countF << ", samples = " << total.countS << "\n";
        std::cout << "stats: sample" << "  New = " << total.countSN << "  New waiting in batch = " << total.countSB << " In Transit =  " << total.countST << ", Wait = " << total.countSW << ", Lost = " << total.countSL << ", Returned -transit = " << total.countSRT;
        std::cout << ", Returned = " << total.countSR << ", Re/Lost = " << total.countSRL << ", LtoF = " << total.countSL2F << ", Other = " << total.countSO << ", avg. return time = " << total.sampleReturnTime/total.countSR;
        std::cout  << "," << test1Name << " = " << total.test1Count << "," << test2Name << " = " << total.test2Count <<"," << test3Name << " = " << total.test3Count <<"," << test4Name << " = " << total.test4Count << "\n";
    }

    if (debug) std::cout << "stats: inside inactive sample at end " <<  "\n";
}



