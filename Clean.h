//
//  Clean.h
//
//
//  Created by L. Dombrowski on 10/15/2021.
//


#ifndef CLEAN_H
#define CLEAN_H

#include <string>
#include <vector>

#include "Disease.h"
#include "FacilitiesList.h"
#include "District.h"
#include "Region.h"

using namespace std;


// dump the disease vector array to a file
void cleanData(FacilitiesList* list, vector<Test*> tests, vector<string> diseaseList, vector<Disease*> diseases, vector<Region*> reg, vector<District*> dist);




#endif // CLEAN_H
