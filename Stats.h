//
//  Stats.h
//  mdrtb
//
//  Created by Trinity on 8/4/15.
//  Copyright (c) 2015 Trinity. All rights reserved.
//

#ifndef __STATS_H
#define __STATS_H

#include <sstream>
#include <array>
#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <iterator>
#include <memory> //for smart pointers
#include <thread>
#include <mutex>
#include "GlobalParameters.h"
#include "Disease.h"


class Disease;

//#include "Functions.h"

using namespace std;
//using namespace FUNCTIONS;
class Stats {
private:
        

    int TNOW ;                      //time counter variable (based on weeks)  TNOW shared among all classes
    int LASTID;                     //unique id counter for population members

    //This is a helper for the outputVector function
    //unique_ptr<ofstream> _setupOutputVectorFile ( const string filename, const bool overwrite) ;

    
    //private:
    //This is a helper for the outputVector function
    unique_ptr<ofstream> _setupOutputVectorFile ( const string filename, const bool overwrite) {
        
        auto base = ios::app;
        
        if (overwrite)
            base = ios_base::out;
        
        //Rewrite using shared pointer removes the need for the calling function to clean up
        //Changed to unique_ptr because the ofstream destructor calls .close()
        return unique_ptr<ofstream> ( new ofstream (filename, base) );
    }
    
public:
    Stats(); //Constructor
    Stats(const Stats &src); //Copy constructor
    //Access functions
    void setTime( const int ) noexcept;
    int getTime() noexcept;
    void incTime() noexcept;
    int getYear() noexcept;
    int getID() noexcept;
    void incID() noexcept;
    void setInfDays(int dist, int reg, int value, int year);
    void getInfDays();
    void setInfDaysAct(int dist, int reg, int value, int year);
    void getInfDaysAct();

    //Array of single counters updated at event time - one for each region, distric and facility
    array<string, 1000> __name;                          // name of region/district/facility
    array<int, 1000> __id;                               // id of facility
    array<int, 1000> __tier;                             // tier of facility
    array<int, 1000> __xpertMach;                        // xpert at facility
    array<int, 1000> __year;                             // year of simulation
    array<string, 1000> __scenario;                      // scenario
    array<string, 1000> __district;                      // name of district
    array<string, 1000> __region;                        // name of region
    array<int, 1000> __population;                       // population of region/district/facility
    array<string, 1000> __disease;                       // statistics of this disease
    array<float, 1000> __incidence;                      // incidence of this disease
    array<float, 1000> __calcIncidence;                  // calculated incidence of this disease
    array<int, 1000> __nPatient;                         // no. of patients
    array<int, 1000> __nPatientInf;                      // no. of patients that are infected
    array<int, 1000> __nPatientLtFUInf;                  // no. of patients that are loss to follow up and represent - infected
    array<int, 1000> __nPatientFromTrans;                // no. of patients that are created from disease transmission
    array<int, 1000> __nPatientNotPresenting;               // no. of patients that are infected but didn't present for diagnosis
    array<int, 1000> __nSamples;                         // no. of samples collected
    array<int, 1000> __nSamplesIn;                       // no. of samples collected in-house
    array<int, 1000> __nSamplesOut;                      // no. of samples sent out for testing
    array<int, 1000> __timeSampleResourceWait;           // total time samples have to wait for resources to be tested
    array<int, 1000> __timeSampleResourceWaitIn;         // total time samples have to wait for resources to be tested in-house
    array<int, 1000> __timeSampleResourceWaitOut;        // total time samples have to wait for resources to be tested at another facility
    array<int, 1000> __nTests;                           // no. of tests done
    array<int, 1000> __nTestsIn;                         // no. of tests done in-house
    array<int, 1000> __nTestsOut;                        // no. of tests done at facility different from where patient presents
    array<int, 1000> __nTransported;                     // no. of samples transported
    array<int, 1000> __timeSampleTransport;              // total time samples were in transport
    array<int, 1000> __timePresentToSampleTestIn;        // total time from patient presentation to sample test - in-house
    array<int, 1000> __nPresentToSampleTestIn;           // no. of patients counted in __timePresentToSampleTestIn
    array<int, 1000> __timePresentToSampleTestOut;       // total time from patient presentation to sample test - at another facility
    array<int, 1000> __nPresentToSampleTestOut;          // no. of patients counted in __timePresentToSampleTestOut
    array<int, 1000> __nTestPos;                         // no. of tests positive
    array<int, 1000> __nTestTruePos;                     // no. of tests true positive
    array<int, 1000> __nTestTruePosIn;                   // no. of tests true positive - in-house
    array<int, 1000> __nTestTruePosOut;                  // no. of tests true positive - at another facility
    array<int, 1000> __nTestFalsePos;                    // no. of tests false positive
    array<int, 1000> __nTestFalsePosIn;                  // no. of tests false positive - in-house
    array<int, 1000> __nTestFalsePosOut;                 // no. of tests false positive - at another facility
    array<int, 1000> __nTestNeg;                         // no. of tests negative
    array<int, 1000> __nTestTrueNeg;                     // no. of tests true negative
    array<int, 1000> __nTestTrueNegIn;                   // no. of tests true negative - in-house
    array<int, 1000> __nTestTrueNegOut;                  // no. of tests true negative - at another facility
    array<int, 1000> __nTestFalseNeg;                    // no. of tests false negative
    array<int, 1000> __nTestFalseNegIn;                  // no. of tests false negative - in-house
    array<int, 1000> __nTestFalseNegOut;                 // no. of tests false negative - at another facility
    array<int, 1000> __nTestPosIn;                       // no. of tests that are returned positive from testing - in-house
    array<int, 1000> __nTestPosOut;                      // no. of tests that are returned positive from testing - at another facility
    array<int, 1000> __nMDCDInfected;                    // clinical diagnoses of infected patients - basically true pos
    array<int, 1000> __nMDCDUninfected;                  // clinical diagnoses of uninfected patients - basically false pos
    array<int, 1000> __nPTMDCDInfected;                  // pre-test clinical diagnoses of infected patients - basically true pos
    array<int, 1000> __nPTMDCDUninfected;                // pre-test clinical diagnoses of uninfected patients - basically false pos
    array<int, 1000> __nDiagnosis;                       // no. of pos. test results - true pos, false pos and MD override true pos. and false pos
    array<int, 1000> __nDiagnosisIn;                     // no. of pos. test results - true pos, false pos and MD override true pos. and false pos - in house
    array<int, 1000> __nDiagnosisOut;                    // no. of pos. test results - true pos, false pos and MD override true pos. and false pos - at another facility
    array<int, 1000> __nDiagnosisDC;                     // no. of pos. test results not counted because md clinical diagnosis gave true pos before test gives positive
    array<int, 1000> __nDiagnosisDCTruePos;              // no. of pos. test results not counted because md clinical diagnosis gave true pos before test gives true positive
    array<int, 1000> __nDiagnosisDCFalsePos;             // no. of pos. test results not counted because md clinical diagnosis gave true pos before test gives false positive
    array<int, 1000> __nPreTestDiagnosisTruePos;         // no. of true pos. pre-test MD override results
    array<int, 1000> __nPreTestDiagnosisFalseNeg;        // no. of false neg. pre-test MD override results
    array<int, 1000> __nPreTestDiagnosisTrueNeg;         // no. of true neg. pre-test MD override results
    array<int, 1000> __nPreTestDiagnosisFalsePos;        // no. of false pos. pre-test MD override results
    array<int, 1000> __nPreTestDiagnosisTotal;           // total no. pre-test true pos and false pos MD override results
    array<int, 1000> __nPreTestDiagnosisReview;          // total no. pre-test MD override results
    array<int, 1000> __nPostTestDiagnosisTruePosIn;      // no. of True pos. post-test MD override results in-house
    array<int, 1000> __nPostTestDiagnosisTruePosOut;     // no. of True pos. post-test MD override results tested out
    array<int, 1000> __nPostTestDiagnosisFalseNeg;       // no. of false neg. post-test MD override results
    array<int, 1000> __nPostTestDiagnosisTrueNeg;        // no. of true neg. post-test MD override results
    array<int, 1000> __nPostTestDiagnosisFalsePosIn;     // no. of false pos. post-test MD override results tested in-house
    array<int, 1000> __nPostTestDiagnosisFalsePosOut;    // no. of false pos. post-test MD override results tested out
    array<int, 1000> __nPostTestDiagnosisTotal;          // total no. of post-test MD true pos and false pos override results
    array<int, 1000> __nPostTestDiagnosisReview;         // total no. of post-test MD override results
    array<int, 1000> __nMDDiagnosisTotal;                // total no. MD true pos and false pos override results - both pre and post test
    array<int, 1000> __nMDDiagnosisReview;               // total no. MD override results - both pre and post test
    array<int, 1000> __nPreTestLtFU;                     // no. of samples lost to pre-test loss to follow up
    array<int, 1000> __nPreTestLtFUInf;                  // no. of samples lost to pre-test loss to follow up for infected patients
    array<int, 1000> __nPreTestLtFUDied;                 // no. of patients whose samples were pre-test ltfu and died
    array<int, 1000> __nPreTestLtFUCured;                // no. of patients whose samples were pre-test ltfu, didn't represent and were cured
    array<int, 1000> __nPreTestLtFURepresent;            // no. of patients whose samples were pre-test ltfu and re-presented
    array<int, 1000> __nPreTreatLtFU;                    // no. of samples lost to pre-treat loss to follow up
    array<int, 1000> __nPreTreatLtFUInf;                 // no. of samples lost to pre-treat loss to follow up for infected patients
    array<int, 1000> __nPreTreatLtFUDied;                // no. of patients whose samples were pre-treat ltfu and died
    array<int, 1000> __nPreTreatLtFUCured;               // no. of patients whose samples were pre-treat ltfu, didn't represent and were cured
    array<int, 1000> __nPreTreatLtFURepresent;           // no. of patients whose samples were pre-treat ltfu and re-presented
    array<int, 1000> __nPreTreatLtFUFN;                  // no. of samples lost to pre-treat loss to follow up due to false neg test result
    array<int, 1000> __nPreTreatLtFUFNDied;              // no. of samples lost to pre-treat loss to follow up due to false neg test result that die
    array<int, 1000> __nPreTreatLtFUFNCured;             // no. of samples lost to pre-treat loss to follow up due to false neg test result that are cured
    array<int, 1000> __nPreTreatLtFUFNRepresent;         // no. of samples lost to pre-treat loss to follow up due to false neg test result that represent
    array<int, 1000> __nPreTreatLtFUPos;                 // no. of patients whose samples were pre-treat ltfu tested positive
    array<int, 1000> __nPreTreatLtFUNeg;                 // no. of patients whose samples were pre-treat ltfu tested negative
    array<int, 1000> __nTreatLtFU;                       // no. of samples treated loss to follow up
    array<int, 1000> __nTreatLtFUInf;                    // no. of samples treated loss to follow up for infected patients
    array<int, 1000> __nTreatLtFUDied;                   // no. of patients treated ltfu and died
    array<int, 1000> __nTreatLtFUCured;                  // no. of patients treated ltfu, didn't represent and were cured
    array<int, 1000> __nTreatLtFURepresent;              // no. of patients treated ltfu and re-presented
    array<int, 1000> __nPatientsTreated;                 // no. of patients treated
    array<int, 1000> __nPatientsTreatedInf;              // no. of patients treated that are infected
    array<int, 1000> __nPatientsTreatedUninf;            // no. of patients treated that are uninfected
    array<int, 1000> __nPatientsPreTreatedLtoFUninf;     // no. of patients treated that are uninfected and loss to Follow up before treatment
    array<int, 1000> __nPatientsTreatedCured;            // no. of patients treated that are cured
    array<int, 1000> __nPatientsTreatedDied;             // no. of patients treated that died
    array<int, 1000> __nPatientsTreatedRelapse;          // no. of patients treated that relapse
    array<int, 1000> __nTreatedRelapse;                  // no. of patients treated who relapse after treatment
    array<int, 1000> __nTreatedRelapseCured;             // no. of patients treated who relapse and are cured
    array<int, 1000> __nTreatedRelapseDied;              // no. of patients treated who relapse and die
    //array<int, 1000> __avePresentToSampleDiagIn;         // average time from patient presentation to sample diagnosis - in-house
    array<int, 1000> __timePresentToSampleDiagIn;        // total time from patient presentation to sample diagnosis - in-house
    array<int, 1000> __nPresentToSampleDiagIn;           // no. of patients counted in __timePresentToSampleDiagIn
    array<int, 1000> __timePresentToSampleDiagOut;       // total time from patient presentation to sample diagnosis - at another facility
    array<int, 1000> __nPresentToSampleDiagOut;          // no. of patients counted in __timePresentToSampleDiagOut
    array<int, 1000> __timePresentToSampleTreat;         // total time from patient presentation to treatment
    array<int, 1000> __nPresentToSampleTreat;            // no. of patients counted in __timePresentToSampleTreat
    array<int, 1000> __timePresentToSampleTreatIn;       // total time from patient presentation to treatment - in-house
    array<int, 1000> __nPresentToSampleTreatIn;          // no. of patients counted in __timePresentToSampleTreatIn
    array<int, 1000> __timePresentToSampleTreatOut;      // total time from patient presentation to treatment - at another facility
    array<int, 1000> __nPresentToSampleTreatOut;         // no. of patients counted in __timePresentToSampleTreatOut
    array<int, 1000> __timePresentToSampleDeath;       // total time from patient presentation to death
    array<int, 1000> __nPresentToSampleDeath;          // no. of patients counted in __timePresentToSampleDeath
    array<int, 1000> __timePresentToSampleDeathIn;       // total time from patient presentation to death - in-house
    array<int, 1000> __nPresentToSampleDeathIn;          // no. of patients counted in __timePresentToSampleDeathIn
    array<int, 1000> __timePresentToSampleDeathOut;      // total time from patient presentation to death - at another facility
    array<int, 1000> __nPresentToSampleDeathOut;         // no. of patients counted in __nPresentToSampleDeathOut
    array<int, 1000> __timePresentToSampleDeathPreTestLTFU; // total time from patient presentation to death after pre-test loss to follow up
    array<int, 1000> __nPresentToSampleDeathPreTestLTFU; // no. of patients counted in __timePresentToSampleDeathPreTestLTFU
    array<int, 1000> __timePresentToSampleDeathPreTreatLTFU; // total time from patient presentation to death after pre-treat loss to follow up
    array<int, 1000> __nPresentToSampleDeathPreTreatLTFU; // no. of patients counted in __timePresentToSampleDeathPreTreatLTFU
    array<int, 1000> __timePresentToSampleDeathTreatLTFU; // total time from patient presentation to death after treat loss to follow up
    array<int, 1000> __nPresentToSampleDeathTreatLTFU;   // no. of patients counted in __timePresentToSampleDeathTreatLTFU
    array<int, 1000> __timePresentToSampleDeathInTreat;  // total time from patient presentation to death while in treatment
    array<int, 1000> __nPresentToSampleDeathInTreat;     // no. of patients counted in __timePresentToSampleDeathInTreat
    array<int, 1000> __timePresentToSampleDeathInRelapse;// total time from patient presentation to death while in relapse treatment
    array<int, 1000> __nPresentToSampleDeathInRelapse;   // no. of patients counted in __timePresentToSampleDeathInRelapse
    array<int, 1000> __timePresentToExit;                // total time from patient presentation to death or cure
    array<int, 1000> __nPresentToExit;                   // no. of patients counted in __timePresentToExit
    array<int, 1000> __nXpert;                           // no. of Xpert tests
    array<int, 1000> __nXpertIn;                         // no. of Xpert tests in-house
    array<int, 1000> __nXpertOut;                        // no. of Xpert tests at another facility
    array<int, 1000> __nXpertPos;                        // no. of Xpert positive tests
    array<int, 1000> __nXpertTruePos;                    // no. of Xpert true positive tests
    array<int, 1000> __nXpertFalsePos;                   // no. of Xpert false positive tests
    array<int, 1000> __nXpertNeg;                        // no. of Xpert negative tests
    array<int, 1000> __nXpertTrueNeg;                    // no. of Xpert true negative tests
    array<int, 1000> __nXpertFalseNeg;                   // no. of Xpert false negative tests
    array<int, 1000> __nSmear;                           // no. of smear tests
    array<int, 1000> __nSmearIn;                         // no. of smear tests in-house
    array<int, 1000> __nSmearOut;                        // no. of smear tests at another facility
    array<int, 1000> __nSmearPos;                        // no. of smear positive tests
    array<int, 1000> __nSmearTruePos;                    // no. of smear true positive tests
    array<int, 1000> __nSmearFalsePos;                   // no. of smear false positive tests
    array<int, 1000> __nSmearNeg;                        // no. of smear true negative tests
    array<int, 1000> __nSmearTrueNeg;                    // no. of smear true negative tests
    array<int, 1000> __nSmearFalseNeg;                   // no. of smear false negative tests
    array<int, 1000> __nDeathPreTest;                    // no. of patients that die from pre-test loss to follow up
    array<int, 1000> __nDeathPreTreat;                   // no. of patients that die from pre-treatment loss to follow up
    array<int, 1000> __nDeathTreat;                      // no. of patients that die during treatment
    array<int, 1000> __nDeathReTreat;                    // no. of patients that die during re-treatment
    array<int, 1000> __nDeathUntreat;                    // no. of patients that die without treatment
    array<int, 1000> __nPatientInfDays;                  // no. of infectious days for patients
    array<int, 1000> __nPatientInfDaysYear0;             // no. of infectious days for patients in year 0 of run
    array<int, 1000> __nPatientInfDaysYear1;             // no. of infectious days for patients in year 1 of run
    array<int, 1000> __nPatientInfDaysYear2;             // no. of infectious days for patients in year 2 of run
    array<int, 1000> __nPatientInfDaysYear3;             // no. of infectious days for patients in year 3 of run
    array<int, 1000> __nPatientInfDaysYear4;             // no. of infectious days for patients in year 4 of run
    array<int, 1000> __nPatientInfDaysYear5;             // no. of infectious days for patients in year 5 of run
    array<int, 1000> __nPatientInfDaysYear6;             // no. of infectious days for patients in year 6 of run
    array<int, 1000> __nPatientInfDaysYear7;             // no. of infectious days for patients in year 7 of run
    array<int, 1000> __nPatientInfDaysYear8;             // no. of infectious days for patients in year 8 of run
    array<int, 1000> __nPatientInfDaysYear9;             // no. of infectious days for patients in year 9 of run
    array<int, 1000> __nPatientInfDaysYear10;            // no. of infectious days for patients in year 10 of run
    array<int, 1000> __nPatientInfDaysYear11;            // no. of infectious days for patients in year 11 of run
    array<int, 1000> __nPatientInfDaysYear12;            // no. of infectious days for patients in year 12 of run
    array<int, 1000> __nPatientInfDaysYear13;            // no. of infectious days for patients in year 13 of run
    array<int, 1000> __nPatientInfDaysYear14;            // no. of infectious days for patients in year 14 of run
    array<int, 1000> __nPatientInfDaysYear15;            // no. of infectious days for patients in year 15 of run
    array<int, 1000> __nPatientInfDaysAct;               // no. of infectious days for active patients
    array<int, 1000> __nPatientInfDaysActYear0;          // no. of infectious days for active patients in year 0 of run
    array<int, 1000> __nPatientInfDaysActYear1;          // no. of infectious days for active patients in year 1 of run
    array<int, 1000> __nPatientInfDaysActYear2;          // no. of infectious days for active patients in year 2 of run
    array<int, 1000> __nPatientInfDaysActYear3;          // no. of infectious days for active patients in year 3 of run
    array<int, 1000> __nPatientInfDaysActYear4;          // no. of infectious days for active patients in year 4 of run
    array<int, 1000> __nPatientInfDaysActYear5;          // no. of infectious days for active patients in year 5 of run
    array<int, 1000> __nPatientInfDaysActYear6;          // no. of infectious days for active patients in year 6 of run
    array<int, 1000> __nPatientInfDaysActYear7;          // no. of infectious days for active patients in year 7 of run
    array<int, 1000> __nPatientInfDaysActYear8;          // no. of infectious days for active patients in year 8 of run
    array<int, 1000> __nPatientInfDaysActYear9;          // no. of infectious days for active patients in year 9 of run
    array<int, 1000> __nPatientInfDaysActYear10;         // no. of infectious days for active patients in year 10 of run
    array<int, 1000> __nPatientInfDaysActYear11;         // no. of infectious days for active patients in year 11 of run
    array<int, 1000> __nPatientInfDaysActYear12;         // no. of infectious days for active patients in year 12 of run
    array<int, 1000> __nPatientInfDaysActYear13;         // no. of infectious days for active patients in year 13 of run
    array<int, 1000> __nPatientInfDaysActYear14;         // no. of infectious days for active patients in year 14 of run
    array<int, 1000> __nPatientInfDaysActYear15;         // no. of infectious days for active patients in year 15 of run
    array<int, 1000> __sPatientWaiting;                  // no. of patients waiting for test results
    array<int, 1000> __sPatientWaitingIn;                // no. of patients waiting for test results - in-house
    array<int, 1000> __sPatientWaitingOut;               // no. of patients waiting for test results at another facility
    array<int, 1000> __sPatientInTreatment;              // no. of patients currently in treatment without resolution
    array<int, 1000> __sSampleWaitingForTest;            // no. of samples waiting to be tested
    array<int, 1000> __sSampleWaitingForTestIn;          // no. of samples waiting to be tested - in-house
    array<int, 1000> __sSampleWaitingForTestOut;         // no. of samples waiting to be tested at another facility

    inline string returnName(int index) { return __name[index]; }
    inline string returnDistrict(int index) { return __district[index]; }
    inline string returnRegion(int index) { return __region[index]; }

    inline void record_xpertMach(int index) { __xpertMach[index]++; }
    inline void record_xpertMachTotal() { __xpertMach[0]++;}
    inline int return_xpertMach(int index) { return __xpertMach[index]; }
    inline void record_population(int population, int index){__population[index] = population;}
        inline int return_population(int index){return __population[index];}
    inline void record_incidence(float incidence, int index){__incidence[index] = incidence;}
        inline float return_incidence(int index){return __incidence[index];}
    inline void record_calcIncidence(float calcIncidence, int index){__calcIncidence[index] = calcIncidence;}
        inline void calc_calcIncidence(){for (int i = 0; i < 1000; i++) { if (__population[i] >= 1) {__calcIncidence[i] = float((float(__nPatientInf[i]) * 100000) / float(__population[i]));} else {__calcIncidence[i] = 0;};};}
        inline float return_calcIncidence(int index){return __calcIncidence[index];}
    inline void record_year(int year){for (int i = 0; i < 1000; i++) {__year[i] = year;};}
        inline int return_year(int index){return __year[index];}
    // fill in whole stat array for each region/district with scenario name
    inline void record_scenario(string scenario){for (int i = 0; i < 1000; i++) {__scenario[i] = scenario;};}    
        inline string return_scenario(int index){return __scenario[index];}
    // fill in whole stat array for each region/district with disease name
    inline void record_disease(string disease){for (int i = 0; i < 1000; i++) {__disease[i] = disease;};}
        inline string return_disease(int index){return __disease[index];}
    inline void record_nPatient(int fac) { __nPatient[fac]++;}
    inline void record_nPatient(int dist, int reg) {__nPatient[0]++; __nPatient[dist]++; __nPatient[reg]++;}
        inline int return_nPatient(int index){return __nPatient[index];}
    inline void record_nPatientInf(int fac) { __nPatientInf[fac]++;}
    inline void record_nPatientInf(int dist, int reg){__nPatientInf[0]++;__nPatientInf[dist]++;__nPatientInf[reg]++;}
        inline int return_nPatientInf(int index){return __nPatientInf[index];}
    inline void record_nPatientLtFUInf(int fac) { __nPatientLtFUInf[fac]++;}
    inline void record_nPatientLtFUInf(int dist, int reg){__nPatientLtFUInf[0]++;__nPatientLtFUInf[dist]++;__nPatientLtFUInf[reg]++;}
        inline int return_nPatientLtFUInf(int index){return __nPatientLtFUInf[index];}
    inline void record_nPatientFromTrans(int fac) { __nPatientFromTrans[fac]++;}
    inline void record_nPatientFromTrans(int dist, int reg) { __nPatientFromTrans[0]++; __nPatientFromTrans[dist]++; __nPatientFromTrans[reg]++; }
        inline int return_nPatientFromTrans(int index) { return __nPatientFromTrans[index]; }
    inline void record_nPatientNotPresenting(int fac, int value) { __nPatientNotPresenting[fac]= __nPatientNotPresenting[fac] + value; }
    inline void record_nPatientNotPresenting(int dist, int reg, int value) { __nPatientNotPresenting[0] = __nPatientNotPresenting[0] + value; __nPatientNotPresenting[dist] = __nPatientNotPresenting[dist] + value; __nPatientNotPresenting[reg] = __nPatientNotPresenting[reg] + value; }
        inline int return_nPatientNotPresenting(int index) { return __nPatientNotPresenting[index]; }
    inline void record_nSamples(int fac){__nSamples[fac]++;}
    inline void record_nSamples(int dist, int reg) { __nSamples[0]++; __nSamples[dist]++; __nSamples[reg]++; }
        inline int return_nSamples(int index){return __nSamples[index];}
    inline void record_nSamplesIn(int fac){__nSamplesIn[fac]++; __nSamples[fac]++;}
    inline void record_nSamplesIn(int dist, int reg) { __nSamplesIn[0]++; __nSamples[0]++; __nSamplesIn[dist]++; __nSamples[dist]++; __nSamplesIn[reg]++; __nSamples[reg]++; }
        inline int return_nSamplesIn(int index){return __nSamplesIn[index];}
    inline void record_nSamplesOut(int fac){__nSamplesOut[fac]++; __nSamples[fac]++;}
    inline void record_nSamplesOut(int dist, int reg) { __nSamplesOut[0]++; __nSamples[0]++; __nSamplesOut[dist]++; __nSamples[dist]++; __nSamplesOut[reg]++; __nSamples[reg]++; }
        inline int return_nSamplesOut(int index){return __nSamplesOut[index];}
    inline void record_timeSampleResourceWait(int time, int fac){__timeSampleResourceWait[fac] = __timeSampleResourceWait[fac] + time;}
    inline void record_timeSampleResourceWait(int time, int dist, int reg) { __timeSampleResourceWait[0] = __timeSampleResourceWait[0] + time; __timeSampleResourceWait[dist] = __timeSampleResourceWait[dist] + time; __timeSampleResourceWait[reg] = __timeSampleResourceWait[reg] + time; }
        inline int return_timeSampleResourceWait(int index){return __timeSampleResourceWait[index];}
    inline void record_timeSampleResourceWaitIn(int time, int fac){__timeSampleResourceWaitIn[fac] = __timeSampleResourceWaitIn[fac] + time;__timeSampleResourceWait[fac] = __timeSampleResourceWait[fac] + time;}
    inline void record_timeSampleResourceWaitIn(int time, int dist, int reg) { __timeSampleResourceWaitIn[0] = __timeSampleResourceWaitIn[0] + time; __timeSampleResourceWait[0] = __timeSampleResourceWait[0] + time; __timeSampleResourceWaitIn[dist] = __timeSampleResourceWaitIn[dist] + time; __timeSampleResourceWait[dist] = __timeSampleResourceWait[dist] + time; __timeSampleResourceWaitIn[reg] = __timeSampleResourceWaitIn[reg] + time; __timeSampleResourceWait[reg] = __timeSampleResourceWait[reg] + time; }
        inline int return_timeSampleResourceWaitIn(int index){return __timeSampleResourceWaitIn[index];}
    inline void record_timeSampleResourceWaitOut(int time, int fac){__timeSampleResourceWaitOut[fac] = __timeSampleResourceWaitOut[fac] + time;__timeSampleResourceWait[fac] = __timeSampleResourceWait[fac] + time;}
    inline void record_timeSampleResourceWaitOut(int time, int dist, int reg) { __timeSampleResourceWaitOut[0] = __timeSampleResourceWaitOut[0] + time; __timeSampleResourceWait[0] = __timeSampleResourceWait[0] + time; __timeSampleResourceWaitOut[dist] = __timeSampleResourceWaitOut[dist] + time; __timeSampleResourceWait[dist] = __timeSampleResourceWait[dist] + time; __timeSampleResourceWaitOut[reg] = __timeSampleResourceWaitOut[reg] + time; __timeSampleResourceWait[reg] = __timeSampleResourceWait[reg] + time; }
        inline int return_timeSampleResourceWaitOut(int index){return __timeSampleResourceWaitOut[index];}
    inline void record_nTests(int fac){__nTests[fac]++;}
    inline void record_nTests(int dist, int reg) { __nTests[0]++; __nTests[dist]++; __nTests[reg]++; }
        inline int return_nTests(int index){return __nTests[index];}
    inline void record_nTestsIn(int fac){__nTestsIn[fac]++; __nTests[fac]++;}
    inline void record_nTestsIn(int dist, int reg) { __nTestsIn[0]++; __nTests[0]++; __nTestsIn[dist]++; __nTests[dist]++; __nTestsIn[reg]++; __nTests[reg]++; }
        inline int return_nTestsIn(int index){return __nTestsIn[index];}
    inline void record_nTestsOut(int fac){__nTestsOut[fac]++; __nTests[fac]++;}
    inline void record_nTestsOut(int dist, int reg) { __nTestsOut[0]++; __nTests[0]++; __nTestsOut[dist]++; __nTests[dist]++; __nTestsOut[reg]++; __nTests[reg]++; }
        inline int return_nTestsOut(int index){return __nTestsOut[index];}
    inline void record_nTransported(int fac){__nTransported[fac]++;}
    inline void record_nTransported(int dist, int reg) { __nTransported[0]++; __nTransported[dist]++; __nTransported[reg]++; }
        inline int return_nTransported(int index){return __nTransported[index];}
    inline void record_timeSampleTransport(int time, int fac){__timeSampleTransport[fac] = __timeSampleTransport[fac] + time;}
    inline void record_timeSampleTransport(int time, int dist, int reg) { __timeSampleTransport[0] = __timeSampleTransport[0] + time; __timeSampleTransport[dist] = __timeSampleTransport[dist] + time; __timeSampleTransport[reg] = __timeSampleTransport[reg] + time; }
        inline int return_timeSampleTransport(int index){return __timeSampleTransport[index];}
    inline void record_timePresentToSampleTestIn(int time, int fac){__timePresentToSampleTestIn[fac] = __timePresentToSampleTestIn[fac] + time;}
    inline void record_timePresentToSampleTestIn(int time, int dist, int reg) { __timePresentToSampleTestIn[0] = __timePresentToSampleTestIn[0] + time; __timePresentToSampleTestIn[dist] = __timePresentToSampleTestIn[dist] + time; __timePresentToSampleTestIn[reg] = __timePresentToSampleTestIn[reg] + time; }
        inline int return_timePresentToSampleTestIn(int index){return __timePresentToSampleTestIn[index];}
    inline void record_nPresentToSampleTestIn(int fac){__nPresentToSampleTestIn[fac]++;}
    inline void record_nPresentToSampleTestIn(int dist, int reg) { __nPresentToSampleTestIn[0]++; __nPresentToSampleTestIn[dist]++; __nPresentToSampleTestIn[reg]++; }
        inline int return_nPresentToSampleTestIn(int index){return __nPresentToSampleTestIn[index];}
    inline void record_timePresentToSampleTestOut(int time, int fac){__timePresentToSampleTestOut[fac] = __timePresentToSampleTestOut[fac] + time;}
    inline void record_timePresentToSampleTestOut(int time, int dist, int reg) { __timePresentToSampleTestOut[0] = __timePresentToSampleTestOut[0] + time; __timePresentToSampleTestOut[dist] = __timePresentToSampleTestOut[dist] + time; __timePresentToSampleTestOut[reg] = __timePresentToSampleTestOut[reg] + time; }
        inline int return_timePresentToSampleTestOut(int index){return __timePresentToSampleTestOut[index];}
    inline void record_nPresentToSampleTestOut(int fac){__nPresentToSampleTestOut[fac]++;}
    inline void record_nPresentToSampleTestOut(int dist, int reg) { __nPresentToSampleTestOut[0]++; __nPresentToSampleTestOut[dist]++; __nPresentToSampleTestOut[reg]++; }
        inline int return_nPresentToSampleTestOut(int index){return __nPresentToSampleTestOut[index];}
    inline void record_nTestPos(int fac){__nTestPos[fac]++;}
    inline void record_nTestPos(int dist, int reg) { __nTestPos[0]++; __nTestPos[dist]++; __nTestPos[reg]++; }
        inline int return_nTestPos(int index){return __nTestPos[index];}
    inline void record_nTestTruePos(int fac){__nTestTruePos[fac]++;}
    inline void record_nTestTruePos(int dist, int reg) { __nTestTruePos[0]++; __nTestTruePos[dist]++; __nTestTruePos[reg]++; }
        inline int return_nTestTruePos(int index){return __nTestTruePos[index];}
    inline void record_nTestTruePosIn(int fac){__nTestTruePosIn[fac]++;__nTestTruePos[fac]++;__nTestPos[fac]++;}
    inline void record_nTestTruePosIn(int dist, int reg) { __nTestTruePosIn[0]++; __nTestTruePos[0]++; __nTestPos[0]++; __nTestTruePosIn[dist]++; __nTestTruePos[dist]++; __nTestPos[dist]++; __nTestTruePosIn[reg]++; __nTestTruePos[reg]++; __nTestPos[reg]++; }
        inline int return_nTestTruePosIn(int index){return __nTestTruePosIn[index];}
    inline void record_nTestTruePosOut(int fac){__nTestTruePosOut[fac]++;__nTestTruePos[fac]++;__nTestPos[fac]++;}
    inline void record_nTestTruePosOut(int dist, int reg) { __nTestTruePosOut[0]++; __nTestTruePos[0]++; __nTestPos[0]++; __nTestTruePosOut[dist]++; __nTestTruePos[dist]++; __nTestPos[dist]++; __nTestTruePosOut[reg]++; __nTestTruePos[reg]++; __nTestPos[reg]++; }
        inline int return_nTestTruePosOut(int index){return __nTestTruePosOut[index];}
    inline void record_nTestFalsePos(int fac){__nTestFalsePos[fac]++;}
    inline void record_nTestFalsePos(int dist, int reg) { __nTestFalsePos[0]++; __nTestFalsePos[dist]++; __nTestFalsePos[reg]++; }
        inline int return_nTestFalsePos(int index){return __nTestFalsePos[index];}
    inline void record_nTestFalsePosIn(int fac){__nTestFalsePosIn[fac]++;__nTestFalsePos[fac]++;__nTestPos[fac]++;}
    inline void record_nTestFalsePosIn(int dist, int reg) { __nTestFalsePosIn[0]++; __nTestFalsePos[0]++; __nTestPos[0]++; __nTestFalsePosIn[dist]++; __nTestFalsePos[dist]++; __nTestPos[dist]++; __nTestFalsePosIn[reg]++; __nTestFalsePos[reg]++; __nTestPos[reg]++; }
        inline int return_nTestFalsePosIn(int index){return __nTestFalsePosIn[index];}
    inline void record_nTestFalsePosOut(int fac){__nTestFalsePosOut[fac]++;__nTestFalsePos[fac]++;__nTestPos[fac]++;}
    inline void record_nTestFalsePosOut(int dist, int reg) { __nTestFalsePosOut[0]++; __nTestFalsePos[0]++; __nTestPos[0]++; __nTestFalsePosOut[dist]++; __nTestFalsePos[dist]++; __nTestPos[dist]++; __nTestFalsePosOut[reg]++; __nTestFalsePos[reg]++; __nTestPos[reg]++; }
        inline int return_nTestFalsePosOut(int index){return __nTestFalsePosOut[index];}
    inline void record_nTestNeg(int fac){__nTestNeg[fac]++;}
    inline void record_nTestNeg(int dist, int reg) { __nTestNeg[0]++; __nTestNeg[dist]++; __nTestNeg[reg]++; }
        inline int return_nTestNeg(int index){return __nTestNeg[index];}
    inline void record_nTestTrueNeg(int fac){__nTestTrueNeg[fac]++;}
    inline void record_nTestTrueNeg(int dist, int reg) { __nTestTrueNeg[0]++; __nTestTrueNeg[dist]++; __nTestTrueNeg[reg]++; }
        inline int return_nTestTrueNeg(int index){return __nTestTrueNeg[index];}
    inline void record_nTestTrueNegIn(int fac){__nTestTrueNegIn[fac]++;__nTestTrueNeg[fac]++;__nTestNeg[fac]++;;}
    inline void record_nTestTrueNegIn(int dist, int reg) { __nTestTrueNegIn[0]++; __nTestTrueNeg[0]++; __nTestNeg[0]++; __nTestTrueNegIn[dist]++; __nTestTrueNeg[dist]++; __nTestNeg[dist]++; __nTestTrueNegIn[reg]++; __nTestTrueNeg[reg]++; __nTestNeg[reg]++; }
        inline int return_nTestTrueNegIn(int index){return __nTestTrueNegIn[index];}
    inline void record_nTestTrueNegOut(int fac){__nTestTrueNegOut[fac]++;__nTestTrueNeg[fac]++;__nTestNeg[fac]++;}
    inline void record_nTestTrueNegOut(int dist, int reg) { __nTestTrueNegOut[0]++; __nTestTrueNeg[0]++; __nTestNeg[0]++; __nTestTrueNegOut[dist]++; __nTestTrueNeg[dist]++; __nTestNeg[dist]++; __nTestTrueNegOut[reg]++; __nTestTrueNeg[reg]++; __nTestNeg[reg]++; }
        inline int return_nTestTrueNegOut(int index){return __nTestTrueNegOut[index];}
    inline void record_nTestFalseNeg(int fac){__nTestFalseNeg[fac]++;}
    inline void record_nTestFalseNeg(int dist, int reg) { __nTestFalseNeg[0]++; __nTestFalseNeg[dist]++; __nTestFalseNeg[reg]++; }
        inline int return_nTestFalseNeg(int index){return __nTestFalseNeg[index];}
    inline void record_nTestFalseNegIn(int fac){__nTestFalseNegIn[fac]++;__nTestFalseNeg[fac]++;__nTestNeg[fac]++;}
    inline void record_nTestFalseNegIn(int dist, int reg) { __nTestFalseNegIn[0]++; __nTestFalseNeg[0]++; __nTestNeg[0]++; __nTestFalseNegIn[dist]++; __nTestFalseNeg[dist]++; __nTestNeg[dist]++; __nTestFalseNegIn[reg]++; __nTestFalseNeg[reg]++; __nTestNeg[reg]++; }
        inline void reduce_nTestFalseNegIn(int fac) { __nTestFalseNegIn[fac]--;__nTestFalseNeg[fac]--;__nTestNeg[fac]--; }
        inline void reduce_nTestFalseNegIn(int dist, int reg) { __nTestFalseNegIn[0]--; __nTestFalseNeg[0]--; __nTestNeg[0]--; __nTestFalseNegIn[dist]--; __nTestFalseNeg[dist]--; __nTestNeg[dist]--; __nTestFalseNegIn[reg]--; __nTestFalseNeg[reg]--; __nTestNeg[reg]--; }
        inline int return_nTestFalseNegIn(int index){return __nTestFalseNegIn[index];}
    inline void record_nTestFalseNegOut(int fac){__nTestFalseNegOut[fac]++;__nTestFalseNeg[fac]++;__nTestNeg[fac]++;}
    inline void record_nTestFalseNegOut(int dist, int reg) { __nTestFalseNegOut[0]++; __nTestFalseNeg[0]++; __nTestNeg[0]++; __nTestFalseNegOut[dist]++; __nTestFalseNeg[dist]++; __nTestNeg[dist]++; __nTestFalseNegOut[reg]++; __nTestFalseNeg[reg]++; __nTestNeg[reg]++; }
        inline int return_nTestFalseNegOut(int index){return __nTestFalseNegOut[index];}
    inline void record_nTestPosIn(int fac){__nTestPosIn[fac]++;}
    inline void record_nTestPosIn(int dist, int reg) { __nTestPosIn[0]++; __nTestPosIn[dist]++; __nTestPosIn[reg]++; }
        inline int return_nTestPosIn(int index){return __nTestPosIn[index];}
    inline void record_nTestPosOut(int fac){__nTestPosOut[fac]++;}
    inline void record_nTestPosOut(int dist, int reg) { __nTestPosOut[0]++; __nTestPosOut[dist]++; __nTestPosOut[reg]++; }
        inline int return_nTestPosOut(int index){return __nTestPosOut[index];}
    inline void record_nMDCDInfected(int fac) { __nMDCDInfected[fac]++;}
    inline void record_nMDCDInfected(int dist, int reg) { __nMDCDInfected[0]++; __nMDCDInfected[dist]++; __nMDCDInfected[reg]++; }
        inline int return_nMDCDInfected(int index) { return __nMDCDInfected[index]; }
    inline void record_nMDCDUninfected(int fac) { __nMDCDUninfected[fac]++;}
    inline void record_nMDCDUninfected(int dist, int reg) { __nMDCDUninfected[0]++; __nMDCDUninfected[dist]++; __nMDCDUninfected[reg]++; }
        inline int return_nMDCDUninfected(int index) { return __nMDCDUninfected[index]; }
    inline void record_nPTMDCDInfected(int fac) { __nPTMDCDInfected[fac]++;}
    inline void record_nPTMDCDInfected(int dist, int reg) { __nPTMDCDInfected[0]++; __nPTMDCDInfected[dist]++; __nPTMDCDInfected[reg]++; }
        inline int return_nPTMDCDInfected(int index) { return __nPTMDCDInfected[index]; }
    inline void record_nPTMDCDUninfected(int fac) { __nPTMDCDUninfected[fac]++;}
    inline void record_nPTMDCDUninfected(int dist, int reg) { __nPTMDCDUninfected[0]++; __nPTMDCDUninfected[dist]++; __nPTMDCDUninfected[reg]++; }
        inline int return_nPTMDCDUninfected(int index) { return __nPTMDCDUninfected[index]; }
    inline void record_nDiagnosis(int fac){__nDiagnosis[fac]++;}
    inline void record_nDiagnosis(int dist, int reg) { __nDiagnosis[0]++; __nDiagnosis[dist]++; __nDiagnosis[reg]++; }
        inline int return_nDiagnosis(int index){return __nDiagnosis[index];}
    inline void record_nDiagnosisIn(int fac){__nDiagnosisIn[fac]++;__nDiagnosis[fac]++;}
    inline void record_nDiagnosisIn(int dist, int reg) { __nDiagnosisIn[0]++; __nDiagnosis[0]++; __nDiagnosisIn[dist]++; __nDiagnosis[dist]++; __nDiagnosisIn[reg]++; __nDiagnosis[reg]++; }
        inline int return_nDiagnosisIn(int index){return __nDiagnosisIn[index];}
    inline void record_nDiagnosisOut(int fac){__nDiagnosisOut[fac]++;__nDiagnosis[fac]++;}
    inline void record_nDiagnosisOut(int dist, int reg) { __nDiagnosisOut[0]++; __nDiagnosis[0]++; __nDiagnosisOut[dist]++; __nDiagnosis[dist]++; __nDiagnosisOut[reg]++; __nDiagnosis[reg]++; }
        inline int return_nDiagnosisOut(int index){return __nDiagnosisOut[index];}
    inline void record_nDiagnosisDC(int fac) {__nDiagnosisDC[fac]++;}
    inline void record_nDiagnosisDC(int dist, int reg) { __nDiagnosisDC[0]++; __nDiagnosisDC[dist]++; __nDiagnosisDC[reg]++; }
        inline int return_nDiagnosisDC(int index) { return __nDiagnosisDC[index]; }
    inline void record_nDiagnosisDCTruePos(int fac) { __nDiagnosisDCTruePos[fac]++;}
    inline void record_nDiagnosisDCTruePos(int dist, int reg) { __nDiagnosisDCTruePos[0]++; __nDiagnosisDCTruePos[dist]++; __nDiagnosisDCTruePos[reg]++; }
        inline int return_nDiagnosisDCTruePos(int index) { return __nDiagnosisDCTruePos[index]; }
    inline void record_nDiagnosisDCFalsePos(int fac) { __nDiagnosisDCFalsePos[fac]++;}
    inline void record_nDiagnosisDCFalsePos(int dist, int reg) { __nDiagnosisDCFalsePos[0]++; __nDiagnosisDCFalsePos[dist]++; __nDiagnosisDCFalsePos[reg]++; }
        inline int return_nDiagnosisDCFalsePos(int index) { return __nDiagnosisDCFalsePos[index]; }
    inline void record_nPreTestDiagnosisTruePos(int fac) { __nPreTestDiagnosisTruePos[fac]++; __nMDDiagnosisReview[fac]++;__nMDDiagnosisTotal[fac]++;;}
    inline void record_nPreTestDiagnosisTruePos(int dist, int reg) { __nPreTestDiagnosisTruePos[0]++; __nPreTestDiagnosisTruePos[dist]++; __nPreTestDiagnosisTruePos[reg]++; __nMDDiagnosisReview[0]++; __nMDDiagnosisReview[dist]++; __nMDDiagnosisReview[reg]++; __nMDDiagnosisTotal[0]++; __nMDDiagnosisTotal[dist]++; __nMDDiagnosisTotal[reg]++; }
        inline int return_nPreTestDiagnosisTruePos(int index) { return __nPreTestDiagnosisTruePos[index]; }
    inline void record_nPreTestDiagnosisFalseNeg(int fac) { __nPreTestDiagnosisFalseNeg[fac]++; __nPreTestDiagnosisReview[fac]++;__nMDDiagnosisReview[fac]++;}
    inline void record_nPreTestDiagnosisFalseNeg(int dist, int reg) { __nPreTestDiagnosisFalseNeg[0]++; __nPreTestDiagnosisFalseNeg[dist]++; __nPreTestDiagnosisFalseNeg[reg]++;  __nPreTestDiagnosisReview[0]++; __nPreTestDiagnosisReview[dist]++; __nPreTestDiagnosisReview[reg]++; __nMDDiagnosisReview[0]++; __nMDDiagnosisReview[dist]++; __nMDDiagnosisReview[reg]++; }
        inline int return_nPreTestDiagnosisFalseNeg(int index) { return __nPreTestDiagnosisFalseNeg[index]; }
    inline void record_nPreTestDiagnosisTrueNeg(int fac) { __nPreTestDiagnosisTrueNeg[fac]++; __nPreTestDiagnosisReview[fac]++;__nMDDiagnosisReview[fac]++;}
    inline void record_nPreTestDiagnosisTrueNeg(int dist, int reg) { __nPreTestDiagnosisTrueNeg[0]++; __nPreTestDiagnosisTrueNeg[dist]++; __nPreTestDiagnosisTrueNeg[reg]++; __nPreTestDiagnosisReview[0]++; __nPreTestDiagnosisReview[dist]++; __nPreTestDiagnosisReview[reg]++; __nMDDiagnosisReview[0]++; __nMDDiagnosisReview[dist]++; __nMDDiagnosisReview[reg]++; }
        inline int return_nPreTestDiagnosisTrueNeg(int index) { return __nPreTestDiagnosisTrueNeg[index]; }
    inline void record_nPreTestDiagnosisFalsePos(int fac) { __nPreTestDiagnosisFalsePos[fac]++; ;__nMDDiagnosisReview[fac]++;__nMDDiagnosisTotal[fac]++;}
    inline void record_nPreTestDiagnosisFalsePos(int dist, int reg) { __nPreTestDiagnosisFalsePos[0]++; __nPreTestDiagnosisFalsePos[dist]++; __nPreTestDiagnosisFalsePos[reg]++; __nMDDiagnosisReview[0]++; __nMDDiagnosisReview[dist]++; __nMDDiagnosisReview[reg]++; __nMDDiagnosisTotal[0]++; __nMDDiagnosisTotal[dist]++; __nMDDiagnosisTotal[reg]++; }
        inline int return_nPreTestDiagnosisFalsePos(int index) { return __nPreTestDiagnosisFalsePos[index]; }
    inline void record_nPreTestDiagnosisTotal(int fac) { __nPreTestDiagnosisTotal[fac]++;}
    inline void record_nPreTestDiagnosisTotal(int dist, int reg) { __nPreTestDiagnosisTotal[0]++; __nPreTestDiagnosisTotal[dist]++; __nPreTestDiagnosisTotal[reg]++; }
        inline int return_nPreTestDiagnosisTotal(int index) { return __nPreTestDiagnosisTotal[index]; }
    inline void record_nPreTestDiagnosisReview(int fac) { __nPreTestDiagnosisReview[fac]++;}
    inline void record_nPreTestDiagnosisReview(int dist, int reg) { __nPreTestDiagnosisReview[0]++; __nPreTestDiagnosisReview[dist]++; __nPreTestDiagnosisReview[reg]++; }
        inline int return_nPreTestDiagnosisReview(int index) { return __nPreTestDiagnosisReview[index]; }
    inline void record_nPostTestDiagnosisTruePosIn(int fac) { __nPostTestDiagnosisTruePosIn[fac]++;__nPostTestDiagnosisReview[fac]++;__nPostTestDiagnosisTotal[fac]++;__nMDDiagnosisReview[fac]++;__nMDDiagnosisTotal[fac]++;}
    inline void record_nPostTestDiagnosisTruePosIn(int dist, int reg) {__nPostTestDiagnosisTruePosIn[0]++; __nPostTestDiagnosisTruePosIn[dist]++; __nPostTestDiagnosisTruePosIn[reg]++; __nPostTestDiagnosisReview[0]++; __nPostTestDiagnosisReview[dist]++; __nPostTestDiagnosisReview[reg]++; __nPostTestDiagnosisTotal[0]++; __nPostTestDiagnosisTotal[dist]++; __nPostTestDiagnosisTotal[reg]++; __nMDDiagnosisReview[0]++; __nMDDiagnosisReview[dist]++; __nMDDiagnosisReview[reg]++; __nMDDiagnosisTotal[0]++; __nMDDiagnosisTotal[dist]++; __nMDDiagnosisTotal[reg]++;}
        inline int return_nPostTestDiagnosisTruePosIn(int index) { return __nPostTestDiagnosisTruePosIn[index]; }
    inline void record_nPostTestDiagnosisTruePosOut(int fac) { __nPostTestDiagnosisTruePosOut[fac]++;__nPostTestDiagnosisReview[fac]++;__nPostTestDiagnosisTotal[fac]++;__nMDDiagnosisReview[fac]++;__nMDDiagnosisTotal[fac]++;}
    inline void record_nPostTestDiagnosisTruePosOut(int dist, int reg) {__nPostTestDiagnosisTruePosOut[0]++; __nPostTestDiagnosisTruePosOut[dist]++; __nPostTestDiagnosisTruePosOut[reg]++; __nPostTestDiagnosisReview[0]++; __nPostTestDiagnosisReview[dist]++; __nPostTestDiagnosisReview[reg]++; __nPostTestDiagnosisTotal[0]++; __nPostTestDiagnosisTotal[dist]++; __nPostTestDiagnosisTotal[reg]++; __nMDDiagnosisReview[0]++; __nMDDiagnosisReview[dist]++; __nMDDiagnosisReview[reg]++; __nMDDiagnosisTotal[0]++; __nMDDiagnosisTotal[dist]++; __nMDDiagnosisTotal[reg]++;}
        inline int return_nPostTestDiagnosisTruePosOut(int index) { return __nPostTestDiagnosisTruePosOut[index]; }
    inline void record_nPostTestDiagnosisFalseNeg(int fac) { __nPostTestDiagnosisFalseNeg[fac]++;__nPostTestDiagnosisReview[fac]++;__nMDDiagnosisReview[fac]++;}
    inline void record_nPostTestDiagnosisFalseNeg(int dist, int reg) { __nPostTestDiagnosisFalseNeg[0]++; __nPostTestDiagnosisFalseNeg[dist]++; __nPostTestDiagnosisFalseNeg[reg]++; __nPostTestDiagnosisReview[0]++; __nPostTestDiagnosisReview[dist]++; __nPostTestDiagnosisReview[reg]++; __nMDDiagnosisReview[0]++; __nMDDiagnosisReview[dist]++; __nMDDiagnosisReview[reg]++; }
        inline int return_nPostTestDiagnosisFalseNeg(int index) { return __nPostTestDiagnosisFalseNeg[index]; }
    inline void record_nPostTestDiagnosisTrueNeg(int fac) { __nPostTestDiagnosisTrueNeg[fac]++;__nPostTestDiagnosisReview[fac]++;__nMDDiagnosisReview[fac]++;}
    inline void record_nPostTestDiagnosisTrueNeg(int dist, int reg) { __nPostTestDiagnosisTrueNeg[0]++; __nPostTestDiagnosisTrueNeg[dist]++; __nPostTestDiagnosisTrueNeg[reg]++; __nPostTestDiagnosisReview[0]++; __nPostTestDiagnosisReview[dist]++; __nPostTestDiagnosisReview[reg]++; __nMDDiagnosisReview[0]++; __nMDDiagnosisReview[dist]++; __nMDDiagnosisReview[reg]++; }
        inline int return_nPostTestDiagnosisTrueNeg(int index) { return __nPostTestDiagnosisTrueNeg[index]; }
    inline void record_nPostTestDiagnosisFalsePosIn(int fac) { __nPostTestDiagnosisFalsePosIn[fac]++; __nPostTestDiagnosisReview[fac]++;__nPostTestDiagnosisTotal[fac]++;__nMDDiagnosisReview[fac]++;__nMDDiagnosisTotal[fac]++;}
    inline void record_nPostTestDiagnosisFalsePosIn(int dist, int reg) { __nPostTestDiagnosisFalsePosIn[0]++; __nPostTestDiagnosisFalsePosIn[dist]++; __nPostTestDiagnosisFalsePosIn[reg]++; __nPostTestDiagnosisReview[0]++; __nPostTestDiagnosisReview[dist]++; __nPostTestDiagnosisReview[reg]++; __nPostTestDiagnosisTotal[0]++; __nPostTestDiagnosisTotal[dist]++; __nPostTestDiagnosisTotal[reg]++; __nMDDiagnosisReview[0]++; __nMDDiagnosisReview[dist]++; __nMDDiagnosisReview[reg]++; __nMDDiagnosisTotal[0]++; __nMDDiagnosisTotal[dist]++; __nMDDiagnosisTotal[reg]++; }
        inline int return_nPostTestDiagnosisFalsePosIn(int index) { return __nPostTestDiagnosisFalsePosIn[index]; }
    inline void record_nPostTestDiagnosisFalsePosOut(int fac) { __nPostTestDiagnosisFalsePosOut[fac]++; __nPostTestDiagnosisReview[fac]++;__nPostTestDiagnosisTotal[fac]++;__nMDDiagnosisReview[fac]++; __nMDDiagnosisTotal[fac]++;}
    inline void record_nPostTestDiagnosisFalsePosOut(int dist, int reg) { __nPostTestDiagnosisFalsePosOut[0]++; __nPostTestDiagnosisFalsePosOut[dist]++; __nPostTestDiagnosisFalsePosOut[reg]++; __nPostTestDiagnosisReview[0]++; __nPostTestDiagnosisReview[dist]++; __nPostTestDiagnosisReview[reg]++; __nPostTestDiagnosisTotal[0]++; __nPostTestDiagnosisTotal[dist]++; __nPostTestDiagnosisTotal[reg]++; __nMDDiagnosisReview[0]++; __nMDDiagnosisReview[dist]++; __nMDDiagnosisReview[reg]++; __nMDDiagnosisTotal[0]++; __nMDDiagnosisTotal[dist]++; __nMDDiagnosisTotal[reg]++;}
        inline int return_nPostTestDiagnosisFalsePosOut(int index) { return __nPostTestDiagnosisFalsePosOut[index]; }
    inline void record_nPostTestDiagnosisTotal(int fac) { __nPostTestDiagnosisTotal[fac]++;}
    inline void record_nPostTestDiagnosisTotal(int dist, int reg) { __nPostTestDiagnosisTotal[0]++; __nPostTestDiagnosisTotal[dist]++; __nPostTestDiagnosisTotal[reg]++; }
        inline int return_nPostTestDiagnosisTotal(int index) { return __nPostTestDiagnosisTotal[index]; }
    inline void record_nPostTestDiagnosisReview(int fac) { __nPostTestDiagnosisReview[fac]++;}
    inline void record_nPostTestDiagnosisReview(int dist, int reg) { __nPostTestDiagnosisReview[0]++; __nPostTestDiagnosisReview[dist]++; __nPostTestDiagnosisReview[reg]++; }
        inline int return_nPostTestDiagnosisReview(int index) { return __nPostTestDiagnosisReview[index]; }
    inline void record_nMDDiagnosisTotal(int fac) { __nMDDiagnosisTotal[fac]++;}
    inline void record_nMDDiagnosisTotal(int dist, int reg) { __nMDDiagnosisTotal[0]++; __nMDDiagnosisTotal[dist]++; __nMDDiagnosisTotal[reg]++; }
        inline int return_nMDDiagnosisTotal(int index) { return __nMDDiagnosisTotal[index]; }
    inline void record_nMDDiagnosisReview(int fac) { __nMDDiagnosisReview[fac]++;}
    inline void record_nMDDiagnosisReview(int dist, int reg) { __nMDDiagnosisReview[0]++; __nMDDiagnosisReview[dist]++; __nMDDiagnosisReview[reg]++; }
        inline int return_nMDDiagnosisReview(int index) { return __nMDDiagnosisReview[index]; }
    inline void record_nPreTestLtFU(int fac){__nPreTestLtFU[fac]++;}
    inline void record_nPreTestLtFU(int dist, int reg) { __nPreTestLtFU[0]++; __nPreTestLtFU[dist]++; __nPreTestLtFU[reg]++; }
        inline int return_nPreTestLtFU(int index){return __nPreTestLtFU[index];}
    inline void record_nPreTestLtFUInf(int fac) { __nPreTestLtFUInf[fac]++;}
    inline void record_nPreTestLtFUInf(int dist, int reg) { __nPreTestLtFUInf[0]++; __nPreTestLtFUInf[dist]++; __nPreTestLtFUInf[reg]++; }
        inline int return_nPreTestLtFUInf(int index) { return __nPreTestLtFUInf[index]; }
    inline void record_nPreTestLtFUDied(int fac){__nPreTestLtFUDied[fac]++;}
    inline void record_nPreTestLtFUDied(int dist, int reg) { __nPreTestLtFUDied[0]++; __nPreTestLtFUDied[dist]++; __nPreTestLtFUDied[reg]++; }
        inline int return_nPreTestLtFUDied(int index){return __nPreTestLtFUDied[index];}
    inline void record_nPreTestLtFUCured(int fac) { __nPreTestLtFUCured[fac]++;}
    inline void record_nPreTestLtFUCured(int dist, int reg) { __nPreTestLtFUCured[0]++; __nPreTestLtFUCured[dist]++; __nPreTestLtFUCured[reg]++; }
        inline int return_nPreTestLtFUCured(int index) { return __nPreTestLtFUCured[index]; }
    inline void record_nPreTestLtFURepresent(int fac){__nPreTestLtFURepresent[fac]++;}
    inline void record_nPreTestLtFURepresent(int dist, int reg) { __nPreTestLtFURepresent[0]++; __nPreTestLtFURepresent[dist]++; __nPreTestLtFURepresent[reg]++; }
        inline int return_nPreTestLtFURepresent(int index){return __nPreTestLtFURepresent[index];}
    inline void record_nPreTreatLtFU(int fac){__nPreTreatLtFU[fac]++;}
    inline void record_nPreTreatLtFU(int dist, int reg) { __nPreTreatLtFU[0]++; __nPreTreatLtFU[dist]++; __nPreTreatLtFU[reg]++; }
        inline void reduce_nPreTreatLtFU(int fac) { __nPreTreatLtFU[fac]--;}
        inline void reduce_nPreTreatLtFU(int dist, int reg) { __nPreTreatLtFU[0]--; __nPreTreatLtFU[dist]--; __nPreTreatLtFU[reg]--; }
        inline int return_nPreTreatLtFU(int index){return __nPreTreatLtFU[index];}
    inline void record_nPreTreatLtFUInf(int fac) { __nPreTreatLtFUInf[fac]++;}
    inline void record_nPreTreatLtFUInf(int dist, int reg) { __nPreTreatLtFUInf[0]++; __nPreTreatLtFUInf[dist]++; __nPreTreatLtFUInf[reg]++; }
        inline int return_nPreTreatLtFUInf(int index) { return __nPreTreatLtFUInf[index]; }
    inline void record_nPreTreatLtFUDied(int fac){__nPreTreatLtFUDied[fac]++;}
    inline void record_nPreTreatLtFUDied(int dist, int reg) { __nPreTreatLtFUDied[0]++; __nPreTreatLtFUDied[dist]++; __nPreTreatLtFUDied[reg]++; }
        inline int return_nPreTreatLtFUDied(int index){return __nPreTreatLtFUDied[index];}
    inline void record_nPreTreatLtFUCured(int fac) { __nPreTreatLtFUCured[fac]++;}
    inline void record_nPreTreatLtFUCured(int dist, int reg) { __nPreTreatLtFUCured[0]++; __nPreTreatLtFUCured[dist]++; __nPreTreatLtFUCured[reg]++; }
        inline int return_nPreTreatLtFUCured(int index) { return __nPreTreatLtFUCured[index]; }
    inline void record_nPreTreatLtFURepresent(int fac){__nPreTreatLtFURepresent[fac]++;}
    inline void record_nPreTreatLtFURepresent(int dist, int reg) { __nPreTreatLtFURepresent[0]++; __nPreTreatLtFURepresent[dist]++; __nPreTreatLtFURepresent[reg]++; }
        inline int return_nPreTreatLtFURepresent(int index){return __nPreTreatLtFURepresent[index];}
    inline void record_nPreTreatLtFUFN(int fac) {__nPreTreatLtFUFN[fac]++;}
    inline void record_nPreTreatLtFUFN(int dist, int reg) { __nPreTreatLtFUFN[0]++; __nPreTreatLtFUFN[dist]++; __nPreTreatLtFUFN[reg]++; }
        inline int return_nPreTreatLtFUFN(int index) { return __nPreTreatLtFUFN[index]; }
    inline void record_nPreTreatLtFUFNDied(int fac) { __nPreTreatLtFUFNDied[fac]++;}
    inline void record_nPreTreatLtFUFNDied(int dist, int reg) { __nPreTreatLtFUFNDied[0]++; __nPreTreatLtFUFNDied[dist]++; __nPreTreatLtFUFNDied[reg]++; }
        inline int return_nPreTreatLtFUFNdied(int index) { return __nPreTreatLtFUFNDied[index]; }
    inline void record_nPreTreatLtFUFNCured(int fac) { __nPreTreatLtFUFNCured[fac]++;}
    inline void record_nPreTreatLtFUFNCured(int dist, int reg) { __nPreTreatLtFUFNCured[0]++; __nPreTreatLtFUFNCured[dist]++; __nPreTreatLtFUFNCured[reg]++; }
        inline int return_nPreTreatLtFUFNCured(int index) { return __nPreTreatLtFUFNCured[index]; }
    inline void record_nPreTreatLtFUFNRepresent(int fac) { __nPreTreatLtFUFNRepresent[fac]++;}
    inline void record_nPreTreatLtFUFNRepresent(int dist, int reg) { __nPreTreatLtFUFNRepresent[0]++; __nPreTreatLtFUFNRepresent[dist]++; __nPreTreatLtFUFNRepresent[reg]++; }
        inline int return_nPreTreatLtFUFNRepresent(int index) { return __nPreTreatLtFUFNRepresent[index]; }
    inline void record_nPreTreatLtFUPos(int fac) { __nPreTreatLtFUPos[fac]++;}
    inline void record_nPreTreatLtFUPos(int dist, int reg) { __nPreTreatLtFUPos[0]++; __nPreTreatLtFUPos[dist]++; __nPreTreatLtFUPos[reg]++; }
        inline int return_nPreTreatLtFUPos(int index) { return __nPreTreatLtFUPos[index]; }
    inline void record_nPreTreatLtFUNeg(int fac) { __nPreTreatLtFUNeg[fac]++;}
    inline void record_nPreTreatLtFUNeg(int dist, int reg) { __nPreTreatLtFUNeg[0]++; __nPreTreatLtFUNeg[dist]++; __nPreTreatLtFUNeg[reg]++; }
        inline int return_nPreTreatLtFUNeg(int index) { return __nPreTreatLtFUNeg[index]; }
    inline void record_nTreatLtFU(int fac){__nTreatLtFU[fac]++;}
    inline void record_nTreatLtFU(int dist, int reg) { __nTreatLtFU[0]++; __nTreatLtFU[dist]++; __nTreatLtFU[reg]++; }
        inline int return_nTreatLtFU(int index){return __nTreatLtFU[index];}
    inline void record_nTreatLtFUInf(int fac) { __nTreatLtFUInf[fac]++;}
    inline void record_nTreatLtFUInf(int dist, int reg) { __nTreatLtFUInf[0]++; __nTreatLtFUInf[dist]++; __nTreatLtFUInf[reg]++; }
        inline int return_nTreatLtFUInf(int index) { return __nTreatLtFUInf[index]; }
    inline void record_nTreatLtFUDied(int fac){__nTreatLtFUDied[fac]++;}
    inline void record_nTreatLtFUDied(int dist, int reg) { __nTreatLtFUDied[0]++; __nTreatLtFUDied[dist]++; __nTreatLtFUDied[reg]++; }
        inline int return_nTreatLtFUDied(int index){return __nTreatLtFUDied[index];}
    inline void record_nTreatLtFUCured(int fac) { __nTreatLtFUCured[fac]++;}
    inline void record_nTreatLtFUCured(int dist, int reg) { __nTreatLtFUCured[0]++; __nTreatLtFUCured[dist]++; __nTreatLtFUCured[reg]++; }
        inline int return_nTreatLtFUCured(int index) { return __nTreatLtFUCured[index]; }
    inline void record_nTreatLtFURepresent(int fac){__nTreatLtFURepresent[fac]++;}
    inline void record_nTreatLtFURepresent(int dist, int reg) { __nTreatLtFURepresent[0]++; __nTreatLtFURepresent[dist]++; __nTreatLtFURepresent[reg]++; }
        inline int return_nTreatLtFURepresent(int index){return __nTreatLtFURepresent[index];}
    inline void record_nPatientsTreated(int fac){__nPatientsTreated[fac]++;}
    inline void record_nPatientsTreated(int dist, int reg) { __nPatientsTreated[0]++; __nPatientsTreated[dist]++; __nPatientsTreated[reg]++; }
        inline int return_nPatientsTreated(int index){return __nPatientsTreated[index];}
    inline void record_nPatientsTreatedInf(int fac){__nPatientsTreatedInf[fac]++;}
    inline void record_nPatientsTreatedInf(int dist, int reg) { __nPatientsTreatedInf[0]++; __nPatientsTreatedInf[dist]++; __nPatientsTreatedInf[reg]++; }
        inline int return_nPatientsTreatedInf(int index){return __nPatientsTreatedInf[index];}
    inline void record_nPatientsTreatedUninf(int fac){__nPatientsTreatedUninf[fac]++;}
    inline void record_nPatientsTreatedUninf(int dist, int reg) { __nPatientsTreatedUninf[0]++; __nPatientsTreatedUninf[dist]++; __nPatientsTreatedUninf[reg]++; }
        inline int return_nPatientsTreatedUninf(int index){return __nPatientsTreatedUninf[index];}
    inline void record_nPatientsPreTreatedLtoFUninf(int fac){__nPatientsPreTreatedLtoFUninf[fac]++;}
    inline void record_nPatientsPreTreatedLtoFUninf(int dist, int reg) { __nPatientsPreTreatedLtoFUninf[0]++; __nPatientsPreTreatedLtoFUninf[dist]++; __nPatientsPreTreatedLtoFUninf[reg]++; }
        inline int return_nPatientsPreTreatedLtoFUninf(int index){return __nPatientsPreTreatedLtoFUninf[index];}
    inline void record_nPatientsTreatedCured(int fac){__nPatientsTreatedCured[fac]++;}
    inline void record_nPatientsTreatedCured(int dist, int reg) { __nPatientsTreatedCured[0]++; __nPatientsTreatedCured[dist]++; __nPatientsTreatedCured[reg]++; }
        inline int return_nPatientsTreatedCured(int index){return __nPatientsTreatedCured[index];}
    inline void record_nPatientsTreatedDied(int fac){__nPatientsTreatedDied[fac]++;}
    inline void record_nPatientsTreatedDied(int dist, int reg) { __nPatientsTreatedDied[0]++; __nPatientsTreatedDied[dist]++; __nPatientsTreatedDied[reg]++; }
        inline int return_nPatientsTreatedDied(int index){return __nPatientsTreatedDied[index];}
    inline void record_nPatientsTreatedRelapse(int fac){__nPatientsTreatedRelapse[fac]++;}
    inline void record_nPatientsTreatedRelapse(int dist, int reg) { __nPatientsTreatedRelapse[0]++; __nPatientsTreatedRelapse[dist]++; __nPatientsTreatedRelapse[reg]++; }
        inline int return_nPatientsTreatedRelapse(int index){return __nPatientsTreatedRelapse[index];}
    inline void record_nTreatedRelapse(int fac){__nTreatedRelapse[fac]++;}
    inline void record_nTreatedRelapse(int dist, int reg) { __nTreatedRelapse[0]++; __nTreatedRelapse[dist]++; __nTreatedRelapse[reg]++; }
        inline int return_nTreatedRelapse(int index){return __nTreatedRelapse[index];}
    inline void record_nTreatedRelapseCured(int fac){__nTreatedRelapseCured[fac]++;__nTreatedRelapse[fac]++;}
    inline void record_nTreatedRelapseCured(int dist, int reg) { __nTreatedRelapseCured[0]++; __nTreatedRelapse[0]++; __nTreatedRelapseCured[dist]++; __nTreatedRelapse[dist]++; __nTreatedRelapseCured[reg]++; __nTreatedRelapse[reg]++; }
        inline int return_nTreatedRelapseCured(int index){return __nTreatedRelapseCured[index];}
    inline void record_nTreatedRelapseDied(int fac){__nTreatedRelapseDied[fac]++;__nTreatedRelapse[fac]++;}
    inline void record_nTreatedRelapseDied(int dist, int reg) { __nTreatedRelapseDied[0]++; __nTreatedRelapse[0]++; __nTreatedRelapseDied[dist]++; __nTreatedRelapse[dist]++; __nTreatedRelapseDied[reg]++; __nTreatedRelapse[reg]++; }
        inline int return_nTreatedRelapseDied(int index){return __nTreatedRelapseDied[index];}
    inline void record_timePresentToSampleDiagIn(int time, int fac){__timePresentToSampleDiagIn[fac] = __timePresentToSampleDiagIn[fac] + time;}
    inline void record_timePresentToSampleDiagIn(int time, int dist, int reg) { __timePresentToSampleDiagIn[0] = __timePresentToSampleDiagIn[0] + time; __timePresentToSampleDiagIn[dist] = __timePresentToSampleDiagIn[dist] + time; __timePresentToSampleDiagIn[reg] = __timePresentToSampleDiagIn[reg] + time; }
        inline int return_timePresentToSampleDiagIn(int index){return __timePresentToSampleDiagIn[index];}
    inline void record_nPresentToSampleDiagIn(int fac){__nPresentToSampleDiagIn[fac]++;}
    inline void record_nPresentToSampleDiagIn(int dist, int reg) { __nPresentToSampleDiagIn[0]++; __nPresentToSampleDiagIn[dist]++; __nPresentToSampleDiagIn[reg]++; }
        inline int return_nPresentToSampleDiagIn(int index){return __nPresentToSampleDiagIn[index];}
    inline void record_timePresentToSampleDiagOut(int time, int fac){__timePresentToSampleDiagOut[fac] = __timePresentToSampleDiagOut[fac] + time;}
    inline void record_timePresentToSampleDiagOut(int time, int dist, int reg) { __timePresentToSampleDiagOut[0] = __timePresentToSampleDiagOut[0] + time; __timePresentToSampleDiagOut[dist] = __timePresentToSampleDiagOut[dist] + time; __timePresentToSampleDiagOut[reg] = __timePresentToSampleDiagOut[reg] + time; }
        inline int return_timePresentToSampleDiagOut(int index){return __timePresentToSampleDiagOut[index];}
    inline void record_nPresentToSampleDiagOut(int fac){__nPresentToSampleDiagOut[fac]++;}
    inline void record_nPresentToSampleDiagOut(int dist, int reg) { __nPresentToSampleDiagOut[0]++; __nPresentToSampleDiagOut[dist]++; __nPresentToSampleDiagOut[reg]++; }
        inline int return_nPresentToSampleDiagOut(int index){return __nPresentToSampleDiagOut[index];}
    inline void record_timePresentToSampleTreat(int time, int fac){__timePresentToSampleTreat[fac] = __timePresentToSampleTreat[fac] + time;}
    inline void record_timePresentToSampleTreat(int time, int dist, int reg) { __timePresentToSampleTreat[0] = __timePresentToSampleTreat[0] + time; __timePresentToSampleTreat[dist] = __timePresentToSampleTreat[dist] + time; __timePresentToSampleTreat[reg] = __timePresentToSampleTreat[reg] + time; }
        inline int return_timePresentToSampleTreat(int index){return __timePresentToSampleTreat[index];}
    inline void record_nPresentToSampleTreat(int fac){__nPresentToSampleTreat[fac]++;}
    inline void record_nPresentToSampleTreat(int dist, int reg) { __nPresentToSampleTreat[0]++; __nPresentToSampleTreat[dist]++; __nPresentToSampleTreat[reg]++; }
        inline int return_nPresentToSampleTreat(int index){return __nPresentToSampleTreat[index];}
    inline void record_timePresentToSampleTreatIn(int time, int fac){__timePresentToSampleTreat[0] = __timePresentToSampleTreat[0] + time;__timePresentToSampleTreatIn[fac] = __timePresentToSampleTreatIn[fac] + time;__timePresentToSampleTreat[fac] = __timePresentToSampleTreat[fac] + time;}
    inline void record_timePresentToSampleTreatIn(int time, int dist, int reg) { __timePresentToSampleTreatIn[0] = __timePresentToSampleTreatIn[0] + time; __timePresentToSampleTreat[0] = __timePresentToSampleTreat[0] + time; __timePresentToSampleTreatIn[dist] = __timePresentToSampleTreatIn[dist] + time; __timePresentToSampleTreat[dist] = __timePresentToSampleTreat[dist] + time; __timePresentToSampleTreatIn[reg] = __timePresentToSampleTreatIn[reg] + time; __timePresentToSampleTreat[reg] = __timePresentToSampleTreat[reg] + time; }
        inline int return_timePresentToSampleTreatIn(int index){return __timePresentToSampleTreatIn[index];}
    inline void record_nPresentToSampleTreatIn(int fac){__nPresentToSampleTreatIn[fac]++;__nPresentToSampleTreat[fac]++;}
    inline void record_nPresentToSampleTreatIn(int dist, int reg) { __nPresentToSampleTreatIn[0]++; __nPresentToSampleTreat[0]++; __nPresentToSampleTreatIn[dist]++; __nPresentToSampleTreat[dist]++; __nPresentToSampleTreatIn[reg]++; __nPresentToSampleTreat[reg]++; }
        inline int return_nPresentToSampleTreatIn(int index){return __nPresentToSampleTreatIn[index];}
    inline void record_timePresentToSampleTreatOut(int time, int fac){__timePresentToSampleTreat[0] = __timePresentToSampleTreat[0] + time;__timePresentToSampleTreatOut[fac] = __timePresentToSampleTreatOut[fac] + time;__timePresentToSampleTreat[fac] = __timePresentToSampleTreat[fac] + time;}
    inline void record_timePresentToSampleTreatOut(int time, int dist, int reg) { __timePresentToSampleTreatOut[0] = __timePresentToSampleTreatOut[0] + time; __timePresentToSampleTreat[0] = __timePresentToSampleTreat[0] + time; __timePresentToSampleTreatOut[dist] = __timePresentToSampleTreatOut[dist] + time; __timePresentToSampleTreat[dist] = __timePresentToSampleTreat[dist] + time; __timePresentToSampleTreatOut[reg] = __timePresentToSampleTreatOut[reg] + time; __timePresentToSampleTreat[reg] = __timePresentToSampleTreat[reg] + time; }
        inline int return_timePresentToSampleTreatOut(int index){return __timePresentToSampleTreatOut[index];}
    inline void record_nPresentToSampleTreatOut(int fac){__nPresentToSampleTreatOut[fac]++;__nPresentToSampleTreat[fac]++;}
    inline void record_nPresentToSampleTreatOut(int dist, int reg) { __nPresentToSampleTreatOut[0]++; __nPresentToSampleTreat[0]++; __nPresentToSampleTreatOut[dist]++; __nPresentToSampleTreat[dist]++; __nPresentToSampleTreatOut[reg]++; __nPresentToSampleTreat[reg]++; }
        inline int return_nPresentToSampleTreatOut(int index){return __nPresentToSampleTreatOut[index];}
    inline void record_timePresentToSampleDeath(int time, int fac){__timePresentToSampleDeath[fac] = __timePresentToSampleDeath[fac] + time;}
    inline void record_timePresentToSampleDeath(int time, int dist, int reg) { __timePresentToSampleDeath[0] = __timePresentToSampleDeath[0] + time; __timePresentToSampleDeath[dist] = __timePresentToSampleDeath[dist] + time; __timePresentToSampleDeath[reg] = __timePresentToSampleDeath[reg] + time; }
        inline int return_timePresentToSampleDeath(int index){return __timePresentToSampleDeath[index];}
    inline void record_nPresentToSampleDeath(int fac){__nPresentToSampleDeath[fac]++;}
    inline void record_nPresentToSampleDeath(int dist, int reg) { __nPresentToSampleDeath[0]++; __nPresentToSampleDeath[dist]++; __nPresentToSampleDeath[reg]++; }
        inline int return_nPresentToSampleDeath(int index){return __nPresentToSampleDeath[index];}
    inline void record_timePresentToSampleDeathIn(int time, int fac){__timePresentToSampleDeath[0] = __timePresentToSampleDeath[0] + time;__timePresentToSampleDeathIn[fac] = __timePresentToSampleDeathIn[fac] + time;__timePresentToSampleDeath[fac] = __timePresentToSampleDeath[fac] + time;}
    inline void record_timePresentToSampleDeathIn(int time, int dist, int reg) { __timePresentToSampleDeathIn[0] = __timePresentToSampleDeathIn[0] + time; __timePresentToSampleDeath[0] = __timePresentToSampleDeath[0] + time; __timePresentToSampleDeathIn[dist] = __timePresentToSampleDeathIn[dist] + time; __timePresentToSampleDeath[dist] = __timePresentToSampleDeath[dist] + time; __timePresentToSampleDeathIn[reg] = __timePresentToSampleDeathIn[reg] + time; __timePresentToSampleDeath[reg] = __timePresentToSampleDeath[reg] + time; }
        inline int return_timePresentToSampleDeathIn(int index){return __timePresentToSampleDeath[index];}
    inline void record_nPresentToSampleDeathIn(int fac){__nPresentToSampleDeathIn[fac]++;__nPresentToSampleDeath[fac]++;}
    inline void record_nPresentToSampleDeathIn(int dist, int reg) { __nPresentToSampleDeathIn[0]++; __nPresentToSampleDeath[0]++; __nPresentToSampleDeathIn[dist]++; __nPresentToSampleDeath[dist]++; __nPresentToSampleDeathIn[reg]++; __nPresentToSampleDeath[reg]++; }
        inline int return_nPresentToSampleDeathIn(int index){return __nPresentToSampleDeathIn[index];}
    inline void record_timePresentToSampleDeathOut(int time, int fac){__timePresentToSampleDeathOut[fac] = __timePresentToSampleDeathOut[fac] + time;__timePresentToSampleDeath[fac] = __timePresentToSampleDeath[fac] + time;}
    inline void record_timePresentToSampleDeathOut(int time, int dist, int reg) { __timePresentToSampleDeathOut[0] = __timePresentToSampleDeathOut[0] + time; __timePresentToSampleDeath[0] = __timePresentToSampleDeath[0] + time; __timePresentToSampleDeathOut[dist] = __timePresentToSampleDeathOut[dist] + time; __timePresentToSampleDeath[dist] = __timePresentToSampleDeath[dist] + time; __timePresentToSampleDeathOut[reg] = __timePresentToSampleDeathOut[reg] + time; __timePresentToSampleDeath[reg] = __timePresentToSampleDeath[reg] + time; }
        inline int return_timePresentToSampleDeathOut(int index){return __timePresentToSampleDeathOut[index];}
    inline void record_nPresentToSampleDeathOut(int fac){__nPresentToSampleDeathOut[fac]++;__nPresentToSampleDeath[fac]++;}
    inline void record_nPresentToSampleDeathOut(int dist, int reg) { __nPresentToSampleDeathOut[0]++; __nPresentToSampleDeath[0]++; __nPresentToSampleDeathOut[dist]++; __nPresentToSampleDeath[dist]++; __nPresentToSampleDeathOut[reg]++; __nPresentToSampleDeath[reg]++; }
        inline int return_nPresentToSampleDeathOut(int index){return __nPresentToSampleDeathOut[index];}
    inline void record_timePresentToSampleDeathPreTestLTFU(int time, int fac){__timePresentToSampleDeathPreTestLTFU[fac] = __timePresentToSampleDeathPreTestLTFU[fac] + time;}
    inline void record_timePresentToSampleDeathPreTestLTFU(int time, int dist, int reg) { __timePresentToSampleDeathPreTestLTFU[0] = __timePresentToSampleDeathPreTestLTFU[0] + time; __timePresentToSampleDeathPreTestLTFU[dist] = __timePresentToSampleDeathPreTestLTFU[dist] + time; __timePresentToSampleDeathPreTestLTFU[reg] = __timePresentToSampleDeathPreTestLTFU[reg] + time; }
        inline int return_timePresentToSampleDeathPreTestLTFU(int index){return __timePresentToSampleDeathPreTestLTFU[index];}
    inline void record_nPresentToSampleDeathPreTestLTFU(int fac){__nPresentToSampleDeathPreTestLTFU[fac]++;}
    inline void record_nPresentToSampleDeathPreTestLTFU(int dist, int reg) { __nPresentToSampleDeathPreTestLTFU[0]++; __nPresentToSampleDeathPreTestLTFU[dist]++; __nPresentToSampleDeathPreTestLTFU[reg]++; }
        inline int return_nPresentToSampleDeathPreTestLTFU(int index){return __nPresentToSampleDeathPreTestLTFU[index];}
    inline void record_timePresentToSampleDeathPreTreatLTFU(int time, int fac){__timePresentToSampleDeathPreTreatLTFU[fac] = __timePresentToSampleDeathPreTreatLTFU[fac] + time;}
    inline void record_timePresentToSampleDeathPreTreatLTFU(int time, int dist, int reg) { __timePresentToSampleDeathPreTreatLTFU[0] = __timePresentToSampleDeathPreTreatLTFU[0] + time; __timePresentToSampleDeathPreTreatLTFU[dist] = __timePresentToSampleDeathPreTreatLTFU[dist] + time; __timePresentToSampleDeathPreTreatLTFU[reg] = __timePresentToSampleDeathPreTreatLTFU[reg] + time; }
        inline int return_timePresentToSampleDeathPreTreatLTFU(int index){return __timePresentToSampleDeathPreTreatLTFU[index];}
    inline void record_nPresentToSampleDeathPreTreatLTFU(int fac){__nPresentToSampleDeathPreTreatLTFU[fac]++;}
    inline void record_nPresentToSampleDeathPreTreatLTFU(int dist, int reg) { __nPresentToSampleDeathPreTreatLTFU[0]++; __nPresentToSampleDeathPreTreatLTFU[dist]++; __nPresentToSampleDeathPreTreatLTFU[reg]++; }
        inline int return_nPresentToSampleDeathPreTreatLTFU(int index){return __nPresentToSampleDeathPreTreatLTFU[index];}
    inline void record_timePresentToSampleDeathTreatLTFU(int time, int fac){__timePresentToSampleDeathTreatLTFU[fac] = __timePresentToSampleDeathTreatLTFU[fac] + time;}
    inline void record_timePresentToSampleDeathTreatLTFU(int time, int dist, int reg) { __timePresentToSampleDeathTreatLTFU[0] = __timePresentToSampleDeathTreatLTFU[0] + time; __timePresentToSampleDeathTreatLTFU[dist] = __timePresentToSampleDeathTreatLTFU[dist] + time; __timePresentToSampleDeathTreatLTFU[reg] = __timePresentToSampleDeathTreatLTFU[reg] + time; }
        inline int return_timePresentToSampleDeathTreatLTFU(int index){return __timePresentToSampleDeathTreatLTFU[index];}
    inline void record_nPresentToSampleDeathTreatLTFU(int fac){__nPresentToSampleDeathTreatLTFU[fac]++;}
    inline void record_nPresentToSampleDeathTreatLTFU(int dist, int reg) { __nPresentToSampleDeathTreatLTFU[0]++; __nPresentToSampleDeathTreatLTFU[dist]++; __nPresentToSampleDeathTreatLTFU[reg]++; }
        inline int return_nPresentToSampleDeathTreatLTFU(int index){return __nPresentToSampleDeathTreatLTFU[index];}
    inline void record_timePresentToSampleDeathInTreat(int time, int fac){__timePresentToSampleDeathInTreat[fac] = __timePresentToSampleDeathInTreat[fac] + time;}
    inline void record_timePresentToSampleDeathInTreat(int time, int dist, int reg) { __timePresentToSampleDeathInTreat[0] = __timePresentToSampleDeathInTreat[0] + time; __timePresentToSampleDeathInTreat[dist] = __timePresentToSampleDeathInTreat[dist] + time; __timePresentToSampleDeathInTreat[reg] = __timePresentToSampleDeathInTreat[reg] + time; }
        inline int return_timePresentToSampleDeathInTreat(int index){return __timePresentToSampleDeathInTreat[index];}
    inline void record_nPresentToSampleDeathInTreat(int fac){__nPresentToSampleDeathInTreat[fac]++;}
    inline void record_nPresentToSampleDeathInTreat(int dist, int reg) { __nPresentToSampleDeathInTreat[0]++; __nPresentToSampleDeathInTreat[dist]++; __nPresentToSampleDeathInTreat[reg]++; }
        inline int return_nPresentToSampleDeathInTreat(int index){return __nPresentToSampleDeathInTreat[index];}
    inline void record_timePresentToSampleDeathInRelapse(int time, int fac){__timePresentToSampleDeathInRelapse[fac] = __timePresentToSampleDeathInRelapse[fac] + time;}
    inline void record_timePresentToSampleDeathInRelapse(int time, int dist, int reg) { __timePresentToSampleDeathInRelapse[0] = __timePresentToSampleDeathInRelapse[0] + time; __timePresentToSampleDeathInRelapse[dist] = __timePresentToSampleDeathInRelapse[dist] + time; __timePresentToSampleDeathInRelapse[reg] = __timePresentToSampleDeathInRelapse[reg] + time; }
        inline int return_timePresentToSampleDeathInRelapse(int index){return __timePresentToSampleDeathInRelapse[index];}
    inline void record_nPresentToSampleDeathInRelapse(int fac){__nPresentToSampleDeathInRelapse[fac]++;}
    inline void record_nPresentToSampleDeathInRelapse(int dist, int reg) { __nPresentToSampleDeathInRelapse[0]++; __nPresentToSampleDeathInRelapse[dist]++; __nPresentToSampleDeathInRelapse[reg]++; }
        inline int return_nPresentToSampleDeathInRelapse(int index){return __nPresentToSampleDeathInRelapse[index];}
    inline void record_timePresentToExit(int time, int fac){__timePresentToExit[fac] = __timePresentToExit[fac] + time;}
    inline void record_timePresentToExit(int time, int dist, int reg) { __timePresentToExit[0] = __timePresentToExit[0] + time; __timePresentToExit[dist] = __timePresentToExit[dist] + time; __timePresentToExit[reg] = __timePresentToExit[reg] + time; }
        inline int return_timePresentToExit(int index){return __timePresentToExit[index];}
    inline void record_nPresentToExit(int fac){__nPresentToExit[fac]++;}
    inline void record_nPresentToExit(int dist, int reg) { __nPresentToExit[0]++; __nPresentToExit[dist]++; __nPresentToExit[reg]++; }
        inline int return_nPresentToExit(int index){return __nPresentToExit[index];}
    inline void record_nXpert(int fac){__nXpert[fac]++;}
    inline void record_nXpert(int dist, int reg) { __nXpert[0]++; __nXpert[dist]++; __nXpert[reg]++; }
        inline int return_nXpert(int index){return __nXpert[index];}
    inline void record_nXpertIn(int fac){__nXpertIn[fac]++;__nXpert[fac]++;}
    inline void record_nXpertIn(int dist, int reg) { __nXpertIn[0]++; __nXpert[0]++; __nXpertIn[dist]++; __nXpert[dist]++; __nXpertIn[reg]++; __nXpert[reg]++; }
        inline int return_nXpertIn(int index){return __nXpertIn[index];}
    inline void record_nXpertOut(int fac){__nXpertOut[fac]++;__nXpert[fac]++;}
    inline void record_nXpertOut(int dist, int reg) { __nXpertOut[0]++; __nXpert[0]++; __nXpertOut[dist]++; __nXpert[dist]++; __nXpertOut[reg]++; __nXpert[reg]++; }
        inline int return_nXpertOut(int index){return __nXpertOut[index];}
    inline void record_nXpertPos(int fac){__nXpertPos[fac]++;}
    inline void record_nXpertPos(int dist, int reg) { __nXpertPos[0]++; __nXpertPos[dist]++; __nXpertPos[reg]++; }
        inline int return_nXpertPos(int index){return __nXpertPos[index];}
    inline void record_nXpertTruePos(int fac){__nXpertTruePos[fac]++;__nXpertPos[fac]++;}
    inline void record_nXpertTruePos(int dist, int reg) { __nXpertTruePos[0]++; __nXpertPos[0]++; __nXpertTruePos[dist]++; __nXpertPos[dist]++; __nXpertTruePos[reg]++; __nXpertPos[reg]++; }
        inline int return_nXpertTruePos(int index){return __nXpertTruePos[index];}
    inline void record_nXpertFalsePos(int fac){__nXpertFalsePos[fac]++;__nXpertPos[fac]++;}
    inline void record_nXpertFalsePos(int dist, int reg) { __nXpertFalsePos[0]++; __nXpertPos[0]++; __nXpertFalsePos[dist]++; __nXpertPos[dist]++; __nXpertFalsePos[reg]++; __nXpertPos[reg]++; }
        inline int return_nXpertFalsePos(int index){return __nXpertFalsePos[index];}
    inline void record_nXpertNeg(int fac){__nXpertNeg[fac]++;}
    inline void record_nXpertNeg(int dist, int reg) { __nXpertNeg[0]++; __nXpertNeg[dist]++; __nXpertNeg[reg]++; }
        inline int return_nXpertNeg(int index){return __nXpertNeg[index];}
    inline void record_nXpertTrueNeg(int fac){__nXpertTrueNeg[fac]++;__nXpertNeg[fac]++;}
    inline void record_nXpertTrueNeg(int dist, int reg) { __nXpertTrueNeg[0]++; __nXpertNeg[0]++; __nXpertTrueNeg[dist]++; __nXpertNeg[dist]++; __nXpertTrueNeg[reg]++; __nXpertNeg[reg]++; }
        inline int return_nXpertTrueNeg(int index){return __nXpertTrueNeg[index];}
    inline void record_nXpertFalseNeg(int fac){__nXpertFalseNeg[fac]++;__nXpertNeg[fac]++;}
    inline void record_nXpertFalseNeg(int dist, int reg) { __nXpertFalseNeg[0]++; __nXpertNeg[0]++; __nXpertFalseNeg[dist]++; __nXpertNeg[dist]++; __nXpertFalseNeg[reg]++; __nXpertNeg[reg]++; }
        inline int return_nXpertFalseNeg(int index){return __nXpertFalseNeg[index];}
    inline void record_nSmear(int fac){__nSmear[fac]++;}
    inline void record_nSmear(int dist, int reg) { __nSmear[0]++; __nSmear[dist]++; __nSmear[reg]++; }
        inline int return_nSmear(int index){return __nSmear[index];}
    inline void record_nSmearIn(int fac){__nSmearIn[fac]++;__nSmear[fac]++;}
    inline void record_nSmearIn(int dist, int reg) { __nSmearIn[0]++; __nSmear[0]++; __nSmearIn[dist]++; __nSmear[dist]++; __nSmearIn[reg]++; __nSmear[reg]++; }
        inline int return_nSmearIn(int index){return __nSmearIn[index];}
    inline void record_nSmearOut(int fac){__nSmearOut[fac]++;__nSmear[fac]++;}
    inline void record_nSmearOut(int dist, int reg) { __nSmearOut[0]++; __nSmear[0]++; __nSmearOut[dist]++; __nSmear[dist]++; __nSmearOut[reg]++; __nSmear[reg]++; }
        inline int return_nSmearOut(int index){return __nSmearOut[index];}
    inline void record_nSmearPos(int fac){__nSmearPos[fac]++;}
    inline void record_nSmearPos(int dist, int reg) { __nSmearPos[0]++; __nSmearPos[dist]++; __nSmearPos[reg]++; }
        inline int return_nSmearPos(int index){return __nSmearPos[index];}
    inline void record_nSmearTruePos(int fac){__nSmearTruePos[fac]++;__nSmearPos[fac]++;}
    inline void record_nSmearTruePos(int dist, int reg) { __nSmearTruePos[0]++; __nSmearPos[0]++; __nSmearTruePos[dist]++; __nSmearPos[dist]++; __nSmearTruePos[reg]++; __nSmearPos[reg]++; }
        inline int return_nSmearTruePos(int index){return __nSmearTruePos[index];}
    inline void record_nSmearFalsePos(int fac){__nSmearFalsePos[fac]++;__nSmearPos[fac]++;}
    inline void record_nSmearFalsePos(int dist, int reg) { __nSmearFalsePos[0]++; __nSmearPos[0]++; __nSmearFalsePos[dist]++; __nSmearPos[dist]++; __nSmearFalsePos[reg]++; __nSmearPos[reg]++; }
        inline int return_nSmearFalsePos(int index){return __nSmearFalsePos[index];}
    inline void record_nSmearNeg(int fac){__nSmearNeg[fac]++;}
    inline void record_nSmearNeg(int dist, int reg) { __nSmearNeg[0]++; __nSmearNeg[dist]++; __nSmearNeg[reg]++; }
        inline int return_nSmearNeg(int index){return __nSmearNeg[index];}
    inline void record_nSmearTrueNeg(int fac){__nSmearTrueNeg[fac]++;__nSmearNeg[fac]++;}
    inline void record_nSmearTrueNeg(int dist, int reg) { __nSmearTrueNeg[0]++; __nSmearNeg[0]++; __nSmearTrueNeg[dist]++; __nSmearNeg[dist]++; __nSmearTrueNeg[reg]++; __nSmearNeg[reg]++; }
        inline int return_nSmearTrueNeg(int index){return __nSmearTrueNeg[index];}
    inline void record_nSmearFalseNeg(int fac){__nSmearFalseNeg[fac]++;__nSmearNeg[fac]++;}
    inline void record_nSmearFalseNeg(int dist, int reg) { __nSmearFalseNeg[0]++; __nSmearNeg[0]++; __nSmearFalseNeg[dist]++; __nSmearNeg[dist]++; __nSmearFalseNeg[reg]++; __nSmearNeg[reg]++; }
        inline int return_nSmearFalseNeg(int index){return __nSmearFalseNeg[index];}
    inline void record_nDeathPreTest(int fac){__nDeathPreTest[fac]++;}
    inline void record_nDeathPreTest(int dist, int reg) { __nDeathPreTest[0]++; __nDeathPreTest[dist]++; __nDeathPreTest[reg]++; }
        inline int return_nDeathPreTest(int index){return __nDeathPreTest[index];}
    inline void record_nDeathPreTreat(int fac){__nDeathPreTreat[fac]++;}
    inline void record_nDeathPreTreat(int dist, int reg) { __nDeathPreTreat[0]++; __nDeathPreTreat[dist]++; __nDeathPreTreat[reg]++; }
        inline int return_nDeathPreTreat(int index){return __nDeathPreTreat[index];}
    inline void record_nDeathTreat(int fac){__nDeathTreat[fac]++;}
    inline void record_nDeathTreat(int dist, int reg) { __nDeathTreat[0]++; __nDeathTreat[dist]++; __nDeathTreat[reg]++; }
        inline int return_nDeathTreat(int index){return __nDeathTreat[index];}
    inline void record_nDeathReTreat(int fac){__nDeathReTreat[fac]++;}
    inline void record_nDeathReTreat(int dist, int reg) { __nDeathReTreat[0]++; __nDeathReTreat[dist]++; __nDeathReTreat[reg]++; }
        inline int return_nDeathReTreat(int index){return __nDeathReTreat[index];}
    inline void record_nDeathUntreat(int fac){__nDeathUntreat[fac]++;}
    inline void record_nDeathUntreat(int dist, int reg) { __nDeathUntreat[0]++; __nDeathUntreat[dist]++; __nDeathUntreat[reg]++; }
        inline int return_nDeathUntreat(int index){return __nDeathUntreat[index];}
    inline void record_nPatientInfDaysYear(int year, int fac, int value) {
        switch (year) {
            case 0:
                record_nPatientInfDaysYear0(fac, value);
                break;
            case 1:
                record_nPatientInfDaysYear1(fac, value);
                break;
            case 2:
                record_nPatientInfDaysYear2(fac, value);
                break;
            case 3:
                record_nPatientInfDaysYear3(fac, value);
                break;
            case 4:
                record_nPatientInfDaysYear4(fac, value);
                break;
            case 5:
                record_nPatientInfDaysYear5(fac, value);
                break;
            case 6:
                record_nPatientInfDaysYear6(fac, value);
                break;
            case 7:
                record_nPatientInfDaysYear7(fac, value);
                break;
            case 8:
                record_nPatientInfDaysYear8(fac, value);
                break;
            case 9:
                record_nPatientInfDaysYear9(fac, value);
                break;
            case 10:
                record_nPatientInfDaysYear10(fac, value);
                break;
            case 11:
                record_nPatientInfDaysYear11(fac, value);
                break;
            case 12:
                record_nPatientInfDaysYear12(fac, value);
                break;
            case 13:
                record_nPatientInfDaysYear13(fac, value);
                break;
            case 14:
                record_nPatientInfDaysYear14(fac, value);
                break;
            case 15:
                record_nPatientInfDaysYear15(fac, value);
                break;
            default:
                record_nPatientInfDaysYear0(fac, value);
                break;
        };
    }
    inline void record_nPatientInfDaysYear(int year, int dist, int reg, int value) {
        switch (year) {
        case 0:
            record_nPatientInfDaysYear0(dist, reg, value);
            break;
        case 1:
            record_nPatientInfDaysYear1(dist, reg, value);
            break;
        case 2:
            record_nPatientInfDaysYear2(dist, reg, value);
            break;
        case 3:
            record_nPatientInfDaysYear3(dist, reg, value);
            break;
        case 4:
            record_nPatientInfDaysYear4(dist, reg, value);
            break;
        case 5:
            record_nPatientInfDaysYear5(dist, reg, value);
            break;
        case 6:
            record_nPatientInfDaysYear6(dist, reg, value);
            break;
        case 7:
            record_nPatientInfDaysYear7(dist, reg, value);
            break;
        case 8:
            record_nPatientInfDaysYear8(dist, reg, value);
            break;
        case 9:
            record_nPatientInfDaysYear9(dist, reg, value);
            break;
        case 10:
            record_nPatientInfDaysYear10(dist, reg, value);
            break;
        case 11:
            record_nPatientInfDaysYear11(dist, reg, value);
            break;
        case 12:
            record_nPatientInfDaysYear12(dist, reg, value);
            break;
        case 13:
            record_nPatientInfDaysYear13(dist, reg, value);
            break;
        case 14:
            record_nPatientInfDaysYear14(dist, reg, value);
            break;
        case 15:
            record_nPatientInfDaysYear15(dist, reg, value);
            break;
        default:
            record_nPatientInfDaysYear0(dist, reg, value);
            break;
        };
    }
    inline void record_nPatientInfDaysActYear(int year, int fac, int value) {
        switch (year) {
        case 0:
            record_nPatientInfDaysActYear0(fac, value);
            break;
        case 1:
            record_nPatientInfDaysActYear1(fac, value);
            break;
        case 2:
            record_nPatientInfDaysActYear2(fac, value);
            break;
        case 3:
            record_nPatientInfDaysActYear3(fac, value);
            break;
        case 4:
            record_nPatientInfDaysActYear4(fac, value);
            break;
        case 5:
            record_nPatientInfDaysActYear5(fac, value);
            break;
        case 6:
            record_nPatientInfDaysActYear6(fac, value);
            break;
        case 7:
            record_nPatientInfDaysActYear7(fac, value);
            break;
        case 8:
            record_nPatientInfDaysActYear8(fac, value);
            break;
        case 9:
            record_nPatientInfDaysActYear9(fac, value);
            break;
        case 10:
            record_nPatientInfDaysActYear10(fac, value);
            break;
        case 11:
            record_nPatientInfDaysActYear11(fac, value);
            break;
        case 12:
            record_nPatientInfDaysActYear12(fac, value);
            break;
        case 13:
            record_nPatientInfDaysActYear13(fac, value);
            break;
        case 14:
            record_nPatientInfDaysActYear14(fac, value);
            break;
        case 15:
            record_nPatientInfDaysActYear15(fac, value);
            break;
        default:
            record_nPatientInfDaysActYear0(fac, value);
            break;
        };
    }
    inline void record_nPatientInfDaysActYear(int year, int dist, int reg, int value) {
        switch (year) {
        case 0:
            record_nPatientInfDaysActYear0(dist, reg, value);
            break;
        case 1:
            record_nPatientInfDaysActYear1(dist, reg, value);
            break;
        case 2:
            record_nPatientInfDaysActYear2(dist, reg, value);
            break;
        case 3:
            record_nPatientInfDaysActYear3(dist, reg, value);
            break;
        case 4:
            record_nPatientInfDaysActYear4(dist, reg, value);
            break;
        case 5:
            record_nPatientInfDaysActYear5(dist, reg, value);
            break;
        case 6:
            record_nPatientInfDaysActYear6(dist, reg, value);
            break;
        case 7:
            record_nPatientInfDaysActYear7(dist, reg, value);
            break;
        case 8:
            record_nPatientInfDaysActYear8(dist, reg, value);
            break;
        case 9:
            record_nPatientInfDaysActYear9(dist, reg, value);
            break;
        case 10:
            record_nPatientInfDaysActYear10(dist, reg, value);
            break;
        case 11:
            record_nPatientInfDaysActYear11(dist, reg, value);
            break;
        case 12:
            record_nPatientInfDaysActYear12(dist, reg, value);
            break;
        case 13:
            record_nPatientInfDaysActYear13(dist, reg, value);
            break;
        case 14:
            record_nPatientInfDaysActYear14(dist, reg, value);
            break;
        case 15:
            record_nPatientInfDaysActYear15(dist, reg, value);
            break;
        default:
            record_nPatientInfDaysActYear0(dist, reg, value);
            break;
        };
    }
    //inline int return_nPatientInfDays(int index) { return __nPatientInfDays[index]; }
    inline int return_nPatientInfDays(int year, int index) {
        switch (year) {
            case 0:
                return __nPatientInfDaysYear0[index];
                break;
            case 1:
                return __nPatientInfDaysYear1[index];
                break;
            case 2:
                return __nPatientInfDaysYear2[index];
                break;
            case 3:
                return __nPatientInfDaysYear3[index];
                break;
            case 4:
                return __nPatientInfDaysYear4[index];
                break;
            case 5:
                return __nPatientInfDaysYear5[index];
                break;
            case 6:
                return __nPatientInfDaysYear6[index];
                break;
            case 7:
                return __nPatientInfDaysYear7[index];
                break;
            case 8:
                return __nPatientInfDaysYear8[index];
                break;
            case 9:
                return __nPatientInfDaysYear9[index];
                break;
            case 10:
                return __nPatientInfDaysYear10[index];
                break;
            case 11:
                return __nPatientInfDaysYear11[index];
                break;
            case 12:
                return __nPatientInfDaysYear12[index];
                break;
            case 13:
                return __nPatientInfDaysYear13[index];
                break;
            case 14:
                return __nPatientInfDaysYear14[index];
                break;
            case 15:
                return __nPatientInfDaysYear15[index];
                break;
            default:
                return __nPatientInfDaysYear0[index];
                break;
        };
    }
    inline void record_nPatientInfDaysYear0(int fac, int value) { __nPatientInfDaysYear0[fac]= __nPatientInfDaysYear0[fac] + value; }
    inline void record_nPatientInfDaysYear0(int dist, int reg, int value) { __nPatientInfDaysYear0[0] = __nPatientInfDaysYear0[0] + value; __nPatientInfDaysYear0[dist] = __nPatientInfDaysYear0[dist] + value; __nPatientInfDaysYear0[reg] = __nPatientInfDaysYear0[reg] + value; }
        inline int return_nPatientInfDaysYear0(int index) { return __nPatientInfDaysYear0[index]; }
    inline void record_nPatientInfDaysYear1(int fac, int value) { __nPatientInfDaysYear1[fac] = __nPatientInfDaysYear1[fac] + value; }
    inline void record_nPatientInfDaysYear1(int dist, int reg, int value) { __nPatientInfDaysYear1[0] = __nPatientInfDaysYear1[0] + value; __nPatientInfDaysYear1[dist] = __nPatientInfDaysYear1[dist] + value; __nPatientInfDaysYear1[reg] = __nPatientInfDaysYear1[reg] + value; }
        inline int return_nPatientInfDaysYear1(int index) { return __nPatientInfDaysYear1[index]; }
    inline void record_nPatientInfDaysYear2(int fac, int value) { __nPatientInfDaysYear2[fac] = __nPatientInfDaysYear2[fac] + value; }
    inline void record_nPatientInfDaysYear2(int dist, int reg, int value) { __nPatientInfDaysYear2[0] = __nPatientInfDaysYear2[0] + value; __nPatientInfDaysYear2[dist] = __nPatientInfDaysYear2[dist] + value; __nPatientInfDaysYear2[reg] = __nPatientInfDaysYear2[reg] + value; }
        inline int return_nPatientInfDaysYear2(int index) { return __nPatientInfDaysYear2[index]; }
    inline void record_nPatientInfDaysYear3(int fac, int value) { __nPatientInfDaysYear3[fac] = __nPatientInfDaysYear3[fac] + value; }
    inline void record_nPatientInfDaysYear3(int dist, int reg, int value) { __nPatientInfDaysYear3[0] = __nPatientInfDaysYear3[0] + value; __nPatientInfDaysYear3[dist] = __nPatientInfDaysYear3[dist] + value; __nPatientInfDaysYear3[reg] = __nPatientInfDaysYear3[reg] + value; }
        inline int return_nPatientInfDaysYear3(int index) { return __nPatientInfDaysYear3[index]; }
    inline void record_nPatientInfDaysYear4(int fac, int value) { __nPatientInfDaysYear4[fac] = __nPatientInfDaysYear4[fac] + value; }
    inline void record_nPatientInfDaysYear4(int dist, int reg, int value) { __nPatientInfDaysYear4[0] = __nPatientInfDaysYear4[0] + value; __nPatientInfDaysYear4[dist] = __nPatientInfDaysYear4[dist] + value; __nPatientInfDaysYear4[reg] = __nPatientInfDaysYear4[reg] + value; }
        inline int return_nPatientInfDaysYear4(int index) { return __nPatientInfDaysYear4[index]; }
    inline void record_nPatientInfDaysYear5(int fac, int value) { __nPatientInfDaysYear5[fac] = __nPatientInfDaysYear5[fac] + value; }
    inline void record_nPatientInfDaysYear5(int dist, int reg, int value) { __nPatientInfDaysYear5[0] = __nPatientInfDaysYear5[0] + value; __nPatientInfDaysYear5[dist] = __nPatientInfDaysYear5[dist] + value; __nPatientInfDaysYear5[reg] = __nPatientInfDaysYear5[reg] + value; }
        inline int return_nPatientInfDaysYear5(int index) { return __nPatientInfDaysYear5[index]; }
    inline void record_nPatientInfDaysYear6(int fac, int value) { __nPatientInfDaysYear6[fac] = __nPatientInfDaysYear6[fac] + value;}
    inline void record_nPatientInfDaysYear6(int dist, int reg, int value) { __nPatientInfDaysYear6[0] = __nPatientInfDaysYear6[0] + value; __nPatientInfDaysYear6[dist] = __nPatientInfDaysYear6[dist] + value; __nPatientInfDaysYear6[reg] = __nPatientInfDaysYear6[reg] + value; }
        inline int return_nPatientInfDaysYear6(int index) { return __nPatientInfDaysYear6[index]; }
    inline void record_nPatientInfDaysYear7(int fac, int value) { __nPatientInfDaysYear7[fac] = __nPatientInfDaysYear7[fac] + value; }
    inline void record_nPatientInfDaysYear7(int dist, int reg, int value) { __nPatientInfDaysYear7[0] = __nPatientInfDaysYear7[0] + value; __nPatientInfDaysYear7[dist] = __nPatientInfDaysYear7[dist] + value; __nPatientInfDaysYear7[reg] = __nPatientInfDaysYear7[reg] + value; }
        inline int return_nPatientInfDaysYear7(int index) { return __nPatientInfDaysYear7[index]; }
    inline void record_nPatientInfDaysYear8(int fac, int value) { __nPatientInfDaysYear8[fac] = __nPatientInfDaysYear8[fac] + value;}
    inline void record_nPatientInfDaysYear8(int dist, int reg, int value) { __nPatientInfDaysYear8[0] = __nPatientInfDaysYear8[0] + value; __nPatientInfDaysYear8[dist] = __nPatientInfDaysYear8[dist] + value; __nPatientInfDaysYear8[reg] = __nPatientInfDaysYear8[reg] + value; }
        inline int return_nPatientInfDaysYear8(int index) { return __nPatientInfDaysYear8[index]; }
    inline void record_nPatientInfDaysYear9(int fac, int value) { __nPatientInfDaysYear9[fac] = __nPatientInfDaysYear9[fac] + value;}
    inline void record_nPatientInfDaysYear9(int dist, int reg, int value) { __nPatientInfDaysYear9[0] = __nPatientInfDaysYear9[0] + value; __nPatientInfDaysYear9[dist] = __nPatientInfDaysYear9[dist] + value; __nPatientInfDaysYear9[reg] = __nPatientInfDaysYear9[reg] + value; }
        inline int return_nPatientInfDaysYear9(int index) { return __nPatientInfDaysYear9[index]; }
    inline void record_nPatientInfDaysYear10(int fac, int value) { __nPatientInfDaysYear10[fac] = __nPatientInfDaysYear10[fac] + value; }
    inline void record_nPatientInfDaysYear10(int dist, int reg, int value) { __nPatientInfDaysYear10[0] = __nPatientInfDaysYear10[0] + value; __nPatientInfDaysYear10[dist] = __nPatientInfDaysYear10[dist] + value; __nPatientInfDaysYear10[reg] = __nPatientInfDaysYear10[reg] + value; }
        inline int return_nPatientInfDaysYear10(int index) { return __nPatientInfDaysYear10[index]; }
    inline void record_nPatientInfDaysYear11(int fac, int value) { __nPatientInfDaysYear11[fac] = __nPatientInfDaysYear11[fac] + value; }
    inline void record_nPatientInfDaysYear11(int dist, int reg, int value) { __nPatientInfDaysYear11[0] = __nPatientInfDaysYear11[0] + value; __nPatientInfDaysYear11[dist] = __nPatientInfDaysYear11[dist] + value; __nPatientInfDaysYear11[reg] = __nPatientInfDaysYear11[reg] + value; }
        inline int return_nPatientInfDaysYear11(int index) { return __nPatientInfDaysYear11[index]; }
    inline void record_nPatientInfDaysYear12(int fac, int value) {__nPatientInfDaysYear12[fac] = __nPatientInfDaysYear12[fac] + value; }
    inline void record_nPatientInfDaysYear12(int dist, int reg, int value) { __nPatientInfDaysYear12[0] = __nPatientInfDaysYear12[0] + value; __nPatientInfDaysYear12[dist] = __nPatientInfDaysYear12[dist] + value; __nPatientInfDaysYear12[reg] = __nPatientInfDaysYear12[reg] + value; }
        inline int return_nPatientInfDaysYear12(int index) { return __nPatientInfDaysYear12[index]; }
    inline void record_nPatientInfDaysYear13(int fac, int value) { __nPatientInfDaysYear13[fac] = __nPatientInfDaysYear13[fac] + value; }
    inline void record_nPatientInfDaysYear13(int dist, int reg, int value) { __nPatientInfDaysYear13[0] = __nPatientInfDaysYear13[0] + value; __nPatientInfDaysYear13[dist] = __nPatientInfDaysYear13[dist] + value; __nPatientInfDaysYear13[reg] = __nPatientInfDaysYear13[reg] + value; }
        inline int return_nPatientInfDaysYear13(int index) { return __nPatientInfDaysYear13[index]; }
    inline void record_nPatientInfDaysYear14(int fac, int value) { __nPatientInfDaysYear14[fac] = __nPatientInfDaysYear14[fac] + value;}
    inline void record_nPatientInfDaysYear14(int dist, int reg, int value) { __nPatientInfDaysYear14[0] = __nPatientInfDaysYear14[0] + value; __nPatientInfDaysYear14[dist] = __nPatientInfDaysYear14[dist] + value; __nPatientInfDaysYear14[reg] = __nPatientInfDaysYear14[reg] + value; }
        inline int return_nPatientInfDaysYear14(int index) { return __nPatientInfDaysYear14[index]; }
    inline void record_nPatientInfDaysYear15(int fac, int value) { __nPatientInfDaysYear15[fac] = __nPatientInfDaysYear15[fac] + value; }
    inline void record_nPatientInfDaysYear15(int dist, int reg, int value) { __nPatientInfDaysYear15[0] = __nPatientInfDaysYear15[0] + value; __nPatientInfDaysYear15[dist] = __nPatientInfDaysYear15[dist] + value; __nPatientInfDaysYear15[reg] = __nPatientInfDaysYear15[reg] + value; }
        inline int return_nPatientInfDaysYear15(int index) { return __nPatientInfDaysYear15[index]; }
    inline void record_nPatientInfDaysActYear0(int fac, int value) { __nPatientInfDaysActYear0[fac] = __nPatientInfDaysActYear0[fac] + value; }
    inline void record_nPatientInfDaysActYear0(int dist, int reg, int value) { __nPatientInfDaysActYear0[0] = __nPatientInfDaysActYear0[0] + value; __nPatientInfDaysActYear0[dist] = __nPatientInfDaysActYear0[dist] + value; __nPatientInfDaysActYear0[reg] = __nPatientInfDaysActYear0[reg] + value; }
        inline int return_nPatientInfDaysActYear0(int index) { return __nPatientInfDaysActYear0[index]; }
    inline void record_nPatientInfDaysActYear1(int fac, int value) { __nPatientInfDaysActYear1[fac] = __nPatientInfDaysActYear1[fac] + value; }
    inline void record_nPatientInfDaysActYear1(int dist, int reg, int value) { __nPatientInfDaysActYear1[0] = __nPatientInfDaysActYear1[0] + value; __nPatientInfDaysActYear1[dist] = __nPatientInfDaysActYear1[dist] + value; __nPatientInfDaysActYear1[reg] = __nPatientInfDaysActYear1[reg] + value; }
        inline int return_nPatientInfDaysActYear1(int index) { return __nPatientInfDaysActYear1[index]; }
    inline void record_nPatientInfDaysActYear2(int fac, int value) { __nPatientInfDaysActYear2[fac] = __nPatientInfDaysActYear2[fac] + value; }
    inline void record_nPatientInfDaysActYear2(int dist, int reg, int value) { __nPatientInfDaysActYear2[0] = __nPatientInfDaysActYear2[0] + value; __nPatientInfDaysActYear2[dist] = __nPatientInfDaysActYear2[dist] + value; __nPatientInfDaysActYear2[reg] = __nPatientInfDaysActYear2[reg] + value; }
        inline int return_nPatientInfDaysActYear2(int index) { return __nPatientInfDaysActYear2[index]; }
    inline void record_nPatientInfDaysActYear3(int fac, int value) { __nPatientInfDaysActYear3[fac] = __nPatientInfDaysActYear3[fac] + value; }
    inline void record_nPatientInfDaysActYear3(int dist, int reg, int value) { __nPatientInfDaysActYear3[0] = __nPatientInfDaysActYear3[0] + value; __nPatientInfDaysActYear3[dist] = __nPatientInfDaysActYear3[dist] + value; __nPatientInfDaysActYear3[reg] = __nPatientInfDaysActYear3[reg] + value; }
        inline int return_nPatientInfDaysActYear3(int index) { return __nPatientInfDaysActYear3[index]; }
    inline void record_nPatientInfDaysActYear4(int fac, int value) { __nPatientInfDaysActYear4[fac] = __nPatientInfDaysActYear4[fac] + value; }
    inline void record_nPatientInfDaysActYear4(int dist, int reg, int value) { __nPatientInfDaysActYear4[0] = __nPatientInfDaysActYear4[0] + value; __nPatientInfDaysActYear4[dist] = __nPatientInfDaysActYear4[dist] + value; __nPatientInfDaysActYear4[reg] = __nPatientInfDaysActYear4[reg] + value; }
        inline int return_nPatientInfDaysActYear4(int index) { return __nPatientInfDaysActYear4[index]; }
    inline void record_nPatientInfDaysActYear5(int fac, int value) { __nPatientInfDaysActYear5[fac] = __nPatientInfDaysActYear5[fac] + value; }
    inline void record_nPatientInfDaysActYear5(int dist, int reg, int value) { __nPatientInfDaysActYear5[0] = __nPatientInfDaysActYear5[0] + value; __nPatientInfDaysActYear5[dist] = __nPatientInfDaysActYear5[dist] + value; __nPatientInfDaysActYear5[reg] = __nPatientInfDaysActYear5[reg] + value; }
        inline int return_nPatientInfDaysActYear5(int index) { return __nPatientInfDaysActYear5[index]; }
    inline void record_nPatientInfDaysActYear6(int fac, int value) { __nPatientInfDaysActYear6[fac] = __nPatientInfDaysActYear6[fac] + value; }
    inline void record_nPatientInfDaysActYear6(int dist, int reg, int value) { __nPatientInfDaysActYear6[0] = __nPatientInfDaysActYear6[0] + value; __nPatientInfDaysActYear6[dist] = __nPatientInfDaysActYear6[dist] + value; __nPatientInfDaysActYear6[reg] = __nPatientInfDaysActYear6[reg] + value; }
        inline int return_nPatientInfDaysActYear6(int index) { return __nPatientInfDaysActYear6[index]; }
    inline void record_nPatientInfDaysActYear7(int fac, int value) { __nPatientInfDaysActYear7[fac] = __nPatientInfDaysActYear7[fac] + value;}
    inline void record_nPatientInfDaysActYear7(int dist, int reg, int value) { __nPatientInfDaysActYear7[0] = __nPatientInfDaysActYear7[0] + value; __nPatientInfDaysActYear7[dist] = __nPatientInfDaysActYear7[dist] + value; __nPatientInfDaysActYear7[reg] = __nPatientInfDaysActYear7[reg] + value; }
        inline int return_nPatientInfDaysActYear7(int index) { return __nPatientInfDaysActYear7[index]; }
    inline void record_nPatientInfDaysActYear8(int fac, int value) { __nPatientInfDaysActYear8[fac] = __nPatientInfDaysActYear8[fac] + value; }
    inline void record_nPatientInfDaysActYear8(int dist, int reg, int value) { __nPatientInfDaysActYear8[0] = __nPatientInfDaysActYear8[0] + value; __nPatientInfDaysActYear8[dist] = __nPatientInfDaysActYear8[dist] + value; __nPatientInfDaysActYear8[reg] = __nPatientInfDaysActYear8[reg] + value; }
        inline int return_nPatientInfDaysActYear8(int index) { return __nPatientInfDaysActYear8[index]; }
    inline void record_nPatientInfDaysActYear9(int fac, int value) { __nPatientInfDaysActYear9[fac] = __nPatientInfDaysActYear9[fac] + value; }
    inline void record_nPatientInfDaysActYear9(int dist, int reg, int value) { __nPatientInfDaysActYear9[0] = __nPatientInfDaysActYear9[0] + value; __nPatientInfDaysActYear9[dist] = __nPatientInfDaysActYear9[dist] + value; __nPatientInfDaysActYear9[reg] = __nPatientInfDaysActYear9[reg] + value; }
        inline int return_nPatientInfDaysActYear9(int index) { return __nPatientInfDaysActYear9[index]; }
    inline void record_nPatientInfDaysActYear10(int fac, int value) { __nPatientInfDaysActYear10[fac] = __nPatientInfDaysActYear10[fac] + value; }
    inline void record_nPatientInfDaysActYear10(int dist, int reg, int value) { __nPatientInfDaysActYear10[0] = __nPatientInfDaysActYear10[0] + value; __nPatientInfDaysActYear10[dist] = __nPatientInfDaysActYear10[dist] + value; __nPatientInfDaysActYear10[reg] = __nPatientInfDaysActYear10[reg] + value; }
        inline int return_nPatientInfDaysActYear10(int index) { return __nPatientInfDaysActYear10[index]; }
    inline void record_nPatientInfDaysActYear11(int fac, int value) { __nPatientInfDaysActYear11[fac] = __nPatientInfDaysActYear11[fac] + value; }
    inline void record_nPatientInfDaysActYear11(int dist, int reg, int value) { __nPatientInfDaysActYear11[0] = __nPatientInfDaysActYear11[0] + value; __nPatientInfDaysActYear11[dist] = __nPatientInfDaysActYear11[dist] + value; __nPatientInfDaysActYear11[reg] = __nPatientInfDaysActYear11[reg] + value; }
        inline int return_nPatientInfDaysActYear11(int index) { return __nPatientInfDaysActYear11[index]; }
    inline void record_nPatientInfDaysActYear12(int fac, int value) { __nPatientInfDaysActYear12[fac] = __nPatientInfDaysActYear12[fac] + value; }
    inline void record_nPatientInfDaysActYear12(int dist, int reg, int value) { __nPatientInfDaysActYear12[0] = __nPatientInfDaysActYear12[0] + value; __nPatientInfDaysActYear12[dist] = __nPatientInfDaysActYear12[dist] + value; __nPatientInfDaysActYear12[reg] = __nPatientInfDaysActYear12[reg] + value; }
        inline int return_nPatientInfDaysActYear12(int index) { return __nPatientInfDaysActYear12[index]; }
    inline void record_nPatientInfDaysActYear13(int fac, int value) { __nPatientInfDaysActYear13[fac] = __nPatientInfDaysActYear13[fac] + value; }
    inline void record_nPatientInfDaysActYear13(int dist, int reg, int value) { __nPatientInfDaysActYear13[0] = __nPatientInfDaysActYear13[0] + value; __nPatientInfDaysActYear13[dist] = __nPatientInfDaysActYear13[dist] + value; __nPatientInfDaysActYear13[reg] = __nPatientInfDaysActYear13[reg] + value; }
        inline int return_nPatientInfDaysActYear13(int index) { return __nPatientInfDaysActYear13[index]; }
    inline void record_nPatientInfDaysActYear14(int fac, int value) { __nPatientInfDaysActYear14[fac] = __nPatientInfDaysActYear14[fac] + value; }
    inline void record_nPatientInfDaysActYear14(int dist, int reg, int value) { __nPatientInfDaysActYear14[0] = __nPatientInfDaysActYear14[0] + value; __nPatientInfDaysActYear14[dist] = __nPatientInfDaysActYear14[dist] + value; __nPatientInfDaysActYear14[reg] = __nPatientInfDaysActYear14[reg] + value; }
        inline int return_nPatientInfDaysActYear14(int index) { return __nPatientInfDaysActYear14[index]; }
    inline void record_nPatientInfDaysActYear15(int fac, int value) { __nPatientInfDaysActYear15[fac] = __nPatientInfDaysActYear15[fac] + value; }
    inline void record_nPatientInfDaysActYear15(int dist, int reg, int value) { __nPatientInfDaysActYear15[0] = __nPatientInfDaysActYear15[0] + value; __nPatientInfDaysActYear15[dist] = __nPatientInfDaysActYear15[dist] + value; __nPatientInfDaysActYear15[reg] = __nPatientInfDaysActYear15[reg] + value; }
        inline int return_nPatientInfDaysActYear15(int index) { return __nPatientInfDaysActYear15[index]; }
    inline void record_sPatientWaiting(int fac){__sPatientWaiting[fac]++;}
    inline void record_sPatientWaiting(int dist, int reg) { __sPatientWaiting[0]++; __sPatientWaiting[dist]++; __sPatientWaiting[reg]++; }
        inline int return_sPatientWaiting(int index){return __sPatientWaiting[index];}
    inline void record_sPatientWaitingIn(int fac){__sPatientWaitingIn[fac]++;__sPatientWaiting[fac]++;}
    inline void record_sPatientWaitingIn(int dist, int reg) { __sPatientWaitingIn[0]++; __sPatientWaiting[0]++; __sPatientWaitingIn[dist]++; __sPatientWaiting[dist]++; __sPatientWaitingIn[reg]++; __sPatientWaiting[reg]++; }
        inline void reduce_sPatientWaitingIn(int fac){__sPatientWaitingIn[fac]--;__sPatientWaiting[fac]--;}
        inline void reduce_sPatientWaitingIn(int dist, int reg) { __sPatientWaitingIn[0]--; __sPatientWaiting[0]--; __sPatientWaitingIn[dist]--; __sPatientWaiting[dist]--; __sPatientWaitingIn[reg]--; __sPatientWaiting[reg]--; }
        inline int return_sPatientWaitingIn(int index){return __sPatientWaitingIn[index];}
    inline void record_sPatientWaitingOut(int fac){__sPatientWaitingOut[fac]++;__sPatientWaiting[fac]++;}
    inline void record_sPatientWaitingOut(int dist, int reg) { __sPatientWaitingOut[0]++; __sPatientWaiting[0]++; __sPatientWaitingOut[dist]++; __sPatientWaiting[dist]++; __sPatientWaitingOut[reg]++; __sPatientWaiting[reg]++; }
        inline void reduce_sPatientWaitingOut(int fac){__sPatientWaitingOut[fac]--;__sPatientWaiting[fac]--;}
        inline void reduce_sPatientWaitingOut(int dist, int reg) { __sPatientWaitingOut[0]--; __sPatientWaiting[0]--; __sPatientWaitingOut[dist]--; __sPatientWaiting[dist]--; __sPatientWaitingOut[reg]--; __sPatientWaiting[reg]--; }
        inline int return_sPatientWaitingOut(int index){return __sPatientWaitingOut[index];}
    inline void record_sPatientInTreatment(int fac){__sPatientInTreatment[fac]++;}
    inline void record_sPatientInTreatment(int dist, int reg) { __sPatientInTreatment[0]++; __sPatientInTreatment[dist]++; __sPatientInTreatment[reg]++; }
        inline void reduce_sPatientInTreatment(int fac){__sPatientInTreatment[fac]--;}
        inline void reduce_sPatientInTreatment(int dist, int reg) { __sPatientInTreatment[0]--; __sPatientInTreatment[dist]--; __sPatientInTreatment[reg]--; }
        inline int return_sPatientInTreatment(int index){return __sPatientInTreatment[index];}
    inline void record_sSampleWaitingForTest(int fac){__sSampleWaitingForTest[fac]++;}
    inline void record_sSampleWaitingForTest(int dist, int reg) { __sSampleWaitingForTest[0]++; __sSampleWaitingForTest[dist]++; __sSampleWaitingForTest[reg]++; }
        inline int return_sSampleWaitingForTest(int index){return __sSampleWaitingForTest[index];}
    inline void record_sSampleWaitingForTestIn(int fac){__sSampleWaitingForTestIn[fac]++;__sSampleWaitingForTest[fac]++;}
    inline void record_sSampleWaitingForTestIn(int dist, int reg) { __sSampleWaitingForTestIn[0]++; __sSampleWaitingForTest[0]++; __sSampleWaitingForTestIn[dist]++; __sSampleWaitingForTest[dist]++; __sSampleWaitingForTestIn[reg]++; __sSampleWaitingForTest[reg]++; }
        inline void reduce_sSampleWaitingForTestIn(int fac) {  __sSampleWaitingForTestIn[fac]--; __sSampleWaitingForTest[fac]--; }
        inline void reduce_sSampleWaitingForTestIn(int dist, int reg) { __sSampleWaitingForTestIn[0]--; __sSampleWaitingForTest[0]--; __sSampleWaitingForTestIn[dist]--; __sSampleWaitingForTest[dist]--; __sSampleWaitingForTestIn[reg]--; __sSampleWaitingForTest[reg]--; }
        inline int return_sSampleWaitingForTestIn(int index){return __sSampleWaitingForTestIn[index];}
    inline void record_sSampleWaitingForTestOut(int fac){__sSampleWaitingForTestOut[fac]++;__sSampleWaitingForTest[fac]++;;}
    inline void record_sSampleWaitingForTestOut(int dist, int reg) { __sSampleWaitingForTestOut[0]++; __sSampleWaitingForTest[0]++; __sSampleWaitingForTestOut[dist]++; __sSampleWaitingForTest[dist]++; __sSampleWaitingForTestOut[reg]++; __sSampleWaitingForTest[reg]++; }
        inline void reduce_sSampleWaitingForTestOut(int fac){__sSampleWaitingForTestOut[fac]--;__sSampleWaitingForTest[fac]--;}
        inline void reduce_sSampleWaitingForTestOut(int dist, int reg) { __sSampleWaitingForTestOut[0]--; __sSampleWaitingForTest[0]--; __sSampleWaitingForTestOut[dist]--; __sSampleWaitingForTest[dist]--; __sSampleWaitingForTestOut[reg]--; __sSampleWaitingForTest[reg]--; }
        inline int return_sSampleWaitingForTestOut(int index){return __sSampleWaitingForTestOut[index];}
 
    
    //output containers: annual values stored in vectors
    //vector<vector<double>> __vvDiseaseStates;//{"SUSDS DS_EL DS_LL DS_A DS_REC DS_TRDS DS_TRDR  SUSDR DR_EL DR_LL DR_A DR_REC DR_TRDS DR_TRDR"} 1--14

    vector<std::array<string, 1000>> __vName;                          // name of region/district/facility
    vector<std::array<int, 1000>> __vId;                               // id of facility
    vector<std::array<int, 1000>> __vTier;                             // tier of facility
    vector<std::array<int, 1000>> __vXpertMach;                        // Xpert in facility
    vector<std::array<int, 1000>> __vYear;                             // year of simulation
    vector<std::array<string, 1000>> __vScenario;                      // scenario
    vector<std::array<string, 1000>> __vDistrict;                      // name of district
    vector<std::array<string, 1000>> __vRegion;                        // name of region
    vector<std::array<int, 1000>> __vPopulation;                       // population of region/district/facility
    vector<std::array<string, 1000>> __vDisease;                       // statistics of this disease
    vector<std::array<float, 1000>> __vIncidence;                      // incidence of this disease
    vector<std::array<float, 1000>> __vCalcIncidence;                  // calculated incidence of this disease
    vector<std::array<int, 1000>> __vNPatient;                         // no. of patients
    vector<std::array<int, 1000>> __vNPatientInf;                      // no. of patients that are infected
    vector<std::array<int, 1000>> __vNPatientLtFUInf;                  // no. of patients that are loss to follow up and represent - infected
    vector<std::array<int, 1000>> __vNPatientFromTrans;                // no. of patients that are created through transmission of disease
    vector<std::array<int, 1000>> __vNPatientNotPresenting;               // no. of patients that are infected but don't present for diagnosis
    vector<std::array<int, 1000>> __vNSamples;                         // no. of samples collected
    vector<std::array<int, 1000>> __vNSamplesIn;                       // no. of samples collected in-house
    vector<std::array<int, 1000>> __vNSamplesOut;                      // no. of samples sent out for testing
    vector<std::array<int, 1000>> __vTimeSampleResourceWait;           // total time samples have to wait for resources to be tested
    vector<std::array<int, 1000>> __vTimeSampleResourceWaitIn;         // total time samples have to wait for resources to be tested in-house
    vector<std::array<int, 1000>> __vTimeSampleResourceWaitOut;        // total time samples have to wait for resources to be tested at another facility
    vector<std::array<int, 1000>> __vNTests;                           // no. of tests done
    vector<std::array<int, 1000>> __vNTestsIn;                         // no. of tests done in-house
    vector<std::array<int, 1000>> __vNTestsOut;                        // no. of tests done at facility different from where patient presents
    vector<std::array<int, 1000>> __vNTransported;                     // no. of samples transported
    vector<std::array<int, 1000>> __vTimeSampleTransport;              // total time samples were in transport
    vector<std::array<int, 1000>> __vTimePresentToSampleTestIn;        // total time from patient presentation to sample test - in-house
    vector<std::array<int, 1000>> __vNPresentToSampleTestIn;           // no. of patients counted in __timePresentToSampleTestIn
    vector<std::array<int, 1000>> __vTimePresentToSampleTestOut;       // total time from patient presentation to sample test - at another facility
    vector<std::array<int, 1000>> __vNPresentToSampleTestOut;          // no. of patients counted in __timePresentToSampleTestOut
    vector<std::array<int, 1000>> __vNTestPos;                         // no. of tests positive 
    vector<std::array<int, 1000>> __vNTestTruePos;                     // no. of tests true positive
    vector<std::array<int, 1000>> __vNTestTruePosIn;                   // no. of tests true positive - in-house
    vector<std::array<int, 1000>> __vNTestTruePosOut;                  // no. of tests true positive - at another facility
    vector<std::array<int, 1000>> __vNTestFalsePos;                    // no. of tests false positive
    vector<std::array<int, 1000>> __vNTestFalsePosIn;                  // no. of tests false positive - in-house
    vector<std::array<int, 1000>> __vNTestFalsePosOut;                 // no. of tests false positive - at another facility
    vector<std::array<int, 1000>> __vNTestNeg;                         // no. of tests negative
    vector<std::array<int, 1000>> __vNTestTrueNeg;                     // no. of tests true negative
    vector<std::array<int, 1000>> __vNTestTrueNegIn;                   // no. of tests true negative - in-house
    vector<std::array<int, 1000>> __vNTestTrueNegOut;                  // no. of tests true negative - at another facility
    vector<std::array<int, 1000>> __vNTestFalseNeg;                    // no. of tests false negative
    vector<std::array<int, 1000>> __vNTestFalseNegIn;                  // no. of tests false negative - in-house
    vector<std::array<int, 1000>> __vNTestFalseNegOut;                 // no. of tests false negative - at another facility
    vector<std::array<int, 1000>> __vNTestPosIn;                       // no. of tests that are returned positive from testing - in-house
    vector<std::array<int, 1000>> __vNTestPosOut;                      // no. of tests that are returned positive from testing - at another facility
    vector<std::array<int, 1000>> __vNMDCDInfected;                    // no. of clinical diagnoses of infected patients - basically true pos
    vector<std::array<int, 1000>> __vNMDCDUninfected;                  // no. of clinical diagnoses of uninfected patients - basically false pos
    vector<std::array<int, 1000>> __vNPTMDCDInfected;                  // no. of pre-test clinical diagnoses of infected patients - basically true pos
    vector<std::array<int, 1000>> __vNPTMDCDUninfected;                // no. of pre-test clinical diagnoses of uninfected patients - basically false pos
    vector<std::array<int, 1000>> __vNDiagnosis;                       // no. of pos. tests - true pos, false pos and MD override true pos and false pos
    vector<std::array<int, 1000>> __vNDiagnosisIn;                     // no. of pos. tests - true pos, false pos and MD override true pos and false pos - in house
    vector<std::array<int, 1000>> __vNDiagnosisOut;                    // no. of pos. tests - true pos, false pos and MD override true pos and false pos - at another facility
    vector<std::array<int, 1000>> __vNDiagnosisDC;                     // no. of pos. tests not counted because of MD clinical diagnosis was pos before test returns positive
    vector<std::array<int, 1000>> __vNDiagnosisDCTruePos;              // no. of pos. tests not counted because of MD clinical diagnosis was pos before test returns true positive
    vector<std::array<int, 1000>> __vNDiagnosisDCFalsePos;             // no. of pos. tests not counted because of MD clinical diagnosis was pos before test returns false positive
    vector<std::array<int, 1000>> __vNPreTestDiagnosisTruePos;         // no. of true pos. pre-test MD override tests
    vector<std::array<int, 1000>> __vNPreTestDiagnosisFalseNeg;        // no. of false neg. pre-test MD override tests
    vector<std::array<int, 1000>> __vNPreTestDiagnosisTrueNeg;         // no. of true neg. pre-test MD override tests
    vector<std::array<int, 1000>> __vNPreTestDiagnosisFalsePos;        // no. of false pos. pre-test MD override tests
    vector<std::array<int, 1000>> __vNPreTestDiagnosisTotal;           // total no. pre-test true pos and false pos MD override tests
    vector<std::array<int, 1000>> __vNPreTestDiagnosisReview;          // total no. pre-test MD override tests
    vector<std::array<int, 1000>> __vNPostTestDiagnosisTruePosIn;      // no. of true pos. post-test MD override tests in-house
    vector<std::array<int, 1000>> __vNPostTestDiagnosisTruePosOut;     // no. of true pos. post-test MD override tests tested out
    vector<std::array<int, 1000>> __vNPostTestDiagnosisFalseNeg;       // no. of false neg. post-test MD override tests
    vector<std::array<int, 1000>> __vNPostTestDiagnosisTrueNeg;        // no. of true neg. post-test MD override tests
    vector<std::array<int, 1000>> __vNPostTestDiagnosisFalsePosIn;     // no. of false pos. post-test MD override tests tested in-house
    vector<std::array<int, 1000>> __vNPostTestDiagnosisFalsePosOut;    // no. of false pos. post-test MD override tests tested out
    vector<std::array<int, 1000>> __vNPostTestDiagnosisTotal;          // total no. post-test MD true pos and false pos override tests
    vector<std::array<int, 1000>> __vNPostTestDiagnosisReview;         // total no. post-test MD override tests
    vector<std::array<int, 1000>> __vNMDDiagnosisTotal;                // total no. MD True pos and false pos override tests pre and post test
    vector<std::array<int, 1000>> __vNMDDiagnosisReview;               // total no. MD override tests pre and post test
    vector<std::array<int, 1000>> __vNPreTestLtFU;                     // no. of samples lost to pre-test loss to follow up
    vector<std::array<int, 1000>> __vNPreTestLtFUInf;                  // no. of samples lost to pre-test loss to follow up for infected samples
    vector<std::array<int, 1000>> __vNPreTestLtFUDied;                 // no. of patients whose samples were pre-test ltfu and died
    vector<std::array<int, 1000>> __vNPreTestLtFUCured;                // no. of patients whose samples were pre-test ltfu, didn't represent and were cured
    vector<std::array<int, 1000>> __vNPreTestLtFURepresent;            // no. of patients whose samples were pre-test ltfu and re-presented
    vector<std::array<int, 1000>> __vNPreTreatLtFU;                    // no. of samples lost to pre-Treat loss to follow up
    vector<std::array<int, 1000>> __vNPreTreatLtFUInf;                 // no. of samples lost to pre-Treat loss to follow up for infected patients
    vector<std::array<int, 1000>> __vNPreTreatLtFUDied;                // no. of patients whose samples were pre-Treat ltfu and died
    vector<std::array<int, 1000>> __vNPreTreatLtFUCured;               // no. of patients whose samples were pre-Treat ltfu, didn't represent and were cured
    vector<std::array<int, 1000>> __vNPreTreatLtFURepresent;           // no. of patients whose samples were pre-Treat ltfu and re-presented
    vector<std::array<int, 1000>> __vNPreTreatLtFUFN;                  // no. of samples lost to pre-Treat loss to follow up due to false neg test results
    vector<std::array<int, 1000>> __vNPreTreatLtFUFNDied;              // no. of samples lost to pre-Treat loss to follow up due to false neg test results that die
    vector<std::array<int, 1000>> __vNPreTreatLtFUFNCured;             // no. of samples lost to pre-Treat loss to follow up due to false neg test results that are cured
    vector<std::array<int, 1000>> __vNPreTreatLtFUFNRepresent;         // no. of samples lost to pre-Treat loss to follow up due to false neg test results that represent
    vector<std::array<int, 1000>> __vNPreTreatLtFUPos;                 // no. of patients whose samples were pre-Treat ltfu and tested postitive
    vector<std::array<int, 1000>> __vNPreTreatLtFUNeg;                 // no. of patients whose samples were pre-Treat ltfu and tested negative
    vector<std::array<int, 1000>> __vNTreatLtFU;                       // no. of samples lost to Treatment loss to follow up
    vector<std::array<int, 1000>> __vNTreatLtFUInf;                    // no. of samples lost to Treatment loss to follow up for infected patients
    vector<std::array<int, 1000>> __vNTreatLtFUDied;                   // no. of patients whose samples were treatment ltfu and died
    vector<std::array<int, 1000>> __vNTreatLtFUCured;                  // no. of patients whose samples were treatment ltfu, didn't represent and were cured
    vector<std::array<int, 1000>> __vNTreatLtFURepresent;              // no. of patients whose samples were treatment ltfu and re-presented
    vector<std::array<int, 1000>> __vNPatientsTreated;                 // no. of patients treated
    vector<std::array<int, 1000>> __vNPatientsTreatedInf;              // no. of patients treated that are infected
    vector<std::array<int, 1000>> __vNPatientsTreatedUninf;            // no. of patients treated that are uninfected
    vector<std::array<int, 1000>> __vNPatientsPreTreatedLtoFUninf;     // no. of patients treated that are loss to follow up before treatment starts uninfected
    vector<std::array<int, 1000>> __vNPatientsTreatedCured;            // no. of patients treated that are cured
    vector<std::array<int, 1000>> __vNPatientsTreatedDied;             // no. of patients treated that died
    vector<std::array<int, 1000>> __vNPatientsTreatedRelapse;          // no. of patients treated that relapse
    vector<std::array<int, 1000>> __vNTreatedRelapse;                  // no. of patients treated who relapse after treatment
    vector<std::array<int, 1000>> __vNTreatedRelapseCured;             // no. of patients treated who relapse and are cured
    vector<std::array<int, 1000>> __vNTreatedRelapseDied;              // no. of patients treated who relapse and die
    vector<std::array<int, 1000>> __vTimePresentToSampleDiagIn;        // total time from patient presentation to sample diagnosis - in-house
    vector<std::array<int, 1000>> __vNPresentToSampleDiagIn;           // no. of patients counted in __timePresentToSampleDiagIn
    vector<std::array<int, 1000>> __vTimePresentToSampleDiagOut;       // total time from patient presentation to sample diagnosis - at another facility
    vector<std::array<int, 1000>> __vNPresentToSampleDiagOut;          // no. of patients counted in __timePresentToSampleDiagOut
    vector<std::array<int, 1000>> __vTimePresentToSampleTreat;         // total time from patient presentation to treatment
    vector<std::array<int, 1000>> __vNPresentToSampleTreat;            // no. of patients counted in __timePresentToSampleTreat
    vector<std::array<int, 1000>> __vTimePresentToSampleTreatIn;       // total time from patient presentation to treatment - in-house
    vector<std::array<int, 1000>> __vNPresentToSampleTreatIn;          // no. of patients counted in __timePresentToSampleTreatIn
    vector<std::array<int, 1000>> __vTimePresentToSampleTreatOut;      // total time from patient presentation to treatment - at another facility
    vector<std::array<int, 1000>> __vNPresentToSampleTreatOut;         // no. of patients counted in __timePresentToSampleTreatOut
    vector<std::array<int, 1000>> __vTimePresentToSampleDeath;         // total time from patient presentation to death
    vector<std::array<int, 1000>> __vNPresentToSampleDeath;            // no. of patients counted in __timePresentToSampleDeath
    vector<std::array<int, 1000>> __vTimePresentToSampleDeathIn;       // total time from patient presentation to death - in-house
    vector<std::array<int, 1000>> __vNPresentToSampleDeathIn;          // no. of patients counted in __timePresentToSampleDeathIn
    vector<std::array<int, 1000>> __vTimePresentToSampleDeathOut;      // total time from patient presentation to death - at another facility
    vector<std::array<int, 1000>> __vNPresentToSampleDeathOut;         // no. of patients counted in __nPresentToSampleDeathOut
    vector<std::array<int, 1000>> __vTimePresentToSampleDeathPreTestLTFU; // total time from patient presentation to death after pre-test loss to follow up
    vector<std::array<int, 1000>> __vNPresentToSampleDeathPreTestLTFU; // no. of patients counted in __timePresentToSampleDeathPreTreatLTFU
    vector<std::array<int, 1000>> __vTimePresentToSampleDeathPreTreatLTFU; // total time from patient presentation to death after pre-test loss to follow up
    vector<std::array<int, 1000>> __vNPresentToSampleDeathPreTreatLTFU; // no. of patients counted in __timePresentToSampleDeathPreTreatLTFU
    vector<std::array<int, 1000>> __vTimePresentToSampleDeathTreatLTFU; // total time from patient presentation to death after treatment loss to follow up
    vector<std::array<int, 1000>> __vNPresentToSampleDeathTreatLTFU; // no. of patients counted in __timePresentToSampleDeathTreatLTFU
    vector<std::array<int, 1000>> __vTimePresentToSampleDeathInTreat;  // total time from patient presentation to death while in treatment
    vector<std::array<int, 1000>> __vNPresentToSampleDeathInTreat;     // no. of patients counted in __timePresentToSampleDeathInTreat
    vector<std::array<int, 1000>> __vTimePresentToSampleDeathInRelapse;// total time from patient presentation to death while in relapse treatment
    vector<std::array<int, 1000>> __vNPresentToSampleDeathInRelapse;   // no. of patients counted in __timePresentToSampleDeathInRelapse
    vector<std::array<int, 1000>> __vTimePresentToExit;                // total time from patient presentation to death or cure
    vector<std::array<int, 1000>> __vNPresentToExit;                   // no. of patients counted in __timePresentToExit
    vector<std::array<int, 1000>> __vNXpert;                           // no. of Xpert tests
    vector<std::array<int, 1000>> __vNXpertIn;                         // no. of Xpert tests in-house
    vector<std::array<int, 1000>> __vNXpertOut;                        // no. of Xpert tests at another facility
    vector<std::array<int, 1000>> __vNXpertPos;                        // no. of Xpert positive tests
    vector<std::array<int, 1000>> __vNXpertTruePos;                    // no. of Xpert true positive tests
    vector<std::array<int, 1000>> __vNXpertFalsePos;                   // no. of Xpert false positive tests
    vector<std::array<int, 1000>> __vNXpertNeg;                        // no. of Xpert negative tests
    vector<std::array<int, 1000>> __vNXpertTrueNeg;                    // no. of Xpert true negative tests
    vector<std::array<int, 1000>> __vNXpertFalseNeg;                   // no. of Xpert false negative tests
    vector<std::array<int, 1000>> __vNSmear;                           // no. of smear tests
    vector<std::array<int, 1000>> __vNSmearIn;                         // no. of smear tests in-house
    vector<std::array<int, 1000>> __vNSmearOut;                        // no. of smear tests at another facility
    vector<std::array<int, 1000>> __vNSmearPos;                        // no. of smear positive tests
    vector<std::array<int, 1000>> __vNSmearTruePos;                    // no. of smear true positive tests
    vector<std::array<int, 1000>> __vNSmearFalsePos;                   // no. of smear false positive tests
    vector<std::array<int, 1000>> __vNSmearNeg;                        // no. of smear negative tests
    vector<std::array<int, 1000>> __vNSmearTrueNeg;                    // no. of smear true negative tests
    vector<std::array<int, 1000>> __vNSmearFalseNeg;                   // no. of smear false negative tests
    vector<std::array<int, 1000>> __vNDeathPreTest;                    // no. of patients that die from pre-test loss to follow up
    vector<std::array<int, 1000>> __vNDeathPreTreat;                   // no. of patients that die from pre-treatment loss to follow up
    vector<std::array<int, 1000>> __vNDeathTreat;                      // no. of patients that die during treatment
    vector<std::array<int, 1000>> __vNDeathReTreat;                    // no. of patients that die during re-treatment
    vector<std::array<int, 1000>> __vNDeathUntreat;                    // no. of patients that die untreated
    vector<std::array<int, 1000>> __vNPatientInfDays;                  // no. of infectious days for patients
    vector<std::array<int, 1000>> __vNPatientInfDaysYear0;             // no. of infectious days for patients in year 0
    vector<std::array<int, 1000>> __vNPatientInfDaysYear1;             // no. of infectious days for patients in year 1
    vector<std::array<int, 1000>> __vNPatientInfDaysYear2;             // no. of infectious days for patients in year 2
    vector<std::array<int, 1000>> __vNPatientInfDaysYear3;             // no. of infectious days for patients in year 3
    vector<std::array<int, 1000>> __vNPatientInfDaysYear4;             // no. of infectious days for patients in year 4
    vector<std::array<int, 1000>> __vNPatientInfDaysYear5;             // no. of infectious days for patients in year 5
    vector<std::array<int, 1000>> __vNPatientInfDaysYear6;             // no. of infectious days for patients in year 6
    vector<std::array<int, 1000>> __vNPatientInfDaysYear7;             // no. of infectious days for patients in year 7
    vector<std::array<int, 1000>> __vNPatientInfDaysYear8;             // no. of infectious days for patients in year 8
    vector<std::array<int, 1000>> __vNPatientInfDaysYear9;             // no. of infectious days for patients in year 9
    vector<std::array<int, 1000>> __vNPatientInfDaysYear10;            // no. of infectious days for patients in year 10
    vector<std::array<int, 1000>> __vNPatientInfDaysYear11;            // no. of infectious days for patients in year 11
    vector<std::array<int, 1000>> __vNPatientInfDaysYear12;            // no. of infectious days for patients in year 12
    vector<std::array<int, 1000>> __vNPatientInfDaysYear13;            // no. of infectious days for patients in year 13
    vector<std::array<int, 1000>> __vNPatientInfDaysYear14;            // no. of infectious days for patients in year 14
    vector<std::array<int, 1000>> __vNPatientInfDaysYear15;            // no. of infectious days for patients in year 15
    vector<std::array<int, 1000>> __vNPatientInfDaysAct;               // no. of infectious days for active patients
    vector<std::array<int, 1000>> __vNPatientInfDaysActYear0;          // no. of infectious days for active patients in year 0
    vector<std::array<int, 1000>> __vNPatientInfDaysActYear1;          // no. of infectious days for active patients in year 1
    vector<std::array<int, 1000>> __vNPatientInfDaysActYear2;          // no. of infectious days for active patients in year 2
    vector<std::array<int, 1000>> __vNPatientInfDaysActYear3;          // no. of infectious days for active patients in year 3
    vector<std::array<int, 1000>> __vNPatientInfDaysActYear4;          // no. of infectious days for active patients in year 4
    vector<std::array<int, 1000>> __vNPatientInfDaysActYear5;          // no. of infectious days for active patients in year 5
    vector<std::array<int, 1000>> __vNPatientInfDaysActYear6;          // no. of infectious days for active patients in year 6
    vector<std::array<int, 1000>> __vNPatientInfDaysActYear7;          // no. of infectious days for active patients in year 7
    vector<std::array<int, 1000>> __vNPatientInfDaysActYear8;          // no. of infectious days for active patients in year 8
    vector<std::array<int, 1000>> __vNPatientInfDaysActYear9;          // no. of infectious days for active patients in year 9
    vector<std::array<int, 1000>> __vNPatientInfDaysActYear10;         // no. of infectious days for active patients in year 10
    vector<std::array<int, 1000>> __vNPatientInfDaysActYear11;         // no. of infectious days for active patients in year 11
    vector<std::array<int, 1000>> __vNPatientInfDaysActYear12;         // no. of infectious days for active patients in year 12
    vector<std::array<int, 1000>> __vNPatientInfDaysActYear13;         // no. of infectious days for active patients in year 13
    vector<std::array<int, 1000>> __vNPatientInfDaysActYear14;         // no. of infectious days for active patients in year 14
    vector<std::array<int, 1000>> __vNPatientInfDaysActYear15;         // no. of infectious days for active patients in year 15
    vector<std::array<int, 1000>> __vSPatientWaiting;                  // no. of patients waiting for tests
    vector<std::array<int, 1000>> __vSPatientWaitingIn;                // no. of patients waiting for tests - in-house
    vector<std::array<int, 1000>> __vSPatientWaitingOut;               // no. of patients waiting for tests at another facility
    vector<std::array<int, 1000>> __vSPatientInTreatment;              // no. of patients still in treatment
    vector<std::array<int, 1000>> __vSSampleWaitingForTest;            // no. of samples waiting for test
    vector<std::array<int, 1000>> __vSSampleWaitingForTestIn;          // no. of samples waiting for test - in-house
    vector<std::array<int, 1000>> __vSSampleWaitingForTestOut;         // no. of samples waiting for test at another facility



    //records:
    void recordOutputs(); //called at the end of each year to collect all outputs
    //inline void record_vPopSize(int n){__vPopSize.push_back(n);}
    //inline void record_vPrevDS(int prev){__vPrevDS.push_back(prev);}
    //inline void record_vPrevDR(int prev){__vPrevDR.push_back(prev);}
    //inline void record_vDurDSTB(int d){__vDurDS.push_back(d);}
    //inline void record_vDurDRTB(int d){__vDurDR.push_back(d);}
    //inline void recordDiseaseStates(vector<std::array<int, 1000>> temp){__vvDiseaseStates.push_back(temp);}
    //inline void record_vOnPt(int d){__vOnPT.push_back(d);}


    //returns:
    //inline vector<vector<double>> return_vDiseaseStates(){return __vvDiseaseStates;}

    inline vector<double> return_vNXpertMach(int index) { return extractVector(__vXpertMach, index); }
    inline vector<double> return_vNPopulation(int index){return extractVector(__vPopulation, index);}
    inline vector<double> return_vNIncidence(int index){return extractVector(__vIncidence, index);}
    inline vector<double> return_vNCalcIncidence(int index){return extractVector(__vCalcIncidence, index);}
    inline vector<string> return_vName(int index){return extractStringVector(__vName, index);}
    inline vector<double> return_vNPatient(int index){return extractVector(__vNPatient, index);}
    inline vector<double> return_vNPatientInf(int index){return extractVector(__vNPatientInf, index);}
    inline vector<double> return_vNPatientLtFUInf(int index){return extractVector(__vNPatientLtFUInf, index);}
    inline vector<double> return_vNPatientFromTrans(int index) { return extractVector(__vNPatientFromTrans, index); }
    inline vector<double> return_vNPatientNotPresenting(int index) { return extractVector(__vNPatientNotPresenting, index); }
    inline vector<double> return_vNSamples(int index){return extractVector(__vNSamples, index);}
    inline vector<double> return_vNSamplesIn(int index){return extractVector(__vNSamplesIn, index);}
    inline vector<double> return_vNSamplesOut(int index){return extractVector(__vNSamplesOut, index);}
    inline vector<double> return_vAveSampleResourceWait(int index){return doubleVecDivide(extractVector(__vTimeSampleResourceWait, index),extractVector(__vNTests, index));}
    inline vector<double> return_vTimeSampleResourceWait(int index){return extractVector(__vTimeSampleResourceWait, index);}
    inline vector<double> return_vAveSampleResourceWaitIn(int index){return doubleVecDivide(extractVector(__vTimeSampleResourceWaitIn, index),extractVector(__vNTestsIn, index));}
    inline vector<double> return_vTimeSampleResourceWaitIn(int index){return extractVector(__vTimeSampleResourceWaitIn, index);}
    inline vector<double> return_vAveSampleResourceWaitOut(int index){return doubleVecDivide(extractVector(__vTimeSampleResourceWaitOut, index),extractVector(__vNTestsOut, index));}
    inline vector<double> return_vTimeSampleResourceWaitOut(int index){return extractVector(__vTimeSampleResourceWaitOut, index);}
    inline vector<double> return_vAveSampleTransport(int index){return doubleVecDivide(extractVector(__vTimeSampleTransport, index),extractVector(__vNTransported, index)); }
    inline vector<double> return_vTimeSampleTransport(int index){return extractVector(__vTimeSampleTransport, index); }
    inline vector<double> return_vNTransported(int index){return extractVector(__vNTransported, index);}
    inline vector<double> return_vNTests(int index){return extractVector(__vNTests, index);}
    inline vector<double> return_vNTestsIn(int index){return extractVector(__vNTestsIn, index);}
    inline vector<double> return_vNTestsOut(int index){return extractVector(__vNTestsOut, index);}
    inline vector<double> return_vAvePresentToSampleTestIn(int index){return doubleVecDivide(extractVector(__vTimePresentToSampleTestIn, index),extractVector(__vNPresentToSampleTestIn, index));}
    inline vector<double> return_vTimePresentToSampleTestIn(int index){return extractVector(__vTimePresentToSampleTestIn, index);}
    inline vector<double> return_vNPresentToSampleTestIn(int index){return extractVector(__vNPresentToSampleTestIn, index); }
    inline vector<double> return_vAvePresentToSampleTestOut(int index){return doubleVecDivide(extractVector(__vTimePresentToSampleTestOut, index),extractVector(__vNPresentToSampleTestOut, index));}
    inline vector<double> return_vTimePresentToSampleTestOut(int index){return extractVector(__vTimePresentToSampleTestOut, index);}
    inline vector<double> return_vNPresentToSampleTestOut(int index){return extractVector(__vNPresentToSampleTestOut, index);}
    inline vector<double> return_vNTestPos(int index){return extractVector(__vNTestPos, index);}
    inline vector<double> return_vPTestTruePos(int index){return doubleVecDivide(extractVector(__vNTestTruePos, index), extractVector(__vNTestPos, index));}
    inline vector<double> return_vNTestTruePos(int index){return extractVector(__vNTestTruePos, index);}
    inline vector<double> return_vNTestTruePosIn(int index){return extractVector(__vNTestTruePosIn, index);}
    inline vector<double> return_vNTestTruePosOut(int index){return extractVector(__vNTestTruePosOut, index);}
    inline vector<double> return_vPTestFalsePos(int index){return doubleVecDivide(extractVector(__vNTestFalsePos, index), extractVector(__vNTestPos, index));}
    inline vector<double> return_vNTestFalsePos(int index){return extractVector(__vNTestFalsePos, index);}
    inline vector<double> return_vNTestFalsePosIn(int index){return extractVector(__vNTestFalsePosIn, index);}
    inline vector<double> return_vNTestFalsePosOut(int index){return extractVector(__vNTestFalsePosOut, index);}
    inline vector<double> return_vNTestNeg(int index){return extractVector(__vNTestNeg, index);}
    inline vector<double> return_vPTestTrueNeg(int index){return doubleVecDivide(extractVector(__vNTestTrueNeg, index),extractVector(__vNTestNeg, index));}
    inline vector<double> return_vNTestTrueNeg(int index){return extractVector(__vNTestTrueNeg, index);}
    inline vector<double> return_vNTestTrueNegIn(int index){return extractVector(__vNTestTrueNegIn, index);}
    inline vector<double> return_vNTestTrueNegOut(int index){return extractVector(__vNTestTrueNegOut, index);}
    inline vector<double> return_vPTestFalseNeg(int index){return doubleVecDivide(extractVector(__vNTestFalseNeg, index), extractVector(__vNTestNeg, index));}
    inline vector<double> return_vNTestFalseNeg(int index){return extractVector(__vNTestFalseNeg, index);}
    inline vector<double> return_vNTestFalseNegIn(int index){return extractVector(__vNTestFalseNegIn, index);}
    inline vector<double> return_vNTestFalseNegOut(int index){return extractVector(__vNTestFalseNegOut, index);}
    inline vector<double> return_vNTestPosIn(int index){return extractVector(__vNTestPosIn, index);}
    inline vector<double> return_vNTestPosOut(int index){return extractVector(__vNTestPosOut, index);}
    inline vector<double> return_vNMDCDInfected(int index) { return extractVector(__vNMDCDInfected, index); }
    inline vector<double> return_vNMDCDUninfected(int index) { return extractVector(__vNMDCDUninfected, index); }
    inline vector<double> return_vNPTMDCDInfected(int index) { return extractVector(__vNPTMDCDInfected, index); }
    inline vector<double> return_vNPTMDCDUninfected(int index) { return extractVector(__vNPTMDCDUninfected, index); }
    inline vector<double> return_vNPostTestDiagnosisTruePosIn(int index) { return extractVector(__vNPostTestDiagnosisTruePosIn, index); }
    inline vector<double> return_vNPostTestDiagnosisTruePosOut(int index) { return extractVector(__vNPostTestDiagnosisTruePosOut, index); }
    inline vector<double> return_vNPostTestDiagnosisFalseNeg(int index){return extractVector(__vNPostTestDiagnosisFalseNeg, index);}
    inline vector<double> return_vNPostTestDiagnosisTrueNeg(int index){return extractVector(__vNPostTestDiagnosisTrueNeg, index);}
    inline vector<double> return_vNPostTestDiagnosisFalsePosIn(int index) { return extractVector(__vNPostTestDiagnosisFalsePosIn, index); }
    inline vector<double> return_vNPostTestDiagnosisFalsePosOut(int index) { return extractVector(__vNPostTestDiagnosisFalsePosOut, index); }
    inline vector<double> return_vNPostTestDiagnosisTotal(int index) { return extractVector(__vNPostTestDiagnosisTotal, index); }
    inline vector<double> return_vNPostTestDiagnosisReview(int index) { return extractVector(__vNPostTestDiagnosisReview, index); }
    inline vector<double> return_vNMDDiagnosisTotal(int index) { return extractVector(__vNMDDiagnosisTotal, index); }
    inline vector<double> return_vNMDDiagnosisReview(int index) { return extractVector(__vNMDDiagnosisReview, index); }
    inline vector<double> return_vNDiagnosis(int index){return extractVector(__vNDiagnosis, index);}
    inline vector<double> return_vNDiagnosisIn(int index){return extractVector(__vNDiagnosisIn, index);}
    inline vector<double> return_vNDiagnosisOut(int index){return extractVector(__vNDiagnosisOut, index);}
    inline vector<double> return_vNDiagnosisDC(int index) { return extractVector(__vNDiagnosisDC, index); }
    inline vector<double> return_vNDiagnosisDCTruePos(int index) { return extractVector(__vNDiagnosisDCTruePos, index); }
    inline vector<double> return_vNDiagnosisDCFalsePos(int index) { return extractVector(__vNDiagnosisDCFalsePos, index); }
    inline vector<double> return_vNPreTestDiagnosisTruePos(int index) { return extractVector(__vNPreTestDiagnosisTruePos, index); }
    inline vector<double> return_vNPreTestDiagnosisFalseNeg(int index) { return extractVector(__vNPreTestDiagnosisFalseNeg, index); }
    inline vector<double> return_vNPreTestDiagnosisTrueNeg(int index) { return extractVector(__vNPreTestDiagnosisTrueNeg, index); }
    inline vector<double> return_vNPreTestDiagnosisFalsePos(int index) { return extractVector(__vNPreTestDiagnosisFalsePos, index); }
    inline vector<double> return_vNPreTestDiagnosisTotal(int index) { return extractVector(__vNPreTestDiagnosisTotal, index); }
    inline vector<double> return_vNPreTestDiagnosisReview(int index) { return extractVector(__vNPreTestDiagnosisReview, index); }
    inline vector<double> return_vNPreTestLtFU(int index){return extractVector(__vNPreTestLtFU, index);}
    inline vector<double> return_vNPreTestLtFUInf(int index) { return extractVector(__vNPreTestLtFUInf, index); }
    inline vector<double> return_vNPreTestLtFUDied(int index){return extractVector(__vNPreTestLtFUDied, index);}
    inline vector<double> return_vNPreTestLtFUCured(int index) { return extractVector(__vNPreTestLtFUCured, index); }
    inline vector<double> return_vNPreTestLtFURepresent(int index){return extractVector(__vNPreTestLtFURepresent, index);}
    inline vector<double> return_vNPreTreatLtFU(int index){return extractVector(__vNPreTreatLtFU, index);}
    inline vector<double> return_vNPreTreatLtFUInf(int index) { return extractVector(__vNPreTreatLtFUInf, index); }
    inline vector<double> return_vNPreTreatLtFUDied(int index){return extractVector(__vNPreTreatLtFUDied, index);}
    inline vector<double> return_vNPreTreatLtFUCured(int index) { return extractVector(__vNPreTreatLtFUCured, index); }
    inline vector<double> return_vNPreTreatLtFURepresent(int index){return extractVector(__vNPreTreatLtFURepresent, index);}
    inline vector<double> return_vNPreTreatLtFUFN(int index) { return extractVector(__vNPreTreatLtFUFN, index); }
    inline vector<double> return_vNPreTreatLtFUFNDied(int index) { return extractVector(__vNPreTreatLtFUFNDied, index); }
    inline vector<double> return_vNPreTreatLtFUFNCured(int index) { return extractVector(__vNPreTreatLtFUFNCured, index); }
    inline vector<double> return_vNPreTreatLtFUFNRepresent(int index) { return extractVector(__vNPreTreatLtFUFNRepresent, index); }
    inline vector<double> return_vNPreTreatLtFUPos(int index) { return extractVector(__vNPreTreatLtFUPos, index); }
    inline vector<double> return_vNPreTreatLtFUNeg(int index) { return extractVector(__vNPreTreatLtFUNeg, index); }
    inline vector<double> return_vNTreatLtFU(int index){return extractVector(__vNTreatLtFU, index);}
    inline vector<double> return_vNTreatLtFUInf(int index) {return extractVector(__vNTreatLtFUInf, index); }
    inline vector<double> return_vNTreatLtFUDied(int index){return extractVector(__vNTreatLtFUDied, index);}
    inline vector<double> return_vNTreatLtFUCured(int index) { return extractVector(__vNTreatLtFUCured, index); }
    inline vector<double> return_vNTreatLtFURepresent(int index){return extractVector(__vNTreatLtFURepresent, index);}
    inline vector<double> return_vNPatientsTreated(int index){return extractVector(__vNPatientsTreated, index);}
    inline vector<double> return_vNPatientsTreatedInf(int index){return extractVector(__vNPatientsTreatedInf, index); }
    inline vector<double> return_vNPatientsTreatedUninf(int index){return extractVector(__vNPatientsTreatedUninf, index); }
    inline vector<double> return_vNPatientsPreTreatedLtoFUninf(int index){return extractVector(__vNPatientsPreTreatedLtoFUninf, index); }
    inline vector<double> return_vNPatientsTreatedCured(int index){return extractVector(__vNPatientsTreatedCured, index);}
    inline vector<double> return_vNPatientsTreatedDied(int index){return extractVector(__vNPatientsTreatedDied, index);}
    inline vector<double> return_vNPatientsTreatedRelapse(int index){return extractVector(__vNPatientsTreatedRelapse, index);}
    inline vector<double> return_vNTreatedRelapse(int index){return extractVector(__vNTreatedRelapse, index);}
    inline vector<double> return_vNTreatedRelapseCured(int index){return extractVector(__vNTreatedRelapseCured, index);}
    inline vector<double> return_vNTreatedRelapseDied(int index){return extractVector(__vNTreatedRelapseDied, index);}
    inline vector<double> return_vAvePresentToSampleDiagIn(int index){return doubleVecDivide(extractVector(__vTimePresentToSampleDiagIn, index),extractVector(__vNPresentToSampleDiagIn, index));}
    inline vector<double> return_vTimePresentToSampleDiagIn(int index){return extractVector(__vTimePresentToSampleDiagIn, index);}
    inline vector<double> return_vNPresentToSampleDiagIn(int index){return extractVector(__vNPresentToSampleDiagIn, index);}
    inline vector<double> return_vAvePresentToSampleDiagOut(int index){return doubleVecDivide(extractVector(__vTimePresentToSampleDiagOut, index),extractVector(__vNPresentToSampleDiagOut, index));}
    inline vector<double> return_vTimePresentToSampleDiagOut(int index){return extractVector(__vTimePresentToSampleDiagOut, index);}
    inline vector<double> return_vNPresentToSampleDiagOut(int index){return extractVector(__vNPresentToSampleDiagOut, index);}
    inline vector<double> return_vAvePresentToSampleTreat(int index){return doubleVecDivide(extractVector(__vTimePresentToSampleTreat, index),extractVector(__vNPresentToSampleTreat, index));}
    inline vector<double> return_vTimePresentToSampleTreat(int index){return extractVector(__vTimePresentToSampleTreat, index);}
    inline vector<double> return_vNPresentToSampleTreat(int index){return extractVector(__vNPresentToSampleTreat, index);}
    inline vector<double> return_vAvePresentToSampleTreatIn(int index){return doubleVecDivide(extractVector(__vTimePresentToSampleTreatIn, index),extractVector(__vNPresentToSampleTreatIn, index));}
    inline vector<double> return_vTimePresentToSampleTreatIn(int index){return extractVector(__vTimePresentToSampleTreatIn, index);}
    inline vector<double> return_vNPresentToSampleTreatIn(int index){return extractVector(__vNPresentToSampleTreatIn, index);}
    inline vector<double> return_vAvePresentToSampleTreatOut(int index){return doubleVecDivide(extractVector(__vTimePresentToSampleTreatOut, index),extractVector(__vNPresentToSampleTreatOut, index));}
    inline vector<double> return_vTimePresentToSampleTreatOut(int index){return extractVector(__vTimePresentToSampleTreatOut, index);}
    inline vector<double> return_vNPresentToSampleTreatOut(int index){return extractVector(__vNPresentToSampleTreatOut, index); }
    inline vector<double> return_vAvePresentToSampleDeath(int index){return doubleVecDivide(extractVector(__vTimePresentToSampleDeath, index),extractVector(__vNPresentToSampleDeath, index));}
    inline vector<double> return_vTimePresentToSampleDeath(int index){return extractVector(__vTimePresentToSampleDeath, index);}
    inline vector<double> return_vNPresentToSampleDeath(int index){return extractVector(__vNPresentToSampleDeath, index);}
    inline vector<double> return_vAvePresentToSampleDeathIn(int index){return doubleVecDivide(extractVector(__vTimePresentToSampleDeathIn, index),extractVector(__vNPresentToSampleDeathIn, index));}
    inline vector<double> return_vTimePresentToSampleDeathIn(int index){return extractVector(__vTimePresentToSampleDeathIn, index);}
    inline vector<double> return_vNPresentToSampleDeathIn(int index){return extractVector(__vNPresentToSampleDeathIn, index);}
    inline vector<double> return_vAvePresentToSampleDeathOut(int index){return doubleVecDivide(extractVector(__vTimePresentToSampleDeathOut, index),extractVector(__vNPresentToSampleDeathOut, index));}
    inline vector<double> return_vTimePresentToSampleDeathOut(int index){return extractVector(__vTimePresentToSampleDeathOut, index);}
    inline vector<double> return_vNPresentToSampleDeathOut(int index){return extractVector(__vNPresentToSampleDeathOut, index);}
    inline vector<double> return_vAvePresentToSampleDeathPreTestLTFU(int index){return doubleVecDivide(extractVector(__vTimePresentToSampleDeathPreTestLTFU, index),extractVector(__vNPresentToSampleDeathPreTestLTFU, index));}
    inline vector<double> return_vTimePresentToSampleDeathPreTestLTFU(int index){return extractVector(__vTimePresentToSampleDeathPreTestLTFU, index);}
    inline vector<double> return_vNPresentToSampleDeathPreTestLTFU(int index){return extractVector(__vNPresentToSampleDeathPreTestLTFU, index); }
    inline vector<double> return_vAvePresentToSampleDeathPreTreatLTFU(int index){return doubleVecDivide(extractVector(__vTimePresentToSampleDeathPreTreatLTFU, index),extractVector(__vNPresentToSampleDeathPreTreatLTFU, index));}
    inline vector<double> return_vTimePresentToSampleDeathPreTreatLTFU(int index){return extractVector(__vTimePresentToSampleDeathPreTreatLTFU, index);}
    inline vector<double> return_vNPresentToSampleDeathPreTreatLTFU(int index){return extractVector(__vNPresentToSampleDeathPreTreatLTFU, index); }
    inline vector<double> return_vAvePresentToSampleDeathTreatLTFU(int index){return doubleVecDivide(extractVector(__vTimePresentToSampleDeathTreatLTFU, index),extractVector(__vNPresentToSampleDeathTreatLTFU, index));}
    inline vector<double> return_vTimePresentToSampleDeathTreatLTFU(int index){return extractVector(__vTimePresentToSampleDeathTreatLTFU, index);}
    inline vector<double> return_vNPresentToSampleDeathTreatLTFU(int index){return extractVector(__vNPresentToSampleDeathTreatLTFU, index); }
    inline vector<double> return_vAvePresentToSampleDeathInTreat(int index){return doubleVecDivide(extractVector(__vTimePresentToSampleDeathInTreat, index),extractVector(__vNPresentToSampleDeathInTreat, index));}
    inline vector<double> return_vTimePresentToSampleDeathInTreat(int index){return extractVector(__vTimePresentToSampleDeathInTreat, index);}
    inline vector<double> return_vNPresentToSampleDeathInTreat(int index){return extractVector(__vNPresentToSampleDeathInTreat, index);}
    inline vector<double> return_vAvePresentToSampleDeathInRelapse(int index){return doubleVecDivide(extractVector(__vTimePresentToSampleDeathInRelapse, index),extractVector(__vNPresentToSampleDeathInRelapse, index));}
    inline vector<double> return_vTimePresentToSampleDeathInRelapse(int index){return extractVector(__vTimePresentToSampleDeathInRelapse, index);}
    inline vector<double> return_vNPresentToSampleDeathInRelapse(int index){return extractVector(__vNPresentToSampleDeathInRelapse, index);}
    inline vector<double> return_vAvePresentToExit(int index){return doubleVecDivide(extractVector(__vTimePresentToExit, index),extractVector(__vNPresentToExit, index));}
    inline vector<double> return_vTimePresentToExit(int index){return extractVector(__vTimePresentToExit, index);}
    inline vector<double> return_vNPresentToExit(int index){return extractVector(__vNPresentToExit, index);}
    inline vector<double> return_vNXpert(int index){return extractVector(__vNXpert, index);}
    inline vector<double> return_vNXpertIn(int index){return extractVector(__vNXpertIn, index);}
    inline vector<double> return_vNXpertOut(int index){return extractVector(__vNXpertOut, index);}
    inline vector<double> return_vNXpertPos(int index){return extractVector(__vNXpertPos, index);}
    inline vector<double> return_vPXpertTruePos(int index){return doubleVecDivide(extractVector(__vNXpertTruePos, index),extractVector(__vNXpertPos, index));}
    inline vector<double> return_vNXpertTruePos(int index){return extractVector(__vNXpertTruePos, index);}
    inline vector<double> return_vNXpertFalsePos(int index){return extractVector(__vNXpertFalsePos, index);}
    inline vector<double> return_vNXpertNeg(int index){return extractVector(__vNXpertNeg, index);}
    inline vector<double> return_vPXpertTrueNeg(int index){return doubleVecDivide(extractVector(__vNXpertTrueNeg, index),extractVector(__vNXpertNeg, index));}
    inline vector<double> return_vNXpertTrueNeg(int index){return extractVector(__vNXpertTrueNeg, index);}
    inline vector<double> return_vNXpertFalseNeg(int index){return extractVector(__vNXpertFalseNeg, index);}
    inline vector<double> return_vNSmear(int index){return extractVector(__vNSmear, index);}
    inline vector<double> return_vNSmearIn(int index){return extractVector(__vNSmearIn, index);}
    inline vector<double> return_vNSmearOut(int index){return extractVector(__vNSmearOut, index);}
    inline vector<double> return_vNSmearPos(int index){return extractVector(__vNSmearPos, index);}
    inline vector<double> return_vPSmearTruePos(int index){return doubleVecDivide(extractVector(__vNSmearTruePos, index),extractVector(__vNSmearPos, index));}
    inline vector<double> return_vNSmearTruePos(int index){return extractVector(__vNSmearTruePos, index);}
    inline vector<double> return_vNSmearFalsePos(int index){return extractVector(__vNSmearFalsePos, index);}
    inline vector<double> return_vNSmearNeg(int index){return extractVector(__vNSmearNeg, index);}
    inline vector<double> return_vPSmearTrueNeg(int index){return doubleVecDivide(extractVector(__vNSmearTrueNeg, index),extractVector(__vNSmearNeg, index));}
    inline vector<double> return_vNSmearTrueNeg(int index){return extractVector(__vNSmearTrueNeg, index);}
    inline vector<double> return_vNSmearFalseNeg(int index){return extractVector(__vNSmearFalseNeg, index);}
    inline vector<double> return_vNDeathPreTest(int index){return extractVector(__vNDeathPreTest, index);}
    inline vector<double> return_vNDeathPreTreat(int index){return extractVector(__vNDeathPreTreat, index);}
    inline vector<double> return_vNDeathTreat(int index){return extractVector(__vNDeathTreat, index);}
    inline vector<double> return_vNDeathReTreat(int index){return extractVector(__vNDeathReTreat, index);}
    inline vector<double> return_vNDeathUntreat(int index){return extractVector(__vNDeathUntreat, index);}
    inline vector<double> return_vNPatientInfDays(int index) { return extractVector(__vNPatientInfDays, index); }
    inline vector<double> return_vNPatientInfDaysYear0(int index) { return extractVector(__vNPatientInfDaysYear0, index); }
    inline vector<double> return_vNPatientInfDaysYear1(int index) { return extractVector(__vNPatientInfDaysYear1, index); }
    inline vector<double> return_vNPatientInfDaysYear2(int index) { return extractVector(__vNPatientInfDaysYear2, index); }
    inline vector<double> return_vNPatientInfDaysYear3(int index) { return extractVector(__vNPatientInfDaysYear3, index); }
    inline vector<double> return_vNPatientInfDaysYear4(int index) { return extractVector(__vNPatientInfDaysYear4, index); }
    inline vector<double> return_vNPatientInfDaysYear5(int index) { return extractVector(__vNPatientInfDaysYear5, index); }
    inline vector<double> return_vNPatientInfDaysYear6(int index) { return extractVector(__vNPatientInfDaysYear6, index); }
    inline vector<double> return_vNPatientInfDaysYear7(int index) { return extractVector(__vNPatientInfDaysYear7, index); }
    inline vector<double> return_vNPatientInfDaysYear8(int index) { return extractVector(__vNPatientInfDaysYear8, index); }
    inline vector<double> return_vNPatientInfDaysYear9(int index) { return extractVector(__vNPatientInfDaysYear9, index); }
    inline vector<double> return_vNPatientInfDaysYear10(int index) { return extractVector(__vNPatientInfDaysYear10, index); }
    inline vector<double> return_vNPatientInfDaysYear11(int index) { return extractVector(__vNPatientInfDaysYear11, index); }
    inline vector<double> return_vNPatientInfDaysYear12(int index) { return extractVector(__vNPatientInfDaysYear12, index); }
    inline vector<double> return_vNPatientInfDaysYear13(int index) { return extractVector(__vNPatientInfDaysYear13, index); }
    inline vector<double> return_vNPatientInfDaysYear14(int index) { return extractVector(__vNPatientInfDaysYear14, index); }
    inline vector<double> return_vNPatientInfDaysYear15(int index) { return extractVector(__vNPatientInfDaysYear15, index); }
    inline vector<double> return_vNPatientInfDaysAct(int index) { return extractVector(__vNPatientInfDaysAct, index); }
    inline vector<double> return_vNPatientInfDaysActYear0(int index) { return extractVector(__vNPatientInfDaysActYear0, index); }
    inline vector<double> return_vNPatientInfDaysActYear1(int index) { return extractVector(__vNPatientInfDaysActYear1, index); }
    inline vector<double> return_vNPatientInfDaysActYear2(int index) { return extractVector(__vNPatientInfDaysActYear2, index); }
    inline vector<double> return_vNPatientInfDaysActYear3(int index) { return extractVector(__vNPatientInfDaysActYear3, index); }
    inline vector<double> return_vNPatientInfDaysActYear4(int index) { return extractVector(__vNPatientInfDaysActYear4, index); }
    inline vector<double> return_vNPatientInfDaysActYear5(int index) { return extractVector(__vNPatientInfDaysActYear5, index); }
    inline vector<double> return_vNPatientInfDaysActYear6(int index) { return extractVector(__vNPatientInfDaysActYear6, index); }
    inline vector<double> return_vNPatientInfDaysActYear7(int index) { return extractVector(__vNPatientInfDaysActYear7, index); }
    inline vector<double> return_vNPatientInfDaysActYear8(int index) { return extractVector(__vNPatientInfDaysActYear8, index); }
    inline vector<double> return_vNPatientInfDaysActYear9(int index) { return extractVector(__vNPatientInfDaysActYear9, index); }
    inline vector<double> return_vNPatientInfDaysActYear10(int index) { return extractVector(__vNPatientInfDaysActYear10, index); }
    inline vector<double> return_vNPatientInfDaysActYear11(int index) { return extractVector(__vNPatientInfDaysActYear11, index); }
    inline vector<double> return_vNPatientInfDaysActYear12(int index) { return extractVector(__vNPatientInfDaysActYear12, index); }
    inline vector<double> return_vNPatientInfDaysActYear13(int index) { return extractVector(__vNPatientInfDaysActYear13, index); }
    inline vector<double> return_vNPatientInfDaysActYear14(int index) { return extractVector(__vNPatientInfDaysActYear14, index); }
    inline vector<double> return_vNPatientInfDaysActYear15(int index) { return extractVector(__vNPatientInfDaysActYear15, index); }
    inline vector<double> return_vSPatientWaiting(int index){return extractVector(__vSPatientWaiting, index);}
    inline vector<double> return_vSPatientWaitingIn(int index){return extractVector(__vSPatientWaitingIn, index);}
    inline vector<double> return_vSPatientWaitingOut(int index){return extractVector(__vSPatientWaitingOut, index);}
    inline vector<double> return_vSPatientInTreatment(int index){return extractVector(__vSPatientInTreatment, index);}
    inline vector<double> return_vSSampleWaitingForTest(int index){return extractVector(__vSSampleWaitingForTest, index);}
    inline vector<double> return_vSSampleWaitingForTestIn(int index){return extractVector(__vSSampleWaitingForTestIn, index);}
    inline vector<double> return_vSSampleWaitingForTestOut(int index){return extractVector(__vSSampleWaitingForTestOut, index);}

    
    inline vector<string> returns_vNXpertMach(int index){return doubleVecToStrLarge(extractVector(__vXpertMach, index));}
    inline vector<string> returns_vNPopulation(int index) { return doubleVecToStrLarge(extractVector(__vPopulation, index)); }
    inline vector<string> returns_vNIncidence(int index){return doubleVecToStr(extractVector(__vIncidence, index));}
    inline vector<string> returns_vNCalcIncidence(int index){return doubleVecToStr(extractVector(__vCalcIncidence, index));}
    inline vector<string> returns_vName(int index){return extractStringVector(__vName, index);}
    inline vector<string> returns_vYear(int index){return doubleVecToStr(extractVector(__vYear, index));}
    inline vector<string> returns_vScenario(int index){return extractStringVector(__vScenario, index);}
    inline vector<string> returns_vDisease(int index){return extractStringVector(__vDisease, index);}
    inline vector<string> returns_vNPatient(int index){return doubleVecToStr(extractVector(__vNPatient, index));}
    inline vector<string> returns_vNPatientInf(int index){return doubleVecToStr(extractVector(__vNPatientInf, index));}
    inline vector<string> returns_vNPatientLtFUInf(int index){return doubleVecToStr(extractVector(__vNPatientLtFUInf, index));}
    inline vector<string> returns_vNPatientFromTrans(int index) { return doubleVecToStr(extractVector(__vNPatientFromTrans, index)); }
    inline vector<string> returns_vNPatientNotPresenting(int index) { return doubleVecToStr(extractVector(__vNPatientNotPresenting, index)); }
    inline vector<string> returns_vNSamples(int index){return doubleVecToStr(extractVector(__vNSamples, index));}
    inline vector<string> returns_vNSamplesIn(int index){return doubleVecToStr(extractVector(__vNSamplesIn, index));}
    inline vector<string> returns_vNSamplesOut(int index){return doubleVecToStr(extractVector(__vNSamplesOut, index));}
    inline vector<string> returns_vAveSampleResourceWait(int index){return doubleVecDivideToStr(extractVector(__vTimeSampleResourceWait, index),extractVector(__vNTests, index));}
    inline vector<string> returns_vTimeSampleResourceWait(int index){return doubleVecToStr(extractVector(__vTimeSampleResourceWait, index));}
    inline vector<string> returns_vAveSampleResourceWaitIn(int index){return doubleVecDivideToStr(extractVector(__vTimeSampleResourceWaitIn, index),extractVector(__vNTestsIn, index));}
    inline vector<string> returns_vTimeSampleResourceWaitIn(int index){return doubleVecToStr(extractVector(__vTimeSampleResourceWaitIn, index));}
    inline vector<string> returns_vAveSampleResourceWaitOut(int index){return doubleVecDivideToStr(extractVector(__vTimeSampleResourceWaitOut, index),extractVector(__vNTestsOut, index));}
    inline vector<string> returns_vTimeSampleResourceWaitOut(int index){return doubleVecToStr(extractVector(__vTimeSampleResourceWaitOut, index));}
    inline vector<string> returns_vAveSampleTransport(int index){return doubleVecDivideToStr(extractVector(__vTimeSampleTransport, index),extractVector(__vNTransported, index)); }
    inline vector<string> returns_vTimeSampleTransport(int index){return doubleVecToStr(extractVector(__vTimeSampleTransport, index)); }
    inline vector<string> returns_vNTransported(int index){return doubleVecToStr(extractVector(__vNTransported, index));}
    inline vector<string> returns_vNTests(int index){return doubleVecToStr(extractVector(__vNTests, index));}
    inline vector<string> returns_vNTestsIn(int index){return doubleVecToStr(extractVector(__vNTestsIn, index));}
    inline vector<string> returns_vNTestsOut(int index){return doubleVecToStr(extractVector(__vNTestsOut, index));}
    inline vector<string> returns_vAvePresentToSampleTestIn(int index){return doubleVecDivideToStr(extractVector(__vTimePresentToSampleTestIn, index),extractVector(__vNPresentToSampleTestIn, index));}
    inline vector<string> returns_vTimePresentToSampleTestIn(int index){return doubleVecToStr(extractVector(__vTimePresentToSampleTestIn, index));}
    inline vector<string> returns_vNPresentToSampleTestIn(int index){return doubleVecToStr(extractVector(__vNPresentToSampleTestIn, index)); }
    inline vector<string> returns_vAvePresentToSampleTestOut(int index){return doubleVecDivideToStr(extractVector(__vTimePresentToSampleTestOut, index),extractVector(__vNPresentToSampleTestOut, index));}
    inline vector<string> returns_vTimePresentToSampleTestOut(int index){return doubleVecToStr(extractVector(__vTimePresentToSampleTestOut, index));}
    inline vector<string> returns_vNPresentToSampleTestOut(int index){return doubleVecToStr( extractVector(__vNPresentToSampleTestOut, index));}
    inline vector<string> returns_vNTestPos(int index){return doubleVecToStr(extractVector(__vNTestPos, index));}
    inline vector<string> returns_vPTestTruePos(int index){return doubleVecDivideToStr(extractVector(__vNTestTruePos, index), extractVector(__vNTestPos, index));}
    inline vector<string> returns_vNTestTruePos(int index){return doubleVecToStr(extractVector(__vNTestTruePos, index));}
    inline vector<string> returns_vNTestTruePosIn(int index){return doubleVecToStr(extractVector(__vNTestTruePosIn, index));}
    inline vector<string> returns_vNTestTruePosOut(int index){return doubleVecToStr(extractVector(__vNTestTruePosOut, index));}
    inline vector<string> returns_vPTestFalsePos(int index){return doubleVecDivideToStr(extractVector(__vNTestFalsePos, index), extractVector(__vNTestPos, index));}
    inline vector<string> returns_vNTestFalsePos(int index){return doubleVecToStr(extractVector(__vNTestFalsePos, index));}
    inline vector<string> returns_vNTestFalsePosIn(int index){return doubleVecToStr(extractVector(__vNTestFalsePosIn, index));}
    inline vector<string> returns_vNTestFalsePosOut(int index){return doubleVecToStr(extractVector(__vNTestFalsePosOut, index));}
    inline vector<string> returns_vNTestNeg(int index){return doubleVecToStr(extractVector(__vNTestNeg, index));}
    inline vector<string> returns_vPTestTrueNeg(int index){return doubleVecDivideToStr(extractVector(__vNTestTrueNeg, index), extractVector(__vNTestNeg, index));}
    inline vector<string> returns_vNTestTrueNeg(int index){return doubleVecToStr(extractVector(__vNTestTrueNeg, index));}
    inline vector<string> returns_vNTestTrueNegIn(int index){return doubleVecToStr(extractVector(__vNTestTrueNegIn, index));}
    inline vector<string> returns_vNTestTrueNegOut(int index){return doubleVecToStr(extractVector(__vNTestTrueNegOut, index));}
    inline vector<string> returns_vPTestFalseNeg(int index){return doubleVecDivideToStr(extractVector(__vNTestFalseNeg, index), extractVector(__vNTestNeg, index));}
    inline vector<string> returns_vNTestFalseNeg(int index){return doubleVecToStr(extractVector(__vNTestFalseNeg, index));}
    inline vector<string> returns_vNTestFalseNegIn(int index){return doubleVecToStr(extractVector(__vNTestFalseNegIn, index));}
    inline vector<string> returns_vNTestFalseNegOut(int index){return doubleVecToStr(extractVector(__vNTestFalseNegOut, index));}
    inline vector<string> returns_vNTestPosIn(int index){return doubleVecToStr(extractVector(__vNTestPosIn, index));}
    inline vector<string> returns_vNTestPosOut(int index){return doubleVecToStr(extractVector(__vNTestPosOut, index));}
    inline vector<string> returns_vNMDCDInfected(int index) { return doubleVecToStr(extractVector(__vNMDCDInfected, index)); }
    inline vector<string> returns_vNMDCDUninfected(int index) { return doubleVecToStr(extractVector(__vNMDCDUninfected, index)); }
    inline vector<string> returns_vNPTMDCDInfected(int index) { return doubleVecToStr(extractVector(__vNPTMDCDInfected, index)); }
    inline vector<string> returns_vNPTMDCDUninfected(int index) { return doubleVecToStr(extractVector(__vNPTMDCDUninfected, index)); }
    inline vector<string> returns_vNPostTestDiagnosisTruePosIn(int index) { return doubleVecToStr(extractVector(__vNPostTestDiagnosisTruePosIn, index)); }
    inline vector<string> returns_vNPostTestDiagnosisTruePosOut(int index) { return doubleVecToStr(extractVector(__vNPostTestDiagnosisTruePosOut, index)); }
    inline vector<string> returns_vNPostTestDiagnosisFalseNeg(int index){return doubleVecToStr(extractVector(__vNPostTestDiagnosisFalseNeg, index));}
    inline vector<string> returns_vNPostTestDiagnosisTrueNeg(int index){return doubleVecToStr(extractVector(__vNPostTestDiagnosisTrueNeg, index));}
    inline vector<string> returns_vNPostTestDiagnosisFalsePosIn(int index) { return doubleVecToStr(extractVector(__vNPostTestDiagnosisFalsePosIn, index)); }
    inline vector<string> returns_vNPostTestDiagnosisFalsePosOut(int index) { return doubleVecToStr(extractVector(__vNPostTestDiagnosisFalsePosOut, index)); }
    inline vector<string> returns_vNPostTestDiagnosisTotal(int index) { return doubleVecToStr(extractVector(__vNPostTestDiagnosisTotal, index)); }
    inline vector<string> returns_vNPostTestDiagnosisReview(int index) { return doubleVecToStr(extractVector(__vNPostTestDiagnosisReview, index)); }
    inline vector<string> returns_vNMDDiagnosisTotal(int index) { return doubleVecToStr(extractVector(__vNMDDiagnosisTotal, index)); }
    inline vector<string> returns_vNMDDiagnosisReview(int index) { return doubleVecToStr(extractVector(__vNMDDiagnosisReview, index)); }
    inline vector<string> returns_vNDiagnosis(int index){return doubleVecToStr(extractVector(__vNDiagnosis, index));}
    inline vector<string> returns_vNDiagnosisIn(int index){return doubleVecToStr(extractVector(__vNDiagnosisIn, index));}
    inline vector<string> returns_vNDiagnosisOut(int index){return doubleVecToStr(extractVector(__vNDiagnosisOut, index));}
    inline vector<string> returns_vNDiagnosisDC(int index) { return doubleVecToStr(extractVector(__vNDiagnosisDC, index)); }
    inline vector<string> returns_vNDiagnosisDCTruePos(int index) { return doubleVecToStr(extractVector(__vNDiagnosisDCTruePos, index)); }
    inline vector<string> returns_vNDiagnosisDCFalsePos(int index) { return doubleVecToStr(extractVector(__vNDiagnosisDCFalsePos, index)); }
    inline vector<string> returns_vNPreTestDiagnosisTruePos(int index) { return doubleVecToStr(extractVector(__vNPreTestDiagnosisTruePos, index)); }
    inline vector<string> returns_vNPreTestDiagnosisFalseNeg(int index) { return doubleVecToStr(extractVector(__vNPreTestDiagnosisFalseNeg, index)); }
    inline vector<string> returns_vNPreTestDiagnosisTrueNeg(int index) { return doubleVecToStr(extractVector(__vNPreTestDiagnosisTrueNeg, index)); }
    inline vector<string> returns_vNPreTestDiagnosisFalsePos(int index) { return doubleVecToStr(extractVector(__vNPreTestDiagnosisFalsePos, index)); }
    inline vector<string> returns_vNPreTestDiagnosisTotal(int index) { return doubleVecToStr(extractVector(__vNPreTestDiagnosisTotal, index)); }
    inline vector<string> returns_vNPreTestDiagnosisReview(int index) { return doubleVecToStr(extractVector(__vNPreTestDiagnosisReview, index)); }
    inline vector<string> returns_vNPreTestLtFU(int index){return doubleVecToStr(extractVector(__vNPreTestLtFU, index));}
    inline vector<string> returns_vNPreTestLtFUInf(int index) { return doubleVecToStr(extractVector(__vNPreTestLtFUInf, index)); }
    inline vector<string> returns_vNPreTestLtFUDied(int index){return doubleVecToStr(extractVector(__vNPreTestLtFUDied, index));}
    inline vector<string> returns_vNPreTestLtFUCured(int index) { return doubleVecToStr(extractVector(__vNPreTestLtFUCured, index)); }
    inline vector<string> returns_vNPreTestLtFURepresent(int index){return doubleVecToStr(extractVector(__vNPreTestLtFURepresent, index));}
    inline vector<string> returns_vNPreTreatLtFU(int index){return doubleVecToStr(extractVector(__vNPreTreatLtFU, index));}
    inline vector<string> returns_vNPreTreatLtFUInf(int index) { return doubleVecToStr(extractVector(__vNPreTreatLtFUInf, index)); }
    inline vector<string> returns_vNPreTreatLtFUDied(int index){return doubleVecToStr(extractVector(__vNPreTreatLtFUDied, index));}
    inline vector<string> returns_vNPreTreatLtFUCured(int index) { return doubleVecToStr(extractVector(__vNPreTreatLtFUCured, index)); }
    inline vector<string> returns_vNPreTreatLtFURepresent(int index){return doubleVecToStr(extractVector(__vNPreTreatLtFURepresent, index));}
    inline vector<string> returns_vNPreTreatLtFUFN(int index) { return doubleVecToStr(extractVector(__vNPreTreatLtFUFN, index)); }
    inline vector<string> returns_vNPreTreatLtFUFNDied(int index) { return doubleVecToStr(extractVector(__vNPreTreatLtFUFNDied, index)); }
    inline vector<string> returns_vNPreTreatLtFUFNCured(int index) { return doubleVecToStr(extractVector(__vNPreTreatLtFUFNCured, index)); }
    inline vector<string> returns_vNPreTreatLtFUFNRepresent(int index) { return doubleVecToStr(extractVector(__vNPreTreatLtFUFNRepresent, index)); }
    inline vector<string> returns_vNPreTreatLtFUPos(int index) { return doubleVecToStr(extractVector(__vNPreTreatLtFUPos, index)); }
    inline vector<string> returns_vNPreTreatLtFUNeg(int index) { return doubleVecToStr(extractVector(__vNPreTreatLtFUNeg, index)); }
    inline vector<string> returns_vNTreatLtFU(int index){return doubleVecToStr(extractVector(__vNTreatLtFU, index));}
    inline vector<string> returns_vNTreatLtFUInf(int index) {return doubleVecToStr(extractVector(__vNTreatLtFUInf, index)); }
    inline vector<string> returns_vNTreatLtFUDied(int index){return doubleVecToStr(extractVector(__vNTreatLtFUDied, index));}
    inline vector<string> returns_vNTreatLtFUCured(int index) { return doubleVecToStr(extractVector(__vNTreatLtFUCured, index)); }
    inline vector<string> returns_vNTreatLtFURepresent(int index){return doubleVecToStr(extractVector(__vNTreatLtFURepresent, index));}
    inline vector<string> returns_vNPatientsTreated(int index){return doubleVecToStr( extractVector(__vNPatientsTreated, index));}
    inline vector<string> returns_vNPatientsTreatedInf(int index){return doubleVecToStr(extractVector(__vNPatientsTreatedInf, index)); }
    inline vector<string> returns_vNPatientsTreatedUninf(int index){return doubleVecToStr(extractVector(__vNPatientsTreatedUninf, index)); }
    inline vector<string> returns_vNPatientsPreTreatedLtoFUninf(int index){return doubleVecToStr(extractVector(__vNPatientsPreTreatedLtoFUninf, index)); }
    inline vector<string> returns_vNPatientsTreatedCured(int index){return doubleVecToStr(extractVector(__vNPatientsTreatedCured, index));}
    inline vector<string> returns_vNPatientsTreatedDied(int index){return doubleVecToStr(extractVector(__vNPatientsTreatedDied, index));}
    inline vector<string> returns_vNPatientsTreatedRelapse(int index){return doubleVecToStr(extractVector(__vNPatientsTreatedRelapse, index));}
    inline vector<string> returns_vNTreatedRelapse(int index){return doubleVecToStr(extractVector(__vNTreatedRelapse, index));}
    inline vector<string> returns_vNTreatedRelapseCured(int index){return doubleVecToStr(extractVector(__vNTreatedRelapseCured, index));}
    inline vector<string> returns_vNTreatedRelapseDied(int index){return doubleVecToStr(extractVector(__vNTreatedRelapseDied, index));}
    inline vector<string> returns_vAvePresentToSampleDiagIn(int index){return doubleVecDivideToStr(extractVector(__vTimePresentToSampleDiagIn, index),extractVector(__vNPresentToSampleDiagIn, index));}
    inline vector<string> returns_vTimePresentToSampleDiagIn(int index){return doubleVecToStr(extractVector(__vTimePresentToSampleDiagIn, index));}
    inline vector<string> returns_vNPresentToSampleDiagIn(int index){return doubleVecToStr(extractVector(__vNPresentToSampleDiagIn, index));}
    inline vector<string> returns_vAvePresentToSampleDiagOut(int index){return doubleVecDivideToStr(extractVector(__vTimePresentToSampleDiagOut, index),extractVector(__vNPresentToSampleDiagOut, index));}
    inline vector<string> returns_vTimePresentToSampleDiagOut(int index){return doubleVecToStr(extractVector(__vTimePresentToSampleDiagOut, index));}
    inline vector<string> returns_vNPresentToSampleDiagOut(int index){return doubleVecToStr(extractVector(__vNPresentToSampleDiagOut, index));}
    inline vector<string> returns_vAvePresentToSampleTreat(int index){return doubleVecDivideToStr(extractVector(__vTimePresentToSampleTreat, index),extractVector(__vNPresentToSampleTreat, index));}
    inline vector<string> returns_vTimePresentToSampleTreat(int index){return doubleVecToStr(extractVector(__vTimePresentToSampleTreat, index));}
    inline vector<string> returns_vNPresentToSampleTreat(int index){return doubleVecToStr(extractVector(__vNPresentToSampleTreat, index));}
    inline vector<string> returns_vAvePresentToSampleTreatIn(int index){return doubleVecDivideToStr(extractVector(__vTimePresentToSampleTreatIn, index),extractVector(__vNPresentToSampleTreatIn, index));}
    inline vector<string> returns_vTimePresentToSampleTreatIn(int index){return doubleVecToStr(extractVector(__vTimePresentToSampleTreatIn, index));}
    inline vector<string> returns_vNPresentToSampleTreatIn(int index){return doubleVecToStr(extractVector(__vNPresentToSampleTreatIn, index));}
    inline vector<string> returns_vAvePresentToSampleTreatOut(int index){return doubleVecDivideToStr(extractVector(__vTimePresentToSampleTreatOut, index),extractVector(__vNPresentToSampleTreatOut, index));}
    inline vector<string> returns_vTimePresentToSampleTreatOut(int index){return doubleVecToStr(extractVector(__vTimePresentToSampleTreatOut, index));}
    inline vector<string> returns_vNPresentToSampleTreatOut(int index){return doubleVecToStr( extractVector(__vNPresentToSampleTreatOut, index)); }
    inline vector<string> returns_vAvePresentToSampleDeath(int index){return doubleVecDivideToStr(extractVector(__vTimePresentToSampleDeath, index),extractVector(__vNPresentToSampleDeath, index));}
    inline vector<string> returns_vTimePresentToSampleDeath(int index){return doubleVecToStr(extractVector(__vTimePresentToSampleDeath, index));}
    inline vector<string> returns_vNPresentToSampleDeath(int index){return doubleVecToStr(extractVector(__vNPresentToSampleDeath, index));}
    inline vector<string> returns_vAvePresentToSampleDeathIn(int index){return doubleVecDivideToStr(extractVector(__vTimePresentToSampleDeathIn, index),extractVector(__vNPresentToSampleDeathIn, index));}
    inline vector<string> returns_vTimePresentToSampleDeathIn(int index){return doubleVecToStr(extractVector(__vTimePresentToSampleDeathIn, index));}
    inline vector<string> returns_vNPresentToSampleDeathIn(int index){return doubleVecToStr(extractVector(__vNPresentToSampleDeathIn, index));}
    inline vector<string> returns_vAvePresentToSampleDeathOut(int index){return doubleVecDivideToStr(extractVector(__vTimePresentToSampleDeathOut, index),extractVector(__vNPresentToSampleDeathOut, index));}
    inline vector<string> returns_vTimePresentToSampleDeathOut(int index){return doubleVecToStr(extractVector(__vTimePresentToSampleDeathOut, index));}
    inline vector<string> returns_vNPresentToSampleDeathOut(int index){return doubleVecToStr(extractVector(__vNPresentToSampleDeathOut, index));}
    inline vector<string> returns_vAvePresentToSampleDeathPreTestLTFU(int index){return doubleVecDivideToStr(extractVector(__vTimePresentToSampleDeathPreTestLTFU, index),extractVector(__vNPresentToSampleDeathPreTestLTFU, index));}
    inline vector<string> returns_vTimePresentToSampleDeathPreTestLTFU(int index){return doubleVecToStr(extractVector(__vTimePresentToSampleDeathPreTestLTFU, index));}
    inline vector<string> returns_vNPresentToSampleDeathPreTestLTFU(int index){return doubleVecToStr(extractVector(__vNPresentToSampleDeathPreTestLTFU, index)); }
    inline vector<string> returns_vAvePresentToSampleDeathPreTreatLTFU(int index){return doubleVecDivideToStr(extractVector(__vTimePresentToSampleDeathPreTreatLTFU, index),extractVector(__vNPresentToSampleDeathPreTreatLTFU, index));}
    inline vector<string> returns_vTimePresentToSampleDeathPreTreatLTFU(int index){return doubleVecToStr(extractVector(__vTimePresentToSampleDeathPreTreatLTFU, index));}
    inline vector<string> returns_vNPresentToSampleDeathPreTreatLTFU(int index){return doubleVecToStr(extractVector(__vNPresentToSampleDeathPreTreatLTFU, index)); }
    inline vector<string> returns_vAvePresentToSampleDeathTreatLTFU(int index){return doubleVecDivideToStr(extractVector(__vTimePresentToSampleDeathTreatLTFU, index),extractVector(__vNPresentToSampleDeathTreatLTFU, index));}
    inline vector<string> returns_vTimePresentToSampleDeathTreatLTFU(int index){return doubleVecToStr(extractVector(__vTimePresentToSampleDeathTreatLTFU, index));}
    inline vector<string> returns_vNPresentToSampleDeathTreatLTFU(int index){return doubleVecToStr(extractVector(__vNPresentToSampleDeathTreatLTFU, index)); }
    inline vector<string> returns_vAvePresentToSampleDeathInTreat(int index){return doubleVecDivideToStr(extractVector(__vTimePresentToSampleDeathInTreat, index),extractVector(__vNPresentToSampleDeathInTreat, index));}
    inline vector<string> returns_vTimePresentToSampleDeathInTreat(int index){return doubleVecToStr(extractVector(__vTimePresentToSampleDeathInTreat, index));}
    inline vector<string> returns_vNPresentToSampleDeathInTreat(int index){return doubleVecToStr(extractVector(__vNPresentToSampleDeathInTreat, index));}
    inline vector<string> returns_vAvePresentToSampleDeathInRelapse(int index){return doubleVecDivideToStr(extractVector(__vTimePresentToSampleDeathInRelapse, index),extractVector(__vNPresentToSampleDeathInRelapse, index));}
    inline vector<string> returns_vTimePresentToSampleDeathInRelapse(int index){return doubleVecToStr(extractVector(__vTimePresentToSampleDeathInRelapse, index));}
    inline vector<string> returns_vNPresentToSampleDeathInRelapse(int index){return doubleVecToStr(extractVector(__vNPresentToSampleDeathInRelapse, index));}
    inline vector<string> returns_vAvePresentToExit(int index){return doubleVecDivideToStr(extractVector(__vTimePresentToExit, index),extractVector(__vNPresentToExit, index));}
    inline vector<string> returns_vTimePresentToExit(int index){return doubleVecToStr(extractVector(__vTimePresentToExit, index));}
    inline vector<string> returns_vNPresentToExit(int index){return doubleVecToStr(extractVector(__vNPresentToExit, index));}
    inline vector<string> returns_vNXpert(int index){return doubleVecToStr(extractVector(__vNXpert, index));}
    inline vector<string> returns_vNXpertIn(int index){return doubleVecToStr(extractVector(__vNXpertIn, index));}
    inline vector<string> returns_vNXpertOut(int index){return doubleVecToStr(extractVector(__vNXpertOut, index));}
    inline vector<string> returns_vNXpertPos(int index){return doubleVecToStr(extractVector(__vNXpertPos, index));}
    inline vector<string> returns_vPXpertTruePos(int index){return doubleVecDivideToStr(extractVector(__vNXpertTruePos, index),extractVector(__vNXpertPos, index));}
    inline vector<string> returns_vNXpertTruePos(int index){return doubleVecToStr(extractVector(__vNXpertTruePos, index));}
    inline vector<string> returns_vNXpertFalsePos(int index){return doubleVecToStr(extractVector(__vNXpertFalsePos, index));}
    inline vector<string> returns_vNXpertNeg(int index){return doubleVecToStr(extractVector(__vNXpertNeg, index));}
    inline vector<string> returns_vPXpertTrueNeg(int index){return doubleVecDivideToStr(extractVector(__vNXpertTrueNeg, index),extractVector(__vNXpertNeg, index));}
    inline vector<string> returns_vNXpertTrueNeg(int index){return doubleVecToStr(extractVector(__vNXpertTrueNeg, index));}
    inline vector<string> returns_vNXpertFalseNeg(int index){return doubleVecToStr(extractVector(__vNXpertFalseNeg, index));}
    inline vector<string> returns_vNSmear(int index){return doubleVecToStr(extractVector(__vNSmear, index));}
    inline vector<string> returns_vNSmearIn(int index){return doubleVecToStr(extractVector(__vNSmearIn, index));}
    inline vector<string> returns_vNSmearOut(int index){return doubleVecToStr(extractVector(__vNSmearOut, index));}
    inline vector<string> returns_vNSmearPos(int index){return doubleVecToStr(extractVector(__vNSmearPos, index));}
    inline vector<string> returns_vPSmearTruePos(int index){return doubleVecDivideToStr(extractVector(__vNSmearTruePos, index),extractVector(__vNSmearPos, index));}
    inline vector<string> returns_vNSmearTruePos(int index){return doubleVecToStr(extractVector(__vNSmearTruePos, index));}
    inline vector<string> returns_vNSmearFalsePos(int index){return doubleVecToStr(extractVector(__vNSmearFalsePos, index));}
    inline vector<string> returns_vNSmearNeg(int index){return doubleVecToStr(extractVector(__vNSmearNeg, index));}
    inline vector<string> returns_vPSmearTrueNeg(int index){return doubleVecDivideToStr(extractVector(__vNSmearTrueNeg, index),extractVector(__vNSmearNeg, index));}
    inline vector<string> returns_vNSmearTrueNeg(int index){return doubleVecToStr(extractVector(__vNSmearTrueNeg, index));}
    inline vector<string> returns_vNSmearFalseNeg(int index){return doubleVecToStr(extractVector(__vNSmearFalseNeg, index));}
    inline vector<string> returns_vNDeathPreTest(int index){return doubleVecToStr(extractVector(__vNDeathPreTest, index));}
    inline vector<string> returns_vNDeathPreTreat(int index){return doubleVecToStr(extractVector(__vNDeathPreTreat, index));}
    inline vector<string> returns_vNDeathTreat(int index){return doubleVecToStr(extractVector(__vNDeathTreat, index));}
    inline vector<string> returns_vNDeathReTreat(int index){return doubleVecToStr(extractVector(__vNDeathReTreat, index));}
    inline vector<string> returns_vNDeathUntreat(int index){return doubleVecToStr(extractVector(__vNDeathUntreat, index));}
    inline vector<string> returns_vNPatientInfDays(int index) { return doubleVecToStr(extractVector(__vNPatientInfDays, index)); }
    inline vector<string> returns_vNPatientInfDaysYear0(int index) { return doubleVecToStr(extractVector(__vNPatientInfDaysYear0, index)); }
    inline vector<string> returns_vNPatientInfDaysYear1(int index) { return doubleVecToStr(extractVector(__vNPatientInfDaysYear1, index)); }
    inline vector<string> returns_vNPatientInfDaysYear2(int index) { return doubleVecToStr(extractVector(__vNPatientInfDaysYear2, index)); }
    inline vector<string> returns_vNPatientInfDaysYear3(int index) { return doubleVecToStr(extractVector(__vNPatientInfDaysYear3, index)); }
    inline vector<string> returns_vNPatientInfDaysYear4(int index) { return doubleVecToStr(extractVector(__vNPatientInfDaysYear4, index)); }
    inline vector<string> returns_vNPatientInfDaysYear5(int index) { return doubleVecToStr(extractVector(__vNPatientInfDaysYear5, index)); }
    inline vector<string> returns_vNPatientInfDaysYear6(int index) { return doubleVecToStr(extractVector(__vNPatientInfDaysYear6, index)); }
    inline vector<string> returns_vNPatientInfDaysYear7(int index) { return doubleVecToStr(extractVector(__vNPatientInfDaysYear7, index)); }
    inline vector<string> returns_vNPatientInfDaysYear8(int index) { return doubleVecToStr(extractVector(__vNPatientInfDaysYear8, index)); }
    inline vector<string> returns_vNPatientInfDaysYear9(int index) { return doubleVecToStr(extractVector(__vNPatientInfDaysYear9, index)); }
    inline vector<string> returns_vNPatientInfDaysYear10(int index) { return doubleVecToStr(extractVector(__vNPatientInfDaysYear10, index)); }
    inline vector<string> returns_vNPatientInfDaysYear11(int index) { return doubleVecToStr(extractVector(__vNPatientInfDaysYear11, index)); }
    inline vector<string> returns_vNPatientInfDaysYear12(int index) { return doubleVecToStr(extractVector(__vNPatientInfDaysYear12, index)); }
    inline vector<string> returns_vNPatientInfDaysYear13(int index) { return doubleVecToStr(extractVector(__vNPatientInfDaysYear13, index)); }
    inline vector<string> returns_vNPatientInfDaysYear14(int index) { return doubleVecToStr(extractVector(__vNPatientInfDaysYear14, index)); }
    inline vector<string> returns_vNPatientInfDaysYear15(int index) { return doubleVecToStr(extractVector(__vNPatientInfDaysYear15, index)); }
    inline vector<string> returns_vNPatientInfDaysAct(int index) { return doubleVecToStr(extractVector(__vNPatientInfDaysAct, index)); }
    inline vector<string> returns_vNPatientInfDaysActYear0(int index) { return doubleVecToStr(extractVector(__vNPatientInfDaysActYear0, index)); }
    inline vector<string> returns_vNPatientInfDaysActYear1(int index) { return doubleVecToStr(extractVector(__vNPatientInfDaysActYear1, index)); }
    inline vector<string> returns_vNPatientInfDaysActYear2(int index) { return doubleVecToStr(extractVector(__vNPatientInfDaysActYear2, index)); }
    inline vector<string> returns_vNPatientInfDaysActYear3(int index) { return doubleVecToStr(extractVector(__vNPatientInfDaysActYear3, index)); }
    inline vector<string> returns_vNPatientInfDaysActYear4(int index) { return doubleVecToStr(extractVector(__vNPatientInfDaysActYear4, index)); }
    inline vector<string> returns_vNPatientInfDaysActYear5(int index) { return doubleVecToStr(extractVector(__vNPatientInfDaysActYear5, index)); }
    inline vector<string> returns_vNPatientInfDaysActYear6(int index) { return doubleVecToStr(extractVector(__vNPatientInfDaysActYear6, index)); }
    inline vector<string> returns_vNPatientInfDaysActYear7(int index) { return doubleVecToStr(extractVector(__vNPatientInfDaysActYear7, index)); }
    inline vector<string> returns_vNPatientInfDaysActYear8(int index) { return doubleVecToStr(extractVector(__vNPatientInfDaysActYear8, index)); }
    inline vector<string> returns_vNPatientInfDaysActYear9(int index) { return doubleVecToStr(extractVector(__vNPatientInfDaysActYear9, index)); }
    inline vector<string> returns_vNPatientInfDaysActYear10(int index) { return doubleVecToStr(extractVector(__vNPatientInfDaysActYear10, index)); }
    inline vector<string> returns_vNPatientInfDaysActYear11(int index) { return doubleVecToStr(extractVector(__vNPatientInfDaysActYear11, index)); }
    inline vector<string> returns_vNPatientInfDaysActYear12(int index) { return doubleVecToStr(extractVector(__vNPatientInfDaysActYear12, index)); }
    inline vector<string> returns_vNPatientInfDaysActYear13(int index) { return doubleVecToStr(extractVector(__vNPatientInfDaysActYear13, index)); }
    inline vector<string> returns_vNPatientInfDaysActYear14(int index) { return doubleVecToStr(extractVector(__vNPatientInfDaysActYear14, index)); }
    inline vector<string> returns_vNPatientInfDaysActYear15(int index) { return doubleVecToStr(extractVector(__vNPatientInfDaysActYear15, index)); }
    inline vector<string> returns_vSPatientWaiting(int index){return doubleVecToStr(extractVector(__vSPatientWaiting, index));}
    inline vector<string> returns_vSPatientWaitingIn(int index){return doubleVecToStr(extractVector(__vSPatientWaitingIn, index));}
    inline vector<string> returns_vSPatientWaitingOut(int index){return doubleVecToStr(extractVector(__vSPatientWaitingOut, index));}
    inline vector<string> returns_vSPatientInTreatment(int index){return doubleVecToStr(extractVector(__vSPatientInTreatment, index));}
    inline vector<string> returns_vSSampleWaitingForTest(int index){return doubleVecToStr(extractVector(__vSSampleWaitingForTest, index));}
    inline vector<string> returns_vSSampleWaitingForTestIn(int index){return doubleVecToStr(extractVector(__vSSampleWaitingForTestIn, index));}
    inline vector<string> returns_vSSampleWaitingForTestOut(int index){return doubleVecToStr(extractVector(__vSSampleWaitingForTestOut, index));}


    
    // some functions for reporting outputs of interest in reporting
    //vector<double> return_vnDiagnosis();//total number of diagnosis and trt for dstb and drtb
    //vector<double> return_vpDR_new();//proportion of DR among new patients
    //vector<double> return_vpDR_retreat();//proportion of DR among retreated patients
    //vector<double> return_vpIncDS_fastprog();//proportion of incidence due to fast prog
    //vector<double> return_vpIncDS_relapse();
    //vector<double> return_vpTransDS_HH();//proportion of transmission that happens in HH
    //vector<double> return_vpTransDS_reinf();//proportion of transmission to previously infected individuals (with DS or DR)
    //vector<double> return_vpTransDS_reinf_HH();
    //vector<double> return_vpIncDR_fastprog();
    //vector<double> return_vpIncDR_relapse();
    //vector<double> return_vpIncDR_resist();
    //vector<double> return_vpTransDR_HH();
    //vector<double> return_vpTransDR_reinf();
    //vector<double> return_vpTransDR_reinf_HH();
    //vector<double> return_vpFailedTrtDR();//proportion of DR treamtments that fail
    
    
    // zero out counts and vector of counts
    void resetOutputs();

    //outputs
    vector<double> returnMeanOutputs(int index);

    // create file with vector outputs for all variables
    void runSaveAnnual(string diseaseName);

    // create file with vector outputs for all variables with comma delimiters
    void runSaveAnnualString(string diseaseName, string statSubScenarioDir, string jobNo, string regionDistrictArray[], int districtToRegionMapping[], string facilityArray[], bool addCommas, bool addTotals, bool addDistrictAndRegion, bool addFacilities, int year, bool includeColHeader, bool subYear);

    vector<vector<double>> returnAnnualOutputs(int index);

    vector<vector<string>> returnAnnualOutputStrings(int index, int tier, int reg, int dist, string name, bool addCommas, int year);

       
    template <typename I>
    void writeVector(const vector< I > &vec, const string &filename, const bool overwrite){
        
        unique_ptr<ofstream> ofile = _setupOutputVectorFile ( filename, overwrite );
        
        ostream_iterator<I> output_iterator(*ofile, " ");
        copy(vec.begin(), vec.end(), output_iterator);
        *ofile<<"\n";
    }
    
    
    template <typename I>
    void writeVector(const vector< vector <I> > &vec, const string &filename, const bool overwrite){
        unique_ptr<ofstream> ofile = _setupOutputVectorFile ( filename, overwrite );
        for (auto outer = vec.begin(); outer != vec.end(); outer ++) {
            if (outer != vec.begin())  //Eliminates the newline at the end of file
                *ofile << endl;
            //   *ofile << endl;
            ostream_iterator<I> output_iterator(*ofile , " ");
            copy(outer->begin(), outer->end(), output_iterator);
        }
        *ofile<<"\n";
    }
    

    template <typename I>
    void writeVector(const vector< vector <I> >& vec, const string& filename, const bool overwrite, const bool noSpace) {
        unique_ptr<ofstream> ofile = _setupOutputVectorFile(filename, overwrite);
        for (auto outer = vec.begin(); outer != vec.end(); outer++) {
            if (outer != vec.begin())  //Eliminates the newline at the end of file
                *ofile << endl;
            //   *ofile << endl;
            ostream_iterator<I> output_iterator(*ofile, " ");
            ostream_iterator<I> output_iterator1(*ofile, "");
            if (noSpace) {

                copy(outer->begin(), outer->end(), output_iterator1);
            }
            else {

                copy(outer->begin(), outer->end(), output_iterator);
            }
            
        }
        *ofile << "\n";
    }

    template <typename I>
    void writeVector(const vector< vector <I> > &vec, const string &filename, vector<string> fistline){
        unique_ptr<ofstream> ofile = _setupOutputVectorFile ( filename, true );
        ostream_iterator<string> output_iterator1(*ofile, " ");
        copy(fistline.begin(), fistline.end(), output_iterator1);
        *ofile<<endl;
        
        for (auto outer = vec.begin(); outer != vec.end(); outer ++) {
            if (outer != vec.begin())  //Eliminates the newline at the end of file
                *ofile << endl;
            ostream_iterator<I> output_iterator2(*ofile , " ");
            copy(outer->begin(), outer->end(), output_iterator2);
        }
    }


    template <typename I>
    void writeVector(const vector< vector< vector <I>>> &vec, const string &filename, vector<string> fistline){
        unique_ptr<ofstream> ofile = _setupOutputVectorFile ( filename, true );
        ostream_iterator<string> output_iterator1(*ofile, " ");
        copy(fistline.begin(), fistline.end(), output_iterator1);
        *ofile<<"\n";
        
        int N1=vec.size();
        for (int i=0;i<N1;i++) {
            vector<vector<I>> outer1=vec[i];
            if (i>0)  //Eliminates the newline at the end of file
                *ofile << endl;
            
            int N2=outer1.size();
            for (int j=0;j<N2;j++) {
                vector<I> outer2=outer1[j];
                if (j>0)  //Eliminates the newline at the end of file
                    *ofile << endl;
                
                ostream_iterator<I> output_iterator2(*ofile, " ");
                copy(outer2.begin(), outer2.end(), output_iterator2);
            }
        }
    }
    
    template <typename I>
    void writeVector(const vector< vector< vector <I>>> &vec, const string &filename, const bool overwrite){
        unique_ptr<ofstream> ofile = _setupOutputVectorFile ( filename, overwrite );
        int N1=vec.size();
        for (int i=0;i<N1;i++) {
            vector<vector<I>> outer1=vec[i];
            if (i>0)  //Eliminates the newline at the end of file
                *ofile << endl;
            
            int N2=outer1.size();
            for (int j=0;j<N2;j++) {
                vector<I> outer2=outer1[j];
                if (j>0)  //Eliminates the newline at the end of file
                    *ofile << endl;
                
                ostream_iterator<I> output_iterator(*ofile , " ");
                copy(outer2.begin(), outer2.end(), output_iterator);
            }
        }
    }
    
  
    template <typename I>
    vector<double> vectorAveCols(const vector<vector<I>> &vec){
        int rows=vec.size();
        if (rows==0) return {-1};
        int cols=vec[1].size();
        //sum all cols
        vector<double> vSum;
        for (auto outer: vec){
            double sum=0;
            for (auto inner:outer){
                sum += inner;
            }
            vSum.push_back(sum);
        }
        //averageing:
        //transform(vSum.begin(), vSum.end(), vSum.begin(), bind2nd(divides<double>(), cols) );
        vector<double>  res;
        for (auto num:vSum){
            res.push_back(num/cols);
        }
        return res;
    }

      template <typename I>
    vector<vector<I>> vectorTranspose(vector<vector<I>> vec){
        vector<vector<I>> res;
        int rows=vec.size();
        int cols=vec[0].size();
        for (int i=0;i<cols;i++){
        vector<I> temp;
            for(int j=0;j<rows;j++){
                temp.push_back(vec[j][i]);
            }
            res.push_back(temp);
        }
        return res;
    }
    
    vector<double> extractVector(const vector<std::array<int, 1000>>& vec, int index) {

        double result;
        vector<double> tempDouble;

        for (unsigned int i(0); i < vec.size(); ++i){
            
            result = vec[i][index];

            tempDouble.push_back(result);
        }

        return tempDouble;
    }
    
    vector<double> extractVector(const vector<std::array<float, 1000>>& vec, int index) {

        double result;
        vector<double> tempDouble;

        for (unsigned int i(0); i < vec.size(); ++i){
            
            result = vec[i][index];

            tempDouble.push_back(result);
        }

        return tempDouble;
    }
    

    vector<string> extractStringVector(const vector<std::array<string, 1000>>& vec, int index) {

        string result;
        vector<string> tempString;

        for (unsigned int i(0); i < vec.size(); ++i){
            
            result = vec[i][index];
            
            //std::cout << "output string vec =" << vec[i][index] << ", i = " << i << ", result =" << result << "\n";
            
            tempString.push_back(result);
        }

        //for (unsigned int i(0); i < vec.size(); ++i) {

            //std::cout << "output tempString =" << tempString[i] << "\n";
        //}

        return tempString;
    }




    vector<double> doubleVecAdd(const vector<double>& vec1, const vector<double>& vec2) {

        double result;
        vector<double> tempDouble;

        for (unsigned int i(0); i < vec1.size(); ++i){
            
            result = vec1[i] + vec2[i];

            tempDouble.push_back(result);
        }

        return tempDouble;
    }
    
    vector<double> doubleVecAdd(const vector<double>& vec1, const vector<double>& vec2, const vector<double>& vec3, const vector<double>& vec4) {

        double result;
        vector<double> tempDouble;

        for (unsigned int i(0); i < vec1.size(); ++i){
            
            result = vec1[i] + vec2[i] + vec3[i] + vec4[i];

            tempDouble.push_back(result);
        }

        return tempDouble;
    }

    vector<string> doubleVecAddToStr(const vector<double>& vec1, const vector<double>& vec2) {

        double result;
        vector<string> tempStr;

        for (unsigned int i(0); i < vec1.size(); ++i){
            
            ostringstream doubleStr;
            result = vec1[i] + vec2[i];
            doubleStr << result;    
            tempStr.push_back(doubleStr.str());
        }

        return tempStr;
    }
    
    vector<string> doubleVecAddToStr(const vector<double>& vec1, const vector<double>& vec2, const vector<double>& vec3, const vector<double>& vec4) {

        double result;
        vector<string> tempStr;

        for (unsigned int i(0); i < vec1.size(); ++i){
            
            ostringstream doubleStr;
            result = vec1[i] + vec2[i] + vec3[i] + vec4[i];
            doubleStr << result;    
            tempStr.push_back(doubleStr.str());
        }

        return tempStr;
    }
    


     vector<double> doubleVecDivide(const vector<double>& num, const vector<double>& div) {

        double result = 0;
        vector<double> tempDouble;

        if (num.size() != div.size()) {

            // vectors different sizes - notify and leave
            for (unsigned int i(0); i < num.size(); ++i){
  
                tempDouble.push_back(0);
            }

            return tempDouble;
        }

        for (unsigned int i(0); i < num.size(); ++i){

            if (div[i] == 0) {

                result = 0;
            }
            else {

                result = num[i]/div[i];
            }
            
            tempDouble.push_back(result);
        }

        return tempDouble;
    }
    
    
    vector<string> doubleVecToStr(const vector<double>& vec) {

        vector<string> tempStr;

        for (unsigned int i(0); i < vec.size(); ++i){
            ostringstream doubleStr;
            doubleStr << vec[i];    
            tempStr.push_back(doubleStr.str());
        }

        return tempStr;
    }


    vector<string> doubleVecToStrLarge(const vector<double>& vec) {

        vector<string> tempStr;

        for (unsigned int i(0); i < vec.size(); ++i) {
            ostringstream doubleStr;
            doubleStr << std::to_string(vec[i]);
            //std::cout << "output string vec = " << std::to_string(vec[i]) << ", doubleStr = " << doubleStr.str() << ", doubleStr1 = " << doubleStr1.str() << "\n";
            tempStr.push_back(doubleStr.str());
        }

        return tempStr;
    }


    vector<string> doubleVecDivideToStr(const vector<double>& num, const vector<double>& div) {

        double result = 0;
        vector<string> tempStr;

        if (num.size() != div.size()) {

            // vectors different sizes - notify and leave
            for (unsigned int i(0); i < num.size(); ++i){
  
                tempStr.push_back("0");
            }

            return tempStr;
        }

        for (unsigned int i(0); i < num.size(); ++i){

            if (div[i] == 0) {

                result = 0;
            }
            else {

                result = num[i]/div[i];
            }

            ostringstream doubleStr;

            doubleStr << result; 
            
            tempStr.push_back(doubleStr.str());
        }

        return tempStr;
    }

   



    vector<double> doubleVecPorp(const vector<double>& arg1, const vector<double>& arg2) {

        double result = 0;
        vector<double> tempDouble;

        if (arg1.size() != arg2.size()) {

            // vectors different sizes - notify and leave
            for (unsigned int i(0); i < arg1.size(); ++i){
  
                tempDouble.push_back(0);
            }

            return tempDouble;
        }

        for (unsigned int i(0); i < arg1.size(); ++i){

            if ((arg1[i] + arg2[i]) == 0) {

                result = 0;
            }
            else {

                result = arg1[i]/(arg1[i] + arg2[i]);
            }
            
            tempDouble.push_back(result);
        }

        return tempDouble;
    }


    vector<string> doubleVecPorpToStr(const vector<double>& arg1, const vector<double>& arg2) {

        double result = 0;
        vector<string> tempStr;

        if (arg1.size() != arg2.size()) {

            // vectors different sizes - notify and leave
            for (unsigned int i(0); i < arg1.size(); ++i){
  
                tempStr.push_back("0");
            }

            return tempStr;
        }

        for (unsigned int i(0); i < arg1.size(); ++i){

            if ((arg1[i] + arg2[i]) == 0) {

                result = 0;
            }
            else {

                result = arg1[i]/(arg1[i] + arg2[i]);
            }

            ostringstream doubleStr;

            doubleStr << result; 
            
            tempStr.push_back(doubleStr.str());
        }

        return tempStr;
    }


};


#endif /* defined(_STATS_H) */
