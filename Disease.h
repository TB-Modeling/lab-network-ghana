//
//  Disease.h
//  
//
//  Created by L. Dombrowski on 1/17/2020.
//

#ifndef DISEASE_H
#define DISEASE_H

#include <string>
#include "Stats.h"

using namespace std;

class Stats;

class Disease {
private:

    string _name;

    float _mortality;

    float _morbidity;

    // incidence rate per person per day for year of run
    float _incidence;

    // incidence adjustment type
    string _incAdjType;

    // incidence adjustment factor per year
    float _incAdjFactor;

    // proportion of infected patients that present
    float _prpIncidencePresent;

    // ratio of uninfected patients to infected patients that present
    float _patientsRatioUninfInf;

    // percent of patients presenting that are infected - this is a fudge factor that lets adjust incidence through priorRangesAndDistributions
    float _perPatientPresentInf;

    // transFlag = true if running transmission, false if not running disease transmission 
    bool _transFlag;
    float _transmission;
    float _transThreshold;

    int _priority;

    // probability that MD will perform clinical diagnosis before send-out for infected patients
    float _perPreTestMDCDInf;

    // probability that MD will perform clinical diagnosis before send-out for uninfected patients
    float _perPreTestMDCDUnInf;
    
    // percent of samples lost in transit for this disease
    float _perPreTestLtoF;

    // percent of samples lost in transit for this disease by tier
    float _perPreTestLtoFTier[ 10 ] = { 0 };
    
    // percentage of loss to follow up before testing by tier
    initTreatLtoFRegionTier _perPreTestLtoFTierRegion[20];

    // amout _perPreTestLtoF is reduced at year 2020 to reflect better specimen transport policies
    float _perPreTestLtoFReduction;
    
    // space holder
    float _space1;
    
    // place holder
    float _space2;

    // space holder
    float _space3;
    
    // place holder
    float _space4;
    
    // percent of samples lost in transit for this disease
    float _perLostSamples;
    
    // percent of diagnosed patients that get treated &&&&&&&
    float _treatedPatientsPercentage;
    
    // Probablility that a patient will return for test results
    float _probPatientReturns;

    // length of intital treatment for disease in timesteps
    int _treatmentTime;

    // length of relapse treatment for disease in timesteps
    int _relapseTime;

    // percentage of initial treatment success
    float _initTreatSuccess;

    // percentage of intial treatment death
    float _initTreatDeath;
    float _initTreatDeathIP;

    // percentage of loss to follow up before treatment
    float _initTreatLtoF;

    // percentage of loss to follow up before treatment by tier
    float _initTreatLtoFTier[10] = { 0 };

    // percentage of loss to follow up before treatment by tier
    initTreatLtoFRegionTier _preTreatLtoFTierRegion[20];

    // percentage of loss to follow up during treatment
    float _treatLtoF;
    float _treatLtoFTier[10] = { 0 };
    float _treatLtoFIP;

    // percentage of loss to follow up before treatment by tier
    initTreatLtoFRegionTier _treatLtoFTierRegion[20];

    // percentage of relapse treatment success
    float _relapseSuccess;

    // probability that non-infected patient was treated anyway
    float _probNonInfTreated;

    // percentage of disease that is drug resistant
    float _perDrugResistant;

    // time from onset to presentation at facility for testing in time steps
    int _onsetToPresentation;

    // percent untreated die
    float _perUntreatedDie;

    // number of time steps until an untreated patient lives before death
    int _timeUntreatedUntilDeath;

    // percent untested patients return for pesentation again
    float _perUntestedRepresent;

    // time untested patients return for pesentation again
    int _timeUntestedRepresent;

    // percent untreated patients return for pesentation again
    float _perUntreatedRepresent;

    // time untreated patients return for pesentation again
    int _timeUntreatedRepresent;
    
    // percent patients LtoF during treatment return for pesentation again
    float _perTreatedRepresent;

    // percent patients LtoF during treatment that die before repesentation
    float _perLtoFRepresentDeath;

    // time patients LtoF during treatment return for pesentation again
    int _timeTreatedRepresent;

    // maximum time of death during retreatment
    int _timeDeathRetreat;

    // maximum time until representation after loss to follow up
    int _timeLtoFRepresent;

    // maximum time for untreated patients to be cured
    int _timeUntreatCure;
    
    // untreated mortality array - initialize to zero
    float _untreatMortality[125] = { 0 };

    // additional no. of patients to add when creating patients, currently only changed by adding transmission patients
    int _additionalUnInfPatients = 0;
    int _additionalInfPatients = 0;

    // keep stats for each Disease - for regions, districts
    Stats* _stats;

    // keep stats for each Disease - for facilities
    Stats* _facilityStats;

public:



    Disease();
    Disease(string name, float mort, float incid, float trans, int prior);
    Disease(string name, float mort, float incid, float trans, int prior, int tPer, int tTime, int rTime, float initTreatSuccess, float initTreatDeath, float rSuccess);
    Disease(string name, float mort, float incid, float trans, int prior, int tPer, int tTime, int rTime, float initTreatSuccess, float initTreatDeath, float initTreatLtoF, float rSuccess,  float probNITreated, float perDrugRest);
    Disease(string name, float mort, float incid, float trans, int prior, int tPer, int tTime, int rTime, float initTreatSuccess, float initTreatDeath, float initTreatLtoF, float rSuccess,  float probNITreated, float perDrugRest, int onsetTime, float unDiePer, int unDieTime, float unRepreTestPer, int unRepreTestTime, float unRepreTreatPer, int unRepreTreatTime);
    Disease(string name, float mort, float incid, string incAdjType, float incAdjFactor, float trans, int prior, float morbidity, float preTestPer, float preTestDeathPer, float space1, float space2, float probRet, float perLostSamples, float tper, int tTime, float initTreatSuccess, float initTreatDeath, float preTreatLtoF, float treatLtoF, int rTime, float rSuccess, float probNITreated, float perDrugRest, int onsetTime, float unDiePer, int unDieTime, float unRepreTestPer, int unRepreTestTime);
    Disease(string name, float mort, float incid, string incAdjType, float incAdjFactor, float trans, int prior, float morbidity, float preTestPer, float preTestDeathPer, float space1, float space2, float probRet, float perLostSamples, float tper, int tTime, float initTreatSuccess, float initTreatDeath, float preTreatLtoF, float treatLtoF, int rTime, float rSuccess, float probNITreated, float perDrugRest, int onsetTime, float unDiePer, int unDieTime, float unRepreTestPer, int maxTimeDeathRetreat, int maxTimeLtoFRepresent, int maxTimeUntreatCure);
    Disease(string name, float mort, float incid, string incAdjType, float incAdjFactor, float trans, int prior, float morbidity, float preTestPer, float preTestDeathPer, float space1, float space2, float probRet, float perLostSamples, float tper, int tTime, float initTreatSuccess, float initTreatDeath, float initTreatDeathIP, float preTreatLtoF, float treatLtoF, float treatLtoFIP, int rTime, float rSuccess, float probNITreated, float perDrugRest, int onsetTime, float unDiePer, int unDieTime, float unRepreTestPer, int maxTimeDeathRetreat, int maxTimeLtoFRepresent, int maxTimeUntreatCure);
    Disease(string name, float mort, float incid, string incAdjType, float incAdjFactor, float prpIncPresent, float patRatioUninfInf, float trans, int prior, float morbidity, float preTestPer, float preTestDeathPer, float space1, float space2, float probRet, float perLostSamples, float tper, int tTime, float initTreatSuccess, float initTreatDeath, float initTreatDeathIP, float preTreatLtoF, float treatLtoF, float treatLtoFIP, int rTime, float rSuccess, float probNITreated, float perDrugRest, int onsetTime, float unDiePer, int unDieTime, float unRepreTestPer, int maxTimeDeathRetreat, int maxTimeLtoFRepresent, int maxTimeUntreatCure);
    Disease(string name, float mort, float incid, string incAdjType, float incAdjFactor, float prpIncPresent, float patRatioUninfInf, float trans, int prior, float morbidity, float preTestMDCDInfPer, float preTestMDCDUnInfPer, float preTestPer, float preTestDeathPer, float space1, float space2, float probRet, float perLostSamples, float tper, int tTime, float initTreatSuccess, float initTreatDeath, float initTreatDeathIP, float preTreatLtoF, float treatLtoF, float treatLtoFIP, int rTime, float rSuccess, float probNITreated, float perDrugRest, int onsetTime, float unDiePer, int unDieTime, float unRepreTestPer, int maxTimeDeathRetreat, int maxTimeLtoFRepresent, int maxTimeUntreatCure);
    Disease(string name, float mort, float incid, string incAdjType, float incAdjFactor, float prpIncPresent, float patRatioUninfInf, float perPatientPresentInf, float trans, int prior, float morbidity, float preTestMDCDInfPer, float preTestMDCDUnInfPer, float preTestPer, float preTestDeathPer, float space1, float space2, float probRet, float perLostSamples, float tper, int tTime, float initTreatSuccess, float initTreatDeath, float initTreatDeathIP, float preTreatLtoF, float treatLtoF, float treatLtoFIP, int rTime, float rSuccess, float probNITreated, float perDrugRest, int onsetTime, float unDiePer, int unDieTime, float unRepreTestPer, int maxTimeDeathRetreat, int maxTimeLtoFRepresent, int maxTimeUntreatCure);
    Disease(string name, float mort, float incid, string incAdjType, float incAdjFactor, float prpIncPresent, float patRatioUninfInf, float perPatientPresentInf, float trans, int prior, float morbidity, float preTestMDCDInfPer, float preTestMDCDUnInfPer, float preTestPer, float preTestPerTier[10], float preTestDeathPer, float space1, float space2, float probRet, float perLostSamples, float tper, int tTime, float initTreatSuccess, float initTreatDeath, float initTreatDeathIP, float preTreatLtoF, float preTreatLtoFTier[10], float treatLtoF, float treatLtoFTier[10], float treatLtoFIP, int rTime, float rSuccess, float probNITreated, float perDrugRest, int onsetTime, float unDiePer, int unDieTime, float unRepreTestPer, int maxTimeDeathRetreat, int maxTimeLtoFRepresent, int maxTimeUntreatCure);
    ~Disease();


    // get disease mortality
    inline string getName() { return _name; };

    // get disease mortality
    inline float getMortality() { return _mortality; };

    // set disease mortality
    inline void setMortality(float mortality) {_mortality = mortality; };
    
    // get disease morbidity
    inline float getMorbidity() { return _morbidity; };

    // set disease morbidity
    inline void setMorbidity(float morbidity) {_morbidity = morbidity; };

    // get disease incidence
    float getIncidence();

    // get disease incidence from year number
    float getIncidence(int year);

    // set disease incidence
    inline void setIncidence(float incidence) { _incidence = incidence; };

    // get incidence adjustment type
    inline string getIncAdjType() { return _incAdjType; };

    // set incidence adjustment type
    inline void setIncAdjType(string incAdjType) { _incAdjType = incAdjType; };

    // get incidence adjustment factor
    inline float getIncAdjFactor() { return _incAdjFactor; };

    // set incidence adjustment factor
    inline void setIncAdjFactor(float incAdjFactor) { _incAdjFactor = incAdjFactor; };

    // get proportion of infected patients presenting for care
    inline float getPrpIncPresenting() { return _prpIncidencePresent; };

    // set proportion of infected patients presenting for care
    inline void setPrpIncPresenting(float prpIncidencePresent) { _prpIncidencePresent = prpIncidencePresent; };

    // get ratio of non-infected patients to infected patients that present
    inline float getPatientsRatioUninfInf() { return _patientsRatioUninfInf; };

    // set ratio of non-infected patients to infected patients that present
    inline void setPatientsRatioUninfInf(float patientsRatioUninfInf) { _patientsRatioUninfInf = patientsRatioUninfInf; };

    // get percentage of patients that are presenting that are infected
    inline float getPerPatientPresentInf() { return _perPatientPresentInf; };

    // set percentage of patients that are presenting that are infected
    inline void setPerPatientPresentInf(float perPatientPresentInf) { _perPatientPresentInf = perPatientPresentInf; };

    // get disease transmission flag
    inline bool getTransFlag() { return _transFlag; };

    // get disease transmission
    inline void turnTransFlagOn() { _transFlag = true;; };

    // get disease transmission
    inline void turnTransFlagOff() { _transFlag = false;; };

    // get disease transmission
    inline void setTransFlag(bool transFlag) { _transFlag = transFlag; };

    // get disease transmission
    inline float getTransmission() { return _transmission; };

    // set disease transmission
    inline void setTransmission(float transmission) { _transmission = transmission; };

    // get disease transmission threshold to create a new patient
    inline float getTransThreshold() { return _transThreshold; };

    // set disease transmission
    inline void setTransThreshold(float transThreshold) { _transThreshold = transThreshold; };

    // get disease priority
    inline int getPriority() { return _priority; };

    // set disease priority
    inline void setPriority(int priority) { _priority = priority; };
        
    // get percent of pre-Diagnostic MD clinical diagnosis for sample send out
    inline float getPerPreTestMDCDInf() { return _perPreTestMDCDInf; };

    // set percent of pre-Diagnositic MD clinical diagnosis for sample send out
    inline void setPerPreTestMDCDInf(float perPreTestMDCDInf) { _perPreTestMDCDInf = perPreTestMDCDInf; };

    // get percent of pre-Diagnostic MD clinical diagnosis for sample send out
    inline float getPerPreTestMDCDUnInf() { return _perPreTestMDCDUnInf; };

    // set percent of pre-Diagnositic MD clinical diagnosis for sample send out
    inline void setPerPreTestMDCDUnInf(float perPreTestMDCDUnInf) { _perPreTestMDCDUnInf = perPreTestMDCDUnInf; };

    // get percent of pre-Diagnostic loss to followup
    inline float getPerPreTestLtoF() { return _perPreTestLtoF; };

    // get percent of pre-Diagnostic loss to followup
    inline float getPerPreTestLtoFTier(int tier) { return _perPreTestLtoFTier[tier-1]; };

    // set percent of pre-Diagnositic loss to followup
    inline void setPerPreTestLtoF(float perPreTestLtoF) {_perPreTestLtoF = perPreTestLtoF; };

    // set percent of pre-Diagnositic loss to followup
    inline void setPerPreTestLtoFTier(int tier, float perPreTestLtoF) { _perPreTestLtoFTier[tier-1] = perPreTestLtoF; };

    // get region name by index
    string getPerPreTestLtoFTierRegionName(int index);
    
    // get percent of pre-Diagnositic loss to follow up by tier
    float getPerPreTestLtoFTierRegion(string region, int tier);

    // set percent of pre-Diagnositic loss to follow up by tier
    void setPerPreTestLtoFTierRegion(string region, int tier, float initTreatLtoF);
    
    // dump pre test loss to follow up by region
    void dumpPerPreTestLtoFTierRegion();

    // set percent of death to pre-diagnostic loss to followup patients
    inline void setPerPreTestLtoFReduction(float perPreTestLtoFReduction) { _perPreTestLtoFReduction = perPreTestLtoFReduction; };
        
    // get percent of death to pre-diagnostic loss to followup patients
    inline float getPerPreTestLtoFReduction() { return _perPreTestLtoFReduction; };

    // get spaceholder value
    inline float getSpace1() { return _space1; };

    // set spaceholder value
    inline void setSpace1(float space1) {_space1 = space1; };
            
    // get spaceholder value
    inline float getSpace2() { return _space2; };

    // set spaceholder value
    inline void setSpace2(float space2) {_space2 = space2; };

    // get percent of samples lost in transit
    inline float getPerLostSamples() { return _perLostSamples; };

    // set percent of samples lost in transit
    inline void setPerLostSamples(float perLostSamples) {_perLostSamples = perLostSamples; };

    // get percent of diagnosed patients that get treatment
    inline int getTreatedPercentage() { return _treatedPatientsPercentage; };

    // set percent of diagnosed patients that get treatment
    inline void setTreatedPercentage(int treatedPatientsPercentage) {_treatedPatientsPercentage = treatedPatientsPercentage; };

    // get probability that a patient will return for test results
    inline int getProbReturn() { return _probPatientReturns; };

    // set probability that a patient will return for test results
    inline void setTProbReturn(int probPatientReturns) {_probPatientReturns = probPatientReturns; };

    // get treatment time
    inline int getTreatmentTime() { return _treatmentTime; };

    // get relapse treatment time
    inline int getRelapseTime() { return _relapseTime; };

    // get initial treatment success rate
    inline float getInitialTreatmentSuccessRate() { return _initTreatSuccess; };

    // set initial treatment success rate
    inline void setInitialTreatmentSuccessRate(float initTreatSuccess) {_initTreatSuccess = initTreatSuccess; };

    // get initial treatment death rate
    inline float getInitialTreatmentDeathRate() { return _initTreatDeath; };

    // set initial treatment death rate
    inline void setInitialTreatmentDeathRate(float initTreatDeath) { _initTreatDeath = initTreatDeath; };

    // get initial treatment death rate input parameter
    inline float getInitialTreatmentDeathRateIP() { return _initTreatDeathIP; };

    // set initial treatment death rate input parameter
    inline void setInitialTreatmentDeathRateIP(float initTreatDeathIP) { _initTreatDeathIP = initTreatDeathIP; };

    // get initial treatment loss to follow up
    inline float getInitialTreatmentLossToFollow() { return _initTreatLtoF; };

    // set initial treatment loss to follow up
    inline void setInitialTreatmentLossToFollow(float initTreatLtoF) { _initTreatLtoF = initTreatLtoF; };

    // get initial treatment loss to follow up by tier
    inline float getPreTreatmentLtoFTier(int tier) { return _initTreatLtoFTier[tier-1]; };

    // set initial treatment loss to follow up by tier
    inline void setPreTreatmentLtoFTier(int tier, float initTreatLtoF) { _initTreatLtoFTier[tier-1] = initTreatLtoF; };

    // get pre-treat loss to follow up region name by index
    string getPreTreatLtoFTierRegionName(int index);
    
    // get initial treatment loss to follow up by tier
    float getPreTreatmentLtoFTierRegion(string region, int tier);

    // set initial treatment loss to follow up by tier
    void setPreTreatmentLtoFTierRegion(string region, int tier, float initTreatLtoF);

    // dump initial treatement loss to follow up by region
    void dumpPreTreatmentLtoFTierRegion();

    // get treatment loss to follow up
    inline float getTreatmentLossToFollow() { return _treatLtoF; };

    // set treatment loss to follow up
    inline void setTreatmentLossToFollow(float treatLtoF) { _treatLtoF = treatLtoF; };

    // get treatment loss to follow up by tier
    inline float getTreatmentLtoFTier(int tier) { return _treatLtoFTier[tier-1]; };

    // set treatment loss to follow up by tier
    inline void setTreatmentLtoFTier(int tier, float treatLtoF) { _treatLtoFTier[tier-1] = treatLtoF; };

    // get percent of treat loss to follow up region name by index
    string getTreatLtoFTierRegionName(int index);

    // get initial treatment loss to follow up by tier
    float getTreatmentLtoFTierRegion(string region, int tier);

    // set initial treatment loss to follow up by tier
    void setTreatmentLtoFTierRegion(string region, int tier, float initTreatLtoF);

    // // dump treatement loss to follow up by region
    void dumpTreatmentLtoFTierRegion();

    // get treatment loss to follow up input parameter
    inline float getTreatmentLossToFollowIP() { return _treatLtoFIP; };

    // set treatment loss to follow up input parameter
    inline void setTreatmentLossToFollowIP(float treatLtoFIP) { _treatLtoFIP = treatLtoFIP; };

    // get relapse treatment success rate
    inline float getRelapseSuccessRate() { return _relapseSuccess; };

    // get no treatment path2 death percentage
    inline int getprobNonInfTreated() { return _probNonInfTreated; };

    // get no treatment path2 death percentage
    inline int getperDrugResistant() { return _perDrugResistant; };

    // get time from onset to presentation at facility for testing in time steps
    inline int getTimeOnsetToPresentation() { return _onsetToPresentation; };

    // get percent untreated die
    inline int getPerUntreatedDie() { return _perUntreatedDie; };

    // set percent untreated die
    inline void setPerUntreatedDie(int percentage) { _perUntreatedDie = percentage; };

    // number of time steps until an untreated patient lives before death
    inline int getUntreatedDeathTime() { return _timeUntreatedUntilDeath; };
    
    // get percent untested patients return for pesentation again
    inline float getPerUntestedRepresentation() { return _perUntestedRepresent; };

     // get time untested patients return for pesentation again
    inline int getTimeUntestedRepresentation() { return _timeUntestedRepresent; };

    // get percent untreated patients return for pesentation again
    inline float getPerUntreatedRepresentation() { return _perUntreatedRepresent; };

     // get time untreated patients return for pesentation again
    inline int getTimeUntreatedRepresentation() { return _timeUntreatedRepresent; };
    
    // get percent patients LtoF during treatment return for pesentation again
    inline float getPerTreatedRepresentation() { return _perTreatedRepresent; };
    
    // get percent patients LtoF during treatment that die before representation
    inline float getPerRepresentationLtoFDeath() { return _perLtoFRepresentDeath; };

    // set percent patients LtoF during treatment that die before representation
    inline void setPerRepresentationLtoFDeath(float perLtoFRepresentDeath) { _perLtoFRepresentDeath = perLtoFRepresentDeath; };

    // get time patients LtoF during treatment return for pesentation again
    inline int getTimeTreatedRepresentation() { return _timeTreatedRepresent; };

    // maximum time of death during retreatment
    inline int getTimeDeathRetreated() { return _timeDeathRetreat; };

    // maximum time of death during retreatment
    inline void setTimeDeathRetreated(float timeDeathRetreat) { _timeDeathRetreat = timeDeathRetreat; };

    // maximum time until representation after loss to follow up
    inline int getTimeLtoFRepresent() { return _timeLtoFRepresent; };

    // maximum time until representation after loss to follow up
    inline void setTimeLtoFRepresent(float timeLtoFRepresent) { _timeLtoFRepresent = timeLtoFRepresent; };

    // maximum time for untreated patients to be cured
    inline int getTimeUntreatedCure() { return _timeUntreatCure; };

    // maximum time for untreated patients to be cured
    inline void setTimeUntreatedCure(float timeUntreatCure) { _timeUntreatCure = timeUntreatCure; };
    
    // set untreated mortality array value
    inline void setUntreatedMortality(int index, float untreatMortality) { _untreatMortality[index] = untreatMortality; };
    
    // get untreated mortality for this time unit
    inline float getUntreatedMortality(int index) { return _untreatMortality[index]; };

    // get untreated mortality for this time unit
    int findUntreatedMortalityIndex(float mortality);
    
    // get the pointer for the region, district stats for this disease
    inline Stats* getStatsPtr() { return _stats; };

    // get the pointer for the facility stats for this disease
    inline Stats* getFacStatsPtr() { return _facilityStats; };

    // get additional uninfected patients to add this time step
    inline int getAddUninfPatients() { return _additionalUnInfPatients; };

    // increment additional uninfected patients
    inline void incAddUninfPatients() { _additionalUnInfPatients++; };

    // set additional uninfected patients to add this time step
    inline void setAddUninfPatients(float additionalUnInfPatients) { _additionalUnInfPatients = additionalUnInfPatients; };

    // get additional infected patients to add this time step
    inline int getAddInfPatients() { return _additionalInfPatients; };

    // increment additional infected patients
    inline void incAddInfPatients() { _additionalInfPatients++; };

    // set additional infected patients to add this time step
    inline void setAddInfPatients(float additionalInfPatients) { _additionalInfPatients = additionalInfPatients; };

};// function to get region names 
string getRegionNames(int regIndex);

#endif                  /* defined(DISEASE_H) */



