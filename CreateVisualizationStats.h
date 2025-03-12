//
//  CreateVisualizationStats.h
//  
//
//  Created by L. Dombrowski on 6/2/21
//
// merge agent and sample stats into 1 file for visualization program
//

//
#ifndef CREATEVISUALIZATIONSTATS_H
#define CREATEVISUALIZATIONSTATS_H

#include <vector>
#include <string>

#include "Facility.h"
#include "FacilitiesList.h"
#include "Test.h"

using namespace std;

// output current agent stats
void visualizationStats(FacilitiesList* list, vector<string> diseaseList, bool fileFlag, bool yearly, int timeStep);



#endif // CREATEVISUALIZATIONSTATS_H


