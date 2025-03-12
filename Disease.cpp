//
//  Disease.cpp
//  mdrtb
//
//  Created by L. Dombrowski on 1/17/2020.

#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include "Disease.h"
#include "Patient.h"
//#include "Functions.h"
#include "GlobalParameters.h"


using namespace std;
//using namespace GLOBALPARAMS;
//using namespace FUNCTIONS;


Disease::Disease() {
};

Disease::Disease(string name, float mort, float incid, float trans, int prior) {

    // store away information on creation
    _name = name;
    _mortality = mort;
    _incidence = incid;
    _incAdjType = "linear";
    _incAdjFactor = 0;
    _prpIncidencePresent = 1;
    _patientsRatioUninfInf = 1;
    _perPatientPresentInf = 1;
    _transFlag = false;
    _transmission = trans;
    _priority = prior;
    
    _stats = new Stats();

    _facilityStats = new Stats();

}

Disease::Disease(string name, float mort, float incid, float trans, int prior, int tPer, int tTime, int rTime, float initTreatSuccess, float initTreatDeath, float rSuccess) {

    // store away information on creation
    _name = name;
    _mortality = mort;
    _incidence = incid;
    _incAdjType = "linear";
    _incAdjFactor = 0;
    _prpIncidencePresent = 1;
    _patientsRatioUninfInf = 1;
    _perPatientPresentInf = 1;
    _transFlag = false;
    _transmission = trans;
    _transThreshold = .1;
    _priority = prior;
    _treatedPatientsPercentage = tPer;
    _probPatientReturns  = probReturn;
    _treatmentTime = tTime;
    _relapseTime = rTime;
    _initTreatSuccess = initTreatSuccess;
    _initTreatDeath = initTreatDeath;
    _initTreatDeathIP = initTreatDeath;
    _relapseSuccess = rSuccess;
    _onsetToPresentation = 0;
    _perUntreatedDie = 50;
    _timeUntreatedUntilDeath = 0;
    _perUntestedRepresent = 17.8;
    _timeUntestedRepresent = 30;
    _perUntreatedRepresent = 17.8;
    _timeUntreatedRepresent = 30;
    _perTreatedRepresent = 17.8;
    _timeTreatedRepresent = 30;
    _perLtoFRepresentDeath = .49;
    _timeDeathRetreat = 180;
    _timeLtoFRepresent = 180;
    _timeUntreatCure = 3650;

    _stats = new Stats();

    _facilityStats = new Stats();

}


Disease::Disease(string name, float mort, float incid, float trans, int prior, int tPer, int tTime, int rTime, float initTreatSuccess, float initTreatDeath, float initTreatLtoF, float rSuccess, float probNITreat, float perDrugResist) {

    // store away information on creation
    _name = name;
    _mortality = mort;
    _incidence = incid;
    _incAdjType = "linear";
    _incAdjFactor = 0;
    _prpIncidencePresent = 1;
    _patientsRatioUninfInf = 1;
    _perPatientPresentInf = 1;
    _transFlag = false;
    _transmission = trans;
    _transThreshold = .1;
    _priority = prior;
    _treatedPatientsPercentage = tPer;
    _probPatientReturns  = probReturn;
    _treatmentTime = tTime;
    _relapseTime = rTime;
    _initTreatSuccess = initTreatSuccess;
    _initTreatDeath = initTreatDeath;
    _initTreatDeathIP = initTreatDeath;
    _initTreatLtoF = initTreatLtoF;
    for (int i = 0; i < 10; i++) { _initTreatLtoFTier[i] = { initTreatLtoF }; }
    for (int i = 0; i < 20; i++) {
        _preTreatLtoFTierRegion[i].regionName = regDist.regions[i];
        for (int j = 0; j < 10; j++) { 
            _preTreatLtoFTierRegion[i].tier[j] = { initTreatLtoF };
        }
    }
    _treatLtoF = initTreatLtoF;
    for (int i = 0; i < 10; i++) { _treatLtoFTier[i] = { initTreatLtoF }; }
    _treatLtoFIP = initTreatLtoF;
    _relapseSuccess = rSuccess;
    _probNonInfTreated = probNITreat;
    _perDrugResistant = perDrugResist;
    _onsetToPresentation = 0;
    _perUntreatedDie = 50;
    _timeUntreatedUntilDeath = 0;
    _perUntestedRepresent = 17.8;
    _timeUntestedRepresent = 30;
    _perUntreatedRepresent = 17.8;
    _timeUntreatedRepresent = 30;
    _perTreatedRepresent = 17.8;
    _timeTreatedRepresent = 30;
    _perLtoFRepresentDeath = .49;
    _timeDeathRetreat = 180;
    _timeLtoFRepresent = 180;
    _timeUntreatCure = 3650;

    _stats = new Stats();

    _facilityStats = new Stats();

}

Disease::Disease(string name, float mort, float incid, float trans, int prior, int tPer, int tTime, int rTime, float initTreatSuccess, float initTreatDeath, float initTreatLtoF, float rSuccess,  float probNITreat, float perDrugResist, int onsetTime, float unDiePer, int unDieTime, float unTestReprePer, int unTestRepreTime, float unTreatReprePer, int unTreatRepreTime) {

    // store away information on creation
    _name = name;
    _mortality = mort;
    _incidence = incid;
    _incAdjType = "linear";
    _incAdjFactor = 0;
    _prpIncidencePresent = 1;
    _patientsRatioUninfInf = 1;
    _perPatientPresentInf = 1;
    _transFlag = false;
    _transmission = trans;
    _transThreshold = .1;
    _priority = prior;
    _treatedPatientsPercentage = tPer;
    _probPatientReturns  = probReturn;
    _treatmentTime = tTime;
    _relapseTime = rTime;
    _initTreatSuccess = initTreatSuccess;
    _initTreatDeath = initTreatDeath;
    _initTreatDeathIP = initTreatDeath;
    _initTreatLtoF = initTreatLtoF;
    for (int i = 0; i < 10; i++) { _initTreatLtoFTier[i] = { initTreatLtoF }; }
    for (int i = 0; i < 20; i++) {
        _preTreatLtoFTierRegion[i].regionName = regDist.regions[i];
        for (int j = 0; j < 10; j++) {
            _preTreatLtoFTierRegion[i].tier[j] = { initTreatLtoF };
        }
    }
    _treatLtoF = initTreatLtoF;
    for (int i = 0; i < 10; i++) { _treatLtoFTier[i] = { initTreatLtoF }; }
    for (int i = 0; i < 20; i++) {
        _treatLtoFTierRegion[i].regionName = regDist.regions[i];
        for (int j = 0; j < 10; j++) {
            _treatLtoFTierRegion[i].tier[j] = { initTreatLtoF };
        }
    }
    _treatLtoFIP = initTreatLtoF;
    _relapseSuccess = rSuccess;
    _probNonInfTreated = probNITreat;
    _perDrugResistant = perDrugResist;
    _onsetToPresentation = onsetTime;
    _perUntreatedDie = unDiePer;
    _timeUntreatedUntilDeath = unDieTime;
    _perUntestedRepresent = unTestReprePer;
    _timeUntestedRepresent = unTestRepreTime;
    _perUntreatedRepresent = unTreatReprePer;
    _timeUntreatedRepresent = unTreatRepreTime;
    _perLtoFRepresentDeath = .49;
    _timeDeathRetreat = 180;
    _timeLtoFRepresent = 180;
    _timeUntreatCure = 3650;

    _stats = new Stats();

    _facilityStats = new Stats();
}

     
Disease::Disease(string name, float mort, float incid, string incAdjType, float incAdjFactor, float trans, int prior, float morbidity, float preTestPer,  float preTestReductionPer,  float space1, float space2, float probRet, float perLostSamples, float tPer, int tTime, float initTreatSuccess, float initTreatDeath, float preTreatLtoF, float treatLtoF, int rTime, float rSuccess, float probNITreat, float perDrugResist, int onsetTime, float unDiePer, int unDieTime, float LtoFReprePer, int LtoFRepreTime) {

    // store away information on creation
    _name = name;
    _mortality = mort;
    _incidence = incid;
    _incAdjType = incAdjType;
    _incAdjFactor = incAdjFactor;
    _prpIncidencePresent = 1;
    _patientsRatioUninfInf = 1;
    _perPatientPresentInf = 1;
    _transFlag = false;
    _transmission = trans;
    _transThreshold = .1;
    _priority = prior;
    _morbidity = morbidity;
    _perPreTestMDCDInf = .15;
    _perPreTestMDCDUnInf = 99;
    _perPreTestLtoF = preTestPer;
    for (int i = 0; i < 10; i++) { _perPreTestLtoFTier[i] = { preTestPer }; }
    for (int i = 0; i < 20; i++) {
        _perPreTestLtoFTierRegion[i].regionName = regDist.regions[i];
        for (int j = 0; j < 10; j++) {
            _perPreTestLtoFTierRegion[i].tier[j] = { preTestPer };
        }
    }
    _perPreTestLtoFReduction = preTestReductionPer;
    _space1 = space1;
    _space2 = space2;
    _perLostSamples = perLostSamples;
    _treatedPatientsPercentage = tPer;
    _probPatientReturns  = probRet;
    _treatmentTime = tTime;
    _relapseTime = rTime;
    _initTreatSuccess = initTreatSuccess;
    _initTreatDeath = initTreatDeath;
    _initTreatDeathIP = initTreatDeath;
    _initTreatLtoF = preTreatLtoF;
    for (int i = 0; i < 10; i++) { _initTreatLtoFTier[i] = { preTreatLtoF }; }
    for (int i = 0; i < 20; i++) {
        _preTreatLtoFTierRegion[i].regionName = regDist.regions[i];
        for (int j = 0; j < 10; j++) {
            _preTreatLtoFTierRegion[i].tier[j] = { preTreatLtoF };
        }
    }
    _treatLtoF = treatLtoF;
    for (int i = 0; i < 10; i++) { _treatLtoFTier[i] = { treatLtoF }; }
    for (int i = 0; i < 20; i++) {
        _treatLtoFTierRegion[i].regionName = regDist.regions[i];
        for (int j = 0; j < 10; j++) {
            _treatLtoFTierRegion[i].tier[j] = { treatLtoF };
        }
    }
    _treatLtoFIP = treatLtoF;
    _relapseSuccess = rSuccess;
    _probNonInfTreated = probNITreat;
    _perDrugResistant = perDrugResist;
    _onsetToPresentation = onsetTime;
    _perUntreatedDie = unDiePer;
    _timeUntreatedUntilDeath = unDieTime;
    _perUntestedRepresent = 1-LtoFReprePer;
    _timeUntestedRepresent = LtoFRepreTime;
    _perUntreatedRepresent = 1-LtoFReprePer;
    _timeUntreatedRepresent = LtoFRepreTime;
    _perTreatedRepresent = 1-LtoFReprePer;
    _timeTreatedRepresent = LtoFRepreTime;
    _perLtoFRepresentDeath = LtoFReprePer;
    _timeDeathRetreat = 180;
    _timeLtoFRepresent = 180;
    _timeUntreatCure = 3650;

    _stats = new Stats();

    _facilityStats = new Stats();

}
 

Disease::Disease(string name, float mort, float incid, string incAdjType, float incAdjFactor, float trans, int prior, float morbidity, float preTestPer, float preTestReductionPer, float space1, float space2, float probRet, float perLostSamples, float tPer, int tTime, float initTreatSuccess, float initTreatDeath, float preTreatLtoF, float treatLtoF, int rTime, float rSuccess, float probNITreat, float perDrugResist, int onsetTime, float unDiePer, int unDieTime, float LtoFReprePer, int maxTimeDeathRetreat, int maxTimeLtoFRepresent, int maxTimeUntreatCure) {
 
    // store away information on creation
    _name = name;
    _mortality = mort;
    _incidence = incid;
    _incAdjType = incAdjType;
    _incAdjFactor = incAdjFactor;
    _prpIncidencePresent = 1;
    _patientsRatioUninfInf = 1;
    _perPatientPresentInf = 1;
    _transFlag = false;
    _transmission = trans;
    _transThreshold = .1;
    _priority = prior;
    _morbidity = morbidity;
    _perPreTestMDCDInf = .15;
    _perPreTestMDCDUnInf = 99;
    _perPreTestLtoF = preTestPer;
    for (int i = 0; i < 10; i++) { _perPreTestLtoFTier[i] = { preTestPer }; }
    for (int i = 0; i < 20; i++) {
        _perPreTestLtoFTierRegion[i].regionName = regDist.regions[i];
        for (int j = 0; j < 10; j++) {
            _perPreTestLtoFTierRegion[i].tier[j] = { preTestPer };
        }
    }
    _perPreTestLtoFReduction = preTestReductionPer;
    _space1 = space1;
    _space2 = space2;
    _perLostSamples = perLostSamples;
    _treatedPatientsPercentage = tPer;
    _probPatientReturns = probRet;
    _treatmentTime = tTime;
    _relapseTime = rTime;
    _initTreatSuccess = initTreatSuccess;
    _initTreatDeath = initTreatDeath;
    _initTreatDeathIP = initTreatDeath;
    _initTreatLtoF = preTreatLtoF;
    for (int i = 0; i < 10; i++) { _initTreatLtoFTier[i] = { preTreatLtoF }; }
    for (int i = 0; i < 20; i++) {
        _preTreatLtoFTierRegion[i].regionName = regDist.regions[i];
        for (int j = 0; j < 10; j++) {
            _preTreatLtoFTierRegion[i].tier[j] = { preTreatLtoF };
        }
    }
    _treatLtoF = treatLtoF;
    for (int i = 0; i < 10; i++) { _treatLtoFTier[i] = { treatLtoF }; }
    for (int i = 0; i < 20; i++) {
        _treatLtoFTierRegion[i].regionName = regDist.regions[i];
        for (int j = 0; j < 10; j++) {
            _treatLtoFTierRegion[i].tier[j] = { treatLtoF };
        }
    }
    _treatLtoFIP = treatLtoF;
    _relapseSuccess = rSuccess;
    _probNonInfTreated = probNITreat;
    _perDrugResistant = perDrugResist;
    _onsetToPresentation = onsetTime;
    _perUntreatedDie = unDiePer;
    _timeUntreatedUntilDeath = unDieTime;
    _perUntestedRepresent = 1-LtoFReprePer;
    _timeUntestedRepresent = maxTimeLtoFRepresent;
    _perUntreatedRepresent = 1-LtoFReprePer;
    _timeUntreatedRepresent = maxTimeLtoFRepresent;
    _perTreatedRepresent = 1-LtoFReprePer;
    _timeTreatedRepresent = maxTimeLtoFRepresent;
    _perLtoFRepresentDeath = LtoFReprePer;
    _timeDeathRetreat = maxTimeDeathRetreat;
    _timeLtoFRepresent = maxTimeLtoFRepresent;
    _timeUntreatCure = maxTimeUntreatCure;

    _stats = new Stats();

    _facilityStats = new Stats();

}

Disease::Disease(string name, float mort, float incid,  string incAdjType, float incAdjFactor, float trans, int prior, float morbidity, float preTestPer, float preTestReductionPer, float space1, float space2, float probRet, float perLostSamples, float tPer, int tTime, float initTreatSuccess, float initTreatDeath, float initTreatDeathIP, float preTreatLtoF, float treatLtoF, float treatLtoFIP, int rTime, float rSuccess, float probNITreat, float perDrugResist, int onsetTime, float unDiePer, int unDieTime, float LtoFReprePer, int maxTimeDeathRetreat, int maxTimeLtoFRepresent, int maxTimeUntreatCure) {

    // store away information on creation
    _name = name;
    _mortality = mort;
    _incidence = incid;
    _incAdjType = incAdjType;
    _incAdjFactor = incAdjFactor;
    _prpIncidencePresent = 1;
    _patientsRatioUninfInf = 1;
    _perPatientPresentInf = 1;
    _transFlag = false;
    _transmission = trans;
    _transThreshold = .1;
    _priority = prior;
    _morbidity = morbidity;
    _perPreTestMDCDInf = .15;
    _perPreTestMDCDUnInf = 99;
    _perPreTestLtoF = preTestPer;
    for (int i = 0; i < 10; i++) { _perPreTestLtoFTier[i] = { preTestPer }; }
    for (int i = 0; i < 20; i++) {
        _perPreTestLtoFTierRegion[i].regionName = regDist.regions[i];
        for (int j = 0; j < 10; j++) {
            _perPreTestLtoFTierRegion[i].tier[j] = { preTestPer };
        }
    }
    _perPreTestLtoFReduction = preTestReductionPer;
    _space1 = space1;
    _space2 = space2;
    _perLostSamples = perLostSamples;
    _treatedPatientsPercentage = tPer;
    _probPatientReturns = probRet;
    _treatmentTime = tTime;
    _relapseTime = rTime;
    _initTreatSuccess = initTreatSuccess;
    _initTreatDeath = initTreatDeath;
    _initTreatDeathIP = initTreatDeathIP;
    _initTreatLtoF = preTreatLtoF;
    for (int i = 0; i < 10; i++) { _initTreatLtoFTier[i] = { preTreatLtoF }; }
    for (int i = 0; i < 20; i++) {
        _preTreatLtoFTierRegion[i].regionName = regDist.regions[i];
        for (int j = 0; j < 10; j++) {
            _preTreatLtoFTierRegion[i].tier[j] = { preTreatLtoF };
        }
    }
    _treatLtoF = treatLtoF;
    for (int i = 0; i < 10; i++) { _treatLtoFTier[i] = { treatLtoF }; }
    for (int i = 0; i < 20; i++) {
        _treatLtoFTierRegion[i].regionName = regDist.regions[i];
        for (int j = 0; j < 10; j++) {
            _treatLtoFTierRegion[i].tier[j] = { treatLtoF };
        }
    }
    _treatLtoFIP = treatLtoFIP;
    _relapseSuccess = rSuccess;
    _probNonInfTreated = probNITreat;
    _perDrugResistant = perDrugResist;
    _onsetToPresentation = onsetTime;
    _perUntreatedDie = unDiePer;
    _timeUntreatedUntilDeath = unDieTime;
    _perUntestedRepresent = 1-LtoFReprePer;
    _timeUntestedRepresent = maxTimeLtoFRepresent;
    _perUntreatedRepresent = 1-LtoFReprePer;
    _timeUntreatedRepresent = maxTimeLtoFRepresent;
    _perTreatedRepresent = 1-LtoFReprePer;
    _perLtoFRepresentDeath = LtoFReprePer;
    _timeTreatedRepresent = maxTimeLtoFRepresent;
    _timeDeathRetreat = maxTimeDeathRetreat;
    _timeLtoFRepresent = maxTimeLtoFRepresent;
    _timeUntreatCure = maxTimeUntreatCure;

    _stats = new Stats();

    _facilityStats = new Stats();

}

Disease::Disease(string name, float mort, float incid, string incAdjType, float incAdjFactor, float prpIncPresent, float patRatioUninfInf, float trans, int prior, float morbidity, float preTestPer, float preTestReductionPer, float space1, float space2, float probRet, float perLostSamples, float tPer, int tTime, float initTreatSuccess, float initTreatDeath, float initTreatDeathIP, float preTreatLtoF, float treatLtoF, float treatLtoFIP, int rTime, float rSuccess, float probNITreat, float perDrugResist, int onsetTime, float unDiePer, int unDieTime, float probRepresentDie, int maxTimeDeathRetreat, int maxTimeLtoFRepresent, int maxTimeUntreatCure) {

    // store away information on creation
    _name = name;
    _mortality = mort;
    _incidence = incid;
    _incAdjType = incAdjType;
    _incAdjFactor = incAdjFactor;
    _prpIncidencePresent = prpIncPresent;
    _patientsRatioUninfInf = patRatioUninfInf;
    _perPatientPresentInf = 1;
    _transFlag = false;
    _transmission = trans;
    _transThreshold = .1;
    _priority = prior;
    _morbidity = morbidity;
    _perPreTestMDCDInf = .15;
    _perPreTestMDCDUnInf = 99;
    _perPreTestLtoF = preTestPer;
    for (int i = 0; i < 10; i++) { _perPreTestLtoFTier[i] = { preTestPer }; }
    for (int i = 0; i < 20; i++) {
        _perPreTestLtoFTierRegion[i].regionName = regDist.regions[i];
        for (int j = 0; j < 10; j++) {
            _perPreTestLtoFTierRegion[i].tier[j] = { preTestPer };
        }
    }
    _perPreTestLtoFReduction = preTestReductionPer;
    _space1 = space1;
    _space2 = space2;
    _perLostSamples = perLostSamples;
    _treatedPatientsPercentage = tPer;
    _probPatientReturns = probRet;
    _treatmentTime = tTime;
    _relapseTime = rTime;
    _initTreatSuccess = initTreatSuccess;
    _initTreatDeath = initTreatDeath;
    _initTreatDeathIP = initTreatDeathIP;
    _initTreatLtoF = preTreatLtoF;
    for (int i = 0; i < 10; i++) { _initTreatLtoFTier[i] = { preTreatLtoF }; }
    for (int i = 0; i < 20; i++) {
        _preTreatLtoFTierRegion[i].regionName = regDist.regions[i];
        for (int j = 0; j < 10; j++) {
            _preTreatLtoFTierRegion[i].tier[j] = { preTreatLtoF };
        }
    }
    _treatLtoF = treatLtoF;
    for (int i = 0; i < 10; i++) { _treatLtoFTier[i] = { treatLtoF }; }
    for (int i = 0; i < 20; i++) {
        _treatLtoFTierRegion[i].regionName = regDist.regions[i];
        for (int j = 0; j < 10; j++) {
            _treatLtoFTierRegion[i].tier[j] = { treatLtoF };
        }
    }
    _treatLtoFIP = treatLtoFIP;
    _relapseSuccess = rSuccess;
    _probNonInfTreated = probNITreat;
    _perDrugResistant = perDrugResist;
    _onsetToPresentation = onsetTime;
    _perUntreatedDie = unDiePer;
    _timeUntreatedUntilDeath = unDieTime;
    _perUntestedRepresent = 1- probRepresentDie;
    _timeUntestedRepresent = maxTimeLtoFRepresent;
    _perUntreatedRepresent = 1-probRepresentDie;
    _timeUntreatedRepresent = maxTimeLtoFRepresent;
    _perTreatedRepresent = 1 - probRepresentDie;
    _perLtoFRepresentDeath = probRepresentDie;
    _timeTreatedRepresent = maxTimeLtoFRepresent;
    _timeDeathRetreat = maxTimeDeathRetreat;
    _timeLtoFRepresent = maxTimeLtoFRepresent;
    _timeUntreatCure = maxTimeUntreatCure;

    _stats = new Stats();

    _facilityStats = new Stats();

}

Disease::Disease(string name, float mort, float incid, string incAdjType, float incAdjFactor, float prpIncPresent, float patRatioUninfInf, float trans, int prior, float morbidity, float preTestMDCDInfPer, float preTestMDCDUnInfPer, float preTestPer, float preTestReductionPer, float space1, float space2, float probRet, float perLostSamples, float tPer, int tTime, float initTreatSuccess, float initTreatDeath, float initTreatDeathIP, float preTreatLtoF, float treatLtoF, float treatLtoFIP, int rTime, float rSuccess, float probNITreat, float perDrugResist, int onsetTime, float unDiePer, int unDieTime, float probRepresentDie, int maxTimeDeathRetreat, int maxTimeLtoFRepresent, int maxTimeUntreatCure) {

    // store away information on creation
    _name = name;
    _mortality = mort;
    _incidence = incid;
    _incAdjType = incAdjType;
    _incAdjFactor = incAdjFactor;
    _prpIncidencePresent = prpIncPresent;
    _patientsRatioUninfInf = patRatioUninfInf;
    _perPatientPresentInf = 1;
    _transFlag = false;
    _transmission = trans;
    _transThreshold = .1;
    _priority = prior;
    _morbidity = morbidity;
    _perPreTestMDCDInf = preTestMDCDInfPer;
    _perPreTestMDCDUnInf = preTestMDCDUnInfPer;
    _perPreTestLtoF = preTestPer;
    for (int i = 0; i < 10; i++) { _perPreTestLtoFTier[i] = { preTestPer }; }
    for (int i = 0; i < 20; i++) {
        _perPreTestLtoFTierRegion[i].regionName = regDist.regions[i];
        for (int j = 0; j < 10; j++) {
            _perPreTestLtoFTierRegion[i].tier[j] = { preTestPer };
        }
    }
    _perPreTestLtoFReduction = preTestReductionPer;
    _space1 = space1;
    _space2 = space2;
    _perLostSamples = perLostSamples;
    _treatedPatientsPercentage = tPer;
    _probPatientReturns = probRet;
    _treatmentTime = tTime;
    _relapseTime = rTime;
    _initTreatSuccess = initTreatSuccess;
    _initTreatDeath = initTreatDeath;
    _initTreatDeathIP = initTreatDeathIP;
    _initTreatLtoF = preTreatLtoF;
    for (int i = 0; i < 10; i++) { _initTreatLtoFTier[i] = { preTreatLtoF }; }
    for (int i = 0; i < 20; i++) {
        _preTreatLtoFTierRegion[i].regionName = regDist.regions[i];
        for (int j = 0; j < 10; j++) {
            _preTreatLtoFTierRegion[i].tier[j] = { preTreatLtoF };
        }
    }
    _treatLtoF = treatLtoF;
    for (int i = 0; i < 10; i++) { _treatLtoFTier[i] = { treatLtoF }; }
    for (int i = 0; i < 20; i++) {
        _treatLtoFTierRegion[i].regionName = regDist.regions[i];
        for (int j = 0; j < 10; j++) {
            _treatLtoFTierRegion[i].tier[j] = { treatLtoF };
        }
    }
    _treatLtoFIP = treatLtoFIP;
    _relapseSuccess = rSuccess;
    _probNonInfTreated = probNITreat;
    _perDrugResistant = perDrugResist;
    _onsetToPresentation = onsetTime;
    _perUntreatedDie = unDiePer;
    _timeUntreatedUntilDeath = unDieTime;
    _perUntestedRepresent = 1 - probRepresentDie;
    _timeUntestedRepresent = maxTimeLtoFRepresent;
    _perUntreatedRepresent = 1 - probRepresentDie;
    _timeUntreatedRepresent = maxTimeLtoFRepresent;
    _perTreatedRepresent = 1 - probRepresentDie;
    _perLtoFRepresentDeath = probRepresentDie;
    _timeTreatedRepresent = maxTimeLtoFRepresent;
    _timeDeathRetreat = maxTimeDeathRetreat;
    _timeLtoFRepresent = maxTimeLtoFRepresent;
    _timeUntreatCure = maxTimeUntreatCure;

    _stats = new Stats();

    _facilityStats = new Stats();

}


Disease::Disease(string name, float mort, float incid, string incAdjType, float incAdjFactor, float prpIncPresent, float patRatioUninfInf, float perPatientPresentInf, float trans, int prior, float morbidity, float preTestMDCDInfPer, float preTestMDCDUnInfPer, float preTestPer, float preTestReductionPer, float space1, float space2, float probRet, float perLostSamples, float tPer, int tTime, float initTreatSuccess, float initTreatDeath, float initTreatDeathIP, float preTreatLtoF, float treatLtoF, float treatLtoFIP, int rTime, float rSuccess, float probNITreat, float perDrugResist, int onsetTime, float unDiePer, int unDieTime, float probRepresentDie, int maxTimeDeathRetreat, int maxTimeLtoFRepresent, int maxTimeUntreatCure) {

    // store away information on creation
    _name = name;
    _mortality = mort;
    _incidence = incid;
    _incAdjType = incAdjType;
    _incAdjFactor = incAdjFactor;
    _prpIncidencePresent = prpIncPresent;
    _patientsRatioUninfInf = patRatioUninfInf;
    _perPatientPresentInf = perPatientPresentInf;
    _transFlag = false;
    _transmission = trans;
    _transThreshold = .1;
    _priority = prior;
    _morbidity = morbidity;
    _perPreTestMDCDInf = preTestMDCDInfPer;
    _perPreTestMDCDUnInf = preTestMDCDUnInfPer;
    _perPreTestLtoF = preTestPer;
    for (int i = 0; i < 10; i++) { _perPreTestLtoFTier[i] = { preTestPer }; }
    for (int i = 0; i < 20; i++) {
        _perPreTestLtoFTierRegion[i].regionName = regDist.regions[i];
        for (int j = 0; j < 10; j++) {
            _perPreTestLtoFTierRegion[i].tier[j] = { preTestPer };
        }
    }
    _perPreTestLtoFReduction = preTestReductionPer;
    _space1 = space1;
    _space2 = space2;
    _perLostSamples = perLostSamples;
    _treatedPatientsPercentage = tPer;
    _probPatientReturns = probRet;
    _treatmentTime = tTime;
    _relapseTime = rTime;
    _initTreatSuccess = initTreatSuccess;
    _initTreatDeath = initTreatDeath;
    _initTreatDeathIP = initTreatDeathIP;
    _initTreatLtoF = preTreatLtoF;
    for (int i = 0; i < 10; i++) { _initTreatLtoFTier[i] = { preTreatLtoF }; }
    for (int i = 0; i < 20; i++) {
        _preTreatLtoFTierRegion[i].regionName = regDist.regions[i];
        for (int j = 0; j < 10; j++) {
            _preTreatLtoFTierRegion[i].tier[j] = { preTreatLtoF };
        }
    }
    _treatLtoF = treatLtoF;
    for (int i = 0; i < 10; i++) { _treatLtoFTier[i] = { treatLtoF }; }
    for (int i = 0; i < 20; i++) {
        _treatLtoFTierRegion[i].regionName = regDist.regions[i];
        for (int j = 0; j < 10; j++) {
            _treatLtoFTierRegion[i].tier[j] = { treatLtoF };
        }
    }
    _treatLtoFIP = treatLtoFIP;
    _relapseSuccess = rSuccess;
    _probNonInfTreated = probNITreat;
    _perDrugResistant = perDrugResist;
    _onsetToPresentation = onsetTime;
    _perUntreatedDie = unDiePer;
    _timeUntreatedUntilDeath = unDieTime;
    _perUntestedRepresent = 1 - probRepresentDie;
    _timeUntestedRepresent = maxTimeLtoFRepresent;
    _perUntreatedRepresent = 1 - probRepresentDie;
    _timeUntreatedRepresent = maxTimeLtoFRepresent;
    _perTreatedRepresent = 1 - probRepresentDie;
    _perLtoFRepresentDeath = probRepresentDie;
    _timeTreatedRepresent = maxTimeLtoFRepresent;
    _timeDeathRetreat = maxTimeDeathRetreat;
    _timeLtoFRepresent = maxTimeLtoFRepresent;
    _timeUntreatCure = maxTimeUntreatCure;

    _stats = new Stats();

    _facilityStats = new Stats();

}

Disease::Disease(string name, float mort, float incid, string incAdjType, float incAdjFactor, float prpIncPresent, float patRatioUninfInf, float perPatientPresentInf, float trans, int prior, float morbidity, float preTestMDCDInfPer, float preTestMDCDUnInfPer, float preTestPer, float preTestPerTier[10], float preTestReductionPer, float space1, float space2, float probRet, float perLostSamples, float tPer, int tTime, float initTreatSuccess, float initTreatDeath, float initTreatDeathIP, float preTreatLtoF, float preTreatLtoFTier[10], float treatLtoF, float treatLtoFTier[10], float treatLtoFIP, int rTime, float rSuccess, float probNITreat, float perDrugResist, int onsetTime, float unDiePer, int unDieTime, float probRepresentDie, int maxTimeDeathRetreat, int maxTimeLtoFRepresent, int maxTimeUntreatCure) {

    // store away information on creation
    _name = name;
    _mortality = mort;
    _incidence = incid;
    _incAdjType = incAdjType;
    _incAdjFactor = incAdjFactor;
    _prpIncidencePresent = prpIncPresent;
    _patientsRatioUninfInf = patRatioUninfInf;
    _perPatientPresentInf = perPatientPresentInf;
    _transFlag = false;
    _transmission = trans;
    _transThreshold = .1;
    _priority = prior;
    _morbidity = morbidity;
    _perPreTestMDCDInf = preTestMDCDInfPer;
    _perPreTestMDCDUnInf = preTestMDCDUnInfPer;
    _perPreTestLtoF = preTestPer;
    for (int i = 0; i < 10; i++) { _perPreTestLtoFTier[i] = preTestPerTier[i]; }
    for (int i = 0; i < 20; i++) {
        _perPreTestLtoFTierRegion[i].regionName = regDist.regions[i];
        for (int j = 0; j < 10; j++) {
            _perPreTestLtoFTierRegion[i].tier[j] = { preTestPerTier[j] };
        }
    }
    _perPreTestLtoFReduction = preTestReductionPer;
    _space1 = space1;
    _space2 = space2;
    _perLostSamples = perLostSamples;
    _treatedPatientsPercentage = tPer;
    _probPatientReturns = probRet;
    _treatmentTime = tTime;
    _relapseTime = rTime;
    _initTreatSuccess = initTreatSuccess;
    _initTreatDeath = initTreatDeath;
    _initTreatDeathIP = initTreatDeathIP;
    _initTreatLtoF = preTreatLtoF;
    for (int i = 0; i < 10; i++) { _initTreatLtoFTier[i] = preTreatLtoFTier[i]; }
    for (int i = 0; i < 20; i++) {
        _preTreatLtoFTierRegion[i].regionName = regDist.regions[i];
        //std::cout << "preTreatltof disease = " << name << " region name = " << _preTreatLtoFTierRegion[i].regionName;
        for (int j = 0; j < 10; j++) {
            _preTreatLtoFTierRegion[i].tier[j] = { preTreatLtoFTier[j] };
            //std::cout << " i = " << _preTreatLtoFTierRegion[i].tier[j];
        }
        //std::cout  << "\n";
    }
    _treatLtoF = treatLtoF;
    for (int i = 0; i < 10; i++) { _treatLtoFTier[i] = treatLtoFTier[i] ; }
    for (int i = 0; i < 20; i++) {
        _treatLtoFTierRegion[i].regionName = regDist.regions[i];
        for (int j = 0; j < 10; j++) {
            _treatLtoFTierRegion[i].tier[j] = { treatLtoFTier[j] };
        }
    }
    _treatLtoFIP = treatLtoFIP;
    _relapseSuccess = rSuccess;
    _probNonInfTreated = probNITreat;
    _perDrugResistant = perDrugResist;
    _onsetToPresentation = onsetTime;
    _perUntreatedDie = unDiePer;
    _timeUntreatedUntilDeath = unDieTime;
    _perUntestedRepresent = 1 - probRepresentDie;
    _timeUntestedRepresent = maxTimeLtoFRepresent;
    _perUntreatedRepresent = 1 - probRepresentDie;
    _timeUntreatedRepresent = maxTimeLtoFRepresent;
    _perTreatedRepresent = 1 - probRepresentDie;
    _perLtoFRepresentDeath = probRepresentDie;
    _timeTreatedRepresent = maxTimeLtoFRepresent;
    _timeDeathRetreat = maxTimeDeathRetreat;
    _timeLtoFRepresent = maxTimeLtoFRepresent;
    _timeUntreatCure = maxTimeUntreatCure;

    _stats = new Stats();

    _facilityStats = new Stats();

}




// get disease incidence
float Disease::getIncidence() { 

    //std::size_t i;
    float returnIncidence;
    
    //std::cout << "get incidence, currentYearInRun = " << currentYearInRun << "\n";

    // default to current incidence value
    returnIncidence = _incidence;

    if (_incAdjType == "linear") {

        // adjust incidence by incAdjFactor by no. of year - incAdjFactor can be pos. or neg.
        if (currentYearInRun > 0) {

            for (int i = 0; i < currentYearInRun; i++) {

                returnIncidence = returnIncidence * (1 + _incAdjFactor);
            }
        }
    }
    else {

        returnIncidence = _incidence;
    }

    // adjust incidence to per person per day
    returnIncidence = returnIncidence / (100000 * 365);

    return returnIncidence;
}

// get disease incidence
float Disease::getIncidence(int year) { 

    //std::size_t i;
    float returnIncidence;

    //std::cout << "get incidence, year = " << year << "\n";

    // default to current incidence value
    returnIncidence = _incidence;

    if (_incAdjType == "linear") {

        // adjust incidence by incAdjFactor by no. of year - incAdjFactor can be pos. or neg.
        if (currentYearInRun > 0) {

            for (int i = 0; i < year; i++) {

                returnIncidence = returnIncidence * (1 + _incAdjFactor);
            }
        }
    }
    else {

        returnIncidence = _incidence;
    }

    // adjust incidence to per person per day
    returnIncidence = returnIncidence / (100000 * 365);

    return returnIncidence;
}



// find index of mortality percentage
int Disease::findUntreatedMortalityIndex(float mortality){

    std::size_t i;
    int retIndex = 0;
    bool foundIndex = false;
    
    // make sure it is between 0-1
    if (mortality > 1) {
         
        mortality = mortality / 100;
    }

    for (i = 0; i < 120; i++) {

        if ((_untreatMortality[i] > mortality) && (!foundIndex)) {

            retIndex = i;

            foundIndex = true;
        }

    }
    
    return retIndex;
}


// get preTest loss to follow up region name by index
string Disease::getPerPreTestLtoFTierRegionName(int index) {

    string name = "";

    name = _perPreTestLtoFTierRegion[index].regionName;
   
    return name;
}


// get pretreatment loss to follow up region name by index
string Disease::getPreTreatLtoFTierRegionName(int index) {

    string name = "";

    name = _preTreatLtoFTierRegion[index].regionName;

    return name;
}


// get treatment loss to follow up region name by index
string Disease::getTreatLtoFTierRegionName(int index) {

    string name = "";

    name = _treatLtoFTierRegion[index].regionName;

    return name;
}



// get initial treatment loss to follow up by tier
float Disease::getPerPreTestLtoFTierRegion(string region, int tier) {

    float perPreTestLtoF = 0;

    // regional tier values default to the global value.  They can be overridden after that
    // pull the first value and override it if there are different regional values
    perPreTestLtoF = _perPreTestLtoFTierRegion[0].tier[tier - 1];

    for (int i = 0; i < 20; i++) {

        if (_perPreTestLtoFTierRegion[i].regionName == region) {

            perPreTestLtoF = _perPreTestLtoFTierRegion[i].tier[tier - 1];

            break;
        }
    }
    //std::cout << "get getperPreTestLtoFtLtoFTierRegion, region = " << region << " tier = " << tier << "\n";

    return perPreTestLtoF;
}


// get initial treatment loss to follow up by tier
float Disease::getPreTreatmentLtoFTierRegion(string region, int tier) {

    float initTreatLtoF = 0;

    // regional tier values default to the global value.  They can be overridden after that
    // pull the first value and override it if there are different regional values
    initTreatLtoF = _preTreatLtoFTierRegion[0].tier[tier - 1];

    //std::cout << "inside getInitialTreatementLtoFTier Region = " << region << " tier = " << tier << " initTreatLtoF = " << initTreatLtoF << '\n';

    for (int i = 0; i < 20; i++) {

        if (_preTreatLtoFTierRegion[i].regionName == region) {

            initTreatLtoF = _preTreatLtoFTierRegion[i].tier[tier-1];

            break;
        }
    }
    //std::cout << "get getInitialTreatmentLtoFTierRegion, region = " << region << " tier = " << tier << "\n";

    return initTreatLtoF;
}

// get initial treatment loss to follow up by tier
float Disease::getTreatmentLtoFTierRegion(string region, int tier) {

    float treatLtoF = 0;

    // regional tier values default to the global value.  They can be overridden after that
    // pull the first value and override it if there are different regional values
    treatLtoF = _treatLtoFTierRegion[0].tier[tier - 1];

    for (int i = 0; i < 20; i++) {

        if (_treatLtoFTierRegion[i].regionName == region) {

            treatLtoF = _treatLtoFTierRegion[i].tier[tier - 1];

            break;
        }
    }
    //std::cout << "get getTreatmentLtoFTierRegion, region = " << region << " tier = " << tier << "\n";

    return treatLtoF;
}



// set initial treatment loss to follow up by tier
void Disease::setPerPreTestLtoFTierRegion(string region, int tier, float preTestLtoF) {

    bool foundFlag = false;

    //std::cout << "setPerPreTestLtoFTierRegion, region = " << region << "\n";
    
    // if can find region, update tier value
    for (int i = 0; i < 20; i++) {

        if (_perPreTestLtoFTierRegion[i].regionName == region) {

            _perPreTestLtoFTierRegion[i].tier[tier] = preTestLtoF;

            foundFlag = true;

            //std::cout << "setPerPreTestLtoFTierRegion, found region setting tier" <<  "\n";

            break;
        }
    }

    // didn't find region, create new item
    if (!foundFlag) {

        for (int i = 0; i < 20; i++) {
            
            // find first open slot
            if (_perPreTestLtoFTierRegion[i].regionName == "") {

                _perPreTestLtoFTierRegion[i].regionName = region;
                _perPreTestLtoFTierRegion[i].tier[tier] = preTestLtoF;

                break;
            }
        }
    }
}


// set initial treatment loss to follow up by tier
void Disease::setPreTreatmentLtoFTierRegion(string region, int tier, float initTreatLtoF) {

    bool foundFlag = false;
    
    // if can find region, update tier value
    for (int i = 0; i < 20; i++) {

        if (_preTreatLtoFTierRegion[i].regionName == region) {
            _preTreatLtoFTierRegion[i].tier[tier] = initTreatLtoF;

            foundFlag = true;

            break;
        }
    }

    // didn't find region, create new item
    if (!foundFlag) {

        for (int i = 0; i < 20; i++) {

            // find first open slot
            if (_preTreatLtoFTierRegion[i].regionName == "") {

                _preTreatLtoFTierRegion[i].regionName = region;
                _preTreatLtoFTierRegion[i].tier[tier] = initTreatLtoF;

                break;
            }
        }
    }
}

// set initial treatment loss to follow up by tier
void Disease::setTreatmentLtoFTierRegion(string region, int tier, float treatLtoF) {

    bool foundFlag = false;

    // if can find region, update tier value
    for (int i = 0; i < 20; i++) {

        if (_treatLtoFTierRegion[i].regionName == region) {
            _treatLtoFTierRegion[i].tier[tier] = treatLtoF;

            foundFlag = true;

            break;
        }
    }

    // didn't find region, create new item
    if (!foundFlag) {

        for (int i = 0; i < 20; i++) {

            // find first open slot
            if (_treatLtoFTierRegion[i].regionName == "") {

                _treatLtoFTierRegion[i].regionName = region;
                _treatLtoFTierRegion[i].tier[tier] = treatLtoF;

                break;
            }
        }
    }
}

// dump pre-test loss to follow up by tier values
void Disease::dumpPerPreTestLtoFTierRegion() {
    
    // if can find region, update tier value
    for (int i = 0; i < 20; i++) {

        std::cout << "pre-test ltof region = " << _perPreTestLtoFTierRegion[i].regionName;
        
        for (int j = 0; j < 10; j++) {
            
            std::cout << ", " << _perPreTestLtoFTierRegion[i].tier[j];
        }
        std::cout << '\n';
    }
}

// dump pre-test loss to follow up by tier values
void Disease::dumpPreTreatmentLtoFTierRegion() {

    // if can find region, update tier value
    for (int i = 0; i < 20; i++) {

        std::cout << "pre-treat ltof region = " << _preTreatLtoFTierRegion[i].regionName;;

        for (int j = 0; j < 10; j++) {

            std::cout << ", " << _preTreatLtoFTierRegion[i].tier[j];
        }
        std::cout << '\n';
    }
}

// dump pre-test loss to follow up by tier values
void Disease::dumpTreatmentLtoFTierRegion() {

    // if can find region, update tier value
    for (int i = 0; i < 20; i++) {

        std::cout << "treat ltof region = " << _treatLtoFTierRegion[i].regionName;;

        for (int j = 0; j < 10; j++) {

            std::cout << ", " << _treatLtoFTierRegion[i].tier[j];
        }
        std::cout << '\n';
    }
}



// function to get region names 
string getRegionNames(int regIndex) {

    string region = "";

    region = regDist.regions[regIndex];

    //std::cout << "getRegNames region = " << region << "\n";

    return region;
}



Disease::~Disease() {
    //cout<<"Removeing Disease class"<<_id<<endl;
}
//---------------------------------------




