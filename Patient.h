//
//  Patient.h
//  mdrtb
//
//  Created by Lad Dombrowski on 1/10/2020.
//

#ifndef PATIENT_H
#define PATIENT_H

#include <string>
#include <vector>

#include "Sample.h"
//#include "Facility.h"

// class Household;

using namespace  std;

// forward declare Sample because Patient is also used in the Sample class
class Sample;

class Patient {
private:

    string _disease;
    string _facility;
    //Facility* _facPtr;
    int _facilityID;
    int _facilityTier;
    string _district;
    int _districtIndex;
    string _region;
    int _regionIndex;

    // this value can be kilometers from facility or time in minutes from facility
    float _distance;

    // which population bin for the presenting facility the patient resides in
    int _popBinIndex;

    bool _infected;
    bool _fromTransmission;      // note if this patient created due to transmission from original patient
    bool _drugResistant;
    bool _trackForRegistry;      // track Patient statistics for Registry

    // PatAwayUnTest 			 patient away from facility - hasn't returned for results
    // PatInUnTest				 patient in-house - no sample results back yet
    // PatInfTestTreat			 patient infected, sample results returned, undergoing treatment
    // PatInfTestRelapse		 patient infected, sample results returned, treated, relapse
    // PatInfTestNoTreat		 Patient infected, sample results returned, no treatment
    // PatInfNoTestNoTreat		 patient infected, no sample results returned, no treatement
    // PatInfTestLtofNoTreat     patient infected, didn't return for results (loss to followup), no treatment
    // PatDead					 patient dead
    // PatCured				     patient cured
    // PatUninfTest			     patient uninfected - sample results returned to patient
    // PatUninfNoTest			 patient uninfected - didn't return for sample results
    // PatUninfTestTreat 		 patient uninfected - sample results returned uninfected, but was treated anyway
    int _status;
    int _prevStatus;
    
    bool _preTestMDCD;          // patient has clinical diagnosis in pre-test - true = yes
    int _preTestMDCDTime;       // patient had clinical diagnosis in pre-test - time
    bool _preTestLtoF;          // patient ever have loss to follow up in pre-test phase - true = yes
    int _preTestLtoFCount;      // patient ever have loss to follow up in pre-test phase - no of times
    bool _relapse;              // patient ever relaspse - true = yes
    bool _represent;            // patient ever represent - true = yes
    
    string _noTreatReason;      // reason patient didn't receive treatment

    string _deathReason;        // reason patient died

    // keep track of false negatives from test and doctor's diagnosis
    bool _testPos;
    bool _mdPos;

    bool _live;                 // true for alive, false for dead

    bool _active;               // true for active, false for inactive

    bool _treated;              // true if patient treated, false for untreated

    bool _tracker;              // true - track patient, false - don't track patient (output data by cout)

    int _age;                   // age in years
    int _gender;
    int _lifeExpectancy;
    int _timeOfDeath;           // time patient died
    int _timeFromOnset;         // time from onset of disease - timesteps
    int _timeFromOnsetTilTreat; // time from onset of disease until patient receives treatment- timesteps
    int _daysInTreatment;       // how long in treatment after test results came back
    int _daysNotInTreatment;    // how long not in treatment after test results came back
    int _noOfClinicalVisits;    // how many clinical visits

    int _timestepPatientCreated;            // at what timestep was the patient created
    int _timestepPatientNoLongerTracked;    // at what timestep did we put patient into inactive container
    int _createTime;                        // time step sample was created
    int _origPresentTime;                   // time step patient originally presented - could be different from _presentTime if patient represents
    int _presentTime;                       // time step patient presented - could be different from _timestepPatientCreated if patient represents 
    int _resultsReturned;                   // time step test result was presented to patient
    int _relapseStartTime;                  // time step to start relapse treatment
    int _representTime;                     // time step patient represents
    int _infectiousnesEndsTime;             // time infected patient is no longer infectious - death, cured
    int _facilityReturnTime;                // how many time step to wait to return to facility for results
    int _noReturnsForResults;               // number of times returned to facility for results
    int _timestepPatientStartedTreatment;   // time step patient started treatment
    int _timestepPatientReceivedResults;    // time step patient received results
    int _timestepPatientReceivedMDResults;  // time step patient received doctor analysis - no results
    int _timestepPatientLtoF;               // time step patient registered as loss to followup

    float _infectiousness;
    float _immunity;

    string _sampleTest;                     // test used for sample
    string _sampleTestStatus;               // Status of test used for sample
    string _sampleTestFacility;             // Facility where sample was tested
    int _sampleTestFacilityEnum;            // Facility enum where sample was tested
    string _sampleTestFacilityDistrict;     // District where sample was tested
    int _sampleTestFacilityDistrictIndex;   // District index into region/district array
    string _sampleTestFacilityRegion;       // Region where sample was tested
    int _sampleTestFacilityRegionIndex;     // Region index into region/district array

    Sample* _sample;             


public:
    Patient();
    Patient(string facName, string disease, float dist, bool infected);
    Patient(string facName, string district, int distIndex, string region, int regIndex, string disease, float dist, int popBinIndex, bool infected, int status, string testName, int timestep);
    Patient(string facName, string district, int distIndex, string region, int regIndex, string disease, float dist, int popBinIndex, bool infected, int status, string testName, int timestep, bool debug);
    Patient(string facName, string district, int distIndex, string region, int regIndex, string disease, float dist, int popBinIndex, bool infected, int status, string testName, int age, int gender, int timestep);
    ~Patient();


    
    //get facility name where patient presented
    inline string getFacility() { return _facility; };
    
    //set facility ID where patient presented
    inline void setFacilityID(int facilityID) { _facilityID = facilityID; };
    
    //get facility enum where patient presented
    inline int getFacilityID() { return _facilityID; };

    //set facility ID where patient presented
    inline void setFacilityTier(int facilityTier) { _facilityTier = facilityTier; };

    //get facility enum where patient presented
    inline int getFacilityTier() { return _facilityTier; };

    //set facility district where patient presented
    inline void setFacilityDistrict(string district) { _district = district; };
    
    //get facility district where patient presented
    inline string getFacilityDistrict() { return _district; };
            
    //set facility district index where patient presented
    inline void setFacilityDistrictIndex(int districtIndex) { _districtIndex = districtIndex; };
    
    //get facility district index where patient presented
    inline int getFacilityDistrictIndex() { return _districtIndex; };
         
    //set facility region where patient presented
    inline void setFacilityRegion(string region) { _region = region; };
    
    //get facility region where patient presented
    inline string getFacilityRegion() { return _region; };
             
    //set facility region index where patient presented
    inline void setFacilityRegionIndex(int regionIndex) { _regionIndex = regionIndex; };
    
    //get facility region index where patient presented
    inline int getFacilityRegionIndex() { return _regionIndex; };

    //get distance 
    inline float getDistance() { return _distance; };

    //set distance from facility where patient resides
    inline void setDistance(float distance) { _distance = distance; };

    //get popBin index 
    inline int getPopBinIndex() { return _popBinIndex; };

    //set popBin index 
    inline void setPopBinIndex(int popBinIndex) { _popBinIndex = popBinIndex; };

    //set whether patient is infected or not
    inline void setInfected(bool flag) { _infected = flag; };

    //get whether patient is infected or not
    inline bool getInfected() { return _infected; };

    //set whether patient is infected or not
    inline void setFromTransmission() { _fromTransmission = true; };

    //get whether patient is infected or not
    inline bool getFromTransmission() { return _fromTransmission; };

    //get whether patient is infected or not
    inline bool isDrugResistant() { return _drugResistant; };

    //get whether patient is infected or not
    inline void setDrugResistant(bool flag) { _drugResistant = flag; };

    //get whether patient is infected or not
    inline bool trackForRegistry() { return _trackForRegistry; };

    //get whether patient is infected or not
    inline void setTrackForRegistry(bool flag) { _trackForRegistry = flag; };

    //is patient alive
    inline bool isAlive() { return _live; };

    //set patient dead
    inline void setDead() { _live = false; };

    //set patient alive status
    inline void setLive(bool live) { _live = live; };
    
    //get patient active status
    inline bool getActive() { return _active; };

    //set patient active status to inactive
    inline void setInactive() { _active = false; };

    //set patient active status
    inline void setActive(bool active) { _active = active; };

    //is patient treated
    inline bool getTreated() { return _treated; };

    //set patient treated
    inline void setTreated() { _treated = true; };

    //is patient being tracked
    inline bool getTracked() { return _tracker; };

    //set patient tracked status
    inline void setTracked() { _tracker = true; };

    //unset patient tracked status
    inline void unsetTracked() { _tracker = false; };

    //set status
    inline void setStatus(int status) {  _prevStatus = _status; _status = status; };

    //get status 
    inline int getStatus() { return _status; };
    
    //set  previous status
    inline void setPrevStatus(int prevStatus) { _prevStatus = prevStatus; };

    //get previous status 
    inline int getPrevStatus() { return _prevStatus; };

    //set pre-test MD Clinical diagnosis occur flag
    inline void setPreTestMDCDFlag(bool status) { _preTestMDCD = status; };

    //get pre-test MD Clinical diagnosis occur flag
    inline bool getPreTestMDCDFlag() { return _preTestMDCD; };

    //set pre-test MD Clinical diagnosis time
    inline void setPreTestMDCDTime(int preTestMDCDTime) { _preTestMDCDTime = preTestMDCDTime; };

    //get pre-test MD Clinical diagnosis time
    inline int getPreTestMDCDTime() { return _preTestMDCDTime; };

    //set pre-diagnostic ltof flag
    inline void setPreTestLtoFFlag(bool status) {  _preTestLtoF = status; _preTestLtoFCount++; };

    //get pre-diagnostic ltof flag
    inline bool getPreTestLtoFFlag() { return _preTestLtoF; };
    
    //get pre-diagnostic ltof flag
    inline int getPreTestLtoFCount() { return _preTestLtoFCount; };

    //set relapse status flag
    inline void setRelapseFlag(bool status) {  _relapse = status;};

    //get relapse status flag
    inline bool getRelapseFlag() { return _relapse; };
    
    //set re-presentation status flag
    inline void setRepresentFlag(bool status) {  _represent = status;};

    //get re-presentation status flag
    inline bool getRepresentFlag() { return _represent; };
    
    //set reason infected patient didn't receive treatement
    inline void setReasonNoTreat(string noTreatReason) { _noTreatReason = noTreatReason; };

    //get reason infected patient didn't receive treatement 
    inline string getReasonNoTreat() { return _noTreatReason; };

    //set reason patient died
    inline void setReasonDied(string deathReason) { _deathReason = deathReason; };

    //get reason patient died 
    inline string getReasonDied() { return _deathReason; };

    //set test result status
    inline void setTestPos(bool testPos) { _testPos = testPos; };

    //get test result status 
    inline bool getTestPos() { return _testPos; };

    //set docotor's diagnosis status
    inline void setMDPos(bool mdPos) { _mdPos = mdPos; };

    //get doctor's diagnosis status 
    inline bool getMDPos() { return _mdPos; };

    //get age
    inline int getAge() { return _age; };

    //set age
    inline void setAge(int age) { _age = age; };

    //get gender
    inline int getGender() { return _gender; };

    //set gender
    inline void setGender(int gender) { _gender = gender; };

    //get disease associated with Patient
    inline string getDisease() { return _disease; };

    //get patient life expectancy
    inline int getLifeExpectancy() { return _lifeExpectancy; };

    //set patient life expectancy
    inline void setLifeExpectancy(int lifeExpectancy) { _lifeExpectancy = lifeExpectancy; };

    //increment time from onset
    inline void incrementOnset() { _timeFromOnset++; };

    //increment time from onset
    inline void incrementOnset(int increment) { _timeFromOnset = _timeFromOnset + increment; };

    //increment time from onset
    inline void setOnsetTilTreatment(int onsetTime) { _timeFromOnsetTilTreat = onsetTime; };
    
    //increment time from onset
    inline int getOnsetTilTreatment() { return _timeFromOnsetTilTreat; };
    
    //get time from getting results
    inline int getTimeStepsInTreatment() { return _daysInTreatment; };

    //set time from getting results
    inline void setTimeStepsInTreatment(int daysInTreatment) { _daysInTreatment = daysInTreatment; };

    // get time from getting results
    inline int getTimeStepsNotInTreatment() { return _daysNotInTreatment; };

    // set time from getting results
    inline void setTimeStepsNotInTreatment(int daysNotInTreatment) { _daysNotInTreatment = daysNotInTreatment; };

    // get time of death
    inline int getTimeOfDeath() { return _timeOfDeath; };

    // set time of death
    inline void setTimeOfDeath(int timeOfDeath) { _timeOfDeath = timeOfDeath; };

    // get time from getting results
    inline int getTimeFromOnset() { return _timeFromOnset; };

    // set time from getting results
    inline void setTimeFromOnset(int timeFromOnset) { _timeFromOnset = timeFromOnset; };

    // get time from getting results
    inline int getNoVisits() { return _noOfClinicalVisits; };

    // set time from getting results
    inline void setNoVisits(int noClinicalVisits) { _noOfClinicalVisits = noClinicalVisits; };

    // get timestep patient was created
    inline int getTimestepCreated() { return _timestepPatientCreated; };

    // get timestep patient was created
    inline void setTimestepCreated(int timestep) { _timestepPatientCreated = timestep; _presentTime = timestep; _origPresentTime = timestep; };

    // get timestep patient was created
    inline int getTimestepPresented() { return _presentTime; };

    // get timestep patient was created
    inline int getTimestepOrigPresented() { return _origPresentTime; };

    // get timestep patient was created
    inline void setTimestepPresented(int timestep) { _origPresentTime = _presentTime; _presentTime = timestep; };

    // get timestep patient was created
    inline int getTimestepInactive() { return _timestepPatientNoLongerTracked; };

    // get timestep patient was created
    inline void setTimestepInactive(int timestep) { _timestepPatientNoLongerTracked = timestep; };

    //increment time from getting results
    inline void incrementTimeStepsInTreatment() { _daysInTreatment++;   _timeFromOnset++; };
    
    //increment time from getting results
    inline void incrementTimeStepsInTreatment(int increment) { _daysInTreatment = _daysInTreatment + increment;   _timeFromOnset = _timeFromOnset + increment; };

    //increment time from getting results
    inline void resetTimeStepsInTreatment() { _daysInTreatment = 0; };

    //increment time from getting results
    inline void incrementTimeStepsNotInTreatment() { _daysNotInTreatment++;  _timeFromOnset++; };
    
    //increment time from getting results
    inline void incrementTimeStepsNotInTreatment(int increment) { _daysNotInTreatment = _daysNotInTreatment + increment;  _timeFromOnset = _timeFromOnset + increment; };

    // set time step sample was created
    inline void setCreateTime(int createTime) { _createTime = createTime; _timestepPatientCreated = createTime; _presentTime = createTime; };

    // get time step sample was created
    inline int getCreateTime() { return _createTime; };
    
    // set time step relapse treatment should start
    inline void setRelapseTime(int relapseStartTime) { _relapseStartTime = relapseStartTime; };

    // get time step relapse treatment should start
    inline int getRelapseTime() { return _relapseStartTime; };
      
    // set time step patient represents
    inline void setRepresentTime(int representTime) { _representTime = representTime; };

    // get time step patient represents
    inline int getRepresentTime() { return _representTime; };

    //decrease no. of timesteps before representing
    inline void decrementRepresentTime() { _representTime--; };
    
    // set time step test results were returned
    inline void setResultsReturnedTime(int resultsReturned) { _resultsReturned = resultsReturned; };

    // get time step test results were returned
    inline int getResultsReturnedTime() { return _resultsReturned; };

    // set time step patient started treatment
    inline void setStartedTreatmentTime(int timestepPatientStartedTreatment) { _timestepPatientStartedTreatment = timestepPatientStartedTreatment; };

    // get time step test results were returned
    inline int getStartedTreatmentTime() { return _timestepPatientStartedTreatment; };

    // set time step test results were returned to patient
    inline void setPatientRecievedResultTime(int timestepPatientReceivedResults) { _timestepPatientReceivedResults = timestepPatientReceivedResults; };

    // get time step test results were returned to patient
    inline int getPatientRecievedResultTime() { return _timestepPatientReceivedResults; };

    // set time step test results were returned
    inline void setPatientRecievedMDResultTime(int timestepPatientReceivedMDResults) { _timestepPatientReceivedMDResults = timestepPatientReceivedMDResults; };

    // get time step test results were returned
    inline int getPatientRecievedMDResultTime() { return _timestepPatientReceivedMDResults; };

    // set time step infectiousnes for patient ends
    inline void setInfectEndTime(int infectiousnesEndsTime) { _infectiousnesEndsTime = infectiousnesEndsTime; };
        
    //increment time for infectiousnes for patient ends
    inline void incrementInfectEndTime(int base, int increment) { _infectiousnesEndsTime = base + increment; };

    // get time step rinfectiousnes for patient ends
    inline int getInfectEndTime() { return _infectiousnesEndsTime; };

    // set time step test results were returned
    inline void seTimePatientLtoF(int timestepPatientLtoF) { _timestepPatientLtoF = timestepPatientLtoF; };

    // get time step test results were returned
    inline int getTimePatientLtoF() { return _timestepPatientLtoF; };  

    // set no. of time steps to wait to return to facility to get results
    inline void setFacilityReturnTime(int facilityReturnTime) { _facilityReturnTime = facilityReturnTime; };

    // get no. of time steps to wait to return to facility to get results
    inline int getFacilityReturnTime() { return _facilityReturnTime; };

    //decrease no. of timesteps before returning for sample results
    inline void decrementFacilityReturnTime() { _facilityReturnTime--; };

    // get no of times returned for sample results
    inline int getNoReturnsForResults() { return _noReturnsForResults; };

    // set no of times returned for sample results
    inline void setNoReturnsForResults(int noReturnsForResults) { _noReturnsForResults = noReturnsForResults; };

    //increment time of times returned for results
    inline void incrementNoReturnsForResults() { _noReturnsForResults++; };

    //increment time from getting results
    inline void incrementNoVisits() { _noOfClinicalVisits++; };

    //increment all time time
    inline void incrementAllTime() { _daysNotInTreatment++;  _daysInTreatment = 0;  _timeFromOnset++; };

    // get infectiousness
    inline float getinfectiousness() { return _infectiousness; };

    // set infectiousness
    inline void setinfectiousness(float infectiousness) { _infectiousness = infectiousness; };

    // get immunity
    inline float getImmunity() { return _immunity; };

    // set immunity
    inline void setImmunity(float immunity) { _immunity = immunity; };

    //get test name used on sample
    inline string getSampleTest() { return _sampleTest; };

    //get test name used on sample
    inline void setSampleTest(string sampleTest) { _sampleTest = sampleTest; };

     //get test name used on sample
    inline string getSampleTestStatus() { return _sampleTestStatus; };

    //get test name used on sample
    inline void setSampleTestStatus(string sampleTestStatus) { _sampleTestStatus = sampleTestStatus; };

     //get test facility name used on sample
    inline string getSampleTestFacility() { return _sampleTestFacility; };

    //set test facility name used on sample
    inline void setSampleTestFacility(string sampleTestFacility) { _sampleTestFacility = sampleTestFacility; };

    //get test facility enum used on sample
    inline int getSampleTestFacilityEnum() { return _sampleTestFacilityEnum; };

    //set test faciltiy enum used on sample
    inline void setSampleTestFacilityEnum(int sampleTestFacilityEnum) { _sampleTestFacilityEnum = sampleTestFacilityEnum; };
  
    //get test facility district used on sample
    inline string getSampleTestFacilityDistrict() { return _sampleTestFacilityDistrict; };

    //set test faciltiy district used on sample
    inline void setSampleTestFacilityDistrict(string sampleTestFacilityDistrict) { _sampleTestFacilityDistrict = sampleTestFacilityDistrict; };
    
    //get test facility district index used on sample
    inline int getSampleTestFacilityDistrictIndex() { return _sampleTestFacilityDistrictIndex; };

    //set test faciltiy district index used on sample
    inline void setSampleTestFacilityDistrictIndex(int sampleTestFacilityDistrictIndex) { _sampleTestFacilityDistrictIndex = sampleTestFacilityDistrictIndex; };

    //get test facility region used on sample
    inline string getSampleTestFacilityRegion() { return _sampleTestFacilityRegion; };

    //set test faciltiy region used on sample
    inline void setSampleTestFacilityRegion(string sampleTestFacilityRegion) { _sampleTestFacilityRegion = sampleTestFacilityRegion; };
    
    //get test facility region index used on sample
    inline int getSampleTestFacilityRegionIndex() { return _sampleTestFacilityRegionIndex; };

    //set test faciltiy region index used on sample
    inline void setSampleTestFacilityRegionIndex(int sampleTestFacilityRegionIndex) { _sampleTestFacilityRegionIndex = sampleTestFacilityRegionIndex; };


    // get Sample pointer
    inline Sample* getSamplePtr() { return _sample; };

    // set Sample pointer
    inline void setSamplePtr(Sample* sample) { _sample = sample; };

};

#endif // PATIENT_H

