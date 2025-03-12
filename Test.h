//
//  Test.h
//  
//
//  Created by L. Dombrowski on 3/17/2020.
//

#ifndef TEST_H
#define TEST_H

#include <string>

using namespace std;


class Test {
private:

    string _name;       // name of test

    string _disease;
    
    // Priority for running test - relative to other disease tests
    int _priority;

    //testing for this test done here
    bool _testing;
    
    // how many machines in for this test
    int _noMachines;

    /// no. of tests that can be done in a test machine at one time
    int _testsPerMachine;

    // no. of cycles the machine can be run in one timestep
    int _cyclesPerTimestep;

    // maximun no. of tests that can be performed in 1 timestep
    int _maxTestPerTimestep;

    // tests done so far today
    int _testsThisTimestep;

    // max tests done in any timestep
    int _maxTestsAllTimesteps;

    // Maximum no. of tests that can be performed in 1 resource cycle
    int _maxTestPerCycle;

    // Maximum no. of tests that were performed in all resource cycles
    int _maxTestsAllCycles;

    // tests done so far this reagent cycle
    int _testsThisReagentCycle;

    // No of timesteps in reagent cycle
    int _reagentCycle;

    // priority for tests - if more than 1 test for same disease, do lower priority
    int _totalTests;

    // how many timesteps to wait before sending out samples in batch mode
    int _batchWaitTime;
    
    // how many timesteps to wait before patient returns for test results
    int _patientReturnTime;

    // percentage of true-positives for infected patients
    float _sensitivity;

    // percentage of true-negatives for non-infected patients
    float _specificity;

    // percentage of true-positives for infected patients for doctor overriding test results
    float _sensitivityMD;

    // percentage of true-negatives for non-infected patients for doctor overriding test results
    float _specificityMD;
    
    // percentage of true-positives for infected patients for doctor overriding test results
    float _sensitivityWithTestMD;

    // percentage of true-negatives for non-infected patients for doctor overriding test results
    float _specificityWithTestMD;

    // Test at this tier
    bool _tier0;
    bool _tier1;
    bool _tier2;
    bool _tier3;
    bool _tier4;
    bool _tier5;
    bool _tier6;
    bool _tier7;
    bool _tier8;
    bool _tier9;



public:
    Test();
    Test(string name, string disease, int testsPerMachine, int cyclesPerTimestep, int maxTestPerTimestep, int maxTestPerCycle, int reagentCycle, int priority, int batchWaitTime, int patientReturnTime, float sensitivity, float specificity, float sensitivityMD, float specificityWithTestMD, float sensitivityWithTestMD, float specificityMD, bool tier0, bool tier1, bool tier2, bool tier3, bool tier4, bool tier5, bool tier6, bool tier7, bool tier8, bool tier9);
    ~Test();

    Test(const Test& t2);

    // get test name
    inline string getTestName() { return _name; };

    // get disease name
    inline string getDiseaseName() { return _disease; };

    // set testing done here flag
    inline void setDiseaseTesting(bool testing) { _testing = testing; };
    
    // get testing done here flag
    inline bool getDiseaseTesting() { return _testing; };
    
    // How many tests can be done on 1 machine at a time
    inline int getNoMachines() { return _noMachines; };

    // How many tests can be done on 1 machine at a time
    inline void setNoMachines(int noMachines) { _noMachines = noMachines; };

    // How many tests can be done on 1 machine at a time
    inline int getTestsPerMachine() { return _testsPerMachine; };

    // set how many tests can be done on 1 machine at a time
    inline void setTestsPerMachine(int testsPerMachine) { _testsPerMachine = testsPerMachine; };
    
    // how many times can the machine be run in one time step
    inline int getCyclesPerTimestep() { return _cyclesPerTimestep; };

    // how many times can the machine be run in one time step
    inline void setCyclesPerTimestep(int cyclesPerTimestep) { _cyclesPerTimestep = cyclesPerTimestep; };

    // maximum tests done in 1 timestep
    inline int getMaxTestPerTimestep() { return _maxTestPerTimestep; };

    // maximum tests done in 1 timestep
    inline void setMaxTestPerTimestep(int maxTestPerTimestep) { _maxTestPerTimestep = maxTestPerTimestep; };

    // get tests done today
    inline int getTestsThisTimestep() { return _testsThisTimestep; };

    // set tests done today
    inline void setTestsThisTimestep(int testsThisTimestep) { _testsThisTimestep = testsThisTimestep; };
    
    // set tests done today
    inline void setMaxTestsAllTimesteps(int maxTestsAllTimesteps) { _maxTestsAllTimesteps = maxTestsAllTimesteps; };

    // get tests done today
    inline int getMaxTestsAllTimesteps() { return _maxTestsAllTimesteps; };

    // maximum tests done in 1 reagent cycle
    inline int getMaxTestPerCycle() { return _maxTestPerCycle; };

    // maximum tests done in 1 reagent cycle
    inline void setMaxTestPerCycle(int maxTestPerCycle) { _maxTestPerCycle = maxTestPerCycle; };

    // maximum tests done in all reagent cycles
    inline int getMaxTestsAllCycles() { return _maxTestsAllCycles; };

    // maximum tests done in all reagent cycles
    inline void setMaxTestsAllCycles(int maxTestsAllCycles) { _maxTestsAllCycles = maxTestsAllCycles; };

    // get tests done this reagent cycle
    inline int getTestsThisReagentCycle() { return _testsThisReagentCycle; };

    // set tests done this reagent cycle
    inline void setTestsThisReagentCycle(int testsThisReagentCycle) { _testsThisReagentCycle = testsThisReagentCycle; };

    // length of reagent cycle in time steps
    inline int getReagentCycle() { return _reagentCycle; };

    // length of reagent cycle in time steps
    inline void setReagentCycle(int reagentCycle) { _reagentCycle = reagentCycle; };

    // get total tests this run
    inline int getTotalTests() { return _totalTests; };

    // set total tests this run
    inline void setTotalTests(int totalTests) { _totalTests = totalTests; };

    // get test priority - lower done first
    inline int getPriority() { return _priority; };

    // set test priority - lower done first
    inline void setPriority(int priority) { _priority = priority; };

    // get batch wait time
    inline int getBatchWaitTime() { return _batchWaitTime; };

    // set batch wait time
    inline void setBatchWaitTime(int batchWaitTime) { _batchWaitTime = batchWaitTime; };

    // get patient return time
    inline int getPatientReturnTime() { return _patientReturnTime; };

    // set patient return time
    inline void setPatientReturnTime(int patientReturnTime) { _patientReturnTime = patientReturnTime; };

    // get sensitivity for this test
    inline float getSense() { return _sensitivity; };

    // set sensitivity for this test
    inline void setSense(float sensitivity) {_sensitivity = sensitivity; };

    // get specificity for this test
    inline float getSpecif() { return _specificity; };

    // set specificity for this test
    inline void setSpecif(float specificity) {_specificity = specificity; };

    // get sensitivity for this test
    inline float getSenseMD() { return _sensitivityMD; };

    // set sensitivity for this test
    inline void setSenseMD(float sensitivityMD) { _sensitivityMD = sensitivityMD; };

    // get specificity for this test
    inline float getSpecifMD() { return _specificityMD; };

    // set specificity for this test
    inline void setSpecifMD(float specificityMD) { _specificityMD = specificityMD; };

    // get sensitivity for this test
    inline float getSenseWithTestMD() { return _sensitivityWithTestMD; };

    // set sensitivity for this test
    inline void setSenseWithTestMD(float sensitivityMD) { _sensitivityWithTestMD = sensitivityMD; };

    // get specificity for this test
    inline float getSpecifWithTestMD() { return _specificityWithTestMD; };

    // set specificity for this test
    inline void setSpecifWithTestMD(float specificityMD) { _specificityWithTestMD = specificityMD; };

    // Test done at this tier ?
    inline bool getTier0() { return _tier0; };
    inline bool getTier1() { return _tier1; };
    inline bool getTier2() { return _tier2; };
    inline bool getTier3() { return _tier3; };
    inline bool getTier4() { return _tier4; };
    inline bool getTier5() { return _tier5; };
    inline bool getTier6() { return _tier6; };
    inline bool getTier7() { return _tier7; };
    inline bool getTier8() { return _tier8; };
    inline bool getTier9() { return _tier9; };

    // increment no. of tests done today
    inline void incrementTests() { _testsThisTimestep++; _testsThisReagentCycle++; _totalTests++; };

    // increment no. of tests done today
    inline void incrementTestsNotReagents() { _testsThisTimestep++; };

    // reset tests done today
    inline void resetTestsThisTimestep() { _testsThisTimestep = 0; };

    // reset tests done today
    inline void resetTestsThisReagentCycle() { _testsThisReagentCycle = 0; };

    // reset tests done today
    inline void incrementTotalTests() { _totalTests++; };

    // copy a test agent into new test agent
    void copyTest(Test* oldTest);


};

#endif                  /* defined(TEST_H) */

