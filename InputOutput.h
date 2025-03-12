

//
//  InputOutput.h
//  mdrtb
//
//  Created by L. Dombrowski on 1/21/20

//
#pragma once
#ifndef INPUTOUTPUT_H
#define INPUTOUTPUT_H

#include <string>
#include <vector>
#include <fstream>
#include "Disease.h"
#include "Test.h"
#include "FacilitiesList.h"
#include "GlobalParameters.h"

using namespace std;



    // read in data from a comma seperated file and store in string vector
    vector<vector<string>> readDataInput(const std::string& filename);

    // read in facilites as comma seperated file and store in string vector
    vector<vector<string>> readInputFile(const std::string& filename);

    // read in facilites as comma seperated file and store in string vector  - different file format
    vector<vector<string>> readInputFile2(const std::string& filename);
     
    // read in facilites as comma seperated file and store in string vector  - different file format
    vector<vector<string>> readInputFile3(const std::string& filename);
    
    // read in facilites as comma seperated file and store in string vector  - different file format
    vector<vector<string>> readInputFile4(const std::string& filename);

    // read in facilites as comma seperated file and store in string vector
    vector<vector<string>> readInputFile6(const std::string& filename);

    inline bool fileExists(const std::string& name) {
        if (FILE* file = fopen(name.c_str(), "r")) {
            fclose(file);
            return true;
        }
        else {
            return false;
        }
    }


    // read in snapshot data from a comma seperated file and facilities, patients, samples, tests and diseases
    void readSnapshot(const std::string& filename, FacilitiesList* facilities, vector<string>* diseaseList, vector<Disease*>* diseases, vector<Test*>* tests);

    // store away facilities, patients, samples, tests and diseases in a comma seperated file
    void saveSnapshot(const std::string& filename, FacilitiesList* list, vector<string> diseaseList, vector<Disease*> diseases, vector<Test*> tests);

    // to create combined list
    void makeNewList(vector<vector<string>> maina, vector<vector<string>> ernest);

    // to create combined list
    void idMatching(vector<vector<string>> id);

    // to create combined list
    void updateTravelTime(vector<vector<string>> pop, vector<vector<string>> travel);

    // to create combined list
    void updateTravelCatch(vector<vector<string>> travel, vector<vector<string>> facilities);

    // to find closest regional hospital
    void updateTravelRegDistance(vector<vector<string>> facilities);

    // using this point as starting point find coord points around and see if travel times go up or ar the same
    void walkGrid(float x, float y, int time, string facility, int id, float inc, int dir, travelData* coords, std::size_t noCoords);

    //fill in grid cells that don't have facility ownership from surrounding cells
    void fillGridCells(float inc, travelData* coords, std::size_t noCoords);

    // update master facilities list
    void updateMasterList(vector<vector<string>> master, vector<vector<string>> newList);

    // add list to master facilities list
    void createMasterList(vector<vector<string>> master, vector<vector<string>> newList);

    // update facility types on masterlist
    void removeEmptyDuplicates(vector<vector<string>> masterListEmpty);
    
    // update master facilities list - map to recognized types
    void mapTypesInMasterList(vector<vector<string>> master, vector<vector<string>> types);

    // update master facilities list with cities
    void addCitiesToMasterList(vector<vector<string>> master, vector<vector<string>> cities);

    // update master facilities list
    void mapDistrictsRegionsTo260Sixteen(vector<vector<string>> master, vector<vector<string>> districts, vector<vector<string>> regions, vector<vector<string>> districtRegion);

    // add list to master facilities list
    void cullMasterList(vector<vector<string>> master, vector<vector<string>> districtRegion);

    // just add master list with results from google query
    void masterListPlusGoogle(vector<vector<string>> masterList, vector<vector<string>> googleList);

    // check two different names for similarity
    bool sameName(string name1, string name2 );

    // check two different names for similarity
    int sameName2(string name1, string name2);

    // Convert facility name to sanitized string - type boolean determines whether to remove (hospital, ChPS, clinic) is removed from name
    string convertName(string name1, bool type);

    // check to see if  name is generic
    int checkGenericName(string name1);

    // check to see if either name is generic
    int checkGenericNamePair(string name1, string name2);

    // routine to check levenshtein distance between two strings for string similarity
    // the larger the returned number, the more dissimilar the strings
    int levenshteinDistance(const std::string& s1, const std::string& s2);

    // Clean up the district names
    string cleanDistrict(string oldDistrict);

    // check to see if facility types are the same
    bool typeCheck(string type1, string type2);

    // find distance between testing center and facility 
    void mapFacDist(vector<vector<string>> facilities, vector<vector<string>> distances);

    // clean up towns.csv plus geo-names 
    void cleanTowns(vector<vector<string>> towns, vector<vector<string>> geoTowns);

    // create google API statements to check travel times between facilities and zonal labs
    void outputGoogleApi(vector<vector<string>> masterList, vector<vector<string>> zonalLabs);

    // find closest regional hospital to facility
    void facilityToRegHospital(vector<vector<string>> facilityList);

    // see change run specific seed paramenters
    unsigned getSeed(vector<vector<string>> runSpecific);

    // see change run specific diseaseList paramenters
    vector<string> getDiseaseList(vector<vector<string>> runSpecific);

    // see change run specific diseaseList paramenters
    void getTiers(vector<vector<string>> runSpecific);

    // see change run specific year parameters
    void getYears(vector<vector<string>> runSpecific);

    // load scenario file, directory and name variables
    scenarioParam getScenarioParameters(int scenario, scenarioParam params[]);

    // create region-district array from outside files
    regionDistrict createRegionDistrictArrays(vector<vector<string>> regionDistricts);

    // create facility name-id array from facility list
    regionDistrict createFacilitiesArray(FacilitiesList* list);

    // remove leading/trailing spaces, make uppercase
    string cleanString(string word);


#endif // INPUTOUTPUT_H

