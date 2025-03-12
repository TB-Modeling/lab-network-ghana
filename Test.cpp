//
//  Test.cpp
//  mdrtb
//
//  Created by L. Dombrowski on 3/17/2020.

#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include "Test.h"
//#include "Functions.h"


using namespace std;
//using namespace GLOBALPARAMS;
//using namespace FUNCTIONS;


Test::Test() {
};



Test::Test(string name, string disease, int testsPerMachine, int cyclesPerTimestep, int maxTestPerTimestep, int maxTestPerCycle, int reagentCycle, int priority, int batchWaitTime, int patientReturnTime, float sensitivity, float specificity, float sensitivityMD, float specificityMD, float sensitivityWithTestMD, float specificityWithTestMD, bool tier0, bool tier1, bool tier2, bool tier3, bool tier4, bool tier5, bool tier6, bool tier7, bool tier8, bool tier9) {


    // store away information on creation
    _name = name;
    _disease = disease;
    _testsPerMachine = testsPerMachine;
    _cyclesPerTimestep = cyclesPerTimestep;
    _maxTestPerTimestep = maxTestPerTimestep;
    _maxTestsAllTimesteps = 0;
    _maxTestPerCycle = maxTestPerCycle;
    _maxTestsAllCycles = 0;
    _reagentCycle = reagentCycle;
    _priority = priority;
    _batchWaitTime = batchWaitTime;
    _patientReturnTime = patientReturnTime;
    _sensitivity = sensitivity;
    _specificity = specificity;
    _sensitivityMD = sensitivityMD;
    _specificityMD = specificityMD;
    _sensitivityWithTestMD = sensitivityWithTestMD;
    _specificityWithTestMD = specificityWithTestMD;
    _tier0 = tier0;
    _tier1 = tier1;
    _tier2 = tier2;
    _tier3 = tier3;
    _tier4 = tier4;
    _tier5 = tier5;
    _tier6 = tier6;
    _tier7 = tier7;
    _tier8 = tier8;
    _tier9 = tier9;
    _noMachines = 1;                        // assume 1 machine per facility unless overrode by facility specific file
    _testsThisTimestep = 0;
    _testsThisReagentCycle = 0;
    _totalTests = 0;

    //std::cout << name << ' ' << disease << ' ' << maxTestPerTimestep << ' ' << maxTestPerCycle << ' ' << reagentCycle << ' ' << priority << ' ' << sensitivity << ' ' << specificity << ' ' << sensitivityMD << ' ' << specificityMD << '\n';

}

Test::Test(const Test& oldTest) {

    // get information from old test agent and store in current test agent

    // store away information on creation
    _name = oldTest._name;
    _disease = oldTest._disease;
    _maxTestPerTimestep = oldTest._maxTestPerTimestep;
    _maxTestsAllTimesteps = 0;
    _maxTestPerCycle = oldTest._maxTestPerCycle;
    _maxTestsAllCycles = 0;
    _reagentCycle = oldTest._reagentCycle;
    _priority = oldTest._priority;
    _batchWaitTime = oldTest._batchWaitTime;
    _sensitivity = oldTest._sensitivity;
    _specificity = oldTest._specificity;
    _sensitivityMD = oldTest._sensitivityMD;
    _specificityMD = oldTest._specificityMD;
    _sensitivityWithTestMD = oldTest._sensitivityWithTestMD;
    _specificityWithTestMD = oldTest._specificityWithTestMD;
    _tier0 = oldTest._tier0;
    _tier1 = oldTest._tier1;
    _tier2 = oldTest._tier2;
    _tier3 = oldTest._tier3;
    _tier4 = oldTest._tier4;
    _tier5 = oldTest._tier5;
    _tier6 = oldTest._tier6;
    _tier7 = oldTest._tier7;
    _tier8 = oldTest._tier8;
    _tier9 = oldTest._tier9;
    _noMachines = oldTest._noMachines;
    _testsThisTimestep = 0;
    _testsThisReagentCycle = 0;
    _totalTests = 0;

}


void Test::copyTest(Test* oldTest) {

    // get information from old test agent and store in current test agent

    // store away information on creation
    _name = oldTest->getTestName();
    _disease = oldTest->getDiseaseName();
    _maxTestPerTimestep = oldTest->getMaxTestPerTimestep();
    _maxTestsAllTimesteps = 0;
    _maxTestPerCycle = oldTest->getMaxTestPerCycle();
    _maxTestsAllCycles = 0;
    _reagentCycle = oldTest->getReagentCycle();
    _priority = oldTest->getPriority();
    _batchWaitTime = oldTest->getBatchWaitTime();
    _sensitivity = oldTest->getSense();
    _specificity = oldTest->getSpecif();
    _sensitivityMD = oldTest->getSenseMD();
    _specificityMD = oldTest->getSpecifMD();
    _sensitivityWithTestMD = oldTest->getSenseWithTestMD();
    _specificityWithTestMD = oldTest->getSpecifWithTestMD();
    _tier0 = oldTest->getTier0();
    _tier1 = oldTest->getTier1();
    _tier2 = oldTest->getTier2();
    _tier3 = oldTest->getTier3();
    _tier4 = oldTest->getTier4();
    _tier5 = oldTest->getTier5();
    _tier6 = oldTest->getTier6();
    _tier7 = oldTest->getTier7();
    _tier8 = oldTest->getTier8();
    _tier9 = oldTest->getTier9();
    _noMachines = oldTest->getNoMachines();
    _testsThisTimestep = 0;
    _testsThisReagentCycle = 0;
    _totalTests = 0;

}


Test::~Test() {
    //cout<<"Removing Test class"<<_id<<endl;
}
//---------------------------------------




