//
//  Sample.cpp
//  mdrtb
//
//  Created by L. Dombrowski on 1/08/2020.
//  Copyright (c) 2015 Trinity. All rights reserved.
//
#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include "Sample.h"


using namespace std;


Sample::Sample() {
};

Sample::Sample(Patient* id, string facility, string testingFacility, bool infected, int priority) {

    // store away information on creation
    _patient = id;
    _facility = facility;
    _facilityTier = 0;
    _facilityRegion = "";
    _facilityDistrict = "";
    _testingFacility = testingFacility;
    _testingFacilityDistance = 0;
    _infected = infected;
    _positive = false;
    _disease = "";
    _priority = priority;
    _status = 0;
    _prevStatus = 0;
    _age = 0;
    _batchWaitTime = 1;
    _batchWaitTimeSave = 0;
    _wait = 0;
    _waitForResource = 0;
    _createTime = 0;
    _createTimeOrig = 0;
    _timeTested = -1;
    _resultsReturned = 0;
    _resultsReturnedOrigFacility = 0;
    _resultsRecievedOrigFacility = 0;
    _transitTime = 0;
    _transitTimeSave = 0;
    _transitTimeReturn = 0;
    _transitTimeReturnSave = 0;


}

Sample::Sample(Patient* id, string facility, string testingFacility, float distance, bool infected, int priority) {

    // store away information on creation
    _patient = id;
    _facility = facility;
    _facilityTier = 0;
    _facilityRegion = "";
    _facilityDistrict = "";
    _testingFacility = testingFacility;
    _testingFacilityDistance = distance;
    _infected = infected;
    _positive = false;
    _disease = "";
    _priority = priority;
    _status = 0;
    _prevStatus = 0;
    _age = 0;
    _batchWaitTime = 1;
    _batchWaitTimeSave = 0;
    _wait = 0;
    _waitForResource = 0;
    _createTime = 0;
    _createTimeOrig = 0;
    _timeTested = -1;
    _resultsReturned = 0;
    _resultsReturnedOrigFacility = 0;
    _resultsRecievedOrigFacility = 0;
    _transitTime = 0;
    _transitTimeSave = 0;
    _transitTimeReturn = 0;
    _transitTimeReturnSave = 0;


}


Sample::Sample(Patient* id, string facility, string testingFacility, float distance, string disease, bool infected, int priority, float perLostSamples) {

    // store away information on creation
    _patient = id;
    _facility = facility;
    _facilityTier = 0;
    _facilityRegion = "";
    _facilityDistrict = "";
    _testingFacility = testingFacility;
    _testingFacilityDistance = distance;
    _infected = infected;
    _positive = false;
    _disease = disease;
    _priority = priority;
    _status = 0;
    _prevStatus = 0;
    _age = 0;
    _batchWaitTime = 1;
    _batchWaitTimeSave = 0;
    _wait = 0;
    _waitForResource = 0;
    _perLostSamples = perLostSamples;
    _createTime = 0;
    _createTimeOrig = 0;
    _timeTested = -1;
    _resultsReturned = 0;
    _resultsReturnedOrigFacility = 0;
    _resultsRecievedOrigFacility = 0;
    _transitTime = 0;
    _transitTimeSave = 0;
    _transitTimeReturn = 0;
    _transitTimeReturnSave = 0;


}


Sample::Sample(Patient* id, string facility, int facilityTier, string testingFacility, float distance, string disease, bool infected, int priority, float perLostSamples) {

    // store away information on creation
    _patient = id;
    _facility = facility;
    _facilityTier = facilityTier;
    _facilityRegion = "";
    _facilityDistrict = "";
    _testingFacility = testingFacility;
    _testingFacilityDistance = distance;
    _infected = infected;
    _positive = false;
    _disease = disease;
    _priority = priority;
    _status = 0;
    _prevStatus = 0;
    _age = 0;
    _batchWaitTime = 1;
    _batchWaitTimeSave = 0;
    _wait = 0;
    _waitForResource = 0;
    _perLostSamples = perLostSamples;
    _createTime = 0;
    _createTimeOrig = 0;
    _timeTested = -1;
    _resultsReturned = 0;
    _resultsReturnedOrigFacility = 0;
    _resultsRecievedOrigFacility = 0;
    _transitTime = 0;
    _transitTimeSave = 0;
    _transitTimeReturn = 0;
    _transitTimeReturnSave = 0;


}


Sample::Sample(Patient* id, string facility, int facilityTier, string region, string district, string testingFacility, float distance, string disease, bool infected, int priority, float perLostSamples) {

    // store away information on creation
    _patient = id;
    _facility = facility;
    _facilityTier = facilityTier;
    _facilityRegion = region;
    _facilityDistrict = district;
    _testingFacility = testingFacility;
    _testingFacilityDistance = distance;
    _infected = infected;
    _positive = false;
    _disease = disease;
    _priority = priority;
    _status = 0;
    _prevStatus = 0;
    _age = 0;
    _batchWaitTime = 1;
    _batchWaitTimeSave = 0;
    _wait = 0;
    _waitForResource = 0;
    _perLostSamples = perLostSamples;
    _createTime = 0;
    _createTimeOrig = 0;
    _timeTested = -1;
    _resultsReturned = 0;
    _resultsReturnedOrigFacility = 0;
    _resultsRecievedOrigFacility = 0;
    _transitTime = 0;
    _transitTimeSave = 0;
    _transitTimeReturn = 0;
    _transitTimeReturnSave = 0;


}



void Sample::updateSampleStatus(int newStatus, int wait) {

    _prevStatus = _status;
    _status = newStatus;
    _wait = wait;
}

void Sample::updateSampleFacility(string facility) {
    _facility = facility;
}



Sample::~Sample() {
    //cout<<"I'm dying "<<_id<<endl;
}
//---------------------------------------





