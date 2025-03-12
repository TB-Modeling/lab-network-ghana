//
//  Diagnosis.h
//  mdrtb
//
//  Created by L. Dombrowski on 1/10/2020.
//

#ifndef DIAGNOSIS_H
#define DIAGNOSIS_H


#include <string>
#include <vector>

#include "GlobalParameters.h" //For diagnosisAttributes
#include "Patient.h"
#include "Testing.h"




class Patient;

using namespace  std;


class Diagnosis {
private:

    Patient* _patient;

    // Diagnosis available
    // 0 - no diagnosis - ex. Zonal Labs
    // 1 - diagnosis available
    bool _diagnosisAvailable;

    // current facility
    string _facility;

    // keep a container around to store patients that have samples associated with them
    std::vector<Patient*> _patientContainer;

    // keep a container around to store patients that are no longer active - still used in total patient statistics 
    std::vector<Patient*> _inactivePatientContainer;

    // keep a container around to store patients that have been created by transmission - they will get moved to the _patentContainer
    std::vector<Patient*> _transPatientContainer;
    
    // list of facility specific attributes for tests
    vector<diagnosisAttributes*> _diagnosisAttributes;

public:
    Diagnosis();
    Diagnosis(bool diagnosis, string facility);
    Diagnosis(bool diagnosis, string facility, string closestTestingFacility, float testingDistance);
    ~Diagnosis();

    // get diagnosis facility name
    inline string getFacility() { return _facility; };

    //set diagnosis available flag
    inline void setDiagnosis(bool flag) { _diagnosisAvailable = flag; };

    // decrement Diagnosis waiting time
    inline bool canDoDiagnosis() { return _diagnosisAvailable; };

    // set the testing Facility associated with this Diagnosis agent
    string getTestingFacility(string disease, string test);

    // set the testing Facility associated with this Diagnosis agent with output of diagnosis attributes
    string getTestingFacility(string disease, string test, bool output);

    // Get testing facility for disease, if testing here, return it first otherwise return first away testing site found
    string getTestingFacility(string disease);

    // create patients for this facility for every disease in list
    void createPatients(string facilityName, vector<string> diseaseList, int population, float binInc, float* popBins, vector<Disease*> diseases, int timestep);

    // return the pointer to the Patient list
    inline vector<Patient*>  getPatientPtr() { return  _patientContainer; };

    // return the pointer to the inactive Patient list
    inline vector<Patient*>  getInactivePatientPtr() { return  _inactivePatientContainer; };

    // return the pointer to the transmission Patient list
    inline vector<Patient*>  getTransPatientPtr() { return  _transPatientContainer; };

    // update list of patients
    inline void storePatient(Patient* patient) { _patientContainer.push_back(patient); };

    // update list of patients
    inline void storeInactivePatient(Patient* patient) { _inactivePatientContainer.push_back(patient); };

    // update list of transmission patients
    inline void storeTransPatient(Patient* patient) { _transPatientContainer.push_back(patient); };

    // update Patients if Samples have returned
    void  updatePatients( Testing* test, vector<Disease*> diseases, vector<Test*> tests, int timeStep);

    // update Patient to inactive patient container
    void  movePatientToInactive(Patient* patient, int reason, string loc, Stats* stats, Stats* facilityStats);
    
    // update Patient to inactive patient container
    void  moveTransPatientToActive();
    
    // get diagnosis disease/test attribute pointer
    inline vector<diagnosisAttributes*> getDiagnosisAttributesPtr() { return _diagnosisAttributes; };

    // add diseaseAttribute object diagnosis disease/test attribute container
    inline void addDiagnosisAttributeToPtr(diagnosisAttributes* d) { _diagnosisAttributes.push_back(d); };

    // are there any tests available for this disease available at this facility
    bool anyTestingHere(string disease);

    // find the treatment time from treatment intialization curve
    int getTestTreatmemtStart(string disease, string test);

    // find out if patient will die of old age after moving to inactive status
    void willPatientDie(Patient* patient, int timeStepsLeftInRun);

    // patient cured
    void patientCured(Patient* patient, int timeStepsLeftInRun);

    // patient relapses
    void patientRelapse(Patient* patient, int timeStepsLeftInRun);

    // patient loss to follow up
    void patientLtoF(Patient* patient, int timeStepsLeftInRun);

    // patient loss to follow up
    void patientRepresents(Patient* patient, int timeStepsLeftInRun);


};

#endif // DIAGNOSIS_H
