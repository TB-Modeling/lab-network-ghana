//
//  Main.cpp
//  Ghana facility ABM
//
//  Created by Trinity on 5/19/16.
//  Copyright © 2016 Trinity. All rights reserved.
//

#include <iostream>
#include <string>
#include <vector>
#include <random>
#include <sys/stat.h>
#include <errno.h> 

#include "GlobalParameters.h"
#include "FacilitiesList.h"
#include "FGlobal.h"
#include "InputOutput.h"

using namespace std;
//namespace fs = std::filesystem;



//enum regionDistrict { "National" ,

//    "GREATER ACCRA", "ASHANTI", "WESTERN", "SAVANNAH", "UPPER EAST", "CENTRAL", "EASTERN", "BONO", "UPPER WEST", "VOLTA", "NORTHERN", "AHAFO", "OTI", "BONO EAST", "WESTERN NORTH", "NORTHERN EAST",

//    "ACCRA METROPOLIS", "KUMASI METROPOLITAN", "SEKONDI TAKORADI METROPOLIS", "EAST GONJA MUNICIPAL", "BOLGATANGA MUNICIPAL", "CAPE COAST METROPOLITAN",
//    "NEW JUABEN SOUTH MUNICIPAL", "SUNYANI MUNICIPAL", "WA MUNICIPAL", "HO MUNICIPAL", "TAMALE METROPOLITAN", "KASENA NANKANA EAST",
//    "ELLEMBELLE", "TANO NORTH MUNICIPAL", "ADA EAST", "TALENSI", "KARAGA", "TAIN", "ASUNAFO SOUTH", "DORMAA WEST", "KWAEBIBIREM",
//    "GA WEST MUNICIPAL", "NKWANTA SOUTH MUNICIPAL", "EKUMFI", "KINTAMPO SOUTH", "BIA WEST", "DORMAA EAST", "WA WEST", "KETU SOUTH",
//    "KPANDAI", "EAST MAMPRUSI", "BAWKU MUNICIPAL", "NANUMBA NORTH", "BUNKPURUGU NAKPANDURI", "BONGO", "GUSHEGU", "SAGNERIGU",
//    "SABOBA", "BUILSA NORTH", "SAVELUGU", "WEST MAMPRUSI MUNICIPAL", "YENDI MUNICIPAL", "ZABZUGU", "BAWKU WEST", "ABURA-ASEBU-KWAMANKESE",
//    "AJUMAKO-ENYAN-ESSIAM",  "KOMENDA-EDINA-EGUAFO-ABIREM MUNICIPAL", "GOMOA WEST", "ASSIN CENTRAL MUNICIPAL", "MFANTSEMAN MUNICIPAL",
//    "EFFUTU MUNICIPAL", "AGONA WEST MUNICIPAL", "ASIKUMA-ODOBEN-BRAKWA", "TWIFO ATTI-MORKWA", "TARKWA NSUAEM", "WASSA EAST", "NZEMA EAST",
//    "AOWIN", "JOMORO", "AHANTA WEST MUNICIPAL", "PRESTEA/HUNI VALLEY", "WASSA AMENFI WEST", "SHAMA", "WASSA AMENFI EAST",
//    "OKAIKWEI NORTH MUNICIPAL", "KORLE KLOTTEY MUNICIPAL", "ASHAIMAN MUNICIPAL", "BIRIM CENTRAL MUNICIPAL", "LOWER MANYA KROBO",
//    "NSAWAM ADOAGYIRE MUNICIPAL", "GA CENTRAL MUNICIPAL", "WEST AKIM MUNICIPAL", "UPPER MANYA KROBO", "FANTEAKWA NORTH",
//    "TEMA METROPOLITAN", "LA-NKWANTANANG-MADINA", "LEDZOKUKU MUNICIPAL", "ATIWA EAST", "LA DADE-KOTOPON", "DENKYEMBOUR",
//    "TEMA WEST MUNICIPAL", "KWAHU SOUTH", "KWAHU WEST MUNICIPAL", "ABUAKWA SOUTH MUNICIPAL", "ABLEKUMA CENTRAL MUNICIPAL",
//    "GA EAST", "SUHUM MUNICIPAL", "AKWAPEM NORTH", "ABLEKUMA NORTH MUNICIPAL", "KPONE KATAMANSO", "ASANTE AKIM CENTRAL MUNICIPAL",
//    "ASANTE AKIM NORTH", "SEKYERE EAST", "BEKWAI MUNICIPAL", "OBUASI MUNICIPAL", "KWABRE EAST", "ATEBUBU AMANTIN",
//    "BIBIANI-ANHWIASO-BEKWAI MUNICIPAL", "UPPER DENKYIRA EAST MUNICIPAL", "BOSOME FREHO", "EJURA-SEKYEDUMASE", "EJISU MUNICIPAL",
//    "JUABEN MUNICIPAL", "ASANTE AKIM SOUTH", "BIRIM NORTH", "BOSOMTWE", "MAMPONG MUNICIPAL", "AHAFO ANO SOUTH WEST", "PRU WEST",
//    "ADANSI SOUTH", "ATWIMA MPONUA",  "SEKYERE SOUTH", "SEFWI-WIAWSO", "SENE WEST", "AMANSIE WEST", "AMANSIE CENTRAL",
//    "OFFINSO MUNICIPAL", "OLD TAFO MUNICIPAL", "TECHIMAN MUNICIPAL", "ASUNAFO NORTH MUNICIPAL",  "TANO SOUTH MUNICIPAL",
//    "BEREKUM EAST MUNICIPAL", "AHAFO ANO NORTH", "DORMAA MUNICIPAL", "WENCHI MUNICIPAL", "JUABOSO", "KINTAMPO NORTH MUNICIPAL",
//    "OFFINSO NORTH", "JAMAN NORTH", "ASUTIFI SOUTH", "JAMAN SOUTH MUNICIPAL",  "NKORANZA SOUTH", "NADOWLI-KALEO", "BOLE",
//    "WEST GONJA", "LAWRA", "NANDOM", "JIRAPA", "SISSALA EAST", "CENTRAL TONGU", "AKATSI SOUTH", "NORTH DAYI", "NORTH TONGU",
//    "SOUTH TONGU", "KWAHU AFRAM PLAINS NORTH", "HOHOE MUNICIPAL", "JASIKAN", "KETA MUNICIPAL", "KRACHI WEST", "KPANDO MUNICIPAL",
//    "KADJEBI", "SOUTH DAYI", "KETU NORTH", "BIAKOYE", "ASSIN NORTH", "PRU EAST", "TATALE SANGULI", "NEW JUABEN NORTH MUNICIPAL",
//    "EFFIA KWESIMINTSIM MUNICIPAL", "ASSIN SOUTH",  "AGONA EAST", "NINGO/PRAMPRAM", "SISSALA WEST", "OBUASI EAST", "OFORIKROM MUNICIPAL",
//    "ASOKWA MUNICIPAL", "GA NORTH MUNICIPAL", "KROWOR MUNICIPAL", "SUAME MUNICIPAL", "WEIJA GBAWE MUNICIPAL", "AYAWASO NORTH MUNICIPAL",
//    "CENTRAL GONJA", "ASOKORE MAMPONG MUNICIPAL", "SEKYERE KUMAWU", "UPPER DENKYIRA WEST", "ASUOGYAMAN", "YILO KROBO MUNICIPAL",
//    "GOMOA EAST", "ABUAKWA NORTH", "HO WEST", "ATWIMA NWABIAGYA SOUTH", "GA SOUTH MUNICIPAL", "AYAWASO WEST", "AWUTU SENYA EAST",
//    "ADA WEST", "SHAI OSUDOKU", "SAWLA-TUNA-KALBA", "LAMBUSSIE-KARNI", "CHEREPONI", "ADENTA MUNICIPAL", "AHAFO ANO SOUTH EAST",
//    "BEREKUM WEST", "GOMOA CENTRAL", "OKERE", "NANTON", "YUNYOO-NASUAN", "BOLGA  EAST", "TEMPANE", "NORTH EAST GONJA",
//    "NKWANTA NORTH", "SEKYERE AFRAM PLAINS NORTH", "BANDA", "TECHIMAN NORTH", "ASUTIFI NORTH", "SENE EAST", "KWAHU EAST",
//    "MAMPRUGU MOAGDURI", "KUMBUNGU", "TOLON", "MION", "NORTH GONJA", "BIRIM SOUTH", "BUILSA SOUTH", "AKYEM MANSA", "NABDAM",
//    "SEFWI AKONTOMBRA",  "DAFFIAMA BUSSIE ISSA", "UPPER WEST AKIM", "AYENSUANO", "KWAHU AFRAM PLAINS SOUTH", "AKWAPEM SOUTH",
//    "AFADZATO SOUTH", "ADANSI ASOKWA", "AKATSI NORTH", "AGOTIME ZIOPE", "KRACHI NCHUMURU", "SEKYERE CENTRAL", "AFIGYA KWABRE SOUTH",
//    "AWUTU SENYA WEST",  "KRACHI EAST MUNICIPAL", "TWIFO HEMANG LOWER DENKYIRA",  "WASSA AMENFI CENTRAL", "MPOHOR", "NKORANZA NORTH",
//    "BODI", "SUNYANI WEST", "BIA EAST", "SUAMAN",  "KASENA NANKANA WEST", "NANUMBA SOUTH", "WA EAST", "AMANSIE SOUTH", "AKROFUOM",
//    "ATWIMA NWABIAGYA NORTH", "AFIGYA KWABRE NORTH", "ANLOGA", "ACHIASE", "ASENE MANSO AKROSO", "ADANSI NORTH", "KWADASO MUNICIPAL",
//    "FANTEAKWA SOUTH", "ADAKLU", "ATWIMA KWANWOMA", "ATIWA WEST", "AYAWASO EAST MUNICIPAL", "PUSIGA", "BINDURI", "GARU",
//    "AYAWASO CENTRAL MUNICIPAL", "ABLEKUMA WEST MUNICIPAL"
//    };

// now stored in external file to handle different countries - ./defaults/input/Region-Districts.csv
// // noElementsDistrictRegionArray
//string regionDistrictArray[277]  = { "Total" ,
//
//    "GREATER ACCRA", "ASHANTI", "WESTERN", "SAVANNAH", "UPPER EAST", "CENTRAL", "EASTERN", "BONO", "UPPER WEST", "VOLTA", "NORTHERN", "AHAFO", "OTI", "BONO EAST", "WESTERN NORTH", "NORTH EAST",
//
//    "ACCRA METROPOLIS", "KUMASI METROPOLITAN", "SEKONDI TAKORADI METROPOLIS", "EAST GONJA MUNICIPAL", "BOLGATANGA MUNICIPAL", "CAPE COAST METROPOLITAN",
//    "NEW JUABEN SOUTH MUNICIPAL", "SUNYANI MUNICIPAL", "WA MUNICIPAL", "HO MUNICIPAL", "TAMALE", "KASENA NANKANA EAST",
//    "ELLEMBELLE", "TANO NORTH MUNICIPAL", "ADA EAST", "TALENSI", "KARAGA", "TAIN", "ASUNAFO SOUTH", "DORMAA WEST", "KWAEBIBIREM",
//    "GA WEST MUNICIPAL", "NKWANTA SOUTH MUNICIPAL", "EKUMFI", "KINTAMPO SOUTH", "BIA WEST", "DORMAA EAST", "WA WEST", "KETU SOUTH",
//    "KPANDAI", "EAST MAMPRUSI", "BAWKU MUNICIPAL", "NANUMBA NORTH", "BUNKPURUGU NAKPANDURI", "BONGO", "GUSHEGU", "SAGNERIGU",
//    "SABOBA", "BUILSA NORTH", "SAVELUGU", "WEST MAMPRUSI MUNICIPAL", "YENDI MUNICIPAL", "ZABZUGU", "BAWKU WEST", "ABURA-ASEBU-KWAMANKESE",
//    "AJUMAKO-ENYAN-ESSIAM",  "KOMENDA-EDINA-EGUAFO-ABIREM MUNICIPAL", "GOMOA WEST", "ASSIN CENTRAL MUNICIPAL", "MFANTSEMAN MUNICIPAL",
//    "EFFUTU MUNICIPAL", "AGONA WEST MUNICIPAL", "ASIKUMA-ODOBEN-BRAKWA", "TWIFO ATTI-MORKWA", "TARKWA NSUAEM", "WASSA EAST", "NZEMA EAST",
//    "AOWIN", "JOMORO", "AHANTA WEST MUNICIPAL", "PRESTEA-HUNI VALLEY", "WASSA AMENFI WEST", "SHAMA", "WASSA AMENFI EAST",
//    "OKAIKWEI NORTH MUNICIPAL", "KORLE KLOTTEY MUNICIPAL", "ASHAIMAN MUNICIPAL", "BIRIM CENTRAL MUNICIPAL", "LOWER MANYA KROBO",
//    "NSAWAM ADOAGYIRE MUNICIPAL", "GA CENTRAL MUNICIPAL", "WEST AKIM MUNICIPAL", "UPPER MANYA KROBO", "FANTEAKWA NORTH",
//    "TEMA METROPOLITAN", "LA-NKWANTANANG-MADINA", "LEDZOKUKU MUNICIPAL", "ATIWA EAST", "LA DADE-KOTOPON", "DENKYEMBOUR",
//    "TEMA WEST MUNICIPAL", "KWAHU SOUTH", "KWAHU WEST MUNICIPAL", "ABUAKWA SOUTH MUNICIPAL", "ABLEKUMA CENTRAL MUNICIPAL",
//    "GA EAST", "SUHUM MUNICIPAL", "AKWAPEM NORTH", "ABLEKUMA NORTH MUNICIPAL", "KPONE KATAMANSO", "ASANTE AKIM CENTRAL MUNICIPAL",
//    "ASANTE AKIM NORTH", "SEKYERE EAST", "BEKWAI MUNICIPAL", "OBUASI MUNICIPAL", "KWABRE EAST", "ATEBUBU AMANTIN",
//    "BIBIANI-ANHWIASO-BEKWAI MUNICIPAL", "UPPER DENKYIRA EAST MUNICIPAL", "BOSOME FREHO", "EJURA-SEKYEDUMASE", "EJISU MUNICIPAL",
//    "JUABEN MUNICIPAL", "ASANTE AKIM SOUTH", "BIRIM NORTH", "BOSOMTWE", "MAMPONG MUNICIPAL", "AHAFO ANO SOUTH WEST", "PRU WEST",
//    "ADANSI SOUTH", "ATWIMA MPONUA",  "SEKYERE SOUTH", "SEFWI-WIAWSO", "SENE WEST", "AMANSIE WEST", "AMANSIE CENTRAL",
//    "OFFINSO MUNICIPAL", "OLD TAFO MUNICIPAL", "TECHIMAN MUNICIPAL", "ASUNAFO NORTH",  "TANO SOUTH MUNICIPAL",
//    "BEREKUM EAST MUNICIPAL", "AHAFO ANO NORTH", "DORMAA MUNICIPAL", "WENCHI MUNICIPAL", "JUABOSO", "KINTAMPO NORTH MUNICIPAL",
//    "OFFINSO NORTH", "JAMAN NORTH", "ASUTIFI SOUTH", "JAMAN SOUTH MUNICIPAL",  "NKORANZA SOUTH", "NADOWLI-KALEO", "BOLE",
//    "WEST GONJA", "LAWRA", "NANDOM", "JIRAPA", "SISSALA EAST", "CENTRAL TONGU", "AKATSI SOUTH", "NORTH DAYI", "NORTH TONGU",
//    "SOUTH TONGU", "KWAHU AFRAM PLAINS NORTH", "HOHOE MUNICIPAL", "JASIKAN", "KETA MUNICIPAL", "KRACHI WEST", "KPANDO MUNICIPAL",
//    "KADJEBI", "SOUTH DAYI", "KETU NORTH", "BIAKOYE", "ASSIN NORTH", "PRU EAST", "TATALE SANGULI", "NEW JUABEN NORTH MUNICIPAL",
//    "EFFIA KWESIMINTSIM MUNICIPAL", "ASSIN SOUTH",  "AGONA EAST", "NINGO PRAMPRAM", "SISSALA WEST", "OBUASI EAST", "OFORIKROM MUNICIPAL",
//    "ASOKWA MUNICIPAL", "GA NORTH MUNICIPAL", "KROWOR MUNICIPAL", "SUAME MUNICIPAL", "WEIJA GBAWE MUNICIPAL", "AYAWASO NORTH MUNICIPAL",
//    "CENTRAL GONJA", "ASOKORE MAMPONG MUNICIPAL", "SEKYERE KUMAWU", "UPPER DENKYIRA WEST", "ASUOGYAMAN", "YILO KROBO MUNICIPAL",
//    "GOMOA EAST", "ABUAKWA NORTH", "HO WEST", "ATWIMA NWABIAGYA SOUTH", "GA SOUTH MUNICIPAL", "AYAWASO WEST", "AWUTU SENYA EAST",
//    "ADA WEST", "SHAI OSUDOKU", "SAWLA-TUNA-KALBA", "LAMBUSSIE-KARNI", "CHEREPONI", "ADENTA MUNICIPAL", "AHAFO ANO SOUTH EAST",
//    "BEREKUM WEST", "GOMOA CENTRAL", "OKERE", "NANTON", "YUNYOO-NASUAN", "BOLGA EAST", "TEMPANE", "NORTH EAST GONJA",
//    "NKWANTA NORTH", "SEKYERE AFRAM PLAINS NORTH", "BANDA", "TECHIMAN NORTH", "ASUTIFI NORTH", "SENE EAST", "KWAHU EAST",
//    "MAMPRUGU MOAGDURI", "KUMBUNGU", "TOLON", "MION", "NORTH GONJA", "BIRIM SOUTH", "BUILSA SOUTH", "AKYEM MANSA", "NABDAM",
//    "SEFWI AKONTOMBRA",  "DAFFIAMA BUSSIE ISSA", "UPPER WEST AKIM", "AYENSUANO", "KWAHU AFRAM PLAINS SOUTH", "AKWAPEM SOUTH",
//    "AFADZATO SOUTH", "ADANSI ASOKWA", "AKATSI NORTH", "AGOTIME ZIOPE", "KRACHI NCHUMURU", "SEKYERE CENTRAL", "AFIGYA KWABRE SOUTH",
//    "AWUTU SENYA WEST",  "KRACHI EAST MUNICIPAL", "TWIFO HEMANG LOWER DENKYIRA",  "WASSA AMENFI CENTRAL", "MPOHOR", "NKORANZA NORTH",
//    "BODI", "SUNYANI WEST", "BIA EAST", "SUAMAN",  "KASENA NANKANA WEST", "NANUMBA SOUTH", "WA EAST", "AMANSIE SOUTH", "AKROFUOM",
//    "ATWIMA NWABIAGYA NORTH", "AFIGYA KWABRE NORTH", "ANLOGA", "ACHIASE", "ASENE MANSO AKROSO", "ADANSI NORTH", "KWADASO MUNICIPAL",
//    "FANTEAKWA SOUTH", "ADAKLU", "ATWIMA KWANWOMA", "ATIWA WEST", "AYAWASO EAST MUNICIPAL", "PUSIGA", "BINDURI", "GARU",
//    "AYAWASO CENTRAL MUNICIPAL", "ABLEKUMA WEST MUNICIPAL"
//    };
//    


bool debug = false;
bool debug1 = false;
bool debugYearly = true;
bool debugFinal = true;



FacilitiesList* facilitiesList = new FacilitiesList();
// facilitiesList = new FacilitiesList();


#include "InputOutput.h"
#include "InputChange.h"
#include "ChangeParameters.h"
#include "CreateAgents.h"
#include "CreateAgentStats.h"
#include "CreateSampleStats.h"
#include "CreateResourceStats.h"
#include "CreateParameterStats.h"
#include "CreateTableauStats.h"
#include "CreateVisualizationStats.h"
#include "MapFacilities.h"
#include "District.h"
#include "Region.h"
#include "Clean.h"
#include "Dump.h"



const std::string myCurrentDateTime() {
    time_t     now = time(0);
    struct tm  tstruct;
    char       buf[80];
    tstruct = *localtime(&now);
    strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct);
    return buf;
}

// generate a random number using mt19937 marisenne twister algorithm
// find next number in sequence - will get slower the longer it runs
void makeRandomNo100( unsigned seed, int max ) {
   
   int startingPt;
   int endingPt;
   int count = 0;
   
   std::mt19937 generator(seed);
   std::uniform_int_distribution<int> randDist100(1, max);
    
   //std::cout << "inside make random no 100 nextNoSeq100 = " << nextNoSeq100 << "\n";
   
   // grab the next chunk of random numbers starting at where we left off last time + how many we have used from the last group this time
   startingPt = countSeq100 + nextNoSeq100;
   endingPt = startingPt + maxNoSeq100;
   countSeq100 = startingPt;
   nextNoSeq100 = 0;
   
   //std::cout << "inside make random no 100 startingPt = " << startingPt << ", endingPt = " << endingPt  << "\n";

   for (int i = 0; i < endingPt; i++) {
    
        if (i > startingPt) {
        
            seq100[count] = randDist100(generator);
            count++;
        }
   }
    
}

void makeRandomNoPop( unsigned seed, vector<string> diseaseList ) {

   int startingPt;
   int endingPt;
   int count;
   bool popDebug = false;

   std::mt19937 generator(seed);
   
  // std::cout << "inside makeRandomNoPop " << "\n";
   
   // loop through disease list and make a random number array for each disease based on population and disease incidence
   for (auto disease : diseaseList) {

        // only update the random numbers for the diseases we are studying
        if (disease == "TB") {
   
            std::uniform_int_distribution<int> randDistPopTB(1, popTB);
   
            // grab the next chunk of random numbers starting at where we left off last time + how many we have used from the last group this time
            startingPt = countSeqPopTB + nextNoSeqPopTB;
            endingPt = startingPt + maxNoSeqPop;
            countSeqPopTB = startingPt;
            nextNoSeqPopTB = 0;
            count = 0;
            if (popDebug) std::cout << "inside make random tb startingPt = " << startingPt << ", endingPt = " << endingPt  << ", popTB = " << popTB  << "\n";

            for(int i = 0; i < endingPt; i++) {
    
                    if (i > startingPt) {

                        seqPopTB[count] = randDistPopTB(generator);
                        //std::cout << " " << seqPopTB[count];
                        count++;
                    }
            }
            if (popDebug) std::cout << "inside after make random tb startingPt " << "\n";
        }
        else if (disease == "HIV") {

            if (popDebug) std::cout << "inside make random HIV before randDistPopHIV" << " popHIV = " << popHIV << "\n";
            
            std::uniform_int_distribution<int> randDistPopHIV(1, popHIV);
    
            startingPt = countSeqPopHIV + nextNoSeqPopHIV;
            endingPt = startingPt + maxNoSeqPop;
            countSeqPopHIV = startingPt;
            nextNoSeqPopHIV = 0;
            count = 0;
            if (popDebug) std::cout << "inside make random HIV startingPt = " << startingPt << ", endingPt = " << endingPt  << ", popHIV = " << popHIV << "\n";

            for(int i = 0; i < endingPt; i++) {
    
                if (i > startingPt) {

                    seqPopHIV[count] = randDistPopHIV(generator);
                    count++;
                }
            }
            if (popDebug) std::cout << "inside after make random hiv startingPt " << "\n";
        }
        else if (disease == "HCV") {

            std::uniform_int_distribution<int> randDistPopHCV(1, popHCV);
    
            startingPt = countSeqPopHCV + nextNoSeqPopHCV;
            endingPt = startingPt + maxNoSeqPop;
            countSeqPopHCV = startingPt;
            nextNoSeqPopHCV = 0;
            count = 0;
            if (popDebug) std::cout << "inside make random HCV startingPt = " << startingPt << ", endingPt = " << endingPt  << "\n";

            for(int i = 0; i < endingPt; i++) {
    
                if (i > startingPt) {

                seqPopHCV[count] = randDistPopHCV(generator);
                count++;
                }
            }
            if (popDebug) std::cout << "inside after make random hcv startingPt " << "\n";
        }
        else if (disease == "Yellow Fever") {

            std::uniform_int_distribution<int> randDistPopYF(1, popYF);
    
            startingPt = countSeqPopYF + nextNoSeqPopYF;
            endingPt = startingPt + maxNoSeqPop;
            countSeqPopYF = startingPt;
            nextNoSeqPopYF = 0;
            count = 0;
            if (popDebug) std::cout << "inside make random yellow fever startingPt = " << startingPt << ", endingPt = " << endingPt << "\n";

            for(int i = 0; i < endingPt; i++) {
    
                if (i > startingPt) {

                    seqPopYF[count] = randDistPopYF(generator);
                    count++;
                }
            }
            if (popDebug) std::cout << "inside after make random yellow fever startingPt " << "\n";
        }
        else if (disease == "Meningitis") {

            std::uniform_int_distribution<int> randDistPopMen(1, popMen);
    
            startingPt = countSeqPopMen + nextNoSeqPopMen;
            endingPt = startingPt + maxNoSeqPop;
            countSeqPopMen = startingPt;
            nextNoSeqPopMen = 0;
            count = 0;
            if (popDebug) std::cout << "inside make random meningitis startingPt = " << startingPt << ", endingPt = " << endingPt << "\n";

            for(int i = 0; i < endingPt; i++) {
    
                if (i > startingPt) {

                    seqPopMen[count] = randDistPopMen(generator);
                    count++;
                }
            }
            if (popDebug) std::cout << "inside after make random meningitis startingPt " << "\n";
        }
        else if (disease == "Measles") {
            std::uniform_int_distribution<int> randDistPopMea(1, popMea);
    
            startingPt = countSeqPopMea + nextNoSeqPopMea;
            endingPt = startingPt + maxNoSeqPop;
            countSeqPopMea = startingPt;
            nextNoSeqPopMea = 0;
            count = 0;
            if (popDebug) std::cout << "inside make random measles startingPt = " << startingPt << ", endingPt = " << endingPt << "\n";

            for(int i = 0; i < endingPt; i++) {
    
                if (i > startingPt) {

                    seqPopMea[count] = randDistPopMea(generator);
                    count++;
                }
            }
            if (popDebug) std::cout << "inside after make random measles startingPt " << "\n";
        }
        else if (disease == "Disease1") {
            std::uniform_int_distribution<int> randDistPopDis1(1, popDis1);

            startingPt = countSeqPopDis1 + nextNoSeqPopDis1;
            endingPt = startingPt + maxNoSeqPop;
            countSeqPopDis1 = startingPt;
            nextNoSeqPopDis1 = 0;
            count = 0;
            if (popDebug) std::cout << "inside make random disease1 startingPt = " << startingPt << ", endingPt = " << endingPt << "\n";

            for (int i = 0; i < endingPt; i++) {

                if (i > startingPt) {

                    seqPopDis1[count] = randDistPopDis1(generator);
                    count++;
                }
            }
            if (popDebug) std::cout << "inside after make random disease1 startingPt " << "\n";
        }
        else if (disease == "Disease2") {
            std::uniform_int_distribution<int> randDistPopDis2(1, popDis2);

            startingPt = countSeqPopDis2 + nextNoSeqPopDis2;
            endingPt = startingPt + maxNoSeqPop;
            countSeqPopDis2 = startingPt;
            nextNoSeqPopDis2 = 0;
            count = 0;
            if (popDebug) std::cout << "inside make random disease2 startingPt = " << startingPt << ", endingPt = " << endingPt << "\n";

            for (int i = 0; i < endingPt; i++) {

                if (i > startingPt) {

                    seqPopDis2[count] = randDistPopDis2(generator);
                    count++;
                }
            }
            if (popDebug) std::cout << "inside after make random disease2 startingPt " << "\n";
        }
    }
}

// generate a exponential random numbers
// find next number in sequence - will get slower the longer it runs
//void makeRandomExpNo(unsigned seed) {
//
//    int startingPt;
//    int endingPt;
//    int count = 0;
//    double expRandNo;
//
//    // exponential distribution 0-10 years = 1-3655 timesteps
//    // p(x|λ) =λe−λx
//    std::default_random_engine expGen(seed);
//    std::exponential_distribution<double> distribution(3.5);
//
//
//    //std::cout << "inside make random exponential nextNoExpSeq = " << nextNoSeq100 << "\n";
//
//    // grab the next chunk of random numbers starting at where we left off last time + how many we have used from the last group this time
//    startingPt = countExpSeq + nextNoExpSeq;
//    endingPt = startingPt + maxNoExpSeq;
//    countExpSeq = startingPt;
//    nextNoExpSeq = 0;
//
//    //std::cout << "inside make random exponential startingPt = " << startingPt << ", endingPt = " << endingPt  << "\n";
//
//    for (int i = 0; i < endingPt; i++) {
//
//        if (i > startingPt) {
//
//            expRandNo = distribution(expGen);
//            expSeq[count] = int(expRandNo * 3600);
//            count++;
//        }
//    }
//
//}



// generate geometric random numbers
// find next number in sequence - will get slower the longer it runs
void makeRandomGeoNo(unsigned seed, float pRepresentation) {

    int startingPt;
    int endingPt;
    int count = 0;

    // geometric distribution
    // P(i|p) = p·(1−p)i
    //std::default_random_engine geoGen;
    std::random_device rd;
    std::mt19937 geoGen(rd());
    std::geometric_distribution<int> distribution(pRepresentation);

    //std::cout << "inside make random geometric nextNoGeoSeq = " << nextNoGeoSeq << " max = " << distribution.max() << " min = " << distribution.min() <<  "\n";

    // grab the next chunk of random numbers starting at where we left off last time + how many we have used from the last group this time
    startingPt = countGeoSeq + nextNoGeoSeq;
    endingPt = startingPt + maxNoGeoSeq;
    countGeoSeq = startingPt;
    nextNoGeoSeq = 0;

    //std::cout << "inside make random geometric startingPt = " << startingPt << ", endingPt = " << endingPt  << "\n";

    for (int i = 0; i < endingPt; i++) {

        if (i > startingPt) {

            // returns number of times coin flipped until desired result - add 1 to include found selection fip
            geoSeq[count] = distribution(geoGen) + 1;
            count++;
        }
    }

}

// generate geometric random numbers
// find next number in sequence - will get slower the longer it runs
void outputRandomGeoNo(unsigned seed, float pRepresentation) {

    int startingPt = 0;
    int endingPt = 100000;
    int number = 0;
    ofstream myfile;
    ofstream myfile1;

    // geometric distribution
    // P(i|p) = p·(1−p)i
    //std::default_random_engine geoGen;
    std::random_device rd;
    std::mt19937 geoGen(rd());
    std::mt19937 geoGen1(rd());
    std::geometric_distribution<int> distribution(pRepresentation);

    myfile.open("./geoDistNo.csv");

    for (int i = startingPt; i < endingPt; i++) {

            number = distribution(geoGen) + 1;
            myfile << number << "\n";
    }
    //myfile << "\n";

    myfile.close();

    //int rng_geo(mt19937 & rng, const double probabilityOfSuccess, const bool includeSuccessAttemptInTotal = true) {
    //    if (probabilityOfSuccess == 0)         
    //        return 1000000000; 
    //    //we return a large positive number so that the time of event is set for a long time from now; use 1,000,000,000 
    // geometric_distribution<int> dist(probabilityOfSuccess);     
    //    auto nFailuresUntilSuccess = dist(rng);     
    //    int nAttempts = !includeSuccessAttemptInTotal ? nFailuresUntilSuccess : nFailuresUntilSuccess + 1;     
    //    return nAttempts; 
    //}
    geometric_distribution<int> dist(pRepresentation);
    myfile1.open("./geoDistNo1.csv");

    for (int i = startingPt; i < endingPt; i++) {

        auto nFailuresUntilSuccess = dist(geoGen1) + 1;
        myfile1 << nFailuresUntilSuccess << "\n";
    }
    //myfile << "\n";

    myfile1.close();


}


void checkRandomNumbers( unsigned seed, vector<string> diseaseList, float pRepresent) {

    float percentage = .65;
    
    //std::cout << "inside checkRandomNumbers nextNoSeq100 = " << nextNoSeq100 << ", maxNoSeq100 * percentage = " << maxNoSeq100 * percentage  << "\n";
    
    // first check to see if 1-100 random number array is within 35% of end
    if (nextNoSeq100 > (maxNoSeq100 * percentage)) {

        // fill in the 1-100 array with the next batch of numbers
        makeRandomNo100( seed, 100 );
    }

    //std::cout << "inside checkRandomNumbers nextNoSeqPopTB = " << nextNoSeqPopTB << ", maxNoSeqPop * percentage = " << maxNoSeqPop * percentage  << "\n";
    
    // now check population random array - if any are within 35% of end, regenerate All
    if (nextNoSeqPopTB > (maxNoSeqPop * percentage)) {

        //fill in the pop array with next batch of numbers
        makeRandomNoPop( seed, diseaseList );
    }
    else if (nextNoSeqPopHIV > (maxNoSeqPop * percentage)) {

        //fill in the pop array with next batch of numbers
        makeRandomNoPop( seed, diseaseList );
    }
    else if (nextNoSeqPopHCV > (maxNoSeqPop * percentage)) {

        //fill in the pop array with next batch of numbers
        makeRandomNoPop( seed, diseaseList );
    }
    else if (nextNoSeqPopYF > (maxNoSeqPop * percentage)) {

        //fill in the pop array with next batch of numbers
        makeRandomNoPop( seed, diseaseList );
    }
    else if (nextNoSeqPopMen > (maxNoSeqPop * percentage)) {

        //fill in the pop array with next batch of numbers
        makeRandomNoPop( seed, diseaseList );
    }
    else if (nextNoSeqPopMea > (maxNoSeqPop * percentage)) {

        //fill in the pop array with next batch of numbers
        makeRandomNoPop( seed, diseaseList );
    }
    else if (nextNoSeqPopDis1 > (maxNoSeqPop * percentage)) {

        //fill in the pop array with next batch of numbers
        makeRandomNoPop(seed, diseaseList);
    }
    else if (nextNoSeqPopDis2 > (maxNoSeqPop * percentage)) {

        //fill in the pop array with next batch of numbers
        makeRandomNoPop(seed, diseaseList);
    }


    // first check to see if 1-100 random exponential number array is within 35% of end
    //if (nextNoExpSeq > (maxNoExpSeq * percentage)) {

        // fill in the exponential array with the next batch of numbers
       // makeRandomExpNo(seed);
    //}

    // first check to see if random geometric number array is within 35% of end
    if (nextNoGeoSeq > (maxNoGeoSeq * percentage)) {

        // fill in the exponential array with the next batch of numbers
        makeRandomGeoNo(seed, pRepresent);
    }
}












int main(int argc, const char * argv[]) {
    
    
    //int b=atoi(argv[2]);
    // cout<<"runing scenario "<<a<<"to"<<b<<endl;

    
    //int timeStep = 0;  // make global
    int year;
    int scenarioCount = 1;
    int tmpIndex;
    int scenarioIndex;
    int countStatDiseases = 0;
    float population = 0;
    float tempIncidence = 0;
    string tempFacName = "";
    string tempInputArg = "";
    bool combinedFlag = false;
    bool readSnapshotFlag = false;
    bool rerunInputFlag = false;

    // true - run a split scenario, first run baseline for a few years, then run another scenario
    bool splitScenario = false;

    // control statistical output
    bool agentStatsFlag = false;
    bool agentPatientsStatsFlag = false;
    bool agentPatientsSamplingStatsFlag = false;
    bool sampleStatsFlag = false;
    bool vizStatFlag = false;
    bool sampleInactiveStatsFlag = false;
    bool resourceStatsFlag = false;
    bool tableauStatsFlag = false;

    // change MD override option
    bool MDClinicDiagFlag = true;

    // intialize scenario array 
    // scenario[0] - Xpert at tiers 1, 2, 3 only - no smear
    // scenario[1] - Xpert at tiers 1, 2, 3, 4 only - no smear
    // scenario[2] - Xpert at tiers 1, 2, 3 and some 4 only - smear at tier 4
    // scenario[3] - Xpert at tiers 1, 2, 3 only - no smear
    // scenario[4] - Xpert at tiers 1, 2, 3, 4 only - no smear
    // scenario[5] - Xpert at tiers 1, 2, 3 and some 4 only - smear at tier 4
    // scenario[6] - Xpert at tiers 1, 2, 3 only - no smear
    // scenario[7] - Xpert at tiers 1, 2, 3, 4 only - no smear
    // scenario[8] - Xpert at tiers 1, 2, 3 and some 4 only - smear at tier 4
    int scenarioNo[9] = {3,0,0,0,0,0,0,0,0};

    // create structure for all input scenarios
    scenarioParam scenParam[12];

    // create newTravelTimes.csv from accessmod catchment point file (accessibility file that is modified in R)
    bool imageFlag = false;

    // update facilities list
    bool facilitiesListFlag = false;

    // read in facility mapping file and distance between facilities
    // find distance to nearinst xpert machine
    bool distBtwnFacFlag = false;

    // flag to create master list from various facility lists
    //  ./main HeFra.csv DHIMS-10k.csv ghana-hdx.csv ghana-healthsites.csv ghana-maina.csv ghana-openstreetmap.csv ghana-whitepages.csv ghana-WHO-TIME-district-hospitals.csv
    bool facilitiesList2Flag = false;

    bool facilitiesList3Flag = false;

    // flag to add gmaps output to master list
    bool facilitiesList4Flag = false;
    
    // find closest regional hospital to facility and output
    bool facilitiesList5Flag = false;

    vector<Disease*> dis;
    vector<Test*> tes;
    vector<Facility*> fac;
    vector<District*> dist;
    vector<Region*> reg;
    vector<string> diseaseList;
    //facilitiesList = new FacilitiesList();

    int timeStepStatOutput = timestepsPerYear;

    // input files
    string temp;
    //string statSubScenarioDir;
    string statSubScenarioDir1;
    string statSubScenarioDir2;
    string statSubScenarioDir3;
    string statSubScenarioDir4;
    string statSubScenarioDir5;
    string statSubScenarioDir6;
    string statSubScenarioDir7;
    string statSubScenarioDir8;
    string statSubScenarioDir9;
    string statSubScenarioDir10;
    string statSubScenarioDir11;
    string statSubScenarioName;
    string statSubScenarioName1;
    string statSubScenarioName2;
    string statSubScenarioName3;
    string statSubScenarioName4;
    string statSubScenarioName5;
    string statSubScenarioName6;
    string statSubScenarioName7;
    string statSubScenarioName8;
    string statSubScenarioName9;
    string statSubScenarioName10;
    string statSubScenarioName11;
    string statSubScenarioSplitName;
    string lastStatSubScenarioDir;
    string subDirBase;
    string subDirBaseScenario;
    string lastSubDirBase;
    string lastSubDirBaseScenario;
    string subDirBaseInput;
    string subDirBaseCatch;
    string subDirBaseDis;
    string subDirBaseFac;
    string subDirBaseFacSpec;
    string subDirBasePop;
    string subDirBaseRegDis;
    string subDirBaseRun;
    string subDirBaseTest; 
    string subDirBaseChange;
    string facilityProjectDir = "./facility-project/";
    string defaultDir = "./defaults/";
    string inputDir = "./input/";
    string filenameDir = "input-file-names/";
    string diseaseDir = "diseases/";
    string testDir = "tests/";
    string facilitiesDir = "facilities/";
    string populationDir = "population/";
    string popBinDir = "catchment/";
    string facilitySpecificDir = "facility-specific-data/";
    string regionDistrictSpecificDir = "region-district-specific-data/";
    string runSpecificDir = "run-specific-data/";
    string parameterDir = "input-change-parameters/";
    string filenameFile = "input-file-names.csv";
    string regDistFile = "Region-Districts.csv";
    string diseaseFile = "diseases.csv";
    string diseaseTBUntreatedMortFile = "survivalMortality_untreatedTB.csv";
    string testFile = "tests.csv";
    //string facilitiesFile = "facilities.csv";
    //string facilitiesFile = "case-reg-703-with-dist.csv";
    string facilitiesFile = "case-reg-853-with-dist.csv";
    //string facilitiesFileNoTop = "facilities-no-top-layer.csv";
    //string facilitiesFileNoTop = "case-reg-703-with-dist-no-top-layer.csv";
    string facilitiesFileNoTop = "case-reg-853-with-dist-no-top-layer.csv";
    string populationFile = "pop_1km_array_2015.csv";
    string travelTimesFile = "newTravelTimes.csv";
    string popBinFile = "Facility-Catchment-Population-Distribution.csv";
    string facilitySpecificFile = "facility-specific-data-current-model-169-smear-testing1234-xpert-testing.csv";
    string facilitySpecificNewFile = "facility-specific-data-current-model-169-smear-testing1234-xpert-testing.csv";
    string facilitySpecificSplitFile = "facility-specific-data-current-model-169-smear-testing1234-xpert-testing.csv";
    string regionDistrictSpecificFile = "region-district-specific-data.csv";
    string runSpecificFile = "run-specific-data.csv";
    //string parameterFlagFile = "parameter-range.csv";
    string parameterFlagFile = "priorRangesAndDistributions.csv";


    // debug - true
    if (debug) {

        std::cout << "popBinFile file = " << popBinFile << '\n';
        std::cout << "diseaseFile file = " << diseaseFile << '\n';
        std::cout << "facilitiesFile file = " << facilitiesFile << '\n';
        std::cout << "facilitySpecificFile file = " << facilitySpecificFile << '\n';
        std::cout << "facilitySpecificNewFile file = " << facilitySpecificNewFile << '\n';
        std::cout << "facilitySpecificSplitFile file = " << facilitySpecificSplitFile << '\n';
        std::cout << "parameterFlagFile file = " << parameterFlagFile << '\n';
        std::cout << "populationFile file = " << populationFile << '\n';
        std::cout << "travelTimesFile file = " << travelTimesFile << '\n';
        std::cout << "regionDistrictSpecificFile file = " << regionDistrictSpecificFile << '\n';
        std::cout << "runSpecificFile file = " << runSpecificFile << '\n';
        std::cout << "test file = " << testFile << '\n';
    }


    // read in the input file names 
    string filenameInput = defaultDir + inputDir + filenameDir + filenameFile;
    vector<vector<string>> filenames = readDataInput(filenameInput);
    for (std::size_t i = 0; i < filenames.size(); i++) {

        //std::cout << filenames[i][0] << " - " << filenames[i][1] << std::endl;

        if (filenames[i][0] == "catchment") {

            popBinFile = filenames[i][1];
        }
        else if (filenames[i][0] == "diseases") {

            diseaseFile = filenames[i][1];
        }
        else if (filenames[i][0] == "facilities") {

            facilitiesFile = filenames[i][1];
        }
        else if (filenames[i][0] == "facility-specific-data") {
            
            scenParam[stoi(filenames[i][3])].facilitySpecificFile = filenames[i][1];
            scenParam[stoi(filenames[i][3])].statSubScenarioDir = filenames[i][2];
            scenParam[stoi(filenames[i][3])].statSubScenarioName = filenames[i][2];
            scenParam[stoi(filenames[i][3])].statSubScenarioSplitName = filenames[i][2];
            scenParam[stoi(filenames[i][3])].facilitySpecificNewFile = filenames[i][1];
        }
        else if (filenames[i][0] == "input-change-parameters") {

            parameterFlagFile = filenames[i][1];
        }
        else if (filenames[i][0] == "population-array") {

            populationFile = filenames[i][1];
        }
        else if (filenames[i][0] == "population-travel-times") {

            travelTimesFile = filenames[i][1];
        }
        else if (filenames[i][0] == "region-district-specific-data") {

            regionDistrictSpecificFile = filenames[i][1];
        }
        else if (filenames[i][0] == "run-specific-data") {

            runSpecificFile = filenames[i][1];
        }
        else if (filenames[i][0] == "tests") {

            testFile = filenames[i][1];
        }

    }

   // debug - true
    if (debug || true) {

        std::cout << "popBinFile file = " << popBinFile << '\n';
        std::cout << "diseaseFile file = " << diseaseFile << '\n';
        std::cout << "facilitiesFile file = " << facilitiesFile << '\n';
        std::cout << "facilitySpecificFile file = " << facilitySpecificFile << '\n';
        std::cout << "facilitySpecificNewFile file = " << facilitySpecificNewFile << '\n';
        std::cout << "facilitySpecificSplitFile file = " << facilitySpecificSplitFile << '\n';
        std::cout << "parameterFlagFile file = " << parameterFlagFile << '\n';
        std::cout << "populationFile file = " << populationFile << '\n';
        std::cout << "travelTimesFile file = " << travelTimesFile << '\n';
        std::cout << "regionDistrictSpecificFile file = " << regionDistrictSpecificFile << '\n';
        std::cout << "runSpecificFile file = " << runSpecificFile << '\n';
        std::cout << "test file = " << testFile << '\n';

        for (int i = 1; i < 12; i++) {

            std::cout << " i = " << i << " statSubScenarioDir " << scenParam[i].statSubScenarioDir << " statSubScenarioName " << scenParam[i].statSubScenarioName << " statSubScenarioSplitName " << scenParam[i].statSubScenarioSplitName << "\n";
            std::cout << " facilitySpecificNewFile " << scenParam[i].facilitySpecificNewFile << " facilitySpecificFile " << scenParam[i].facilitySpecificFile << " splitScenario " << scenParam[i].splitScenario << "\n";

        }
    }

    string diseaseInput = defaultDir + inputDir + diseaseDir + diseaseFile;
    string diseaseTBUntreatedMortInput = defaultDir + inputDir + diseaseDir + diseaseTBUntreatedMortFile;
    string testInput = defaultDir + inputDir + testDir + testFile;
    string facilitiesInput = defaultDir + inputDir + facilitiesDir + facilitiesFile;
    string facilitiesInputNoTop = defaultDir + inputDir + facilitiesDir + facilitiesFileNoTop;
    string populationInput = defaultDir + inputDir + populationDir + populationFile;
    string travelTimesInput = defaultDir + inputDir + populationDir + travelTimesFile;
    string popBinInput = defaultDir + inputDir + popBinDir + popBinFile;
    string facilitySpecificInput = defaultDir + inputDir + facilitySpecificDir + facilitySpecificFile;
    string regionDistrictSpecificInput = defaultDir + inputDir + regionDistrictSpecificDir + regionDistrictSpecificFile;
    string runSpecificInput = defaultDir + inputDir + runSpecificDir + runSpecificFile;
    string parameterInput = defaultDir + inputDir + parameterDir + parameterFlagFile;

    string snapshotDir = "./snapshot/";
    string snapshotFile = "snapshot.csv";
    string snapshotInput = snapshotDir + snapshotFile;




    if (distBtwnFacFlag) {
    
        vector<vector<string>> fac = readDataInput("./dist2fac/FacilitesMap.csv");
        std::cout << "after reading fac!\n";

        vector<vector<string>> access = readDataInput("./dist2fac/table_referral_nclass2_tt.csv");
        std::cout << "after reading access!\n";

        mapFacDist(fac, access);

        return 0;

    }


    // routines to read in different facilites and combine them
    if (combinedFlag) {
        vector<vector<string>> maina = readDataInput("./files/ghana-maina.csv");
        std::cout << "after reading maina!\n";

        vector<vector<string>> ernest = readDataInput("./files/ernest.csv");
        std::cout << "after reading earnest!\n";

        makeNewList(maina, ernest);

        vector<vector<string>> id = readDataInput("./files/id-matching.csv");
        std::cout << "after reading id matching!\n";

        idMatching(id);

        return 0;
    }

    // routines to read in a .img file and extract information from the pixels
    if (imageFlag) {
        
        //vector<vector<string>> facInput = readInputFile(facilitiesInputNoTop);
        //vector<vector<string>> facInputReg = readInputFile4("./defaults/input/facilities/case-reg-703-no-labs.csv");
        vector<vector<string>> facInputReg = readInputFile4("./defaults/input/facilities/case-reg-853-eid-tb.csv");
        std::cout << "after read facilities\n";

        // find closest travel time to regional hospital
        updateTravelRegDistance(facInputReg);
        std::cout << "after update travel reg hospital distance\n";

        vector<vector<string>> pop = readDataInput("./defaults/input/population/pop_1km_array_2015.csv");
        std::cout << "after reading population\n";

        // vector<vector<string>> travel = readDataInput("./default/input/population/nclass2.csv");
        //vector<vector<string>> travel = readDataInput("./defaults/input/population/raster_travel_time_accessibility_703_minus_labs.csv");
        vector<vector<string>> travel = readDataInput("./defaults/input/population/raster_travel_time_accessibility_853.csv");
        std::cout << "after reading travel\n";

        //vector<vector<string>> facInput = readInputFile(facilitiesInputNoTop);
        //vector<vector<string>> facInput = readInputFile3("./defaults/input/facilities/case-reg-1337-with-dist.csv");
        //vector<vector<string>> facInput = readInputFile3("./defaults/input/facilities/case-reg-703-with-dist.csv");
        vector<vector<string>> facInput = readInputFile3("./defaults/input/facilities/case-reg-853-with-dist.csv");
        std::cout << "after read facilities\n";

        // combine closest travel time cluster with cluster from population
        updateTravelCatch(travel, facInput);
        std::cout << "after update travel catch\n";

        // combine closest travel time cluster with cluster from population
        //updateTravelTime(pop, travel);
        std::cout << "after update travel time\n";

        return 0;
    }

    // routines to read in different facilites and combine them
    if (facilitiesListFlag) {
    
        if (argc > 1) {

            for (int i = 1; i < argc; i++) {

                vector<vector<string>> masterList = readDataInput("./facility-project/MasterFacilityList.csv");
                std::cout << "after reading master list!\n";

                temp = facilityProjectDir + argv[i];

                vector<vector<string>> newList = readDataInput(temp);
                std::cout << "after reading " << argv[i] << "\n";

                updateMasterList(masterList, newList);
                std::cout << "after updating master list!\n";
            }
        }
        else {

            vector<vector<string>> masterList = readDataInput("./facility-project/MasterFacilityList.csv");
            std::cout << "after reading master list!\n";

            vector<vector<string>> newList = readDataInput("./facility-project/ghana-hdx.csv");
            std::cout << "after reading new list!\n";

            updateMasterList(masterList, newList);
            std::cout << "after updating master list!\n";
        }

        //vector<vector<string>> id = readDataInput("./files/id-matching.csv");
        //std::cout << "after reading id matching!\n";

        //idMatching(id);

        return 0;
    }
    
    // routines to read in different facilites and combine them
    if (facilitiesList2Flag) {

        // bypass for testing cities code
        if (false) {
            if (argc > 1) {

                for (int i = 1; i < argc; i++) {

                    vector<vector<string>> masterList = readDataInput("./facility-project/MasterFacilityListRaw.csv");
                    std::cout << "after reading master list\n";

                    temp = facilityProjectDir + argv[i];

                    vector<vector<string>> newList = readDataInput(temp);
                    std::cout << "after reading " << argv[i] << "\n";

                    createMasterList(masterList, newList);
                    std::cout << "after creating master list\n";
                }
            }
            else {

                vector<vector<string>> masterList = readDataInput("./facility-project/MasterFacilityListRaw.csv");
                std::cout << "after reading master list\n";

                vector<vector<string>> newList = readDataInput("./facility-project/ghana-hdx.csv");
                std::cout << "after reading new list\n";

                createMasterList(masterList, newList);
                std::cout << "after creating master list\n";
            }

        }


        // map original types to tier types
        vector<vector<string>> masterListTypes = readDataInput("./facility-project/MasterFacilityListRaw.csv");
        std::cout << "after reading master list\n";

        // read mapping of facility types
        vector<vector<string>> types = readDataInput("./facility-project/Ghana-type-mapping.csv");
        std::cout << "after reading type map\n";

        // update facility types on masterlist
        mapTypesInMasterList(masterListTypes, types);

        

        // map districts and regions to 260 district and 16 region map
        vector<vector<string>> masterListDistricts = readDataInput("./facility-project/MasterFacilityListTypes.csv");
        std::cout << "after reading master list\n";

        // read in districts to 260 mapping and map facilities to 260 districts
        vector<vector<string>> districts = readDataInput("./facility-project/MasterListTo216-260Map.csv");
        std::cout << "after reading district mapping\n";

        // read in districts to 216 mapping and map facilities to 216 districts
        vector<vector<string>> regions = readDataInput("./facility-project/Map16to10to16regions.csv");
        std::cout << "after reading region mapping\n";

        // read in districts to 216 mapping and map facilities to 260 districts
        vector<vector<string>> districtRegions = readDataInput("./facility-project/Region-District-16-260.csv");
        std::cout << "after reading district/region mapping\n";

        // add cities-districts to masterlist
        mapDistrictsRegionsTo260Sixteen(masterListDistricts, districts, regions, districtRegions);




        // remove facilites that have the same name but no coordinates, regions or distircts
        vector<vector<string>> masterListEmpty = readDataInput("./facility-project/MasterFacilityListCitiesCleanDistricts.csv");
        std::cout << "after reading master list\n";

        // update facility types on masterlist
        removeEmptyDuplicates(masterListEmpty);




        if (true) {
            // read in town list and remove duplicates
            vector<vector<string>> towns = readDataInput("./facility-project/towns/towns.csv");
            std::cout << "after reading towns\n";

            // read in town list and remove duplicates
            vector<vector<string>> geoTowns = readDataInput("./facility-project/towns/towns-geo-name.csv");
            std::cout << "after reading geo towns\n";
            
            cleanTowns(towns, geoTowns);
        }
        

        // map cities to GPS coordinates and names
        vector<vector<string>> masterListCities = readDataInput("./facility-project/MasterFacilityListCitiesCleanDistricts2.csv");
        std::cout << "after reading master list\n";

        // read in cities/districts and match to facility coordinates to get closest city
        vector<vector<string>> cities = readDataInput("./facility-project/newTowns.csv");
        std::cout << "after reading cities\n";

        // add cities-districts to masterlist
        addCitiesToMasterList(masterListCities, cities);

        

        vector<vector<string>> masterListReduced = readDataInput("./facility-project/MasterFacilityListCities.csv");
        std::cout << "after reading master list cities\n";

        // read in districts to 216 mapping and map facilities to 260 districts
        vector<vector<string>> regionsDistricts = readDataInput("./facility-project/Region-District-16-260.csv");
        std::cout << "after reading district/region mapping\n";

        // now remove duplicate coordinated and same name facilities
        cullMasterList(masterListReduced, regionsDistricts);
        
        //idMatching(id);

        return 0;
    }


    // output google API script
    if (facilitiesList3Flag) {

        vector<vector<string>> masterListReduced = readDataInput("./facility-project/googleAPI/top-tier-facility-by-district.csv");
        std::cout << "after reading top tiers\n";

        // read in zonal labs
        vector<vector<string>> regionsDistricts = readDataInput("./facility-project/googleAPI/zonal-labs.csv");
        std::cout << "after reading zonal labs\n";

        // output google script file
        outputGoogleApi(masterListReduced, regionsDistricts);

        return 0;

    }
    
    // output google API script
    if (facilitiesList4Flag) {

         

        vector<vector<string>> ml = readDataInput("./Ghana/Master/MasterFacilityList.csv");
        std::cout << "after reading master list cities\n";

        // read in districts to 216 mapping and map facilities to 260 districts
        vector<vector<string>> gmaps = readDataInput("./facility-project/gmaps-validated.csv");
        std::cout << "after reading district/region mapping\n";
        
        // just add master list with results from google query
        masterListPlusGoogle( ml, gmaps);
        
        return 0;
   }

   // Find closest regional hospital to facility
   if (facilitiesList5Flag) {

        vector<vector<string>> fl = readDataInput("./defaults/input/facilities/facilities.csv");
        std::cout << "after reading facility list cities\n";
      
        // check regional hospital distance to facilities
        facilityToRegHospital( fl);
        
        return 0;
   }

   







   
   // run ABM
   
   // read in arguments
   if (argc > 1) {

       std::cout << "ABM arguments ";

       // keep track of scenarios - ignore keywords - assume first one is the job number or stat sub-directory
       scenarioIndex = 1;

        for (int i = 1; i < argc; i++) {

            std::cout << " arg[" << i << "] = " << argv[i];
             
            tempInputArg = argv[i];

            // find numbers and treat seperately from words
            if (isdigit(tempInputArg[0])) {

                //std::cout << "first char is a digit " << tempInputArg << "\n";

                if (scenarioIndex == 1) {

                    // save away stat sub-directory
                    statSubDir = argv[i];
                    statSubDirNo = stoi(statSubDir);
                    scenarioIndex++;
                }

                // scenarios to be processed in argv[2]
                // scenarios 5-8 will use existing input paramameters if the job n directory already exists
                // 1 - Xpert at tiers 1, 2, 3
                // 2 - Xpert at tiers 1, 2, 3, 4
                // 3 - Xpert at tiers 1, 2, 3 and some 4, smear at tier 4
                // 4 - Xpert at tiers 1, 2, 3 
                // 5 - Xpert at tiers 1, 2, 3, 4
                // 6 - Xpert at tiers 1, 2, 3, and some 4, smear at tier 4
                // 7 - Xpert at tiers 1, 2, 3
                // 8 - Xpert at tiers 1, 2, 3, 4
                // 9 - Xpert at tiers 1, 2, 3, 4 and some 4, smear at tier 4
                else if (scenarioIndex == 2) {

                    scenarioNo[0] = stoi(argv[i]);
                    scenarioCount = 1;
                    scenarioIndex++;
                }
                else if (scenarioIndex == 3) {

                    scenarioNo[1] = stoi(argv[i]);
                    scenarioCount = 2;
                    scenarioIndex++;
                }
                else if (scenarioIndex == 4) {

                    scenarioNo[2] = stoi(argv[i]);
                    scenarioCount = 3;
                    scenarioIndex++;
                }
                else if (scenarioIndex == 5) {

                    scenarioNo[3] = stoi(argv[i]);
                    scenarioCount = 4;
                    scenarioIndex++;
                }
                else if (scenarioIndex == 6) {

                    scenarioNo[4] = stoi(argv[i]);
                    scenarioCount = 5;
                    scenarioIndex++;
                }
                else if (scenarioIndex == 7) {

                    scenarioNo[5] = stoi(argv[i]);
                    scenarioCount = 6;
                    scenarioIndex++;
                }
                else if (scenarioIndex == 8) {

                    scenarioNo[6] = stoi(argv[i]);
                    scenarioCount = 7;
                    scenarioIndex++;
                }
                else if (scenarioIndex == 9) {

                    scenarioNo[7] = stoi(argv[i]);
                    scenarioCount = 8;
                    scenarioIndex++;
                }
                else if (scenarioIndex == 10) {

                    scenarioNo[8] = stoi(argv[i]);
                    scenarioCount = 9;
                    scenarioIndex++;
                }
            }
            else {

                // keywords
                if (tempInputArg == "SampleStats") {

                    sampleStatsFlag = true;
                }
                else if (tempInputArg == "ResourceStats") {

                    resourceStatsFlag = true;
                }
                else if (tempInputArg == "VisualizationStats") {

                    vizStatFlag = true;
                    sampleStatsFlag = true;
                }
                else if (tempInputArg == "TableauStats") {

                    tableauStatsFlag = true;
                }
                else if (tempInputArg == "PatientStats") {

                    agentPatientsStatsFlag = true;
                }
                else if (tempInputArg == "PatientSamplingStats") {

                    agentPatientsSamplingStatsFlag = true;
                }
                else {

                    std::cout << "\n";
                    std::cout << "keyword not recognized = " << tempInputArg << "\n";
                }
            }
            
        }
        std::cout << "\n";
   }

   std::cout << "scenario = " << scenarioNo[0] << ", " << scenarioNo[1] << ", " << scenarioNo[2] << ", " << scenarioNo[3] << "\n";

   //switch (scenarioNo[0]){
   //     case 1:
   //         // Xpert at tiers 1, 2, 3 only - no smear
   //         if (MDClinicDiagFlag) {
   //         
   //             statSubScenarioDir = statSubScenarioDir1;
   //             statSubScenarioName = statSubScenarioName1;
   //             statSubScenarioSplitName = statSubScenarioName;
   //         }
   //         else {
   //         
   //             statSubScenarioDir = "Centralized";
   //             statSubScenarioName = "Centralized";
   //             statSubScenarioSplitName = statSubScenarioName;
   //         }
   //         facilitySpecificNewFile = facilitySpecificFile1;
   //         facilitySpecificFile = facilitySpecificFile1;
   //         break;
   //     case 2:
   //         // Xpert at tiers 1, 2, 3, 4 only - no smear
   //         if (MDClinicDiagFlag) {
   //         
   //             statSubScenarioDir = statSubScenarioDir2;
   //             statSubScenarioName = statSubScenarioName2;
   //             statSubScenarioSplitName = statSubScenarioName;
   //         }
   //         else {
   //         
   //             statSubScenarioDir = "Decentralized";
   //             statSubScenarioName = "Decentralized";
   //             statSubScenarioSplitName = statSubScenarioName;
   //         }
   //         facilitySpecificNewFile = facilitySpecificFile2;
   //         facilitySpecificFile = facilitySpecificFile2;
   //         break;
   //     case 3:
   //         // Xpert at tiers 1, 2, 3 and some 4 only - smear at tier 4
   //         if (MDClinicDiagFlag) {
   //         
   //             statSubScenarioDir = statSubScenarioDir3;
   //             statSubScenarioName = statSubScenarioName3;
   //             statSubScenarioSplitName = statSubScenarioName;
   //         }
   //         else {
   //         
   //             statSubScenarioDir = "Baseline";
   //             statSubScenarioName = "Baseline";
   //             statSubScenarioSplitName = statSubScenarioName;
   //         }
   //         facilitySpecificNewFile = facilitySpecificFile3;
   //         facilitySpecificFile = facilitySpecificFile3;
   //         break;
   //     case 4:
   //         // Xpert at tier 1, 2, 3 and some 4 only only - smear at tiers 4, 5, 6
   //         statSubScenarioDir = statSubScenarioDir4;
   //         statSubScenarioName = statSubScenarioName4;
   //         statSubScenarioSplitName = statSubScenarioName;
   //         facilitySpecificNewFile = facilitySpecificFile4;
   //         facilitySpecificFile = facilitySpecificFile4;
   //         break;
   //     case 5:
   //         // Xpert at tiers 1, 2, 3 only - no smear
   //         statSubScenarioDir = statSubScenarioDir5;
   //         statSubScenarioName = statSubScenarioName5;
   //         statSubScenarioSplitName = statSubScenarioName;
   //         facilitySpecificNewFile = facilitySpecificFile5;
   //         facilitySpecificFile = facilitySpecificFile5;
   //         break;
   //     case 6:
   //         // Xpert at tiers 1, 2, 3, 4 only - no smear
   //         statSubScenarioDir = statSubScenarioDir6;
   //         statSubScenarioName = statSubScenarioName5;
   //         statSubScenarioSplitName = statSubScenarioName;
   //         facilitySpecificNewFile = facilitySpecificFile6;
   //         facilitySpecificFile = facilitySpecificFile6;
   //         break;
   //     case 7:
   //         // Xpert at tiers 1, 2, 3 and some 4 only - smear at tier 4
   //         statSubScenarioDir = statSubScenarioDir7;
   //         statSubScenarioName = statSubScenarioName7;
   //         statSubScenarioSplitName = statSubScenarioName;
   //         facilitySpecificNewFile = facilitySpecificFile7;
   //         facilitySpecificFile = facilitySpecificFile7;
   //         break;
   //     case 8:
   //         // Xpert at tier 1, 2, 3 and some 4 only only - smear at tiers 4, 5, 6
   //         statSubScenarioDir = statSubScenarioDir8;
   //         statSubScenarioName = statSubScenarioName8;
   //         statSubScenarioSplitName = statSubScenarioName;
   //         //facilitySpecificNewFile = "facility-specific-data-tier1-2-3-4-xpert-only-by-tier.csv";
   //         //facilitySpecificFile = "facility-specific-data-tier1-2-3-4-xpert-only-by-tier.csv";
   //         //facilitySpecificNewFile = "facility-specific-data-tier1-2-3-4-plus-current-169-xpert-only.csv";
   //         //facilitySpecificFile = "facility-specific-data-tier1-2-3-4-plus-current-169-xpert-only.csv";
   //         facilitySpecificNewFile = facilitySpecificFile8;
   //         facilitySpecificFile = facilitySpecificFile8;
   //         break;
   //     case 9:
   //         // Xpert at tier 1, 2, 3 and some 4 only - no smear at tiers 4, 5, 6
   //         statSubScenarioDir = statSubScenarioDir9;
   //         statSubScenarioName = statSubScenarioName9;
   //         statSubScenarioSplitName = statSubScenarioName;
   //         facilitySpecificNewFile = facilitySpecificFile9;
   //         facilitySpecificFile = facilitySpecificFile9;

   //         break;
   //     case 10:
   //         // Xpert at tier 1, 2, 3, 4, 5, 6 and 7
   //         statSubScenarioDir = statSubScenarioDir10;
   //         statSubScenarioName = statSubScenarioName10;
   //         statSubScenarioSplitName = statSubScenarioName;
   //         facilitySpecificNewFile = facilitySpecificFile10;
   //         facilitySpecificFile = facilitySpecificFile10;
   //         break;
   //     case 11:
   //         // Xpert at tier 1, 2, 3 and some 4 only - no smear at tiers 4, 5, 6 - 169 Xpert placement plus 29 suggested Xpert placement
   //         statSubScenarioDir = statSubScenarioDir11;
   //         statSubScenarioName = statSubScenarioName11;
   //         statSubScenarioSplitName = statSubScenarioName;
   //         facilitySpecificNewFile = facilitySpecificFile11;
   //         facilitySpecificFile = facilitySpecificFile11;
   //         break;
   //     case 21:
   //         // start at baseline scenario and switch to centralized scenario
   //         statSubScenarioDir = "Base-" + statSubScenarioDir7;
   //         statSubScenarioName = statSubScenarioName9;
   //         statSubScenarioSplitName = statSubScenarioName7;
   //         facilitySpecificNewFile = facilitySpecificFile9;
   //         facilitySpecificFile = facilitySpecificFile9;
   //         facilitySpecificSplitFile = facilitySpecificFile7;
   //         splitScenario = true;
   //         break;
   //     case 22:
   //         // start at baseline scenario and switch to centralized scenario
   //         statSubScenarioDir = "Base-" + statSubScenarioDir8;
   //         statSubScenarioName = statSubScenarioName9;
   //         statSubScenarioSplitName = statSubScenarioName8;
   //         facilitySpecificNewFile = facilitySpecificFile9;
   //         facilitySpecificFile = facilitySpecificFile9;
   //         facilitySpecificSplitFile = facilitySpecificFile8;
   //         splitScenario = true;
   //         break;
   //     case 23:
   //         // start at baseline scenario and switch to centralized scenario
   //         statSubScenarioDir = "Base-" + statSubScenarioDir10;
   //         statSubScenarioName = statSubScenarioName9;
   //         statSubScenarioSplitName = statSubScenarioName10;
   //         facilitySpecificNewFile = facilitySpecificFile9;
   //         facilitySpecificFile = facilitySpecificFile9;
   //         facilitySpecificSplitFile = facilitySpecificFile10;
   //         splitScenario = true;
   //         break;
   //     case 24:
   //         // start at baseline scenario and switch to baseline plus 29 DNO sites
   //         statSubScenarioDir = "Base-" + statSubScenarioDir11;
   //         statSubScenarioName = statSubScenarioName9;
   //         statSubScenarioSplitName = statSubScenarioName11;
   //         facilitySpecificNewFile = facilitySpecificFile9;
   //         facilitySpecificFile = facilitySpecificFile9;
   //         facilitySpecificSplitFile = facilitySpecificFile11;
   //         splitScenario = true;
   //         break;
   //     // added these four to bump job numbers for Rockfish
   //     case 91:
   //         // Xpert at tier 1, 2, 3 and some 4 only - no smear at tiers 4, 5, 6
   //         statSubScenarioDir = statSubScenarioDir9;
   //         statSubScenarioName = "Baseline-CD-Everywhere";
   //         statSubScenarioSplitName = statSubScenarioName;
   //         facilitySpecificNewFile = facilitySpecificFile9;
   //         facilitySpecificFile = facilitySpecificFile9;
   //         statSubDirNo = statSubDirNo + 10000;
   //         statSubDir = to_string(statSubDirNo);
   //         break;
   //     case 92:
   //         // Xpert at tier 1, 2, 3 and some 4 only - no smear at tiers 4, 5, 6
   //         statSubScenarioDir = statSubScenarioDir9;
   //         statSubScenarioName = statSubScenarioName9;
   //         statSubScenarioSplitName = statSubScenarioName;
   //         facilitySpecificNewFile = facilitySpecificFile9;
   //         facilitySpecificFile = facilitySpecificFile9;
   //         statSubDirNo = statSubDirNo + 20000;
   //         statSubDir = to_string(statSubDirNo);
   //         break;
   //     case 93:
   //         // Xpert at tier 1, 2, 3 and some 4 only - no smear at tiers 4, 5, 6
   //         statSubScenarioDir = statSubScenarioDir9;
   //         statSubScenarioName = statSubScenarioName9;
   //         statSubScenarioSplitName = statSubScenarioName;
   //         facilitySpecificNewFile = facilitySpecificFile9;
   //         facilitySpecificFile = facilitySpecificFile9;
   //         statSubDirNo = statSubDirNo + 30000;
   //         statSubDir = to_string(statSubDirNo);
   //         break;
   //     case 94:
   //         // Xpert at tier 1, 2, 3 and some 4 only - no smear at tiers 4, 5, 6
   //         statSubScenarioDir = statSubScenarioDir9;
   //         statSubScenarioName = statSubScenarioName9;
   //         statSubScenarioSplitName = statSubScenarioName;
   //         facilitySpecificNewFile = facilitySpecificFile9;
   //         facilitySpecificFile = facilitySpecificFile9;
   //         statSubDirNo = statSubDirNo + 40000;
   //         statSubDir = to_string(statSubDirNo);
   //         break;
   //     default :
   //         // Xpert at tiers 1, 2, 3 and some 4 only - smear at tier 4
   //         statSubScenarioDir = statSubScenarioDir9;
   //         statSubScenarioName = statSubScenarioName9;
   //         statSubScenarioSplitName = statSubScenarioName;
   //         facilitySpecificNewFile = facilitySpecificFile9;
   //         facilitySpecificFile = facilitySpecificFile9;
   //}
       
   scenarioParam tempScenarioParam{ getScenarioParameters(scenarioNo[0], scenParam) };

   switch (scenarioNo[0]){

        //added these four to bump job numbers for Rockfish
        case 91:
            statSubDirNo = statSubDirNo + 10000;
            statSubDir = to_string(statSubDirNo);
            break;
        case 92:
            statSubDirNo = statSubDirNo + 20000;
            statSubDir = to_string(statSubDirNo);
            break;
        case 93:
            statSubDirNo = statSubDirNo + 30000;
            statSubDir = to_string(statSubDirNo);
            break;
        case 94:
            statSubDirNo = statSubDirNo + 40000;
            statSubDir = to_string(statSubDirNo);
            break;
   }

   statSubScenarioDir = tempScenarioParam.statSubScenarioDir;
   statSubScenarioName = tempScenarioParam.statSubScenarioName;
   statSubScenarioSplitName = tempScenarioParam.statSubScenarioSplitName;
   facilitySpecificNewFile = tempScenarioParam.facilitySpecificNewFile;
   facilitySpecificFile = tempScenarioParam.facilitySpecificFile;
   facilitySpecificSplitFile = tempScenarioParam.facilitySpecificSplitFile;
   splitScenario = tempScenarioParam.splitScenario;

   // debug - true
   if (debug) {

       std::cout << " statSubScenarioDir " << statSubScenarioDir << " statSubScenarioName " << statSubScenarioName << " statSubScenarioSplitName " << statSubScenarioSplitName << "\n";
       std::cout << " facilitySpecificNewFile " << facilitySpecificNewFile << " facilitySpecificFile " << facilitySpecificFile << " splitScenario " << splitScenario << "\n";
   }

   // error checking
   for (int i = 0; i < 9; i++) {

        // &&& change back to 23 instead of 94
       if ((scenarioNo[i] > 94) || (scenarioNo[i] < 0)) {

            std::cout << " Invalid scenario for parameter " << i+2 << "\n";
        }
   }
   // if a snapshot exists use it to start the program
   // facilities list is global so it is updated, diseaseList, dis and tes pass by address
   // to change values
   //readSnapshot(snapshotInput, facilitiesList, &diseaseList, &dis, &tes);

   //if (dis.size() > 0) {

   //     readSnapshotFlag = true;

   //     // found snapshot - now have to update lateral, up and down connections for facilities
   //     updateFacilityConnections(facilitiesList);

   //     // update random number arrays to current state
   //     makeRandomNo100( seed, 100 );
   //     makeRandomNoPop( seed, diseaseList );
   //}
   //else {

   // copy files from default input directory and store away in stats/job-no. directory

   // make sure stats/scenario/job-no. directory exists and stats/scenario/job-no./input directories exist
   subDirBaseScenario =  "./stats/" + statSubScenarioDir;
   if (statSubDir.length() > 0) {
        subDirBase = subDirBaseScenario + "/" + "job-" + statSubDir;
   }
   else {
        subDirBase = subDirBaseScenario + "/" + "job-" + to_string(1);
        statSubDir = "1";
   }

   struct stat info;

   const char * c = subDirBase.c_str();

   if( stat( c, &info ) != 0 ) {

        std::cout <<  "cannot access " << subDirBase << "\n";
        rerunInputFlag = false;
   }
   else if( info.st_mode & S_IFDIR ) {
   
        // S_ISDIR() doesn't exist on my windows 
        std::cout <<  subDirBase << " exists \n";
        rerunInputFlag = true;
        // don't know why this was originally in here
        /*if (scenarioNo[0] > 4) {
            rerunInputFlag = true;
        }
        else {
            rerunInputFlag = false;
        }*/
   }
   else {

        std::cout << subDirBase << " doesn't exist\n";
        rerunInputFlag = false;
   }
        
   // if reruning with the same input parameters, skip copying of input parameters
   if (!rerunInputFlag) {

        copyInput("./defaults", subDirBaseScenario, scenarioNo[0], statSubDir, popBinFile, diseaseFile, facilitiesFile, scenParam, parameterFlagFile, filenameFile, regDistFile, populationFile, regionDistrictSpecificFile, runSpecificFile, testFile);

        // copy default input paramters to ./stats/statSubScenarioDir/job-n
        std::cout << "copying input files from source " << "default" << " to " << subDirBaseScenario << " \n";
   }
   else {

       // use copied input-file-names.csv file 
       //std::cout << "JEFF " << subDirBase + "/input/" + "input-file-names/" + filenameFile << std::endl;
       vector<vector<string>> filenames = readDataInput(subDirBase + "/input/" + "input-file-names/" + filenameFile);

       for (std::size_t i = 0; i < filenames.size(); i++) {

           if (filenames[i][0] == "catchment") {

               popBinFile = filenames[i][1];
           }
           else if (filenames[i][0] == "diseases") {

               diseaseFile = filenames[i][1];
           }
           else if (filenames[i][0] == "facilities") {

               facilitiesFile = filenames[i][1];
           }
           else if (filenames[i][0] == "facility-specific-data") {

               scenParam[stoi(filenames[i][3])].facilitySpecificFile = filenames[i][1];
               scenParam[stoi(filenames[i][3])].statSubScenarioDir = filenames[i][2];
               scenParam[stoi(filenames[i][3])].statSubScenarioName = filenames[i][2];
               scenParam[stoi(filenames[i][3])].statSubScenarioSplitName = filenames[i][2];
               scenParam[stoi(filenames[i][3])].facilitySpecificNewFile = filenames[i][1];
           }
           else if (filenames[i][0] == "input-change-parameters") {

               parameterFlagFile = filenames[i][1];
           }
           else if (filenames[i][0] == "population-array") {

               populationFile = filenames[i][1];
           }
           else if (filenames[i][0] == "population-travel-times") {

               travelTimesFile = filenames[i][1];
           }
           else if (filenames[i][0] == "region-district-specific-data") {

               regionDistrictSpecificFile = filenames[i][1];
           }
           else if (filenames[i][0] == "run-specific-data") {

               runSpecificFile = filenames[i][1];
           }
           else if (filenames[i][0] == "tests") {

               testFile = filenames[i][1];
           }

       }

       // debug - true
       if (debug) {

           std::cout << "from " << subDirBase << " popBinFile file = " << popBinFile << '\n';
           std::cout << "from " << subDirBase << " diseaseFile file = " << diseaseFile << '\n';
           std::cout << "from " << subDirBase << " facilitiesFile file = " << facilitiesFile << '\n';
           std::cout << "from " << subDirBase << " facilitySpecificFile file = " << facilitySpecificFile << '\n';
           std::cout << "from " << subDirBase << " facilitySpecificNewFile file = " << facilitySpecificNewFile << '\n';
           std::cout << "from " << subDirBase << " facilitySpecificSplitFile file = " << facilitySpecificSplitFile << '\n';
           std::cout << "from " << subDirBase << " parameterFlagFile file = " << parameterFlagFile << '\n';
           std::cout << "from " << subDirBase << " populationFile file = " << populationFile << '\n';
           std::cout << "from " << subDirBase << " travelTimesFile file = " << travelTimesFile << '\n';
           std::cout << "from " << subDirBase << " regionDistrictSpecificFile file = " << regionDistrictSpecificFile << '\n';
           std::cout << "from " << subDirBase << " runSpecificFile file = " << runSpecificFile << '\n';
           std::cout << "from " << subDirBase << " test file = " << testFile << '\n';
       }

       //switch (scenarioNo[0]) {
       //case 1:
       //    // Xpert at tiers 1, 2, 3 only - no smear
       //    if (MDClinicDiagFlag) {

       //        statSubScenarioDir = statSubScenarioDir1;
       //        statSubScenarioName = statSubScenarioName1;
       //        statSubScenarioSplitName = statSubScenarioName;
       //    }
       //    else {

       //        statSubScenarioDir = "Centralized";
       //        statSubScenarioName = "Centralized";
       //        statSubScenarioSplitName = statSubScenarioName;
       //    }
       //    facilitySpecificNewFile = facilitySpecificFile1;
       //    facilitySpecificFile = facilitySpecificFile1;
       //    break;
       //case 2:
       //    // Xpert at tiers 1, 2, 3, 4 only - no smear
       //    if (MDClinicDiagFlag) {

       //        statSubScenarioDir = statSubScenarioDir2;
       //        statSubScenarioName = statSubScenarioName2;
       //        statSubScenarioSplitName = statSubScenarioName;
       //    }
       //    else {

       //        statSubScenarioDir = "Decentralized";
       //        statSubScenarioName = "Decentralized";
       //        statSubScenarioSplitName = statSubScenarioName;
       //    }
       //    facilitySpecificNewFile = facilitySpecificFile2;
       //    facilitySpecificFile = facilitySpecificFile2;
       //    break;
       //case 3:
       //    // Xpert at tiers 1, 2, 3 and some 4 only - smear at tier 4
       //    if (MDClinicDiagFlag) {

       //        statSubScenarioDir = statSubScenarioDir3;
       //        statSubScenarioName = statSubScenarioName3;
       //        statSubScenarioSplitName = statSubScenarioName;
       //    }
       //    else {

       //        statSubScenarioDir = "Baseline";
       //        statSubScenarioName = "Baseline";
       //        statSubScenarioSplitName = statSubScenarioName;
       //    }
       //    facilitySpecificNewFile = facilitySpecificFile3;
       //    facilitySpecificFile = facilitySpecificFile3;
       //    break;
       //case 4:
       //    // Xpert at tier 1, 2, 3 and some 4 only only - smear at tiers 4, 5, 6
       //    statSubScenarioDir = statSubScenarioDir4;
       //    statSubScenarioName = statSubScenarioName4;
       //    statSubScenarioSplitName = statSubScenarioName;
       //    facilitySpecificNewFile = facilitySpecificFile4;
       //    facilitySpecificFile = facilitySpecificFile4;
       //    break;
       //case 5:
       //    // Xpert at tiers 1, 2, 3 only - no smear
       //    statSubScenarioDir = statSubScenarioDir5;
       //    statSubScenarioName = statSubScenarioName5;
       //    statSubScenarioSplitName = statSubScenarioName;
       //    facilitySpecificNewFile = facilitySpecificFile5;
       //    facilitySpecificFile = facilitySpecificFile5;
       //    break;
       //case 6:
       //    // Xpert at tiers 1, 2, 3, 4 only - no smear
       //    statSubScenarioDir = statSubScenarioDir6;
       //    statSubScenarioName = statSubScenarioName6;
       //    statSubScenarioSplitName = statSubScenarioName;
       //    facilitySpecificNewFile = facilitySpecificFile6;
       //    facilitySpecificFile = facilitySpecificFile6;
       //    break;
       //case 7:
       //    // Xpert at tiers 1, 2, 3 and some 4 only - smear at tier 4
       //    statSubScenarioDir = statSubScenarioDir7;
       //    statSubScenarioName = statSubScenarioName7;
       //    statSubScenarioSplitName = statSubScenarioName;
       //    facilitySpecificNewFile = facilitySpecificFile7;
       //    facilitySpecificFile = facilitySpecificFile7;
       //    break;
       //case 8:
       //    // Xpert at tier 1, 2, 3 and some 4 only only - smear at tiers 4, 5, 6
       //    statSubScenarioDir = statSubScenarioDir8;
       //    statSubScenarioName = statSubScenarioName8;
       //    statSubScenarioSplitName = statSubScenarioName;
       //    //facilitySpecificNewFile = "facility-specific-data-tier1-2-3-4-xpert-only-by-tier.csv";
       //    //facilitySpecificFile = "facility-specific-data-tier1-2-3-4-xpert-only-by-tier.csv";
       //    //facilitySpecificNewFile = "facility-specific-data-tier1-2-3-4-plus-current-169-xpert-only.csv";
       //    //facilitySpecificFile = "facility-specific-data-tier1-2-3-4-plus-current-169-xpert-only.csv";
       //    facilitySpecificNewFile = facilitySpecificFile8;
       //    facilitySpecificFile = facilitySpecificFile8;
       //    break;
       //case 9:
       //    // Xpert at tier 1, 2, 3 and some 4 only - no smear at tiers 4, 5, 6
       //    statSubScenarioDir = statSubScenarioDir9;
       //    statSubScenarioName = statSubScenarioName9;
       //    statSubScenarioSplitName = statSubScenarioName;
       //    facilitySpecificNewFile = facilitySpecificFile9;
       //    facilitySpecificFile = facilitySpecificFile9;

       //    break;
       //case 10:
       //    // Xpert at tier 1, 2, 3, 4, 5, 6 and 7
       //    statSubScenarioDir = statSubScenarioDir10;
       //    statSubScenarioName = statSubScenarioName10;
       //    statSubScenarioSplitName = statSubScenarioName;
       //    facilitySpecificNewFile = facilitySpecificFile10;
       //    facilitySpecificFile = facilitySpecificFile10;
       //    break;
       //case 11:
       //    // Xpert at tier 1, 2, 3 and some 4 only - no smear at tiers 4, 5, 6 - 169 Xpert placement plus 29 suggested Xpert placement
       //    statSubScenarioDir = statSubScenarioDir11;
       //    statSubScenarioName = statSubScenarioName11;
       //    statSubScenarioSplitName = statSubScenarioName;
       //    facilitySpecificNewFile = facilitySpecificFile11;
       //    facilitySpecificFile = facilitySpecificFile11;
       //    break;
       //case 21:
       //    // start at baseline scenario and switch to centralized scenario
       //    statSubScenarioDir = "Base-" + statSubScenarioDir7;
       //    statSubScenarioName = statSubScenarioName9;
       //    statSubScenarioSplitName = statSubScenarioName7;
       //    facilitySpecificNewFile = facilitySpecificFile9;
       //    facilitySpecificFile = facilitySpecificFile9;
       //    facilitySpecificSplitFile = facilitySpecificFile7;
       //    splitScenario = true;
       //    break;
       //case 22:
       //    // start at baseline scenario and switch to centralized scenario
       //    statSubScenarioDir = "Base-" + statSubScenarioDir8;
       //    statSubScenarioName = statSubScenarioName9;
       //    statSubScenarioSplitName = statSubScenarioName8;
       //    facilitySpecificNewFile = facilitySpecificFile9;
       //    facilitySpecificFile = facilitySpecificFile9;
       //    facilitySpecificSplitFile = facilitySpecificFile8;
       //    splitScenario = true;
       //    break;
       //case 23:
       //    // start at baseline scenario and switch to centralized scenario
       //    statSubScenarioDir = "Base-" + statSubScenarioDir10;
       //    statSubScenarioName = statSubScenarioName9;
       //    statSubScenarioSplitName = statSubScenarioName10;
       //    facilitySpecificNewFile = facilitySpecificFile9;
       //    facilitySpecificFile = facilitySpecificFile9;
       //    facilitySpecificSplitFile = facilitySpecificFile10;
       //    splitScenario = true;
       //    break;
       //case 24:
       //    // start at baseline scenario and switch to baseline plus 29 DNO sites
       //    statSubScenarioDir = "Base-" + statSubScenarioDir11;
       //    statSubScenarioName = statSubScenarioName9;
       //    statSubScenarioSplitName = statSubScenarioName11;
       //    facilitySpecificNewFile = facilitySpecificFile9;
       //    facilitySpecificFile = facilitySpecificFile9;
       //    facilitySpecificSplitFile = facilitySpecificFile11;
       //    splitScenario = true;
       //    break;
       //    // added these four to bump job numbers for Rockfish
       //case 91:
       //    // Xpert at tier 1, 2, 3 and some 4 only - no smear at tiers 4, 5, 6
       //    statSubScenarioDir = statSubScenarioDir9;
       //    statSubScenarioName = statSubScenarioName9;
       //    statSubScenarioSplitName = statSubScenarioName;
       //    facilitySpecificNewFile = facilitySpecificFile9;
       //    facilitySpecificFile = facilitySpecificFile9;
       //    statSubDirNo = statSubDirNo + 10000;
       //    statSubDir = to_string(statSubDirNo);
       //    break;
       //case 92:
       //    // Xpert at tier 1, 2, 3 and some 4 only - no smear at tiers 4, 5, 6
       //    statSubScenarioDir = statSubScenarioDir9;
       //    statSubScenarioName = statSubScenarioName9;
       //    statSubScenarioSplitName = statSubScenarioName;
       //    facilitySpecificNewFile = facilitySpecificFile9;
       //    facilitySpecificFile = facilitySpecificFile9;
       //    statSubDirNo = statSubDirNo + 20000;
       //    statSubDir = to_string(statSubDirNo);
       //    break;
       //case 93:
       //    // Xpert at tier 1, 2, 3 and some 4 only - no smear at tiers 4, 5, 6
       //    statSubScenarioDir = statSubScenarioDir9;
       //    statSubScenarioName = statSubScenarioName9;
       //    statSubScenarioSplitName = statSubScenarioName;
       //    facilitySpecificNewFile = facilitySpecificFile9;
       //    facilitySpecificFile = facilitySpecificFile9;
       //    statSubDirNo = statSubDirNo + 30000;
       //    statSubDir = to_string(statSubDirNo);
       //    break;
       //case 94:
       //    // Xpert at tier 1, 2, 3 and some 4 only - no smear at tiers 4, 5, 6
       //    statSubScenarioDir = statSubScenarioDir9;
       //    statSubScenarioName = statSubScenarioName9;
       //    statSubScenarioSplitName = statSubScenarioName;
       //    facilitySpecificNewFile = facilitySpecificFile9;
       //    facilitySpecificFile = facilitySpecificFile9;
       //    statSubDirNo = statSubDirNo + 40000;
       //    statSubDir = to_string(statSubDirNo);
       //    break;
       //default:
       //    // Xpert at tiers 1, 2, 3 and some 4 only - smear at tier 4
       //    statSubScenarioDir = statSubScenarioDir9;
       //    statSubScenarioName = statSubScenarioName9;
       //    statSubScenarioSplitName = statSubScenarioName;
       //    facilitySpecificNewFile = facilitySpecificFile9;
       //    facilitySpecificFile = facilitySpecificFile9;
       //}

       scenarioParam tempScenarioParam{ getScenarioParameters(scenarioNo[0], scenParam) };

       switch (scenarioNo[0]) {

           //added these four to bump job numbers for Rockfish
       case 91:
           statSubDirNo = statSubDirNo + 10000;
           statSubDir = to_string(statSubDirNo);
           break;
       case 92:
           statSubDirNo = statSubDirNo + 20000;
           statSubDir = to_string(statSubDirNo);
           break;
       case 93:
           statSubDirNo = statSubDirNo + 30000;
           statSubDir = to_string(statSubDirNo);
           break;
       case 94:
           statSubDirNo = statSubDirNo + 40000;
           statSubDir = to_string(statSubDirNo);
           break;
       }

       statSubScenarioDir = tempScenarioParam.statSubScenarioDir;
       statSubScenarioName = tempScenarioParam.statSubScenarioName;
       statSubScenarioSplitName = tempScenarioParam.statSubScenarioSplitName;
       facilitySpecificNewFile = tempScenarioParam.facilitySpecificNewFile;
       facilitySpecificFile = tempScenarioParam.facilitySpecificFile;
       facilitySpecificSplitFile = tempScenarioParam.facilitySpecificSplitFile;
       splitScenario = tempScenarioParam.splitScenario;

       // debug - true
       if (debug) {

           std::cout << " statSubScenarioDir " << statSubScenarioDir << " statSubScenarioName " << statSubScenarioName << " statSubScenarioSplitName " << statSubScenarioSplitName << "\n";
           std::cout << " facilitySpecificNewFile " << facilitySpecificNewFile << " facilitySpecificFile " << facilitySpecificFile << " splitScenario " << splitScenario << "\n";
       }
   }

   // change input directory to ./stats/input 
   inputDir = subDirBase + "/input/";
   subDirBaseInput = subDirBase + "/input/";
   diseaseInput = inputDir + diseaseDir + diseaseFile;
   diseaseTBUntreatedMortInput = inputDir + diseaseDir + diseaseTBUntreatedMortFile;
   testInput = inputDir + testDir + testFile;
   facilitiesInput = inputDir + facilitiesDir + facilitiesFile;
   facilitiesInputNoTop = inputDir + facilitiesDir + facilitiesFileNoTop;
   populationInput = inputDir + populationDir + populationFile;
   popBinInput = inputDir + popBinDir + popBinFile;
   facilitySpecificInput = inputDir + facilitySpecificDir + facilitySpecificFile;
   regionDistrictSpecificInput = inputDir + regionDistrictSpecificDir + regionDistrictSpecificFile;
   runSpecificInput = inputDir + runSpecificDir + runSpecificFile;
   parameterInput = inputDir + parameterDir + parameterFlagFile;


   // read in regions and districts and setup up region-district array and district to region mapping array
   string regDistInput = inputDir + filenameDir + regDistFile;

   vector<vector<string>> RegionDistricts = readDataInput(regDistInput);

   regionDistrict regDist = createRegionDistrictArrays(RegionDistricts);
   
   // debug - true 
   if (debug) {
        std::cout << " noRegions " << noRegions << " noDistricts " << noDistricts << "\n";
        for (int i = 0; i < noElementsDistrictRegionArray; i++) {

            std::cout << " i " << i << " " << regDist.regionDistrict[i] << "\n";
        }
   }

   // debug - true
   if (debug)  std::cout << " new input directories  = " << inputDir << ", " << diseaseInput << ", " << testInput << ", " << facilitySpecificInput << "\n";
   
   if (!rerunInputFlag) {
   
        // adjust input parameters for beta distributions if asked for - check ./defaults/input/input-change-parameters/priorRangesAndDistributions.csv
        changeParameters(statSubScenarioDir, statSubDir, facilitySpecificFile);
        if (true)  std::cout << "changing parameters" << "\n";
   }
   else {  // global variables stored in InputParameters.csv

       // running with current parameters - load prob.ltfu.represent gloabl variable from current InputParameters.csv file
       //  and store in pRepresentation
       // because the default global variables don't match what is in InputParameters.csv
       vector<vector<string>> inputParametersFile = readDataInput(subDirBase + "/InputParameters.csv");

       std::cout << "Reading InputParameters.csv file named " << subDirBase << "/InputParameters.csv" << '\n';

       // find geo dist value to be used to calculate re-presentation times
       tmpIndex = 0;
       for (std::size_t i = 0; i < inputParametersFile.size(); i++) {

               for (std::size_t j = 0; j < inputParametersFile[i].size(); j++) {

                   //std::cout << "found Tests size = " << inputParametersFile[i].size() << " j = " << j << " value = " << inputParametersFile[i][j] <<'\n';
                   
                   if ((inputParametersFile[i][j] == "param.prob.geo.dist") || (inputParametersFile[i][j] == "prob.geo.dist")) {

                       tmpIndex = j;
                   }
               }

               //std::cout << "found param.prob.geo.dis index = " << tmpIndex << " size = " << inputParametersFile[i].size() << '\n';
       }

       // read in input parameter file
       for (std::size_t i = 0; i < inputParametersFile.size(); i++) {
           if ((inputParametersFile[i][0] == "Test") && (inputParametersFile[i][1] == "Xpert") && (inputParametersFile[i][2] == "TB")) {

               pRepresentation = stof(inputParametersFile[i][tmpIndex]);

               //std::cout << "after reading TB prob.ltfu.represent = " << stof(inputParametersFile[i][18]) << " pRepresentation = " << pRepresentation <<'\n';
           }
       }
   }
   if (debug)  std::cout << "after change Input parameters pRepresentation = " << pRepresentation <<  "\n";
       
   // loop through for each scenario processed
   for (int scenarioIndex = 0; scenarioIndex < scenarioCount; scenarioIndex++) {

        // output if zero
        if (scenarioIndex == 0) {
         
            std::cout << " **** scenario " << statSubScenarioDir << " loop = " << scenarioIndex << " value = " << scenarioNo[scenarioIndex] << " count = " << scenarioCount << "\n"; 
        }

        // change MD override during runs
        if (MDClinicDiagFlag) {

            if (scenarioIndex < 3) {

                MDClinicDiagText = "RegionOnly";
                MDClinicDiagOption = 1;
            }
            else if (scenarioIndex < 6) {

                MDClinicDiagText = "DistrictAndAbove";
                MDClinicDiagOption = 2;
            }
            else {

                MDClinicDiagText = "Everywhere";
                MDClinicDiagOption = 3;
            }
        }
        // debug - true
        if (debug) {

            std::cout << " **** scenario " << statSubScenarioDir << " loop = " << scenarioIndex << " value = " << scenarioNo[scenarioIndex] << " count = " << scenarioCount << " mdClinicDiag option = " << MDClinicDiagText << " mdClinicDiag option = " << MDClinicDiagOption << " file = " << facilitySpecificFile << "\n";
        }

        // if another scenario run again
        if (scenarioNo[scenarioIndex] > 0) {
    
            // after first time through, copy all input files to the new scenario directory and update the new facilitySpecificData.csv file
            // with the parameters that have been changed (noMachines and returnTime)
            if (scenarioIndex > 0) {
                
                // keep track of last scenario directory
                lastStatSubScenarioDir = statSubScenarioDir;

                //switch (scenarioNo[scenarioIndex]){
                //    case 1:
                //        // Xpert at tiers 1, 2, 3 only - no smear
                //        if (MDClinicDiagFlag) {
                //        
                //            statSubScenarioDir = statSubScenarioDir1;
                //            statSubScenarioName = statSubScenarioName1;
                //            statSubScenarioSplitName = statSubScenarioName;
                //        }
                //        else {

                //            statSubScenarioDir = "Centralized";
                //            statSubScenarioName = "Centralized";
                //            statSubScenarioSplitName = statSubScenarioName;
                //        }
                //        facilitySpecificNewFile = facilitySpecificFile1;
                //        break;
                //    case 2:
                //        // Xpert at tiers 1, 2, 3, 4 only - no smear
                //        if (MDClinicDiagFlag) {
                //        
                //            statSubScenarioDir = statSubScenarioDir2;
                //            statSubScenarioName = statSubScenarioName2;
                //            statSubScenarioSplitName = statSubScenarioName;
                //        }
                //        else {
                //            statSubScenarioDir = "Decentralized";
                //            statSubScenarioName = "Decentralized";
                //            statSubScenarioSplitName = statSubScenarioName;
                //        }
                //        facilitySpecificNewFile = facilitySpecificFile2;
                //        break;
                //    case 3:
                //        // Xpert at tiers 1, 2, 3 and some 4 only - smear at tier 4
                //        if (MDClinicDiagFlag) {
                //        
                //            statSubScenarioDir = statSubScenarioDir3;
                //            statSubScenarioName = statSubScenarioName3;
                //            statSubScenarioSplitName = statSubScenarioName;
                //        }
                //        else {
                //            statSubScenarioDir = "Baseline";
                //            statSubScenarioName = "Baseline";
                //            statSubScenarioSplitName = statSubScenarioName;
                //        }
                //        facilitySpecificNewFile = facilitySpecificFile3;
                //        break;
                //    case 4:
                //        // Xpert at tiers 1, 2, 3 only - no smear
                //        statSubScenarioDir = statSubScenarioDir4;
                //        statSubScenarioName = statSubScenarioName4;
                //        statSubScenarioSplitName = statSubScenarioName;
                //        facilitySpecificNewFile = facilitySpecificFile4;
                //        break;
                //    case 5:
                //        // Xpert at tiers 1, 2, 3, 4 only - no smear
                //        statSubScenarioDir = statSubScenarioDir5;
                //        statSubScenarioName = statSubScenarioName5;
                //        statSubScenarioSplitName = statSubScenarioName;
                //        facilitySpecificNewFile = facilitySpecificFile5;
                //        break;
                //    case 6:
                //        // Xpert at tiers 1, 2, 3 and some 4 only - smear at tier 4
                //        statSubScenarioDir = statSubScenarioDir6;
                //        statSubScenarioName = statSubScenarioName6;
                //        statSubScenarioSplitName = statSubScenarioName;
                //        facilitySpecificNewFile = facilitySpecificFile6;
                //        break;
                //    case 7:
                //        // Xpert at tiers 1, 2, 3 only - no smear
                //        statSubScenarioDir = statSubScenarioDir7;
                //        statSubScenarioName = statSubScenarioName7;
                //        statSubScenarioSplitName = statSubScenarioName;
                //        facilitySpecificNewFile = facilitySpecificFile7;
                //        break;
                //    case 8:
                //        // Xpert at tiers 1, 2, 3, 4 only - no smear
                //        statSubScenarioDir = statSubScenarioDir8;
                //        statSubScenarioName = statSubScenarioName8;
                //        statSubScenarioSplitName = statSubScenarioName;
                //        //facilitySpecificNewFile =  "facility-specific-data-tier1-2-3-4-xpert-only-by-tier.csv";
                //        facilitySpecificNewFile = facilitySpecificFile8;
                //        break;
                //    case 9:
                //        // Xpert at tiers 1, 2, 3 and some 4 only - smear at tier 4
                //        statSubScenarioDir = statSubScenarioDir9;
                //        statSubScenarioName = statSubScenarioName9;
                //        statSubScenarioSplitName = statSubScenarioName;
                //        facilitySpecificNewFile = facilitySpecificFile9;
                //        break;
                //    case 10:
                //        // Xpert at tier 1, 2, 3, 4, 5, 6 and 7
                //        statSubScenarioDir = statSubScenarioDir10;
                //        statSubScenarioName = statSubScenarioName10;
                //        statSubScenarioSplitName = statSubScenarioName;
                //        facilitySpecificNewFile = facilitySpecificFile10;
                //        break;
                //    case 11:
                //        // Xpert at tiers 1, 2, 3 and some 4 only - smear at tier 4 - Xpert at 169 sites - 29 DNO suggested sites
                //        statSubScenarioDir = statSubScenarioDir11;
                //        statSubScenarioName = statSubScenarioName11;
                //        statSubScenarioSplitName = statSubScenarioName;
                //        facilitySpecificNewFile = facilitySpecificFile11;
                //        break;
                //    case 21:
                //        // start at baseline and switch to centralized scenario
                //        statSubScenarioDir = "Base-" + statSubScenarioDir7;
                //        statSubScenarioName = statSubScenarioName9;
                //        statSubScenarioSplitName = statSubScenarioName7;
                //        facilitySpecificNewFile = facilitySpecificFile9;
                //        facilitySpecificSplitFile = facilitySpecificFile7;
                //        splitScenario = true;
                //        break;
                //    case 22:
                //        // start at baseline and switch to centralized scenario
                //        statSubScenarioDir = "Base-" + statSubScenarioDir8;
                //        statSubScenarioName = statSubScenarioName9;
                //        statSubScenarioSplitName = statSubScenarioName8;
                //        facilitySpecificNewFile = facilitySpecificFile9;
                //        facilitySpecificSplitFile = facilitySpecificFile8;
                //        splitScenario = true;
                //        break;
                //    case 23:
                //        // start at baseline and switch to centralized scenario
                //        statSubScenarioDir = "Base-" + statSubScenarioDir10;
                //        statSubScenarioName = statSubScenarioName9;
                //        statSubScenarioSplitName = statSubScenarioName10;
                //        facilitySpecificNewFile = facilitySpecificFile9;
                //        facilitySpecificSplitFile = facilitySpecificFile10;
                //        splitScenario = true;
                //        break;
                //    case 24:
                //        // start at baseline and switch to baseline plus 29 DNO sites
                //        statSubScenarioDir = "Base-" + statSubScenarioDir11;
                //        statSubScenarioName = statSubScenarioName9;
                //        statSubScenarioSplitName = statSubScenarioName11;
                //        facilitySpecificNewFile = facilitySpecificFile9;
                //        facilitySpecificFile = facilitySpecificFile9;
                //        facilitySpecificSplitFile = facilitySpecificFile11;
                //        splitScenario = true;
                //        break;
                //    // added these 4 cases to handle jobnumbers on Rockfish
                //    case 91:
                //        // Xpert at tiers 1, 2, 3 and some 4 only - smear at tier 4
                //        statSubScenarioDir = statSubScenarioDir9;
                //        statSubScenarioName = statSubScenarioName9;
                //        statSubScenarioSplitName = statSubScenarioName;
                //        facilitySpecificNewFile = facilitySpecificFile9;
                //        statSubDirNo = statSubDirNo + 10000;
                //        statSubDir = to_string(statSubDirNo);
                //        break;
                //    case 92:
                //        // Xpert at tiers 1, 2, 3 and some 4 only - smear at tier 4
                //        statSubScenarioDir = statSubScenarioDir9;
                //        statSubScenarioName = statSubScenarioName9;
                //        statSubScenarioSplitName = statSubScenarioName;
                //        facilitySpecificNewFile = facilitySpecificFile9;
                //        statSubDirNo = statSubDirNo + 20000;
                //        statSubDir = to_string(statSubDirNo);
                //        break;
                //    case 93:
                //        // Xpert at tiers 1, 2, 3 and some 4 only - smear at tier 4
                //        statSubScenarioDir = statSubScenarioDir9;
                //        statSubScenarioName = statSubScenarioName9;
                //        statSubScenarioSplitName = statSubScenarioName;
                //        facilitySpecificNewFile = facilitySpecificFile9;
                //        statSubDirNo = statSubDirNo + 30000;
                //        statSubDir = to_string(statSubDirNo);
                //        break;
                //    case 94:
                //        // Xpert at tiers 1, 2, 3 and some 4 only - smear at tier 4
                //        statSubScenarioDir = statSubScenarioDir9;
                //        statSubScenarioName = statSubScenarioName9;
                //        statSubScenarioSplitName = statSubScenarioName;
                //        facilitySpecificNewFile = facilitySpecificFile9;
                //        statSubDirNo = statSubDirNo + 40000;
                //        statSubDir = to_string(statSubDirNo);
                //        break;
                //    default :
                //        // Xpert at tiers 1, 2, 3 and some 4 only - smear at tier 4
                //        statSubScenarioDir = statSubScenarioDir9;
                //        statSubScenarioName = statSubScenarioName9;
                //        statSubScenarioSplitName = statSubScenarioName;
                //        facilitySpecificNewFile = facilitySpecificFile9;
                //}

                std::cout << " **** scenario " << statSubScenarioDir << " loop = " << scenarioIndex << " value = " << scenarioNo[scenarioIndex] << " count = " << scenarioCount << " file = " << facilitySpecificNewFile << "\n";

                scenarioParam tempScenarioParam{ getScenarioParameters(scenarioIndex, scenParam) };

                switch (scenarioIndex) {

                    //added these four to bump job numbers for Rockfish
                case 91:
                    statSubDirNo = statSubDirNo + 10000;
                    statSubDir = to_string(statSubDirNo);
                    break;
                case 92:
                    statSubDirNo = statSubDirNo + 20000;
                    statSubDir = to_string(statSubDirNo);
                    break;
                case 93:
                    statSubDirNo = statSubDirNo + 30000;
                    statSubDir = to_string(statSubDirNo);
                    break;
                case 94:
                    statSubDirNo = statSubDirNo + 40000;
                    statSubDir = to_string(statSubDirNo);
                    break;
                }

                statSubScenarioDir = tempScenarioParam.statSubScenarioDir;
                statSubScenarioName = tempScenarioParam.statSubScenarioName;
                statSubScenarioSplitName = tempScenarioParam.statSubScenarioSplitName;
                facilitySpecificNewFile = tempScenarioParam.facilitySpecificNewFile;
                facilitySpecificFile = tempScenarioParam.facilitySpecificFile;
                facilitySpecificSplitFile = tempScenarioParam.facilitySpecificSplitFile;
                splitScenario = tempScenarioParam.splitScenario;

                // debug - true
                if (true) {

                    std::cout << " statSubScenarioDir " << statSubScenarioDir << " statSubScenarioName " << statSubScenarioName << " statSubScenarioSplitName " << statSubScenarioSplitName << "\n";
                    std::cout << " facilitySpecificNewFile " << facilitySpecificNewFile << " facilitySpecificFile " << facilitySpecificFile << " facilitySpecificSplitFile " << facilitySpecificSplitFile << " splitScenario " << splitScenario << "\n";
                }

                subDirBaseScenario =  "./stats/" + statSubScenarioDir;
                if (statSubDir.length() > 0) {
                    subDirBase = subDirBaseScenario + "/" + "job-" + statSubDir;
                }
                else {
                    subDirBase = subDirBaseScenario + "/" + "job-" + to_string(1);
                    statSubDir = 1;
                }
                subDirBaseInput = subDirBase + "/input/";

                // if reruning with the same input parameters, skip copying of input parameters
                copyInput("./stats/" + lastStatSubScenarioDir, subDirBaseScenario, scenarioNo[scenarioIndex], statSubDir, popBinFile, diseaseFile, facilitiesFile, scenParam, parameterFlagFile, filenameFile, regDistFile, populationFile, regionDistrictSpecificFile, runSpecificFile, testFile);
                
                // changed this code, instead of copying the different facility specific files into "facility-specific-data.csv", we now carry all
                // the facility-specific files into each job run so that we can re-run different scenarios with same input parameters
                // // read in last facilitySpecificData file
                ////vector<vector<string>> lastFacilitySpecificInput = readDataInput(defaultDir + facilitySpecificDir + fsf);
                //vector<vector<string>> lastFacilitySpecificInput = readDataInput("./defaults/input/" + facilitySpecificDir + facilitySpecificNewFile);
            
                // // read in new lastFacilitySpecificInput file
                //vector<vector<string>> facilitySpecificInput = readDataInput(subDirBaseInput +  facilitySpecificDir + facilitySpecificFile);
            
                //if (debug) {
                //
                //    std::cout << "Copy facility specific input new = " <<  subDirBaseInput +  facilitySpecificDir + facilitySpecificFile;
                //    std::cout << ", old = " <<"./defaults/input/" + facilitySpecificDir + facilitySpecificNewFile  << " \n";
                //}

                //// now copy the input parameters from the last scenario facilitySpecificData.csv file to the new facilitySpecificData.csv file
                //inputCopyFacility(lastFacilitySpecificInput, facilitySpecificInput, "facility-specific-data.csv", statSubScenarioDir, statSubDir );

                facilitySpecificFile = facilitySpecificNewFile;
            }


            // make sure these are all correct -  change input directory to ./stats/input 
            inputDir = subDirBaseInput;
            diseaseInput = inputDir + diseaseDir + diseaseFile;
            diseaseTBUntreatedMortInput = inputDir + diseaseDir + diseaseTBUntreatedMortFile;
            testInput = inputDir + testDir + testFile;
            facilitiesInput = inputDir + facilitiesDir + facilitiesFile;
            facilitiesInputNoTop = inputDir + facilitiesDir + facilitiesFileNoTop;
            populationInput = inputDir + populationDir + populationFile;
            popBinInput = inputDir + popBinDir + popBinFile;
            facilitySpecificInput = inputDir + facilitySpecificDir + facilitySpecificFile;
            regionDistrictSpecificInput = inputDir + regionDistrictSpecificDir + regionDistrictSpecificFile;
            runSpecificInput = inputDir + runSpecificDir + runSpecificFile;
            parameterInput = inputDir + parameterDir + parameterFlagFile;
            
            if (debug) {
            
                std::cout << "Copy facility specific  = " <<  regionDistrictSpecificInput << ", runSpecificInput = " << runSpecificInput  << " \n";
                std::cout << "before reading input files inputDir  = " <<  inputDir << ", subDirBaseInput = " << subDirBaseInput  << " \n";
            }

            // read in un specific parameters - then get the seed and disease list
            vector<vector<string>> runSpecific = readDataInput(runSpecificInput);
        
            std::cout << "after read run specific input " << runSpecificInput << " - before getSeed \n";

            // update seed, starting year, and which diseases to analyze
            seed = getSeed(runSpecific);
            getYears(runSpecific);
            diseaseList = getDiseaseList(runSpecific);


            // hardcode the disease list for now - eventually become global variable
            //diseaseList.push_back("TB");
            //diseaseList.push_back("HIV");
            //diseaseList.push_back("HCV");
            //diseaseList.push_back("Yellow Fever");
            //diseaseList.push_back("Meningitis");
            //diseaseList.push_back("Measles");
            //diseaseList.push_back("Disease1");
            //diseaseList.push_back("Disease2");

            // get active tiers and store in GlobalParameters
            getTiers(runSpecific);

            //read in input files and start from scratch

            // read in different disease parameters
            vector<vector<string>> diseases = readDataInput(diseaseInput);

            if (diseases.size() < 2) {
                return 0;
            }

            // debug - true
            if (debug) {
                std::cout << "after readDiseaseObjects!\n";
            }

            // read in untreated mortality values for TB - switch this to just use single value for ltfu untreated death percentage
            //vector<vector<string>> diseaseTBUntreatedMort = readDataInput(diseaseTBUntreatedMortInput);

            // create Disease objects from input file
            //dis = createDiseaseObjects(diseases, diseaseTBUntreatedMort);
            dis = createDiseaseObjects(diseases, runSpecific);

            
            // debug - true
            if (debug) {
                std::cout << "after createDiseaseObjects!\n";
            }

            // read in different tests parameters
            vector<vector<string>> tests = readDataInput(testInput);

            if (tests.size() < 2) {
                return 0;
            }

            if (debug) {
                std::cout << "after readTestObjects!\n";
            }

            // create Disease objects from input file
            tes = createTestObjects(tests);

            if (debug) {
                std::cout << "after createTestObjects!\n";
            }

            std::cout << "after creating disease and test objects \n";

            // read in information for tier for facility
            //vector<vector<string>> facInput = readInputFile("D:/work/ghana-fac/tier3-4/reg-hosp/facilities.csv");
            //vector<vector<string>> facInput = readInputFile(facilitiesInput);
            // use latest format
            //vector<vector<string>> facInput = readInputFile6("./defaults/input/facilities/case-reg-1337-with-dist.csv");
            vector<vector<string>> facInput = readInputFile6(facilitiesInput);
            
            if (facInput.size() < 2) {
                return 0;
            }

            if (debug) {
                std::cout << "after readInputFile size = " << facInput.size() << "\n";
            }

            // read in population in format (longitude, latitude, population)
            // read in facility nam, total population, pop density probability bins and bin increment value
            // The distance to the fartherest population center in catchment is broken down into
            // 10 bins and the probability of a Patient being in any of the bins is calculated
            // the incremental distance of the bins is also stored
            // use 1 if you have the population bin file already, use 2 if you need to build it from population clusters

            // ***** 2 ****** read in the ghana population in population clusters
            vector<vector<string>> populationData = readDataInput(populationInput);
            if (populationData.size() < 2) {
                return 0;
            }

            // **** 1 ***** check to see if you already have a pop bin csv file - if so use it, otherwise create it
            //vector<vector<string>> popData = readDataInput("D:/work/ghana-fac/tier3-4/pop_2km_catch_tier34_percentage.csv");
            vector<vector<string>> popData = readDataInput(popBinInput);

            if (popData.size() < 2) {

                std::cout << "Creating new population bin file \n";
                
                // ****** 2 ******* otherwise calculate the population bins from populationData and facInput. 
                // this routine will write them out to ./stats/Facility-Catchment-Population-Distribution.csv
                // move them out to ./input/catchment/Facility-Catchment-Population-Distribution.csv
                //vector<vector<string>> popData;
                if (true) {
            
                    // Read in travel time catchments from accessmod 5 and use time instead of distance for population distribrutions for facilities
                    vector<vector<string>> catchData = readDataInput(travelTimesInput);
                    if (debug) {
                        std::cout << "after reading newTravelTimes!\n";
                    }
                    if (catchData.size() < 2) {
                        return 0;
                    }
            
                    std::cout << "Before create popBins \n";
                    
                    popData = createPopBins(facInput, populationData, catchData, 2);

                    std::cout << "After create popBins \n";
                }
                else {
           
                    // use crows fly distance calculation for population distribution bins
                    popData = createPopBins(facInput, populationData, populationData, 1);
                }
            }

            // debug - true
            if (debug) {
                std::cout << "after readPopData\n";
            }

            // read in disease specific information for each region or district from file
            vector<vector<string>> regionDistrictSpecific = readDataInput(regionDistrictSpecificInput);
            if (regionDistrictSpecific.size() < 2) {
                std::cout << "No region/district specific file, using defaults.\n";
            }

            if (debug) {
                std::cout << "after readRegionDistrictSpecific Disease Data\n";
            }


            // read in disease specific information for each facility from file
            //vector<vector<string>> facilitySpecific = readDataInput("D:/work/ghana-fac/tier3-4/facility-specific-disease-data.csv");
            vector<vector<string>> facilitySpecific = readDataInput(facilitySpecificInput);
            if (facilitySpecific.size() < 2) {
                std::cout << "No facility specific file, using defaults.\n";
            }
            
            // debug - true
            if (true) {
                std::cout << "after read facility specific input " << facilitySpecificInput << "\n";
            }


            // after reading in all the input files, change any run specific parameters
            // if anything in the file, adjust other input files
            if (facilitySpecific.size() > 1) {
                changeFiles(runSpecific, dis, tes);

                std::cout << "after readRunSpecificDiseaseData\n";
            }

            if (debug) std::cout << "files regionDistrictSpecificInput list = " << regionDistrictSpecificInput << " facilitiesInput = " << facilitiesInput << "\n";

     
            // create facilities from input file
            //fac = createFacilitesFromInput(facInput, popData);
            createFacilitesFromInput(facilitiesList, facInput, popData, regionDistrictSpecific, facilitySpecific, diseaseList, dis, tes, regDist.regionDistrict, statSubDirNo);
            
            // create facilities array of name and id for facility stat keeping
            regionDistrict facList = createFacilitiesArray(facilitiesList);

            // dump facList debug - true
            if (debug) {

                for (int i = 0; i < noFacilities + 1; i++) {

                    std::cout << " facility = " << facList.regionDistrict[i] << ", " << facList.districtToRegionMapping[i];
                }
                std::cout << "\n";
            }

            // This is now stored with disease and is called prpIncidencePresent
            //if (debug) std::cout << "after createFacilitiesFromInput incAdjustment = " << incAdjustment << "\n";

            // debug - true
            if (debug) {
                std::cout << "after createFacilitiesFromInput!\n";
                if (sampleStatsFlag)  sampleStats(facilitiesList, diseaseList, false, true, timeStep);
                if (agentStatsFlag)   agentStats(facilitiesList, diseaseList, dis, false, agentPatientsStatsFlag, agentPatientsSamplingStatsFlag, true, timeStep);
            }
            
            // create region and district objects from facilities list and populate with data from facilities
            reg = createRegionObjects(regDist.regionDistrict);
            if (debug) std::cout << "after createRegionObject!\n";

            dist = createDistrictObjects(regDist.regionDistrict);
            if (debug) std::cout << "after createDistrictObjects!\n";

            updateRegionAndDistrictsFromFacilities(facilitiesList, dist, reg);

            if (debug) std::cout << "after createFacilitiesFromInput!\n";

            //} old snapshot code
   
            // get pointer to Facility List
            fac = facilitiesList->getListPtr();

            // debug - true
            if (true) {
                // map all facilities 
                mapFacilities(facilitiesList, diseaseList, true);

                std::cout << "mapping facilities\n";
            }




            // skip if read snapshot, random numbers already loaded
            if (!readSnapshotFlag) {
   
                // load seed for random number generator - default = 0
                //std::mt19937 generator(seed);
                //std::uniform_int_distribution<int> randDist100(1, 100);
                //std::uniform_int_distribution<int> randDistPop(1,population*100);
                if (seed == 1) {
    
                    seed = std::chrono::system_clock::now().time_since_epoch().count();
                }

                // make an array of random numbers for 1-100 and 1 for 1 to population*incidence
                makeRandomNo100( seed, 100 );

                // loop through disease list and make a random number array for each disease based on population and disease incidence
                for (auto disease : diseaseList) {

                    population = 0;
        
                    vector<Facility*> facilities = facilitiesList->getListPtr();
                    for (auto facility : facilities) {
                

                        population = population + (facility->getPopulation() * facility->getDiseaseIncidence(disease));  

                        if (debug) std::cout << "total pop = " << population << ", disease = " << disease << ", fac pop = " << facility->getPopulation() << " fac inc = " << facility->getDiseaseIncidence(disease) << "\n";

                        tempFacName = facility->getName();
                        tempIncidence = facility->getPopulation() * facility->getDiseaseIncidence(disease);
                        
                        //std::cout << ", facility = " << facility->getName() << ", population = " << facility->getPopulation() << ", incidence = " << facility->getDiseaseIncidence(disease) << ", popcalc = " << (facility->getPopulation() * facility->getDiseaseIncidence(disease)) << "\n";
                    }
                    // debug - true
                    if (debug) std::cout << ", total pop = " << population << ", current year = " << currentYearInRun << ", disease = " << disease << " facility = " << tempFacName << " incidence = " << tempIncidence <<  "\n";
        
                    // problems occur if population is too small.  For example, incidence is zero in all districts but one
                    if (population < .01) {

                        // this will give a random number spread less than 1, default it to 1
                        population = .01;

                        std::cout << "Total population plus incidence too low to generate random no. distribution for " << disease << ", reset to 1" << "\n";

                    }
                    if (disease == "TB") {
                        popTB = population*100;
                    }
                    else if (disease == "HIV") {
                        popHIV = population*100;
                    }
                    else if (disease == "HCV") {
                        popHCV = population*100;
                    }
                    else if (disease == "Yellow Fever") {
                        popYF = population*100;
                    }
                    else if (disease == "Meningitis") {
                        popMen = population*100;
                    }
                    else if (disease == "Measles") {
                        popMea = population*100;
                    }
                    else if (disease == "Disease1") {
                        popDis1 = population * 100;
                    }
                    else if (disease == "Disease2") {
                        popDis2 = population * 100;
                    }
                }

                // debug - true
                if (debug) std::cout << "disease pop tb = " << popTB << ", popHIV = " << popHIV <<  ", popHCV = " << popHCV << ", popYF = " << popYF << ", popMen = " << popMen << ", popMea = " << popMea << ", popDis1 = " << popDis1 << ", popDis2 = " << popDis2 << "\n";

                makeRandomNoPop( seed, diseaseList );

                // debug - true
                if (debug) std::cout << "before makeRandomGeoNo" << "\n";
                
                // make an array of random exponential numbers
                //makeRandomExpNo(seed);

                // make an array of random geometric numbers
                makeRandomGeoNo(seed, pRepresentation);

                if (debug) outputRandomGeoNo(seed, pRepresentation);
            }
        
            std::cout << "before parameterStats" << "\n";
            // output input parameters to file before any calculations, that way if it fails - you can track reasons
            parameterStats(dis, tes, facilitiesList);
            //std::cout << "after parameterStats" << "\n";

            // initialize - first step - create patients and samples on time step one
            // Diagnosis–create new Patients(infected, non - infected) and Samples
            // this is now done inside timeStep loop
            //createPatients(fac, facilitiesList, diseaseList, dis, tes, 0);

            /*if (debug) std::cout << "after first create patients" <<  "\n";

            if (debug) {
                std::cout << "after initialization\n";

                if (sampleStatsFlag)   sampleStats(facilitiesList, diseaseList, true, true, timeStep);

                if (agentStatsFlag)    agentStats(facilitiesList, diseaseList,  dis, true, agentPatientsStatsFlag, agentPatientsSamplingStatsFlag, true, timeStep);
            }*/

            // debug - true
            if (debug) std::cout << "before main loop referenceYear = " << referenceYear << " startingYear = " << startingYear << " splitYear = " << splitYear << " skipYears = " << skipYears << " yearsInRun = " << yearsInRun <<  "\n";
            
            // time step loop - assume timesteps are 24 hours
            // timeStep is either 1 (default) or read in from snapshot
            for (int timeStep = 0; timeStep <= noSteps; timeStep++) {
                 
                // skipYears is the number of years between referenceYear and startingYear
                // this allows the population to grow and incidence to change during the difference years
                if ((timeStep - 1) / timestepsPerYear >= skipYears) {

                    //std::cout << "timestep =  " << timeStep << "\n";
                    //if (timeStep == 635) debug = true;

                    // if split scenario, change after certain number of years
                    if (((timeStep - (timestepsPerYear * splitYear + 1)) == 0) && (splitScenario)) {

                        // load in new scenario facilites mapping
                        statSubScenarioName = statSubScenarioSplitName;
                        facilitySpecificInput = inputDir + facilitySpecificDir + facilitySpecificSplitFile;

                        facilitySpecific = readDataInput(facilitySpecificInput);
                        if (facilitySpecific.size() < 2) {
                            std::cout << "No facility specific file, using defaults.\n";
                        }

                        // change the Xpert placement to match new scenario for TB
                        changeXpert(facilitiesList, facilitySpecific);

                        // dump facility attributes
                        // debug - true
                        if (debug) dumpFacilityAttributes(facilitiesList);


                        // now have to update lateral, up and down connections for facilities - because closest facility with testing may have changed
                        updateFacilityConnections(facilitiesList);

                        std::cout << "switching to scenario " << statSubScenarioName << " at timeStep = " << timeStep << " file = " << facilitySpecificInput << "\n";
                    }

                    // debug - true - false
                    if (debug) {
                        if (timeStep == (5 * timeStepStatOutput)) {

                            // save snapshot of data - test
                            saveSnapshot("./snapshot/snapshot.csv", facilitiesList, diseaseList, dis, tes);
                        }
                    }
                    // debug - true
                    if (debug) std::cout << "after save snapshot" << "\n";

                    // using built-in random generator:
                    std::random_shuffle(fac.begin(), fac.end());

                    // 4.Diagnosis–create new Patients(infected, non - infected) and Samples
                    // for (auto facility : fac) {

                        // create new Patients for this timestep
                        //     facility->createPatients(diseaseList, dis, timestep, generator, randDist100, randDistPop);
                    //}
                    createPatients(fac, facilitiesList, diseaseList, dis, tes, timeStep);

                    // debug - true - false
                    if (debug) std::cout << "after 4a. create new patients" << "\n";

                    if (debug) {
                        if (timeStep % timeStepStatOutput == 0) {

                            std::cout << " year = " << currentYearInRun << "   time step = " << timeStep << "\n";

                            if (sampleStatsFlag)  sampleStats(facilitiesList, diseaseList, true, true, timeStep);
                            if (agentStatsFlag)   agentStats(facilitiesList, diseaseList, dis, true, agentPatientsStatsFlag, agentPatientsSamplingStatsFlag, true, timeStep);
                            if (vizStatFlag)      visualizationStats(facilitiesList, diseaseList, true, true, timeStep);
                            //if (resourceStatsFlag)        resourceStats(facilitiesList, false, timeStep);
                        }
                    }
                    // debug - true - false
                    if (debug) std::cout << "after 4. create new patients" << "\n";

                    // 1. Testing–reset resources(if new supplies  come in)
                    // reset no. of tests done today at each facility
                    for (auto facility : fac) {

                        facility->resetResources(timeStep);

                        //std::cout << "after reset resources " << "\n";
                    }
                    if (debug) std::cout << "after 1. testing-reset resources timestep = " << timeStep << "\n";

                    if (debug) {
                        if (timeStep % timeStepStatOutput == 0) {

                            //std::cout << "after reset Tests " << "\n";
                            if (sampleStatsFlag)  sampleStats(facilitiesList, diseaseList, true, true, timeStep);
                            if (agentStatsFlag)   agentStats(facilitiesList, diseaseList, dis, true, agentPatientsStatsFlag, agentPatientsSamplingStatsFlag, true, timeStep);
                            if (vizStatFlag)      visualizationStats(facilitiesList, diseaseList, true, true, timeStep);
                        }
                    }
                    if (debug) std::cout << "after 1. testing-reset resources output timestep = " << timeStep << "\n";

                    // 2.Samples –Check to see if current Facility has Testing to change status, if so reduce Testingresources
                    //    or move to another Facility.Some get lost along the way or expire, may take multiple  time steps to get to new Facility
                    for (auto facility : fac) {

                        for (auto diseaseName : diseaseList) {

                            facility->updateSamples(timeStep, diseaseName, dis);
                        }

                    }
                    if (debug) std::cout << "after 2. update samples" << "\n";

                    if (debug) {
                        if (timeStep % timeStepStatOutput == 0) {

                            std::cout << "after update Samples " << "\n";
                            if (sampleStatsFlag)  sampleStats(facilitiesList, diseaseList, true, true, timeStep);
                            if (agentStatsFlag)   agentStats(facilitiesList, diseaseList, dis, true, agentPatientsStatsFlag, agentPatientsSamplingStatsFlag, true, timeStep);
                            if (vizStatFlag)      visualizationStats(facilitiesList, diseaseList, true, true, timeStep);
                        }
                    }

                    // 3. Patients –Update Patient status, check for sample  return, determine  if Patient will  return for results  based  on distance.
                    //    Run general  probability  statistics  on patients that receive a Sample result or don’t return
                    for (auto facility : fac) {

                        // update existing Patients with respect to Samples
                        facility->updatePatients(dis, tes, timeStep);
                    }

                    if (debug) {
                        if (timeStep % timeStepStatOutput == 0) {

                            std::cout << "after update patients " << "\n";
                            if (sampleStatsFlag)  sampleStats(facilitiesList, diseaseList, true, true, timeStep);
                            if (agentStatsFlag)   agentStats(facilitiesList, diseaseList, dis, true, agentPatientsStatsFlag, agentPatientsSamplingStatsFlag, true, timeStep);
                            if (vizStatFlag)      visualizationStats(facilitiesList, diseaseList, true, true, timeStep);
                        }
                    }
                    if (debug) std::cout << "after 3. update patients" << "\n";

                    //// 4.Diagnosis–create new Patients(infected, non - infected) and Samples
                    //// for (auto facility : fac) {

                    //    // create new Patients for this timestep
                    //    //     facility->createPatients(diseaseList, dis, timestep, generator, randDist100, randDistPop);
                    ////}
                    //createPatients(fac, facilitiesList, diseaseList, dis, tes, timeStep);
                    //if (debug) std::cout << "after 4a. create new patients" << "\n";

                    //if (debug) {
                    //    if (timeStep % timeStepStatOutput == 0) {

                    //        std::cout << " year = " << currentYearInRun << "   time step = " << timeStep <<  "\n";

                    //        if (sampleStatsFlag)  sampleStats(facilitiesList, diseaseList, true, true, timeStep);
                    //        if (agentStatsFlag)   agentStats(facilitiesList, diseaseList, dis, true, agentPatientsStatsFlag,  agentPatientsSamplingStatsFlag, true, timeStep);
                    //        if (vizStatFlag)      visualizationStats(facilitiesList, diseaseList, true, true, timeStep);
                    //        //if (resourceStatsFlag)        resourceStats(facilitiesList, false, timeStep);
                    //    }
                    //}
                    //if (debug) std::cout << "after 4. create new patients" << "\n";

                    // save away output for each year run
                    if ((timeStep % timeStepStatOutput == 0) && (timeStep > 0)) {

                        currentYearInRun = timeStep / timestepsPerYear;

                        std::cout << " year = " << referenceYear + currentYearInRun << "   time step = " << timeStep << "\n";

                        // create a subdirectory for each year and store stats
                        if (sampleStatsFlag)          sampleStats(facilitiesList, diseaseList, true, true, timeStep);
                        if (sampleInactiveStatsFlag)  sampleInactiveStats(facilitiesList, diseaseList, true, true, timeStep);
                        if (agentStatsFlag)           agentStats(facilitiesList, diseaseList, dis, true, agentPatientsStatsFlag, agentPatientsSamplingStatsFlag, true, timeStep);
                        if (resourceStatsFlag)        resourceStats(facilitiesList, true, timeStep);

                        if (statsCount) {

                            // run through diseases and compare with list of dieases.  reset individual counters, update vector of counters
                            for (auto d : dis) {

                                for (auto dl : diseaseList) {

                                    if (d->getName() == dl) {

                                        //(d->getStatsPtr())->runSaveAnnual(timeStep);
                                        //(d->getFacStatsPtr())->runSaveAnnual(timeStep);
                                        (d->getStatsPtr())->record_year(referenceYear + currentYearInRun - 1);
                                        (d->getFacStatsPtr())->record_year(referenceYear + currentYearInRun - 1);
                                        (d->getStatsPtr())->record_scenario(statSubScenarioName);
                                        (d->getFacStatsPtr())->record_scenario(statSubScenarioName);
                                        (d->getStatsPtr())->record_disease(d->getName());
                                        (d->getFacStatsPtr())->record_disease(d->getName());
                                        updateStatsPopulation(reg, dist, d->getStatsPtr(), currentYearInRun);
                                        updateStatsPopulation(reg, dist, d->getFacStatsPtr(), currentYearInRun);
                                        updateStatsIncidence(reg, dist, d->getStatsPtr(), d->getName(), currentYearInRun);
                                        updateStatsIncidence(reg, dist, d->getFacStatsPtr(), d->getName(), currentYearInRun);
                                        updateStatsXpert(reg, dist, d->getStatsPtr(), facilitiesList);
                                        updateStatsXpert(reg, dist, d->getFacStatsPtr(), facilitiesList);
                                        (d->getStatsPtr())->calc_calcIncidence();
                                        (d->getFacStatsPtr())->calc_calcIncidence();

                                        // collect infectious days for active patients
                                        activePatientInfDays(d, facilitiesList, d->getStatsPtr());
                                        activePatientInfDays(d, facilitiesList, d->getFacStatsPtr());
                                        
                                        if (debug) {
                                            std::cout << " year = " << referenceYear + currentYearInRun << " incidence = " << d->getIncidence()*365*100000 << "\n";
                                        }

                                        // debug - true
                                        if (debug) {
                                            
                                            std::cout << "mdcd inf = " << (d->getStatsPtr())->return_nMDCDInfected(0) << " mdcd uninfected = " << (d->getStatsPtr())->return_nMDCDUninfected(0) << "\n";
                                            std::cout << "mdcd pre-test inf = " << (d->getStatsPtr())->return_nPTMDCDInfected(0) << " mdcd uninfect = " << (d->getStatsPtr())->return_nPTMDCDUninfected(0) << "\n";
                                            std::cout << "mdcd pre-test inf per = " << d->getPerPreTestMDCDInf() << " no = " << d->getPerPreTestMDCDInf() * (d->getStatsPtr())->return_nPTMDCDInfected(0) << " mdcd uninfect per= " << d->getPerPreTestMDCDUnInf() << " no = " << d->getPerPreTestMDCDUnInf() * (d->getStatsPtr())->return_nPTMDCDUninfected(0) << "\n";
                                            std::cout << "mdcd pre-test true-pos = " << (d->getStatsPtr())->return_nPreTestDiagnosisTruePos(0) << " mdcd false-pos = " << (d->getStatsPtr())->return_nPreTestDiagnosisFalsePos(0) << "\n";

                                            std::cout << "true pos = " << (d->getStatsPtr())->return_nTestTruePos(0) << " true neg = " << (d->getStatsPtr())->return_nTestTrueNeg(0);
                                            std::cout << " false neg = " << (d->getStatsPtr())->return_nTestFalseNeg(0) << " false pos = " << (d->getStatsPtr())->return_nTestFalsePos(0);
                                            std::cout << " md diag inf = " << (d->getStatsPtr())->return_nPreTestDiagnosisTotal(0) << " ndiagnosis = " << (d->getStatsPtr())->return_nDiagnosis(0) << "\n";
                                            std::cout << " inf days = " << (d->getStatsPtr())->return_nPatientInfDaysYear4(0) << " , " << (d->getStatsPtr())->return_nPatientInfDaysYear5(0) << " , " << (d->getStatsPtr())->return_nPatientInfDaysYear6(0) << "\n";
                                        }

                                        (d->getStatsPtr())->recordOutputs();
                                        (d->getFacStatsPtr())->recordOutputs();
                                    }
                                }
                            }
                        }

                        // need to run after record stats for this year
                        if (vizStatFlag) {

                            countStatDiseases = 0;

                            // run through diseases and compare to disease list.  reset individual counters, update vector of counters
                            for (auto d : dis) {

                                for (auto dl : diseaseList) {

                                    if (d->getName() == dl) {

                                        // count the number of diesease being output to suppress extra headers
                                        countStatDiseases++;

                                        // output stats file with and without commas
                                        if (countStatDiseases == 1) {

                                            (d->getStatsPtr())->runSaveAnnualString(d->getName(), statSubScenarioDir, statSubDir, regDist.regionDistrict, regDist.districtToRegionMapping, facList.regionDistrict, true, true, true, false, currentYearInRun, true, true);
                                            (d->getFacStatsPtr())->runSaveAnnualString(d->getName(), statSubScenarioDir, statSubDir, facList.regionDistrict, facList.districtToRegionMapping, facList.regionDistrict, true, false, false, true, currentYearInRun, true, true);
                                        }
                                        else {

                                            // 2nd disease and beyond, suppress column headers and append to existing files
                                            (d->getStatsPtr())->runSaveAnnualString(d->getName(), statSubScenarioDir, statSubDir, regDist.regionDistrict, regDist.districtToRegionMapping, facList.regionDistrict, true, true, true, false, currentYearInRun, false, true);
                                            (d->getFacStatsPtr())->runSaveAnnualString(d->getName(), statSubScenarioDir, statSubDir, facList.regionDistrict, facList.districtToRegionMapping, facList.regionDistrict, true, false, false, true, currentYearInRun, false, true);
                                        }
                                    }
                                }
                            }

                            visualizationStats(facilitiesList, diseaseList, true, true, timeStep);
                        }

                    }

                    //std::cout << "Random number indexes nextNoSeq100 = " << nextNoSeq100 << ", nextNoSeqPopTB = " << nextNoSeqPopTB << ", nextNoSeqPopHIV = " << nextNoSeqPopHIV <<"\n";

                    // see if getting within 25% of end of random number arrays, if so, generate the next group of random numbers and put in arrays
                    checkRandomNumbers(seed, diseaseList, pRepresentation);
                }       // loop for skipped years
            }
            
            timeStep = noSteps;
            

            //std::cout << "Before debug final current year = " << currentYearInRun <<"\n";

            if (debugFinal) {

                if (sampleStatsFlag)          sampleStats(facilitiesList, diseaseList, true, true, timeStep);

                if (sampleInactiveStatsFlag)  sampleInactiveStats(facilitiesList, diseaseList, true, true, timeStep);

                if (agentStatsFlag)           agentStats(facilitiesList, diseaseList, dis, true, agentPatientsStatsFlag, agentPatientsSamplingStatsFlag, false, timeStep);

                //if (vizStatFlag)            visualizationStats(facilitiesList, diseaseList, true, false, timeStep);
        
                if (resourceStatsFlag)        resourceStats(facilitiesList, true, timeStep);
                //std::cout << "after resource stats, current year = " << currentYearInRun << "\n";

                // calcluate year for output file - some factor of timesteps
                year = 2016;
                if (tableauStatsFlag)        tableauStats(facilitiesList, diseaseList, tes, true, true, timeStep, year);
                //std::cout << "after tableauStats" << "\n";

                // output individual stats counts
                if (statsCount) {

                    countStatDiseases = 0;
    
                    // run through diseases and compare to disease list.  reset individual counters, update vector of counters
                    for (auto d : dis) {
            
                        for (auto dl : diseaseList) {
                        
                            if (d->getName() == dl) {

                                // count the number of diesease being output to suppress extra headers
                                countStatDiseases++;
                            
                                //std::cout << " before active inf days = " << (d->getStatsPtr())->return_nPatientInfDaysYear4(0) << " , " << (d->getStatsPtr())->return_nPatientInfDaysYear5(0) << " , " << (d->getStatsPtr())->return_nPatientInfDaysYear6(0) << "\n";
                                //std::cout << " before active inf active days = " << (d->getStatsPtr())->return_nPatientInfDaysActYear1(0) << " , " << (d->getStatsPtr())->return_nPatientInfDaysActYear2(0) << " , " << (d->getStatsPtr())->return_nPatientInfDaysActYear3(0);
                                //std::cout << " , " << (d->getStatsPtr())->return_nPatientInfDaysActYear4(0) << " , " << (d->getStatsPtr())->return_nPatientInfDaysActYear5(0) << " , " << (d->getStatsPtr())->return_nPatientInfDaysActYear6(0);
                                //std::cout << " , " << (d->getStatsPtr())->return_nPatientInfDaysActYear6(0) << " , " << (d->getStatsPtr())->return_nPatientInfDaysActYear8(0) << " , " << (d->getStatsPtr())->return_nPatientInfDaysActYear9(0) << "\n";

                                //std::cout << " after active inf days = " << (d->getStatsPtr())->return_nPatientInfDaysYear4(0) << " , " << (d->getStatsPtr())->return_nPatientInfDaysYear5(0) << " , " << (d->getStatsPtr())->return_nPatientInfDaysYear6(0) << "\n";
                                //std::cout << " after active inf active days = " << (d->getStatsPtr())->return_nPatientInfDaysActYear1(0) << " , " << (d->getStatsPtr())->return_nPatientInfDaysActYear2(0) << " , " << (d->getStatsPtr())->return_nPatientInfDaysActYear3(0);
                                //std::cout << " , " << (d->getStatsPtr())->return_nPatientInfDaysActYear4(0) << " , " << (d->getStatsPtr())->return_nPatientInfDaysActYear5(0) << " , " << (d->getStatsPtr())->return_nPatientInfDaysActYear6(0);
                                //std::cout << " , " << (d->getStatsPtr())->return_nPatientInfDaysActYear7(0) << " , " << (d->getStatsPtr())->return_nPatientInfDaysActYear8(0) << " , " << (d->getStatsPtr())->return_nPatientInfDaysActYear9(0) << "\n";
                                
                                // output stats file with and without commas
                                //(d->getStatsPtr())->runSaveAnnualString(d->getName(), statSubScenarioDir, statSubDir, regDist.regionDistrict, regDist.districtToRegionMapping, false, true, false);
                                if (countStatDiseases == 1) {

                                    (d->getStatsPtr())->runSaveAnnualString(d->getName(), statSubScenarioDir, statSubDir, regDist.regionDistrict, regDist.districtToRegionMapping, facList.regionDistrict, true, true, true, false, currentYearInRun, true, false);
                                    (d->getFacStatsPtr())->runSaveAnnualString(d->getName(), statSubScenarioDir, statSubDir, facList.regionDistrict, facList.districtToRegionMapping, facList.regionDistrict, true, false, false, true, currentYearInRun, true, false);

                                    std::cout << "year0 = " << (d->getStatsPtr())->return_nPatientInfDaysYear0(0) << " current year in run = " << currentYearInRun << "\n";
                                }
                                else {

                                    // 2nd disease and beyond, suppress column headers and append to existing files
                                    (d->getStatsPtr())->runSaveAnnualString(d->getName(), statSubScenarioDir, statSubDir, regDist.regionDistrict, regDist.districtToRegionMapping, facList.regionDistrict, true, true, true, false, currentYearInRun, false, false);
                                    (d->getFacStatsPtr())->runSaveAnnualString(d->getName(), statSubScenarioDir, statSubDir, facList.regionDistrict, facList.districtToRegionMapping, facList.regionDistrict, true, false, false, true, currentYearInRun, false, false);
                                }
                                //std::cout << "year1 = " << (d->getStatsPtr())->return_nPatientInfDaysYear1(0) << "\n";
                                //std::cout << "year2 = " << (d->getStatsPtr())->return_nPatientInfDaysYear2(0) << "\n";
                                //std::cout << "stats: inside agent stats after runSaveAnnualWCommas"  << "\n";
                            }
                        }
                    }
                }
            }
    

            // gather statistics for patients in each facility
            // Facility::gatherFacilityStatistics(vector<Facility*> facilities);


            if (debug) {
                std::cout << "after statistical gathering\n";
            }
        }

        // run through diseases and reset individual counters, update vector of counters
        //for (auto d : dis) {
                                                  
            //(d->getStatsPtr())->runSaveAnnual(timeStep);
            //(d->getFacStatsPtr())->runSaveAnnual(timeStep);
        //}

        if (debug) {
            std::cout << "after main loop \n";
        }

        if (statsCount) {
                    
            // run through diseases and reset individual counters, update vector of counters
            for (auto d : dis) {
         
                (d->getStatsPtr())->resetOutputs();
                (d->getFacStatsPtr())->resetOutputs();
            }
        }

        if (debug) {
            std::cout << "after reset outputs \n";
        }
        // delete current test, disease, patient, sample and facility objects 
        //cleanData(facilitiesList, tes, diseaseList, dis, reg, dist);

        if (debug) {
            std::cout << "after clean data \n";
        }
        // reset year number to 0
        currentYearInRun = 0;
   }  // scenarioCount loop

    return 0;
    
}

