//
//  InputOutput.cpp
//
//  Created by L.Dombrowski on 1/21/2020.

//

#include <iostream>
#include <sys/stat.h> 
#include <sstream>
#include <string>
#include <vector>
#include <cmath>
#include <cstring>

//#include "EasyXLS.h"
#include "InputOutput.h"
#include "GlobalParameters.h"
#include "FacilitiesList.h"

using namespace std;

// read in data from a comma seperated file and store in string vector
vector<vector<string>> readDataInput(const std::string& filename) {
    // read in each of the columns 
    // create the each disease object

    string line;
    vector<vector<string>> matrix;
    std::vector<std::string>::iterator it;

    //open file
    std::ifstream myfile(filename);

    //std::cout << "Open "<< filename  << endl;
    
    if (myfile.is_open())
    {
        // loop through .csv files, using , as delimiter
        while (getline(myfile, line, '\n'))
        {
            stringstream ss(line);
            vector<string> cols;

            string in_line;
            //cout << ss.str() << endl;

            // now pull out each field in each line seperated by commas
            while (getline(ss, in_line, ',')) {
                // cout << in_line << endl;
                cols.push_back(in_line);
            }

            // DEBUG4 - true
            if (DEBUG4) {
                for (it = cols.begin(); it < cols.end(); it++)
                    std::cout << ' ' << *it;

                std::cout << '\n';
            }
            matrix.push_back(cols);
        }
        myfile.close();
    }
    else std::cout << "Unable to open "<< filename << endl;

    if (matrix.size() < 2) std::cout << "Problem with reading input file " << filename << " size = " << matrix.size() << endl;

    return matrix;
}




// read in regional to district hospital relationship csv file and create facilities agent from data
// the second col - regional hospital name, 3rd col - district hospital name, 4th col - distance between regional and district hospital,
// col 25 - type of facility (tier), 37th col - Longitude, 38th col - Latitude, 39th col - region, 40th col - district, 41th col town, col 42 - facility.id,
//col 43 - Xpert machine here
// make sure if converting from .xlxs to .csv that you first remove commas from names

vector<vector<string>> readInputFile(const std::string& filename) {
    // read in each of the 6 columns 
    // create the each district facility object

    string line;
    vector<vector<string>> matrix;
    std::vector<std::string>::iterator it;

    //open file
    std::ifstream myfile(filename);
    if (myfile.is_open())
    {
        // loop through .csv files, using , as delimiter
        while (getline(myfile, line, '\n'))
        {
            stringstream ss(line);
            vector<string> cols;
            vector<string> smallCols;
            string in_line;
            //std::cout << ss.str() << endl;

            // now pull out each field in each line seperated by commas
            while (getline(ss, in_line, ',')) {
                // cols.push_back(std::stod(in_line, 0));
                cols.push_back(in_line);
            }

            if (DEBUG4) {
                for (auto i = 0u; i < cols.size(); i++) {
                    std::cout << " cols no = " << i << ", cols[i] = " << cols[i] << '\n';
                }
            }

            // only keep the columns that we need for now
            smallCols.push_back(cols[1]);       // regional hospital
            smallCols.push_back(cols[2]);       // facility name
            smallCols.push_back(cols[3]);       // distance to regional hospital
            smallCols.push_back(cols[24]);      // type of facillity
            smallCols.push_back(cols[36]);      // longitude
            smallCols.push_back(cols[37]);      // latitude
            smallCols.push_back(cols[39]);      // region
            smallCols.push_back(cols[40]);      // district
            smallCols.push_back(cols[42]);      // unitID
            smallCols.push_back(cols[43]);      // Xpert

            if (DEBUG4) {
                for (it = smallCols.begin(); it < smallCols.end(); it++)
                    std::cout << ' ' << *it;
                std::cout << cols[1] << ',' << cols[2] << ',' << cols[3] << ',' << cols[24] << ',' << cols[36] << ',' << cols[37] <<',' << cols[39] << ',' << cols[40] <<',' << cols[42] <<'\n';
                //std::cout << '\n';
            }
            matrix.push_back(smallCols);
        }
        myfile.close();
    }
    else cout << "Unable to open " << filename << endl;

    if (matrix.size() < 2) cout << "Problem with reading input file" << endl;

    return matrix;
}

// format
// col0  col1    col2            col3   col4     col5        col6        col7        col8       col9     col10       col11  col 12   col13       col14   col15           col16       col17       col18            col19      
// ID	Status	Diff.District	Ref.ID	Name	Same.name	Longitude	Latititude	Same.GPS	Type	Orig.type	Address	Town	District	Region	District.Code	GPS.Source	List.Source	Primary.Town	Alt.Town
// read in regional to district hospital relationship csv file and create facilities agent from data
// the second col - regional hospital name, 3rd col - district hospital name, 4th col - distance between regional and district hospital,
// col 25 - type of facility (tier), 37th col - Longitude, 38th col - Latitude, 39th col - region, 40th col - district, 41th col town, col 42 - facility.id,
// col 43 - Xpert machine here
// make sure if converting from .xlxs to .csv that you first remove commas from names
vector<vector<string>> readInputFile2(const std::string& filename) {
    // read in each of the 6 columns 
    // create the each district facility object

    string line;
    vector<vector<string>> matrix;
    std::vector<std::string>::iterator it;

    //open file
    std::ifstream myfile(filename);
    if (myfile.is_open())
    {
        // loop through .csv files, using , as delimiter
        while (getline(myfile, line, '\n'))
        {
            stringstream ss(line);
            vector<string> cols;
            vector<string> smallCols;
            string in_line;
            //std::cout << ss.str() << endl;

            // now pull out each field in each line seperated by commas
            while (getline(ss, in_line, ',')) {
                // cols.push_back(std::stod(in_line, 0));
                cols.push_back(in_line);
            }

            if (DEBUG4) {
                for (auto i = 0u; i < cols.size(); i++) {
                    std::cout << " cols no = " << i << ", cols[i] = " << cols[i] << '\n';
                }
            }

            // only keep the columns that we need for now
            smallCols.push_back(cols[1]);       // regional hospital
            smallCols.push_back(cols[2]);       // facility name
            smallCols.push_back(cols[3]);       // distance to regional hospital
            smallCols.push_back(cols[24]);      // type of facillity
            smallCols.push_back(cols[36]);      // longitude
            smallCols.push_back(cols[37]);      // latitude
            smallCols.push_back(cols[39]);      // region
            smallCols.push_back(cols[40]);      // district
            smallCols.push_back(cols[42]);      // unitID
            smallCols.push_back(cols[43]);      // Xpert

            if (DEBUG4) {
                for (it = smallCols.begin(); it < smallCols.end(); it++)
                    std::cout << ' ' << *it;
                std::cout << cols[1] << ',' << cols[2] << ',' << cols[3] << ',' << cols[24] << ',' << cols[36] << ',' << cols[37] <<',' << cols[39] << ',' << cols[40] <<',' << cols[42] <<'\n';
                //std::cout << '\n';
            }
            matrix.push_back(smallCols);
        }
        myfile.close();
    }
    else cout << "Unable to open " << filename << endl;

    if (matrix.size() < 2) cout << "Problem with reading input file" << endl;

    return matrix;
}




// format
// Region.hospital	Facility	Dist.to.reg.hosp	tier	 Longitude	 Latitude 	region	district	Xpert unitID
// make sure if converting from .xlxs to .csv that you first remove commas from names
vector<vector<string>> readInputFile3(const std::string& filename) {

    // read in each of the 6 columns 
    // create the each district facility object

    string placeHolder = "0";
    string line;
    vector<vector<string>> matrix;
    std::vector<std::string>::iterator it;

    //open file
    std::ifstream myfile(filename);
    if (myfile.is_open())
    {
        // loop through .csv files, using , as delimiter
        while (getline(myfile, line, '\n'))
        {
            stringstream ss(line);
            vector<string> cols;
            vector<string> smallCols;
            string in_line;
            //std::cout << ss.str() << endl;

            // now pull out each field in each line seperated by commas
            while (getline(ss, in_line, ',')) {
                // cols.push_back(std::stod(in_line, 0));
                cols.push_back(in_line);
            }

            if (DEBUG4) {
                for (auto i = 0u; i < cols.size(); i++) {
                    std::cout << " cols no = " << i << ", cols[i] = " << cols[i] << '\n';
                }
            }

            // only keep the columns that we need for now
            smallCols.push_back(cols[0]);      // regional hospital
            smallCols.push_back(cols[1]);      // facility name
            smallCols.push_back(cols[2]);      // distance to regional hospital
            smallCols.push_back(cols[3]);      // type of facillity
            smallCols.push_back(cols[4]);      // longitude
            smallCols.push_back(cols[5]);      // latitude
            smallCols.push_back(cols[6]);      // region
            smallCols.push_back(cols[7]);      // district
            smallCols.push_back(cols[8]);      // Xpert
            smallCols.push_back(cols[9]);      // unitID
            //smallCols.push_back(placeHolder);             // Xpert

            if (DEBUG4) {
                for (it = smallCols.begin(); it < smallCols.end(); it++)
                    std::cout << ' ' << *it;
                std::cout << cols[1] << ',' << cols[2] << ',' << cols[3] << ',' << cols[24] << ',' << cols[36] << ',' << cols[37] <<',' << cols[39] << ',' << cols[40] <<',' << cols[42] <<'\n';
                //std::cout << '\n';
            }
            matrix.push_back(smallCols);
        }
        myfile.close();
    }
    else cout << "Unable to open " << filename << endl;

    if (matrix.size() < 2) cout << "Problem with reading input file" << endl;

    return matrix;
}




// format
// Name	Region	District	CR District	 Longitude 	 Latitude	Tier	Type	Original Type	Xpert	other name

// make sure if converting from .xlxs to .csv that you first remove commas from names
vector<vector<string>> readInputFile4(const std::string& filename) {


    // read in each of the 6 columns 
    // create the each district facility object

    string placeHolder = "0";
    string line;
    vector<vector<string>> matrix;
    std::vector<std::string>::iterator it;

    //open file
    std::ifstream myfile(filename);
    if (myfile.is_open())
    {
        // loop through .csv files, using , as delimiter
        while (getline(myfile, line, '\n'))
        {
            stringstream ss(line);
            vector<string> cols;
            vector<string> smallCols;
            string in_line;
            //std::cout << ss.str() << endl;

            // now pull out each field in each line seperated by commas
            while (getline(ss, in_line, ',')) {
                // cols.push_back(std::stod(in_line, 0));
                cols.push_back(in_line);
            }

            if (DEBUG4) {
                for (auto i = 0u; i < cols.size(); i++) {
                    std::cout << " cols no = " << i << ", cols[i] = " << cols[i] << '\n';
                }
            }

            // only keep the columns that we need for now
            smallCols.push_back(cols[0]);       // facility name
            smallCols.push_back(cols[1]);      // region
            smallCols.push_back(cols[2]);      // district
            smallCols.push_back(cols[3]);      // tier
            smallCols.push_back(cols[4]);      // type of facillity
            smallCols.push_back(cols[5]);      // longitude
            smallCols.push_back(cols[6]);      // latitude
            smallCols.push_back(cols[7]);      // type of facillity
            smallCols.push_back(cols[8]);      // case registration type of facility
            smallCols.push_back(cols[9]);      // Xpert placement here

            if (DEBUG4) {
                for (it = smallCols.begin(); it < smallCols.end(); it++)
                    std::cout << ' ' << *it;
                std::cout << cols[0] << ',' << cols[1] << ',' << cols[2] << ',' << cols[3] << ',' << cols[4] << ',' << cols[5] <<',' << cols[6] << ',' << cols[7] << ',' << cols[8] << ',' << cols[9] <<'\n';
                //std::cout << '\n';
            }
            matrix.push_back(smallCols);
        }
        myfile.close();
    }
    else cout << "Unable to open " << filename << endl;

    if (matrix.size() < 2) cout << "Problem with reading input file" << endl;

    return matrix;
}


// read in regional to district hospital relationship csv file and create facilities agent from data
// Region.hospital	Facility	Dist.to.reg.hosp	tier	 Longitude	 Latitude 	region	district  Xpert  unitID
//dummy for now - Xpert machine here
// make sure if converting from .xlxs to .csv that you first remove commas from names

vector<vector<string>> readInputFile6(const std::string& filename) {
    // read in each of the 6 columns 
    // create the each district facility object

    string line;
    vector<vector<string>> matrix;
    std::vector<std::string>::iterator it;

    //open file
    std::ifstream myfile(filename);
    if (myfile.is_open())
    {
        // loop through .csv files, using , as delimiter
        while (getline(myfile, line, '\n'))
        {
            stringstream ss(line);
            vector<string> cols;
            vector<string> smallCols;
            string in_line;
            //std::cout << ss.str() << endl;

            // now pull out each field in each line seperated by commas
            while (getline(ss, in_line, ',')) {
                // cols.push_back(std::stod(in_line, 0));
                cols.push_back(in_line);
            }

            if (DEBUG4) {
                for (auto i = 0u; i < cols.size(); i++) {
                    std::cout << " cols no = " << i << ", cols[i] = " << cols[i] << '\n';
                }
            }

            // only keep the columns that we need for now
            smallCols.push_back(cols[0]);      // regional hospital
            smallCols.push_back(cols[1]);      // facility name
            smallCols.push_back(cols[2]);      // distance to regional hospital
            smallCols.push_back(cols[3]);      // type of facillity
            smallCols.push_back(cols[4]);      // longitude
            smallCols.push_back(cols[5]);      // latitude
            smallCols.push_back(cols[6]);      // region
            smallCols.push_back(cols[7]);      // district
            smallCols.push_back(cols[8]);      // Xpert
            smallCols.push_back(cols[9]);      // unitID

            if (DEBUG4) {
                for (it = smallCols.begin(); it < smallCols.end(); it++)
                    std::cout << ' ' << *it;
                std::cout << cols[0] << ',' << cols[1] << ',' << cols[2] << ',' << cols[3] << ',' << cols[4] << ',' << cols[5] <<',' << cols[6] << ',' << cols[7] <<',' << cols[8] << ',' << cols[9] << '\n';
                //std::cout << '\n';
            }
            matrix.push_back(smallCols);
        }
        myfile.close();
    }
    else cout << "Unable to open " << filename << endl;

    if (matrix.size() < 2) cout << "Problem with reading input file" << endl;

    return matrix;
}






// read in snapshot data from a comma seperated file and facilities, patients, samples, tests and diseases
void readSnapshot(const std::string& filename, FacilitiesList* list, vector<string> * diseaseList, vector<Disease*> * diseases, vector<Test*> * tests) {


    struct SampleArr {
        string patientID;
        Sample* sample;
    };
    
    vector<SampleArr*> sampleArr;


    int j;
    string name;
    string id;
    float longitude;
    float latitude;
    string type;
    string upConn;
    string latConn;
    string downConn;
    float dist = 0;
    int tier;
    int pop;
    float inc;
    float bins[10];
    string region;
    string district;
    string town;
    bool diag;
    bool debug = false;
    bool tier0 = false;
    bool tier1 = false;
    bool tier2 = false;
    bool tier3 = false;
    bool tier4 = false;
    bool tier5 = false;
    bool tier6 = false;
    bool tier7 = false;
    bool tier8 = false;
    bool tier9 = false;
    bool Xpert = false;
    string facility;
    Diagnosis* diagnosis;
    //FacilitiesList* list;
    
   if (debug) std::cout << " inside readSnapshot " << '\n';

    // read in the snapshot file
   vector<vector<string>> raw = readDataInput(filename);

   // the raw data has a key word in the first position
   // they will be Facility, FacAttr, Patient, Sample, DiseaseInRun, Disease, and Test
   // Each facility will be grouped with it's FacAttr, Patients and Samples
   // DiseaseInRun is a list of the diseases to be analyzed during this run
   // Disease is a standard description of a disease 
   // Test is a standard description of a test

   // first loop through the raw data pulling out a set of Facility, FacAttr, Patient and Sample lines
   // use those to create a Facility with Patients and Samples
   for (auto i = 1u; i < raw.size(); i++) {

       if (debug) std::cout << " inside readSnapshot loop Object =  " << raw[i][0] << '\n';

            // store time related data
       if (raw[i][0] == "Random") {

            timeStep = stoi(raw[i][1]);
            seed = stof(raw[i][2]);
            maxNoSeq100 = stoi(raw[i][3]);
            maxNoSeqPop = stoi(raw[i][4]);
            countSeq100 = stoi(raw[i][5]);
            countSeqPopTB = stoi(raw[i][6]);
            countSeqPopHIV = stoi(raw[i][7]);
            countSeqPopHCV = stoi(raw[i][8]);
            countSeqPopYF = stoi(raw[i][9]);
            countSeqPopMen = stoi(raw[i][10]);
            countSeqPopMea = stoi(raw[i][11]);
            nextNoSeq100 = stoi(raw[i][12]);
            nextNoSeqPopTB = stoi(raw[i][13]);
            nextNoSeqPopHIV = stoi(raw[i][14]);
            nextNoSeqPopHCV = stoi(raw[i][15]);
            nextNoSeqPopYF = stoi(raw[i][16]);
            nextNoSeqPopMen = stoi(raw[i][17]);
            nextNoSeqPopMea = stoi(raw[i][18]);
            popTB = stof(raw[i][19]);
            popHIV = stof(raw[i][20]);
            popHCV = stof(raw[i][21]);
            popYF = stof(raw[i][22]);
            popMen = stof(raw[i][23]);
            popMea = stof(raw[i][24]);
       }

       if (debug) std::cout << " inside readSnapshot after random  " << '\n';

       if (raw[i][0] == "DiseaseInRun") {

            if (debug) std::cout << " inside diseaseList no =  " << raw[i][0] << "," << raw[i][1] <<  "," << raw[i][2] << '\n';   

            for (int j = 0; j < stoi(raw[i][1]); j++) {
           
                if (debug) std::cout << " inside diseaseList j =  " << j << ", disease = " << raw[i][j+2] << '\n';
                
                // store disease on list
                diseaseList->push_back(raw[i][j+2]);
            }
       }
       if (debug) std::cout << " inside readSnapshot after diseaseList =  " << raw[i][0] << '\n';

       if (raw[i][0] == "Disease") {

           //Object	 Disease	 mortality	 incidence	 incAdjType	 incAdjFactor	 incidence3	incidence4	 incidence5	 incidence0=6	 incidence7	 incidence8	 incidence9	  transmission	 priority morbidity preTestLtoF preTestLtoFDeath space1  space1 probabilityPatienReturns  treatedPatientPercentage treatment time	 relapse time	 init treat success	 int treat death	 pre-treat LtoF   treat LtoF	 relapse success	 per drug resist	 time onset to pre	 untreat die per	 untreat die time	 untest repre per   untest repre time	 untreat repre per   untreat repre time   perLostSamples
            diseases->push_back(new Disease(raw[i][1], stof(raw[i][2]), stof(raw[i][3]), raw[i][4], stof(raw[i][5]), stof(raw[i][6]), stoi(raw[i][7]), stof(raw[i][8]), stof(raw[i][9]), stof(raw[i][10]), stof(raw[i][11]), stof(raw[i][12]), stof(raw[i][13]), stof(raw[i][14]), stof(raw[i][15]), stof(raw[i][16]), stoi(raw[i][17]), stof(raw[i][18]), stof(raw[i][19]), stof(raw[i][20]),  0, stof(raw[i][22]), stof(raw[i][23]), stof(raw[i][24]), stoi(raw[i][25]), stof(raw[i][26]), stoi(raw[i][27]), stof(raw[i][28]), stoi(raw[i][29])));
       }

       if (debug) std::cout << " inside readSnapshot before test " << '\n';

       if (raw[i][0] == "Test") {
     
           if (stoi(raw[i][20]) == 1) {
               tier0 = true;
           }
           if (stoi(raw[i][21]) == 1) {
               tier1 = true;
           }
           if (stoi(raw[i][22]) == 1) {
               tier2 = true;
           }
           if (stoi(raw[i][23]) == 1) {
               tier3 = true;
           }
           if (stoi(raw[i][24]) == 1) {
               tier4 = true;
           }
           if (stoi(raw[i][25]) == 1) {
               tier5 = true;
           }
           if (stoi(raw[i][26]) == 1) {
               tier6 = true;
           }
           if (stoi(raw[i][27]) == 1) {
               tier7 = true;
           }
           if (stoi(raw[i][28]) == 1) {
               tier8 = true;
           }
           if (stoi(raw[i][29]) == 1) {
               tier9 = true;
           }
           
           //Object	 Test	 disease	 no machines	 tests per machine	 cycles per ts(5)	 max tests per ts	 tests this ts	 max test per cycle	 tests this reagent cycle	 reagent cycle	 total tests(11)	 priority	 batch wait time(13)  Patient Return Time  sensitivity	 specficity	 sensitivityMD(16)	 specificityWithTestMD	sensitivityWithTestMD	 specificityMD tier0	 tier1	 tier2	 tier3	 tier4	 tier5	 tier6	 tier7	 tier8	 tier9
           Test* newTest = new Test(raw[i][1], raw[i][2], stoi(raw[i][4]), stoi(raw[i][5]), stoi(raw[i][6]), stoi(raw[i][8]), stoi(raw[i][10]), stoi(raw[i][12]), stoi(raw[i][13]), stoi(raw[i][14]),  stof(raw[i][15]), stof(raw[i][16]), stof(raw[i][17]), stof(raw[i][18]), stof(raw[i][19]), stof(raw[i][20]), tier0, tier1, tier2, tier3, tier4, tier5, tier6, tier7, tier8, tier9);
 
           // update current tests condition
           newTest->setTestsThisTimestep(stoi(raw[i][7]));
           newTest->setTestsThisReagentCycle(stoi(raw[i][9]));
           newTest->setTotalTests(stoi(raw[i][11]));


           tests->push_back(newTest);

       }

       if (debug) std::cout << " inside readSnapshot before facility " << '\n';

       if (raw[i][0] == "Facility") {
           j = 1;

           facility = raw[i][1];

           // create a vector of facilityAttribute pointers to be stored at each facility
           vector<facilityAttributes*> attrPtr;

           while (raw[i + j][0] != "FacilityEnd") {

               // fill population distribution bins
               bins[0] = stof(raw[i][16]);
               bins[1] = stof(raw[i][17]);
               bins[2] = stof(raw[i][18]);
               bins[3] = stof(raw[i][19]);
               bins[4] = stof(raw[i][20]);
               bins[5] = stof(raw[i][21]);
               bins[6] = stof(raw[i][22]);
               bins[7] = stof(raw[i][23]);
               bins[8] = stof(raw[i][24]);
               bins[9] = stof(raw[i][25]);

               name = raw[i][1];
               id = raw[i][2];
               type = raw[i][3];
               longitude = stof(raw[i][4]);
               latitude = stof(raw[i][5]);
               town = raw[i][6];
               district = raw[i][7];
               region = raw[i][8];
               tier = stoi(raw[i][9]);
               diag = stoi(raw[i][10]);
               upConn = raw[i][11];
               latConn = raw[i][12];
               downConn = raw[i][13];
               pop = stoi(raw[i][14]);
               inc = stof(raw[i][15]);
               //test = stoi(raw[i][11]);
               if (raw[i][26] == "1") {
                   Xpert = true;
               }
               else {
                   Xpert = false;
               }

               if (debug) std::cout << " inside readSnapshot before facility attributes" << '\n';

               while (raw[i + j][0] == "FacAttr") {

                   // handle facility specific attributes first
                   facilityAttributes* facAttr = new facilityAttributes;

                   facAttr->disease = raw[i+j][27];
                   facAttr->testName = raw[i + j][28];
                   if (stoi(raw[i + j][29]) == 1) {
                        facAttr->testing = true;
                   }
                   else {
                        facAttr->testing = false;
                   }
                   facAttr->incidence = stof(raw[i+j][30]);
                   facAttr->incAdjType = raw[i+j][31];
                   facAttr->incAdjFactor = stof(raw[i+j][32]);
                   facAttr->returnCycle = stoi(raw[i + j][33]);
                   facAttr->noMachines = stoi(raw[i + j][34]);
                   facAttr->maxReagentPerCycle = stoi(raw[i + j][35]);
                   facAttr->timeStepsInCycle = stoi(raw[i + j][36]);
                   facAttr->batchWaitTime = stoi(raw[i + j][37]);
                   facAttr->closetConnectionWithTesting = raw[i + j][38];
                   facAttr->closestConnectionDistance = stof(raw[i + j][39]);

                   attrPtr.push_back(facAttr);

                   j++;
               }

               if (debug) {
               
                    for (auto f: attrPtr) {

                        std::cout << " snapshot facattr facility = " << name << "disease = " << f->disease << ", test = " << f->testName << ", testing = " << f->testing;
                        std::cout << ", inc = " << f->incidence << ", incAdjFactor = " << f->incAdjFactor << ", up conn = " << f->closetConnectionWithTesting  << ", up dist = " << f->closestConnectionDistance  << '\n';
                    }
               }
               if (debug) std::cout << " inside readSnapshot after facility attributes" << '\n';
                
               // make the facility
               Facility* newFacility = new Facility(name, id, longitude, latitude, type, upConn, dist, tier, pop, inc, bins, region, district, diag, *diseaseList, *tests, attrPtr, Xpert);

               // add it to FacilitiesList
               list->addFacility(newFacility);

               // resolve lateral and down connections and up connection pointers after inporting the snapshot file with updateFacilityConnectioins


               // eat up Object keyword
               if (raw[i + j][0] == "Object") {
                   j++;
               }

               while (raw[i + j][0] == "Sample") {

                   // create and array of samples to check against patients.  Then after Patients created update sample patient ptr.
                   // with the patients
                   // Object	patient	 status	 age	 wait	 transit time	 transit time save	 transit time return	 transit time return save	 create time(9)	 results returned ts	 infected	 positive	 disease	 test(14)	 orig facilty	 testing facility	 testing facility dist	 priority

                   if (debug) std::cout << " samples  patient id =  " << raw[i + j][15] << ", sample = " << stof(raw[i + j][17]) << '\n';
                   Sample * s = new Sample(0, raw[i+j][15], raw[i + j][16], stof(raw[i + j][17]), raw[i + j][13], stoi(raw[i + j][11]), stoi(raw[i + j][18]), stof(raw[i + j][19]));

                   if (debug) std::cout << " made samples  patient id =  " << stoi(raw[i + j][2]) << ", sample = " << stoi(raw[i + j][12]) << '\n';
                   s->setStatus(stoi(raw[i + j][2]));
                   s->setAge(stoi(raw[i + j][3]));
                   s->setWait(stoi(raw[i + j][4]));
                   s->setTransitTime(stoi(raw[i + j][5]));
                   s->setTransitTimeSave(stoi(raw[i + j][6]));
                   s->setReturnTransitTime(stoi(raw[i + j][7]));
                   s->setReturnTransitTimeSave(stoi(raw[i + j][8]));
                   s->setResultsReturnedTime(stoi(raw[i + j][10]));
                   s->setPositive(stoi(raw[i + j][12]));
                   s->setTest(raw[i + j][14]);
                   s->setBatchWaitTime(stoi(raw[i + j][19]));

                   // now loop through standard tests to get the pointer for the test name in raw[i+j][14]
                   for (auto standTest : *tests) {

                       if (raw[i + j][14] == standTest->getTestName()) {

                           s->setTestPtr(standTest);

                           //std::cout << " updated test pointer in sample" <<  '\n';
                       }
                   }

                   if (debug) std::cout << " before push  patient id =  " << stoi(raw[i + j][2]) << ", sample = " << stoi(raw[i + j][12]) << '\n';
                   sampleArr.push_back(new SampleArr{ raw[i + j][1], s });

                   //  store samples in testing sample container
                   newFacility->addToSampleContainer(s);

                   j++;
                }

               if (debug) {
                   for (auto a : sampleArr) {
                       std::cout << " sampleArr patient id =  " << a->patientID << ", sample = " << a->sample << '\n';
                   }
               }

               // read in inactive samples and store them away
               while (raw[i + j][0] == "ISample") {

                   // Create sample and store in inactive sample array - don't connect up to inactive patients - may have to do this in the future
                   // Object	patient	 status	 age	 wait	 transit time	 transit time save	 transit time return	 transit time return save	 create time(9)	 results returned ts	 infected	 positive	 disease	 test(14)	 orig facilty	 testing facility	 testing facility dist	 priority

                   //std::cout << " samples  patient id =  " << raw[i + j][15] << ", sample = " << stof(raw[i + j][17]) << '\n';
                   Sample* s = new Sample(0, raw[i + j][15], raw[i + j][16], stof(raw[i + j][17]), raw[i + j][13], stoi(raw[i + j][11]), stoi(raw[i + j][18]),  stof(raw[i + j][19]));

                   //std::cout << " made samples  patient id =  " << stoi(raw[i + j][2]) << ", sample = " << stoi(raw[i + j][12]) << '\n';
                   s->setStatus(stoi(raw[i + j][2]));
                   s->setAge(stoi(raw[i + j][3]));
                   s->setWait(stoi(raw[i + j][4]));
                   s->setTransitTime(stoi(raw[i + j][5]));
                   s->setTransitTimeSave(stoi(raw[i + j][6]));
                   s->setReturnTransitTime(stoi(raw[i + j][7]));
                   s->setReturnTransitTimeSave(stoi(raw[i + j][8]));
                   s->setResultsReturnedTime(stoi(raw[i + j][10]));
                   s->setPositive(stoi(raw[i + j][12]));
                   s->setTest(raw[i + j][14]);
                   s->setBatchWaitTime(stoi(raw[i + j][19]));

                   // now loop through standard tests to get the pointer for the test name in raw[i+j][14]
                   for (auto standTest : *tests) {

                       if (raw[i + j][14] == standTest->getTestName()) {

                           s->setTestPtr(standTest);

                           //std::cout << " updated test pointer in sample" <<  '\n';
                       }
                   }

                   //  store samples in testing inactive sample container
                   newFacility->addToInactiveSampleContainer(s);

                   j++;
               }


               // eat up Object keyword
               if (raw[i + j][0] == "Object") {
                   j++;
               }

               while (raw[i + j][0] == "Patient") {

                   // create and array of patients and check against sample array to connect the two. 
                   //Object	id	 facility	 disease	 distance	 infected(5)	 drug resistant	 track for reg	 status	 test pos	 md pos(10)	 live	 age	 gender	 life expect	 time from onset(15)	 ts in treat	 ts not in treat	 no of clinical visits	 ts patient created	 ts patient no longer tracked(20)	 create sample ts	 results returned ts	 facility return time	 no returns for results	 infectiousness(25)	 immunity	 sample test

                   Patient * p = new Patient(raw[i+j][2], "", 0, "", 0, raw[i + j][3], stof(raw[i + j][4]), 0, stoi(raw[i + j][5]), stoi(raw[i + j][8]), raw[i + j][27], stoi(raw[i + j][19]));
                   
                   // now update all the attributes of the patient
                   p->setDrugResistant(stoi(raw[i + j][6]));
                   p->setTrackForRegistry(stoi(raw[i + j][7]));
                   p->setTestPos(stoi(raw[i + j][9]));
                   p->setMDPos(stoi(raw[i + j][10]));
                   p->setLive(stoi(raw[i + j][11]));
                   p->setAge(stoi(raw[i + j][12]));
                   p->setGender(stoi(raw[i + j][13]));
                   p->setLifeExpectancy(stoi(raw[i + j][14]));
                   p->setTimeFromOnset(stoi(raw[i + j][15]));
                   p->setTimeStepsInTreatment(stoi(raw[i + j][16]));
                   p->setTimeStepsNotInTreatment(stoi(raw[i + j][17]));
                   p->setNoVisits(stoi(raw[i + j][18]));
                   p->setTimestepInactive(stoi(raw[i + j][20]));
                   p->setCreateTime(stoi(raw[i + j][21]));
                   p->setResultsReturnedTime(stoi(raw[i + j][22]));
                   p->setFacilityReturnTime(stoi(raw[i + j][23]));
                   p->setNoReturnsForResults(stoi(raw[i + j][24]));
                   p->setinfectiousness(stof(raw[i + j][25]));
                   p->setImmunity(stof(raw[i + j][26]));
                   p->setSampleTest(raw[i + j][27]);

                   if (debug) std::cout << " inside readSnapshot inside patients  after sets" << '\n';

                   diagnosis = newFacility->getDiagnosisPtr();
                   diagnosis->storePatient(p);

                   if (debug) std::cout << " inside readSnapshot inside patients  after store patient" << '\n';

                   // don't forget to update Sample with Patient pointer. 
                   // go through sample list and see if patient id matches for any of them
                   // if so, update patient pointer in sample
                   for (auto a : sampleArr) {

                       if (raw[i + j][1] == a->patientID) {

                           a->sample->setPatientPtr(p);

                          if (debug) std::cout << " updated patient pointer in sample " <<  '\n';
                       }
                   }

                   j++;
               }

               // handle inactive patients
               while (raw[i + j][0] == "IPatient") {

                   // create an array of patients 
                   //Object	id	 facility	 disease	 distance	 infected(5)	 drug resistant	 track for reg	 status	 test pos	 md pos(10)	 live	 age	 gender	 life expect	 time from onset(15)	 ts in treat	 ts not in treat	 no of clinical visits	 ts patient created	 ts patient no longer tracked(20)	 create sample ts	 results returned ts	 facility return time	 no returns for results	 infectiousness(25)	 immunity	 sample test

                   Patient* p = new Patient(raw[i + j][2],  "", 0, "", 0, raw[i + j][3], stof(raw[i + j][4]), 0, stoi(raw[i + j][5]), stoi(raw[i + j][8]), raw[i + j][27], stoi(raw[i + j][19]));

                   // now update all the attributes of the patient
                   p->setDrugResistant(stoi(raw[i + j][6]));
                   p->setTrackForRegistry(stoi(raw[i + j][7]));
                   p->setTestPos(stoi(raw[i + j][9]));
                   p->setMDPos(stoi(raw[i + j][10]));
                   p->setLive(stoi(raw[i + j][11]));
                   p->setAge(stoi(raw[i + j][12]));
                   p->setGender(stoi(raw[i + j][13]));
                   p->setLifeExpectancy(stoi(raw[i + j][14]));
                   p->setTimeFromOnset(stoi(raw[i + j][15]));
                   p->setTimeStepsInTreatment(stoi(raw[i + j][16]));
                   p->setTimeStepsNotInTreatment(stoi(raw[i + j][17]));
                   p->setNoVisits(stoi(raw[i + j][18]));
                   p->setTimestepInactive(stoi(raw[i + j][20]));
                   p->setCreateTime(stoi(raw[i + j][21]));
                   p->setResultsReturnedTime(stoi(raw[i + j][22]));
                   p->setFacilityReturnTime(stoi(raw[i + j][23]));
                   p->setNoReturnsForResults(stoi(raw[i + j][24]));
                   p->setinfectiousness(stof(raw[i + j][25]));
                   p->setImmunity(stof(raw[i + j][26]));
                   p->setSampleTest(raw[i + j][27]);

                   if (debug) std::cout << " inside readSnapshot inside patients  after sets" << '\n';

                   diagnosis = newFacility->getDiagnosisPtr();
                   diagnosis->storeInactivePatient(p);

                   j++;
               }
           }

           // this will eat up the FacilityEnd as well as advancing to next Facility
           i = i + j;
           
           if (debug) {
               std::cout << " at end of make facility =   " << facility << '\n';
               std::cout << " inside facility after Facility check j =  " << j << '\n';
           }
       }
   }


}



// store away facilities, patients, samples, tests and diseases in a comma seperated file
void saveSnapshot(const std::string& filename, FacilitiesList* list, vector<string> diseaseList, vector<Disease*> diseases, vector<Test*> tests) {


    bool debug = true;
    Diagnosis* d;
    Testing* t;
    vector<Patient*> p;
    vector<Patient*> pInactive;
    vector<Sample*> s;
    vector<Sample*> sInactive;
    vector<Test*> testsList;
    vector<Facility*> latF;
    vector<facilityAttributes*> facAttr;
    ofstream myfile;

    vector<Facility*> facilities = list->getListPtr();


    myfile.open(filename);

    if (debug) {
        std::cout << " inside saveSnapshot   " <<  '\n';
    }
   

     // store time related data and random no. related data
    myfile << "Object" << ",timeStep" << ",Seed" << ",maxNoSeq100" << ",maxNoSeqPop" << ",countSeq100" << ",countSeqPopTB" << ",countSeqPopHIV";
    myfile << ",countSeqPopHCV" << ",countSeqPopYF" << ",countSeqPopMen" << ",countSeqPopMea" << ",nnextNoSeq100";
    myfile << ",nextNoSeqPopTB" << ",nextNoSeqPopHIV" << ",nextNoSeqPopHCV" << ",nextNoSeqPopYF" << ",nextNoSeqPopMen" << ",nextNoSeqPopMea";
    myfile << ",popTB" << ",popHIV" << ",popHCV" << ",popYF" << ",popMen" << ",popMea";
    myfile << "\n";

    myfile << "Random" <<  "," << timeStep << "," << seed << "," << maxNoSeq100 << "," << maxNoSeqPop << "," << countSeq100 << "," << countSeqPopTB << "," << countSeqPopHIV;
    myfile << "," << countSeqPopHCV << "," << countSeqPopYF << "," << countSeqPopMen << "," << countSeqPopMea << "," << nextNoSeq100;
    myfile << "," << nextNoSeqPopTB << "," << nextNoSeqPopHIV << "," << nextNoSeqPopHCV << "," << nextNoSeqPopYF << "," << nextNoSeqPopMen << "," << nextNoSeqPopMea;
    myfile << "," << popTB << "," << popHIV << "," << popHCV << "," << popYF << "," << popMen << "," << popMea;
    myfile << "\n";

    

    // store awy disease list
    myfile << "Object" << ",count" << ",DiseaseInRun" << "\n";
    myfile << "DiseaseInRun" << "," << diseaseList.size();
    //std::cout << " snap disease list size =   " << diseaseList.size() << '\n';
    for (auto dl : diseaseList) {
    
        myfile << "," << dl;
        //std::cout << " disease list  =   " << dl << '\n';
    }
    myfile << "\n";

    // store away specific disease parameters
    myfile << "Object" << ",Disease" << ",mortality" << ",incidence" << ",incidence.adjustment.type" <<",incidence.adjustment.factor";
    myfile << ",patients.ratio.uninf.inf" << ",per.patient.present.inf";
    myfile << ",transmission" << ",priority" << ",morbidity" << ",preTreatLtoF" <<  ",preTreatLtoFReduction" << ",space1" << ",space2";
    myfile <<",probPatientReturns" << ",perLostSamples" << ",treatedPatientPercentage" <<  ",init treat time";
    myfile <<  ",init treat success" << ",int treat death" << ",init treat LtoF" <<",relapse time" << ",relapse success";
    myfile << ",prob.NI.treated" << ",per drug resist" << ",onset to presentation" << ",per untreat die" << ",time untreat die" << ",per untreat represent" << ",time untreat represent" << "\n";

    for (auto dS : diseases) {

        myfile << "Disease" << "," << dS->getName() << "," << dS->getMortality() << "," << dS->getIncidence() << "," << dS->getIncAdjType() << "," << dS->getIncAdjFactor();
        myfile << "," << dS->getPatientsRatioUninfInf() << "," << dS->getPerPatientPresentInf();
        myfile << "," << dS->getTransmission() << "," << dS->getPriority() << "," << dS->getMorbidity() << "," << dS->getPerPreTestLtoF() << "," << dS->getPerPreTestLtoFReduction();
        myfile << "," << dS->getSpace1() << "," << dS->getSpace2() << "," << dS->getProbReturn() << "," << dS->getPerLostSamples() << "," << dS->getTreatedPercentage();
        myfile << "," << dS->getTreatmentTime() << "," << dS->getInitialTreatmentSuccessRate() << "," << dS->getInitialTreatmentDeathRate() << "," << dS->getInitialTreatmentLossToFollow();
        myfile << "," << dS->getRelapseTime() << "," << dS->getRelapseSuccessRate() << "," << dS->getprobNonInfTreated() << "," << dS->getperDrugResistant() << "," << dS->getTimeOnsetToPresentation() << "," << dS->getPerUntreatedDie() << "," << dS->getUntreatedDeathTime();
        myfile << "," << dS->getPerUntreatedRepresentation() << "," << dS->getTimeUntreatedRepresentation() << "\n";
    }

    // store away specific test parameters
    myfile << "Object" << ",Test" << ",disease" << ",no machines" << ",tests per machine" << ",cycles per ts" << ",max tests per ts" << ",tests this ts" << ",max test per cycle" << ",tests this reagent cycle";
    myfile << ",reagent cycle" << ",total tests" << ",priority" << ",batch wait time" << ",patient return time" << ",sensitivity" << ",specficity" << ",sensitivityMD" << ",specificityMD" << ",sensitivityWithTestMD" << ",specificityWithTestMD" << ",tier0";
    myfile << ",tier1" << ", tier2" << ",tier3" << ",tier4" << ",tier5" << ",tier6" << ",tier7" << ",tier8" << ",tier9" << "\n";
    for (auto tS : tests) {

        myfile << "Test" << "," << tS->getTestName() << "," << tS->getDiseaseName() <<  "," << tS->getNoMachines() << "," << tS->getTestsPerMachine() << "," << tS->getCyclesPerTimestep() << "," << tS->getMaxTestPerTimestep();
        myfile << "," << tS->getTestsThisTimestep() << "," << tS->getMaxTestPerCycle() << "," << tS->getTestsThisReagentCycle() << "," << tS->getReagentCycle() << "," << tS->getTotalTests();
        myfile << "," << tS->getPriority() << "," << tS->getBatchWaitTime() << "," << tS->getPatientReturnTime() <<  "," << tS->getSense() << "," << tS->getSpecif() << "," << tS->getSenseMD() << "," << tS->getSpecifMD() <<  "," << tS->getSenseWithTestMD() << "," << tS->getSpecifWithTestMD();
        myfile << "," << tS->getTier0() << "," << tS->getTier1() << "," << tS->getTier2() << "," << tS->getTier3() << "," << tS->getTier4();
        myfile << "," << tS->getTier5() << "," << tS->getTier6() << "," << tS->getTier7() << "," << tS->getTier8() << "," << tS->getTier9() << "\n";
    }



    // store away facilities
    // create header for file for readablility
    // resolve lateral and down connections and up connection pointers after inporting the snapshot file with updateFacilityConnectioins
    myfile << "Object" << ",name" << ",id " << ",type" << ",long" << ",lat" << ",town" << ",district" << ",region" << ",tier" << ",diagnosis" << ",Up Conn" << ", Lat Conn";
    myfile << ",Down Conn" << ",Pop" << ",BinInc" << ",popbin0" << ",popbin1" << ",popbin2" << ",popbin3" << ",popbin4" << ",popbin5" << ",popbin6";
    myfile << ",popbin7" << ",popbin8" << ",popbin9" << ",Xpert";;
    myfile <<  ",disease" << ",test" << ",testing" << ",incidence0" << ",incidence1" << ",incidence2" << ",incidence3" << ",incidence4" << ",incidence5" << ",incidence6" << ",incidence7" << ",incidence8";
    myfile << ",incidence9" << ",return time for results" << ",no. of machines" << ",max reagents per cycle" << ",time steps in cycle" << ",sample batch wait time" << ",closet-Connection-With-Testing" << ",closest-Connection-Distance" << "\n";


     // loop through each facility and extract information about each facility
    for (auto facility : facilities) {

        // get the up name for this facility
        d = facility->getDiagnosisPtr();
        t = facility->getTestingPtr();
        facAttr = facility->getFacilityAttributesPtr();
        p = d->getPatientPtr();
        pInactive = d->getInactivePatientPtr();
        s = t->getSamplePtr();
        sInactive = t->getInactiveSamplePtr();
        testsList = t->getTeststPtr();

        myfile << "Facility" << "," << facility->getName() << "," << facility->getID() << "," << facility->getType() << "," << facility->getLongitude() << "," << facility->getLatitude() << "," << facility->getTown();
        myfile << "," << facility->getDistrict() << "," << facility->getRegion() << "," << facility->getTier() << "," << facility->diagnosisAvailable();
        myfile << "," << facility->getUpName() << "," << "need lat name" << "," << "need down name";
        myfile << "," << facility->getPopulation() << "," << facility->getBinInc() << "," << facility->getPopBin0() << "," << facility->getPopBin1() << "," << facility->getPopBin2();
        myfile << "," << facility->getPopBin3() << "," << facility->getPopBin4() << "," << facility->getPopBin5() << "," << facility->getPopBin6() << "," << facility->getPopBin7();
        myfile << "," << facility->getPopBin8() << "," << facility->getPopBin9() << "," << facility->getXpert() << "\n";

        for (auto a : facAttr) {
            myfile <<  "FacAttr,,,,,,,,,,,,,,,,,,,,,,,,,,," << a->disease <<  "," << a->testName <<  "," << a->testing <<"," << a->incidence << "," << a->incAdjType << "," << a->incAdjFactor;
            myfile << "," << a->noMachines;
            myfile << "," << a->maxReagentPerCycle<< "," << a->timeStepsInCycle <<   "," << a->batchWaitTime << "," << a->closetConnectionWithTesting <<  "," << a->closestConnectionDistance << "\n";
        }

        // store Samples associated with facility
        // patient is the address of the pointer of the patient agent.  This connects the sample to the patient
        myfile << "Object" << ",patient" << ",status" << ",age" << ",wait" << ",transit time" << ",transit time save" << ",transit time return" << ",transit time return save" << ",create time";
        myfile << ",results returned ts" << ",infected" << ",positive" << ",disease" << ",test" << ",orig facilty" << ",testing facility" << ",testing facility dist" << ",priority";
        myfile << ",batch wait time" << ",per Lost Samples" << "\n";

        for (auto samp : s) {

            myfile << "Sample" << "," << samp->getPatientPtr() << "," << samp->getStatus() << "," << samp->getAge() << "," << samp->getWait() << "," << samp->getTransitTime() << "," << samp->getTransitTimeSave();
            myfile << "," << samp->getReturnTransitTime() << "," << samp->getReturnTransitTimeSave() << "," << samp->getCreateTime() << "," << samp->getResultsReturnedTime() << "," << samp->getInfected();
            myfile << "," << samp->getPositive() << "," << samp->getDisease() << "," << samp->getTest() << "," << samp->getOrigFacility() << "," << samp->getTestingFacility();
            myfile << "," << samp->getTestingDistance() << "," << samp->getPriority() << "," << samp->getBatchWaitTime() << "," << samp->getPerLostSamples() << "\n";
        }

        // handle inactive samples
        for (auto samp : sInactive) {

            myfile << "ISample" << "," << samp->getPatientPtr() << "," << samp->getStatus() << "," << samp->getAge() << "," << samp->getWait() << "," << samp->getTransitTime() << "," << samp->getTransitTimeSave();
            myfile << "," << samp->getReturnTransitTime() << "," << samp->getReturnTransitTimeSave() << "," << samp->getCreateTime() << "," << samp->getResultsReturnedTime() << "," << samp->getInfected();
            myfile << "," << samp->getPositive() << "," << samp->getDisease() << "," << samp->getTest() << "," << samp->getOrigFacility() << "," << samp->getTestingFacility();
            myfile << "," << samp->getTestingDistance() << "," << samp->getPriority() << "," << samp->getBatchWaitTime() << "\n";
        }

        // store Patients associated with facility
        // store address of pointer to make an id that can be connected to samples for this facility
        myfile << "Object" << ",id" << ",facility" << ",disease" << ",distance" << ",infected" << ",drug resistant" << ",track for reg" << ",status" << ",test pos" << ",md pos" << ",live";
        myfile << ",age" << ",gender" << ",life expect" << ",time from onset" << ",ts in treat" << ",ts not in treat" << ",no of clinical visits" << ",ts patient created";
        myfile << ",ts patient no longer tracked" << ",create sample ts" << ",results returned ts" << ",facility return time" << ",no returns for results" << ",infectiousness" << ",immunity" << ",sample test" << "\n";

        //std::cout << "Object" << ",facility" << ", disease" << ", distance" << ", infected" << ", drug resistant" << ", track for reg" << ", status" << ", test pos" << ", md pos" << ", live";
        //std::cout << ", age" << ", gender" << ", life expect" << ", time from onset" << ", ts in treat" << ", ts not in treat" << ", no of clinical visits" << ", ts patient created";
        //std::cout << ", ts patient no longer tracked" << ", create sample ts" << ", results returned ts" << ", facility return time" << ", no returns for results" << ", infectiousness" << ", immunity" << ", sample test" << "\n";
        
        for (auto pat : p) {

            myfile << "Patient" << "," << pat << "," << pat->getFacility() << "," << pat->getDisease() << "," << pat->getDistance() << "," << pat->getInfected() << "," << pat->isDrugResistant() << "," << pat->trackForRegistry();
            myfile << "," << pat->getStatus() << "," << pat->getTestPos() << "," << pat->getMDPos() << "," << pat->isAlive() << "," << pat->getAge() << "," << pat->getGender();
            myfile << "," << pat->getLifeExpectancy() << "," << pat->getTimeFromOnset() << "," << pat->getTimeStepsInTreatment() << "," << pat->getTimeStepsNotInTreatment() << "," << pat->getNoVisits();
            myfile << "," << pat->getTimestepCreated() << "," << pat->getTimestepInactive() << "," << pat->getTimestepPresented() << "," << pat->getResultsReturnedTime() << "," << pat->getFacilityReturnTime();
            myfile << "," << pat->getNoReturnsForResults() << "," << pat->getNoReturnsForResults() << "," << pat->getNoReturnsForResults() << "," << pat->getSampleTest() << "\n";
        
        }

        // handle inactive patients 
        for (auto pat : pInactive) {

            myfile << "IPatient" << "," << pat << "," << pat->getFacility() << "," << pat->getDisease() << "," << pat->getDistance() << "," << pat->getInfected() << "," << pat->isDrugResistant() << "," << pat->trackForRegistry();
            myfile << "," << pat->getStatus() << "," << pat->getTestPos() << "," << pat->getMDPos() << "," << pat->isAlive() << "," << pat->getAge() << "," << pat->getGender();
            myfile << "," << pat->getLifeExpectancy() << "," << pat->getTimeFromOnset() << "," << pat->getTimeStepsInTreatment() << "," << pat->getTimeStepsNotInTreatment() << "," << pat->getNoVisits();
            myfile << "," << pat->getTimestepCreated() << "," << pat->getTimestepInactive() << "," << pat->getTimestepPresented() << "," << pat->getResultsReturnedTime() << "," << pat->getFacilityReturnTime();
            myfile << "," << pat->getNoReturnsForResults() << "," << pat->getNoReturnsForResults() << "," << pat->getNoReturnsForResults() << "," << pat->getSampleTest() << "\n";

        }


        // mark end of this facility output
        myfile << "FacilityEnd" << "\n";
    }

    myfile.close();

}



// to create combined list
void makeNewList(vector<vector<string>> maina, vector<vector<string>> ernest) {

    ofstream myfile;

    std::size_t i,j,k, combinedCount;
    int commonCount = 0;
    std::size_t mainaSize = maina.size();
    std::size_t ernestSize = ernest.size();
    float newDist;
    bool notFound;

    struct mainaStruct {
        string region;
        string facility;
        string type;
        string source;
        float longitude;
        float latitude;
        string geoSource;
        float closestDist;
        string closestFacility;
    };

    struct ernestStruct {
        string region;
        string otherRegion;
        string facility;
        string comment;
        string type;
        int typeID;
        string source;
        string geoSource;
        string otherGeoSource;
        float longitude;
        float latitude;
        float closestDist;
        string closestFacility;
    };


    struct joeStruct {
        int id;
        string facility;
        string type;
        string ownership;
        string district;
        string region;
        string dataSource;
        string geoSource;
        float longitude;
        float latitude;
        string geoPoint;
        string eName;
        string mName;

    };



    struct mainaStruct mainaArray[mainaSize];
    struct ernestStruct ernestArray[ernestSize];
    struct joeStruct combined[ernestSize + mainaSize];


    std::cout << "inside makeNewList mainaSize = " << mainaSize << '\n';

    // maina has format country, region, name, type, source, longitude, lattitude, gps-source
    for (i = 0; i < mainaSize; i++) {

        if (false) {
            std::cout << maina[i][0] << ", " << maina[i][1] << ", " << maina[i][2] << ", " << maina[i][3] << ", " << maina[i][4] << ", " << maina[i][6] << ", " << maina[i][5] << '\n';
        }

        // convert mortality and incidence to floats from strings
        mainaArray[i].region = maina[i][1];
        mainaArray[i].facility = maina[i][2];
        mainaArray[i].type = maina[i][3];
        mainaArray[i].source = maina[i][4];
        mainaArray[i].longitude = stof(maina[i][6]);
        mainaArray[i].latitude = stof(maina[i][5]);
        mainaArray[i].geoSource = maina[i][7];
        mainaArray[i].closestDist = 20;
        mainaArray[i].closestFacility = "";
    }
    // loop through array and find next closest facility
    if (false) {
        for (i = 0; i < mainaSize; i++) {

            for (j = 0; j < mainaSize; j++) {

                if (mainaArray[i].facility != mainaArray[j].facility) {

                    newDist = sqrt(pow(mainaArray[i].longitude - mainaArray[j].longitude, 2) + (pow(mainaArray[i].latitude - mainaArray[j].latitude, 2)));

                    if (newDist < mainaArray[i].closestDist) {

                        mainaArray[i].closestDist = newDist;
                        mainaArray[i].closestFacility = mainaArray[j].facility;
                    }
                }
            }

        }
        for (i = 0; i < mainaSize; i++) {
            if (mainaArray[i].closestDist < .0001) {
                std::cout << "facility close to name = " << mainaArray[i].facility << ", dist = " << mainaArray[i].closestDist << ", other fac = " << mainaArray[i].closestFacility << '\n';
            }
        }
    }



    std::cout << "inside makeNewList ernestSize = " << ernestSize << '\n';

    // ernest has format unitld, unit type, Unit type name, lvlld, region, region name, code, name, comment, code_1, Name_1, near_fid, near_dis, geometry c(long,lat), longitude, latitude
    for (i = 1; i < ernestSize; i++) {

        if (false) {
            std::cout << ernest[i][5] << ", " << ernest[i][7] << ", " << ernest[i][8] << ", " << ernest[i][2] << ", " << ernest[i][1] << ", " << ernest[i][16] << ", " << ernest[i][15] << '\n';
        }

        // convert mortality and incidence to floats from strings
        ernestArray[i-1].region = ernest[i][5];
        ernestArray[i-1].otherRegion = "";
        ernestArray[i-1].facility = ernest[i][7];
        ernestArray[i-1].comment = ernest[i][8];
        ernestArray[i-1].type = ernest[i][2];
        ernestArray[i-1].geoSource = "[Undetermined]";
        ernestArray[i-1].otherGeoSource = "[Undetermined]";
        ernestArray[i-1].typeID = stoi(ernest[i][1]);
        ernestArray[i-1].longitude = stof(ernest[i][16]);
        ernestArray[i-1].latitude = stof(ernest[i][15]);
        ernestArray[i-1].closestDist = 20;
        ernestArray[i-1].closestFacility = "";
    }

    // adjust size to remove header row
    ernestSize = ernestSize - 1;

    if (false) {
        // see if duplicates within ernest list
        for (i = 0; i < ernestSize; i++) {

            for (j = 0; j < ernestSize; j++) {

                if (ernestArray[i].facility != ernestArray[j].facility) {

                    newDist = sqrt(pow(ernestArray[i].longitude - ernestArray[j].longitude, 2) + (pow(ernestArray[i].latitude - ernestArray[j].latitude, 2)));
                    //std::cout << "newDist " << newDist << '\n';

                    if (newDist < ernestArray[i].closestDist) {

                        ernestArray[i].closestDist = newDist;
                        ernestArray[i].closestFacility = ernestArray[j].facility;
                    }
                }
            }

        }
        for (i = 0; i < ernestSize; i++) {
            if (ernestArray[i].closestDist == 0) {
                std::cout << "facility " << ernestArray[i].facility << " dist = " << ernestArray[i].closestDist << " " << ernestArray[i].closestFacility << '\n';
            }
        }
    }
   


    if (true) {
        for (i = 0; i < ernestSize; i++) {

            // see if duplicates between lists

            for (j = 0; j < mainaSize; j++) {

                    newDist = sqrt(pow(ernestArray[i].longitude - mainaArray[j].longitude, 2) + (pow(ernestArray[i].latitude - mainaArray[j].latitude, 2)));
                    //std::cout << "newDist " << newDist << '\n';

                    if (newDist < ernestArray[i].closestDist) {

                        ernestArray[i].closestDist = newDist;
                        ernestArray[i].closestFacility = mainaArray[j].facility;
                        ernestArray[i].otherGeoSource = mainaArray[j].geoSource;
                    }
            }

        }
        //for (i = 0; i < ernestSize; i++) {
        //    if (ernestArray[i].closestDist < .001) {
        //        std::cout << "facility " << ernestArray[i].facility << " dist = " << ernestArray[i].closestDist << " " << ernestArray[i].closestFacility << '\n';
        //    }
        //}
    }

    // start creating combined structure
    // first add in ernest stuff
    for (k = 0; k < ernestSize; k++) {

        // fill in structure - modifying it for maina being the same
        combined[k].id = k+1;
        combined[k].facility = ernestArray[k].facility;
        combined[k].type = ernestArray[k].type;
        combined[k].ownership = "[Undetermined]";
        combined[k].district = "";
        combined[k].region = ernestArray[k].region;
        combined[k].dataSource = "ErnestFacilityList";
        combined[k].geoSource = ernestArray[k].geoSource;
        combined[k].longitude = ernestArray[k].longitude;
        combined[k].latitude = ernestArray[k].latitude;
        combined[k].geoPoint = "POINT (" + to_string(ernestArray[k].longitude) + " " + to_string(ernestArray[k].latitude) + ")";
        combined[k].eName = ernestArray[k].facility;
        combined[k].mName = "";

        if (ernestArray[k].closestDist < .001) {

            combined[k].mName = ernestArray[k].closestFacility;
            combined[k].geoSource = ernestArray[k].otherGeoSource;
            commonCount++;
        }
    }

   // first add in non-common maina entries
    combinedCount = ernestSize;

    for (k = 0; k < mainaSize; k++) {

        notFound = true;
        
        for (i = 0; i < ernestSize; i++) {
            
            if (mainaArray[k].facility == ernestArray[i].closestFacility) {
                if (ernestArray[i].closestDist < .001) {
                    notFound = false;
                }
            }

        }

        if (notFound) {
            combined[combinedCount].id = combinedCount + 1;
            combined[combinedCount].facility = mainaArray[k].facility;
            combined[combinedCount].type = mainaArray[k].type;
            combined[combinedCount].ownership = mainaArray[k].source;
            combined[combinedCount].district = "";
            combined[combinedCount].region = mainaArray[k].region;
            combined[combinedCount].dataSource = "Maina";
            combined[combinedCount].geoSource = mainaArray[k].geoSource;
            combined[combinedCount].longitude = mainaArray[k].longitude;
            combined[combinedCount].latitude = mainaArray[k].latitude;
            combined[combinedCount].geoPoint = "POINT (" + to_string(mainaArray[k].longitude) + " " + to_string(mainaArray[k].latitude) + ")";
            combined[combinedCount].mName = mainaArray[k].facility;
            combined[combinedCount].eName = "";

            combinedCount++;
        }

    }

    myfile.open("./files/newCombined.csv");

    myfile << "id " << ",name " << ", type " << ",ownership" << ",district.name" << ",region.name" << ",dataSource" << ", geo.source" << ", geo.long" << ", geo.lat" << ",geo.all.wkt" << ", Ernest.Name" << ", Maina.Name" << '\n';

    for (i = 0; i < combinedCount; i++) {

        myfile << combined[i].id << "," << combined[i].facility << "," << combined[i].type << "," << combined[i].ownership << "," << combined[i].district << "," << combined[i].region;
        myfile << "," << combined[i].dataSource << "," << combined[i].geoSource << "," << combined[i].longitude << "," << combined[i].latitude << "," << combined[i].geoPoint << "," << combined[i].eName;
        myfile << "," << combined[i].mName << '\n';
    }

    myfile.close();
}


// to create combined list
void idMatching(vector<vector<string>> id) {

    ofstream myfile;

    std::size_t i,j,idCount;
    std::size_t idSize = id.size();
    float dist;

    struct idStruct {
        float longitude;
        float latitude;
        string region;
        string newRegion;
        string district;
        string newDistrict;
        int oldId;
        int newId;
    };

    struct facStruct {
        int id;
        string district;
        string region;;
        float longitude;
        float latitude;
    };

    struct idStruct idArray[idSize];
    struct facStruct facArray[idSize];

    std::cout << "inside idMatch idSize = " << idSize << '\n';

    idCount = 0;

    // id has format longitude, latitude, region, district, id, space, facilityId, facilityDistrict, facilityRegion, facilityLongitude, facilityLatitude
    for (i = 1; i < idSize; i++) {

        if (false) {
            std::cout << id[i][0] << ", " << id[i][1] << ", " << id[i][2] << ", " << id[i][3] << ", " << id[i][4] << ", " << id[i][6] << ", " << id[i][5] << '\n';
        }


        // convert mortality and incidence to floats from strings
        if (id[i][0] != "") {
            idArray[i - 1].longitude = stof(id[i][0]);
            idArray[i - 1].latitude = stof(id[i][1]);
            idArray[i - 1].region = id[i][2];
            idArray[i - 1].newRegion = "";
            idArray[i - 1].district = id[i][3];
            idArray[i - 1].newDistrict = "";
            idArray[i - 1].oldId = stoi(id[i][4]);
            idArray[i - 1].newId = 0;
            idCount++;
        }
        facArray[i - 1].id = stoi(id[i][6]);
        facArray[i - 1].district = id[i][7];
        facArray[i - 1].region = id[i][8];
        facArray[i - 1].longitude = stof(id[i][9]);
        facArray[i - 1].latitude = stof(id[i][10]);
    }

    idSize = idSize - 1;


    // now compare two arrays
    for (i = 0; i < idCount; i++) {

        for (j = 0; j < idSize; j++) {

            dist = sqrt(pow(idArray[i].longitude - facArray[j].longitude, 2) + (pow(idArray[i].latitude - facArray[j].latitude, 2)));
            if (dist < .001) {

                idArray[i].newId = facArray[j].id;
                idArray[i].newRegion = facArray[j].region;
                idArray[i].newDistrict = facArray[j].district;
            }
        }
    }

    if (false) {
        std::cout << "longitude" << ", Latitude " << ",region" << ",newRegion " << ",distict" << ",newDistrict " << ",oldId " << ",newId " << '\n';

        for (i = 0; i < idCount; i++) {
            std::cout << idArray[i].longitude << "," << idArray[i].latitude << ", " << idArray[i].region << ", " << idArray[i].newRegion << ", " << idArray[i].district;
            std::cout << ", " << idArray[i].newDistrict << ", " << idArray[i].oldId << ", " << idArray[i].newId << '\n';

            if (idArray[i].region != idArray[i].newRegion) {
                std::cout << "regions don't match" << '\n';
            }
            if (idArray[i].district != idArray[i].newDistrict) {
                std::cout << "districts don't match" << '\n';
            }
        }
    }


    myfile.open("./input/facilities/idMatchFinal.csv");

    myfile << "longitude" << ", Latitude " << ",region" << ",newRegion " << ",distict" << ",newDistrict " << ",oldId " << ",newId " << '\n';

    for (i = 0; i < idCount; i++) {

        myfile << idArray[i].longitude << "," << idArray[i].latitude << ", " << idArray[i].region << ", " << idArray[i].newRegion << ", " << idArray[i].district;
        myfile << ", " << idArray[i].newDistrict << ", " << idArray[i].oldId << ", " << idArray[i].newId << '\n';
    }

    myfile.close();

}




// add travel times from accessmod to population file
void updateTravelTime(vector<vector<string>> pop, vector<vector<string>> travel) {

    ofstream myfile;
    
    int startingIndex = 1;
    std::size_t i,j;
    std::size_t popSize = pop.size();
    std::size_t travelSize = travel.size();
    std::size_t travelSizeLeft = travel.size();
    std::size_t travelSizeIndex = 30000;
    float oldDist;
    float dist;

    std::cout << "inside update travel time popSize = " << popSize << ", travelSize = " << travelSize << '\n';

    float popArray[popSize][4];

    std::cout << "inside update travel time1" << '\n';

    float travelArray[travelSizeIndex][3];

    std::cout << "inside update travel time2" << '\n';

    // load the two arrays - skip header row
    for (i = 1; i < popSize; i++) {

        if (false) {
            std::cout << pop[i][0] << ", " << pop[i][1] << ", " << pop[i][2] << ", " << pop[i][3] <<  '\n';
        }
        popArray[i - 1][0] = stof(pop[i][1]);
        popArray[i - 1][1] = stof(pop[i][2]);
        if (pop[i][3] == "NA") {
            popArray[i - 1][2] = 0;
        }
        else {
            popArray[i - 1][2] = stof(pop[i][3]);
        }
        popArray[i - 1][3] = 0;
    }

    // update array sizes to reflect removed header
    popSize = popSize - 1;

    std::cout << "after pop structures" << '\n';

    // Can't load both pop and travel arrays into memory
    // loop through a section at a time
    while (travelSizeLeft > 1) {

        for (i = 0; i < travelSizeIndex; i++) {

            if (false) {
                std::cout << travel[startingIndex + i][0] << ", " << travel[startingIndex + i][1] << ", " << travel[startingIndex + i][2] << '\n';
            }

            travelArray[i][0] = stof(travel[startingIndex + i][0]);
            travelArray[i][1] = stof(travel[startingIndex + i][1]);
            travelArray[i][2] = stof(travel[startingIndex + i][2]);
        }

        std::cout << "after update travel structures" << '\n';


        // find closest point in travel time file that is withing 500 m of population cluster point
        for (i = 1; i < popSize; i++) {

            oldDist = 1;

            // check each travel time point against a population cluster point, must be within .5 km and closest one
            for (j = 0; j < travelSizeIndex; j++) {

                // dist = sqrt(pow((popArray[i].longitude - travelArray[j].longitude), 2) - pow((popArray[i].latitude - travelArray[j].latitude), 2)); /
                dist = sqrt(pow((popArray[i][0] - travelArray[j][0]), 2) + pow((popArray[i][1] - travelArray[j][1]), 2));

                // less than .5 km away
                if (dist < .005) {

                    // closer than any other
                    if (dist < oldDist) {

                        oldDist = dist;
                        popArray[i][3] = travelArray[j][2];
                    }
                }

            }
        }
        travelSizeLeft = travelSizeLeft - travelSizeIndex;
        startingIndex = startingIndex + travelSizeIndex;

        if (travelSizeLeft < travelSizeIndex) {
            travelSizeIndex = travelSizeLeft - 1;
        }
        std::cout << "travelSizeLeft = " << travelSizeLeft << ", starting index =" << startingIndex << ", travelSizeIndex = " << travelSizeIndex << '\n';
    }
    std::cout << "Before file output " << '\n';

    myfile.open("./input/population/newpop.csv");

    myfile << "longitude" << ", Latitude " << ",population" << ",travelTime" << '\n';

    for (i = 0; i < popSize; i++) {

        myfile << popArray[i][0] << "," << popArray[i][1] << ", " << popArray[i][2] << ", " << popArray[i][3] <<  '\n';
    }

    myfile.close();
}




// add travel times from accessmod to population file
void updateTravelCatch( vector<vector<string>> travel, vector<vector<string>> facilities) {

    // travel is the output of accessmod travel image file

    ofstream myfile;
    ofstream myfile1;
    ofstream myfile2;

    std::size_t i, j;
    std::size_t travelSize = travel.size();
    std::size_t facSize = facilities.size();
    float dist, calcDist;
    float pt1Long, pt1Lat;
    float pt2Long = 0, pt2Lat = 0;
    float calcDistMin;
    float gridInc;
    string tempString;
    bool foundPoint = false;


    struct facData {
        string facName;
        float longitude;
        float latitude;
        int id;
        string xpert;
        int travelIndex;
        float travelLong;
        float travelLat;
    };

    std::cout << "inside update travel catch facSize = " << facSize << ", travelSize = " << travelSize << '\n';


    travelData* travelList = new travelData[travelSize];

    std::cout << "inside update travel time" << '\n';

    for (i = 1; i < travelSize; i++) {

        if (false) {
            std::cout << travel[i][0] << ", " << travel[i][1] << ", " << travel[i][2] << '\n';
        }

        travelList[i-1].longitude = stof(travel[i][0]);
        travelList[i-1].latitude = stof(travel[i][1]);
        travelList[i-1].time = stoi(travel[i][2]);
        travelList[i - 1].facility = "";
        travelList[i-1].id = 0;
    }

    // update array sizes to reflect removed header
    travelSize = travelSize - 1;

    std::cout << "after travel structure" << '\n';

    std::cout << "i" << ",longitude" << ",latitude" << ",time" << ",facility" << ",id" << '\n';
    for (i = 0; i < 4; i++) {

        std::cout << i << "," << travelList[i].longitude << ", " << travelList[i].latitude << ", " << travelList[i].time;
        std::cout << ", " << travelList[i].facility << ", " << travelList[i].id << '\n';
    }



    facData* facList = new facData[facSize];

    std::cout << "inside update facilites - no. of facilities = " << facSize << '\n';

    // facilities has the format 
    // 0 = region hospital, 
    // 1 = facility name, 
    // 2 = distance to regional hospital
    // 3 = tier (type - hospital, teaching hospital, etc.)
    // 4 = longitude
    // 5 = latitude
    // 6 = region
    // 7 = district
    // 8 = Xpert
    // 9 = unitID

    std::cout << "i" << ",longitude" << ",latitude" << ",time" << ",xpert" << ",id" << '\n';
    for (i = 0; i < 4; i++) {

        std::cout << i << "," << facilities[i][1] << ", " << facilities[i][4] << ", " << facilities[i][5];
        std::cout << ", " << facilities[i][8] << ", " << facilities[i][9] << '\n';
    }

     // convert from string to float
    for (i = 1; i < facSize; i++) {
        facList[i-1].facName = facilities[i][1];
        facList[i-1].longitude = stof(facilities[i][4]);
        facList[i-1].latitude = stof(facilities[i][5]);
        facList[i-1].xpert = facilities[i][8];
        tempString = facilities[i][9];

        // don't include end of line characters
        tempString.erase(std::remove(tempString.begin(), tempString.end(), '\r'), tempString.end());
        tempString.erase(std::remove(tempString.begin(), tempString.end(), '\n'), tempString.end());

        facList[i-1].id = stoi(tempString);
        facList[i - 1].travelLong = 0;
        facList[i - 1].travelLat = 0;
    }

    // update array sizes to reflect removed header
    facSize = facSize - 1;

    std::cout << "after facilities structure" << '\n';

    std::cout << "i" << ",name" << ",longitude" << ",latitude" << ",time" << ",xpert" << ",id" << '\n';
    for (i = 0; i < 4; i++) {

        std::cout << i << ", " << facList[i].facName << ", " << facList[i].longitude << ", " << facList[i].latitude;
        std::cout << ", " << facList[i].xpert << ", " << facList[i].id << '\n';
    }

    // find grid increment
    // take first point and find closest point, then find x or y increment
    pt1Long = travelList[0].longitude;
    pt1Lat = travelList[0].latitude;
    dist = 1;

    // loop through and find closest point - should be 0 in x or y
    for (i = 1; i < travelSize; i++) {
        calcDist = abs(pt1Long - travelList[i].longitude) + abs(pt1Lat - travelList[i].latitude);

        if (calcDist < dist) {
            dist = calcDist;
            pt2Long = travelList[i].longitude;
            pt2Lat = travelList[i].latitude;
        }
        
    }

    gridInc = dist;

    std::cout << "after inc test gridInc =  " << gridInc << ", pt1 = " << pt1Long << ", " << pt1Lat << ", pt2 = " << pt2Long << ", " << pt2Lat << '\n';

    // find the closest travel time grid point to facility and store it away
    for (j = 0; j < facSize; j++) {

        pt1Long = facList[j].longitude;
        pt1Lat = facList[j].latitude;
        facList[j].travelIndex = 0;
        //dist = 1;
        dist = 1.5;
        foundPoint = false;
        calcDistMin = 100;

        for (i = 0; i < travelSize; i++) {
            calcDist = sqrt(pow((pt1Long - travelList[i].longitude), 2) + pow((pt1Lat - travelList[i].latitude), 2));

            if (calcDist < dist) {
                dist = calcDist;
                facList[j].travelLong = travelList[i].longitude;
                facList[j].travelLat = travelList[i].latitude;
                facList[j].travelIndex = i;
                foundPoint = true;
            }

            if (calcDistMin > calcDist) calcDistMin = calcDist;
        }

        if (!foundPoint) {

            std::cout << "couldn't find facility travel point closest for " << facList[j].facName << " calc dist min = " << calcDistMin << '\n';
        }
    }
    std::cout << "after finding closest grid point " << '\n';

    std::cout << "i" << ",name" << ",longitude" << ",latitude" << ",time" << ",facility" << ",id" << ",travelLong" << ",travelLat" << ",index" << '\n';
    for (i = 0; i < 5; i++) {

        std::cout << i << ", " << facList[i].facName << ", " << facList[i].longitude << ", " << facList[i].latitude;
        std::cout << ", " << facList[i].xpert << ", " << facList[i].id << ", " << facList[i].travelLong << ", " << facList[i].travelLat << ", " << facList[i].travelIndex << '\n';
    }

    // now check to see if any facilities have the same travel points if so, adjust slightly
    //for (i = 0; i < facSize; i++) {

    //    for (j = 0; j < facSize; j++) {

    //        if ((i != j) && (facList[i].travelIndex == facList[j].travelIndex)) {
    //            
    //            // travel long and lat same for both points, adjust down 1 spot
    //            facList[j].travelLong = travelList[facList[j].travelIndex+1].longitude;
    //            facList[j].travelLat = travelList[facList[j].travelIndex+1].latitude;
    //            facList[j].travelIndex = facList[j].travelIndex+1;

    //            std::cout << "changed travel long/lat for facility " << facList[j].facName << " index = " << facList[j].travelIndex << '\n';
    //        }
    //    }
    //}
    
    // update the facility name and id for it's grid point
    for (j = 0; j < facSize; j++) {

        pt1Long = facList[j].travelLong;
        pt1Lat = facList[j].travelLat;
        foundPoint = false;
        calcDistMin = 100;

        for (i = 0; i < travelSize; i++) {
            calcDist = abs(pt1Long - travelList[i].longitude) + abs(pt1Lat - travelList[i].latitude);
   
            // same point update facility name and id
            if (calcDist < .0001) {
                travelList[i].facility = facList[j].facName;
                travelList[i].id = facList[j].id;

                // set time to zero because we are already there
                //std::cout << "travel time for facility =  " << facList[j].facName << ", time = " << travelList[i].time << '\n';
                travelList[i].time = 0;

                foundPoint = true;
            }

            if (calcDistMin > calcDist) calcDistMin = calcDist;
        }

        if (!foundPoint) {

            std::cout << "couldn't update travel list for facility " << facList[j].facName << " calc dist min = " << calcDistMin <<'\n';
        }
    }
    std::cout << "after updating facility grid points with names and id " << '\n';

    if (true) {

        myfile1.open("./defaults/input/population/facilitesModified.csv");

        myfile1 << "Facility" << ",Facility.ID" << ", Longitude" << ", Latitude " << ",travel.longitude" << ",travel.latitude" << '\n';

        for (i = 0; i < facSize; i++) {

            myfile1 << facList[i].facName << "," << facList[i].id << "," << facList[i].longitude << "," << facList[i].latitude <<  "," << facList[i].travelLong << "," << facList[i].travelLat << '\n';
        }

        myfile1.close();

        std::cout << "after facilitesModified.csv " << '\n';
    }

    // now walk through all the coordinates and if travel times are going up or the same, mark it as belonging to the facility
    // where you started. If the travel times are going down - stop going in that direction. If you run into another facility - stop
    // If a grid point is already marked, ignore it.
    // start going east to the next coordinate if it exists, then from that point go north, east, south
    // keep doing this recursively
    // now go west and do the same thing - this should cover all pixels 
    // this assumes pixel increment is the same in both x and y
    for (j = 0; j < facSize; j++) {

        // first go right
        walkGrid(facList[j].travelLong, facList[j].travelLat, 0, facList[j].facName, facList[j].id, gridInc, 1, travelList, travelSize);

        // now go left
        walkGrid(facList[j].travelLong, facList[j].travelLat, 0, facList[j].facName, facList[j].id, gridInc, -1, travelList, travelSize);

    }
    std::cout << "after walking grid - travelSize = " << travelSize << '\n';

    myfile2.open("./defaults/input/population/newTravelTimes-prefill-grid-cells.csv");

    myfile2 << "Longitude" << ", Latitude " << ",travelTime" << ",Facility" << ",Facility.ID" << '\n';

    for (i = 0; i < travelSize; i++) {

        myfile2 << travelList[i].longitude << "," << travelList[i].latitude << "," << travelList[i].time << "," << travelList[i].facility << "," << travelList[i].id << '\n';
    }

    myfile2.close();

    std::cout << "after output newTravelTimes-prefill file" << '\n';


    // the walk grid covers most cases - it fails where some condition causes the time to drop in adjacent grid points because of 
    // something like a road - check the grid points that don't have a facility and look at the cells surrounding the point
    // make the facility be the same as the adjacent cell - look left, right, up and down and diagonally
    for (i = 0; i < 10; i++) {
        fillGridCells(gridInc, travelList, travelSize);
        std::cout << "after filling grid cells i = " << i << '\n';
    }
    std::cout << "after filling grid cells " <<  '\n';


    myfile.open("./defaults/input/population/newTravelTimes.csv");

    myfile << "Longitude" << ", Latitude " << ",travelTime" << ",Facility" << ",Facility.ID" << '\n';

    for (i = 0; i < travelSize; i++) {

        myfile << travelList[i].longitude << "," << travelList[i].latitude << "," << travelList[i].time << "," << travelList[i].facility << "," << travelList[i].id << '\n';
    }

    myfile.close();

    std::cout << "after output newTravelTimes file - leaving updateTravelCatch" << '\n';
}





// add travel times from accessmod to population file
void updateTravelRegDistance(vector<vector<string>> facilities) {

    ofstream myfile;
    ofstream myfile1;

    std::size_t i, noRegions, k;
    std::size_t facSize = facilities.size();
    int unitID = 1;
    float dist;
    bool debug = false;


    struct facData {
        string facName;
        string region;
        string district;
        string crDistrict;
        float longitude;
        float latitude;
        int tier;
        string type;
        string crType;
        string xpert;
        int unitID;
        string upFacility;
        float upDistance;
    };

    struct regData {
        string name;
        float longitude;
        float latitude;
    };

    std::cout << "inside update travel reg hospital distance facSize = " << facSize << '\n';

    facData* facList = new facData[facSize];

    regData* regHospitals = new regData[20];

    std::cout << "inside update facilites" << '\n';

    // facilities has the format , 
    // 0 = facility name
    // 1 = region
    // 2 = district
    // 3 = case registration district
    // 4 = longitude
    // 5 = latitude
    // 6 = tier (type - hospital, teaching hospital, etc.)
    // 7 = case registration tier (type - hospital, teaching hospital, etc.)
    // 8 = Xpert placed here
    // 9 = other name

     // convert from string to float
    for (i = 1; i < facSize; i++) {

        // debug - true
        if (debug) {

            std::cout << "facilities structure" << "," << facilities[i][0] << "," << facilities[i][1] << "," << facilities[i][2] << "," << facilities[i][4] << "," << facilities[i][5] << "," << facilities[i][6] << '\n';
        }

        facList[i-1].facName = facilities[i][0];
        facList[i-1].region = facilities[i][1];
        facList[i-1].district = facilities[i][2];
        facList[i-1].longitude = stof(facilities[i][4]);
        facList[i-1].latitude = stof(facilities[i][5]);
        facList[i-1].tier = stoi(facilities[i][6]);
        facList[i-1].type = facilities[i][7];
        facList[i-1].xpert = facilities[i][9];

        // don't include end of line characters
        facList[i - 1].xpert.erase(std::remove(facList[i - 1].xpert.begin(), facList[i - 1].xpert.end(), '\r'), facList[i - 1].xpert.end());
        facList[i - 1].xpert.erase(std::remove(facList[i - 1].xpert.begin(), facList[i - 1].xpert.end(), '\n'), facList[i - 1].xpert.end());

        // give facilities sequential id numbers
        facList[i - 1].unitID = unitID;
        unitID++;

        facList[i - 1].upFacility = "";
        facList[i - 1].upDistance = 10;
    }

    // update array sizes to reflect removed header
    facSize = facSize - 1;

    std::cout << "after facilities structure" << '\n';

    // find the 16 regional hospitals
    noRegions = 0;
    for (i = 0; i < facSize; i++) {
        
        if (facList[i].tier == 3) {

            regHospitals[noRegions].name = facList[i].facName;
            regHospitals[noRegions].longitude = facList[i].longitude;
            regHospitals[noRegions].latitude = facList[i].latitude;
            noRegions = noRegions + 1;
        }
    }
    std::cout << "after finding regions " << '\n';

    // find closest regional hospital for each facility
    for (i = 0; i < facSize; i++) {
        
        for (k = 0; k < noRegions; k++) {
        
            dist = sqrt(pow((facList[i].longitude - regHospitals[k].longitude),2) + pow((facList[i].latitude - regHospitals[k].latitude),2));

            if (dist < facList[i].upDistance) {

                facList[i].upDistance = dist;
                facList[i].upFacility = regHospitals[k].name;
            }
        }
    }

    std::cout << "after finding closest upFacility and upDistance " << '\n';

    // convert all distances to meters
    for (i = 0; i < facSize; i++) {

        facList[noRegions].upDistance = facList[i].upDistance * 111139;
    }

    // facilities has the format 
    // 0 = region hospital, 
    // 1 = facility name, 
    // 2 = distance to regional hospital
    // 3 = tier (type - hospital, teaching hospital, etc.)
    // 4 = longitude
    // 5 = latitude
    // 6 = region
    // 7 = district
    // 8 = xpert
    // 9 = unitID

    myfile1.open("./defaults/input/facilities/case-reg-853-with-dist.csv");

    myfile1 << "Region.hospital" << ",Facility" << ",Dist.to.reg.hosp" << ",tier" << ",Longitude" << ",Latitude " << ",region" << ",district" << ",xpert" << ",unitID" << '\n';

    for (i = 0; i < facSize; i++) {

        myfile1 << facList[i].upFacility << "," <<facList[i].facName << "," << facList[i].upDistance << "," << facList[i].tier << "," << facList[i].longitude;
        myfile1 << "," << facList[i].latitude <<  "," << facList[i].region << "," << facList[i].district << "," << facList[i].xpert << "," << facList[i].unitID << '\n';
    }

    myfile1.close();

}






void walkGrid(float x, float y, int time, string facility, int id, float inc, int dir, travelData* coords, std::size_t noCoords) {

    std::size_t i;
    float thisInc = inc * dir;

    if (false) {
        std::cout << "inside walk grid facility =  " << facility << ", pt1 = " << x << ", " << y << ", time = " << time << ", bin inc = " << inc << ", fac id = " << id << ", dir = " << dir << '\n';
    }

    // first need to walk north
    // find point closest to the north 
    for (i = 0; i < noCoords; i++) {

        // same longitude
        if (abs(x - coords[i].longitude) < .001) {

            // next coordinate in latitude
            if (abs((y + inc) - coords[i].latitude) < .001) {

                // doesn't already belong to a facility
                if (coords[i].facility == "") {

                    // greater or equal to current time
                    if (coords[i].time >= time) {

                        // mark it as belonging to this facility
                        coords[i].facility = facility;
                        coords[i].id = id;

                        // call this routine with new point
                        walkGrid(coords[i].longitude, coords[i].latitude, coords[i].time, facility, id, inc, dir, coords, noCoords);

                    }

                }
                break;
            }
        }
    }
    // first need to walk east/west
    // find point closest to the south 
    for (i = 0; i < noCoords; i++) {

        // same latitude
        if (abs(y - coords[i].latitude) < .001) {

            // next coordinate in longitude
            if (abs((x + thisInc) - coords[i].longitude) < .001) {

                // doesn't already belong to a facility
                if (coords[i].facility == "") {

                    // greater or equal to current time
                    if (coords[i].time >= time) {

                        // mark it as belonging to this facility
                        coords[i].facility = facility;
                        coords[i].id = id;

                        // call this routine with new point
                        walkGrid(coords[i].longitude, coords[i].latitude, coords[i].time, facility, id, inc, dir, coords, noCoords);

                    }

                }
                break;
            }
        }
    }
    // last need to walk south
        // find point closest to the north 
        for (i = 0; i < noCoords; i++) {

            // same longitude
            if (abs(x - coords[i].longitude) < .001) {

                // next coordinate in latitude
                if (abs((y - inc) - coords[i].latitude) < .001) {

                    // doesn't already belong to a facility
                    if (coords[i].facility == "") {

                        // greater or equal to current time
                        if (coords[i].time >= time) {

                            // mark it as belonging to this facility
                            coords[i].facility = facility;
                            coords[i].id = id;

                            // call this routine with new point
                            walkGrid(coords[i].longitude, coords[i].latitude, coords[i].time, facility, id, inc, dir, coords, noCoords);

                        }

                    }
                    break;
                }
            }
        }
}


void fillGridCells(float inc, travelData* coords, std::size_t noCoords) {

    std::size_t i, j;

    // loop through grid finding cells without facility
    // look at adjacent grid cells and copy what is there
    for (i = 0; i < noCoords; i++) {

        if (coords[i].facility == "") {

            // look east
            for (j = 0; j < noCoords; j++) {

                if (coords[j].facility != "") {

                    if ((abs((coords[i].longitude + inc) - coords[j].longitude) + abs(coords[i].latitude - coords[j].latitude)) < .0001) {

                        coords[i].facility = coords[j].facility;
                        coords[i].id = coords[j].id;
                    }
                }
            }
        }
        if (coords[i].facility == "") {

            // look east
            for (j = 0; j < noCoords; j++) {

                if (coords[j].facility != "") {

                    if ((abs((coords[i].longitude - inc) - coords[j].longitude) + abs(coords[i].latitude - coords[j].latitude)) < .0001) {

                        coords[i].facility = coords[j].facility;
                        coords[i].id = coords[j].id;
                    }
                }
            }
        }
        if (coords[i].facility == "") {

            // look east
            for (j = 0; j < noCoords; j++) {

                if (coords[j].facility != "") {

                    if ((abs(coords[i].longitude - coords[j].longitude) + abs((coords[i].latitude + inc) - coords[j].latitude)) < .0001) {

                        coords[i].facility = coords[j].facility;
                        coords[i].id = coords[j].id;
                    }
                }
            }
        }
        if (coords[i].facility == "") {

            // look east
            for (j = 0; j < noCoords; j++) {

                if (coords[j].facility != "") {

                    if ((abs(coords[i].longitude - coords[j].longitude) + abs((coords[i].latitude - inc) - coords[j].latitude)) < .0001) {

                        coords[i].facility = coords[j].facility;
                        coords[i].id = coords[j].id;
                    }
                }
            }
        }

        // check diagonals
        if (coords[i].facility == "") {

            // look east
            for (j = 0; j < noCoords; j++) {

                if (coords[j].facility != "") {

                    if ((abs((coords[i].longitude + inc) - coords[j].longitude) + abs((coords[i].latitude + inc) - coords[j].latitude)) < .001) {

                        coords[i].facility = coords[j].facility;
                        coords[i].id = coords[j].id;
                    }
                }
            }
        }
        if (coords[i].facility == "") {

            // look east
            for (j = 0; j < noCoords; j++) {

                if (coords[j].facility != "") {

                    if ((abs((coords[i].longitude - inc) - coords[j].longitude) + abs((coords[i].latitude + inc) - coords[j].latitude)) < .001) {

                        coords[i].facility = coords[j].facility;
                        coords[i].id = coords[j].id;
                    }
                }
            }
        }
        if (coords[i].facility == "") {

            // look east
            for (j = 0; j < noCoords; j++) {

                if (coords[j].facility != "") {

                    if ((abs((coords[i].longitude + inc) - coords[j].longitude) + abs((coords[i].latitude - inc) - coords[j].latitude)) < .001) {

                        coords[i].facility = coords[j].facility;
                        coords[i].id = coords[j].id;
                    }
                }
            }
        }
        if (coords[i].facility == "") {

            // look east
            for (j = 0; j < noCoords; j++) {

                if (coords[j].facility != "") {

                    if ((abs((coords[i].longitude - inc) - coords[j].longitude) + abs((coords[i].latitude - inc) - coords[j].latitude)) < .001) {

                        coords[i].facility = coords[j].facility;
                        coords[i].id = coords[j].id;
                    }
                }
            }
        }
    }

}

// to create combined list
void updateMasterList(vector<vector<string>> masterList, vector<vector<string>> newList) {

    ofstream myfile;

   std::size_t i, j;
    std::size_t masterSize = masterList.size();
    std::size_t newSize = newList.size();
    float newDist;
    bool sameNameFlag;

    struct masterStruct {
        int id;
        int coordFlag;
        float longitude;
        float latitude;
        string geometry1;
        string geometry2;
        string name;
        string type;
        string origType;
        string address;
        string town;
        string district;
        string region;
        string geoSource;
        string listSource;
        int priority;
    };

    struct newStruct {
        int ignoreFlag;
        int id;
        int coordFlag;
        float longitude;
        float latitude;
        string geometry1;
        string geometry2;
        string name;
        string type;
        string origType;
        string address;
        string town;
        string district;
        string region;
        string geoSource;
        string listSource;
        int priority;
    };

    struct masterStruct masterArray[masterSize];
    struct newStruct newArray[newSize];

    // adjust array index for removed header
    masterSize = masterSize - 1;
    newSize = newSize - 1;

    std::cout << "inside updateMasterList Master Size = " << masterSize << '\n';

    // master has format id, longitude, latitude, geometry(c(x,y)), name, type, original type, address, district,  gps-source, list source, priority
    for (i = 0; i < masterSize; i++) {

        if (false) {
            std::cout << masterList[i+1][0] << ", " << masterList[i+1][1] << ", " << masterList[i+1][2] << ", " << masterList[i+1][3] << ", " << masterList[i+1][4] << ", " << masterList[i+1][5] << ", " << masterList[i+1][6];
            std::cout << ", " << masterList[i+1][7] << ", " << masterList[i+1][8] << ", " << masterList[i+1][9] << ", " << masterList[i+1][10] << ", " << masterList[i+1][11];
            std::cout << ", " << masterList[i + 1][12] << ", " << masterList[i + 1][13] << ", " << masterList[i + 1][14];
            if (masterList[i + 1][4] == "") {
                std::cout << '\n';
            }
            else {
                std::cout << ", " << masterList[i + 1][15] << '\n';
            }
        }

        // convert where neccessary
        masterArray[i].id = stoi(masterList[i+1][0]);
        masterArray[i].coordFlag = stoi(masterList[i + 1][1]);
        if (masterList[i + 1][2] == "") {
            masterArray[i].longitude = 100;
            masterArray[i].latitude = 100;
        }
        else {

            masterArray[i].longitude = stof(masterList[i + 1][2]);
            masterArray[i].latitude = stof(masterList[i + 1][3]);
        }
        if (masterList[i + 1][4] == "") {

            // no C(x,y) geometry definition, only read 1 delimited space
            //std::cout << "inside updateMasterList no c(x,y) name = " << masterList[i+1][5] << '\n';

            masterArray[i].geometry1 = masterList[i + 1][4];
            masterArray[i].geometry2 = "";
            masterArray[i].name = masterList[i + 1][5];
            masterArray[i].type = masterList[i + 1][6];
            masterArray[i].origType = masterList[i + 1][7];
            masterArray[i].address = masterList[i + 1][8];
            masterArray[i].town = masterList[i + 1][9];
            masterArray[i].district = masterList[i + 1][10];
            masterArray[i].region = masterList[i + 1][11];
            masterArray[i].geoSource = masterList[i + 1][12];
            masterArray[i].listSource = masterList[i + 1][13];
            masterArray[i].priority = stoi(masterList[i + 1][14]);
        }
        else {

            masterArray[i].geometry1 = masterList[i + 1][4];
            masterArray[i].geometry2 = masterList[i + 1][5];
            masterArray[i].name = masterList[i + 1][6];
            masterArray[i].type = masterList[i + 1][7];
            masterArray[i].origType = masterList[i + 1][8];
            masterArray[i].address = masterList[i + 1][9];
            masterArray[i].town = masterList[i + 1][10];
            masterArray[i].district = masterList[i + 1][11];
            masterArray[i].region = masterList[i + 1][12];
            masterArray[i].geoSource = masterList[i + 1][13];
            masterArray[i].listSource = masterList[i + 1][14];
            masterArray[i].priority = stoi(masterList[i + 1][15]);
        }

    }

    

    // output array
    if (false) {
        for (i = 0; i < masterSize; i++) {
            std::cout << "facility name = " << masterArray[i].name << ", id = " << masterArray[i].id << ", x,y = " << masterArray[i].longitude << ", " << masterArray[i].latitude << '\n';
        }
    }



    std::cout << "inside update master newSize = " << newSize << '\n';

    // new list has format id, longitude, latitude, geometry(c(x,y)), name, type, original type, address, district,  gps-source, list source, priority
    for (i = 0; i < newSize; i++) {

        if (false) {
            std::cout << newList[i + 1][0] << ", " << newList[i + 1][1] << ", " << newList[i + 1][2] << ", " << newList[i + 1][3] << ", " << newList[i + 1][4] << ", " << newList[i + 1][5];
            std::cout << ", " << newList[i + 1][6] << ", " << newList[i + 1][7] << ", " << newList[i + 1][8] << ", " << newList[i + 1][9] << ", " << newList[i + 1][10] << ", " << newList[i + 1][11];
            std::cout << ", " << newList[i + 1][12] << ", " << newList[i + 1][13];
            if (newList[i + 1][4] == "") {
                std::cout << '\n';
            }
            else {
                std::cout << ", " << newList[i + 1][14] << '\n';
            }
        }

        // every line is valid for now, ignoreFlag changes to 1, when the row is a duplicate of one in masterList
        newArray[i].ignoreFlag = 0;

        newArray[i].id = stoi(newList[i+1][0]);

        // check to see if x,y values, otherwise decompose geometry if it exists
        if (newList[i+1][2] == "") {

            // no x or y values check geonetry
            if (newList[i+1][4] == "") {

                // no coordinates
                newArray[i].coordFlag = 0;
                newArray[i].longitude = 100;
                newArray[i].latitude = 100;
            }
            else {

            // decompose c(x,y) to x and y
            //std::cout << newList[i + 1][4] << "," << newList[i+1][5] << '\n';
            //std::cout << newList[i + 1][4].substr(3,newList[i+1][4].length()) << '\n';
            //std::cout << newList[i + 1][5].substr(0,newList[i+1][5].length()-2) << '\n';
            newArray[i].longitude = stof(newList[i + 1][4].substr(3, newList[i + 1][4].length()));
            newArray[i].latitude = stof(newList[i + 1][5].substr(0, newList[i + 1][5].length() - 2));
            newArray[i].coordFlag = 1;
            }
        }
        else {

            newArray[i].coordFlag = 1;
            newArray[i].longitude = stof(newList[i + 1][2]);
            newArray[i].latitude = stof(newList[i + 1][3]);
        }
       
        if (newList[i + 1][4] == "") {
            newArray[i].geometry1 = newList[i + 1][4];
            newArray[i].geometry2 = "";
            newArray[i].name = newList[i + 1][5];
            newArray[i].type = newList[i + 1][6];
            newArray[i].origType = newList[i + 1][7];
            newArray[i].address = newList[i + 1][8];
            newArray[i].town = newList[i][9];
            newArray[i].district = newList[i + 1][10];
            newArray[i].region = newList[i + 1][11];
            newArray[i].geoSource = newList[i + 1][12];
            newArray[i].listSource = newList[i + 1][13];
            newArray[i].priority = stoi(newList[i + 1][14]);
        }
        else {
            newArray[i].geometry1 = newList[i + 1][4];
            newArray[i].geometry2 = newList[i + 1][5];
            newArray[i].name = newList[i + 1][6];
            newArray[i].type = newList[i + 1][7];
            newArray[i].origType = newList[i + 1][8];
            newArray[i].address = newList[i + 1][8];
            newArray[i].town = newList[i][10];
            newArray[i].district = newList[i + 1][11];
            newArray[i].region = newList[i + 1][12];
            newArray[i].geoSource = newList[i + 1][13];
            newArray[i].listSource = newList[i + 1][14];
            newArray[i].priority = stoi(newList[i + 1][15]);
        }
    }

    std::cout << "inside update master after decomposing coordinates " << '\n';

    for (i = 0; i < newSize; i++) {

        // remove entries that don't have coordinates and names
        if (newArray[i].longitude == 100) {
            if (newArray[i].name == "") {
                newArray[i].ignoreFlag = 1;
            }
        }

    }


    for (i = 0; i < newSize; i++) {

        if (newArray[i].ignoreFlag == 0) {

            // see if duplicates between lists
            for (j = 0; j < masterSize; j++) {

                sameNameFlag = false;

                // see if new entry has coordinates
                if (newArray[i].coordFlag == 1) {

                    // see if coordinates of any of the master list entries are within a given distance
                    if (masterArray[j].coordFlag == 1) {
                        newDist = sqrt(pow(newArray[i].longitude - masterArray[j].longitude, 2) + (pow(newArray[i].latitude - masterArray[j].latitude, 2)));
                        //std::cout << "newDist " << newDist << '\n';

                        // less than 111 meters
                        if (newDist < .001) {

                            newArray[i].ignoreFlag = 1;
                            // std::cout << "inside update master found duplicate coordinates =  " << newArray[i].name << "," << masterArray[j].name << '\n';
                        }
                    }
                    else {

                        // no coordinate in the list for this master facility, try name similarity checking
                        sameNameFlag = sameName(masterArray[j].name, newArray[i].name);

                        if (sameNameFlag) {

                            newArray[i].ignoreFlag = 1;
                        }
                    }
                }
                else {

                    // need to do name simalarity checking
                    // no coordinate in the list for this master facility, try name similarity checking
                    sameNameFlag = sameName(masterArray[j].name, newArray[i].name);

                    if (sameNameFlag) {

                        newArray[i].ignoreFlag = 1;
                    }
                }
            }
        }

    }

    std::cout << "inside update master after duplicate checking " << '\n';
    if (false) {

        for (i = 0; i < newSize; i++) {
            if (newArray[i].ignoreFlag == 0) {

                std::cout << "new facility name = " << newArray[i].name << " coordFlag = " << newArray[i].coordFlag << " " << newArray[i].longitude << ", " << newArray[i].latitude << ", i = " << i << '\n';
            }
        }
    }

    myfile.open("./facility-project/MasterFacilityList.csv");

    myfile << "id " << ",coordFlag " << ", longitude " << ",latitude" << ",geometry" << ",name" << ",type" << ", orig-type" << ", address" << ", town" << ",district" << ", region" << ", gps-source" << ", list-source" << ", priority" << '\n';

    for (i = 0; i < masterSize; i++) {

        myfile << masterArray[i].id << "," << masterArray[i].coordFlag;
        if (masterArray[i].coordFlag == 1) {
            myfile << "," << masterArray[i].longitude << "," << masterArray[i].latitude;
        }
        else {
            myfile << "," << "" << "," << "";
        }
        myfile << "," << masterArray[i].geometry1;
        if (masterArray[i].geometry2 != "") {
            myfile << "," << masterArray[i].geometry2;
        }
        myfile << "," << masterArray[i].name << "," << masterArray[i].type << "," << masterArray[i].origType << "," << masterArray[i].address << "," << masterArray[i].town << "," << masterArray[i].district << "," << masterArray[i].region;
        myfile << "," << masterArray[i].geoSource << "," << masterArray[i].listSource << "," << masterArray[i].priority << '\n';
    }
    // now add new entries
    for (i = 0; i < newSize; i++) {

        if (newArray[i].ignoreFlag == 0) {

            myfile << newArray[i].id << "," << newArray[i].coordFlag;
            if (newArray[i].coordFlag == 1) {
                myfile << "," << newArray[i].longitude << "," << newArray[i].latitude;
            }
            else {
                myfile << "," << "" << "," << "";
            } 
            myfile << "," << newArray[i].geometry1;
            if (newArray[i].geometry2 != "") {
                myfile << "," << newArray[i].geometry2;
            }
            myfile << "," << newArray[i].name << "," << newArray[i].type << "," << newArray[i].origType << "," << newArray[i].address << "," << newArray[i].town << "," << newArray[i].district << "," << newArray[i].region;
            myfile << "," << newArray[i].geoSource << "," << newArray[i].listSource << "," << newArray[i].priority << '\n';
        }
    }
    myfile.close();
}
// just add lists together
void createMasterList(vector<vector<string>> masterList, vector<vector<string>> newList) {

    ofstream myfile;

    int count = 0;
    int ids = 1;
    string newName;
    std::size_t i;
    std::size_t masterSize = masterList.size();
    std::size_t newSize = newList.size();

    struct masterStruct {
        int id;
        int coordFlag;
        float longitude;
        float latitude;
        string geometry1;
        string geometry2;
        string name;
        string type;
        string origType;
        string address;
        string town;
        string district;
        string region;
        string geoSource;
        string listSource;
        int priority;
    };

    struct newStruct {
        int id;
        int coordFlag;
        int ignoreFlag;
        float longitude;
        float latitude;
        string geometry1;
        string geometry2;
        string name;
        string type;
        string origType;
        string address;
        string town;
        string district;
        string region;
        string geoSource;
        string listSource;
        int priority;
    };

    vector<masterStruct> masterArray;
    vector<newStruct> newArray;

    // adjust array index for removed header
    masterSize = masterSize - 1;
    newSize = newSize - 1;

    std::cout << "inside createMasterList Master Size = " << masterSize << '\n';

    // master has format id, longitude, latitude, geometry(c(x,y)), name, type, original type, address, district,  gps-source, list source
    for (i = 0; i < masterSize; i++) {

        if (false) {
            std::cout << masterList[i + 1][0] << ", " << masterList[i + 1][1] << ", " << masterList[i + 1][2] << ", " << masterList[i + 1][3] << ", " << masterList[i + 1][4] << ", " << masterList[i + 1][5] << ", " << masterList[i + 1][6];
            std::cout << ", " << masterList[i + 1][7] << ", " << masterList[i + 1][8] << ", " << masterList[i + 1][9] << ", " << masterList[i + 1][10] << ", " << masterList[i + 1][11];
            std::cout << ", " << masterList[i + 1][12] << ", " << masterList[i + 1][13] << ", " << masterList[i + 1][14];
            if (masterList[i + 1][4] == "") {
                std::cout << '\n';
            }
            else {
                std::cout << ", " << masterList[i + 1][15] << '\n';
            }
        }
        newName = "masterSruct" + std::to_string(count++);

        masterStruct newName;

        // convert where neccessary
        newName.id = stoi(masterList[i + 1][0]);
        newName.coordFlag = stoi(masterList[i + 1][1]);
        if (masterList[i + 1][2] == "") {
            newName.longitude = 100;
            newName.latitude = 100;
        }
        else {

            newName.longitude = stof(masterList[i + 1][2]);
            newName.latitude = stof(masterList[i + 1][3]);
        }
        if (masterList[i + 1][4] == "") {

            // no C(x,y) geometry definition, only read 1 delimited space
            //std::cout << "inside updateMasterList no c(x,y) name = " << masterList[i+1][5] << '\n';

            newName.geometry1 = masterList[i + 1][4];
            newName.geometry2 = "";
            newName.name = masterList[i + 1][5];
            newName.type = masterList[i + 1][6];
            newName.origType = masterList[i + 1][7];
            newName.address = masterList[i + 1][8];
            newName.town = masterList[i + 1][9];
            //newName.district = cleanDistrict(masterList[i + 1][10]);
            newName.district = masterList[i + 1][10];
            newName.region = masterList[i + 1][11];
            newName.geoSource = masterList[i + 1][12];
            newName.listSource = masterList[i + 1][13];
            newName.priority = stoi(masterList[i + 1][14]);
        }
        else {

            newName.geometry1 = masterList[i + 1][4];
            newName.geometry2 = masterList[i + 1][5];
            newName.name = masterList[i + 1][6];
            newName.type = masterList[i + 1][7];
            newName.origType = masterList[i + 1][8];
            newName.address = masterList[i + 1][9];
            newName.town = masterList[i + 1][10];
            //newName.district = cleanDistrict(masterList[i + 1][11]);
            newName.district = masterList[i + 1][11];
            newName.region = masterList[i + 1][12];
            newName.geoSource = masterList[i + 1][13];
            newName.listSource = masterList[i + 1][14];
            newName.priority = stoi(masterList[i + 1][15]);
        }
        masterArray.push_back(newName);

    }

    masterSize = masterArray.size();



    // output array
    if (false) {
        for (i = 0; i < masterSize; i++) {
            std::cout << "facility name = " << masterArray[i].name << ", id = " << masterArray[i].id << ", x,y = " << masterArray[i].longitude << ", " << masterArray[i].latitude << '\n';
        }
    }



    std::cout << "inside update master newSize = " << newSize << '\n';

    count = 0;

    // new list has format id, longitude, latitude, geometry(c(x,y)), name, type, original type, address, district,  gps-source, etc
    for (i = 0; i < newSize; i++) {

        if (false) {
            std::cout << newList[i + 1][0] << ", " << newList[i + 1][1] << ", " << newList[i + 1][2] << ", " << newList[i + 1][3] << ", " << newList[i + 1][4] << ", " << newList[i + 1][5];
            std::cout << ", " << newList[i + 1][6] << ", " << newList[i + 1][7] << ", " << newList[i + 1][8] << ", " << newList[i + 1][9] << ", " << newList[i + 1][10] << ", " << newList[i + 1][11];
            std::cout << ", " << newList[i + 1][12] << ", " << newList[i + 1][13] << ", " << newList[i + 1][14];
            if (newList[i + 1][4] == "") {
                std::cout << '\n';
            }
            else {
                std::cout << ", " << newList[i + 1][15] << '\n';
            }
        }
        newName = "newSruct" + std::to_string(count++);

        newStruct newName;

        newName.ignoreFlag = 0;

        newName.id = stoi(newList[i + 1][0]);

        // check to see if x,y values, otherwise decompose geometry if it exists
        if (newList[i + 1][2] == "") {

            // no x or y values check geonetry
            if (newList[i + 1][4] == "") {

                // no coordinates
                newName.coordFlag = 0;
                newName.longitude = 100;
                newName.latitude = 100;
            }
            else {

                // decompose c(x,y) to x and y
                //std::cout << newList[i + 1][4] << "," << newList[i+1][5] << '\n';
                //std::cout << newList[i + 1][4].substr(3,newList[i+1][4].length()) << '\n';
                //std::cout << newList[i + 1][5].substr(0,newList[i+1][5].length()-2) << '\n';
                newName.longitude = stof(newList[i + 1][4].substr(3, newList[i + 1][4].length()));
                newName.latitude = stof(newList[i + 1][5].substr(0, newList[i + 1][5].length() - 2));
                newName.coordFlag = 1;
            }
        }
        else {

            newName.coordFlag = 1;
            newName.longitude = stof(newList[i + 1][2]);
            newName.latitude = stof(newList[i + 1][3]);
        }

        if (newList[i + 1][4] == "") {
            newName.geometry1 = newList[i + 1][4];
            newName.geometry2 = "";
            newName.name = newList[i + 1][5];
            newName.type = newList[i + 1][6];
            newName.origType = newList[i + 1][7];
            newName.address = newList[i + 1][8];
            newName.town = newList[i+1][9];
            //newName.district = cleanDistrict(newList[i + 1][10]);
            newName.district = newList[i + 1][10];
            newName.region = newList[i + 1][11];
            newName.geoSource = newList[i + 1][12];
            newName.listSource = newList[i + 1][13];
            newName.priority = stoi(newList[i + 1][14]);
        }
        else {
            newName.geometry1 = newList[i + 1][4];
            newName.geometry2 = newList[i + 1][5];
            newName.name = newList[i + 1][6];
            newName.type = newList[i + 1][7];
            newName.origType = newList[i + 1][8];
            newName.address = newList[i + 1][8];
            newName.town = newList[i+1][10];
            //newName.district = cleanDistrict(newList[i + 1][11]);
            newName.district = newList[i + 1][11];
            newName.region = newList[i + 1][12];
            newName.geoSource = newList[i + 1][13];
            newName.listSource = newList[i + 1][14];
            newName.priority = stoi(newList[i + 1][15]);
        }
        newArray.push_back(newName);

    }

    newSize = newArray.size();

    std::cout << "inside update master after decomposing coordinates " << '\n';

    for (i = 0; i < newSize; i++) {

        // remove entries that don't have coordinates and names
        if (newArray[i].longitude == 100) {
            if (newArray[i].name == "") {
                newArray[i].ignoreFlag = 1;
            }
        }

    }

    if (false) {

        for (i = 0; i < newSize; i++) {
            if (newArray[i].ignoreFlag == 0) {

                std::cout << "new facility name = " << newArray[i].name << " coordFlag = " << newArray[i].coordFlag << " " << newArray[i].longitude << ", " << newArray[i].latitude << ", i = " << i << '\n';
            }
        }
    }

    myfile.open("./facility-project/MasterFacilityListRaw.csv");

    myfile << "id " << ",coordFlag " << ", longitude " << ",latitude" << ",geometry" << ",name" << ",type" << ", orig-type" << ", address" << ", town" << ",district" << ", region" << ", gps-source" << ", list-source" << ", priority" << '\n';

    for (i = 0; i < masterSize; i++) {

        myfile << ids++ << "," << masterArray[i].coordFlag;
        if (masterArray[i].coordFlag == 1) {
            myfile << "," << masterArray[i].longitude << "," << masterArray[i].latitude;
        }
        else {
            myfile << "," << "" << "," << "";
        }
        myfile << "," << masterArray[i].geometry1;
        if (masterArray[i].geometry2 != "") {
            myfile << "," << masterArray[i].geometry2;
        }
        myfile << "," << masterArray[i].name << "," << masterArray[i].type << "," << masterArray[i].origType << "," << masterArray[i].address << "," << masterArray[i].town << "," << masterArray[i].district << "," << masterArray[i].region;
        myfile << "," << masterArray[i].geoSource << "," << masterArray[i].listSource << '\n';
    }
    // now add new entries
    for (i = 0; i < newSize; i++) {

        if (newArray[i].ignoreFlag == 0) {

            myfile << ids++ << "," << newArray[i].coordFlag;
            if (newArray[i].coordFlag == 1) {
                myfile << "," << newArray[i].longitude << "," << newArray[i].latitude;
            }
            else {
                myfile << "," << "" << "," << "";
            }
            myfile << "," << newArray[i].geometry1;
            if (newArray[i].geometry2 != "") {
                myfile << "," << newArray[i].geometry2;
            }
            myfile << "," << newArray[i].name << "," << newArray[i].type << "," << newArray[i].origType << "," << newArray[i].address << "," << newArray[i].town << "," << newArray[i].district << "," << newArray[i].region;
            myfile << "," << newArray[i].geoSource << "," << newArray[i].listSource << "," << newArray[i].priority << '\n';
        }
    }
    myfile.close();
}


// to create combined list
bool sameName(string name1, string name2) {

    if (name1.length() == 0) {
        return false;
    }
    if (name2.length() == 0) {
        return false;
    }

   if (name1.compare(name2) == 0) {
       //std::cout << "inside same name found match =  " << name1 << "," << name2 << '\n';
       return true;
   }
   else {
       //std::cout << "inside same name =  " << name1 << "," << name2 << '\n';
       return false;
   }

}





// Clean up the district names
string cleanDistrict(string oldDistrict) {

    string district;
    string temp;
    std::size_t n;

    district = oldDistrict;

    if (district.length() == 0) {
        return district;
    }

    // now substitute Municipal from district name
    n = district.find(" Municipal");
    if (n != district.npos) {
        district.replace(n, 10, "");
    }

    // now substitute Metro for Metropolitan
    //n = district.find("Metropolitan");
    //if (n != district.npos) {
    //    district.replace(n, 12, "Metro");
    //}

    // remove first hypen
    n = district.find("-");
    if (n != district.npos) {
        district.replace(n, 1, " ");
    }

    // remvove any second hyphen
    n = district.find("-");
    if (n != district.npos) {
        district.replace(n, 1, " ");
    }


    // remvove any third hyphen
    n = district.find("-");
    if (n != district.npos) {
        district.replace(n, 1, " ");
    }

    // remove leading space
    const char* WhiteSpace = " \t\n\r\f\v";
    const char* letters = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
    n = district.find_first_of(letters);
    if (n == 1) {
        district = district.substr(1);
    }
    //district = std::regex_replace(district, std::regex("^ +"), "");

    // remove trailing spaces
    n = district.find_last_not_of(WhiteSpace);
    if (n != std::string::npos) {
        district.erase(n + 1);
    }
    //district = std::regex_replace(district, std::regex(" +$"), "");

    //std::cout << "clean district =  " << oldDistrict << "," << district << '\n';

    return district;
}


// check to see if name is generic
int checkGenericName(string name) {

    string uName;

    // if either of the two facilities has a generic name, then return value that that is true
    // generic names are: empty, NA, Voice Clinic, RCH, R.C.H., CHPS, Mat. Home, Clinic, clinic, Community Clinic, Homeopathic, SDA, S.D.A.
    //                    SDA Hospital, District Hospital, Dist. Hospital, Reproductive and Child Health, PHC, NTC, Dist. Hosp, SDA Hosp.
    //                    Government Hospital, R.C.H, STATE CLINIC, RCH Clinic, Rregional Hospital, State Clinic, M.C.H Clinic
    // return value = 0, neither is generic
    // return value = 1, name is generic

   
    
    // check to see if name is empty
    if (name.length() == 0) {
        return 1;
    }

    uName = name;

    // first change both names to upper case
    std::transform(uName.begin(), uName.end(), uName.begin(), ::toupper);

    // now remove all spaces in name
    uName.erase(std::remove_if(uName.begin(), uName.end(), ::isspace), uName.end());
    //std::cout << uName << '\n';

    // check for Voice Clinic
    if (uName == "NA") {
        return 1;
    }

    // check for Voice Clinic
    if (uName == "VOICECLINIC") {
        return 1;
    }

    // check for RCH
    if (uName == "RCH") {
        return 1;
    }

    // check for RCH Clinic
    if (uName == "RCHCLINIC") {
        return 1;
    }

    // check for M.C.H Clinic
    if (uName == "M.C>HCLINIC") {
        return 1;
    }

    // check for R.C.H.
    if (uName == "R.C.H.") {
        return 1;
    }

    // check for R.C.H
    if (uName == "R.C.H") {
        return 1;
    }

    // check for CHPS
    if (uName == "CHPS") {
        return 1;
    }

    // check for Mat. Home
    if (uName == "MAT.HOME") {
        return 1;
    }

    // check for Clinic
    if (uName == "CLINIC") {
        return 1;
    }

    // check for Community Clinic
    if (uName == "COMMUNITYCLINIC") {
        return 1;
    }

    // check for State Clinic
    if (uName == "STATECLINIC") {
        return 1;
    }

    // check for Homeopathic
    if (uName == "HOMEOPATHIC") {
        return 1;
    }

    // check for SDA Hospital
    if (uName == "SDAHOSPITAL") {
        return 1;
    }

    // check for District Hospital
    if (uName == "DISTRICTHOSPITAL") {
        return 1;
    }

    // check for Regional Hospital
    if (uName == "REGIONALHOSPITAL") {
        return 1;
    }

    // check for District Hospital
    if (uName == "GOVERNMENTHOSPITAL") {
        return 1;
    }

    // check for Dist. Hospital
    if (uName == "DIST.HOSPITAL") {
        return 1;
    }

    // check for Dist. Hospital
    if (uName == "DIST.HOSP") {
        return 1;
    }

    // check for SDA Clinic
    if (uName == "SDACLINIC") {
        return 1;
    }

    // check for SDA
    if (uName == "SDA") {
        return 1;
    }

    // check for SDA Hosp.
    if (uName == "SDAHOSP.") {
        return 1;
    }

    // check for S.D.A. Clinic
    if (uName == "S.D.A.CLINIC") {
        return 1;
    }

    // check for Reproductive and Child Health
    if (uName == "REPRODUCTIVEANDCHILDHEALTH") {
        return 1;
    }

    // check for PHC
    if (uName == "PHC") {
        return 1;
    }

    // check for NTC
    if (uName == "NTC") {
        return 1;
    }

    // failed all tests, must have a unique name
    return 0;

}

// check to see if either name is generic
int checkGenericNamePair(string name1, string name2) {

    int status;

    // if either of the two facilities has a generic name, then return value that that is true
    // generic names are: empty, NA, Voice Clinic, RCH, R.C.H., CHPS, Mat. Home, Clinic, clinic, Community Clinic, Homeopathic, SDA, S.D.A.
    //                    SDA Hospital, District Hospital, Dist. Hospital, Reproductive and Child Health, PHC, NTC, Dist. Hosp, SDA Hosp.
    //                    Government Hospital, R.C.H, STATE CLINIC, RCH Clinic, Rregional Hospital, State Clinic, M.C.H Clinic
    // return value = 0, neither is generic
    // return value = 1, name1 is generic
    // return value = 2, name2 is generic

    // check name1
    status = checkGenericName(name1);

    if (status == 1) {
        return 1;
    }

    // check name2
    status = checkGenericName(name2);

    if (status == 1) {
        return 2;
    }

    // neither generic
    return 0;

}



// check to see if facility types are the same
bool typeCheck(string type1, string type2) {

    bool type1Hospital = false;
    bool type2Hospital = false;

    // check to see if exactly the same
    // if different, check to see if one is Hospital and the other is District Hospital, 
    // Referral Hospital or Regional Hospital

    // direct check
    if (type1 == type2) {

        return true;
    }

    if ((type1 == "Hospital") || (type1 == "District Hospital") || (type1 == "Referral Hospital") || (type1 == "Regional Hospital")) {

        type1Hospital = true;
    }
    if ((type2 == "Hospital") || (type2 == "District Hospital") || (type2 == "Referral Hospital") || (type2 == "Regional Hospital")) {

        type2Hospital = true;
    }

    if (type1Hospital && type2Hospital) {

        return true;
    }

    // no match
    return false;

}






// Convert facility name to sanitized string
string convertName(string name1, bool typeFlag) {

    string uName1;
    string temp;
    std::size_t n, m,  i;

    if (name1.length() == 0) {
        return "";
    }

    uName1 = name1;
    //std::cout << "inside convert name - " << uName1 << ", " << uName1 << '\n';

    // this routine transforms a facility name to a uppercase string with no spaces or special characters
    // It also swaps out abbreviations for longer words and converts over to Ghana tier mapping
    // order of checks is sometimes important

    // first change both names to upper case
    std::transform(uName1.begin(), uName1.end(), uName1.begin(), ::toupper);

    // now remove all spaces in name
    uName1.erase(std::remove_if(uName1.begin(), uName1.end(), ::isspace), uName1.end());

    // if last 3 letters are Hom change it to Home
    //if (uName1.substr(uName1.size()-3) == "HOM") {
    //    uName1 = uName1 + "E";
    //}

    // now substitute out Infirmary for PHC
    n = uName1.find("INFIRMARY");
    if (n != uName1.npos) {
        uName1.replace(n, 9, "PHC");
    }

    // now substitute out Chips for CHPS
    n = uName1.find("CHIPS");
    if (n != uName1.npos) {
        uName1.replace(n, 5, "CHPS");
    }

    // now substitute out Chip for CHPS
    n = uName1.find("CHIP");
    if (n != uName1.npos) {
        uName1.replace(n, 4, "CHPS");
    }

    // now substitute out & Laboratory for ""
    n = uName1.find("&LABORATORY");
    if (n != uName1.npos) {
        uName1.replace(n, 10, "");
    }

    // now substitute out CENTER for CENTRE
    n = uName1.find("CENTER");
    if (n != uName1.npos) {
        uName1.replace(n, 6, "CENTRE");
    }

    // now substitute out HEALTH CENTRE for HC
    n = uName1.find("HEALTHCENTRE");
    if (n != uName1.npos) {
        uName1.replace(n, 12, "HC");
    }

    // now substitute out HEALTH CLINIC for HC
    n = uName1.find("HEALTHCLINIC");
    if (n != uName1.npos) {
        uName1.replace(n, 12, "HC");
    }

    // now substitute out Community-based Health Planning and Services for CHPS
    n = uName1.find("COMMUNITY-BASEDHEALTHPLANNINGANDSERVICES");
    if (n != uName1.npos) {
        uName1.replace(n, 40, "CHPS");
    }

    // now substitute out CHEPS for CHPS
    n = uName1.find("CHEPS");
    if (n != uName1.npos) {
        uName1.replace(n, 5, "CHPS");
    }

    // now substitute out Rural Clinic for Clinic
    n = uName1.find("RURALCLINIC");
    if (n != uName1.npos) {
        uName1.replace(n, 11, "CLINIC");
    }

    // now substitute out Rural Health Center HC
    n = uName1.find("RURALHEALTHCENTRE");
    if (n != uName1.npos) {
        uName1.replace(n, 17, "HC");
    }

    // now substitute out RCH CLINIC for RCH
    n = uName1.find("RCHCLINIC");
    if (n != uName1.npos) {
        uName1.replace(n, 9, "RCH");
    }

    // now substitute out Rural Health Center HC
    n = uName1.find("RCH");
    if (n != uName1.npos) {
        uName1.replace(n, 3, "CLINIC");
    }

    // now substitute out EYE Clinic for Clinic
    n = uName1.find("EYECLINIC");
    if (n != uName1.npos) {
        uName1.replace(n, 9, "CLINIC");
    }

    // now substitute out Surgical Center for Clinic
    n = uName1.find("SURGICALCENTRE");
    if (n != uName1.npos) {
        uName1.replace(n, 14, "CLINC");
    }

    // now substitute out Rural Center for Clinic
    n = uName1.find("RURALCENTRE");
    if (n != uName1.npos) {
        uName1.replace(n, 11, "CLINC");
    }

    // now substitute out Clinic & Laboratory for Clinic
    n = uName1.find("CLINIC&LABORATORY");
    if (n != uName1.npos) {
        uName1.replace(n, 17, "CLINC");
    }

    // now substitute out Clinic & Lab for Clinic
    n = uName1.find("CLINIC&LAB");
    if (n != uName1.npos) {
        uName1.replace(n, 10, "CLINC");
    }

    // now substitute out Clinic/Screening for Clinic
    n = uName1.find("CLINIC/SCREENING");
    if (n != uName1.npos) {
        uName1.replace(n, 16, "CLINC");
    }

    // now substitute out Polyclinic for Hospital
    n = uName1.find("POLYCLINIC");
    if (n != uName1.npos) {
        uName1.replace(n, 10, "HOSPITAL");
    }

    // now substitute out Comm Clinic for Clinic
    n = uName1.find("COMMCLINIC");
    if (n != uName1.npos) {
        uName1.replace(n, 10, "CLINC");
    }

    // now substitute out Medical Centre for Clinic
    n = uName1.find("MEDICALCENTRE");
    if (n != uName1.npos) {
        uName1.replace(n, 13, "CLINIC");
    }

    // now substitute out Rehabilitation for Rehab
    n = uName1.find("REHABILITATION");
    if (n != uName1.npos) {
        uName1.replace(n, 14, "REHAB");
    }

    // remove all other CENTRE occurances
    n = uName1.find("CENTRE");
    if (n != uName1.npos) {
        uName1.replace(n, 6, "");
    }

    // now substitute out Home/Clinic for Home
    n = uName1.find("HOME/CLINIC");
    if (n != uName1.npos) {
        uName1.replace(n,11, "HOME");
    }

    // now substitute out Maternity Home for PHC
    n = uName1.find("MATERNITYHOME");
    if (n != uName1.npos) {
        uName1.replace(n, 13, "PHC");
    }

    // now substitute out Maternity Hom for PHC
    n = uName1.find("MATERNITYHOM");
    if (n != uName1.npos) {
        uName1.replace(n, 12, "PHC");
    }

    // now substitute out Mat Home for PHC
    n = uName1.find("MATHOME");
    if (n != uName1.npos) {
        uName1.replace(n, 7, "PHC");
    }

    // now substitute out Mat. Hom for PHC
    n = uName1.find("MATHOM");
    if (n != uName1.npos) {
        uName1.replace(n, 6, "PHC");
    }

    // now substitute out Mat. Home for PHC
    n = uName1.find("MAT.HOME");
    if (n != uName1.npos) {
        uName1.replace(n, 8, "PHC");
    }

    // now substitute out Mat. Hom for PHC
    n = uName1.find("MAT.HOM");
    if (n != uName1.npos) {
        uName1.replace(n, 7, "PHC");
    }

    // now substitute out Nut. for Nutritional
    n = uName1.find("NUT.");
    if (n != uName1.npos) {
        uName1.replace(n, 4, "NUTRITIONAL");
    }

    // now substitute out Nutritional rehab and Feeding Center for PHC
    n = uName1.find("NUTRITIONALREHABANDFEEDINGCENTER");
    if (n != uName1.npos) {
        uName1.replace(n, 32, "PHC");
    }

    // now substitute out Nutritional rehab and Feeding Center for PHC
    n = uName1.find("NUTRITIONCENTER");
    if (n != uName1.npos) {
        uName1.replace(n, 15, "PHC");
    }

    // now substitute out Rehab CNT for Rahabitational Centre
    n = uName1.find("REHABCNT");
    if (n != uName1.npos) {
        uName1.replace(n, 8, "REHABITATIONALCENTRE");
    }

    // now substitute out Hosp. for Hospital
    n = uName1.find("HOSP.");
    if (n != uName1.npos) {
        uName1.replace(n, 5, "HOSPITAL");
    }

    // now substitute out Military Hospital for Referral Hospital
    n = uName1.find("MILITARYHOSPITAL");
    if (n != uName1.npos) {
        uName1.replace(n, 16, "REFERRALHOSPITAL");
    }

    // now substitute out Teaching Hospital for Referral Hospital
    n = uName1.find("TEACHINGHOSPITAL");
    if (n != uName1.npos) {
        uName1.replace(n, 16, "REFERRALHOSPITAL");
    }

    // now substitute out Hospital and Teaching Hospital for Referral Hospital
    n = uName1.find("HOSPITALANDTEACHINGHOSPITAL");
    if (n != uName1.npos) {
        uName1.replace(n, 27, "REFERRALHOSPITAL");
    }

    // expand gov't abbreviation
    n = uName1.find("GOV'T");
    if (n != uName1.npos) {
        uName1.replace(n, 5, "GOVERNMENT");
    }

    // expand gov't abbreviation
    n = uName1.find("GOVT");
    if (n != uName1.npos) {
        uName1.replace(n, 4, "GOVERNMENT");
    }

    // expand gov. abbreviation
    n = uName1.find("GOV.");
    if (n != uName1.npos) {
        uName1.replace(n, 4, "GOVERNMENT");
    }

    // now substitute out SCH. for School
    n = uName1.find("SCH.");
    if (n != uName1.npos) {
        uName1.replace(n, 4, "SCHOOL");
    }

    // replace dist. for District
    n = uName1.find("DIST.");
    if (n != uName1.npos) {
        uName1.replace(n, 5, "DISTRICT");
    }

    // replace Gen. with General
    n = uName1.find("GEN.");
    if (n != uName1.npos) {
        uName1.replace(n, 4, "GENERAL");
    }

    // replace Municipal Hospital for District Hospital
    n = uName1.find("MUNICIPALHOSPITAL");
    if (n != uName1.npos) {
        uName1.replace(n, 17, "DISTRICTHOSPITAL");
    }

    // replace General Hospital for District Hospital
    n = uName1.find("GENERALHOSPITAL");
    if (n != uName1.npos) {
        uName1.replace(n, 15, "DISTRICTHOSPITAL");
    }

    // replace Specialist Hospital for Hospital
    n = uName1.find("GOVERNMENTHOSPITAL");
    if (n != uName1.npos) {
        uName1.replace(n, 17, "DISTRICTHOSPITAL");
    }

    // replace Specialist Hospital for Hospital
    n = uName1.find("GOVERNMENTHOSP");
    if (n != uName1.npos) {
        uName1.replace(n, 14, "DISTRICTHOSPITAL");
    }

    // replace Psychiatric Hospital for Hospital
    n = uName1.find("PSYCHIATRICHOSPITAL");
    if (n != uName1.npos) {
        uName1.replace(n, 19, "HOSPITAL");
    }

    // replace Specialist Hospital for Hospital
    n = uName1.find("SPECIALISTHOSPITAL");
    if (n != uName1.npos) {
        uName1.replace(n, 18, "HOSPITAL");
    }

    // replace Metropolitan Hospital for Hospital
    n = uName1.find("METROPOLITANHOSPITAL");
    if (n != uName1.npos) {
        uName1.replace(n, 20, "HOSPITAL");
    }

    // replace District Hospital for Hospital
    n = uName1.find("DISTRICTHOSPITAL");
    if (n != uName1.npos) {
        uName1.replace(n, 16, "HOSPITAL");
    }

    // remove MUNICIPAL
    n = uName1.find("MUNICIPAL");
    if (n != uName1.npos) {
        uName1.replace(n, 9, "");
    }

    // remove MEMORIAL
    n = uName1.find("MEMORIAL");
    if (n != uName1.npos) {
        uName1.replace(n, 8, "");
    }

    // remove MEM.
    n = uName1.find("MEM.");
    if (n != uName1.npos) {
        uName1.replace(n, 4, "");
    }

    // remove MEMORAL
    n = uName1.find("MEMORAL");
    if (n != uName1.npos) {
        uName1.replace(n, 7, "");
    }

    // remove Government
    n = uName1.find("GOVERNMENT");
    if (n != uName1.npos) {
        uName1.replace(n, 10, "");
    }

    // remove Govermnt
    n = uName1.find("GOVERMNT");
    if (n != uName1.npos) {
        uName1.replace(n, 8, "");
    }

    // remove Ltd
    n = uName1.find("LTD");
    if (n != uName1.npos) {
        uName1.replace(n, 3, "");
    }

    // remove Limited
    n = uName1.find("LIMITED");
    if (n != uName1.npos) {
        uName1.replace(n, 7, "");
    }

    // remove General
    n = uName1.find("GENERAL");
    if (n != uName1.npos) {
        uName1.replace(n, 7, "");
    }

    // remove Compound
    n = uName1.find("COMPOUND");
    if (n != uName1.npos) {
        uName1.replace(n, 8, "");
    }

    // remove Comp.
    n = uName1.find("COMP.");
    if (n != uName1.npos) {
        uName1.replace(n, 5, "");
    }

    // remove Comm.
    n = uName1.find("COMM.");
    if (n != uName1.npos) {
        uName1.replace(n, 5, "");
    }

    // remove Community
    n = uName1.find("COMMUNITY");
    if (n != uName1.npos) {
        uName1.replace(n, 9, "");
    }

    // misspelling CINIC for CLINIC
    n = uName1.find("CINIC");
    if (n != uName1.npos) {
        uName1.replace(n, 5, "CLINIC");
    }

    // remove /FP
    n = uName1.find("/FP");
    if (n != uName1.npos) {
        uName1.replace(n, 3, "");
    }

    // remove Rural
    n = uName1.find("RURAL");
    if (n != uName1.npos) {
        uName1.replace(n, 5, "");
    }

    // remove ZONE
    n = uName1.find("ZONE");
    if (n != uName1.npos) {
        uName1.replace(n, 4, "");
    }

    // remove possesives
    n = uName1.find("'S");
    if (n != uName1.npos) {
        uName1.replace(n, 2, "S");
    }

    // remove THE from beginning of string
    if (uName1.substr(0, 3) == "THE") {
        uName1 = uName1.substr(3);
    }

    // remove NEW from beginning of string
    if (uName1.substr(0, 3) == "NEW") {
        uName1 = uName1.substr(3);
    }

    // remove Old from beginning of string
    if (uName1.substr(0, 3) == "Old") {
        uName1 = uName1.substr(3);
    }

    // remove everything between parans - only 1 - remove until end
    n = uName1.find("(");
    if (n != uName1.npos) {
        //std::cout << "inside same name2 found paren =  " << uName1 << "," << n;
        m = uName1.find(")");
        if (m != uName1.npos) {
            uName1 = uName1.substr(0, n) + uName1.substr(m+1);
        }
        else {
            uName1 = uName1.substr(0, n);
        }
        //std::cout << ", after substring =  " << uName1 << '\n';
    }

    if (typeFlag) {

        // Remove facility type and do check for type in code
        // remove Catholic
        n = uName1.find("CATHOLIC");
        if (n != uName1.npos) {
            uName1.replace(n, 8, "");
        }
        // remove Muslim
        n = uName1.find("MUSLIM");
        if (n != uName1.npos) {
            uName1.replace(n, 6, "");
        }
        // remove Presby
        n = uName1.find("PRESBY");
        if (n != uName1.npos) {
            uName1.replace(n, 5, "");
        }
        // remove Anglican
        n = uName1.find("ANGLICAN");
        if (n != uName1.npos) {
            uName1.replace(n, 8, "");
        }
        // remove Mission
        n = uName1.find("MISSION");
        if (n != uName1.npos) {
            uName1.replace(n, 7, "");
        }
        // remove CHPS
        n = uName1.find("CHPS");
        if (n != uName1.npos) {
            uName1.replace(n, 4, "");
        }
        // remove Clinic
        n = uName1.find("CLINIC");
        if (n != uName1.npos) {
            uName1.replace(n, 6, "");
        }
        // remove Comm Cl
        n = uName1.find("COMCL");
        if (n != uName1.npos) {
            uName1.replace(n, 5, "");
        }
        // remove Specialist
        n = uName1.find("SPECIALIST");
        if (n != uName1.npos) {
            uName1.replace(n, 10, "");
        }
        // remove Referral Hospital
        n = uName1.find("REFERRALHOSPITAL");
        if (n != uName1.npos) {
            uName1.replace(n, 16, "");
        }
        // remove Regional Hospital
        n = uName1.find("REGIONALHOSPITAL");
        if (n != uName1.npos) {
            uName1.replace(n, 16, "");
        }
        // remove Hospital
        n = uName1.find("HOSPITAL");
        if (n != uName1.npos) {
            uName1.replace(n, 8, "");
        }
        // remove Gen Hosp
        n = uName1.find("GENHOSP");
        if (n != uName1.npos) {
            uName1.replace(n, 7, "");
        }
        // remove Hosp
        n = uName1.find("HOSP");
        if (n != uName1.npos) {
            uName1.replace(n, 4, "");
        }
        // remove PHC
        n = uName1.find("PHC");
        if (n != uName1.npos) {
            uName1.replace(n, 3, "");
        }
        // remove HC - Health Center
        n = uName1.find("HC");
        if (n != uName1.npos) {
            uName1.replace(n, 2, "");
        }

       
    }


    // remove special characters from strings
    temp = "";
    for (i = 0; i < uName1.size(); ++i) {
        if ((uName1[i] >= '0' && uName1[i] <= '9') || (uName1[i] >= 'A' && uName1[i] <= 'Z')) {
            temp = temp + uName1[i];
        }
    }
    uName1 = temp;

    return uName1;

}

// check to see if two facility names are the same
int sameName2(string name1, string name2) {

    string uName1, uName2;
    string temp;
    int noChanges = 0;

    // if the same - say so
    if (name1 == name2) {
        return noChanges;
    }

    if (name1.length() == 0) {
        return 100;
    }
    if (name2.length() == 0) {
        return 100;
    }

    // check similarity between strings using levenshtein distance algorithm
    // the larger the returned number, the more dissimilar the strings
    //if (uName1.compare(uName2) == 0) {
    noChanges = levenshteinDistance(name1, name2);

    return noChanges;

}


// routine to check levenshtein distance between two strings for string similarity
// the larger the returned number, the more dissimilar the strings
int levenshteinDistance(const std::string& s1, const std::string& s2)
{
    const size_t m(s1.size());
    const size_t n(s2.size());

    if (m == 0) return n;
    if (n == 0) return m;

    size_t* costs = new size_t[n + 1];

    for (size_t k = 0; k <= n; k++) costs[k] = k;

    size_t i = 0;
    for (std::string::const_iterator it1 = s1.begin(); it1 != s1.end(); ++it1, ++i)
    {
        costs[0] = i + 1;
        size_t corner = i;

        size_t j = 0;
        for (std::string::const_iterator it2 = s2.begin(); it2 != s2.end(); ++it2, ++j)
        {
            size_t upper = costs[j + 1];
            if (*it1 == *it2)
            {
                costs[j + 1] = corner;
            }
            else
            {
                size_t t(upper < corner ? upper : corner);
                costs[j + 1] = (costs[j] < t ? costs[j] : t) + 1;
            }

            corner = upper;
        }
    }

    size_t result = costs[n];
    delete[] costs;

    return result;
}


// remove duplicate names with insufficient data
void removeEmptyDuplicates(vector<vector<string>> masterList) {

    ofstream myfile;

    std::size_t i, j;
    int counter = 0;
    string newName;
    std::size_t masterSize = masterList.size();

    struct masterStruct {
        int id;
        int coordFlag;
        int ignoreFlag;
        float longitude;
        float latitude;
        string geometry1;
        string geometry2;
        string name;
        string type;
        string origType;
        string address;
        string town;
        string district;
        string region;
        string matchRegion;
        string geoSource;
        string listSource;
        int priority;
        int code;
    };

    std::cout << "inside remove dup empty Master Size = " << masterSize << '\n';

    vector<masterStruct> masterArray;

    // adjust array index for removed header
    masterSize = masterSize - 1;


    // master has format id, longitude, latitude, geometry(c(x,y)), name, type, original type, address, district,  gps-source, list source, priority, district code
    for (i = 0; i < masterSize; i++) {

        if (false) {
            std::cout << masterList[i + 1][0] << ", " << masterList[i + 1][1] << ", " << masterList[i + 1][2] << ", " << masterList[i + 1][3] << ", " << masterList[i + 1][4] << ", " << masterList[i + 1][5] << ", " << masterList[i + 1][6];
            std::cout << ", " << masterList[i + 1][7] << ", " << masterList[i + 1][8] << ", " << masterList[i + 1][9] << ", " << masterList[i + 1][10] << ", " << masterList[i + 1][11];
            std::cout << ", " << masterList[i + 1][12] << ", " << masterList[i + 1][13] << ", " << masterList[i + 1][14];
            if (masterList[i + 1][4] == "") {
                std::cout << '\n';
            }
            else {
                std::cout << ", " << masterList[i + 1][14] << '\n';
            }
        }

        newName = "masterStruct" + std::to_string(counter++);

        masterStruct newName;


        // convert where neccessary
        newName.id = stoi(masterList[i + 1][0]);
        newName.ignoreFlag = 0;
        newName.coordFlag = stoi(masterList[i + 1][1]);
        if (masterList[i + 1][2] == "") {
            newName.longitude = 100;
            newName.latitude = 100;
        }
        else {

            newName.longitude = stof(masterList[i + 1][2]);
            newName.latitude = stof(masterList[i + 1][3]);
        }
        if (masterList[i + 1][4] == "") {

            // no C(x,y) geometry definition, only read 1 delimited space
            //std::cout << "inside updateMasterList no c(x,y) name = " << masterList[i+1][5] << '\n';

            newName.geometry1 = masterList[i + 1][4];
            newName.geometry2 = "";
            newName.name = masterList[i + 1][5];
            newName.type = masterList[i + 1][6];
            newName.origType = masterList[i + 1][7];
            newName.address = masterList[i + 1][8];
            newName.town = masterList[i + 1][9];
            newName.district = masterList[i + 1][10];
            newName.region = masterList[i + 1][11];
            newName.matchRegion = masterList[i + 1][12];
            newName.geoSource = masterList[i + 1][13];
            newName.listSource = masterList[i + 1][14];
            newName.priority = stoi(masterList[i + 1][15]);
            newName.code = stoi(masterList[i + 1][16]);
        }
        else {

            newName.geometry1 = masterList[i + 1][4];
            newName.geometry2 = masterList[i + 1][5];
            newName.name = masterList[i + 1][6];
            newName.type = masterList[i + 1][7];
            newName.origType = masterList[i + 1][8];
            newName.address = masterList[i + 1][9];
            newName.town = masterList[i + 1][10];
            newName.district = masterList[i + 1][11];
            newName.region = masterList[i + 1][12];
            newName.matchRegion = masterList[i + 1][13];
            newName.geoSource = masterList[i + 1][14];
            newName.listSource = masterList[i + 1][15];
            newName.priority = stoi(masterList[i + 1][16]);
            newName.code = stoi(masterList[i + 1][17]);
        }

        masterArray.push_back(newName);

    }

    masterSize = masterArray.size();

    // output array
    if (false) {
        for (i = 0; i < masterSize; i++) {
            std::cout << "facility name = " << masterArray[i].name << ", id = " << masterArray[i].id << ", x,y = " << masterArray[i].longitude << ", " << masterArray[i].latitude << '\n';
        }
    }

    // check to see if facilities with no coordinates, district or regions match any that do
     // update type with mapping from origType
    // find city closest to each facility with coordinates
    for (i = 0; i < masterSize; i++) {

        // now coord, district or region
        if ((masterArray[i].coordFlag == 0) && (masterArray[i].district == "") && (masterArray[i].region == "")) {

            for (j = 0; j < masterSize; j++) {

                // dow't check against itself
                if (i != j) {

                    if (masterArray[i].name == masterArray[j].name) {

                        masterArray[i].ignoreFlag = 1;
                        //std::cout << "removing = " << masterArray[i].name << "," << masterArray[i].district << "," << masterArray[i].region << '\n';
                    }
                }
            }
        }
    }

    // check to see if facilities with  namee, district and region match but no coordinates, remove one with no coordinates
    // update type with mapping from origType
   // find city closest to each facility with coordinates
    for (i = 0; i < masterSize; i++) {

        if (masterArray[i].ignoreFlag == 0) {

            // now coord, district or region
            if (masterArray[i].coordFlag == 0) {

                for (j = 0; j < masterSize; j++) {

                    // dow't check against itself or facilites already being ignored
                    if ((i != j) && (masterArray[j].ignoreFlag == 0)) {

                        if ((masterArray[i].name == masterArray[j].name) && (masterArray[i].district == masterArray[j].district) && (masterArray[i].region == masterArray[j].region)) {
                            {

                                masterArray[i].ignoreFlag = 1;
                                //std::cout << "removing district/region same = " << masterArray[i].name << "," << masterArray[i].district << "," << masterArray[i].region << '\n';
                            }
                        }
                    }
                }
            }
        }
    }




    myfile.open("./facility-project/MasterFacilityListCitiesCleanDistricts2.csv");

    myfile << "id " << ",coordFlag " << ", longitude " << ",latitude" << ",geometry" << ",name" << ",type" << ", orig-type" << ", address" << ", town" << ",district" << ", region" << ", match-region" << ", gps-source" << ", list-source" << ", priority" << ", district-code" << '\n';

    for (i = 0; i < masterSize; i++) {

        // ignore duplicates
        if (masterArray[i].ignoreFlag == 0) {

            myfile << masterArray[i].id << "," << masterArray[i].coordFlag;
            if (masterArray[i].coordFlag == 1) {
                myfile << "," << masterArray[i].longitude << "," << masterArray[i].latitude;
            }
            else {
                myfile << "," << "" << "," << "";
            }
            myfile << "," << masterArray[i].geometry1;
            if (masterArray[i].geometry2 != "") {
                myfile << "," << masterArray[i].geometry2;
            }
            myfile << "," << masterArray[i].name << "," << masterArray[i].type << "," << masterArray[i].origType << "," << masterArray[i].address << "," << masterArray[i].town << "," << masterArray[i].district << "," << masterArray[i].region;
            myfile << "," << masterArray[i].matchRegion << "," << masterArray[i].geoSource << "," << masterArray[i].listSource << "," << masterArray[i].priority <<"," << masterArray[i].code << '\n';
        }
    }

    myfile.close();
}

    
    
    // update original facility type to recognized tier facility type
void mapTypesInMasterList(vector<vector<string>> masterList, vector<vector<string>> types) {

    ofstream myfile;

    std::size_t i, j, n;
    int counter = 0;
    string newName;
    std::size_t masterSize = masterList.size();
    std::size_t newSize = types.size();

    struct masterStruct {
        int id;
        int coordFlag;
        float longitude;
        float latitude;
        string geometry1;
        string geometry2;
        string name;
        string type;
        string origType;
        string address;
        string town;
        string district;
        string region;
        string geoSource;
        string listSource;
        int priority;
    };

    struct newStruct {
        string origType;
        string mappedType;
    };

    std::cout << "inside map types Master Size = " << masterSize << "," << newSize << '\n';

    vector<masterStruct> masterArray;
    vector<newStruct> newArray;

    // adjust array index for removed header
    masterSize = masterSize - 1;
    newSize = newSize - 1;

    std::cout << "inside map types Master Size = " << masterSize << '\n';


    // master has format id, longitude, latitude, geometry(c(x,y)), name, type, original type, address, district,  gps-source, list source, priority
    for (i = 0; i < masterSize; i++) {

        if (false) {
            std::cout << masterList[i + 1][0] << ", " << masterList[i + 1][1] << ", " << masterList[i + 1][2] << ", " << masterList[i + 1][3] << ", " << masterList[i + 1][4] << ", " << masterList[i + 1][5] << ", " << masterList[i + 1][6];
            std::cout << ", " << masterList[i + 1][7] << ", " << masterList[i + 1][8] << ", " << masterList[i + 1][9] << ", " << masterList[i + 1][10] << ", " << masterList[i + 1][11];
            std::cout << ", " << masterList[i + 1][12] << ", " << masterList[i + 1][13] << ", " << masterList[i + 1][14];
            if (masterList[i + 1][4] == "") {
                std::cout << '\n';
            }
            else {
                std::cout << ", " << masterList[i + 1][14] << '\n';
            }
        }

        newName = "masterStruct" + std::to_string(counter++);

        masterStruct newName;


        // convert where neccessary
        newName.id = stoi(masterList[i + 1][0]);
        newName.coordFlag = stoi(masterList[i + 1][1]);
        if (masterList[i + 1][2] == "") {
            newName.longitude = 100;
            newName.latitude = 100;
        }
        else {

            newName.longitude = stof(masterList[i + 1][2]);
            newName.latitude = stof(masterList[i + 1][3]);
        }
        if (masterList[i + 1][4] == "") {

            // no C(x,y) geometry definition, only read 1 delimited space
            //std::cout << "inside updateMasterList no c(x,y) name = " << masterList[i+1][5] << '\n';

            newName.geometry1 = masterList[i + 1][4];
            newName.geometry2 = "";
            newName.name = masterList[i + 1][5];
            newName.type = masterList[i + 1][6];
            newName.origType = masterList[i + 1][7];
            newName.address = masterList[i + 1][8];
            newName.town = masterList[i + 1][9];
            //newName.district = cleanDistrict(masterList[i + 1][10];
            newName.district = masterList[i + 1][10];
            newName.region = masterList[i + 1][11];
            newName.geoSource = masterList[i + 1][12];
            newName.listSource = masterList[i + 1][13];
            newName.priority = stoi(masterList[i + 1][14]);
        }
        else {

            newName.geometry1 = masterList[i + 1][4];
            newName.geometry2 = masterList[i + 1][5];
            newName.name = masterList[i + 1][6];
            newName.type = masterList[i + 1][7];
            newName.origType = masterList[i + 1][8];
            newName.address = masterList[i + 1][9];
            newName.town = masterList[i + 1][10];
            //newName.district = cleanDistrict(masterList[i + 1][11]);
            newName.district = masterList[i + 1][11];
            newName.region = masterList[i + 1][12];
            newName.geoSource = masterList[i + 1][13];
            newName.listSource = masterList[i + 1][14];
            newName.priority = stoi(masterList[i + 1][15]);
        }

        masterArray.push_back(newName);

    }

    masterSize = masterArray.size();

    // remove any leading spaces from facility names
    const char* letters = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";

    for (i = 0; i < masterSize; i++) {

        // if first letter is something other than a number or letter - remove it
        n = masterArray[i].name.find_first_of(letters);
        if (n == 1) {
            masterArray[i].name = masterArray[i].name.substr(1);
        }
    }


    // output array
    if (false) {
        for (i = 0; i < masterSize; i++) {
            std::cout << "facility name = " << masterArray[i].name << ", id = " << masterArray[i].id << ", x,y = " << masterArray[i].longitude << ", " << masterArray[i].latitude << '\n';
        }
    }

    std::cout << "inside map types newSize = " << newSize << '\n';

    counter = 0;

    // new list has orig-type, mapped-type
    for (i = 0; i < newSize; i++) {

        if (false) {
            std::cout << types[i + 1][0] << ", " << types[i + 1][1] <<  '\n';
        }

        newName = "newStruct" + std::to_string(counter++);

        newStruct newName;

        newName.origType = types[i + 1][0];
        newName.mappedType = types[i + 1][1];

        newArray.push_back(newName);
    }

    newSize = newArray.size();

    std::cout << "inside map type - read in types " << '\n';


    // dump types array
    if (false) {

        for (i = 0; i < newSize; i++) {
            std::cout << "types array - name = " << newArray[i].origType << "," << newArray[i].mappedType << '\n';
        }
    }

    // update type with mapping from origType
    // find city closest to each facility with coordinates
    for (i = 0; i < masterSize; i++) {

        for (j = 0; j < newSize; j++) {

           if (masterArray[i].origType == newArray[j].origType) {

                        masterArray[i].type = newArray[j].mappedType;
                        //std::cout << "types array -found match = " << masterArray[i].type << "," << masterArray[i].origType << '\n';
            }
        }
    }





    myfile.open("./facility-project/MasterFacilityListTypes.csv");

    myfile << "id " << ",coordFlag " << ", longitude " << ",latitude" << ",geometry" << ",name" << ",type" << ", orig-type" << ", address" << ", town" << ",district" << ", region"  << ", gps-source" << ", list-source" << ", priority" << '\n';

    for (i = 0; i < masterSize; i++) {

        myfile << masterArray[i].id << "," << masterArray[i].coordFlag;
        if (masterArray[i].coordFlag == 1) {
            myfile << "," << masterArray[i].longitude << "," << masterArray[i].latitude;
        }
        else {
            myfile << "," << "" << "," << "";
        }
        myfile << "," << masterArray[i].geometry1;
        if (masterArray[i].geometry2 != "") {
            myfile << "," << masterArray[i].geometry2;
        }
        myfile << "," << masterArray[i].name << "," << masterArray[i].type << "," << masterArray[i].origType << "," << masterArray[i].address << "," << masterArray[i].town << "," << masterArray[i].district << "," << masterArray[i].region;
        myfile << "," << masterArray[i].geoSource << "," << masterArray[i].listSource << "," << masterArray[i].priority << '\n';
    }

    myfile.close();
}




// just add cites to list by GPS and name similarities
void cleanTowns( vector<vector<string>> towns, vector<vector<string>> geoTowns) {

    ofstream myfile;

    std::size_t i, j;
    int counter = 0;
    float newDist;
    string newName;
    string geoName;
    std::size_t newSize = towns.size();
    std::size_t geoSize = geoTowns.size();

    struct newStruct {
        bool ignoreFlag;
        string name;
        string name1;
        string name2;
        string name3;
        float longitude;
        float latitude;
        string district;
        string district216;
        string region;
        string correctRegion;
        string altName1;
        string altName2;
        string altName3;
    };

    vector<newStruct> newArray;

    struct geoStruct {
        bool ignoreFlag;
        string name;
        string name1;
        string name2;
        string name3;
        float longitude;
        float latitude;
        string district;
        string region;
        string altName1;
        string altName2;
        string altName3;
    };

    vector<geoStruct> geoArray;

    // adjust array index for removed header
    newSize = newSize - 1;
    geoSize = geoSize - 1;

    std::cout << "inside clean towns newSize = " << newSize << '\n';

    counter = 0;

    // new list has format blank, id, name, name 1st word, name 2nd word, name 3rd word, latitude, longitude, district ,district from 216 shapefile, district id, district name OCHA, 
    // district id OCHA, region, mapped region from 216, district from 260, region 16, region id, type, class, alt. name, etc
    for (i = 0; i < newSize; i++) {

        if (false) {
            std::cout << towns[i + 1][0] << ", " << towns[i + 1][1] << ", " << towns[i + 1][2] << ", " << towns[i + 1][3] << ", " << towns[i + 1][4] << ", " << towns[i + 1][5];
            std::cout << ", " << towns[i + 1][6] << ", " << towns[i + 1][7] << ", " << towns[i + 1][8] << ", " << towns[i + 1][15] << ", " << towns[i + 1][20] << '\n';
        }

        newName = "newStruct" + std::to_string(counter++);

        newStruct newName;

        newName.ignoreFlag = 0;
        newName.name = towns[i + 1][2];
        newName.name1 = towns[i + 1][3];
        newName.name2 = towns[i + 1][4];
        newName.name3 = towns[i + 1][5];
        newName.latitude = stof(towns[i + 1][6]);
        newName.longitude = stof(towns[i + 1][7]);
        // if no district exists from shapefile, use original district
        if (towns[i + 1][15] == "NA") {
            //newName.district = cleanDistrict(towns[i + 1][8]);
            newName.district = "";
        }
        else {
            //newName.district = cleanDistrict(towns[i + 1][15]);
            newName.district = towns[i + 1][15];
        }
        newName.region = towns[i + 1][16];
        newName.altName1 = towns[i + 1][20];
        newName.altName2 = "";
        newName.altName3 = "";

        newArray.push_back(newName);
    }

    newSize = newArray.size();

    std::cout << "inside clean towns - read in towns  " << '\n';


    // dump cities array
    if (false) {

        for (i = 0; i < newSize; i++) {
            std::cout << "towns array - name = " << newArray[i].name << "," << newArray[i].altName1 << "," << newArray[i].latitude << "," << newArray[i].longitude << "," << newArray[i].district << "," << newArray[i].region << '\n';
        }
    }


    std::cout << "inside clean towns geo Size = " << geoSize << '\n';

    counter = 0;

    // geo list has format name, name 1st word, name 2nd word, name 3rd word, latitude, longitude, district 216, region 10, district 260 region 16,  alt.name1, alt.name2, alt.name3....
    for (i = 0; i < geoSize; i++) {

        if (false) {
            std::cout << geoTowns[i + 1][0] << ", " << geoTowns[i + 1][1] << ", " << geoTowns[i + 1][2] << ", " << geoTowns[i + 1][3] << ", " << geoTowns[i + 1][4] << ", " << geoTowns[i + 1][5];
            std::cout << ", " << geoTowns[i + 1][6] << ", " << geoTowns[i + 1][7] << ", " << geoTowns[i + 1][8] << ", " << geoTowns[i + 1][9] << ", " << geoTowns[i + 1][10] << ", " << geoTowns[i + 1][11] << '\n';
        }

        geoName = "geoStruct" + std::to_string(counter++);

        geoStruct geoName;

        geoName.ignoreFlag = 0;
        geoName.name = geoTowns[i + 1][0];
        geoName.name1 = geoTowns[i + 1][1];
        geoName.name2 = geoTowns[i + 1][2];
        geoName.name3 = geoTowns[i + 1][3];
        geoName.latitude = stof(geoTowns[i + 1][4]);
        geoName.longitude = stof(geoTowns[i + 1][5]);
        if (geoTowns[i + 1][8] == "") {
            geoName.district = "";
        }
        else {
            //geoName.district = cleanDistrict(geoTowns[i + 1][8]);
            geoName.district = geoTowns[i + 1][8];
        }
        geoName.region = geoTowns[i + 1][9];
        geoName.altName1 = geoTowns[i + 1][10];
        geoName.altName2 = geoTowns[i + 1][11];
        geoName.altName3 = "";
        //geoName.altName3 = geoTowns[i + 1][12];



        geoArray.push_back(geoName);
    }

    geoSize = geoArray.size();

    std::cout << "inside clean towns - read in geo towns  " << '\n';


    // dump cities array
    if (false) {

        for (i = 0; i < geoSize; i++) {
            std::cout << "towns array - name = " << geoArray[i].name << "," << geoArray[i].altName1 << "," << geoArray[i].latitude << "," << geoArray[i].longitude << "," << geoArray[i].district << "," << geoArray[i].region << '\n';
        }
    }

    // remove duplicate cities with same name and within .5 kilometers
    for (i = 0; i < newSize; i++) {

        for (j = 0; j < geoSize; j++) {

                // same name
                if ((newArray[i].name == geoArray[j].name) || (newArray[i].name == geoArray[j].altName1) || (newArray[i].altName1 == geoArray[j].name)) {

                    // calculate distance between each city and facility and find shortest value
                    newDist = sqrt(pow((newArray[i].longitude - geoArray[j].longitude), 2) + pow((newArray[i].latitude - geoArray[j].latitude), 2));

                    //std::cout << "name = " << newArray[i].name << "," << newArray[i].longitude << "," << geoArray[j].longitude << "," << newArray[i].latitude << "," << geoArray[j].latitude << ", newDist = " << newDist<< '\n';

                    if (newDist <= .05) {

                        geoArray[j].ignoreFlag = 1;
                    }
                }
        }
    }


    myfile.open("./facility-project/newTowns.csv");

    myfile << "id " << ",name " << ",name.first" << ",name.2nd" << ",name.3rd" << ",latitude" << ", longitude " << ",district" << ", region" <<  ", alt.name1" << ",alt.name2" <<  '\n';

    counter = 1;

    for (i = 0; i < newSize; i++) {

            myfile << counter++ << "," << newArray[i].name << "," << newArray[i].name1 << "," << newArray[i].name2 << "," << newArray[i].name3;
            myfile << "," << newArray[i].latitude << "," << newArray[i].longitude << "," << newArray[i].district;
            myfile << "," <<newArray[i].region << "," << newArray[i].altName1 << "," << newArray[i].altName2 << "," << '\n';
    }
    for (i = 0; i < geoSize; i++) {

        if (!geoArray[i].ignoreFlag) {
            myfile << counter++ << "," << geoArray[i].name << "," << geoArray[i].name1 << "," << geoArray[i].name2 << "," << geoArray[i].name3;
            myfile << "," << geoArray[i].latitude << "," << geoArray[i].longitude << "," << geoArray[i].district;
            myfile << "," << geoArray[i].region << "," << geoArray[i].altName1 << "," << geoArray[i].altName2 << "," << '\n';
        }
    }
    myfile.close();






}




// just add cites to list by GPS and name similarities
void addCitiesToMasterList(vector<vector<string>> masterList, vector<vector<string>> cities) {

    ofstream myfile;
    ofstream myfile1;
    ofstream myfile2;

    std::size_t i, j, pos, pos2, pos3;
    int counter = 0;
    int stringSimRating;
    int firstRating;
    int secondRating;
    float newDist;
    float oldDist;
    string newName;
    string firstWord;
    string secondWord;
    bool exactMatch;
    bool updateFlag;
    bool foundFlag;
    std::size_t masterSize = masterList.size();
    std::size_t newSize = cities.size();

    struct masterStruct {
        int id;
        int coordFlag;
        float longitude;
        float latitude;
        string geometry1;
        string geometry2;
        string name;
        string type;
        string origType;
        string address;
        string town;
        string pTown1;
        string pTown2;
        string district;
        string region;
        string matchRegion;
        string geoSource;
        string listSource;
        int priority;
        int code;
    };

    struct newStruct {
        int id;
        float longitude;
        float latitude;
        string name;
        string name1st;
        string name2nd;
        string name3rd;
        string altName1;
        string altName2;
        string district;
        string region;
    };

    std::cout << "inside add cities Master Size = " << masterSize << "," << newSize << '\n';

    vector<masterStruct> masterArray;
    vector<newStruct> newArray;

    std::cout << "inside add cities after newArray " << '\n';

    // adjust array index for removed header
    masterSize = masterSize - 1;
    newSize = newSize - 1;

    std::cout << "inside add cities Master Size = " << masterSize << '\n';


    // master has format id, longitude, latitude, geometry(c(x,y)), name, type, original type, address, district,  gps-source, list source, priority, district code
    for (i = 0; i < masterSize; i++) {

        if (false) {
            std::cout << masterList[i + 1][0] << ", " << masterList[i + 1][1] << ", " << masterList[i + 1][2] << ", " << masterList[i + 1][3] << ", " << masterList[i + 1][4] << ", " << masterList[i + 1][5] << ", " << masterList[i + 1][6];
            std::cout << ", " << masterList[i + 1][7] << ", " << masterList[i + 1][8] << ", " << masterList[i + 1][9] << ", " << masterList[i + 1][10] << ", " << masterList[i + 1][11];
            std::cout << ", " << masterList[i + 1][12] << ", " << masterList[i + 1][13] << ", " << masterList[i + 1][14];
            if (masterList[i + 1][4] == "") {
                std::cout << '\n';
            }
            else {
                std::cout << ", " << masterList[i + 1][14] << '\n';
            }
        }

        newName = "masterStruct" + std::to_string(counter++);

        masterStruct newName;


        // convert where neccessary
        newName.id = stoi(masterList[i + 1][0]);
        newName.coordFlag = stoi(masterList[i + 1][1]);
        if (masterList[i + 1][2] == "") {
            newName.longitude = 100;
            newName.latitude = 100;
        }
        else {

            newName.longitude = stof(masterList[i + 1][2]);
            newName.latitude = stof(masterList[i + 1][3]);
        }
        if (masterList[i + 1][4] == "") {

            // no C(x,y) geometry definition, only read 1 delimited space
            //std::cout << "inside updateMasterList no c(x,y) name = " << masterList[i+1][5] << '\n';

            newName.geometry1 = masterList[i + 1][4];
            newName.geometry2 = "";
            newName.name = masterList[i + 1][5];
            newName.type = masterList[i + 1][6];
            newName.origType = masterList[i + 1][7];
            newName.address = masterList[i + 1][8];
            newName.town = masterList[i + 1][9];
            newName.district = masterList[i + 1][10];
            newName.region = masterList[i + 1][11];
            newName.matchRegion = masterList[i + 1][12];
            newName.geoSource = masterList[i + 1][13];
            newName.listSource = masterList[i + 1][14];
            newName.priority = stoi(masterList[i + 1][15]);
            newName.code = stoi(masterList[i + 1][16]);
        }
        else {

            newName.geometry1 = masterList[i + 1][4];
            newName.geometry2 = masterList[i + 1][5];
            newName.name = masterList[i + 1][6];
            newName.type = masterList[i + 1][7];
            newName.origType = masterList[i + 1][8];
            newName.address = masterList[i + 1][9];
            newName.town = masterList[i + 1][10];
            newName.district = masterList[i + 1][11];
            newName.region = masterList[i + 1][12];
            newName.matchRegion = masterList[i + 1][13];
            newName.geoSource = masterList[i + 1][14];
            newName.listSource = masterList[i + 1][15];
            newName.priority = stoi(masterList[i + 1][16]);
            newName.code = stoi(masterList[i + 1][17]);
        }

        newName.pTown1 = "";
        newName.pTown1 = "";

        masterArray.push_back(newName);

    }

    masterSize = masterArray.size();

    // output array
    if (false) {
        for (i = 0; i < masterSize; i++) {
            std::cout << "facility name = " << masterArray[i].name << ", id = " << masterArray[i].id << ", x,y = " << masterArray[i].longitude << ", " << masterArray[i].latitude << '\n';
        }
    }

    std::cout << "inside add cities newSize = " << newSize << '\n';

    counter = 0;

    // new list has format ID, name, name 1st word, name 2nd word, name 3rd word, latitude, longitude, district ,region , alt.name1, alt.name2
    for (i = 0; i < newSize; i++) {

        if (false) {
            std::cout << cities[i + 1][0] << ", " << cities[i + 1][1] << ", " << cities[i + 1][2] << ", " << cities[i + 1][3] << ", " << cities[i + 1][4] << ", " << cities[i + 1][5];
            std::cout << ", " << cities[i + 1][6] << ", " << cities[i + 1][7] << ", " << cities[i + 1][8] << ", " << cities[i + 1][9] << ", " << cities[i + 1][10] << '\n';
        }

        newName = "newStruct" + std::to_string(counter++);

        newStruct newName;

        newName.id = stoi(cities[i + 1][0]);
        newName.name = cities[i + 1][1];
        newName.name1st = cities[i + 1][2];
        newName.name2nd = cities[i + 1][3];
        newName.name3rd = cities[i + 1][4];
        newName.latitude = stof(cities[i + 1][5]);
        newName.longitude = stof(cities[i + 1][6]);
        //newName.district = cleanDistrict(cities[i + 1][7]);
        newName.district = cities[i + 1][7];
        newName.region = cities[i + 1][8];
        newName.altName1 = cities[i + 1][9];
        newName.altName2 = cities[i + 1][10];

        newArray.push_back(newName);
    }

    newSize = newArray.size();

    std::cout << "inside add cities - read in city list " << '\n';


    // dump cities array
    if (false) {

        for (i = 0; i < newSize; i++) {
            std::cout << "cities array - nmae = " << newArray[i].name << "," << newArray[i].altName1 << "," << newArray[i].latitude << "," << newArray[i].longitude << "," << newArray[i].district << "," << newArray[i].region << '\n';
        }
    }


    myfile1.open("./facility-project/MasterFacilityListFirstWordTownMatch.csv");
    myfile2.open("./facility-project/MasterFacilityListFirstWordTownMismatch.csv");

    myfile1 << "facility " << ", facility town name" << ", firstWord " << ", secondWord " << ", facility region " << ", town region " << ", facility district " << ", town district " << ",updated flag " << '\n';
    myfile2 << "facility " << ", facility town name" << ", firstWord " << ", secondWord " << ", facility region " << ", town region " << ", facility district " << ", town district " << ",updated flag " << '\n';


    //Towns must be within at least 5.5 kilometers
    oldDist = .05;

    // find city closest to each facility with coordinates
    for (i = 0; i < masterSize; i++) {

        if (masterArray[i].coordFlag == 1) {

            // if it already has a town - keep it
            if ((masterArray[i].town == "") || (masterArray[i].town == "NA")) {

                foundFlag = false;

                for (j = 0; j < newSize; j++) {

                    // use first one within 5.5 kilometers
                    if (!foundFlag) {

                        // calculate distance between each city and facility and find shortest value
                        newDist = sqrt(pow((masterArray[i].longitude - newArray[j].longitude), 2) + pow((masterArray[i].latitude - newArray[j].latitude), 2));

                        if (newDist < oldDist) {

                            masterArray[i].town = newArray[j].name;

                            // if no district assigned, use the one from the town
                            if ((masterArray[i].district == "") || (masterArray[i].district == "NA")) {

                                masterArray[i].district = newArray[j].district;
                                masterArray[i].matchRegion = newArray[j].region;
                            }

                            foundFlag = true;
                        }
                    }
                }
            }

        }
        else {

            // pull off first word and compare with town names to see if you have a match

            // pull of first word in name and see if it matches any town name
            pos = masterArray[i].name.find(" ");
            firstWord = masterArray[i].name.substr(0, pos);

            // go to the second word in the name
            pos2 = masterArray[i].name.find(" ", pos + 1);
            secondWord = masterArray[i].name.substr(pos + 1, (pos2-(pos + 1)));


            // ignore "The", "La", "Le", "Old" and "New" as first words
            if (firstWord == "The") {

                // Swap 2nd and first words
                firstWord = secondWord;

                // go to the second word in the name
                pos3 = masterArray[i].name.find(" ", pos2 + 1);
                secondWord = masterArray[i].name.substr(pos2 + 1, (pos3 - (pos2 + 1)));
            }
            if (firstWord == "La") {

                // Swap 2nd and first words
                firstWord = secondWord;
                
                // go to the second word in the name
                pos3 = masterArray[i].name.find(" ", pos2 + 1);
                secondWord = masterArray[i].name.substr(pos2 + 1, (pos3 - (pos2 + 1)));
            }
            if (firstWord == "Le") {

                // Swap 2nd and first words
                firstWord = secondWord;
                
                // go to the second word in the name
                pos3 = masterArray[i].name.find(" ", pos2 + 1);
                secondWord = masterArray[i].name.substr(pos2 + 1, (pos3 - (pos2 + 1)));
            }
            if (firstWord == "New") {

                // Swap 2nd and first words
                firstWord = secondWord;
                
                // go to the second word in the name
                pos3 = masterArray[i].name.find(" ", pos2 + 1);
                secondWord = masterArray[i].name.substr(pos2 + 1, (pos3 - (pos2 + 1)));
            }
            if (firstWord == "Old") {

                // go to the second word in the name
                pos3 = masterArray[i].name.find(" ", pos2 + 1);
                secondWord = masterArray[i].name.substr(pos2 + 1, (pos3 - (pos2 + 1)));
            }

            exactMatch = false;
            updateFlag = false;
            secondRating = 10;
            firstRating = 10;
            
            for (j = 0; j < newSize; j++) {

                //updateFlag = false;
                // only update once
                if (!updateFlag) {

                    //std::cout << "inside add cities - no gps name comparison facility= " << masterArray[i].name << ", town name = " << newArray[j].name << ", firstWord = " << firstWord << ", updateFlag =" << updateFlag << '\n';

                    // if no region or regions match possible update 
                    if ((masterArray[i].region == "") || (masterArray[i].region == newArray[j].region)) {

                        // districts match - update - skip district match
                        //if ((masterArray[i].district == "") || (masterArray[i].district == newArray[j].district)) {

                            // check for an exact match with first word of town name
                            if (firstWord == newArray[j].name1st) {
                                //if (sameName2(firstWord, newArray[j].name1st) < 2) {

                                    // now check to see if there is a second word to either name
                                if ((secondWord == newArray[j].name2nd) || (newArray[j].name2nd == "")) {
                                    //if ((sameName2(secondWord, newArray[j].name2nd) < 2) || (newArray[j].name2nd == "")) {

                                    if (newArray[j].name2nd != "") {

                                        // only set if two word match
                                        updateFlag = true;
                                    }


                                    // update town name and gps coordinates
                                    masterArray[i].town = newArray[j].name;
                                    masterArray[i].longitude = newArray[j].longitude;
                                    masterArray[i].latitude = newArray[j].latitude;

                                    // town districts/regions correct in towns file - overwrite what's there
                                    masterArray[i].district = newArray[j].district;
                                    masterArray[i].matchRegion = newArray[j].region;
                                    masterArray[i].coordFlag = 1;
                                    masterArray[i].geoSource = "town";

                                    exactMatch = true;

                                    myfile1 << masterArray[i].name << "," << newArray[j].name << ", " << firstWord << "," << secondWord << "," << masterArray[i].region << "," << newArray[j].region << ", " << masterArray[i].district << "," << newArray[j].district << "," << updateFlag << '\n';

                                    //std::cout << "inside add cities - no gps name comparison facility= " << masterArray[i].name << ", town name = " << newArray[j].name << ", firstWord = " << firstWord << ", region = " << masterArray[i].region;
                                    //std::cout << ", district = " << masterArray[i].district << ",updated flag = " << updateFlag << '\n';
                                    //std::cout << "inside add cities - no gps name comparison facility= " << masterArray[i].name << ", town name = " << newArray[j].name << ", firstWord = " << firstWord;
                                    //std::cout << ", longitude = " << newArray[j].longitude << ", latitude = " << newArray[j].latitude << '\n';
                                }
                            }

                            // If couldn't find an exact match - check alternate town names
                            if (!exactMatch) {

                                if ((firstWord == newArray[j].altName1) || (firstWord == newArray[j].altName2)) {

                                    // update town name and gps coordinates
                                    masterArray[i].town = newArray[j].name;
                                    masterArray[i].longitude = newArray[j].longitude;
                                    masterArray[i].latitude = newArray[j].latitude;
                                    masterArray[i].district = newArray[j].district;
                                    masterArray[i].matchRegion = newArray[j].region;
                                    masterArray[i].coordFlag = 1;
                                    masterArray[i].geoSource = "town";

                                    //std::cout << "inside add cities - no gps name comparison alternate city name facility= " << masterArray[i].name << ", town name = " << newArray[j].name << ", firstWord = " << firstWord;
                                    //std::cout << ", longitude = " << newArray[j].longitude << ", latitude = " << newArray[j].latitude << '\n';
                                }

                            }

                            // If couldn't find an exact match - try string similarity
                            if (!exactMatch) {

                                stringSimRating = sameName2(firstWord, newArray[j].name1st);
                                
                                if (stringSimRating < 4) {

                                    // check to see if it is lower than either of the two we already have
                                    if (stringSimRating < firstRating) {

                                        // update possible town name
                                        masterArray[i].pTown1 = newArray[j].name;

                                        // move first to second if second is higher
                                        if (firstRating < secondRating) {

                                            masterArray[i].pTown2 = masterArray[i].pTown1;
                                            secondRating = firstRating;
                                        }
                                        firstRating = stringSimRating;
                                    }
                                    else if(stringSimRating < secondRating) {

                                        // update other possible town name
                                        masterArray[i].pTown2 = newArray[j].name;
                                        secondRating = stringSimRating;
                                    }

                                    //std::cout << "inside add cities - no gps name comparison alternate city name facility= " << masterArray[i].name << ", town name = " << newArray[j].name << ", firstWord = " << firstWord;
                                    //std::cout << ", longitude = " << newArray[j].longitude << ", latitude = " << newArray[j].latitude << '\n';
                                }

                            }
                        //}
                    }

                }      // updateFlag == false
            }
        }
    }
    myfile1.close();
    myfile2.close();






    myfile.open("./facility-project/MasterFacilityListCities.csv");

    myfile << "id " << ",coordFlag " << ", longitude " << ",latitude" << ",geometry" << ",name" << ",type" << ", orig-type" << ", address" << ", town" << ",district" << ", region" << ", match-region" << ", gps-source" << ", list-source"  << ",ptown1" << ", ptown2" << ", priority" << ", district-code" << '\n';

    for (i = 0; i < masterSize; i++) {

        myfile << masterArray[i].id << "," << masterArray[i].coordFlag;
        if (masterArray[i].coordFlag == 1) {
            myfile << "," << masterArray[i].longitude << "," << masterArray[i].latitude;
        }
        else {
            myfile << "," << "" << "," << "";
        }
        myfile << "," << masterArray[i].geometry1;
        if (masterArray[i].geometry2 != "") {
            myfile << "," << masterArray[i].geometry2;
        }
        myfile << "," << masterArray[i].name << "," << masterArray[i].type << "," << masterArray[i].origType << "," << masterArray[i].address << "," << masterArray[i].town;
        myfile << "," << masterArray[i].district << "," << masterArray[i].region << "," << masterArray[i].matchRegion << "," << masterArray[i].geoSource;
        myfile << "," << masterArray[i].listSource  << "," << masterArray[i].pTown1 << "," << masterArray[i].pTown2 << "," << masterArray[i].priority << "," << masterArray[i].code <<'\n';
    }

    myfile.close();
}



// map the districts to the 260 districts
void mapDistrictsRegionsTo260Sixteen(vector<vector<string>> masterList, vector<vector<string>> districts, vector<vector<string>> regions, vector<vector<string>> districtRegion) {

    ofstream myfile;

    std::size_t i, j, k;
    int counter = 0;
    string newName;
    string regName;
    string distRegName;
    std::size_t masterSize = masterList.size();
    std::size_t newSize = districts.size();
    std::size_t regSize = regions.size();
    std::size_t distRegSize = districtRegion.size();

    struct masterStruct {
        int id;
        int coordFlag;
        float longitude;
        float latitude;
        string geometry1;
        string geometry2;
        string name;
        string type;
        string origType;
        string address;
        string town;
        string district;
        string region;
        string matchRegion;
        string geoSource;
        string listSource;
        int priority;
        int code;
    };

    struct newStruct {
        string oldDistrict;
        string newDistrict;
        string region;
    };

    struct regStruct {
        string oldRegion;
        string newRegion;
    };


    struct distRegStruct {
        string district;
        string region;
        int code;
    };


    std::cout << "inside map districts Master Size = " << masterSize << "," << newSize << '\n';

    vector<masterStruct> masterArray;
    vector<newStruct> newArray;
    vector<regStruct> regArray;
    vector<distRegStruct> distRegArray;

    std::cout << "inside map districts after newArray " << '\n';

    // adjust array index for removed header
    masterSize = masterSize - 1;
    newSize = newSize - 1;
    regSize = regSize - 1;
    distRegSize = distRegSize - 1;

    std::cout << "inside map districts Master Size = " << masterSize << '\n';


    // master has format id, longitude, latitude, geometry(c(x,y)), name, type, original type, address, district, region, gps-source, list source, priority
    for (i = 0; i < masterSize; i++) {

        if (false) {
            std::cout << masterList[i + 1][0] << ", " << masterList[i + 1][1] << ", " << masterList[i + 1][2] << ", " << masterList[i + 1][3] << ", " << masterList[i + 1][4] << ", " << masterList[i + 1][5] << ", " << masterList[i + 1][6];
            std::cout << ", " << masterList[i + 1][7] << ", " << masterList[i + 1][8] << ", " << masterList[i + 1][9] << ", " << masterList[i + 1][10] << ", " << masterList[i + 1][11];
            std::cout << ", " << masterList[i + 1][12] << ", " << masterList[i + 1][13] << ", " << masterList[i + 1][14];
            if (masterList[i + 1][4] == "") {
                std::cout << '\n';
            }
            else {
                std::cout << ", " << masterList[i + 1][14] << '\n';
            }
        }

        newName = "masterStruct" + std::to_string(counter++);

        masterStruct newName;


        // convert where neccessary
        newName.id = stoi(masterList[i + 1][0]);
        newName.coordFlag = stoi(masterList[i + 1][1]);
        if (masterList[i + 1][2] == "") {
            newName.longitude = 100;
            newName.latitude = 100;
        }
        else {

            newName.longitude = stof(masterList[i + 1][2]);
            newName.latitude = stof(masterList[i + 1][3]);
        }
        if (masterList[i + 1][4] == "") {

            // no C(x,y) geometry definition, only read 1 delimited space
            //std::cout << "inside updateMasterList no c(x,y) name = " << masterList[i+1][5] << '\n';

            newName.geometry1 = masterList[i + 1][4];
            newName.geometry2 = "";
            newName.name = masterList[i + 1][5];
            newName.type = masterList[i + 1][6];
            newName.origType = masterList[i + 1][7];
            newName.address = masterList[i + 1][8];
            newName.town = masterList[i + 1][9];
            newName.district = masterList[i + 1][10];
            newName.region = masterList[i + 1][11];
            newName.matchRegion = "";
            newName.geoSource = masterList[i + 1][12];
            newName.listSource = masterList[i + 1][13];
            newName.priority = stoi(masterList[i + 1][14]);
        }
        else {

            newName.geometry1 = masterList[i + 1][4];
            newName.geometry2 = masterList[i + 1][5];
            newName.name = masterList[i + 1][6];
            newName.type = masterList[i + 1][7];
            newName.origType = masterList[i + 1][8];
            newName.address = masterList[i + 1][9];
            newName.town = masterList[i + 1][10];
            newName.district = masterList[i + 1][11];
            newName.region = masterList[i + 1][12];
            newName.matchRegion = "";
            newName.geoSource = masterList[i + 1][13];
            newName.listSource = masterList[i + 1][14];
            newName.priority = stoi(masterList[i + 1][15]);
        }

        // default code
        newName.code = 0;

        masterArray.push_back(newName);

    }

    masterSize = masterArray.size();


    // output array
    if (false) {
        for (i = 0; i < masterSize; i++) {
            std::cout << "facility name = " << masterArray[i].name << ", id = " << masterArray[i].id << ", x,y = " << masterArray[i].longitude << ", " << masterArray[i].latitude << '\n';
        }
    }

    std::cout << "inside clean districts newSize = " << newSize << '\n';

    counter = 0;

    // new list has format old district, 216 district, region, 260 district, region, 260 district - pretty, 216 district code, dummy
    for (i = 0; i < newSize; i++) {

        if (false) {
            std::cout << districts[i + 1][0] << ", " << districts[i + 1][1] << '\n';
        }

        newName = "newStruct" + std::to_string(counter++);

        newStruct newName;

        newName.oldDistrict = districts[i + 1][0];
        // removed clean district call
        newName.newDistrict = districts[i + 1][3];
        newName.region = districts[i + 1][4];

        newArray.push_back(newName);
    }

    newSize = newArray.size();

    std::cout << "inside clean districts - read in district mapping list " << '\n';

    counter = 0;

    // reg list has format 16 regions, 10 regions, 16 regions dummy
    for (i = 0; i < regSize; i++) {

        if (false) {
            std::cout << regions[i + 1][0] << ", " << regions[i + 1][1] << ", " << regions[i + 1][2] << '\n';
        }

        regName = "regStruct" + std::to_string(counter++);

        regStruct regName;

        regName.oldRegion = regions[i + 1][0];
        regName.newRegion = regions[i + 1][2];

        regArray.push_back(regName);
    }

    regSize = regArray.size();

    std::cout << "inside clean districts - read in region mapping list " << '\n';

    counter = 0;

    // district region list has format district, region, district without dash, dummy
    // district region list has format region, district from shapefile, district pretty,  district without dash, district without "Municipal",  area, area KM2, population, status,  district code216, district code260, dummy
    for (i = 0; i < distRegSize; i++) {

        if (false) {
            std::cout << districtRegion[i + 1][0] << ", " << districtRegion[i + 1][1] << ", " << districtRegion[i + 1][10] << '\n';
        }

        distRegName = "distRegStruct" + std::to_string(counter++);

        distRegStruct distRegName;

        distRegName.region = districtRegion[i + 1][0];
        distRegName.district = districtRegion[i + 1][1];
        distRegName.code = stoi(districtRegion[i + 1][10]);

        distRegArray.push_back(distRegName);
    }

    distRegSize = distRegArray.size();

    std::cout << "inside clean districts - read in district/region mapping list " << '\n';





    // Map current district to 216 district configuration
    for (i = 0; i < masterSize; i++) {

        // only map existing districts
        if (masterArray[i].district != "") {

            for (j = 0; j < newSize; j++) {

                if (masterArray[i].district == newArray[j].oldDistrict) {

                    // use mapped district and it's associated region
                    masterArray[i].district = newArray[j].newDistrict;
                    masterArray[i].region = newArray[j].region;
                }
            }
        }
        // only map existing regions
        if (masterArray[i].region != "") {

            for (k = 0; k < regSize; k++) {

                if (masterArray[i].region == regArray[k].oldRegion) {

                    // use mapped region
                    masterArray[i].region = regArray[k].newRegion;
                }
            }
        }
    }

    // now check to make sure all the districts and regions line up
    for (i = 0; i < masterSize; i++) {

        // only map existing districts
        if (masterArray[i].district != "") {

            for (j = 0; j < distRegSize; j++) {

                if (masterArray[i].district == distRegArray[j].district) {

                    // use mapped region
                    masterArray[i].region = distRegArray[j].region;
                    masterArray[i].code = distRegArray[j].code;

                }
            }
        }
    }




    myfile.open("./facility-project/MasterFacilityListCitiesCleanDistricts.csv");

    myfile << "id " << ",coordFlag " << ", longitude " << ",latitude" << ",geometry" << ",name" << ",type" << ", orig-type" << ", address" << ", town" << ",district" << ", region" << ", match-region" << ", gps-source" << ", list-source" << ", priority" << ",distict-code" << '\n';

    for (i = 0; i < masterSize; i++) {

        myfile << masterArray[i].id << "," << masterArray[i].coordFlag;
        if (masterArray[i].coordFlag == 1) {
            myfile << "," << masterArray[i].longitude << "," << masterArray[i].latitude;
        }
        else {
            myfile << "," << "" << "," << "";
        }
        myfile << "," << masterArray[i].geometry1;
        if (masterArray[i].geometry2 != "") {
            myfile << "," << masterArray[i].geometry2;
        }
        myfile << "," << masterArray[i].name << "," << masterArray[i].type << "," << masterArray[i].origType << "," << masterArray[i].address << "," << masterArray[i].town << "," << masterArray[i].district << "," << masterArray[i].region;
        myfile << "," << masterArray[i].matchRegion << "," << masterArray[i].geoSource << "," << masterArray[i].listSource << "," << masterArray[i].priority << "," << masterArray[i].code << '\n';
    }

    myfile.close();
}





// just add lists together
void cullMasterList(vector<vector<string>> masterList, vector<vector<string>> regionsDistricts) {

    ofstream myfile;
    ofstream myfile1;
    ofstream myfile2;
    ofstream myfile3;
    ofstream myfile4;
    ofstream myfile5;
    ofstream myfile6;
    ofstream myfile7;

    float dist;
    int count = 0;
    int newID = 1;
    int genericStatus;
    int result;
    int keepIndex;
    int loseIndex;
    string newName;
    string rdName;
    string temp;
    string sameGPS;
    string sameNames;
    string dirName;
    string baseDirName;
    string fileName;
    string reason;
    //char* dirName;
    std::size_t i, j;
    std::size_t masterSize = masterList.size();
    std::size_t rdSize = regionsDistricts.size();

    struct masterStruct {
        int id, genericName;
        int ignoreFlag;
        int coordFlag;
        float longitude;
        float latitude;
        string geometry1;
        string geometry2;
        string name;
        string convertedName;
        string type;
        string origType;
        string address;
        string town;
        string pTown1;
        string pTown2;
        string district;
        string region;
        string matchRegion;
        string geoSource;
        string listSource;
        int priority;
        int code;
        string GPS;
        string Names;
        int outputFlag;
        string reason;

    };

    struct rdStruct {
        string region;
        string district;
    };


    //struct masterStruct masterArray[masterSize];
    vector<masterStruct> masterArray;
    vector<rdStruct> rdArray;

    // adjust array index for removed header
    masterSize = masterSize - 1;
    rdSize = rdSize - 1;

    std::cout << "inside culleMasterList Master Size = " << masterSize << '\n';

    // master has format id, longitude, latitude, geometry(c(x,y)), name, type, original type, address, district,  region, match-region, gps-source, list source, priority
    for (i = 0; i < masterSize; i++) {

        if (false) {
            std::cout << masterList[i + 1][0] << ", " << masterList[i + 1][1] << ", " << masterList[i + 1][2] << ", " << masterList[i + 1][3] << ", " << masterList[i + 1][4] << ", " << masterList[i + 1][5] << ", " << masterList[i + 1][6];
            std::cout << ", " << masterList[i + 1][7] << ", " << masterList[i + 1][8] << ", " << masterList[i + 1][9] << ", " << masterList[i + 1][10] << ", " << masterList[i + 1][11];
            std::cout << ", " << masterList[i + 1][12] << ", " << masterList[i + 1][13] << ", " << masterList[i + 1][14] << ", " << masterList[i + 1][15];
            if (masterList[i + 1][4] == "") {
                std::cout << '\n';
            }
            else {
                std::cout << ", " << masterList[i + 1][15] << '\n';
            }
        }

        newName = "masterSruct" + std::to_string(count++);

        masterStruct newName;


        // convert where neccessary
        //newName.id = i;
        newName.id = stoi(masterList[i + 1][0]);
        newName.coordFlag = stoi(masterList[i + 1][1]);
        newName.ignoreFlag = 0;
        if (masterList[i + 1][2] == "") {
            newName.longitude = 100;
            newName.latitude = 100;
        }
        else {

            newName.longitude = stof(masterList[i + 1][2]);
            newName.latitude = stof(masterList[i + 1][3]);
        }
        if (masterList[i + 1][4] == "") {

            // no C(x,y) geometry definition, only read 1 delimited space
            //std::cout << "inside updateMasterList no c(x,y) name = " << masterList[i+1][5] << '\n';

            newName.geometry1 = masterList[i + 1][4];
            newName.geometry2 = "";
            newName.name = masterList[i + 1][5];
            newName.type = masterList[i + 1][6];
            newName.origType = masterList[i + 1][7];
            newName.address = masterList[i + 1][8];
            newName.town = masterList[i + 1][9];
            newName.district = masterList[i + 1][10];
            newName.region = masterList[i + 1][11];
            newName.matchRegion = masterList[i + 1][12];
            newName.geoSource = masterList[i + 1][13];
            newName.listSource = masterList[i + 1][14];
            newName.pTown1 = masterList[i + 1][15];
            newName.pTown2 = masterList[i + 1][16];
            newName.priority = stoi(masterList[i + 1][17]);
            newName.code = stoi(masterList[i + 1][18]);
        }
        else {

            newName.geometry1 = masterList[i + 1][4];
            newName.geometry2 = masterList[i + 1][5];
            newName.name = masterList[i + 1][6];
            newName.type = masterList[i + 1][7];
            newName.origType = masterList[i + 1][8];
            newName.address = masterList[i + 1][9];
            newName.town = masterList[i + 1][10];
            newName.district = masterList[i + 1][11];
            newName.region = masterList[i + 1][12];
            newName.matchRegion = masterList[i + 1][13];
            newName.geoSource = masterList[i + 1][14];
            newName.listSource = masterList[i + 1][15];
            newName.pTown1 = masterList[i + 1][16];
            newName.pTown2 = masterList[i + 1][17];
            newName.priority = stoi(masterList[i + 1][18]);
            newName.code = stoi(masterList[i + 1][19]);
        }

        // convert facility name to cleaner version
        newName.convertedName = convertName(newName.name, true); 

        // flag to note when written to output file
        newName.outputFlag = 0;

        // reset reason for ignoring facility
        newName.reason = "";

        // if no value in matchRegion - copy from region
        if (newName.matchRegion == "") {
            newName.matchRegion = newName.region;
        }

        masterArray.push_back(newName);
    }

    masterSize = masterArray.size();


    // output array
    if (false) {
        for (i = 0; i < masterSize; i++) {
            std::cout << "facility name = " << masterArray[i].name << ", id = " << masterArray[i].id << ", x,y = " << masterArray[i].longitude << ", " << masterArray[i].latitude << '\n';
        }
    }

    std::cout << "inside culleMasterList before different regions same coordinate " <<  '\n';

    count = 0;

    // district region list has format  region, district from shapefile, district-pretty,  district without dash, district without municipal,  area(sq ft), population, status, district code215, district code260, dummy
    for (i = 0; i < rdSize; i++) {

        if (false) {
            std::cout << regionsDistricts[i + 1][0] << ", " << regionsDistricts[i + 1][1] << ", " << regionsDistricts[i + 1][2] << '\n';
        }

        rdName = "regDistStruct" + std::to_string(count++);

        rdStruct rdName;

        rdName.region = regionsDistricts[i + 1][0];
        rdName.district = regionsDistricts[i + 1][1];
        

        rdArray.push_back(rdName);
    }

    rdSize = rdArray.size();

    std::cout << "inside Cull list - read in district/region mapping list " << '\n';



    // output all facilities that have different regions
    myfile2.open("./facility-project/MasterFacilityListReducedDiffRegions.csv");

    myfile2 << "id " << ",coordFlag " << ", longitude " << ",latitude" << ",geometry" << ",name" << ",type" << ", orig-type" << ", address" << ", town" << ",district" << ", region" << ", match-region" << ", gps-source" << ", list-source" << '\n';

    for (i = 0; i < masterSize; i++) {

        // different regions
        if (masterArray[i].region != "") {

            if (masterArray[i].region != masterArray[i].matchRegion) {

                myfile2 << masterArray[i].id << "," << masterArray[i].coordFlag;
                if (masterArray[i].coordFlag == 1) {
                    myfile2 << "," << masterArray[i].longitude << "," << masterArray[i].latitude;
                }
                else {
                    myfile2 << "," << "" << "," << "";
                }
                myfile2 << "," << masterArray[i].geometry1;
                if (masterArray[i].geometry2 != "") {
                    myfile2 << "," << masterArray[i].geometry2;
                }
                myfile2 << "," << masterArray[i].name << "," << masterArray[i].type << "," << masterArray[i].origType << "," << masterArray[i].address << "," << masterArray[i].town << "," << masterArray[i].district << "," << masterArray[i].region;
                myfile2 << "," << masterArray[i].matchRegion << "," << masterArray[i].geoSource << "," << masterArray[i].listSource << '\n';
            }
        }
    }

    myfile2.close();


    std::cout << "inside culleMasterList before removing pharmacies, dentists, [Non-Facility]" << '\n';

    // output all facilities that have different regions
    myfile6.open("./facility-project/MasterFacilityListReducedPharmacy.csv");

    myfile6 << "id " << ",coordFlag " << ", longitude " << ",latitude" << ",geometry" << ",name" << ",type" << ", orig-type" << ", address" << ", town" << ",district" << ", region" << ", match-region" << ", gps-source" << ", list-source" << '\n';

    for (i = 0; i < masterSize; i++) {

        // pharmacies or dentist - ignore
        if ((masterArray[i].origType == "pharmacy") || (masterArray[i].origType == "dentist")) {

            // ignore this facility
            masterArray[i].ignoreFlag = 1;
            masterArray[i].reason = "Wrong type - " + masterArray[i].origType;

            myfile6 << masterArray[i].id << "," << masterArray[i].coordFlag;
            if (masterArray[i].coordFlag == 1) {
                myfile6 << "," << masterArray[i].longitude << "," << masterArray[i].latitude;
            }
            else {
                myfile6 << "," << "" << "," << "";
            }
            myfile6 << "," << masterArray[i].geometry1;
            if (masterArray[i].geometry2 != "") {
                myfile6 << "," << masterArray[i].geometry2;
            }
            myfile6 << "," << masterArray[i].name << "," << masterArray[i].type << "," << masterArray[i].origType << "," << masterArray[i].address << "," << masterArray[i].town << "," << masterArray[i].district << "," << masterArray[i].region;
            myfile6 << "," << masterArray[i].matchRegion << "," << masterArray[i].geoSource << "," << masterArray[i].listSource << '\n';

        }
        if (masterArray[i].type == "[Non-Facility]") {

            // ignore this facility
            masterArray[i].ignoreFlag = 1;
            masterArray[i].reason = "Wrong type - " + masterArray[i].origType;

            myfile6 << masterArray[i].id << "," << masterArray[i].coordFlag;
            if (masterArray[i].coordFlag == 1) {
                myfile6 << "," << masterArray[i].longitude << "," << masterArray[i].latitude;
            }
            else {
                myfile6 << "," << "" << "," << "";
            }
            myfile6 << "," << masterArray[i].geometry1;
            if (masterArray[i].geometry2 != "") {
                myfile6 << "," << masterArray[i].geometry2;
            }
            myfile6 << "," << masterArray[i].name << "," << masterArray[i].type << "," << masterArray[i].origType << "," << masterArray[i].address << "," << masterArray[i].town << "," << masterArray[i].district << "," << masterArray[i].region;
            myfile6 << "," << masterArray[i].matchRegion << "," << masterArray[i].geoSource << "," << masterArray[i].listSource << '\n';

        }
    }

    myfile6.close();


    std::cout << "inside culleMasterList before gps check " << '\n';

    // output all facilities that have same coordinates but different names
    myfile3.open("./facility-project/MasterFacilityListSameGPSDiffNames.csv");

    myfile3 << "name 1" << ",converted-name 1" << ",longitude 1" << ",latitude 1" << ",source 1" << ",name 2" << ",converted-name 2" << ",longitude 2" << ",latitude 2" << ",source 2" << '\n';


    // first find coordinates withing 11 meters of each other and output them to a file
    for (i = 0; i < masterSize; i++) {

        // if ignore flag already triggered - ignore this facilty for comparison purposes
        if (masterArray[i].ignoreFlag == 0) {

            // only check facilities with coordinates
            if (masterArray[i].coordFlag > 0) {

                for (j = 0; j < masterSize; j++) {

                    // don't compare aginst itself
                    if (masterArray[i].id != masterArray[j].id) {

                        // against other facilities with coordinates
                        if (masterArray[j].coordFlag > 0) {

                            if (masterArray[j].ignoreFlag == 0) {

                                dist = sqrt(pow((masterArray[i].longitude - masterArray[j].longitude), 2) + pow((masterArray[i].latitude - masterArray[j].latitude), 2));

                                // 111 meters
                                if (dist <= .001) {

                                    // within 111 meters - see if either has a generic name - if so, just assume they are the same
                                    genericStatus = checkGenericNamePair(masterArray[i].name, masterArray[j].name);

                                    if (genericStatus == 1) {

                                        // masterArray[i].name generic - assume both locations are the same and copy masterArray[j].name to masterArray[i].name
                                        masterArray[j].ignoreFlag = 1;
                                        masterArray[i].name = masterArray[j].name;
                                        masterArray[i].convertedName = masterArray[j].convertedName;
                                        masterArray[j].reason = "Same GPS - generic name - id = " + to_string(masterArray[i].id);

                                    }
                                    else if (genericStatus == 2) {

                                        // masterArray[j].name generic - assume both locations are the same
                                        masterArray[j].ignoreFlag = 1;
                                        masterArray[j].reason = "Same GPS - generic name - id = " + to_string(masterArray[i].id);
                                    }
                                    else {

                                        // check to see if names are the same
                                        if (sameName2(masterArray[i].convertedName, masterArray[j].convertedName) < 4) {

                                            // must be same type of facility
                                            if ((typeCheck(masterArray[i].type, masterArray[j].type)) || (masterArray[i].type == "") || (masterArray[j].type == "") || (masterArray[i].type == "Undetermined") || (masterArray[j].type == "Undetermined")) {

                                                // keep one with lower priority
                                                if (masterArray[i].priority <= masterArray[j].priority) {

                                                    keepIndex = i;
                                                    loseIndex = j;
                                                }
                                                else {

                                                    keepIndex = j;
                                                    loseIndex = i;
                                                }

                                                // same name - remove facility
                                                masterArray[loseIndex].ignoreFlag = 1;

                                                // if keep facility type is undetermined or undefined, copy the other type
                                                if (masterArray[keepIndex].type == "Undetermined") {
                                                    masterArray[keepIndex].type = masterArray[loseIndex].type;
                                                    masterArray[loseIndex].reason = "Same GPS - Undetermined type - id = " + to_string(masterArray[i].id);
                                                }
                                                else if (masterArray[keepIndex].type == "") {
                                                    masterArray[keepIndex].type = masterArray[loseIndex].type;
                                                    masterArray[loseIndex].reason = "Same GPS - Undefined type - id = " + to_string(masterArray[i].id);
                                                }
                                                else {
                                                    masterArray[loseIndex].reason = "Same GPS - type same - id = " + to_string(masterArray[i].id);
                                                }
                                            }
                                        }
                                        else {

                                            //std::cout << "coodinates the same - names different = " << masterArray[i].name << ",  " << masterArray[j].name << '\n';

                                            // remove newline character from listSource to make it work in the .csv file
                                            temp = masterArray[i].listSource;
                                            if (!temp.empty()) {
                                                temp.erase(temp.size() - 1);
                                            }

                                            myfile3 << masterArray[i].name << "," << masterArray[i].convertedName << "," << masterArray[i].longitude << "," << masterArray[i].latitude << "," << temp << ",";
                                            myfile3 << masterArray[j].name << "," << masterArray[j].convertedName << "," << masterArray[j].longitude << "," << masterArray[j].latitude << "," << masterArray[j].listSource << '\n';
                                        }
                                    }
                                }
                            }
                        }
                        else {

                            // check facility with coordinate against facility names without coordinates
                            if (masterArray[j].ignoreFlag == 0) {

                                // against other facilities without coordinates
                                if (masterArray[j].coordFlag == 0) {

                                    // check to see if names are the same
                                    if (sameName2(masterArray[i].convertedName, masterArray[j].convertedName) < 4) {

                                        // check matchRegion and district match - no region - possible match, matchRegions match - possible match
                                        if ((masterArray[j].matchRegion == "") || (masterArray[i].matchRegion == masterArray[j].matchRegion)) {

                                            // check districts - no district - match, districts match -match
                                            if ((masterArray[j].district == "") || (masterArray[i].district == masterArray[j].district)) {

                                                // types must match as well
                                                if ((typeCheck(masterArray[i].type, masterArray[j].type)) || (masterArray[i].type == "") || (masterArray[j].type == "") || (masterArray[i].type == "Undetermined") || (masterArray[j].type == "Undetermined")) {

                                                    // keep the one with the coordinate reguardless of priority
                                                    
                                                    // same name - remove facility without coordinates
                                                    masterArray[j].ignoreFlag = 1;

                                                    // if type is Undetermined or undefined, copy other type in
                                                    if (masterArray[i].type == "Undetermined") {
                                                        masterArray[i].type = masterArray[j].type;
                                                        masterArray[j].reason = "Same name - Undetermined type - id = " + to_string(masterArray[i].id);
                                                    }
                                                    else if (masterArray[j].type == "Undetermined") {
                                                        masterArray[j].type = masterArray[i].type;
                                                        masterArray[j].reason = "Same name - Undetermined type - id = " + to_string(masterArray[i].id);
                                                    }
                                                    else if (masterArray[i].type == "") {
                                                        masterArray[i].type = masterArray[j].type;
                                                        masterArray[j].reason = "Same name - Undefined type - id = " + to_string(masterArray[i].id);
                                                    }
                                                    else if (masterArray[j].type == "") {
                                                        masterArray[j].type = masterArray[i].type;
                                                        masterArray[j].reason = "Same name - Undefined type - id = " + to_string(masterArray[i].id);
                                                    }
                                                    else {
                                                        masterArray[j].reason = "Same name - Type the same - id = " + to_string(masterArray[i].id);
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }          //if - else
                    }
                }
            }
        }
    }
    myfile3.close();



    myfile.open("./facility-project/MasterFacilityListReducedAfterGPS.csv");

    myfile << "id " << ",coordFlag " << ", longitude " << ",latitude" << ",geometry" << ",name" << ",type" << ", orig-type" << ", address" << ", town" << ",district" << ", region" << ", match-region" << ", gps-source" << ", list-source" << '\n';

    for (i = 0; i < masterSize; i++) {

        // Don't save duplicates
        if (masterArray[i].ignoreFlag == 0) {

            myfile << masterArray[i].id << "," << masterArray[i].coordFlag;
            if (masterArray[i].coordFlag == 1) {
                myfile << "," << masterArray[i].longitude << "," << masterArray[i].latitude;
            }
            else {
                myfile << "," << "" << "," << "";
            }
            myfile << "," << masterArray[i].geometry1;
            if (masterArray[i].geometry2 != "") {
                myfile << "," << masterArray[i].geometry2;
            }
            myfile << "," << masterArray[i].name << "," << masterArray[i].type << "," << masterArray[i].origType << "," << masterArray[i].address << "," << masterArray[i].town << "," << masterArray[i].district << "," << masterArray[i].region;
            myfile << "," << masterArray[i].matchRegion << "," << masterArray[i].geoSource << "," << masterArray[i].listSource << '\n';
        }
    }

    myfile.close();

    std::cout << "inside culleMasterList before same name check " << '\n';


    // output all facilities that have same coordinates but different names
    myfile4.open("./facility-project/MasterFacilityListSameNameDiffDistrictss.csv");

    myfile4 << "name 1" << ",converted name1" << ",district 1" << ",source 1" << ",name 2" << "converted name2" << ",district 2" << ",source 2" << '\n';

    // find names in common with exception of single names like clinic, polyclinic, hospital, health centre.
    for (i = 0; i < masterSize; i++) {

        // Don't check if already being ignored
        if (masterArray[i].ignoreFlag == 0) {

            // don't check any with coordinates
            if (masterArray[i].coordFlag == 0) {

                // if generic name, don't check for uniqueness
                if (checkGenericName(masterArray[i].name) == 1);
                else {
                    // check to see if same name and same district
                    for (j = 0; j < masterSize; j++) {

                        // don't compare against itself
                        if (masterArray[i].id != masterArray[j].id) {

                            // don't look at it if it is already a duplicate
                            if (masterArray[j].ignoreFlag == 0) {

                                // don't check any wiht coordinates
                                if (masterArray[j].coordFlag == 0) {

                                    // see if the facility names are similar
                                    if (sameName2(masterArray[i].convertedName, masterArray[j].convertedName) < 4) {

                                        //std::cout << "same anme1 = " << masterArray[i].name << ", id = " << masterArray[i].id << ", name2 = " << masterArray[j].name << ", id = " << masterArray[j].id << ", possible type match fac1 type = " << masterArray[i].type << ",fac2 type = " << masterArray[j].type << '\n';
                    
                                        if ((typeCheck(masterArray[i].type, masterArray[j].type)) || (masterArray[i].type == "") || (masterArray[j].type == "") || (masterArray[i].type == "Undetermined") || (masterArray[j].type == "Undetermined")) {

                                            //std::cout << "region 1 = " << masterArray[i].matchRegion << ", region 2 = " << masterArray[j].matchRegion << ", district 1 = " << masterArray[i].district << ", district 2 = " << masterArray[j].district  << '\n';
                                            
                                            // if region missing for first facility
                                            if ((masterArray[i].matchRegion == masterArray[j].matchRegion) || (masterArray[i].matchRegion == "") || (masterArray[j].matchRegion == "")) {

                                                // if region missing for first facility
                                                if ((masterArray[i].district == masterArray[j].district) || (masterArray[i].district == "") || (masterArray[j].district == "")) {

                                                    // facilities match - lose one with lower priority
                                                    reason = "Same name";

                                                    // decide which one to keep based on priority
                                                    if (masterArray[i].priority <= masterArray[j].priority) {

                                                        keepIndex = i;
                                                        loseIndex = j;
                                                        masterArray[j].ignoreFlag = 1;
                                                    }
                                                    else {

                                                        keepIndex = j;
                                                        loseIndex = i;
                                                        masterArray[i].ignoreFlag = 1;
                                                    }


                                                    // region
                                                    if (masterArray[keepIndex].region == "") {

                                                        masterArray[keepIndex].region = masterArray[loseIndex].region;
                                                    }
                                                    if ((masterArray[keepIndex].matchRegion == "") || (masterArray[loseIndex].matchRegion == "")) {

                                                        reason = reason + " - Region empty";
                                                    }
                                                    else {

                                                        reason = reason + " - Region match";
                                                    }

                                                    // district
                                                    if (masterArray[keepIndex].district == "") {

                                                        masterArray[keepIndex].district = masterArray[loseIndex].district;
                                                    }
                                                    if ((masterArray[keepIndex].district == "") || (masterArray[loseIndex].district == "")) {

                                                        reason = reason + " - District empty";
                                                    }
                                                    else {

                                                        reason = reason + " - District match";
                                                    }


                                                    // name
                                                    if (checkGenericName(masterArray[keepIndex].name) == 1) {

                                                        masterArray[keepIndex].name = masterArray[loseIndex].name;
                                                        masterArray[keepIndex].convertedName = masterArray[loseIndex].convertedName;
                                                    }

                                                    // Type
                                                    if ((masterArray[keepIndex].type == "") || (masterArray[keepIndex].type == "Undetermined")) {

                                                        masterArray[keepIndex].type = masterArray[loseIndex].type;
                                                    }
                                                    if ((masterArray[keepIndex].type == "Undetermined") || (masterArray[loseIndex].type == "Undetermined")) {

                                                        reason = reason + " - Type Undetermined";
                                                    }
                                                    else if ((masterArray[keepIndex].type == "") || (masterArray[loseIndex].type == "")) {

                                                        reason = reason + " - Type undefined";
                                                    }
                                                    else {
                                                        reason = reason + " - Type match";
                                                    }

                                                    // add in facility id
                                                    reason = reason + " - id = " + to_string(masterArray[keepIndex].id);

                                                    // update reason for duplication
                                                    masterArray[loseIndex].reason = reason;

                                                }
                                                else {

                                                    // same name no region match
                                                    myfile4 << masterArray[i].name << "," << masterArray[i].convertedName << "," << masterArray[i].district << "," << masterArray[i].listSource.substr(0, masterArray[i].listSource.length() - 1);
                                                    myfile4 << "," << masterArray[j].name << "," << masterArray[j].convertedName << "," << masterArray[j].district << "," << masterArray[j].listSource << '\n';
                                                }
                                            }
                                            else {

                                                // same name no region match
                                                myfile4 << masterArray[i].name << "," << masterArray[i].convertedName << "," << masterArray[i].district << "," << masterArray[i].listSource.substr(0, masterArray[i].listSource.length() - 1);
                                                myfile4 << "," << masterArray[j].name << "," << masterArray[j].convertedName << "," << masterArray[j].district << "," << masterArray[j].listSource << '\n';
                                            }
                                        }
                                        else {

                                            // same name no type match
                                            myfile4 << masterArray[i].name << "," << masterArray[i].convertedName << "," << masterArray[i].district << "," << masterArray[i].listSource.substr(0, masterArray[i].listSource.length() - 1);
                                            myfile4 << "," << masterArray[j].name << "," << masterArray[j].convertedName << "," << masterArray[j].district << "," << masterArray[j].listSource << '\n';
                                        }
                                    }
                                } // region check
                            }      // ignore flag ckeck
                        }
                    }
                }
                //std::cout << "name the same = " << masterArray[i].name << '\n';
            }
        }
    }

    myfile4.close();



    myfile7.open("./facility-project/MasterFacilityListReducedAfterNameCheck.csv");

    myfile7 << "id " << ",coordFlag " << ", longitude " << ",latitude" << ",geometry" << ",name" << ",type" << ", orig-type" << ", address" << ", town" << ",district" << ", region" << ", match-region" << ", gps-source" << ", list-source" << '\n';

    for (i = 0; i < masterSize; i++) {

        // Don't save duplicates
        if (masterArray[i].ignoreFlag == 0) {

            myfile7 << masterArray[i].id << "," << masterArray[i].coordFlag;
            if (masterArray[i].coordFlag == 1) {
                myfile7 << "," << masterArray[i].longitude << "," << masterArray[i].latitude;
            }
            else {
                myfile7 << "," << "" << "," << "";
            }
            myfile << "," << masterArray[i].geometry1;
            if (masterArray[i].geometry2 != "") {
                myfile7 << "," << masterArray[i].geometry2;
            }
            myfile7 << "," << masterArray[i].name << "," << masterArray[i].type << "," << masterArray[i].origType << "," << masterArray[i].address << "," << masterArray[i].town << "," << masterArray[i].district << "," << masterArray[i].region;
            myfile7 << "," << masterArray[i].matchRegion << "," << masterArray[i].geoSource << "," << masterArray[i].listSource << '\n';
        }
    }

    myfile7.close();

    std::cout << "inside culleMasterList after same name check " << '\n';

    std::cout << "inside culleMasterList before output " << '\n';







    // write out master list broken down by region and district
    // mkdir("../abc"); returns 0 if successful
     // create other directory
    baseDirName = "./Ghana";
    char temp5[baseDirName.size() + 1];
    strcpy(temp5, baseDirName.c_str());
    result = mkdir(temp5, 0777);


    for (i = 0; i < rdSize; i++) {

        // create sub-directories for each region
        dirName = baseDirName +"/" + rdArray[i].region;
        char temp2[dirName.size() + 1];
        strcpy(temp2, dirName.c_str());
        result = mkdir(temp2, 0777);

        // now make one for each district
        //dirName = baseDirName + "/" + rdArray[i].region + "/" + rdArray[i].district;
        //char temp1[dirName.size() + 1];
        //strcpy(temp1, dirName.c_str());
        //result = mkdir(temp1, 0777);

        //std::cout << "Trying to create subdirectory  " << temp << " , success = " << result << '\n';
    }

    // create other directory
    dirName = baseDirName + "/Other-Facilities";
    char temp3[dirName.size() + 1];
    strcpy(temp3, dirName.c_str());
    result = mkdir(temp3, 0777);


    // create coombined master directory
    dirName = baseDirName + "/Master";
    char temp4[dirName.size() + 1];
    strcpy(temp4, dirName.c_str());
    result = mkdir(temp4, 0777);

    std::cout << "Trying create subdirectories = "  << result << ", writing combined master file" << '\n';




    myfile1.open("./Ghana/Master/MasterFacilityList.csv");

    myfile1 << "id " << ",ref-id " << ", longitude " << ",latitude" << ",same-gps" << ",name" << ",same-name" << ",converted-name" << ",type" << ", orig-type" << ", address" << ", town" << ",district" << ", region" <<", district-code" << ", gps-source" << ", list-source" << ", possible town" << ", possible town" << '\n';

    for (i = 0; i < masterSize; i++) {

        // create same GPS column and the same name column - for debugging purposes
        // Don't check against one already considered duplicates
        if (masterArray[i].ignoreFlag == 0) {

            sameGPS = "";
            sameNames = "";

            // check to see if same name or same GPS
            for (j = 0; j < masterSize; j++) {

                // only look at active facilities
                if (masterArray[j].ignoreFlag == 0) {

                    // don't compare against itself
                    if (masterArray[i].id != masterArray[j].id) {

                        if ((masterArray[i].coordFlag > 0) && (masterArray[j].coordFlag > 0)) {

                            // check for same GPS 
                            dist = sqrt(pow((masterArray[i].longitude - masterArray[j].longitude), 2) + pow((masterArray[i].latitude - masterArray[j].latitude), 2));
                            if (dist <= .0001) {
                                if (sameGPS == "") {
                                    sameGPS = to_string(masterArray[j].id);
                                }
                                else {
                                    sameGPS = sameGPS + " " + to_string(masterArray[j].id);
                                }
                            }
                        }

                        // ignore these
                        if (checkGenericName(masterArray[i].name) == 1);
                        else {
                            if (sameName(masterArray[i].name, masterArray[j].name)) {
                                if (sameNames == "") {
                                    sameNames = to_string(masterArray[j].id);
                                }
                                else {
                                    sameNames = sameNames + " " + to_string(masterArray[j].id);
                                }
                            }
                        }
                    }
                }
            }

            // store away for use when outputing districts/regions
            masterArray[i].GPS = sameGPS;
            masterArray[i].Names = sameNames;

            myfile1 << newID++ << "," << masterArray[i].id;
            if (masterArray[i].coordFlag == 1) {
                myfile1 << "," << masterArray[i].longitude << "," << masterArray[i].latitude;
            }
            else {
                myfile1 << "," << "" << "," << "";
            }
            //myfile1 << "," << masterArray[i].geometry1;
            //if (masterArray[i].geometry2 != "") {
            //    myfile1 << "," << masterArray[i].geometry2;
            //}
            myfile1 << "," << masterArray[i].GPS << "," << masterArray[i].name << "," << masterArray[i].Names << "," << masterArray[i].convertedName << "," << masterArray[i].type << "," << masterArray[i].origType << "," << masterArray[i].address << "," << masterArray[i].town;
            myfile1 << "," << masterArray[i].district << "," << masterArray[i].matchRegion << "," << masterArray[i].code << "," << masterArray[i].geoSource << "," << masterArray[i].listSource << "," << masterArray[i].pTown1 << "," << masterArray[i].pTown2 << '\n';

        }
    }

    myfile1.close();


    std::cout << "output individual district files  " << '\n';

    for (i = 0; i < rdSize; i++) {

        // create files for each district/region
        //dirName = baseDirName + "/" + rdArray[i].region + "/" + rdArray[i].district;

        dirName = baseDirName + "/" + rdArray[i].region;
        fileName = "/" + rdArray[i].district + ".csv";
        myfile1.open(dirName + fileName);

        myfile1 << "id " << ",status" << ",ref-id " << ",name" << ",same-name" << ", longitude " << ",latitude" << ",same-gps" <<  ",type" << ", orig-type" << ", address" << ", town" << ",district" << ", region" << ", district-code" << ", gps-source" << ", list-source" << ", primary town" << ", alt. town" << " alt. town" << '\n';
        for (j = 0; j < masterSize; j++) {

            // create same GPS column and the same name column - for debugging purposes
            // Don't check against one already considered duplicates
            if (masterArray[j].ignoreFlag == 0) {

                if (masterArray[j].outputFlag == 0) {

                    if (masterArray[j].matchRegion == rdArray[i].region) {

                        if (masterArray[j].district == rdArray[i].district) {

                            myfile1 << newID++ << "," << "Keep" << "," << masterArray[j].id << "," << masterArray[j].name << "," << masterArray[j].Names;
                            if (masterArray[j].coordFlag == 1) {
                                myfile1 << "," << masterArray[j].longitude << "," << masterArray[j].latitude;
                            }
                            else {
                                myfile1 << "," << "" << "," << "";
                            }
                            //myfile1 << "," << masterArray[j].geometry1;
                            //if (masterArray[j].geometry2 != "") {
                            //    myfile1 << "," << masterArray[j].geometry2;
                            //}
                            myfile1 << "," << masterArray[j].GPS << "," << masterArray[j].type << "," << masterArray[j].origType << "," << masterArray[j].address << "," << masterArray[j].town;
                            myfile1 << "," << masterArray[j].district << "," << masterArray[j].matchRegion << "," << masterArray[j].code << "," << masterArray[j].geoSource;
                            myfile1 << "," << masterArray[j].listSource << "," << masterArray[j].town << "," << masterArray[j].pTown1 << "," << masterArray[j].pTown2 << '\n';

                            masterArray[j].outputFlag = 1;
                        }
                    }
                }
            }
        }

        myfile1.close();
    }



    std::cout << "output other district files  " << '\n';

    if (true) {
        // now write out remaaining facilities that don't have any district and region
        myfile1.open("./Ghana/Other-Facilities/MasterFacilityList-other.csv");

        myfile1 << "id " << ",ref-id " << ", longitude " << ",latitude" << ",same-gps" << ",name" << ",same-name" << ",converted-name" << ",type" << ", orig-type" << ", address" << ", town" << ",district" << ", region" << ", district-code" << ", gps-source" << ", list-source" << '\n';

        for (j = 0; j < masterSize; j++) {

            // create same GPS column and the same name column - for debugging purposes
            // Don't check against one already considered duplicates
            if (masterArray[j].ignoreFlag == 0) {

                if (masterArray[j].outputFlag == 0) {

                    myfile1 << newID++ << "," << masterArray[j].id;
                    if (masterArray[j].coordFlag == 1) {
                        myfile1 << "," << masterArray[j].longitude << "," << masterArray[j].latitude;
                    }
                    else {
                        myfile1 << "," << "" << "," << "";
                    }
                    //myfile1 << "," << masterArray[j].geometry1;
                    //if (masterArray[j].geometry2 != "") {
                    //    myfile1 << "," << masterArray[j].geometry2;
                    //}
                    myfile1 << "," << masterArray[j].GPS << "," << masterArray[j].name << "," << masterArray[j].Names << "," << masterArray[j].convertedName << "," << masterArray[j].type << "," << masterArray[j].origType << "," << masterArray[j].address << "," << masterArray[j].town;
                    myfile1 << "," << masterArray[j].district <<  "," << masterArray[j].matchRegion <<  "," << masterArray[j].code << "," << masterArray[j].geoSource << "," << masterArray[j].listSource << '\n';

                    masterArray[j].outputFlag = 1;
                }
            }
        }

        myfile1.close();
    }



    std::cout << "output ignored files  " << '\n';

    // now put out all the excluded files
    myfile5.open("./Ghana/Master/MasterFacilityListDuplicateOrExcludedFacilities.csv");

    myfile5 << "id " << ",coordFlag " << ", longitude " << ",latitude" << ",name" << ",converted-names" << ",type" << ", orig-type" << ", address" << ", town" << ",district" << ", region" << ", district-code" << ", gps-source" << ", list-source" << ", reason" << '\n';

    for (i = 0; i < masterSize; i++) {

        // Don't save duplicates
        if (masterArray[i].ignoreFlag == 1) {

            myfile5 << masterArray[i].id << "," << masterArray[i].coordFlag;
            if (masterArray[i].coordFlag == 1) {
                myfile5 << "," << masterArray[i].longitude << "," << masterArray[i].latitude;
            }
            else {
                myfile5 << "," << "" << "," << "";
            }
            //myfile5 << "," << masterArray[i].geometry1;
            //if (masterArray[i].geometry2 != "") {
            //    myfile5 << "," << masterArray[i].geometry2;
            //}
            myfile5 << "," << masterArray[i].name << "," << masterArray[i].convertedName << "," << masterArray[i].type << "," << masterArray[i].origType << "," << masterArray[i].address << "," << masterArray[i].town << "," << masterArray[i].district;
            myfile5 << "," << masterArray[i].matchRegion << "," << masterArray[i].code << "," << masterArray[i].geoSource << "," << masterArray[i].listSource << "," << masterArray[i].reason << '\n';
        }
    }

    myfile5.close();
}



// create google API statements to check travel times between facilities and zonal labs
void outputGoogleApi(vector<vector<string>> masterList, vector<vector<string>> zonalLabs) {

    ofstream myfile;

    int count = 0;
    int countInDistrict;
    string currentDistrict;
    string newName;
    string zonalName;
    string temp;
    string sameGPS;
    string sameNames;
    string dirName;
    string baseDirName;
    string fileName;
    string reason;
    //char* dirName;
    std::size_t i;
    std::size_t masterSize = masterList.size();
    std::size_t zonalSize = zonalLabs.size();

    struct masterStruct {
        int id;
        int refID;
        float longitude;
        float latitude;
        string sameGPS;
        string name;
        string sameName;
        string convertedName;
        string type;
        string origType;
        string address;
        string town;
        string district;
        string region;

    };

    struct zonalStruct {
        string name;
        float longitude;
        float latitude;
    };


    //struct masterStruct masterArray[masterSize];
    vector<masterStruct> masterArray;
    vector<zonalStruct> zonalArray;

    // adjust array index for removed header
    masterSize = masterSize - 1;
    zonalSize = zonalSize - 1;

    std::cout << "inside culleMasterList Master Size = " << masterSize << '\n';


    // master has format id, ref id, longitude, latitude, same-gps, name, same-name, type, original type, address, district,  region, gps-source, list source, possible town
    for (i = 0; i < masterSize; i++) {

        if (false) {
            std::cout << masterList[i + 1][0] << ", " << masterList[i + 1][1] << ", " << masterList[i + 1][2] << ", " << masterList[i + 1][3] << ", " << masterList[i + 1][4] << ", " << masterList[i + 1][5] << ", " << masterList[i + 1][6];
            std::cout << ", " << masterList[i + 1][7] << ", " << masterList[i + 1][8] << ", " << masterList[i + 1][9] << ", " << masterList[i + 1][10] << ", " << masterList[i + 1][11];
            std::cout << ", " << masterList[i + 1][12] << ", " << masterList[i + 1][13] << ", " << masterList[i + 1][14] << ", " << masterList[i + 1][15];
            if (masterList[i + 1][4] == "") {
                std::cout << '\n';
            }
            else {
                std::cout << ", " << masterList[i + 1][15] << '\n';
            }
        }

        newName = "masterSruct" + std::to_string(count++);

        masterStruct newName;


        newName.longitude = stof(masterList[i + 1][2]);
        newName.latitude = stof(masterList[i + 1][3]);
        newName.district = masterList[i + 1][12];

        masterArray.push_back(newName);
    }

    masterSize = masterArray.size();

    std::cout << "inside google API before reading in zonal labs " << '\n';

    count = 0;

    // district region list has format district, region, district without dash, dummy
    for (i = 0; i < zonalSize; i++) {


        zonalName = "regDistStruct" + std::to_string(count++);

        zonalStruct zonalName;

        zonalName.longitude = stof(zonalLabs[i + 1][1]);
        zonalName.latitude = stof(zonalLabs[i + 1][2]);

        zonalArray.push_back(zonalName);
    }

    zonalSize = zonalArray.size();

    std::cout << "inside google api  - read in zonal labs " << '\n';


    // now put out all the excluded files
    myfile.open("./facility-project/googleAPI/googleAPIScript.txt");

    // https:// maps.googleapis.com / mapsmaps / api / distancematrix / json ? units = imperial & origins = 40.6655101, -73.89188969999998 & destinationsdestinations = enc:_kjwFjtsbMt % 60EgnKcqLcaOzkGari % 40naPxhVg % 7CJjjb % 40cqLcaOzkGari % 40naPxhV : &key = YOUR_API_KEY
    // limit of 8192 characters
   

    currentDistrict = "";
    countInDistrict = 0;
    

    for (i = 0; i < masterSize; i++) {

        if (currentDistrict == "")  {
            myfile << " https://maps.googleapis.com/maps/api/distancematrix/json?units=imperial&origins=";
            currentDistrict = masterArray[i].district;

            std::cout << "current district empty =  " << currentDistrict << "," << countInDistrict << '\n';
        }

        // write out origins
        if (currentDistrict == masterArray[i].district) {

            // add | if more than 1 facility in district
            if (countInDistrict > 0) {

                myfile << "|" << masterArray[i].latitude << "," << masterArray[i].longitude;
            }
            else {

                myfile <<  masterArray[i].latitude << "," << masterArray[i].longitude;
            }
            countInDistrict++;

            std::cout << "district the same =  " << currentDistrict << "," << countInDistrict << "," << countInDistrict << "," << masterArray[i].district <<'\n';
        }
        else {
            // district changed - add destinations and start a new line 

            myfile << "&destinations=";

            myfile << zonalArray[0].latitude << "," << zonalArray[0].longitude << "|" << zonalArray[1].latitude << "," << zonalArray[1].longitude;
            myfile << "|" << zonalArray[2].latitude << "," << zonalArray[2].longitude << "|" << zonalArray[3].latitude << "," << zonalArray[3].longitude;

            myfile << "&mode=driving&key=AIzaSyB1zrSDaR-DKSD8bnwbRAWymyt_GGLE_b0" << '\n';
                
            std::cout << "district the same =  " << currentDistrict << "," << countInDistrict << "," << countInDistrict << "," << masterArray[i].district <<'\n';

            countInDistrict = 0;

            myfile << " https://maps.googleapis.com/maps/api/distancematrix/json?units=imperial&origins=";
            currentDistrict = masterArray[i].district;

            myfile << masterArray[i].latitude << "," << masterArray[i].longitude;

            countInDistrict = 1;
        }

    }

    // finish off last script line
    myfile << "&destinations=";

    myfile << zonalArray[0].longitude << "," << zonalArray[0].latitude << "|" << zonalArray[1].longitude << "," << zonalArray[1].latitude;
    myfile << "|" << zonalArray[2].longitude << "," << zonalArray[2].latitude << "|" << zonalArray[3].longitude << "," << zonalArray[3].latitude;

    myfile << "&mode=driving&key=AIzaSyB1zrSDaR-DKSD8bnwbRAWymyt_GGLE_b0" << '\n';



    myfile.close();
}


// just add master list with results from google query
void masterListPlusGoogle(vector<vector<string>> masterList, vector<vector<string>> gmaps) {

    ofstream myfile;
   
    int count = 0;
    int newID = 1;
    int currentFacId;
    float dist;
    string newName;
    string gmapName;
    string temp;
    string sameGPS;

    //char* dirName;
    std::size_t i, j;
    std::size_t masterSize = masterList.size();
    std::size_t gmapSize = gmaps.size();

    // master file format - id,ref-id , longitude ,latitude same-gps ,name ,same-name ,converted-name ,type , orig-type, address , town ,district , region , district-code , gps-source , list-source , possible town , possible town 
    struct masterStruct {
        int id;
        int refID;
        float longitude;
        float latitude;
        string sameGPS;
        string name;
        string names;
        string convertedName;
        string type;
        string origType;
        string address;
        string town;
        string district;
        string region;
        string matchRegion;
        int districtCode;
        string geoSource;
        string listSource;
        string pTown1;
        string pTown2;

    };

    struct gmapStruct {
        int facId;
        string name;
        float longitude;
        float latitude;
        string good;
    };


    //struct masterStruct masterArray[masterSize];
    vector<masterStruct> masterArray;
    vector<gmapStruct> gmapArray;

    // adjust array index for removed header
    masterSize = masterSize - 1;
    gmapSize = gmapSize - 1;

    std::cout << "inside master+google Master Size = " << masterSize << '\n';

    // master has format id, longitude, latitude, geometry(c(x,y)), name, type, original type, address, district,  region, match-region, gps-source, list source, priority
    for (i = 0; i < masterSize; i++) {

        if (false) {
            std::cout << masterList[i + 1][0] << ", " << masterList[i + 1][1] << ", " << masterList[i + 1][2] << ", " << masterList[i + 1][3] << ", " << masterList[i + 1][4] << ", " << masterList[i + 1][5] << ", " << masterList[i + 1][6];
            std::cout << ", " << masterList[i + 1][7] << ", " << masterList[i + 1][8] << ", " << masterList[i + 1][9] << ", " << masterList[i + 1][10] << ", " << masterList[i + 1][11];
            std::cout << ", " << masterList[i + 1][12] << ", " << masterList[i + 1][13] << ", " << masterList[i + 1][14] << ", " << masterList[i + 1][15]<< ", " << masterList[i + 1][16];
            std::cout << ", " << masterList[i + 1][17] <<'\n';
            //std::cout << ", " << masterList[i + 1][18] << '\n';
        }

        newName = "masterSruct" + std::to_string(count++);

        masterStruct newName;

        // convert where neccessary
        //newName.id = i;
        newName.id = stoi(masterList[i + 1][0]);
        newName.refID = stoi(masterList[i + 1][1]);
        if (masterList[i + 1][2] == "") {
            newName.longitude = 100;
            newName.latitude = 100;
        }
        else {

            newName.longitude = stof(masterList[i + 1][2]);
            newName.latitude = stof(masterList[i + 1][3]);
        }

        newName.sameGPS = masterList[i + 1][4];
        newName.name = masterList[i + 1][5];
        newName.names = masterList[i + 1][6];
        newName.convertedName = masterList[i + 1][7];
        newName.type = masterList[i + 1][8];
        newName.origType = masterList[i + 1][9];
        newName.address = masterList[i + 1][10];
        newName.town = masterList[i + 1][11];
        newName.district = masterList[i + 1][12];
        newName.region = masterList[i + 1][13];
        newName.districtCode = stoi(masterList[i + 1][14]);
        newName.geoSource = masterList[i + 1][15];
        newName.listSource = masterList[i + 1][16];
        newName.pTown1 = masterList[i + 1][17];
        //if (masterList[i+1][17] == "") {
            newName.pTown2 = "";
        //}
        //else {
        //    newName.pTown2 = masterList[i + 1][18];
        //}

        masterArray.push_back(newName);
    }

    masterSize = masterArray.size();


    // output array
    if (false) {
        for (i = 0; i < masterSize; i++) {
            std::cout << "facility name = " << masterArray[i].name << ", id = " << masterArray[i].id << ", x,y = " << masterArray[i].longitude << ", " << masterArray[i].latitude << '\n';
        }
    }

    std::cout << "inside master+google before reading in gmap " <<  '\n';

    count = 0;

    // district region list has format  facility_id, name, district, search term, cand name, cand longitude, cand lattitude, cand district, cand in district, URL
    for (i = 0; i < gmapSize; i++) {

        if (false) {
                std::cout << gmaps[i + 1][0] << ", " << gmaps[i + 1][1] << ", " << gmaps[i + 1][2] << ", " << gmaps[i + 1][3] << ", " << gmaps[i + 1][4];
                std::cout<< ", " << gmaps[i + 1][5] << ", " << gmaps[i + 1][6] << ", " << gmaps[i + 1][7] <<", " << gmaps[i + 1][8] << '\n';
        }

        gmapName = "gmapStruct" + std::to_string(count++);

        gmapStruct gmapName;

        gmapName.facId = stoi(gmaps[i + 1][0]);
        gmapName.name = gmaps[i + 1][1];
        gmapName.good = gmaps[i + 1][8];
        if (gmapName.good == "TRUE") {
            gmapName.longitude = stof(gmaps[i + 1][5]);
            gmapName.latitude = stof(gmaps[i + 1][6]);
        }
        else {
            gmapName.longitude = 100;
            gmapName.latitude = 100;
        }

        gmapArray.push_back(gmapName);
    }

    gmapSize = gmapArray.size();

    std::cout << "inside master+google - read in gmap list = " << gmapSize << '\n';



    currentFacId = 0;
    // now loop through the gmaps list. Adjust the coordinates for matching facility ids. Take the first entry out of the multiple entries for the same facilityId
    for (i = 0; i < gmapSize; i++) {

        //std::cout << "inside master+google -" << gmapArray[i].good << "-" << '\n';

        if (gmapArray[i].good == "TRUE") {

            //std::cout << "inside master+google - true " << gmapArray[i].name << ", " << gmapArray[i].facId << '\n';

            if (currentFacId != gmapArray[i].facId) {

                //std::cout << "inside master+google - true " << gmapArray[i].facId << ", " << currentFacId << '\n';
                
                // find the matching one in the master list and update longitude and latitude
                for (j = 0; j < masterSize; j++) {
            
                    if (gmapArray[i].facId == masterArray[j].id) {

                        masterArray[j].longitude = gmapArray[i].longitude;
                        masterArray[j].latitude = gmapArray[i].latitude;

                        //std::cout << "inside master+google - found match " << gmapArray[i].name << ", " << masterArray[j].name << '\n';
                    }
                }

                currentFacId = gmapArray[i].facId;
            }
        }
    }

    std::cout << "inside master+google - output new master list " << '\n';

    myfile.open("./Ghana/Master/MasterFacilityListPlusGmaps.csv");

    myfile << "id " << ",ref-id " << ", longitude " << ",latitude" << ",same-gps" << ",name" << ",same-name" << ",converted-name" << ",type" << ", orig-type" << ", address" << ", town" << ",district" << ", region" <<", district-code" << ", gps-source" << ", list-source" << ", possible town" << ", possible town" << '\n';

    for (i = 0; i < masterSize; i++) {

        // create same name column - for debugging purposes
        sameGPS = "";

        // check to see if same name or same GPS
        for (j = 0; j < masterSize; j++) {

            // don't compare against itself
            if (masterArray[i].id != masterArray[j].id) {

                if ((masterArray[i].longitude != 100) && (masterArray[j].longitude !=100)) {

                    // check for same GPS 
                    dist = sqrt(pow((masterArray[i].longitude - masterArray[j].longitude), 2) + pow((masterArray[i].latitude - masterArray[j].latitude), 2));
                    if (dist <= .0001) {
                        if (sameGPS == "") {
                            sameGPS = to_string(masterArray[j].id);
                        }
                        else {
                             sameGPS = sameGPS + " " + to_string(masterArray[j].id);
                        }
                    }
                }
            }
        }

        // store away for use when outputing districts/regions
        masterArray[i].sameGPS = sameGPS;

        myfile << newID++ << "," << masterArray[i].id;
        if (masterArray[i].longitude == 100) {
            myfile << "," << "" << "," << "";
        }
        else {
            myfile << "," << masterArray[i].longitude << "," << masterArray[i].latitude;
        }
        //myfile << "," << masterArray[i].geometry1;
        //if (masterArray[i].geometry2 != "") {
        //    myfile << "," << masterArray[i].geometry2;
        //}
        myfile << "," << masterArray[i].sameGPS << "," << masterArray[i].name << "," << masterArray[i].names << "," << masterArray[i].convertedName << "," << masterArray[i].type << "," << masterArray[i].origType << "," << masterArray[i].address << "," << masterArray[i].town;
        myfile << "," << masterArray[i].district << "," << masterArray[i].matchRegion << "," << masterArray[i].districtCode << "," << masterArray[i].geoSource << "," << masterArray[i].listSource << "," << masterArray[i].pTown1 << "," << masterArray[i].pTown2 << '\n';
    }

    myfile.close();
}
    


// find closest regional hospital to facility
void facilityToRegHospital(vector<vector<string>> facilityList) {

    ofstream myfile;
    std::size_t i, j, regStartIndex;
    std::size_t facilitySize = facilityList.size();
    std::size_t regSize = 14;
    float dist;
    float newDist;

    struct facilityStruct {
        float longitude;
        float latitude;
        string regFacilityName;
        float regFacDist;
    };

    struct regStruct {
        float longitude;
        float latitude;
        string regFacilityName;        
    };

    struct facilityStruct facilityArray[facilitySize];
    struct regStruct regArray[regSize];

    // adjust array index for removed header
    facilitySize = facilitySize - 1;

    std::cout << "inside updateMasterList facility Size = " << facilitySize << '\n';

    // store regional hospitals and national labs into seperate structure
    j = 0;

    // master has format id, longitude, latitude, geometry(c(x,y)), name, type, original type, address, district,  gps-source, list source, priority
    for (i = 0; i < facilitySize; i++) {

        if (false) {
            std::cout << facilityList[i+1][36] << ", " << facilityList[i+1][37] << '\n';
        }

        
        facilityArray[i].longitude = stof(facilityList[i + 1][36]);
        facilityArray[i].latitude = stof(facilityList[i + 1][37]);
        facilityArray[i].regFacilityName = "";
        facilityArray[i].regFacDist = 10.0;

        if (i >= facilitySize-regSize) {
            regArray[j].longitude = stof(facilityList[i + 1][36]);
            regArray[j].latitude = stof(facilityList[i + 1][37]);
            regArray[j].regFacilityName = facilityList[i + 1][35];
            
            if (false) {
                std::cout << "regions name and index " << regArray[j].regFacilityName << ", " << j << '\n';
            }

            j++;
        }
    }
    regStartIndex = facilitySize - regSize;

    // now find out which regional hospital is closest to a facility
    for (i = 0; i < regStartIndex; i++) {

        dist = 10;

        for (j = 0; j < regSize - 4; j++) {

            newDist = sqrt( pow((facilityArray[i].longitude - regArray[j].longitude),2) + pow((facilityArray[i].latitude - regArray[j].latitude),2));

            if (newDist < dist) {

                facilityArray[i].regFacDist = newDist;
                facilityArray[i].regFacilityName = regArray[j].regFacilityName;

                dist = newDist;
            }

        }
    }

    // now figure out which national lab is closest to which regional hospital
    for (i = 0; i < regSize - 4; i++) {

        dist = 10;

        for (j = 10; j < regSize; j++) {

            newDist = sqrt( pow((regArray[i].longitude - regArray[j].longitude),2) + pow((regArray[i].latitude - regArray[j].latitude),2));

            if (newDist < dist) {

                facilityArray[regStartIndex + i].regFacDist = newDist;
                facilityArray[regStartIndex + i].regFacilityName = regArray[j].regFacilityName;

                dist = newDist;
            }

        }
    }


    // now calculate distance between national lab and zonal labs
    facilityArray[facilitySize - 4].regFacDist = sqrt( pow((facilityArray[facilitySize - 4].longitude - facilityArray[facilitySize - 1].longitude),2) + pow((facilityArray[facilitySize - 4].latitude - facilityArray[facilitySize - 1].latitude),2));
    facilityArray[facilitySize - 4].regFacilityName = regArray[regSize - 1].regFacilityName;

    facilityArray[facilitySize - 3].regFacDist = sqrt( pow((facilityArray[facilitySize - 3].longitude - facilityArray[facilitySize - 1].longitude),2) + pow((facilityArray[facilitySize - 3].latitude - facilityArray[facilitySize - 1].latitude),2));
    facilityArray[facilitySize - 3].regFacilityName = regArray[regSize - 1].regFacilityName;

    facilityArray[facilitySize - 2].regFacDist = sqrt( pow((facilityArray[facilitySize - 2].longitude - facilityArray[facilitySize - 1].longitude),2) + pow((facilityArray[facilitySize - 2].latitude - facilityArray[facilitySize - 1].latitude),2));
    facilityArray[facilitySize - 2].regFacilityName = regArray[regSize - 1].regFacilityName;

    facilityArray[facilitySize - 1].regFacDist = 0;
    facilityArray[facilitySize - 1].regFacilityName = regArray[regSize - 1].regFacilityName;

    myfile.open("./input/facilities/facilities1.csv");

    myfile <<"ID " << ",	Regional Hospital " << ",	Facility_Name " << ",	Distance to Reg Hosp " << ",	Maina...Facility.name ";
    myfile << ",	Fac.NAME " << ",	Admin1 " << ",	Facility.name " << ",	Facility.type " << ",	Ownership " << ",	LL.source ";
    myfile << ",	Lat	" << ",	Long " << ",	FacLong " << ",	FacLat " << ",	Source " << ",	geometry...longitude " << ",	geometry.latitude ";
    myfile << ",	UnitId " << ",	UnitType " << ",	UnitTypeName " << ",	TypeMainaOverGhana " << ",	FacilitySwitch " << ",	MainaSwitch ";
    myfile << ",	NewFacilityTypes " << ",	UnitTypeNaFac " << ",	LvlId " << ",	Region " << ",	CODE " << ",	NAME " << ",	COMMENT	 " << ",	CODE_1 ";
    myfile << ",	NAME_1 " << ",	NEAR_FID " << ",	NEAR_DIST " << ",	Facility_Name " << ",	Longitude " << ",	Latitude " << ",	geometry ";
    myfile << ",	Region " << ",	District " << ",	Town " << ",	facilityID " << ",	Xpert " << ",	dummy " << '\n';

    for (i = 0; i < facilitySize; i++) {

        myfile << facilityList[i + 1][0] << "," << facilityArray[i].regFacilityName << "," << facilityList[i + 1][2] << "," << facilityArray[i].regFacDist << "," << facilityList[i + 1][4] << "," << facilityList[i + 1][5] << "," << facilityList[i + 1][6] << "," << facilityList[i + 1][7];
        myfile << "," << facilityList[i + 1][8] << "," << facilityList[i + 1][9] << "," << facilityList[i + 1][10] << "," << facilityList[i + 1][11] << "," << facilityList[i + 1][12] << "," << facilityList[i + 1][13] << "," << facilityList[i + 1][14] << "," << facilityList[i + 1][15];
        myfile << "," << facilityList[i + 1][16] << "," << facilityList[i + 1][17] << "," << facilityList[i + 1][18] << "," << facilityList[i + 1][19] << "," << facilityList[i + 1][20] << "," << facilityList[i + 1][21] << "," << facilityList[i + 1][22] << "," << facilityList[i + 1][23];
        myfile << "," << facilityList[i + 1][24] << "," << facilityList[i + 1][25] << "," << facilityList[i + 1][26] << "," << facilityList[i + 1][27] << "," << facilityList[i + 1][28] << "," << facilityList[i + 1][29] << "," << facilityList[i + 1][30] << "," << facilityList[i + 1][31];
        myfile << "," << facilityList[i + 1][32] << "," << facilityList[i + 1][33] << "," << facilityList[i + 1][34] << "," << facilityList[i + 1][35] << "," << facilityList[i + 1][36] << "," << facilityList[i + 1][37] << "," << facilityList[i + 1][38] << "," << facilityList[i + 1][39];
        myfile << "," << facilityList[i + 1][40] << "," << facilityList[i + 1][41] << "," << facilityList[i + 1][42] << "," << facilityList[i + 1][43] << "," << facilityList[i + 1][44] << '\n';
    }



     myfile.close();
}



// find distance between facilities and testing center
void mapFacDist( vector<vector<string>> fac, vector<vector<string>> dist) {

    ofstream myfile;

    std::size_t i, j;
    int counter = 0;
    string distName;
    string facName;
    std::size_t distSize = dist.size();
    std::size_t facSize = fac.size();

    struct distStruct {

        string name;
        string toName;
        int time;

    };

    vector<distStruct> distArray;

    struct facStruct {
        string name;
        string type;
        float longitude;
        float latitude;
        string district;
        string region;
        int tier;
        int testing;
        int Xpert;
        string upName;
        int time;

    };

    vector<facStruct> facArray;

    // adjust array index for removed header
    distSize = distSize - 1;
    facSize = facSize - 1;

    std::cout << "inside fac2dist facSize = " << facSize << '\n';

    counter = 0;

    // fac list has format facility name, id, type, long, lat, town, district, region, tier, diagmosis flag, testing flag ,Xpert here, up facility
    for (i = 0; i < facSize; i++) {

        facName = "facStruct" + std::to_string(counter++);

        //std::cout << fac[i + 1][0] << ", " << fac[i + 1][2] << ", " << fac[i + 1][3] << ", " << fac[i + 1][4]  << ", " << fac[i + 1][6]  << ", " << fac[i + 1][7] << ", " << fac[i + 1][8]  << ", " << fac[i + 1][10]  << ", " << fac[i + 1][11] << ", " << fac[i + 1][12] << '\n';

        facStruct facName;

        facName.name = fac[i + 1][0];
        facName.type = fac[i + 1][2];
        facName.longitude = stof(fac[i + 1][3]);
        facName.latitude = stof(fac[i + 1][4]);
        facName.district = fac[i + 1][6];
        facName.region = fac[i + 1][7];
        facName.tier = stoi(fac[i + 1][8]);
        facName.testing = stoi(fac[i + 1][10]);
        facName.Xpert =  stoi(fac[i + 1][11]);
        facName.upName = fac[i + 1][12];
        facName.time = 0;

        facArray.push_back(facName);
    }

    facSize = facArray.size();

    std::cout << "inside fac2dist read in facilities  " << '\n';


    // dump cities array
    if (false) {

        for (i = 0; i < facSize; i++) {
            std::cout << "facilities array - name = " << facArray[i].name << "," << facArray[i].type << "," << facArray[i].latitude;
            std::cout << "," << facArray[i].longitude << "," << facArray[i].district << "," << facArray[i].region << "," << facArray[i].upName <<  '\n';
        }
    }


    std::cout << "inside fac2dist accessmod dist Size = " << distSize << '\n';

    counter = 0;

    // geo list has format id, from name, id, to name, distance, time
    for (i = 0; i < distSize; i++) {

        if (false) {
            std::cout << dist[i + 1][1] << ", " << dist[i + 1][3]  << ", " << dist[i + 1][5]  << '\n';
        }

        distName = "distStruct" + std::to_string(counter++);

        distStruct distName;

        distName.name = dist[i + 1][1];
        distName.toName = dist[i + 1][3];
        distName.time = stoi(dist[i + 1][5]);



        distArray.push_back(distName);
    }

    distSize = distArray.size();

    std::cout << "inside dist2fac after reading accessmod dist file  " << '\n';


    // dump cities array
    if (false) {

        for (i = 0; i < distSize; i++) {
            std::cout << "dist array - name = " << distArray[i].name << "," << distArray[i].toName << "," << distArray[i].time << '\n';
        }
    }

    // find distance between facility and up connection if testing isn't done here
    for (i = 0; i < facSize; i++) {

        if (facArray[i].Xpert < 1) {

            for (j = 0; j < distSize; j++) {

                // same name
                if ((facArray[i].name == distArray[j].name) && (facArray[i].upName == distArray[j].toName)) {

                        facArray[i].time = distArray[j].time;
                }
            }
        }
    }


    myfile.open("./dist2fac/facility-connection-distance.csv");

    myfile << "name" << ", longitude " << ",latitude" << ",district" << ",region" << ",tier" << ", testing " << ", Xpert" <<",up-connection" <<   ", time" << '\n';

    counter = 1;

    for (i = 0; i < facSize; i++) {

            myfile  << facArray[i].name << "," << facArray[i].longitude << "," << facArray[i].latitude << "," << facArray[i].district;
            myfile << "," << facArray[i].region << "," << facArray[i].tier << "," << facArray[i].testing;
            myfile << "," <<facArray[i].Xpert << "," << facArray[i].upName << "," << facArray[i].time << '\n';
    }

    myfile.close();






}



// see change run specific seed paramenters
unsigned getSeed(vector<vector<string>> runSpecific) {


    int runSpecificSize;
    unsigned seed = 0;
    
    runSpecificSize = runSpecific.size();

    // go through each line and either modify or append to appropriate vector
    for (int i = 0; i < runSpecificSize; i++) {
        
        if (DEBUG5) {
            std::cout << runSpecific[i][0] << ' ' << runSpecific[i][1] << ' ' << runSpecific[i][2] << '\n';
        }

        // column 0 has either DiseaseList, Disease or Test
        if (runSpecific[i][0] == "Seed") {

            if (DEBUG5) {
                std::cout << "inside seed " << '\n';
            }
            
            // update seed to use for this run
            seed = stoi(runSpecific[i][1]);

            if (DEBUG5) {
                std::cout << "inside seed = " << seed << '\n';
            }
        }

    }

    return seed;
}


// see change run specific diseaseList paramenters
vector<string> getDiseaseList(vector<vector<string>> runSpecific) {


    int runSpecificSize;
    int diseaseListSize;
    bool debugOn = false;
    vector<string> diseaseList;
    
    runSpecificSize = runSpecific.size();

    // go through each line and either modify or append to appropriate vector
    for (int i = 0; i < runSpecificSize; i++) {
        
        if (debugOn) {
            std::cout << runSpecific[i][0] << ' ' << runSpecific[i][1] << ' ' << runSpecific[i][2] << '\n';
        }

        // column 0 has either DiseaseList, Disease or Test
        if (runSpecific[i][0] == "DiseaseList") {

            diseaseListSize = stoi(runSpecific[i][1]);

            if (debugOn) {
                std::cout << "inside disease list - no. of diseaes =" << diseaseListSize << '\n';
            }
            
            // update list of diseases to use for this run
            // loop through disease list and  add it
            // diseases start at col 2 - so add 2 to start and end points of loop
            for (int j = 2; j < diseaseListSize + 2; j++) {

                diseaseList.push_back(runSpecific[i][j]);

                if (debugOn) std::cout << "Added " << runSpecific[i][j] << " to disease list. " << '\n';

            }
        }
    }

    return diseaseList;
}

// see change run specific diseaseList paramenters
void getTiers(vector<vector<string>> runSpecific) {


    int runSpecificSize;
    bool debugOn = false;
    
    runSpecificSize = runSpecific.size();

    // update diagnosis and testing tiers
    for (int i = 0; i < runSpecificSize; i++) {
        
        if (debugOn) {
            std::cout << runSpecific[i][0] << ' ' << runSpecific[i][1] << '\n';
        }

        // if column 0 is "diagnosis" update global parameters for diagnosis - this may have to be expanded later to seperate by disease
        // format - diagnosis, empty, empty, tier1, tier2, tier3, tier4, tier5, tier6, tier7, tier8, tier9, tier10
        if (runSpecific[i][0] == "diagnosis") {

            if (stoi(runSpecific[i][1]) == 1) {
                tier1Diagnosis = true;
            }
            else {
                tier1Diagnosis = false;
            }
            if (stoi(runSpecific[i][2]) == 1) {
                tier2Diagnosis = true;
            }
            else {
                tier2Diagnosis = false;
            }
            if (stoi(runSpecific[i][3]) == 1) {
                tier3Diagnosis = true;
            }
            else {
                tier3Diagnosis = false;
            }
            if (stoi(runSpecific[i][4]) == 1) {
                tier4Diagnosis = true;
            }
            else {
                tier4Diagnosis = false;
            }
            if (stoi(runSpecific[i][5]) == 1) {
                tier5Diagnosis = true;
            }
            else {
                tier5Diagnosis = false;
            }
            if (stoi(runSpecific[i][6]) == 1) {
                tier6Diagnosis = true;
            }
            else {
                tier6Diagnosis = false;
            }
            if (stoi(runSpecific[i][7]) == 1) {
                tier7Diagnosis = true;
            }
            else {
                tier7Diagnosis = false;
            }
            if (stoi(runSpecific[i][8]) == 1) {
                tier8Diagnosis = true;
            }
            else {
                tier8Diagnosis = false;
            }
            if (stoi(runSpecific[i][9]) == 1) {
                tier9Diagnosis = true;
            }
            else {
                tier9Diagnosis = false;
            }
            if (stoi(runSpecific[i][10]) == 1) {
                tier10Diagnosis = true;
            }
            else {
                tier10Diagnosis = false;
            }
            if (debugOn) {
                std::cout << "inside diagnosis list " <<  '\n';
            }
        }
        // if column 0 is "testing" update global parameters for testing for each disease and test
        // this indicates if testing is done for this disease and test is done at this tier        
        // format - testing, disease, test, tier1, tier2, tier3, tier4, tier5, tier6, tier7, tier8, tier9, tier10
        if (runSpecific[i][0] == "testing") {

            // break up by disease
            if (runSpecific[i][1] == "TB") {

                // break up by Test
                if (runSpecific[i][2] == "Xpert") {

                    if (stoi(runSpecific[i][3]) == 1) {
                        tier1TBTestingXpert = true;
                    }
                    else {
                        tier1TBTestingXpert = false;
                    }
                    if (stoi(runSpecific[i][4]) == 1) {
                        tier2TBTestingXpert = true;
                    }
                    else {
                        tier2TBTestingXpert = false;
                    }
                    if (stoi(runSpecific[i][5]) == 1) {
                        tier3TBTestingXpert = true;
                    }
                    else {
                        tier3TBTestingXpert = false;
                    }
                    if (stoi(runSpecific[i][6]) == 1) {
                        tier4TBTestingXpert = true;
                    }
                    else {
                        tier4TBTestingXpert = false;
                    }
                    if (stoi(runSpecific[i][7]) == 1) {
                        tier5TBTestingXpert = true;
                    }
                    else {
                        tier5TBTestingXpert = false;
                    }
                    if (stoi(runSpecific[i][8]) == 1) {
                        tier6TBTestingXpert = true;
                    }
                    else {
                        tier6TBTestingXpert = false;
                    }
                    if (stoi(runSpecific[i][9]) == 1) {
                        tier7TBTestingXpert = true;
                    }
                    else {
                        tier7TBTestingXpert = false;
                    }
                    if (stoi(runSpecific[i][10]) == 1) {
                        tier8TBTestingXpert = true;
                    }
                    else {
                        tier8TBTestingXpert = false;
                    }
                    if (stoi(runSpecific[i][11]) == 1) {
                        tier9TBTestingXpert = true;
                    }
                    else {
                        tier9TBTestingXpert = false;
                    }
                    if (stoi(runSpecific[i][12]) == 1) {
                        tier10TBTestingXpert = true;
                    }
                    else {
                        tier10TBTestingXpert = false;
                    }
                }
                else if (runSpecific[i][2] == "smear") {

                    if (stoi(runSpecific[i][3]) == 1) {
                        tier1TBTestingSmear = true;
                    }
                    else {
                        tier1TBTestingSmear = false;
                    }
                    if (stoi(runSpecific[i][4]) == 1) {
                        tier2TBTestingSmear = true;
                    }
                    else {
                        tier2TBTestingSmear = false;
                    }
                    if (stoi(runSpecific[i][5]) == 1) {
                        tier3TBTestingSmear = true;
                    }
                    else {
                        tier3TBTestingSmear = false;
                    }
                    if (stoi(runSpecific[i][6]) == 1) {
                        tier4TBTestingSmear = true;
                    }
                    else {
                        tier4TBTestingSmear = false;
                    }
                    if (stoi(runSpecific[i][7]) == 1) {
                        tier5TBTestingSmear = true;
                    }
                    else {
                        tier5TBTestingSmear = false;
                    }
                    if (stoi(runSpecific[i][8]) == 1) {
                        tier6TBTestingSmear = true;
                    }
                    else {
                        tier6TBTestingSmear = false;
                    }
                    if (stoi(runSpecific[i][9]) == 1) {
                        tier7TBTestingSmear = true;
                    }
                    else {
                        tier7TBTestingSmear = false;
                    }
                    if (stoi(runSpecific[i][10]) == 1) {
                        tier8TBTestingSmear = true;
                    }
                    else {
                        tier8TBTestingSmear = false;
                    }
                    if (stoi(runSpecific[i][11]) == 1) {
                        tier9TBTestingSmear = true;
                    }
                    else {
                        tier9TBTestingSmear = false;
                    }
                    if (stoi(runSpecific[i][12]) == 1) {
                        tier10TBTestingSmear = true;
                    }
                    else {
                        tier10TBTestingSmear = false;
                    }
                }
            }
            else if (runSpecific[i][1] == "HIV") {

                // break up by Test
                if (runSpecific[i][2] == "Xpert") {

                    if (stoi(runSpecific[i][3]) == 1) {
                        tier1HIVTestingXpert = true;
                    }
                    else {
                        tier1HIVTestingXpert = false;
                    }
                    if (stoi(runSpecific[i][4]) == 1) {
                        tier2HIVTestingXpert = true;
                    }
                    else {
                        tier2HIVTestingXpert = false;
                    }
                    if (stoi(runSpecific[i][5]) == 1) {
                        tier3HIVTestingXpert = true;
                    }
                    else {
                        tier3HIVTestingXpert = false;
                    }
                    if (stoi(runSpecific[i][6]) == 1) {
                        tier4HIVTestingXpert = true;
                    }
                    else {
                        tier4HIVTestingXpert = false;
                    }
                    if (stoi(runSpecific[i][7]) == 1) {
                        tier5HIVTestingXpert = true;
                    }
                    else {
                        tier5HIVTestingXpert = false;
                    }
                    if (stoi(runSpecific[i][8]) == 1) {
                        tier6HIVTestingXpert = true;
                    }
                    else {
                        tier6HIVTestingXpert = false;
                    }
                    if (stoi(runSpecific[i][9]) == 1) {
                        tier7HIVTestingXpert = true;
                    }
                    else {
                        tier7HIVTestingXpert = false;
                    }
                    if (stoi(runSpecific[i][10]) == 1) {
                        tier8HIVTestingXpert = true;
                    }
                    else {
                        tier8HIVTestingXpert = false;
                    }
                    if (stoi(runSpecific[i][11]) == 1) {
                        tier9HIVTestingXpert = true;
                    }
                    else {
                        tier9HIVTestingXpert = false;
                    }
                    if (stoi(runSpecific[i][12]) == 1) {
                        tier10HIVTestingXpert = true;
                    }
                    else {
                        tier10HIVTestingXpert = false;
                    }
                }
            }
            else if (runSpecific[i][1] == "HCV") {

                // break up by Test
                if (runSpecific[i][2] == "Xpert") {

                    if (stoi(runSpecific[i][3]) == 1) {
                        tier1HCVTestingXpert = true;
                    }
                    else {
                        tier1HCVTestingXpert = false;
                    }
                    if (stoi(runSpecific[i][4]) == 1) {
                        tier2HCVTestingXpert = true;
                    }
                    else {
                        tier2HCVTestingXpert = false;
                    }
                    if (stoi(runSpecific[i][5]) == 1) {
                        tier3HCVTestingXpert = true;
                    }
                    else {
                        tier3HCVTestingXpert = false;
                    }
                    if (stoi(runSpecific[i][6]) == 1) {
                        tier4HCVTestingXpert = true;
                    }
                    else {
                        tier4HCVTestingXpert = false;
                    }
                    if (stoi(runSpecific[i][7]) == 1) {
                        tier5HCVTestingXpert = true;
                    }
                    else {
                        tier5HCVTestingXpert = false;
                    }
                    if (stoi(runSpecific[i][8]) == 1) {
                        tier6HCVTestingXpert = true;
                    }
                    else {
                        tier6HCVTestingXpert = false;
                    }
                    if (stoi(runSpecific[i][9]) == 1) {
                        tier7HCVTestingXpert = true;
                    }
                    else {
                        tier7HCVTestingXpert = false;
                    }
                    if (stoi(runSpecific[i][10]) == 1) {
                        tier8HCVTestingXpert = true;
                    }
                    else {
                        tier8HCVTestingXpert = false;
                    }
                    if (stoi(runSpecific[i][11]) == 1) {
                        tier9HCVTestingXpert = true;
                    }
                    else {
                        tier9HCVTestingXpert = false;
                    }
                    if (stoi(runSpecific[i][12]) == 1) {
                        tier10HCVTestingXpert = true;
                    }
                    else {
                        tier10HCVTestingXpert = false;
                    }
                }
            }
            else if (runSpecific[i][1] == "Yellow Fever") {

                // break up by Test
                if (runSpecific[i][2] == "Xpert") {

                    if (stoi(runSpecific[i][3]) == 1) {
                        tier1YFTestingXpert = true;
                    }
                    else {
                        tier1YFTestingXpert = false;
                    }
                    if (stoi(runSpecific[i][4]) == 1) {
                        tier2YFTestingXpert = true;
                    }
                    else {
                        tier2YFTestingXpert = false;
                    }
                    if (stoi(runSpecific[i][5]) == 1) {
                        tier3YFTestingXpert = true;
                    }
                    else {
                        tier3YFTestingXpert = false;
                    }
                    if (stoi(runSpecific[i][6]) == 1) {
                        tier4YFTestingXpert = true;
                    }
                    else {
                        tier4YFTestingXpert = false;
                    }
                    if (stoi(runSpecific[i][7]) == 1) {
                        tier5YFTestingXpert = true;
                    }
                    else {
                        tier5YFTestingXpert = false;
                    }
                    if (stoi(runSpecific[i][8]) == 1) {
                        tier6YFTestingXpert = true;
                    }
                    else {
                        tier6YFTestingXpert = false;
                    }
                    if (stoi(runSpecific[i][9]) == 1) {
                        tier7YFTestingXpert = true;
                    }
                    else {
                        tier7YFTestingXpert = false;
                    }
                    if (stoi(runSpecific[i][10]) == 1) {
                        tier8YFTestingXpert = true;
                    }
                    else {
                        tier8YFTestingXpert = false;
                    }
                    if (stoi(runSpecific[i][11]) == 1) {
                        tier9YFTestingXpert = true;
                    }
                    else {
                        tier9YFTestingXpert = false;
                    }
                    if (stoi(runSpecific[i][12]) == 1) {
                        tier10YFTestingXpert = true;
                    }
                    else {
                        tier10YFTestingXpert = false;
                    }
                }
            }
            else if (runSpecific[i][1] == "Meningitis") {

                // break up by Test
                if (runSpecific[i][2] == "PCR") {

                    if (stoi(runSpecific[i][3]) == 1) {
                        tier1MenTestingPCR = true;
                    }
                    else {
                        tier1MenTestingPCR = false;
                    }
                    if (stoi(runSpecific[i][4]) == 1) {
                        tier2MenTestingPCR = true;
                    }
                    else {
                        tier2MenTestingPCR = false;
                    }
                    if (stoi(runSpecific[i][5]) == 1) {
                        tier3MenTestingPCR = true;
                    }
                    else {
                        tier3MenTestingPCR = false;
                    }
                    if (stoi(runSpecific[i][6]) == 1) {
                        tier4MenTestingPCR = true;
                    }
                    else {
                        tier4MenTestingPCR = false;
                    }
                    if (stoi(runSpecific[i][7]) == 1) {
                        tier5MenTestingPCR = true;
                    }
                    else {
                        tier5MenTestingPCR = false;
                    }
                    if (stoi(runSpecific[i][8]) == 1) {
                        tier6MenTestingPCR = true;
                    }
                    else {
                        tier6MenTestingPCR = false;
                    }
                    if (stoi(runSpecific[i][9]) == 1) {
                        tier7MenTestingPCR = true;
                    }
                    else {
                        tier7MenTestingPCR = false;
                    }
                    if (stoi(runSpecific[i][10]) == 1) {
                        tier8MenTestingPCR = true;
                    }
                    else {
                        tier8MenTestingPCR = false;
                    }
                    if (stoi(runSpecific[i][11]) == 1) {
                        tier9MenTestingPCR = true;
                    }
                    else {
                        tier9MenTestingPCR = false;
                    }
                    if (stoi(runSpecific[i][12]) == 1) {
                        tier10MenTestingPCR = true;
                    }
                    else {
                        tier10MenTestingPCR = false;
                    }
                }
            }
            else if (runSpecific[i][1] == "Measles") {

                // break up by Test
                if (runSpecific[i][2] == "culture") {

                    if (stoi(runSpecific[i][3]) == 1) {
                        tier1MeaTestingCulture = true;
                    }
                    else {
                        tier1MeaTestingCulture = false;
                    }
                    if (stoi(runSpecific[i][4]) == 1) {
                        tier2MeaTestingCulture = true;
                    }
                    else {
                        tier2MeaTestingCulture = false;
                    }
                    if (stoi(runSpecific[i][5]) == 1) {
                        tier3MeaTestingCulture = true;
                    }
                    else {
                        tier3MeaTestingCulture = false;
                    }
                    if (stoi(runSpecific[i][6]) == 1) {
                        tier4MeaTestingCulture = true;
                    }
                    else {
                        tier4MeaTestingCulture = false;
                    }
                    if (stoi(runSpecific[i][7]) == 1) {
                        tier5MeaTestingCulture = true;
                    }
                    else {
                        tier5MeaTestingCulture = false;
                    }
                    if (stoi(runSpecific[i][8]) == 1) {
                        tier6MeaTestingCulture = true;
                    }
                    else {
                        tier6MeaTestingCulture = false;
                    }
                    if (stoi(runSpecific[i][9]) == 1) {
                        tier7MeaTestingCulture = true;
                    }
                    else {
                        tier7MeaTestingCulture = false;
                    }
                    if (stoi(runSpecific[i][10]) == 1) {
                        tier8MeaTestingCulture = true;
                    }
                    else {
                        tier8MeaTestingCulture = false;
                    }
                    if (stoi(runSpecific[i][11]) == 1) {
                        tier9MeaTestingCulture = true;
                    }
                    else {
                        tier9MeaTestingCulture = false;
                    }
                    if (stoi(runSpecific[i][12]) == 1) {
                        tier10MeaTestingCulture = true;
                    }
                    else {
                        tier10MeaTestingCulture = false;
                    }
                }
            }
            else if (runSpecific[i][1] == "Disease1") {

                // break up by Test
                if (runSpecific[i][2] == "Test") {

                    if (stoi(runSpecific[i][3]) == 1) {
                        tier1Dis1TestingTest = true;
                    }
                    else {
                        tier1Dis1TestingTest = false;
                    }
                    if (stoi(runSpecific[i][4]) == 1) {
                        tier2Dis1TestingTest = true;
                    }
                    else {
                        tier2Dis1TestingTest = false;
                    }
                    if (stoi(runSpecific[i][5]) == 1) {
                        tier3Dis1TestingTest = true;
                    }
                    else {
                        tier3Dis1TestingTest = false;
                    }
                    if (stoi(runSpecific[i][6]) == 1) {
                        tier4Dis1TestingTest = true;
                    }
                    else {
                        tier4Dis1TestingTest = false;
                    }
                    if (stoi(runSpecific[i][7]) == 1) {
                        tier5Dis1TestingTest = true;
                    }
                    else {
                        tier5Dis1TestingTest = false;
                    }
                    if (stoi(runSpecific[i][8]) == 1) {
                        tier6Dis1TestingTest = true;
                    }
                    else {
                        tier6Dis1TestingTest = false;
                    }
                    if (stoi(runSpecific[i][9]) == 1) {
                        tier7Dis1TestingTest = true;
                    }
                    else {
                        tier7Dis1TestingTest = false;
                    }
                    if (stoi(runSpecific[i][10]) == 1) {
                        tier8Dis1TestingTest = true;
                    }
                    else {
                        tier8Dis1TestingTest = false;
                    }
                    if (stoi(runSpecific[i][11]) == 1) {
                        tier9Dis1TestingTest = true;
                    }
                    else {
                        tier9Dis1TestingTest = false;
                    }
                    if (stoi(runSpecific[i][12]) == 1) {
                        tier10Dis1TestingTest = true;
                    }
                    else {
                        tier10Dis1TestingTest = false;
                    }
                }
            }
            else if (runSpecific[i][1] == "Disease2") {

                // break up by Test
                if (runSpecific[i][2] == "Test") {

                    if (stoi(runSpecific[i][3]) == 1) {
                        tier1Dis2TestingTest = true;
                    }
                    else {
                        tier1Dis2TestingTest = false;
                    }
                    if (stoi(runSpecific[i][4]) == 1) {
                        tier2Dis2TestingTest = true;
                    }
                    else {
                        tier2Dis2TestingTest = false;
                    }
                    if (stoi(runSpecific[i][5]) == 1) {
                        tier3Dis2TestingTest = true;
                    }
                    else {
                        tier3Dis2TestingTest = false;
                    }
                    if (stoi(runSpecific[i][6]) == 1) {
                        tier4Dis2TestingTest = true;
                    }
                    else {
                        tier4Dis2TestingTest = false;
                    }
                    if (stoi(runSpecific[i][7]) == 1) {
                        tier5Dis2TestingTest = true;
                    }
                    else {
                        tier5Dis2TestingTest = false;
                    }
                    if (stoi(runSpecific[i][8]) == 1) {
                        tier6Dis2TestingTest = true;
                    }
                    else {
                        tier6Dis2TestingTest = false;
                    }
                    if (stoi(runSpecific[i][9]) == 1) {
                        tier7Dis2TestingTest = true;
                    }
                    else {
                        tier7Dis2TestingTest = false;
                    }
                    if (stoi(runSpecific[i][10]) == 1) {
                        tier8Dis2TestingTest = true;
                    }
                    else {
                        tier8Dis2TestingTest = false;
                    }
                    if (stoi(runSpecific[i][11]) == 1) {
                        tier9Dis2TestingTest = true;
                    }
                    else {
                        tier9Dis2TestingTest = false;
                    }
                    if (stoi(runSpecific[i][12]) == 1) {
                        tier10Dis2TestingTest = true;
                    }
                    else {
                        tier10Dis2TestingTest = false;
                    }
                }
            }
        }
    }

}

// see change run specific year parameters
void getYears(vector<vector<string>> runSpecific) {


    int runSpecificSize;

    bool debug = false;
   
    runSpecificSize = runSpecific.size();

    // go through each line and either modify or append to appropriate vector
    for (int i = 0; i < runSpecificSize; i++) {

        // column 0 has either Year, DiseaseList, Disease or Test
        if (runSpecific[i][0] == "Year") {

            if (debug) {
                std::cout << "inside getYear " << '\n';
            }

            // update year to start to use for this run
            startingYear = stoi(runSpecific[i][1]);

            // now update no. of years we will skip between the reference year and the starting year for the run;
            skipYears = startingYear - referenceYear;

            if (skipYears < 0) {
                skipYears = 0;
            }

            // update number of years to run to use for this run
            yearsInRun = stoi(runSpecific[i][2]) + skipYears;

            // update year to change scenarios if running split scenarios to use for this run
            splitYear = stoi(runSpecific[i][3]) + skipYears;

            // update no. of steps to run calculation
            noSteps = timestepsPerYear * yearsInRun;

            if (debug) {

                std::cout << "start year " << runSpecific[i][1] << " run for " << runSpecific[i][2] << " years and split at " << runSpecific[i][3] << '\n';
                std::cout << "startingYear = " << startingYear << " yearsInRun = " << yearsInRun << " splitYear = " << splitYear << '\n';
            }
        }

    }
}

// load scenario file, directory and name variables
scenarioParam getScenarioParameters(int scenario, scenarioParam params[]) {

    scenarioParam sParam;

    // assume all scenarios run through without switching to another 
    sParam.splitScenario = false;

    //std::cout << "inside getScenarioParameters scenario  = " << scenario <<  "\n";

    switch (scenario) {
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
    case 9:
    case 10:
    case 11:
        // load scenario values from scenarios 1-11
        sParam.statSubScenarioDir = params[scenario].statSubScenarioDir;
        sParam.statSubScenarioName = params[scenario].statSubScenarioName;
        sParam.statSubScenarioSplitName = params[scenario].statSubScenarioName;
        sParam.facilitySpecificNewFile = params[scenario].facilitySpecificFile;
        sParam.facilitySpecificFile = params[scenario].facilitySpecificFile;
        sParam.facilitySpecificSplitFile = params[scenario].facilitySpecificFile;
        break;
    case 21:
        // start at baseline scenario and switch to centralized scenario
        sParam.statSubScenarioDir = "Base-" + params[7].statSubScenarioDir;
        sParam.statSubScenarioName = params[9].statSubScenarioName;
        sParam.statSubScenarioSplitName = params[7].statSubScenarioName;
        sParam.facilitySpecificNewFile = params[9].facilitySpecificFile;
        sParam.facilitySpecificFile = params[9].facilitySpecificFile;
        sParam.facilitySpecificSplitFile = params[7].facilitySpecificFile;
        sParam.splitScenario = true;
        break;
    case 22:
        // start at baseline scenario and switch to centralized scenario
        sParam.statSubScenarioDir = "Base-" + params[8].statSubScenarioDir;
        sParam.statSubScenarioName = params[9].statSubScenarioName;
        sParam.statSubScenarioSplitName = params[8].statSubScenarioName;
        sParam.facilitySpecificNewFile = params[9].facilitySpecificFile;
        sParam.facilitySpecificFile = params[9].facilitySpecificFile;
        sParam.facilitySpecificSplitFile = params[8].facilitySpecificFile;
        sParam.splitScenario = true;
        break;
    case 23:
        // start at baseline scenario and switch to centralized scenario
        sParam.statSubScenarioDir = "Base-" + params[10].statSubScenarioDir;
        sParam.statSubScenarioName = params[9].statSubScenarioName;
        sParam.statSubScenarioSplitName = params[10].statSubScenarioName;
        sParam.facilitySpecificNewFile = params[9].facilitySpecificFile;
        sParam.facilitySpecificFile = params[9].facilitySpecificFile;
        sParam.facilitySpecificSplitFile = params[10].facilitySpecificFile;
        sParam.splitScenario = true;
        break;
    case 24:
        // start at baseline scenario and switch to baseline plus 29 DNO sites
        sParam.statSubScenarioDir = "Base-" + params[11].statSubScenarioDir;
        sParam.statSubScenarioName = params[9].statSubScenarioName;
        sParam.statSubScenarioSplitName = params[11].statSubScenarioName;
        sParam.facilitySpecificNewFile = params[9].facilitySpecificFile;
        sParam.facilitySpecificFile = params[9].facilitySpecificFile;
        sParam.facilitySpecificSplitFile = params[11].facilitySpecificFile;
        sParam.splitScenario = true;
        break;
        // added these four to bump job numbers for Rockfish
    case 91:
        // Xpert at tier 1, 2, 3 and some 4 only - no smear at tiers 4, 5, 6
        sParam.statSubScenarioDir = params[9].statSubScenarioDir;
        sParam.statSubScenarioName = params[9].statSubScenarioName;
        sParam.statSubScenarioSplitName = sParam.statSubScenarioName;
        sParam.facilitySpecificNewFile = params[9].facilitySpecificFile;
        sParam.facilitySpecificFile = params[9].facilitySpecificFile;
        //sParam.statSubDirNo = statSubDirNo + 10000;
        //sParam.statSubDir = to_string(statSubDirNo);
        break;
    case 92:
        // Xpert at tier 1, 2, 3 and some 4 only - no smear at tiers 4, 5, 6
        sParam.statSubScenarioDir = params[9].statSubScenarioDir;
        sParam.statSubScenarioName = params[9].statSubScenarioName;
        sParam.statSubScenarioSplitName = sParam.statSubScenarioName;
        sParam.facilitySpecificNewFile = params[9].facilitySpecificFile;
        sParam.facilitySpecificFile = params[9].facilitySpecificFile;
        //sParam.statSubDirNo = statSubDirNo + 20000;
        //sParam.statSubDir = to_string(statSubDirNo);
        break;
    case 93:
        // Xpert at tier 1, 2, 3 and some 4 only - no smear at tiers 4, 5, 6
        sParam.statSubScenarioDir = params[9].statSubScenarioDir;
        sParam.statSubScenarioName = params[9].statSubScenarioName;
        sParam.statSubScenarioSplitName = sParam.statSubScenarioName;
        sParam.facilitySpecificNewFile = params[9].facilitySpecificFile;
        sParam.facilitySpecificFile = params[9].facilitySpecificFile;
        //sParam.statSubDirNo = statSubDirNo + 30000;
        //sParam.statSubDir = to_string(statSubDirNo);
        break;
    case 94:
        // Xpert at tier 1, 2, 3 and some 4 only - no smear at tiers 4, 5, 6
        sParam.statSubScenarioDir = params[9].statSubScenarioDir;
        sParam.statSubScenarioName = params[9].statSubScenarioName;
        sParam.statSubScenarioSplitName = sParam.statSubScenarioName;
        sParam.facilitySpecificNewFile = params[9].facilitySpecificFile;
        sParam.facilitySpecificFile = params[9].facilitySpecificFile;
        //sParam.statSubDirNo = statSubDirNo + 40000;
        //sParam.statSubDir = to_string(statSubDirNo);
        break;
    default:
        // Xpert at tiers 1, 2, 3 and some 4 only - smear at tier 4
        sParam.statSubScenarioDir = params[1].statSubScenarioDir;
        sParam.statSubScenarioName = params[1].statSubScenarioName;
        sParam.statSubScenarioSplitName = sParam.statSubScenarioName;
        sParam.facilitySpecificNewFile = params[1].facilitySpecificFile;
        sParam.facilitySpecificFile = params[1].facilitySpecificFile;
    }

    //std::cout << " statSubScenarioDir " << sParam.statSubScenarioDir << " statSubScenarioName " << sParam.statSubScenarioName << " statSubScenarioSplitName " << sParam.statSubScenarioSplitName << "\n";
    //std::cout << " facilitySpecificNewFile " << sParam.facilitySpecificNewFile << " facilitySpecificFile " << sParam.facilitySpecificFile << " splitScenario " << sParam.splitScenario << "\n";

    return sParam;

}

// create region-district array from outside files
regionDistrict createRegionDistrictArrays(vector<vector<string>> regionDistricts) {

    
    int arraySize;
    int tmpDistrictSize;
    string district;
    bool debug = false;

    regionDistrict RD;

    // intialize global variables
    noRegions = 0;
    noDistricts = 0;

    noRegions = regionDistricts.size();

    // total statistics always first entry in array
    RD.regionDistrict[0] = "Total";

    // first add in the regions to the array
    for (int i = 1; i < noRegions; i++) {

        RD.regionDistrict[i] = cleanString(regionDistricts[i][1]);
        RD.regions[i - 1] = RD.regionDistrict[i];
        regDist.regions[i - 1] = RD.regionDistrict[i];
    }

    // start districts after regions
    arraySize = noRegions;

    // first add in the districts to the array
    for (int i = 1; i < noRegions; i++) {

        tmpDistrictSize = regionDistricts[i].size();
        
        for (int j = 2; j < tmpDistrictSize; j++) {

            district = regionDistricts[i][j];

            // remove end of line from string names
            district.erase(std::remove(district.begin(), district.end(), '\r'), district.end());
            district.erase(std::remove(district.begin(), district.end(), '\n'), district.end());

            // skip empty strings 
            if (!district.empty()) {

                // now clean district name - remove leading/trailing spaces, make uppercase
                RD.regionDistrict[arraySize] = cleanString(district);
                RD.districtToRegionMapping[noDistricts] = i;

                arraySize++;
                noDistricts++;
            }
        }
    }

    // adjust noRegions to remove header column
    noRegions = noRegions - 1;

    // debug - true
    if (debug) {
        std::cout << " noRegions " << noRegions << " noDistricts " << noDistricts << " arraySize " << arraySize << "\n";
        for (int i = 0; i < arraySize; i++) {

            std::cout << " i " << i << " " << RD.regionDistrict[i] << "\n";
        }
        for (int i = 0; i < noDistricts; i++) {

            std::cout << " i " << i << " " << RD.districtToRegionMapping[i] << "\n";
        }
        std::cout << "\n";
    }

    return RD;
}

// create facility name-ID array from facility list
regionDistrict createFacilitiesArray(FacilitiesList* list) {


    int index = 1;
    string district;
    bool debug = false;

    regionDistrict Fac;

    if (debug) std::cout << "inside createFacilitiesArray " << "\n";

    vector<Facility*> facilities = list->getListPtr();

    noFacilities = facilities.size();

    // total statistics always first entry in array
    Fac.regionDistrict[0] = "Total";
    Fac.districtToRegionMapping[0] = 0;

    // pull out name-id pairs
    for (auto facility : facilities) {

        Fac.regionDistrict[index] = facility->getName();;
        Fac.districtToRegionMapping[index] = stoi(facility->getID());
        index++;
    }

    return Fac;
}


// remove leading/trailing spaces, make uppercase
string cleanString(string word) {


    // make uppercase
    transform(word.begin(), word.end(), word.begin(), ::toupper);

    // remove leading spaces
    word.erase(word.begin(), std::find_if(word.begin(), word.end(), [](unsigned char c){return std::isspace(c);}));

    // remove trailing spaces
    word.erase(std::find_if(word.rbegin(), word.rend(), [](unsigned char c){return std::isspace(c);}).base(), word.end());
    
    return word;
}



