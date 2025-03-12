//
//  Sample.h
//
//
//  Created by L. Dombrowski on 1/8/2020.
//


#ifndef SAMPLE_H
#define SAMPLE_H

#include <string>
#include <vector>

#include "Patient.h"
#include "Test.h"
class Patient;

using namespace  std;


class Sample {
private:

    Patient* _patient;

    // status of sample
    // SampCreatedUnTest = 0;			// created but untested
    // SampCreatedUnTestBatch = 1;		// created, untested waiting to be sent out in batch mode
    // SampUnTestInTransit = 2;	        // in transit
    // SampUnTestWaitRepresent = 3;	    // waiting for patient to represent
    // SampWaitForTest = 4;		        // waiting for test
    // SampLostExpired = 5;		        // lost/expired
    // SampTestedInTransit = 6;	        // results in transit back to orig facility
    // SampReturnedAvail = 7;	        // results returned - available
    // SampReturnedLostExpired = 8;	    // loss to follow up
    // SampLtoF = 9;			            // loss to follow up
    // SampPatDead = 10;			        // patient dead
    // SampPatuntreatedInactive = 11;	// patient went untreated, sample now longer active
    int _status;
    int _prevStatus;
    
    // how many time steps since it was created
    int _age;

    // time steps in batch cycle before sending out for testing
    int _batchWaitTime;

    // time to wait before batch send out
    int _batchWaitTimeSave;

    // time steps waiting without testing
    int _wait;

    // time steps waited for resources to become available for testing
    int _waitForResource;

    // time step sample was created - can be reset with a representation
    int _createTime;

    // time step sample was created - originally
    int _createTimeOrig;

    // time sample was tested
    int _timeTested;

    // time steps in transit to testing facility
    int _transitTime;
    int _transitTimeSave;

    // time steps in transit returning to original facility
    int _transitTimeReturn;
    int _transitTimeReturnSave;

    // percentage of samples lost in transit - depends on disease and increases with distance
    float _perLostSamples;

    // time step test result are returned to testing facility
    int _resultsReturnedOrigFacility;

    // time step test result are recieved in diagnosis facility
    int _resultsRecievedOrigFacility;

    // time step test result was presented to patient
    int _resultsReturned;

    // whether sample is infected or not - this just has to do with whether 
    // to look for a patient on return
    bool _infected;

    // determine whether sample returned is positve or negative
    bool _positive;

    // disease name
    string _disease;

    // test used for this sample
    string _test;
    Test* _testPtr;

    // facility sample came from 
    string _facility;

    // tier of facility sample came from 
    int _facilityTier;

    // region of facility sample came from 
    string _facilityRegion;

    // district of facility sample came from 
    string _facilityDistrict;

    // facility sample tested at 
    string _testingFacility;
    float _testingFacilityDistance;

    // priority - larger number - higher priority
    // some diseases may have a higher priority than others
    int _priority;

public:
    Sample();
    Sample(Patient* PatientId, string facility, string testingFacility, bool infected, int priority);
    Sample(Patient* PatientId, string facility, string testingFacility, float distance, bool infected, int priority);
    Sample(Patient* PatientId, string facility, string testingFacility, float distance, string disease, bool infected, int priority, float perLostSamples);
    Sample(Patient* PatientId, string facility, int facilityTier, string testingFacility, float distance, string disease, bool infected, int priority, float perLostSamples);
    Sample(Patient* PatientId, string facility, int facilityTier, string region, string district, string testingFacility, float distance, string disease, bool infected, int priority, float perLostSamples); 
    ~Sample();



    // get Patient pointer
    inline Patient* getPatientPtr() { return _patient; };

    // set Patient pointer
    inline void setPatientPtr(Patient* patient) { _patient = patient; };

    // get original facility
    inline string getOrigFacility() { return _facility; };

    // get original facility tier
    inline int getOrigFacilityTier() { return _facilityTier; };

    // set original facility tier
    inline void setOrigFacilityTier(int tier) { _facilityTier = tier; };

    // get original facility region
    inline string getOrigFacilityRegion() { return _facilityRegion; };

    // set original facility region
    inline void setOrigFacilityRegion(string region) { _facilityRegion = region; };

    // get original facility district
    inline string getOrigFacilityDistrict() { return _facilityDistrict; };

    // set original facility district
    inline void setOrigFacilityDistrict(int district) { _facilityDistrict = district; };

    // get testing facility name
    inline string getTestingFacility() { return _testingFacility; };

    // get current status
    inline int getStatus() { return _status; };

    // update the current status of sample
    inline void setStatus(int newStatus) { _prevStatus = _status; _status = newStatus; };
    
    // get previous status of sample
    inline int getPrevStatus() { return _prevStatus; };

    // update the previous status of sample
    inline void setPrevStatus(int prevStatus) { _prevStatus = prevStatus; };

    // get status of patient if infected or not
    inline bool getInfected() { return _infected; };

    // get result of Sample - positive or negative
    inline bool isPositive() { return _positive; };

    // set result of Sample - positive or negative
    inline void setPositive(bool flag) { _positive = flag; };

    // set result of Sample - positive or negative
    inline bool getPositive() { return _positive; };

    // get age
    inline int getAge() { return _age; };

    // set age
    inline void setAge(int age) { _age = age; };

    // increment days waiting
    inline void incrementAge() { _age++; };

    // get transit time to testing
    inline int getTransitTime() { return _transitTime; };

    // set transit time to testing
    inline void setTransitTime(int time) { _transitTime = time; };

    // get transit time to testing and save for reference
    inline int getTransitTimeSave() { return _transitTimeSave; };

    // set transit time to testing for reference
    inline void setTransitTimeSave(int time) { _transitTimeSave = time; };

    // get time sample was tested
    inline int getTestedTime() { return _timeTested; };

    // set time sample was tested
    inline void setTestedTime(int timeTested) { _timeTested = timeTested; };

    // decrement transit time 
    inline void reduceTransitTime() { _transitTime--; if (_transitTime < 0) { _transitTime = 0; }; };

    // get transit time to return sample
    inline int getReturnTransitTime() { return _transitTimeReturn; };

    // set transit time to return sample
    inline void setReturnTransitTime(int time) { _transitTimeReturn = time; };

    // get transit time to return sample for reference
    inline int getReturnTransitTimeSave() { return _transitTimeReturnSave; };

    // set transit time to return sample for reference
    inline void setReturnTransitTimeSave(int time) { _transitTimeReturnSave = time; };

    // decrement transit time 
    inline void reduceReturnTransitTime() { _transitTimeReturn--; if (_transitTimeReturn < 0) { _transitTimeReturn = 0; }; };
    
    // setpercentage no. of lost samples
    inline void setPerLostSamples(float perLostSamples) { _perLostSamples = perLostSamples; };

    // get percentage of lost samples
    inline float getPerLostSamples() { return _perLostSamples; };

    // set time step sample was created
    inline void setCreateTime(int createTime) { _createTimeOrig = _createTime; _createTime = createTime; };

    // get time step sample was created
    inline int getCreateTime() { return _createTime; };

    // set time step sample was created originally, could be different from _createTime if patient represents
    inline void setCreateTimeOrig(int createTimeOrig) { _createTimeOrig = createTimeOrig; };

    // get time step sample was created originally
    inline int getCreateTimeOrig() { return _createTimeOrig; };

    // set time step test results were returned to original facility
    inline void setResultsReturnedtoOrigFacilityTime(int resultsReturnedOrigFacility) { _resultsReturnedOrigFacility = resultsReturnedOrigFacility; };

    // get time step test results were returned to original facility
    inline int getResultsReturnedToOrigFacilityTime() { return _resultsReturnedOrigFacility; };

    // set time step test results recieved at original facility
    inline void setResultsRecievedtoOrigFacilityTime(int resultsRecievedOrigFacility) { _resultsRecievedOrigFacility = resultsRecievedOrigFacility; };

    // get time step test results recieved atoriginal facility
    inline int getResultsRecievedToOrigFacilityTime() { return _resultsRecievedOrigFacility; };

    // set time step test results were returned
    inline void setResultsReturnedTime(int resultsReturned) { _resultsReturned = resultsReturned; };

    // get time step test results were returned
    inline int getResultsReturnedTime() { return _resultsReturned; };

    // update the current status of sample with waiting time
    void updateSampleStatus(int newStatus, int noOfDaysToWait);

    //copy all the parameters from new mp file
    void updateSampleFacility(string newFacility);
    
    // set time steps in batch cycle before sending out for testing
    inline void setBatchWaitTime(int batchWaitTime) { _batchWaitTime = batchWaitTime; };

    // get time steps in batch cycle before sending out for testing
    inline int getBatchWaitTime() { return _batchWaitTime; };

    // set time steps sample will have to wait in batch cycle before sending out for testing
    inline void setBatchWaitTimeSave(int batchWaitTimeSave) { _batchWaitTimeSave = batchWaitTimeSave; };

    // get time steps samples will have to wait in batch cycle before sending out for testing
    inline int getBatchWaitTimeSave() { return _batchWaitTimeSave; };

    // set time steps waiting
    inline void setWait(int wait) { _wait = wait;};

    // get time steps waiting
    inline int getWait() { return _wait; };

    // increment time steps waiting
    inline void incrementWait() { _wait++; };

    // set time steps waiting
    inline void setWaitForResource(int waitForResource) { _waitForResource = waitForResource;};

    // get time steps waiting
    inline int getWaitForResource() { return _waitForResource; };

    // increment time steps waiting
    inline void incrementWaitForResource() { _waitForResource++; };

    // set priority
    inline void setPriority(int priority) { _priority = priority; };

    // get priority of test, lower is higher priority
    inline int getPriority() { return _priority; };

    // get distance to testing facility from patient facility
    inline float getTestingDistance() { return _testingFacilityDistance; };

    // get name of disease for this sample to be tested
    inline string getDisease() { return _disease; };

    // get name of test used on this sample
    inline string getTest() { return _test; };

    // set name of test used on this sample
    inline void setTest(string test) { _test = test; };

    // get pointer for test used on this sample
    inline Test* getTestPtr() { return _testPtr; };

    // get pointer for test used on this sample
    inline void setTestPtr(Test* testPtr) { _testPtr = testPtr; };


};

#endif
