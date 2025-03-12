//
//  GlobalParameters.h
//  
//
//  Created by L. Dombrowski on 1/22/20


#ifndef MY_GLOBALS_H
#define MY_GLOBALS_H


#include <string>

using namespace std;

// create stucture to store away incidence for each disease with each facility 
// also store away any test information specific to each facility
struct facilityAttributes {
	string disease;
	string testName;
	bool testing;
	int priority;
	float incidence;
	float transmission;
	string incAdjType;
	float incAdjFactor;
	float prpIncidencePresenting;
	float patientsRatioUninfInf;
	float perPatientPresentInf;
	int returnCycle;
	int noMachines;
	int maxReagentPerCycle;
	int timeStepsInCycle;
	int batchWaitTime;

	// closest connection id with testing, could be latteral or up or zonal
	string closetConnectionWithTesting;
	//Facility* closestConnectionWithTestingPtr;
	float closestConnectionDistance;

};

// create stucture to store away any test information specific to each facility
struct diagnosisAttributes {
	string disease;
	string testName;
	int testing;

	// closest connection id with testing, could be latteral or up or zonal
	string closetConnectionWithTesting;
	float closestConnectionDistance;
};

// create a structure for testing available flags for each disease
struct testingAvailableAttr {
        string disease;
		string testName;
        bool testing;
};

// use this to store away travel time data
struct travelData {
	float longitude;
	float latitude;
	int time;
	string facility;
	int id;
};

// this is the structure to store away scenario parameters
struct scenarioParam {
	string statSubScenarioDir;
	string statSubScenarioName;
	string statSubScenarioSplitName;
	string facilitySpecificNewFile;
	string facilitySpecificFile;
	string facilitySpecificSplitFile;
	bool splitScenario;
};

// use pre-treatment loss to follow up times by tier and by region
struct initTreatLtoFRegionTier {
	string regionNameTier;
	string regionName;
	float tier[10];
};


// create structure to store names for region/states and district/counties - they will get loaded from defaults/input/Region-Districts.csv
struct regionDistrict {
	
	// contains in this order: the word total, all the regions, all the districts
	string regionDistrict[1000];
	
	// the numbers in this array point to the index in the regionDistrict array that represents the region the district is in.  
	// There will be the same number of valid elements in this array as noDistricts
	int districtToRegionMapping[1000];

	// list of regions
	string regions[20];
};

extern struct regionDistrict regDist;

// counter used for any debugging purpose
extern float debugCounter;		

// counter used for any debugging purpose - count up individual stats while running ABM
extern bool statsCount;		

// mean value used for geometric distributions
extern float pRepresentation;

// next number in random number generator sequence
extern unsigned seed;
extern int maxNoSeq100;
extern int maxNoSeqPop;
//extern int maxNoSeqPopDummy;
//extern int maxNoExpSeq;
extern int maxNoGeoSeq;
extern int nextNoSeq100;
extern int nextNoSeqPopTB;
extern int nextNoSeqPopHIV;
extern int nextNoSeqPopHCV;
extern int nextNoSeqPopYF;
extern int nextNoSeqPopMen;
extern int nextNoSeqPopMea;
extern int nextNoSeqPopDis1;
extern int nextNoSeqPopDis2;
extern int nextNoExpSeq;
extern int nextNoGeoSeq;
extern int countSeq100;
extern int countSeqPopTB;
extern int countSeqPopHIV;
extern int countSeqPopHCV;
extern int countSeqPopYF;
extern int countSeqPopMen;
extern int countSeqPopMea;
extern int countSeqPopDis1;
extern int countSeqPopDis2;
extern int countExpSeq;
extern int countGeoSeq;
extern float popTB;
extern float popHIV;
extern float popHCV;
extern float popYF;
extern float popMen;
extern float popMea;
extern float popDis1;
extern float popDis2;
extern int seq100[1000000];
extern int seqPopTB[2000000];
extern int seqPopHIV[2000000];
extern int seqPopHCV[2000000];
extern int seqPopYF[2000000];
extern int seqPopMen[2000000];
extern int seqPopMea[2000000];
extern int seqPopDis1[2000000];
extern int seqPopDis2[2000000];
//extern int expSeq[1000000];
extern int geoSeq[1000000];

// current timestep
extern int timeStep;

// no. of steps in a year
extern int timestepsPerYear;

// no. of years in this run
extern int yearsInRun;

// no. of steps for run
extern int noSteps;

// how long has the simulation been running in years
extern int currentYearInRun;

// for split scenarios, how many years after start do we split
extern int splitYear;

// difference between reference year and starting year
extern int skipYears;

// save Patient data for registry - enter percentage of Patients to track
extern int pecSamplingForRegistry;

// sub-directory for running statistics in batch mode - usually job number
extern string statSubDir;
extern int statSubDirNo;

// sub-directory for running statistics in batch mode - usually scenario name
extern string statSubScenarioDir;

//max no. of diseases
extern int maxNoDiseases;

// number of elements in the region/district array - 1 for national, 16 for regions, 260 for districts
extern int noElementsDistrictRegionArray;

// number of regions/states used in model - largest division under national level - 2nd column in Region-Districts.csv file in default/inputs
extern int noRegions;

// number of district/counties used in model - largest division under regional/state level - 3rd column and beyond in Region-Districts.csv file in default/inputs
extern int noDistricts;

// number of facilities used in model
extern int noFacilities;

// MD override decription - RegionOnly, DistrictAndAbove, Everywhere
extern string MDClinicDiagText;

// MD override option - 1 = RegionOnly, 2 = DistrictAndAbove, 3 = Everywhere
extern int MDClinicDiagOption;

// control debug output
extern bool DEBUG;
extern bool DEBUG1;
extern bool DEBUG2;
extern bool DEBUG3;
extern bool DEBUG4;
extern bool DEBUG5;

// number of tiers in country
extern int noOfTiers;

// maybe should be in header file
extern bool tier1TBTestingXpert;
extern bool tier2TBTestingXpert;
extern bool tier3TBTestingXpert;
extern bool tier4TBTestingXpert;
extern bool tier5TBTestingXpert;
extern bool tier6TBTestingXpert;
extern bool tier7TBTestingXpert;
extern bool tier8TBTestingXpert;
extern bool tier9TBTestingXpert;
extern bool tier10TBTestingXpert;

// maybe should be in header file
extern bool tier1TBTestingSmear;
extern bool tier2TBTestingSmear;
extern bool tier3TBTestingSmear;
extern bool tier4TBTestingSmear;
extern bool tier5TBTestingSmear;
extern bool tier6TBTestingSmear;
extern bool tier7TBTestingSmear;
extern bool tier8TBTestingSmear;
extern bool tier9TBTestingSmear;
extern bool tier10TBTestingSmear;

extern bool tier1HIVTestingXpert;
extern bool tier2HIVTestingXpert;
extern bool tier3HIVTestingXpert;
extern bool tier4HIVTestingXpert;
extern bool tier5HIVTestingXpert;
extern bool tier6HIVTestingXpert;
extern bool tier7HIVTestingXpert;
extern bool tier8HIVTestingXpert;
extern bool tier9HIVTestingXpert;
extern bool tier10HIVTestingXpert;

extern bool tier1HCVTestingXpert;
extern bool tier2HCVTestingXpert;
extern bool tier3HCVTestingXpert;
extern bool tier4HCVTestingXpert;
extern bool tier5HCVTestingXpert;
extern bool tier6HCVTestingXpert;
extern bool tier7HCVTestingXpert;
extern bool tier8HCVTestingXpert;
extern bool tier9HCVTestingXpert;
extern bool tier10HCVTestingXpert;

extern bool tier1YFTestingXpert;
extern bool tier2YFTestingXpert;
extern bool tier3YFTestingXpert;
extern bool tier4YFTestingXpert;
extern bool tier5YFTestingXpert;
extern bool tier6YFTestingXpert;
extern bool tier7YFTestingXpert;
extern bool tier8YFTestingXpert;
extern bool tier9YFTestingXpert;
extern bool tier10YFTestingXpert;

extern bool tier1MenTestingPCR;
extern bool tier2MenTestingPCR;
extern bool tier3MenTestingPCR;
extern bool tier4MenTestingPCR;
extern bool tier5MenTestingPCR;
extern bool tier6MenTestingPCR;
extern bool tier7MenTestingPCR;
extern bool tier8MenTestingPCR;
extern bool tier9MenTestingPCR;
extern bool tier10MenTestingPCR;

extern bool tier1MeaTestingCulture;
extern bool tier2MeaTestingCulture;
extern bool tier3MeaTestingCulture;
extern bool tier4MeaTestingCulture;
extern bool tier5MeaTestingCulture;
extern bool tier6MeaTestingCulture;
extern bool tier7MeaTestingCulture;
extern bool tier8MeaTestingCulture;
extern bool tier9MeaTestingCulture;
extern bool tier10MeaTestingCulture;

extern bool tier1Dis1TestingTest;
extern bool tier2Dis1TestingTest;
extern bool tier3Dis1TestingTest;
extern bool tier4Dis1TestingTest;
extern bool tier5Dis1TestingTest;
extern bool tier6Dis1TestingTest;
extern bool tier7Dis1TestingTest;
extern bool tier8Dis1TestingTest;
extern bool tier9Dis1TestingTest;
extern bool tier10Dis1TestingTest;

extern bool tier1Dis2TestingTest;
extern bool tier2Dis2TestingTest;
extern bool tier3Dis2TestingTest;
extern bool tier4Dis2TestingTest;
extern bool tier5Dis2TestingTest;
extern bool tier6Dis2TestingTest;
extern bool tier7Dis2TestingTest;
extern bool tier8Dis2TestingTest;
extern bool tier9Dis2TestingTest;
extern bool tier10Dis2TestingTest;


// maybe should be in header file
extern bool tier1Diagnosis;
extern bool tier2Diagnosis;
extern bool tier3Diagnosis;
extern bool tier4Diagnosis;
extern bool tier5Diagnosis;
extern bool tier6Diagnosis;
extern bool tier7Diagnosis;
extern bool tier8Diagnosis;
extern bool tier9Diagnosis;
extern bool tier10Diagnosis;

// tier mapping
extern string tier1Mapping;
extern string tier2Mapping;
extern string tier3aMapping;
extern string tier3bMapping;
extern string tier4aMapping;
extern string tier4bMapping;
extern string tier4cMapping;
extern string tier4dMapping;
extern string tier5aMapping;
extern string tier5bMapping;
extern string tier5cMapping;
extern string tier6Mapping;
extern string tier7aMapping;
extern string tier7bMapping;
extern string tier8Mapping;
extern string tier9Mapping;
extern string tier10Mapping;

// total population
extern int totalPopulation;

// total population growth rate to convert from 2015 to whatever year is being run
// assume every run is starting at reference year (2017) and skip the years between starting year and reference year
// if you change the referencen year, you have to change the popInitialGrowthRate to match that year and change the 
// /defaults/input/catchment/Facility-Catchment-Population-Distribution.csv file to match that year
extern int referenceYear;
extern int startingYear;
extern float popInitialGrowthRate;
extern float popInitialGrowthRate2016;			// 2016
extern float popInitialGrowthRate2017;			// 2017
extern float popInitialGrowthRate2018;			// 2018
extern float popInitialGrowthRate2019;			// 2019
extern float popInitialGrowthRate2020;			// 2020
extern float popInitialGrowthRate2021;			// 2021
extern float popInitialGrowthRate2022;			// 2022
extern float popInitialGrowthRate2023;			// 2023
extern float popInitialGrowthRate2024;			// 2024
extern float popInitialGrowthRate2025;			// 2025
extern float popInitialGrowthRate2026;			// 2026
extern float popInitialGrowthRate2027;			// 2027
extern float popInitialGrowthRate2028;			// 2028
extern float popInitialGrowthRate2029;			// 2029
extern float popInitialGrowthRate2030;			// 2030
extern float popInitialGrowthRate2031;			// 2031
extern float popInitialGrowthRate2032;			// 2032
extern float popInitialGrowthRate2033;			// 2033
extern float popInitialGrowthRate2034;			// 2034
extern float popInitialGrowthRate2035;			// 2035
extern float popInitialGrowthRate2036;			// 2036
extern float popInitialGrowthRate2037;			// 2037
extern float popInitialGrowthRate2038;			// 2038
extern float popInitialGrowthRate2039;			// 2039


// growth rates from 2016 to 2040
extern float popGrowthRateArray[26][2];


// total population
extern float popGrowthRate;

// incidence adjustment factor - now stored with each disease
//extern float incAdjustment;
//extern float incAdjustmentOrig;

// how many uninfected patients for every infected patient - to create and track
// now stored with each disease
//extern float patientsRatioNonInfInf;

// how many patients do we make each timestep - infected and uninfected
extern int patientsPerTimestep;

// transit factor - used to calculate the number of time steps in transit
// divide distance to facility by distancePerTimeStep
extern float distancePerTimeStep;

// probability patients will return to facility to get results - percent
extern int probReturn;

// sample life - how long does a sample still remain viable
extern int sampleLife;

// number of times a patient will return to get sample results
extern int patientReturns;

//no. of time steps patient must wait before returning to facility to get sample results
extern int returnCycle;

// Testing parameters

// how many tests can be done in a day
extern int maxTests;

// how many machines in facility
extern int noMachines;

// how many tests can be done with full reagent stocks
extern int maxTestsReagents;

// How many timesteps before reagents get restocked
extern int reagentRestockTime;

// how many people available to do tests
extern int noPersonel;

// patient states
extern int PatAwayUnTest;					// patient away from facility - hasn't returned for results
extern int PatInUnTest;						// patient in-house - no sample results back yet
extern int PatInfTestTreat;					// patient infected, sample results returned, undergoing treatment
extern int PatInfTestRelapse;				// patient infected, sample results returned, treated, relapse
extern int PatInfTestNoTreat;				// Patient infected, sample results returned, no treatment
extern int PatInfNoTestNoTreat;				// patient infected, no sample results returned, no treatement
extern int PatInfTestLtofNoTreat;			// patient infected, sample returned, patient didn't (loss to followup), no treatment
extern int PatDead;							// patient dead
extern int PatCured;						// patient cured
extern int PatUninfTest;					// patient uninfected - sample results returned to patient
extern int PatUninfNoTest;					// patient uninfected - didn't return for sample results
extern int PatUninfTestTreat;				// patient uninfected - sample results returned uninfected, but was treated anyway
extern int PatUninfTestLtoF;				// patient uninfected - sample results returned uninfected, Loss to follow up before or during treatment

// sample states
extern int SampCreatedUnTest;		// created but untested
extern int SampCreatedUnTestBatch;	// created, untested waiting to be sent out in batch mode
extern int SampUnTestInTransit;		// in transit
extern int SampUnTestWaitRepresent;	// waiting for patient to represent
extern int SampWaitForTest;			// waiting for test
extern int SampLostExpired;			// lost/expired
extern int SampTestedInTransit;		// results in transit back to orig facility
extern int SampReturnedAvail;		// results returned - available
extern int SampReturnedLostExpired;	// loss to follow up
extern int SampLtoF;				// loss to follow up
extern int SampPatDead;				// patient dead
extern int SampPatuntreatedInactive;	// patient went untreated, sample no longer active



#endif
