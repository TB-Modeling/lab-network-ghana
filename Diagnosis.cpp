//
//  Diagnosis.cpp
//  mdrtb
//
//  Created by L. Dombrowski on 1/10/2020.
//
//#include <random> //Code that uses this commented out only,
//#include <chrono> //will not delete JP
#include <iostream>
#include <vector>
#include <string>

#include "Diagnosis.h"
#include "FGlobal.h"
#include "GlobalParameters.h"
#include "CreateAgents.h"


using namespace std;



Diagnosis::Diagnosis() {
};

Diagnosis::Diagnosis(bool diagnosis, string facility) {

    // store away information on creation
    _facility = facility;
    _diagnosisAvailable = diagnosis;

}

Diagnosis::Diagnosis(bool diagnosis, string facility, string closestTestingFacility, float testingDistance) {

    // store away information on creation
    _facility = facility;

    _diagnosisAvailable = diagnosis;

}

// set the testing Facility associated with this Diagnosis agent
string Diagnosis::getTestingFacility(string disease, string test, bool outputFlag) {

    string name = "";
    //vector<diagnosisAttributes*> diagnosisAtt;

    if (outputFlag) {
        std::cout << "inside getTestingFacility  " << "\n";
    }

    // find closest testing facility based on disease and test name
    for (auto diagAttr : _diagnosisAttributes) {

        if (outputFlag) {
            std::cout << "getTestingFacility  disease = " << diagAttr->disease << ", testName = " << diagAttr->testName << ", testing = " << diagAttr->testing << ", closetConnectionWithTesting = " << diagAttr->closetConnectionWithTesting << ", closestConnectionDistance = " << diagAttr->closestConnectionDistance << "\n";
        }
        if (diagAttr->disease == disease) {

            if (diagAttr->testName == test) {

                name = diagAttr->closetConnectionWithTesting;
            }
        }
    }

    return name;
}


// set the testing Facility associated with this Diagnosis agent
string Diagnosis::getTestingFacility(string disease, string test) {

    string name = "";
    vector<diagnosisAttributes*> diagnosisAtt;

    //std::cout << "inside getTestingFacility  disease = " << disease << ", test = " << test << "\n";

    // find closest testing facility based on disease and test name
    for (auto diagAttr : _diagnosisAttributes) {

        if (diagAttr->disease == disease) {

            if (diagAttr->testName == test) {

                name = diagAttr->closetConnectionWithTesting;
                //std::cout << "inside getTestingFacility  closest connection = "  << name << "\n";
}           
        }
    }

    return name;
}


// Get testing facility for disease, if testing here, retrun it first otherwise return first away testing site found
string Diagnosis::getTestingFacility(string disease) {

    string facility = "";
    vector<diagnosisAttributes*> diagnosisAtt;

    // see if any testing done here for this disease - eventually will need to figure in priority of tests
    for (auto diagAttr : _diagnosisAttributes) {

        if (diagAttr->disease == disease) {

            if (diagAttr->testing > 0) {

                facility = _facility;

                return facility;
            }

        }
    }

    // didn't find any tests for the disease here - find first one away
    for (auto diagAttr : _diagnosisAttributes) {

        if (diagAttr->disease == disease) {

            if (diagAttr->testing < 1) {

                facility = _facility;

                return facility;
            }

        }
    }
    return facility;
}




// create patients for this facility for every disease in list
void Diagnosis::createPatients(string fName, vector<string> list, int pop, float binInc, float* popBins, vector<Disease*> diseases, int timestep) {

    // loop through list of diseases and make patients for each disease based on catchment population distributions and 
    // disease probablilities
    //int i,j,k,l;
    //These types are more properly std::size_t, which is unsigned
    std::size_t i, j, k;
    int l;
    int noInfected;
    int noNotInfected;
    int totNoInfected;
    int randInt;
    int transitDays;
    int distIndex = 0;
    int regIndex = 0;
    float distance{};
    float testingFacilityDistance = 0;
    float rand;
    //float mort;
    float incid;
    //float trans;
    int perDrugResistant{};
    int priority;
    float perLostSamples = 0;
    bool success;
    string testingFacility;
    string diseaseOnListName;
    string diseaseName;
    string patientDiseaseName;
    string testName;
    string district = "";
    string region = "";
    Patient* p;
    Sample* s;
    Facility* f;

    std::cout << "inside diagnosis createPatients " << canDoDiagnosis() << "\n";

    // check to see if Diagnosis if available at this facility
    if (canDoDiagnosis()) {
        std::cout << "inside createPatients after check \n";

        // loop through the list of diseases to create and get the chacteristics of that disease
        // and create Patients with that disease
        for (i = 0; i < list.size(); i++) {

            // get disease name from list of diseases to watch
            diseaseOnListName = list[i];

            success = false;

            // default values
            //mort = 0;
            incid = 0;
            //trans = 0;
            priority = 0;

            // loop through list of disease objects and find one that matches
            for (j = 0; j < diseases.size(); j++) {

                diseaseName = diseases[j]->getName();

                // if disease found update the disease parameters
                if (diseaseOnListName == diseaseName) {

                    success = true;
                    patientDiseaseName = diseaseName;
                    //mort = diseases[j]->getMortality();
                    incid = diseases[j]->getIncidence();
                    //trans = diseases[j]->getTransmission();
                    priority = diseases[j]->getPriority();
                    perDrugResistant = diseases[j]->getperDrugResistant();
                    perLostSamples = diseases[j]->getPerLostSamples();
                    totNoInfected = int(incid * totalPopulation);
                    std::cout << "inside createPatients diseaseName = " << diseaseName << ", incid = " << incid << " \n";

                }
            }

            // if found disease create a certain number of Patients that show the symptoms - infected and non-infected
            if (success) {

                // create Patients based off of cachment size, probablilty bins, disease incidence, transmission &&&&&
                // incidence is per 1000 people per year
                noInfected = int(pop * incid * 3);
                std::cout << "inside createPatients totNoInfected = " << totNoInfected << ", noIfected = " << noInfected << " \n";

                noNotInfected = noInfected * 2;

                // get random number between 0 and 1
                //unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
                //std::mt19937 generator(seed);

                //std::uniform_int_distribution<int> distribution(1, 100);

                if ((noNotInfected + noInfected) > 0) {
                    for (l = 1; l <= noInfected + noNotInfected; l++) {

                        // generate a unique distance for each patient
                            //randInt = distribution(generator);
                        randInt = getNextRandomNo100();

                        rand = (float)randInt / 100;
                        // std::cout << "rand " << randInt << "," << rand << "\n";
                        
                        // calculate distance from facility
                        k = -1;
                        do {
                            k++;
                            // std::cout << "popbins " << k << "," << popBins[k] << "\n";
                        } while (rand > popBins[k] && k < 9);

                        distance = (float)(k + 1) * binInc;
                        // std::cout << "patient distance " << distance <<  "\n";
                         
                        // create the patient
                        if (l <= noInfected) {

                            // loop through the tests for this disease.  going from highest to lowest priority, if testing is here, do it here 
                            // so keep patient, otherwise send them home and sample sent to closest facility that has highest priority testing
                            testingFacility = getTestingFacility(patientDiseaseName);;
                            testName = "Xpert";

                            if (_facility == testingFacility) {
                                p = new Patient(fName, district, distIndex, region, regIndex, patientDiseaseName, distance, k, true, PatInUnTest, testName, timestep);
                            }
                            else {
                                p = new Patient(fName, district, distIndex, region, regIndex, patientDiseaseName, distance, k, true, PatAwayUnTest, testName, timestep);
                            }

                            // find out if this strain is or will become drug resistant - disease dependent
                            //randInt = distribution(generator);
                            randInt = getNextRandomNo100();

                            if (randInt < perDrugResistant) {
                                p->setDrugResistant(true);
                            }
                            else {
                                p->setDrugResistant(false);
                            }

                            // add patient to patient container
                            _patientContainer.push_back(p);

                            // std::cout << "create patient " << _facility << ", " << _testingFacility << "\n";

                            // create samples for each of the Patients
                            s = new Sample(p, _facility, testingFacility, testingFacilityDistance, patientDiseaseName, true, priority, perLostSamples);
                            // std::cout << "after new sample " << "\n";

                            // update Sample status to in-transit - calculate transit time by distance to travel for testing
                            transitDays = (int)ceil(testingFacilityDistance / distancePerTimeStep);
                            s->setTransitTime(transitDays);
                            s->setStatus(SampUnTestInTransit);
                            // std::cout << "transit time " << transitDays << "\n";

                            // now add Sample to _testingFacility _sampleContainer
                            f = facilitiesList->getPtr(testingFacility);
                            // std::cout << "after getptr " << "\n";

                            if (f) {
                                f->addToSampleContainer(s);
                                // std::cout << "after add to sample " << "\n";
                            }
                            else {
                                std::cout << "couldn't find testing Facilty for new Sample " << _facility << ", " << testingFacility << "\n";
                            }

                        }
                        else {
                            // create patients who aren't infected
                            testName = "Xpert";

                            if (_facility == testingFacility) {
                                p = new Patient(fName, district, distIndex, region, regIndex, patientDiseaseName, distance, k, false, PatInUnTest, testName, timestep);
                            }
                            else {
                                p = new Patient(fName, district, distIndex, region, regIndex, patientDiseaseName, distance, k, false, PatAwayUnTest, testName, timestep);
                            }

                            // no drug-resistance for non-infected patients
                            p->setDrugResistant(false);

                            // add patient to patient container
                            _patientContainer.push_back(p);

                            // create samples for each of the Patients
                            s = new Sample(p, _facility, testingFacility, testingFacilityDistance, patientDiseaseName, false, priority, perLostSamples);
                            // std::cout << "after new sample " << "\n";


                            // update Sample status to in-transit - calculate transit time by distance to travel for testing
                            transitDays = (int)ceil(testingFacilityDistance / distancePerTimeStep);
                            s->setTransitTime(transitDays);
                            s->setStatus(SampUnTestInTransit);
                            // std::cout << "transit time " << transitDays << "\n";

                            // now add Sample to _testingFacility _sampleContainer
                            f = facilitiesList->getPtr(testingFacility);
                            // std::cout << "after getptr " << "\n";

                            if (f) {
                                f->addToSampleContainer(s);
                                // std::cout << "after add to sample " << "\n";
                            }
                            else {
                                std::cout << "couldn't find testing Facilty for new Sample " << _facility << ", " << testingFacility << "\n";
                            }
                        }
                    }
                }
            }
        }
        // std::cout << "no. of patients created for " << _facility << ", no. = " << _patientContainer.size() << "\n";
    }

}






// update Patients
void Diagnosis::updatePatients(Testing* test, vector<Disease*> diseases, vector<Test*> testList, int timeStep) {

    //int i;
    std::size_t i;
    int timesteps;
    int timeStepsLeftInRun;
    int randInt;
    int calcTimestep;
    int onsetTilTreat;
    int countPH = 0;
    int countNI = 0;
    int extraTreatTime = 0;
    int distIndex = 0;
    int regIndex = 0;
    int noTreatDeathTime;
    int noMonthsRepresent = 0;
    int noDaysRepresentOrig = 0;
    int returnTime = 0;
    int noTransPatients = 0;
    //int testCount = 0;
    float sensitivity;
    float sensitivityWithTestMD;
    float specificity;
    float specificityWithTestMD;
    float diagnosisPer;
    float untreatedDeathPer = 0;
    float transmission = 0;
    float transThreshold = 0;
    float inter;
    float randFloat;
    bool result;
    bool MDClinicDiagFlag = true;
    bool falseNegFlag = false;
    bool transFlag = false;
    bool debug = false;
    bool debug3 = false;
    string testName;
    string diseaseName;
    string facilityName;
    string testingFacilityName;
    Disease* d = NULL;
    Facility* f;
    Test* t;
    Testing* testing;
    vector<Sample*> s;
    Sample* samplePtr;
    Stats* stats = NULL;
    Stats* facilityStats = NULL;

    //int treatedPatients{};    // percent
    int treatmentTime{};        // timesteps
    int relapseTime{};          // timesteps
    float initTreatSuccess{};   // percent
    float initTreatDeath{};     // percent
    float initTreatLtoF{};      // percent
    //float initTreatLtoFRegion{};// percent
    float treatLtoF{};          // percent
    float relapseSuccess{};     // percent
    float noTreatDeathPer{};    // percent
    //int noTreatRepreTime{};     // timesteps
    //float noTreatReprePer{};    // percent
    //int treatRepreTime{};       // timesteps
    //float treatReprePer{};      // percent
    //int probPatientReturns{}; // percent

    // how many tests
    //testCount = testList.size();

    if (debug) std::cout << "inside update patient-diagnosis " << "\n";

    // figure out how much time is left in run, to determine if inactive patients die before end of run
    timeStepsLeftInRun = noSteps - timeStep;
    
    // get random numbers between 0 and 1
    //unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    //std::mt19937 generator(seed);

    //std::uniform_int_distribution<int> distribution(1, 100);
    //std::uniform_int_distribution<int> distribution2(1, 10000);

    // go through the Patient list and update
    for (auto patient : _patientContainer) {

        if (debug) std::cout << "inside update patient status = " << patient->getStatus() << ", disease =  " << patient->getDisease() << "\n";


        // get disease asssociated with Patient
        // get attributes for this disease
        for (auto disease : diseases) {
            if (disease->getName() == patient->getDisease()) {

                // extract disease attributes and use them to update the total stats for model
                //treatedPatients = disease->getTreatedPercentage();
                treatmentTime = disease->getTreatmentTime();
                relapseTime = disease->getRelapseTime();
                initTreatSuccess = disease->getInitialTreatmentSuccessRate();
                initTreatDeath = disease->getInitialTreatmentDeathRate();
                //initTreatLtoF = disease->getPreTreatmentLtoFTier(patient->getFacilityTier());
                initTreatLtoF = disease->getPreTreatmentLtoFTierRegion(patient->getFacilityRegion(), patient->getFacilityTier());
                //treatLtoF = disease->getTreatmentLtoFTier(patient->getFacilityTier());
                treatLtoF = disease->getTreatmentLtoFTierRegion(patient->getFacilityRegion(), patient->getFacilityTier());
                relapseSuccess = disease->getRelapseSuccessRate();
                noTreatDeathPer = disease->getPerUntreatedDie();
                transFlag = disease->getTransFlag();
                transmission = disease->getTransmission();
                transThreshold = disease->getTransThreshold();
                //noTreatRepreTime = disease->getTimeUntreatedRepresentation();
                //noTreatReprePer = disease->getPerUntreatedRepresentation();
                //treatRepreTime = disease->getTimeTreatedRepresentation();
                //treatReprePer = disease->getPerTreatedRepresentation();
                stats = disease->getStatsPtr();
                facilityStats = disease->getFacStatsPtr();
                //probPatientReturns = disease->getProbReturn();
                d = disease;
            }
        }

        if (debug) {

            std::cout << "inside update patient initTreatSuccess = " << initTreatSuccess << ", initTreatDeath =  " << initTreatDeath << ", initTreatLtoF =  " << initTreatLtoF;
            std::cout << ", treatLtoF = " << treatLtoF << ", relapseSuccess =  " << relapseSuccess << ", noTreatDeathPer =  " << noTreatDeathPer <<", transmission =  " << transmission << "\n";
            std::cout << "inside update patient initTreatLtoF = " << initTreatLtoF << ", initTreatLtoFRegion =  " << patient->getFacilityRegion() << "\n";
        }
       
        // get a new copy of sampleList pointer each time through
        vector<Sample*> sampleList = test->getSamplePtr();

        //std::cout << "inside update patient status2 = " << patient->getStatus() << ", disease =  " << patient->getDisease() << "\n";
        // Depending on status - update patient
        // PatAwayUnTest = 0;
        // PatInUnTest = 1;
        // PatInfTestTreat = 2;
        // PatInfTestRelapse = 3;
        // PatInfTestNoTreat = 4;
        // PatInfNoTestNoTreat = 5;
        // PatInfTestLtofNoTreat = 6;
        // PatDead = 7;
        // PatCured = 8;
        // PatUninfTest = 9;
        // PatUninfNoTest = 10;
        // PatUninfTestTreat = 11;

        //std::cout << "update patient seed  = " << seed <<  "\n";

        // find the region and district indices for stats counting
        distIndex = patient->getFacilityDistrictIndex();
        regIndex = patient->getFacilityRegionIndex();

        switch (patient->getStatus()) {
        case 0: // PatAwayUnTest
            // away from facility waiting for Sample results call

            // increase Patient time from onset
            patient->incrementAllTime();

            break;
        case 1: // PatInUnTest
            // in-house - waiting for Sample results

            // update no. of times they have been here
            patient->incrementNoReturnsForResults();
            
            if ((false) && (patient->getNoReturnsForResults() > 400)) {

                samplePtr = patient->getSamplePtr();
                std::cout << "patient inhouse sample = " << samplePtr << " pptr " << samplePtr->getPatientPtr() << " ptr " << patient << " stat " << patient->getSampleTestStatus() << " size " << sampleList.size() <<  "\n";

            }
            
            //for (auto sample : sampleList) {
            for (i = 0; i < sampleList.size(); i++) {

                if ((sampleList[i]->getPatientPtr() == patient) && (patient->getSampleTestStatus() == "SampReturnedAvail")) {
                    
                    if (debug) std::cout << "patient inhouse found patient sample status = " << sampleList[i]->getStatus() << " test = " << sampleList[i]->getTest() << ", pos =  " << sampleList[i]->isPositive() << ", infe = " << sampleList[i]->getInfected() << "\n";
                    //if (sampleList[i]->isPositive() && !sampleList[i]->getInfected()) std::cout << "patient sample infect = " << sampleList[i]->getInfected() << " samp pos = " << sampleList[i]->isPositive() << ", pat inf =  " << patient->getInfected() << ", pat pos = " << patient->getTestPos() << "\n";
                    
                    // store test used with sample with patient
                    patient->setSampleTest(sampleList[i]->getTest());

                    // include doctors sensitivity and specificity values as well
                    // Infected -  two results from sensitivity for each test
                    // Infected -               true positive - treat
                    // Infected -               false negative - (Snmd - Snt)/(1-Snt)
                    // Uninfected - two results from specificity for each test
                    // Uninfected -             false positive - treatment
                    // Uninfected -             true negative - (Spt - Spmd)/Sp
                        
                    // where Snt - sensitivity for test, Snmd - sensitivity for Doctor's diagnosis
                    //       Spt - specificity for test, Spmd - specificity for Doctor's diagnosis
                    
                    // mark time test results are returned
                    sampleList[i]->setResultsReturnedTime(timeStep);
                    patient->setResultsReturnedTime(timeStep);
                    patient->setPatientRecievedResultTime(timeStep);
                    sampleList[i]->setResultsRecievedtoOrigFacilityTime(timeStep);

                    if (debug) std::cout << "patient in-house set time "  << "\n";
                    if (statsCount) {

                        if (patient->getInfected()) {

                            stats->record_timePresentToSampleDiagIn(timeStep - patient->getTimestepPresented(), distIndex, regIndex);
                            facilityStats->record_timePresentToSampleDiagIn(timeStep - patient->getTimestepPresented(), patient->getFacilityID());
                            stats->record_nPresentToSampleDiagIn(distIndex, regIndex);
                            facilityStats->record_nPresentToSampleDiagIn(patient->getFacilityID());
                        }
                    }

                    t = sampleList[i]->getTestPtr();

                    if (debug) std::cout << "patient in-house get test pointer "  << "\n";

                    sensitivity = t->getSense();
                    specificity = t->getSpecif();

                    if (debug) std::cout << "patient in-house sensititivity " << sensitivity << ", " << specificity << "\n";

                    sensitivityWithTestMD = t->getSenseWithTestMD();
                    specificityWithTestMD = t->getSpecifWithTestMD();

                    //debug - true
                    if (debug) std::cout << "patient in-house test name = " << t->getTestName() << ", sense = " << sensitivity << ", senseMD = " << sensitivityWithTestMD << " ,specif = " << specificity << ", specifMD = " << specificityWithTestMD << "\n";

                    // update patient test result information
                    patient->setTestPos(sampleList[i]->getPositive());

                    // only set when doctor overrides false-neg
                    patient->setMDPos(false);

                    if (sampleList[i]->isPositive()) {

                        // record patients who should start treatment
                        if (statsCount) {

                            stats->record_sPatientInTreatment(distIndex, regIndex);
                            facilityStats->record_sPatientInTreatment(patient->getFacilityID());
                        }
                        
                        if (debug) std::cout << "test positve infected = " << patient->getInfected() << "\n";

                        // test result - true 
                        if (sampleList[i]->getInfected()) {

                            // infected - true positive send to treatment
                            patient->setStatus(PatInfTestTreat);
                                
                            // update patient time from onset to treatment time
                            //patient->setOnsetTilTreatment(patient->getTimeFromOnset());
                            patient->setStartedTreatmentTime(timeStep);
                            onsetTilTreat = timeStep - patient->getTimestepCreated();
                            patient->setOnsetTilTreatment(onsetTilTreat); 
                            patient->setTreated();

                            if (debug) std::cout << "patient going to treatment onsetTilTreat = " << onsetTilTreat << ", time step = " << timeStep << ", create time = " << patient->getTimestepCreated() << "\n";

                            // update patient time from onset to treatment time
                            //patient->setOnsetTilTreatment(patient->getTimeFromOnset());
                        }
                        else {

                            // treat all postive results as infected - even patients that are uninfected
                            patient->setStatus(PatUninfTestTreat);
                        }
                        countPH++;
                    }
                    else {      // test result is negative

                        // - MD override everywhere for now - in future - only use MD overrides on patients that present to regional hospitals
                        /*switch (MDClinicDiagOption) {
                        
                            case 1:
                            
                                if ((sampleList[i]->getTestingDistance() == 0) && ( facilitiesList->getPtr(patient->getSampleTestFacility())->getTier() == 3)) MDClinicDiagFlag = true;
                                break;
                            case 2:
                            
                                if ((sampleList[i]->getTestingDistance() == 0) && ( facilitiesList->getPtr(patient->getSampleTestFacility())->getTier() <= 4)) MDClinicDiagFlag = true;
                                break; 
                            default:

                                MDClinicDiagFlag = true;
                        }*/

                        // if MD Clinical diagnosis sensitivity for test and disease is 0 and specificity is 1 then assume no MD Clinical diagnosis
                        if ((sensitivityWithTestMD == 0) && (specificityWithTestMD == 1)) {

                            MDClinicDiagFlag = false;
                        }

                        if (MDClinicDiagFlag) {
                            //std::cout << "patient testing distance = " << sampleList[i]->getTestingDistance() << ", facility = " << patient->getSampleTestFacility() << ", tier = " <<  facilitiesList->getPtr(patient->getSampleTestFacility())->getTier() << "\n";

                            // test result - negative
                            if (sampleList[i]->getInfected()) {

                                // infected - false negative - send some to treatment anyway
                                //inter = (float)(sensitivityWithTestMD - sensitivity) / float (100 - sensitivity);
                                inter = (float)(sensitivityWithTestMD);
                                diagnosisPer = inter * 100;

                                randInt = getNextRandomNo100()-1;

                                randFloat = float(randInt);

                                randInt = getNextRandomNo100()-1;

                                randFloat = randFloat + float(randInt)/100;
                                if (randFloat > 100) randFloat = 100;

                                if (debug) std::cout << "false negative test = " << t->getTestName() << ", randFloat = " << randFloat << ", diagnosisPer = " << diagnosisPer << ", inter = " << inter << "\n";
                                //std::cout << "false neg rand = " << randFloat << ", dignosis prob = " << diagnosisPer <<  "\n";

                                if (randFloat <= diagnosisPer) {

                                    // treat patient anyway
                                    patient->setStatus(PatInfTestTreat);

                                    // update patient time from onset to treatment time
                                    //patient->setOnsetTilTreatment(patient->getTimeFromOnset());
                                    patient->setStartedTreatmentTime(timeStep);
                                    onsetTilTreat = timeStep - patient->getTimestepCreated();
                                    patient->setOnsetTilTreatment(onsetTilTreat); 
                                    patient->setTreated();

                                    //debug
                                    if (debug) std::cout << "patient going to treatment onsetTilTreat = " << onsetTilTreat << ", time step = " << timeStep << ", create time = " << patient->getTimestepCreated() << "\n";

                                    // update patient test result information
                                    patient->setMDPos(true);
                                    patient->setPatientRecievedMDResultTime(timeStep);

                                    if (statsCount) {
                                        
                                        //stats->reduce_nTestFalseNegIn(distIndex, regIndex);

                                        // record patients who should start treatment
                                        stats->record_sPatientInTreatment(distIndex, regIndex);
                                        facilityStats->record_sPatientInTreatment(patient->getFacilityID());

                                        stats->record_nMDCDInfected(distIndex, regIndex);
                                        facilityStats->record_nMDCDInfected(distIndex, regIndex);
                                        
                                        if (sampleList[i]->getOrigFacility() == sampleList[i]->getTestingFacility()) {
                                        
                                            stats->record_nDiagnosisIn(distIndex, regIndex);
                                            facilityStats->record_nDiagnosisIn(patient->getFacilityID());
                                            stats->record_nPostTestDiagnosisTruePosIn(distIndex, regIndex);
                                            facilityStats->record_nPostTestDiagnosisTruePosIn(patient->getFacilityID());
                                            //std::cout << "nDiagnosisIn for diagnosisTruePos = " <<  "\n";

                                        }
                                        else {

                                            stats->record_nDiagnosisOut(distIndex, regIndex);
                                            facilityStats->record_nDiagnosisOut(patient->getFacilityID());
                                            stats->record_nPostTestDiagnosisTruePosOut(distIndex, regIndex);
                                            facilityStats->record_nPostTestDiagnosisTruePosOut(patient->getFacilityID());
                                            //std::cout << "nDiagnosisOut for diagnosisTruePos " << "\n";
                                        }
                                    }
                                }
                                else {

                                    // stays false negative 
                                    patient->setStatus(PatInfTestNoTreat);
                                    patient->setReasonNoTreat("PatInfTestNoTreat");

                                    // track LtoF time
                                    patient->seTimePatientLtoF(timeStep);

                                    if (debug) std::cout << "false negative untreated patient PatInfTestNoTreat " << "\n";

                                    if (statsCount) {

                                        stats->record_nPostTestDiagnosisFalseNeg(distIndex, regIndex);
                                        facilityStats->record_nPostTestDiagnosisFalseNeg(patient->getFacilityID());
                                        
                                        stats->record_nPreTreatLtFU(distIndex, regIndex);
                                        facilityStats->record_nPreTreatLtFU(patient->getFacilityID());
                                        stats->record_nPreTreatLtFUFN(distIndex, regIndex);
                                        facilityStats->record_nPreTreatLtFUFN(patient->getFacilityID());
                                    }
                                }
                            }
                            else {

                                // non-infected - true negative - send some to treatment anyway based on doctor's diagnosis
                                //inter = (float)(specificity - specificityWithTestMD) / float (specificity);
                                inter = (float)(specificityWithTestMD);
                                diagnosisPer = inter * 100;

                                randInt = getNextRandomNo100()-1;

                                randFloat = float(randInt);

                                randInt = getNextRandomNo100()-1;

                                randFloat = randFloat + float(randInt)/100;
                                if (randFloat > 100) randFloat = 100;

                                if (debug) std::cout << "true negative test = " << t->getTestName() << ", randint = " << randFloat << ", diagnosisPer = " << diagnosisPer  << ", inter = " << inter << "\n";
                                //std::cout << "true neg rand = " << randFloat << ", dignosis prob = " << diagnosisPer <<  "\n";

                                if ((randFloat >= diagnosisPer) && (diagnosisPer > 0)) {

                                    // treat uninfected patient anyway
                                    patient->setStatus(PatUninfTestTreat);

                                    // update patient test result information
                                    patient->setMDPos(true);
                                    patient->setPatientRecievedMDResultTime(timeStep);

                                    patient->setStartedTreatmentTime(timeStep);
                                    patient->setTreated();

                                    patient->setStartedTreatmentTime(timeStep);

                                    if (statsCount) {
                                        
                                        // record patients who should start treatment
                                        stats->record_sPatientInTreatment(distIndex, regIndex);
                                        facilityStats->record_sPatientInTreatment(patient->getFacilityID());
      
                                        stats->record_nMDCDUninfected(distIndex, regIndex);
                                        facilityStats->record_nMDCDUninfected(patient->getFacilityID());

                                        if (sampleList[i]->getOrigFacility() == sampleList[i]->getTestingFacility()) {
                                        
                                            stats->record_nDiagnosisIn(distIndex, regIndex);
                                            facilityStats->record_nDiagnosisIn(patient->getFacilityID());
                                            stats->record_nPostTestDiagnosisFalsePosIn(distIndex, regIndex);
                                            facilityStats->record_nPostTestDiagnosisFalsePosIn(patient->getFacilityID());
                                            //std::cout << "nDiagnosisIn for diagnosisFalsePos " << "\n";
                                        }
                                        else {

                                            stats->record_nDiagnosisOut(distIndex, regIndex);
                                            facilityStats->record_nDiagnosisOut(patient->getFacilityID());
                                            stats->record_nPostTestDiagnosisFalsePosOut(distIndex, regIndex);
                                            facilityStats->record_nPostTestDiagnosisFalsePosOut(patient->getFacilityID());
                                            //std::cout << "nDiagnosisOut for diagnosisFalsePos " << "\n";
                                        }
                                    }
                                }
                                else {

                                    patient->setStatus(PatUninfTest);

                                    // make sure patient doesn't die of old age after during run
                                    willPatientDie(patient, timeStepsLeftInRun);
                                
                                    // update Patient to inactive patient container
                                    movePatientToInactive(patient, PatUninfTest, "NotInfectedTested", stats, facilityStats);

                                    patient->setStartedTreatmentTime(timeStep);
                                    patient->setTimestepInactive(timeStep);

                                    if (statsCount) {
                                        
                                        stats->record_nPostTestDiagnosisTrueNeg(distIndex, regIndex);
                                        facilityStats->record_nPostTestDiagnosisTrueNeg(patient->getFacilityID());
                                    }
                                }
                            }
                            countNI++;
                        }
                        else {
                            
                            // test result - negative
                            if (sampleList[i]->getInfected()) {

                                patient->setStatus(PatInfTestNoTreat);
                                patient->setReasonNoTreat("PatInfTestNoTreat");

                                //debug
                                if (debug) std::cout << "false negative untreated patient PatInfTestNoTreat " <<  "\n";

                                if (statsCount) {

                                    stats->record_nPreTreatLtFU(distIndex, regIndex);
                                    facilityStats->record_nPreTreatLtFU(patient->getFacilityID());
                                    stats->record_nPreTreatLtFUFN(distIndex, regIndex);
                                    facilityStats->record_nPreTreatLtFUFN(patient->getFacilityID());
                                    //stats->reduce_nTestFalseNegIn(distIndex, regIndex);
                                }
                            }
                            else {

                                // non-infected - true-negative -
                                patient->setStatus(PatUninfTest);

                                // make sure patient doesn't die of old age after during run
                                willPatientDie(patient, timeStepsLeftInRun);
                                
                                // update Patient to inactive patient container
                                movePatientToInactive(patient, PatUninfTest, "NotInfectedTested", stats, facilityStats);

                                patient->setStartedTreatmentTime(timeStep);
                                patient->setTimestepInactive(timeStep);
                            }
                        }
                    }

                    //std::cout << "before remove Sample and delete size = " << sampleList.size() << "\n";

                    // remove patient's sample from sample list at testing facility, put it in inactive sample list
                    if ((patient->getStatus() == PatUninfTest) || (patient->getStatus() == PatUninfTestTreat)) {

                        result = test->removePatientSampleFromList(patient, SampReturnedAvail);

                        if (!result) {

                            if (debug) std::cout << "before get testing facility, testing  =  " << patient->getDisease() << ", test = " << patient->getSampleTest() << "\n";
                            // find the closest testing facilityj
                            testingFacilityName = getTestingFacility(patient->getDisease(), patient->getSampleTest());

                            if (debug) std::cout << "testing facility =  " << testingFacilityName << "\n";

                            // decode string into pointer
                            f = facilitiesList->getPtr(testingFacilityName);

                            // get testing pointer there
                            testing = f->getTestingPtr();

                            // remove patient's sample from sample list at testing facility, put it in inactive sample list
                            result = testing->removePatientSampleFromList(patient, SampReturnedAvail);
                        }
                    }
                    break;
                }
                else if (sampleList[i]->getPatientPtr() == patient) {

                
                //if (patient->getSampleTestStatus() == "SampReturnedAvail")
                    //std::cout << "patient here where's sample = " << patient->getSampleTestStatus() << ", sample status = " << sampleList[i]->getStatus() << ", sample wait for resource time = " << sampleList[i]->getWaitForResource() << " patient test facility = " << patient->getSampleTestFacility()  << "\n";
                }
                else {
                    if (debug) std::cout << "patient inhouse ---error---- patient sample status = " << sampleList[i]->getStatus() << " test = " << sampleList[i]->getTest() << ", pos =  " << sampleList[i]->isPositive() << ", infe = " << sampleList[i]->getInfected() << "\n";

                }
            }

            // increase Patient age
            patient->incrementAllTime();

            break;
        case 2:
            // received results - undergoing treatment - only infected patients
            // make sure Patient is alive or just ignore
            if (patient->isAlive()) {

                timesteps = patient->getTimeStepsInTreatment();

                if (debug) std::cout << "in treatment time from onset to treatment = " << patient->getOnsetTilTreatment() <<  "\n";

                // For Ghana, treatment success rate decreased slightly from 83.1 % during the first year to 80.2 % in 2017, 
                // with an overall treatment success rate of 82.5 % (95 % CI: 81.3–83.8 %).Treatment failure, death, and 
                //lost to follow up rates were 0.8 % (range 0.5–1.2 %), 13.5 % (range 12.4–14.7 %), and 3.1 % (range 2.6–3.8 %) respectively.
                // treatment lasts 8-12 months, check at 2 months
                // https://ghrp.biomedcentral.com/articles/10.1186/s41256-019-0128-9

                // Do all calculations when assigned to treatment
                // as a first pass, I might expect deaths to occur uniformly in the first 2 months (because it's mostly due 
                // to people being sick when they first present), whereas loss to follow-up would occur uniformly over 
                // the entire treatment period. (There's a systematic review on this, and it's a reasonable assumption.) 
                // I think it's safe to use 6 months as the treatment period - and for those who fail, they are immediately sick again. 
                // Those who relapse, maybe within a year? - Dr Dowdy


                // get random number between 0 and 100
                randInt = getNextRandomNo100()-1;
                
                randFloat = float(randInt);

                randInt = getNextRandomNo100()-1;

                randFloat = randFloat + float(randInt)/100;
                if (randFloat > 100) randFloat = 100;

                if (false) std::cout << "treated patient randFloat = " << randFloat << " randInt = " << randInt << " initTreatLtoF = " << initTreatLtoF << "\n";
                                    
                // remove those that actually start treatment or decided not to start treatment
                if (statsCount) {

                    stats->reduce_sPatientInTreatment(distIndex, regIndex);
                    facilityStats->reduce_sPatientInTreatment(patient->getFacilityID());
                }

                if (patient->getTracked()) std::cout << "ltof patient ready to undergo treatment " << " \n";

                if (patient->getInfected()) {

                    samplePtr = patient->getSamplePtr();
                    if ((!samplePtr->isPositive()) && (!patient->getMDPos()))std::cout << "treated patient infected test neg " << ", md override - " << patient->getMDPos() << " \n";
                    
                    // loss to follow up before treatment starts - falls into category of infected patient received results, no-treatment
                    // patient either re-presents or dies
                    if (randFloat < initTreatLtoF) {

                        if (patient->getTracked()) std::cout << "ltof patient pre treatment ltof " << " \n";

                        //std::cout << "sample returns but patient never shows up for results - lost to follow up "  << "\n";

                        patient->setStatus(PatInfTestNoTreat);

                        patient->setReasonNoTreat("PatInfTestNoTreat");
                    
                        // track LtoF time
                        patient->seTimePatientLtoF(timeStep);

                        // count pre-treatment loss to follow up patients 
                        if (statsCount) {

                            stats->record_nPreTreatLtFU(distIndex, regIndex);
                            facilityStats->record_nPreTreatLtFU(patient->getFacilityID());
                            stats->record_nPreTreatLtFUInf(distIndex, regIndex);
                            facilityStats->record_nPreTreatLtFUInf(patient->getFacilityID());
                        }
                    }
                    else {

                        // four treatment outcomes - cured, died, ltof, relapse
                        
                        // get random number between 0 and 100 for treatment results
                        randInt = getNextRandomNo100()-1;

                        randFloat = float(randInt);

                        randInt = getNextRandomNo100()-1;

                        randFloat = randFloat + float(randInt)/100;
                        if (randFloat > 100) randFloat = 100;

                        if (false) std::cout << "treated intected patient randFloat = " << randFloat << " treatLtoF = " << treatLtoF << " initTreatSuccess = " << initTreatSuccess << " initTreatDeath = " << initTreatDeath << "\n";
                        
                        // infected patient - get time until treatment starts - currently putting on hold
                        //extraTreatTime = getTestTreatmemtStart(patient->getDisease(), patient->getSampleTest());
                        //extraTreatTime = 0;
                
                        if (statsCount) {
                            
                            stats->record_nPatientsTreated(distIndex, regIndex);
                            facilityStats->record_nPatientsTreated(patient->getFacilityID());
                            stats->record_nPatientsTreatedInf(distIndex, regIndex);
                            facilityStats->record_nPatientsTreatedInf(patient->getFacilityID());

                            if (patient->getFacility() == patient->getSampleTestFacility()) {

                                stats->record_timePresentToSampleTreatIn(timeStep - patient->getTimestepPresented(), distIndex, regIndex);
                                facilityStats->record_timePresentToSampleTreatIn(timeStep - patient->getTimestepPresented(), patient->getFacilityID());
                                stats->record_nPresentToSampleTreatIn(distIndex, regIndex);
                                facilityStats->record_nPresentToSampleTreatIn(patient->getFacilityID());
                            }
                            else {

                                stats->record_timePresentToSampleTreatOut(timeStep - patient->getTimestepPresented(), distIndex, regIndex);
                                facilityStats->record_timePresentToSampleTreatOut(timeStep - patient->getTimestepPresented(), patient->getFacilityID());
                                stats->record_nPresentToSampleTreatOut(distIndex, regIndex);
                                facilityStats->record_nPresentToSampleTreatOut(patient->getFacilityID());
                            }
                        
                        }
                    
                        //std::cout << "treated patient extra treat time = " << extraTreatTime << ", disease = " << patient->getDisease() << ", test = " << patient->getSampleTest() <<  "\n";
                        if (debug) std::cout << "randFloat = " << randFloat << " initTreatSuccess = " << initTreatSuccess << " initTreatDeath = " << initTreatDeath <<  "\n";

                        if (randFloat < treatLtoF) {

                            // patient - loss to followup - falls into category of infected patient received results was supposed to start treatment but never showed up
                            if (patient->getTracked()) std::cout << "ltof patient treatment ltof " << " \n";

                            //std::cout << "patient assigned treatment but never shows up - lost to follow up extraTreatTime = " << extraTreatTime << "\n";
                            
                            // calculate time steps in treatment before loss to follow up
                            randInt = getNextRandomNo100();

                            // within first 6 months - assuming 24 hour timestep
                            calcTimestep = randInt*treatmentTime/100;

                            patient->incrementTimeStepsInTreatment(calcTimestep);

                            patient->setStatus(PatInfTestLtofNoTreat);
                                
                            // track LtoF time
                            patient->seTimePatientLtoF(timeStep + calcTimestep);

                            patient->setReasonNoTreat("PatInfTestLtofTreat");

                            // mark here just in case we add a re-presentation or prob of death if statement
                            if (statsCount) {

                                stats->record_nTreatLtFU(distIndex, regIndex);
                                facilityStats->record_nTreatLtFU(patient->getFacilityID());
                                stats->record_nTreatLtFUInf(distIndex, regIndex);
                                facilityStats->record_nTreatLtFUInf(patient->getFacilityID());
                            }
                            
                            // if testing facility is different from patient presenting
                            // 
                            // need to move sample to inactive list
                            //result = test->removePatientSampleFromList(patient, SampPatDead);

                            //if (!result) {

                            //    // find the closest testing facilityj
                            //    testingFacilityName = getTestingFacility(patient->getDisease(), patient->getSampleTest());

                            //    // decode string into pointer
                            //    f = facilitiesList->getPtr(testingFacilityName);

                            //    // get testing pointer there
                            //    testing = f->getTestingPtr();

                            //    // remove patient's sample from sample list at testing facility
                            //    result = testing->removePatientSampleFromList(patient, SampPatDead);
                            //}
                        }
                        else if (randFloat <= initTreatSuccess + treatLtoF) {

                             // patient cured
                             patient->setStatus(PatCured);

                             if (patient->getTracked()) std::cout << "ltof patient treatment cured " << " \n";

                             // increase time from onset and time in treatment
                             patient->incrementTimeStepsInTreatment(treatmentTime); 
                     
                             // mark time infectiousnes ends - current timestep + treatment time
                             patient->incrementInfectEndTime(timeStep, extraTreatTime);

                             // update treatment start time
                             patient->setStartedTreatmentTime(timeStep + extraTreatTime);
                     
                             // make sure patient doesn't die of old age after during run
                             willPatientDie(patient, timeStepsLeftInRun);
                                
                             // update Patient to inactive patient container
                             movePatientToInactive(patient, PatCured, "InfectTreat", stats, facilityStats);

                             patient->setTimestepInactive(timeStep  + extraTreatTime + treatmentTime);

                             if (statsCount) {
                         
                                stats->record_nPatientsTreatedCured(distIndex, regIndex);
                                facilityStats->record_nPatientsTreatedCured(patient->getFacilityID());
                                stats->record_timePresentToExit(timeStep  + extraTreatTime + treatmentTime - patient->getTimestepPresented(), distIndex, regIndex);
                                facilityStats->record_timePresentToExit(timeStep + extraTreatTime + treatmentTime - patient->getTimestepPresented(), patient->getFacilityID());
                                stats->record_nPresentToExit(distIndex, regIndex);
                                facilityStats->record_nPresentToExit(patient->getFacilityID());
                                
                            }

                            // in either case need to remove sample so it doesn't return and sit around for a patient that isn't coming back
                            result = test->removePatientSampleFromList(patient, SampReturnedAvail);

                            if (!result) {

                                //std::cout << "before get testing facility, testing  =  " << patient->getDisease() << ", test = " << patient->getSampleTest() << "\n";
                                // find the closest testing facilityj
                                testingFacilityName = getTestingFacility(patient->getDisease(),patient->getSampleTest());

                                //std::cout << "testing facility =  " << testingFacilityName << "\n";

                                // decode string into pointer
                                f = facilitiesList->getPtr(testingFacilityName);

                                // get testing pointer there
                                testing = f->getTestingPtr();

                                // remove patient's sample from sample list at testing facility, put it in inactive sample list
                                result = testing->removePatientSampleFromList(patient, SampReturnedAvail);
                            } 
                        }
                        else if (randFloat <= (initTreatSuccess + initTreatDeath + treatLtoF)) {
                        
                            // patient died
                            if (patient->getTracked()) std::cout << "ltof patient treatment died " << " \n";

                            // check disease transmission for infected patients
                            if (transFlag) {

                                // check to see if patient has transmitted the disease to anyone else before coming in
                                noTransPatients = checkTransPatients(facilitiesList->getPtr(patient->getFacility()), patient->getDisease(), patient->getPopBinIndex(), transThreshold, extraTreatTime);

                                // debug3 = true
                                if (debug3) std::cout << "after checkTransPatients =  ltof patient treatment died" << "\n";
                                
                                createTransPatients(noTransPatients, facilitiesList, patient->getFacility(), diseases, patient->getDisease(), testList, patient->getPopBinIndex(), extraTreatTime);
                            }

                            patient->setDead();
                            patient->setStatus(PatDead);
                            patient->setReasonDied("TreatDeath");
                            if (debug3) std::cout << "patient is dead reason =  TreatDeath" << "\n";

                            // calculate time steps in treatment before death
                            randInt = getNextRandomNo100();

                            // update treatment start time
                            patient->setStartedTreatmentTime(timeStep + extraTreatTime);
                     
                            // update Patient to inactive patient container
                            movePatientToInactive(patient, PatDead, "8", stats, facilityStats);

                            // within first 2 months - assuming 24 hour timestep
                            calcTimestep = randInt*(treatmentTime/3)/100;
                            
                            patient->incrementTimeStepsInTreatment(calcTimestep);

                            patient->setTimestepInactive(timeStep + calcTimestep + extraTreatTime);

                            patient->setTimeOfDeath(timeStep + calcTimestep + extraTreatTime);
  
                            // mark time infectiousnes ends - current timestep + calcTimestep time
                            patient->incrementInfectEndTime(timeStep, calcTimestep + extraTreatTime);

                            if (statsCount) {

                                stats->record_timePresentToSampleDeathInTreat(timeStep + calcTimestep + extraTreatTime - patient->getTimestepPresented(), distIndex, regIndex);
                                facilityStats->record_timePresentToSampleDeathInTreat(timeStep + calcTimestep + extraTreatTime - patient->getTimestepPresented(), patient->getFacilityID());
                                stats->record_nPresentToSampleDeathInTreat(distIndex, regIndex);
                                facilityStats->record_nPresentToSampleDeathInTreat(patient->getFacilityID());
                                stats->record_nPatientsTreatedDied(distIndex, regIndex);
                                facilityStats->record_nPatientsTreatedDied(patient->getFacilityID());
                                stats->record_timePresentToSampleDeath(timeStep + calcTimestep - patient->getTimestepPresented(), distIndex, regIndex);
                                facilityStats->record_timePresentToSampleDeath(timeStep + calcTimestep - patient->getTimestepPresented(), patient->getFacilityID());
                                stats->record_nPresentToSampleDeath(distIndex, regIndex);
                                facilityStats->record_nPresentToSampleDeath(patient->getFacilityID());
                                stats->record_timePresentToExit(timeStep  + extraTreatTime + treatmentTime - patient->getTimestepPresented(), distIndex, regIndex);
                                facilityStats->record_timePresentToExit(timeStep + extraTreatTime + treatmentTime - patient->getTimestepPresented(), patient->getFacilityID());
                                stats->record_nPresentToExit(distIndex, regIndex);
                                facilityStats->record_nPresentToExit(patient->getFacilityID());
                                stats->record_nDeathTreat(distIndex, regIndex);
                                facilityStats->record_nDeathTreat(patient->getFacilityID());
                                
                            }
                        
                        
                            // need to move sample to inactive list
                            result = test->removePatientSampleFromList(patient, SampPatDead);
                        
                            //std::cout << "patient died result = " << result << ", facility = " << getFacility() << ", disease = " << patient->getDisease() << ", test = " << patient->getSampleTest() <<  "\n";

                            if (!result) {

                                // find the closest testing facilityj
                                testingFacilityName = getTestingFacility(patient->getDisease(), patient->getSampleTest());;

                                // decode string into pointer
                                f = facilitiesList->getPtr(testingFacilityName);

                                // get testing pointer there
                                testing = f->getTestingPtr();

                                // remove patient's sample from sample list at testing facility
                                result = testing->removePatientSampleFromList(patient, SampPatDead);
                            }
                        }
                        else {

                            // at end of treatment time - patient relapses
                            if (patient->getTracked()) std::cout << "ltof patient treatment relapse " << " \n";

                            //std::cout << "relapse " << "\n";
                            
                            // check disease transmission for infected patients
                            if (transFlag) {

                                // check to see if patient has transmitted the disease to anyone else before coming in
                                noTransPatients = checkTransPatients(facilitiesList->getPtr(patient->getFacility()), patient->getDisease(), patient->getPopBinIndex(), transThreshold, extraTreatTime);

                                // debug3 = true
                                if (debug3) std::cout << "after checkTransPatients =  ltof patient treatment relapse" << "\n";
                                
                                createTransPatients(noTransPatients, facilitiesList, patient->getFacility(), diseases, patient->getDisease(), testList, patient->getPopBinIndex(), extraTreatTime);
                            }

                            patient->setStatus(PatInfTestRelapse);

                            // increase time from onset and time in treatment
                            patient->incrementTimeStepsInTreatment(treatmentTime + extraTreatTime);

                            // update treatment start time
                            patient->setStartedTreatmentTime(timeStep + extraTreatTime);

                            // set time to start relapse treatment
                            patient->setRelapseFlag(true);

                            patient->setRelapseTime(timeStep + treatmentTime + extraTreatTime);
                         
                            if (statsCount) {

                                stats->record_nPatientsTreatedRelapse(distIndex, regIndex);
                                facilityStats->record_nPatientsTreatedRelapse(patient->getFacilityID());
                            }
                            
                        }
                        //else {
                            //patient->incrementTimeStepsInTreatment();
                        //}
                    }
                }
                else {

                    std::cout << "false positive and starting treatment - should never get here" << "\n";
                }
            }
            break;
        case 3:
            // received results - relapse (treatment failure) - relapse program for TB is 6 months
            // assuming 83% survive and are cured the rest die - need to check this number - now in disease.csv
            // make sure Patient is alive and active or just ignore
            if ((patient->isAlive()) && (patient->getActive())) {

                //timesteps = patient->getTimeStepsInTreatment();

                // At start of relapse check to see results
                if (timeStep > patient->getRelapseTime()) {

                    
                    // get random number between 0 and 100
                    randInt = getNextRandomNo100()-1;

                    randFloat = float(randInt);

                    randInt = getNextRandomNo100()-1;

                    randFloat = randFloat + float(randInt)/100;
                    if (randFloat > 100) randFloat = 100;

                    if (debug) std::cout << "Inside relapse " <<  " randInt = " << randInt << " relapseSuccess = " << relapseSuccess <<  "\n";

                    if (randFloat < relapseSuccess) {
                        // patient cured
                        patient->setStatus(PatCured);

                        // increase time from onset and time in treatment
                        patient->incrementTimeStepsInTreatment(relapseTime);

                        // make sure patient doesn't die of old age after during run
                         willPatientDie(patient, timeStepsLeftInRun);
                         
                         // update Patient to inactive patient container
                        movePatientToInactive(patient, PatCured, "2", stats, facilityStats);
                        
                        // mark time infectiousnes ends - current timestep + relapseTime time
                        patient->incrementInfectEndTime(timeStep, relapseTime);
                    
                        patient->setTimestepInactive(timeStep + relapseTime);

                        if (statsCount) {
                        
                            stats->record_nTreatedRelapseCured(distIndex, regIndex);
                            facilityStats->record_nTreatedRelapseCured(patient->getFacilityID());
                            stats->record_timePresentToExit(timeStep + relapseTime - patient->getTimestepPresented(), distIndex, regIndex);
                            facilityStats->record_timePresentToExit(timeStep + relapseTime - patient->getTimestepPresented(), patient->getFacilityID());
                            stats->record_nPresentToExit(distIndex, regIndex);
                            facilityStats->record_nPresentToExit(patient->getFacilityID());
                            
                        }
                        
                        // in either case need to remove sample so it doesn't return and sit around for a patient that isn't coming back
                        result = test->removePatientSampleFromList(patient, SampReturnedAvail);

                        if (!result) {

                            //std::cout << "before get testing facility, testing  =  " << patient->getDisease() << ", test = " << patient->getSampleTest() << "\n";
                            // find the closest testing facilityj
                            testingFacilityName =getTestingFacility(patient->getDisease(),patient->getSampleTest());

                            //std::cout << "testing facility =  " << testingFacilityName << "\n";

                            // decode string into pointer
                            f = facilitiesList->getPtr(testingFacilityName);

                            // get testing pointer there
                            testing = f->getTestingPtr();

                            // remove patient's sample from sample list at testing facility, put it in inactive sample list
                            result = testing->removePatientSampleFromList(patient, SampReturnedAvail);
                        } 
                        //std::cout << "relapse patient cured time " << relapseTime <<  "\n";
                    }
                    else {
                        // patient died
                        patient->setDead();
                        patient->setStatus(PatDead);
                        patient->setReasonDied("RelapseDeath");
                        if (debug3) std::cout << "patient is dead  reason =  RelapseDeath" << "\n";
                       
                        //std::cout << "relapse patient died time " << patient->getRelapseTime() <<  "\n";

                        // calculate time steps in treatment before death
                        randInt = getNextRandomNo100();

                        // within first 6 months - assuming 24 hour timestep
                        calcTimestep = randInt* (d->getTimeDeathRetreated()/30)/100;

                        //debug - true 
                        if (debug) std::cout << "relapse patient died timeStep =  " << timeStep << " calc timestep = " << calcTimestep << " relapse time = " << relapseTime << "\n";

                        patient->incrementTimeStepsInTreatment(calcTimestep);

                        // update Patient to inactive patient container
                        movePatientToInactive(patient, PatDead, "1", stats, facilityStats);
                        
                        // mark time infectiousnes ends - current timestep + calcTimestep time
                        patient->incrementInfectEndTime(timeStep, calcTimestep);
                        
                        patient->setTimestepInactive(timeStep + calcTimestep);

                        patient->setTimeOfDeath(timeStep + calcTimestep);

                        // check disease transmission for infected patients
                        if (transFlag) {

                            // check to see if patient has transmitted the disease to anyone else before coming in
                            noTransPatients = checkTransPatients(facilitiesList->getPtr(patient->getFacility()), patient->getDisease(), patient->getPopBinIndex(), transThreshold, calcTimestep);

                            // debug3 = true
                            if (debug3) std::cout << "after checkTransPatients =  relapse patient died" << "\n";
                            
                            createTransPatients(noTransPatients, facilitiesList, patient->getFacility(), diseases, patient->getDisease(), testList, patient->getPopBinIndex(), calcTimestep);
                        }

                        if (statsCount) {

                            stats->record_nTreatedRelapseDied(distIndex, regIndex);
                            facilityStats->record_nTreatedRelapseDied(patient->getFacilityID());
                            stats->record_timePresentToSampleDeathInRelapse(timeStep + calcTimestep - patient->getTimestepPresented(), distIndex, regIndex);
                            facilityStats->record_timePresentToSampleDeathInRelapse(timeStep + calcTimestep - patient->getTimestepPresented(), patient->getFacilityID());
                            stats->record_nPresentToSampleDeathInRelapse(distIndex, regIndex);
                            facilityStats->record_nPresentToSampleDeathInRelapse(patient->getFacilityID());
                            stats->record_timePresentToSampleDeath(timeStep + calcTimestep - patient->getTimestepPresented(), distIndex, regIndex);
                            facilityStats->record_timePresentToSampleDeath(timeStep + calcTimestep - patient->getTimestepPresented(), patient->getFacilityID());
                            stats->record_nPresentToSampleDeath(distIndex, regIndex);
                            facilityStats->record_nPresentToSampleDeath(patient->getFacilityID());
                            stats->record_timePresentToExit(timeStep + calcTimestep - patient->getTimestepPresented(), distIndex, regIndex);
                            facilityStats->record_timePresentToExit(timeStep + calcTimestep - patient->getTimestepPresented(), patient->getFacilityID());
                            stats->record_nPresentToExit(distIndex, regIndex);
                            facilityStats->record_nPresentToExit(patient->getFacilityID());
                            stats->record_nDeathReTreat(distIndex, regIndex);
                            facilityStats->record_nDeathReTreat(patient->getFacilityID());
                            
                        }
                        
                        // need to move sample to inactive list
                        result = test->removePatientSampleFromList(patient, SampPatDead);

                        if (!result) {

                            // find the closest testing facilityj
                            testingFacilityName = getTestingFacility(patient->getDisease(), patient->getSampleTest());

                            // decode string into pointer
                            f = facilitiesList->getPtr(testingFacilityName);

                            // get testing pointer there
                            testing = f->getTestingPtr();

                            // remove patient's sample from sample list at testing facility
                            result = testing->removePatientSampleFromList(patient, SampPatDead);
                        }
                    }
                }
            }
            break;
        case 4:  // PatInfTestNoTreat
            // no treatment - sample returned (false negative)

            // make sure Patient is alive or just ignore
            if (patient->isAlive()) {

                timesteps = patient->getTimeFromOnset();

                if (debug) std::cout << "untreated patient beginning reason =  " << patient->getReasonNoTreat() << " timesteps = " << timesteps << " status = " << patient->getStatus() << "\n";
                
                // Current models of untreated tuberculosis that assume a total duration of 2 years until self - cure or death 
                // underestimate the duration of disease by about one year, but their case fatality estimates of 70 % for smear - positive 
                //and 20 % for culture - positive smear - negative tuberculosis appear to be satisfactory
                // https://www.ncbi.nlm.nih.gov/pmc/articles/PMC3070694/
                // ignore this for now
                
                // make sure patient hasn'T died of old age
                if ((patient->getAge() + (timesteps / timestepsPerYear)) > patient->getLifeExpectancy()) {
                    // patient dead
                    patient->setDead();
                    patient->setStatus(PatDead);
                    patient->setReasonDied("OldAge");
                    if (debug3) std::cout << "patient is dead  reason =  OldAge" << "\n";

                    // update Patient to inactive patient container
                    movePatientToInactive(patient, PatDead, "2", stats, facilityStats);

                    // mark time infectiousnes ends - current timestep + calcTimestep time
                    patient->setInfectEndTime(timeStep + timesteps);
                        
                    patient->setTimestepInactive(timeStep);

                    patient->setTimeOfDeath(timeStep + timesteps);

                    // need to move sample to inactive list
                    result = test->removePatientSampleFromList(patient, SampPatDead);

                    if (!result) {

                        // find the closest testing facilityj
                        testingFacilityName = getTestingFacility(patient->getDisease(), patient->getSampleTest());

                        // decode string into pointer
                        f = facilitiesList->getPtr(testingFacilityName);

                        // get testing pointer there
                        testing = f->getTestingPtr();

                        // remove patient's sample from sample list at testing facility
                        result = testing->removePatientSampleFromList(patient, SampPatDead);
                    }

                    break;
                }
                                
                // only look at active patients
                if (patient->getActive()) {

                    // assume that patient isn't here as a result of a false negative diagnosis
                    falseNegFlag = false;
                    
                    samplePtr = patient->getSamplePtr();
                    if ((!samplePtr->getPositive()) && (!patient->getMDPos())) {

                        falseNegFlag = true;
                    }
                    
                    // some untreated patients will die, some will get better and some will re-present for new Diagnosis
                    // mark here just in case we add a re-presentation or prob of death if statement

                    // first calculate the geometric distribution of the patient representing.
                    // If this time is greater than that of the patient max.time.untreat.cure, patient will be cured and inactive
                    // If not, calculate the chance of the patient dying from TB before then (prob.death.represent). If so, mark the patient dead and inactive. 
                    // Otherwise, patient will represent.


                    // find out how many days before re-presenting 
                    // the geometric distribution represents a geometric distribution
                    noDaysRepresentOrig = getNextRandomGeoNo();
                    //noMonthsRepresent = (noDaysRepresentOrig * (d->getTimeUntreatedCure() / 30)) / 100;

                    // now calculate the probability of death
                    if (d->getName() == "TB") {

                        //// get death percentage from untreated TB surviability table
                        /*if (noMonthsRepresent > (d->getTimeUntreatedCure()/30)) {

                            untreatedDeathPer = d->getUntreatedMortality(d->getTimeUntreatedCure()/30) * 100;
                        }
                        else {

                            untreatedDeathPer = d->getUntreatedMortality(noMonthsRepresent) * 100;
                        }*/

                        // get the percentage of ltof patients that die before representing
                        untreatedDeathPer = d->getPerRepresentationLtoFDeath();
                    }
                    else {

                        untreatedDeathPer = d->getMortality();
                    }

                    // get random number between 0 and 100
                    randInt = getNextRandomNo100()-1;

                    randFloat = float(randInt);

                    randInt = getNextRandomNo100()-1;

                    randFloat = randFloat + float(randInt) / 100;
                    if (randFloat > 100) randFloat = 100;

                    //debug3 = true
                    if (debug3) {

                        std::cout << "patient ltof untreated randFloat = " << randFloat << ", untreated death per = " << untreatedDeathPer;
                        std::cout << ", no.days represent orig = " << noDaysRepresentOrig << " time untreated cure = " << d->getTimeUntreatedCure() << "\n";
                    }

                    // check to see if time to represent is longer than time to cure - if so, consider patient cured
                    if (noDaysRepresentOrig > d->getTimeUntreatedCure()) {

                        // untreated but not dead after untreated cure years

                        if (debug3) std::cout << "untreated patient lived reason =  " << patient->getReasonNoTreat() << " patient status = " << patient->getStatus() << "\n";

                        // make sure patient doesn't die of old age after during run
                        willPatientDie(patient, timeStepsLeftInRun);

                        // update Patient to inactive patient container
                        movePatientToInactive(patient, 99, "untreat-not-dead", stats, facilityStats);

                        // mark time infectiousnes ends - current timestep + time untreated cure
                        calcTimestep = d->getTimeUntreatedCure();

                        patient->setInfectEndTime(timeStep + calcTimestep);

                        patient->setTimestepInactive(timeStep);

                        // need to move sample to inactive list
                        result = test->removePatientSampleFromList(patient, SampPatuntreatedInactive);

                        if (!result) {

                            // find the closest testing facilityj
                            testingFacilityName = getTestingFacility(patient->getDisease(), patient->getSampleTest());

                            // decode string into pointer
                            f = facilitiesList->getPtr(testingFacilityName);

                            // get testing pointer there
                            testing = f->getTestingPtr();

                            // remove patient's sample from sample list at testing facility
                            result = testing->removePatientSampleFromList(patient, SampPatuntreatedInactive);
                        }

                        if (falseNegFlag) {

                            stats->record_nPreTreatLtFUFNCured(distIndex, regIndex);
                            facilityStats->record_nPreTreatLtFUFNCured(patient->getFacilityID());
                        }
                        else {

                            stats->record_nPreTreatLtFUCured(distIndex, regIndex);
                            facilityStats->record_nPreTreatLtFUCured(patient->getFacilityID());
                            
                        }
                    }
                    // check to see if patient has died before representing
                    else if (randFloat <= untreatedDeathPer) {

                        // check to see if patient dies from disease
                    
                        if (debug3) std::cout << "untreated patient noTreatTestTime reason =  " << patient->getReasonNoTreat() << " untreatedDeathPer = " << untreatedDeathPer << " randFloat = " << randFloat;
                        
                        // update counter for number dead
                        //debugCounter++;

                        // calculate the death date by finding the month closest to the initDeathPer 
                        //noMonthsRepresent = d->findUntreatedMortalityIndex(randFloat);
                        
                        //debug = true
                        if (debug) std::cout << "patient ltof died untreatedDeathPer = " << untreatedDeathPer << " time untreated represent = " << noDaysRepresentOrig << "\n";
                        
                        if (debug3) std::cout << " new noMonthsRepreset =  " << noMonthsRepresent << "\n";
                            
                        noTreatDeathTime = noDaysRepresentOrig;
                        
                        if (debug3) std::cout << "patient is dead  reason =  NoTreatDeath, time = " << noTreatDeathTime << "\n";

                        // increment how patient onset time
                        //patient->incrementOnset(noTreatTestTime);
                        patient->incrementOnset(noTreatDeathTime);

                        patient->setDead();
                        patient->setStatus(PatDead);
                        patient->setReasonNoTreat("PatInfTestNoTreat");
                        patient->setReasonDied("NoTreatDeath");
                        if (debug3) std::cout << "patient is dead  reason =  NoTreatDeath" << "\n";

                        // update Patient to inactive patient container
                        movePatientToInactive(patient, PatDead, "3", stats, facilityStats);

                        // check disease transmission for infected patients
                        if (transFlag) {

                            // check to see if patient has transmitted the disease to anyone else before coming in
                            noTransPatients = checkTransPatients(facilitiesList->getPtr(patient->getFacility()), patient->getDisease(), patient->getPopBinIndex(), transThreshold, noTreatDeathTime);

                            // debug3 = true
                            if (debug3) std::cout << "after checkTransPatients =  NoTreatDeath" << "\n";
                            
                            createTransPatients(noTransPatients, facilitiesList, patient->getFacility(), diseases, patient->getDisease(), testList, patient->getPopBinIndex(), noTreatDeathTime);
                        }

                        // get random number between 0 and 100
                        //randInt = getNextRandomNo100();

                        // within first 6 months - assuming 24 hour timestep
                        //calcTimestep = randInt*relapseTime/100;

                        // mark time infectiousnes ends - current timestep + calcTimestep time
                        patient->setInfectEndTime(timeStep + noTreatDeathTime);
                    
                        patient->setTimestepInactive(timeStep + noTreatDeathTime);

                        patient->setTimeOfDeath(timeStep + noTreatDeathTime);

                        // debug3 - true
                        if (debug3) std::cout << "untreated patient died reason =  " << patient->getReasonNoTreat() << " patient status = " << patient->getStatus() << " sample test = " << patient->getSampleTest() << "\n";
                        
                        if (statsCount) {

                            stats->record_timePresentToSampleDeath(timeStep + noTreatDeathTime - patient->getTimestepPresented(), distIndex, regIndex);
                            facilityStats->record_timePresentToSampleDeath(timeStep + noTreatDeathTime - patient->getTimestepPresented(), patient->getFacilityID());
                            stats->record_nPresentToSampleDeath(distIndex, regIndex);
                            facilityStats->record_nPresentToSampleDeath(patient->getFacilityID());
                            stats->record_timePresentToSampleDeathPreTreatLTFU(timeStep + noTreatDeathTime - patient->getTimestepPresented(), distIndex, regIndex);
                            facilityStats->record_timePresentToSampleDeathPreTreatLTFU(timeStep + noTreatDeathTime - patient->getTimestepPresented(), patient->getFacilityID());
                            stats->record_nPresentToSampleDeathPreTreatLTFU(distIndex, regIndex);
                            facilityStats->record_nPresentToSampleDeathPreTreatLTFU(patient->getFacilityID());
                            stats->record_timePresentToExit(timeStep + noTreatDeathTime - patient->getTimestepPresented(), distIndex, regIndex);
                            facilityStats->record_timePresentToExit(timeStep + noTreatDeathTime - patient->getTimestepPresented(), patient->getFacilityID());
                            stats->record_nPresentToExit(distIndex, regIndex);
                            facilityStats->record_nPresentToExit(patient->getFacilityID());
                            stats->record_nDeathUntreat(distIndex, regIndex);
                            facilityStats->record_nDeathUntreat(patient->getFacilityID());

                            if (falseNegFlag) {
                                
                                stats->record_nPreTreatLtFUFNDied(distIndex, regIndex);
                                facilityStats->record_nPreTreatLtFUFNDied(patient->getFacilityID());
                            }
                            else {

                                stats->record_nPreTreatLtFUDied(distIndex, regIndex);
                                facilityStats->record_nPreTreatLtFUDied(patient->getFacilityID());
                                
                            }
                        }
                        // debug3 - true
                        if (debug3) std::cout << "after checkTransPatients =  NoTreatDeath" << "\n";


                        // need to move sample to inactive list
                        result = test->removePatientSampleFromList(patient, SampPatDead);
                    }
                    // untreated patient didn't die, then it must represent
                    else {

                        // debug3 - true
                        if (debug3) std::cout << "patient represents - ltof cure max time = " << d->getTimeUntreatedCure() << " time untreated represent = " << noDaysRepresentOrig << "\n";
                        
                        // make sure return time is at least 7 timesteps
                        returnTime = noDaysRepresentOrig;
                        /*if (returnTime < 7) {

                            returnTime = 7;
                        }*/
                        // cap to end of run
                        if (returnTime > (noSteps - timeStep)) {

                            returnTime = (noSteps - timeStep) + 1;
                        }

                        // check disease transmission for infected patients
                        if (transFlag) {

                            // check to see if patient has transmitted the disease to anyone else before coming in
                            noTransPatients = checkTransPatients(facilitiesList->getPtr(patient->getFacility()), patient->getDisease(), patient->getPopBinIndex(), transThreshold, returnTime);

                            // debug3 = true
                            if (debug3) std::cout << "after checkTransPatients =  untreated patient represents" << "\n";
                            
                            createTransPatients(noTransPatients, facilitiesList, patient->getFacility(), diseases, patient->getDisease(), testList, patient->getPopBinIndex(), returnTime);
                        }

                        // increment how patient onset time
                        patient->incrementOnset(returnTime);

                        // keep track of patients that re-present
                        patient->setRepresentFlag(true);
                        patient->setRepresentTime(timeStep + returnTime);

                        // set patient away from facility return time
                        patient->setFacilityReturnTime(returnTime);

                        // update sample status to wait until representation time is over to test sample
                        samplePtr = patient->getSamplePtr();

                        samplePtr->setStatus(SampUnTestWaitRepresent);
                        patient->setSampleTestStatus("SampUnTestWaitRepresent");   

                        testingFacilityName = getTestingFacility(patient->getDisease(), patient->getSampleTest());
                        
                        // need to move sample to testing facility if different from facility where patient presents
                        if (patient->getFacility() != testingFacilityName) {

                            // change status to patient out but returning
                            patient->setStatus(PatAwayUnTest);
                            
                            // decode string into pointer
                            f = facilitiesList->getPtr(testingFacilityName);

                            // get testing pointer there
                            testing = f->getTestingPtr();
                            
                            // get the pointer to the sample object for patient
                            samplePtr = patient->getSamplePtr();
                            
                            // add it to the sample container at the testing facility
                            testing->addToSampleContainer(samplePtr);
                                
                            // get facility pointer to facility patient presented at
                            f = facilitiesList->getPtr(patient->getFacility());

                            // get testing pointer there
                            testing = f->getTestingPtr();
                            
                            // remove sample from sample container from facility where patient presented
                            testing->removeSampleFromList(samplePtr);
                        }
                        else {

                            // change status to patient waiting to return for in-house testing
                            patient->setStatus(PatInUnTest);
                        }
                       
                        // count as new additional samples 
                        if (statsCount) {

                            // keep track of loss to follow up patients that re-enter the system
                            stats->record_nPatientLtFUInf(distIndex, regIndex);
                            facilityStats->record_nPatientLtFUInf(patient->getFacilityID());


                            if (samplePtr->getOrigFacility() == samplePtr->getTestingFacility()) {

                                stats->record_nSamplesIn(distIndex, regIndex);
                                facilityStats->record_nSamplesIn(patient->getFacilityID());
                                stats->record_sPatientWaitingIn(distIndex, regIndex);
                                facilityStats->record_sPatientWaitingIn(patient->getFacilityID());
                            }
                            else {

                                stats->record_nSamplesOut(distIndex, regIndex);
                                facilityStats->record_nSamplesOut(patient->getFacilityID());
                                stats->record_timeSampleTransport(samplePtr->getTransitTimeSave(), distIndex, regIndex);
                                facilityStats->record_timeSampleTransport(samplePtr->getTransitTimeSave(), patient->getFacilityID());
                                stats->record_nTransported(distIndex, regIndex);
                                facilityStats->record_nTransported(patient->getFacilityID());
                                stats->record_sPatientWaitingOut(distIndex, regIndex);
                                facilityStats->record_sPatientWaitingOut(patient->getFacilityID());
                            }

                            if (falseNegFlag) {

                                stats->record_nPreTreatLtFUFNRepresent(distIndex, regIndex);
                                facilityStats->record_nPreTreatLtFUFNRepresent(patient->getFacilityID());
                            }
                            else {

                                stats->record_nPreTreatLtFURepresent(distIndex, regIndex);
                                facilityStats->record_nPreTreatLtFURepresent(patient->getFacilityID());
                                
                            }
                        }
                    }
                }
                //if (patient->getStatus() > 4) std::cout << "untreated patient2  reason =  " << patient->getReasonNoTreat() << " timesteps = " << timesteps << " status = " << patient->getStatus() << "\n";
            }
            break;
        case 5:        // PatInfNoTestNoTreat
            // no treatment - no sample returned

            // make sure Patient is alive or just ignore
            if (patient->isAlive()) {

                timesteps = patient->getTimeFromOnset();

                // debug - true
                if (debug) std::cout << "untreated patient beginning reason =  " << patient->getReasonNoTreat() << " timesteps = " << timesteps << " status = " << patient->getStatus() << "\n";
                
                // Current models of untreated tuberculosis that assume a total duration of 2 years until self - cure or death 
                // underestimate the duration of disease by about one year, but their case fatality estimates of 70 % for smear - positive 
                //and 20 % for culture - positive smear - negative tuberculosis appear to be satisfactory
                // https://www.ncbi.nlm.nih.gov/pmc/articles/PMC3070694/
                // ignore this for now
                
                // make sure patient hasn'T died of old age
                if ((patient->getAge() + (timesteps / timestepsPerYear)) > patient->getLifeExpectancy()) {
                    // patient dead
                    patient->setDead();
                    patient->setStatus(PatDead);
                    patient->setReasonDied("OldAge");
                    if (debug3) std::cout << "patient is dead  reason =  OldAge" << "\n";

                    // update Patient to inactive patient container
                    movePatientToInactive(patient, PatDead, "2", stats, facilityStats);

                    // mark time infectiousnes ends - current timestep + calcTimestep time
                    patient->setInfectEndTime(timeStep + timesteps);
                        
                    patient->setTimestepInactive(timeStep);

                    patient->setTimeOfDeath(timeStep + timesteps);

                    // need to move sample to inactive list
                    result = test->removePatientSampleFromList(patient, SampPatDead);

                    if (!result) {

                        // find the closest testing facilityj
                        testingFacilityName = getTestingFacility(patient->getDisease(), patient->getSampleTest());

                        // decode string into pointer
                        f = facilitiesList->getPtr(testingFacilityName);

                        // get testing pointer there
                        testing = f->getTestingPtr();

                        // remove patient's sample from sample list at testing facility
                        result = testing->removePatientSampleFromList(patient, SampPatDead);
                    }

                    break;
                }
                                
                // only look at active patients
                if (patient->getActive()) {
                
                    // some untreated patients will die, some will get better and some will re-present for new Diagnosis
                    // 
                    // first calculate the geometric distribution of the patient representing.
                    // Then calculate the chance of the patient dying from TB before then. If so, mark the patient dead and inactive. 
                    // If the patient is alive at re-presentation time and the time is less than 10 years, the patient represents.
                    // otherwise the patient is assumed to be infected, untreated and inactive

                    // find out how many months before re-presenting
                    // the geometric distribution represents a geometric distribution
                    noDaysRepresentOrig = getNextRandomGeoNo();
                    //noMonthsRepresent = (noDaysRepresentOrig * (d->getTimeUntreatedCure() / 30)) / 100;

                    // now calculate the probability of death
                    if (d->getName() == "TB") {

                        //// get mortality percentage from survivability table for untreated TB
                        //if (noMonthsRepresent > (d->getTimeUntreatedCure() / 30)) {

                        //    untreatedDeathPer = d->getUntreatedMortality((d->getTimeUntreatedCure() / 30)) * 100;
                        //}
                        //else {

                        //    untreatedDeathPer = d->getUntreatedMortality(noMonthsRepresent) * 100;
                        //}

                        // get the percentage of ltof patients that die before representing
                        untreatedDeathPer = d->getPerRepresentationLtoFDeath();
                    }
                    else {

                        untreatedDeathPer = d->getMortality();
                    }

                    if (debug) std::cout << "untreated patient noMonthsRepresent = " << noMonthsRepresent << ", untreated death per = " << untreatedDeathPer << "\n";

                    // get random number between 0 and 100
                    randInt = getNextRandomNo100()-1;

                    randFloat = float(randInt);

                    randInt = getNextRandomNo100()-1;

                    randFloat = randFloat + float(randInt)/100;
                    if (randFloat > 100) randFloat = 100;

                    if (noDaysRepresentOrig > d->getTimeUntreatedCure()) {

                        // what to do with those untreated but not dead - after 10 years move to inactive

                        //debug - true
                        if (debug) std::cout << "untreated patient lived reason =  " << patient->getReasonNoTreat() << " patient status = " << patient->getStatus() << "\n";

                        // make sure patient doesn't die of old age after during run
                        willPatientDie(patient, timeStepsLeftInRun);

                        // update Patient to inactive patient container
                        movePatientToInactive(patient, 99, "untreat-not-dead", stats, facilityStats);

                        // mark time infectiousnes ends - current timestep + time untreated cure years
                        calcTimestep = d->getTimeUntreatedCure();

                        patient->setInfectEndTime(timeStep + calcTimestep);

                        patient->setTimestepInactive(timeStep);

                        // need to move sample to inactive list
                        result = test->removePatientSampleFromList(patient, SampPatuntreatedInactive);

                        if (!result) {

                            // find the closest testing facilityj
                            testingFacilityName = getTestingFacility(patient->getDisease(), patient->getSampleTest());

                            // decode string into pointer
                            f = facilitiesList->getPtr(testingFacilityName);

                            // get testing pointer there
                            testing = f->getTestingPtr();

                            // remove patient's sample from sample list at testing facility
                            result = testing->removePatientSampleFromList(patient, SampPatuntreatedInactive);
                        }
                        if (statsCount) {

                            stats->record_nPreTestLtFUCured(distIndex, regIndex);
                            facilityStats->record_nPreTestLtFUCured(patient->getFacilityID());

                        }
                    }
                    else if (randFloat <= untreatedDeathPer) {

                        // check to see if patient dies from disease - debug - true
                        if (debug) std::cout << "untreated patient patient died noTreatTestTime reason =  " << patient->getReasonNoTreat() << " death per = " << noTreatDeathPer << " randint = " << randInt << "\n";
                        
                        // update counter for number dead
                        //debugCounter++;
                        
                        // calculate the death date by finding the month closest to the initDeathPer 
                        //noMonthsRepresent = d->findUntreatedMortalityIndex(randFloat);
                        
                        // geometric distribution
                        noTreatDeathTime = noDaysRepresentOrig;
                        
                        // increment how patient onset time
                        patient->incrementOnset(noTreatDeathTime);
                    
                        patient->setDead();
                        patient->setStatus(PatDead);
                        patient->setReasonDied("NoTreatDeath");
                        if (debug3) std::cout << "patient is dead  reason =  NoTreatDeath" << "\n";

                        // update Patient to inactive patient container
                        movePatientToInactive(patient, PatDead, "3", stats, facilityStats);

                        // get random number between 0 and 100
                        //randInt = getNextRandomNo100();

                        // within first 6 months - assuming 24 hour timestep
                        //calcTimestep = randInt*relapseTime/100;
                        
                        // check disease transmission for infected patients
                        if (transFlag) {

                            // check to see if patient has transmitted the disease to anyone else before coming in
                            noTransPatients = checkTransPatients(facilitiesList->getPtr(patient->getFacility()), patient->getDisease(), patient->getPopBinIndex(), transThreshold, noTreatDeathTime);

                            // debug3 = true
                            if (debug3) std::cout << "after checkTransPatients =  NoTreatDeath" << "\n";
                            
                            createTransPatients(noTransPatients, facilitiesList, patient->getFacility(), diseases, patient->getDisease(), testList, patient->getPopBinIndex(), noTreatDeathTime);
                        }

                        // mark time infectiousnes ends - current timestep + calcTimestep time
                        patient->setInfectEndTime(timeStep + noTreatDeathTime);
                        
                        patient->setTimestepInactive(timeStep + noTreatDeathTime);

                        patient->setTimeOfDeath(timeStep + noTreatDeathTime);

                        if (false) std::cout << "untreated patient died reason =  " << patient->getReasonNoTreat() << " patient status = " << patient->getStatus() << " sample test = " << patient->getSampleTest() << "\n";
                        
                        if (statsCount) {

                            stats->record_timePresentToSampleDeath(timeStep + noTreatDeathTime - patient->getTimestepPresented(), distIndex, regIndex);
                            facilityStats->record_timePresentToSampleDeath(timeStep + noTreatDeathTime - patient->getTimestepPresented(), patient->getFacilityID());
                            stats->record_timePresentToSampleDeathPreTestLTFU(timeStep + noTreatDeathTime - patient->getTimestepPresented(), distIndex, regIndex);
                            facilityStats->record_timePresentToSampleDeathPreTestLTFU(timeStep + noTreatDeathTime - patient->getTimestepPresented(), patient->getFacilityID());
                            stats->record_nPresentToSampleDeathPreTestLTFU(distIndex, regIndex);
                            facilityStats->record_nPresentToSampleDeathPreTestLTFU(patient->getFacilityID());
                            stats->record_nPresentToSampleDeath(distIndex, regIndex);
                            facilityStats->record_nPresentToSampleDeath(patient->getFacilityID());
                            stats->record_timePresentToExit(timeStep + noTreatDeathTime - patient->getTimestepPresented(), distIndex, regIndex);
                            facilityStats->record_timePresentToExit(timeStep + noTreatDeathTime - patient->getTimestepPresented(), patient->getFacilityID());
                            stats->record_nPresentToExit(distIndex, regIndex);
                            facilityStats->record_nPresentToExit(patient->getFacilityID());
                            stats->record_nDeathUntreat(distIndex, regIndex);
                            facilityStats->record_nDeathUntreat(patient->getFacilityID());
                            stats->record_nPreTestLtFUDied(distIndex, regIndex);
                            facilityStats->record_nPreTestLtFUDied(patient->getFacilityID());
                            
                        }
                        
                        // need to move sample to inactive list
                        result = test->removePatientSampleFromList(patient, SampPatDead);

                        if (!result) {

                            // find the closest testing facilityj
                            testingFacilityName = getTestingFacility(patient->getDisease(), patient->getSampleTest());

                            // decode string into pointer
                            f = facilitiesList->getPtr(testingFacilityName);

                            // get testing pointer there
                            testing = f->getTestingPtr();

                            // remove patient's sample from sample list at testing facility
                            result = testing->removePatientSampleFromList(patient, SampPatDead);
                        }
                    }
                    // untreated patient decides to go back in for representation
                    else {

                        // make sure return time is at least 7 timesteps
                        returnTime = noDaysRepresentOrig;
                        //if (returnTime < 7) {

                        //    returnTime = 7;
                        //}
                        // only mark until end of run
                        if (returnTime > (noSteps - timeStep)) {

                            returnTime = (noSteps - timeStep) + 1;
                        }
                        
                        if (debug) {

                            if (patient->getPreTestMDCDFlag()) {

                                std::cout << "untreated MDCD patient represents returnTime =  " << returnTime << "\n";
                            }
                            else {
                                std::cout << "untreated patient represents returnTime =  " << returnTime << "\n";
                            }
                        }

                        // check disease transmission for infected patients
                        if (transFlag) {

                            // check to see if patient has transmitted the disease to anyone else before coming in
                            noTransPatients = checkTransPatients(facilitiesList->getPtr(patient->getFacility()), patient->getDisease(), patient->getPopBinIndex(), transThreshold, returnTime);

                            // debug3 = true
                            if (debug3) std::cout << "after checkTransPatients =  untreated MDCD patient represents" << "\n";
                            
                            createTransPatients(noTransPatients, facilitiesList, patient->getFacility(), diseases, patient->getDisease(), testList, patient->getPopBinIndex(), returnTime);
                        }
                        if (debug3) std::cout << "after createTransPatients" << "\n";

                        // increment how patient onset time
                        patient->incrementOnset(returnTime);

                        // keep track of patients that re-present
                        patient->setRepresentFlag(true);
                        patient->setRepresentTime(timeStep + returnTime);
                        
                        // set patient away from facility return time
                        patient->setFacilityReturnTime(returnTime);

                        // update sample status to wait until representation time is over to test sample
                        samplePtr = patient->getSamplePtr();

                        samplePtr->setStatus(SampUnTestWaitRepresent);
                        patient->setSampleTestStatus("SampUnTestWaitRepresent");

                        //debug - true
                        if (debug) std::cout << "untreated patient represents noMonthsRepresent =  " << noMonthsRepresent << " patient status = " << patient->getStatus() << "\n";
                        
                        testingFacilityName = getTestingFacility(patient->getDisease(), patient->getSampleTest());
                        
                        // need to move sample to testing facility if different from facility where patient presents
                        if (patient->getFacility() != testingFacilityName) {

                            // change status to patient out but returning
                            patient->setStatus(PatAwayUnTest);
                            
                            // decode string into pointer
                            f = facilitiesList->getPtr(testingFacilityName);

                            // get testing pointer there
                            testing = f->getTestingPtr();

                            // get the pointer to the sample object for patient
                            samplePtr = patient->getSamplePtr();

                            // add it to the sample container at the testing facility
                            testing->addToSampleContainer(samplePtr);

                            // get facility pointer to facility patient presented at
                            f = facilitiesList->getPtr(patient->getFacility());

                            // get testing pointer there
                            testing = f->getTestingPtr();

                            // remove sample from sample container from facility where patient presented
                            testing->removeSampleFromList(samplePtr);
                        }
                        else {

                            // change status to patient waiting to return for in-house testing
                            patient->setStatus(PatInUnTest);
                        }
                        
                        // count as new additional samples 
                        if (statsCount) {

                            // keep track of loss to follow up patients that re-enter the system
                            stats->record_nPatientLtFUInf(distIndex, regIndex);
                            facilityStats->record_nPatientLtFUInf(patient->getFacilityID());


                            if (samplePtr->getOrigFacility() == samplePtr->getTestingFacility()) {

                                stats->record_nSamplesIn(distIndex, regIndex);
                                facilityStats->record_nSamplesIn(patient->getFacilityID());
                                stats->record_sPatientWaitingIn(distIndex, regIndex);
                                facilityStats->record_sPatientWaitingIn(patient->getFacilityID());
                            }
                            else {

                                stats->record_nSamplesOut(distIndex, regIndex);
                                facilityStats->record_nSamplesOut(patient->getFacilityID());
                                stats->record_timeSampleTransport(samplePtr->getTransitTimeSave(), distIndex, regIndex);
                                facilityStats->record_timeSampleTransport(samplePtr->getTransitTimeSave(), patient->getFacilityID());
                                stats->record_nTransported(distIndex, regIndex);
                                facilityStats->record_nTransported(patient->getFacilityID());
                                stats->record_sPatientWaitingOut(distIndex, regIndex);
                                facilityStats->record_sPatientWaitingOut(patient->getFacilityID());
                            }

                            stats->record_nPreTestLtFURepresent(distIndex, regIndex);
                            facilityStats->record_nPreTestLtFURepresent(patient->getFacilityID());
                            

                            if (debug) std::cout << "after stats" << "\n";
                        }
                    }
                }
                //if (patient->getStatus() > 4) std::cout << "untreated patient2  reason =  " << patient->getReasonNoTreat() << " timesteps = " << timesteps << " status = " << patient->getStatus() << "\n";
            }
            break;
        case 6:        // PatInfTestLtofNoTreat
            // no treatment - sample returned - patient loss to follow up

            // make sure Patient is alive or just ignore
            if (patient->isAlive()) {

                timesteps = patient->getTimeFromOnset();

                // debug = true
                if (debug) std::cout << "untreated patient beginning reason =  " << patient->getReasonNoTreat() << " timesteps = " << timesteps << " status = " << patient->getStatus() << "\n";
                
                // Current models of untreated tuberculosis that assume a total duration of 2 years until self - cure or death 
                // underestimate the duration of disease by about one year, but their case fatality estimates of 70 % for smear - positive 
                //and 20 % for culture - positive smear - negative tuberculosis appear to be satisfactory
                // https://www.ncbi.nlm.nih.gov/pmc/articles/PMC3070694/
                // ignore this for now
                
                // make sure patient hasn'T died of old age
                if ((patient->getAge() + (timesteps / timestepsPerYear)) > patient->getLifeExpectancy()) {
                    // patient dead
                    patient->setDead();
                    patient->setStatus(PatDead);
                    patient->setReasonDied("OldAge");
                    if (debug3) std::cout << "patient is dead  reason =  OldAge" << "\n";

                    // update Patient to inactive patient container
                    movePatientToInactive(patient, PatDead, "2", stats, facilityStats);

                    // mark time infectiousnes ends - current timestep + calcTimestep time
                    patient->setInfectEndTime(timeStep + timesteps);
                        
                    patient->setTimestepInactive(timeStep);

                    patient->setTimeOfDeath(timeStep + timesteps);

                    // need to move sample to inactive list
                    result = test->removePatientSampleFromList(patient, SampPatDead);

                    if (!result) {

                        // find the closest testing facilityj
                        testingFacilityName = getTestingFacility(patient->getDisease(), patient->getSampleTest());

                        // decode string into pointer
                        f = facilitiesList->getPtr(testingFacilityName);

                        // get testing pointer there
                        testing = f->getTestingPtr();

                        // remove patient's sample from sample list at testing facility
                        result = testing->removePatientSampleFromList(patient, SampPatDead);
                    }

                    break;
                }
                                
                // only look at active patients
                if (patient->getActive()) {

                    // some untreated patients will die and some will re-present for new Diagnosis
                    // first calculate the geometric distribution of the patient representing.
                    // Then calculate the chance of the patient dying from TB before then. If so, mark the patient dead and inactive. 
                    // If the patient is alive at re-presentation time and the time is less than 10 years, the patient represents.
                    // otherwise the patient is assumed to be infected, untreated and inactive
                    
                    // find out how many months before re-presenting
                    // the geometric distribution represents a geometric distribution
                    noDaysRepresentOrig = getNextRandomGeoNo();
                    //noMonthsRepresent = (noDaysRepresentOrig * (d->getTimeUntreatedCure() / 30)) / 100;

                    // now calculate the probability of death
                    if (d->getName() == "TB") {

                        //// get death percentage from untreated TB surviability table
                        //if (noMonthsRepresent > (d->getTimeUntreatedCure() / 30)) {

                        //    untreatedDeathPer = d->getUntreatedMortality((d->getTimeUntreatedCure() / 30)) * 100;
                        //}
                        //else {

                        //    untreatedDeathPer = d->getUntreatedMortality(noMonthsRepresent) * 100;
                        //}

                        // get the percentage of ltof patients that die before representing
                        untreatedDeathPer = d->getPerRepresentationLtoFDeath();
                    }
                    else {

                        untreatedDeathPer = d->getMortality();
                    }

                    // get random number between 0 and 100
                    randInt = getNextRandomNo100()-1;

                    randFloat = float(randInt);

                    randInt = getNextRandomNo100()-1;

                    randFloat = randFloat + float(randInt)/100;
                    if (randFloat > 100) randFloat = 100;
                    
                    //debug3 = true
                    if (debug3) std::cout << "****untreated patient noTreatTestTime untreatedDeathPer =  " << untreatedDeathPer << " noMonthsRepresent = " << noMonthsRepresent << " randFloat = " << randFloat << "\n";

                    if (noDaysRepresentOrig > d->getTimeUntreatedCure()) {
                        // what to do with those untreated but not dead - after 10 years assume remission

                        if (debug3) std::cout << "ltof treated patient lived reason =  " << patient->getReasonNoTreat() << " patient status = " << patient->getStatus() << "\n";

                        // make sure patient doesn't die of old age after during run
                        willPatientDie(patient, timeStepsLeftInRun);

                        // update Patient to inactive patient container
                        movePatientToInactive(patient, 99, "untreat-not-dead", stats, facilityStats);

                        // mark time infectiousnes ends - current timestep + untreated cure years

                        calcTimestep = d->getTimeUntreatedCure();

                        patient->setInfectEndTime(timeStep + calcTimestep);

                        patient->setTimestepInactive(timeStep);

                        // need to move sample to inactive list
                        result = test->removePatientSampleFromList(patient, SampPatuntreatedInactive);

                        if (!result) {

                            // find the closest testing facilityj
                            testingFacilityName = getTestingFacility(patient->getDisease(), patient->getSampleTest());

                            // decode string into pointer
                            f = facilitiesList->getPtr(testingFacilityName);

                            // get testing pointer there
                            testing = f->getTestingPtr();

                            // remove patient's sample from sample list at testing facility
                            result = testing->removePatientSampleFromList(patient, SampPatuntreatedInactive);
                        }
                        if (statsCount) {

                            stats->record_nTreatLtFUCured(distIndex, regIndex);
                            facilityStats->record_nTreatLtFUCured(patient->getFacilityID());
                        }
                        
                    }
                    else if (randFloat <= untreatedDeathPer) {

                        // patient dies from disease
                    
                        // debug3 = true
                        if (debug3) std::cout << "untreated patient noTreatTestTime reason =  " << patient->getReasonNoTreat() << " death per = " << noTreatDeathPer << " randint = " << randInt << "\n";
                        
                        // update counter for number dead
                        //debugCounter++;

                        // calculate the death date by finding the month closest to the initDeathPer 
                        //noMonthsRepresent = d->findUntreatedMortalityIndex(randFloat);

                        // geometric distribution
                        noTreatDeathTime = noDaysRepresentOrig;

                        // check disease transmission for infected patients
                        if (transFlag) {

                            // check to see if patient has transmitted the disease to anyone else before coming in
                            noTransPatients = checkTransPatients(facilitiesList->getPtr(patient->getFacility()), patient->getDisease(), patient->getPopBinIndex(), transThreshold, noTreatDeathTime);

                            // debug3 = true
                            if (debug3) std::cout << "after checkTransPatients =  untreated patient dies" << "\n";
                            
                            createTransPatients(noTransPatients, facilitiesList, patient->getFacility(), diseases, patient->getDisease(), testList, patient->getPopBinIndex(), noTreatDeathTime);
                        }

                        // increment how patient onset time
                        patient->incrementOnset(noTreatDeathTime);
                    
                        patient->setDead();
                        patient->setStatus(PatDead);
                        patient->setReasonDied("NoTreatDeathPreTestLtFU");

                        if (debug3) std::cout << "patient is dead  reason =  NoTreatDeath" << "\n";

                        // update Patient to inactive patient container
                        movePatientToInactive(patient, PatDead, "3", stats, facilityStats);

                        // get random number between 0 and 100
                        //randInt = getNextRandomNo100();

                        // within first 6 months - assuming 24 hour timestep
                        //calcTimestep = randInt*relapseTime/100;
                        
                        // mark time infectiousnes ends - current timestep + calcTimestep time
                        patient->setInfectEndTime(timeStep + noTreatDeathTime);
                        
                        patient->setTimestepInactive(timeStep + noTreatDeathTime);

                        patient->setTimeOfDeath(timeStep + noTreatDeathTime);

                        if (statsCount) {

                            stats->record_nTreatLtFUDied(distIndex, regIndex);
                            facilityStats->record_nTreatLtFUDied(patient->getFacilityID());
                            stats->record_timePresentToSampleDeathTreatLTFU(timeStep + noTreatDeathTime - patient->getTimestepPresented(), distIndex, regIndex);
                            facilityStats->record_timePresentToSampleDeathTreatLTFU(timeStep + noTreatDeathTime - patient->getTimestepPresented(), patient->getFacilityID());
                            stats->record_nPresentToSampleDeathTreatLTFU(distIndex, regIndex);
                            facilityStats->record_nPresentToSampleDeathTreatLTFU(patient->getFacilityID());
                            stats->record_timePresentToSampleDeath(timeStep + noTreatDeathTime - patient->getTimestepPresented(), distIndex, regIndex);
                            facilityStats->record_timePresentToSampleDeath(timeStep + noTreatDeathTime - patient->getTimestepPresented(), patient->getFacilityID());
                            stats->record_nPresentToSampleDeath(distIndex, regIndex);
                            facilityStats->record_nPresentToSampleDeath(patient->getFacilityID());
                            stats->record_timePresentToExit(timeStep + noTreatDeathTime - patient->getTimestepPresented(), distIndex, regIndex);
                            facilityStats->record_timePresentToExit(timeStep + noTreatDeathTime - patient->getTimestepPresented(), patient->getFacilityID());
                            stats->record_nPresentToExit(distIndex, regIndex);
                            facilityStats->record_nPresentToExit(patient->getFacilityID());
                            //stats->reduce_sSampleWaitingForTestOut(distIndex, regIndex);
                            stats->record_nDeathTreat(distIndex, regIndex);
                            facilityStats->record_nDeathTreat(patient->getFacilityID());
                            
                        }
   
                        if (debug3) std::cout << "untreated patient died reason =  " << patient->getReasonNoTreat() << " patient status = " << patient->getStatus() << " sample test = " << patient->getSampleTest() << "\n";
                        
                        // need to move sample to inactive list
                        result = test->removePatientSampleFromList(patient, SampPatDead);

                        if (!result) {

                            // find the closest testing facilityj
                            testingFacilityName = getTestingFacility(patient->getDisease(), patient->getSampleTest());

                            // decode string into pointer
                            f = facilitiesList->getPtr(testingFacilityName);

                            // get testing pointer there
                            testing = f->getTestingPtr();

                            // remove patient's sample from sample list at testing facility
                            result = testing->removePatientSampleFromList(patient, SampPatDead);
                        }
                    }
                    // untreated patient decides to go back in for representation
                    else {

                        if (debug3) std::cout << "ltof treated patient represent monthsrepresent =  " << noMonthsRepresent << " max time cure = " << d->getTimeUntreatedCure() / 30 << "\n";
                        
                        // make sure return time is at least 7 timesteps and at most 1 more than the end of run
                        returnTime = noDaysRepresentOrig;
                        /*if (returnTime < 7) {

                            returnTime = 7;
                        }*/
                        // if longer than end of run, cap it there
                        if (returnTime > (noSteps - timeStep)) {

                            returnTime = (noSteps - timeStep) + 1;
                        }

                        // check disease transmission for infected patients
                        if (transFlag) {

                            // check to see if patient has transmitted the disease to anyone else before coming in
                            noTransPatients = checkTransPatients(facilitiesList->getPtr(patient->getFacility()), patient->getDisease(), patient->getPopBinIndex(), transThreshold, returnTime);

                            // debug3 = true
                            if (debug3) std::cout << "after checkTransPatients =  represent monthsrepresent" << "\n";

                            createTransPatients(noTransPatients, facilitiesList, patient->getFacility(), diseases, patient->getDisease(), testList, patient->getPopBinIndex(), returnTime);
                        }

                        // increment how patient onset time
                        patient->incrementOnset(returnTime);

                        // keep track of patients that re-present
                        patient->setRepresentFlag(true);
                        patient->setRepresentTime(timeStep + returnTime);

                        // set patient away from facility return time
                        patient->setFacilityReturnTime(returnTime);

                        // update sample status to wait until representation time is over to test sample
                        samplePtr = patient->getSamplePtr();

                        samplePtr->setStatus(SampUnTestWaitRepresent);
                        patient->setSampleTestStatus("SampUnTestWaitRepresent");

                        // need to move sample to testing facility if different from facility where patient presents
                        testingFacilityName = getTestingFacility(patient->getDisease(), patient->getSampleTest());

                        if (patient->getFacility() != testingFacilityName) {

                            // change status to patient out but returning
                            patient->setStatus(PatAwayUnTest);
                            
                            // decode string into pointer
                            f = facilitiesList->getPtr(testingFacilityName);

                            // get testing pointer there
                            testing = f->getTestingPtr();

                            // get the pointer to the sample object for patient
                            samplePtr = patient->getSamplePtr();

                            // add it to the sample container at the testing facility
                            testing->addToSampleContainer(samplePtr);

                            // get facility pointer to facility patient presented at
                            f = facilitiesList->getPtr(patient->getFacility());

                            // get testing pointer there
                            testing = f->getTestingPtr();

                            // remove sample from sample container from facility where patient presented
                            testing->removeSampleFromList(samplePtr);
                        }
                        else {

                            // change status to patient waiting to return for in-house testing
                            patient->setStatus(PatInUnTest);
                        }
                        
                        // count as new additional samples 
                        if (statsCount) {

                            // keep track of loss to follow up patients that re-enter the system
                            stats->record_nPatientLtFUInf(distIndex, regIndex);
                            facilityStats->record_nPatientLtFUInf(patient->getFacilityID());

                            stats->record_nTreatLtFURepresent(distIndex, regIndex);
                            facilityStats->record_nTreatLtFURepresent(patient->getFacilityID());

                            if (samplePtr->getOrigFacility() == samplePtr->getTestingFacility()) {

                                stats->record_nSamplesIn(distIndex, regIndex);
                                facilityStats->record_nSamplesIn(patient->getFacilityID());
                                stats->record_sPatientWaitingIn(distIndex, regIndex);
                                facilityStats->record_sPatientWaitingIn(patient->getFacilityID());
                            }
                            else {

                                stats->record_nSamplesOut(distIndex, regIndex);
                                facilityStats->record_nSamplesOut(patient->getFacilityID());
                                stats->record_timeSampleTransport(samplePtr->getTransitTimeSave(), distIndex, regIndex);
                                facilityStats->record_timeSampleTransport(samplePtr->getTransitTimeSave(), patient->getFacilityID());
                                stats->record_nTransported(distIndex, regIndex);
                                facilityStats->record_nTransported(patient->getFacilityID());
                                stats->record_sPatientWaitingOut(distIndex, regIndex);
                                facilityStats->record_sPatientWaitingOut(patient->getFacilityID());
                                
                            }
                        }
                    }
                    //debug3 = false;
                }
                //if (patient->getStatus() > 4) std::cout << "untreated patient2  reason =  " << patient->getReasonNoTreat() << " timesteps = " << timesteps << " status = " << patient->getStatus() << "\n";
            }
            break;
        case 7:  //PatDead = 7;
            
            // patient is dead - do nothing

            if (debug) std::cout << "patient is dead  reason =  " << patient->getReasonNoTreat() << " infected = " << patient->getInfected() << " status = " << patient->getStatus() << "\n";
            break;
        case 8: // PatCured = 8;
            
            // patient cured, ignore unless older than life expectancy, then kill
            // make sure Patient is alive or just ignore
            if (patient->isAlive()) {
                timesteps = patient->getCreateTime();

                if ((patient->getAge() + (timesteps / timestepsPerYear)) > patient->getLifeExpectancy()) {
                    // patient dead
                    patient->setDead();
                    patient->setStatus(PatDead);
                    patient->setReasonDied("OldAge");
                    if (debug3) std::cout << "patient is dead  reason =  OldAge" << "\n";

                    // update Patient to inactive patient container
                    movePatientToInactive(patient, PatDead, "4", stats, facilityStats);

                    // mark time infectiousnes ends - current timestep + calcTimestep time
                    patient->setInfectEndTime(timeStep + timesteps);
                        
                    patient->setTimestepInactive(timeStep);

                    patient->setTimeOfDeath(timeStep + timesteps);

                    // need to move sample to inactive list
                    result = test->removePatientSampleFromList(patient, SampPatDead);

                    if (!result) {

                        // find the closest testing facilityj
                        testingFacilityName = getTestingFacility(patient->getDisease(), patient->getSampleTest());

                        // decode string into pointer
                        f = facilitiesList->getPtr(testingFacilityName);

                        // get testing pointer there
                        testing = f->getTestingPtr();

                        // remove patient's sample from sample list at testing facility
                        result = testing->removePatientSampleFromList(patient, SampPatDead);
                    }
                }
                else {
                    // increase Patient age
                    patient->incrementTimeStepsNotInTreatment();
                }
            }

            break;
        case 9: // PatUninfTest = 9;
            
            // patient uninfected, ignore unless older than life expectancy, then kill
            // make sure Patient is alive or just ignore
            if (patient->isAlive()) {
                timesteps = patient->getTimeFromOnset();

                if ((patient->getAge() + (timesteps / timestepsPerYear)) > patient->getLifeExpectancy()) {
                    // patient dead
                    patient->setDead();
                    patient->setStatus(PatDead);
                    patient->setReasonDied("OldAge");
                    if (debug3) std::cout << "patient is dead  reason =  OldAge" << "\n";

                    // update Patient to inactive patient container
                    movePatientToInactive(patient, PatDead, "5", stats, facilityStats);

                    patient->setTimestepInactive(timeStep);

                    patient->setTimeOfDeath(timeStep + timesteps);

                    // need to move sample to inactive list
                    result = test->removePatientSampleFromList(patient, SampPatDead);

                    if (!result) {

                        // find the closest testing facilityj
                        testingFacilityName = getTestingFacility(patient->getDisease(), patient->getSampleTest());

                        // decode string into pointer
                        f = facilitiesList->getPtr(testingFacilityName);

                        // get testing pointer there
                        testing = f->getTestingPtr();

                        // remove patient's sample from sample list at testing facility
                        result = testing->removePatientSampleFromList(patient, SampPatDead);
                    }
                }
                else {
                    // increase Patient age
                    patient->incrementTimeStepsNotInTreatment();
                }
            }

            break;
        case 10: // PatUninfNoTest = 10;
            
            // patient uninfected - didn't return for diagnosis, ignore unless older than life expectancy, then kill
            // make sure Patient is alive or just ignore
            if (patient->isAlive()) {
                timesteps = patient->getTimeFromOnset();

                if ((patient->getAge() + (timesteps / timestepsPerYear)) > patient->getLifeExpectancy()) {
                    // patient dead
                    patient->setDead();
                    patient->setStatus(PatDead);
                    patient->setReasonDied("OldAge");
                    if (debug3) std::cout << "patient is dead  reason =  OldAge" << "\n";

                    // update Patient to inactive patient container
                    movePatientToInactive(patient, PatDead, "6", stats, facilityStats);

                    patient->setTimestepInactive(timeStep);

                    patient->setTimeOfDeath(timeStep + timesteps);

                    // need to move sample to inactive list
                    result = test->removePatientSampleFromList(patient, SampPatDead);

                    if (!result) {

                        // find the closest testing facilityj
                        testingFacilityName = getTestingFacility(patient->getDisease(), patient->getSampleTest());

                        // decode string into pointer
                        f = facilitiesList->getPtr(testingFacilityName);

                        // get testing pointer there
                        testing = f->getTestingPtr();

                        // remove patient's sample from sample list at testing facility
                        result = testing->removePatientSampleFromList(patient, SampPatDead);
                    }
                }
                else {
                    // increase Patient age
                    patient->incrementTimeStepsNotInTreatment();
                }
            }

            break;
        case 11: // PatUninfTestTreat
            // patient uninfected - but treated anyway deal with repercussions
            if (patient->isAlive()) {
                timesteps = patient->getTimeFromOnset();

                if ((patient->getAge() + (timesteps / timestepsPerYear)) > patient->getLifeExpectancy()) {
                    // patient dead
                    patient->setDead();
                    patient->setStatus(PatDead);
                    patient->setReasonDied("OldAge");
                    if (debug3) std::cout << "patient is dead  reason =  OldAge" << "\n";

                    // update Patient to inactive patient container
                    movePatientToInactive(patient, PatDead, "7", stats, facilityStats);

                    patient->setTimestepInactive(timeStep);

                    patient->setTimeOfDeath(timeStep + timesteps);

                    // need to move sample to inactive list
                    result = test->removePatientSampleFromList(patient, SampPatDead);

                    if (!result) {

                        // find the closest testing facilityj
                        testingFacilityName = getTestingFacility(patient->getDisease(), patient->getSampleTest());

                        // decode string into pointer
                        f = facilitiesList->getPtr(testingFacilityName);

                        // get testing pointer there
                        testing = f->getTestingPtr();

                        // remove patient's sample from sample list at testing facility
                        result = testing->removePatientSampleFromList(patient, SampPatDead);
                    }
                }
                else {
                    // ignore if inactive
                    if (patient->getActive()) {
   
                        randInt = getNextRandomNo100()-1;
                
                        randFloat = float(randInt);

                        randInt = getNextRandomNo100()-1;

                        randFloat = randFloat + float(randInt)/100;
                        if (randFloat > 100) randFloat = 100;

                        //std::cout << "treated uninfected patient randFloat = " << randFloat << " randInt = " << randInt << "\n";    

                        // remove those that actually start treatment - only counting infected patients
                        //if (statsCount) stats->reduce_sPatientInTreatment(distIndex, regIndex);
                            
                        // make sure patient doesn't die of old age after during run
                        willPatientDie(patient, timeStepsLeftInRun);
                        
                        // loss to follow up before treatment starts - falls into category of uninfected patient received results, no-treatment
                        // patient either re-presents or dies
                        if (randFloat < initTreatLtoF) {

                            if (debug) std::cout << "false positive and loss to follow up before treatement" << "\n";
                            
                            // increase Patient age
                            //patient->incrementTimeStepsNotInTreatment();
                         
                            patient->incrementTimeStepsNotInTreatment();
                            
                            // update Patient to inactive patient container
                            movePatientToInactive(patient, PatUninfTestLtoF, "UninfectLtoFBeforeTreat", stats, facilityStats);

                            patient->setTimestepInactive(timeStep);

                            if (statsCount) {

                                stats->record_nPreTreatLtFU(distIndex, regIndex);
                                facilityStats->record_nPreTreatLtFU(patient->getFacilityID());
                                
                                stats->record_nPatientsPreTreatedLtoFUninf(distIndex, regIndex);
                                facilityStats->record_nPatientsPreTreatedLtoFUninf(patient->getFacilityID());
                                
                            }
                        }
                        else {
                
                            // increase Patient age
                            //patient->incrementTimeStepsNotInTreatment();
                            // get random number between 0 and 100 for treatment results
                            randInt = getNextRandomNo100()-1;

                            randFloat = float(randInt);

                            randInt = getNextRandomNo100()-1;

                            randFloat = randFloat + float(randInt) / 100;
                            if (randFloat > 100) randFloat = 100;


                            // update patient test result information
                            patient->setTreated();

                            patient->setStartedTreatmentTime(timeStep);

                            if (randFloat < treatLtoF) {

                                // patient - loss to followup - falls into category of uninfected patient received results started treatment but didn't finish

                                //std::cout << "patient assigned treatment but didn't finish - lost to follow up extraTreatTime = " << extraTreatTime << "\n";

                                // calculate time steps in treatment before loss to follow up
                                randInt = getNextRandomNo100();

                                // within first 6 months - assuming 24 hour timestep
                                calcTimestep = randInt * treatmentTime / 100;

                                patient->incrementTimeStepsInTreatment(calcTimestep);

                                patient->setStatus(PatUninfTestLtoF);

                                // track LtoF time
                                patient->seTimePatientLtoF(timeStep + calcTimestep);

                                patient->setReasonNoTreat("PatUninfTestLtofTreat");

                                // mark here just in case we add a re-presentation or prob of death if statement
                                if (statsCount) {

                                    stats->record_nTreatLtFU(distIndex, regIndex);
                                    facilityStats->record_nTreatLtFU(patient->getFacilityID());
                                    //stats->record_nTreatLtFUUninf(distIndex, regIndex);
                                    
                                }

                            }
                            else {

                                // update Patient to inactive patient container
                                movePatientToInactive(patient, PatUninfTestTreat, "1", stats, facilityStats);

                                // increase time from onset and time in treatment
                                patient->incrementTimeStepsInTreatment(treatmentTime);

                                patient->setStartedTreatmentTime(timeStep);
                                patient->setTimestepInactive(timeStep + treatmentTime);

                                if (statsCount) {

                                    stats->record_nPatientsTreated(distIndex, regIndex);
                                    facilityStats->record_nPatientsTreated(patient->getFacilityID());
                                    stats->record_nPatientsTreatedUninf(distIndex, regIndex);
                                    facilityStats->record_nPatientsTreatedUninf(patient->getFacilityID());
                                    
                                }
                            }
                        }

                        // need to move sample to inactive list
                        result = test->removePatientSampleFromList(patient, SampReturnedAvail);

                        if (!result) {

                            // find the closest testing facilityj
                            testingFacilityName = getTestingFacility(patient->getDisease(), patient->getSampleTest());

                            // decode string into pointer
                            f = facilitiesList->getPtr(testingFacilityName);

                            // get testing pointer there
                            testing = f->getTestingPtr();

                            // remove patient's sample from sample list at testing facility
                            result = testing->removePatientSampleFromList(patient, SampReturnedAvail);
                        }
                    }
                }
            }

            break;
        }
        // debug - true
        if (debug) std::cout << "inside update patients after case statement" << "\n";
    }

    // clear samplesToDelete vector
    // samplesToDelete.clear();

    // now move patients from patient transmission container to patient container
    if (transFlag) {

        moveTransPatientToActive();
    }


    // debug - true
    if (debug) std::cout << "leaving update patients" << "\n";
}


// update Patients if Samples have returned
void Diagnosis::movePatientToInactive(Patient* patient, int reason, string loc, Stats* stats, Stats* facilityStats) {

    string reasonString;
    int lowerTime = 0;
    int upperTime = 0;

    bool dead;
    bool doneWithInfTime = false;
    bool debug = false;

    dead = patient->isAlive();

    // set patient active status to inactive
    patient->setInactive();

    auto itp = std::find(_inactivePatientContainer.begin(), _inactivePatientContainer.end(), patient);

    if (itp == _inactivePatientContainer.end()) {

        // add patient to inactive patient container
        _inactivePatientContainer.push_back(patient);
    }
    else {

        if (debug) std::cout << "Patient already in inactive container, don't add more than once " <<  " \n";
    }

    // add patient to inactive patient container
   // _inactivePatientContainer.push_back(patient);

    auto it = std::find(_patientContainer.begin(), _patientContainer.end(), patient);

    if (it != _patientContainer.end()) {

        _patientContainer.erase(it);
        // std::cout << "found match in movePatientToInactive, list size = " << _patientContainer.size() << " status= " << patient->getStatus()  << " \n";
        // std::cout << "found match in movePatientToInactive, list size = " << _inactivePatientContainer.size() << " status= " << patient->getStatus()  << " \n";
    }
    else {

        switch (reason) {
        
            case 0 :
                reasonString = "PatAwayUnTest";				// patient away from facility - hasn't returned for results
                break;
            case 1 :
                reasonString = "PatInUnTest";				// patient in-house - no sample results back yet
                break;
            case 2 :
                reasonString = "PatInfTestTreat";			// patient infected, sample results returned, undergoing treatment
                break;
            case 3 :
                reasonString = "PatInfTestRelapse";			// patient infected, sample results returned, treated, relapse
                break;
            case 4 :
                reasonString = "PatInfTestNoTreat";			// Patient infected, sample results returned, no treatment
                break;
            case 5 :
                reasonString = "PatInfNoTestNoTreat";			// Patient infected, no sample results returned, no treatment
                break;
            case 6 :
                reasonString = "PatInfTestNoTreatLtoF";			// Patient infected, sample results returned, loss to follow up - no treatment
                break;
            case 7 :
                reasonString = "PatDead";					// patient dead
                break;
            case 8 :
                reasonString = "PatCured ";					// patient cured
                break;
            case 9 :
                reasonString = "PatUninfTest";				// patient uninfected - sample results returned to patient
                break;
            case 10 :
                reasonString = "PatUninfNoTest";				// patient uninfected - didn't return for sample results
                break;
            case 11:
                reasonString = "PatUninfTestTreat";			// patient uninfected - sample results returned uninfected, but was treated anyway
                break;
            case 99 :
                reasonString = "PatUninfNoTest";				// patient uninfected - didn't return for sample results
                break;
            default:
                reasonString = "unknown";
        }

        if (debug) {
        
            std::cout << "movePatientToInacitve, couldn't find match in patient container for patient moved to inactive container reason = " << reasonString;
            std::cout << ", " << loc << ", alive = " << dead << ", reason = " << patient->getReasonNoTreat() << " \n";
        }
    }

    // now calculate infectious days and update appropriate stat year variable
    if (patient->getInfected()) {

        lowerTime = patient->getTimestepCreated();
        upperTime = patient->getInfectEndTime();

        //if ((upperTime - lowerTime) > 60) debug = true;

        if (debug) std::cout << "inactive patient infected created time = " << lowerTime << ", inactive time = " << upperTime << ", years in run = " << yearsInRun << ", timestepsPerYear = " << timestepsPerYear << " \n";

        for (int i = 0; i < yearsInRun; i++) {

            if (!doneWithInfTime) {

                if (debug) std::cout << "year = " << i << ", lowerTime = " << lowerTime << ", (i + 1) * timestepsPerYear = " << (i + 1) * timestepsPerYear << " \n";

                if (lowerTime < (i + 1) * timestepsPerYear) {

                    // some infectious time in this year
                    // count it up and save it away
                    // two cases - all time in this year - or time split between this year and next
                    if (upperTime < (i + 1) * timestepsPerYear) {

                        if (debug) std::cout << "all in one year = " << i << " \n";

                        // all in this year
                        stats->record_nPatientInfDaysYear(i, patient->getFacilityDistrictIndex(), patient->getFacilityRegionIndex(), upperTime-lowerTime);
                        facilityStats->record_nPatientInfDaysYear(i, patient->getFacilityID(), upperTime - lowerTime);
                        
                        doneWithInfTime = true;
                    }
                    else {

                        if (debug) std::cout << "split between years = " << i << " \n";
                        
                        // remove the amount for this year and keep going
                        stats->record_nPatientInfDaysYear(i, patient->getFacilityDistrictIndex(), patient->getFacilityRegionIndex(), ((i+1)*timestepsPerYear - lowerTime));
                        facilityStats->record_nPatientInfDaysYear(i, patient->getFacilityID(), ((i + 1) * timestepsPerYear - lowerTime));

                        lowerTime = (i + 1) * timestepsPerYear;
                    }
                }
            }
        }
    }

}



// move the tranmission patients to the active patient container
void Diagnosis::moveTransPatientToActive() {


    bool debug = false;

    //// loop through transmission patient container and move to active patient container and then delete from transmission container
    //for (auto patient : _transPatientContainer) {

    //    auto itp = std::find(_transPatientContainer.begin(), _transPatientContainer.end(), patient);

    //    if (itp == _transPatientContainer.end()) {

    //        // add patient to active patient container
    //        _patientContainer.push_back(patient);
    //    }
    //    else {

    //        if (debug) std::cout << "Patient already in active container, don't add more than once " << " \n";
    //    }
    //}

    if (_transPatientContainer.size() < 1) {

        if (debug) std::cout << "inside move trans patient - no trans patients size = " << _transPatientContainer.size() << " \n";
        return;
    }

    if (debug) std::cout << "inside move trans patient to active container code size = " << _patientContainer.size() << " \n";

    _patientContainer.insert(_patientContainer.end(), std::make_move_iterator(_transPatientContainer.begin()),std::make_move_iterator(_transPatientContainer.end()));

    if (debug) std::cout << "inside move trans patient to active container - move to active container size = " << _patientContainer.size() << " trans size = " << _transPatientContainer.size() << " \n";
    
    // remove all the object pointers from the transmission patient container
    _transPatientContainer.erase(_transPatientContainer.begin(), _transPatientContainer.end());

    //stats->record_nPatientTransmission(patient->getFacilityDistrictIndex(), patient->getFacilityRegionIndex()));

    if (debug) std::cout << "leaving move trans patient to active container size = " << _transPatientContainer.size() << " \n";

}


// are there any tests available for this disease available at this facility
bool Diagnosis::anyTestingHere(string disease) {

    vector<diagnosisAttributes*> diagnosisAtt;

    // see if any testing done here for this disease - eventually will need to figure in priority of tests
    for (auto diagAttr : diagnosisAtt) {

        if (diagAttr->disease == disease) {

            if (diagAttr->testing > 0) {

                return true;
            }
        }
    }

    return false;
}

// find the treatment time from treatment intialization curve
int Diagnosis::getTestTreatmemtStart(string disease, string test) {

    // this routine calculates the number of days that it takes for a patient to start treatment after receiving results of a test
    // It is dependent on the disease and test.  Currently we only have it programmed for TB with Xpert or smear
    // Each polynomial defines the percentage curve versus days until treatment start.  Each increment is 5 days long
    // This comes from Mwansa-Kambafwile_pretx_ltf_plos2017.pdf in Zimbabwe
    int treatmentStartTime = 0;
    int percent = 0;
    int randInt = 0;

    randInt = getNextRandomNo100();
    
    // we have treatment start time cures for Xpert and smear of TB - need to add more for other diseases/tests
    if (disease == "TB") {

        // default to max, work from there
        treatmentStartTime = 0;
        
        if (test == "Xpert") {

            // Xpert equation =-0.0001*D2^4+0.0111*D2^3 -0.4482*D2^2 + 7.933*D2 + 41.387
            for (int i = 0; i < 44; i++) {
            
                percent = -0.0001*pow(i, 4) + 0.0111*pow(i, 3) -0.4482*pow(i, 2) + 7.933*i + 41.387;

                //std::cout << "percent " << percent << ", randInt = " << randInt << ", treatmentStartTime = " << i * 5 << " \n";
                
                if (i == 43) {
                
                    // if at end assume loss to follow up (220 timesteps)
                    //treatmentStartTime = 220;
                    treatmentStartTime = -1;
                }
                else if (percent <= randInt) {

                    // calculate no. of days before treatment starts
                    treatmentStartTime = i * 5;
                }
                else {

                    break;
                }
            }
        }
        else if (test == "smear") {

            // smear equation =0.0001*D2^4 - 0.0071*D2^3 +  0.0658*D2^2 + 3.99*D2 + 1.76
            for (int i = 0; i < 44; i++) {
            
                percent = 0.0001*pow(i, 4) - 0.0071*pow(i, 3) +  0.0658*pow(i, 2) + 3.99*i + 1.76;

                //std::cout << "percent smear " << percent << ", randInt = " << randInt << ", treatmentStartTime = " << i * 5 << " \n";
                
                if (i == 43) {
                
                    // if at end assume loss to follow up (220 timesteps)
                    //treatmentStartTime = 220;
                    treatmentStartTime = -1;
                }
                else if (percent <= randInt) {

                    // calculate no. of days before treatment starts
                    treatmentStartTime = i * 5;
                }
                else {

                    break;
                }
            }
        }
    }

    return treatmentStartTime;
}


// will the patient die of old age during the study
void Diagnosis::willPatientDie(Patient* patient, int timeStepsLeftInRun) {
    
    // make sure patient hasn'T died of old age
    if ((patient->getAge() + (timeStepsLeftInRun / timestepsPerYear)) >= patient->getLifeExpectancy()) {
    
        //std::cout << "dies from old age" << " \n";
        
        // set patient to dead status - patient just dies from old age, don't change any other parameters
        patient->setDead();

        patient->setReasonDied("OldAge");
    }
}

// patient cured
void patientCured(Patient* patient, int timeStepsLeftInRun) {


}


// patient relapses
void patientRelapse(Patient* patient, int timeStepsLeftInRun) {


}


// patient loss to follow up
void patientLtoF(Patient* patient, int timeStepsLeftInRun) {


}


// patient loss to follow up
void patientRepresents(Patient* patient, int timeStepsLeftInRun) {


}


Diagnosis::~Diagnosis() {
    //cout<<"Remove diagnosis "<<_id<<endl;
}
//---------------------------------------




