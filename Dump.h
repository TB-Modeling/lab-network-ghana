//
//  Dump.h
//
//
//  Created by L. Dombrowski on 2/29/2020.
//


#ifndef DUMP_H
#define DUMP_H

#include <string>
#include <vector>

#include "Disease.h"
#include "Test.h"
#include "Region.h"
#include "District.h"

using namespace std;


// dump the disease vector array to a file
void dumpDisease(vector<Disease*> diseases);

// dump the test vector array to a file
void dumpTest(vector<Test*> tests);

// dump the disease list vector array to a file
void dumpDiseaseList(vector<string> diseaseList);

// dump the region vector
void dumpRegion(vector<Region*> regions);

// dump the district vector array to a file
void dumpDistrict(vector<District*> districts);


#endif // DUMP_H
