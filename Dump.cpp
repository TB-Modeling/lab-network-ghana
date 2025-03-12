//
//  Dump.cpp
//  mdrtb
//
//  Created by L. Dombrowski on 5/29/2020.
//  Copyright (c) 2015 Trinity. All rights reserved.
//
#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include "Dump.h"


using namespace std;





// dump the disease vector array to a file
void dumpDisease(vector<Disease*> diseases) {

    ofstream myfile;

    myfile.open("./dump/diseases.csv");

    // store away specific disease parameters
    myfile << " Disease" << ", mortality" << ", incidence" << ", transmission" << ", priority" << ", treatedPatientPercentage" << ", treatment time" << ", relapse time" << ", init treat success";
    myfile << ", int treat death" << ", init treat LtoF" << ", relapse success" << ", no treat test time" << ", no treat split per" << ", no treat death1" << ", no treate split 2 per" << ", no treat death2";
    myfile << ", per drug resist" << ", per sense test1" << ", per spec test1" << ", per sense test2" << ", per spec test2" << "\n";

    for (auto dS : diseases) {

        myfile <<  dS->getName() << ", " << dS->getMortality() << ", " << dS->getIncidence() << ", " << dS->getTransmission() << ", " << dS->getPriority() << ", " << dS->getTreatedPercentage();
        myfile << ", " << dS->getTreatmentTime() << ", " << dS->getRelapseTime() << ", " << dS->getInitialTreatmentSuccessRate() << ", " << dS->getInitialTreatmentDeathRate() << ", " << dS->getInitialTreatmentLossToFollow();
        myfile << ", " << dS->getRelapseSuccessRate() << ", " << dS->getperDrugResistant() << ", " << dS->getTimeOnsetToPresentation() << ", " << dS->getPerUntreatedDie() << ", " << dS->getUntreatedDeathTime();
        myfile << ", " << dS->getPerUntreatedRepresentation() << ", " << dS->getTimeUntreatedRepresentation() << "\n";
    }


    myfile.close();
}

// dump the test vector array to a file
void dumpTest(vector<Test*> tests) {
    
    ofstream myfile;

    std::cout << "**** inside dump test *******" << "\n";
    
    myfile.open("./dump/tests.csv");

    // store away specific test parameters
    myfile << " Test" << ", disease" << ", no machines" << ", tests per machine" << ", cycles per ts" << ", max tests per ts" << ", tests this ts" << ", max test per cycle" << ", tests this reagent cycle";
    myfile << ", reagent cycle" << ", total tests" << ", priority" << ", sensitivity" << ", specficity" << ", sensitivityMD" << ", specificityMD" << ", tier0";
    myfile << ", tier1" << ", tier2" << ", tier3" << ", tier4" << ", tier5" << ", tier6" << ", tier7" << ", tier8" << ", tier9" << "\n";
    for (auto tS : tests) {

        myfile <<  tS->getTestName() << ", " << tS->getDiseaseName() << ", " << tS->getNoMachines() << ", " << tS->getTestsPerMachine() << ", " << tS->getCyclesPerTimestep() << ", " << tS->getMaxTestPerTimestep();
        myfile << ", " << tS->getTestsThisTimestep() << ", " << tS->getMaxTestPerCycle() << ", " << tS->getTestsThisReagentCycle() << ", " << tS->getReagentCycle() << ", " << tS->getTotalTests();
        myfile << ", " << tS->getPriority() << ", " << tS->getSense() << ", " << tS->getSpecif() << ", " << tS->getSenseMD() << ", " << tS->getSpecifMD();
        myfile << ", " << tS->getTier0() << ", " << tS->getTier1() << ", " << tS->getTier2() << ", " << tS->getTier3() << ", " << tS->getTier4();
        myfile << ", " << tS->getTier5() << ", " << tS->getTier6() << ", " << tS->getTier7() << ", " << tS->getTier8() << ", " << tS->getTier9() << "\n";
    }

    myfile.close();
}

// dump the disease list vector array to a file
void dumpDiseaseList(vector<string> diseaseList) {

    ofstream myfile;

    myfile.open("./dump/diseaseList.csv");

    myfile << "Disease" << "\n";
    for (auto dl : diseaseList) {

        myfile << dl << "\n";
    }

    myfile.close();
}

// dump the region vector array to a file
void dumpRegion(vector<Region*> regions) {
    
    ofstream myfile;

    std::cout << "**** inside dump regions *******" << "\n";
    
    myfile.open("./dump/regions.csv");

    // store away specific test parameters
    myfile << " Region" << ", ID" << ", population" << ", incidence" << "\n";

    for (auto reg : regions) {

        myfile <<  reg->getName() << ", " << reg->getID() << ", " << reg->getPopulation() << ", " << reg->getTBIncidence() << "\n";
    }

    myfile.close();
}

// dump the district vector array to a file
void dumpDistrict(vector<District*> districts) {
    
    ofstream myfile;

    std::cout << "**** inside dump districts *******" << "\n";
    
    myfile.open("./dump/districts.csv");

    // store away specific test parameters
    myfile << " District" << ", ID" << ", population" << ", incidence" << ", region" << ", region ID" << "\n";

    for (auto dist : districts) {

        myfile <<  dist->getName() << ", " << dist->getID() << ", " << dist->getPopulation() << ", " << dist->getTBIncidence() <<  ", " << dist->getRegionName() << ", " << dist->getRegionID() << "\n";
    }

    myfile.close();
}



//---------------------------------------





