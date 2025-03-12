//
//  District.h
//  mdrtb
//
//  Created by L. Dombrowski on 8/30/2022.
//

#ifndef DISTRICT_H
#define DISTRICT_H


#include <random>
#include <string>
#include <vector>
#include "GlobalParameters.h"


// class Testing;
// class Diagnosis;

using namespace  std;


class District {
private:

    // name of district
    string _name;
    int _id;
    int _population;
    float _TBincidence;
    float _TBincAdjFactor;
    float _TBprpIncidencePresenting;
    float _HIVincidence;
    float _HIVincAdjFactor;
    float _HIVprpIncidencePresenting;
    float _HCVincidence;
    float _HCVincAdjFactor;
    float _HCVprpIncidencePresenting;
    float _YFincidence;
    float _YFincAdjFactor;
    float _YFprpIncidencePresenting;
    float _Menincidence;
    float _MenincAdjFactor;
    float _MenprpIncidencePresenting;
    float _Meaincidence;
    float _MeaincAdjFactor;
    float _MeaprpIncidencePresenting;
    float _Dis1incidence;
    float _Dis1incAdjFactor;
    float _Dis1prpIncidencePresenting;
    float _Dis2incidence;
    float _Dis2incAdjFactor;
    float _Dis2prpIncidencePresenting;

    string _regionName;
    int _regionID;
    

public:
    District();
    District(string name, int id, float incidence);
    District(string name, int id, float incidence, string region, int regionID);
    ~District();

    // get district name
    inline string getName() { return _name; };
        
    // set district name
    inline void setName(string name) { _name = name; };

    // get district id
    inline int getID() { return _id; };
            
    // set district id
    inline void setID(int id) { _id = id; };
    
    // get population
    inline int getPopulation() { return _population; };
            
    // set population
    inline void setPopulation(int population) { _population = population; };
                  
    // increment population
    inline void incrementPopulation(int population) { _population = _population + population; };
         
    // get incidence
    inline float getTBIncidence() { return _TBincidence; };
    inline float getHIVIncidence() { return _HIVincidence; };
    inline float getHCVIncidence() { return _HCVincidence; };
    inline float getYFIncidence() { return _YFincidence; };
    inline float getMenIncidence() { return _Menincidence; };
    inline float getMeaIncidence() { return _Meaincidence; };
    inline float getDis1Incidence() { return _Dis1incidence; };
    inline float getDis2Incidence() { return _Dis2incidence; };
            
    // set incidence
    inline void setTBIncidence(float TBincidence) { _TBincidence = TBincidence; };
    inline void setHIVIncidence(float HIVincidence) { _HIVincidence = HIVincidence; };
    inline void setHCVIncidence(float HCVincidence) { _HCVincidence = HCVincidence; };
    inline void setYFIncidence(float YFincidence) { _YFincidence = YFincidence; };
    inline void setMenIncidence(float Menincidence) { _Menincidence = Menincidence; };
    inline void setMeaIncidence(float Meaincidence) { _Meaincidence = Meaincidence; };
    inline void setDis1Incidence(float Meaincidence) { _Dis1incidence = Meaincidence; };
    inline void setDis2Incidence(float Meaincidence) { _Dis2incidence = Meaincidence; };

    // get incidence adjustment factor
    inline float getTBIncAdjFactor() { return _TBincAdjFactor; };
    inline float getHIVIncAdjFactor() { return _HIVincAdjFactor; };
    inline float getHCVIncAdjFactor() { return _HCVincAdjFactor; };
    inline float getYFIncAdjFactor() { return _YFincAdjFactor; };
    inline float getMenIncAdjFactor() { return _MenincAdjFactor; };
    inline float getMeaIncAdjFactor() { return _MeaincAdjFactor; };
    inline float getDis1IncAdjFactor() { return _Dis1incAdjFactor; };
    inline float getDis2IncAdjFactor() { return _Dis2incAdjFactor; };

    // set incidence adjustment factor
    inline void setTBIncAdjFactor(float TBincAdjFactor) { _TBincAdjFactor = TBincAdjFactor; };
    inline void setHIVIncAdjFactor(float HIVincAdjFactor) { _HIVincAdjFactor = HIVincAdjFactor; };
    inline void setHCVIncAdjFactor(float HCVincAdjFactor) { _HCVincAdjFactor = HCVincAdjFactor; };
    inline void setYFIncAdjFactor(float YFincAdjFactor) { _YFincAdjFactor = YFincAdjFactor; };
    inline void setMenIncAdjFactor(float MenincAdjFactor) { _MenincAdjFactor = MenincAdjFactor; };
    inline void setMeaIncAdjFactor(float MeaincAdjFactor) { _MeaincAdjFactor = MeaincAdjFactor; };
    inline void setDis1IncAdjFactor(float Dis1incAdjFactor) { _Dis1incAdjFactor = Dis1incAdjFactor; };
    inline void setDis2IncAdjFactor(float Dis2incAdjFactor) { _Dis2incAdjFactor = Dis2incAdjFactor; };

    // get proportion of infected patients presenting for care
    inline float getTBPrpIncidencePresenting() { return _TBprpIncidencePresenting; };
    inline float getHIVPrpIncidencePresenting() { return _HIVprpIncidencePresenting; };
    inline float getHCVPrpIncidencePresenting() { return _HCVprpIncidencePresenting; };
    inline float getYFPrpIncidencePresenting() { return _YFprpIncidencePresenting; };
    inline float getMenPrpIncidencePresenting() { return _MenprpIncidencePresenting; };
    inline float getMeaPrpIncidencePresenting() { return _MeaprpIncidencePresenting; };
    inline float getDis1PrpIncidencePresenting() { return _Dis1prpIncidencePresenting; };
    inline float getDis2PrpIncidencePresenting() { return _Dis2prpIncidencePresenting; };

    // set proportion of infected patients presenting for care
    inline void setTBPrpIncidencePresenting(float TBprpIncidencePresenting) { _TBprpIncidencePresenting = TBprpIncidencePresenting; };
    inline void setHIVPrpIncidencePresenting(float HIVprpIncidencePresenting) { _HIVprpIncidencePresenting = HIVprpIncidencePresenting; };
    inline void setHCVPrpIncidencePresenting(float HCVprpIncidencePresenting) { _HCVprpIncidencePresenting = HCVprpIncidencePresenting; };
    inline void setYFPrpIncidencePresenting(float YFprpIncidencePresenting) { _YFprpIncidencePresenting = YFprpIncidencePresenting; };
    inline void setMenPrpIncidencePresenting(float MenprpIncidencePresenting) { _MenprpIncidencePresenting = MenprpIncidencePresenting; };
    inline void setMeaPrpIncidencePresenting(float MeaprpIncidencePresenting) { _MeaprpIncidencePresenting = MeaprpIncidencePresenting; };
    inline void setDis1PrpIncidencePresenting(float Dis1prpIncidencePresenting) { _Dis1prpIncidencePresenting = Dis1prpIncidencePresenting; };
    inline void setDis2PrpIncidencePresenting(float Dis2prpIncidencePresenting) { _Dis2prpIncidencePresenting = Dis2prpIncidencePresenting; };

    // get region name
    inline string getRegionName() { return _regionName; };
        
    // set region name
    inline void setRegionName(string regionName) { _regionName = regionName; };

    // get region id
    inline int getRegionID() { return _regionID; };
            
    // set region id
    inline void setRegionID(int regionID) { _regionID = regionID; };
     
};

#endif /* defined(DISTRICT_H) */
