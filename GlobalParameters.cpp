//
//  GlobalParameters.cpp
// 
//  Created by L. Dombrowski on 1/22/20

//
#include "GlobalParameters.h"

// for storing region and district names
struct regionDistrict regDist;

// counter used for any debugging purpose
float debugCounter = 0;

// counter used for any debugging purpose - count up individual stats while running ABM
bool statsCount = true;	

// mean value used for geometric distributions
float pRepresentation = .1;

// next number in random number generator sequence
unsigned seed = 0;
int maxNoSeq100 = 1000000;
int maxNoSeqPop = 2000000;
//int maxNoSeqPopDummy = 200;
//int maxNoExpSeq = 1000000;
int maxNoGeoSeq = 1000000;
int nextNoSeq100 = 1;
int nextNoSeqPopTB = 1;
int nextNoSeqPopHIV = 1;
int nextNoSeqPopHCV = 1;
int nextNoSeqPopYF = 1;
int nextNoSeqPopMen = 1;
int nextNoSeqPopMea = 1;
int nextNoSeqPopDis1 = 1;
int nextNoSeqPopDis2 = 1;
int nextNoExpSeq = 1;
int nextNoGeoSeq = 1;
int countSeq100 = 1;
int countSeqPopTB = 1;
int countSeqPopHIV = 1;
int countSeqPopHCV = 1;
int countSeqPopYF = 1;
int countSeqPopMen = 1;
int countSeqPopMea = 1;
int countSeqPopDis1 = 1;
int countSeqPopDis2 = 1;
int countExpSeq = 1;
int countGeoSeq = 1;
float popTB = 1;
float popHIV = 1;
float popHCV = 1;
float popYF = 1;
float popMen = 1;
float popMea = 1;
float popDis1 = 1;
float popDis2 = 1;
int seq100[1000000] = {0};
int seqPopTB[2000000] = {0};
int seqPopHIV[2000000] = {0};
int seqPopHCV[2000000] = {0};
int seqPopYF[2000000] = {0};
int seqPopMen[2000000] = {0};
int seqPopMea[2000000] = {0};
int seqPopDis1[2000000] = { 0 };
int seqPopDis2[2000000] = { 0 };
//int expSeq[1000000] = { 0 };
int geoSeq[1000000] = { 0 };


// current timestep
int timeStep = 1;

// no. of steps in a year
int timestepsPerYear = 365;

// no. of years in this run
int yearsInRun = 8;


// how long has the simulation been running in years
int currentYearInRun = 0;

// for split scenarios, how many years after start do we split
int splitYear = 1;

// difference between reference year and starting year
int skipYears = startingYear - referenceYear;

// no. of steps for run
int noSteps = timestepsPerYear * (yearsInRun + skipYears);

// save Patient data for registry - percent of 100 patients
int pecSamplingForRegistry = 1;

// sub-directory for running statistics in batch mode
string statSubDir = "1";
int statSubDirNo = 1;

// sub-directory for running statistics in batch mode
string statSubScenarioDir = "Decentralized-smear4";

//max no. of diseases
int maxNoDiseases = 8;

// number of elements in the region/district array - 1 for national, 16 for regions, 260 for districts
int noElementsDistrictRegionArray = 277;

// number of regions/states used in model - largest division under national level - 2nd column in Region-Districts.csv file in default/inputs
int noRegions = 16;

// number of district/counties used in model - largest division under regional/state level - 3rd column and beyond in Region-Districts.csv file in default/inputs
int noDistricts = 260;

// number of facilities used in model
int noFacilities = 999;

// MD override decription - RegionOnly, DistrictAndAbove, Everywhere
string MDClinicDiagText = "RegionOnly";

// MD override option - 1 = RegionOnly, 2 = DistrictAndAbove, 3 = Everywhere
int MDClinicDiagOption = 1;

// control debug output
bool DEBUG = true;
bool DEBUG1 = false;
bool DEBUG2 = false;
bool DEBUG3 = false;
bool DEBUG4 = false;
bool DEBUG5 = false;

// number of tiers in country
int noOfTiers = 6;

// Testing at this tier
bool tier1TBTestingXpert = true;	// Tier 1
bool tier2TBTestingXpert = true;	// Tier 2
bool tier3TBTestingXpert = true;	// Tier 3
bool tier4TBTestingXpert = false;	// Tier 4
bool tier5TBTestingXpert = false;	// Tier 5
bool tier6TBTestingXpert = false;	// Tier 6
bool tier7TBTestingXpert = false;	// Tier 7
bool tier8TBTestingXpert = false;	// Tier 8
bool tier9TBTestingXpert = false;	// Tier 9
bool tier10TBTestingXpert = false;	// Tier 10	

// Testing at this tier
bool tier1TBTestingSmear = true;	// Tier 1
bool tier2TBTestingSmear = true;	// Tier 2
bool tier3TBTestingSmear = true;	// Tier 3
bool tier4TBTestingSmear = false;	// Tier 4
bool tier5TBTestingSmear = false;	// Tier 5
bool tier6TBTestingSmear = false;	// Tier 6
bool tier7TBTestingSmear = false;	// Tier 7
bool tier8TBTestingSmear = false;	// Tier 8
bool tier9TBTestingSmear = false;	// Tier 9
bool tier10TBTestingSmear = false;	// Tier 10	


bool tier1HIVTestingXpert = true;	// Tier 1
bool tier2HIVTestingXpert = true;	// Tier 2
bool tier3HIVTestingXpert = true;	// Tier 3
bool tier4HIVTestingXpert = false;	// Tier 4
bool tier5HIVTestingXpert = false;	// Tier 5
bool tier6HIVTestingXpert = false;	// Tier 6
bool tier7HIVTestingXpert = false;	// Tier 7
bool tier8HIVTestingXpert = false;	// Tier 8
bool tier9HIVTestingXpert = false;	// Tier 9
bool tier10HIVTestingXpert = false;	// Tier 10

bool tier1HCVTestingXpert = true;	// Tier 1
bool tier2HCVTestingXpert = true;	// Tier 2
bool tier3HCVTestingXpert = true;	// Tier 3
bool tier4HCVTestingXpert = false;	// Tier 4
bool tier5HCVTestingXpert = false;	// Tier 5
bool tier6HCVTestingXpert = false;	// Tier 6
bool tier7HCVTestingXpert = false;	// Tier 7
bool tier8HCVTestingXpert = false;	// Tier 8
bool tier9HCVTestingXpert = false;	// Tier 9
bool tier10HCVTestingXpert = false;	// Tier 10

bool tier1YFTestingXpert = true;	// Tier 1
bool tier2YFTestingXpert = true;	// Tier 2
bool tier3YFTestingXpert = true;	// Tier 3
bool tier4YFTestingXpert = true;	// Tier 4
bool tier5YFTestingXpert = true;	// Tier 5
bool tier6YFTestingXpert = true;	// Tier 6
bool tier7YFTestingXpert = false;	// Tier 7
bool tier8YFTestingXpert = false;	// Tier 8
bool tier9YFTestingXpert = false;	// Tier 9
bool tier10YFTestingXpert = false;	// Tier 10

bool tier1MenTestingPCR = true;	// Tier 1
bool tier2MenTestingPCR = true;	// Tier 2
bool tier3MenTestingPCR = true;	// Tier 3
bool tier4MenTestingPCR = false;	// Tier 4
bool tier5MenTestingPCR = false;	// Tier 5
bool tier6MenTestingPCR = false;	// Tier 6
bool tier7MenTestingPCR = false;	// Tier 7
bool tier8MenTestingPCR = false;	// Tier 8
bool tier9MenTestingPCR = false;	// Tier 9
bool tier10MenTestingPCR = false;	// Tier 10

bool tier1MeaTestingCulture = true;	// Tier 1
bool tier2MeaTestingCulture = true;	// Tier 2
bool tier3MeaTestingCulture = true;	// Tier 3
bool tier4MeaTestingCulture = true;	// Tier 4
bool tier5MeaTestingCulture = true;	// Tier 5
bool tier6MeaTestingCulture = false;	// Tier 6
bool tier7MeaTestingCulture = false;	// Tier 7
bool tier8MeaTestingCulture = false;	// Tier 8
bool tier9MeaTestingCulture = false;	// Tier 9
bool tier10MeaTestingCulture = false;	// Tier 10

bool tier1Dis1TestingTest = true;	// Tier 1
bool tier2Dis1TestingTest = true;	// Tier 2
bool tier3Dis1TestingTest = true;	// Tier 3
bool tier4Dis1TestingTest = true;	// Tier 4
bool tier5Dis1TestingTest = true;	// Tier 5
bool tier6Dis1TestingTest = false;	// Tier 6
bool tier7Dis1TestingTest = false;	// Tier 7
bool tier8Dis1TestingTest = false;	// Tier 8
bool tier9Dis1TestingTest = false;	// Tier 9
bool tier10Dis1TestingTest = false;	// Tier 10

bool tier1Dis2TestingTest = true;	// Tier 1
bool tier2Dis2TestingTest = true;	// Tier 2
bool tier3Dis2TestingTest = true;	// Tier 3
bool tier4Dis2TestingTest = true;	// Tier 4
bool tier5Dis2TestingTest = true;	// Tier 5
bool tier6Dis2TestingTest = false;	// Tier 6
bool tier7Dis2TestingTest = false;	// Tier 7
bool tier8Dis2TestingTest = false;	// Tier 8
bool tier9Dis2TestingTest = false;	// Tier 9
bool tier10Dis2TestingTest = false;	// Tier 10

// Diagnosis at this tier
bool tier1Diagnosis = false;		// Tier 1
bool tier2Diagnosis = false;		// Tier 2
bool tier3Diagnosis = true;			// Tier 3
bool tier4Diagnosis = true;			// Tier 4
bool tier5Diagnosis = true;			// Tier 5
bool tier6Diagnosis = true;			// Tier 6
bool tier7Diagnosis = true;			// Tier 7
bool tier8Diagnosis = true;			// Tier 8
bool tier9Diagnosis = true;			// Tier 9
bool tier10Diagnosis = true;		// Tier 10

// tier mapping
string tier1Mapping = "National Public Health Lab";
string tier2Mapping = "Public Health Zonal Lab";
string tier3aMapping = "Regional Hospital";
string tier3bMapping = "Alt Regional Hospital";
string tier4aMapping = "District Hospital";
string tier4bMapping = "District and Teaching Hospital";
string tier4cMapping = "Alt District Hospital";
string tier4dMapping = "District Hospital and Referral Hospital";
string tier5aMapping = "Hospital";
string tier5bMapping = "Teaching Hospital";
string tier5cMapping = "Hospital and Referral Hospital";
string tier6Mapping = "Health Centre";
string tier7aMapping = "Clinic";
string tier7bMapping = "CHPS";
string tier8Mapping = "";
string tier9Mapping = "";
string tier10Mapping = "";

// total population
int totalPopulation = 0;

// total population growth rate to convert from 2015 to whatever year is being run on who population grid
// changing this value will require a new build of the /defaults/input/catchment/Facility-Catchment-Population-Distribution.csv file.
// remove that one, and another one will be in /stats/  - copy the resulting file over to /defaults/input/catchment/
// population for 2015 should be 28870939
// ghana-population-2023-10-04.csv (estimating priors)
// Original Source : United Nations - World Population Prospects

int referenceYear = 2017;
int startingYear = 2017;
//float popInitialGrowthRate = 1.2048;				// 2017 - 2.26
float popInitialGrowthRate = 1.236973;				// 2017 - 2.26
//			 		 conversion rate to 2015	   year	population
float popInitialGrowthRate2016 = 4.79;			// 2016 29554303
float popInitialGrowthRate2017 = 7.16;			// 2017 30222262
float popInitialGrowthRate2018 = 9.47;			// 2018 30870641
float popInitialGrowthRate2019 = 11.78;			// 2019 31522290
float popInitialGrowthRate2020 = 14.11;			// 2020 32180401
float popInitialGrowthRate2021 = 16.43;			// 2021 32833031
float popInitialGrowthRate2022 = 18.71;			// 2022 33475870
float popInitialGrowthRate2023 = 21.00;			// 2023 34121985 
float popInitialGrowthRate2024 = 23.32;			// 2024 34777522
float popInitialGrowthRate2025 = 25.67;			// 2025 35439785
float popInitialGrowthRate2026 = 28.02;			// 2026 36102736
float popInitialGrowthRate2027 = 30.37;			// 2027 36767488
float popInitialGrowthRate2028 = 32.75;			// 2028 37435099
float popInitialGrowthRate2029 = 35.12;			// 2029 38104805
float popInitialGrowthRate2030 = 37.50;			// 2030 38775850
float popInitialGrowthRate2031 = 39.88;			// 2031 39447704
float popInitialGrowthRate2032 = 42.27;			// 2032 40121418
float popInitialGrowthRate2033 = 44.68;			// 2033 40797660
float popInitialGrowthRate2034 = 47.08;			// 2034 41475941
float popInitialGrowthRate2035 = 49.49;			// 2035 42158202
float popInitialGrowthRate2036 = 51.93;			// 2036 42844158
float popInitialGrowthRate2037 = 54.37;			// 2037 43533065
float popInitialGrowthRate2038 = 56.83;			// 2038 44223240
float popInitialGrowthRate2039 = 59.3;			// 2039 44910363

//
// 2016, 1.0237
// growth rates from 2016 to 2040
float popGrowthRateArray[26][2] = { {2015, 0},
									{2016, 1.050883},
									{2017, 1.022601},
									{2018, 1.021454},
									{2019, 1.021109},
									{2020, 1.020878},
									{2021, 1.020280},
									{2022, 1.019579},
									{2023, 1.019301},
									{2024, 1.019212},
									{2025, 1.019043},
									{2026, 1.018706},
									{2027, 1.018413},
									{2028, 1.018158},
									{2029, 1.017890},
									{2030, 1.017611},
									{2031, 1.017327},
									{2032, 1.017079},
									{2033, 1.016855},
									{2034, 1.016625},
									{2035, 1.016450},
									{2036, 1.016271},
									{2037, 1.016079},
									{2038, 1.015854},
									{2039, 1.015538},
									{2040, 1.0153} };

// total population
//float popGrowthRate = 1.0212;
float popGrowthRate = 1.023;

// incidence adjustment factor
// Now stored with each disease
//float incAdjustment = 1.0;
//float incAdjustmentOrig = 1.0;

// how many uninfected patients for every infected patient - to create and track
// now stored with each disease
//float patientsRatioNonInfInf = 12;

// how many patients do we make each timestep - infected and uninfected
int patientsPerTimestep = 0;

// transit factor - used to calculate the number of time steps in transit
// divide distance to facility by distancePerTimeStep
float distancePerTimeStep = .7;

// probability patients will return to facility to get results - percent
int probReturn = 95;

// sample life - how many timesteps does a sample still remain viable
int sampleLife = 40;

// number of times a patient will return to get sample results
int patientReturns = 3;

// how often do the patients return to facility to get results - timesteps
int returnCycle = -1;

// Testing parameters
// how many tests can be done in a time step
int maxTests = 24;

// how many machines in facility
int noMachines = 1;

// how many tests can be done with full reagent stocks
int maxTestsReagents = 672;

// How many timesteps before reagents get restocked
int reagentRestockTime = 28;

// how many people available to do tests
int noPersonel = 1;

// patient states	
int PatAwayUnTest = 0;				// patient away from facility - hasn't returned for results
int PatInUnTest = 1;				// patient in-house - no sample results back yet
int PatInfTestTreat = 2;			// patient infected, sample results returned, undergoing treatment
int PatInfTestRelapse = 3;			// patient infected, sample results returned, treated, relapse
int PatInfTestNoTreat = 4;			// Patient infected, sample results returned, no treatment
int PatInfNoTestNoTreat = 5;		// patient infected, no sample results returned, no treatment
int PatInfTestLtofNoTreat = 6;		// patient infected, sample returned, patient didn't (loss to followup), no treatment
int PatDead = 7;					// patient dead
int PatCured = 8;					// patient cured
int PatUninfTest = 9;				// patient uninfected - sample results returned to patient
int PatUninfNoTest = 10;			// patient uninfected - didn't return for sample results
int PatUninfTestTreat = 11;			// patient uninfected - sample results returned uninfected, but was treated anyway
int PatUninfTestLtoF = 12;			// patient uninfected - sample results returned uninfected, loss to follow up before or during treatment

// sample states
int SampCreatedUnTest = 0;			// created but untested
int SampCreatedUnTestBatch = 1;		// created, untested waiting to be sent out in batch mode
int SampUnTestInTransit = 2;	    // in transit
int SampUnTestWaitRepresent = 3;	// waiting for patient to represent
int SampWaitForTest = 4;		    // waiting for test
int SampLostExpired = 5;		    // lost/expired
int SampTestedInTransit = 6;	    // results in transit back to orig facility
int SampReturnedAvail = 7;	        // results returned - available
int SampReturnedLostExpired = 8;	// loss to follow up
int SampLtoF = 9;			        // loss to follow up
int SampPatDead = 10;			    // patient dead
int SampPatuntreatedInactive = 11;	// patient went untreated, sample now longer active
