//
//  CreateVisualizationStats.cpp
// 
//
//  Created by L. Dombrowski on 6/2/21



#include <vector>
#include <string>
#include <cstring>
#include <sys/stat.h>
#include <errno.h> 

#include "CreateVisualizationStats.h"
#include "GlobalParameters.h"
#include "FacilitiesList.h"
#include "InputOutput.h"
using namespace std;




// count how many Samples currently in existence 
void visualizationStats(FacilitiesList* list, vector<string> diseaseList, bool fileFlag, bool yearly, int timeStep) {

    string agentRegion;
    string sampleRegion;
    string visRegion;
    string agentDistrict;
    string sampleDistrict;
    string visDistrict;
    string agentFacilities;
    string sampleFacilities;
    string visFacilities;
    //Diagnosis* d;
    //Testing* t;
    //vector<Patient*> p;
    //vector<Sample*> s;
    ofstream myfile1;
    ofstream myfile2;
    ofstream myfile3;

    int sampleCount;
    int untreated;
    //int facilityCountS = 0;
    //int count = 0;
    //int status;
    int errNo;
    string diseaseName;
    string subDirBase;
    string subDir;
    string subDirYear;
    string test1Name = "";
    string test2Name = "";
    string test3Name = "";
    string test4Name = "";
    bool debug = false;
    bool allFields = true;

 // keep patient statistics by region and district
    struct sampleStats {
        string name;
        int samples;
        string disease;
        int newSample;
        int newInBatch;
        int inTransit;
        int wait;
        int lost;
        int returnedTransit;
        int returned;
        int reLost;
        int other;
        float avgReturnTime;
        string testName1;
        int testCount1;
        string testName2;
        int testCount2;
        string testName3;
        int testCount3;
        string testName4;
        int testCount4;
        float longitude;
        float latitude;
        string district;
        string region;
        int tier;
        int testing;
        int Xpert;
        string upConnection;
    };

    if (debug) std::cout << "inside create visualization stats fileFlag = " << fileFlag << "\n";

    vector<Facility*> facilities = list->getListPtr();

    // if fileFlag is true, output to a file
    if (fileFlag) {

        if (statSubDir.length() > 0) {
            subDirBase = "./stats/" + statSubScenarioDir + "/job-" + statSubDir;
        }
        else {
            subDirBase = "./stats/" + statSubScenarioDir + "/job-" + to_string(1);
        }
  
        // convert to char array
        char subArray[subDirBase.length() +1];
        // copying the contents of the
        // string to char array
        strcpy(subArray, subDirBase.c_str());


        errno = 0;
            
        errNo = mkdir(subArray, 0777);

        // check if directory is created or currently exists 
        if ((errNo != 0) && (errno != EEXIST)) {

            //std::cout << "unable to create directory = " << subArray << " errNO = " << errNo << " errno = " << errno << " EEXIST = " << EEXIST << "\n"; 
        }

        //std::cout << "inside create visualization stats current year = " << currentYearInRun << " yearly = " << yearly << "\n";

        if (yearly) {

            subDirYear = "year-" + to_string(currentYearInRun-1);
            
            subDir = subDirBase + "/" + subDirYear;
        }
        else {
            
            subDir = subDirBase + "/total";
        }

        // convert to char array
        char subArray4[subDir.length() +1];
        // copying the contents of the
        // string to char array
        strcpy(subArray4, subDir.c_str());
            
        // check if directory is created or not 
        if (mkdir(subArray4,0777) != 0) {
                
            // std::cout << "unable to create directory = " << subArray4 << "\n"; 
        }


        if (debug) std::cout << "inside visualization stats subDir = " << subDir << "\n";


        sampleFacilities = subDir + "/SamplesStats-facilities.csv";
        sampleDistrict = subDir + "/SamplesStats-districts.csv";
        sampleRegion = subDir + "/SamplesStats-regions.csv";
        agentFacilities = subDir + "/AgentStats-Facility.csv";
        agentDistrict = subDir + "/AgentStats-District.csv";
        agentRegion = subDir + "/AgentStats-Region.csv";
        visFacilities = subDir + "/VisualizationStats-facilities.csv";
        visDistrict = subDir + "/VisualizationStats-districts.csv";
        visRegion = subDir + "/VisualizationStats-regions.csv";

        if (debug) std::cout << "inside create visualization bfore open file sampleRegion =  " << sampleRegion <<  "\n";


        myfile1.open(visFacilities);
        myfile2.open(visDistrict);
        myfile3.open(visRegion);

        if (allFields) {
            
            myfile1 << "name" << ",longitude" << ",latitude" << ",district" << ",region" << ",tier" << ",testing";
            myfile1 << ",Xpert" << ",up-connection" << ",time" << ",time.minutes" << ",time.hours" << ",id" << ",agent.population" << ",agent.patientscount" << ",agent.disease" << ",agent.year";
            myfile1 << ",agent.calculatedprevalence" << ",agent.Untested-out" << ",agent.Untested-in" << ",agent.undertreat" << ",agent.failedtreat" << ",agent.untreated" << ",agent.cured";
            myfile1 << ",agent.infec-treat" << ",agent.uninfec-treat" << ",agent.Other" << ",agent.Non-infected" << ",agent.Non-infected-notreturned" << ",agent.dead-infected";
            myfile1 << ",agent.dead-infected-untreated" << ",agent.dead-uninfected" << ",agent.total-infected" << ",agent.PosTest" << ",agent.PosTrue";
            myfile1 << ",agent.Neg.False" << ",agent.MDoverinf" << ",agent.MDoveruninf" << ",agent.Time.to.treatment";
            myfile1 << "facility" << ",samples" << ",disease" << ",New" << ",In.Transit" << ",Wait" << ",Lost" << ",Returned-transit" << ",Returned" << ",Re/Lost";
            myfile1 << ",Other" << ",avg.return.time" << ",test.name1" << ",test.count1" << ",test.name2" << ",test.count2" << ",test.name3" << ",test.count3" << ",test.name4" << ",test.count4" << "\n";
        
            myfile2 << "name" << ",shapefile.code" << ",region.name" << ",missingInAgent?: 3" << ",missingInSample?: 3" << ",agent.population" << ",agent.patientscount" << ",agent.disease" << ",agent.year";
            myfile2 << ",agent.calculatedprevalence" << ",agent.Untested-out" << ",agent.Untested-in" << ",agent.undertreat" << ",agent.failedtreat" << ",agent.untreated" << ",agent.cured";
            myfile2 << ",agent.infec-treat" << ",agent.uninfec-treat" << ",agent.Other" << ",agent.Non-infected" << ",agent.Non-infected-notreturned" << ",agent.dead-infected";
            myfile2 << ",agent.dead-infected-untreated" << ",agent.dead-uninfected" << ",agent.total-infected" << ",agent.PosTest" << ",agent.PosTrue";
            myfile2 << ",agent.Neg.False" << ",agent.MDoverinf" << ",agent.MDoveruninf" << ",agent.Time.to.treatment";
            myfile2 << "facility" << ",samples" << ",disease" << ",New" << ",In.Transit" << ",Wait" << ",Lost" << ",Returned-transit" << ",Returned" << ",Re/Lost";
            myfile2 << ",Other" << ",avg.return.time" << ",test.name1" << ",test.count1" << ",test.name2" << ",test.count2" << ",test.name3" << ",test.count3" << ",test.name4" << ",test.count4" << "\n";


            myfile3 << "name" << ",shapefile.code" << ",missingInAgent?: 0" << ",missingInSample?: 0" << ",agent.population" << ",agent.patientscount" << ",agent.disease" << ",agent.year";
            myfile3 << ",agent.calculatedprevalence" << ",agent.Untested-out" << ",agent.Untested-in" << ",agent.undertreat" << ",agent.failedtreat" << ",agent.untreated" << ",agent.cured";
            myfile3 << ",agent.infec-treat" << ",agent.uninfec-treat" << ",agent.Other" << ",agent.Non-infected" << ",agent.Non-infected-notreturned" << ",agent.dead-infected";
            myfile3 << ",agent.dead-infected-untreated" << ",agent.dead-uninfected" << ",agent.total-infected" << ",agent.PosTest" << ",agent.PosTrue";
            myfile3 << ",agent.Neg.False" << ",agent.MDoverinf" << ",agent.MDoveruninf" << ",agent.Time.to.treatment";
            myfile3 <<",sample.samples" << ",sample.disease" << ",sample.New" << ",sample.InTransit=" << ",sample.Wait" << ",sample.Lost" << ",sample.Returned-transit" << ",sample.Returned" << ",sample.Re/Lost";
            myfile3 << ",sample.Other" << ",sample.avg.returntime" << ",sample.test.name1" << ",sample.test.count1" << ",sample.test.name2" << ",sample.test.count2" << ",sample.test.name3" << ",sample.test.count3";
            myfile3 << ",sample.test.name4" << ",sample.test.count4" << "\n";
        }
        else {
            myfile1 << "name" << ",longitude" << ",latitude" << ",district" << ",region" << ",tier" << ",testing";
            myfile1 << ",Xpert" << ",up-connection" << ",time" << ",time.minutes" << ",time.hours" << ",id" << ",agent.population" << ",agent.patientscount" << ",agent.disease" << ",agent.year";
            myfile1 << ",agent.calculatedprevalence" << ",agent.Untested-out" << ",agent.Untested-in" << ",agent.undertreat" << ",agent.failedtreat" << ",agent.untreated" << ",agent.cured";
            myfile1 << ",agent.uninfec-treat" << ",agent.Other" << ",agent.Non-infected" << ",agent.Non-infected-notreturned" << ",agent.dead-infected" << ",agent.dead-uninfected" << ",agent.total-infected";
            myfile1 << ",agent.PosTest" << ",agent.MDoverinf" << ",agent.MDoveruninf";
            myfile1 << "facility" << ",samples" << ",disease" << ",New" << ",In.Transit" << ",Wait" << ",Lost" << ",Returned-transit" << ",Returned" << ",Re/Lost";
            myfile1 << ",Other" << ",avg.return.time" << ",test.name1" << ",test.count1" << ",test.name2" << ",test.count2" << ",test.name3" << ",test.count3" << ",test.name4" << ",test.count4" << "\n";
        
            myfile2 << "name" << ",shapefile.code" << ",region.name" << ",missingInAgent?: 3" << ",missingInSample?: 3" << ",agent.population" << ",agent.patientscount" << ",agent.disease" << ",agent.year";
            myfile2 << ",agent.calculatedprevalence" << ",agent.Untested-out" << ",agent.Untested-in" << ",agent.undertreat" << ",agent.failedtreat" << ",agent.untreated" << ",agent.cured";
            myfile2 << ",agent.uninfec-treat" << ",agent.Other" << ",agent.Non-infected" << ",agent.Non-infected-notreturned" << ",agent.dead-infected" << ",agent.dead-uninfected" << ",agent.total-infected";
            myfile2 << ",agent.PosTest" << ",agent.MDoverinf" << ",agent.MDoveruninf";
            myfile2 << "facility" << ",samples" << ",disease" << ",New" << ",In.Transit" << ",Wait" << ",Lost" << ",Returned-transit" << ",Returned" << ",Re/Lost";
            myfile2 << ",Other" << ",avg.return.time" << ",test.name1" << ",test.count1" << ",test.name2" << ",test.count2" << ",test.name3" << ",test.count3" << ",test.name4" << ",test.count4" << "\n";


            myfile3 << "name" << ",shapefile.code" << ",missingInAgent?: 0" << ",missingInSample?: 0" << ",agent.population" << ",agent.patientscount" << ",agent.disease" << ",agent.year";
            myfile3 << ",agent.calculatedprevalence" << ",agent.Untested-out" << ",agent.Untested-in" << ",agent.undertreat" << ",agent.failedtreat" << ",agent.untreated" << ",agent.cured";
            myfile3 << ",agent.uninfec-treat" << ",agent.Other" << ",agent.Non-infected" << ",agent.Non-infected-notreturned" << ",agent.dead-infected" << ",agent.dead-uninfected" << ",agent.total-infected";
            myfile3 << ",agent.PosTest" << ",agent.MDoverinf" << ",agent.MDoveruninf";
            myfile3 <<",sample.samples" << ",sample.disease" << ",sample.New" << ",sample.InTransit=" << ",sample.Wait" << ",sample.Lost" << ",sample.Returned-transit" << ",sample.Returned" << ",sample.Re/Lost";
            myfile3 << ",sample.Other" << ",sample.avg.returntime" << ",sample.test.name1" << ",sample.test.count1" << ",sample.test.name2" << ",sample.test.count2" << ",sample.test.name3" << ",sample.test.count3";
            myfile3 << ",sample.test.name4" << ",sample.test.count4" << "\n";
        }  
    }

    if (debug) std::cout << "inside create visualization before read facility files sampleFacilities = " <<  sampleFacilities <<  ", agentfacilities = " << agentFacilities <<  "\n";
    
//    // read in facility agent and sample stats
//    vector<vector<string>> sFacility = readDataInput(sampleFacilities);
//    if (sFacility.size() < 2) {
//
//        std::cout << "inside create visualization eror reading facility sample files " << sampleFacilities <<  "\n";
//        return;
//    }
//    vector<vector<string>> aFacility = readDataInput(agentFacilities);
//    if (aFacility.size() < 2) {
//
//        std::cout << "inside create visualization eror reading facility agent files " << agentFacilities <<  "\n";
//        return;
//    }
//    if (debug) std::cout << "inside create visualization after read facility files " <<   "\n";
//
//
//    // use this as an count to create an array for samples
//    sampleCount = sFacility.size();
//    sampleStats samples[sampleCount];
//
//    
//    // load sample data from file - skip header row - we have already made a new one
//    for (auto i = 1u; i < sFacility.size(); i++) {
//;
//        samples[i-1].name = sFacility[i][0];
//        samples[i-1].samples = stoi(sFacility[i][1]);
//        samples[i-1].disease = sFacility[i][2];
//        samples[i-1].newSample = stoi(sFacility[i][3]);
//        samples[i-1].newInBatch = stoi(sFacility[i][4]);
//        samples[i-1].inTransit = stoi(sFacility[i][5]);
//        samples[i-1].wait = stoi(sFacility[i][6]);
//        samples[i-1].lost = stoi(sFacility[i][7]);
//        samples[i-1].returnedTransit = stoi(sFacility[i][8]);
//        samples[i-1].returned = stoi(sFacility[i][9]);
//        samples[i-1].reLost = stoi(sFacility[i][10]);
//        samples[i-1].other = stoi(sFacility[i][11]);
//        samples[i-1].avgReturnTime = stof(sFacility[i][12]);
//        samples[i-1].testName1 = sFacility[i][13];
//        samples[i-1].testCount1 = stoi(sFacility[i][14]);
//        samples[i-1].testName2 = sFacility[i][15];
//        samples[i-1].testCount2 = stoi(sFacility[i][16]);
//        samples[i-1].testName3 = sFacility[i][17];
//        samples[i-1].testCount3 = stoi(sFacility[i][18]);
//        samples[i-1].testName4 = sFacility[i][19];
//        samples[i-1].testCount4 = stoi(sFacility[i][20]);
//
//        // now fill in rest of list with facility information
//        for (auto fac : facilities) {
//
//            if (samples[i-1].name == fac->getName()) {
//                samples[i-1].longitude = fac->getLongitude();
//                samples[i-1].latitude = fac->getLatitude();
//                samples[i-1].district = fac->getDistrict();
//                samples[i-1].region = fac->getRegion();
//                samples[i-1].tier = fac->getTier();
//                if (fac->testingAvailableAny("TB")) {
//                    samples[i-1].testing = 1;
//                }
//                else {
//                    samples[i-1].testing = 0;
//                }
//                if (fac->getXpert()){
//                    samples[i-1].Xpert = 1;
//                }
//                else {
//                    samples[i-1].Xpert = 0;
//                }
//                samples[i-1].upConnection = fac->getUpName();
//            }
//
//        }
//
//    }
//
//    
//    
//    
//    if (debug) std::cout << "inside create visualization after load sample structure " <<   "\n";
//
//    // now read in agent data and store directly into visualization file
//    // load sample data from file - skip header row - we have already made a new one
//    for (auto i = 1u; i < aFacility.size(); i++) {
//
//        if (aFacility[i][0] != "Total") {
//
//            // now find corresponding sample data and output it
//            for (auto j = 0u; j < sFacility.size() - 1; j++) {
//
//                if (aFacility[i][0] == samples[j].name) {
//            
//                    untreated = stoi(aFacility[i][47]) + stoi(aFacility[i][48]) - stoi(aFacility[i][51]);
//
//                    if (debug) std::cout << "name the same " << samples[j].name <<  "\n";
//                    myfile1 << aFacility[i][0] << "," << samples[j].longitude << "," << samples[j].latitude << "," << samples[j].district << "," << samples[j].region << "," << samples[j].tier << "," << samples[j].testing;
//                    myfile1 << "," << samples[j].Xpert << "," << samples[j].upConnection << ",0" << ",0" << ",0";
//                    if (allFields) {
//
//                        myfile1 << "," << stoi(aFacility[i][1]) << "," << stoi(aFacility[i][8]) << "," << stoi(aFacility[i][13]) + stoi(aFacility[i][15]) << "," << aFacility[i][10] << "," << stoi(aDistrict[i][4]) << "," << stoi(aFacility[i][11]);
//                        myfile1 << "," << stof(aFacility[i][18]) << "," << stoi(aFacility[i][19]) << "," << stoi(aFacility[i][98]) << "," << stoi(aFacility[i][103]) + stoi(aFacility[i][102]) << "," << untreated << "," << stoi(aFacility[i][101]) << "," << stoi(aFacility[i][99]) << "," << stoi(aFacility[i][100]);
//                        myfile1 << "," << "0" << "," << (stoi(aFacility[i][13]) + stoi(aFacility[i][15])) - stoi(aFacility[i][14]) << "," << stoi(aFacility[i][73]) - stoi(aFacility[i][74]) << "," << stoi(aFacility[i][75]) + stoi(aFacility[i][80]) + stoi(aFacility[i][89]) + stoi(aFacility[i][102]) + stoi(aFacility[i][106]);
//                        myfile1 << "," << stoi(aFacility[i][75]) + stoi(aFacility[i][80]) + stoi(aFacility[i][89]) << "," << "0" << "," << stoi(aFacility[i][14]);
//                        myfile1 << "," << stoi(aFacility[i][35]) << "," << stoi(aFacility[i][37]) + stoi(aFacility[i][38]) << "," << stoi(aFacility[i][42]) << "," << stoi(aFacility[i][51]) << "," << stoi(aFacility[i][52]) << "," << stoi(aFacility[i][107]); 
//                    }
//                    else {
//                        myfile1 << "," << stoi(aFacility[i][1]) << "," << stoi(aFacility[i][3]) << "," << stoi(aFacility[i][4]) << "," << aFacility[i][5];
//                        myfile1 << "," << stof(aFacility[i][6]) << "," << stoi(aFacility[i][7]) << "," << stoi(aFacility[i][8]) << "," << stoi(aFacility[i][9]) << "," << stoi(aFacility[i][10]) << "," << untreated << "," << stoi(aFacility[i][14]);
//                        myfile1 << "," << stoi(aFacility[i][16]) << "," << stoi(aFacility[i][17]) << "," << stoi(aFacility[i][18]) << "," << stoi(aFacility[i][19]) << "," << stoi(aFacility[i][20]) << "," << stoi(aFacility[i][22]) << "," << stoi(aFacility[i][23]);
//                        myfile1 << "," << stoi(aFacility[i][24]) << "," << stoi(aFacility[i][27])  << "," << stoi(aFacility[i][28]);
//                    }
//
//                    myfile1 <<"," << samples[j].samples << "," << samples[j].disease << "," << samples[j].newSample << "," << samples[j].inTransit << "," << samples[j].wait << "," << samples[j].lost << "," << samples[j].returnedTransit << "," << samples[j].returned << "," << samples[j].reLost;
//                    myfile1 << "," << samples[j].other << "," << samples[j].avgReturnTime << "," << samples[j].testName1 << "," << samples[j].testCount1 << "," << samples[j].testName2 << "," << samples[j].testCount2 << "," << samples[j].testName3 << "," << samples[j].testCount3;
//                    myfile1 << "," << samples[j].testName4 << "," << samples[j].testCount4 << "\n";
//                
//                    if (debug) std::cout << "after name the same " << samples[j].name <<  "\n";
//                }
//            }
//        }
//    }
//    if (debug) std::cout << "inside create visualization after writing facility file out" <<   "\n";




    // read in samples and agent district data an merge them
    vector<vector<string>> sDistrict = readDataInput(sampleDistrict);
    if (sDistrict.size() < 2) {

        std::cout << "inside create visualization eror reading district sample files " << sampleDistrict <<  "\n";
        return;
    }
    vector<vector<string>> aDistrict = readDataInput(agentDistrict);
    if (aDistrict.size() < 2) {

        std::cout << "inside create visualization eror reading district agent files " << agentDistrict <<  "\n";
        return;
    }

    // use this as an count to create an array for samples
    sampleCount = sDistrict.size();
    sampleStats samplesD[sampleCount];

    
    // load sample data from file - skip header row - we have already made a new one
    for (auto i = 1u; i < sDistrict.size(); i++) {
;
        samplesD[i-1].name = sDistrict[i][0];
        samplesD[i-1].samples = stoi(sDistrict[i][1]);
        samplesD[i-1].disease = sDistrict[i][2];
        samplesD[i-1].newSample = stoi(sDistrict[i][3]);
        samplesD[i-1].newInBatch = stoi(sDistrict[i][4]);
        samplesD[i-1].inTransit = stoi(sDistrict[i][5]);
        samplesD[i-1].wait = stoi(sDistrict[i][6]);
        samplesD[i-1].lost = stoi(sDistrict[i][7]);
        samplesD[i-1].returnedTransit = stoi(sDistrict[i][8]);
        samplesD[i-1].returned = stoi(sDistrict[i][9]);
        samplesD[i-1].reLost = stoi(sDistrict[i][10]);
        samplesD[i-1].other = stoi(sDistrict[i][11]);
        samplesD[i-1].avgReturnTime = stof(sDistrict[i][12]);
        samplesD[i-1].testName1 = sDistrict[i][13];
        samplesD[i-1].testCount1 = stoi(sDistrict[i][14]);
        samplesD[i-1].testName2 = sDistrict[i][15];
        samplesD[i-1].testCount2 = stoi(sDistrict[i][16]);
        samplesD[i-1].testName3 = sDistrict[i][17];
        samplesD[i-1].testCount3 = stoi(sDistrict[i][18]);
        samplesD[i-1].testName4 = sDistrict[i][19];
        samplesD[i-1].testCount4 = stoi(sDistrict[i][20]);

       if(samplesD[i-1].name == "ADENTA MUNICIPAL") { samplesD[i-1].district = "305"; samplesD[i-1].region = "GREATER ACCRA"; }
       if(samplesD[i-1].name == "LEDZOKUKU MUNICIPAL") { samplesD[i-1].district = "306"; samplesD[i-1].region = "GREATER ACCRA"; }
       if(samplesD[i-1].name == "CHEREPONI") { samplesD[i-1].district = "817"; samplesD[i-1].region = "NORTHERN EAST"; }
       if(samplesD[i-1].name == "WA EAST") { samplesD[i-1].district = "1003"; samplesD[i-1].region = "UPPER WEST"; }
       if(samplesD[i-1].name == "WA MUNICIPAL") { samplesD[i-1].district = "1002"; samplesD[i-1].region = "UPPER WEST"; }
       if(samplesD[i-1].name == "SABOBA") { samplesD[i-1].district = "816"; samplesD[i-1].region = "NORTHERN"; }
       if(samplesD[i-1].name == "JIRAPA") { samplesD[i-1].district = "1006"; samplesD[i-1].region = "UPPER WEST"; }
       if(samplesD[i-1].name == "GUSHEGU") { samplesD[i-1].district = "815"; samplesD[i-1].region = "NORTHERN"; }
       if(samplesD[i-1].name == "SISSALA WEST") { samplesD[i-1].district = "1007"; samplesD[i-1].region = "UPPER WEST"; }
       if(samplesD[i-1].name == "NANUMBA NORTH") { samplesD[i-1].district = "808"; samplesD[i-1].region = "NORTHERN"; }
       if(samplesD[i-1].name == "LAMBUSSIE-KARNI") { samplesD[i-1].district = "1008"; samplesD[i-1].region = "UPPER WEST"; }
       if(samplesD[i-1].name == "BOLGATANGA MUNICIPAL") { samplesD[i-1].district = "904"; samplesD[i-1].region = "UPPER EAST"; }
       if(samplesD[i-1].name == "NANUMBA SOUTH") { samplesD[i-1].district = "807"; samplesD[i-1].region = "NORTHERN"; }
       if(samplesD[i-1].name == "WENCHI MUNICIPAL") { samplesD[i-1].district = "714"; samplesD[i-1].region = "BONO"; }
       if(samplesD[i-1].name == "BONGO") { samplesD[i-1].district = "906"; samplesD[i-1].region = "UPPER EAST"; }
       if(samplesD[i-1].name == "TANO SOUTH MUNICIPAL") { samplesD[i-1].district = "706"; samplesD[i-1].region = "AHAFO"; }
       if(samplesD[i-1].name == "WA WEST") { samplesD[i-1].district = "1001"; samplesD[i-1].region = "UPPER WEST"; }
       if(samplesD[i-1].name == "SUNYANI MUNICIPAL") { samplesD[i-1].district = "708"; samplesD[i-1].region = "BONO"; }
       if(samplesD[i-1].name == "SAWLA-TUNA-KALBA") { samplesD[i-1].district = "802"; samplesD[i-1].region = "SAVANNAH"; }
       if(samplesD[i-1].name == "ASANTE AKIM SOUTH") { samplesD[i-1].district = "609"; samplesD[i-1].region = "ASHANTI"; }
       if(samplesD[i-1].name == "SISSALA EAST") { samplesD[i-1].district = "1004"; samplesD[i-1].region = "UPPER WEST"; }
       if(samplesD[i-1].name == "ADA EAST") { samplesD[i-1].district = "310"; samplesD[i-1].region = "GREATER ACCRA"; }
       if(samplesD[i-1].name == "SHAI OSUDOKU") { samplesD[i-1].district = "309"; samplesD[i-1].region = "GREATER ACCRA"; }
       if(samplesD[i-1].name == "KASENA NANKANA EAST") { samplesD[i-1].district = "903"; samplesD[i-1].region = "UPPER EAST"; }
       if(samplesD[i-1].name == "ADA WEST") { samplesD[i-1].district = "316"; samplesD[i-1].region = "GREATER ACCRA"; }
       if(samplesD[i-1].name == "BUNKPURUGU NAKPANDURI") { samplesD[i-1].district = "818"; samplesD[i-1].region = "NORTHERN EAST"; }
       if(samplesD[i-1].name == "GARU") { samplesD[i-1].district = "908"; samplesD[i-1].region = "UPPER EAST"; }
       if(samplesD[i-1].name == "NINGO/PRAMPRAM") { samplesD[i-1].district = "315"; samplesD[i-1].region = "GREATER ACCRA"; }
       if(samplesD[i-1].name == "EAST MAMPRUSI") { samplesD[i-1].district = "819"; samplesD[i-1].region = "NORTHERN EAST"; }
       if(samplesD[i-1].name == "LA DADE-KOTOPON") { samplesD[i-1].district = "312"; samplesD[i-1].region = "GREATER ACCRA"; }
       if(samplesD[i-1].name == "BAWKU WEST") { samplesD[i-1].district = "907"; samplesD[i-1].region = "UPPER EAST"; }
       if(samplesD[i-1].name == "AOWIN") { samplesD[i-1].district = "112"; samplesD[i-1].region = "WESTERN NORTH"; }
       if(samplesD[i-1].name == "DORMAA EAST") { samplesD[i-1].district = "705"; samplesD[i-1].region = "BONO"; }
       if(samplesD[i-1].name == "SUAMAN") { samplesD[i-1].district = "120"; samplesD[i-1].region = "WESTERN NORTH"; }
       if(samplesD[i-1].name == "BEREKUM EAST MUNICIPAL") { samplesD[i-1].district = "710"; samplesD[i-1].region = "BONO"; }
       if(samplesD[i-1].name == "BIA EAST") { samplesD[i-1].district = "122"; samplesD[i-1].region = "WESTERN NORTH"; }
       if(samplesD[i-1].name == "JAMAN SOUTH MUNICIPAL") { samplesD[i-1].district = "711"; samplesD[i-1].region = "BONO"; }
       if(samplesD[i-1].name == "BIA WEST") { samplesD[i-1].district = "117"; samplesD[i-1].region = "WESTERN NORTH"; }
       if(samplesD[i-1].name == "SUNYANI WEST") { samplesD[i-1].district = "709"; samplesD[i-1].region = "BONO"; }
       if(samplesD[i-1].name == "ASUNAFO NORTH MUNICIPAL") { samplesD[i-1].district = "702"; samplesD[i-1].region = "AHAFO"; }
       if(samplesD[i-1].name == "BODI") { samplesD[i-1].district = "121"; samplesD[i-1].region = "WESTERN NORTH"; }
       if(samplesD[i-1].name == "JUABOSO") { samplesD[i-1].district = "116"; samplesD[i-1].region = "WESTERN NORTH"; }
       if(samplesD[i-1].name == "NKORANZA NORTH") { samplesD[i-1].district = "717"; samplesD[i-1].region = "BONO EAST"; }
       if(samplesD[i-1].name == "KINTAMPO SOUTH") { samplesD[i-1].district = "721"; samplesD[i-1].region = "BONO EAST"; }
       if(samplesD[i-1].name == "SHAMA") { samplesD[i-1].district = "106"; samplesD[i-1].region = "WESTERN"; }
       if(samplesD[i-1].name == "TANO NORTH MUNICIPAL") { samplesD[i-1].district = "707"; samplesD[i-1].region = "AHAFO"; }
       if(samplesD[i-1].name == "WASSA EAST") { samplesD[i-1].district = "107"; samplesD[i-1].region = "WESTERN"; }
       if(samplesD[i-1].name == "KETU NORTH") { samplesD[i-1].district = "404"; samplesD[i-1].region = "VOLTA"; }
       if(samplesD[i-1].name == "MPOHOR") { samplesD[i-1].district = "118"; samplesD[i-1].region = "WESTERN"; }
       if(samplesD[i-1].name == "WASSA AMENFI WEST") { samplesD[i-1].district = "111"; samplesD[i-1].region = "WESTERN"; }
       if(samplesD[i-1].name == "KETU SOUTH") { samplesD[i-1].district = "403"; samplesD[i-1].region = "VOLTA"; }
       if(samplesD[i-1].name == "WASSA AMENFI CENTRAL") { samplesD[i-1].district = "119"; samplesD[i-1].region = "WESTERN"; }
       if(samplesD[i-1].name == "KETA MUNICIPAL") { samplesD[i-1].district = "402"; samplesD[i-1].region = "VOLTA"; }
       if(samplesD[i-1].name == "KADJEBI") { samplesD[i-1].district = "414"; samplesD[i-1].region = "OTI"; }
       if(samplesD[i-1].name == "MFANTSEMAN MUNICIPAL") { samplesD[i-1].district = "204"; samplesD[i-1].region = "CENTRAL"; }
       if(samplesD[i-1].name == "SOUTH TONGU") { samplesD[i-1].district = "401"; samplesD[i-1].region = "VOLTA"; }
       if(samplesD[i-1].name == "EKUMFI") { samplesD[i-1].district = "219"; samplesD[i-1].region = "CENTRAL"; }
       if(samplesD[i-1].name == "TWIFO HEMANG LOWER DENKYIRA") { samplesD[i-1].district = "218"; samplesD[i-1].region = "CENTRAL"; }
       if(samplesD[i-1].name == "JASIKAN") { samplesD[i-1].district = "413"; samplesD[i-1].region = "OTI"; }
       if(samplesD[i-1].name == "TWIFO ATTI-MORKWA") { samplesD[i-1].district = "215"; samplesD[i-1].region = "CENTRAL"; }
       if(samplesD[i-1].name == "KRACHI EAST MUNICIPAL") { samplesD[i-1].district = "415"; samplesD[i-1].region = "OTI"; }
       if(samplesD[i-1].name == "AWUTU SENYA WEST") { samplesD[i-1].district = "209"; samplesD[i-1].region = "CENTRAL"; }
       if(samplesD[i-1].name == "NKWANTA SOUTH MUNICIPAL") { samplesD[i-1].district = "417"; samplesD[i-1].region = "OTI"; }
       if(samplesD[i-1].name == "AWUTU SENYA EAST") { samplesD[i-1].district = "220"; samplesD[i-1].region = "CENTRAL"; }
       if(samplesD[i-1].name == "AFIGYA KWABRE SOUTH") { samplesD[i-1].district = "619"; samplesD[i-1].region = "ASHANTI"; }
       if(samplesD[i-1].name == "LA-NKWANTANANG-MADINA") { samplesD[i-1].district = "313"; samplesD[i-1].region = "GREATER ACCRA"; }
       if(samplesD[i-1].name == "ATWIMA KWANWOMA") { samplesD[i-1].district = "613"; samplesD[i-1].region = "ASHANTI"; }
       if(samplesD[i-1].name == "SEKYERE SOUTH") { samplesD[i-1].district = "621"; samplesD[i-1].region = "ASHANTI"; }
       if(samplesD[i-1].name == "GA EAST") { samplesD[i-1].district = "303"; samplesD[i-1].region = "GREATER ACCRA"; }
       if(samplesD[i-1].name == "AYAWASO WEST") { samplesD[i-1].district = "304"; samplesD[i-1].region = "GREATER ACCRA"; }
       if(samplesD[i-1].name == "SEKYERE EAST") { samplesD[i-1].district = "623"; samplesD[i-1].region = "ASHANTI"; }
       if(samplesD[i-1].name == "GA SOUTH MUNICIPAL") { samplesD[i-1].district = "301"; samplesD[i-1].region = "GREATER ACCRA"; }
       if(samplesD[i-1].name == "GA WEST MUNICIPAL") { samplesD[i-1].district = "302"; samplesD[i-1].region = "GREATER ACCRA"; }
       if(samplesD[i-1].name == "SEKYERE CENTRAL") { samplesD[i-1].district = "625"; samplesD[i-1].region = "ASHANTI"; }
       if(samplesD[i-1].name == "GA CENTRAL MUNICIPAL") { samplesD[i-1].district = "311"; samplesD[i-1].region = "GREATER ACCRA"; }
       if(samplesD[i-1].name == "OFFINSO NORTH") { samplesD[i-1].district = "627"; samplesD[i-1].region = "ASHANTI"; }
       if(samplesD[i-1].name == "KRACHI WEST") { samplesD[i-1].district = "416"; samplesD[i-1].region = "OTI"; }
       if(samplesD[i-1].name == "OFFINSO MUNICIPAL") { samplesD[i-1].district = "618"; samplesD[i-1].region = "ASHANTI"; }
       if(samplesD[i-1].name == "OBUASI MUNICIPAL") { samplesD[i-1].district = "605"; samplesD[i-1].region = "ASHANTI"; }
       if(samplesD[i-1].name == "KRACHI NCHUMURU") { samplesD[i-1].district = "425"; samplesD[i-1].region = "OTI"; }
       if(samplesD[i-1].name == "KWABRE EAST") { samplesD[i-1].district = "620"; samplesD[i-1].region = "ASHANTI"; }
       if(samplesD[i-1].name == "AGOTIME ZIOPE") { samplesD[i-1].district = "407"; samplesD[i-1].region = "VOLTA"; }
       if(samplesD[i-1].name == "ADAKLU") { samplesD[i-1].district = "421"; samplesD[i-1].region = "VOLTA"; }
       if(samplesD[i-1].name == "EJURA-SEKYEDUMASE") { samplesD[i-1].district = "626"; samplesD[i-1].region = "ASHANTI"; }
       if(samplesD[i-1].name == "ATWIMA NWABIAGYA SOUTH") { samplesD[i-1].district = "615"; samplesD[i-1].region = "ASHANTI"; }
       if(samplesD[i-1].name == "AKATSI SOUTH") { samplesD[i-1].district = "405"; samplesD[i-1].region = "VOLTA"; }
       if(samplesD[i-1].name == "ATWIMA MPONUA") { samplesD[i-1].district = "601"; samplesD[i-1].region = "ASHANTI"; }
       if(samplesD[i-1].name == "AKATSI NORTH") { samplesD[i-1].district = "420"; samplesD[i-1].region = "VOLTA"; }
       if(samplesD[i-1].name == "HO MUNICIPAL") { samplesD[i-1].district = "408"; samplesD[i-1].region = "VOLTA"; }
       if(samplesD[i-1].name == "ADANSI SOUTH") { samplesD[i-1].district = "604"; samplesD[i-1].region = "ASHANTI"; }
       if(samplesD[i-1].name == "HOHOE MUNICIPAL") { samplesD[i-1].district = "411"; samplesD[i-1].region = "VOLTA"; }
       if(samplesD[i-1].name == "ADANSI ASOKWA") { samplesD[i-1].district = "606"; samplesD[i-1].region = "ASHANTI"; }
       if(samplesD[i-1].name == "BOSOME FREHO") { samplesD[i-1].district = "608"; samplesD[i-1].region = "ASHANTI"; }
       if(samplesD[i-1].name == "AFADZATO SOUTH") { samplesD[i-1].district = "423"; samplesD[i-1].region = "VOLTA"; }
       if(samplesD[i-1].name == "AHAFO ANO SOUTH EAST") { samplesD[i-1].district = "616"; samplesD[i-1].region = "ASHANTI"; }
       if(samplesD[i-1].name == "NORTH DAYI") { samplesD[i-1].district = "424"; samplesD[i-1].region = "VOLTA"; }
       if(samplesD[i-1].name == "BEKWAI MUNICIPAL") { samplesD[i-1].district = "607"; samplesD[i-1].region = "ASHANTI"; }
       if(samplesD[i-1].name == "KPANDO MUNICIPAL") { samplesD[i-1].district = "410"; samplesD[i-1].region = "VOLTA"; }
       if(samplesD[i-1].name == "BOSOMTWE") { samplesD[i-1].district = "612"; samplesD[i-1].region = "ASHANTI"; }
       if(samplesD[i-1].name == "NORTH TONGU") { samplesD[i-1].district = "419"; samplesD[i-1].region = "VOLTA"; }
       if(samplesD[i-1].name == "CENTRAL TONGU") { samplesD[i-1].district = "406"; samplesD[i-1].region = "VOLTA"; }
       if(samplesD[i-1].name == "MAMPONG MUNICIPAL") { samplesD[i-1].district = "622"; samplesD[i-1].region = "ASHANTI"; }
       if(samplesD[i-1].name == "NSAWAM ADOAGYIRE MUNICIPAL") { samplesD[i-1].district = "505"; samplesD[i-1].region = "EASTERN"; }
       if(samplesD[i-1].name == "AJUMAKO-ENYAN-ESSIAM") { samplesD[i-1].district = "205"; samplesD[i-1].region = "CENTRAL"; }
       if(samplesD[i-1].name == "GOMOA WEST") { samplesD[i-1].district = "206"; samplesD[i-1].region = "CENTRAL"; }
       if(samplesD[i-1].name == "AKWAPEM SOUTH") { samplesD[i-1].district = "523"; samplesD[i-1].region = "EASTERN"; }
       if(samplesD[i-1].name == "AGONA WEST MUNICIPAL") { samplesD[i-1].district = "211"; samplesD[i-1].region = "CENTRAL"; }
       if(samplesD[i-1].name == "DENKYEMBOUR") { samplesD[i-1].district = "525"; samplesD[i-1].region = "EASTERN"; }
       if(samplesD[i-1].name == "KWAEBIBIREM") { samplesD[i-1].district = "514"; samplesD[i-1].region = "EASTERN"; }
       if(samplesD[i-1].name == "AGONA EAST") { samplesD[i-1].district = "210"; samplesD[i-1].region = "CENTRAL"; }
       if(samplesD[i-1].name == "ASSIN NORTH") { samplesD[i-1].district = "214"; samplesD[i-1].region = "CENTRAL"; }
       if(samplesD[i-1].name == "KWAHU AFRAM PLAINS SOUTH") { samplesD[i-1].district = "526"; samplesD[i-1].region = "EASTERN"; }
       if(samplesD[i-1].name == "ASSIN SOUTH") { samplesD[i-1].district = "213"; samplesD[i-1].region = "CENTRAL"; }
       if(samplesD[i-1].name == "KWAHU AFRAM PLAINS NORTH") { samplesD[i-1].district = "521"; samplesD[i-1].region = "EASTERN"; }
       if(samplesD[i-1].name == "SUHUM MUNICIPAL") { samplesD[i-1].district = "504"; samplesD[i-1].region = "EASTERN"; }
       if(samplesD[i-1].name == "ASIKUMA-ODOBEN-BRAKWA") { samplesD[i-1].district = "212"; samplesD[i-1].region = "CENTRAL"; }
       if(samplesD[i-1].name == "EFFUTU MUNICIPAL") { samplesD[i-1].district = "207"; samplesD[i-1].region = "CENTRAL"; }
       if(samplesD[i-1].name == "AYENSUANO") { samplesD[i-1].district = "524"; samplesD[i-1].region = "EASTERN"; }
       if(samplesD[i-1].name == "ABUAKWA NORTH") { samplesD[i-1].district = "513"; samplesD[i-1].region = "EASTERN"; }
       if(samplesD[i-1].name == "GOMOA EAST") { samplesD[i-1].district = "208"; samplesD[i-1].region = "CENTRAL"; }
       if(samplesD[i-1].name == "UPPER WEST AKIM") { samplesD[i-1].district = "522"; samplesD[i-1].region = "EASTERN"; }
       if(samplesD[i-1].name == "ABURA-ASEBU-KWAMANKESE") { samplesD[i-1].district = "203"; samplesD[i-1].region = "CENTRAL"; }
       if(samplesD[i-1].name == "JOMORO") { samplesD[i-1].district = "101"; samplesD[i-1].region = "WESTERN"; }
       if(samplesD[i-1].name == "WEST AKIM MUNICIPAL") { samplesD[i-1].district = "503"; samplesD[i-1].region = "EASTERN"; }
       if(samplesD[i-1].name == "ELLEMBELLE") { samplesD[i-1].district = "102"; samplesD[i-1].region = "WESTERN"; }
       if(samplesD[i-1].name == "DORMAA WEST") { samplesD[i-1].district = "724"; samplesD[i-1].region = "BONO"; }
       if(samplesD[i-1].name == "NADOWLI-KALEO") { samplesD[i-1].district = "1005"; samplesD[i-1].region = "UPPER WEST"; }
       if(samplesD[i-1].name == "PRESTEA/HUNI VALLEY") { samplesD[i-1].district = "109"; samplesD[i-1].region = "WESTERN"; }
       if(samplesD[i-1].name == "AHANTA WEST MUNICIPAL") { samplesD[i-1].district = "104"; samplesD[i-1].region = "WESTERN"; }
       if(samplesD[i-1].name == "DAFFIAMA BUSSIE ISSA") { samplesD[i-1].district = "1010"; samplesD[i-1].region = "UPPER WEST"; }
       if(samplesD[i-1].name == "NZEMA EAST") { samplesD[i-1].district = "103"; samplesD[i-1].region = "WESTERN"; }
       if(samplesD[i-1].name == "LAWRA") { samplesD[i-1].district = "1009"; samplesD[i-1].region = "UPPER WEST"; }
       if(samplesD[i-1].name == "NANDOM") { samplesD[i-1].district = "1011"; samplesD[i-1].region = "UPPER WEST"; }
       if(samplesD[i-1].name == "TARKWA NSUAEM") { samplesD[i-1].district = "108"; samplesD[i-1].region = "WESTERN"; }
       if(samplesD[i-1].name == "TALENSI") { samplesD[i-1].district = "905"; samplesD[i-1].region = "UPPER EAST"; }
       if(samplesD[i-1].name == "SEFWI AKONTOMBRA") { samplesD[i-1].district = "113"; samplesD[i-1].region = "WESTERN NORTH"; }
       if(samplesD[i-1].name == "EFFIA KWESIMINTSIM MUNICIPAL") { samplesD[i-1].district = "105"; samplesD[i-1].region = "WESTERN"; }
       if(samplesD[i-1].name == "NABDAM") { samplesD[i-1].district = "911"; samplesD[i-1].region = "UPPER EAST"; }
       if(samplesD[i-1].name == "AKYEM MANSA") { samplesD[i-1].district = "515"; samplesD[i-1].region = "EASTERN"; }
       if(samplesD[i-1].name == "BUILSA SOUTH") { samplesD[i-1].district = "910"; samplesD[i-1].region = "UPPER EAST"; }
       if(samplesD[i-1].name == "KWAHU SOUTH") { samplesD[i-1].district = "519"; samplesD[i-1].region = "EASTERN"; }
       if(samplesD[i-1].name == "BUILSA NORTH") { samplesD[i-1].district = "901"; samplesD[i-1].region = "UPPER EAST"; }
       if(samplesD[i-1].name == "BIRIM CENTRAL MUNICIPAL") { samplesD[i-1].district = "502"; samplesD[i-1].region = "EASTERN"; }
       if(samplesD[i-1].name == "BINDURI") { samplesD[i-1].district = "912"; samplesD[i-1].region = "UPPER EAST"; }
       if(samplesD[i-1].name == "PUSIGA") { samplesD[i-1].district = "913"; samplesD[i-1].region = "UPPER EAST"; }
       if(samplesD[i-1].name == "UPPER MANYA KROBO") { samplesD[i-1].district = "511"; samplesD[i-1].region = "EASTERN"; }
       if(samplesD[i-1].name == "BIRIM SOUTH") { samplesD[i-1].district = "501"; samplesD[i-1].region = "EASTERN"; }
       if(samplesD[i-1].name == "BAWKU MUNICIPAL") { samplesD[i-1].district = "909"; samplesD[i-1].region = "UPPER EAST"; }
       if(samplesD[i-1].name == "NORTH GONJA") { samplesD[i-1].district = "821"; samplesD[i-1].region = "SAVANNAH"; }
       if(samplesD[i-1].name == "AKWAPEM NORTH") { samplesD[i-1].district = "506"; samplesD[i-1].region = "EASTERN"; }
       if(samplesD[i-1].name == "WEST GONJA") { samplesD[i-1].district = "803"; samplesD[i-1].region = "SAVANNAH"; }
       if(samplesD[i-1].name == "YILO KROBO MUNICIPAL") { samplesD[i-1].district = "508"; samplesD[i-1].region = "EASTERN"; }
       if(samplesD[i-1].name == "ATIWA EAST") { samplesD[i-1].district = "517"; samplesD[i-1].region = "EASTERN"; }
       if(samplesD[i-1].name == "MION") { samplesD[i-1].district = "824"; samplesD[i-1].region = "NORTHERN"; }
       if(samplesD[i-1].name == "NEW JUABEN NORTH MUNICIPAL") { samplesD[i-1].district = "507"; samplesD[i-1].region = "EASTERN"; }
       if(samplesD[i-1].name == "YENDI MUNICIPAL") { samplesD[i-1].district = "810"; samplesD[i-1].region = "NORTHERN"; }
       if(samplesD[i-1].name == "FANTEAKWA SOUTH") { samplesD[i-1].district = "512"; samplesD[i-1].region = "EASTERN"; }
       if(samplesD[i-1].name == "TOLON") { samplesD[i-1].district = "812"; samplesD[i-1].region = "NORTHERN"; }
       if(samplesD[i-1].name == "KUMBUNGU") { samplesD[i-1].district = "822"; samplesD[i-1].region = "NORTHERN"; }
       if(samplesD[i-1].name == "BIRIM NORTH") { samplesD[i-1].district = "516"; samplesD[i-1].region = "EASTERN"; }
       if(samplesD[i-1].name == "WEST MAMPRUSI MUNICIPAL") { samplesD[i-1].district = "820"; samplesD[i-1].region = "NORTHERN EAST"; }
       if(samplesD[i-1].name == "ASUOGYAMAN") { samplesD[i-1].district = "510"; samplesD[i-1].region = "EASTERN"; }
       if(samplesD[i-1].name == "MAMPRUGU MOAGDURI") { samplesD[i-1].district = "826"; samplesD[i-1].region = "NORTHERN EAST"; }
       if(samplesD[i-1].name == "KWAHU EAST") { samplesD[i-1].district = "520"; samplesD[i-1].region = "EASTERN"; }
       if(samplesD[i-1].name == "ZABZUGU") { samplesD[i-1].district = "809"; samplesD[i-1].region = "NORTHERN"; }
       if(samplesD[i-1].name == "KWAHU WEST MUNICIPAL") { samplesD[i-1].district = "518"; samplesD[i-1].region = "EASTERN"; }
       if(samplesD[i-1].name == "SENE EAST") { samplesD[i-1].district = "727"; samplesD[i-1].region = "BONO EAST"; }
       if(samplesD[i-1].name == "BIBIANI-ANHWIASO-BEKWAI MUNICIPAL") { samplesD[i-1].district = "115"; samplesD[i-1].region = "WESTERN NORTH"; }
       if(samplesD[i-1].name == "SENE WEST") { samplesD[i-1].district = "719"; samplesD[i-1].region = "BONO EAST"; }
       if(samplesD[i-1].name == "UPPER DENKYIRA EAST MUNICIPAL") { samplesD[i-1].district = "216"; samplesD[i-1].region = "CENTRAL"; }
       if(samplesD[i-1].name == "ASUTIFI NORTH") { samplesD[i-1].district = "703"; samplesD[i-1].region = "AHAFO"; }
       if(samplesD[i-1].name == "AHAFO ANO NORTH") { samplesD[i-1].district = "617"; samplesD[i-1].region = "ASHANTI"; }
       if(samplesD[i-1].name == "AMANSIE CENTRAL") { samplesD[i-1].district = "603"; samplesD[i-1].region = "ASHANTI"; }
       if(samplesD[i-1].name == "AMANSIE WEST") { samplesD[i-1].district = "602"; samplesD[i-1].region = "ASHANTI"; }
       if(samplesD[i-1].name == "ASUTIFI SOUTH") { samplesD[i-1].district = "723"; samplesD[i-1].region = "AHAFO"; }
       if(samplesD[i-1].name == "TECHIMAN MUNICIPAL") { samplesD[i-1].district = "715"; samplesD[i-1].region = "BONO EAST"; }
       if(samplesD[i-1].name == "UPPER DENKYIRA WEST") { samplesD[i-1].district = "217"; samplesD[i-1].region = "CENTRAL"; }
       if(samplesD[i-1].name == "TECHIMAN NORTH") { samplesD[i-1].district = "725"; samplesD[i-1].region = "BONO EAST"; }
       if(samplesD[i-1].name == "CAPE COAST METROPOLITAN") { samplesD[i-1].district = "202"; samplesD[i-1].region = "CENTRAL"; }
       if(samplesD[i-1].name == "KOMENDA-EDINA-EGUAFO-ABIREM MUNICIPAL") { samplesD[i-1].district = "201"; samplesD[i-1].region = "CENTRAL"; }
       if(samplesD[i-1].name == "BANDA") { samplesD[i-1].district = "726"; samplesD[i-1].region = "BONO"; }
       if(samplesD[i-1].name == "SEFWI-WIAWSO") { samplesD[i-1].district = "114"; samplesD[i-1].region = "WESTERN NORTH"; }
       if(samplesD[i-1].name == "JAMAN NORTH") { samplesD[i-1].district = "712"; samplesD[i-1].region = "BONO"; }
       if(samplesD[i-1].name == "ASUNAFO SOUTH") { samplesD[i-1].district = "701"; samplesD[i-1].region = "AHAFO"; }
       if(samplesD[i-1].name == "TAIN") { samplesD[i-1].district = "713"; samplesD[i-1].region = "BONO"; }
       if(samplesD[i-1].name == "NKORANZA SOUTH") { samplesD[i-1].district = "716"; samplesD[i-1].region = "BONO EAST"; }
       if(samplesD[i-1].name == "SEKYERE KUMAWU") { samplesD[i-1].district = "624"; samplesD[i-1].region = "ASHANTI"; }
       if(samplesD[i-1].name == "BOLE") { samplesD[i-1].district = "801"; samplesD[i-1].region = "SAVANNAH"; }
       if(samplesD[i-1].name == "SEKYERE AFRAM PLAINS NORTH") { samplesD[i-1].district = "630"; samplesD[i-1].region = "ASHANTI"; }
       if(samplesD[i-1].name == "LOWER MANYA KROBO") { samplesD[i-1].district = "509"; samplesD[i-1].region = "EASTERN"; }
       if(samplesD[i-1].name == "ASANTE AKIM NORTH") { samplesD[i-1].district = "629"; samplesD[i-1].region = "ASHANTI"; }
       if(samplesD[i-1].name == "ASANTE AKIM CENTRAL MUNICIPAL") { samplesD[i-1].district = "610"; samplesD[i-1].region = "ASHANTI"; }
       if(samplesD[i-1].name == "KINTAMPO NORTH MUNICIPAL") { samplesD[i-1].district = "722"; samplesD[i-1].region = "BONO EAST"; }
       if(samplesD[i-1].name == "ASOKORE MAMPONG MUNICIPAL") { samplesD[i-1].district = "628"; samplesD[i-1].region = "ASHANTI"; }
       if(samplesD[i-1].name == "ATEBUBU AMANTIN") { samplesD[i-1].district = "718"; samplesD[i-1].region = "BONO EAST"; }
       if(samplesD[i-1].name == "KWADASO MUNICIPAL") { samplesD[i-1].district = "614"; samplesD[i-1].region = "ASHANTI"; }
       if(samplesD[i-1].name == "KPANDAI") { samplesD[i-1].district = "806"; samplesD[i-1].region = "NORTHERN"; }
       if(samplesD[i-1].name == "EJISU MUNICIPAL") { samplesD[i-1].district = "611"; samplesD[i-1].region = "ASHANTI"; }
       if(samplesD[i-1].name == "NKWANTA NORTH") { samplesD[i-1].district = "418"; samplesD[i-1].region = "OTI"; }
       if(samplesD[i-1].name == "BIAKOYE") { samplesD[i-1].district = "412"; samplesD[i-1].region = "OTI"; }
       if(samplesD[i-1].name == "WASSA AMENFI EAST") { samplesD[i-1].district = "110"; samplesD[i-1].region = "WESTERN"; }
       if(samplesD[i-1].name == "CENTRAL GONJA") { samplesD[i-1].district = "804"; samplesD[i-1].region = "SAVANNAH"; }
       if(samplesD[i-1].name == "TEMA WEST MUNICIPAL") { samplesD[i-1].district = "308"; samplesD[i-1].region = "GREATER ACCRA"; }
       if(samplesD[i-1].name == "ASHAIMAN MUNICIPAL") { samplesD[i-1].district = "307"; samplesD[i-1].region = "GREATER ACCRA"; }
       if(samplesD[i-1].name == "PRU EAST") { samplesD[i-1].district = "720"; samplesD[i-1].region = "BONO EAST"; }
       if(samplesD[i-1].name == "KPONE KATAMANSO") { samplesD[i-1].district = "314"; samplesD[i-1].region = "GREATER ACCRA"; }
       if(samplesD[i-1].name == "SAVELUGU") { samplesD[i-1].district = "813"; samplesD[i-1].region = "NORTHERN"; }
       if(samplesD[i-1].name == "TAMALE METROPOLITAN") { samplesD[i-1].district = "811"; samplesD[i-1].region = "NORTHERN"; }
       if(samplesD[i-1].name == "SAGNERIGU") { samplesD[i-1].district = "823"; samplesD[i-1].region = "NORTHERN"; }
       if(samplesD[i-1].name == "KARAGA") { samplesD[i-1].district = "814"; samplesD[i-1].region = "NORTHERN"; }
       if(samplesD[i-1].name == "NORTH EAST GONJA") { samplesD[i-1].district = "805"; samplesD[i-1].region = "SAVANNAH"; }
       if(samplesD[i-1].name == "SOUTH DAYI") { samplesD[i-1].district = "409"; samplesD[i-1].region = "VOLTA"; }
       if(samplesD[i-1].name == "TEMPANE") { samplesD[i-1].district = "908"; samplesD[i-1].region = "UPPER EAST"; }
       if(samplesD[i-1].name == "BOLGA  EAST") { samplesD[i-1].district = "904"; samplesD[i-1].region = "UPPER EAST"; }
       if(samplesD[i-1].name == "YUNYOO-NASUAN") { samplesD[i-1].district = "818"; samplesD[i-1].region = "NORTHERN EAST"; }
       if(samplesD[i-1].name == "NANTON") { samplesD[i-1].district = "813"; samplesD[i-1].region = "NORTHERN"; }
       if(samplesD[i-1].name == "ATIWA WEST") { samplesD[i-1].district = "517"; samplesD[i-1].region = "EASTERN"; }
       if(samplesD[i-1].name == "FANTEAKWA NORTH") { samplesD[i-1].district = "512"; samplesD[i-1].region = "EASTERN"; }
       if(samplesD[i-1].name == "OKERE") { samplesD[i-1].district = "506"; samplesD[i-1].region = "EASTERN"; }
       if(samplesD[i-1].name == "ASSIN CENTRAL MUNICIPAL") { samplesD[i-1].district = "214"; samplesD[i-1].region = "CENTRAL"; }
       if(samplesD[i-1].name == "GOMOA CENTRAL") { samplesD[i-1].district = "206"; samplesD[i-1].region = "CENTRAL"; }
       if(samplesD[i-1].name == "PRU WEST") { samplesD[i-1].district = "720"; samplesD[i-1].region = "BONO EAST"; }
       if(samplesD[i-1].name == "BEREKUM WEST") { samplesD[i-1].district = "710"; samplesD[i-1].region = "BONO"; }
       if(samplesD[i-1].name == "AHAFO ANO SOUTH WEST") { samplesD[i-1].district = "616"; samplesD[i-1].region = "ASHANTI"; }
       if(samplesD[i-1].name == "AMANSIE SOUTH") { samplesD[i-1].district = "602"; samplesD[i-1].region = "ASHANTI"; }
       if(samplesD[i-1].name == "ATWIMA NWABIAGYA NORTH") { samplesD[i-1].district = "615"; samplesD[i-1].region = "ASHANTI"; }
       if(samplesD[i-1].name == "AKROFUOM") { samplesD[i-1].district = "604"; samplesD[i-1].region = "ASHANTI"; }
       if(samplesD[i-1].name == "ADANSI NORTH") { samplesD[i-1].district = "606"; samplesD[i-1].region = "ASHANTI"; }
       if(samplesD[i-1].name == "OBUASI EAST") { samplesD[i-1].district = "605"; samplesD[i-1].region = "ASHANTI"; }
       if(samplesD[i-1].name == "AFIGYA KWABRE NORTH") { samplesD[i-1].district = "619"; samplesD[i-1].region = "ASHANTI"; }
       if(samplesD[i-1].name == "KUMASI METROPOLITAN") { samplesD[i-1].district = "614"; samplesD[i-1].region = "ASHANTI"; }
       if(samplesD[i-1].name == "OFORIKROM MUNICIPAL") { samplesD[i-1].district = "614"; samplesD[i-1].region = "ASHANTI"; }
       if(samplesD[i-1].name == "ASOKWA  MUNICIPAL") { samplesD[i-1].district = "614"; samplesD[i-1].region = "ASHANTI"; }
       if(samplesD[i-1].name == "OLD TAFO MUNICIPAL") { samplesD[i-1].district = "614"; samplesD[i-1].region = "ASHANTI"; }
       if(samplesD[i-1].name == "SUAME MUNICIPAL") { samplesD[i-1].district = "614"; samplesD[i-1].region = "ASHANTI"; }
       if(samplesD[i-1].name == "JUABEN MUNICIPAL") { samplesD[i-1].district = "611"; samplesD[i-1].region = "ASHANTI"; }
       if(samplesD[i-1].name == "ABLEKUMA CENTRAL MUNICIPAL") { samplesD[i-1].district = "304"; samplesD[i-1].region = "GREATER ACCRA"; }
       if(samplesD[i-1].name == "KORLE KLOTTEY MUNICIPAL") { samplesD[i-1].district = "304"; samplesD[i-1].region = "GREATER ACCRA"; }
       if(samplesD[i-1].name == "ABLEKUMA NORTH MUNICIPAL") { samplesD[i-1].district = "304"; samplesD[i-1].region = "GREATER ACCRA"; }
       if(samplesD[i-1].name == "AYAWASO NORTH MUNICIPAL") { samplesD[i-1].district = "304"; samplesD[i-1].region = "GREATER ACCRA"; }
       if(samplesD[i-1].name == "AYAWASO EAST MUNICIPAL") { samplesD[i-1].district = "304"; samplesD[i-1].region = "GREATER ACCRA"; }
       if(samplesD[i-1].name == "OKAIKWEI NORTH MUNICIPAL") { samplesD[i-1].district = "304"; samplesD[i-1].region = "GREATER ACCRA"; }
       if(samplesD[i-1].name == "GA NORTH MUNICIPAL") { samplesD[i-1].district = "302"; samplesD[i-1].region = "GREATER ACCRA"; }
       if(samplesD[i-1].name == "WEIJA GBAWE MUNICIPAL") { samplesD[i-1].district = "301"; samplesD[i-1].region = "GREATER ACCRA"; }
       if(samplesD[i-1].name == "KROWOR MUNICIPAL") { samplesD[i-1].district = "306"; samplesD[i-1].region = "GREATER ACCRA"; }
       if(samplesD[i-1].name == "TEMA METROPOLITAN") { samplesD[i-1].district = "308"; samplesD[i-1].region = "GREATER ACCRA"; }
       if(samplesD[i-1].name == "EAST GONJA MUNICIPAL") { samplesD[i-1].district = "805"; samplesD[i-1].region = "SAVANNAH"; }
       if(samplesD[i-1].name == "ABUAKWA SOUTH MUNICIPAL") { samplesD[i-1].district = "513"; samplesD[i-1].region = "EASTERN"; }
       if(samplesD[i-1].name == "NEW JUABEN SOUTH MUNICIPAL") { samplesD[i-1].district = "507"; samplesD[i-1].region = "EASTERN"; }
       if(samplesD[i-1].name == "ABLEKUMA WEST MUNICIPAL") { samplesD[i-1].district = "304"; samplesD[i-1].region = "GREATER ACCRA"; }
       if(samplesD[i-1].name == "AYAWASO CENTRAL MUNICIPAL") { samplesD[i-1].district = "304"; samplesD[i-1].region = "GREATER ACCRA"; }
       if(samplesD[i-1].name == "ACCRA METROPOLIS") { samplesD[i-1].district = "304"; samplesD[i-1].region = "GREATER ACCRA"; }
       if(samplesD[i-1].name == "ANLOGA") { samplesD[i-1].district = "402"; samplesD[i-1].region = "VOLTA"; }
       if(samplesD[i-1].name == "SEKONDI TAKORADI METROPOLIS") { samplesD[i-1].district = "105"; samplesD[i-1].region = "WESTERN"; }
       if(samplesD[i-1].name == "ACHIASE") { samplesD[i-1].district = "501"; samplesD[i-1].region = "EASTERN"; }
       if(samplesD[i-1].name == "ASENE MANSO AKROSO") { samplesD[i-1].district = "502"; samplesD[i-1].region = "EASTERN"; }
       if(samplesD[i-1].name == "DORMAA MUNICIPAL") { samplesD[i-1].district = "0"; samplesD[i-1].region = "BONO"; }
       if(samplesD[i-1].name == "TATALE SANGULI") { samplesD[i-1].district = "0"; samplesD[i-1].region = "NORTHERN"; }
       if(samplesD[i-1].name == "HO WEST") { samplesD[i-1].district = "0"; samplesD[i-1].region = "VOLTA"; }
       if(samplesD[i-1].name == "KASENA NANKANA WEST") { samplesD[i-1].district = "0"; samplesD[i-1].region = "UPPER EAST"; }




    }
    if (debug) std::cout << "inside create visualization after load district sample structure " <<   "\n";

    // now read in agent data and store directly into visualization file
    // load sample data from file - skip header row - we have already made a new one
    for (auto i = 1u; i < aDistrict.size(); i++) {
        
        // skip rows that begin with "Total"
        if (aDistrict[i][0] != "Total") {
 
            if (debug) std::cout << "inside district loop " << aDistrict[i][0] <<  "\n";

            // now find corresponding sample data and output it
            for (auto j = 0u; j < sDistrict.size() - 1; j++) {

                if ((aDistrict[i][0] == samplesD[j].name) && (aDistrict[i][10] == samplesD[j].disease)) {

                    if (debug) std::cout << "district names match " << aDistrict[i][0] << " samples " << samplesD[j].name << "\n";
                    
                    untreated = stoi(aDistrict[i][47]) + stoi(aDistrict[i][48]) - stoi(aDistrict[i][51]);
                    
                    myfile2 << aDistrict[i][0] << "," << samplesD[j].district << "," << samplesD[j].region << ",0" << ",0";
                    if (allFields) {
                    
                        myfile2 << "," << stoi(aDistrict[i][8]) << "," << stoi(aDistrict[i][13])+ stoi(aDistrict[i][15]) << "," << aDistrict[i][10] << "," << stoi(aDistrict[i][4]) << "," << stoi(aDistrict[i][11]);
                        myfile2 << "," << stof(aDistrict[i][18]) << "," << stoi(aDistrict[i][19]) << "," << stoi(aDistrict[i][98]) << "," << stoi(aDistrict[i][103])+stoi(aDistrict[i][102]) << "," << untreated << "," << stoi(aDistrict[i][101]) << "," << stoi(aDistrict[i][99]) << "," << stoi(aDistrict[i][100]);
                        myfile2 << "," << "0" << "," << (stoi(aDistrict[i][13]) + stoi(aDistrict[i][15])) - stoi(aDistrict[i][14]) << "," << stoi(aDistrict[i][73]) - stoi(aDistrict[i][74]) << "," << stoi(aDistrict[i][75]) + stoi(aDistrict[i][80]) + stoi(aDistrict[i][89]) + stoi(aDistrict[i][102]) + stoi(aDistrict[i][106]);
                        myfile2 << "," << stoi(aDistrict[i][75]) + stoi(aDistrict[i][80]) + stoi(aDistrict[i][89]) << "," << "0" << "," << stoi(aDistrict[i][14]);
                        myfile2 << "," << stoi(aDistrict[i][35]) << "," << stoi(aDistrict[i][37])+ stoi(aDistrict[i][38]) << "," << stoi(aDistrict[i][42]) << "," << stoi(aDistrict[i][51]) << "," << stoi(aDistrict[i][52]) << "," << stoi(aDistrict[i][107]);
                    }
                    else {
                    
                        myfile2 << "," << stoi(aDistrict[i][1]) << "," << stoi(aDistrict[i][2]) << "," << aDistrict[i][3] << "," << stoi(aDistrict[i][4]);
                        myfile2 << "," << stof(aDistrict[i][5]) << "," << stoi(aDistrict[i][6]) << "," << stoi(aDistrict[i][7]) << "," << stoi(aDistrict[i][8]) << "," << untreated << "," << stoi(aDistrict[i][12]) << "," << stoi(aDistrict[i][14]);
                        myfile2 << "," << stoi(aDistrict[i][15]) << "," << stoi(aDistrict[i][16]) << "," << stoi(aDistrict[i][17]) << "," << stoi(aDistrict[i][18]) << "," << stoi(aDistrict[i][19]) << "," << stoi(aDistrict[i][20]) << "," << stoi(aDistrict[i][21]);
                        myfile2 << "," << stoi(aDistrict[i][24]) << "," << stoi(aDistrict[i][25]);
                    }

                    myfile2 <<"," << samplesD[j].samples << "," << samplesD[j].disease << "," << samplesD[j].newSample << "," << samplesD[j].inTransit << "," << samplesD[j].wait << "," << samplesD[j].lost << "," << samplesD[j].returnedTransit << "," << samplesD[j].returned << "," << samplesD[j].reLost;
                    myfile2 << "," << samplesD[j].other << "," << samplesD[j].avgReturnTime << "," << samplesD[j].testName1 << "," << samplesD[j].testCount1 << "," << samplesD[j].testName2 << "," << samplesD[j].testCount2 << "," << samplesD[j].testName3 << "," << samplesD[j].testCount3;
                    myfile2 << "," << samplesD[j].testName4 << "," << samplesD[j].testCount4 << "\n";

                }
            }
        }
    }
    if (debug) std::cout << "inside create visualization after output district file " <<   "\n";

    // read in samples and agent region data an merge them
    vector<vector<string>> sRegion = readDataInput(sampleRegion);
    if (sRegion.size() < 2) {

        std::cout << "inside create visualization eror reading region sample files " << sampleRegion <<  "\n";
        return;
    }
    vector<vector<string>> aRegion = readDataInput(agentRegion);
    if (aRegion.size() < 2) {

        std::cout << "inside create visualization eror reading region agent files " << agentRegion <<  "\n";
        return;
    }

    // use this as an count to create an array for samples
    sampleCount = sRegion.size();
    sampleStats samplesR[sampleCount];

    
    // load sample data from file - skip header row - we have already made a new one
    for (auto i = 1u; i < sRegion.size(); i++) {
;
        samplesR[i-1].name = sRegion[i][0];
        samplesR[i-1].samples = stoi(sRegion[i][1]);
        samplesR[i-1].disease = sRegion[i][2];
        samplesR[i-1].newSample = stoi(sRegion[i][3]);
        samplesR[i-1].newInBatch = stoi(sRegion[i][4]);
        samplesR[i-1].inTransit = stoi(sRegion[i][5]);
        samplesR[i-1].wait = stoi(sRegion[i][6]);
        samplesR[i-1].lost = stoi(sRegion[i][7]);
        samplesR[i-1].returnedTransit = stoi(sRegion[i][8]);
        samplesR[i-1].returned = stoi(sRegion[i][9]);
        samplesR[i-1].reLost = stoi(sRegion[i][10]);
        samplesR[i-1].other = stoi(sRegion[i][11]);
        samplesR[i-1].avgReturnTime = stof(sRegion[i][12]);
        samplesR[i-1].testName1 = sRegion[i][13];
        samplesR[i-1].testCount1 = stoi(sRegion[i][14]);
        samplesR[i-1].testName2 = sRegion[i][15];
        samplesR[i-1].testCount2 = stoi(sRegion[i][16]);
        samplesR[i-1].testName3 = sRegion[i][17];
        samplesR[i-1].testCount3 = stoi(sRegion[i][18]);
        samplesR[i-1].testName4 = sRegion[i][19];
        samplesR[i-1].testCount4 = stoi(sRegion[i][20]);

        if (samplesR[i-1].name == "AHAFO") samplesR[i-1].region = "GH01";
        if (samplesR[i-1].name == "ASHANTI") samplesR[i-1].region = "GH02";
        if (samplesR[i-1].name == "BONO") samplesR[i-1].region = "GH03";
        if (samplesR[i-1].name == "BONO EAST") samplesR[i-1].region = "GH04";
        if (samplesR[i-1].name == "CENTRAL") samplesR[i-1].region = "GH05";
        if (samplesR[i-1].name == "EASTERN") samplesR[i-1].region = "GH06";
        if (samplesR[i-1].name == "GREATER ACCRA") samplesR[i-1].region = "GH07";
        if (samplesR[i-1].name == "NORTHERN") samplesR[i-1].region = "GH08";
        if (samplesR[i-1].name == "NORTHERN EAST") samplesR[i-1].region = "GH09";
        if (samplesR[i-1].name == "OTI") samplesR[i-1].region = "GH10";
        if (samplesR[i-1].name == "SAVANNAH") samplesR[i-1].region = "GH11";
        if (samplesR[i-1].name == "UPPER EAST") samplesR[i-1].region = "GH12";
        if (samplesR[i-1].name == "UPPER WEST") samplesR[i-1].region = "GH13";
        if (samplesR[i-1].name == "VOLTA") samplesR[i-1].region = "GH14";
        if (samplesR[i-1].name == "WESTERN") samplesR[i-1].region = "GH15";
        if (samplesR[i-1].name == "WESTERN NORTH") samplesR[i-1].region = "GH16";

    }
    if (debug) std::cout << "inside create visualization after load region sample structure " <<   "\n";

    // now read in agent data and store directly into visualization file
    // load sample data from file - skip header row - we have already made a new one
    for (auto i = 1u; i < aRegion.size(); i++) {

        if (aRegion[i][0] != "Total") {

            // now find corresponding sample data for facility and disease and output it
            for (auto j = 0u; j < sRegion.size() - 1; j++) {

                if ((aRegion[i][0] == samplesR[j].name) && (aRegion[i][10] == samplesR[j].disease)) {

                    untreated = stoi(aRegion[i][47]) + stoi(aRegion[i][48]) - stoi(aRegion[i][51]);
                    
                    myfile3 << aRegion[i][0] << "," << samplesR[j].region << ",0" << ",0";
                    if (allFields) {
                    
                        myfile3 << "," << stoi(aRegion[i][8]) << "," << stoi(aRegion[i][13]) + stoi(aRegion[i][15]) << "," << aRegion[i][10] << "," << stoi(aRegion[i][4]) << "," << stoi(aRegion[i][11]);
                        myfile3 << "," << stof(aRegion[i][18]) << "," << stoi(aRegion[i][19]) << "," << stoi(aRegion[i][98]) << "," << stoi(aRegion[i][103]) + stoi(aRegion[i][102]) << "," << untreated << "," << stoi(aRegion[i][101]) << "," << stoi(aRegion[i][99]) << "," << stoi(aRegion[i][100]);
                        myfile3 << "," << "0" << "," << (stoi(aRegion[i][13]) + stoi(aRegion[i][15])) - stoi(aRegion[i][14]) << "," << stoi(aRegion[i][73]) - stoi(aRegion[i][74]) << "," << stoi(aRegion[i][75]) + stoi(aRegion[i][80]) + stoi(aRegion[i][89]) + stoi(aRegion[i][102]) + stoi(aRegion[i][106]);
                        myfile3 << "," << stoi(aRegion[i][75]) + stoi(aRegion[i][80]) + stoi(aRegion[i][89]) << "," << "0" << "," << stoi(aRegion[i][14]);
                        myfile3 << "," << stoi(aRegion[i][35]) << "," << stoi(aRegion[i][37]) + stoi(aRegion[i][38]) << "," << stoi(aRegion[i][42]) << "," << stoi(aRegion[i][51]) << "," << stoi(aRegion[i][52]) << "," << stoi(aRegion[i][107]);
                    }
                    else {
                    
                        myfile3 << "," << stoi(aRegion[i][1]) << "," << stoi(aRegion[i][2]) << "," << aRegion[i][3] << "," <<stoi( aRegion[i][4]);
                        myfile3 << "," << stof(aRegion[i][5]) << "," << stoi(aRegion[i][6]) << "," << stoi(aRegion[i][7]) << "," << stoi(aRegion[i][8]) << "," << untreated <<  "," << stoi(aRegion[i][12]) <<"," << stoi(aRegion[i][14]);
                        myfile3 << "," << stoi(aRegion[i][15]) << "," << stoi(aRegion[i][16]) << "," << stoi(aRegion[i][17]) << "," << stoi(aRegion[i][18]) << "," << stoi(aRegion[i][19]) << "," << stoi(aRegion[i][20]) << "," << stoi(aRegion[i][21]);
                        myfile3 << "," << stoi(aRegion[i][24]) << "," << stoi(aRegion[i][25]);
                    }

                    myfile3 <<"," << samplesR[j].samples << "," << samplesR[j].disease << "," << samplesR[j].newSample << "," << samplesR[j].inTransit << "," << samplesR[j].wait << "," << samplesR[j].lost << "," << samplesR[j].returnedTransit << "," << samplesR[j].returned << "," << samplesR[j].reLost;
                    myfile3 << "," << samplesR[j].other << "," << samplesR[j].avgReturnTime << "," << samplesR[j].testName1 << "," << samplesR[j].testCount1 << "," << samplesR[j].testName2 << "," << samplesR[j].testCount2 << "," << samplesR[j].testName3 << "," << samplesR[j].testCount3;
                    myfile3 << "," << samplesR[j].testName4 << "," << samplesR[j].testCount4 << "\n";

                }
            }
        }
    }
    if (debug) std::cout << "inside create visualization after output region file " <<   "\n";

    // loop through each facility and the number of Patients and Samples at each facility
    //for (auto facility : facilities) {

        // get the up name for this facility
        //d = facility->getDiagnosisPtr();
        //t = facility->getTestingPtr();
        //p = d->getPatientPtr();
        //s = t->getSamplePtr();
       // region = facility->getRegion();
       // district = facility->getDistrict();

        
    //}

    
    if (fileFlag) {

        myfile1.close();
        myfile2.close();
        myfile3.close();
    }
}




