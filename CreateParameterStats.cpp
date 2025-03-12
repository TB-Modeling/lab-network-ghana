//
//  CreateParameterStats.cpp
// 
//
//  Created by L. Dombrowski on 5/7/21

//
// output input parameter list that might change

#include <vector>
#include <string>
#include <cstring>
#include <sys/stat.h>
#include <errno.h>

#include "CreateParameterStats.h"
#include "GlobalParameters.h"
#include "FacilitiesList.h"
using namespace std;




// output input parameter list that were used to run this model run
void parameterStats( vector<Disease*> dis, vector<Test*> tes, FacilitiesList* list) {
   
    // relevant input data parameterStats already stored in FacilitiesResources.csv for each job
    // region-distric-specific-data - incidence
    // Facility-Specific-Disease-Data - noMachinesFlag
    // Test-Objects - maxReagentFlag, timeStepsPerCycleFlag

    // still need to be output
    // Disease-Objects - initTreatSuccessFlag, initTreatDeathFlag, retreatmentFlag
    // Test-Objects -  sampleWaitFlag, sensitivityFlag, specificityFlag, MDSensitivityFlag, MDSpecificityFlag
    // 
    // input parameters from defaults/input/input-change-parameters/priorRangesAndDistributions.csv
    // infUninfRatio sensitivity specificity MDSensitivity MDSpecificity initTreatSuccess initTreatDeath PreTestMDCDInf PreTestMDCDUnInf PreTestLtoF PreTreatLtoF TreatLtoF
    // relapseSuccess probDeathRepresent incidenceAdjustment probRepresentingPostLtoF timeDeathRetreat timeUntreatCure


    int errNo;
    int noTests;
    int noDiseases;
    int countT = 0;
    int countD = 0;
    int countDistricts = 0;
    int countRegions = 0;
    int actualDistrictCount = 0;
    int actualRegionCount = 0;
    int population;
    float yearIncidence = 0;
    bool regionFoundFlag = false;
    bool districtFoundFlag = false;
    bool debug = false;
    bool regLtoFFlag = true;
    string subDirBase;
    string facName;
    string upName;
    string district;
    string region;
    string regionName;
    ofstream myfile;
    vector<facilityAttributes*> facAttr;

    struct tests {

        string name;            // name of test
        string disease;       
        int batchWaitTime;      // how many timesteps to wait before sending out samples in batch mode    
        float sensitivity;      // percentage of true-positives for infected patients     
        float specificity;      // percentage of true-negatives for non-infected patients     
        float sensitivityWithTestMD;  // percentage of true-positives for infected patients for doctor overriding test results   
        float specificityWithTestMD;   // percentage of true-negatives for non-infected patients for doctor overriding test results
    };


    struct diseases {

        string disease;  
        float patientsRatioUninfInf;     // ratio of uninfected patients to infected patients presenting
        float prpIncidencePresent;       // proportion of infected patients that present
        float perPatientPresentInf;      // percentage of patients presenting that are infected - will override uninf/inf ratio
        float initTreatSuccess;          // percentage of initial treatment success   
        float initTreatDeath;            // percentage of intial treatment death   
        float probReturns;               // percentage of probaility patient returns
        float perPreTestLtoF;            // percentage of samples loss to follow up
        float perPreTreatLtoF;           // percentage of patients loss to follow up before starting treatment
        float perTreatLtoF;              // percentage of patients loss to follow up after diagnosis and before treatment
        float relapseSuccess;            // percentage of patients that are successfully treated after relapse
        int representationLtoFTime;      // no. of timestep before patient represents for LtoF
        float probDeathRepresent;        // probability patient represents for LtoF
        float probGeoDist;               // mean geometric value for calculating geometric distributions for representation times
        int timeDeathRetreat;            // max. no. of timestep before patient dies after LtoF
        int timeUntreatCure;             // max. no. of timestep before untreated patient is cured after LtoF
        float probPreTestMDCDInf;        // probability of MD clinical diagnosis positive before pre-test sample send out for infected patients
        float probPreTestMDCDUnInf;      // probability of MD clinical diagnosis positive before pre-test sample send out for uninfected patients
        initTreatLtoFRegionTier preTestLtof[20];    // pre-test loss to follow up by region for each tier
        initTreatLtoFRegionTier preTreatLtof[20];   // pre-treat loss to follow up by region for each tier
        initTreatLtoFRegionTier treatLtof[20];      // treatment loss to follow up by region for each tier
    };

    struct district {

        string name;  
        string disease;
        string test;
        int population;                // population of district   
        float incidence0;              // incidence year 0 for district
        float incidence1;              // incidence year 1 for district
        float incidence2;              // incidence year 2 for district
        float incidence3;              // incidence year 3 for district
        float incidence4;              // incidence year 4 for district
        float incidence5;              // incidence year 5 for district
        float incidence6;              // incidence year 6 for district
        float incidence7;              // incidence year 7 for district
        float incidence8;              // incidence year 8 for district
        float incidence9;              // incidence year 9 for district
        int inc0Count;                 // no. of incidence appeared
        int inc1Count;                 // no. of incidence appeared
        int inc2Count;                 // no. of incidence appeared
        int inc3Count;                 // no. of incidence appeared
        int inc4Count;                 // no. of incidence appeared
        int inc5Count;                 // no. of incidence appeared
        int inc6Count;                 // no. of incidence appeared
        int inc7Count;                 // no. of incidence appeared
        int inc8Count;                 // no. of incidence appeared
        int inc9Count;                 // no. of incidence appeared
    };

    struct region {

        string name;  
        string disease;
        string test;
        int population;                // population of region  
        float incidence0;              // incidence year 0 for region
        float incidence1;              // incidence year 1 for region
        float incidence2;              // incidence year 2 for region
        float incidence3;              // incidence year 3 for region
        float incidence4;              // incidence year 4 for region
        float incidence5;              // incidence year 5 for region
        float incidence6;              // incidence year 6 for region
        float incidence7;              // incidence year 7 for region
        float incidence8;              // incidence year 8 for region
        float incidence9;              // incidence year 9 for region
        int inc0Count;                 // no. of incidence appeared
        int inc1Count;                 // no. of incidence appeared
        int inc2Count;                 // no. of incidence appeared
        int inc3Count;                 // no. of incidence appeared
        int inc4Count;                 // no. of incidence appeared
        int inc5Count;                 // no. of incidence appeared
        int inc6Count;                 // no. of incidence appeared
        int inc7Count;                 // no. of incidence appeared
        int inc8Count;                 // no. of incidence appeared
        int inc9Count;                 // no. of incidence appeared
    };


    countT = tes.size();
    countD = dis.size();

    // get statistical array for each facility
    tests testStats[countT];

    // get statistical array for each facility
    diseases diseaseStats[countD];

    // get statistical array for all districts - count 6 diseases per district and 2 tests per Disease
    countDistricts = 260 * 6 * 2;
    struct district districts[countDistricts];

    // get statistical array for all regions - count 6 diseases per region and 2 tests per Disease
    countRegions = 16 * 6 * 2;
    struct region regions[countRegions];

    // intialize districts to nothing
    for (int i = 0; i < countDistricts; i++) {

        districts[i].name = "";
        districts[i].disease = "";
        districts[i].test = "";
        districts[i].population = 0;
        districts[i].incidence0 = 0;
        districts[i].incidence1 = 0;
        districts[i].incidence2 = 0;
        districts[i].incidence3 = 0;
        districts[i].incidence4 = 0;
        districts[i].incidence5 = 0;
        districts[i].incidence6 = 0;
        districts[i].incidence7 = 0;
        districts[i].incidence8 = 0;
        districts[i].incidence9 = 0;
        districts[i].inc0Count = 0;
        districts[i].inc1Count = 0;
        districts[i].inc2Count = 0;
        districts[i].inc3Count = 0;
        districts[i].inc4Count = 0;
        districts[i].inc5Count = 0;
        districts[i].inc6Count = 0;
        districts[i].inc7Count = 0;
        districts[i].inc8Count = 0;
        districts[i].inc9Count = 0;
    }
    
    // intialize districts to nothing
    for (int i = 0; i < countRegions; i++) {

        regions[i].name = "";
        regions[i].disease = "";
        regions[i].test = "";
        regions[i].population = 0;
        regions[i].incidence0 = 0;
        regions[i].incidence1 = 0;
        regions[i].incidence2 = 0;
        regions[i].incidence3 = 0;
        regions[i].incidence4 = 0;
        regions[i].incidence5 = 0;
        regions[i].incidence6 = 0;
        regions[i].incidence7 = 0;
        regions[i].incidence8 = 0;
        regions[i].incidence9 = 0;
        regions[i].inc0Count = 0;
        regions[i].inc1Count = 0;
        regions[i].inc2Count = 0;
        regions[i].inc3Count = 0;
        regions[i].inc4Count = 0;
        regions[i].inc5Count = 0;
        regions[i].inc6Count = 0;
        regions[i].inc7Count = 0;
        regions[i].inc8Count = 0;
        regions[i].inc9Count = 0;
    }


    // extract data from tests and store in structure array
    noTests = 0;
    for (auto t : tes) {

        // extract data and store in array
        testStats[noTests].name = t->getTestName();
        testStats[noTests].disease = t->getDiseaseName();
        testStats[noTests].batchWaitTime = t->getBatchWaitTime();
        testStats[noTests].sensitivity = t->getSense();
        testStats[noTests].specificity = t->getSpecif();
        testStats[noTests].sensitivityWithTestMD = t->getSenseWithTestMD();
        testStats[noTests].specificityWithTestMD = t->getSpecifWithTestMD();

        noTests++;
    }

    // extract data from diseases and store in structure array
    noDiseases = 0;
    for (auto d : dis) {

        // extract data and store in array
        diseaseStats[noDiseases].disease = d->getName();
        diseaseStats[noDiseases].prpIncidencePresent = d->getPrpIncPresenting();
        diseaseStats[noDiseases].patientsRatioUninfInf = d->getPatientsRatioUninfInf();
        diseaseStats[noDiseases].perPatientPresentInf = d->getPerPatientPresentInf();
        diseaseStats[noDiseases].initTreatSuccess = d->getInitialTreatmentSuccessRate();
        // get the input parameter value, not the calculated probability
        diseaseStats[noDiseases].initTreatDeath = d->getInitialTreatmentDeathRateIP();
        diseaseStats[noDiseases].probReturns = d->getProbReturn();
        diseaseStats[noDiseases].perPreTestLtoF = d->getPerPreTestLtoF();
        diseaseStats[noDiseases].perPreTreatLtoF = d->getInitialTreatmentLossToFollow();
        // get the input parameter value, not the calculated probability
        diseaseStats[noDiseases].perTreatLtoF = d->getTreatmentLossToFollowIP();
        diseaseStats[noDiseases].representationLtoFTime = d->getTimeUntestedRepresentation();
        diseaseStats[noDiseases].probDeathRepresent = d->getPerRepresentationLtoFDeath();
        diseaseStats[noDiseases].probGeoDist = pRepresentation;
        diseaseStats[noDiseases].relapseSuccess = d->getRelapseSuccessRate();
        diseaseStats[noDiseases].timeDeathRetreat = d->getTimeDeathRetreated();
        diseaseStats[noDiseases].timeUntreatCure = d->getTimeUntreatedCure();
        diseaseStats[noDiseases].probPreTestMDCDInf = d->getPerPreTestMDCDInf();
        diseaseStats[noDiseases].probPreTestMDCDUnInf = d->getPerPreTestMDCDUnInf();
        for (int i = 0; i < 20; i++) {

            diseaseStats[noDiseases].preTestLtof[i].regionName = d->getPerPreTestLtoFTierRegionName(i);
            diseaseStats[noDiseases].preTreatLtof[i].regionName = d->getPreTreatLtoFTierRegionName(i);
            diseaseStats[noDiseases].treatLtof[i].regionName = d->getTreatLtoFTierRegionName(i);

            // adjust for the difference between tiers 1-10 and arrays starting at 0
            for (int j = 0; j < 10; j++) {

                //if (d->getName() == "HIV") std::cout << "dis = " << d->getName() << " rn = " << d->getPerPreTestLtoFTierRegionName(i) << " regionName - " << regionName << " j = " << j << " preTest - " << d->getPerPreTestLtoFTierRegion(d->getPerPreTestLtoFTierRegionName(i), j + 1) << "\n";

                diseaseStats[noDiseases].preTestLtof[i].tier[j] = d->getPerPreTestLtoFTierRegion(d->getPerPreTestLtoFTierRegionName(i), j+1);
                diseaseStats[noDiseases].preTreatLtof[i].tier[j] = d->getPreTreatmentLtoFTierRegion(d->getPerPreTestLtoFTierRegionName(i), j+1);
                diseaseStats[noDiseases].treatLtof[i].tier[j] = d->getTreatmentLtoFTierRegion(d->getPerPreTestLtoFTierRegionName(i), j+1);
            }
      
        }

        noDiseases++;
    }

    //std::cout << "inside resource stats "  << "\n";


    if (statSubDir.length() > 0) {
        subDirBase = "./stats/" + statSubScenarioDir + "/job-" + statSubDir;
    }
    else {
        subDirBase = "./stats/" + statSubScenarioDir + "/job-" + to_string(1);
    }

    // convert to char array
    char subArray[subDirBase.length() + 1];
    // copying the contents of the
    // string to char array
    strcpy(subArray, subDirBase.c_str());


    errno = 0;

    errNo = mkdir(subArray, 0777);

    // check if directory is created or currently exists 
    if ((errNo != 0) && (errno != EEXIST)) {

        //std::cout << "unable to create directory = " << subArray << " errNO = " << errNo << " errno = " << errno << " EEXIST = " << EEXIST << "\n"; 
    }

    myfile.open(subDirBase + "/InputParameters.csv");

    // first dump the random number generator seed
    myfile << "Random.Number.Seed" << "," << seed << "\n";
    //myfile  <<  "\n";
    // myfile << "" << ",Test.name" << ",disease" << ",batch.wait.time" << ",sensitivity" << ",specificity" << ",sensitivity.with.test.MD" << ",specificity.with.test.MD";
    //myfile << ",init.treat.success" << ",init.treat.death" << ",relapse.success" << ",pre.test.ltof" << ",pre.treat.ltof" << ",treat.ltof" << ",ltof.representation.time" << ",ltof.representation.per";
    //myfile << ",incidence.adj" << ",inf.uninf.ratio" << ",MD.clinical.diagnosis.type" << "\n";
    // 
    //infUninfRatio sensitivity specificity MDSensitivity MDSpecificity initTreatSuccess initTreatDeath PreTestLtoF PreTreatLtoF TreatLtoF
    // relapseSuccess probDeathRepresent incidenceAdjustment probRepresentionPostLtuF timeDeathRetreat timeUntreatCure
    myfile << "Tests" << ",Test.name" << ",disease" << ",param.inf.uninf.ratio" << ",param.sensitivity" << ",param.specificity" << ",param.sensitivity.with.test.md" << ",param.specificity.with.test.md";
    myfile << ",param.init.treat.success" << ",param.init.treat.death" << ",param.prop.clinicalDiag.upfront.infected" << ",param.prop.clinicalDiag.upfront.uninfected" << ",param.prob.patient.pre.test.ltof";
    myfile << ",param.prob.patient.pre.treat.ltof" << ",param.prob.patient.treat.ltof" << ",param.relapse.success" << ",param.prob.death.represent";
    myfile << ",param.prp.incidence.presenting" << ",param.prob.geo.dist" << ",param.max.time.death.retreat" << ",param.max.time.untreat.cure";
    if (regLtoFFlag){

        // count no. of regions
        for (int i = 0; i < 16; i++) { 
            
            myfile << ",param.reg" << i + 1 << ".name";

            // output regional loss to follow up values for pretTest preTreat and treat loss to follow up by tier
            for (int j = 1; j < 8; j++) { myfile << ",param.reg" << i + 1 << ".preTestLtoF.tier" << j; }

            for (int j = 1; j < 8; j++) { myfile << ",param.reg" << i + 1 << ".preTreatLtoF.tier" << j; }
            
            for (int j = 1; j < 8; j++) { myfile << ",param.reg" << i + 1 << ".TreatLtoF.tier" << j; }
        }
    }
    myfile << "\n";

    // loop through each test and output data to input parameter file along with disease data
    for (auto t : testStats) {

        for (auto d : diseaseStats) {

            if (t.disease == d.disease) {

                //myfile << "Test" << "," << t.name << "," << t.disease << "," << t.batchWaitTime << "," << t.sensitivity << "," << t.specificity << "," << t.sensitivityWithTestMD << "," << t.specificityWithTestMD;
                //myfile << "," << d.initTreatSuccess << "," <<  d.initTreatDeath << "," <<  d.relapseSuccess << "," <<  d.perPreTestLtoF << "," <<  d.perPreTreatLtoF << "," <<  d.perTreatLtoF << "," <<  d.representationLtoFTime;
                //myfile << "," <<  d.probDeathRepresent << "," << incAdjustment;
                //myfile << "," << d.patientsRatioNonInfInf << "," << MDClinicDiagText << "\n";
                myfile << "Test" << "," << t.name << "," << t.disease << "," << d.patientsRatioUninfInf << "," << t.sensitivity << "," << t.specificity << "," << t.sensitivityWithTestMD << "," << t.specificityWithTestMD;
                myfile << "," << d.initTreatSuccess/100 << "," << d.initTreatDeath/100 << "," << d.probPreTestMDCDInf << "," << d.probPreTestMDCDUnInf << "," << d.perPreTestLtoF/100 << "," << d.perPreTreatLtoF/100 << "," << d.perTreatLtoF/100 << "," << d.relapseSuccess/100 << "," << d.probDeathRepresent/100;
                myfile << "," << d.prpIncidencePresent << "," << d.probGeoDist << "," << d.timeDeathRetreat << "," << d.timeUntreatCure;
                if (regLtoFFlag) {

                    // count no. of regions
                    for (int i = 0; i < 16; i++) {

                        // output regional loss to follow up values for pretTest preTreat and treat loss to follow up by tier
                        regionName = d.preTestLtof[i].regionName;

                        myfile << "," << regionName;
                        
                        for (int j = 0; j < 7; j++) {
                            
                            
                            myfile << "," << d.preTestLtof[i].tier[j];
                        }

                        for (int j = 0; j < 7; j++) { 
                            
                            myfile << "," << d.preTreatLtof[i].tier[j];
                        }

                        for (int j = 0; j < 7; j++) { 
                            
                            myfile << "," << d.treatLtof[i].tier[j];
                        }
                    }
                }
                myfile << "\n";
            }
        }

    }

    // get vector of facilities
    vector<Facility*> facilities = list->getListPtr();
     
     // loop through each array and get population and incidence to create district and region level incidence
    for (auto facility : facilities) {

        facName = facility->getName();
        region = facility->getRegion();
        district = facility->getDistrict();
        population = facility->getPopulation();
        

        // Now loop through facility attributes and put out values
        facAttr = facility->getFacilityAttributesPtr();
        for (auto a : facAttr) {
            
            if (debug) std::cout << "facilites loop " << facName << "," << region << "," << district << "," << a->disease << "," << a->testName << "\n";
            
            // loop through each reagion and see if exists - if so add to current data,
            regionFoundFlag = false;
            
            for (int i = 0; i < countRegions; i++) {

                if ((region == regions[i].name) && (a->disease == regions[i].disease) && (a->testName == regions[i].test)) {

                    if (debug) std::cout << "found match for region, disease test " << regions[i].name << "," << regions[i].disease << "," << regions[i].test << "," << regions[i].incidence0 << "," << a->incidence << "," << regions[i].inc0Count << "\n";

                    // incidence if change of getting disease per person per day
                    regionFoundFlag = true;
                    regions[i].population = regions[i].population + population;
                    yearIncidence = a->incidence;
                    regions[i].incidence0 = regions[i].incidence0 + yearIncidence;
                    regions[i].incidence1 = regions[i].incidence1 + yearIncidence * (1 + (a->incAdjFactor * 1));
                    regions[i].incidence2 = regions[i].incidence2 + yearIncidence * (1 + (a->incAdjFactor * 2));
                    regions[i].incidence3 = regions[i].incidence3 + yearIncidence * (1 + (a->incAdjFactor * 3));
                    regions[i].incidence4 = regions[i].incidence4 + yearIncidence * (1 + (a->incAdjFactor * 4));
                    regions[i].incidence5 = regions[i].incidence5 + yearIncidence * (1 + (a->incAdjFactor * 5));
                    regions[i].incidence6 = regions[i].incidence6 + yearIncidence * (1 + (a->incAdjFactor * 6));
                    regions[i].incidence7 = regions[i].incidence7 + yearIncidence * (1 + (a->incAdjFactor * 7));
                    regions[i].incidence8 = regions[i].incidence8 + yearIncidence * (1 + (a->incAdjFactor * 8));
                    regions[i].incidence9 = regions[i].incidence9 + yearIncidence * (1 + (a->incAdjFactor * 9));
                    regions[i].inc0Count = regions[i].inc0Count + 1;
                    regions[i].inc1Count = regions[i].inc1Count + 1;
                    regions[i].inc2Count = regions[i].inc2Count + 1;
                    regions[i].inc3Count = regions[i].inc3Count + 1;
                    regions[i].inc4Count = regions[i].inc4Count + 1;
                    regions[i].inc5Count = regions[i].inc5Count + 1;
                    regions[i].inc6Count = regions[i].inc6Count + 1;
                    regions[i].inc7Count = regions[i].inc7Count + 1;
                    regions[i].inc8Count = regions[i].inc8Count + 1;
                    regions[i].inc9Count = regions[i].inc9Count + 1;

                    if (debug) std::cout << "new inc = " << regions[i].incidence0 << "," << regions[i].inc0Count << "\n";

                    break;
                }
            }

            // didn't find region/disease/test in array, add it
            if (!regionFoundFlag) {

                if (debug) std::cout << "didn't found match for region, disease test " << region << "," << a->disease << "," << a->testName << "," << a->incidence << "," << regions[actualRegionCount].inc0Count << "\n";
                
                // add it to regions array
                regions[actualRegionCount].name = region;
                regions[actualRegionCount].disease = a->disease;
                regions[actualRegionCount].test = a->testName;

                regions[actualRegionCount].population = population;
                yearIncidence = a->incidence;
                regions[actualRegionCount].incidence0 = yearIncidence;
                regions[actualRegionCount].incidence1 = yearIncidence * (1 + (a->incAdjFactor * 1));
                regions[actualRegionCount].incidence2 = yearIncidence * (1 + (a->incAdjFactor * 2));
                regions[actualRegionCount].incidence3 = yearIncidence * (1 + (a->incAdjFactor * 3));
                regions[actualRegionCount].incidence4 = yearIncidence * (1 + (a->incAdjFactor * 4));
                regions[actualRegionCount].incidence5 = yearIncidence * (1 + (a->incAdjFactor * 5));
                regions[actualRegionCount].incidence6 = yearIncidence * (1 + (a->incAdjFactor * 6));
                regions[actualRegionCount].incidence7 = yearIncidence * (1 + (a->incAdjFactor * 7));
                regions[actualRegionCount].incidence8 = yearIncidence * (1 + (a->incAdjFactor * 8));
                regions[actualRegionCount].incidence9 = yearIncidence * (1 + (a->incAdjFactor * 9));

                regions[actualRegionCount].inc0Count = 1;
                regions[actualRegionCount].inc1Count = 1;
                regions[actualRegionCount].inc2Count = 1;
                regions[actualRegionCount].inc3Count = 1;
                regions[actualRegionCount].inc4Count = 1;
                regions[actualRegionCount].inc5Count = 1;
                regions[actualRegionCount].inc6Count = 1;
                regions[actualRegionCount].inc7Count = 1;
                regions[actualRegionCount].inc8Count = 1;
                regions[actualRegionCount].inc9Count = 1;
                    
                actualRegionCount++;
            }
            
            // loop through each district and see if exists - if so add to current data,
            districtFoundFlag = false;
            
            for (int i = 0; i < countDistricts; i++) {

                if ((district == districts[i].name) && (a->disease == districts[i].disease) && (a->testName == districts[i].test)) {
                    
                    if (debug) std::cout << "found match for district, disease test " << districts[i].name << "," << districts[i].disease << "," << districts[i].test << "," << districts[i].incidence0 << "," << a->incidence << "," << districts[i].inc0Count << "\n";
                    
                    // incidence if change of getting disease per person per day
                    // to get it into common terms incidence = incidence * 365 * 1000000 * (population / 100000)
                    districtFoundFlag = true;
                    districts[i].population = districts[i].population + population;
                    yearIncidence = a->incidence;
                    districts[i].incidence0 = districts[i].incidence0 + yearIncidence;
                    districts[i].incidence1 = districts[i].incidence1 + yearIncidence * (1 + (a->incAdjFactor * 1));
                    districts[i].incidence2 = districts[i].incidence2 + yearIncidence * (1 + (a->incAdjFactor * 2));
                    districts[i].incidence3 = districts[i].incidence3 + yearIncidence * (1 + (a->incAdjFactor * 3));
                    districts[i].incidence4 = districts[i].incidence4 + yearIncidence * (1 + (a->incAdjFactor * 4));
                    districts[i].incidence5 = districts[i].incidence5 + yearIncidence * (1 + (a->incAdjFactor * 5));
                    districts[i].incidence6 = districts[i].incidence6 + yearIncidence * (1 + (a->incAdjFactor * 6));
                    districts[i].incidence7 = districts[i].incidence7 + yearIncidence * (1 + (a->incAdjFactor * 7));
                    districts[i].incidence8 = districts[i].incidence8 + yearIncidence * (1 + (a->incAdjFactor * 8));
                    districts[i].incidence9 = districts[i].incidence9 + yearIncidence * (1 + (a->incAdjFactor * 9));
                    districts[i].inc0Count = districts[i].inc0Count + 1;
                    districts[i].inc1Count = districts[i].inc1Count + 1;
                    districts[i].inc2Count = districts[i].inc2Count + 1;
                    districts[i].inc3Count = districts[i].inc3Count + 1;
                    districts[i].inc4Count = districts[i].inc4Count + 1;
                    districts[i].inc5Count = districts[i].inc5Count + 1;
                    districts[i].inc6Count = districts[i].inc6Count + 1;
                    districts[i].inc7Count = districts[i].inc7Count + 1;
                    districts[i].inc8Count = districts[i].inc8Count + 1;
                    districts[i].inc9Count = districts[i].inc9Count + 1;
   
                    if (debug) std::cout << "new inc = " << districts[i].incidence0 << "," << districts[i].inc0Count << "\n";
                
                    break;
                }
            }

            // didn't find district/disease/test in array, add it
            if (!districtFoundFlag) {
                
                if (debug) std::cout << "didn't found match for district, disease test " << region << "," << a->disease << "," << a->testName << "," << a->incidence << "," << districts[actualRegionCount].inc0Count << "\n";
                
                // add it to regions array
                districts[actualDistrictCount].name = district;
                districts[actualDistrictCount].disease = a->disease;
                districts[actualDistrictCount].test = a->testName;

                districts[actualDistrictCount].population = population;
                yearIncidence = a->incidence;
                districts[actualDistrictCount].incidence0 = yearIncidence;
                districts[actualDistrictCount].incidence1 = yearIncidence * (1 + (a->incAdjFactor * 1));
                districts[actualDistrictCount].incidence2 = yearIncidence * (1 + (a->incAdjFactor * 2));
                districts[actualDistrictCount].incidence3 = yearIncidence * (1 + (a->incAdjFactor * 3));
                districts[actualDistrictCount].incidence4 = yearIncidence * (1 + (a->incAdjFactor * 4));
                districts[actualDistrictCount].incidence5 = yearIncidence * (1 + (a->incAdjFactor * 5));
                districts[actualDistrictCount].incidence6 = yearIncidence * (1 + (a->incAdjFactor * 6));
                districts[actualDistrictCount].incidence7 = yearIncidence * (1 + (a->incAdjFactor * 7));
                districts[actualDistrictCount].incidence8 = yearIncidence * (1 + (a->incAdjFactor * 8));
                districts[actualDistrictCount].incidence9 = yearIncidence * (1 + (a->incAdjFactor * 9));

                districts[actualDistrictCount].inc0Count = 1;
                districts[actualDistrictCount].inc1Count = 1;
                districts[actualDistrictCount].inc2Count = 1;
                districts[actualDistrictCount].inc3Count = 1;
                districts[actualDistrictCount].inc4Count = 1;
                districts[actualDistrictCount].inc5Count = 1;
                districts[actualDistrictCount].inc6Count = 1;
                districts[actualDistrictCount].inc7Count = 1;
                districts[actualDistrictCount].inc8Count = 1;
                districts[actualDistrictCount].inc9Count = 1;
                
                actualDistrictCount++;
            }
        }
    }

    if (debug) std::cout << "actualDistrictCount " << actualDistrictCount << ", actuaRegionCount " << "\n";

    if (debug) {
    
        for (int i = 0; i < actualRegionCount; i++) {

            std::cout << "Region" << "," << regions[i].name << "," << regions[i].population << "," << regions[i].disease << "," << regions[i].test;
            std::cout << "," << regions[i].incidence0 << "," << regions[i].inc0Count << "," << regions[i].incidence1 << "," << regions[i].inc1Count;
            std::cout << "," << regions[i].incidence2 << "," << regions[i].inc2Count << "," << regions[i].incidence3 << "," << regions[i].inc3Count << "\n";
        }
    }
    if (debug) {
    
        for (int i = 0; i < actualDistrictCount; i++) {
            std::cout << "District" << "," << districts[i].name << "," << districts[i].population << "," << districts[i].disease << "," << districts[i].test;
            std::cout << "," << districts[i].incidence0 << "," << districts[i].inc0Count << "," << districts[i].incidence1 << "," << districts[i].inc1Count;
            std::cout << "," << districts[i].incidence2 << "," << districts[i].inc2Count << "," << districts[i].incidence3 << "," << districts[i].inc3Count << "\n";
        }
    }


    // now loop through facilities pulling out incidence, etc.
    //myfile  <<  "\n";
    myfile << "" << ",Facility.name" << ",district" << ",region" <<  ",up.facility" <<",population" << ",disease" << ",test";
    myfile << ",incidence.year" << ",incidence.adjustment.type" << ",incidence.adjustment.factor";
    myfile << ",no.of.machines" << ",no.max.reagents.per.cycle" << ",reagent.cycle.time" <<  "\n";
     
    for (auto facility : facilities) {

        facName = facility->getName();
        upName = facility->getUpName();
region = facility->getRegion();
district = facility->getDistrict();
population = facility->getPopulation();

// Now loop through facility attributes and put out values
facAttr = facility->getFacilityAttributesPtr();
for (auto a : facAttr) {
    myfile << "Facility" << "," << facName << "," << district << "," << region << "," << upName << "," << population << "," << a->disease << "," << a->testName;
    myfile << "," << a->incidence << "," << a->incAdjType << "," << a->incAdjFactor;
    myfile << "," << a->noMachines << "," << a->maxReagentPerCycle << "," << a->timeStepsInCycle << "\n";
}
    }

    // now loop through districts pulling out incidence.
    //myfile  <<  "\n";
    myfile << "" << ",District" << ",population" << ",disease" << ",test" << ",incidence.year0" << ",incidence.year1";
    myfile << ",incidence.year2" << ",incidence.year3" << ",incidence.year4" << ",incidence.year5" << ",incidence.year6" << ",incidence.year7";
    myfile << ",incidence.year8" << ",incidence.year9" << "\n";

    for (int i = 0; i < actualDistrictCount; i++) {

        myfile << "District" << "," << districts[i].name << "," << districts[i].population << "," << districts[i].disease << "," << districts[i].test;
        myfile << "," << districts[i].incidence0 / districts[i].inc0Count << "," << districts[i].incidence1 / districts[i].inc1Count;
        myfile << "," << districts[i].incidence2 / districts[i].inc2Count << "," << districts[i].incidence3 / districts[i].inc3Count;
        myfile << "," << districts[i].incidence4 / districts[i].inc4Count << "," << districts[i].incidence5 / districts[i].inc5Count;
        myfile << "," << districts[i].incidence6 / districts[i].inc6Count << "," << districts[i].incidence7 / districts[i].inc7Count;
        myfile << "," << districts[i].incidence8 / districts[i].inc8Count << "," << districts[i].incidence9 / districts[i].inc9Count << "\n";
    }

    // now loop through regions pulling out incidence.
    //myfile  <<  "\n";
    myfile << "" << ",Region" << ",population" << ",disease" << ",test" << ",incidence.year0" << ",incidence.year1";
    myfile << ",incidence.year2" << ",incidence.year3" << ",incidence.year4" << ",incidence.year5" << ",incidence.year6" << ",incidence.year7";
    myfile << ",incidence.year8" << ",incidence.year9" << "\n";

    for (int i = 0; i < actualRegionCount; i++) {

        myfile << "Region" << "," << regions[i].name << "," << regions[i].population << "," << regions[i].disease << "," << regions[i].test;
        myfile << "," << regions[i].incidence0 / regions[i].inc0Count << "," << regions[i].incidence1 / regions[i].inc1Count;
        myfile << "," << regions[i].incidence2 / regions[i].inc2Count << "," << regions[i].incidence3 / regions[i].inc3Count;
        myfile << "," << regions[i].incidence4 / regions[i].inc4Count << "," << regions[i].incidence5 / regions[i].inc5Count;
        myfile << "," << regions[i].incidence6 / regions[i].inc6Count << "," << regions[i].incidence7 / regions[i].inc7Count;
        myfile << "," << regions[i].incidence8 / regions[i].inc8Count << "," << regions[i].incidence9 / regions[i].inc9Count << "\n";
    }

    myfile.close();


    //std::cout << "insideinput parameter ouput - done "  << "\n";
}

// loop through the active patients and update the infectious day stats
void activePatientInfDays(Disease* dis, FacilitiesList* facList, Stats* stats) {


    int lowerTime = 0;
    int infTime = 0;
    int noActiveInfPatients = 0;
    int count1 = 0;
    int count2 = 0;
    //Diagnosis* d;
    vector<Patient*> p;
    bool debug = false;

    vector<Facility*> facilities = facList->getListPtr();

    if (debug) std::cout << "inside activePatientInfDays " << "\n";

    if (debug) std::cout << "activePatientInfDays. " << currentYearInRun* timestepsPerYear << " patient status ";

    // loop through each facility and the number of Patients and Samples at each facility
    for (auto fac : facilities) {

        // get patient list for facility
        //d = fac->getDiagnosisPtr();
        p = (fac->getDiagnosisPtr())->getPatientPtr();

        if (debug) std::cout << "inside activePatientInfDays facility = " << fac->getName() << "\n";

        // loop through patients at facility and count infectious days for those active and infected
        for (auto patient : p) {

            // now calculate infectious days and update appropriate stat year variable
            if ((patient->getInfected()) && (patient->getActive())) {

                noActiveInfPatients++;

                // infected time should run from when the patient was created until now - which should be last timestep in run
                lowerTime = patient->getTimestepCreated();
                // upperTime = timeStep;

                if (debug) std::cout << "inside activePatientInfDays patient status = " << patient->getStatus() << " , created = " << lowerTime << "\n";

                if ((patient->getStatus() == 1) && (patient->getRepresentTime() < timeStep)) {
                    if (false) std::cout << " ; id " << patient << ", pp " << patient->getPrevStatus() << ", s " << (patient->getSamplePtr())->getStatus() << ", sp " << (patient->getSamplePtr())->getPrevStatus() << ", " << patient->getRepresentTime() << ", " << patient->getFacilityReturnTime() << ", inf " << patient->getInfected() << ", spos " << (patient->getSamplePtr())->isPositive() << ", fac " << patient->getFacility() << ", tf "<< patient->getSampleTestFacility();
                    if ((patient->getSamplePtr())->getStatus() == 0) count1++;
                    if ((patient->getSamplePtr())->getStatus() == 3) count2++;
                }
                else {
                    if (false) std::cout << " ; XXXX s " << (patient->getSamplePtr())->getStatus() << ", sp " << (patient->getSamplePtr())->getPrevStatus() << ", p 1, pp " << patient->getPrevStatus() << ", " << patient->getRepresentTime() << ", " << patient->getFacilityReturnTime();
                    if ((patient->getSamplePtr())->getStatus() == 0) count1++;
                    if ((patient->getSamplePtr())->getStatus() == 3) count2++;
                }

                // debug true
                if (debug) {

                    std::cout << "active patient inf create time = " << lowerTime << ", inf time = " << infTime << ", pstatus = " << patient->getStatus() << ", sstatus = " << (patient->getSamplePtr())->getStatus() << ", sresult = " << (patient->getSamplePtr())->getPositive() << ", return = " << patient->getFacilityReturnTime() << ", represent = " << patient->getRepresentTime() << ", ltof = " << patient->getTimePatientLtoF() << ", reason = " << patient->getReasonNoTreat() << " \n";
                    //std::cout << ", fac = " << patient->getFacility() << ", tfac = " << patient->getSampleTestFacility() << ", sfac = " << (patient->getSamplePtr())->getOrigFacility() << ", stfac = " << (patient->getSamplePtr())->getTestingFacility() << " \n";

                }

                // only count the infectious days in this year - ignore other years. Earlier ones are already counted, 
                // later years will be counted later.  When active patients go inactive, they will be counted again.
                // first find out whether to use patient create time or beginning of year
                if (lowerTime < ((currentYearInRun - 1)* timestepsPerYear)) {
                    
                    lowerTime = (currentYearInRun - 1) * timestepsPerYear;
                }

                // patient infected until end of year
                infTime = (currentYearInRun * timestepsPerYear) - lowerTime;

                // store away in this year
                stats->record_nPatientInfDaysActYear(currentYearInRun - 1, patient->getFacilityDistrictIndex(), patient->getFacilityRegionIndex(), infTime);

                //// now store infect days away in the appropriate year
                //for (int i = 1; i < yearsInRun+1; i++) {

                //    // find no. of timesteps in that year
                //    if (lowerTime < timestepsPerYear * i) {

                //        // lower time is in this year - figure out how many time-steps
                //        infTime = (timestepsPerYear * i) - lowerTime;

                //        // move lower bound to next year
                //        lowerTime = (timestepsPerYear * i) + 1;

                //        stats->record_nPatientInfDaysActYear(i-1, patient->getFacilityDistrictIndex(), patient->getFacilityRegionIndex(), infTime);

                //        if (debug) std::cout << "active patient inf create time = " << lowerTime << ", inf time = " << infTime << ", year = " << i-1 << " \n";
                //    }
                //}
                //stats->record_nPatientInfDaysYear(currentYearInRun - 1, patient->getFacilityDistrictIndex(), patient->getFacilityRegionIndex(), upperTime - lowerTime);
            }
        }

    }
    if (debug) std::cout << " leaving activePatientInfDays. No. of active infected patients = " << noActiveInfPatients << ", createUntest = " << count2 << ", represent = " << count1 << "\n";
}


