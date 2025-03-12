//
//  Clean.cpp
//  mdrtb
//
//  Created by L. Dombrowski on 10/15/2021.
//  Copyright (c) 2015 Trinity. All rights reserved.
//
#include <iostream>
#include <vector>
#include <algorithm>
#include "FacilitiesList.h"
#include "Clean.h"


using namespace std;





// remove all the test, disease, patient, sample and facility objects
void cleanData(FacilitiesList* list, vector<Test*> tests, vector<string> diseaseList, vector<Disease*> diseases, vector<Region*> reg, vector<District*> dist) {


    Diagnosis* d;
    Testing* t;
    vector<Patient*> p;
    vector<Patient*> pI;
    vector<Sample*> s;
    vector<Sample*> sI;
    vector<facilityAttributes*> attr;

    int countF;
    bool debug = false;

    int diseaseCount;
    int diseaseIndex;
    string diseaseName;
    string subDirBase;
    string subDir;
    string yearDir;
    string reason;
    string testName;

    // find out how many diseases to save stats for
    // use this as an count to create arrays for each of the diseases being run
    diseaseCount = diseaseList.size();

     // run through the facilities list and remvoe facility objects and remove pointer
    // in each facility, remove patients and samples associated with each facility
    vector<Facility*> facilities = list->getListPtr();

    countF = facilities.size();

    std::cout << "Clean data no. of facilities =  " << countF << "\n";


    // loop through each facility and the number of Patients and Samples at each facility
    for (auto facility : facilities) {

        // get the up name for this facility
        d = facility->getDiagnosisPtr();
        t = facility->getTestingPtr();
        attr = facility->getFacilityAttributesPtr();
        p = d->getPatientPtr();
        pI = d->getInactivePatientPtr();
        s = t->getSamplePtr();
        sI = t->getInactiveSamplePtr();

        if (debug) std::cout << "Clean data before patient loop n = " << p.size()  << "\n";

        for (auto pat : p) {

            delete pat;
         }      
                  
         // clear the pointer 
         p.clear();

         if (debug) std::cout << "Clean data before inactive patient loop patients now = " << p.size() << ", patient inactive = " << pI.size() << "\n";



        // now loop through the inactive patients
        for (auto patI : pI) {

            //if (debug) std::cout << "patient alive = " << patI->isAlive() << "\n";
            delete patI;
        }
                
        // clear the pointer
        pI.clear();
        
        if (debug) std::cout << "Clean data after pI loop patient inactive = " << pI.size() << ", sample size = " << s.size()  << "\n";
        
        // now loop through the samples
        for (auto samp : s) {

            delete samp;
        }
        
        // clear the pointer list
        s.clear();
        
        if (debug) std::cout << "Clean data after sample loop n = " << s.size()  << "\n";

        // now loop through the inactive samples
        for (auto samp : sI) {

            delete samp;
        }
        
        // clear the pointer list
        sI.clear();
        
        if (debug) std::cout << "Clean data after inactive sample loop n = " << sI.size()  << "\n";

         // now loop through the facility attributes
        for (auto a : attr) {

            delete a;
        }
        
        // clear the pointer list
        attr.clear();
        
        if (debug) std::cout << "Clean data after facility attribute loop n = " << attr.size()  << "\n";

        // delete facility
        delete facility;
    }
    // clean up facility pointer list
    facilities.clear();


    if (debug) std::cout << "Clean data after delete facility size = "  << facilities.size() << "\n";
         
    // clean up facilityList pointer list
    list->clear();

    // run through the tests list and remove test objects and remove pointer
    for (auto t : tests) {

        delete t;
    }

    // clear the pointer list
    //t.clear();

    if (debug) std::cout << "Clean data after removing test objects"  << "\n";

    
    // run through diseases and remove disease objects
    // run through the tests list and remove test objects and remove pointer
    for (auto d : diseases) {

        delete d;
    }

    //run through regions and remove region objects
    for (auto region : reg) {

        delete region;
    }


    //run through districts and remove district objects
    for (auto district : dist) {

        delete district;
    }


    // clear the pointer list
    //d.clear();


    for (diseaseIndex = 0; diseaseIndex < diseaseCount; diseaseIndex++) {

    }
    
    if (debug) std::cout << "Clean data after removing disease objects"  << "\n";
                    

     if (debug) std::cout << "end of clean data"  << "\n";
}




  





//---------------------------------------




