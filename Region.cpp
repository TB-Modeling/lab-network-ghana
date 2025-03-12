//
//  Region.cpp
//  mdrtb
//
//  Created by L. Dombrowski on 8/30/2022.
//
#include <random>
#include <iostream>
#include <vector>
#include <algorithm>
#include "Region.h"

using namespace std;


Region::Region() {
};

Region::Region(string name, int id, float incidence) {
    
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

    // default population to 0
    _population = 0;

}




Region::~Region() {
    //cout<<"Removing Region "<<_id<<endl;
}


