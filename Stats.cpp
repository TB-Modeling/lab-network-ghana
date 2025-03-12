//
//  Stats.cpp
//  mdrtb
//
//  Created by Trinity on 8/4/15.
//  Copyright (c) 2015 Trinity. All rights reserved.
//


#include <vector>
#include <sstream>
#include <iostream>
#include <fstream>
//#include "Population.h"
//#include "Household.h"
//#include "Person.h"
//#include "Functions.h"
#include "Stats.h"

using namespace std; //for std::vector
//using namespace FUNCTIONS;


Stats::Stats() : TNOW(1), LASTID(0){
     
    
    for (int i = 0; i < noElementsDistrictRegionArray; i++) {
    
        __name[i] = "";
        __id[i] = 0;
        __tier[i] = 0;
        __xpertMach[i] = 0;
        __year[i] = 0;
        __scenario[i] = "";
        __district[i] = "";
        __region[i] = "";
        __population[i] = 0;
        __disease[i] = "";
        __incidence[i] = 0;
        __calcIncidence[i] = 0;
        __nPatient[i] = 0;
        __nPatientInf[i] = 0;
        __nPatientLtFUInf[i] = 0;
        __nPatientFromTrans[i] = 0;
        __nPatientNotPresenting[i] = 0;
        __nSamples[i] = 0;
        __nSamplesIn[i] = 0;
        __nSamplesOut[i] = 0;
        __timeSampleResourceWait[i] = 0;
        __timeSampleResourceWaitIn[i] = 0;
        __timeSampleResourceWaitOut[i] = 0;
        __nTests[i] = 0;
        __nTestsIn[i] = 0;
        __nTestsOut[i] = 0;
        __nTransported[i] = 0;
        __timeSampleTransport[i] = 0;
        __timePresentToSampleTestIn[i] = 0;
        __nPresentToSampleTestIn[i] = 0;
        __timePresentToSampleTestOut[i] = 0;
        __nPresentToSampleTestOut[i] = 0;
        __nTestPos[i] = 0;
        __nTestTruePos[i] = 0;
        __nTestTruePosIn[i] = 0;
        __nTestTruePosOut[i] = 0;
        __nTestFalsePos[i] = 0;
        __nTestFalsePosIn[i] = 0;
        __nTestFalsePosOut[i] = 0;
        __nTestNeg[i] = 0;
        __nTestTrueNeg[i] = 0;
        __nTestTrueNegIn[i] = 0;
        __nTestTrueNegOut[i] = 0;
        __nTestFalseNeg[i] = 0;
        __nTestFalseNegIn[i] = 0;
        __nTestFalseNegOut[i] = 0;
        __nTestPosIn[i] = 0;
        __nTestPosOut[i] = 0;
        __nMDCDInfected[i] = 0;
        __nMDCDUninfected[i] = 0;
        __nPTMDCDInfected[i] = 0;
        __nPTMDCDUninfected[i] = 0;
        __nDiagnosis[i] = 0;
        __nDiagnosisIn[i] = 0;
        __nDiagnosisOut[i] = 0;
        __nDiagnosisDC[i] = 0;
        __nDiagnosisDCTruePos[i] = 0;
        __nDiagnosisDCFalsePos[i] = 0;
        __nPreTestDiagnosisTruePos[i] = 0;
        __nPreTestDiagnosisFalseNeg[i] = 0;
        __nPreTestDiagnosisTrueNeg[i] = 0;
        __nPreTestDiagnosisFalsePos[i] = 0;
        __nPreTestDiagnosisTotal[i] = 0;
        __nPreTestDiagnosisReview[i] = 0;
        __nPostTestDiagnosisTruePosIn[i] = 0;
        __nPostTestDiagnosisTruePosOut[i] = 0;
        __nPostTestDiagnosisFalseNeg[i] = 0;
        __nPostTestDiagnosisTrueNeg[i] = 0;
        __nPostTestDiagnosisFalsePosIn[i] = 0;
        __nPostTestDiagnosisFalsePosOut[i] = 0;
        __nPostTestDiagnosisTotal[i] = 0;
        __nPostTestDiagnosisReview[i] = 0;
        __nMDDiagnosisTotal[i] = 0;
        __nMDDiagnosisReview[i] = 0;
        __nPreTestLtFU[i] = 0;
        __nPreTestLtFUInf[i] = 0;
        __nPreTestLtFUDied[i] = 0;
        __nPreTestLtFUCured[i] = 0;
        __nPreTestLtFURepresent[i] = 0;
        __nPreTreatLtFU[i] = 0;
        __nPreTreatLtFUInf[i] = 0;
        __nPreTreatLtFUDied[i] = 0;
        __nPreTreatLtFUCured[i] = 0;
        __nPreTreatLtFURepresent[i] = 0;
        __nPreTreatLtFUFN[i] = 0;
        __nPreTreatLtFUFNDied[i] = 0;
        __nPreTreatLtFUFNCured[i] = 0;
        __nPreTreatLtFUFNRepresent[i] = 0;
        __nPreTreatLtFUPos[i] = 0;
        __nPreTreatLtFUNeg[i] = 0;
        __nTreatLtFU[i] = 0;
        __nTreatLtFUInf[i] = 0;
        __nTreatLtFUDied[i] = 0;
        __nTreatLtFUCured[i] = 0;
        __nTreatLtFURepresent[i] = 0;
        __nPatientsTreated[i] = 0;
        __nPatientsTreatedInf[i] = 0;
        __nPatientsTreatedUninf[i] = 0;
        __nPatientsPreTreatedLtoFUninf[i] = 0;
        __nPatientsTreatedCured[i] = 0;
        __nPatientsTreatedDied[i] = 0;
        __nPatientsTreatedRelapse[i] = 0;
        __nTreatedRelapse[i] = 0;
        __nTreatedRelapseCured[i] = 0;
        __nTreatedRelapseDied[i] = 0;
        __timePresentToSampleDiagIn[i] = 0;
        __nPresentToSampleDiagIn[i] = 0;
        __timePresentToSampleDiagOut[i] = 0;
        __nPresentToSampleDiagOut[i] = 0;
        __timePresentToSampleTreat[i] = 0;
        __nPresentToSampleTreat[i] = 0;
        __timePresentToSampleTreatIn[i] = 0;
        __nPresentToSampleTreatIn[i] = 0;
        __timePresentToSampleTreatOut[i] = 0;
        __nPresentToSampleTreatOut[i] = 0;
        __timePresentToSampleDeath[i] = 0;
        __nPresentToSampleDeath[i] = 0;
        __timePresentToSampleDeathIn[i] = 0;
        __nPresentToSampleDeathIn[i] = 0;
        __timePresentToSampleDeathOut[i] = 0;
        __nPresentToSampleDeathOut[i] = 0;
        __timePresentToSampleDeathPreTestLTFU[i] = 0;
        __nPresentToSampleDeathPreTestLTFU[i] = 0;
        __timePresentToSampleDeathPreTreatLTFU[i] = 0;
        __nPresentToSampleDeathPreTreatLTFU[i] = 0;
        __timePresentToSampleDeathTreatLTFU[i] = 0;
        __nPresentToSampleDeathTreatLTFU[i] = 0;
        __timePresentToSampleDeathInTreat[i] = 0;
        __nPresentToSampleDeathInTreat[i] = 0;
        __timePresentToSampleDeathInRelapse[i] = 0;
        __nPresentToSampleDeathInRelapse[i] = 0;
        __timePresentToExit[i] = 0;
        __nPresentToExit[i] = 0;
        __nXpert[i] = 0;
        __nXpertIn[i] = 0;
        __nXpertOut[i] = 0;
        __nXpertPos[i] = 0;
        __nXpertTruePos[i] = 0;
        __nXpertFalsePos[i] = 0;
        __nXpertNeg[i] = 0;
        __nXpertTrueNeg[i] = 0;
        __nXpertFalseNeg[i] = 0;
        __nSmear[i] = 0;
        __nSmearIn[i] = 0;
        __nSmearOut[i] = 0;
        __nSmearPos[i] = 0;
        __nSmearTruePos[i] = 0;
        __nSmearFalsePos[i] = 0;
        __nSmearNeg[i] = 0;
        __nSmearTrueNeg[i] = 0;
        __nSmearFalseNeg[i] = 0;
        __nDeathPreTest[i] = 0;
        __nDeathPreTreat[i] = 0;
        __nDeathTreat[i] = 0;
        __nDeathReTreat[i] = 0;
        __nDeathUntreat[i] = 0;
        __nPatientInfDaysYear0[i] = 0;
        __nPatientInfDaysYear1[i] = 0;
        __nPatientInfDaysYear2[i] = 0;
        __nPatientInfDaysYear3[i] = 0;
        __nPatientInfDaysYear4[i] = 0;
        __nPatientInfDaysYear5[i] = 0;
        __nPatientInfDaysYear6[i] = 0;
        __nPatientInfDaysYear7[i] = 0;
        __nPatientInfDaysYear8[i] = 0;
        __nPatientInfDaysYear9[i] = 0;
        __nPatientInfDaysYear10[i] = 0;
        __nPatientInfDaysYear11[i] = 0;
        __nPatientInfDaysYear12[i] = 0;
        __nPatientInfDaysYear13[i] = 0;
        __nPatientInfDaysYear14[i] = 0;
        __nPatientInfDaysYear15[i] = 0;
        __nPatientInfDaysActYear0[i] = 0;
        __nPatientInfDaysActYear1[i] = 0;
        __nPatientInfDaysActYear2[i] = 0;
        __nPatientInfDaysActYear3[i] = 0;
        __nPatientInfDaysActYear4[i] = 0;
        __nPatientInfDaysActYear5[i] = 0;
        __nPatientInfDaysActYear6[i] = 0;
        __nPatientInfDaysActYear7[i] = 0;
        __nPatientInfDaysActYear8[i] = 0;
        __nPatientInfDaysActYear9[i] = 0;
        __nPatientInfDaysActYear10[i] = 0;
        __nPatientInfDaysActYear11[i] = 0;
        __nPatientInfDaysActYear12[i] = 0;
        __nPatientInfDaysActYear13[i] = 0;
        __nPatientInfDaysActYear14[i] = 0;
        __nPatientInfDaysActYear15[i] = 0;
        __sPatientWaiting[i] = 0;
        __sPatientWaitingIn[i] = 0;
        __sPatientWaitingOut[i] = 0;
        __sPatientInTreatment[i] = 0;
        __sSampleWaitingForTest[i] = 0;
        __sSampleWaitingForTestIn[i] = 0;
        __sSampleWaitingForTestOut[i] = 0;
    }

}

Stats::Stats(const Stats &src)
: TNOW(src.TNOW), LASTID(src.LASTID),

    __name(src.__name),
    __id(src.__id),
    __tier(src.__tier),
    __xpertMach(src.__xpertMach),
    __year(src.__year),
    __scenario(src.__scenario),
    __district(src.__district),
    __region(src.__region),
    __population(src.__population),
    __disease(src.__disease),
    __incidence(src.__incidence),
    __calcIncidence(src.__calcIncidence),
    __nPatient(src.__nPatient),
    __nPatientInf(src.__nPatientInf),
    __nPatientLtFUInf(src.__nPatientLtFUInf),
    __nPatientFromTrans(src.__nPatientFromTrans),
    __nPatientNotPresenting(src.__nPatientNotPresenting),
    __nSamples(src.__nSamples),
    __nSamplesIn(src.__nSamplesIn),
    __nSamplesOut(src.__nSamplesOut),
    __timeSampleResourceWait(src.__timeSampleResourceWait),
    __timeSampleResourceWaitIn(src.__timeSampleResourceWaitIn),
    __timeSampleResourceWaitOut(src.__timeSampleResourceWaitOut),
    __nTests(src.__nTests),
    __nTestsIn(src.__nTestsIn),
    __nTestsOut(src.__nTestsOut),
    __nTransported(src.__nTransported),
    __timeSampleTransport(src.__timeSampleTransport),
    __timePresentToSampleTestIn(src.__timePresentToSampleTestIn),
    __nPresentToSampleTestIn(src.__nPresentToSampleTestIn),
    __timePresentToSampleTestOut(src.__timePresentToSampleTestOut),
    __nPresentToSampleTestOut(src.__nPresentToSampleTestOut),
    __nTestPos(src.__nTestPos),
    __nTestTruePos(src.__nTestTruePos),
    __nTestTruePosIn(src.__nTestTruePosIn),
    __nTestTruePosOut(src.__nTestTruePosOut),
    __nTestFalsePos(src.__nTestFalsePos),
    __nTestFalsePosIn(src.__nTestFalsePosIn),
    __nTestFalsePosOut(src.__nTestFalsePosOut),
    __nTestNeg(src.__nTestNeg),
    __nTestTrueNeg(src.__nTestTrueNeg),
    __nTestTrueNegIn(src.__nTestTrueNegIn),
    __nTestTrueNegOut(src.__nTestTrueNegOut),
    __nTestFalseNeg(src.__nTestFalseNeg),
    __nTestFalseNegIn(src.__nTestFalseNegIn),
    __nTestFalseNegOut(src.__nTestFalseNegOut),
    __nTestPosIn(src.__nTestPosIn),
    __nTestPosOut(src.__nTestPosOut),
    __nMDCDInfected(src.__nMDCDInfected),
    __nMDCDUninfected(src.__nMDCDUninfected),
    __nPTMDCDInfected(src.__nPTMDCDInfected),
    __nPTMDCDUninfected(src.__nPTMDCDUninfected),
    __nDiagnosis(src.__nDiagnosis),
    __nDiagnosisIn(src.__nDiagnosisIn),
    __nDiagnosisOut(src.__nDiagnosisOut),
    __nDiagnosisDC(src.__nDiagnosisDC),
    __nDiagnosisDCTruePos(src.__nDiagnosisDCTruePos),
    __nDiagnosisDCFalsePos(src.__nDiagnosisDCFalsePos),
    __nPreTestDiagnosisTruePos(src.__nPreTestDiagnosisTruePos),
    __nPreTestDiagnosisFalseNeg(src.__nPreTestDiagnosisFalseNeg),
    __nPreTestDiagnosisTrueNeg(src.__nPreTestDiagnosisTrueNeg),
    __nPreTestDiagnosisFalsePos(src.__nPreTestDiagnosisFalsePos),
    __nPreTestDiagnosisTotal(src.__nPreTestDiagnosisTotal),
    __nPreTestDiagnosisReview(src.__nPreTestDiagnosisReview),
    __nPostTestDiagnosisTruePosIn(src.__nPostTestDiagnosisTruePosIn),
    __nPostTestDiagnosisTruePosOut(src.__nPostTestDiagnosisTruePosOut),
    __nPostTestDiagnosisFalseNeg(src.__nPostTestDiagnosisFalseNeg),
    __nPostTestDiagnosisTrueNeg(src.__nPostTestDiagnosisTrueNeg),
    __nPostTestDiagnosisFalsePosIn(src.__nPostTestDiagnosisFalsePosIn),
    __nPostTestDiagnosisFalsePosOut(src.__nPostTestDiagnosisFalsePosOut),
    __nPostTestDiagnosisTotal(src.__nPostTestDiagnosisTotal),
    __nPostTestDiagnosisReview(src.__nPostTestDiagnosisReview),
    __nMDDiagnosisTotal(src.__nMDDiagnosisTotal), 
    __nMDDiagnosisReview(src.__nMDDiagnosisReview),
    __nPreTestLtFU(src.__nPreTestLtFU),
    __nPreTestLtFUInf(src.__nPreTestLtFUInf),
    __nPreTestLtFUDied(src.__nPreTestLtFUDied),
    __nPreTestLtFUCured(src.__nPreTestLtFUCured),
    __nPreTestLtFURepresent(src.__nPreTestLtFURepresent),
    __nPreTreatLtFU(src.__nPreTreatLtFU),
    __nPreTreatLtFUInf(src.__nPreTreatLtFUInf),
    __nPreTreatLtFUDied(src.__nPreTreatLtFUDied),
    __nPreTreatLtFUCured(src.__nPreTreatLtFUCured),
    __nPreTreatLtFURepresent(src.__nPreTreatLtFURepresent),
    __nPreTreatLtFUFN(src.__nPreTreatLtFUFN),
    __nPreTreatLtFUFNDied(src.__nPreTreatLtFUFNDied),
    __nPreTreatLtFUFNCured(src.__nPreTreatLtFUFNCured),
    __nPreTreatLtFUFNRepresent(src.__nPreTreatLtFUFNRepresent),
    __nPreTreatLtFUPos(src.__nPreTreatLtFUPos),
    __nPreTreatLtFUNeg(src.__nPreTreatLtFUNeg),
    __nTreatLtFU(src.__nTreatLtFU),
    __nTreatLtFUInf(src.__nTreatLtFUInf),
    __nTreatLtFUDied(src.__nTreatLtFUDied),
    __nTreatLtFUCured(src.__nTreatLtFUCured),
    __nTreatLtFURepresent(src.__nTreatLtFURepresent),
    __nPatientsTreated(src.__nPatientsTreated),
    __nPatientsTreatedInf(src.__nPatientsTreatedInf),
    __nPatientsTreatedUninf(src.__nPatientsTreatedUninf),
    __nPatientsPreTreatedLtoFUninf(src.__nPatientsPreTreatedLtoFUninf),
    __nPatientsTreatedCured(src.__nPatientsTreatedCured),
    __nPatientsTreatedDied(src.__nPatientsTreatedDied),
    __nPatientsTreatedRelapse(src.__nPatientsTreatedRelapse),
    __nTreatedRelapse(src.__nTreatedRelapse),
    __nTreatedRelapseCured(src.__nTreatedRelapseCured),
    __nTreatedRelapseDied(src.__nTreatedRelapseDied),
    __timePresentToSampleDiagIn(src.__timePresentToSampleDiagIn),
    __nPresentToSampleDiagIn(src.__nPresentToSampleDiagIn),
    __timePresentToSampleDiagOut(src.__timePresentToSampleDiagOut),
    __nPresentToSampleDiagOut(src.__nPresentToSampleDiagOut),
    __timePresentToSampleTreat(src.__timePresentToSampleTreat),
    __nPresentToSampleTreat(src.__nPresentToSampleTreat),
    __timePresentToSampleTreatIn(src.__timePresentToSampleTreatIn),
    __nPresentToSampleTreatIn(src.__nPresentToSampleTreatIn),
    __timePresentToSampleTreatOut(src.__timePresentToSampleTreatOut),
    __nPresentToSampleTreatOut(src.__nPresentToSampleTreatOut),
    __timePresentToSampleDeath(src.__timePresentToSampleDeath),
    __nPresentToSampleDeath(src.__nPresentToSampleDeath),
    __timePresentToSampleDeathIn(src.__timePresentToSampleDeathIn),
    __nPresentToSampleDeathIn(src.__nPresentToSampleDeathIn),
    __timePresentToSampleDeathOut(src.__timePresentToSampleDeathOut),
    __nPresentToSampleDeathOut(src.__nPresentToSampleDeathOut),
    __timePresentToSampleDeathPreTestLTFU(src.__timePresentToSampleDeathPreTestLTFU),
    __nPresentToSampleDeathPreTestLTFU(src.__nPresentToSampleDeathPreTestLTFU),
    __timePresentToSampleDeathPreTreatLTFU(src.__timePresentToSampleDeathPreTreatLTFU),
    __nPresentToSampleDeathPreTreatLTFU(src.__nPresentToSampleDeathPreTreatLTFU),
    __timePresentToSampleDeathTreatLTFU(src.__timePresentToSampleDeathTreatLTFU),
    __nPresentToSampleDeathTreatLTFU(src.__nPresentToSampleDeathTreatLTFU),
    __timePresentToSampleDeathInTreat(src.__timePresentToSampleDeathInTreat),
    __nPresentToSampleDeathInTreat(src.__nPresentToSampleDeathInTreat),
    __timePresentToSampleDeathInRelapse(src.__timePresentToSampleDeathInRelapse),
    __nPresentToSampleDeathInRelapse(src.__nPresentToSampleDeathInRelapse),
    __timePresentToExit(src.__timePresentToExit),
    __nPresentToExit(src.__nPresentToExit),
    __nXpert(src.__nXpert),
    __nXpertIn(src.__nXpertIn),
    __nXpertOut(src.__nXpertOut),
    __nXpertPos(src.__nXpertPos),
    __nXpertTruePos(src.__nXpertTruePos),
    __nXpertFalsePos(src.__nXpertFalsePos),
    __nXpertNeg(src.__nXpertNeg),
    __nXpertTrueNeg(src.__nXpertTrueNeg),
    __nXpertFalseNeg (src.__nXpertFalseNeg),
    __nSmear(src.__nSmear),
    __nSmearIn(src.__nSmearIn),
    __nSmearOut(src.__nSmearOut),
    __nSmearPos(src.__nSmearPos),
    __nSmearTruePos(src.__nSmearTruePos),
    __nSmearFalsePos(src.__nSmearFalsePos),
    __nSmearNeg(src.__nSmearNeg),
    __nSmearTrueNeg(src.__nSmearTrueNeg),
    __nSmearFalseNeg(src.__nSmearFalseNeg),
    __nDeathPreTest(src.__nDeathPreTest),
    __nDeathPreTreat(src.__nDeathPreTreat),
    __nDeathTreat(src.__nDeathTreat),
    __nDeathReTreat(src.__nDeathReTreat),
    __nDeathUntreat(src.__nDeathUntreat),
    __nPatientInfDaysYear0(src.__nPatientInfDaysYear0),
    __nPatientInfDaysYear1(src.__nPatientInfDaysYear1),
    __nPatientInfDaysYear2(src.__nPatientInfDaysYear2),
    __nPatientInfDaysYear3(src.__nPatientInfDaysYear3),
    __nPatientInfDaysYear4(src.__nPatientInfDaysYear4),
    __nPatientInfDaysYear5(src.__nPatientInfDaysYear5),
    __nPatientInfDaysYear6(src.__nPatientInfDaysYear6),
    __nPatientInfDaysYear7(src.__nPatientInfDaysYear7),
    __nPatientInfDaysYear8(src.__nPatientInfDaysYear8),
    __nPatientInfDaysYear9(src.__nPatientInfDaysYear9),
    __nPatientInfDaysYear10(src.__nPatientInfDaysYear10),
    __nPatientInfDaysYear11(src.__nPatientInfDaysYear11),
    __nPatientInfDaysYear12(src.__nPatientInfDaysYear12),
    __nPatientInfDaysYear13(src.__nPatientInfDaysYear13),
    __nPatientInfDaysYear14(src.__nPatientInfDaysYear14),
    __nPatientInfDaysYear15(src.__nPatientInfDaysYear15),
    __nPatientInfDaysActYear0(src.__nPatientInfDaysActYear0),
    __nPatientInfDaysActYear1(src.__nPatientInfDaysActYear1),
    __nPatientInfDaysActYear2(src.__nPatientInfDaysActYear2),
    __nPatientInfDaysActYear3(src.__nPatientInfDaysActYear3),
    __nPatientInfDaysActYear4(src.__nPatientInfDaysActYear4),
    __nPatientInfDaysActYear5(src.__nPatientInfDaysActYear5),
    __nPatientInfDaysActYear6(src.__nPatientInfDaysActYear6),
    __nPatientInfDaysActYear7(src.__nPatientInfDaysActYear7),
    __nPatientInfDaysActYear8(src.__nPatientInfDaysActYear8),
    __nPatientInfDaysActYear9(src.__nPatientInfDaysActYear9),
    __nPatientInfDaysActYear10(src.__nPatientInfDaysActYear10),
    __nPatientInfDaysActYear11(src.__nPatientInfDaysActYear11),
    __nPatientInfDaysActYear12(src.__nPatientInfDaysActYear12),
    __nPatientInfDaysActYear13(src.__nPatientInfDaysActYear13),
    __nPatientInfDaysActYear14(src.__nPatientInfDaysActYear14),
    __nPatientInfDaysActYear15(src.__nPatientInfDaysActYear15),
    __sPatientWaiting(src.__sPatientWaiting),
    __sPatientWaitingIn(src.__sPatientWaitingIn),
    __sPatientWaitingOut(src.__sPatientWaitingOut),
    __sPatientInTreatment(src.__sPatientInTreatment),
    __sSampleWaitingForTest(src.__sSampleWaitingForTest),
    __sSampleWaitingForTestIn(src.__sSampleWaitingForTestIn),
    __sSampleWaitingForTestOut(src.__sSampleWaitingForTestOut),


    //__vvDiseaseStates(src.__vvDiseaseStates)
    __vName(src.__vName),
    __vId(src.__vId),
    __vTier(src.__vTier),
    __vXpertMach(src.__vXpertMach),
    __vYear(src.__vYear),
    __vScenario(src.__vScenario),
    __vDistrict(src.__vDistrict),
    __vRegion(src.__vRegion),
    __vPopulation(src.__vPopulation),
    __vDisease(src.__vDisease),
    __vIncidence(src.__vIncidence),
    __vCalcIncidence(src.__vCalcIncidence),
    __vNPatient(src.__vNPatient),
    __vNPatientInf(src.__vNPatientInf),
    __vNPatientLtFUInf(src.__vNPatientLtFUInf),
    __vNPatientFromTrans(src.__vNPatientFromTrans),
    __vNPatientNotPresenting(src.__vNPatientNotPresenting),
    __vNSamples(src.__vNSamples),
    __vNSamplesIn(src.__vNSamplesIn),
    __vNSamplesOut(src.__vNSamplesOut),
    __vTimeSampleResourceWait(src.__vTimeSampleResourceWait),
    __vTimeSampleResourceWaitIn(src.__vTimeSampleResourceWaitIn),
    __vTimeSampleResourceWaitOut(src.__vTimeSampleResourceWaitOut),
    __vNTests(src.__vNTests),
    __vNTestsIn(src.__vNTestsIn),
    __vNTestsOut(src.__vNTestsOut),
    __vNTransported(src.__vNTransported),
    __vTimeSampleTransport(src.__vTimeSampleTransport),
    __vTimePresentToSampleTestIn(src.__vTimePresentToSampleTestIn),
    __vNPresentToSampleTestIn(src.__vNPresentToSampleTestIn),
    __vTimePresentToSampleTestOut(src.__vTimePresentToSampleTestOut),
    __vNPresentToSampleTestOut(src.__vNPresentToSampleTestOut),
    __vNTestPos(src.__vNTestPos),
    __vNTestTruePos(src.__vNTestTruePos),
    __vNTestTruePosIn(src.__vNTestTruePosIn),
    __vNTestTruePosOut(src.__vNTestTruePosOut),
    __vNTestFalsePos(src.__vNTestFalsePos),
    __vNTestFalsePosIn(src.__vNTestFalsePosIn),
    __vNTestFalsePosOut(src.__vNTestFalsePosOut),
    __vNTestNeg(src.__vNTestNeg),
    __vNTestTrueNeg(src.__vNTestTrueNeg),
    __vNTestTrueNegIn(src.__vNTestTrueNegIn),
    __vNTestTrueNegOut(src.__vNTestTrueNegOut),
    __vNTestFalseNeg(src.__vNTestFalseNeg),
    __vNTestFalseNegIn(src.__vNTestFalseNegIn),
    __vNTestFalseNegOut(src.__vNTestFalseNegOut),
    __vNTestPosIn(src.__vNTestPosIn),
    __vNTestPosOut(src.__vNTestPosOut),
    __vNMDCDInfected(src.__vNMDCDInfected),
    __vNMDCDUninfected(src.__vNMDCDUninfected),
    __vNPTMDCDInfected(src.__vNPTMDCDInfected),
    __vNPTMDCDUninfected(src.__vNPTMDCDUninfected),
    __vNDiagnosis(src.__vNDiagnosis),
    __vNDiagnosisIn(src.__vNDiagnosisIn),
    __vNDiagnosisOut(src.__vNDiagnosisOut),
    __vNDiagnosisDC(src.__vNDiagnosisDC),
    __vNDiagnosisDCTruePos(src.__vNDiagnosisDCTruePos),
    __vNDiagnosisDCFalsePos(src.__vNDiagnosisDCFalsePos),
    __vNPreTestDiagnosisTruePos(src.__vNPreTestDiagnosisTruePos),
    __vNPreTestDiagnosisFalseNeg(src.__vNPreTestDiagnosisFalseNeg),
    __vNPreTestDiagnosisTrueNeg(src.__vNPreTestDiagnosisTrueNeg),
    __vNPreTestDiagnosisFalsePos(src.__vNPreTestDiagnosisFalsePos),
    __vNPreTestDiagnosisTotal(src.__vNPreTestDiagnosisTotal),
    __vNPreTestDiagnosisReview(src.__vNPreTestDiagnosisReview),
    __vNPostTestDiagnosisTruePosIn(src.__vNPostTestDiagnosisTruePosIn),
    __vNPostTestDiagnosisTruePosOut(src.__vNPostTestDiagnosisTruePosOut),
    __vNPostTestDiagnosisFalseNeg(src.__vNPostTestDiagnosisFalseNeg),
    __vNPostTestDiagnosisTrueNeg(src.__vNPostTestDiagnosisTrueNeg),
    __vNPostTestDiagnosisFalsePosIn(src.__vNPostTestDiagnosisFalsePosIn),
    __vNPostTestDiagnosisFalsePosOut(src.__vNPostTestDiagnosisFalsePosOut),
    __vNPostTestDiagnosisTotal(src.__vNPostTestDiagnosisTotal),
    __vNPostTestDiagnosisReview(src.__vNPostTestDiagnosisReview),
    __vNMDDiagnosisTotal(src.__vNMDDiagnosisTotal), 
    __vNMDDiagnosisReview(src.__vNMDDiagnosisReview),
    __vNPreTestLtFU(src.__vNPreTestLtFU),
    __vNPreTestLtFUInf(src.__vNPreTestLtFUInf),
    __vNPreTestLtFUDied(src.__vNPreTestLtFUDied),
    __vNPreTestLtFUCured(src.__vNPreTestLtFUCured),
    __vNPreTestLtFURepresent(src.__vNPreTestLtFURepresent),
    __vNPreTreatLtFU(src.__vNPreTreatLtFU),
    __vNPreTreatLtFUInf(src.__vNPreTreatLtFUInf),
    __vNPreTreatLtFUDied(src.__vNPreTreatLtFUDied),
    __vNPreTreatLtFUCured(src.__vNPreTreatLtFUCured),
    __vNPreTreatLtFURepresent(src.__vNPreTreatLtFURepresent),
    __vNPreTreatLtFUFN(src.__vNPreTreatLtFUFN),
    __vNPreTreatLtFUFNDied(src.__vNPreTreatLtFUFNDied),
    __vNPreTreatLtFUFNCured(src.__vNPreTreatLtFUFNCured),
    __vNPreTreatLtFUFNRepresent(src.__vNPreTreatLtFUFNRepresent),
    __vNPreTreatLtFUPos(src.__vNPreTreatLtFUPos),
    __vNPreTreatLtFUNeg(src.__vNPreTreatLtFUNeg),
    __vNTreatLtFU(src.__vNTreatLtFU),
    __vNTreatLtFUInf(src.__vNTreatLtFUInf),
    __vNTreatLtFUDied(src.__vNTreatLtFUDied),
    __vNTreatLtFUCured(src.__vNTreatLtFUCured),
    __vNTreatLtFURepresent(src.__vNTreatLtFURepresent),
    __vNPatientsTreated(src.__vNPatientsTreated),
    __vNPatientsTreatedInf(src.__vNPatientsTreatedInf),
    __vNPatientsTreatedUninf(src.__vNPatientsTreatedUninf),
    __vNPatientsPreTreatedLtoFUninf(src.__vNPatientsPreTreatedLtoFUninf),
    __vNPatientsTreatedCured(src.__vNPatientsTreatedCured),
    __vNPatientsTreatedDied(src.__vNPatientsTreatedDied),
    __vNPatientsTreatedRelapse(src.__vNPatientsTreatedRelapse),
    __vNTreatedRelapse(src.__vNTreatedRelapse),
    __vNTreatedRelapseCured(src.__vNTreatedRelapseCured),
    __vNTreatedRelapseDied(src.__vNTreatedRelapseDied),
    __vTimePresentToSampleDiagIn(src.__vTimePresentToSampleDiagIn),
    __vNPresentToSampleDiagIn(src.__vNPresentToSampleDiagIn),
    __vTimePresentToSampleDiagOut(src.__vTimePresentToSampleDiagOut),
    __vNPresentToSampleDiagOut(src.__vNPresentToSampleDiagOut),
    __vTimePresentToSampleTreat(src.__vTimePresentToSampleTreat),
    __vNPresentToSampleTreat(src.__vNPresentToSampleTreat),
    __vTimePresentToSampleTreatIn(src.__vTimePresentToSampleTreatIn),
    __vNPresentToSampleTreatIn(src.__vNPresentToSampleTreatIn),
    __vTimePresentToSampleTreatOut(src.__vTimePresentToSampleTreatOut),
    __vNPresentToSampleTreatOut(src.__vNPresentToSampleTreatOut),
    __vTimePresentToSampleDeath(src.__vTimePresentToSampleDeath),
    __vNPresentToSampleDeath(src.__vNPresentToSampleDeath),
    __vTimePresentToSampleDeathIn(src.__vTimePresentToSampleDeathIn),
    __vNPresentToSampleDeathIn(src.__vNPresentToSampleDeathIn),
    __vTimePresentToSampleDeathOut(src.__vTimePresentToSampleDeathOut),
    __vNPresentToSampleDeathOut(src.__vNPresentToSampleDeathOut),
    __vTimePresentToSampleDeathPreTestLTFU(src.__vTimePresentToSampleDeathPreTestLTFU),
    __vNPresentToSampleDeathPreTestLTFU(src.__vNPresentToSampleDeathPreTestLTFU),
    __vTimePresentToSampleDeathPreTreatLTFU(src.__vTimePresentToSampleDeathPreTreatLTFU),
    __vNPresentToSampleDeathPreTreatLTFU(src.__vNPresentToSampleDeathPreTreatLTFU),
    __vTimePresentToSampleDeathTreatLTFU(src.__vTimePresentToSampleDeathTreatLTFU),
    __vNPresentToSampleDeathTreatLTFU(src.__vNPresentToSampleDeathTreatLTFU),
    __vTimePresentToSampleDeathInTreat(src.__vTimePresentToSampleDeathInTreat),
    __vNPresentToSampleDeathInTreat(src.__vNPresentToSampleDeathInTreat),
    __vTimePresentToSampleDeathInRelapse(src.__vTimePresentToSampleDeathInRelapse),
    __vNPresentToSampleDeathInRelapse(src.__vNPresentToSampleDeathInRelapse),
    __vTimePresentToExit(src.__vTimePresentToExit),
    __vNPresentToExit(src.__vNPresentToExit),
    __vNXpert(src.__vNXpert),
    __vNXpertIn(src.__vNXpertIn),
    __vNXpertOut(src.__vNXpertOut),
    __vNXpertPos(src.__vNXpertPos),
    __vNXpertTruePos(src.__vNXpertTruePos),
    __vNXpertFalsePos(src.__vNXpertFalsePos),
    __vNXpertNeg(src.__vNXpertNeg),
    __vNXpertTrueNeg(src.__vNXpertTrueNeg),
    __vNXpertFalseNeg(src.__vNXpertFalseNeg),
    __vNSmear(src.__vNSmear),
    __vNSmearIn(src.__vNSmearIn),
    __vNSmearOut(src.__vNSmearOut),
    __vNSmearPos(src.__vNSmearPos),
    __vNSmearTruePos(src.__vNSmearTruePos),
    __vNSmearFalsePos(src.__vNSmearFalsePos),
    __vNSmearNeg(src.__vNSmearNeg),
    __vNSmearTrueNeg(src.__vNSmearTrueNeg),
    __vNSmearFalseNeg(src.__vNSmearFalseNeg),
    __vNDeathPreTest(src.__vNDeathPreTest),
    __vNDeathPreTreat(src.__vNDeathPreTreat),
    __vNDeathTreat(src.__vNDeathTreat),
    __vNDeathReTreat(src.__vNDeathReTreat),
    __vNDeathUntreat(src.__vNDeathUntreat),
    __vNPatientInfDaysYear0(src.__vNPatientInfDaysYear0),
    __vNPatientInfDaysYear1(src.__vNPatientInfDaysYear1),
    __vNPatientInfDaysYear2(src.__vNPatientInfDaysYear2),
    __vNPatientInfDaysYear3(src.__vNPatientInfDaysYear3),
    __vNPatientInfDaysYear4(src.__vNPatientInfDaysYear4),
    __vNPatientInfDaysYear5(src.__vNPatientInfDaysYear5),
    __vNPatientInfDaysYear6(src.__vNPatientInfDaysYear6),
    __vNPatientInfDaysYear7(src.__vNPatientInfDaysYear7),
    __vNPatientInfDaysYear8(src.__vNPatientInfDaysYear8),
    __vNPatientInfDaysYear9(src.__vNPatientInfDaysYear9),
    __vNPatientInfDaysYear10(src.__vNPatientInfDaysYear10),
    __vNPatientInfDaysYear11(src.__vNPatientInfDaysYear11),
    __vNPatientInfDaysYear12(src.__vNPatientInfDaysYear12),
    __vNPatientInfDaysYear13(src.__vNPatientInfDaysYear13),
    __vNPatientInfDaysYear14(src.__vNPatientInfDaysYear14),
    __vNPatientInfDaysYear15(src.__vNPatientInfDaysYear15),
    __vNPatientInfDaysActYear0(src.__vNPatientInfDaysActYear0),
    __vNPatientInfDaysActYear1(src.__vNPatientInfDaysActYear1),
    __vNPatientInfDaysActYear2(src.__vNPatientInfDaysActYear2),
    __vNPatientInfDaysActYear3(src.__vNPatientInfDaysActYear3),
    __vNPatientInfDaysActYear4(src.__vNPatientInfDaysActYear4),
    __vNPatientInfDaysActYear5(src.__vNPatientInfDaysActYear5),
    __vNPatientInfDaysActYear6(src.__vNPatientInfDaysActYear6),
    __vNPatientInfDaysActYear7(src.__vNPatientInfDaysActYear7),
    __vNPatientInfDaysActYear8(src.__vNPatientInfDaysActYear8),
    __vNPatientInfDaysActYear9(src.__vNPatientInfDaysActYear9),
    __vNPatientInfDaysActYear10(src.__vNPatientInfDaysActYear10),
    __vNPatientInfDaysActYear11(src.__vNPatientInfDaysActYear11),
    __vNPatientInfDaysActYear12(src.__vNPatientInfDaysActYear12),
    __vNPatientInfDaysActYear13(src.__vNPatientInfDaysActYear13),
    __vNPatientInfDaysActYear14(src.__vNPatientInfDaysActYear14),
    __vNPatientInfDaysActYear15(src.__vNPatientInfDaysActYear15),
    __vSPatientWaiting(src.__vSPatientWaiting),
    __vSPatientWaitingIn(src.__vSPatientWaitingIn),
    __vSPatientWaitingOut(src.__vSPatientWaitingOut),
    __vSPatientInTreatment(src.__vSPatientInTreatment),
    __vSSampleWaitingForTest(src.__vSSampleWaitingForTest),
    __vSSampleWaitingForTestIn(src.__vSSampleWaitingForTestIn),
    __vSSampleWaitingForTestOut(src.__vSSampleWaitingForTestOut)

{
}


void Stats::setTime( const int t ) noexcept {
    TNOW = t;
}

int Stats::getTime() noexcept {
    return TNOW;
}

void Stats::incTime() noexcept {
    TNOW++;
}

int Stats::getYear() noexcept {
    return TNOW/12;
}

int Stats::getID() noexcept {
    return LASTID;
}

void Stats::incID() noexcept {
    LASTID++;
}

// get infections days for the correct year
void Stats::getInfDays(){

    bool debug = false;

    if (debug) std::cout << "inside getInfDays skip year = " << skipYears << " years in run = " << yearsInRun << "\n";
    if (debug) std::cout << "inf days = " << __nPatientInfDaysYear4[0] << " , " << __nPatientInfDaysYear5[0] << " , " << __nPatientInfDaysYear6[0] << "\n";
    
    //The infectious days for each year is stored in a vector for that year. Need to do this because when a patient goes inactive, the previous year might
    // need to be updated with infectious days. At end, build a vector from each of the yearly infectious days just before sending to output
    // to be used as a column in stat output
    for (int i = skipYears; i < yearsInRun + 1; i++) {
        switch (i) {
        case 0:
            __vNPatientInfDays.push_back(__nPatientInfDaysYear0);
            if (debug) std::cout << "inf days year 0 = " << __nPatientInfDaysYear0[0] << "\n";
            break;
        case 1:
            __vNPatientInfDays.push_back(__nPatientInfDaysYear1);
            if (debug) std::cout << "inf days year 1 = " << __nPatientInfDaysYear1[0] << "\n";
            break;
        case 2:
            __vNPatientInfDays.push_back(__nPatientInfDaysYear2);
            if (debug) std::cout << "inf days year 2 = " << __nPatientInfDaysYear2[0] << "\n";
            break;
        case 3:
            __vNPatientInfDays.push_back(__nPatientInfDaysYear3);
            if (debug) std::cout << "inf days year 3 = " << __nPatientInfDaysYear3[0] << "\n";
            break;
        case 4:
            __vNPatientInfDays.push_back(__nPatientInfDaysYear4);
            if (debug) std::cout << "inf days year 4 = " << __nPatientInfDaysYear4[0] << "\n";
            break;
        case 5:
            __vNPatientInfDays.push_back(__nPatientInfDaysYear5);
            if (debug) std::cout << "inf days year 5 = " << __nPatientInfDaysYear5[0] << "\n";
            break;
        case 6:
            __vNPatientInfDays.push_back(__nPatientInfDaysYear6);
            if (debug) std::cout << "inf days year 6 = " << __nPatientInfDaysYear6[0] << "\n";
            break;
        case 7:
            __vNPatientInfDays.push_back(__nPatientInfDaysYear7);
            break;
        case 8:
            __vNPatientInfDays.push_back(__nPatientInfDaysYear8);
            break;
        case 9:
            __vNPatientInfDays.push_back(__nPatientInfDaysYear9);
            break;
        case 10:
            __vNPatientInfDays.push_back(__nPatientInfDaysYear10);
            break;
        case 11:
            __vNPatientInfDays.push_back(__nPatientInfDaysYear11);
            break;
        case 12:
            __vNPatientInfDays.push_back(__nPatientInfDaysYear12);
            break;
        case 13:
            __vNPatientInfDays.push_back(__nPatientInfDaysYear12);
            break;
        case 14:
            __vNPatientInfDays.push_back(__nPatientInfDaysYear13);
            break;
        case 15:
            __vNPatientInfDays.push_back(__nPatientInfDaysYear14);
            break;
        default:
            __vNPatientInfDays.push_back(__nPatientInfDaysYear15);
            if (debug) std::cout << "inf days year 0 = " << __nPatientInfDaysYear0[0] << "\n";
            break;
        }
    }
}

// get infections days for the correct year
void Stats::getInfDaysAct() {

    bool debug = false;

    if (debug) std::cout << "inside getInfDays skip year = " << skipYears << " years in run = " << yearsInRun << "\n";
    if (debug) std::cout << "inf days = " << __nPatientInfDaysActYear4[0] << " , " << __nPatientInfDaysActYear5[0] << " , " << __nPatientInfDaysActYear6[0] << "\n";

    //The infectious days for each year is stored in a vector for that year. Need to do this because when a patient goes inactive, the previous year might
    // need to be updated with infectious days. At end, build a vector from each of the yearly infectious days just before sending to output
    // to be used as a column in stat output
    for (int i = skipYears; i < yearsInRun + 1; i++) {
        switch (i) {
        case 0:
            __vNPatientInfDaysAct.push_back(__nPatientInfDaysActYear0);
            if (debug) std::cout << "inf days year 0 = " << __nPatientInfDaysActYear0[0] << "\n";
            break;
        case 1:
            __vNPatientInfDaysAct.push_back(__nPatientInfDaysActYear1);
            if (debug) std::cout << "inf days year 1 = " << __nPatientInfDaysActYear1[0] << "\n";
            break;
        case 2:
            __vNPatientInfDaysAct.push_back(__nPatientInfDaysActYear2);
            if (debug) std::cout << "inf days year 2 = " << __nPatientInfDaysActYear2[0] << "\n";
            break;
        case 3:
            __vNPatientInfDaysAct.push_back(__nPatientInfDaysActYear3);
            if (debug) std::cout << "inf days year 3 = " << __nPatientInfDaysActYear3[0] << "\n";
            break;
        case 4:
            __vNPatientInfDaysAct.push_back(__nPatientInfDaysActYear4);
            if (debug) std::cout << "inf days year 4 = " << __nPatientInfDaysActYear4[0] << "\n";
            break;
        case 5:
            __vNPatientInfDaysAct.push_back(__nPatientInfDaysActYear5);
            if (debug) std::cout << "inf days year 5 = " << __nPatientInfDaysActYear5[0] << "\n";
            break;
        case 6:
            __vNPatientInfDaysAct.push_back(__nPatientInfDaysActYear6);
            if (debug) std::cout << "inf days year 6 = " << __nPatientInfDaysActYear6[0] << "\n";
            break;
        case 7:
            __vNPatientInfDaysAct.push_back(__nPatientInfDaysActYear7);
            break;
        case 8:
            __vNPatientInfDaysAct.push_back(__nPatientInfDaysActYear8);
            break;
        case 9:
            __vNPatientInfDaysAct.push_back(__nPatientInfDaysActYear9);
            break;
        case 10:
            __vNPatientInfDaysAct.push_back(__nPatientInfDaysActYear10);
            break;
        case 11:
            __vNPatientInfDaysAct.push_back(__nPatientInfDaysActYear11);
            break;
        case 12:
            __vNPatientInfDaysAct.push_back(__nPatientInfDaysActYear12);
            break;
        case 13:
            __vNPatientInfDaysAct.push_back(__nPatientInfDaysActYear13);
            break;
        case 14:
            __vNPatientInfDaysAct.push_back(__nPatientInfDaysActYear14);
            break;
        case 15:
            __vNPatientInfDaysAct.push_back(__nPatientInfDaysActYear15);
            break;
        default:
            __vNPatientInfDaysAct.push_back(__nPatientInfDaysActYear0);
            if (debug) std::cout << "inf days year 0 = " << __nPatientInfDaysActYear0[0] << "\n";
            break;
        }
    }
}

// record the infections day for the correct year in global, district and region variables
void Stats::setInfDays(int dist, int reg, int value, int year) {

    
    switch (year) {

    case 0:
        Stats::record_nPatientInfDaysYear0(dist, reg, value);
        break;
    case 1:
        Stats::record_nPatientInfDaysYear1(dist, reg, value);
        break;
    case 2:
        Stats::record_nPatientInfDaysYear2(dist, reg, value);
        break;
    case 3:
        Stats::record_nPatientInfDaysYear3(dist, reg, value);
        break;
    case 4:
        Stats::record_nPatientInfDaysYear4(dist, reg, value);
        break;
    case 5:
        Stats::record_nPatientInfDaysYear5(dist, reg, value);
        break;
    case 6:
        Stats::record_nPatientInfDaysYear6(dist, reg, value);
        break;
    case 7:
        Stats::record_nPatientInfDaysYear7(dist, reg, value);
        break;
    case 8:
        Stats::record_nPatientInfDaysYear8(dist, reg, value);
        break;
    case 9:
        Stats::record_nPatientInfDaysYear9(dist, reg, value);
        break;
    case 10:
        Stats::record_nPatientInfDaysYear10(dist, reg, value);
        break;
    case 11:
        Stats::record_nPatientInfDaysYear11(dist, reg, value);
        break;
    case 12:
        Stats::record_nPatientInfDaysYear12(dist, reg, value);
        break;
    case 13:
        Stats::record_nPatientInfDaysYear13(dist, reg, value);
        break;
    case 14:
        Stats::record_nPatientInfDaysYear14(dist, reg, value);
        break;
    case 15:
        Stats::record_nPatientInfDaysYear15(dist, reg, value);
        break;
    default:
        Stats::record_nPatientInfDaysYear0(dist, reg, value);
    }

}


// record the infections day for active patients for the correct year in global, district and region variables
void Stats::setInfDaysAct(int dist, int reg, int value, int year) {


    switch (year) {

    case 0:
        Stats::record_nPatientInfDaysActYear0(dist, reg, value);
        break;
    case 1:
        Stats::record_nPatientInfDaysActYear1(dist, reg, value);
        break;
    case 2:
        Stats::record_nPatientInfDaysActYear2(dist, reg, value);
        break;
    case 3:
        Stats::record_nPatientInfDaysActYear3(dist, reg, value);
        break;
    case 4:
        Stats::record_nPatientInfDaysActYear4(dist, reg, value);
        break;
    case 5:
        Stats::record_nPatientInfDaysActYear5(dist, reg, value);
        break;
    case 6:
        Stats::record_nPatientInfDaysActYear6(dist, reg, value);
        break;
    case 7:
        Stats::record_nPatientInfDaysActYear7(dist, reg, value);
        break;
    case 8:
        Stats::record_nPatientInfDaysActYear8(dist, reg, value);
        break;
    case 9:
        Stats::record_nPatientInfDaysActYear9(dist, reg, value);
        break;
    case 10:
        Stats::record_nPatientInfDaysActYear10(dist, reg, value);
        break;
    case 11:
        Stats::record_nPatientInfDaysActYear11(dist, reg, value);
        break;
    case 12:
        Stats::record_nPatientInfDaysActYear12(dist, reg, value);
        break;
    case 13:
        Stats::record_nPatientInfDaysActYear13(dist, reg, value);
        break;
    case 14:
        Stats::record_nPatientInfDaysActYear14(dist, reg, value);
        break;
    case 15:
        Stats::record_nPatientInfDaysActYear15(dist, reg, value);
        break;
    default:
        Stats::record_nPatientInfDaysActYear0(dist, reg, value);
    }

}


void Stats::recordOutputs(){

    //std::cout << "inside recordOutputs population =  " << __population[0] << "\n";
    //std::cout << "inf days =  " << __nPatientInfDaysYear0[0] << ", " << __nPatientInfDaysYear1[0] << ", " << __nPatientInfDaysYear2[0] << ", " << __nPatientInfDaysYear3[0];
    //std::cout << ", " << __nPatientInfDaysYear4[0] << ", " << __nPatientInfDaysYear5[0] << ", " << __nPatientInfDaysYear6[0] << ", " << __nPatientInfDaysYear7[0] << ", " << "\n";

    __vName.push_back(__name);
    __vId.push_back(__id);
    __vTier.push_back(__tier);
    __vXpertMach.push_back(__xpertMach);
    __vYear.push_back(__year);
    __vScenario.push_back(__scenario);
    __vDistrict.push_back(__district);
    __vRegion.push_back(__region);
    __vPopulation.push_back(__population);
    __vDisease.push_back(__disease);
    __vIncidence.push_back(__incidence);
    __vCalcIncidence.push_back(__calcIncidence);
    __vNPatient.push_back(__nPatient);
    __vNPatientInf.push_back(__nPatientInf);
    __vNPatientLtFUInf.push_back(__nPatientLtFUInf);
    __vNPatientFromTrans.push_back(__nPatientFromTrans);
    __vNPatientNotPresenting.push_back(__nPatientNotPresenting);
    __vNSamples.push_back(__nSamples);
    __vNSamplesIn.push_back(__nSamplesIn);
    __vNSamplesOut.push_back(__nSamplesOut);
    __vTimeSampleResourceWait.push_back(__timeSampleResourceWait);
    __vTimeSampleResourceWaitIn.push_back(__timeSampleResourceWaitIn);
    __vTimeSampleResourceWaitOut.push_back(__timeSampleResourceWaitOut);
    __vNTests.push_back(__nTests);
    __vNTestsIn.push_back(__nTestsIn);
    __vNTestsOut.push_back(__nTestsOut);
    __vNTransported.push_back(__nTransported);
    __vTimeSampleTransport.push_back(__timeSampleTransport);
    __vTimePresentToSampleTestIn.push_back(__timePresentToSampleTestIn);
    __vNPresentToSampleTestIn.push_back(__nPresentToSampleTestIn);
    __vTimePresentToSampleTestOut.push_back(__timePresentToSampleTestOut);
    __vNPresentToSampleTestOut.push_back(__nPresentToSampleTestOut);
    __vNTestPos.push_back(__nTestPos);
    __vNTestTruePos.push_back(__nTestTruePos);
    __vNTestTruePosIn.push_back(__nTestTruePosIn);
    __vNTestTruePosOut.push_back(__nTestTruePosOut);
    __vNTestFalsePos.push_back(__nTestFalsePos);
    __vNTestFalsePosIn.push_back(__nTestFalsePosIn);
    __vNTestFalsePosOut.push_back(__nTestFalsePosOut);
    __vNTestNeg.push_back(__nTestNeg);
    __vNTestTrueNeg.push_back(__nTestTrueNeg);
    __vNTestTrueNegIn.push_back(__nTestTrueNegIn);
    __vNTestTrueNegOut.push_back(__nTestTrueNegOut);
    __vNTestFalseNeg.push_back(__nTestFalseNeg);
    __vNTestFalseNegIn.push_back(__nTestFalseNegIn);
    __vNTestFalseNegOut.push_back(__nTestFalseNegOut);
    __vNTestPosIn.push_back(__nTestPosIn);
    __vNTestPosOut.push_back(__nTestPosOut);
    __vNMDCDInfected.push_back(__nMDCDInfected);
    __vNMDCDUninfected.push_back(__nMDCDUninfected);
    __vNPTMDCDInfected.push_back(__nPTMDCDInfected);
    __vNPTMDCDUninfected.push_back(__nPTMDCDUninfected);
    __vNDiagnosis.push_back(__nDiagnosis);
    __vNDiagnosisIn.push_back(__nDiagnosisIn);
    __vNDiagnosisOut.push_back(__nDiagnosisOut);
    __vNDiagnosisDC.push_back(__nDiagnosisDC);
    __vNDiagnosisDCTruePos.push_back(__nDiagnosisDCTruePos);
    __vNDiagnosisDCFalsePos.push_back(__nDiagnosisDCFalsePos);
    __vNPreTestDiagnosisTruePos.push_back(__nPreTestDiagnosisTruePos);
    __vNPreTestDiagnosisFalseNeg.push_back(__nPreTestDiagnosisFalseNeg);
    __vNPreTestDiagnosisTrueNeg.push_back(__nPreTestDiagnosisTrueNeg);
    __vNPreTestDiagnosisFalsePos.push_back(__nPreTestDiagnosisFalsePos);
    __vNPreTestDiagnosisTotal.push_back(__nPreTestDiagnosisTotal);
    __vNPreTestDiagnosisReview.push_back(__nPreTestDiagnosisReview);
    __vNPostTestDiagnosisTruePosIn.push_back(__nPostTestDiagnosisTruePosIn);
    __vNPostTestDiagnosisTruePosOut.push_back(__nPostTestDiagnosisTruePosOut);
    __vNPostTestDiagnosisFalseNeg.push_back(__nPostTestDiagnosisFalseNeg);
    __vNPostTestDiagnosisTrueNeg.push_back(__nPostTestDiagnosisTrueNeg);
    __vNPostTestDiagnosisFalsePosIn.push_back(__nPostTestDiagnosisFalsePosIn);
    __vNPostTestDiagnosisFalsePosOut.push_back(__nPostTestDiagnosisFalsePosOut);
    __vNPostTestDiagnosisTotal.push_back(__nPostTestDiagnosisTotal);
    __vNPostTestDiagnosisReview.push_back(__nPostTestDiagnosisReview);
    __vNMDDiagnosisTotal.push_back(__nMDDiagnosisTotal);
    __vNMDDiagnosisReview.push_back(__nMDDiagnosisReview);
    __vNPreTestLtFU.push_back(__nPreTestLtFU);
    __vNPreTestLtFUInf.push_back(__nPreTestLtFUInf);
    __vNPreTestLtFUDied.push_back(__nPreTestLtFUDied);
    __vNPreTestLtFUCured.push_back(__nPreTestLtFUCured);
    __vNPreTestLtFURepresent.push_back(__nPreTestLtFURepresent);
    __vNPreTreatLtFU.push_back(__nPreTreatLtFU);
    __vNPreTreatLtFUInf.push_back(__nPreTreatLtFUInf);
    __vNPreTreatLtFUDied.push_back(__nPreTreatLtFUDied);
    __vNPreTreatLtFUCured.push_back(__nPreTreatLtFUCured);
    __vNPreTreatLtFURepresent.push_back(__nPreTreatLtFURepresent);
    __vNPreTreatLtFUFN.push_back(__nPreTreatLtFUFN);
    __vNPreTreatLtFUFNDied.push_back(__nPreTreatLtFUFNDied);
    __vNPreTreatLtFUFNCured.push_back(__nPreTreatLtFUFNCured);
    __vNPreTreatLtFUFNRepresent.push_back(__nPreTreatLtFUFNRepresent);
    __vNPreTreatLtFUPos.push_back(__nPreTreatLtFUPos);
    __vNPreTreatLtFUNeg.push_back(__nPreTreatLtFUNeg);
    __vNTreatLtFU.push_back(__nTreatLtFU);
    __vNTreatLtFUInf.push_back(__nTreatLtFUInf);
    __vNTreatLtFUDied.push_back(__nTreatLtFUDied);
    __vNTreatLtFUCured.push_back(__nTreatLtFUCured);
    __vNTreatLtFURepresent.push_back(__nTreatLtFURepresent);
    __vNPatientsTreated.push_back(__nPatientsTreated);
    __vNPatientsTreatedInf.push_back(__nPatientsTreatedInf);
    __vNPatientsTreatedUninf.push_back(__nPatientsTreatedUninf);
    __vNPatientsPreTreatedLtoFUninf.push_back(__nPatientsPreTreatedLtoFUninf);
    __vNPatientsTreatedCured.push_back(__nPatientsTreatedCured);
    __vNPatientsTreatedDied.push_back(__nPatientsTreatedDied);
    __vNPatientsTreatedRelapse.push_back(__nPatientsTreatedRelapse);
    __vNTreatedRelapse.push_back(__nTreatedRelapse);
    __vNTreatedRelapseCured.push_back(__nTreatedRelapseCured);
    __vNTreatedRelapseDied.push_back(__nTreatedRelapseDied);
    __vTimePresentToSampleDiagIn.push_back(__timePresentToSampleDiagIn);
    __vNPresentToSampleDiagIn.push_back(__nPresentToSampleDiagIn);
    __vTimePresentToSampleDiagOut.push_back(__timePresentToSampleDiagOut);
    __vNPresentToSampleDiagOut.push_back(__nPresentToSampleDiagOut);
    __vTimePresentToSampleTreat.push_back(__timePresentToSampleTreat);
    __vNPresentToSampleTreat.push_back(__nPresentToSampleTreat);
    __vTimePresentToSampleTreatIn.push_back(__timePresentToSampleTreatIn);
    __vNPresentToSampleTreatIn.push_back(__nPresentToSampleTreatIn);
    __vTimePresentToSampleTreatOut.push_back(__timePresentToSampleTreatOut);
    __vNPresentToSampleTreatOut.push_back(__nPresentToSampleTreatOut);
    __vTimePresentToSampleDeath.push_back(__timePresentToSampleDeath);
    __vNPresentToSampleDeath.push_back(__nPresentToSampleDeath);
    __vTimePresentToSampleDeathIn.push_back(__timePresentToSampleDeathIn);
    __vNPresentToSampleDeathIn.push_back(__nPresentToSampleDeathIn);
    __vTimePresentToSampleDeathOut.push_back(__timePresentToSampleDeathOut);
    __vNPresentToSampleDeathOut.push_back(__nPresentToSampleDeathOut);
    __vTimePresentToSampleDeathPreTestLTFU.push_back(__timePresentToSampleDeathPreTestLTFU);
    __vNPresentToSampleDeathPreTestLTFU.push_back(__nPresentToSampleDeathPreTestLTFU);
    __vTimePresentToSampleDeathPreTreatLTFU.push_back(__timePresentToSampleDeathPreTreatLTFU);
    __vNPresentToSampleDeathPreTreatLTFU.push_back(__nPresentToSampleDeathPreTreatLTFU);
    __vTimePresentToSampleDeathTreatLTFU.push_back(__timePresentToSampleDeathTreatLTFU);
    __vNPresentToSampleDeathTreatLTFU.push_back(__nPresentToSampleDeathTreatLTFU);
    __vTimePresentToSampleDeathInTreat.push_back(__timePresentToSampleDeathInTreat);
    __vNPresentToSampleDeathInTreat.push_back(__nPresentToSampleDeathInTreat);
    __vTimePresentToSampleDeathInRelapse.push_back(__timePresentToSampleDeathInRelapse);
    __vNPresentToSampleDeathInRelapse.push_back(__nPresentToSampleDeathInRelapse);
    __vTimePresentToExit.push_back(__timePresentToExit);
    __vNPresentToExit.push_back(__nPresentToExit);
    __vNXpert.push_back(__nXpert);
    __vNXpertIn.push_back(__nXpertIn);
    __vNXpertOut.push_back(__nXpertOut);
    __vNXpertPos.push_back(__nXpertPos);
    __vNXpertTruePos.push_back(__nXpertTruePos);
    __vNXpertFalsePos.push_back(__nXpertFalsePos);
    __vNXpertNeg.push_back(__nXpertNeg);
    __vNXpertTrueNeg.push_back(__nXpertTrueNeg);
    __vNXpertFalseNeg.push_back(__nXpertFalseNeg);
    __vNSmear.push_back(__nSmear);
    __vNSmearIn.push_back(__nSmearIn);
    __vNSmearOut.push_back(__nSmearOut);
    __vNSmearPos.push_back(__nSmearPos);
    __vNSmearTruePos.push_back(__nSmearTruePos);
    __vNSmearFalsePos.push_back(__nSmearFalsePos);
    __vNSmearNeg.push_back(__nSmearNeg);
    __vNSmearTrueNeg.push_back(__nSmearTrueNeg);
    __vNSmearFalseNeg.push_back(__nSmearFalseNeg);
    __vNDeathPreTest.push_back(__nDeathPreTest);
    __vNDeathPreTreat.push_back(__nDeathPreTreat);
    __vNDeathTreat.push_back(__nDeathTreat);
    __vNDeathReTreat.push_back(__nDeathReTreat);
    __vNDeathUntreat.push_back(__nDeathUntreat);
    __vNPatientInfDaysYear0.push_back(__nPatientInfDaysYear0);
    __vNPatientInfDaysYear1.push_back(__nPatientInfDaysYear1);
    __vNPatientInfDaysYear2.push_back(__nPatientInfDaysYear2);
    __vNPatientInfDaysYear3.push_back(__nPatientInfDaysYear3);
    __vNPatientInfDaysYear4.push_back(__nPatientInfDaysYear4);
    __vNPatientInfDaysYear5.push_back(__nPatientInfDaysYear5);
    __vNPatientInfDaysYear6.push_back(__nPatientInfDaysYear6);
    __vNPatientInfDaysYear7.push_back(__nPatientInfDaysYear7);
    __vNPatientInfDaysYear8.push_back(__nPatientInfDaysYear8);
    __vNPatientInfDaysYear9.push_back(__nPatientInfDaysYear9);
    __vNPatientInfDaysYear10.push_back(__nPatientInfDaysYear10);
    __vNPatientInfDaysYear11.push_back(__nPatientInfDaysYear11);
    __vNPatientInfDaysYear12.push_back(__nPatientInfDaysYear12);
    __vNPatientInfDaysYear13.push_back(__nPatientInfDaysYear13);
    __vNPatientInfDaysYear14.push_back(__nPatientInfDaysYear14);
    __vNPatientInfDaysYear15.push_back(__nPatientInfDaysYear15);
    __vNPatientInfDaysActYear0.push_back(__nPatientInfDaysActYear0);
    __vNPatientInfDaysActYear1.push_back(__nPatientInfDaysActYear1);
    __vNPatientInfDaysActYear2.push_back(__nPatientInfDaysActYear2);
    __vNPatientInfDaysActYear3.push_back(__nPatientInfDaysActYear3);
    __vNPatientInfDaysActYear4.push_back(__nPatientInfDaysActYear4);
    __vNPatientInfDaysActYear5.push_back(__nPatientInfDaysActYear5);
    __vNPatientInfDaysActYear6.push_back(__nPatientInfDaysActYear6);
    __vNPatientInfDaysActYear7.push_back(__nPatientInfDaysActYear7);
    __vNPatientInfDaysActYear8.push_back(__nPatientInfDaysActYear8);
    __vNPatientInfDaysActYear9.push_back(__nPatientInfDaysActYear9);
    __vNPatientInfDaysActYear10.push_back(__nPatientInfDaysActYear10);
    __vNPatientInfDaysActYear11.push_back(__nPatientInfDaysActYear11);
    __vNPatientInfDaysActYear12.push_back(__nPatientInfDaysActYear12);
    __vNPatientInfDaysActYear13.push_back(__nPatientInfDaysActYear13);
    __vNPatientInfDaysActYear14.push_back(__nPatientInfDaysActYear14);
    __vNPatientInfDaysActYear15.push_back(__nPatientInfDaysActYear15);
    __vSPatientWaiting.push_back(__sPatientWaiting);
    __vSPatientWaitingIn.push_back(__sPatientWaitingIn);
    __vSPatientWaitingOut.push_back(__sPatientWaitingOut);
    __vSPatientInTreatment.push_back(__sPatientInTreatment);
    __vSSampleWaitingForTest.push_back(__sSampleWaitingForTest);
    __vSSampleWaitingForTestIn.push_back(__sSampleWaitingForTestIn);
    __vSSampleWaitingForTestOut.push_back(__sSampleWaitingForTestOut);

    
    //
    for (int i = 0; i < noElementsDistrictRegionArray; i++) {

        __name[i] = "";
        __id[i] = 0;
        __tier[i] = 0;
        __xpertMach[i] = 0;
        __year[i] = 0;
        __scenario[i] = "";
        __district[i] = "";
        __region[i] = "";
        __population[i] = 0;
        __disease[i] = "";
        __incidence[i] = 0;
        __calcIncidence[i] = 0;
        __nPatient[i] = 0;
        __nPatientInf[i] = 0;
        __nPatientLtFUInf[i] = 0;
        __nPatientFromTrans[i] = 0;
        __nPatientNotPresenting[i] = 0;
        __nSamples[i] = 0;
        __nSamplesIn[i] = 0;
        __nSamplesOut[i] = 0;
        __timeSampleResourceWait[i] = 0;
        __timeSampleResourceWaitIn[i] = 0;
        __timeSampleResourceWaitOut[i] = 0;
        __nTests[i] = 0;
        __nTestsIn[i] = 0;
        __nTestsOut[i] = 0;
        __nTransported[i] = 0;
        __timeSampleTransport[i] = 0;
        __timePresentToSampleTestIn[i] = 0;
        __nPresentToSampleTestIn[i] = 0;
        __timePresentToSampleTestOut[i] = 0;
        __nPresentToSampleTestOut[i] = 0;
        __nTestPos[i] = 0;
        __nTestTruePos[i] = 0;
        __nTestTruePosIn[i] = 0;
        __nTestTruePosOut[i] = 0;
        __nTestFalsePos[i] = 0;
        __nTestFalsePosIn[i] = 0;
        __nTestFalsePosOut[i] = 0;
        __nTestNeg[i] = 0;
        __nTestTrueNeg[i] = 0;
        __nTestTrueNegIn[i] = 0;
        __nTestTrueNegOut[i] = 0;
        __nTestFalseNeg[i] = 0;
        __nTestFalseNegIn[i] = 0;
        __nTestFalseNegOut[i] = 0;
        __nTestPosIn[i] = 0;
        __nTestPosOut[i] = 0;
        __nMDCDInfected[i] = 0;
        __nMDCDUninfected[i] = 0;
        __nPTMDCDInfected[i] = 0;
        __nPTMDCDUninfected[i] = 0;
        __nDiagnosis[i] = 0;
        __nDiagnosisIn[i] = 0;
        __nDiagnosisOut[i] = 0;
        __nDiagnosisDC[i] = 0;
        __nDiagnosisDCTruePos[i] = 0;
        __nDiagnosisDCFalsePos[i] = 0;
        __nPreTestDiagnosisTruePos[i] = 0;
        __nPreTestDiagnosisFalseNeg[i] = 0;
        __nPreTestDiagnosisTrueNeg[i] = 0;
        __nPreTestDiagnosisFalsePos[i] = 0;
        __nPreTestDiagnosisTotal[i] = 0;
        __nPreTestDiagnosisReview[i] = 0;
        __nPostTestDiagnosisTruePosIn[i] = 0;
        __nPostTestDiagnosisTruePosOut[i] = 0;
        __nPostTestDiagnosisFalseNeg[i] = 0;
        __nPostTestDiagnosisTrueNeg[i] = 0;
        __nPostTestDiagnosisFalsePosIn[i] = 0;
        __nPostTestDiagnosisFalsePosOut[i] = 0;
        __nPostTestDiagnosisTotal[i] = 0;
        __nPostTestDiagnosisReview[i] = 0;
        __nMDDiagnosisTotal[i] = 0;
        __nMDDiagnosisReview[i] = 0;
        __nPreTestLtFU[i] = 0;
        __nPreTestLtFUInf[i] = 0;
        __nPreTestLtFUDied[i] = 0;
        __nPreTestLtFUCured[i] = 0;
        __nPreTestLtFURepresent[i] = 0;
        __nPreTreatLtFU[i] = 0;
        __nPreTreatLtFUInf[i] = 0;
        __nPreTreatLtFUDied[i] = 0;
        __nPreTreatLtFUCured[i] = 0;
        __nPreTreatLtFURepresent[i] = 0;
        __nPreTreatLtFUFN[i] = 0;
        __nPreTreatLtFUFNDied[i] = 0;
        __nPreTreatLtFUFNCured[i] = 0;
        __nPreTreatLtFUFNRepresent[i] = 0;
        __nPreTreatLtFUPos[i] = 0;
        __nPreTreatLtFUNeg[i] = 0;
        __nTreatLtFU[i] = 0;
        __nTreatLtFUInf[i] = 0;
        __nTreatLtFUDied[i] = 0;
        __nTreatLtFUCured[i] = 0;
        __nTreatLtFURepresent[i] = 0;
        __nPatientsTreated[i] = 0;
        __nPatientsTreatedInf[i] = 0;
        __nPatientsTreatedUninf[i] = 0;
        __nPatientsPreTreatedLtoFUninf[i] = 0;
        __nPatientsTreatedCured[i] = 0;
        __nPatientsTreatedDied[i] = 0;
        __nPatientsTreatedRelapse[i] = 0;
        __nTreatedRelapse[i] = 0;
        __nTreatedRelapseCured[i] = 0;
        __nTreatedRelapseDied[i] = 0;
        __timePresentToSampleDiagIn[i] = 0;
        __nPresentToSampleDiagIn[i] = 0;
        __timePresentToSampleDiagOut[i] = 0;
        __nPresentToSampleDiagOut[i] = 0;
        __timePresentToSampleTreat[i] = 0;
        __nPresentToSampleTreat[i] = 0;
        __timePresentToSampleTreatIn[i] = 0;
        __nPresentToSampleTreatIn[i] = 0;
        __timePresentToSampleTreatOut[i] = 0;
        __nPresentToSampleTreatOut[i] = 0;
        __timePresentToSampleDeath[i] = 0;
        __nPresentToSampleDeath[i] = 0;
        __timePresentToSampleDeathIn[i] = 0;
        __nPresentToSampleDeathIn[i] = 0;
        __timePresentToSampleDeathOut[i] = 0;
        __nPresentToSampleDeathOut[i] = 0;
        __timePresentToSampleDeathPreTestLTFU[i] = 0;
        __nPresentToSampleDeathPreTestLTFU[i] = 0;
        __timePresentToSampleDeathPreTreatLTFU[i] = 0;
        __nPresentToSampleDeathPreTreatLTFU[i] = 0;
        __timePresentToSampleDeathPreTreatLTFU[i] = 0;
        __nPresentToSampleDeathPreTreatLTFU[i] = 0;
        __timePresentToSampleDeathTreatLTFU[i] = 0;
        __nPresentToSampleDeathTreatLTFU[i] = 0;
        __timePresentToSampleDeathTreatLTFU[i] = 0;
        __nPresentToSampleDeathTreatLTFU[i] = 0;
        __timePresentToSampleDeathInTreat[i] = 0;
        __nPresentToSampleDeathInTreat[i] = 0;
        __timePresentToSampleDeathInRelapse[i] = 0;
        __nPresentToSampleDeathInRelapse[i] = 0;
        __timePresentToExit[i] = 0;
        __nPresentToExit[i] = 0;
        __nXpert[i] = 0;
        __nXpertIn[i] = 0;
        __nXpertOut[i] = 0;
        __nXpertPos[i] = 0;
        __nXpertTruePos[i] = 0;
        __nXpertFalsePos[i] = 0;
        __nXpertNeg[i] = 0;
        __nXpertTrueNeg[i] = 0;
        __nXpertFalseNeg[i] = 0;
        __nSmear[i] = 0;
        __nSmearIn[i] = 0;
        __nSmearOut[i] = 0;
        __nSmearPos[i] = 0;
        __nSmearTruePos[i] = 0;
        __nSmearFalsePos[i] = 0;
        __nSmearNeg[i] = 0;
        __nSmearTrueNeg[i] = 0;
        __nSmearFalseNeg[i] = 0;
        __nDeathPreTest[i] = 0;
        __nDeathPreTreat[i] = 0;
        __nDeathTreat[i] = 0;
        __nDeathReTreat[i] = 0;
        __nDeathUntreat[i] = 0;
        // don't reset infectious days 
        //__nPatientInfDaysYear0[i] = 0;
        __sPatientWaiting[i] = 0;
        __sPatientWaitingIn[i] = 0;
        __sPatientWaitingOut[i] = 0;
        __sPatientInTreatment[i] = 0;
        __sSampleWaitingForTest[i] = 0;
        __sSampleWaitingForTestIn[i] = 0;
        __sSampleWaitingForTestOut[i] = 0;
    }
    //std::cout << "leaving recordOutputs  " <<  "\n";
}

void Stats::resetOutputs(){

    for (int i = 0; i < noElementsDistrictRegionArray; i++) {

        __name[i] = "";
        __id[i] = 0;
        __tier[i] = 0;
        __xpertMach[i] = 0;
        __year[i] = 0;
        __scenario[i] = "";
        __district[i] = "";
        __region[i] = "";
        __population[i] = 0;
        __disease[i] = "";
        __incidence[i] = 0;
        __calcIncidence[i] = 0;
        __nPatient[i] = 0;
        __nPatientInf[i] = 0;
        __nPatientLtFUInf[i] = 0;
        __nPatientFromTrans[i] = 0;
        __nPatientNotPresenting[i] = 0;
        __nSamples[i] = 0;
        __nSamplesIn[i] = 0;
        __nSamplesOut[i] = 0;
        __timeSampleResourceWait[i] = 0;
        __timeSampleResourceWaitIn[i] = 0;
        __timeSampleResourceWaitOut[i] = 0;
        __nTests[i] = 0;
        __nTestsIn[i] = 0;
        __nTestsOut[i] = 0;
        __nTransported[i] = 0;
        __timeSampleTransport[i] = 0;
        __timePresentToSampleTestIn[i] = 0;
        __nPresentToSampleTestIn[i] = 0;
        __timePresentToSampleTestOut[i] = 0;
        __nPresentToSampleTestOut[i] = 0;
        __nTestPos[i] = 0;
        __nTestTruePos[i] = 0;
        __nTestTruePosIn[i] = 0;
        __nTestTruePosOut[i] = 0;
        __nTestFalsePos[i] = 0;
        __nTestFalsePosIn[i] = 0;
        __nTestFalsePosOut[i] = 0;
        __nTestNeg[i] = 0;
        __nTestTrueNeg[i] = 0;
        __nTestTrueNegIn[i] = 0;
        __nTestTrueNegOut[i] = 0;
        __nTestFalseNeg[i] = 0;
        __nTestFalseNegIn[i] = 0;
        __nTestFalseNegOut[i] = 0;
        __nTestPosIn[i] = 0;
        __nTestPosOut[i] = 0;
        __nMDCDInfected[i] = 0;
        __nMDCDUninfected[i] = 0;
        __nPTMDCDInfected[i] = 0;
        __nPTMDCDUninfected[i] = 0;
        __nDiagnosis[i] = 0;
        __nDiagnosisIn[i] = 0;
        __nDiagnosisOut[i] = 0;
        __nDiagnosisDC[i] = 0;
        __nDiagnosisDCTruePos[i] = 0;
        __nDiagnosisDCFalsePos[i] = 0;
        __nPreTestDiagnosisTruePos[i] = 0;
        __nPreTestDiagnosisFalseNeg[i] = 0;
        __nPreTestDiagnosisTrueNeg[i] = 0;
        __nPreTestDiagnosisFalsePos[i] = 0;
        __nPreTestDiagnosisTotal[i] = 0;
        __nPreTestDiagnosisReview[i] = 0;
        __nPostTestDiagnosisTruePosIn[i] = 0;
        __nPostTestDiagnosisTruePosOut[i] = 0;
        __nPostTestDiagnosisFalseNeg[i] = 0;
        __nPostTestDiagnosisTrueNeg[i] = 0;
        __nPostTestDiagnosisFalsePosIn[i] = 0;
        __nPostTestDiagnosisFalsePosOut[i] = 0;
        __nPostTestDiagnosisTotal[i] = 0;
        __nPostTestDiagnosisReview[i] = 0;
        __nMDDiagnosisTotal[i] = 0;
        __nMDDiagnosisReview[i] = 0;
        __nPreTestLtFU[i] = 0;
        __nPreTestLtFUInf[i] = 0;
        __nPreTestLtFUDied[i] = 0;
        __nPreTestLtFUCured[i] = 0;
        __nPreTestLtFURepresent[i] = 0;
        __nPreTreatLtFU[i] = 0;
        __nPreTreatLtFUInf[i] = 0;
        __nPreTreatLtFUDied[i] = 0;
        __nPreTreatLtFUCured[i] = 0;
        __nPreTreatLtFURepresent[i] = 0;
        __nPreTreatLtFUFN[i] = 0;
        __nPreTreatLtFUFNDied[i] = 0;
        __nPreTreatLtFUFNCured[i] = 0;
        __nPreTreatLtFUFNRepresent[i] = 0;
        __nPreTreatLtFUPos[i] = 0;
        __nPreTreatLtFUNeg[i] = 0;
        __nTreatLtFU[i] = 0;
        __nTreatLtFUInf[i] = 0;
        __nTreatLtFUDied[i] = 0;
        __nTreatLtFUCured[i] = 0;
        __nTreatLtFURepresent[i] = 0;
        __nPatientsTreated[i] = 0;
        __nPatientsTreatedInf[i] = 0;
        __nPatientsTreatedUninf[i] = 0;
        __nPatientsPreTreatedLtoFUninf[i] = 0;
        __nPatientsTreatedCured[i] = 0;
        __nPatientsTreatedDied[i] = 0;
        __nPatientsTreatedRelapse[i] = 0;
        __nTreatedRelapse[i] = 0;
        __nTreatedRelapseCured[i] = 0;
        __nTreatedRelapseDied[i] = 0;
        __timePresentToSampleDiagIn[i] = 0;
        __nPresentToSampleDiagIn[i] = 0;
        __timePresentToSampleDiagOut[i] = 0;
        __nPresentToSampleDiagOut[i] = 0;
        __timePresentToSampleTreat[i] = 0;
        __nPresentToSampleTreat[i] = 0;
        __timePresentToSampleTreatIn[i] = 0;
        __nPresentToSampleTreatIn[i] = 0;
        __timePresentToSampleTreatOut[i] = 0;
        __nPresentToSampleTreatOut[i] = 0;
        __timePresentToSampleDeath[i] = 0;
        __nPresentToSampleDeath[i] = 0;
        __timePresentToSampleDeathIn[i] = 0;
        __nPresentToSampleDeathIn[i] = 0;
        __timePresentToSampleDeathOut[i] = 0;
        __nPresentToSampleDeathOut[i] = 0;
        __timePresentToSampleDeathPreTestLTFU[i] = 0;
        __nPresentToSampleDeathPreTestLTFU[i] = 0;
        __timePresentToSampleDeathPreTreatLTFU[i] = 0;
        __nPresentToSampleDeathPreTreatLTFU[i] = 0;
        __timePresentToSampleDeathTreatLTFU[i] = 0;
        __nPresentToSampleDeathTreatLTFU[i] = 0;
        __timePresentToSampleDeathInTreat[i] = 0;
        __nPresentToSampleDeathInTreat[i] = 0;
        __timePresentToSampleDeathInRelapse[i] = 0;
        __nPresentToSampleDeathInRelapse[i] = 0;
        __timePresentToExit[i] = 0;
        __nPresentToExit[i] = 0;
        __nXpert[i] = 0;
        __nXpertIn[i] = 0;
        __nXpertOut[i] = 0;
        __nXpertPos[i] = 0;
        __nXpertTruePos[i] = 0;
        __nXpertFalsePos[i] = 0;
        __nXpertNeg[i] = 0;
        __nXpertTrueNeg[i] = 0;
        __nXpertFalseNeg[i] = 0;
        __nSmear[i] = 0;
        __nSmearIn[i] = 0;
        __nSmearOut[i] = 0;
        __nSmearPos[i] = 0;
        __nSmearTruePos[i] = 0;
        __nSmearFalsePos[i] = 0;
        __nSmearNeg[i] = 0;
        __nSmearTrueNeg[i] = 0;
        __nSmearFalseNeg[i] = 0;
        __nDeathPreTest[i] = 0;
        __nDeathPreTreat[i] = 0;
        __nDeathTreat[i] = 0;
        __nDeathReTreat[i] = 0;
        __nDeathUntreat[i] = 0;
        __nPatientInfDays[i] = 0;
        __nPatientInfDaysAct[i] = 0;
        __sPatientWaiting[i] = 0;
        __sPatientWaitingIn[i] = 0;
        __sPatientWaitingOut[i] = 0;
        __sPatientInTreatment[i] = 0;
        __sSampleWaitingForTest[i] = 0;
        __sSampleWaitingForTestIn[i] = 0;
        __sSampleWaitingForTestOut[i] = 0;
    }

    //
    //__vvDiseaseStates.clear();
    __vName.clear();
    __vId.clear();
    __vTier.clear();
    __vXpertMach.clear();
    __vYear.clear();
    __vScenario.clear();
    __vDistrict.clear();
    __vRegion.clear();
    __vPopulation.clear();
    __vDisease.clear();
    __vIncidence.clear();
    __vCalcIncidence.clear();
    __vNPatient.clear();
    __vNPatientInf.clear();
    __vNPatientLtFUInf.clear();
    __vNPatientFromTrans.clear();
    __vNPatientNotPresenting.clear();
    __vNSamples.clear();
    __vNSamplesIn.clear();
    __vNSamplesOut.clear();
    __vTimeSampleResourceWait.clear();
    __vTimeSampleResourceWaitIn.clear();
    __vTimeSampleResourceWaitOut.clear();
    __vNTests.clear();
    __vNTestsIn.clear();
    __vNTestsOut.clear();
    __vNTransported.clear();
    __vTimeSampleTransport.clear();
    __vTimePresentToSampleTestIn.clear();
    __vNPresentToSampleTestIn.clear();
    __vTimePresentToSampleTestOut.clear();
    __vNPresentToSampleTestOut.clear();
    __vNTestPos.clear();
    __vNTestTruePos.clear();
    __vNTestTruePosIn.clear();
    __vNTestTruePosOut.clear();
    __vNTestFalsePos.clear();
    __vNTestFalsePosIn.clear();
    __vNTestFalsePosOut.clear();
    __vNTestNeg.clear();
    __vNTestTrueNeg.clear();
    __vNTestTrueNegIn.clear();
    __vNTestTrueNegOut.clear();
    __vNTestFalseNeg.clear();
    __vNTestFalseNegIn.clear();
    __vNTestFalseNegOut.clear();
    __vNTestPosIn.clear();
    __vNTestPosOut.clear();
    __vNMDCDInfected.clear();
    __vNMDCDUninfected.clear();
    __vNPTMDCDInfected.clear();
    __vNPTMDCDUninfected.clear();
    __vNDiagnosis.clear();
    __vNDiagnosisIn.clear();
    __vNDiagnosisOut.clear();
    __vNDiagnosisDC.clear();
    __vNDiagnosisDCTruePos.clear();
    __vNDiagnosisDCFalsePos.clear();
    __vNPreTestDiagnosisTruePos.clear();
    __vNPreTestDiagnosisFalseNeg.clear();
    __vNPreTestDiagnosisTrueNeg.clear();
    __vNPreTestDiagnosisFalsePos.clear();
    __vNPreTestDiagnosisTotal.clear();
    __vNPreTestDiagnosisReview.clear();
    __vNPostTestDiagnosisTruePosIn.clear();
    __vNPostTestDiagnosisTruePosOut.clear();
    __vNPostTestDiagnosisFalseNeg.clear();
    __vNPostTestDiagnosisTrueNeg.clear();
    __vNPostTestDiagnosisFalsePosIn.clear();
    __vNPostTestDiagnosisFalsePosOut.clear();
    __vNPostTestDiagnosisTotal.clear();
    __vNPostTestDiagnosisReview.clear();
    __vNMDDiagnosisTotal.clear();
    __vNMDDiagnosisReview.clear();
    __vNPreTestLtFU.clear();
    __vNPreTestLtFUInf.clear();
    __vNPreTestLtFUDied.clear();
    __vNPreTestLtFUCured.clear();
    __vNPreTestLtFURepresent.clear();
    __vNPreTreatLtFU.clear();
    __vNPreTreatLtFUInf.clear();
    __vNPreTreatLtFUDied.clear();
    __vNPreTreatLtFUCured.clear();
    __vNPreTreatLtFURepresent.clear();
    __vNPreTreatLtFUFN.clear();
    __vNPreTreatLtFUFNDied.clear();
    __vNPreTreatLtFUFNCured.clear();
    __vNPreTreatLtFUFNRepresent.clear();
    __vNPreTreatLtFUPos.clear();
    __vNPreTreatLtFUNeg.clear();
    __vNTreatLtFU.clear();
    __vNTreatLtFUInf.clear();
    __vNTreatLtFUDied.clear();
    __vNTreatLtFUCured.clear();
    __vNTreatLtFURepresent.clear();
    __vNPatientsTreated.clear();
    __vNPatientsTreatedInf.clear();
    __vNPatientsTreatedUninf.clear();
    __vNPatientsPreTreatedLtoFUninf.clear();
    __vNPatientsTreatedCured.clear();
    __vNPatientsTreatedDied.clear();
    __vNPatientsTreatedRelapse.clear();
    __vNTreatedRelapse.clear();
    __vNTreatedRelapseCured.clear();
    __vNTreatedRelapseDied.clear();
    __vTimePresentToSampleDiagIn.clear();
    __vNPresentToSampleDiagIn.clear();
    __vTimePresentToSampleDiagOut.clear();
    __vNPresentToSampleDiagOut.clear();
    __vTimePresentToSampleTreat.clear();
    __vNPresentToSampleTreat.clear();
    __vTimePresentToSampleTreatIn.clear();
    __vNPresentToSampleTreatIn.clear();
    __vTimePresentToSampleTreatOut.clear();
    __vNPresentToSampleTreatOut.clear();
    __vTimePresentToSampleDeath.clear();
    __vNPresentToSampleDeath.clear();
    __vTimePresentToSampleDeathIn.clear();
    __vNPresentToSampleDeathIn.clear();
    __vTimePresentToSampleDeathOut.clear();
    __vNPresentToSampleDeathOut.clear();
    __vTimePresentToSampleDeathPreTestLTFU.clear();
    __vNPresentToSampleDeathPreTestLTFU.clear();
    __vTimePresentToSampleDeathPreTreatLTFU.clear();
    __vNPresentToSampleDeathPreTreatLTFU.clear();
    __vTimePresentToSampleDeathTreatLTFU.clear();
    __vNPresentToSampleDeathTreatLTFU.clear();
    __vTimePresentToSampleDeathInTreat.clear();
    __vNPresentToSampleDeathInTreat.clear();
    __vTimePresentToSampleDeathInRelapse.clear();
    __vNPresentToSampleDeathInRelapse.clear();
    __vTimePresentToExit.clear();
    __vNPresentToExit.clear();
    __vNXpert.clear();
    __vNXpertIn.clear();
    __vNXpertOut.clear();
    __vNXpertPos.clear();
    __vNXpertTruePos.clear();
    __vNXpertFalsePos.clear();
    __vNXpertNeg.clear();
    __vNXpertTrueNeg.clear();
    __vNXpertFalseNeg.clear();
    __vNSmear.clear();
    __vNSmearIn.clear();
    __vNSmearOut.clear();
    __vNSmearTruePos.clear();
    __vNSmearPos.clear();
    __vNSmearFalsePos.clear();
    __vNSmearNeg.clear();
    __vNSmearTrueNeg.clear();
    __vNSmearFalseNeg.clear();
    __vNDeathPreTest.clear();
    __vNDeathPreTreat.clear();
    __vNDeathTreat.clear();
    __vNDeathReTreat.clear();
    __vNDeathUntreat.clear();
    __vNPatientInfDays.clear();
    __vNPatientInfDaysAct.clear();
    __vSPatientWaiting.clear();
    __vSPatientWaitingIn.clear();
    __vSPatientWaitingOut.clear();
    __vSPatientInTreatment.clear();
    __vSSampleWaitingForTest.clear();
    __vSSampleWaitingForTestIn.clear();
    __vSSampleWaitingForTestOut.clear();
    
}


//vector<double> Stats::return_vpIncDS_fastprog(){return (vectorDivision(__vIncDS_fastprog,__vIncDS));}
//vector<double> Stats::return_vpIncDS_relapse(){return (vectorDivision(__vIncDS_relapse,__vIncDS));}
//vector<double> Stats::return_vpTransDS_HH(){return (vectorDivision(__vTransDS_HH,__vTransDS));}
//vector<double> Stats::return_vpTransDS_reinf(){return (vectorDivision(__vTransDS_reinf,__vTransDS));}
//vector<double> Stats::return_vpTransDS_reinf_HH(){return (vectorDivision(__vTransDS_reinf_HH,__vTransDS));}

//vector<double> Stats::return_vpIncDR_fastprog(){return (vectorDivision(__vIncDR_fastprog,__vIncDR));}
//vector<double> Stats::return_vpIncDR_relapse(){return (vectorDivision(__vIncDR_relapse,__vIncDR));}
//vector<double> Stats::return_vpIncDR_resist(){return (vectorDivision(__vIncDR_resist,__vIncDR));}
//vector<double> Stats::return_vpTransDR_HH(){return (vectorDivision(__vTransDR_HH,__vTransDR));}
//vector<double> Stats::return_vpTransDR_reinf(){return (vectorDivision(__vTransDR_reinf,__vTransDR));}
//vector<double> Stats::return_vpTransDR_reinf_HH(){return (vectorDivision(__vTransDR_reinf_HH,__vTransDR));}

//vector<double> Stats::return_vpFailedTrtDR(){return (vectorDivision(return_vFailedTrtDR(),__vTrtDR_DR));}


void Stats::runSaveAnnual(string diseaseName){
    
    // generate the annual file
    stringstream of;
    
    vector<string> colNames={"name","id","tier","xpert","year","scenario","district","region","population","seed","disease","calculated.incidence","incidence",
                            "n.patients.enter.new", "n.patients.enter.new.inf", "n.patients.enter.ltfu.inf", "n.patients.transmission", "n.patients.not.presenting", "n.samples.collected", "n.samples.out", "n.samples.inhouse", "ave.sample.resource.wait.time", "ave.sample.resource.wait.time.in.house",
                            "ave.sample.resource.wait.time.out", "ave.sample.transport.time", "total.samples.transported", "n.samples.transported", "n.samples.tested", "n.samples.tested.in.house", "n.samples.tested.out",
                            "ave.delay.from.sample.to.test.in.house.inf", "total.delay.from.sample.to.test.in.house.inf", "n.delay.from.sample.to.test.in.house.inf",
                            "ave.delay.from.sample.to.test.out.inf", "total.delay.from.sample.to.test.out.inf", "n.delay.from.sample.to.test.out.inf", "n.tests.pos", "p.tests.true.pos","n.tests.true.pos.in","n.tests.true.pos.out",
                            "p.tests.false.pos","n.tests.false.pos.in","n.tests.false.pos.out", "n.tests.neg", "p.tests.true.neg","n.tests.true.neg.in","n.tests.true.neg.out", "p.tests.false.neg", "n.tests.false.neg.in", "n.tests.false.neg.out",
                            "n.result.false.pos", "n.result.false.neg", "n.md.clinical.diagnosis.infected", "n.md.clinical.diagnosis.uninfected", "n.result.pos", "n.diagnosis", "n.diagnosis.in.house", "n.diagnosis.out", "n.diagnosis.md.cd.not.counted","n.diagnosis.md.cd.not.counted.true.pos","n.diagnosis.md.cd.not.counted.false.pos",
                            "n.pre.test.md.clinical.diagnosis.true.pos", "n.pre.test.md.clinical.diagnosis.false.pos", "n.pre.test.md.clinical.diagnosis",
                            "n.post.test.md.clinical.diagnosis.true.pos.in", "n.post.test.md.clinical.diagnosis.true.pos.out", "n.post.test.md.clinical.diagnosis.false.neg", "n.post.test.md.clinical.diagnosis.true.neg", "n.post.test.md.clinical.diagnosis.false.pos.in",  "n.post.test.md.clinical.diagnosis.false.pos.out", "n.post.test.md.clinical.diagnosis", "n.post.test.md.clinical.diagnosis.review",
                            "n.md.clinical.diagnosis", "n.md.clinical.diagnosis.review",
                            "n.pre.test.ltfu.out", "n.pre.test.ltfu.out.inf", "n.pre.test.ltfu.dead.inf", "n.pre.test.ltfu.cured.inf", "n.pre.test.ltfu.return.inf",
                            "n.pre.treat.ltfu", "n.pre.treat.ltfu.inf", "n.pre.treat.ltfu.dead.inf", "n.pre.treat.ltfu.cured.inf", "n.pre.treat.ltfu.return.inf",
                            "n.false.neg.diag.ltfu", "n.false.neg.diag.ltfu.dead.inf", "n.false.neg.diag.ltfu.cured.inf", "n.false.neg.diag.ltfu.return.inf",
                            "n.treat.ltfu", "n.treat.ltfu.inf", "n.treat.ltfu.dead.inf", "n.treat.ltfu.cured.inf", "n.treat.ltfu.return.inf",
                            "ave.delay.sample.to.diag.in.house.inf", "total.delay.sample.to.diag.in.house.inf", "n.delay.sample.to.diag.in.house.inf",
                            "ave.delay.sample.to.diag.out.inf", "total.delay.sample.to.diag.out.inf", "n.delay.sample.to.diag.out.inf",
                            "n.start.trt", "n.start.trt.infected", "n.start.trt.uninfected", "n.trt.cured.inf", "n.trt.died.inf", "n.trt.relapse.inf", 
                            "n.start.retrt.inf", "n.retrt.cured.inf", "n.retrt.died.inf",  "ave.delay.sample.to.trt.inf", "total.delay.sample.to.trt.inf", "n.delay.sample.to.trt.inf",
                            "ave.delay.sample.to.trt.in.house.inf", "total.delay.sample.to.trt.in.house.inf", "n.delay.sample.to.trt.in.house.inf", 
                            "ave.delay.sample.to.trt.out.inf", "total.delay.sample.to.trt.out.inf", "n.delay.sample.to.trt.out.inf",
                            "ave.delay.sample.to.death.inf", "total.delay.sample.to.death.inf", "n.delay.sample.to.death.inf", 
                            "ave.delay.sample.to.death.pre.test.ltfu.inf", "total.delay.sample.to.death.pre.test.ltfu.inf", "n.delay.sample.to.death.pre.test.ltfu.inf", 
                            "ave.delay.sample.to.death.pre.treat.ltfu.inf", "total.delay.sample.to.death.pre.treat.ltfu.inf", "n.delay.sample.to.death.pre.treat.ltfu.inf",
                            "ave.delay.sample.to.death.treat.ltfu.inf", "total.delay.sample.to.death.treat.ltfu.inf", "n.delay.sample.to.death.treat.ltfu.inf",
                            "ave.delay.sample.to.death.in.trt.inf", "total.delay.sample.to.death.in.trt.inf", "n.delay.sample.to.death.in.trt.inf",
                            "ave.delay.sample.to.death.in.retrt.inf", "total.delay.sample.to.death.in.retrt.inf", "n.delay.sample.to.death.in.retrt.inf", 
                            "ave.delay.sample.to.exit.inf", "total.delay.sample.to.exit.inf", "n.delay.sample.to.exit.inf",
                            "n.xpert",  "n.xpert.in.house", "n.xpert.out", "n.smear", "n.smear.pos", "p.smear.true.pos", "n.smear.neg", "p.smear.true.neg", 
                            "n.xpert.pos", "p.xpert.true.pos", "n.xpert.neg", "p.xpert.true.neg",
                            "n.death.re.treat", "n.death.untreated","n.infectious.days","n.infectious.days.active.patients",
                            "s.patient.waiting.in", "s.patient.waiting.out", " s.in.treatment", "s.samp.wait.for.test", "s.samp.wait.for.test.in", "s.samp.wait.for.test.out" };

    

    of.str("");
    of<<"output_ABM_Events_Annual_" << diseaseName << ".csv";
    writeVector(colNames,of.str(),true);
    writeVector(returnAnnualOutputs(0), of.str(), false);
}

void Stats::runSaveAnnualString(string diseaseName, string statSubScenarioDir, string jobNo, string regionDistrictArray[], int districtToRegionMapping[], string facilityArray[], bool addCommas, bool addTotals, bool addDistrictAndRegion, bool addFacilities, int year, bool includeColHeader, bool subYear){
    
    // generate the annual file
    stringstream of;
    string subDirBase;
    string name;
    string totalName;
    bool debug = false;

    vector<string> colNames={"name","id","tier","xpert","year","scenario","district","region","population","seed","disease","calculated.incidence","incidence",
                            "n.patients.enter.new", "n.patients.enter.new.inf", "n.patients.enter.ltfu.inf", "n.patients.transmission", "n.patients.not.presenting", "n.samples.collected", "n.samples.out", "n.samples.inhouse", "ave.sample.resource.wait.time", "ave.sample.resource.wait.time.in.house",
                            "ave.sample.resource.wait.time.out", "ave.sample.transport.time", "total.samples.transported", "n.samples.transported", "n.samples.tested", "n.samples.tested.in.house", "n.samples.tested.out",
                            "ave.delay.from.sample.to.test.in.house.inf", "total.delay.from.sample.to.test.in.house.inf", "n.delay.from.sample.to.test.in.house.inf",
                            "ave.delay.from.sample.to.test.out.inf", "total.delay.from.sample.to.test.out.inf", "n.delay.from.sample.to.test.out.inf", "n.tests.pos", "p.tests.true.pos", "n.tests.true.pos.in", "n.tests.true.pos.out",
                            "p.tests.false.pos","n.tests.false.pos.in","n.tests.false.pos.out", "n.tests.neg", "p.tests.true.neg", "n.tests.true.neg.in", "n.tests.true.neg.out", "p.tests.false.neg", "n.tests.false.neg.in", "n.tests.false.neg.out",
                            "n.result.false.pos", "n.result.false.neg", "n.md.clinical.diagnosis.infected", "n.md.clinical.diagnosis.uninfected", "n.result.pos", "n.diagnosis", "n.diagnosis.in.house", "n.diagnosis.out", "n.diagnosis.md.cd.not.counted", "n.diagnosis.md.cd.not.counted.true.pos", "n.diagnosis.md.cd.not.counted.false.pos",
                            "n.pre.test.md.clinical.diagnosis.true.pos", "n.pre.test.md.clinical.diagnosis.false.pos", "n.pre.test.md.clinical.diagnosis",
                            "n.post.test.md.clinical.diagnosis.true.pos.in", "n.post.test.md.clinical.diagnosis.true.pos.out", "n.post.test.md.clinical.diagnosis.false.neg",  "n.post.test.md.clinical.diagnosis.true.neg", "n.post.test.md.clinical.diagnosis.false.pos.in", "n.post.test.md.clinical.diagnosis.false.pos.out", "n.post.test.md.clinical.diagnosis", "n.post.test.md.clinical.diagnosis.review",
                            "n.md.clinical.diagnosis", "n.md.clinical.diagnosis.review",
                            "n.pre.test.ltfu.out", "n.pre.test.ltfu.out.inf", "n.pre.test.ltfu.dead.inf", "n.pre.test.ltfu.cured.inf", "n.pre.test.ltfu.return.inf",
                            "n.pre.treat.ltfu",  "n.pre.treat.ltfu.inf", "n.pre.treat.ltfu.dead.inf", "n.pre.treat.ltfu.cured.inf", "n.pre.treat.ltfu.return.inf",
                            "n.false.neg.diag.ltfu", "n.false.neg.diag.ltfu.dead.inf", "n.false.neg.diag.ltfu.cured.inf", "n.false.neg.diag.ltfu.return.inf",
                            "n.treat.ltfu", "n.treat.ltfu.inf", "n.treat.ltfu.dead.inf", "n.treat.ltfu.cured.inf", "n.treat.ltfu.return.inf",
                            "ave.delay.sample.to.diag.in.house.inf", "total.delay.sample.to.diag.in.house.inf", "n.delay.sample.to.diag.in.house.inf",
                            "ave.delay.sample.to.diag.out.inf", "total.delay.sample.to.diag.out.inf", "n.delay.sample.to.diag.out.inf",
                            "n.start.trt", "n.start.trt.infected", "n.start.trt.uninfected", "n.pre.trt.ltof.uninf", "n.trt.cured.inf", "n.trt.died.inf", "n.trt.relapse.inf", 
                            "n.start.retrt.inf", "n.retrt.cured.inf", "n.retrt.died.inf",  "ave.delay.sample.to.trt.inf", "total.delay.sample.to.trt.inf", "n.delay.sample.to.trt.inf",
                            "ave.delay.sample.to.trt.in.house.inf", "total.delay.sample.to.trt.in.house.inf", "n.delay.sample.to.trt.in.house.inf", 
                            "ave.delay.sample.to.trt.out.inf", "total.delay.sample.to.trt.out.inf", "n.delay.sample.to.trt.out.inf",
                            "ave.delay.sample.to.death.inf", "total.delay.sample.to.death.inf", "n.delay.sample.to.death.inf", 
                            "ave.delay.sample.to.death.pre.test.ltfu.inf", "total.delay.sample.to.death.pre.test.ltfu.inf", "n.delay.sample.to.death.pre.test.ltfu.inf",
                            "ave.delay.sample.to.death.pre.treat.ltfu.inf", "total.delay.sample.to.death.pre.treat.ltfu.inf", "n.delay.sample.to.death.pre.treat.ltfu.inf",
                            "ave.delay.sample.to.death.treat.ltfu.inf", "total.delay.sample.to.death.treat.ltfu.inf", "n.delay.sample.to.death.treat.ltfu.inf",
                            "ave.delay.sample.to.death.in.trt.inf", "total.delay.sample.to.death.in.trt.inf", "n.delay.sample.to.death.in.trt.inf",
                            "ave.delay.sample.to.death.in.retrt.inf", "total.delay.sample.to.death.in.retrt.inf", "n.delay.sample.to.death.in.retrt.inf", 
                            "ave.delay.sample.to.exit.inf", "total.delay.sample.to.exit.inf", "n.delay.sample.to.exit.inf",
                            "n.xpert",  "n.xpert.in.house", "n.xpert.out", "n.smear", "n.smear.pos", "p.smear.true.pos", "n.smear.neg", "p.smear.true.neg", 
                            "n.xpert.pos", "p.xpert.true.pos", "n.xpert.neg", "p.xpert.true.neg",
                            "n.death.re.treat", "n.death.untreated", "n.infectious.days", "n.infectious.days.active.patients",
                             "s.patient.waiting.in", "s.patient.waiting.out", " s.in.treatment", "s.samp.wait.for.test", "s.samp.wait.for.test.in", "s.samp.wait.for.test.out" };
                            
    vector<string> colNamesWCommas={"name,","id,","tier,","xpert,","year,","scenario,","district,","region,","population,","seed,","disease,","calculated.incidence,","incidence,",
                            "n.patients.enter.new,", "n.patients.enter.new.inf,", "n.patients.enter.ltfu.inf,", "n.patients.transmission,", "n.patients.not.presenting,",  "n.samples.collected,", "n.samples.out,", "n.samples.inhouse,", "ave.sample.resource.wait.time,", "ave.sample.resource.wait.time.in.house,",
                            "ave.sample.resource.wait.time.out,", "ave.sample.transport.time,", "total.samples.transported,", "n.samples.transported,", "n.samples.tested,", "n.samples.tested.in.house,", "n.samples.tested.out,",
                            "ave.delay.from.sample.to.test.in.house.inf,", "total.delay.from.sample.to.test.in.house.inf,", "n.delay.from.sample.to.test.in.house.inf,",
                            "ave.delay.from.sample.to.test.out.inf,",  "total.delay.from.sample.to.test.out.inf,", "n.delay.from.sample.to.test.out.inf,", "n.tests.pos,", "p.tests.true.pos,", "n.tests.true.pos.in,", "n.tests.true.pos.out,",
                            "p.tests.false.pos,","n.tests.false.pos.in,","n.tests.false.pos.out,", "n.tests.neg,", "p.tests.true.neg,", "n.tests.true.neg.in,", "n.tests.true.neg.out,", "p.tests.false.neg,", "n.tests.false.neg.in,", "n.tests.false.neg.out,",
                            "n.result.false.pos,", "n.result.false.neg,", "n.md.clinical.diagnosis.infected,", "n.md.clinical.diagnosis.uninfected,", "n.result.pos,", "n.diagnosis,", "n.diagnosis.in.house,", "n.diagnosis.out,", "n.diagnosis.md.cd.not.counted,", "n.diagnosis.md.cd.not.counted.true.pos,", "n.diagnosis.md.cd.not.counted.false.pos,",
                            "n.pre.test.md.clinical.diagnosis.true.pos,", "n.pre.test.md.clinical.diagnosis.false.pos,", "n.pre.test.md.clinical.diagnosis,",
                            "n.post.test.md.clinical.diagnosis.true.pos.in,", "n.post.test.md.clinical.diagnosis.true.pos.out,", "n.post.test.md.clinical.diagnosis.false.neg,", "n.post.test.md.clinical.diagnosis.true.neg,", "n.post.test.md.clinical.diagnosis.false.pos.in,", "n.post.test.md.clinical.diagnosis.false.pos.out,", "n.post.test.md.clinical.diagnosis,", "n.post.test.md.clinical.diagnosis.review,",
                            "n.md.clinical.diagnosis,", "n.md.clinical.diagnosis.review,",
                            "n.pre.test.ltfu.out,", "n.pre.test.ltfu.out.inf,", "n.pre.test.ltfu.dead.inf,", "n.pre.test.ltfu.cured.inf,", "n.pre.test.ltfu.return.inf,",
                            "n.pre.treat.ltfu,", "n.pre.treat.ltfu.inf,", "n.pre.treat.ltfu.dead.inf,", "n.pre.treat.ltfu.cured.inf,", "n.pre.treat.ltfu.return.inf,",
                            "n.false.neg.diag.ltfu,", "n.false.neg.diag.ltfu.dead.inf,", "n.false.neg.diag.ltfu.cured.inf,", "n.false.neg.diag.ltfu.return.inf,",
                            "n.treat.ltfu,", "n.treat.ltfu.inf,", "n.treat.ltfu.dead.inf,","n.treat.ltfu.cured.inf,", "n.treat.ltfu.return.inf,",
                            "ave.delay.sample.to.diag.in.house.inf,", "total.delay.sample.to.diag.in.house.inf,", "n.delay.sample.to.diag.in.house.inf,",
                            "ave.delay.sample.to.diag.out.inf,", "total.delay.sample.to.diag.out.inf,", "n.delay.sample.to.diag.out.inf,",
                            "n.start.trt,", "n.start.trt.infected,", "n.start.trt.uninfected,", "n.trt.cured.inf,", "n.trt.died.inf,", "n.trt.relapse.inf,",
                            "n.start.retrt.inf,", "n.retrt.cured.inf,", "n.retrt.died.inf,", "ave.delay.sample.to.trt.inf,", "total.delay.sample.to.trt.inf,", "n.delay.sample.to.trt.inf,",
                            "ave.delay.sample.to.trt.in.house.inf,", "total.delay.sample.to.trt.in.house.inf,", "n.delay.sample.to.trt.in.house.inf,", 
                            "ave.delay.sample.to.trt.out.inf,", "total.delay.sample.to.trt.out.inf,", "n.delay.sample.to.trt.out.inf,",
                            "ave.delay.sample.to.death.inf,", "total.delay.sample.to.death.inf,", "n.delay.sample.to.death.inf,", 
                            "ave.delay.sample.to.death.pre.test.ltfu.inf,", "total.delay.sample.to.death.pre.test.ltfu.inf,", "n.delay.sample.to.death.pre.test.ltfu.inf,",
                            "ave.delay.sample.to.death.pre.treat.ltfu.inf,", "total.delay.sample.to.death.pre.treat.ltfu.inf,", "n.delay.sample.to.death.pre.treat.ltfu.inf,",
                            "ave.delay.sample.to.death.treat.ltfu.inf,", "total.delay.sample.to.death.treat.ltfu.inf,", "n.delay.sample.to.death.treat.ltfu.inf,",
                            "ave.delay.sample.to.death.in.trt.inf,", "total.delay.sample.to.death.in.trt.inf,", "n.delay.sample.to.death.in.trt.inf,",
                            "ave.delay.sample.to.death.in.retrt.inf,", "total.delay.sample.to.death.in.retrt.inf,", "n.delay.sample.to.death.in.retrt.inf,", 
                            "ave.delay.sample.to.exit.inf,", "total.delay.sample.to.exit.inf,", "n.delay.sample.to.exit.inf,",
                            "n.xpert,",  "n.xpert.in.house,", "n.xpert.out,", "n.smear,", "n.smear.pos,", "p.smear.true.pos,", "n.smear.neg,", "p.smear.true.neg,", 
                            "n.xpert.pos,", "p.xpert.true.pos,", "n.xpert.neg,", "p.xpert.true.neg,",
                            "n.death.re.treat,", "n.death.untreated,","n.infectious.days,","n.infectious.days.active.patients,",
                            "s.patient.waiting.in,", "s.patient.waiting.out,", "s.in.treatment,", "s.samp.wait.for.test,", "s.samp.wait.for.test.in,", "s.samp.wait.for.test.out" };

   
    // This array maps the region indexes to each of the districts in regionDistrictArray
    //int districtToRegionMapping[260] = {1,2,3,4,5,6,7,8,9,10,11,5,3,12,1,5,11,8,12,8,7,1,13,6,14,15,8,9,10,11,16,5,11,16,5,11,11,11,5,11,16,11,11,5,6,6,6,6,6,6,6,6,6,6,
    //    3,3,3,15,3,3,3,3,3,3,1,1,1,7,7,7,1,7,7,7,1,1,1,7,1,7,1,7,7,7,1,1,7,7,1,1,2,2,2,2,2,2,14,15,6,2,2,2,2,2,7,2,2,2,14,2,2,2,15,14,2,2,2,2,14,12,12,8,2,8,8,15,14,2,
    //    8,12,8,14,9,4,4,9,9,9,9,10,10,10,10,10,7,10,13,10,13,10,13,10,10,13,6,14,11,7,3,6,6,1,9,2,2,2,1,1,2,1,1,4,2,2,6,7,7,6,7,10,2,1,1,6,1,1,4,9,16,1,2,8,6,7,11,
    //    16,5,5,4,13,2,8,14,12,14,7,16,11,11,11,4,7,5,7,5,15,9,7,7,7,7,10,2,10,10,13,2,2,6,13,6,3,3,14,15,8,15,15,5,11,9,2,2,2,2,10,7,7,2,2,7,10,2,7,1,5,5,5,1,1 };

    // debug - true
    if (debug) std::cout << "inside runSaveAnnualString, includeColHeader =  " << includeColHeader << "\n";
    
    // create directory for file output
    subDirBase = "./stats/" + statSubScenarioDir + "/job-" + jobNo + "/";
    
    // extend directory if storing stats in job-x/year-x
    if (subYear) {

        subDirBase = subDirBase + "year-" + to_string(year-1) + "/";
    }

    // debug - true
    if (debug) std::cout << "subYear = " << subYear << " subDirBase =  " << subDirBase << "\n";

    // store away infectious days for each year - need to do at last year because infectious days can cross over years
    getInfDays();

    // store away infectious days for active patients for each year - need to do at last year because infectious days can cross over years
    getInfDaysAct();

    if (addTotals) {

        of.str("");

        // get region/district/facilty name
        totalName = regionDistrictArray[0];

        if (addCommas) {

            of << subDirBase << "AgentStats" << ".csv";

            if (includeColHeader)  writeVector(colNamesWCommas, of.str(), true);

            // write out total statistics
            writeVector(returnAnnualOutputStrings(0, 10, 0, 0, totalName, true, currentYearInRun), of.str(), false, true);
        }
        else {

            of << subDirBase << "AgentStats-" << diseaseName << "-without-commas" << ".csv";

            if (includeColHeader)  writeVector(colNames, of.str(), true);

            // write out total statistics
            writeVector(returnAnnualOutputStrings(0, 10, 0, 0, totalName, false, currentYearInRun), of.str(), false);
        }
    }


    // Districts
    // addDistrictAndRegion - false
    if (addDistrictAndRegion) {
    
        of.str("");

        if (addCommas) {
    
            of << subDirBase << "AgentStats-District" << ".csv";

            if (includeColHeader)  writeVector(colNamesWCommas,of.str(), true);

            for (int i = noRegions+1; i < noElementsDistrictRegionArray; i++) {
    
                // extract each district and write out stats
                name = regionDistrictArray[i];

                //std::cout << "name = " << name << ", mapping = " << districtToRegionMapping[i - (noRegions+1)] << ", of.str = " << of.str() << "\n";
                //std::cout << "index = " << i << " name = " << name << ", region = R" << districtToRegionMapping[i - (noRegions+1)] << " district = D" << i - noRegions << "\n";
         
                writeVector(returnAnnualOutputStrings(i, 40, districtToRegionMapping[i-(noRegions+1)], i-noRegions, name, true, currentYearInRun), of.str(), false, true);
            }                                       //int index, int tier, int reg, int dist, string name, bool addCommas, int year
            // write out total statistics
            writeVector(returnAnnualOutputStrings(0, 10, 0, 0, totalName, true, currentYearInRun), of.str(), false, true);
        } 
        else {

            of << subDirBase << "AgentStats-District-" << diseaseName <<  "-without-commas" << ".csv";

            if(includeColHeader)  writeVector(colNames,of.str(),true);

            for (int i = noRegions+1; i < noElementsDistrictRegionArray; i++) {
    
                // extract each district and write out stats
                name = regionDistrictArray[i];
                writeVector(returnAnnualOutputStrings(i, 40, 0, i- noRegions, name, false, currentYearInRun), of.str(), false);
            }
            // write out total statistics
            writeVector(returnAnnualOutputStrings(0, 10, 0, 0, totalName, false, currentYearInRun), of.str(), false);
        }
    }
    
    //Regions
    if (addDistrictAndRegion) {
    
        of.str("");

        if (addCommas) {
    
            of << subDirBase << "AgentStats-Region" << ".csv";

            if (includeColHeader)  writeVector(colNamesWCommas,of.str(), true);

            for (int i = 1; i < noRegions+1; i++) {
    
                // extract each region and write out stats
                name = regionDistrictArray[i];
                writeVector(returnAnnualOutputStrings(i, 30, i, 0, name, true, currentYearInRun), of.str(), false, true);
            }
            // write out total statistics
            writeVector(returnAnnualOutputStrings(0, 10, 0, 0, totalName, true, currentYearInRun), of.str(), false, true);
        } 
        else {

            of << subDirBase << "AgentStats-Region-" << diseaseName <<  "-without-commas" << ".csv";

            writeVector(colNames,of.str(),true);
        
            for (int i = 1; i < noRegions+1; i++) {
    
                // extract each region and write out stats
                name = regionDistrictArray[i];
                writeVector(returnAnnualOutputStrings(i, 30, i, 0, name, false, currentYearInRun), of.str(), false);
            }
            // write out total statistics
            writeVector(returnAnnualOutputStrings(0, 10, 0, 0, totalName, false, currentYearInRun), of.str(), false);
        }
    }
    
    //Facilities
    if (addFacilities) {

        of.str("");

        if (addCommas) {

            of << subDirBase << "AgentStats-Facility" << ".csv";

            if (includeColHeader)  writeVector(colNamesWCommas, of.str(), true);

            for (int i = 1; i < noFacilities + 1; i++) {

                // extract each region and write out stats
                name = facilityArray[i];
                writeVector(returnAnnualOutputStrings(i, 50, i, 0, name, true, currentYearInRun), of.str(), false, true);
            }
            // write out total statistics
            writeVector(returnAnnualOutputStrings(0, 10, 0, 0, totalName, true, currentYearInRun), of.str(), false, true);
        }
        else {

            of << subDirBase << "AgentStats-Facility-" << diseaseName << "-without-commas" << ".csv";

            writeVector(colNames, of.str(), true);

            for (int i = 1; i < noFacilities + 1; i++) {

                // extract each region and write out stats
                name = facilityArray[i];
                writeVector(returnAnnualOutputStrings(i, 50, i, 0, name, false, currentYearInRun), of.str(), false);
            }
            // write out total statistics
            writeVector(returnAnnualOutputStrings(0, 10, 0, 0, totalName, false, currentYearInRun), of.str(), false);
        }
    }

    // debug - true
    if (debug) std::cout << "leaving runSaveAnnualString  " <<  "\n";
}


 //takes the mean of annually recorded outputs
 vector<double> Stats::returnMeanOutputs(int index){
    vector<vector<double>> temp=returnAnnualOutputs(index);
    vector<double> res=vectorAveCols(temp);
    return res;
}

 //returns annually recorded outputs
 vector<vector<double>> Stats::returnAnnualOutputs(int index){

    //vector<string> colNames={"n.patients.enter.new", "n.patients.enter.new.inf","n.patients.enter.ltfu.inf "n.samples.collected", "n.samples.out", "n.samples.inhouse", "ave.sample.resource.wait.time", "ave.sample.resource.wait.time.in.house",
    //                        "ave.sample.resource.wait.time.out", "nTests", "nTestsIn", "nTestsOut", "nTransported", "timeSampleTransport", "nIncDR",
    //                        "timePresentToTestIn", "nPresentToTestIn", "timePresentToTestOut", "nPresentToTestOut", "nTruePosIn", "nTruePosOut",
    //                        "nFalsePosIn", "nFalsePosOut", "nTrueNegIn", "nTrueNegOut",
    //                        "nPostTestDiagnosisTruePosIn","nPostTestDiagnosisTruePosOut", "nPostTestDiagnosisFalseNeg", "nPostTestDiagnosisTrueNeg","nPostTestDiagnosisFalsePosIn","nPostTestDiagnosisFalsePosOut","nPostTestDiagnosisTotal","nPostTestDiagnosisReview","nMDDiagnosisTotal","nMDDiagnosisReview",
    //                        "nPreTestLtFU,",  "nPreTestLtFUDied,", "nPreTestLtFURepresent,", "nPatientsTreated,", "nPatientsTreatedInf,", "nPatientsTreatedUninf,", "nPatientsTreatedCured,", "nPatientsTreatedDied,",
    //                        "nTreatedRelapse,", "nTreatedRelapseCured,", "nTreatedRelapseDied,", "avePresentToSampleDiagIn,", "timePresentToDiagIn,", "nPresentToDiagIn,",
    //                        "avePresentToDiagOut,", "timePresentToDiagOut,", "nPresentToDiagOut,", "avePresentToTreat,", "timePresentToTreat,", "nPresentToTreat,",
    //                        "avePresentToTreatIn,", "timePresentToTreatIn,", "nPresentToTreatIn,", 
    //                        "avePresentToTreatOut,", "timePresentToTreatOut,", "nPresentToTreatOut,", "avePresentToDeath,", "timePresentToDeath,", "nPresentToDeath,", 
    //                        "avePresentToDeathPreTestLTFU,", "timePresentToDeathPreTestLTFU,", "nPresentToDeathPreTestLTFU,", 
    //                        "avePresentToDeathInTreat,", "timePresentToDeathInTreat,", "nPresentToDeathInTreat,", "avePresentToDeathInRelapse,", "timePresentToDeathInRelapse,", "nPresentToDeathInRelapse,", 
    //                        "avePresentToExit,", "timePresentToExit,", "nPresentToExit,", "nXpert,", "nXpertIn,", "nXpertOut,", "nXpertTruePos,", "nXpertFalsePos,", "nXpertTrueNeg,", "nXpertFalseNeg,", 
    //                        "nSmear,", "nSmearIn,", "nSmearOut,", "nSmearTruePos,", "nSmearFalsePos,", "nSmearTrueNeg,", "nSmearFalseNeg" };

    
    vector<vector<double>> temp;
    //std::cout << "inside returnAnnualOutputs  " <<  "\n";

    //temp.push_back(dis[0]->_stats->return_vNPatient());
    temp.push_back(return_vNPatient(index));
    temp.push_back(return_vNPatient(index));
    temp.push_back(return_vNPatient(index));
    temp.push_back(return_vNXpertMach(index));
    temp.push_back(return_vNPatient(index));
    temp.push_back(return_vNPatient(index));
    temp.push_back(return_vNPatient(index));
    temp.push_back(return_vNPatient(index));
    temp.push_back(return_vNPatient(index));
    temp.push_back(return_vNPopulation(index));
    temp.push_back(return_vNPatient(index));
    temp.push_back(return_vNCalcIncidence(index));
    temp.push_back(return_vNIncidence(index));
    temp.push_back(return_vNPatient(index));
    temp.push_back(return_vNPatientInf(index));
    temp.push_back(return_vNPatientLtFUInf(index));
    temp.push_back(return_vNPatientFromTrans(index));
    temp.push_back(return_vNPatientNotPresenting(index));
    temp.push_back(return_vNSamples(index));
    temp.push_back(return_vNSamplesOut(index));
    temp.push_back(return_vNSamplesIn(index));
    temp.push_back(return_vAveSampleResourceWait(index));
    temp.push_back(return_vAveSampleResourceWaitIn(index));
    temp.push_back(return_vAveSampleResourceWaitOut(index));
    temp.push_back(return_vAveSampleTransport(index));
    temp.push_back(return_vTimeSampleTransport(index));
    temp.push_back(return_vNTransported(index));
    temp.push_back(return_vNTests(index));
    temp.push_back(return_vNTestsIn(index));
    temp.push_back(return_vNTestsOut(index));
    temp.push_back(return_vAvePresentToSampleTestIn(index));
    temp.push_back(return_vTimePresentToSampleTestIn(index));
    temp.push_back(return_vNPresentToSampleTestIn(index));
    temp.push_back(return_vAvePresentToSampleTestOut(index));
    temp.push_back(return_vTimePresentToSampleTestOut(index));
    temp.push_back(return_vNPresentToSampleTestOut(index));
    temp.push_back(return_vNTestPos(index));
    temp.push_back(return_vPTestTruePos(index));
    temp.push_back(return_vNTestTruePosIn(index));
    temp.push_back(return_vNTestTruePosOut(index));
    temp.push_back(return_vPTestFalsePos(index));
    temp.push_back(return_vNTestFalsePosIn(index));
    temp.push_back(return_vNTestFalsePosOut(index));
    temp.push_back(return_vNTestNeg(index));
    temp.push_back(return_vPTestTrueNeg(index));
    temp.push_back(return_vNTestTrueNegIn(index));
    temp.push_back(return_vNTestTrueNegOut(index));
    temp.push_back(return_vPTestFalseNeg(index));
    temp.push_back(return_vNTestFalseNegIn(index));
    temp.push_back(return_vNTestFalseNegOut(index));
    temp.push_back(return_vNTestFalsePos(index));
    temp.push_back(return_vNTestFalseNeg(index));
    temp.push_back(return_vNMDCDInfected(index));
    temp.push_back(return_vNMDCDUninfected(index));
    temp.push_back(return_vNTestPos(index));
    temp.push_back(return_vNDiagnosis(index));
    temp.push_back(return_vNDiagnosisIn(index));
    temp.push_back(return_vNDiagnosisOut(index));
    temp.push_back(return_vNDiagnosisDC(index));
    temp.push_back(return_vNDiagnosisDCTruePos(index));
    temp.push_back(return_vNDiagnosisDCFalsePos(index));
    temp.push_back(return_vNPreTestDiagnosisTruePos(index));
    temp.push_back(return_vNPreTestDiagnosisFalsePos(index));
    temp.push_back(return_vNPreTestDiagnosisTotal(index));
    temp.push_back(return_vNPostTestDiagnosisTruePosIn(index));
    temp.push_back(return_vNPostTestDiagnosisTruePosOut(index));
    temp.push_back(return_vNPostTestDiagnosisFalseNeg(index));
    temp.push_back(return_vNPostTestDiagnosisTrueNeg(index));
    temp.push_back(return_vNPostTestDiagnosisFalsePosIn(index));
    temp.push_back(return_vNPostTestDiagnosisFalsePosOut(index));
    temp.push_back(return_vNPostTestDiagnosisTotal(index));
    temp.push_back(return_vNPostTestDiagnosisReview(index));
    temp.push_back(return_vNMDDiagnosisTotal(index));
    temp.push_back(return_vNMDDiagnosisReview(index));
    temp.push_back(return_vNPreTestLtFU(index));
    temp.push_back(return_vNPreTestLtFUInf(index));
    temp.push_back(return_vNPreTestLtFUDied(index));
    temp.push_back(return_vNPreTestLtFUCured(index));
    temp.push_back(return_vNPreTestLtFURepresent(index));
    temp.push_back(return_vNPreTreatLtFU(index));
    //temp.push_back(return_vNPatientsPreTreatedLtoFUninf(index));
    temp.push_back(return_vNPreTreatLtFUInf(index));
    temp.push_back(return_vNPreTreatLtFUDied(index));
    temp.push_back(return_vNPreTreatLtFUCured(index));
    temp.push_back(return_vNPreTreatLtFURepresent(index));
    temp.push_back(return_vNPreTreatLtFUFN(index));
    temp.push_back(return_vNPreTreatLtFUFNDied(index));
    temp.push_back(return_vNPreTreatLtFUFNCured(index));
    temp.push_back(return_vNPreTreatLtFUFNRepresent(index));
    //temp.push_back(return_vNPreTreatLtFUPos(index));
    //temp.push_back(return_vNPreTreatLtFUNeg(index));
    temp.push_back(return_vNTreatLtFU(index));
    temp.push_back(return_vNTreatLtFUInf(index));
    temp.push_back(return_vNTreatLtFUDied(index));
    temp.push_back(return_vNTreatLtFUCured(index));
    temp.push_back(return_vNTreatLtFURepresent(index));
    temp.push_back(return_vAvePresentToSampleDiagIn(index));
    temp.push_back(return_vTimePresentToSampleDiagIn(index));
    temp.push_back(return_vNPresentToSampleDiagIn(index));
    temp.push_back(return_vAvePresentToSampleDiagOut(index));
    temp.push_back(return_vTimePresentToSampleDiagOut(index));
    temp.push_back(return_vNPresentToSampleDiagOut(index));
    temp.push_back(return_vNPatientsTreated(index));
    temp.push_back(return_vNPatientsTreatedInf(index));
    temp.push_back(return_vNPatientsTreatedUninf(index));
    temp.push_back(return_vNPatientsTreatedCured(index));
    temp.push_back(return_vNPatientsTreatedDied(index));
    temp.push_back(return_vNPatientsTreatedRelapse(index));
    temp.push_back(return_vNTreatedRelapse(index));
    temp.push_back(return_vNTreatedRelapseCured(index));
    temp.push_back(return_vNTreatedRelapseDied(index));
    temp.push_back(return_vAvePresentToSampleTreat(index));
    temp.push_back(return_vTimePresentToSampleTreat(index));
    temp.push_back(return_vNPresentToSampleTreat(index));
    temp.push_back(return_vAvePresentToSampleTreatIn(index));
    temp.push_back(return_vTimePresentToSampleTreatIn(index));
    temp.push_back(return_vNPresentToSampleTreatIn(index));
    temp.push_back(return_vAvePresentToSampleTreatOut(index));
    temp.push_back(return_vTimePresentToSampleTreatOut(index));
    temp.push_back(return_vNPresentToSampleTreatOut(index));
    temp.push_back(return_vAvePresentToSampleDeath(index));
    temp.push_back(return_vTimePresentToSampleDeath(index));
    temp.push_back(return_vNPresentToSampleDeath(index));
    temp.push_back(return_vAvePresentToSampleDeathPreTestLTFU(index));
    temp.push_back(return_vTimePresentToSampleDeathPreTestLTFU(index));
    temp.push_back(return_vNPresentToSampleDeathPreTestLTFU(index));
    temp.push_back(return_vAvePresentToSampleDeathPreTreatLTFU(index));
    temp.push_back(return_vTimePresentToSampleDeathPreTreatLTFU(index));
    temp.push_back(return_vNPresentToSampleDeathPreTreatLTFU(index));
    temp.push_back(return_vAvePresentToSampleDeathTreatLTFU(index));
    temp.push_back(return_vTimePresentToSampleDeathTreatLTFU(index));
    temp.push_back(return_vNPresentToSampleDeathTreatLTFU(index));
    temp.push_back(return_vAvePresentToSampleDeathInTreat(index));
    temp.push_back(return_vTimePresentToSampleDeathInTreat(index));
    temp.push_back(return_vNPresentToSampleDeathInTreat(index));
    temp.push_back(return_vAvePresentToSampleDeathInRelapse(index));
    temp.push_back(return_vTimePresentToSampleDeathInRelapse(index));
    temp.push_back(return_vNPresentToSampleDeathInRelapse(index));
    temp.push_back(return_vAvePresentToExit(index));
    temp.push_back(return_vTimePresentToExit(index));
    temp.push_back(return_vNPresentToExit(index));
    temp.push_back(return_vNXpert(index));
    temp.push_back(return_vNXpertIn(index));
    temp.push_back(return_vNXpertOut(index));
    temp.push_back(return_vNSmear(index));
    temp.push_back(return_vNSmearPos(index));
    temp.push_back(return_vPSmearTruePos(index));
    temp.push_back(return_vNSmearNeg(index));
    temp.push_back(return_vPSmearTrueNeg(index));
    temp.push_back(return_vNXpertPos(index));
    temp.push_back(return_vPXpertTruePos(index));
    temp.push_back(return_vNXpertNeg(index));
    temp.push_back(return_vPXpertTrueNeg(index));
    temp.push_back(return_vNDeathReTreat(index));
    temp.push_back(return_vNDeathUntreat(index)); 
    temp.push_back(return_vNPatientInfDays(index));
    temp.push_back(return_vNPatientInfDaysAct(index));
    temp.push_back(return_vSPatientWaitingIn(index));
    temp.push_back(return_vSPatientWaitingOut(index));
    temp.push_back(return_vSPatientInTreatment(index));
    temp.push_back(return_vSSampleWaitingForTest(index));
    temp.push_back(return_vSSampleWaitingForTestIn(index));
    temp.push_back(return_vSSampleWaitingForTestOut(index));
    
    //std::cout << "inside runSaveAnnualString before transpose " <<  "\n";
    
    //
    vector<vector<double>> res=vectorTranspose(temp);

    //std::cout << "inside leaving returnAnnualOutputs" <<  "\n";

    return res;

}



//returns annually recorded outputs as strings
vector<vector<string>> Stats::returnAnnualOutputStrings(int index, int tier, int reg, int dist, string name, bool addCommas, int yearInRun){

    // vector<string> colNames={"nPatient nPatientInf nSamples nSamplesIn nSamplesOut timeResourceWait timeResourceWaitIn
    //                          timeResourceWaitOut nTests nTestsIn nTestsOut nTransported timeSampleTransport nIncDR timePresentToTestIn
    //                          nPresentToTestIn timePresentToTestOut nPresentToTestOut nTruePosIn nTruePosOut nFalsePosIn nFalsePosOut
    //                          nTrueNegIn nTrueNegOut nFalseNegIn nFalseNegOut nTestPosIn nTestPosOut  nPostTestDiagnosisTruePosIn nPostTestDiagnosisTruePosOut
    //                          nPostTestDiagnosisFalseNeg nPostTestDiagnosisTrueNeg nPostTestDiagnosisFalsePosIn nPostTestDiagnosisFalsePosOut nPostTestDiagnosisTotal nPostTestDiagnosisReview nMDDiagnosisTotal nMDDiagnosisReview"};
    
    vector<vector<string>> temp;

    vector<string> myName(__vNPatient.size(), name);
    vector<string> myId(__vNPatient.size(), "0");
    vector<string> myTier(__vNPatient.size(), to_string(tier));
    //vector<ostringstream> mySeed(__vNPatient.size(), to_string(seed));
    vector<string> mySeed(__vNPatient.size(), to_string(seed));
    vector<string> myDistrict(__vNPatient.size(), "D" + to_string(dist));
    vector<string> myRegion(__vNPatient.size(), "R" + to_string(reg));
    vector<string> myComma(__vNPatient.size(), ",");

    //std::cout << "inside leaving returnAnnualOutputString no Years in run = " << yearsInRun << " skip years = " << skipYears << "\n";

    //temp.push_back(dis[0]->_stats->return_vNPatient());
    temp.push_back(myName);
    if (addCommas) temp.push_back(myComma);
    temp.push_back(myId);
    if (addCommas) temp.push_back(myComma);
    temp.push_back(myTier);
    if (addCommas) temp.push_back(myComma);
    temp.push_back(returns_vNXpertMach(index));
    if (addCommas) temp.push_back(myComma);
    temp.push_back(returns_vYear(index));
    if (addCommas) temp.push_back(myComma);
    temp.push_back(returns_vScenario(index));
    if (addCommas) temp.push_back(myComma);
    temp.push_back(myDistrict);
    if (addCommas) temp.push_back(myComma);
    temp.push_back(myRegion);
    if (addCommas) temp.push_back(myComma);
    temp.push_back(returns_vNPopulation(index));
    if (addCommas) temp.push_back(myComma);
    temp.push_back(mySeed);
    if (addCommas) temp.push_back(myComma);
    temp.push_back(returns_vDisease(index));
    if (addCommas) temp.push_back(myComma);
    temp.push_back(returns_vNCalcIncidence(index));
    if (addCommas) temp.push_back(myComma);
    temp.push_back(returns_vNIncidence(index));
    if (addCommas) temp.push_back(myComma);
    temp.push_back(returns_vNPatient(index));
    if (addCommas) temp.push_back(myComma);
    temp.push_back(returns_vNPatientInf(index));
    if (addCommas) temp.push_back(myComma);
    temp.push_back(returns_vNPatientLtFUInf(index));
    if (addCommas) temp.push_back(myComma);
    temp.push_back(returns_vNPatientFromTrans(index));
    if (addCommas) temp.push_back(myComma);
    temp.push_back(returns_vNPatientNotPresenting(index));
    if (addCommas) temp.push_back(myComma);
    temp.push_back(returns_vNSamples(index));
    if (addCommas) temp.push_back(myComma);
    temp.push_back(returns_vNSamplesOut(index));
    if (addCommas) temp.push_back(myComma);
    temp.push_back(returns_vNSamplesIn(index));
    if (addCommas) temp.push_back(myComma);
    temp.push_back(returns_vAveSampleResourceWait(index));
    if (addCommas) temp.push_back(myComma);
    temp.push_back(returns_vAveSampleResourceWaitIn(index));
    if (addCommas) temp.push_back(myComma);
    temp.push_back(returns_vAveSampleResourceWaitOut(index));
    if (addCommas) temp.push_back(myComma);
    temp.push_back(returns_vAveSampleTransport(index));
    if (addCommas) temp.push_back(myComma);
    temp.push_back(returns_vTimeSampleTransport(index));
    if (addCommas) temp.push_back(myComma);
    temp.push_back(returns_vNTransported(index));
    if (addCommas) temp.push_back(myComma);
    temp.push_back(returns_vNTests(index));
    if (addCommas) temp.push_back(myComma);
    temp.push_back(returns_vNTestsIn(index));
    if (addCommas) temp.push_back(myComma);
    temp.push_back(returns_vNTestsOut(index));
    if (addCommas) temp.push_back(myComma);
    temp.push_back(returns_vAvePresentToSampleTestIn(index));
    if (addCommas) temp.push_back(myComma);
    temp.push_back(returns_vTimePresentToSampleTestIn(index));
    if (addCommas) temp.push_back(myComma);
    temp.push_back(returns_vNPresentToSampleTestIn(index));
    if (addCommas) temp.push_back(myComma);
    temp.push_back(returns_vAvePresentToSampleTestOut(index));
    if (addCommas) temp.push_back(myComma);
    temp.push_back(returns_vTimePresentToSampleTestOut(index));
    if (addCommas) temp.push_back(myComma);
    temp.push_back(returns_vNPresentToSampleTestOut(index));
    if (addCommas) temp.push_back(myComma);
    temp.push_back(returns_vNTestPos(index));
    if (addCommas) temp.push_back(myComma);
    temp.push_back(returns_vPTestTruePos(index));
    if (addCommas) temp.push_back(myComma);
    temp.push_back(returns_vNTestTruePosIn(index));
    if (addCommas) temp.push_back(myComma);
    temp.push_back(returns_vNTestTruePosOut(index));
    if (addCommas) temp.push_back(myComma);
    temp.push_back(returns_vPTestFalsePos(index));
    if (addCommas) temp.push_back(myComma);
    temp.push_back(returns_vNTestFalsePosIn(index));
    if (addCommas) temp.push_back(myComma);
    temp.push_back(returns_vNTestFalsePosOut(index));
    if (addCommas) temp.push_back(myComma);
    temp.push_back(returns_vNTestNeg(index));
    if (addCommas) temp.push_back(myComma);
    temp.push_back(returns_vPTestTrueNeg(index));
    if (addCommas) temp.push_back(myComma);
    temp.push_back(returns_vNTestTrueNegIn(index));
    if (addCommas) temp.push_back(myComma);
    temp.push_back(returns_vNTestTrueNegOut(index));
    if (addCommas) temp.push_back(myComma);
    temp.push_back(returns_vPTestFalseNeg(index));
    if (addCommas) temp.push_back(myComma);
    temp.push_back(returns_vNTestFalseNegIn(index));
    if (addCommas) temp.push_back(myComma);
    temp.push_back(returns_vNTestFalseNegOut(index));
    if (addCommas) temp.push_back(myComma);
    temp.push_back(returns_vNTestFalsePos(index));
    if (addCommas) temp.push_back(myComma);
    temp.push_back(returns_vNTestFalseNeg(index));
    if (addCommas) temp.push_back(myComma);
    temp.push_back(returns_vNMDCDInfected(index));
    if (addCommas) temp.push_back(myComma);
    temp.push_back(returns_vNMDCDUninfected(index));
    if (addCommas) temp.push_back(myComma);
    temp.push_back(returns_vNTestPos(index));
    if (addCommas) temp.push_back(myComma);
    temp.push_back(returns_vNDiagnosis(index));
    if (addCommas) temp.push_back(myComma);
    temp.push_back(returns_vNDiagnosisIn(index));
    if (addCommas) temp.push_back(myComma);
    temp.push_back(returns_vNDiagnosisOut(index));
    if (addCommas) temp.push_back(myComma);
    temp.push_back(returns_vNDiagnosisDC(index));
    if (addCommas) temp.push_back(myComma);
    temp.push_back(returns_vNDiagnosisDCTruePos(index));
    if (addCommas) temp.push_back(myComma);
    temp.push_back(returns_vNDiagnosisDCFalsePos(index));
    if (addCommas) temp.push_back(myComma);
    temp.push_back(returns_vNPreTestDiagnosisTruePos(index));
    if (addCommas) temp.push_back(myComma);
    temp.push_back(returns_vNPreTestDiagnosisFalsePos(index));
    if (addCommas) temp.push_back(myComma);
    temp.push_back(returns_vNPreTestDiagnosisTotal(index));
    if (addCommas) temp.push_back(myComma);
    temp.push_back(returns_vNPostTestDiagnosisTruePosIn(index));
    if (addCommas) temp.push_back(myComma);
    temp.push_back(returns_vNPostTestDiagnosisTruePosOut(index));
    if (addCommas) temp.push_back(myComma);
    temp.push_back(returns_vNPostTestDiagnosisFalseNeg(index));
    if (addCommas) temp.push_back(myComma);
    temp.push_back(returns_vNPostTestDiagnosisTrueNeg(index));
    if (addCommas) temp.push_back(myComma);
    temp.push_back(returns_vNPostTestDiagnosisFalsePosIn(index));
    if (addCommas) temp.push_back(myComma);
    temp.push_back(returns_vNPostTestDiagnosisFalsePosOut(index));
    if (addCommas) temp.push_back(myComma);
    temp.push_back(returns_vNPostTestDiagnosisTotal(index));
    if (addCommas) temp.push_back(myComma);
    temp.push_back(returns_vNPostTestDiagnosisReview(index));
    if (addCommas) temp.push_back(myComma);
    temp.push_back(returns_vNMDDiagnosisTotal(index));
    if (addCommas) temp.push_back(myComma);
    temp.push_back(returns_vNMDDiagnosisReview(index));
    if (addCommas) temp.push_back(myComma);
    temp.push_back(returns_vNPreTestLtFU(index));
    if (addCommas) temp.push_back(myComma);
    temp.push_back(returns_vNPreTestLtFUInf(index));
    if (addCommas) temp.push_back(myComma);
    temp.push_back(returns_vNPreTestLtFUDied(index));
    if (addCommas) temp.push_back(myComma);
    temp.push_back(returns_vNPreTestLtFUCured(index));
    if (addCommas) temp.push_back(myComma);
    temp.push_back(returns_vNPreTestLtFURepresent(index));
    if (addCommas) temp.push_back(myComma);
    temp.push_back(returns_vNPreTreatLtFU(index));
    if (addCommas) temp.push_back(myComma);
    //temp.push_back(returns_vNPatientsPreTreatedLtoFUninf(index));
    //if (addCommas) temp.push_back(myComma);
    temp.push_back(returns_vNPreTreatLtFUInf(index));
    if (addCommas) temp.push_back(myComma);
    temp.push_back(returns_vNPreTreatLtFUDied(index));
    if (addCommas) temp.push_back(myComma);
    temp.push_back(returns_vNPreTreatLtFUCured(index));
    if (addCommas) temp.push_back(myComma);
    temp.push_back(returns_vNPreTreatLtFURepresent(index));
    if (addCommas) temp.push_back(myComma);
    temp.push_back(returns_vNPreTreatLtFUFN(index));
    if (addCommas) temp.push_back(myComma);
    temp.push_back(returns_vNPreTreatLtFUFNDied(index));
    if (addCommas) temp.push_back(myComma);
    temp.push_back(returns_vNPreTreatLtFUFNCured(index));
    if (addCommas) temp.push_back(myComma);
    temp.push_back(returns_vNPreTreatLtFUFNRepresent(index));
    if (addCommas) temp.push_back(myComma);
    //temp.push_back(returns_vNPreTreatLtFUPos(index));
    //if (addCommas) temp.push_back(myComma);
    //temp.push_back(returns_vNPreTreatLtFUNeg(index));
    //if (addCommas) temp.push_back(myComma);
    temp.push_back(returns_vNTreatLtFU(index));
    if (addCommas) temp.push_back(myComma);
    temp.push_back(returns_vNTreatLtFUInf(index));
    if (addCommas) temp.push_back(myComma);
    temp.push_back(returns_vNTreatLtFUDied(index));
    if (addCommas) temp.push_back(myComma);
    temp.push_back(returns_vNTreatLtFUCured(index));
    if (addCommas) temp.push_back(myComma);
    temp.push_back(returns_vNTreatLtFURepresent(index));
    if (addCommas) temp.push_back(myComma);
    temp.push_back(returns_vAvePresentToSampleDiagIn(index));
    if (addCommas) temp.push_back(myComma);
    temp.push_back(returns_vTimePresentToSampleDiagIn(index));
    if (addCommas) temp.push_back(myComma);
    temp.push_back(returns_vNPresentToSampleDiagIn(index));
    if (addCommas) temp.push_back(myComma);
    temp.push_back(returns_vAvePresentToSampleDiagOut(index));
    if (addCommas) temp.push_back(myComma);
    temp.push_back(returns_vTimePresentToSampleDiagOut(index));
    if (addCommas) temp.push_back(myComma);
    temp.push_back(returns_vNPresentToSampleDiagOut(index));
    if (addCommas) temp.push_back(myComma);
    temp.push_back(returns_vNPatientsTreated(index));
    if (addCommas) temp.push_back(myComma);
    temp.push_back(returns_vNPatientsTreatedInf(index));
    if (addCommas) temp.push_back(myComma);
    temp.push_back(returns_vNPatientsTreatedUninf(index));
    if (addCommas) temp.push_back(myComma);
    temp.push_back(returns_vNPatientsTreatedCured(index));
    if (addCommas) temp.push_back(myComma);
    temp.push_back(returns_vNPatientsTreatedDied(index));
    if (addCommas) temp.push_back(myComma);
    temp.push_back(returns_vNPatientsTreatedRelapse(index));
    if (addCommas) temp.push_back(myComma);
    temp.push_back(returns_vNTreatedRelapse(index));
    if (addCommas) temp.push_back(myComma);
    temp.push_back(returns_vNTreatedRelapseCured(index));
    if (addCommas) temp.push_back(myComma);
    temp.push_back(returns_vNTreatedRelapseDied(index));
    if (addCommas) temp.push_back(myComma);
    temp.push_back(returns_vAvePresentToSampleTreat(index));
    if (addCommas) temp.push_back(myComma);
    temp.push_back(returns_vTimePresentToSampleTreat(index));
    if (addCommas) temp.push_back(myComma);
    temp.push_back(returns_vNPresentToSampleTreat(index));
    if (addCommas) temp.push_back(myComma);
    temp.push_back(returns_vAvePresentToSampleTreatIn(index));
    if (addCommas) temp.push_back(myComma);
    temp.push_back(returns_vTimePresentToSampleTreatIn(index));
    if (addCommas) temp.push_back(myComma);
    temp.push_back(returns_vNPresentToSampleTreatIn(index));
    if (addCommas) temp.push_back(myComma);
    temp.push_back(returns_vAvePresentToSampleTreatOut(index));
    if (addCommas) temp.push_back(myComma);
    temp.push_back(returns_vTimePresentToSampleTreatOut(index));
    if (addCommas) temp.push_back(myComma);
    temp.push_back(returns_vNPresentToSampleTreatOut(index));
    if (addCommas) temp.push_back(myComma);
    temp.push_back(returns_vAvePresentToSampleDeath(index));
    if (addCommas) temp.push_back(myComma);
    temp.push_back(returns_vTimePresentToSampleDeath(index));
    if (addCommas) temp.push_back(myComma);
    temp.push_back(returns_vNPresentToSampleDeath(index));
    if (addCommas) temp.push_back(myComma);
    temp.push_back(returns_vAvePresentToSampleDeathPreTestLTFU(index));
    if (addCommas) temp.push_back(myComma);
    temp.push_back(returns_vTimePresentToSampleDeathPreTestLTFU(index));
    if (addCommas) temp.push_back(myComma);
    temp.push_back(returns_vNPresentToSampleDeathPreTestLTFU(index));
    if (addCommas) temp.push_back(myComma);
    temp.push_back(returns_vAvePresentToSampleDeathPreTreatLTFU(index));
    if (addCommas) temp.push_back(myComma);
    temp.push_back(returns_vTimePresentToSampleDeathPreTreatLTFU(index));
    if (addCommas) temp.push_back(myComma);
    temp.push_back(returns_vNPresentToSampleDeathPreTreatLTFU(index));
    if (addCommas) temp.push_back(myComma);
    temp.push_back(returns_vAvePresentToSampleDeathTreatLTFU(index));
    if (addCommas) temp.push_back(myComma);
    temp.push_back(returns_vTimePresentToSampleDeathTreatLTFU(index));
    if (addCommas) temp.push_back(myComma);
    temp.push_back(returns_vNPresentToSampleDeathTreatLTFU(index));
    if (addCommas) temp.push_back(myComma);
    temp.push_back(returns_vAvePresentToSampleDeathInTreat(index));
    if (addCommas) temp.push_back(myComma);
    temp.push_back(returns_vTimePresentToSampleDeathInTreat(index));
    if (addCommas) temp.push_back(myComma);
    temp.push_back(returns_vNPresentToSampleDeathInTreat(index));
    if (addCommas) temp.push_back(myComma);
    temp.push_back(returns_vAvePresentToSampleDeathInRelapse(index));
    if (addCommas) temp.push_back(myComma);
    temp.push_back(returns_vTimePresentToSampleDeathInRelapse(index));
    if (addCommas) temp.push_back(myComma);
    temp.push_back(returns_vNPresentToSampleDeathInRelapse(index));
    if (addCommas) temp.push_back(myComma);
    temp.push_back(returns_vAvePresentToExit(index));
    if (addCommas) temp.push_back(myComma);
    temp.push_back(returns_vTimePresentToExit(index));
    if (addCommas) temp.push_back(myComma);
    temp.push_back(returns_vNPresentToExit(index));
    if (addCommas) temp.push_back(myComma);
    temp.push_back(returns_vNXpert(index));
    if (addCommas) temp.push_back(myComma);
    temp.push_back(returns_vNXpertIn(index));
    if (addCommas) temp.push_back(myComma);
    temp.push_back(returns_vNXpertOut(index));
    if (addCommas) temp.push_back(myComma);
    temp.push_back(returns_vNSmear(index));
    if (addCommas) temp.push_back(myComma);
    temp.push_back(returns_vNSmearPos(index));
    if (addCommas) temp.push_back(myComma);
    temp.push_back(returns_vPSmearTruePos(index));
    if (addCommas) temp.push_back(myComma);
    temp.push_back(returns_vNSmearNeg(index));
    if (addCommas) temp.push_back(myComma);
    temp.push_back(returns_vPSmearTrueNeg(index));
    if (addCommas) temp.push_back(myComma);
    temp.push_back(returns_vNXpertPos(index));
    if (addCommas) temp.push_back(myComma);
    temp.push_back(returns_vPXpertTruePos(index));
    if (addCommas) temp.push_back(myComma);
    temp.push_back(returns_vNXpertNeg(index));
    if (addCommas) temp.push_back(myComma);
    temp.push_back(returns_vPXpertTrueNeg(index));
    if (addCommas) temp.push_back(myComma);
    temp.push_back(returns_vNDeathReTreat(index));
    if (addCommas) temp.push_back(myComma);
    temp.push_back(returns_vNDeathUntreat(index));
    if (addCommas) temp.push_back(myComma);
    temp.push_back(returns_vNPatientInfDays(index));
    if (addCommas) temp.push_back(myComma);
    temp.push_back(returns_vNPatientInfDaysAct(index));
    if (addCommas) temp.push_back(myComma);
    temp.push_back(returns_vSPatientWaitingIn(index));
    if (addCommas) temp.push_back(myComma);
    temp.push_back(returns_vSPatientWaitingOut(index));
    if (addCommas) temp.push_back(myComma);
    temp.push_back(returns_vSPatientInTreatment(index));
    if (addCommas) temp.push_back(myComma);
    temp.push_back(returns_vSSampleWaitingForTest(index));
    if (addCommas) temp.push_back(myComma);
    temp.push_back(returns_vSSampleWaitingForTestIn(index));
    if (addCommas) temp.push_back(myComma);
    temp.push_back(returns_vSSampleWaitingForTestOut(index));
    
    //std::cout << "inside leaving returnAnnualOutputStrings before vector transpose" <<  "\n";
    
    //
    vector<vector<string>> res=vectorTranspose(temp);

    //std::cout << "inside leaving returnAnnualOutputStrings" <<  "\n";

    return res;

}
