//
//  MapFacilities.cpp
// 
//
//  Created by L. Dombrowski on 4/29/20

//
//  Split out of CreateStats on 5/14/20
//


#include <vector>
#include <string>
#include <cstring>
#include <sys/stat.h>
#include <errno.h>

#include "MapFacilities.h"
#include "GlobalParameters.h"
#include "FacilitiesList.h"
using namespace std;

// count resources used at each facility for this timestep
void mapFacilities(FacilitiesList* list, vector<string> diseaseList, bool fileFlag) {

    Diagnosis* d;
    Testing* t;
    vector<Patient*> p;
    vector<Sample*> s;
    vector<Test*> tests;
    vector<Facility*> latF;
    vector<facilityAttributes*> facAttr;
    ofstream myfile;
    ofstream myfile1;

    int countTestsToday = 0;
    int countTestsCycle = 0;
    int noFacilities = 0;
    int avgTests = 0;
    int errNo;
    int i;
    float TBIncid = 0;
    float HIVIncid = 0;
    float HCVIncid = 0;
    float YFIncid = 0;
    float MenIncid = 0;
    float MeaIncid = 0;
    float Dis1Incid = 0;
    float Dis2Incid = 0;
    string testingName;
    string subDirBase;
    bool TBDiseaseFlag = false;
    bool HIVDiseaseFlag = false;
    bool HCVDiseaseFlag = false;
    bool YFDiseaseFlag = false;
    bool MenDiseaseFlag = false;
    bool MeaDiseaseFlag = false;
    bool Dis1DiseaseFlag = false;
    bool Dis2DiseaseFlag = false;
    bool XpertHere = false;
    bool mapDebug = false;

    vector<Facility*> facilities = list->getListPtr();
    
    noFacilities = facilities.size();

    // catchment for each facility = initialize to zero
    int catchPop[noFacilities] = { };
    string catchPopName[noFacilities];

    // mapDebug - true
    if (mapDebug)  std::cout << "inside map facilities file flag  = " << fileFlag << ", no. of facilities = " << noFacilities <<  "\n";
    
    // find out which diseases to track
    for (auto d : diseaseList) {

        if (d == "TB") TBDiseaseFlag = true;
        else if (d == "HIV") HIVDiseaseFlag = true;
        else if (d == "HCV") HCVDiseaseFlag = true;
        else if (d == "Yellow Fever") YFDiseaseFlag = true;
        else if (d == "Meningitis") MenDiseaseFlag = true;
        else if (d == "Measles") MeaDiseaseFlag = true;
        else if (d == "Disease1") Dis1DiseaseFlag = true;
        else if (d == "Disease2") Dis2DiseaseFlag = true;

    }

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

        myfile.open(subDirBase + "/FacilitesMap.csv");
        myfile1.open(subDirBase + "/FacilitesResources.csv");

        myfile << "name" << ",id" << ",type" << ",long" << ",lat" << ",town" << ",district" << ",district.index" << ",region" << ",region.index" << ",tier" << ",diagnosis" << ",Up.Conn";
        if (TBDiseaseFlag) myfile << ",TB.Xpert.testing" << ",TB.smear.testing";
        if (HIVDiseaseFlag) myfile << ",HIV.Xpert.testing";
        if (HCVDiseaseFlag) myfile << ",HCV.smear.testing";
        if (YFDiseaseFlag) myfile << ",YF.Xpert.testing";
        if (MenDiseaseFlag) myfile << ",Meningitis.PCR.testing" << ",Meningitis.culture.testing";
        if (MeaDiseaseFlag) myfile << ",Measles.culture.testing";
        if (Dis1DiseaseFlag) myfile << ",Disease1.testing";
        if (Dis2DiseaseFlag) myfile << ",Disease2.testing";
        if (TBDiseaseFlag) myfile << ",TB.incidence";
        if (HIVDiseaseFlag) myfile << ",HIV.incidence";
        if (HCVDiseaseFlag) myfile << ",HCV.incidence";
        if (YFDiseaseFlag) myfile << ",YF.incidence";
        if (MenDiseaseFlag) myfile << ",Meningitis.incidence";
        if (MeaDiseaseFlag) myfile << ",Measles.incidence";
        if (Dis1DiseaseFlag) myfile << ",Disease1.incidence";
        if (Dis2DiseaseFlag) myfile << ",Disease2.incidence";
        if (TBDiseaseFlag) myfile << ",TB.uinf.inf.ratio";
        if (HIVDiseaseFlag) myfile << ",HIV.uinf.inf.ratio";
        if (HCVDiseaseFlag) myfile << ",HCV.uinf.inf.ratio";
        if (YFDiseaseFlag) myfile << ",YF.uinf.inf.ratio";
        if (MenDiseaseFlag) myfile << ",Meningitis.uinf.inf.ratio";
        if (MeaDiseaseFlag) myfile << ",Measles.uinf.inf.ratio";
        if (Dis1DiseaseFlag) myfile << ",Disease1.uinf.inf.ratio";
        if (Dis2DiseaseFlag) myfile << ",Disease2.uinf.inf.ratio";
        myfile << ",Xpert" << ",Closest.TB.Xpert" << ",Closest.dist.TB.Xpert" << ",Catch.Pop.TB.Xpert" << ",Closest.TB.smear" <<  ",Closest.dist.TB.smear";
        myfile << ",Closest.HIV.Xpert" << ",Closest.dist.HIV.Xpert" << ",Closest.HCV.Xpert" <<  ",Closest.dist.HCV.Xpert";
        myfile << ",Closest.YF.Xpert" << ",Closest.dist.YF.Xpert" << ",Closest.Meningitis.PCR" <<  ",Closest.dist.Meningitis.PCR";
        myfile << ",Closest.Meningitis.culture" << ",Closest.dist.Meningitis.culture" << ",Closest.Meningitis.culture" <<  ",Closest.dist.Meningitis.culture";
        myfile << ",Closest.Disease1.Test" << ",Closest.dist.Disease1.Test" << ",Closest.Disease2.Test" << ",Closest.dist.Disease2.Test";
        myfile << ",Pop" << ",BinInc" << ",popbin0" << ",popbin1" << ",popbin2" << ",popbin3" << ",popbin4" << ",popbin5" << ",popbin6";
        myfile << ",popbin7" << ",popbin8" << ",popbin9" << ",cache.med.dist" << ",cache.farthest.dist" << "\n";

        myfile1 << "fac.name" << ",region" << ",district" << ",disease" << ",incidence" << ",inc.adj.type" <<  ",inc.adj.factor";
        myfile1 << ",test" << ",test.flag" << ",return.time.for.results" << ",no.of.machines" << ",max.reagents.per.cycle";
        myfile1 << ",time.steps.in.cycle" << ",.closet.fac.w.test" << ",closest.dist" << "\n";
    }
    else {

        std::cout << "fac name" << ", region" << ", district" << ", diagnosis" << ", testing" << ", up conn" << ", closet fac/w/test" << ", closest dist" << ", longitude" << ", latitude" << "\n";
    }

    // loop through each facility and extract name for each facility
    i = 0;
    for (auto facility : facilities) {

        // get the name for this facility
        catchPopName[i] = facility->getName();

        i = i + 1;
    }
    
    // loop through each facility and extract population catchment for each facility with a Xpert machine
    for (auto facility : facilities) {

        // get the name for this facility
        testingName = facility->getClosestNameByTest("TB", "Xpert");

        for (i = 0; i < noFacilities; i++) {

            if (testingName == catchPopName[i]) {

                catchPop[i] = catchPop[i] + facility->getPopulation();
            }
        }
    }


    // loop through each facility and extract information about each facility
    i = 0;
    for (auto facility : facilities) {

        // get the up name for this facility
        d = facility->getDiagnosisPtr();
        t = facility->getTestingPtr();
        facAttr = facility->getFacilityAttributesPtr();
        p = d->getPatientPtr();
        s = t->getSamplePtr();
        tests = t->getTeststPtr();

        if (mapDebug) {
        
            std::cout << "inside map facilities loop facility  = " << facility->getName() << "\n";
            std::cout << "inside map facilities loop testing  = " << facility->testingAvailable("TB","Xpert") << "\n";
        }

        // check to see if Xpert is here
        XpertHere = false;

        if (facility->getClosestDistance("TB", "Xpert") == 0) {
            XpertHere = true;
        }

        // extract disease incidences
        TBIncid = 0;
        HIVIncid = 0;
        HCVIncid = 0;
        YFIncid = 0;
        MenIncid = 0;
        MeaIncid = 0;
        Dis1Incid = 0;
        Dis1Incid = 0;
        for (auto a : facAttr) {

            if (a->disease == "TB") TBIncid = a->incidence;
            else if (a->disease == "HIV") HIVIncid = a->incidence;
            else if (a->disease == "HCV") HCVIncid = a->incidence;
            else if (a->disease == "Yellow Fever") YFIncid = a->incidence;
            else if (a->disease == "Meningitis") MenIncid = a->incidence;
            else if (a->disease == "Measles") MeaIncid = a->incidence;
            else if (a->disease == "Disease1") Dis1Incid = a->incidence;
            else if (a->disease == "Disease2") Dis2Incid = a->incidence;
        }

        if (fileFlag) {

            myfile << facility->getName() << "," << facility->getIDint() << "," << facility->getType() << "," << facility->getLongitude() << "," << facility->getLatitude() << "," << facility->getTown();
            myfile << "," << facility->getDistrict() << ",D" << facility->getDistrictIndex() << "," << facility->getRegion() << ",R" << facility->getRegionIndex() << "," << facility->getTier() << "," << facility->diagnosisAvailable() << "," << facility->getUpName();
            if (TBDiseaseFlag) myfile << "," << facility->testingAvailable("TB","Xpert") << ","<< facility->testingAvailable("TB","smear");
            if (HIVDiseaseFlag) myfile << "," << facility->testingAvailable("HIV", "Xpert");
            if (HCVDiseaseFlag) myfile << "," << facility->testingAvailable("HCV","Xpert");
            if (YFDiseaseFlag) myfile << "," << facility->testingAvailable("Yellow Fever", "Xpert");
            if (MenDiseaseFlag) myfile << "," << facility->testingAvailable("Meningitis", "PCR") << "," << facility->testingAvailable("Meningitis", "culture");
            if (MeaDiseaseFlag) myfile << "," << facility->testingAvailable("Measles", "culture");
            if (Dis1DiseaseFlag) myfile << "," << facility->testingAvailable("Disease1", "Test");
            if (Dis2DiseaseFlag) myfile << "," << facility->testingAvailable("Disease2", "Test");
            if (TBDiseaseFlag) myfile << "," << TBIncid;
            if (HIVDiseaseFlag) myfile << "," << HIVIncid;
            if (HCVDiseaseFlag) myfile << "," << HCVIncid;
            if (YFDiseaseFlag) myfile << "," << YFIncid;
            if (MenDiseaseFlag) myfile << "," << MenIncid;
            if (MeaDiseaseFlag) myfile << "," << MeaIncid;
            if (Dis1DiseaseFlag) myfile << "," << Dis1Incid;
            if (Dis2DiseaseFlag) myfile << "," << Dis2Incid;
            if (TBDiseaseFlag) myfile << "," << facility->getDiseaseRatioUninfInf("TB");
            if (HIVDiseaseFlag) myfile << "," << facility->getDiseaseRatioUninfInf("HIV");
            if (HCVDiseaseFlag) myfile << "," << facility->getDiseaseRatioUninfInf("HCV");
            if (YFDiseaseFlag) myfile << "," << facility->getDiseaseRatioUninfInf("Yellow Fever");
            if (MenDiseaseFlag) myfile << "," << facility->getDiseaseRatioUninfInf("Meningitis");
            if (MeaDiseaseFlag) myfile << "," << facility->getDiseaseRatioUninfInf("Measles");
            if (Dis2DiseaseFlag) myfile << "," << facility->getDiseaseRatioUninfInf("Disease1");
            if (Dis2DiseaseFlag) myfile << "," << facility->getDiseaseRatioUninfInf("Disease2");
            myfile << "," << XpertHere << ","<< facility->getClosestNameByTest("TB","Xpert") << "," << facility->getClosestDistance("TB","Xpert") << "," << catchPop[i];
            myfile << "," << facility->getClosestNameByTest("TB","smear") << "," << facility->getClosestDistance("TB","smear");
            myfile << "," << facility->getClosestNameByTest("HIV","Xpert") << "," << facility->getClosestDistance("HIV","Xpert");
            myfile << "," << facility->getClosestNameByTest("HCV","Xpert") << "," << facility->getClosestDistance("HCV","Xpert");
            myfile << "," << facility->getClosestNameByTest("Yellow Fever","Xpert") << "," << facility->getClosestDistance("Yellow Fever","Xpert");
            myfile << "," << facility->getClosestNameByTest("Meningitis","PCR") << "," << facility->getClosestDistance("Meningitis","PCR");
            myfile << "," << facility->getClosestNameByTest("Meningitis","culture") << "," << facility->getClosestDistance("Meningitis","culture");
            myfile << "," << facility->getClosestNameByTest("Measles","culture") << "," << facility->getClosestDistance("Measles","culture");
            myfile << "," << facility->getClosestNameByTest("Disease1", "Test") << "," << facility->getClosestDistance("Disease1", "Test");
            myfile << "," << facility->getClosestNameByTest("Disease2", "Test") << "," << facility->getClosestDistance("Disease2", "Test");
            myfile << "," << facility->getPopulation() << "," << facility->getBinInc() << "," << facility->getPopBin0() << "," << facility->getPopBin1() << "," << facility->getPopBin2();
            myfile << "," << facility->getPopBin3() << "," << facility->getPopBin4() << "," << facility->getPopBin5() << "," << facility->getPopBin6() << "," << facility->getPopBin7();
            myfile << "," << facility->getPopBin8() << "," << facility->getPopBin9() << "," << facility->getBinInc()*5 << "," << facility->getBinInc()*10 << "\n";

            if (mapDebug)  std::cout << "inside map facilities before facAttr loop  = " << "\n";

            for (auto a : facAttr) {
                myfile1 << facility->getName() << "," << facility->getRegion() << "," << facility->getDistrict() << "," << a->disease << "," << a->incidence;
                myfile1 << "," << a->incAdjType << "," << a->incAdjFactor;
                myfile1 << "," << a->testName << "," << a->testing << "," << a->returnCycle << "," << a->noMachines;
                myfile1 << "," << a->maxReagentPerCycle << "," << a->timeStepsInCycle << "," << a->closetConnectionWithTesting << "," << a->closestConnectionDistance  << "\n";
            }
        }
        else {
            std::cout << facility->getName() << ", " << facility->getRegion() << ", " << facility->getDistrict() << ", " << facility->diagnosisAvailable() << ", " << facility->testingAvailable("TB","Xpert") << ", " << facility->getUpName() << ", " << facility->getLongitude() << ", " << facility->getLatitude() << "\n";
        }

        //if (facility->getTier() == 2) {
        //    latF = facility->getLatPtr();
        //    for (auto latFac : latF) {
        //        myfile << "lateral facility name for zonal labs = " << latFac->getName() << "\n";
        //    }
        //}
        i = i + 1;
    }

    if (mapDebug)  std::cout << "inside map facilities before totals " << "\n";

    if (noFacilities > 0) {
        avgTests = countTestsToday / noFacilities;
    }
    if (fileFlag) {
        myfile << "Resource-total.tests.today-" << countTestsToday << ",total.tests.this.cycle-" << countTestsCycle << ",avg.test.today.per.fac-" << avgTests << "\n";
        myfile.close();

        myfile1.close();
    }
    else {
        std::cout << "Resource-total.tests.today-" << countTestsToday << ",total.tests.this.cycle-" << countTestsCycle << ",avg.test.today.per.fac-" << avgTests << "\n";
    }

    if (mapDebug) std::cout << "inside map facilities leaving " << "\n";
}


