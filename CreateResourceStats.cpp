//
//  CreateResourceStats.cpp
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




// count resources used at each facility for this timestep
void resourceStats(FacilitiesList* list, bool fileFlag, int timeStep) {

    Diagnosis* d;
    Testing* t;
    vector<Patient*> p;
    vector<Sample*> s;
    vector<Test*> tests;
    vector<facilityAttributes*> fa;
    ofstream myfile;

    int noFacilities = 0;
    float avgTests = 0;
    int countF = 0;
    int testCount = 0;
    int maxTestCount = 0;
    int allCountNoMachines = 0;
    int allCountTestsToday = 0;
    int allCountTestsCycle = 0;
    int allCountTotalTests = 0;
    int errNo;
    string subDirBase;

    // basic resource structure
    struct resourceCount {
        string testName;
        string diseaseName;
        int noMachines;
        int countTotalTests;
    };

    // keep resource statistics by region and district
    struct geographicalStats {
        string name;
        bool testing;
        int testCount;
        int noMachines;
        resourceCount test[10];
    };

    geographicalStats total;

    // keep sample statistics by region and district
    geographicalStats regions[20];
    geographicalStats districts[270];

    int regionCount = 0;
    int districtCount = 0;
    int totalCount = 0;
    string region;
    string district;
    bool totalFound;
    bool regionNotFound;
    bool regionTestNotFound;
    bool districtNotFound;
    bool districtTestNotFound;

    // extra facility output
    bool fileFacilityFlag = 0;

    vector<Facility*> facilities = list->getListPtr();

    countF = facilities.size();

    // get statistical array for each facility
    geographicalStats facilityStats[countF];

    // initialize all region data to 0
    for (int i = 0; i < 20; i++) {

        regions[i].name = "";
        regions[i].testCount = 0;
        regions[i].noMachines = 0;
        for (int j = 0; j < 10; j++) {
            regions[i].test[j].testName = "";
            regions[i].test[j].diseaseName = "";
            regions[i].test[j].noMachines = 0;
            regions[i].test[j].countTotalTests = 0;
        }
    }


    // initialize all district data to 0
    for (int i = 0; i < 270; i++) {

        districts[i].name = "";
        districts[i].testCount = 0;
        districts[i].noMachines = 0;
        for (int j = 0; j < 10; j++) {
            districts[i].test[j].testName = "";
            districts[i].test[j].diseaseName = "";
            districts[i].test[j].noMachines = 0;
            districts[i].test[j].countTotalTests = 0;
        }
    }

    //std::cout << "inside resource stats "  << "\n";

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

        myfile.open(subDirBase + "/ResourceStats.csv");

        myfile << "Facility" << ",disease" << ",test.name" << ",tests.today" << ",tests.this.cycle" << ",total.tests.this.run" << ",avg.tests.per.timestep";
        myfile << ",max.tests.per.timestep" << ",max.tests.that.occurred.per.timestep" << ",max.tests.per.cycle" << ",max.tests.that.occurred.per.cycle";
        myfile << ",number.of.test.machines" << "\n";

    }

    if (false) std::cout << "inside resource stats subDirBase = " << subDirBase << ", statsubscenariodir = " << statSubScenarioDir << ", fileflag = " << fileFlag << "\n";
    
    noFacilities = 0;

    // loop through each facility and extract resouce numbers for each test (for each disease) that is done there
    for (auto facility : facilities) {

        // get the up name for this facility
        d = facility->getDiagnosisPtr();
        t = facility->getTestingPtr();
        p = d->getPatientPtr();
        s = t->getSamplePtr();
        fa = facility->getFacilityAttributesPtr();
        tests = t->getTeststPtr();
        region = facility->getRegion();
        district = facility->getDistrict();

        testCount = 0;

        if (!fileFlag) std::cout << "facility = " << facility->getName() << ", facility can do testing  = " << facility->testingAvailable("TB", "Xpert") << "\n";

        // need to loop through the facility attributes for each facility
        // this has all the resources stored for that test 

        // update facility statistics
        facilityStats[noFacilities].name = facility->getName();
        facilityStats[noFacilities].testCount = 0;

        for (int j = 0; j < 10; j++) {
            facilityStats[noFacilities].test[j].testName = "";
            facilityStats[noFacilities].test[j].diseaseName = "";
            facilityStats[noFacilities].test[j].noMachines = 0;
            facilityStats[noFacilities].test[j].countTotalTests = 0;
        }

        for (auto fattr : fa) {
            
            // only track resources for facilities that do testing
            if (fattr->testing) {

                // loop through each of the tests and pull out tests done today and total tests this reagent cycle
                for (auto test : tests) {

                    if ((fattr->testName == test->getTestName()) && (fattr->disease == test->getDiseaseName())) {

                        //std::cout << "can do testing -  facility = " << facility->getName() << ", disease = " << fattr->disease << ", test = " << fattr->testName << ", test available = " << fattr->testing << "\n";
                        //std::cout << "can do testing -  facility attr no. machines = " << fattr->noMachines << ", test no. machines = " << test->getNoMachines() << "\n";

                        // output resource stats if this facility can do testing
                        if (fileFlag) {

                            if (testCount == 0) {

                                myfile << facility->getName();
                            }
                            myfile << "," << test->getDiseaseName() << "," << test->getTestName() << "," << test->getTestsThisTimestep();
                            myfile << "," << test->getTestsThisReagentCycle() << "," << test->getTotalTests() << ",";
                            if (timeStep == 0) {

                                myfile << test->getTotalTests();
                            }
                            else {

                                myfile << test->getTotalTests() / timeStep;
                            }
                            myfile << "," << test->getMaxTestPerTimestep() << "," << test->getMaxTestsAllTimesteps();
                            myfile << "," << test->getMaxTestPerCycle() << "," << test->getMaxTestsAllCycles();
                            myfile << "," << fattr->noMachines << "\n";
                        }
                        else {

                            if (testCount == 0) {

                                std::cout << "Resources: fac name =  " << facility->getName() << "\n";
                            }
                            std::cout << "                 disease = " << test->getDiseaseName() << ", test name = " << test->getTestName() << ", tests today = ";
                            std::cout << test->getTestsThisTimestep() << ", tests this cycle = " << test->getTestsThisReagentCycle() << ", total tests this run = ";
                            std::cout << test->getTotalTests() << ", avg tests per timestep = ";
                            if (timeStep == 0) {

                                std::cout << test->getTotalTests();
                            }
                            else {

                                std::cout << test->getTotalTests() / timeStep;
                            }
                            std::cout << ", max.tests.all.machines.in.a.timestep = " << test->getMaxTestsAllTimesteps() << ", max.tests.all.machines.in.a.cycle = " << test->getMaxTestsAllCycles() << ", no. of test machines = " << fattr->noMachines << "\n";
                        }

                        // store resource statistics in facility/test, total facility/test and totals    
                        facilityStats[noFacilities].test[testCount].testName = test->getTestName();
                        facilityStats[noFacilities].testCount = testCount;
                        facilityStats[noFacilities].noMachines = fattr->noMachines;
                        facilityStats[noFacilities].test[testCount].diseaseName = test->getDiseaseName();
                        facilityStats[noFacilities].test[testCount].countTotalTests = test->getTotalTests();
                        totalFound = false;
                        for (int i = 0; i < totalCount; i++) {

                            if ((total.test[i].testName == facilityStats[noFacilities].test[testCount].testName) && (total.test[i].diseaseName == facilityStats[noFacilities].test[testCount].diseaseName)) {

                                //total.test[i].testName = facilityStats[noFacilities].test[testCount].testName;
                                //total.test[i].diseaseName = facilityStats[noFacilities].test[testCount].diseaseName;
                                total.test[i].countTotalTests = total.test[i].countTotalTests + facilityStats[noFacilities].test[testCount].countTotalTests;
                                total.test[i].noMachines = total.test[i].noMachines + facilityStats[noFacilities].noMachines;
                                totalFound = true;
                            }
                        }
                        // if a total structure doesn't exist for this test and disease, create one
                        if (!totalFound) {
                        
                            total.test[totalCount].testName = facilityStats[noFacilities].test[testCount].testName;
                            total.test[totalCount].diseaseName = facilityStats[noFacilities].test[testCount].diseaseName;
                            total.test[totalCount].countTotalTests = facilityStats[noFacilities].test[testCount].countTotalTests;
                            total.test[totalCount].noMachines = facilityStats[noFacilities].noMachines;
                            totalCount++;
                        }

                        allCountTestsToday = allCountTestsToday + test->getTestsThisTimestep();
                        allCountTestsCycle = allCountTestsCycle + test->getTestsThisReagentCycle();
                        //allCountTotalTests = allCountTotalTests + facilityStats[noFacilities].test[testCount].countTotalTests;
                        //allCountNoMachines = allCountNoMachines + facilityStats[noFacilities].test[testCount].noMachines;
                        allCountTotalTests = allCountTotalTests + test->getTotalTests();
                        allCountNoMachines = allCountNoMachines + fattr->noMachines;

                        //std::cout << "after all sums " <<  "\n";

                        // now update stats for regions - only compile if output is to file
                        if (fileFlag) {
                            regionNotFound = true;
                            regionTestNotFound = true;

                            //std::cout << "before loop regionCount = " << regionCount << "\n";

                            for (int i = 0; i < regionCount; i++) {

                                if (region == regions[i].name) {

                                    //std::cout << "found region name = " << region << "\n";

                                    //update statistics for that region
                                    regionNotFound = false;

                                    for (int j = 0; j < regions[i].testCount; j++) {

                                        if ((test->getTestName() == regions[i].test[j].testName) && (test->getDiseaseName() == regions[i].test[j].diseaseName)) {

                                            //std::cout << "found test = " << regions[i].test[j].testName << "\n";

                                            //update statistics for that region/test
                                            regionTestNotFound = false;

                                            regions[i].test[j].countTotalTests = regions[i].test[j].countTotalTests + test->getTotalTests();
                                            regions[i].test[j].noMachines = regions[i].test[j].noMachines + fattr->noMachines;

                                        }
                                    }

                                    // region exists but need to add new test entry
                                    if (regionTestNotFound) {

                                        //std::cout << "didn't find test testCount = " << regions[i].testCount << "\n";

                                        regions[i].test[regions[i].testCount].testName = test->getTestName();
                                        regions[i].test[regions[i].testCount].diseaseName = test->getDiseaseName();
                                        regions[i].test[regions[i].testCount].countTotalTests = test->getTotalTests();
                                        regions[i].test[regions[i].testCount].noMachines = fattr->noMachines;

                                        regions[i].testCount = regions[i].testCount + 1;
                                    }
                                }
                            }

                            // region not found - add
                            if (regionNotFound) {

                                //std::cout << "didn't find region name = " << region << "\n";

                                regions[regionCount].name = region;
                                regions[regionCount].testCount = regions[regionCount].testCount + 1;
                                regions[regionCount].test[0].testName = test->getTestName();
                                regions[regionCount].test[0].diseaseName = test->getDiseaseName();
                                regions[regionCount].test[0].countTotalTests = test->getTotalTests();
                                regions[regionCount].test[0].noMachines = fattr->noMachines;

                                regionCount = regionCount + 1;

                            }


                            // now update stats for districts
                            districtNotFound = true;
                            districtTestNotFound = true;

                            for (int i = 0; i < districtCount; i++) {

                                if (district == districts[i].name) {

                                    //update statistics for that region
                                    districtNotFound = false;

                                    for (int j = 0; j < districts[i].testCount; j++) {

                                        if ((test->getTestName() == districts[i].test[j].testName) && (test->getDiseaseName() == districts[i].test[j].diseaseName)) {

                                            //update statistics for that region
                                            districtTestNotFound = false;

                                            districts[i].test[j].countTotalTests = districts[i].test[j].countTotalTests + test->getTotalTests();
                                            districts[i].test[j].noMachines = districts[i].test[j].noMachines + fattr->noMachines;
                                        }
                                    }

                                    // region exists but no test entry - add it
                                    if (districtTestNotFound) {

                                        districts[i].test[districts[i].testCount].testName = test->getTestName();
                                        districts[i].test[districts[i].testCount].diseaseName = test->getDiseaseName();
                                        districts[i].test[districts[i].testCount].countTotalTests = test->getTotalTests();
                                        districts[i].test[districts[i].testCount].noMachines = fattr->noMachines;

                                        districts[i].testCount = districts[i].testCount + 1;
                                    }
                                }
                            }

                            // district not found - add it
                            if (districtNotFound) {

                                districts[districtCount].name = district;
                                districts[districtCount].testCount = districts[districtCount].testCount + 1;
                                districts[districtCount].test[0].testName = test->getTestName();
                                districts[districtCount].test[0].diseaseName = test->getDiseaseName();
                                districts[districtCount].test[0].countTotalTests = test->getTotalTests();
                                districts[districtCount].test[0].noMachines = fattr->noMachines;

                                districtCount = districtCount + 1;

                            }
                        }

                        facilityStats[noFacilities].testCount = facilityStats[noFacilities].testCount + 1;
                        testCount = testCount + 1;

                        // keep track of how many tests are available
                        if (maxTestCount < testCount) maxTestCount = testCount;

                    }
                    
                    if (!fileFlag) std::cout << "resource stats no. facilities = " << noFacilities << "\n";

                }   //loop though store test data
            }       // check to see of testing is done for this disease/test here
        }           // loop through facility attributes to find out what tests are associated with this facility
        noFacilities++;
    }

    if (false) std::cout << "inside resource stats after facilities loop " <<  "\n";
    
    if (!fileFlag) std::cout << "after checking all facilities noFacilities = " << noFacilities << "\n";

    // dump statistics for each facility to a file
    if (fileFacilityFlag) {

        myfile << "-----------------------------------------------------------------------------------  " << "\n";

        myfile << "facility" << ",disease" << ",test.name" <<  ",total.tests.this.run" << ",avg.tests.per.timestep" << ",number.of.test.machines" << "\n";

        for (int i = 0; i < noFacilities; i++) {

            for (int j = 0; j < facilityStats[i].testCount; j++) {

                if (j == 0) {

                    myfile <<  facilityStats[i].name;
                }
                myfile << "," << facilityStats[i].test[j].diseaseName << "," << facilityStats[i].test[j].testName << "," << facilityStats[i].test[j].countTotalTests;
                if (timeStep == 0) {
                
                    myfile << "," << facilityStats[i].test[j].countTotalTests;
                }
                else {
                
                    myfile << "," << facilityStats[i].test[j].countTotalTests / timeStep;
                }
                myfile <<"," << facilityStats[i].test[j].noMachines << "\n";
            }
        }
    }
    if (!fileFlag) std::cout << "inside resource stats after facilities "  << "\n";
    
    // dump statistics for each region and district to a file
    if (fileFlag) {

        myfile << "-----------------------------------------------------------------------------------  " << "\n";

        myfile << "Region" << ",disease" << ",test.name" << ",total.tests.this.run" << ",avg.tests.per.timestep" << ",number.of.test.machines" << "\n";

        for (int i = 0; i < regionCount; i++) {

            myfile <<  regions[i].name;

            for (int j = 0; j < regions[i].testCount; j++) {
                myfile << "," << regions[i].test[j].diseaseName << "," << regions[i].test[j].testName << "," << regions[i].test[j].countTotalTests;
                if (timeStep == 0) {
                
                    myfile << "," << regions[i].test[j].countTotalTests;
                }
                else {
                
                    myfile << "," << regions[i].test[j].countTotalTests / timeStep;
                }
                myfile  << "," << regions[i].test[j].noMachines << "\n";
            }
        }

        myfile << "-----------------------------------------------------------------------------------  " << "\n";

        myfile << "District" << ",disease" << ",test.name" << ",total.tests.this.run" << ",avg.tests.per.timestep" << ",number.of.test.machines" << "\n";

        for (int i = 0; i < districtCount; i++) {

            myfile << districts[i].name;

            for (int j = 0; j < districts[i].testCount; j++) {
                myfile << "," << districts[i].test[j].diseaseName << "," << districts[i].test[j].testName << "," << districts[i].test[j].countTotalTests;
                if (timeStep == 0) {
                
                    myfile << "," << districts[i].test[j].countTotalTests;
                }
                else {
                
                    myfile << "," << districts[i].test[j].countTotalTests / timeStep;
                }
                myfile << "," << districts[i].test[j].noMachines << "\n";
            }
        }

        myfile << "-----------------------------------------------------------------------------------  " << "\n";
    }
    if (!fileFlag) std::cout << "inside resource stats allCountTestsToday = " << allCountTestsToday << ", noFacilities = " << noFacilities << "\n";

    if (noFacilities > 0) {
        avgTests = allCountTestsToday / noFacilities;
    }

    if (noFacilities > 0) {
        avgTests = allCountTestsToday / noFacilities;
    }

    if (fileFlag) {
        myfile << "Totals" << ",Disease" << ",Test" << ",total.Tests.this.run" << ",avg.tests.per.timestep" << ",number.of.test.machines" << "\n";

        for (int i = 0; i < totalCount; i++) {

            myfile << "" << "," <<  total.test[i].diseaseName << "," << total.test[i].testName << "," << total.test[i].countTotalTests;
            if (timeStep == 0) {

                myfile << "," << total.test[i].countTotalTests;
            }
            else {

                myfile << "," << total.test[i].countTotalTests / timeStep;
            }
            myfile << "," << total.test[i].noMachines << "\n";
        }
     
        // count of all testing machines really doesn't make any sense - can't add prc tests to Xperts tests
        /*myfile << "Totals" << "," << "" << "," << "" << "," << allCountTotalTests;
        if (timeStep == 0) {
        
            myfile << "," << allCountTotalTests;
        }
        else {

            myfile << "," << allCountTotalTests / timeStep;
        }
        myfile << "," << allCountNoMachines << "\n";*/

        myfile.close();
    }
    else {
        std::cout << "Resource: total tests today = " << allCountTestsToday << " total tests this cycle =  " << allCountTestsCycle << ", avg test today per fac = " << avgTests << ", totalTests this run = " << allCountTotalTests;
        if (timeStep == 0) {
        
            std::cout << ", avg tests per timestep = " << allCountTotalTests;
        }
        else {
        
            std::cout << ", avg tests per timestep = " << allCountTotalTests / timeStep;
        }
        std::cout << ", no. of test machines = " << allCountNoMachines << "\n";
    }

    //std::cout << "inside resource stats - done "  << "\n";
}



