//
//  Facility.cpp
//  mdrtb
//
//  Created by L. Dombrowski on 1/10/2020.
//
#include <random>
#include <iostream>
#include <vector>
#include <algorithm>
#include "Facility.h"
#include "GlobalParameters.h"
#include "FGlobal.h"

using namespace std;


Facility::Facility() {
};

Facility::Facility(string name, string id, float longitude, float latitude, string type, int tier) {
    
    // store away information on creation
    _name = name;

    // generate a unique id for the facility - this will be used for finding the facility
    _id = id;

    // save away type
    _type = type;

    // store away latitude and longitude
    _longitude = longitude;
    _latitude = latitude;

    _testing = new Testing(name);

    // create a Diagnosis agent - assume no diagnosis
    _diagnosis = new Diagnosis(false, name);

    // create tier off of type
    _tier = tier;

    // note where Xpert machine is here
    _Xpert = false;

}

Facility::Facility(string name, string id, float longitude, float latitude, string type, string up, float dist, int tier) {
    
    // store away information on creation
    _name = name;

    // generate a unique id for the facility - this will be used for finding the facility
    _id = id;

    // save away type
    _type = type;

    // store away latitude and longitude
    _longitude = longitude;
    _latitude = latitude;

    // create a Diagnosis agent - assume no diagnosis
    _diagnosis = new Diagnosis(false, name);
    
    _testing = new Testing(name);


    // create tier off of type
    _tier = tier;

    // update parent connection

    // connections - only 1 up connection
    _upConnection = up;

    // note where Xpert machine is here
    _Xpert = false;

    //std::cout << "after readInputFile  " << _name << "\n";


}

Facility::Facility(string name, string id, float longitude, float latitude, string type, string up, float dist, int tier, int pop, float inc, float bins[10], bool diag, bool test) {

    int i;

    // store away information on creation
    _name = name;

    // generate a unique id for the facility - this will be used for finding the facility
    _id = id;

    // save away type
    _type = type;

    // store away latitude and longitude
    _longitude = longitude;
    _latitude = latitude;

    // create a Diagnosis agent
    _diagnosis = new Diagnosis(diag, name);

    _testing = new Testing(name, maxTests, maxTestsReagents, reagentRestockTime, noPersonel);

    // create tier off of type
    _tier = tier;

    // update parent connection

    // connections - only 1 up connection
    _upConnection = up;

    // population in catchment
    _population = pop;

    // pop bin increment value
    _binInc = inc;

    // probability that the population is a given distance away
    for (i = 0; i < 10; i++) {
        _popBins[i] = bins[i];
        _popDensityBins[i] = 0;
    }

    // note where Xpert machine is here
    _Xpert = false;

}


Facility::Facility(string name, string id, float longitude, float latitude, string type, string up, float dist, int tier, int pop, float inc, float bins[10], string region, string district, bool diag, vector<string> diseaseList, vector<Test*> tests, vector <facilityAttributes*> attrPtr, bool Xpert) {

    int i;
    bool debug = false;

    if (debug) std::cout << "inside new Facility " << "\n";
    
    // store away information on creation
    _name = name;

    // generate a unique id for the facility - this will be used for finding the facility
    _id = id;

    // save away type
    _type = type;

    // store away latitude and longitude
    _longitude = longitude;
    _latitude = latitude;

    // create a Diagnosis agent
    _diagnosis = new Diagnosis(diag, name);

    if (debug) std::cout << "inside new Facility after new diagnosis " << "\n";

    // create a Testing agent with values from GlobalParams.h
    //testing = new Testing(test, name, maxTests, noMachines, maxTestsReagents, reagentRestockTime, noPersonel);
    _testing = new Testing(name, diseaseList, tests, attrPtr);

    if (debug) std::cout << "inside new Facility after new testing " << "\n";

    // create tier off of type
    _tier = tier;

    // update parent connection

    // connections - only 1 up connection
    _upConnection = up;

    // population in catchment
    _population = pop;

    // pop bin increment value
    _binInc = inc;

    // probability that the population is a given distance away
    for (i = 0; i < 10; i++) {
        _popBins[i] = bins[i];
        _popDensityBins[i] = 0;
    }

    // store away region and district for the facility
    _region = region;
    _regionIndex = 0;
    _district = district;
    _districtIndex = 0;

    // store away incidence for each disease for this facility
    _facilityAttributes = attrPtr;

    for (auto a : _facilityAttributes) {

        if (debug) {

            std::cout << "facility = " << name << ", facattr disease = " << a->disease << ", test = " << a->testName << ", testing = " << a->testing;
            std::cout << ", priority = " << a->priority << ", facattr incidence = " << a->incidence << ", facattr incidence factor = " << a->incAdjFactor << ", no machines = " << a->noMachines << "\n";
        }

        // see if testing is done here for this disease
        if (a->testing) {

              // if testing is done here for this disease and test, assume this facility is closest and set distance to zero
              a->closetConnectionWithTesting = name;
              a->closestConnectionDistance = 0;
        }
        else {

             // default to testing away
             a->closetConnectionWithTesting = up;
             a->closestConnectionDistance = dist;
        }
    }

    // note where Xpert machine is here
    _Xpert = Xpert;

    if (debug) std::cout << "leaving new Facility " << "\n";
}


Facility::Facility(string name, string id, float longitude, float latitude, string type, string up, float dist, int tier, int pop, float inc, float bins[10], float densityBins[10], string region, int regIndex, string district, int distIndex, bool diag, vector<string> diseaseList, vector<Test*> tests, vector <facilityAttributes*> attrPtr, bool Xpert) {

    int i;
    bool debug = false;

    if (debug) std::cout << "inside new Facility " << "\n";
    
    // store away information on creation
    _name = name;

    // generate a unique id for the facility - this will be used for finding the facility
    _id = id;

    // save away type
    _type = type;

    // store away latitude and longitude
    _longitude = longitude;
    _latitude = latitude;

    // create a Diagnosis agent
    _diagnosis = new Diagnosis(diag, name);

    if (debug) std::cout << "inside new Facility after new diagnosis " << "\n";

    // create a Testing agent with values from GlobalParams.h
    //testing = new Testing(test, name, maxTests, noMachines, maxTestsReagents, reagentRestockTime, noPersonel);
    _testing = new Testing(name, diseaseList, tests, attrPtr);

    if (debug) std::cout << "inside new Facility after new testing " << "\n";

    // create tier off of type
    _tier = tier;

    // update parent connection

    // connections - only 1 up connection
    _upConnection = up;

    // population in catchment
    _population = pop;

    // pop bin increment value
    _binInc = inc;

    // probability that the population is a given distance away
    for (i = 0; i < 10; i++) {
        _popBins[i] = bins[i];
        _popDensityBins[i] = densityBins[i];
    }

    // store away region and district for the facility
    _region = region;
    _regionIndex = regIndex;
    _district = district;
    _districtIndex = distIndex;

    // store away incidence for each disease for this facility
    _facilityAttributes = attrPtr;

    for (auto a : _facilityAttributes) {

        if (debug) {

            std::cout << "facility = " << name << ", facattr disease = " << a->disease << ", test = " << a->testName << ", testing = " << a->testing;
            std::cout << ", priority = " << a->priority << ", facattr incidence = " << a->incidence << ", facattr incidence factor = " << a->incAdjFactor << ", no machines = " << a->noMachines << "\n";
        }
             
        // see if testing is done here for this disease
        if (a->testing) {

              // if testing is done here for this disease and test, assume this facility is closest and set distance to zero
              a->closetConnectionWithTesting = name;
              a->closestConnectionDistance = 0;
        }
        else {

             // default to testing away
             a->closetConnectionWithTesting = up;
             a->closestConnectionDistance = dist;
        }
    }

    // note where Xpert machine is here
    _Xpert = Xpert;

    if (debug) std::cout << "leaving new Facility " << "\n";
}

bool Facility::diagnosisAvailable() {
    return _diagnosis->canDoDiagnosis();

}

bool Facility::testingAvailable(string disease, string test) {

     // assume not testing - check facility attributes to see if testing for this disease
     bool testingAvailableFlag = false;
     
     for (auto a : _facilityAttributes) {

        //std::cout << "test avail disease = " << disease << ", at dis = " << a->disease << ", test = " << test << ",at test = " << a->testName << ", testing " << a->testing << "\n";

        if (a->disease == disease) {
        
            if (a->testName == test) {
            
                testingAvailableFlag = a->testing;
            }
        }
    }

    return testingAvailableFlag;
}

// set testing flag for disease and test - turn tests on 
void Facility::setTestingAvailable(string disease, string test, bool testing) {

    bool debug = false;
    
    Testing* t;
    vector<Test*> testsContainer;

    for (auto a : _facilityAttributes) {

        //std::cout << "set test avail disease = " << disease << ", at dis = " << a->disease << ", test = " << test << ",at test = " << a->testName << ", testing " << a->testing << "\n";

        if (a->disease == disease) {

            if (a->testName == test) {

                a->testing = testing;
            }
        }
    }

    // now go into tests list and turn on the test
    t = getTestingPtr();

    testsContainer = t->getTeststPtr();

    // loop through each of the tests, match up the disease name, do highest priority first
    for (auto checkTest : testsContainer) {

        if ((disease == checkTest->getDiseaseName()) && (test == checkTest->getTestName())) {

            if (debug) std::cout << "disease = " << disease << ", test name = " << checkTest->getTestName() << ",priority = " << checkTest->getPriority() << ", testing = " << checkTest->getDiseaseTesting() << " \n";

            checkTest->setDiseaseTesting(testing);
        }
    }
}

 // Testing at this facility by priority
 bool Facility::testingAvailableByPriority(string disease) {

     // check to see if any testing with a priority of 1 exists here, use it
     
     // assume not testing - check facility attributes to see if testing for this disease
     bool testingAvailableFlag = false;
     
     for (auto a : _facilityAttributes) {

        //std::cout << "test avail disease = " << disease << ", at dis = " << a->disease << ", test = " << test << ",at test = " << a->testName << ", testing " << a->testing << "\n";

        if (a->disease == disease) {
        
            // check to see if any testing with a priority of 1 and testing capiblility exists here, use it
            if ((a->priority == 1) && (a->testing)) {
            
                testingAvailableFlag = a->testing;
            }
        }
    }

    return testingAvailableFlag;
}




// Testing at this facility
bool Facility::testingAvailableAny(string disease) {

     // check to see if any testing for this disease exists here
     
     // assume not testing - check facility attributes to see if testing for this disease
     bool testingAvailableFlag = false;
     
     for (auto a : _facilityAttributes) {

        //std::cout << "test avail disease = " << disease << ", at dis = " << a->disease << ", test = " << test << ",at test = " << a->testName << ", testing " << a->testing << "\n";

        if (a->disease == disease) {
        
            // check to see if any testing capiblility exists here, use it
            if (a->testing) {
            
                testingAvailableFlag = a->testing;
            }
        }
     }

     return testingAvailableFlag;
}



 // Testing at this facility
string Facility::testingAvailableName(string disease) {

     // check to see if any testing for this disease exists here

     // send back testing with highest priority
     int priority = 10;
     
     // assume not testing - check facility attributes to see if testing for this disease
     string testingAvailablename = "";
     
     for (auto a : _facilityAttributes) {

        //std::cout << "test avail disease = " << disease << ", at dis = " << a->disease << ", test = " << test << ",at test = " << a->testName << ", testing " << a->testing << "\n";

        if (a->disease == disease) {
        
            // check to see if any testing capiblility exists here, use it
            if (a->testing) {

                if (a->priority < priority) {
            
                    testingAvailablename = a->testName;

                    priority = a->priority;
                }
            }
        }
    }

    return testingAvailablename;
}





// check to see if facilites on another tier are closer and can be used
void Facility::gatherFacilityStatistics(vector<Facility*> facilities) {

    string type;
    
    for (auto facility : facilities) {
        facility->getType();
    };
}

// no probility available, assume everyone is 1/2 distance away
void Facility::updatePopBins() {
    int i; for (i = 0; i < 10; i++) {
        _popBins[i] = 0;
    }
    _popBins[5] = 1;
}

// fill in population bin probablility if available
void Facility::updatePopBins(float bin[]) {
    int i; for (i = 0; i < 10; i++) {
        _popBins[i] = bin[i];
    }
}


// Get incidence related to specific disease for this facility
float Facility::getDiseaseIncidence(string disease) {

    // loop through disease incidence structure array and try to match up disease name
    // if no match, return 0 for incidence
    float incidence = 0;
    float prpIncPresenting = 1;
    float perPatientPresentInf = 1;
    //float yearInc = 0;
    //float newIncFactor = 1;

    //std::cout << "inside getDiseaseIncidence disease =" << disease  << "\n";

    for (auto facSpec : _facilityAttributes) {

        //std::cout << "inside getDiseaseIncidence loop facSpec->disease =" << facSpec->disease  << " \n";
        
        if (disease == facSpec->disease) {

            // get incidence and proportion of infected patients actually presenting
            incidence = facSpec->incidence;
            prpIncPresenting = facSpec->prpIncidencePresenting;
            perPatientPresentInf = facSpec->perPatientPresentInf;

            // adjust incidence by incidence adjustment factor// for TB correlation - the incidence will be the middle of the range and incidence adjust factor will be between 0 and twice the range
            // divide the incAdjFactor by the mean to get the value - now handled in InputChange.cpp - incidence changed in disease.csv
            //yearInc = incidence * 100000 * 365;

            //newIncFactor = incAdjustment / yearInc;

            //// adjust incidence by incidence adjustment factor then apply
            //incidence = incidence * newIncFactor;

            // adjust base incidence each year if it decreases or increases
            if (facSpec->incAdjType == "linear") {

                incidence = incidence * (1 + (facSpec->incAdjFactor * currentYearInRun));
            }

            //std::cout << "inside getDiseaseIncidence loop incidence = " << incidence << " \n";
        }
    }
    
    //std::cout << "outside getDiseaseIncidence loop incidence = " << incidence << "  prpIncPresenting = " << prpIncPresenting <<"  perPatientPresentInf = " << perPatientPresentInf << " \n";
    
    incidence = incidence * prpIncPresenting * perPatientPresentInf;

    //std::cout << "outside getDiseaseIncidence loop incidence = " << incidence << "  prpIncPresenting = " << prpIncPresenting << " \n";

    // adjust to per person per day
    incidence = incidence / (100000 * 365);
    
    // debugInc
    //std::cout << "outside getDiseaseIncidence loop incidence = " << incidence << "  prpIncPresenting = " << prpIncPresenting << " \n";


    return incidence;
}

// Get incidence related to specific disease for infected patients not presenting to this facility
float Facility::getDiseaseIncidenceNotPresenting(string disease) {

    // loop through disease incidence structure array and try to match up disease name
    // if no match, return 0 for incidence
    float incidence = 0;
    float prpIncPresenting = 1;
    float perPatientPresentInf = 1;
    //float yearInc = 0;
    //float newIncFactor = 1;

    //std::cout << "inside getDiseaseIncidence disease =" << disease  << "\n";

    for (auto facSpec : _facilityAttributes) {

        //std::cout << "inside getDiseaseIncidence loop facSpec->disease =" << facSpec->disease  << " \n";

        if (disease == facSpec->disease) {

            // get incidence and proportion of infected patients actually presenting
            incidence = facSpec->incidence;
            prpIncPresenting = facSpec->prpIncidencePresenting;
            perPatientPresentInf = facSpec->perPatientPresentInf;

            // adjust incidence by incidence adjustment factor// for TB correlation - the incidence will be the middle of the range and incidence adjust factor will be between 0 and twice the range
            // divide the incAdjFactor by the mean to get the value - now handled in InputChange.cpp - incidence changed in disease.csv
            //yearInc = incidence * 100000 * 365;

            //newIncFactor = incAdjustment / yearInc;

            //// adjust incidence by incidence adjustment factor then apply
            //incidence = incidence * newIncFactor;

            // adjust base incidence each year if it decreases or increases
            if (facSpec->incAdjType == "linear") {

                incidence = incidence * (1 + (facSpec->incAdjFactor * currentYearInRun));
            }

            //std::cout << "inside getDiseaseIncidence loop incidence = " << incidence << " \n";
        }
    }

    //std::cout << "outside getDiseaseIncidence loop incidence = " << incidence << "  prpIncPresenting = " << prpIncPresenting << " \n";

    incidence = incidence * (1 - (prpIncPresenting * perPatientPresentInf));

    //std::cout << "outside getDiseaseIncidenceNotPresenting loop incidence = " << incidence << "  prpIncNotPresenting = " << 1-prpIncPresenting << " \n";

    // adjust to per person per day
    incidence = incidence / (100000 * 365);

    // debugInc
    //std::cout << "outside getDiseaseIncidence loop incidence = " << incidence << "  prpIncPresenting = " << prpIncPresenting << " \n";


    return incidence;
}



// Get ratio of uninfected to infected patients related to specific disease for this facility
float Facility::getDiseaseRatioUninfInf(string disease) {

    // loop through disease ratio structure array and try to match up disease name
    // if no match, return 0 for incidence
    float ratioUninfInf = 1;


    //std::cout << "inside getDiseaseRatioUninfInf disease =" << disease  << "\n";

    for (auto facSpec : _facilityAttributes) {

        //std::cout << "inside getDiseaseRatioUninfInf loop facSpec->disease =" << facSpec->disease  << " \n";

        if (disease == facSpec->disease) {

            // get incidence and proportion of infected patients actually presenting
            ratioUninfInf = facSpec->patientsRatioUninfInf;

            //std::cout << "inside getDiseaseRatioUninfInf loop ratioUninfInf = " << ratioUninfInf << " \n";
        }
    }

    //std::cout << "outside getDiseaseRatioUninfInf loop ratioUninfInf = " << ratioUninfInf << " \n";

    return ratioUninfInf;
}


// get no. of time steps to have to wait for sample results to return
int Facility::getTestReturnTime(string disease, string test) {

    // loop through disease returnCycle structure array and try to match up disease name
    // if no match, return 0 for incidence
    int returnCycle = 1;

    for (auto facSpec : _facilityAttributes) {

        if (disease == facSpec->disease) {

            if (test == facSpec->testName) {
                
                returnCycle = facSpec->returnCycle;
            }
        }
    }

    return returnCycle;
}




// Get no. of machines related to specific disease for this facility
int Facility::getDiseaseNoMachines(string disease, string test) {

    // loop through disease incidence structure array and try to match up disease name
    // if no match, return 0 for number of machines
    int noMachines = 1;

    for (auto facSpec : _facilityAttributes) {

        if (disease == facSpec->disease) {

            if (test == facSpec->testName) {

                noMachines = facSpec->noMachines;
            }
        }
    }

    return noMachines;
}


// Get no. of timestep to wait before sending out samples for testing
int Facility::getBatchWaitTime(string disease) {

    // loop through disease incidence structure array and try to match up disease name
    // if no match, return 1 for batch wait time
    int batchWaitTime = 1;

    for (auto facSpec : _facilityAttributes) {

        if (disease == facSpec->disease) {

            batchWaitTime = facSpec->batchWaitTime;
        }
    }

    return batchWaitTime;
}




// Create Patents for each disease based on catchment size and disease attributes
// most of the work is done in Diagnosis class
void Facility::createPatients(vector<string> dList, vector<Disease*> diseases, int timeStep) {

    //std::cout << "inside Facility createPatients \n";

   // create the patients with and without the disease
    _diagnosis->createPatients(_name, dList, _population, _binInc, _popBins, diseases, timeStep);

}


// update patients for this timestep with respect to Samples
void Facility::updatePatients(vector<Disease*> diseases, vector<Test*> testList, int timeStep) {

    //std::cout << "inside Facility updatePatients \n";

   // update status of patient based on Sample(test results) - oass facility pointer as this
    if (_diagnosis->canDoDiagnosis()) {

        _diagnosis->updatePatients( _testing, diseases, testList, timeStep);
    }

}

// Handle Sample testing for this timestep
void Facility::updateSamples(int timeStep, string disease, vector<Disease*> dis) {
    
    bool debug = false;

    if (debug) std::cout << "inside Facility updateSamples facility = " << _name << " \n";

   // create the patients with and without the disease
     for (auto a : _facilityAttributes) {

        if (a->disease == disease) {
        
            if (a->testing) {

                if (debug) {

                    std::cout << "inside Facility going to testing-updateSamples \n";

                                std::cout << "facattr disease = " << a->disease << ", test = " << a->testName << ", testing = " << a->testing;
                                std::cout << ", facattr incidence = " << a->incidence << ", facattr incAdjType = " << a->incAdjType << ", facattr incAdjFactor = " << a->incAdjFactor;
                                std::cout << ", prpIncPresent = " << a->prpIncidencePresenting << ", patRatioUninfInf = " << a->patientsRatioUninfInf << ", returnCycle = " << a->returnCycle;
                                std::cout << ", no machines = " << a->noMachines << ", no maxReagentPerCycle = " << a->maxReagentPerCycle;
                                std::cout << ", no timeStepsInCycle = " << a->timeStepsInCycle;
                                std::cout << ", closest facility = " << a->closetConnectionWithTesting << ", closest distance = " << a->closestConnectionDistance << "\n";
                }

                _testing->updateSamples(timeStep, dis);

                if (debug) std::cout << "inside Facility after testing-updateSamples \n";
            }
        }
    }

    

}


// name of next closest Testing Facility for disease
string Facility::getClosestName(string disease) {
    
    string closestName = "";
    
    // loop through tests and find the one that matcheds for diseasefor this facility
    for (auto facSpec : _facilityAttributes) {

        if (disease == facSpec->disease) {

            closestName = facSpec->closetConnectionWithTesting;
        }
    } 
    
    return closestName;

}


// name of next closest Testing Facility for disease and test
string Facility::getClosestNameByTest(string disease, string test) {
    
    string closestName = "";
    
    // loop through tests and find the one that matcheds for disease and test for this facility
    for (auto facSpec : _facilityAttributes) {

        if (disease == facSpec->disease) {

            if (test == facSpec->testName) {

                closestName = facSpec->closetConnectionWithTesting;
            }
        }
    } 
    
    return closestName;

}


// name of next closest Testing Facility for disease = if you change this logic you have to change getClosestDistByDisease
string Facility::getClosestNameByDisease(string disease, string facility) {

    string closestName = "";

    // loop through tests looking for any for the disease and if closest for one of the
    //  the tests is current facility - use that one
    for (auto facSpec : _facilityAttributes) {

        if (disease == facSpec->disease) {

            closestName = facSpec->closetConnectionWithTesting;

            if (closestName == facility) {

                // if testing here, you're done
                break;
            }
        }
    }

    return closestName;

}

// name of next closest Testing Facility distancefor disease = if you change this logic you have to chagne getClosestNameByDisease
float Facility::getClosestDistByDisease(string disease, string facility) {

    float closesDist = 0;
    string closestName = "";

    // loop through tests looking for any for the disease and if closest for one of the
    //  the tests is current facility - use that one
    for (auto facSpec : _facilityAttributes) {

        if (disease == facSpec->disease) {

            closestName = facSpec->closetConnectionWithTesting;
            closesDist = facSpec->closestConnectionDistance;

            if (closestName == facility) {

                // if testing here, you're done
                break;
            }
        }
    }

    return closesDist;

}




// name of next closest Testing Facility for this disease and test
void Facility::setClosestName(string disease, string test, string name) {
    
    for (auto facSpec : _facilityAttributes) {

        if (disease == facSpec->disease) {

            if (test == facSpec->testName) {

                facSpec->closetConnectionWithTesting = name;
            }
        }
    }
}

// distance to next closest Testing Facility for this disease and test
float Facility::getClosestDistance(string disease, string test) { 
    
    float closestDist = 0;

    for (auto facSpec : _facilityAttributes) {

        if (disease == facSpec->disease) {

            if (test == facSpec->testName) {

                closestDist = facSpec->closestConnectionDistance;
            }
        }
    }

    return closestDist;

}

// distance to next closest Testing Facility for this disease and test
void Facility::setClosestDistance(string disease, string test, float dist) { 

    for (auto facSpec : _facilityAttributes) {

        if (disease == facSpec->disease) {

            if (test == facSpec->testName) {

                facSpec->closestConnectionDistance = dist;
            }
        }
    }
}

// set closest connection pointer for this disease and test
Facility* Facility::getClosestPtr(string disease, string test) { 
    
    Facility* closestPtr;
    string name;

    name = getClosestNameByTest(disease, test);

    closestPtr = facilitiesList->getPtr(name);

    return closestPtr;

}

// create testing available container
vector<testingAvailableAttr*> Facility::createTestingAvailObjects() {


    vector<testingAvailableAttr*> testingContainer;
    
    testingAvailableAttr* testingAttr = new testingAvailableAttr;

    testingAttr->disease = "TB";
    testingAttr->testing = 0;

    testingContainer.push_back(testingAttr);

    testingAvailableAttr* testingAttr1 = new testingAvailableAttr;
    testingAttr->disease = "HIV";
    testingAttr->testing = 0;
    
    testingContainer.push_back(testingAttr1);

    testingAvailableAttr* testingAttr2 = new testingAvailableAttr;
    testingAttr->disease = "HCV";
    testingAttr->testing = 0;
    
    testingContainer.push_back(testingAttr2);

    testingAvailableAttr* testingAttr3 = new testingAvailableAttr;
    testingAttr->disease = "YF";
    testingAttr->testing = 0;
    
    testingContainer.push_back(testingAttr3);

    testingAvailableAttr* testingAttr4 = new testingAvailableAttr;
    testingAttr->disease = "Meningitis";
    testingAttr->testing = 0;
    
    testingContainer.push_back(testingAttr4);

    testingAvailableAttr* testingAttr5 = new testingAvailableAttr;
    testingAttr->disease = "Measles";
    testingAttr->testing = 0;

    testingContainer.push_back(testingAttr5);

    testingAvailableAttr* testingAttr6 = new testingAvailableAttr;
    testingAttr->disease = "Disease1";
    testingAttr->testing = 0;

    testingContainer.push_back(testingAttr6);

    testingAvailableAttr* testingAttr7 = new testingAvailableAttr;
    testingAttr->disease = "Disease2";
    testingAttr->testing = 0;

    testingContainer.push_back(testingAttr7);

    return testingContainer;

}


// get the name of the test that can be done here for this disease - by priority
string Facility::getTestName(string disease){

    string test = "";
    int priority = 10;
    
    // loop through facility attributes and find the tests that match the disease and send back one with highest priority that is tested here
    for (auto facSpec : _facilityAttributes) {

        if ((disease == facSpec->disease) && (facSpec->testing)){

            if (facSpec->priority  < priority) {

                test = facSpec->testName;

                priority = facSpec->priority;
            }
        }
    }

    return test;
}



// find incidence for a given disease and year
float Facility::getIncidence(string disease, int yearNo) {

     // check to see if any testing for this disease exists here
     
    float incidence = 0;
    //float yearInc = 0;
    //float newIncFactor = 1;
     
    for (auto facSpec : _facilityAttributes) {

        //std::cout << "test avail disease = " << disease << ", at dis = " << a->disease << ", test = " << test << ",at test = " << a->testName << ", testing " << a->testing << "\n";

        if (facSpec->disease == disease) {
        
            incidence = facSpec->incidence;
            
            // now handled in inputChange
            //yearInc = incidence * 100000 * 365;

            //newIncFactor = incAdjustment / yearInc;

            //// adjust incidence by incidence adjustment value
            //incidence = incidence * newIncFactor;
            
            // adjust incidence by any annual increase or decrease in incidence
            if (facSpec->incAdjType == "linear") {
            
                for (int i = 0; i < currentYearInRun; i++) {

                    incidence = incidence * (1 + facSpec->incAdjFactor);
                }
            }

            incidence = incidence * facSpec->prpIncidencePresenting;
            
            // adjust incidence to per person per day
            incidence = incidence / (100000 * 365);
        }
    }
    
    // debugInc
    //std::cout << "inside getIncidence incidence = " << incidence << " \n";

    return incidence;
}

// find incidence of those patients not presenting for diagnosis for a given disease and year
float Facility::getIncidenceNotPresenting(string disease, int yearNo) {

    // check to see if any testing for this disease exists here

    float incidence = 0;
    //float yearInc = 0;
    //float newIncFactor = 1;

    for (auto facSpec : _facilityAttributes) {

        //std::cout << "test avail disease = " << disease << ", at dis = " << a->disease << ", test = " << test << ",at test = " << a->testName << ", testing " << a->testing << "\n";

        if (facSpec->disease == disease) {

            incidence = facSpec->incidence;

            // now handled in inputChange
            //yearInc = incidence * 100000 * 365;

            //newIncFactor = incAdjustment / yearInc;

            //// adjust incidence by incidence adjustment value
            //incidence = incidence * newIncFactor;

            // adjust incidence by any annual increase or decrease in incidence
            if (facSpec->incAdjType == "linear") {

                for (int i = 0; i < currentYearInRun; i++) {

                    incidence = incidence * (1 + facSpec->incAdjFactor);
                }
            }

            incidence = incidence * (1 - facSpec->prpIncidencePresenting);

            // adjust incidence to per person per day
            incidence = incidence / (100000 * 365);
        }
    }

    // debugInc
    //std::cout << "inside getIncidence incidence = " << incidence << " \n";

    return incidence;
}


// find incidence adjustment factor for a given disease
float Facility::getIncAdjFactor(string disease) {

    // check to see if any testing for this disease exists here

    float incAdjFactor = 0;
    //float yearInc = 0;
    //float newIncFactor = 1;

    for (auto facSpec : _facilityAttributes) {

        //std::cout << "test avail disease = " << disease << ", at dis = " << a->disease << ", test = " << test << ",at test = " << a->testName << ", testing " << a->testing << "\n";

        if (facSpec->disease == disease) {

            incAdjFactor = facSpec->incAdjFactor;
        }
    }

    //std::cout << "inside getIncidence incAdjFactor = " << incAdjFactor << " \n";

    return incAdjFactor;
}



// find proportion of infected patients presenting for this disease
float Facility::getPrpIncidencePresenting(string disease) {

    // check to see if any testing for this disease exists here

    float prpIncPresenting = 0;
    //float yearInc = 0;
    //float newIncFactor = 1;

    for (auto facSpec : _facilityAttributes) {

        //std::cout << "test avail disease = " << disease << ", at dis = " << a->disease << ", test = " << test << ",at test = " << a->testName << ", testing " << a->testing << "\n";

        if (facSpec->disease == disease) {

            prpIncPresenting = facSpec->prpIncidencePresenting;
        }
    }

    //std::cout << "inside getIncidence prpIncPresenting = " << prpIncPresenting << " \n";

    return prpIncPresenting;
}

// downstream facility connections
int Facility::getPopulation() {
    
    int population = 0;
    int startIndex = 0;
    int endIndex = 0;
    bool debug = false;

    population = _population;

    startIndex = referenceYear - 2015;
    endIndex = startIndex + currentYearInRun;
    
    // debug - true
    if (debug) {
        if ((referenceYear + currentYearInRun) > startingYear) {
            std::cout << "inside get population = " << population << " years in run = " << yearsInRun << " cur year = " << currentYearInRun;
            std::cout << " ref year = " << referenceYear << " start year = " << startingYear << " skip = " << skipYears;
            std::cout << " startIndex = " << startIndex << " endIndex = " << endIndex << " \n";
        }
    }

    if (currentYearInRun > 0) {

        //std::cout << " popb = " << population;
        for (int i = startIndex; i < endIndex; i++) {

            // multiply by growth rate for x number of years
            population = population * popGrowthRateArray[i][1];
            //std::cout << " rate = " << i << " ," << popGrowthRateArray[i][1];
        }
        //std::cout << " popa = " << population << " \n";

        population = int(population);
    }
    
    return population; 


};


Facility::~Facility() {
    //cout<<"Removing facility "<<_id<<endl;
}


