//
//  Testing.h
//  mdrtb
//
//  Created by L. Dombrowski on 1/10/2020.
//

#ifndef TESTING_H
#define TESTING_H

#include <string>
#include <vector>
#include "GlobalParameters.h" //for facilityAttributes
#include "Disease.h"
#include "Patient.h"
#include "Sample.h"
#include "Test.h"

using namespace  std;


class Testing {
private:

    Sample* _sample;

    // current facility
    string _facility;

    // keep a container around to store samples that need to be tested
    std::vector<Sample*> _sampleContainer;

    // keep a container around to store samples that are inactive
    // either returned to patient or patient didn't return to get results
    std::vector<Sample*> _inactiveSampleContainer;

    // keep a container to store all tests available at this testing facility
    vector<Test*> _testsContainer;

    // how many people available to do tests
    int _noPersonel;

public:
    Testing();
    Testing(string facilityName);
    Testing(string facilityName, int tier, vector<string> diseaseList, vector<Test*> tests, vector <facilityAttributes*> attrPtr);
    Testing(string facilityName, vector<string> diseaseList, vector<Test*> tests, vector <facilityAttributes*> attrPtr);
    Testing(string facilityName, int maxTests, int maxTestsReagents, int reagentRestockTime, int noPersonel);
    ~Testing();

    //get facility name
    inline string getFacilityName() { return _facility; };

    // reset testing resources for single disease
    void resetResources(int timeStep, string disease);

    // reset testing resources - all diseases
    void resetResources(int timeStep);

    // check to see if available resources to do test
    bool checkResources(string disease);
    
    // check to see if available resources to do test
    bool checkResources(string disease, string testName);

    // increment testing resources for a disease
    Test* incrementResourcesDisease(string disease);
    
    // increment testing resources for a test
    Test* incrementResourcesTest(string testName);

    // increment testing resources for a specific test for a disease
    Test* incrementResources(string disease, string test);

    // get sensitivity for disease to detmine true/positive, false/positives
    float getSensitivityPercent(string disease);
    
    // get sensitivity for disease to detmine true/positive, false/positives
    float getSensitivityPercent(string disease, string test);

    // get specificity for disease to detmine true/negative, false/negatives
    float getSpecificityPercent(string disease);
    
    // get specificity for disease to detmine true/negative, false/negatives
    float getSpecificityPercent(string disease, string test);

    // get sensitivity for disease to detmine true/positive, false/positives
    float getSensitivityMDPercent(string disease);
    
    // get sensitivity for disease to detmine true/positive, false/positives
    float getSensitivityMDPercent(string disease, string test);

    // get specificity for disease to detmine true/negative, false/negatives
    float getSpecificityMDPercent(string disease);
    
    // get specificity for disease to detmine true/negative, false/negatives
    float getSpecificityMDPercent(string disease, string test);

    // get sample container pointer
    inline vector<Sample*> getSamplePtr() { return _sampleContainer; };

    // get sample container pointer
    inline vector<Sample*> getInactiveSamplePtr() { return _inactiveSampleContainer; };

    // return the pointer to the Patient list
    inline vector<Test*>  getTeststPtr() { return  _testsContainer; };

    // put sample into sample container
    inline void addToSampleContainer(Sample* sample) { _sampleContainer.push_back(sample); };

    // put sample into inactive sample container
    inline void addToInactiveSampleContainer(Sample* sample) { _inactiveSampleContainer.push_back(sample); };

    // update Samples for this time step
    void updateSamples(int timeStep, vector<Disease*> dis);

    // reorder Samples, highest priority first, then oldest to newest
    void reorderSamples();

    // reverse Samples, oldest to newest
    void reverseSamples();

    // bring the last 20 samples to the front of the list
    void bringLastSamplesToFront();
    
    // bring the last 20 samples to the front of the list
    void bringLastSamplesToFront2();

    // print Sample list
    void printSamples();

    // remove Sample from _sampleContainer and delete
    void removeSamplesFromList(vector<Sample*> list, vector<Sample*> sample, bool deleteFlag);

    // remove Sample from _sampleContainer
    void removeSampleFromList(Sample* sample);

    // remove Sample from _sampleContainer and delete Sample object
    void removeSampleFromListAndDelete(Sample* sample);

    // look through active sample container and move sample associated with patient to inactive sample container
    bool removePatientSampleFromList(Patient* patient, int status);
    
    // look through inactive sample container and move sample associated with patient to active sample container
    bool restorePatientSampleFromList(Patient* patient, int status);

};

#endif
