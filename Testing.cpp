//
//  Testing.cpp
//  mdrtb
//
//  Created by L. Dombrowski on 1/10/2020.
//
//#include <random> //The code that uses these is only commented out
//#include <chrono> //So I didn't remove them JP
#include <iostream>
#include <string>
#include <vector>

#include "Testing.h"
#include "Facility.h"
#include "FacilitiesList.h"
#include "FGlobal.h"
#include "GlobalParameters.h"
#include "CreateAgents.h"

using namespace std;



Testing::Testing() {
};

Testing::Testing(string facilityName) {

    // Facility name
    _facility = facilityName;

    // how many tests can be done in a day
    // One lab we surveyed in ghana did 300 per year…
    // That was for TB smear microscopy.But, an Xpert runs a TB sample in about 1 hour,
    // I think.So, you can calculate that max utilization from that. Might have more 
    // than 1 module, also. Dr. Lee Schroeder

}
Testing::Testing(string facilityName, int maxTests, int maxTestsReagents, int reagentRestockTime, int noPersonel) {

    // Facility name
    _facility = facilityName;

    // how many tests can be done in a day
    //_maxTests = maxTests;

    // how many tests can be done with full reagent stocks
    //_maxTestsReagents = maxTestsReagents;

    // How many timesteps before reagents get restocked
    //_reagentRestockTime = reagentRestockTime;

    // how many people available to do tests
    _noPersonel = noPersonel;

}

Testing::Testing(string facilityName, int tier, vector<string> diseaseList, vector<Test*> tests, vector <facilityAttributes*> attrPtr) {

    int nMachines;
    int maxTestPerTimeStep;
    bool createAgent;

    // Facility name
    _facility = facilityName;

            // create a test agent for each disease
            for (auto test : tests) {

                switch (tier) {
                case 0:
                    if (test->getTier0()) {
                        createAgent = true;
                    }
                    break;
                case 1:
                    if (test->getTier1()) {
                        createAgent = true;
                    }
                    break;
                case 2:
                    if (test->getTier2()) {
                        createAgent = true;
                    }
                    break;
                case 3:
                    if (test->getTier3()) {
                        createAgent = true;
                    }
                    break;
                case 4:
                    if (test->getTier4()) {
                        createAgent = true;
                    }
                    break;
                case 5:
                    if (test->getTier5()) {
                        createAgent = true;
                    }
                    break;
                case 6:
                    if (test->getTier6()) {
                        createAgent = true;
                    }
                    break;
                case 7:
                    if (test->getTier7()) {
                        createAgent = true;
                    }
                    break;
                case 8:
                    if (test->getTier8()) {
                        createAgent = true;
                    }
                    break;
                case 9:
                    if (test->getTier9()) {
                        createAgent = true;
                    }
                    break;
                }
                if (createAgent) {

                    // copy test agent into a new test agent
                    Test* newTest = new Test(*test);

                    // default to 1
                    nMachines = 1;

                    //std::cout << "before loop " << '\n';

                    // get the no. of machines for this facility to calculate the max tests per time step
                    for (auto a : attrPtr) {

                        //std::cout << "loop disease match = " << test->getTestName() << ", a->disease = " << a->disease << ", nomachines" << a->noMachines << '\n';

                        if (test->getDiseaseName() == a->disease) {

                            if (test->getTestName() == a->testName) {

                                // found a facilityAttributes object that matches disease and test - get no. of machines from it
                                nMachines = a->noMachines;
                            }
                        }
                    }

                    // calculate no. of reagents per day based off of no. of machines * no. of test cycles a day * no. tests per machine at one time
                    maxTestPerTimeStep = test->getTestsPerMachine() * test->getCyclesPerTimestep() * nMachines;

                    //std::cout << "max = " << maxTestPerTimeStep << ", tests per machine = " << test->getTestsPerMachine() << ", cycles per timestep =" << test->getCyclesPerTimestep() << ", no machines =" << nMachines << '\n';

                    newTest->setMaxTestPerTimestep(maxTestPerTimeStep);
    
                    // store with list of available tests
                    _testsContainer.push_back(newTest);
                }
            }

            _noPersonel = 1;

            //for (auto bob : _testsContainer) {
            //    std::cout << "stored tests  test name = " << bob->getTestName() << ", disease = " << bob->getDiseaseName() << ", pri = " << bob->getPriority() << " \n";
            //}
}



Testing::Testing(string facilityName, vector<string> diseaseList, vector<Test*> tests, vector <facilityAttributes*> attrPtr) {

    int nMachines;
    int maxTestPerTimeStep;
    int maxTestPerCycle;
    bool testing;
    bool debug = false;

    // Facility name
    _facility = facilityName;

    // create a test agent for each disease

       for (auto test : tests) {

            // copy test agent into a new test agent
            Test* newTest = new Test(*test);

            // default to 1
            nMachines = 1;

            // get max no. of tests that can be performed in 1 reagent cycle
            maxTestPerCycle = test->getMaxTestPerCycle();

            // find default testing
            testing = test->getDiseaseTesting();
            
            if (debug) std::cout << "before loop " << '\n';

            // get the no. of machines for this facility to calculate the max tests per time step
            for (auto a : attrPtr) {

                if (debug) std::cout << "loop disease match = " << test->getTestName() << ", a->disease = " << a->disease << ", nomachines = " << a->noMachines << '\n';

                if (test->getDiseaseName() == a->disease) {

                    if (test->getTestName() == a->testName) {

                        // found a facilityAttributes object that matches disease and test - get no. of machines from it
                        nMachines = a->noMachines;

                        maxTestPerCycle = a->maxReagentPerCycle;

                        testing = a->testing;
                    }
                }
            }

            // calculate no. of reagents per day based off of no. of machines * no. of test cycles a day * no. tests per machine at one time
            maxTestPerTimeStep = test->getTestsPerMachine() * test->getCyclesPerTimestep() * nMachines;

            if (debug) std::cout << "max = " << maxTestPerTimeStep << ", tests per machine = " << test->getTestsPerMachine() << ", cycles per timestep =" << test->getCyclesPerTimestep();
            if (debug) std::cout << ", no machines =" << nMachines << ", max tests in cycle(test) = " << test->getMaxTestPerCycle() << ", max tests in cycle(attr) = " << maxTestPerCycle << '\n';

            newTest->setMaxTestPerTimestep(maxTestPerTimeStep);

            newTest->setMaxTestPerCycle(maxTestPerCycle);
                    
            newTest->setDiseaseTesting(testing);
            
             // store with list of available tests
            _testsContainer.push_back(newTest);
       }

    _noPersonel = 1;

    if (debug)  {
    
        for (auto bob : _testsContainer) {
            std::cout << "stored tests  test name = " << bob->getTestName() << ", disease = " << bob->getDiseaseName() << ", pri = " << bob->getPriority() << " \n";
        }
    }
}





// update Samples for this time step
void Testing::resetResources(int timeStep, string disease) {

      // reset resources for all diseases
      
      // loop through each of the tests and reset today, reset reagents if mod of reagent cycle time
      for (auto test : _testsContainer) {

            // reset no. of tests done today
            test->resetTestsThisTimestep();

            // check if time to restock reagents
            if (test->getReagentCycle() > 0) {
                if (timeStep % test->getReagentCycle() == 0)
                {
                    test->resetTestsThisReagentCycle();
                }
            }
      }

}

// update Samples for this time step
void Testing::resetResources(int timeStep) {

            // loop through each of the tests and reset today, reset reagents if mod of reagent cycle time
            for (auto test : _testsContainer) {

                // reset no. of tests done today
                test->resetTestsThisTimestep();

                // check if time to restock reagents
                if (test->getReagentCycle() > 0) {
                    if (timeStep % test->getReagentCycle() == 0)
                    {
                        test->resetTestsThisReagentCycle();
                    }
                }
            }

}


// check to see if available resources to do test for this disease
bool Testing::checkResources(string disease) {

    bool useSecondary = false;
    bool success = false;
    bool debug = false;

    // Only primary test is used until all reagents in this reagent cycle are used up
    // if used up then secondary test is used
    // doctors will hold a sample until the next timestep to use an expert machine if capacity is met for current timestep

    if (debug) std::cout << "inside check resources "  << " \n";

    // loop through each of the tests, match up the disease name  - check primary tests status
    for (auto test : _testsContainer) {

        // see if any reagents left for any of the tests this cycle
        if (disease == test->getDiseaseName()) {

            if (debug) std::cout << "disease match = " << disease <<  ", test name = " << test->getTestName() << " \n";
            
            // find the priority test and make sure testing for it is available
            if ((test->getPriority() == 1) && (test->getDiseaseTesting())) {

                if (debug) std::cout << "reagent tests this cycle = " << test->getTestsThisReagentCycle() << ", max test this cycle = " << test->getMaxTestPerCycle() << " \n";

                // any reagents left this reagent cycle
                if (test->getTestsThisReagentCycle() < test->getMaxTestPerCycle()) {

                    if (debug) std::cout << "tests this time step = " << test->getTestsThisTimestep() << ", max test this time step = " << test->getMaxTestPerTimestep() << " \n";

                    // still reagents left - see how many tests have been done today already
                    if (test->getTestsThisTimestep() < test->getMaxTestPerTimestep()) {
                        success = true;

                        break;
                    }
                }
                else {

                    // look at secondary tests resources
                    useSecondary = true;
                }
            }
            else {

                // look at secondary tests resources
                useSecondary = true;
            }
        }
    }

    // if out of primary test resouces this reagent cycle, check secondary test
    if (useSecondary) {

        if (debug) std::cout << "no primary tests - check secondary tests " << " \n";
        
        // loop through each of the tests, match up the disease name
        for (auto test : _testsContainer) {

            // see if any reagents left for any of the tests this cycle
            if (disease == test->getDiseaseName()) {

                if (debug) std::cout << "secondary tests disease match = " << disease << ", test name = " << test->getTestName() << ", priority = " << test->getPriority() <<", testing = " << test->getDiseaseTesting() << " \n";
                
                // find the priority test
                if ((test->getPriority() == 2) && (test->getDiseaseTesting())) {

                    if (debug) std::cout << "secondary test cur no. tests this cycle = " << test->getTestsThisReagentCycle() << ", max test this cycle = " << test->getMaxTestPerCycle() <<", testing = " << test->getDiseaseTesting()<< " \n";
                    
                    // any reagents left this reagent cycle
                    if (test->getTestsThisReagentCycle() < test->getMaxTestPerCycle()) {

                        if (debug) std::cout << "secondary tests time step = " << test->getTestsThisTimestep() << ", max test this time step = " << test->getMaxTestPerTimestep() << " \n";
                        
                        // still reagents left - see how many tests have been done today already
                        if (test->getTestsThisTimestep() < test->getMaxTestPerTimestep()) {
                            success = true;

                            break;
                        }
                    }
                }
            }
        }
    }

    return success;
}

// check to see if available resources to do test for this disease
bool Testing::checkResources(string disease, string testName) {

    bool success = false;
    bool debug = false;
    
    // Only primary test is used until all reagents in this reagent cycle are used up
    // if used up then secondary test is used
    // doctors will hold a sample until the next timestep to use an expert machine if capacity is met for current timestep

    //if (timeStep > 365) debug = true;
    
    if (debug) std::cout << "inside check resources - disease = " << disease << " testName = " << testName << " \n";

    // loop through each of the tests, match up the disease and test names
    for (auto test : _testsContainer) {

        if (debug) std::cout << "inside check resources loop - disease = " << test->getDiseaseName() << " test = " << test->getTestName() << " \n";

        // see if any reagents left for any of the tests this cycle
        if ((disease == test->getDiseaseName()) && (testName == test->getTestName())) {

            // debug - true
            if (debug) {
            
                std::cout << "disease match = " << disease <<  ", test name = " << test->getTestName();
                std::cout << " reagent tests this cycle = " << test->getTestsThisReagentCycle() << ", max test this cycle = " << test->getMaxTestPerCycle();
                std::cout << " tests this time step = " << test->getTestsThisTimestep() << ", max test this time step = " << test->getMaxTestPerTimestep() << " \n";
            }

            // any reagents left this reagent cycle
            if (test->getTestsThisReagentCycle() < test->getMaxTestPerCycle()) {

                if (debug) std::cout << "tests this time step = " << test->getTestsThisTimestep() << ", max test this time step = " << test->getMaxTestPerTimestep() << " \n";

                // still reagents left - see how many tests have been done today already
                if (test->getTestsThisTimestep() < test->getMaxTestPerTimestep()) {
                    success = true;

                    break;
                }
                else {

                    //std::cout << "all tests done for today - need to wait for tomorrow - test =  " << test->getTestName() <<  ", disease  " << disease << " no machines =  " << test->getNoMachines() << " max tests per timestep =  " << test->getMaxTestPerTimestep() << " \n";
                }
            }
            else {

                 // ERROR MESSAGE
                 //std::cout << "test " << test->getTestName() <<  ", ran out of reagents for  " << disease << " \n";
            }
        }
    }

    return success;
}




// increment testing resources for a test
Test* Testing::incrementResourcesDisease(string disease) {

    int curPriority = 10;
    bool foundDisease = false;
    Test* curPtr = NULL;
    string testName = "";
    bool debug = false;

    if (debug) std::cout << "inside increment resources Disease " << " \n";

    // loop through each of the tests, match up the disease name, do highest priority first
    for (auto test : _testsContainer) {

        if (disease == test->getDiseaseName()) {

            if (debug) std::cout << "disease = " << disease << ", test name = " << test->getTestName() <<  ",priority = " << test->getPriority()  << ", testing = " << test->getDiseaseTesting() << " \n";
            
            // find test with lowest priority
            if ((test->getPriority() < curPriority) && (test->getDiseaseTesting())) {

                if (debug) std::cout << "lower priority curPriority =  " << curPriority << " \n";

                // any tests left at higher priority = then use it, otherwise keep same priority
                if (test->getTestsThisTimestep() < test->getMaxTestPerTimestep()) {

                    curPriority = test->getPriority();

                    curPtr = test;

                    foundDisease = true;
                }
            }

        }
    }
    

    // check to see if test for disease found and any reagents are left
    if (foundDisease) {

        if (debug) std::cout << "increment resources curPriority = " << curPriority << ", foundDisease = " << foundDisease << ", test name = " << curPtr->getTestName() << " \n";
        
        // increment no. of tests done this reagent cycle
        curPtr->incrementTests();

        // get test name
        //testName = curPtr->getTestName();  
    }

    return curPtr;
}


// increment testing resources for a test
Test* Testing::incrementResourcesTest(string testName) {

    Test* curPtr = NULL;
    string disease = "";
    bool debug = false;

    if (debug) std::cout << "inside increment resources Test " << " \n";

    // loop through each of the tests, match up the test name, do highest priority first
    for (auto test : _testsContainer) {

        if (testName == test->getTestName()) {

            // found test, update pointer
            curPtr = test;
            
            if (debug) std::cout << "test = " << testName << ", disease name = " << test->getDiseaseName() << ",priority = " << test->getPriority() << ", testing = " << test->getDiseaseTesting() << " \n";

            // any tests left at higher priority = then use it, otherwise keep same priority
            if (test->getTestsThisTimestep() < test->getMaxTestPerTimestep()) {

                // increment no. of tests done this reagent cycle
                curPtr->incrementTests();

                // get test name
                //testName = curPtr->getTestName();
            }
        }
    }

    return curPtr;
}


// increment testing resources for a test
Test* Testing::incrementResources(string disease, string testName) {

    Test* curPtr = NULL;
    bool debug = false;
    
    if (debug) std::cout << "inside increment resources " << " \n";

    // loop through each of the tests, match up the disease name, do highest priority first
    for (auto test : _testsContainer) {

        // first find out if test name matches
        if (testName == test->getTestName()) {

            // if disease matches, count the test and note the reagent being used
            if (disease == test->getDiseaseName()) {
                if (debug) std::cout << "disease = " << disease << ", test name = " << test->getTestName() << ",priority = " << test->getPriority() << ", testing = " << test->getDiseaseTesting() << " \n";

                // find test
                if (test->getDiseaseTesting()) {

                    if (debug) std::cout << "get disease testing " << " \n";

                    // any tests left then use it
                    if (test->getTestsThisTimestep() < test->getMaxTestPerTimestep()) {

                        curPtr = test;

                        // increment no. of tests done this reagent cycle
                        curPtr->incrementTests();

                        // keep track of most number of tests in any timestep - used to determine max resourses
                        if (curPtr->getMaxTestsAllTimesteps() < curPtr->getTestsThisTimestep()) {

                            // update max no. of tests for any timestep if no. of tests for this time step is greater
                            curPtr->setMaxTestsAllTimesteps(curPtr->getTestsThisTimestep());
                        }

                        // keep track of most number of tests in any cycle - used to determine max resourses
                        if (curPtr->getMaxTestsAllCycles() < curPtr->getTestsThisReagentCycle()) {

                            // update max no. of tests for any timestep if no. of tests for this time step is greater
                            curPtr->setMaxTestsAllCycles(curPtr->getTestsThisReagentCycle());
                        }
                        if (debug) std::cout << "get curPtr " << curPtr << " \n";
                    }
                }
            }
            else {

                // if disease doesn't match, count the test but don't reduce reagents for this disease
 
                // any tests left then use it
                if (test->getTestsThisTimestep() < test->getMaxTestPerTimestep()) {

                    // increment no. of tests done this reagent cycle
                    test->incrementTestsNotReagents();

                    // keep track of most number of tests in any timestep - used to determine max resourses
                    if (test->getMaxTestsAllTimesteps() < test->getTestsThisTimestep()) {

                        // update max no. of tests for any timestep if no. of tests for this time step is greater
                        test->setMaxTestsAllTimesteps(test->getTestsThisTimestep());
                    }

                    // keep track of most number of tests in any cycle - used to determine max resourses
                    if (test->getMaxTestsAllCycles() < test->getTestsThisReagentCycle()) {

                        // update max no. of tests for any timestep if no. of tests for this time step is greater
                        test->setMaxTestsAllCycles(test->getTestsThisReagentCycle());
                    }
                }
            }
        }
    }

    return curPtr;
}





// get sensitivity for this disease test
float Testing::getSensitivityPercent(string disease) {

    float sensitivity = 100;
    int curPriority = 10;
    bool foundDisease = false;
    Test* curPtr;

    // loop through each of the tests, match up the disease name, do highest priority
    for (auto test : _testsContainer) {

        if (disease == test->getDiseaseName()) {

            // find test with lowest priority
            if (test->getPriority() < curPriority) {
                curPriority = test->getPriority();

                curPtr = test;

                foundDisease = true;
            }

        }
    }

    // check to see if test for disease found and return sensitivity for it
    if (foundDisease) {

        // increment no. of tests done this reagent cycle
        sensitivity = curPtr->getSense() * 100;

    }
    return sensitivity;
}

// get sensitivity for this disease test
float Testing::getSensitivityPercent(string disease, string testName) {

    float sensitivity = 100;

    // loop through each of the tests, match up the disease and test names
    for (auto test : _testsContainer) {

        if ((disease == test->getDiseaseName()) && (testName == test->getTestName())) {

            sensitivity = test->getSense() *100;
        }
    }

    return sensitivity;
}


// get sensitivity for this disease test - for doctor's diagnosis
float Testing::getSensitivityMDPercent(string disease) {

    float sensitivity = 100;
    int curPriority = 10;
    bool foundDisease = false;
    Test* curPtr;

    // loop through each of the tests, match up the disease name, do highest priority
    for (auto test : _testsContainer) {

        if (disease == test->getDiseaseName()) {

            // find test with lowest priority
            if (test->getPriority() < curPriority) {
                curPriority = test->getPriority();

                curPtr = test;

                foundDisease = true;
            }

        }
    }

    // check to see if test for disease found and return MD sensitivity for it
    if (foundDisease) {

        // increment no. of tests done this reagent cycle
        sensitivity = curPtr->getSenseMD() * 100;

    }
    return sensitivity;
}

// get sensitivity for this disease test - for doctor's diagnosis
float Testing::getSensitivityMDPercent(string disease, string testName) {

    float sensitivity = 100;

    // loop through each of the tests, match up the disease and test names=
    for (auto test : _testsContainer) {

        if ((disease == test->getDiseaseName()) && (testName == test->getTestName())) {

            sensitivity = test->getSenseMD() * 100;
        }
    }
    return sensitivity;
}


// get specificity for this disease test
float Testing::getSpecificityPercent(string disease) {

    float specificity = 100;
    int curPriority = 10;
    bool foundDisease = false;
    Test* curPtr;

    // loop through each of the tests, match up the disease name, do highest priority
    for (auto test : _testsContainer) {

        if (disease == test->getDiseaseName()) {

            // find test with lowest priority
            if (test->getPriority() < curPriority) {
                curPriority = test->getPriority();

                curPtr = test;

                foundDisease = true;
            }

        }
    }

    // check to see if test for disease found and return specificity
    if (foundDisease) {

        // increment no. of tests done this reagent cycle
        specificity = curPtr->getSpecif() * 100;

    }
    return specificity;
}


// get specificity for this disease test
float Testing::getSpecificityPercent(string disease, string testName) {

    float specificity = 100;

    //std::cout << "inside getSpecificity percent disease = " << disease << " testName = " << testName << " \n";

    // loop through each of the tests, match up the disease name, do highest priority
    for (auto test : _testsContainer) {
        
        if ((disease == test->getDiseaseName()) && (testName == test->getTestName())) {

            specificity = test->getSpecif()* 100;
        }
    }

    return specificity;
}



// get specificity for this disease test - for doctor's diagnosis
float Testing::getSpecificityMDPercent(string disease) {

    float specificity = 100;
    int curPriority = 10;
    bool foundDisease = false;
    Test* curPtr;

    // loop through each of the tests, match up the disease name, do highest priority
    for (auto test : _testsContainer) {

        if (disease == test->getDiseaseName()) {

            // find test with lowest priority
            if (test->getPriority() < curPriority) {
                curPriority = test->getPriority();

                curPtr = test;

                foundDisease = true;
            }

        }
    }

    // check to see if test for disease found and return MD specificity for it
    if (foundDisease) {

        // increment no. of tests done this reagent cycle
        specificity = curPtr->getSpecifMD() *100;

    }
    return specificity;
}


// get specificity for this disease test - for doctor's diagnosis
float Testing::getSpecificityMDPercent(string disease, string testName) {

    float specificity = 100;

    // loop through each of the tests, match up the disease and test names
    for (auto test : _testsContainer) {

        if ((disease == test->getDiseaseName()) && (testName == test->getTestName())) {

            specificity = test->getSpecifMD() * 100;
        }
    }
    return specificity;
}



// update Samples for this time step
void Testing::updateSamples(int timeStep, vector<Disease*> dis) {

    int status;
    int counter = 0;
    int randInt;
    int transitTime;
    int batchWaitTime;
    int distIndex = 0;
    int regIndex = 0;
    int facIndex = 0;
    int origFacilityTier = 0;
    float randFloat;
    float randFloat2;
    float diagnosisPer;
    float inter;
    float preTestLtoFPer = 0;
    float preTestMDCDInfPer = 0;
    float preTestMDCDUnInfPer = 0;
    Sample* sample;
    Sample* sample1;
    string facName;
    Facility* f;
    string testName;
    string diseaseName;
    string origFacilityRegion;
    bool debugOn = false;
    Test* testPtr;
    Patient* patPtr;
    Stats* stats = NULL;
    Stats* facilityStats = NULL;

    //vector<Sample*> samplesToDelete;

    if (debugOn) std::cout << "update Samples size before = " << _sampleContainer.size() << " \n";
    //unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    //std::mt19937 generator(seed);

    // need to reorder Samples - oldest to newest - highest priority first
    //reorderSamples();
    
    
    if (debugOn) {
    
        std::cout << "before ";
        printSamples();
    }

    // reverse the sample list
    //reverseSamples();
    //std::reverse(_sampleContainer.begin(), _sampleContainer.end());

    // bring back 20 to front of sample list
    //bringLastSamplesToFront2();

    
    if (debugOn) {
    
        std::cout << "after ";
        printSamples();
    }

    // using built-in random generator: doesn't seem to work
    //std::random_shuffle(_sampleContainer.begin(), _sampleContainer.end());

    // loop through all Samples at this Facility - reverse order to get oldest ones first
    //for (auto sample : _sampleContainer) {
    for (auto rit = _sampleContainer.rbegin(); rit != _sampleContainer.rend(); ++rit) {

        sample = *rit;
        
        if (debugOn) {
            std::cout << "**** updateSample, sample list size = " << _sampleContainer.size() << " index = " << counter << ", facility = " << _facility << ", sample = " << sample << " \n";
            
            for (auto sit = _sampleContainer.rbegin(); sit != _sampleContainer.rend(); ++sit) {

                sample1 = *sit;
                std::cout << " sit= " << sample1;
                //std::cout << " patient ptr = " << sample1->getPatientPtr();
            }
            std::cout << "\n";
        }

        // make it older by 1 time step
        sample->incrementAge();
        if (debugOn) std::cout << "updateSample, after increment age " <<  " \n";

        // get Sample status
        status = sample->getStatus();

        // get tier of facility where sample was taken
        origFacilityTier = sample->getOrigFacilityTier();
        origFacilityRegion = sample->getOrigFacilityRegion();

        // get the patient pointer so that we can update sample status for patient
        patPtr = sample->getPatientPtr();
        if (debugOn) std::cout << "updateSample, after patPtr " << " \n";

        // find the region and district indices for stats counting
        distIndex = patPtr->getFacilityDistrictIndex();
        regIndex = patPtr->getFacilityRegionIndex();
        facIndex = patPtr->getFacilityID();

        if (debugOn) std::cout << "updateSample, after getRegion " << " \n";
        
        // get disease from Sample
        diseaseName = sample->getDisease();
        if (debugOn) std::cout << "updateSample, after getdisease " << " \n";

        //find pointer for the disease associated with sample
        for (auto d : dis) {

            if (d->getName() == diseaseName) {

                // for 2020 and later the specimen transport system changed and pre-test loss to follow up changed.
                // prePreTestLtoFReduction allows the preTestLtoF percentage to change to reflect this.  The default is 100%
                // so the preTestLtoF stays the unchanged if it isn't explicitly set
                if ((referenceYear + currentYearInRun) > 2019) {

                    //preTestLtoFPer = d->getPerPreTestLtoFTier(origFacilityTier) * (d->getPerPreTestLtoFReduction()/100);
                    preTestLtoFPer = d->getPerPreTestLtoFTierRegion(origFacilityRegion, origFacilityTier) * (d->getPerPreTestLtoFReduction() / 100);
                }
                else {

                    //preTestLtoFPer = d->getPerPreTestLtoFTier(origFacilityTier);
                    preTestLtoFPer = d->getPerPreTestLtoFTierRegion(origFacilityRegion, origFacilityTier);
                }

                preTestMDCDInfPer = d->getPerPreTestMDCDInf() * 100;
                preTestMDCDUnInfPer = d->getPerPreTestMDCDUnInf() * 100;
                
                stats = d->getStatsPtr();
                facilityStats = d->getFacStatsPtr();

                // debugOn - true
                if (debugOn) std::cout << "pre-test ltof per " << preTestLtoFPer << ", preTestPer " << d->getPerPreTestLtoFTierRegion(origFacilityRegion, origFacilityTier) << ", origFacilityTier " << origFacilityTier << ", origFacilityRegion " << origFacilityRegion << ", prePreTestPer " << d->getPerPreTestLtoFReduction() <<  " \n";
            }
        }

        // debugOn - true
        if (debugOn) std::cout << "Final pre-test ltof per " << preTestLtoFPer << " preTestMDCDInfPer " << preTestMDCDInfPer << " preTestMDCDUnInfPer " << preTestMDCDUnInfPer << " \n";

        counter++;
        if (debugOn) {
            std::cout << "updateSample, counter = " << counter << " ,sample age = " << sample->getAge() << " ,status = " << status << ", Test name = " << sample->getTest() << " \n";
            std::cout << "inside Testing updateSamples status = " << status << " \n";
            std::cout << "sense = " << getSensitivityPercent(sample->getDisease()) << ", specif = " << getSpecificityPercent(sample->getDisease()) << " \n";
        }
        
        // sample states
        // int SampCreatedUnTest = 0;       // created but untested
        // SampCreatedUnTestBatch = 1;		// created, untested waiting to be sent out in batch mode
        // SampUnTestInTransit = 2;	        // in transit
        // SampUnTestWaitRepresent = 3;	    // waiting for patient to represent
        // SampWaitForTest = 4;		        // waiting for test
        // SampLostExpired = 5;		        // lost/expired
        // SampTestedInTransit = 6;	        // results in transit back to orig facility
        // SampReturnedAvail = 7;	        // results returned - available
        // SampReturnedLostExpired = 8;	    // loss to follow up
        // SampLtoF = 9;			        // loss to follow up
        // SampPatDead = 10;			    // patient dead
        // SampPatuntreatedInactive = 11;	// patient went untreated, sample now longer activeve

        if (false) {

            std::cout << "inside Testing updateSamples status = " << status << " \n";
            
            for (auto test : _testsContainer) {

                std::cout << "disease = " << test->getDiseaseName() << ", test name = " << test->getTestName() << ",priority = " << test->getPriority() << ", testing = " << test->getDiseaseTesting() << " test1 = " << test->getTestsThisTimestep() << " test2 = " << test->getMaxTestPerTimestep() << " \n";
            }
            //std::cout << "inside Testing updateSamples status = 0  after get testPtr, test = " << sample->getTest() << ", disease = " << sample->getDisease() << ", testPtr = " << testPtr << " \n";
        }
        
        switch (status) {
        case 0:   // SampCreatedUnTest -new - testing available here in-house

            //if (timeStep > 365) debugOn = true;

            // debugOn - true
            if (debugOn) {

                std::cout << "inside Testing updateSamples status = 0, before check resources " << " disease = " << sample->getDisease() << " \n";
                
                // loop through each of the tests, match up the disease and test names
                for (auto test : _testsContainer) {

                    // see if any reagents left for any of the tests this cycle
                    if ((sample->getDisease() == test->getDiseaseName()) && (sample->getTest() == test->getTestName())) {

                        std::cout << "disease match = " << test->getDiseaseName() << ", test name = " << test->getTestName();
                        std::cout << " reagent tests this cycle = " << test->getTestsThisReagentCycle() << ", max test this cycle = " << test->getMaxTestPerCycle();
                        std::cout << " tests this time step = " << test->getTestsThisTimestep() << ", max test this time step = " << test->getMaxTestPerTimestep() << " \n";
                    }
                }
            }

            // check to see if there are still tests available for this time step
            if (checkResources(sample->getDisease(), sample->getTest())) {

                if (debugOn) std::cout << "inside Testing updateSamples status = 0  resources available" << " \n";
                
                // update resources to indicate a sample was tested
                testPtr = incrementResources(sample->getDisease(), sample->getTest());
                //testName = incrementResources2(sample->getDisease());

                if (debugOn) {

                    for (auto test : _testsContainer) {

                        std::cout << "disease = " << test->getDiseaseName() << ", test name = " << test->getTestName() << ",priority = " << test->getPriority() << ", testing = " << test->getDiseaseTesting() << " test1 = " << test->getTestsThisTimestep() << " test2 = " << test->getMaxTestPerTimestep() << " \n";
                    }
                    std::cout << "inside Testing updateSamples status = 0  after get testPtr, test = " << sample->getTest() << ", disease = " << sample->getDisease() << ", testPtr = " << testPtr << " \n";
                }

                // set time sample is tested
                sample->setTestedTime(timeStep);

                // change from new to returned
                sample->setStatus(SampReturnedAvail);
                patPtr->setSampleTestStatus("SampReturnedAvail");

                if (debugOn) std::cout << "inside Testing updateSamples status = 0  after set sample status" << " \n";

                // mark time results are returned to facility
                sample->setResultsReturnedtoOrigFacilityTime(timeStep);

                if (debugOn) std::cout << "inside Testing updateSamples status = 0  after set sample returned" << " \n";

                // update test used for sample - watch for possible NULL pointer out of incrementResources
                testName = testPtr->getTestName();
                if (debugOn) std::cout << "inside Testing updateSamples status = 0  resources available, test name = " << testName <<  " \n";

                sample->setTest(testName);
                sample->setTestPtr(testPtr);

                // record test as in-house and total tests
                if (statsCount) {
                
                    stats->record_nTestsIn(distIndex, regIndex);
                    facilityStats->record_nTestsIn(facIndex);
                    stats->reduce_sPatientWaitingIn(distIndex, regIndex);
                    facilityStats->reduce_sPatientWaitingIn(facIndex);
                    stats->reduce_sSampleWaitingForTestIn(distIndex, regIndex);
                    facilityStats->reduce_sSampleWaitingForTestIn(facIndex);

                    if (sample->getInfected()) {

                        if (debugOn) std::cout << "inside Testing present to sample time = " << timeStep - patPtr->getTimestepPresented() << " \n";

                        stats->record_timePresentToSampleTestIn(timeStep - patPtr->getTimestepPresented(), distIndex, regIndex);
                        facilityStats->record_timePresentToSampleTestIn(timeStep - patPtr->getTimestepPresented(), facIndex);
                        stats->record_nPresentToSampleTestIn(distIndex, regIndex);
                        facilityStats->record_nPresentToSampleTestIn(facIndex);
                    }

                    stats->record_timeSampleResourceWaitIn(sample->getWaitForResource(), distIndex, regIndex);
                    facilityStats->record_timeSampleResourceWaitIn(sample->getWaitForResource(), facIndex);

                    if (testName == "Xpert") {
                    
                        stats->record_nXpertIn(distIndex, regIndex);
                        facilityStats->record_nXpertIn(facIndex);
                    }
                    else {

                        stats->record_nSmearIn(distIndex, regIndex);
                        facilityStats->record_nSmearIn(facIndex);
                    }
                }

                // debugOn true false
                if (debugOn) std::cout << "inside Testing updateSamples testName = " << testName << ", orig fac = " << sample->getOrigFacility() << ", test fac = " << sample->getTestingFacility() << " \n";

                // update return transit time
                transitTime = 0;
                sample->setReturnTransitTime(transitTime);
                sample->setReturnTransitTimeSave(transitTime);

                //if (patPtr->getInfected()) std::cout << "stats: before pat pos = "  << patPtr->getTestPos() << " pat infected = " << patPtr->getInfected() << " samp pos = " << sample->isPositive() << " samp infected = " << sample->getInfected() << "\n";
                
                // test result positive or negative 
                //std::uniform_int_distribution<int> distribution(1, 100);
                //randInt = distribution(generator);
                randInt = getNextRandomNo100()-1;
                randFloat = float(getNextRandomNo100()-1) + float(randInt) / 100;
                if (randFloat > 100) randFloat = 100;

                if (debugOn) std::cout << "testing in-house sensitivity = " << getSensitivityPercent(sample->getDisease(), sample->getTest()) << " randFloat = " << randFloat << "\n";

                if (sample->getInfected()) {

                    // infected can only be true-positive or false-negative
                    
                    // if infected get sensitivity for test
                    if (randFloat <= getSensitivityPercent(sample->getDisease(), sample->getTest())) {

                        sample->setPositive(true);
                        patPtr->setTestPos(true);

                        if (statsCount) {

                            stats->record_nTestTruePosIn(distIndex, regIndex);
                            facilityStats->record_nTestTruePosIn(facIndex);
                            stats->record_nTestPosIn(distIndex, regIndex);
                            facilityStats->record_nTestPosIn(facIndex);
                            stats->record_nDiagnosisIn(distIndex, regIndex);
                            facilityStats->record_nDiagnosisIn(facIndex);

                            if (testName == "Xpert") {
                    
                                stats->record_nXpertTruePos(distIndex, regIndex);
                                facilityStats->record_nXpertTruePos(facIndex);
                            }
                            else {

                                stats->record_nSmearTruePos(distIndex, regIndex);
                                facilityStats->record_nSmearTruePos(facIndex);
                            }
                        }
                    }
                    else {
                        //std::cout << "infected with neg test result " << " \n";

                        if (statsCount) {

                            stats->record_nTestFalseNegIn(distIndex, regIndex);
                            facilityStats->record_nTestFalseNegIn(facIndex);

                            // false negative work the same as pre-treat loss to follow up
                            //stats->record_nPreTreatLtFU(distIndex, regIndex);
                            //stats->record_nPreTreatLtFUFN(distIndex, regIndex);

                            if (testName == "Xpert") {
                    
                                stats->record_nXpertFalseNeg(distIndex, regIndex);
                                facilityStats->record_nXpertFalseNeg(facIndex);
                            }
                            else {

                                stats->record_nSmearFalseNeg(distIndex, regIndex);
                                facilityStats->record_nSmearFalseNeg(facIndex);
                            }
                        }
                    }
                }
                else {

                    // non-infected can only be false-positive or true-negative
                    
                    //std::cout << "stats: specificty = "  << getSpecificityPercent(sample->getDisease(), sample->getTest()) << " randInt = " << randInt << "\n";
                    
                    // if non-infected get specificity for test
                    if (randFloat > getSpecificityPercent(sample->getDisease(), sample->getTest())) {

                        sample->setPositive(true);
                        patPtr->setTestPos(true);

                        if (statsCount) {

                            stats->record_nTestFalsePosIn(distIndex, regIndex);
                            facilityStats->record_nTestFalsePosIn(facIndex);
                            stats->record_nTestPosIn(distIndex, regIndex);
                            facilityStats->record_nTestPosIn(facIndex);
                            stats->record_nDiagnosisIn(distIndex, regIndex);
                            facilityStats->record_nDiagnosisIn(facIndex);

                            if (testName == "Xpert") {
                    
                                stats->record_nXpertFalsePos(distIndex, regIndex);
                                facilityStats->record_nXpertFalsePos(facIndex);
                            }
                            else {

                                stats->record_nSmearFalsePos(distIndex, regIndex);
                                facilityStats->record_nSmearFalsePos(facIndex);
                            }
                        }
                    }
                    else {

                        if (statsCount) {

                            stats->record_nTestTrueNegIn(distIndex, regIndex);
                            facilityStats->record_nTestTrueNegIn(facIndex);

                            if (testName == "Xpert") {
                    
                                stats->record_nXpertTrueNeg(distIndex, regIndex);
                                facilityStats->record_nXpertTrueNeg(facIndex);
                            }
                            else {

                                stats->record_nSmearTrueNeg(distIndex, regIndex);
                                facilityStats->record_nSmearTrueNeg(facIndex);
                            }
                        }
                    }
                }
                //if (!sample->getInfected() && sample->isPositive()) std::cout << "stats: pat pos = "  << patPtr->getTestPos() << " pat infected = " << patPtr->getInfected() << " samp pos = " << sample->isPositive() << " samp infected = " << sample->getInfected() << "\n";
            }
            else {

                // debugOn - true 
                if (true) std::cout << "inside Testing updateSamples status = 0  no resources available - wait = " << sample->getWait() << " disease = " << sample->getDisease() << " test = " << sample->getTest() << " test facility = " << sample->getTestingFacility() << " \n";

                // no tests left - keep in new status and update age
                sample->incrementWait();
                sample->incrementWaitForResource();

                // consider this transit time
                sample->setReturnTransitTimeSave(sample->getWait());
            }
            break;
        case 1:  // SampCreatedUnTestBatch - created, untested waiting for batch transport

            //std::cout << "inside Testing updateSamples status = 1, batchWaittime = " << sample->getBatchWaitTime() << " timestep = " << timeStep << " \n";
            
            if (timeStep % sample->getBatchWaitTime() == 0) {

                // time to send out batch of samples - transit time already set, already in testing facility sample container, just put status into transit
                sample->setStatus(SampUnTestInTransit); // set status to in transit
                patPtr->setSampleTestStatus("SampUnTestInTransit");
                //std::cout << "inside Testing updateSamples timestep%2 = 0 "  << " \n";
            }
            else {

                // batch time not here yet - do nothing
            }
            break;
        case 2:   // SampUnTestInTransit - in transit
            // check no. of days in transit
            if (sample->getTransitTime() < 1) {

                // change from in transit to waiting to be tested
                sample->setStatus(SampWaitForTest);
                patPtr->setSampleTestStatus("SampWaitForTest");

                // debugOn -true
                if (debugOn) std::cout << "inside update sample switching from in transit to wait for test present time =  " <<  patPtr->getTimestepPresented() << " timestep = " << timeStep <<  "\n";
            }
            else {
                // percentage of samples get lost in transit - Now covered in pre-diagnostic loss to follow up

                // leave in transit - decrement transit time
                sample->reduceTransitTime();

                if (debugOn) std::cout << "inside update sample reducine in transit time " << patPtr->getTimestepPresented() << " timestep = " << timeStep << "\n";
            }
            break;
        case 3:   // SampUnTestWaitRepresent - waiting for patient to re-present

            //std::cout << "patient representing after pre-diagnostic loss to followup sample time = " << patPtr->getFacilityReturnTime() << " \n";

            // check no. of days waiting for representation
            if (patPtr->getFacilityReturnTime() < 2) {

                // assume patient will represent at same facility
                // reset time patient presents and when sample gets created
                patPtr->setTimestepPresented(timeStep);
                sample->setCreateTime(timeStep);

                //if (patPtr->getFacility() != patPtr->getSampleTestFacility()) {
                if (debugOn) {
                    if (patPtr->getPreTestMDCDFlag()) {

                        std::cout << "inside Testing updateSamples MDCD change create time = " << patPtr->getTimestepCreated() << ", present time = " << patPtr->getTimestepPresented() << ", orig present = ";
                        std::cout << patPtr->getTimestepOrigPresented() << ", sampc = " << sample->getCreateTime() << ", sampt = " << sample->getTestedTime();
                        std::cout << ", md cd = " << patPtr->getPreTestMDCDTime() << ", p to m = ";
                        std::cout << patPtr->getPreTestMDCDTime() - patPtr->getTimestepOrigPresented() << ", m to r = ";
                        std::cout << patPtr->getTimestepPresented() - patPtr->getPreTestMDCDTime() << " \n";
                    }
                    else {

                        std::cout << "inside Testing updateSamples repre change create time = " << patPtr->getTimestepCreated() << ", present time = " << patPtr->getTimestepPresented() << ", orig present = ";
                        std::cout << patPtr->getTimestepOrigPresented() << ", sampc = " << sample->getCreateTime() << ", sampt = " << sample->getTestedTime() << ", p to r = ";
                        std::cout << patPtr->getTimestepPresented() - patPtr->getTimestepOrigPresented() << " \n";
                    }
                }

                // need to check whether testing is available at this facility or sample must be sent out
                if (patPtr->getFacility() == patPtr->getSampleTestFacility()) {

                    // change from waiting to re-present to in-house waiting for sample results
                    sample->setStatus(SampCreatedUnTest);
                    patPtr->setSampleTestStatus("SampCreatedUnTest");

                    if (statsCount) {

                        stats->record_sPatientWaitingIn(distIndex, regIndex); 
                        facilityStats->record_sPatientWaitingIn(facIndex);
                        stats->record_sSampleWaitingForTestIn(distIndex, regIndex);
                        facilityStats->record_sSampleWaitingForTestIn(facIndex);
                    }
                }
                else {
                    // update Sample status to in-transit - calculate transit time by distance to travel for testing
                    transitTime = sample->getTransitTimeSave();
                    batchWaitTime = sample->getBatchWaitTimeSave();

                    // testing done at another Facility
                    // Check to see if must wait for batch sending - determine this by checking timeStep mod batch interval
                    // if no remainder, send sample  - otherwise hold it
                    //debugOn - true
                    if (debugOn) std::cout << "inside update patient representing before batch wait timestep " << timeStep << " batchWaitTime = " << batchWaitTime << " transitTime = " << transitTime << " present time = " << patPtr->getTimestepPresented() << "\n";

                    if ((batchWaitTime == 0) || (timeStep % batchWaitTime == 0)) {

                        if (debugOn) std::cout << "inside create patient sending out sample " << "\n";

                        // time to send out batch of samples
                        sample->setStatus(SampUnTestInTransit); // set status to in transit

                        sample->setTransitTime(transitTime);

                        // set how long waiting in batch to 0
                        sample->setBatchWaitTimeSave(0);
                    }
                    else {

                        if (debugOn) std::cout << "inside create patient not yet hold sample " << "\n";

                        // not yet, hold sample until next time steps
                        sample->setStatus(SampCreatedUnTestBatch); // set status to waiting for batch transit

                        sample->setTransitTime(transitTime);

                        // set how long waiting in batch to days left before sending out in batch mode
                        sample->setBatchWaitTimeSave(timeStep % batchWaitTime);
                    }
                    // change from waiting to re-present to waiting for sample results
                    //sample->setStatus(SampWaitForTest);
                    patPtr->setSampleTestStatus("SampWaitForTest");

                    if (statsCount) {

                        stats->record_sPatientWaitingOut(distIndex, regIndex);
                        facilityStats->record_sPatientWaitingOut(facIndex);
                        stats->record_sSampleWaitingForTestOut(distIndex, regIndex);
                        facilityStats->record_sSampleWaitingForTestOut(facIndex);
                    }
                }
                // start tracking patients that are waiting for samples
                //patPtr->setTracked();

                //std::cout << "patient representing after pre-diagnostic loss to followup sample status = " << sample->getStatus() << " time = " << patPtr->getFacilityReturnTime() << " \n";
            }
            else {

                // leave waiting for representation - decrement representation time
                patPtr->decrementFacilityReturnTime();
            }
            break;
        case 4:   //SampWaitForTest -  waiting

            //std::cout << "waiting check resource = " << checkResources(sample->getDisease(), sample->getTest()) << " \n";

            // check to see if there are still tests available for this time step
            if (checkResources(sample->getDisease(), sample->getTest())) {

                // check to see if MD clinical diagnosis will occur before sample is sent out
                // if so, start patient on treatment - otherwise send sample through system
                randInt = getNextRandomNo100()-1;
                randFloat = float(getNextRandomNo100()-1) + float(randInt) / 100;
                if (randFloat > 100) randFloat = 100;

                randInt = getNextRandomNo100()-1;
                randFloat2 = float(getNextRandomNo100()-1) + float(randInt) / 100;
                if (randFloat2 > 100) randFloat2 = 100;

                if (sample->getInfected()) {
                    stats->record_nPTMDCDInfected(distIndex, regIndex);
                    facilityStats->record_nPTMDCDInfected(facIndex);
                }
                else {
                    stats->record_nPTMDCDUninfected(distIndex, regIndex);
                    facilityStats->record_nPTMDCDUninfected(facIndex);
                }
                //std::cout << randFloat << " \n";

                //std::cout << "randFloat " << randFloat << " preTestMDCDInfPer = " << preTestMDCDInfPer << " preTestMDCDUnInfPer = " << preTestMDCDUnInfPer << " \n";
                //std::cout << " randFloat = " << randFloat << " randFloat2 = " << randFloat2 << " inf = " << sample->getInfected() << " not = " << !sample->getInfected() << " \n";

                // for samples tested out - check to see if patient infected and probability below MDCD infected percentage
                if ((preTestMDCDInfPer > 0) && (randFloat <= preTestMDCDInfPer) && (sample->getInfected())) {

                    // debugOn - true
                    if (debugOn) std::cout << "SampleWaitForTest infected preTestMDCDInf and true pos randFloat = " << randFloat << " preTestMDCDInfPer = " << preTestMDCDInfPer * 100 << " infected = " << sample->getInfected() << " \n";

                    // update MD CD information
                    patPtr->setPreTestMDCDFlag(true);
                    patPtr->setPreTestMDCDTime(timeStep);

                    testPtr = sample->getTestPtr();

                    if (debugOn) std::cout << "false negative test = " << testPtr->getTestName() << ", randFloat = " << randFloat << ", diagnosisPer = " << diagnosisPer << ", inter = " << inter << "\n";
                    //std::cout << "false neg rand = " << randFloat << ", dignosis prob = " << diagnosisPer <<  "\n";
                    
                    // true positive - start treatment on patient
                    patPtr->setMDPos(true);

                    patPtr->setStatus(PatInfTestTreat);

                    patPtr->setReasonNoTreat("PatInfMDCDTreat");

                    if (statsCount) {

                        // record patients who should start treatment
                        stats->record_sPatientInTreatment(distIndex, regIndex);
                        facilityStats->record_sPatientInTreatment(facIndex);

                        stats->record_nDiagnosisOut(distIndex, regIndex);
                        facilityStats->record_nDiagnosisOut(facIndex);

                        stats->record_nPreTestDiagnosisTruePos(distIndex, regIndex);
                        facilityStats->record_nPreTestDiagnosisTruePos(facIndex);
                        stats->record_nPreTestDiagnosisTotal(distIndex, regIndex);
                        facilityStats->record_nPreTestDiagnosisTotal(facIndex);
                        stats->record_nPreTestDiagnosisReview(distIndex, regIndex);
                        facilityStats->record_nPreTestDiagnosisReview(facIndex);

                        //stats->record_nPTMDCDInfected(distIndex, regIndex);
                        stats->record_nMDCDInfected(distIndex, regIndex);
                        facilityStats->record_nMDCDInfected(facIndex);
                    }

                    // check to see if sample is loss to follow-up
                    randInt = getNextRandomNo100()-1;
                    randFloat = float(getNextRandomNo100()-1) + float(randInt) / 100;
                    if (randFloat > 100) randFloat = 100;

                    if (randFloat < preTestLtoFPer) {

                        // sample loss to follow-up
                        sample->setStatus(SampLostExpired);
                        patPtr->setSampleTestStatus("SampLostExpired");

                        patPtr->setPreTestLtoFFlag(true);

                        if (statsCount) {

                            stats->record_nPreTestLtFU(distIndex, regIndex);
                            facilityStats->record_nPreTestLtFU(facIndex);
                            stats->reduce_sSampleWaitingForTestOut(distIndex, regIndex);
                            facilityStats->reduce_sSampleWaitingForTestOut(facIndex);
                            stats->reduce_sPatientWaitingOut(distIndex, regIndex);
                            facilityStats->reduce_sPatientWaitingOut(facIndex);

                            if (sample->getInfected()) {

                                stats->record_nPreTestLtFUInf(distIndex, regIndex);
                                facilityStats->record_nPreTestLtFUInf(facIndex);
                            }
                        }
                        // update Sample back to original facility Sample list
                        // get pointer to Samples original Facility
                        f = facilitiesList->getPtr(sample->getOrigFacility());

                        // add to Sample list where it came from 
                        f->addToSampleContainer(sample);
                        //std::cout << "sample sent out, MD override, sample lost"  " \n";

                        // remove it from Sample list in this container
                        //samplesToDelete.push_back(sample);
                        auto it = std::find(_sampleContainer.begin(), _sampleContainer.end(), sample);
                        if (it != _sampleContainer.end()) {
                            _sampleContainer.erase(it);
                        }
                    }
                    else {

                        // sample results returned
                        testPtr = incrementResources(sample->getDisease(), sample->getTest());
                        // testName = incrementResources2(sample->getDisease());

                        if (statsCount) {

                            // debugOn - true
                            if (debugOn) std::cout << "inside update sample patient away after transit waiting for result timestep =  " << timeStep << ", present time = " << patPtr->getTimestepPresented() << "\n";

                            stats->record_timePresentToSampleDiagOut(timeStep - patPtr->getTimestepPresented(), distIndex, regIndex);
                            facilityStats->record_timePresentToSampleDiagOut(timeStep - patPtr->getTimestepPresented(), facIndex);
                            stats->record_nPresentToSampleDiagOut(distIndex, regIndex);
                            facilityStats->record_nPresentToSampleDiagOut(facIndex);
                        }

                        // set sample test time
                        sample->setTestedTime(timeStep);

                        // update test used for sample - watch for possible NULL pointer out of incrementResources
                        testName = testPtr->getTestName();

                        sample->setTest(testName);
                        sample->setTestPtr(testPtr);
                        //std::cout << "inside Testing updateSamples testName = " << testName << " \n";


                        

                        // calculate return transit time
                        //std::uniform_int_distribution<int> distribution(1, 14);
                        //transitTime = distribution(generator);
                        // if we want to add in later - change transit time to a random number
                        //transitTime = 1;
                        transitTime = sample->getTransitTimeSave();
                        //if (transitTime > 1) std::cout << "mdcd transit time = " << transitTime << "\n";

                        // update return transit time and set status to Sample in return transit
                        sample->setReturnTransitTime(transitTime);
                        sample->setReturnTransitTimeSave(transitTime);
                        // update Sample status to in-transit - calculate transit time by distance to travel for testing
                        //transitTime = sample->getTransitTimeSave();
                        //batchWaitTime = sample->getBatchWaitTimeSave();

                        if (transitTime > 1) {

                            // keep sample at this facility until return transit time has been reduced to 1

                            sample->setStatus(SampTestedInTransit);
                            //std::cout << "Status 2 change to status = " << sample->getStatus() << ", transit days = " << sample->getReturnTransitTime() << "\n";
                        }
                        else {

                            // record test as away and total tests
                            if (statsCount) {

                                stats->record_nTestsOut(distIndex, regIndex);
                                facilityStats->record_nTestsOut(facIndex);
                                stats->reduce_sPatientWaitingOut(distIndex, regIndex);
                                facilityStats->reduce_sPatientWaitingOut(facIndex);
                                stats->reduce_sSampleWaitingForTestOut(distIndex, regIndex);
                                facilityStats->reduce_sSampleWaitingForTestOut(facIndex);

                                if (sample->getInfected()) {

                                    //std::cout << "present to test mdcd = " << timeStep - patPtr->getTimestepPresented() << "\n";
                                    stats->record_timePresentToSampleTestOut(timeStep - patPtr->getTimestepPresented(), distIndex, regIndex);
                                    facilityStats->record_timePresentToSampleTestOut(timeStep - patPtr->getTimestepPresented(), facIndex);
                                    stats->record_nPresentToSampleTestOut(distIndex, regIndex);
                                    facilityStats->record_nPresentToSampleTestOut(facIndex);
                                    //if (patPtr->getRepresentFlag()) std::cout << "inside Testing updateSamples present to test out present = " << patPtr->getTimestepPresented() << ", orig present = " << patPtr->getTimestepOrigPresented() << ", time = " << timeStep - patPtr->getTimestepPresented() << " \n";
                                }

                                stats->record_timeSampleResourceWaitOut(sample->getWaitForResource(), distIndex, regIndex);
                                facilityStats->record_timeSampleResourceWaitOut(sample->getWaitForResource(), facIndex);

                                if (testName == "Xpert") {

                                    stats->record_nXpertOut(distIndex, regIndex);
                                    facilityStats->record_nXpertOut(facIndex);
                                }
                                else {

                                    stats->record_nSmearOut(distIndex, regIndex);
                                    facilityStats->record_nSmearOut(facIndex);
                                }
                            }
                            
                            // change status to returned
                            sample->setStatus(SampReturnedAvail);
                            patPtr->setSampleTestStatus("SampReturnedAvail");

                            // mark time results are returned to facility
                            sample->setResultsReturnedtoOrigFacilityTime(timeStep + 1);

                            // set patient status as patient called waiting for test results - don't need this - patient already either treated or not
                            //patPtr->setStatus(PatInUnTest);

                            // test result positive or negative 
                            //std::uniform_int_distribution<int> distribution(1, 100);
                            //randInt = distribution(generator);
                            randInt = getNextRandomNo100()-1;
                            randFloat = float(getNextRandomNo100()-1) + float(randInt) / 100;
                            if (randFloat > 100) randFloat = 100;

                            //std::cout << "randInt = " << randInt << ", sensPer " << getSensitivityPercent(sample->getDisease(), sample->getTest()) << " \n";

                            if (sample->getInfected()) {

                                // infected can only be true-positive or false-negative

                                // if infected get sensitivity for test
                                if (debugOn) std::cout << "patient infected randFloat = " << randFloat << " disease " << sample->getDisease() << " test " << sample->getTest() << ", sensPer " << getSensitivityPercent(sample->getDisease(), sample->getTest()) << " \n";

                                if (randFloat <= getSensitivityPercent(sample->getDisease(), sample->getTest())) {

                                    sample->setPositive(true);
                                    patPtr->setTestPos(true);

                                    if (statsCount) {

                                        if (debugOn) std::cout << "true positive test distIndex = " << distIndex << ", regIndex " << regIndex << " \n";

                                        stats->record_nTestTruePosOut(distIndex, regIndex);
                                        facilityStats->record_nTestTruePosOut(facIndex);
                                        stats->record_nTestPosOut(distIndex, regIndex);
                                        facilityStats->record_nTestPosOut(facIndex);

                                        // don't double count for MD clinical diagnosis
                                        if (!patPtr->getMDPos()) {

                                            stats->record_nDiagnosisOut(distIndex, regIndex);
                                            facilityStats->record_nDiagnosisOut(facIndex);
                                        }
                                        else {

                                            stats->record_nDiagnosisDC(distIndex, regIndex);
                                            facilityStats->record_nDiagnosisDC(facIndex);
                                            stats->record_nDiagnosisDCTruePos(distIndex, regIndex);
                                            facilityStats->record_nDiagnosisDCTruePos(facIndex);
                                            std::cout << "double count for true positive no MDPos = " << patPtr->getMDPos() << " \n";
                                        }

                                        //stats->record_sPatientInTreatment(distIndex, regIndex);

                                        if (testName == "Xpert") {

                                            stats->record_nXpertTruePos(distIndex, regIndex);
                                            facilityStats->record_nXpertTruePos(facIndex);
                                        }
                                        else {

                                            stats->record_nSmearTruePos(distIndex, regIndex);
                                            facilityStats->record_nSmearTruePos(facIndex);
                                        }
                                    }

                                    if (false) {

                                        std::cout << "set pos for infected in update sample, facility = " << sample->getOrigFacility() << ", " << sample->getTestingFacility();
                                        std::cout << ", sample disease = " << sample->getDisease() << ", test = " << sample->getTest() << ", randInt = " << randInt;
                                        std::cout << ", sensitivity = " << getSensitivityPercent(sample->getDisease(), sample->getTest()) << " \n";
                                    }
                                }
                                else {
                                    //std::cout << "infected with neg test result " << " \n";

                                    if (statsCount) {

                                        if (debugOn) std::cout << "false neg test distIndex = " << distIndex << ", regIndex " << regIndex << " \n";

                                        stats->record_nTestFalseNegOut(distIndex, regIndex);
                                        facilityStats->record_nTestFalseNegOut(facIndex);

                                        // false negative work the same as pre-treat loss to follow up
                                        //stats->record_nPreTreatLtFU(distIndex, regIndex);
                                        //stats->record_nPreTreatLtFUFN(distIndex, regIndex);

                                        if (testName == "Xpert") {

                                            stats->record_nXpertFalseNeg(distIndex, regIndex);
                                            facilityStats->record_nXpertFalseNeg(facIndex);
                                        }
                                        else {

                                            stats->record_nSmearFalseNeg(distIndex, regIndex);
                                            facilityStats->record_nSmearFalseNeg(facIndex);
                                        }

                                    }
                                }
                            }
                            else {

                                // non-infected can only be false-positive or true-negative

                                // if non-infected get specificity for test
                                if (randFloat > getSpecificityPercent(sample->getDisease(), sample->getTest())) {

                                    sample->setPositive(true);
                                    patPtr->setTestPos(true);

                                    if (statsCount) {

                                        stats->record_nTestFalsePosOut(distIndex, regIndex);
                                        facilityStats->record_nTestFalsePosOut(facIndex);
                                        stats->record_nTestPosOut(distIndex, regIndex);
                                        facilityStats->record_nTestPosOut(facIndex);

                                        // don't double count for MD clinical diagnosis
                                        if (!patPtr->getMDPos()) {

                                            stats->record_nDiagnosisOut(distIndex, regIndex);
                                            facilityStats->record_nDiagnosisOut(facIndex);
                                        }
                                        else {

                                            stats->record_nDiagnosisDC(distIndex, regIndex);
                                            facilityStats->record_nDiagnosisDC(facIndex);
                                            stats->record_nDiagnosisDCFalsePos(distIndex, regIndex);
                                            facilityStats->record_nDiagnosisDCFalsePos(facIndex);
                                        }

                                        //stats->record_sPatientInTreatment(distIndex, regIndex);

                                        if (testName == "Xpert") {

                                            stats->record_nXpertFalsePos(distIndex, regIndex);
                                            facilityStats->record_nXpertFalsePos(facIndex);
                                        }
                                        else {

                                            stats->record_nSmearFalsePos(distIndex, regIndex);
                                            facilityStats->record_nSmearFalsePos(facIndex);
                                        }
                                    }

                                    if (false) {

                                        std::cout << "set pos for uninfected in update sample,  facility = " << sample->getOrigFacility() << ", " << sample->getTestingFacility();
                                        std::cout << ", sample disease = " << sample->getDisease() << ", test = " << sample->getTest() << ", randInt = " << randInt;
                                        std::cout << ", sensitivity = " << getSpecificityPercent(sample->getDisease(), sample->getTest()) << " \n";
                                    }
                                }
                                else {

                                    if (statsCount) {

                                        stats->record_nTestTrueNegOut(distIndex, regIndex);
                                        facilityStats->record_nTestTrueNegOut(facIndex);

                                        if (testName == "Xpert") {

                                            stats->record_nXpertTrueNeg(distIndex, regIndex);
                                            facilityStats->record_nXpertTrueNeg(facIndex);
                                        }
                                        else {

                                            stats->record_nSmearTrueNeg(distIndex, regIndex);
                                            facilityStats->record_nSmearTrueNeg(facIndex);
                                        }
                                    }
                                }
                            }

                            // return sample to original facility
                            // get pointer to Samples original Facility
                            f = facilitiesList->getPtr(sample->getOrigFacility());

                            // add to Sample list where it came from 
                            f->addToSampleContainer(sample);

                            //std::cout << "found match in updateSample, sample age = " << sample->getAge() << " \n";

                            // remove it from Sample list in this container
                            //samplesToDelete.push_back(sample);
                            auto it = std::find(_sampleContainer.begin(), _sampleContainer.end(), sample);
                            if (it != _sampleContainer.end()) {
                                _sampleContainer.erase(it);
                                // std::cout << "found match in removeSample, list size = " << _sampleContainer.size() << " status= " << sample->getStatus()  << " \n";
                            }
                        }
                    }
                }
                // for samples tested out - check to see if patient infected and probability below MDCD infected percentage
                else if ((randFloat <= preTestMDCDUnInfPer) && (!sample->getInfected()) && (preTestMDCDUnInfPer > 0)) {

                     // debugOn - true
                     if (debugOn) std::cout << "sampleWaitForTest non-infected and preTestMDCD true negative test = " << randFloat << ", preTestMDCDUnInfPer = " << preTestMDCDUnInfPer << ", inf = " << sample->getInfected() << "\n";
                     //std::cout << "true neg rand = " << randFloat << ", dignosis prob = " << diagnosisPer <<  "\n";

                     // false positive - uninfected patient starts treatment PatUninfTestTreat
                     patPtr->setMDPos(true);
                            
                     patPtr->setStatus(PatUninfTestTreat);

                     if (statsCount) {

                        // record patients who should start treatment
                        stats->record_sPatientInTreatment(distIndex, regIndex);
                        facilityStats->record_sPatientInTreatment(facIndex);

                        stats->record_nDiagnosisOut(distIndex, regIndex);
                        facilityStats->record_nDiagnosisOut(facIndex);
                                
                        stats->record_nPreTestDiagnosisFalsePos(distIndex, regIndex);
                        facilityStats->record_nPreTestDiagnosisFalsePos(facIndex);
                        stats->record_nPreTestDiagnosisTotal(distIndex, regIndex);
                        facilityStats->record_nPreTestDiagnosisTotal(facIndex);
                        stats->record_nPreTestDiagnosisReview(distIndex, regIndex);
                        facilityStats->record_nPreTestDiagnosisReview(facIndex);

                        //stats->record_nPTMDCDUninfected(distIndex, regIndex);
                        stats->record_nMDCDUninfected(distIndex, regIndex);
                        facilityStats->record_nMDCDUninfected(facIndex);
                     } 

                    // check to see if sample is loss to follow-up
                    randInt = getNextRandomNo100()-1;
                    randFloat = float(getNextRandomNo100()-1) + float(randInt) / 100;
                    if (randFloat > 100) randFloat = 100;

                    if (randFloat < preTestLtoFPer) {
                        
                        // sample loss to follow-up
                        sample->setStatus(SampLostExpired);
                        patPtr->setSampleTestStatus("SampLostExpired");

                        patPtr->setPreTestLtoFFlag(true);

                        if (statsCount) {

                            stats->record_nPreTestLtFU(distIndex, regIndex);
                            facilityStats->record_nPreTestLtFU(facIndex);
                            stats->reduce_sSampleWaitingForTestOut(distIndex, regIndex);
                            facilityStats->reduce_sSampleWaitingForTestOut(facIndex);
                            stats->reduce_sPatientWaitingOut(distIndex, regIndex);
                            facilityStats->reduce_sPatientWaitingOut(facIndex);

                            if (sample->getInfected()) {

                                stats->record_nPreTestLtFUInf(distIndex, regIndex);
                                facilityStats->record_nPreTestLtFUInf(facIndex);
                            }
                        }
                        // update Sample back to original facility Sample list
                        // get pointer to Samples original Facility
                        f = facilitiesList->getPtr(sample->getOrigFacility());

                        // add to Sample list where it came from 
                        f->addToSampleContainer(sample);
                        //std::cout << "sample sent out, MD override, sample lost"  " \n";

                        // remove it from Sample list in this container
                        //samplesToDelete.push_back(sample);
                        auto it = std::find(_sampleContainer.begin(), _sampleContainer.end(), sample);
                        if (it != _sampleContainer.end()) {
                            _sampleContainer.erase(it);
                        }
                    }
                    else {

                        // sample results returned
                        testPtr = incrementResources(sample->getDisease(), sample->getTest());
                        // testName = incrementResources2(sample->getDisease());

                        if (statsCount) {

                            // debugOn - true
                            if (debugOn) std::cout << "inside update sample patient away after transit waiting for result timestep =  " << timeStep << ", present time = " << patPtr->getTimestepPresented() << "\n";

                            stats->record_timePresentToSampleDiagOut(timeStep - patPtr->getTimestepPresented(), distIndex, regIndex);
                            facilityStats->record_timePresentToSampleDiagOut(timeStep - patPtr->getTimestepPresented(), facIndex);
                            stats->record_nPresentToSampleDiagOut(distIndex, regIndex);
                            facilityStats->record_nPresentToSampleDiagOut(facIndex);
                        }

                        // set sample test time
                        sample->setTestedTime(timeStep);

                        // update test used for sample - watch for possible NULL pointer out of incrementResources
                        testName = testPtr->getTestName();

                        sample->setTest(testName);
                        sample->setTestPtr(testPtr);
                        //std::cout << "inside Testing updateSamples testName = " << testName << " \n";


                        

                        // calculate return transit time
                        //std::uniform_int_distribution<int> distribution(1, 14);
                        //transitTime = distribution(generator);
                        // if we want to add in later - change transit time to a random number
                        //transitTime = 1;
                        transitTime = sample->getTransitTimeSave();
                        //if (transitTime > 1) std::cout << "mdcd transit time = " << transitTime << "\n";

                        // update return transit time and set status to Sample in return transit
                        sample->setReturnTransitTime(transitTime);
                        sample->setReturnTransitTimeSave(transitTime);
                        // update Sample status to in-transit - calculate transit time by distance to travel for testing
                        //transitTime = sample->getTransitTimeSave();
                        //batchWaitTime = sample->getBatchWaitTimeSave();

                        if (transitTime > 1) {

                            // keep sample at this facility until return transit time has been reduced to 1

                            sample->setStatus(SampTestedInTransit);
                            //std::cout << "Status 2 change to status = " << sample->getStatus() << ", transit days = " << sample->getReturnTransitTime() << "\n";
                        }
                        else {

                            // record test as away and total tests
                            if (statsCount) {

                                stats->record_nTestsOut(distIndex, regIndex);
                                facilityStats->record_nTestsOut(facIndex);
                                stats->reduce_sPatientWaitingOut(distIndex, regIndex);
                                facilityStats->reduce_sPatientWaitingOut(facIndex);
                                stats->reduce_sSampleWaitingForTestOut(distIndex, regIndex);
                                facilityStats->reduce_sSampleWaitingForTestOut(facIndex);

                                if (sample->getInfected()) {

                                    //std::cout << "present to test mdcd = " << timeStep - patPtr->getTimestepPresented() << "\n";
                                    stats->record_timePresentToSampleTestOut(timeStep - patPtr->getTimestepPresented(), distIndex, regIndex);
                                    facilityStats->record_timePresentToSampleTestOut(timeStep - patPtr->getTimestepPresented(), facIndex);
                                    stats->record_nPresentToSampleTestOut(distIndex, regIndex);
                                    facilityStats->record_nPresentToSampleTestOut(distIndex, regIndex);
                                    //if (patPtr->getRepresentFlag()) std::cout << "inside Testing updateSamples present to test out present = " << patPtr->getTimestepPresented() << ", orig present = " << patPtr->getTimestepOrigPresented() << ", time = " << timeStep - patPtr->getTimestepPresented() << " \n";
                                }

                                stats->record_timeSampleResourceWaitOut(sample->getWaitForResource(), distIndex, regIndex);
                                facilityStats->record_timeSampleResourceWaitOut(sample->getWaitForResource(), facIndex);

                                if (testName == "Xpert") {

                                    stats->record_nXpertOut(distIndex, regIndex);
                                    facilityStats->record_nXpertOut(facIndex);
                                }
                                else {

                                    stats->record_nSmearOut(distIndex, regIndex);
                                    facilityStats->record_nSmearOut(facIndex);
                                }
                            }
                            
                            // change status to returned
                            sample->setStatus(SampReturnedAvail);
                            patPtr->setSampleTestStatus("SampReturnedAvail");

                            // mark time results are returned to facility
                            sample->setResultsReturnedtoOrigFacilityTime(timeStep + 1);

                            // set patient status as patient called waiting for test results - don't need this - patient already either treated or not
                            //patPtr->setStatus(PatInUnTest);

                            // test result positive or negative 
                            //std::uniform_int_distribution<int> distribution(1, 100);
                            //randInt = distribution(generator);
                            randInt = getNextRandomNo100()-1;
                            randFloat = float(getNextRandomNo100()-1) + float(randInt) / 100;
                            if (randFloat > 100) randFloat = 100;

                            //std::cout << "randInt = " << randInt << ", sensPer " << getSensitivityPercent(sample->getDisease(), sample->getTest()) << " \n";

                            if (sample->getInfected()) {

                                // infected can only be true-positive or false-negative

                                // if infected get sensitivity for test
                                if (debugOn) std::cout << "patient infected randFloat = " << randFloat << " disease " << sample->getDisease() << " test " << sample->getTest() << ", sensPer " << getSensitivityPercent(sample->getDisease(), sample->getTest()) << " \n";

                                if (randFloat <= getSensitivityPercent(sample->getDisease(), sample->getTest())) {

                                    sample->setPositive(true);
                                    patPtr->setTestPos(true);

                                    if (statsCount) {

                                        if (debugOn) std::cout << "true positive test distIndex = " << distIndex << ", regIndex " << regIndex << " \n";

                                        stats->record_nTestTruePosOut(distIndex, regIndex);
                                        facilityStats->record_nTestTruePosOut(facIndex);
                                        stats->record_nTestPosOut(distIndex, regIndex);
                                        facilityStats->record_nTestPosOut(facIndex);

                                        // don't double count for MD clinical diagnosis
                                        if (!patPtr->getMDPos()) {

                                            stats->record_nDiagnosisOut(distIndex, regIndex);
                                            facilityStats->record_nDiagnosisOut(facIndex);
                                        }
                                        else {

                                            stats->record_nDiagnosisDC(distIndex, regIndex);
                                            facilityStats->record_nDiagnosisDC(facIndex);
                                            stats->record_nDiagnosisDCTruePos(distIndex, regIndex);
                                            facilityStats->record_nDiagnosisDCTruePos(facIndex);
                                        }

                                        //stats->record_sPatientInTreatment(distIndex, regIndex);

                                        if (testName == "Xpert") {

                                            stats->record_nXpertTruePos(distIndex, regIndex);
                                            facilityStats->record_nXpertTruePos(facIndex);
                                        }
                                        else {

                                            stats->record_nSmearTruePos(distIndex, regIndex);
                                            facilityStats->record_nSmearTruePos(facIndex);
                                        }
                                    }

                                    if (false) {

                                        std::cout << "set pos for infected in update sample, facility = " << sample->getOrigFacility() << ", " << sample->getTestingFacility();
                                        std::cout << ", sample disease = " << sample->getDisease() << ", test = " << sample->getTest() << ", randInt = " << randInt;
                                        std::cout << ", sensitivity = " << getSensitivityPercent(sample->getDisease(), sample->getTest()) << " \n";
                                    }
                                }
                                else {
                                    //std::cout << "infected with neg test result " << " \n";

                                    if (statsCount) {

                                        if (debugOn) std::cout << "false neg test distIndex = " << distIndex << ", regIndex " << regIndex << " \n";

                                        stats->record_nTestFalseNegOut(distIndex, regIndex);
                                        facilityStats->record_nTestFalseNegOut(facIndex);

                                        // false negative work the same as pre-treat loss to follow up
                                        //stats->record_nPreTreatLtFU(distIndex, regIndex);
                                        //stats->record_nPreTreatLtFUFN(distIndex, regIndex);

                                        if (testName == "Xpert") {

                                            stats->record_nXpertFalseNeg(distIndex, regIndex);
                                            facilityStats->record_nXpertFalseNeg(facIndex);
                                        }
                                        else {

                                            stats->record_nSmearFalseNeg(distIndex, regIndex);
                                            facilityStats->record_nSmearFalseNeg(facIndex);
                                        }

                                    }
                                }
                            }
                            else {

                                // non-infected can only be false-positive or true-negative

                                // if non-infected get specificity for test
                                if (randFloat > getSpecificityPercent(sample->getDisease(), sample->getTest())) {

                                    sample->setPositive(true);
                                    patPtr->setTestPos(true);

                                    if (statsCount) {

                                        stats->record_nTestFalsePosOut(distIndex, regIndex);
                                        facilityStats->record_nTestFalsePosOut(facIndex);
                                        stats->record_nTestPosOut(distIndex, regIndex);
                                        facilityStats->record_nTestPosOut(facIndex);

                                        // don't double count for MD clinical diagnosis
                                        if (!patPtr->getMDPos()) {

                                            stats->record_nDiagnosisOut(distIndex, regIndex);
                                            facilityStats->record_nDiagnosisOut(facIndex);
                                        }
                                        else {

                                            stats->record_nDiagnosisDC(distIndex, regIndex);
                                            facilityStats->record_nDiagnosisDC(facIndex);
                                            stats->record_nDiagnosisDCFalsePos(distIndex, regIndex);
                                            facilityStats->record_nDiagnosisDCFalsePos(facIndex);
                                        }
                                           
                                        //stats->record_sPatientInTreatment(distIndex, regIndex);

                                        if (testName == "Xpert") {

                                            stats->record_nXpertFalsePos(distIndex, regIndex);
                                            facilityStats->record_nXpertFalsePos(facIndex);
                                        }
                                        else {

                                            stats->record_nSmearFalsePos(distIndex, regIndex);
                                            facilityStats->record_nSmearFalsePos(facIndex);
                                        }
                                    }

                                    if (false) {

                                        std::cout << "set pos for uninfected in update sample,  facility = " << sample->getOrigFacility() << ", " << sample->getTestingFacility();
                                        std::cout << ", sample disease = " << sample->getDisease() << ", test = " << sample->getTest() << ", randInt = " << randInt;
                                        std::cout << ", sensitivity = " << getSpecificityPercent(sample->getDisease(), sample->getTest()) << " \n";
                                    }
                                }
                                else {

                                    if (statsCount) {

                                        stats->record_nTestTrueNegOut(distIndex, regIndex);
                                        facilityStats->record_nTestTrueNegOut(facIndex);

                                        if (testName == "Xpert") {

                                            stats->record_nXpertTrueNeg(distIndex, regIndex);
                                            facilityStats->record_nXpertTrueNeg(facIndex);
                                        }
                                        else {

                                            stats->record_nSmearTrueNeg(distIndex, regIndex);
                                            facilityStats->record_nSmearTrueNeg(facIndex);
                                        }
                                    }
                                }
                            }

                            // return sample to original facility
                            // get pointer to Samples original Facility
                            f = facilitiesList->getPtr(sample->getOrigFacility());

                            // add to Sample list where it came from 
                            f->addToSampleContainer(sample);

                            //std::cout << "found match in updateSample, sample age = " << sample->getAge() << " \n";

                            // remove it from Sample list in this container
                            //samplesToDelete.push_back(sample);
                            auto it = std::find(_sampleContainer.begin(), _sampleContainer.end(), sample);
                            if (it != _sampleContainer.end()) {
                                _sampleContainer.erase(it);
                                // std::cout << "found match in removeSample, list size = " << _sampleContainer.size() << " status= " << sample->getStatus()  << " \n";
                            }
                        }
                    }
                }
                else if (randFloat2 < preTestLtoFPer) {

                    // debugOn - true
                    if (debugOn) std::cout << "SampWaitForTest pre-test ltof randFloat2 = " << randFloat2 << " randFloat = " << randFloat << " preTestLtoFPer = " << preTestLtoFPer << " infected = " << sample->getInfected() <<  " \n";
           
                    // percentage of samples to pretreatment loss to follow up - possible list from Zimbabwe
                    // 1.) 24.3% were registered as presumptive TB but sputum was not collected
                    // 2.) 15.5% had sputum collected but not sent
                    // 3.) 56.5% had sputum sent but not received at the laboratory, 
                    // 4.) 1% had a specimen received at the laboratory but not tested
                    // 6.) 2.6% had a specimen tested but the result was not available.
            
                    if (patPtr->getTracked()) std::cout << "ltof infected patient relost to pre-test ltof " <<  " \n";
                        
                    // change from in transit to waiting to be tested
                    sample->setStatus(SampLostExpired);
                    patPtr->setSampleTestStatus("SampLostExpired");

                    patPtr->setPreTestLtoFFlag(true);

                    // track LtoF time
                    patPtr->seTimePatientLtoF(timeStep);
                    
                    // change patient status to untested - sample lost
                    if (sample->getInfected()) {
                    
                        patPtr->setStatus(PatInfNoTestNoTreat);
                        
                        patPtr->setReasonNoTreat("PatInfNoTestLtofPreTest");

                        //std::cout << "sample pretest LtoF "  << " \n";
                    }
                    else {
                        
                        patPtr->setStatus(PatUninfNoTest);

                        //std::cout << "patient loss to follow up - dead test time =  " << sample->getTestedTime() << " \n";

                    }

                    if (statsCount) {

                        stats->record_nPreTestLtFU(distIndex, regIndex);
                        facilityStats->record_nPreTestLtFU(facIndex);
                        stats->reduce_sSampleWaitingForTestOut(distIndex, regIndex);
                        facilityStats->reduce_sSampleWaitingForTestOut(facIndex);
                        stats->reduce_sPatientWaitingOut(distIndex, regIndex);
                        facilityStats->reduce_sPatientWaitingOut(facIndex);

                        if (sample->getInfected()) {

                            stats->record_nPreTestLtFUInf(distIndex, regIndex);
                            facilityStats->record_nPreTestLtFUInf(facIndex);
                        }
                    }

                    // update Sample back to original facility Sample list
                    // get pointer to Samples original Facility
                    f = facilitiesList->getPtr(sample->getOrigFacility());

                    // add to Sample list where it came from 
                    f->addToSampleContainer(sample);
                    //std::cout << "found match in updateSample, sample age = " << sample->getAge() << " \n";

                    // remove it from Sample list in this container
                    //samplesToDelete.push_back(sample);
                    auto it = std::find(_sampleContainer.begin(), _sampleContainer.end(), sample);
                    if (it != _sampleContainer.end()) {
                        _sampleContainer.erase(it);
                        // std::cout << "found match in removeSample, list size = " << _sampleContainer.size() << " status= " << sample->getStatus()  << " \n";
                    }
                }
                else {
                
                    // debugOn - true
                    if (debugOn) std::cout << "SampWaitForTest normal sample send out randFloat2 = " << randFloat2 << " randFloat = " << randFloat << " preTestLtoFPer = " << preTestLtoFPer << " infected = " << sample->getInfected() << "\n";
                    
                    if (patPtr->getTracked()) std::cout << "sample away being tested " << " \n";
                    
                    // update resources to indicate a sample was tested
                    testPtr = incrementResources(sample->getDisease(), sample->getTest());
                    // testName = incrementResources2(sample->getDisease());
                                  
                    if (statsCount) {

                        // debugOn - true
                        if (debugOn) std::cout << "inside update sample patient away after transit waiting for result timestep =  " << timeStep << ", present time = " << patPtr->getTimestepPresented()  << "\n";
                        
                        stats->record_timePresentToSampleDiagOut(timeStep - patPtr->getTimestepPresented(), distIndex, regIndex);
                        facilityStats->record_timePresentToSampleDiagOut(timeStep - patPtr->getTimestepPresented(), facIndex);
                        stats->record_nPresentToSampleDiagOut(distIndex, regIndex);
                        facilityStats->record_nPresentToSampleDiagOut(facIndex);
                    }

                    // set sample test time
                    sample->setTestedTime(timeStep);

                    // update test used for sample - watch for possible NULL pointer out of incrementResources
                    testName = testPtr->getTestName();

                    sample->setTest(testName);
                    sample->setTestPtr(testPtr);
                    //std::cout << "inside Testing updateSamples testName = " << testName << " \n";
                
                    
                    // record test as away and total tests
                    if (statsCount) {
                    
                        stats->record_nTestsOut(distIndex, regIndex);
                        facilityStats->record_nTestsOut(facIndex);
                        stats->reduce_sPatientWaitingOut(distIndex, regIndex);
                        facilityStats->reduce_sPatientWaitingOut(facIndex);
                        stats->reduce_sSampleWaitingForTestOut(distIndex, regIndex);
                        facilityStats->reduce_sSampleWaitingForTestOut(facIndex);

                        if (sample->getInfected()) {

                            //std::cout << "present to test reg = " << timeStep - patPtr->getTimestepPresented() << "\n";
                            stats->record_timePresentToSampleTestOut(timeStep - patPtr->getTimestepPresented(), distIndex, regIndex);
                            facilityStats->record_timePresentToSampleTestOut(timeStep - patPtr->getTimestepPresented(), facIndex);
                            stats->record_nPresentToSampleTestOut(distIndex, regIndex);
                            facilityStats->record_nPresentToSampleTestOut(facIndex);
                        }

                        stats->record_timeSampleResourceWaitOut(sample->getWaitForResource(), distIndex, regIndex);
                        facilityStats->record_timeSampleResourceWaitOut(sample->getWaitForResource(), facIndex);

                        if (testName == "Xpert") {
                    
                             stats->record_nXpertOut(distIndex, regIndex);
                             facilityStats->record_nXpertOut(facIndex);
                        }
                        else {

                             stats->record_nSmearOut(distIndex, regIndex);
                             facilityStats->record_nSmearOut(facIndex);
                        }
                    }
                
                    // calculate return transit time
                    //std::uniform_int_distribution<int> distribution(1, 14);
                    //transitTime = distribution(generator);
                    // if we want to add in later - change transit time to a random number
                    transitTime = 1;

                    // update return transit time and set status to Sample in return transit
                    sample->setReturnTransitTime(transitTime);
                    sample->setReturnTransitTimeSave(transitTime);
                        
                    if (transitTime > 1) {

                        // keep sample at this facility until return transit time has been reduced to 1

                        sample->setStatus(SampTestedInTransit);
                        std::cout << "Status 2 change to status = " << sample->getStatus() << ", transit days = " << sample->getReturnTransitTime() << "\n";
                    }
                    else {

                        // change status to returned
                        sample->setStatus(SampReturnedAvail);
                        patPtr->setSampleTestStatus("SampReturnedAvail");
                                        
                        // mark time results are returned to facility
                        sample->setResultsReturnedtoOrigFacilityTime(timeStep + 1);

                        // set patient status as patient called waiting for test results
                        patPtr->setStatus(PatInUnTest);

                        // test result positive or negative 
                        //std::uniform_int_distribution<int> distribution(1, 100);
                        //randInt = distribution(generator);
                        randInt = getNextRandomNo100()-1;
                        randFloat = float(getNextRandomNo100()-1) + float(randInt) / 100;
                        if (randFloat > 100) randFloat = 100;

                        //std::cout << "randInt = " << randInt << ", sensPer " << getSensitivityPercent(sample->getDisease(), sample->getTest()) << " \n";
                        
                        if (sample->getInfected()) {

                            // infected can only be true-positive or false-negative
                    
                            // if infected get sensitivity for test
                            if (debugOn) std::cout << "patient infected randFloat = " << randFloat << " disease " << sample->getDisease() << " test " << sample->getTest() << ", sensPer " << getSensitivityPercent(sample->getDisease(), sample->getTest()) << " \n";
                            
                            if (randFloat <= getSensitivityPercent(sample->getDisease(), sample->getTest())) {

                                sample->setPositive(true);
                                patPtr->setTestPos(true);

                                if (statsCount) {

                                    if (debugOn) std::cout << "true positive test distIndex = " << distIndex << ", regIndex " << regIndex << " \n";
                                    
                                    stats->record_nTestTruePosOut(distIndex, regIndex);
                                    facilityStats->record_nTestTruePosOut(facIndex);
                                    stats->record_nTestPosOut(distIndex, regIndex);
                                    facilityStats->record_nTestPosOut(facIndex);
                                    stats->record_nDiagnosisOut(distIndex, regIndex);
                                    facilityStats->record_nDiagnosisOut(facIndex);
                                    stats->record_sPatientInTreatment(distIndex, regIndex);
                                    facilityStats->record_sPatientInTreatment(facIndex);

                                    if (testName == "Xpert") {
                    
                                        stats->record_nXpertTruePos(distIndex, regIndex);
                                        facilityStats->record_nXpertTruePos(facIndex);
                                    }
                                    else {

                                        stats->record_nSmearTruePos(distIndex, regIndex);
                                        facilityStats->record_nSmearTruePos(facIndex);
                                    }
                                }
                        
                                if (false) {
                            
                                    std::cout << "set pos for infected in update sample, facility = " << sample->getOrigFacility() << ", " << sample->getTestingFacility();
                                    std::cout << ", sample disease = " << sample->getDisease() << ", test = " <<  sample->getTest() << ", randInt = " << randInt;
                                    std::cout << ", sensitivity = " << getSensitivityPercent(sample->getDisease(), sample->getTest()) << " \n";
                                }
                            }
                            else {
                                //std::cout << "infected with neg test result " << " \n";

                                if (statsCount) {

                                    if (debugOn) std::cout << "false neg test distIndex = " << distIndex << ", regIndex " << regIndex << " \n";
                                    
                                    stats->record_nTestFalseNegOut(distIndex, regIndex);
                                    facilityStats->record_nTestFalseNegOut(facIndex);

                                    // false negative work the same as pre-treat loss to follow up
                                    //stats->record_nPreTreatLtFU(distIndex, regIndex);
                                    //stats->record_nPreTreatLtFUFN(distIndex, regIndex);

                                    if (testName == "Xpert") {
                    
                                        stats->record_nXpertFalseNeg(distIndex, regIndex);
                                        facilityStats->record_nXpertFalseNeg(facIndex);
                                    }
                                    else {

                                        stats->record_nSmearFalseNeg(distIndex, regIndex);
                                        facilityStats->record_nSmearFalseNeg(facIndex);
                                    }

                                }
                            }
                        }
                        else {

                            // non-infected can only be false-positive or true-negative

                            // if non-infected get specificity for test
                            if (randFloat > getSpecificityPercent(sample->getDisease(), sample->getTest())) {

                                sample->setPositive(true);
                                patPtr->setTestPos(true);

                                if (statsCount) {

                                    stats->record_nTestFalsePosOut(distIndex, regIndex);
                                    facilityStats->record_nTestFalsePosOut(facIndex);
                                    stats->record_nTestPosOut(distIndex, regIndex);
                                    facilityStats->record_nTestPosOut(facIndex);
                                    stats->record_nDiagnosisOut(distIndex, regIndex);
                                    facilityStats->record_nDiagnosisOut(facIndex);
                                    stats->record_sPatientInTreatment(distIndex, regIndex);
                                    facilityStats->record_sPatientInTreatment(facIndex);

                                    if (testName == "Xpert") {
                    
                                        stats->record_nXpertFalsePos(distIndex, regIndex);
                                        facilityStats->record_nXpertFalsePos(facIndex);
                                    }
                                    else {

                                        stats->record_nSmearFalsePos(distIndex, regIndex);
                                        facilityStats->record_nSmearFalsePos(facIndex);
                                    }
                                }
                                
                                if (false) {
                            
                                    std::cout << "set pos for uninfected in update sample,  facility = " << sample->getOrigFacility() << ", " << sample->getTestingFacility();
                                    std::cout << ", sample disease = " << sample->getDisease() << ", test = " <<  sample->getTest() << ", randInt = " << randInt;
                                    std::cout << ", sensitivity = " << getSpecificityPercent(sample->getDisease(), sample->getTest()) << " \n";
                                }
                            }
                            else {

                                if (statsCount) {

                                    stats->record_nTestTrueNegOut(distIndex, regIndex);
                                    facilityStats->record_nTestTrueNegOut(facIndex);

                                    if (testName == "Xpert") {
                    
                                        stats->record_nXpertTrueNeg(distIndex, regIndex);
                                        facilityStats->record_nXpertTrueNeg(facIndex);
                                    }
                                    else {

                                        stats->record_nSmearTrueNeg(distIndex, regIndex);
                                        facilityStats->record_nSmearTrueNeg(facIndex);
                                    }
                                }
                            }
                        }

                        // return sample to original facility
                        // get pointer to Samples original Facility
                        f = facilitiesList->getPtr(sample->getOrigFacility());

                        // add to Sample list where it came from 
                        f->addToSampleContainer(sample);

                        //std::cout << "found match in updateSample, sample age = " << sample->getAge() << " \n";

                        // remove it from Sample list in this container
                        //samplesToDelete.push_back(sample);
                        auto it = std::find(_sampleContainer.begin(), _sampleContainer.end(), sample);
                        if (it != _sampleContainer.end()) {
                            _sampleContainer.erase(it);
                            // std::cout << "found match in removeSample, list size = " << _sampleContainer.size() << " status= " << sample->getStatus()  << " \n";
                        }
                    }
                }
                //std::cout << "after waiting status = " << sample->getStatus() << ", transit days = " << sample->getReturnTransitTime() << "\n";
            }
            else {
                
                if (patPtr->getTracked()) std::cout << "patient no tests left - still waiting for sample testing " << " \n";

                // no tests left - keep in wait status and update age
                sample->incrementWait();
                sample->incrementWaitForResource();

                //debugOn - true
                if (debugOn) std::cout << "inside Testing updateSamples status = 4  no resources available - wait = " << sample->getWait() << " disease = " << sample->getDisease() << " test = " << sample->getTest() << sample->getTest() << " test facility = " << sample->getTestingFacility() << " \n";
            }

            break;
        case 5:  // SampLostExpired - lost

            if (debugOn) std::cout << "inside sample lost in transit "  << " \n";
            
            // return sample to original facility
            // get pointer to Samples original Facility
            f = facilitiesList->getPtr(sample->getOrigFacility());

            // add to Sample list where it came from 
            //f->addToSampleContainer(sample);

            if (debugOn) {

                std::cout << "inside sample lost in transit return samples to original facility f = " << f->getName() << " \n";
                std::cout << "found match in updateSample, sample age = " << sample->getAge() << "original fac = " << sample->getOrigFacility() << " testing fac = " << sample->getTestingFacility() << " \n";
            }

            // remove it from Sample list in this container
            //samplesToDelete.push_back(sample);
            // have to add (true) test to localize IT to this case
            if (false) {
            
                auto it = std::find(_sampleContainer.begin(), _sampleContainer.end(), sample);
                if (it != _sampleContainer.end()) {

                    if (debugOn) std::cout << "found match in sample lost removeSample, list size = " << _sampleContainer.size() << " status= " << sample->getStatus() << " sample= " << sample << " \n";

                     _sampleContainer.erase(it);
                     
                }
            }

            // change status to lost -returned
            sample->setStatus(SampReturnedLostExpired);

            if (debugOn) std::cout << "sample lost in transit after setStatus " << " \n";

            patPtr->setSampleTestStatus("SampReturnedLostExpired");
            
            if (debugOn)  std::cout << "leaving sample lost in transit "  << " \n";

            break;
        case 6:  // SampTestedInTransit - tested - in transit back to original facility

            // *** should never get here, this status is never set
            //std::cout << "case 5 transit time= " << sample->getReturnTransitTime() << "\n";

            // check no. of days in transit
            if (sample->getReturnTransitTime() < 2) {
                // change from in transit to returned
                sample->setStatus(SampReturnedAvail);
                patPtr->setSampleTestStatus("SampReturnedAvail");

                // mark time results are returned to facility
                sample->setResultsReturnedtoOrigFacilityTime(timeStep + 1);

                // now cacluated whether the sample being returned is postive or negative
                // probabilities different depending whether patient is infected or not
                //std::uniform_int_distribution<int> distribution(1, 100);
                //randInt = distribution(generator);
                randInt = getNextRandomNo100()-1;
                randFloat = float(getNextRandomNo100()-1) + float(randInt) / 100;
                if (randFloat > 100) randFloat = 100;

                if (sample->getInfected()) {
                    // if infected assume positive return based on sensitivity
                    if (randFloat <= getSensitivityPercent(sample->getDisease(), sample->getTest())) {
                        sample->setPositive(true);
                        patPtr->setTestPos(true);
                    }
                }
                else {
                    // if non-infected assume negative return based specificity
                    if (randFloat > getSpecificityPercent(sample->getDisease(), sample->getTest())) {
                        sample->setPositive(true);
                        patPtr->setTestPos(true);
                    }
                }

                sample->setTestedTime(timeStep);
                
                // update Sample back to original facility Sample list
                // get pointer to Samples original Facility
                f = facilitiesList->getPtr(sample->getOrigFacility());

                // add to Sample list where it came from 
                f->addToSampleContainer(sample);
                //std::cout << "found match in updateSample, sample age = " << sample->getAge() << " \n";

                // remove it from Sample list in this container
                //samplesToDelete.push_back(sample);
                auto it = std::find(_sampleContainer.begin(), _sampleContainer.end(), sample);
                if (it != _sampleContainer.end()) {
                    _sampleContainer.erase(it);
                    // std::cout << "found match in removeSample, list size = " << _sampleContainer.size() << " status= " << sample->getStatus()  << " \n";
                }

                //std::cout << "case 4 after sample now there status = " << sample->getStatus() << "\n";
            }
            else {
                // leave in transit - decrement transit time
                sample->reduceReturnTransitTime();
                //std::cout << "case 4 after sample decrement transit time = " << sample->getReturnTransitTime() << "\n";

            }
            break;
        case 7:  // SampReturnedAvail - returned - do nothing - wait for patient interaction

            break;
        case 8:  // SampReturnedLostExpired - returned/lost - do nothing - wait for patient interaction

            break;
        case 9:  // SampLtoF - Loss to Folloup - do nothing 

            break;
        case 10:  // SampPatDead - Patient dead - do nothing 

            break;
        default:
            std::cout << "some other status = " << status << " \n";
        }

        // if (debugOn) 
        if (debugOn) std::cout << "updateSample-end, counter = " << counter << " ,sample age = " << sample->getAge() << " ,status = " << status << ", Test name = " << sample->getTest() <<" \n";
    }

    // remove all samples at once
    //this->removeSamplesFromList(_sampleContainer, samplesToDelete, false);

    if (debugOn) std::cout << "update Samples size after = " << _sampleContainer.size() << " \n";
}


// reorder Samples - highest priority first, then oldest to newest
void Testing::reorderSamples() {

    //int i;
    //int j;
    std::size_t i, j;
    int oldAge;
    int newAge;
    int oldPriority;
    int newPriority;
    Sample* tempPtr;

    // don't reorder for less than 2 samples
    if (_sampleContainer.size() > 1) {

        // first by age of sample
        for (i = 0; i < (_sampleContainer.size() - 1); i++) {

            // get priority to check against
            oldAge = _sampleContainer[i]->getAge();

            for (j = i + 1; j < _sampleContainer.size(); j++) {

                // get priority from rest of loop
                newAge = _sampleContainer[j]->getAge();

                if (newAge > oldAge) {

                    // swap pointers
                    tempPtr = _sampleContainer[i];
                    _sampleContainer[i] = _sampleContainer[j];
                    _sampleContainer[j] = tempPtr;

                    oldAge = newAge;
                }
            }

        }

        // now by priority - this should keep the age - old to new intact for each priority
        for (i = 0; i < (_sampleContainer.size() - 1); i++) {

            // get priority to check against
            oldPriority = _sampleContainer[i]->getPriority();

            for (j = i + 1; j < _sampleContainer.size(); j++) {

                // get priority from rest of loop
                newPriority = _sampleContainer[j]->getPriority();

                if (newPriority > oldPriority) {

                    // swap pointers
                    tempPtr = _sampleContainer[i];
                    _sampleContainer[i] = _sampleContainer[j];
                    _sampleContainer[j] = tempPtr;

                    oldPriority = newPriority;

                }
            }

        }
    }
}

// reorder Samples - reverse list - this should reorder oldest to newest instead of newest to oldest
void Testing::reverseSamples() {

    //int i;
    //int j;
    std::size_t i, j;
    std::size_t sampleListSize;
    std::size_t halfIndex = 0;
    Sample* tempPtr;

    sampleListSize = _sampleContainer.size();
    
    // reorder lists of size 10 or more
    if (sampleListSize > 9) {
    
        // swap most of the samples on either side of the midpoint of the list
        halfIndex = sampleListSize/2 - 1;
        
        //start 1 pointer at end
        j = sampleListSize - 1;
        
        // start other pointer at begining
        for (i = 0; i < halfIndex; i++) {

            // swap pointers
            tempPtr = _sampleContainer[i];
            _sampleContainer[i] = _sampleContainer[j];
            _sampleContainer[j] = tempPtr;

            // decrement end pointer
            j--;
        }
    }
}


// reorder Samples - bring the last 20 samples to the front
void Testing::bringLastSamplesToFront() {

    //int i;
    //int j;
    std::size_t i, j;
    std::size_t sampleListSize;
    Sample* tempPtr;

    sampleListSize = _sampleContainer.size();
    
    // reorder lists of size 20 or more
    if (sampleListSize > 20) {
    

        if ((sampleListSize - 20) < 21) {
        
            // just reverse the list
            std::reverse(_sampleContainer.begin(), _sampleContainer.end());

            //std::cout << " reverse ";
        }
        else {

            //start 1 pointer at end
            j = sampleListSize - 1;
        
            // start other pointer at begining
            for (i = 0; i < 20; i++) {

                // swap pointers
                tempPtr = _sampleContainer[i];
                _sampleContainer[i] = _sampleContainer[j];
                _sampleContainer[j] = tempPtr;

                // decrement end pointer
                j--;
            }
            //std::cout << " last 20 ";
        }
    }
}


// reorder Samples - bring the last 20 samples to the front, push everything back
void Testing::bringLastSamplesToFront2() {

    std::size_t i;
    std::size_t sampleListSize;
    Sample* tempPtr;

    sampleListSize = _sampleContainer.size();
    
    // reorder lists of size 20 or more
    if (sampleListSize > 20) {
    

        if ((sampleListSize - 20) < 41) {
        
            // just reverse the list
            std::reverse(_sampleContainer.begin(), _sampleContainer.end());

            //std::cout << " reverse ";
        }
        else {

            // rotate all but last 20 elements to back
            //std::rotate(_sampleContainer.begin(),_sampleContainer.end()-20, _sampleContainer.end());
            for (i = 0; i < 20; i++) {
            
                //std::swap(_sampleContainer[i], _sampleContainer[sampleListSize-1-i]);
                
                tempPtr = _sampleContainer[sampleListSize-1];        //get last item in vector list
 
                _sampleContainer.erase(_sampleContainer.end()-1);     // remove it

                _sampleContainer.push_back(tempPtr);                // put it back in front
             }
            //std::cout << " last 20 ";
        }
    }
}



// print out Samples 
void Testing::printSamples() {

    std::size_t i;
    
    std::cout << "sample list size  = " << _sampleContainer.size();

    for (i = 0; i < _sampleContainer.size(); i++) {

        std::cout << " patient = " << _sampleContainer[i]->getPatientPtr() << " status  = " << _sampleContainer[i]->getStatus() << " wait  = " << _sampleContainer[i]->getWait() << ", ";
    }

    std::cout << " \n";
}




// remove Sample from _sampleContainer and delete
void Testing::removeSamplesFromList(vector<Sample*> list, vector<Sample*> sampleToDelete, bool deleteFlag) {


    if (DEBUG2) {
        std::cout << "in - no of samples to delete  = " << sampleToDelete.size() << "  no of samples left  = " << list.size() << " \n";
        for (auto sample : sampleToDelete) {
            std::cout << "sample to delete  = " << sample << " \n";
        }
    }

    for (auto sample : sampleToDelete) {
        //std::cout << "this sample to delete  = " << sample << " \n";

        // loop through all Samples at this Facility
        //for (int j = 0; j < list.size(); ++j){
           //std::cout << "sample to test  = " << list[j] << " \n";
            //if (sample == list[j]) {

                //std::cout << "found match in removeSample, list size = " << list.size() << " j = " << j << " status= " << sample->getStatus() << " delete flag = " << deleteFlag << " \n";

                //list.erase(list.begin() + j);
                //std::cout << "after erase"  << " \n";

                //if (deleteFlag) {
                //    delete sample;
                //    //std::cout << "after delete" << " \n";
                //}

                //break;
            //}
        //}
        //std::cout << "after for statement " << " \n";
        auto it = std::find(list.begin(), list.end(), sample);
        if (it != list.end()) {
            list.erase(it);
            //std::cout << "found match in removeSample, list size = " << list.size() << " status= " << sample->getStatus() << " delete flag = " << deleteFlag << " \n";

            if (deleteFlag) {
                delete sample;
                std::cout << "after delete" << " \n";
            }
        }
    }

    if (DEBUG2) {
        std::cout << "out - no of samples to delete  = " << sampleToDelete.size() << "  no of samples left  = " << list.size() << " \n";
    }
}

// remove Sample from _sampleContainer and delete
void Testing::removeSampleFromList(Sample* sample) {

    auto it = std::find(_sampleContainer.begin(), _sampleContainer.end(), sample);
    if (it != _sampleContainer.end()) {
        _sampleContainer.erase(it);
        // std::cout << "found match in removeSample, list size = " << _sampleContainer.size() << " status= " << sample->getStatus()  << " \n";
    }

}

// remove Sample from _sampleContainer and delete - need to add pointer to a pointer
void Testing::removeSampleFromListAndDelete(Sample* sample) {

    //std::cout << "inside removeSample, list size = " << _sampleContainer.size() << " status= " << sample->getStatus() << " \n";

    auto it = std::find(_sampleContainer.begin(), _sampleContainer.end(), sample);
    if (it != _sampleContainer.end()) {
        _sampleContainer.erase(it);
        //std::cout << "found match in removeSample, list size = " << _sampleContainer.size() << " status= " << sample->getStatus()  << " \n";

        // save sample away in inactive sample list
        _inactiveSampleContainer.push_back(sample);
        
        //std::cout << "after delete sample" <<  " \n";
    }
    //std::cout << "inside after removeSample, list size = " << _sampleContainer.size() << " \n";
}


// look through sample container and move sample associated with patient to inactive sample list
bool Testing::removePatientSampleFromList(Patient* patient, int status) {

    bool returnValue = false;


    // loop through samples and find sample that was propagated by patient
    for (auto sample : _sampleContainer) {

        if (sample->getPatientPtr() == patient) {

            // update sample for the tests available at this facility before removing from list - now done when patient created
            //for (auto t : _testsContainer) {

            //    if ((patient->getDisease() == t->getDiseaseName()) && (t->getDiseaseTesting())) {

            //        sample->setTest(t->getTestName());
            //    }

            //}
    
            auto it = std::find(_sampleContainer.begin(), _sampleContainer.end(), sample);

            _sampleContainer.erase(it);
            //std::cout << "found match in removeSample, list size = " << _sampleContainer.size() << " status= " << sample->getStatus()  << " \n";

            // save sample away in inactive sample list
            //delete sample;

            sample->setStatus(status);

            _inactiveSampleContainer.push_back(sample);

            returnValue = true;

            break;
        }

    }

    return returnValue;
}


// look through iactive sample container and move sample associated with patient back to active sample list
bool Testing::restorePatientSampleFromList(Patient* patient, int status) {

    bool returnValue = false;


    // loop through samples and find sample that was propagated by patient
    for (auto sample : _inactiveSampleContainer) {

        if (sample->getPatientPtr() == patient) {

            // update sample for the tests available at this facility before removing from list - now done when patient created
            //for (auto t : _testsContainer) {

            //    if ((patient->getDisease() == t->getDiseaseName()) && (t->getDiseaseTesting())) {

            //        sample->setTest(t->getTestName());
            //    }

            //}
    
            auto it = std::find(_inactiveSampleContainer.begin(), _inactiveSampleContainer.end(), sample);

            _inactiveSampleContainer.erase(it);
            //std::cout << "found match in removeSample, list size = " << _sampleContainer.size() << " status= " << sample->getStatus()  << " \n";

            // save sample away in inactive sample list
            //delete sample;

            sample->setStatus(status);

            _sampleContainer.push_back(sample);

            returnValue = true;

            break;
        }

    }

    return returnValue;
}




Testing::~Testing() {
    //cout<<"removing Testing"<<_id<<endl;
}
//---------------------------------------






