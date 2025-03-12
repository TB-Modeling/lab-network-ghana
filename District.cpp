//
//  District.cpp
//  mdrtb
//
//  Created by L. Dombrowski on 8/30/2022.
//
#include <random>
#include <iostream>
#include <vector>
#include <algorithm>
#include "District.h"

using namespace std;


District::District() {
};

District::District(string name, int id, float incidence) {
    
    // store away information on creation
    _name = name;

    // generate a unique id for the district - this will be used for finding the district
    _id = id;

    // save away incidence
    _TBincidence = incidence;
    _HIVincidence = incidence;
    _HCVincidence = incidence;
    _YFincidence = incidence;
    _Menincidence = incidence;
    _Meaincidence = incidence;

    // store away region data for this district
    _regionName = "";
    _regionID = 0;

    // default population to 0
    _population = 0;

}


District::District(string name, int id, float incidence, string region, int regionID) {
    
    // store away information on creation
    _name = name;

    // generate a unique id for the district - this will be used for finding the district
    _id = id;

    // save away type
    _TBincidence = incidence;
    _HIVincidence = incidence;
    _HCVincidence = incidence;
    _YFincidence = incidence;
    _Menincidence = incidence;
    _Meaincidence = incidence;

    // store away region data for this district
    _regionName = region;
    _regionID = regionID;

    // default population to 0
    _population = 0;

}


District::~District() {
    //cout<<"Removing District "<<_id<<endl;
}


