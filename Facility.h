//
//  Facility.h
//  mdrtb
//
//  Created by L. Dombrowski on 1/10/2020.
//

#ifndef FACILITY_H
#define FACILITY_H


#include <string>
#include <vector>

#include "Diagnosis.h"
#include "GlobalParameters.h"
//#include "Facility.h"
#include "Testing.h"

// class Testing;
// class Diagnosis;

using namespace  std;


class Facility {
private:

    // name of Facility
    string _name;
    string _id;
    string _type;

    float _longitude;
    float _latitude;

    string _town;
    string _district;
    string _region;
    int _districtIndex;
    int _regionIndex;

    // Tier 1 - National Lab
    // tier 2 - Zonal Lab
    // tier 3 - Regional Hospital
    // tier 4 - District Hospital
    // tier 5 - Hospital
    // tier 6 - Health Centers
    // tier 7 - Clinic
    int _tier;

    // Diagnosis agent for this facility
    Diagnosis* _diagnosis;

    // Testing agent for this facility
    Testing* _testing;

    // connections - only 1 up connection
    string _upConnection;
    Facility* _upConnectionPtr;

    // latteral connection - zonal labs, other facilities in tier
    vector<string> _latConnection;
    vector<Facility*> _latConnectionPtr;

    // downward connection - what facilities does this one own
    vector<string> _downConnection;
    vector<Facility*> _downConnectionPtr;

    // population in catchment
    int _population;

    // pop bin increment value
    float _binInc;

    // probability that the population is a given distance away
    float _popBins[10];

    // density of population in a given popBin - used for transmission of disease
    float _popDensityBins[10];

    // list of facility specific attributes for tests
    vector<facilityAttributes*> _facilityAttributes;

    // Xpert here
    bool _Xpert;

public:
    Facility();
    Facility(string name, string id, float longitude, float latitude, string type, int tier);
    Facility(string name, string id, float longitude, float latitude, string type, string up, float dist, int tier);
    Facility(string name, string id, float longitude, float latitude, string type, string up, float dist, int tier, int pop, float inc, float bins[10], bool diag, bool test);
    Facility(string name, string id, float longitude, float latitude, string type, string up, float dist, int tier, int pop, float inc, float bins[10], string region, string district, bool diag, vector<string> diseaseList, vector<Test*> tests, vector <facilityAttributes*> attrPtr, bool Xpert);
    Facility(string name, string id, float longitude, float latitude, string type, string up, float dist, int tier, int pop, float inc, float bins[10], float densityBins[10], string region, int regIndex, string district, int distIndex, bool diag, vector<string> diseaseList, vector<Test*> tests, vector <facilityAttributes*> attrPtr, bool Xpert);
    ~Facility();

    // Diagnosis at this facility
    bool diagnosisAvailable();

    // Testing at this facility
    bool testingAvailable(string disease, string test);

    // Testing at this facility
    bool testingAvailableByPriority(string disease);
    
    // Any Testing for this disease at this facility
    bool testingAvailableAny(string disease);

    // Any Testing for this disease at this facility - return test name
    string testingAvailableName(string disease);

    // set testing flag for disease and test
    void setTestingAvailable(string disease, string test, bool testing);

    // add reset number of tests available at testing facility
    inline void resetResources(int timeStep) { _testing->resetResources(timeStep); };

    // add same tier facility name
    inline void addLatConnection(string facility) { _latConnection.push_back(facility); };

    // add same tier facility ptr
    inline void addLatConnectionPtr(Facility* ptr) { _latConnectionPtr.push_back(ptr); };

    // latteral facility connections
    inline void addUpConnection(string facility) { _upConnection = facility; };

    // downstream facility connections
    inline void addDownConnection(string facility) { _downConnection.push_back(facility); };

    // downstream facility connections
    inline void updatePopulation(int population) { _population = population; };
    
    // downstream facility connections
    int getPopulation();

    // downstream facility connections
    inline void updateBinInc(float inc) { _binInc = inc; };

    // downstream facility connections
    inline float getBinInc() { return _binInc; };

    // no population probablilty bins available
    void updatePopBins();

    // fill in population probablilty bins
    void updatePopBins(float bin[]);

    // add Sample to Sample container in Testing
    inline void addToSampleContainer(Sample* sample) { _testing->addToSampleContainer(sample); };

    // add Sample to inactive Sample container in Testing
    inline void addToInactiveSampleContainer(Sample* sample) { _testing->addToInactiveSampleContainer(sample); };

    // get facility name
    inline string getName() { return _name; };

    // get facility id
    inline string getID() { return _id; };

    // get facility id as an int
    inline int getIDint() { return stoi(_id); };

    // get facility name
    inline string getType() { return _type; };

    // get up connection name
    inline string getUpName() { return _upConnection; };

    // get up connection name
    inline void setUpName(string upConnection) { _upConnection = upConnection; };

    // get longitude of Facility
    inline float getLongitude() { return _longitude; };

    // get latitude of Facility
    inline float getLatitude() { return _latitude; };

    // get town where Facility is located
    inline string getTown() { return _town; };

    // set town where Facility is located
    inline void setTown(string town) { _town = town; };

    // get district where Facility is located
    inline string getDistrict() { return _district; };

    // set district where Facility is located
    inline void setDistrict(string district) { _district = district; };
    
    // get district index where Facility is located
    inline int getDistrictIndex() { return _districtIndex; };

    // set district index where Facility is located
    inline void setDistrictIndex(int districtIndex) { _districtIndex = districtIndex; };

    // get region where Facility is located
    inline string getRegion() { return _region; };

    // set region where Facility is located
    inline void setRegion(string region) { _region = region; };
    
    // get region index where Facility is located
    inline int getRegionIndex() { return _regionIndex; };

    // set region index where Facility is located
    inline void setRegionIndex(int regionIndex) { _regionIndex = regionIndex; };

    // get tier of Facility
    inline int getTier() { return _tier; };

    // set up connection pointer
    inline Diagnosis* getDiagnosisPtr() { return _diagnosis; };

    // set up connection pointer
    inline Testing* getTestingPtr() { return _testing; };

    // get no. of time steps to have to wait for sample results to return
    int getTestReturnTime(string disease, string test);

    // set up connection pointer
    inline void setUpPtr(Facility* f) { _upConnectionPtr = f; };

    // set up connection pointer
    inline Facility* getUpPtr() { return _upConnectionPtr; };

    // name of next closest Testing Facility
    string getClosestName(string disease);

    // name of next closest Testing Facility
    string getClosestNameByTest(string disease, string test);

    // name of next closest Testing Facility - based off of disease - return this one if possible
    string getClosestNameByDisease(string disease, string facility);

    // name of next closest Testing Facility - based off of disease - return this one if possible
    float getClosestDistByDisease(string disease, string facility);

    // name of next closest Testing Facility
    void setClosestName(string disease, string test, string name);

    // distance to next closest Testing Facility
    float getClosestDistance(string disease, string test);

    // distance to next closest Testing Facility
    void setClosestDistance(string disease, string test, float dist);

    // set closest connection pointer
    Facility* getClosestPtr(string disease, string test);

    // set up connection pointer
    void setClosestPtr(string disease, string test, Facility* f);

    // set latteral connection pointer
    inline vector<string> getLatNameVector() { return _latConnection; };

    // get latteral connection pointer
    inline vector<Facility*> getLatPtr() { return _latConnectionPtr; };

    // get population probablilty bins
    inline float* getPopBins() { return _popBins; };

    // get population probablilty bins
    inline float getPopBin0() { return _popBins[0]; };
    inline float getPopBin1() { return _popBins[1]; };
    inline float getPopBin2() { return _popBins[2]; };
    inline float getPopBin3() { return _popBins[3]; };
    inline float getPopBin4() { return _popBins[4]; };
    inline float getPopBin5() { return _popBins[5]; };
    inline float getPopBin6() { return _popBins[6]; };
    inline float getPopBin7() { return _popBins[7]; };
    inline float getPopBin8() { return _popBins[8]; };
    inline float getPopBin9() { return _popBins[9]; };

    // get density of population in each probablilty bin
    inline float* getPopDensityBins() { return _popDensityBins; };

    // get density of population in each probablilty bin
    inline float getPopDensityBin0() { return _popDensityBins[0]; };
    inline float getPopDensityBin1() { return _popDensityBins[1]; };
    inline float getPopDensityBin2() { return _popDensityBins[2]; };
    inline float getPopDensityBin3() { return _popDensityBins[3]; };
    inline float getPopDensityBin4() { return _popDensityBins[4]; };
    inline float getPopDensityBin5() { return _popDensityBins[5]; };
    inline float getPopDensityBin6() { return _popDensityBins[6]; };
    inline float getPopDensityBin7() { return _popDensityBins[7]; };
    inline float getPopDensityBin8() { return _popDensityBins[8]; };
    inline float getPopDensityBin9() { return _popDensityBins[9]; };

    // get density of population in specific probablilty bin
    inline float getLocalDensity(int index) { return _popDensityBins[index]; };

    // Get incidence related to specific disease for this facility
    float getDiseaseIncidence(string disease);

    // Get incidence related to specific disease for infected patients not presenting to this facility
    float getDiseaseIncidenceNotPresenting(string disease);

    // Get ratio of uninfected to infected patients related to specific disease for this facility
    float getDiseaseRatioUninfInf(string disease);

    // Get incidence related to specific disease for this facility
    int getDiseaseNoMachines(string disease, string test);
    
    // Get batch interval for sending out samples for testing
    int getBatchWaitTime(string disease);

    // update patients for this timestep with respect to Samples
    void updatePatients(vector<Disease*> diseases, vector<Test*> tests, int timeStep);

    // Handle Sample testing for this timestep
    void updateSamples(int timeStep, string disease, vector<Disease*> dis);

    // create patients for this timestep
    void createPatients(vector<string> diseaseList, vector<Disease*> diseases, int timeStep);

    // gather statistics on each population for  each facility
    void gatherFacilityStatistics(vector<Facility*> facilities);

    // get facilities attribute pointer
    inline vector<facilityAttributes*> getFacilityAttributesPtr() { return _facilityAttributes; };

    // downstream facility connections
    inline bool getXpert() { return _Xpert; };

    // downstream facility connections
    inline void setXpert(bool Xpert) { _Xpert = Xpert; };
    
    // create testing available container
    vector<testingAvailableAttr*> createTestingAvailObjects();

    // get the name of the test that can be done here for this disease - by priority
    string getTestName(string disease);

    // get the incidence value associated with the disease and year
    float getIncidence(string disease, int yearNo);

    // get the incidence value of those patients not presenting for diagnosis associated with the disease and year
    float getIncidenceNotPresenting(string disease, int yearNo);

    // get the incidence adjustment factor associated with the disease
    float getIncAdjFactor(string disease);

    // get the proportion of infected patient presenting for this disease
    float getPrpIncidencePresenting(string disease);

};

#endif /* defined(FACILITY_H) */
