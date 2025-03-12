//
//  Patient.cpp
//  mdrtb
//
//  Created by Lad Dombrowski on 1/10/2020.
//
#include <random>
#include <chrono>
#include "Patient.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include "GlobalParameters.h"
#include "CreateAgents.h"

using namespace std;




Patient::Patient() {
};

Patient::Patient(string facName, string disease, float distance, bool infected) {

    int randInt;

    _disease = disease;
    _facility = facName;
    _facilityTier = 0;
    _district = "";
    _districtIndex = 0;
    _region = "";
    _regionIndex = 0;
    _distance = distance;
    _popBinIndex = 1;
    _infected = infected;

    _status = 1;            // patient currently at facility
    _prevStatus = 1;        // patient currently at facility
    _preTestMDCD = false;
    _preTestMDCDTime = 0;
    _preTestLtoF = false;
    _preTestLtoFCount = 0;
    _relapse = false;
    _represent = false;
    _live = true;           // patient is alive
    
    _active = true;         // patient is active

    _treated = false;       // patient is untreated

    _tracker = false;       // patient is not being tracked

    // get random number between 0 and 63 (life expectancy)
    //unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    //std::mt19937 generator(seed);
    //std::uniform_int_distribution<int> distribution(1, 63);

    _age = getNextRandomNo100() % 63;

    // generate sex
    _gender = getNextRandomNo100() % 2;

    if (_infected) {
        // find out if this strain is or will become drug resistant
        std::uniform_int_distribution<int> distribution(1, 100);
        randInt = getNextRandomNo100();

        if (randInt < pecSamplingForRegistry) {
            _trackForRegistry = true;
        }
        else {
            _trackForRegistry = false;
        }
    }
    else {
        // non-infected patients
        _trackForRegistry = false;
    }

    _noTreatReason = "";

    _lifeExpectancy = 63;

     _timeOfDeath = -1;
     _timeFromOnset = 0;
     _timeFromOnsetTilTreat = 0;
    _daysInTreatment = 0;
    _daysNotInTreatment = 0;
    _createTime = 0;  
    _presentTime = 0;
    _resultsReturned = 0;
    _representTime = 0;
    _noOfClinicalVisits = 0;

    _infectiousnesEndsTime = 0;
    _infectiousness = 0;
    _facilityReturnTime = 0;
    _relapseStartTime = 0;
    _noReturnsForResults = 0;
    _timestepPatientStartedTreatment = 0;   
    _timestepPatientReceivedMDResults = 0; 
    _timestepPatientLtoF = 0;  

    _testPos = false;
    _mdPos = false;
    
    _sampleTest = "none";
    _sampleTestStatus = "none";

    _sample = nullptr;
}


Patient::Patient(string facName, string district, int distIndex, string region, int regIndex, string disease, float distance, int popBinIndex, bool infected, int status, string testName, int timestep) {

    int randInt;

    _disease = disease;
    _facility = facName;
    _facilityTier = 0;
    _district = district;
    _districtIndex = distIndex;
    _region = region;
    _regionIndex = regIndex;
    _distance = distance;
    _popBinIndex = popBinIndex;
    _infected = infected;
    _fromTransmission = false;
    _status = status;
    _prevStatus = status; 
    _preTestMDCD = false;
    _preTestMDCDTime = 0;
    _preTestLtoF = false;
    _preTestLtoFCount = 0;
    _relapse = false;
    _represent = false;

    _live = true;           // patient is alive

    _active = true;         // patient is active
    
    _treated = false;       // patient is untreated

    _tracker = false;       // patient is not being tracked

    // get random number between 0 and 63 (life expectancy)
    //unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    //std::mt19937 generator(seed);
    //std::uniform_int_distribution<int> distribution(1, 63);

    _age = getNextRandomNo100() % 63;

    // generate sex
    _gender = getNextRandomNo100() % 2;

    if (_infected) {
        // find out if this strain is or will become drug resistant
        //std::uniform_int_distribution<int> distribution(1, 100);
        randInt = getNextRandomNo100();

        if (randInt < pecSamplingForRegistry) {
            _trackForRegistry = true;
        }
        else {
            _trackForRegistry = false;
        }
    }
    else {
        // non-infected patients
        _trackForRegistry = false;
    }

    _noTreatReason = "";
    
    _lifeExpectancy = 63;

    _timeOfDeath = -1;
    _timeFromOnset = 0;
    _timeFromOnsetTilTreat = 0;
    _daysInTreatment = 0;
    _daysNotInTreatment = 0;
    _createTime = timestep;
    _presentTime = timestep;
    _resultsReturned = 0;
    _representTime = 0;
    _noOfClinicalVisits = 0;

    _timestepPatientCreated = timestep;
    _timestepPatientNoLongerTracked = 0;
    _infectiousnesEndsTime = timestep;
    _infectiousness = 0;

    _facilityReturnTime = 0;
    _relapseStartTime = 0;
    _noReturnsForResults = 0;
    _timestepPatientStartedTreatment = 0;   
    _timestepPatientReceivedMDResults = 0; 
    _timestepPatientLtoF = 0;

    _testPos = false;
    _mdPos = false;

    _sampleTest = testName;
    _sampleTestStatus = "none";

    _sample = nullptr;
}

Patient::Patient(string facName, string district, int distIndex, string region, int regIndex, string disease, float distance, int popBinIndex, bool infected, int status, string testName, int timestep, bool debug) {

    int randInt;

    _disease = disease;
    _facility = facName;
    _facilityTier = 0;
    _district = district;
    _districtIndex = distIndex;
    _region = region;
    _regionIndex = regIndex;
    _distance = distance;
    _popBinIndex = 1;
    _infected = infected;
    _fromTransmission = false;
    _status = status;
    _prevStatus = status; 
    _preTestMDCD = false;
    _preTestMDCDTime = 0;
    _preTestLtoF = false;
    _preTestLtoFCount = 0;
    _relapse = false;
    _represent = false;

    _live = true;           // patient is alive
    
    _active = true;         // patient is active
    
    _treated = false;       // patient is untreated

    _tracker = false;       // patient is not being tracked

    // get random number between 0 and 63 (life expectancy)
    //unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    //std::mt19937 generator(seed);
    //std::uniform_int_distribution<int> distribution(1, 63);

    if (debug) std::cout << "inside make patient timestep = " << timestep << "\n";
    
    _age = getNextRandomNo100() % 63;

    if (debug) std::cout << "inside make patient after age" << "\n";
    
    // generate sex
    _gender = getNextRandomNo100() % 2;

    if (debug) std::cout << "inside make patient after gender" << "\n";
    
    if (_infected) {
        // find out if this strain is or will become drug resistant
        //std::uniform_int_distribution<int> distribution(1, 100);
        randInt = getNextRandomNo100();

        if (randInt < pecSamplingForRegistry) {
            _trackForRegistry = true;
        }
        else {
            _trackForRegistry = false;
        }
    }
    else {
        // non-infected patients
        _trackForRegistry = false;
    }

    if (debug) std::cout << "inside make patient after registry" << "\n";

    _noTreatReason = "";
    
    _lifeExpectancy = 63;

    _timeOfDeath = -1;
    _timeFromOnset = 0;
    _timeFromOnsetTilTreat = 0;
    _daysInTreatment = 0;
    _daysNotInTreatment = 0;
    _createTime = timestep;
    _presentTime = timestep;
    _resultsReturned = 0;
    _representTime = 0;
    _noOfClinicalVisits = 0;

    _timestepPatientCreated = timestep;
    _timestepPatientNoLongerTracked = 0;
    _infectiousnesEndsTime = timestep;
    _infectiousness = 0;

    _facilityReturnTime = 0;
    _relapseStartTime = 0;
    _noReturnsForResults = 0;
    _timestepPatientStartedTreatment = 0;   
    _timestepPatientReceivedMDResults = 0; 
    _timestepPatientLtoF = 0; 

    _testPos = false;
    _mdPos = false;

    _sampleTest = testName;
    _sampleTestStatus = "none";

    _sample = nullptr;

    if (debug) std::cout << "leaving make patient" << "\n";

}


Patient::Patient(string facName, string district, int distIndex, string region, int regIndex, string disease, float distance, int popBinIndex, bool infected, int status, string testName, int age, int gender, int timestep) {

    int randInt;

    _disease = disease;
    _facility = facName;
    _facilityTier = 0;
    _district = district;
    _districtIndex = distIndex;
    _region = region;
    _regionIndex = regIndex;
    _distance = distance;
    _popBinIndex = popBinIndex;
    _infected = infected;
    _fromTransmission = false;
    _status = status;
    _prevStatus = status;
    _preTestMDCD = false;
    _preTestMDCDTime = 0;
    _preTestLtoF = false;
    _preTestLtoFCount = 0;
    _relapse = false;
    _represent = false;

    _live = true;           // patient is alive

    _active = true;         // patient is active

    _treated = false;       // patient is untreated

    _tracker = false;       // patient is not being tracked

    // get random number between 0 and 63 (life expectancy)
    //unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    //std::mt19937 generator(seed);
    //std::uniform_int_distribution<int> distribution(1, 63);

    _age = age;

    // generate sex
    _gender = gender;

    if (_infected) {
        // find out if this strain is or will become drug resistant
        //std::uniform_int_distribution<int> distribution(1, 100);
        randInt = getNextRandomNo100();

        if (randInt < pecSamplingForRegistry) {
            _trackForRegistry = true;
        }
        else {
            _trackForRegistry = false;
        }
    }
    else {
        // non-infected patients
        _trackForRegistry = false;
    }

    _noTreatReason = "";

    _lifeExpectancy = 63;

    _timeOfDeath = -1;
    _timeFromOnset = 0;
    _timeFromOnsetTilTreat = 0;
    _daysInTreatment = 0;
    _daysNotInTreatment = 0;
    _createTime = timestep;
    _presentTime = timestep;
    _resultsReturned = 0;
    _representTime = 0;
    _noOfClinicalVisits = 0;

    _timestepPatientCreated = timestep;
    _timestepPatientNoLongerTracked = 0;
    _infectiousnesEndsTime = timestep;
    _infectiousness = 0;

    _facilityReturnTime = 0;
    _relapseStartTime = 0;
    _noReturnsForResults = 0;
    _timestepPatientStartedTreatment = 0;
    _timestepPatientReceivedMDResults = 0;
    _timestepPatientLtoF = 0;

    _testPos = false;
    _mdPos = false;

    _sampleTest = testName;
    _sampleTestStatus = "none";

    _sample = nullptr;
}


Patient::~Patient() {
    //cout<<"Remove Patient"<<_id<<endl;
}



