//
//  CreateAgentStats.cpp
// 
//
//  Created by L. Dombrowski on 4/29/20

//
// split out of CreateStats on 5/14/20

#include <sys/stat.h>
#include <cstring>
#include <errno.h> 

#include "CreateAgentStats.h"
#include "GlobalParameters.h"
#include "FacilitiesList.h"
#include "Stats.h"

using namespace std;


// count how many Patients currently in existence 
void agentStats(FacilitiesList* list, vector<string> diseaseList, vector<Disease*> dis, bool fileFlag, bool individualFlag, bool individualSamplingFlag, bool yearly, int timeStep) {

    Diagnosis* d;
    Testing* t;
    vector<Patient*> p;
    vector<Patient*> pI;
    vector<Sample*> sRemote;
    vector<Sample*> sIRemote;
    vector<Sample*> s;
    vector<Sample*> sI;
    Sample* samplePtr;
    ofstream myfile;    // AgentStats.csv
    ofstream myfile1;   // AgentStatsPatientsFacility.csv
    ofstream myfile2;   // AgentStatsPatientsRegion.csv
    ofstream myfile3;   // AgentStatsPatientsDistrict.csv
    ofstream myfile4;   // AgentStatsPatientsAway.csv
    ofstream myfile5;   // AgentStatsPatientsIn.csv
    ofstream myfile6;   // AgentStats-Facility.csv
    ofstream myfile7;   // AgentStats-District.csv
    ofstream myfile8;   // AgentStats-Region.csv
    ofstream myfile9;   // AgentStatsPatients.csv
    ofstream myfile10;  // AgentStatsPatientsRaw.csv
    ofstream myfile11;  // AgentStatsPatientsRwaAdmin.csv
    ofstream myfile12;  // AgentStatsPatientsSamplingRaw.csv
    
    int count = 0;
    int startIndex = 0;
    int endIndex = 0;
    //int treated = 0;
    //int infected;
    int facilityCountP = 0;
    int countF = 0;
    int facilityPop = 0;
    int status;
    int lagTime;
    int withTB;
    int noOfYears = 0;
    int lowerBound = 0;
    int upperBound = 0;
    int patientCreated = 0;
    int noTotalPatients = 0;
    int noPatientInactive;
    int newPop;
    int errNo;
    int patReturnedCount;
    int tempTime = 0;
    int sampleTempTime = 0;
    int collectTime = 0;
    int presentToTreatTime = 0;
    int sampleToDeathTime = 0;
    float yll;
    float totalSampleCountPos;
    float totalSampleCountNeg;
    bool testedInHouse = true;
    bool debug = false;
    bool debug1 = false;
    bool debug2 = false;
    bool debug4 = false;
    bool debugOutput = false;

    int prevalence;
    int diseaseCount;
    int diseaseIndex;
    int noRegions = 20;
    int noDistricts = 270;
    int patientSamplingRate = 1000;
    string facilityDisease = "TB";
    string statusString;
    string diseaseName;
    string subDirBase;
    string subDir;
    string yearDir;
    string reason;
    string testName;
    string sampleStatusString;


    // find out how many diseases to save stats for
    // use this as an count to create arrays for each of the diseases being run
    diseaseCount = diseaseList.size();

    int indNoPatients[diseaseCount];
    int indAge[diseaseCount];
    int indNoAlive[diseaseCount];
    int indNoInfected[diseaseCount];
    int indNoTestPos[diseaseCount];
    int indNoMDOverride[diseaseCount];
    int indTotalTimeFromOnset[diseaseCount];
    int indTotalTimeInfectious[diseaseCount];
    int indTotaltimeInTreatment[diseaseCount];
    int indTotalTimeNotInTreatment[diseaseCount];
    int indTotalTimeFacilityReturnTime[diseaseCount];
    int indTotalTimeResultsReturned[diseaseCount];
    int indTotalNoOfReturnVisits[diseaseCount];
    string indDisease[diseaseCount];

    int distNoPatients[diseaseCount];
    int distAge[diseaseCount];
    int distNoAlive[diseaseCount];
    int distNoInfected[diseaseCount];
    int distNoTestPos[diseaseCount];
    int distNoMDOverride[diseaseCount];
    int distTotalTimeFromOnset[diseaseCount];
    int distTotalTimeInfectious[diseaseCount];
    int distTotaltimeInTreatment[diseaseCount];
    int distTotalTimeNotInTreatment[diseaseCount];
    int distTotalTimeFacilityReturnTime[diseaseCount];
    int distTotalTimeResultsReturned[diseaseCount];
    int distTotalNoOfReturnVisits[diseaseCount];
    string distDisease[diseaseCount];

    int regNoPatients[diseaseCount];
    int regAge[diseaseCount];
    int regNoAlive[diseaseCount];
    int regNoInfected[diseaseCount];
    int regNoTestPos[diseaseCount];
    int regNoMDOverride[diseaseCount];
    int regTotalTimeFromOnset[diseaseCount];
    int regTotalTimeInfectious[diseaseCount];
    int regTotaltimeInTreatment[diseaseCount];
    int regTotalTimeNotInTreatment[diseaseCount];
    int regTotalTimeFacilityReturnTime[diseaseCount];
    int regTotalTimeResultsReturned[diseaseCount];
    int regTotalNoOfReturnVisits[diseaseCount]; 
    string regDisease[diseaseCount];
		
	//S represents the current state of the simulation at the end of the year when we report the output
		
    //name                  name                        facility, district or region name
    //id	                id	                        ID – facility ID from master facility list
    //tier                  tier                        tier of facility
    //district	            district	                district of facility or district
    //region                region	                    region of facility, district or region
    //population	        population	                Population – population in catchment zone or population from facilities catchment zones in region or district
    //disease	            disease	                    Disease – disease modeled values
    //prevalence	        calculated.prevalence	    number of people with active TB at the end of the year
				
    //nPatientsEnter.new		                        number of patients entering the model who are new (never treated)
    //nPatientsenter.ltfu		                        number of patients entering the model who are were previously treated and were LTFU
    //nSamplesCollected	    tests.sent.away+tests.inhouse	total number of samples collected this year
    //nSamplesOut	        tests.sent.away	total       number of samples sent to other facilities for testing
    //nSamplesInhouse	    tests.inhouse	            total number of samples processed in house
		
    //aveSampleResourceWaitTime		                    average wait time for resources among all samples
    //aveSampleResourceWaitTimeInhouse		            average wait time for resources among samples tested in house
    //aveSampleResourceWaitTimeOut		                average wait time for resources among samples sent out
    //aveSampleTransportTime		                    average transport time for samples that are sent out
		
    //nSamplesTested		                            total number of samples successfully tested
    //nSamplesTestedInhouse	test.inhouse	            number of tests in house
    //nSamplesTestedOut	    test.sent.away	            number of tests perfomed at other facilities
    //nSamplesLostOut	                	            number of samples that were lost out of facility

    //aveDelayFromSampleToTestInhouse		            average delay from sample collection to testing in house
    //aveDelayFromSampleToTestOut		                average delay from sample collection to testing out
		
		
    //nTestsPos	            pos.test	                number of positive test results
    //pTestsTruePos	        pos.true/(pos.test+neg.test)	ratio of true pos test results
    //pTestsFalsePos	    pos.false/(pos.test+neg.test)	ratio of false pos test results
    //nTestsNeg	            neg.test	                number of negative test results
    //pTestsTrueNeg	        neg.true(pos.test+neg.test)	ratio of true neg test results
    //pTestsFalseNeg	    neg.false(pos.test+neg.test)	ratio of false neg test results
		
    //nMdOverrideInfected	md.over.false.neg.for.inf	MD overrides a false negative results for an infected patient
    //nMdOvertideUninfected	md.over.true.neg.for.uninf	MD overrides a true negative results for an uninfected patient
		
    //nTestMdPos            pos.test+md.over.false.neg.for.inf	number of total positive results from tests and MD calls
    //nTestMdFalsPos	    neg.false	                number of infected patients receiving a false negative result
    //nTestMdFalseNeg	    pos.false	                number of uninfected patients to receive a false positive result
			
    //nDiagnosis	        same as line 39	total       number of patients receiving a positive result (in house or out)
    //nDiagnosisInhouse		                            number of diagnosis made in house (patients received a positive result)
    //nDiagnosisOut		                                number of diagnosis made from samples sent out (patients received a positive result)
    //nPreDiagLTFUOut		                            number of patient that didn’t receive their positive diagnosis result and were LTFU
    //nPreDiagLTFU.dead	    dead.prediagnostic	        number of pre-diag LTFU patients who died
    //nPreDiagLTFU.return		                        number of pre-diag LTFU patients who returned to care
		
    //aveDelaySampleToDiagInhouse		                average delay from sample collection to receiving a positive diagnosis in house
    //aveDelaySampleToDiagOut		                    average delay from sample collection to receiving a positive diagnosis out
		
    //nStartTrt		                                    number of patients starting treatment
    //nStartTrtInfected	    infec.treat	                number of infected patients starting treatment
    //nStartTrtUninfected	uninf.treat	                number of uninfected patients starting treatment
    //nTrt.cured	        cured	                    number of patients successfully completing original treatment who got cured
    //nTrt.died	            dead.infected.treat	        number of patients dying during treatment
    //nTrt.relapse		                                number of patients who relapse after original treatment
    //nStartRetreated	    infec.treat+uninf.treat	    number of patients starting retreatment
    //nRetrt.cured		                                number of patients cured after retreatment
    //nRetrt.died		                                number of patients died during retreatment

    //aveDelaySampleToTrt	avg.time.to.treat	        average delay from sample collection to starting treatment among all patients
    //aveDelaySampleToTrtInhouse		                average delay from sample collection to starting treatment in house
    //aveDelaySampleToTrtOut		                    average delay from sample collection to starting treatment out
				
    //aveDelaySampleToDeath		                        average delay from sample collection to death among all patients who died
    //aveDelaySampleToDeath.preDiagLtfu		            average delay from sample collection to death among patients who died after prediag LTFU
    //aveDelaySampleToDeath.inTrt		                average delay from sample collection to death among patients who died during treatment
    //aveDelaySampleToDeath.inReTrt		                average delay from sample collection to death among patients who died during retreatment
	//aveDelaySampleTExit		                        average delay from sample collection to patient exit
    
    //nSmear	            test2.count	                number of smear tests ran
    //nXpert	            test1.count	                number of Xpert tests ran
    //nXpertInhouse		                                number of Xpert tests ran inhouse
    //nXpertOut		                                    number of Xpert tests ran out
    //nSmear.pos		                                number of smear tests resulted positive
    //pSmear.truePos		                            prop of pos smear results that are true pos
    //nSmear.neg		                                number of smear tests resulted negative
    //pSmear.trueNeg		                            prop of neg smear results that are true neg
    //nXpert.pos		                                number of Xpert tests resulted positive
    //pXpert.truePos		                            prop of pos Xpert results that are true pos
    //nXpert.neg		                                number of Xpert tests resulted negative
    //pXpert.trueNeg		                            prop of neg Xpert results that are true neg
				
    //s.patientsSimulated	patients.count	            number of patients in the simulation
    //s.patientWaitingIn	s.untested.in	            number of patients waiting for sample results inhouse
    //s.patientsWaitingOut	s.untested.out	            number of patients waiting for sample results from other facilities
    //s.inTreatment	        countPT	                    number of patients in treatment
    //s.samp.wait.for.test  sampleWaitTestCount         number of samples waiting to be tested
    //s.samp.returned.pat.not.notified patientNotNotified          sample returned, patient still not notified
    //s.samp.returned.pat.not.notified.pos patientNotNotifiedPos   sample returned, patient still not notified, positive resullt
    
	string statHeader1 = "name,id,tier,district,region,population,disease,prevalence";
    string statHeader2 = ",n.patients.enter.new,n.patients.enter.new.inf,n.patients.enter.ltfu.inf,n.samples.collected,n.samples.out,n.samples.inhouse";
    string statHeader3 = ",ave.sample.resource.wait.time,ave.sample.resource.wait.time.in.house,ave.sample.resource.wait.time.out,ave.sample.transport.time,n.samples.transported";
    string statHeader4 = ",n.samples.tested,n.samples.tested.in.house,n.samples.tested.out";         //,n.samples.lost.out
    string statHeader5 = ",ave.delay.from.sample.to.test.in.house.inf,total.delay.from.sample.to.test.in.house.inf,n.delay.from.sample.to.test.in.house.inf";
    string statHeader5a = ",ave.delay.from.sample.to.test.out.inf,total.delay.from.sample.to.test.out.inf,n.delay.from.sample.to.test.out.inf";
    string statHeader6 = ",n.tests.pos,p.tests.true.pos,p.tests.false.pos,n.tests.neg,p.tests.true.neg,p.tests.false.neg";
    string statHeader7 = ",n.md.override.infected,n.md.override.uninfected";
    string statHeader8 = ",n.result.pos,n.result.false.pos,n.result.false.neg";
    string statHeader9 = ",n.diagnosis,n.diagnosis.in.house,n.diagnosis.out,n.pre.test.ltfu.out,n.pre.test.ltfu.out.minus.tested.samples,n.pre.test.ltfu.dead.inf,n.pre.test.ltfu.return.inf";
    string statHeader10 = ",ave.delay.sample.to.diag.in.house.inf,total.delay.sample.to.diag.in.house.inf,n.delay.sample.to.diag.in.house.inf";
    string statHeader10a = ",ave.delay.sample.to.diag.out.inf,total.delay.sample.to.diag.out.inf,n.delay.sample.to.diag.out.inf";
    string statHeader11 = ",n.start.trt,n.start.trt.infected,n.start.trt.uninfected,n.trt.cured.inf,n.trt.died.inf,n.trt.relapse.inf,n.start.retreated.inf,n.retrt.cured.inf,n.retrt.died.inf";
    string statHeader12 = ",ave.delay.sample.to.trt.inf,total.delay.sample.to.trt.inf,n.delay.sample.to.trt.inf";
    string statHeader12a = ",ave.delay.sample.to.trt.in.house.inf,total.delay.sample.to.trt.in.house.inf,n.delay.sample.to.trt.in.house.inf";
    string statHeader12b = ",ave.delay.sample.to.trt.out.inf,total.delay.sample.to.trt.out.inf,n.delay.sample.to.trt.out.inf";
    string statHeader13 = ",ave.delay.sample.to.death.inf,total.delay.sample.to.death.inf,n.delay.sample.to.death.inf";
    string statHeader13a = ",ave.delay.sample.to.death.pre.test.ltfu.inf,total.delay.sample.to.death.pre.test.ltfu.inf,n.delay.sample.to.death.pre.test.ltfu.inf";
    string statHeader13b = ",ave.delay.sample.to.death.in.trt.inf,total.delay.sample.to.death.in.trt.inf,n.delay.sample.to.death.in.trt.inf";
    string statHeader13c = ",ave.delay.sample.to.death.in.retrt.inf,total.delay.sample.to.death.in.retrt.inf,n.delay.sample.to.death.in.retrt.inf";
    string statHeader13d = ",ave.delay.sample.to.exit.inf,total.delay.sample.to.exit.inf,n.delay.sample.to.exit.inf";
    string statHeader14 = ",n.xpert,n.xpert.in.house,n.xpert.out,n.smear,n.smear.pos,p.smear.true.pos";
    string statHeader15 = ",n.smear.neg,p.smear.true.neg,n.xpert.pos,p.xpert.true.pos,n.xpert.neg,p.xpert.true.neg";
    string statHeader16 = ",n.patients.simulated,s.patient.waiting.in,s.patient.waiting.out,s.in.treatment,s.samp.wait.for.test,s.samp.wait.for.test.in,s.samp.wait.for.test.out";
    string statHeader17 = ",s.pre.test.ltfu.sample.tested,s.samp.returned.pat.not.notified,s.samp.returned.pat.not.notified.pos.result\n";
    
    //std::cout << statHeader1 << statHeader2 << statHeader3 << statHeader4 << statHeader5 << statHeader5a << statHeader6 << statHeader7 << statHeader8;
    //std::cout << statHeader9 << statHeader10 << statHeader10a << statHeader11 << statHeader12 << statHeader12a << statHeader12b;
    //std::cout << statHeader13 << statHeader13a << statHeader13b << statHeader13c << statHeader13d << statHeader14 << statHeader15 << statHeader16 << statHeader17;

    //std::cout << name << "," << id << "," << tier << "," << district << "," << region << "," << population << "," << disease << "," << prevalence << "," << countP;
    //std::cout << "," << countPInf << "," << countPLtoF << "," << outCollectedCount + inCollectedCount << "," << outCollectedCount << "," << inCollectedCount;
    //std::cout << "," << (sampleWaitForResourceInTime+sampleWaitForResourceOutTime)/(sampleWaitForResourceInCount+sampleWaitForResourceOutCount) << "," << sampleWaitForResourceInTime/sampleWaitForResourceInCount << "," << sampleWaitForResourceOutTime/sampleWaitForResourceOutCount << "," << sampleTransTime/sampleTransCount << "," << sampleTransCount;
    //std::cout << "," << sampleTestedInCount + sampleTestedOutCount << "," << sampleTestedInCount << "," << sampleTestedOutCount;
    //std::cout << "," << sampleLtoFCount;
    //std::cout << "," << sampleCollectToTestIn << "," << sampleCollectToTestOut;
    //std::cout << "," << countTestPos << "," << countTruePos/countTestPos << "," << countFalsePos/countTestPos << "," << countTestNeg << "," << countTrueNeg/countTestNeg << "," << countFalseNeg/countTestNeg;
    //std::cout << "," << countMDInf << "," << countMDUninf;
    //std::cout << "," << countMDInf+countTestPos << "," << countFalsePos << "," << countFalseNeg;
    //std::cout << "," << countDiagnosis << "," << countDiagnosisIn << "," << countDiagnosisOut << "," << countPreTestLtoFOut << "," << countPreTestLtoFOutMinusTestedSamp << "," << countPreTestLtoFDead << "," << countPreTestLtoFORepresent;
    //std::cout << "," << sampleTestPosTimeIn/sampleTestPosCountIn << "," << sampleTestPosTimeOut/sampleTestPosCountOut;
    //std::cout << "," << countPInfTreated+countPUninfTreated << "," << countPInfTreated << "," << countPUninfTreated << "," << countPTC << "," << countPTD << "," << countPTR  << "," << countRetreat << "," << countRetreatCure << "," << countRetreatDead;
    //std::cout << "," << onsetToTreat << "," << onsetToTreatIn << "," << onsetToTreatOut;
    //std::cout << "," << "aveSampletoDeath" << "," << "aveSampletoDeathPreDiagLtoF" << "," << "aveSampletoDeathInTreat"  << "," << "aveSampletoDeathInReTreat" << "," << "aveSampletoExit";
    //std::cout << "," << test2Count << "," << test1Count << "," << "nXpertIn"  << "," << "nXpertOut" << "," << "nSmearPos" << "," << "pSmearTruePos" << "," << "nSmearNeg"  << "," << "pSmearTrueNeg" << "," << "nXpertPos" << "," << "pXpertTruePos" << "," << "nXpertNeg"  << "," << "pXpertTrueNeg";
    //std::cout << "," << countP << "," << countPI << "," << countPU  << "," << countPT << "," << sampleWaitTestCount  << "," << sampleWaitTestCountIn  << "," << sampleWaitTestCountOut  <<;
    //std::cout << "," << countPreTestLtoFOutTested" << "," << patientNotNotified" << "," << patientNotNotifiedPos";
    //std::cout << "\n";

    // keep patient statistics by region and district
    struct geographicalStats {
        string name;
        string district;
        string region;
        string id;
        int startIndex = 0;
        int endIndex = 0;
        int tier = 0;           // facility tier
        int countP = 0;         // patients count
        int countPLtoF = 0;     // patients that were loss to follow up at any point
        int countPInf = 0;      // patients infected
        int countPInfTreated = 0;  // patients infected treated
        int countPInfTreatedIn = 0;  // patients infected treated in house
        int countPInfTreatedOut = 0;  // patients infected treated for samples sent out
        int countPUninfTreated = 0;  // patients uninfected treated
        int countPU = 0;        // patients untested out
        int countPI = 0;        // patients untesed in
        int countPT = 0;        // patients under treatment
        int countPR = 0;        // patients relapsed
        int countPUT = 0;       // patients untreated
        int countPUTFN = 0;     // patients untreated - Patient infected, sample results returned, no treatment - false neg - PatInfTestNoTreat
        int countPUTNS = 0;     // patients untreated - patient infected, no sample results returned, no treatement - sample didn't return - PatInfNoTestNoTreat
        int countPUTLtoF = 0;   // patients untreated - patient infected, sample returned, patient didn't (loss to followup), no treatment - PatInfTestLtofNoTreat
        int countPC = 0;        // patients cured
        int countPDI = 0;       // patients dead - infected
        int countPDIT = 0;      // patients dead - infected - treated
        int countPDIUT = 0;     // patients dead - infected - untreated
        int countPDIUTSR = 0;   // patients dead - infected - untreated - Patient infected, sample results returned, no treatment - false neg - PatInfTestNoTreat
        int countPDIUTSNR = 0;  // patients dead - infected - untreated - patient infected, no sample results returned, no treatement - sample didn't return - PatInfNoTestNoTreat
        int countPDIUTLtF = 0;  // patients dead - infected - untreated - patient infected, sample returned, patient didn't (loss to followup), no treatment - PatInfTestLtofNoTreat
        int countPDU = 0;       // patients dead - uninfected
        int countPDOA = 0;      // patients dead - old age
        int countPDIUTPTLtoF = 0; // patients dead - infected - untreated - prediagnostic loss to follow up
        int countPDIUTSRLtoF = 0; // patients dead - infected - sample results returned, no treatement - patient didn't start treatment
        int countPDITLtoF = 0;  // patients dead - infected - treated - patient loss to follow up after starting teatment
        int countPTC = 0;       // patient treated cured
        int countPTD = 0;       // patient treated died
        int countPTR = 0;       // patient treated relapse
        int countRetreat = 0;  // patients retreated
        int countRetreatCure = 0;  // patients retreated cured
        int countRetreatDead = 0;  // patients retreated died
        int countPO = 0;        // patients other
        int countPUI = 0;       // patients non-infected
        int countPUINR = 0;     // patients non-infected not returned
        int countPUIUT = 0;     // patients uninfected but treated
        int countMDInf = 0;     // doctor override infected
        int countMDUninf = 0;   // doctor override uninfected
        int countTestPos = 0;   // positive tests results
        int countTestNeg = 0;   // negastive tests results
        int countTruePos = 0;   // true positive count
        int countFalsePos = 0;  // false positive count
        int countTrueNeg = 0;   // true negative count
        int countFalseNeg = 0;  // false negative count
        int countDiagnosis = 0; // no. of patients receiving a pos result
        int countDiagnosisIn = 0;  // no. of patients receiving a pos result in house
        int countDiagnosisOut = 0;  // no. of patients receiving a pos result from samples sent out
        int countPreTestLtoFOut = 0;  // no. of patients that didn't receive a diagnosis and were LtoF
        int countPreTestLtoFOutTested = 0;  // no. of patients that didn't receive a diagnosis and were LtoF but their sample was tested
        int countPreTestLtoFOutUnTested = 0;  // no. of patients that didn't receive a diagnosis and were LtoF but their sample was untested
        int countPreTestLtoFOutUnTested2 = 0;  // no. of patients that didn't receive a diagnosis and were LtoF but their sample was untested
        int countPreTestLtoFDead = 0;  // no. of patients that didn't receive a diagnosis and were LtoF and died
        int countPreTestLtoFORepresent = 0;  // no. of patients that didn't receive a diagnosis and were LtoF and re-presented
        int onsetToTreat = 0;   //time from presentation to treatment
        int onsetToTreatIn = 0; //time from presentation to treatment - in house
        int onsetToTreatOut = 0;//time from presentation to treatment - sample sent out
        int timeInfectious = 0; //time from presentation to cure or death
        int sampleTransTime = 0;// time from sample creation until sample arrives at testing facility
        int sampleTransCount = 0;// number of samples used in sampleTransTime
        int sampleWaitForResourceInTime = 0;// time sample waits for resource to becomve available after arriving at testing facility
        int sampleWaitForResourceInCount = 0;// number of samples used in sampleWaitForResourceInTime
        int sampleWaitForResourceOutTime = 0;// time sample waits for resource to become available after arriving at testing facility
        int sampleWaitForResourceOutCount = 0;// number of samples used in sampleWaitForResourceOutTime
        int sampleCollectToTestInTime = 0;// time from sample creation until sample test in house
        int sampleCollectToTestOutTime = 0;// time from sample creation until sample test sent out
        int sampleCollectToTestInCount = 0;// count of no. of samples creation until sample test in house
        int sampleCollectToTestOutCount = 0;// count of no. of  samples creation until sample test sent out
        int sampleTestTime = 0; // time from sample creation until sample is tested
        int sampleTestPosTimeIn = 0; // time from sample creation until positive sample is tested in house
        int sampleTestPosTimeOut = 0; // time from sample creation until postitive sample is tested at another facility
        int sampleTestPosCountIn = 0; // number of  samples creation until positive sample is tested in house
        int sampleTestPosCountOut = 0; // number of samples creation until postitive sample is tested at another facility
        int sampleReturnTime = 0; // time from sample creation to patient called for sample return
        int sampleReturnedCount = 0; // number of samples returned to patient
        int sampleReturnedInCount = 0; // number of samples tested in house returned to patient
        int sampleReturnedOutCount = 0; // number of samples sent out returned to patient
        int sampleTestedInCount = 0; // number of samples tested in house patient
        int sampleTestedOutCount = 0; // number of samples tested away from diagnosis facility patient
        int sampleNotTestedInCount = 0; // number of samples not tested in house patient
        int sampleNotTestedOutCount = 0; // number of samples not tested away from diagnosis facility patient
        int sampleReturnToDeathTime = 0; // time from sample creation to patient dies
        int sampleReturnToDeathPreDiagLtoFTime = 0; // time from sample creation to patient dies
        int sampleReturnToDeathInTrtTime = 0; // time from sample creation to patient dies
        int sampleReturnToDeathInRetrtTime = 0; // time from sample creation to patient dies
        int sampleReturnToExitTime = 0; // time from sample creation to patient exit
        int sampleReturnToDeathCount = 0; // number of samples returned where patient dies
        int sampleReturnToDeathPreDiagLtoFCount = 0; // number of samples returned where patient dies due to pre-diagnostic LtoF
        int sampleReturnToDeathInTrtCount = 0; // number of samples returned where patient dies in treatment
        int sampleReturnToDeathInRetrtCount = 0; // number of samples returned where patient dies in re-treatment
        int sampleReturnToExitCount = 0; // number of samples returned for patient exit
        int sampleLtoFCount = 0; // number of samples loss to followup
        int sampleWaitTestCount = 0; // number of samples waiting to be tested
        int sampleWaitTestCountIn = 0; // number of samples waiting to be tested in house
        int sampleWaitTestCountOut = 0; // number of samples waiting to be tested somewhere else
        int patientNotNotified = 0;  // sample returned, patient still not notified
        int patientNotNotifiedPos = 0;  // sample returned, patient still not notified positive result
        int patientNotNotifiedIn = 0;  // sample returned in house, patient still not notified
        int patientNotNotifiedOut = 0;  // sample returned from outside testing, patient still not notified
        int inTestCount = 0;    // count of tests done inhouse
        int outTestCount = 0;   // count of tests sent away
        int inCollectedCount = 0;    // count of collected tests done inhouse - whether test was lost or returned
        int outCollectedCount = 0;   // count of collected tests sent away - whether test was lost or returned
        string test1Name = "";  // name of first test used
        int test1Count = 0;     // count of first test used
        int test1CountIn = 0;   // count of first test used in House
        int test1CountOut = 0;  // count of first test used that were sent out
        int test1PosCount = 0;  // count of first test positive results
        int test1TruePosCount = 0;  // count of first test true positive results
        int test1NegCount = 0;  // count of first test negative results
        int test1TrueNegCount = 0;  // count of first test true negative results
        string test2Name = "";  // name of second test used
        int test2Count = 0;     // count of second test used
        int test2CountIn = 0;   // count of second test used in House
        int test2CountOut = 0;  // count of second test used that were sent out
        int test2PosCount = 0;  // count of second test positive results
        int test2TruePosCount = 0;  // count of second test true positive results
        int test2NegCount = 0;  // count of second test negative results
        int test2TrueNegCount = 0;  // count of second test true negative results
        string test3Name = "";  // name of third test used
        int test3Count = 0;     // count of third test used
        int tier1Count = 0;     // count of tests at tier 1
        int tier2Count = 0;     // count of tests at tier 2
        int tier3Count = 0;     // count of tests at tier 3
        int tier4Count = 0;     // count of tests at tier 4
        int tier5Count = 0;     // count of tests at tier 5
        int tier6Count = 0;     // count of tests at tier 6
        int countPreTestLtoFOutMinusTestedSamp = 0;    // pre-test loss to follow up minus tested samples
        int dummy2Count = 0;    // dummy count
        int dummy3Count = 0;    // dummy count
        int dummy4Count = 0;    // dummy count
        int dummy5Count = 0;    // dummy count
        int dummy6Count = 0;    // dummy count
        int pop = 0;

        // a vector of patient container vectors
        vector< vector<Patient*> > patientContainer;
    };

    geographicalStats total;
    geographicalStats totalFacility;
    geographicalStats totalRegion;
    geographicalStats totalDistrict;
    geographicalStats diseaseTotal[diseaseCount];
    geographicalStats diseaseTotalRegion[diseaseCount];
    geographicalStats diseaseTotalDistrict[diseaseCount];


    // keep sample statistics by region and district
    geographicalStats regions[diseaseCount][noRegions];
    geographicalStats districts[diseaseCount][noDistricts];

    int regionCount[diseaseCount];
    int districtCount[diseaseCount];
    int patientCount[diseaseCount];
    string region;
    string district;
    bool regionNotFound;
    bool districtNotFound;

    vector<Facility*> facilities = list->getListPtr();

    countF = facilities.size();

    for (int i = 0; i < diseaseCount; i++) {

        regionCount[i] = 0;
        districtCount[i] = 0;
    }

    // get statistical array for each facility
    geographicalStats facilityStats[diseaseCount][countF];

    if (debug) std::cout << "stats: inside create agent stats no. of facilities =  " << countF << ", disease count = " << diseaseCount << "\n";

    // initialize all region data to 0
    for (int w = 0; w < diseaseCount; w++) {

        if (debug1) std::cout << "stats: inside create agent stats disease count =  " << w << "\n";

        // initialize all region data to 0
        for (int i = 0; i < noRegions; i++) {

            regions[w][i].name = "";
            regions[w][i].district = "0";
            regions[w][i].region = "";
            regions[w][i].id = "";
            regions[w][i].tier = 30;
            regions[w][i].pop = 0;
            regions[w][i].countP = 0;
            regions[w][i].countPLtoF = 0;
            regions[w][i].countPInf = 0;
            regions[w][i].countPInfTreated = 0;
            regions[w][i].countPInfTreatedIn = 0;
            regions[w][i].countPInfTreatedOut = 0;
            regions[w][i].countPUninfTreated = 0;
            regions[w][i].countPU = 0;
            regions[w][i].countPI = 0;
            regions[w][i].countPT = 0;
            regions[w][i].countPR = 0;
            regions[w][i].countPUT = 0;
            regions[w][i].countPUTFN = 0;
            regions[w][i].countPUTNS = 0;
            regions[w][i].countPUTLtoF = 0;
            regions[w][i].countPC = 0;
            regions[w][i].countPDI = 0;
            regions[w][i].countPDIT = 0;
            regions[w][i].countPDIUT = 0;
            regions[w][i].countPDIUTSR = 0;
            regions[w][i].countPDIUTSNR = 0;
            regions[w][i].countPDIUTLtF = 0;
            regions[w][i].countPDU = 0;
            regions[w][i].countPDOA = 0;
            regions[w][i].countPDIUTPTLtoF = 0;
            regions[w][i].countPDIUTSRLtoF = 0;
            regions[w][i].countPDITLtoF = 0;
            regions[w][i].countPTC = 0;
            regions[w][i].countPTD = 0;
            regions[w][i].countPTR = 0;
            regions[w][i].countRetreat = 0;
            regions[w][i].countRetreatCure = 0;
            regions[w][i].countRetreatDead = 0;
            regions[w][i].countPO = 0;
            regions[w][i].countPUI = 0;
            regions[w][i].countPUINR = 0;
            regions[w][i].countPUIUT = 0;
            regions[w][i].countMDInf = 0;
            regions[w][i].countMDUninf = 0;
            regions[w][i].countTestPos = 0;
            regions[w][i].countTestNeg = 0;
            regions[w][i].countTruePos = 0;
            regions[w][i].countFalsePos = 0;
            regions[w][i].countTrueNeg = 0;
            regions[w][i].countFalseNeg = 0;
            regions[w][i].countDiagnosis = 0;
            regions[w][i].countDiagnosisIn = 0;
            regions[w][i].countDiagnosisOut = 0;
            regions[w][i].countPreTestLtoFOut = 0;
            regions[w][i].countPreTestLtoFOutTested = 0;
            regions[w][i].countPreTestLtoFOutUnTested = 0;
            regions[w][i].countPreTestLtoFOutUnTested2 = 0;
            regions[w][i].countPreTestLtoFDead = 0;
            regions[w][i].countPreTestLtoFORepresent = 0;
            regions[w][i].onsetToTreat = 0;
            regions[w][i].onsetToTreatIn = 0;
            regions[w][i].onsetToTreatOut = 0;
            regions[w][i].timeInfectious = 0;
            regions[w][i].sampleTransTime = 0;
            regions[w][i].sampleTransCount = 0;
            regions[w][i].sampleWaitForResourceInTime = 0;
            regions[w][i].sampleWaitForResourceInCount = 0;
            regions[w][i].sampleWaitForResourceOutTime = 0;
            regions[w][i].sampleWaitForResourceOutCount = 0;
            regions[w][i].sampleCollectToTestInTime  = 0;
            regions[w][i].sampleCollectToTestOutTime = 0;
            regions[w][i].sampleCollectToTestInCount  = 0;
            regions[w][i].sampleCollectToTestOutCount = 0;
            regions[w][i].sampleTestTime = 0;
            regions[w][i].sampleTestPosTimeIn = 0;
            regions[w][i].sampleTestPosTimeOut = 0;
            regions[w][i].sampleTestPosCountIn = 0;
            regions[w][i].sampleTestPosCountOut = 0;
            regions[w][i].sampleReturnTime = 0;
            regions[w][i].sampleReturnedCount = 0;
            regions[w][i].sampleReturnedInCount = 0;
            regions[w][i].sampleReturnedOutCount = 0;
            regions[w][i].sampleTestedInCount = 0;
            regions[w][i].sampleTestedOutCount = 0;
            regions[w][i].sampleNotTestedInCount = 0;
            regions[w][i].sampleNotTestedOutCount = 0;
            regions[w][i].sampleReturnToDeathTime = 0;
            regions[w][i].sampleReturnToDeathPreDiagLtoFTime = 0;
            regions[w][i].sampleReturnToDeathInTrtTime = 0;
            regions[w][i].sampleReturnToDeathInRetrtTime = 0;
            regions[w][i].sampleReturnToExitTime = 0;
            regions[w][i].sampleReturnToDeathCount = 0;
            regions[w][i].sampleReturnToDeathPreDiagLtoFCount = 0;
            regions[w][i].sampleReturnToDeathInTrtCount = 0;
            regions[w][i].sampleReturnToDeathInRetrtCount = 0;
            regions[w][i].sampleReturnToExitCount = 0;
            regions[w][i].sampleLtoFCount = 0;
            regions[w][i].sampleWaitTestCount = 0;
            regions[w][i].sampleWaitTestCountIn = 0;
            regions[w][i].sampleWaitTestCountOut = 0;
            regions[w][i].patientNotNotified = 0;
            regions[w][i].patientNotNotifiedPos = 0;
            regions[w][i].patientNotNotifiedIn = 0;
            regions[w][i].patientNotNotifiedOut = 0;
            regions[w][i].inTestCount = 0;
            regions[w][i].outTestCount = 0;
            regions[w][i].inCollectedCount = 0;
            regions[w][i].outCollectedCount = 0;
            regions[w][i].test1Name = "";
            regions[w][i].test1Count = 0;
            regions[w][i].test1CountIn = 0;
            regions[w][i].test1CountOut = 0;
            regions[w][i].test1PosCount = 0;
            regions[w][i].test1TruePosCount = 0;
            regions[w][i].test1NegCount = 0;
            regions[w][i].test1TrueNegCount = 0;
            regions[w][i].test2Name = "";
            regions[w][i].test2Count = 0;
            regions[w][i].test2CountIn = 0;
            regions[w][i].test2CountOut = 0;
            regions[w][i].test2PosCount = 0;
            regions[w][i].test2TruePosCount = 0;
            regions[w][i].test2NegCount = 0;
            regions[w][i].test2TrueNegCount = 0;
            regions[w][i].test3Name = "";
            regions[w][i].test3Count = 0;
            regions[w][i].tier1Count = 0;
            regions[w][i].tier2Count = 0;
            regions[w][i].tier3Count = 0;
            regions[w][i].tier4Count = 0;
            regions[w][i].tier5Count = 0;
            regions[w][i].tier6Count = 0;
            regions[w][i].countPreTestLtoFOutMinusTestedSamp = 0;
            regions[w][i].dummy2Count = 0;
            regions[w][i].dummy3Count = 0;
            regions[w][i].dummy4Count = 0;
            regions[w][i].dummy5Count = 0;
            regions[w][i].dummy6Count = 0;
        }
        if (debug1) std::cout << "stats: inside create agent stats after regions - disease count =  " << w << "\n";

        // initialize all district data to 0
        for (int i = 0; i < noDistricts; i++) {

            districts[w][i].name = "";
            districts[w][i].district = "";
            districts[w][i].region = "";
            districts[w][i].id = "";
            districts[w][i].tier = 40;
            districts[w][i].pop = 0;
            districts[w][i].countP = 0;
            districts[w][i].countPLtoF = 0;
            districts[w][i].countPInf = 0;
            districts[w][i].countPInfTreated = 0;
            districts[w][i].countPInfTreatedIn = 0;
            districts[w][i].countPInfTreatedOut = 0;
            districts[w][i].countPUninfTreated = 0;
            districts[w][i].countPU = 0;
            districts[w][i].countPI = 0;
            districts[w][i].countPT = 0;
            districts[w][i].countPR = 0;
            districts[w][i].countPUT = 0;
            districts[w][i].countPUTFN = 0;
            districts[w][i].countPUTNS = 0;
            districts[w][i].countPUTLtoF = 0;
            districts[w][i].countPC = 0;
            districts[w][i].countPDI = 0;
            districts[w][i].countPDIT = 0;
            districts[w][i].countPDIUT = 0;
            districts[w][i].countPDIUTSR = 0;
            districts[w][i].countPDIUTSNR = 0;
            districts[w][i].countPDIUTLtF = 0;
            districts[w][i].countPDU = 0;
            districts[w][i].countPDOA = 0;
            districts[w][i].countPDIUTPTLtoF = 0;
            districts[w][i].countPDIUTSRLtoF = 0;
            districts[w][i].countPDITLtoF = 0;
            districts[w][i].countPTC = 0;
            districts[w][i].countPTD = 0;
            districts[w][i].countPTR = 0;
            districts[w][i].countRetreat = 0;
            districts[w][i].countRetreatCure = 0;
            districts[w][i].countRetreatDead = 0;
            districts[w][i].countPO = 0;
            districts[w][i].countPUI = 0;
            districts[w][i].countPUINR = 0;
            districts[w][i].countPUIUT = 0;
            districts[w][i].countMDInf = 0;
            districts[w][i].countMDUninf = 0;
            districts[w][i].countTestPos = 0;
            districts[w][i].countTestNeg = 0;
            districts[w][i].countTruePos = 0;
            districts[w][i].countFalsePos = 0;
            districts[w][i].countTrueNeg = 0;
            districts[w][i].countFalseNeg = 0;
            districts[w][i].countDiagnosis = 0;
            districts[w][i].countDiagnosisIn = 0;
            districts[w][i].countDiagnosisOut = 0;
            districts[w][i].countPreTestLtoFOut = 0;
            districts[w][i].countPreTestLtoFOutTested = 0;
            districts[w][i].countPreTestLtoFOutUnTested = 0;
            districts[w][i].countPreTestLtoFOutUnTested2 = 0;
            districts[w][i].countPreTestLtoFDead = 0;
            districts[w][i].countPreTestLtoFORepresent = 0;
            districts[w][i].onsetToTreat = 0;
            districts[w][i].onsetToTreatIn = 0;
            districts[w][i].onsetToTreatOut = 0;
            districts[w][i].timeInfectious = 0;
            districts[w][i].sampleTransTime = 0;
            districts[w][i].sampleTransCount = 0;
            districts[w][i].sampleWaitForResourceInTime = 0;
            districts[w][i].sampleWaitForResourceInCount = 0;
            districts[w][i].sampleWaitForResourceOutTime = 0;
            districts[w][i].sampleWaitForResourceOutCount = 0;
            districts[w][i].sampleCollectToTestInTime  = 0;
            districts[w][i].sampleCollectToTestOutTime = 0;
            districts[w][i].sampleCollectToTestInCount  = 0;
            districts[w][i].sampleCollectToTestOutCount = 0;
            districts[w][i].sampleTestTime = 0;
            districts[w][i].sampleTestPosTimeIn = 0;
            districts[w][i].sampleTestPosTimeOut = 0;
            districts[w][i].sampleTestPosCountIn = 0;
            districts[w][i].sampleTestPosCountOut = 0;
            districts[w][i].sampleReturnTime = 0;
            districts[w][i].sampleReturnedCount = 0;
            districts[w][i].sampleReturnedInCount = 0;
            districts[w][i].sampleReturnedOutCount = 0;
            districts[w][i].sampleTestedInCount = 0;
            districts[w][i].sampleTestedOutCount = 0;
            districts[w][i].sampleNotTestedInCount = 0;
            districts[w][i].sampleNotTestedOutCount = 0;
            districts[w][i].sampleReturnToDeathTime = 0;
            districts[w][i].sampleReturnToDeathPreDiagLtoFTime = 0;
            districts[w][i].sampleReturnToDeathInTrtTime = 0;
            districts[w][i].sampleReturnToDeathInRetrtTime = 0;
            districts[w][i].sampleReturnToExitTime = 0;
            districts[w][i].sampleReturnToDeathCount = 0;
            districts[w][i].sampleReturnToDeathPreDiagLtoFCount = 0;
            districts[w][i].sampleReturnToDeathInTrtCount = 0;
            districts[w][i].sampleReturnToDeathInRetrtCount = 0;
            districts[w][i].sampleReturnToExitCount = 0;
            districts[w][i].sampleLtoFCount = 0;
            districts[w][i].sampleWaitTestCount = 0;
            districts[w][i].sampleWaitTestCountIn = 0;
            districts[w][i].sampleWaitTestCountOut = 0;
            districts[w][i].patientNotNotified = 0;
            districts[w][i].patientNotNotifiedPos = 0;
            districts[w][i].patientNotNotifiedIn = 0;
            districts[w][i].patientNotNotifiedOut = 0;
            districts[w][i].inTestCount = 0;
            districts[w][i].outTestCount = 0;
            districts[w][i].inCollectedCount = 0;
            districts[w][i].outCollectedCount = 0;
            districts[w][i].test1Name = "";
            districts[w][i].test1Count = 0;
            districts[w][i].test1CountIn = 0;
            districts[w][i].test1CountOut = 0;
            districts[w][i].test1PosCount = 0;
            districts[w][i].test1TruePosCount = 0;
            districts[w][i].test1NegCount = 0;
            districts[w][i].test1TrueNegCount = 0;
            districts[w][i].test2Name = "";
            districts[w][i].test2Count = 0;
            districts[w][i].test2CountIn = 0;
            districts[w][i].test2CountOut = 0;
            districts[w][i].test2PosCount = 0;
            districts[w][i].test2TruePosCount = 0;
            districts[w][i].test2NegCount = 0;
            districts[w][i].test2TrueNegCount = 0;
            districts[w][i].test3Name = "";
            districts[w][i].test3Count = 0;
            districts[w][i].tier1Count = 0;
            districts[w][i].tier2Count = 0;
            districts[w][i].tier3Count = 0;
            districts[w][i].tier4Count = 0;
            districts[w][i].tier5Count = 0;
            districts[w][i].tier6Count = 0;
            districts[w][i].countPreTestLtoFOutMinusTestedSamp = 0;
            districts[w][i].dummy2Count = 0;
            districts[w][i].dummy3Count = 0;
            districts[w][i].dummy4Count = 0;
            districts[w][i].dummy5Count = 0;
            districts[w][i].dummy6Count = 0;
        }
        if (debug1) std::cout << "stats: inside create agent stats after districts - disease count =  " << w << "\n";

        // initialize total regioal and district stats
        diseaseTotalRegion[w].tier = 10;
        diseaseTotalRegion[w].district = "0";
        diseaseTotalRegion[w].region = "0";
        diseaseTotalRegion[w].pop = 0;
        diseaseTotalRegion[w].countP = 0;
        diseaseTotalRegion[w].countPLtoF = 0;
        diseaseTotalRegion[w].countPInf = 0;
        diseaseTotalRegion[w].countPInfTreated = 0;
        diseaseTotalRegion[w].countPInfTreatedIn = 0;
        diseaseTotalRegion[w].countPInfTreatedOut = 0;
        diseaseTotalRegion[w].countPUninfTreated = 0;
        diseaseTotalRegion[w].countPU = 0;
        diseaseTotalRegion[w].countPI = 0;
        diseaseTotalRegion[w].countPT = 0;
        diseaseTotalRegion[w].countPR = 0;
        diseaseTotalRegion[w].countPUT = 0;
        diseaseTotalRegion[w].countPUTFN = 0;
        diseaseTotalRegion[w].countPUTNS = 0;
        diseaseTotalRegion[w].countPUTLtoF = 0;
        diseaseTotalRegion[w].countPC = 0;
        diseaseTotalRegion[w].countPDI = 0;
        diseaseTotalRegion[w].countPDIT = 0;
        diseaseTotalRegion[w].countPDIUT = 0;
        diseaseTotalRegion[w].countPDIUTSR = 0;
        diseaseTotalRegion[w].countPDIUTSNR = 0;
        diseaseTotalRegion[w].countPDIUTLtF = 0;
        diseaseTotalRegion[w].countPDU = 0;
        diseaseTotalRegion[w].countPDOA = 0;
        diseaseTotalRegion[w].countPDIUTPTLtoF = 0;
        diseaseTotalRegion[w].countPDIUTSRLtoF = 0;
        diseaseTotalRegion[w].countPDITLtoF = 0;
        diseaseTotalRegion[w].countPTC = 0;
        diseaseTotalRegion[w].countPTD = 0;
        diseaseTotalRegion[w].countPTR = 0;
        diseaseTotalRegion[w].countRetreat = 0;
        diseaseTotalRegion[w].countRetreatCure = 0;
        diseaseTotalRegion[w].countRetreatDead = 0;
        diseaseTotalRegion[w].countPO = 0;
        diseaseTotalRegion[w].countPUI = 0;
        diseaseTotalRegion[w].countPUINR = 0;
        diseaseTotalRegion[w].countPUIUT = 0;
        diseaseTotalRegion[w].countMDInf = 0;
        diseaseTotalRegion[w].countMDUninf = 0;
        diseaseTotalRegion[w].countTestPos = 0;
        diseaseTotalRegion[w].countTestNeg = 0;
        diseaseTotalRegion[w].countTruePos = 0;
        diseaseTotalRegion[w].countFalsePos = 0;
        diseaseTotalRegion[w].countTrueNeg = 0;
        diseaseTotalRegion[w].countFalseNeg = 0;
        diseaseTotalRegion[w].countDiagnosis = 0;
        diseaseTotalRegion[w].countDiagnosisIn = 0;
        diseaseTotalRegion[w].countDiagnosisOut = 0;
        diseaseTotalRegion[w].countPreTestLtoFOut = 0;
        diseaseTotalRegion[w].countPreTestLtoFOutTested = 0;
        diseaseTotalRegion[w].countPreTestLtoFOutUnTested = 0;
        diseaseTotalRegion[w].countPreTestLtoFOutUnTested2 = 0;
        diseaseTotalRegion[w].countPreTestLtoFDead = 0;
        diseaseTotalRegion[w].countPreTestLtoFORepresent = 0;
        diseaseTotalRegion[w].onsetToTreat = 0;
        diseaseTotalRegion[w].onsetToTreatIn = 0;
        diseaseTotalRegion[w].onsetToTreatOut = 0;
        diseaseTotalRegion[w].timeInfectious = 0;
        diseaseTotalRegion[w].sampleTransTime = 0;
        diseaseTotalRegion[w].sampleTransCount = 0;
        diseaseTotalRegion[w].sampleWaitForResourceInTime = 0;
        diseaseTotalRegion[w].sampleWaitForResourceInCount = 0;
        diseaseTotalRegion[w].sampleWaitForResourceOutTime = 0;
        diseaseTotalRegion[w].sampleWaitForResourceOutCount = 0;
        diseaseTotalRegion[w].sampleCollectToTestInTime  = 0;
        diseaseTotalRegion[w].sampleCollectToTestOutTime = 0;
        diseaseTotalRegion[w].sampleCollectToTestInCount  = 0;
        diseaseTotalRegion[w].sampleCollectToTestOutCount = 0;
        diseaseTotalRegion[w].sampleTestTime = 0;
        diseaseTotalRegion[w].sampleTestPosTimeIn = 0;
        diseaseTotalRegion[w].sampleTestPosTimeOut = 0;
        diseaseTotalRegion[w].sampleTestPosCountIn = 0;
        diseaseTotalRegion[w].sampleTestPosCountOut = 0;
        diseaseTotalRegion[w].sampleReturnTime = 0;
        diseaseTotalRegion[w].sampleReturnedCount = 0;
        diseaseTotalRegion[w].sampleReturnedInCount = 0;
        diseaseTotalRegion[w].sampleReturnedOutCount = 0;
        diseaseTotalRegion[w].sampleTestedInCount = 0;
        diseaseTotalRegion[w].sampleTestedOutCount = 0;
        diseaseTotalRegion[w].sampleNotTestedInCount = 0;
        diseaseTotalRegion[w].sampleNotTestedOutCount = 0;
        diseaseTotalRegion[w].sampleReturnToDeathTime = 0;
        diseaseTotalRegion[w].sampleReturnToDeathPreDiagLtoFTime = 0;
        diseaseTotalRegion[w].sampleReturnToDeathInTrtTime = 0;
        diseaseTotalRegion[w].sampleReturnToDeathInRetrtTime = 0;
        diseaseTotalRegion[w].sampleReturnToExitTime = 0;
        diseaseTotalRegion[w].sampleReturnToDeathCount = 0;
        diseaseTotalRegion[w].sampleReturnToDeathPreDiagLtoFCount = 0;
        diseaseTotalRegion[w].sampleReturnToDeathInTrtCount = 0;
        diseaseTotalRegion[w].sampleReturnToDeathInRetrtCount = 0;
        diseaseTotalRegion[w].sampleReturnToExitCount = 0;
        diseaseTotalRegion[w].sampleLtoFCount = 0;
        diseaseTotalRegion[w].sampleWaitTestCount = 0;
        diseaseTotalRegion[w].sampleWaitTestCountIn = 0;
        diseaseTotalRegion[w].sampleWaitTestCountOut = 0;
        diseaseTotalRegion[w].patientNotNotified = 0;
        diseaseTotalRegion[w].patientNotNotifiedPos = 0;
        diseaseTotalRegion[w].patientNotNotifiedIn = 0;
        diseaseTotalRegion[w].patientNotNotifiedOut = 0;
        diseaseTotalRegion[w].inTestCount = 0;
        diseaseTotalRegion[w].outTestCount = 0;
        diseaseTotalRegion[w].inCollectedCount = 0;
        diseaseTotalRegion[w].outCollectedCount = 0;
        diseaseTotalRegion[w].test1Name = "";
        diseaseTotalRegion[w].test1Count = 0;
        diseaseTotalRegion[w].test1CountIn = 0;
        diseaseTotalRegion[w].test1CountOut = 0;
        diseaseTotalRegion[w].test1PosCount = 0;
        diseaseTotalRegion[w].test1TruePosCount = 0;
        diseaseTotalRegion[w].test1NegCount = 0;
        diseaseTotalRegion[w].test1TrueNegCount = 0;
        diseaseTotalRegion[w].test2Name = "";
        diseaseTotalRegion[w].test2Count = 0;
        diseaseTotalRegion[w].test2CountIn = 0;
        diseaseTotalRegion[w].test2CountOut = 0;
        diseaseTotalRegion[w].test2PosCount = 0;
        diseaseTotalRegion[w].test2TruePosCount = 0;
        diseaseTotalRegion[w].test2NegCount = 0;
        diseaseTotalRegion[w].test2TrueNegCount = 0;
        diseaseTotalRegion[w].test3Name = "";
        diseaseTotalRegion[w].test3Count = 0;
        diseaseTotalRegion[w].tier1Count = 0;
        diseaseTotalRegion[w].tier2Count = 0;
        diseaseTotalRegion[w].tier3Count = 0;
        diseaseTotalRegion[w].tier4Count = 0;
        diseaseTotalRegion[w].tier5Count = 0;
        diseaseTotalRegion[w].tier6Count = 0;
        diseaseTotalRegion[w].countPreTestLtoFOutMinusTestedSamp = 0;
        diseaseTotalRegion[w].dummy2Count = 0;
        diseaseTotalRegion[w].dummy3Count = 0;
        diseaseTotalRegion[w].dummy4Count = 0;
        diseaseTotalRegion[w].dummy5Count = 0;
        diseaseTotalRegion[w].dummy6Count = 0;
        diseaseTotalDistrict[w].tier = 10;
        diseaseTotalDistrict[w].district = "0";
        diseaseTotalDistrict[w].region = "0";
        diseaseTotalDistrict[w].pop = 0;
        diseaseTotalDistrict[w].countP = 0;
        diseaseTotalDistrict[w].countPLtoF = 0;
        diseaseTotalDistrict[w].countPInf = 0;
        diseaseTotalDistrict[w].countPInfTreated = 0;
        diseaseTotalDistrict[w].countPInfTreatedIn = 0;
        diseaseTotalDistrict[w].countPInfTreatedOut = 0;
        diseaseTotalDistrict[w].countPUninfTreated = 0;
        diseaseTotalDistrict[w].countPU = 0;
        diseaseTotalDistrict[w].countPI = 0;
        diseaseTotalDistrict[w].countPT = 0;
        diseaseTotalDistrict[w].countPR = 0;
        diseaseTotalDistrict[w].countPUT = 0;
        diseaseTotalDistrict[w].countPUTFN = 0;
        diseaseTotalDistrict[w].countPUTNS = 0;
        diseaseTotalDistrict[w].countPUTLtoF = 0;
        diseaseTotalDistrict[w].countPC = 0;
        diseaseTotalDistrict[w].countPDI = 0;
        diseaseTotalDistrict[w].countPDIT = 0;
        diseaseTotalDistrict[w].countPDIUT = 0;
        diseaseTotalDistrict[w].countPDIUTSR = 0;
        diseaseTotalDistrict[w].countPDIUTSNR = 0;
        diseaseTotalDistrict[w].countPDIUTLtF = 0;
        diseaseTotalDistrict[w].countPDU = 0;
        diseaseTotalDistrict[w].countPDOA = 0;
        diseaseTotalDistrict[w].countPDIUTPTLtoF = 0;
        diseaseTotalDistrict[w].countPDIUTSRLtoF = 0;
        diseaseTotalDistrict[w].countPDITLtoF = 0;
        diseaseTotalDistrict[w].countPTC = 0;
        diseaseTotalDistrict[w].countPTD = 0;
        diseaseTotalDistrict[w].countPTR = 0;
        diseaseTotalDistrict[w].countRetreat = 0;
        diseaseTotalDistrict[w].countRetreatCure = 0;
        diseaseTotalDistrict[w].countRetreatDead = 0;
        diseaseTotalDistrict[w].countPO = 0;
        diseaseTotalDistrict[w].countPUI = 0;
        diseaseTotalDistrict[w].countPUINR = 0;
        diseaseTotalDistrict[w].countPUIUT = 0;
        diseaseTotalDistrict[w].countMDInf = 0;
        diseaseTotalDistrict[w].countMDUninf = 0;
        diseaseTotalDistrict[w].countTestPos = 0;
        diseaseTotalDistrict[w].countTestNeg = 0;
        diseaseTotalDistrict[w].countTruePos = 0;
        diseaseTotalDistrict[w].countFalsePos = 0;
        diseaseTotalDistrict[w].countTrueNeg = 0;
        diseaseTotalDistrict[w].countFalseNeg = 0;
        diseaseTotalDistrict[w].countDiagnosis = 0;
        diseaseTotalDistrict[w].countDiagnosisIn = 0;
        diseaseTotalDistrict[w].countDiagnosisOut = 0;
        diseaseTotalDistrict[w].countPreTestLtoFOut = 0;
        diseaseTotalDistrict[w].countPreTestLtoFOutTested = 0;
        diseaseTotalDistrict[w].countPreTestLtoFOutUnTested = 0;
        diseaseTotalDistrict[w].countPreTestLtoFOutUnTested2 = 0;
        diseaseTotalDistrict[w].countPreTestLtoFDead = 0;
        diseaseTotalDistrict[w].countPreTestLtoFORepresent = 0;
        diseaseTotalDistrict[w].onsetToTreat = 0;
        diseaseTotalDistrict[w].onsetToTreatIn = 0;
        diseaseTotalDistrict[w].onsetToTreatOut = 0;
        diseaseTotalDistrict[w].timeInfectious = 0;
        diseaseTotalDistrict[w].sampleTransTime = 0;
        diseaseTotalDistrict[w].sampleTransCount = 0;
        diseaseTotalDistrict[w].sampleWaitForResourceInTime = 0;
        diseaseTotalDistrict[w].sampleWaitForResourceInCount = 0;
        diseaseTotalDistrict[w].sampleWaitForResourceOutTime = 0;
        diseaseTotalDistrict[w].sampleWaitForResourceOutCount = 0;
        diseaseTotalDistrict[w].sampleCollectToTestInTime  = 0;
        diseaseTotalDistrict[w].sampleCollectToTestOutTime = 0;
        diseaseTotalDistrict[w].sampleCollectToTestInCount  = 0;
        diseaseTotalDistrict[w].sampleCollectToTestOutCount = 0;
        diseaseTotalDistrict[w].sampleTestTime = 0;
        diseaseTotalDistrict[w].sampleTestPosTimeIn = 0;
        diseaseTotalDistrict[w].sampleTestPosTimeOut = 0;
        diseaseTotalDistrict[w].sampleTestPosCountIn = 0;
        diseaseTotalDistrict[w].sampleTestPosCountOut = 0;
        diseaseTotalDistrict[w].sampleReturnTime = 0;
        diseaseTotalDistrict[w].sampleReturnedCount = 0;
        diseaseTotalDistrict[w].sampleReturnedInCount = 0;
        diseaseTotalDistrict[w].sampleReturnedOutCount = 0;
        diseaseTotalDistrict[w].sampleTestedInCount = 0;
        diseaseTotalDistrict[w].sampleTestedOutCount = 0;
        diseaseTotalDistrict[w].sampleNotTestedInCount = 0;
        diseaseTotalDistrict[w].sampleNotTestedOutCount = 0;
        diseaseTotalDistrict[w].sampleReturnToDeathTime = 0;
        diseaseTotalDistrict[w].sampleReturnToDeathPreDiagLtoFTime = 0;
        diseaseTotalDistrict[w].sampleReturnToDeathInTrtTime = 0;
        diseaseTotalDistrict[w].sampleReturnToDeathInRetrtTime = 0;
        diseaseTotalDistrict[w].sampleReturnToExitTime = 0;
        diseaseTotalDistrict[w].sampleReturnToDeathCount = 0;
        diseaseTotalDistrict[w].sampleReturnToDeathPreDiagLtoFCount = 0;
        diseaseTotalDistrict[w].sampleReturnToDeathInTrtCount = 0;
        diseaseTotalDistrict[w].sampleReturnToDeathInRetrtCount = 0;
        diseaseTotalDistrict[w].sampleReturnToExitCount = 0;
        diseaseTotalDistrict[w].sampleLtoFCount = 0;
        diseaseTotalDistrict[w].sampleWaitTestCount = 0;
        diseaseTotalDistrict[w].sampleWaitTestCountIn = 0;
        diseaseTotalDistrict[w].sampleWaitTestCountOut = 0;
        diseaseTotalDistrict[w].patientNotNotified = 0;
        diseaseTotalDistrict[w].patientNotNotifiedPos = 0;
        diseaseTotalDistrict[w].patientNotNotifiedIn = 0;
        diseaseTotalDistrict[w].patientNotNotifiedOut = 0;
        diseaseTotalDistrict[w].inTestCount = 0;
        diseaseTotalDistrict[w].outTestCount = 0;
        diseaseTotalDistrict[w].inCollectedCount = 0;
        diseaseTotalDistrict[w].outCollectedCount = 0;
        diseaseTotalDistrict[w].test1Name = "";
        diseaseTotalDistrict[w].test1Count = 0;
        diseaseTotalDistrict[w].test1CountIn = 0;
        diseaseTotalDistrict[w].test1CountOut = 0;
        diseaseTotalDistrict[w].test1PosCount = 0;
        diseaseTotalDistrict[w].test1TruePosCount = 0;
        diseaseTotalDistrict[w].test1NegCount = 0;
        diseaseTotalDistrict[w].test1TrueNegCount = 0;
        diseaseTotalDistrict[w].test2Name = "";
        diseaseTotalDistrict[w].test2Count = 0;
        diseaseTotalDistrict[w].test2CountIn = 0;
        diseaseTotalDistrict[w].test2CountOut = 0;
        diseaseTotalDistrict[w].test2PosCount = 0;
        diseaseTotalDistrict[w].test2TruePosCount = 0;
        diseaseTotalDistrict[w].test2NegCount = 0;
        diseaseTotalDistrict[w].test2TrueNegCount = 0;
        diseaseTotalDistrict[w].test3Name = "";
        diseaseTotalDistrict[w].test3Count = 0;
        diseaseTotalDistrict[w].tier1Count = 0;
        diseaseTotalDistrict[w].tier2Count = 0;
        diseaseTotalDistrict[w].tier3Count = 0;
        diseaseTotalDistrict[w].tier4Count = 0;
        diseaseTotalDistrict[w].tier5Count = 0;
        diseaseTotalDistrict[w].tier6Count = 0;
        diseaseTotalDistrict[w].countPreTestLtoFOutMinusTestedSamp = 0;
        diseaseTotalDistrict[w].dummy2Count = 0;
        diseaseTotalDistrict[w].dummy3Count = 0;
        diseaseTotalDistrict[w].dummy4Count = 0;
        diseaseTotalDistrict[w].dummy5Count = 0;
        diseaseTotalDistrict[w].dummy6Count = 0;

        if (debug1) std::cout << "stats: inside create agent stats after totals =  " << w << "\n";
    }

    if (debug) std::cout << "stats: inside create agent stats between structures  " << "\n";
    
    // initialize total regioal, facility and district stats
    totalFacility.tier = 10;
    totalFacility.district = "0";
    totalFacility.region = "0";
    totalFacility.pop = 0;
    totalFacility.countP = 0;
    totalFacility.countPLtoF = 0;
    totalFacility.countPInf = 0;
    totalFacility.countPInfTreated = 0;
    totalFacility.countPInfTreatedIn = 0;
    totalFacility.countPInfTreatedOut = 0;
    totalFacility.countPUninfTreated = 0;
    totalFacility.countPU = 0;
    totalFacility.countPI = 0;
    totalFacility.countPT = 0;
    totalFacility.countPR = 0;
    totalFacility.countPUT = 0;
    totalFacility.countPUTFN = 0;
    totalFacility.countPUTNS = 0;
    totalFacility.countPUTLtoF = 0;
    totalFacility.countPC = 0;
    totalFacility.countPDI = 0;
    totalFacility.countPDIT = 0;
    totalFacility.countPDIUT = 0;
    totalFacility.countPDIUTSR = 0;
    totalFacility.countPDIUTSNR = 0;
    totalFacility.countPDIUTLtF = 0;
    totalFacility.countPDU = 0;
    totalFacility.countPDOA = 0;
    totalFacility.countPDIUTPTLtoF = 0;
    totalFacility.countPDIUTSRLtoF = 0;
    totalFacility.countPDITLtoF = 0;
    totalFacility.countPTC = 0;
    totalFacility.countPTD = 0;
    totalFacility.countPTR = 0;
    totalFacility.countRetreat = 0;
    totalFacility.countRetreatCure = 0;
    totalFacility.countRetreatDead = 0;
    totalFacility.countPO = 0;
    totalFacility.countPUI = 0;
    totalFacility.countPUINR = 0;
    totalFacility.countPUIUT = 0;
    totalFacility.countMDInf = 0;
    totalFacility.countMDUninf = 0;
    totalFacility.countTestPos = 0;
    totalFacility.countTestNeg = 0;
    totalFacility.countTruePos = 0;
    totalFacility.countFalsePos = 0;
    totalFacility.countTrueNeg = 0;
    totalFacility.countFalseNeg = 0;
    totalFacility.countDiagnosis = 0;
    totalFacility.countDiagnosisIn = 0;
    totalFacility.countDiagnosisOut = 0;
    totalFacility.countPreTestLtoFOut = 0;
    totalFacility.countPreTestLtoFOutTested = 0;
    totalFacility.countPreTestLtoFOutUnTested = 0;
    totalFacility.countPreTestLtoFOutUnTested2 = 0;
    totalFacility.countPreTestLtoFDead = 0;
    totalFacility.countPreTestLtoFORepresent = 0;
    totalFacility.onsetToTreat = 0;
    totalFacility.onsetToTreatIn = 0;
    totalFacility.onsetToTreatOut = 0;
    totalFacility.timeInfectious = 0;
    totalFacility.sampleTransTime = 0;
    totalFacility.sampleTransCount = 0;
    totalFacility.sampleWaitForResourceInTime = 0;
    totalFacility.sampleWaitForResourceInCount = 0;
    totalFacility.sampleWaitForResourceOutTime = 0;
    totalFacility.sampleWaitForResourceOutCount = 0;
    totalFacility.sampleCollectToTestInTime  = 0;
    totalFacility.sampleCollectToTestOutTime = 0;
    totalFacility.sampleCollectToTestInCount  = 0;
    totalFacility.sampleCollectToTestOutCount = 0;
    totalFacility.sampleTestTime = 0;
    totalFacility.sampleTestPosTimeIn = 0;
    totalFacility.sampleTestPosTimeOut = 0;
    totalFacility.sampleTestPosCountIn = 0;
    totalFacility.sampleTestPosCountOut = 0;
    totalFacility.sampleReturnTime = 0;
    totalFacility.sampleReturnedCount = 0;
    totalFacility.sampleReturnedInCount = 0;
    totalFacility.sampleReturnedOutCount = 0;
    totalFacility.sampleTestedInCount = 0;
    totalFacility.sampleTestedOutCount = 0;
    totalFacility.sampleNotTestedInCount = 0;
    totalFacility.sampleNotTestedOutCount = 0;
    totalFacility.sampleReturnToDeathTime = 0;
    totalFacility.sampleReturnToDeathPreDiagLtoFTime = 0;
    totalFacility.sampleReturnToDeathInTrtTime = 0;
    totalFacility.sampleReturnToDeathInRetrtTime = 0;
    totalFacility.sampleReturnToExitTime = 0;
    totalFacility.sampleReturnToDeathCount = 0;
    totalFacility.sampleReturnToDeathPreDiagLtoFCount = 0;
    totalFacility.sampleReturnToDeathInTrtCount = 0;
    totalFacility.sampleReturnToDeathInRetrtCount = 0;
    totalFacility.sampleReturnToExitCount = 0;
    totalFacility.sampleLtoFCount = 0;
    totalFacility.sampleWaitTestCount = 0;
    totalFacility.sampleWaitTestCountIn = 0;
    totalFacility.sampleWaitTestCountOut = 0;
    totalFacility.patientNotNotified = 0;
    totalFacility.patientNotNotifiedPos = 0;
    totalFacility.patientNotNotifiedIn = 0;
    totalFacility.patientNotNotifiedOut = 0;
    totalFacility.inTestCount = 0;
    totalFacility.outTestCount = 0;
    totalFacility.inCollectedCount = 0;
    totalFacility.outCollectedCount = 0;
    totalFacility.test1Count = 0;
    totalFacility.test1CountIn = 0;
    totalFacility.test1CountOut = 0;
    totalFacility.test1PosCount = 0;
    totalFacility.test1TruePosCount = 0;
    totalFacility.test1NegCount = 0;
    totalFacility.test1TrueNegCount = 0;
    totalFacility.test2Name = "";
    totalFacility.test2Count = 0;
    totalFacility.test2CountIn = 0;
    totalFacility.test2CountOut = 0;
    totalFacility.test2PosCount = 0;
    totalFacility.test2TruePosCount = 0;
    totalFacility.test2NegCount = 0;
    totalFacility.test2TrueNegCount = 0;
    totalFacility.test3Name = "";
    totalFacility.test3Count = 0;
    totalFacility.tier1Count = 0;
    totalFacility.tier2Count = 0;
    totalFacility.tier3Count = 0;
    totalFacility.tier4Count = 0;
    totalFacility.tier5Count = 0;
    totalFacility.tier6Count = 0;
    totalFacility.countPreTestLtoFOutMinusTestedSamp = 0;
    totalFacility.dummy2Count = 0;
    totalFacility.dummy3Count = 0;
    totalFacility.dummy4Count = 0;
    totalFacility.dummy5Count = 0;
    totalFacility.dummy6Count = 0;
    totalRegion.tier = 10;
    totalRegion.district = "0";
    totalRegion.region = "0";
    totalRegion.pop = 0;
    totalRegion.countP = 0;
    totalRegion.countPLtoF = 0;
    totalRegion.countPInf = 0;
    totalRegion.countPInfTreated = 0;
    totalRegion.countPInfTreatedIn = 0;
    totalRegion.countPInfTreatedOut = 0;
    totalRegion.countPUninfTreated = 0;
    totalRegion.countPU = 0;
    totalRegion.countPI = 0;
    totalRegion.countPT = 0;
    totalRegion.countPR = 0;
    totalRegion.countPUT = 0;
    totalRegion.countPUTFN = 0;
    totalRegion.countPUTNS = 0;
    totalRegion.countPUTLtoF = 0;
    totalRegion.countPC = 0;
    totalRegion.countPDI = 0;
    totalRegion.countPDIT = 0;
    totalRegion.countPDIUT = 0;
    totalRegion.countPDIUTSR = 0;
    totalRegion.countPDIUTSNR = 0;
    totalRegion.countPDIUTLtF = 0;
    totalRegion.countPDU = 0;
    totalRegion.countPDOA = 0;
    totalRegion.countPDIUTPTLtoF = 0;
    totalRegion.countPDIUTSRLtoF = 0;
    totalRegion.countPDITLtoF = 0;
    totalRegion.countPTC = 0;
    totalRegion.countPTD = 0;
    totalRegion.countPTR = 0;
    totalRegion.countRetreat = 0;
    totalRegion.countRetreatCure = 0;
    totalRegion.countRetreatDead = 0;
    totalRegion.countPO = 0;
    totalRegion.countPUI = 0;
    totalRegion.countPUINR = 0;
    totalRegion.countPUIUT = 0;
    totalRegion.countMDInf = 0;
    totalRegion.countMDUninf = 0;
    totalRegion.countTestPos = 0;
    totalRegion.countTestNeg = 0;
    totalRegion.countTruePos = 0;
    totalRegion.countFalsePos = 0;
    totalRegion.countTrueNeg = 0;
    totalRegion.countFalseNeg = 0;
    totalRegion.countDiagnosis = 0;
    totalRegion.countDiagnosisIn = 0;
    totalRegion.countDiagnosisOut = 0;
    totalRegion.countPreTestLtoFOut = 0;
    totalRegion.countPreTestLtoFOutTested = 0;
    totalRegion.countPreTestLtoFOutUnTested = 0;
    totalRegion.countPreTestLtoFOutUnTested2 = 0;
    totalRegion.countPreTestLtoFDead = 0;
    totalRegion.countPreTestLtoFORepresent = 0;
    totalRegion.onsetToTreat = 0;
    totalRegion.onsetToTreatIn = 0;
    totalRegion.onsetToTreatOut = 0;
    totalRegion.timeInfectious = 0;
    totalRegion.sampleTransTime = 0;
    totalRegion.sampleTransCount = 0;
    totalRegion.sampleWaitForResourceInTime = 0;
    totalRegion.sampleWaitForResourceInCount = 0;
    totalRegion.sampleWaitForResourceOutTime = 0;
    totalRegion.sampleWaitForResourceOutCount = 0;
    totalRegion.sampleCollectToTestInTime  = 0;
    totalRegion.sampleCollectToTestOutTime = 0;
    totalRegion.sampleCollectToTestInCount  = 0;
    totalRegion.sampleCollectToTestOutCount = 0;
    totalRegion.sampleTestTime = 0;
    totalRegion.sampleTestPosTimeIn = 0;
    totalRegion.sampleTestPosTimeOut = 0;
    totalRegion.sampleTestPosCountIn = 0;
    totalRegion.sampleTestPosCountOut = 0;
    totalRegion.sampleReturnTime = 0;
    totalRegion.sampleReturnedCount = 0;
    totalRegion.sampleReturnedInCount = 0;
    totalRegion.sampleReturnedOutCount = 0;
    totalRegion.sampleTestedInCount = 0;
    totalRegion.sampleTestedOutCount = 0;
    totalRegion.sampleNotTestedInCount = 0;
    totalRegion.sampleNotTestedOutCount = 0;
    totalRegion.sampleReturnToDeathTime = 0;
    totalRegion.sampleReturnToDeathPreDiagLtoFTime = 0;
    totalRegion.sampleReturnToDeathInTrtTime = 0;
    totalRegion.sampleReturnToDeathInRetrtTime = 0;
    totalRegion.sampleReturnToExitTime = 0;
    totalRegion.sampleReturnToDeathCount = 0;
    totalRegion.sampleReturnToDeathPreDiagLtoFCount = 0;
    totalRegion.sampleReturnToDeathInTrtCount = 0;
    totalRegion.sampleReturnToDeathInRetrtCount = 0;
    totalRegion.sampleReturnToExitCount = 0;
    totalRegion.sampleLtoFCount = 0;
    totalRegion.sampleWaitTestCount = 0;
    totalRegion.sampleWaitTestCountIn = 0;
    totalRegion.sampleWaitTestCountOut = 0;
    totalRegion.patientNotNotified = 0;
    totalRegion.patientNotNotifiedPos = 0;
    totalRegion.patientNotNotifiedIn = 0;
    totalRegion.patientNotNotifiedOut = 0;
    totalRegion.inTestCount = 0;
    totalRegion.outTestCount = 0;
    totalRegion.inCollectedCount = 0;
    totalRegion.outCollectedCount = 0;
    totalRegion.test1Count = 0;
    totalRegion.test1CountIn = 0;
    totalRegion.test1CountOut = 0;
    totalRegion.test1PosCount = 0;
    totalRegion.test1TruePosCount = 0;
    totalRegion.test1NegCount = 0;
    totalRegion.test1TrueNegCount = 0;
    totalRegion.test1Name = "";
    totalRegion.test2Name = "";
    totalRegion.test2Count = 0;
    totalRegion.test2CountIn = 0;
    totalRegion.test2CountOut = 0;
    totalRegion.test2PosCount = 0;
    totalRegion.test2TruePosCount = 0;
    totalRegion.test2NegCount = 0;
    totalRegion.test2TrueNegCount = 0;
    totalRegion.test3Name = "";
    totalRegion.test3Count = 0;
    totalRegion.tier1Count = 0;
    totalRegion.tier2Count = 0;
    totalRegion.tier3Count = 0;
    totalRegion.tier4Count = 0;
    totalRegion.tier5Count = 0;
    totalRegion.tier6Count = 0;
    totalRegion.countPreTestLtoFOutMinusTestedSamp = 0;
    totalRegion.dummy2Count = 0;
    totalRegion.dummy3Count = 0;
    totalRegion.dummy4Count = 0;
    totalRegion.dummy5Count = 0;
    totalRegion.dummy6Count = 0;
    totalDistrict.tier = 10;
    totalDistrict.district = "0";
    totalDistrict.region = "0";
    totalDistrict.pop = 0;
    totalDistrict.countP = 0;
    totalDistrict.countPLtoF = 0;
    totalDistrict.countPInf = 0;
    totalDistrict.countPInfTreated = 0;
    totalDistrict.countPInfTreatedIn = 0;
    totalDistrict.countPInfTreatedOut = 0;
    totalDistrict.countPUninfTreated = 0;
    totalDistrict.countPU = 0;
    totalDistrict.countPI = 0;
    totalDistrict.countPT = 0;
    totalDistrict.countPR = 0;
    totalDistrict.countPUT = 0;
    totalDistrict.countPUTFN = 0;
    totalDistrict.countPUTNS = 0;
    totalDistrict.countPUTLtoF = 0;
    totalDistrict.countPC = 0;
    totalDistrict.countPDI = 0;
    totalDistrict.countPDIT = 0;
    totalDistrict.countPDIUT = 0;
    totalDistrict.countPDIUTSR = 0;
    totalDistrict.countPDIUTSNR = 0;
    totalDistrict.countPDIUTLtF = 0;
    totalDistrict.countPDU = 0;
    totalDistrict.countPDOA = 0;
    totalDistrict.countPDIUTPTLtoF = 0;
    totalDistrict.countPDIUTSRLtoF = 0;
    totalDistrict.countPDITLtoF = 0;
    totalDistrict.countPTC = 0;
    totalDistrict.countPTD = 0;
    totalDistrict.countPTR = 0;
    totalDistrict.countRetreat = 0;
    totalDistrict.countRetreatCure = 0;
    totalDistrict.countRetreatDead = 0;
    totalDistrict.countPO = 0;
    totalDistrict.countPUI = 0;
    totalDistrict.countPUINR = 0;
    totalDistrict.countPUIUT = 0;
    totalDistrict.countMDInf = 0;
    totalDistrict.countMDUninf = 0;
    totalDistrict.countTestPos = 0;
    totalDistrict.countTestNeg = 0;
    totalDistrict.countTruePos = 0;
    totalDistrict.countFalsePos = 0;
    totalDistrict.countTrueNeg = 0;
    totalDistrict.countFalseNeg = 0;
    totalDistrict.countDiagnosis = 0;
    totalDistrict.countDiagnosisIn = 0;
    totalDistrict.countDiagnosisOut = 0;
    totalDistrict.countPreTestLtoFOut = 0;
    totalDistrict.countPreTestLtoFOutTested = 0;
    totalDistrict.countPreTestLtoFOutUnTested = 0;
    totalDistrict.countPreTestLtoFOutUnTested2 = 0;
    totalDistrict.countPreTestLtoFDead = 0;
    totalDistrict.countPreTestLtoFORepresent = 0;
    totalDistrict.onsetToTreat = 0;
    totalDistrict.onsetToTreatIn = 0;
    totalDistrict.onsetToTreatOut = 0;
    totalDistrict.timeInfectious = 0;
    totalDistrict.sampleTransTime = 0;
    totalDistrict.sampleTransCount = 0;
    totalDistrict.sampleWaitForResourceInTime = 0;
    totalDistrict.sampleWaitForResourceInCount = 0;
    totalDistrict.sampleWaitForResourceOutTime = 0;
    totalDistrict.sampleWaitForResourceOutCount = 0;
    totalDistrict.sampleCollectToTestInTime  = 0;
    totalDistrict.sampleCollectToTestOutTime = 0;
    totalDistrict.sampleCollectToTestInCount  = 0;
    totalDistrict.sampleCollectToTestOutCount = 0;
    totalDistrict.sampleTestTime = 0;
    totalDistrict.sampleTestPosTimeIn = 0;
    totalDistrict.sampleTestPosTimeOut = 0;
    totalDistrict.sampleTestPosCountIn = 0;
    totalDistrict.sampleTestPosCountOut = 0;
    totalDistrict.sampleReturnTime = 0;
    totalDistrict.sampleReturnedCount = 0;
    totalDistrict.sampleReturnedInCount = 0;
    totalDistrict.sampleReturnedOutCount = 0;
    totalDistrict.sampleTestedInCount = 0;
    totalDistrict.sampleTestedOutCount = 0;
    totalDistrict.sampleNotTestedInCount = 0;
    totalDistrict.sampleNotTestedOutCount = 0;
    totalDistrict.sampleReturnToDeathTime = 0;
    totalDistrict.sampleReturnToDeathPreDiagLtoFTime = 0;
    totalDistrict.sampleReturnToDeathInTrtTime = 0;
    totalDistrict.sampleReturnToDeathInRetrtTime = 0;
    totalDistrict.sampleReturnToExitTime = 0;
    totalDistrict.sampleReturnToDeathCount = 0;
    totalDistrict.sampleReturnToDeathPreDiagLtoFCount = 0;
    totalDistrict.sampleReturnToDeathInTrtCount = 0;
    totalDistrict.sampleReturnToDeathInRetrtCount = 0;
    totalDistrict.sampleReturnToExitCount = 0;
    totalDistrict.sampleLtoFCount = 0;
    totalDistrict.sampleWaitTestCount = 0;
    totalDistrict.sampleWaitTestCountIn = 0;
    totalDistrict.sampleWaitTestCountOut = 0;
    totalDistrict.patientNotNotified = 0;
    totalDistrict.patientNotNotifiedPos = 0;
    totalDistrict.patientNotNotifiedIn = 0;
    totalDistrict.patientNotNotifiedOut = 0;
    totalDistrict.inTestCount = 0;
    totalDistrict.outTestCount = 0;
    totalDistrict.inCollectedCount = 0;
    totalDistrict.outCollectedCount = 0;
    totalDistrict.test1Name = "";
    totalDistrict.test1Count = 0;
    totalDistrict.test1CountIn = 0;
    totalDistrict.test1CountOut = 0;
    totalDistrict.test1PosCount = 0;
    totalDistrict.test1TruePosCount = 0;
    totalDistrict.test1NegCount = 0;
    totalDistrict.test1TrueNegCount = 0;
    totalDistrict.test2Name = "";
    totalDistrict.test2Count = 0;
    totalDistrict.test2CountIn = 0;
    totalDistrict.test2CountOut = 0;
    totalDistrict.test2PosCount = 0;
    totalDistrict.test2TruePosCount = 0;
    totalDistrict.test2NegCount = 0;
    totalDistrict.test2TrueNegCount = 0;
    totalDistrict.test3Name = "";
    totalDistrict.test3Count = 0;
    totalDistrict.tier1Count = 0;
    totalDistrict.tier2Count = 0;
    totalDistrict.tier3Count = 0;
    totalDistrict.tier4Count = 0;
    totalDistrict.tier5Count = 0;
    totalDistrict.tier6Count = 0;
    totalDistrict.countPreTestLtoFOutMinusTestedSamp = 0;
    totalDistrict.dummy2Count = 0;
    totalDistrict.dummy3Count = 0;
    totalDistrict.dummy4Count = 0;
    totalDistrict.dummy5Count = 0;
    totalDistrict.dummy6Count = 0;

    if (debug) std::cout << "stats: inside create agent stats before timesteps  "  << "\n";
    
    // update total population to take into account population if simulation run for more than 1 year
    // years start at 0
    if (timeStep <= timestepsPerYear) {

        noOfYears = 0;
        lowerBound = 0;
        upperBound = timestepsPerYear;
    }
    else if ((timeStep > timestepsPerYear) && (timeStep <= 2 * timestepsPerYear)) {

        noOfYears = 1;
        lowerBound = timestepsPerYear + 1;
        upperBound = 2 * timestepsPerYear;
    }
    else if ((timeStep > 2 * timestepsPerYear) && (timeStep <= 3 * timestepsPerYear)) {

        noOfYears = 2;
        lowerBound = 2 * timestepsPerYear + 1;
        upperBound = 3 * timestepsPerYear;
    }
    else if ((timeStep > 3 * timestepsPerYear) && (timeStep <= 4 * timestepsPerYear)) {

        noOfYears = 3;
        lowerBound = 3 * timestepsPerYear + 1;
        upperBound = 4 * timestepsPerYear;
    }
    else if ((timeStep > 4 * timestepsPerYear) && (timeStep <= 5 * timestepsPerYear)) {

        noOfYears = 4;
        lowerBound = 4 * timestepsPerYear + 1;
        upperBound = 5 * timestepsPerYear;
    }
    else if ((timeStep > 5 * timestepsPerYear) && (timeStep <= 6 * timestepsPerYear)) {

        noOfYears = 5;
        lowerBound = 5* timestepsPerYear + 1;
        upperBound = 6 * timestepsPerYear;
    }
    else if ((timeStep > 6 * timestepsPerYear) && (timeStep <= 7 * timestepsPerYear)) {

        noOfYears = 6;
        lowerBound = 6 * timestepsPerYear + 1;
        upperBound = 7 * timestepsPerYear;
    }
    else if ((timeStep > 7 * timestepsPerYear) && (timeStep <= 8 * timestepsPerYear)) {

        noOfYears = 7;
        lowerBound = 7 * timestepsPerYear + 1;
        upperBound = 8 * timestepsPerYear;
    }
    else if ((timeStep > 8 * timestepsPerYear) && (timeStep <= 9 * timestepsPerYear)) {

        noOfYears = 8;
        lowerBound = 8 * timestepsPerYear + 1;
        upperBound = 9 * timestepsPerYear;
    }
    else if ((timeStep > 9 * timestepsPerYear) && (timeStep <= 10 * timestepsPerYear)) {

        noOfYears = 9;
        lowerBound = 9 * timestepsPerYear + 1;
        upperBound = 10 * timestepsPerYear;
    }

    //noOfYears = (int) timeStep/timestepsPerYear;
    if (debug) std::cout << "stats: noOfYears =  "  << noOfYears << ", lowerBound = " << lowerBound << ", upperBound = " << upperBound << "\n";

    if (debug) {
    
        std::cout << "stats: inside agent stats time steps ="  << timeStep << ", ts per year = " << timestepsPerYear;
        std::cout << ", no. years = " << noOfYears << ", growth rate = " << popGrowthRate << "\n";
    }


    // if fileFlag is true, output to a file
    if (fileFlag) {

        if (statSubDir.length() > 0) {
            subDirBase = "./stats/" + statSubScenarioDir + "/job-" + statSubDir;
        }
        else {
            subDirBase = "./stats/" + statSubScenarioDir + "/job-" + to_string(1);
        }
  
        // convert to char array
        char subArray[subDirBase.length() +1];
        // copying the contents of the
        // string to char array
        strcpy(subArray, subDirBase.c_str());

        if (debug) std::cout << "inside agentStats subDirBase = " << subDirBase << "\n";

        errno = 0;
            
        errNo = mkdir(subArray, 0777);

        // check if directory is created or currently exists 
        if ((errNo != 0) && (errno != EEXIST)) {

            //std::cout << "unable to create directory = " << subArray << " errNO = " << errNo << " errno = " << errno << " EEXIST = " << EEXIST << "\n"; 
        }

        // save yearly stats away in per year directories
        if (yearly) {

            //std::cout << " statSubScenarioDir = " << statSubScenarioDir << "statSubDir = " << statSubDir <<" length = " << statSubDir.length() << "\n";
            //yearDir = "year-" + to_string(currentYearInRun);
            yearDir = "year-" + to_string(noOfYears);

            subDir = subDirBase + "/" + yearDir;
  
            // convert to char array
            char subArray2[subDir.length() +1];
            // copying the contents of the
            // string to char array
            strcpy(subArray2, subDir.c_str());

            errno = 0;

            errNo = mkdir(subArray2, 0777);
            
            // check if directory is created or not 
            if ((errNo != 0) && (errno != EEXIST)) {
                //std::cout << "unable to create directory = " << subArray2 << "\n"; 
            }

            // "./stats/scenario/job#/year#""

            myfile.open(subDir + "/AgentStats.csv");
            myfile6.open(subDir + "/AgentStats-Facility.csv");
            myfile7.open(subDir + "/AgentStats-District.csv");
            myfile8.open(subDir + "/AgentStats-Region.csv");
        }
        else {

            subDir = subDirBase + "/total";
  
            // convert to char array
            char subArray7[subDir.length() +1];
            // copying the contents of the
            // string to char array
            strcpy(subArray7, subDir.c_str());

            errno = 0;
            
            errNo = mkdir(subArray7, 0777);

            // check if directory is created or currently exists 
            if ((errNo != 0) && (errno != EEXIST)) {

                //std::cout << "unable to create directory = " << subArray7 << " errNO = " << errNo << " errno = " << errno << " EEXIST = " << EEXIST << "\n"; 
            }

            //std::cout << " statSubScenarioDir = " << statSubScenarioDir << "statSubDir = " << statSubDir << " length = " << statSubDir.length() << "\n";

            // "./stats/job#/total"

            myfile.open(subDir + "/AgentStats.csv");
            myfile6.open(subDir + "/AgentStats-Facility.csv");
            myfile7.open(subDir + "/AgentStats-District.csv");
            myfile8.open(subDir + "/AgentStats-Region.csv");

        }

        myfile << statHeader1 << statHeader2 << statHeader3 << statHeader4 << statHeader5 << statHeader5a << statHeader6 << statHeader7 << statHeader8;
        myfile << statHeader9 << statHeader10 << statHeader10a << statHeader11 << statHeader12 << statHeader12a << statHeader12b;
        myfile << statHeader13 << statHeader13a << statHeader13b << statHeader13c << statHeader13d << statHeader14 << statHeader15 << statHeader16 << statHeader17;

        myfile6 << statHeader1 << statHeader2 << statHeader3 << statHeader4 << statHeader5 << statHeader5a << statHeader6 << statHeader7 << statHeader8;
        myfile6 << statHeader9 << statHeader10 << statHeader10a << statHeader11 << statHeader12 << statHeader12a << statHeader12b;
        myfile6 << statHeader13 << statHeader13a << statHeader13b << statHeader13c << statHeader13d << statHeader14 << statHeader15 << statHeader16 << statHeader17;
    }

    // dump statistics for each patient - only do it to file
    if (individualFlag) {
        
        if (statSubDir.length() > 0) {
            subDirBase = "./stats/" + statSubScenarioDir + "/job-" + statSubDir;
        }
        else {
            subDirBase = "./stats/" + statSubScenarioDir + "/job-" + to_string(1);
        }
  
        // convert to char array
        char subArray[subDirBase.length() +1];
        // copying the contents of the
        // string to char array
        strcpy(subArray, subDirBase.c_str());


        errno = 0;
            
        errNo = mkdir(subArray, 0777);

        // check if directory is created or currently exists 
        if ((errNo != 0) && (errno != EEXIST)) {

            //std::cout << "unable to create directory = " << subArray << " errNO = " << errNo << " errno = " << errno << " EEXIST = " << EEXIST << "\n"; 
        }

        if (debug) std::cout << "inside agentStats subDirBase = " << subDirBase << ", yearly flag = " << yearly << "\n";

        // save yearly stats away in per year directories
        if (yearly) {

            //yearDir = "year-" + to_string(currentYearInRun);
            yearDir = "year-" + to_string(noOfYears);

            subDir = subDirBase + "/" + yearDir;
  
            // convert to char array
            char subArray3[subDir.length() +1];
            // copying the contents of the
            // string to char array
            strcpy(subArray3, subDir.c_str());

            // check if directory is created or not 
            if (mkdir(subArray3,0777) != 0) {

                //std::cout << "unable to create directory = " << subArray3 << "\n"; 
            }           
   
            myfile1.open(subDir + "/AgentStatsPatientsFacility.csv");
            myfile2.open(subDir + "/AgentStatsPatientsRegion.csv");
            myfile3.open(subDir + "/AgentStatsPatientsDistrict.csv");
            myfile4.open(subDir + "/AgentStatsPatientsAway.csv");
            myfile5.open(subDir + "/AgentStatsPatientsIn.csv");
            myfile9.open(subDir + "/AgentStatsPatients.csv");
            myfile10.open(subDir + "/AgentStatsPatientsRaw.csv");
            myfile11.open(subDir + "/AgentStatsPatientsRawAdmin.csv");
        }
        else {

            subDir = subDirBase + "/total/";
  
            // convert to char array
            char subArray5[subDir.length() +1];
            // copying the contents of the
            // string to char array
            strcpy(subArray5, subDir.c_str());

            // check if directory is created or not 
            if (mkdir(subArray5,0777) != 0) {

                //std::cout << "unable to create directory = " << subArray5 << "\n"; 
            }

            myfile1.open(subDir + "/AgentStatsPatientsFacility.csv");
            myfile2.open(subDir + "/AgentStatsPatientsRegion.csv");
            myfile3.open(subDir + "/AgentStatsPatientsDistrict.csv");
            myfile4.open(subDir + "/AgentStatsPatientsAway.csv");
            myfile5.open(subDir + "/AgentStatsPatientsIn.csv"); 
            myfile9.open(subDir + "/AgentStatsPatients.csv"); 
            myfile10.open(subDir + "/AgentStatsPatientsRaw.csv");
            myfile11.open(subDir + "/AgentStatsPatientsRawAdmin.csv");
        }

        // put header in raw patient file
        myfile10 << "facility" << ",district" << ",region" << ",id" << ",tier";
        myfile10 << ",status" << ",status.string" << ",previous.status" << ",disease" << ",active" << ",age" << ",alive" << ",infected" << ",test.pos" << ",test.neg" << ",md.override" <<  ",time.patient.created" << ",time.sample.created";
        myfile10 <<  ",sample.status" << ",sample.status.string" << ",sample.previous.status" << ",time.results.returned.to.patient" <<  ",time.presentation.til.patient.receives.result";
        myfile10 << ",time.mdoveride.no.test" <<  ",time.started.treatment" <<   ",presentation.til.treatment" << ",time.in.treatment";
        myfile10 << ",time.not.in.treatment" << ",time.onset.to.treatment" << ",time.step.loss.to.followup" << ",no.times.returned.results" << ",sample.transit.time" << ",sample.return.transit.time";
        myfile10 << ",time.samp.result.was.tested" << ",time.samp.result.returned.to.patient" << ",test" << ",testing.facility" << ",testing.facility.distance";
        myfile10 << "\n";

        myfile11 << "admin.unit" << ",name" << ",population" << ",patients" << "\n";
    }

    // dump statistics for each patient - only do it to file
    if (individualSamplingFlag) {
        
        if (statSubDir.length() > 0) {
            subDirBase = "./stats/" + statSubScenarioDir + "/job-" + statSubDir;
        }
        else {
            subDirBase = "./stats/" + statSubScenarioDir + "/job-" + to_string(1);
        }
  
        // convert to char array
        char subArray[subDirBase.length() +1];
        // copying the contents of the
        // string to char array
        strcpy(subArray, subDirBase.c_str());


        errno = 0;
            
        errNo = mkdir(subArray, 0777);

        // check if directory is created or currently exists 
        if ((errNo != 0) && (errno != EEXIST)) {

            //std::cout << "unable to create directory = " << subArray << " errNO = " << errNo << " errno = " << errno << " EEXIST = " << EEXIST << "\n"; 
        }

        if (debug) std::cout << "inside agentStats subDirBase = " << subDirBase << ", yearly flag = " << yearly << "\n";

        // save yearly stats away in per year directories
        if (yearly) {

            //yearDir = "year-" + to_string(currentYearInRun);
            yearDir = "year-" + to_string(noOfYears);

            subDir = subDirBase + "/" + yearDir;
  
            // convert to char array
            char subArray3[subDir.length() +1];
            // copying the contents of the
            // string to char array
            strcpy(subArray3, subDir.c_str());

            // check if directory is created or not 
            if (mkdir(subArray3,0777) != 0) {

                //std::cout << "unable to create directory = " << subArray3 << "\n"; 
            }           
   
            myfile12.open(subDir + "/AgentStatsPatientsSamplingRaw.csv");
        }
        else {

            subDir = subDirBase + "/total/";
  
            // convert to char array
            char subArray5[subDir.length() +1];
            // copying the contents of the
            // string to char array
            strcpy(subArray5, subDir.c_str());

            // check if directory is created or not 
            if (mkdir(subArray5,0777) != 0) {

                //std::cout << "unable to create directory = " << subArray5 << "\n"; 
            }

            myfile12.open(subDir + "/AgentStatsPatientsSamplingRaw.csv");

        }

        // put header in raw sampling patient file
        myfile12 << "facility" << ",district" << ",region" << ",id" << ",tier";
        myfile12 << ",status" << ",status.string" << ",previous.status" << ",disease" << ",active" << ",age" << ",alive" << ",infected" << ",test.pos" << ",test.neg" << ",md.override" <<  ",time.patient.created" << ",time.sample.created";
        myfile12 <<  ",sample.status" << ",sample.status.string" << ",sample.previous.status" << ",time.results.returned.to.patient" <<  ",time.presentation.til.patient.receives.result";
        myfile12 << ",time.mdoveride.no.test" <<  ",time.started.treatment" <<   ",presentation.til.treatment" << ",time.in.treatment";
        myfile12 << ",time.not.in.treatment" << ",time.onset.to.treatment" << ",time.step.loss.to.followup" << ",sample.transit.time" << ",sample.return.transit.time";
        myfile12 << ",time.samp.result.was.tested" << ",time.samp.result.returned.to.patient" << ",test" << ",testing.facility" << ",testing.facility.distance";
        myfile12 << ",patientNo" << "\n";
    }

    // loop through each facility and the number of Patients and Samples at each facility
    for (auto facility : facilities) {

        // get the up name for this facility
        d = facility->getDiagnosisPtr();
        t = facility->getTestingPtr();
        p = d->getPatientPtr();
        pI = d->getInactivePatientPtr();
        s = t->getSamplePtr();
        sI = t->getInactiveSamplePtr();
        region = facility->getRegion();
        district = facility->getDistrict();
        facilityPop = facility->getPopulation();
        total.pop = total.pop + facilityPop;
        noPatientInactive = 0;

        // get patient stats
        patReturnedCount = 0;
        facilityCountP = p.size();
        total.countP = total.countP + facilityCountP;

        // update facility statistics
        for (int w = 0; w < diseaseCount; w++) {
            facilityStats[w][count].name = facility->getName();
            facilityStats[w][count].district = district;
            facilityStats[w][count].region = region;
            facilityStats[w][count].id = facility->getID();
            facilityStats[w][count].tier = facility->getTier();
            facilityStats[w][count].pop = facilityPop;
            facilityStats[w][count].countP = 0;
            facilityStats[w][count].countPLtoF = 0;
            facilityStats[w][count].countPInf = 0;
            facilityStats[w][count].countPInfTreated = 0;
            facilityStats[w][count].countPInfTreatedIn = 0;
            facilityStats[w][count].countPInfTreatedOut = 0;
            facilityStats[w][count].countPUninfTreated = 0;
            facilityStats[w][count].countPU = 0;
            facilityStats[w][count].countPI = 0;
            facilityStats[w][count].countPT = 0;
            facilityStats[w][count].countPR = 0;
            facilityStats[w][count].countPUT = 0;
            facilityStats[w][count].countPUTFN = 0;
            facilityStats[w][count].countPUTNS = 0;
            facilityStats[w][count].countPUTLtoF = 0;
            facilityStats[w][count].countPC = 0;
            facilityStats[w][count].countPDI = 0;
            facilityStats[w][count].countPDIT = 0;
            facilityStats[w][count].countPDIUT = 0;
            facilityStats[w][count].countPDIUTSR = 0;
            facilityStats[w][count].countPDIUTSNR = 0;
            facilityStats[w][count].countPDIUTLtF = 0;
            facilityStats[w][count].countPDU = 0;
            facilityStats[w][count].countPDOA = 0;
            facilityStats[w][count].countPDIUTPTLtoF = 0;
            facilityStats[w][count].countPDIUTSRLtoF = 0;
            facilityStats[w][count].countPDITLtoF = 0;
            facilityStats[w][count].countPTC = 0;
            facilityStats[w][count].countPTD = 0;
            facilityStats[w][count].countPTR = 0;
            facilityStats[w][count].countRetreat = 0;
            facilityStats[w][count].countRetreatCure = 0;
            facilityStats[w][count].countRetreatDead = 0;
            facilityStats[w][count].countPO = 0;
            facilityStats[w][count].countPUI = 0;
            facilityStats[w][count].countPUINR = 0;
            facilityStats[w][count].countPUIUT = 0;
            facilityStats[w][count].countMDInf = 0;
            facilityStats[w][count].countMDUninf = 0;
            facilityStats[w][count].countTestPos = 0;
            facilityStats[w][count].countTestNeg = 0;
            facilityStats[w][count].countTruePos = 0;
            facilityStats[w][count].countFalsePos = 0;
            facilityStats[w][count].countTrueNeg = 0;
            facilityStats[w][count].countFalseNeg = 0;
            facilityStats[w][count].countDiagnosis = 0;
            facilityStats[w][count].countDiagnosisIn = 0;
            facilityStats[w][count].countDiagnosisOut = 0;
            facilityStats[w][count].countPreTestLtoFOut = 0;
            facilityStats[w][count].countPreTestLtoFOutTested = 0;
            facilityStats[w][count].countPreTestLtoFOutUnTested = 0;
            facilityStats[w][count].countPreTestLtoFOutUnTested2 = 0;
            facilityStats[w][count].countPreTestLtoFDead = 0;
            facilityStats[w][count].countPreTestLtoFORepresent = 0;
            facilityStats[w][count].onsetToTreat = 0;
            facilityStats[w][count].onsetToTreatIn = 0;
            facilityStats[w][count].onsetToTreatOut = 0;
            facilityStats[w][count].timeInfectious = 0;
            facilityStats[w][count].sampleTransTime = 0;
            facilityStats[w][count].sampleTransCount = 0;
            facilityStats[w][count].sampleWaitForResourceInTime = 0;
            facilityStats[w][count].sampleWaitForResourceInCount = 0;
            facilityStats[w][count].sampleWaitForResourceOutTime = 0;
            facilityStats[w][count].sampleWaitForResourceOutCount = 0;
            facilityStats[w][count].sampleCollectToTestInTime  = 0;
            facilityStats[w][count].sampleCollectToTestOutTime = 0;
            facilityStats[w][count].sampleCollectToTestInCount  = 0;
            facilityStats[w][count].sampleCollectToTestOutCount = 0;
            facilityStats[w][count].sampleTestTime = 0;
            facilityStats[w][count].sampleTestPosTimeIn = 0;
            facilityStats[w][count].sampleTestPosTimeOut = 0;
            facilityStats[w][count].sampleTestPosCountIn = 0;
            facilityStats[w][count].sampleTestPosCountOut = 0;
            facilityStats[w][count].sampleReturnTime = 0;
            facilityStats[w][count].sampleReturnedCount = 0;
            facilityStats[w][count].sampleReturnedInCount = 0;
            facilityStats[w][count].sampleReturnedOutCount = 0;
            facilityStats[w][count].sampleTestedInCount = 0;
            facilityStats[w][count].sampleTestedOutCount = 0;
            facilityStats[w][count].sampleNotTestedInCount = 0;
            facilityStats[w][count].sampleNotTestedOutCount = 0;
            facilityStats[w][count].sampleReturnToDeathTime = 0;
            facilityStats[w][count].sampleReturnToDeathPreDiagLtoFTime = 0;
            facilityStats[w][count].sampleReturnToDeathInTrtTime = 0;
            facilityStats[w][count].sampleReturnToDeathInRetrtTime = 0;
            facilityStats[w][count].sampleReturnToExitTime = 0;
            facilityStats[w][count].sampleReturnToDeathCount = 0;
            facilityStats[w][count].sampleReturnToDeathPreDiagLtoFCount = 0;
            facilityStats[w][count].sampleReturnToDeathInTrtCount = 0;
            facilityStats[w][count].sampleReturnToDeathInRetrtCount = 0;
            facilityStats[w][count].sampleReturnToExitCount = 0;
            facilityStats[w][count].sampleLtoFCount = 0;
            facilityStats[w][count].sampleWaitTestCount = 0;
            facilityStats[w][count].sampleWaitTestCountIn = 0;
            facilityStats[w][count].sampleWaitTestCountOut = 0;
            facilityStats[w][count].patientNotNotified = 0;
            facilityStats[w][count].patientNotNotifiedPos = 0;
            facilityStats[w][count].patientNotNotifiedIn = 0;
            facilityStats[w][count].patientNotNotifiedOut = 0;
            facilityStats[w][count].inTestCount = 0;
            facilityStats[w][count].outTestCount = 0;
            facilityStats[w][count].inCollectedCount = 0;
            facilityStats[w][count].outCollectedCount = 0;
            facilityStats[w][count].test1Count = 0;
            facilityStats[w][count].test1CountIn = 0;
            facilityStats[w][count].test1CountOut = 0;
            facilityStats[w][count].test1PosCount = 0;
            facilityStats[w][count].test1TruePosCount = 0;
            facilityStats[w][count].test1NegCount = 0;
            facilityStats[w][count].test1TrueNegCount = 0;
            facilityStats[w][count].test2Name = "";
            facilityStats[w][count].test2Count = 0;
            facilityStats[w][count].test2CountIn = 0;
            facilityStats[w][count].test2CountOut = 0;
            facilityStats[w][count].test2PosCount = 0;
            facilityStats[w][count].test2TruePosCount = 0;
            facilityStats[w][count].test2NegCount = 0;
            facilityStats[w][count].test2TrueNegCount = 0;
            facilityStats[w][count].test3Name = "";
            facilityStats[w][count].test3Count = 0;
            facilityStats[w][count].tier1Count = 0;
            facilityStats[w][count].tier2Count = 0;
            facilityStats[w][count].tier3Count = 0;
            facilityStats[w][count].tier4Count = 0;
            facilityStats[w][count].tier5Count = 0;
            facilityStats[w][count].tier6Count = 0;
            facilityStats[w][count].countPreTestLtoFOutMinusTestedSamp = 0;
            facilityStats[w][count].dummy2Count = 0;
            facilityStats[w][count].dummy3Count = 0;
            facilityStats[w][count].dummy4Count = 0;
            facilityStats[w][count].dummy5Count = 0;
            facilityStats[w][count].dummy6Count = 0;

            patientCount[w] = 0;

            // individual facility totals
            indNoPatients[w] = 0;
            indAge[w] = 0;
            indNoAlive[w] = 0;
            indNoInfected[w] = 0;
            indNoTestPos[w] = 0;
            indNoMDOverride[w] = 0;
            indTotalTimeFromOnset[w] = 0;
            indTotalTimeInfectious[w] = 0;
            indTotaltimeInTreatment[w] = 0;
            indTotalTimeNotInTreatment[w] = 0;
            indTotalTimeFacilityReturnTime[w] = 0;
            indTotalTimeResultsReturned[w] = 0;
            indTotalNoOfReturnVisits[w] = 0;
            indDisease[w] = diseaseList[w];
        }

        if (individualFlag) {

            myfile1 << "-----------------------------------------------------------------------------------  " << "\n";
            myfile1 << "stats.fac" << ",id" << ",tier" << ",patients" <<  "\n";
            myfile1 << facilityStats[0][count].name << "," <<  facilityStats[0][count].id << "," <<  facilityStats[0][count].tier << "," << facilityCountP << "\n";
            if (facilityCountP  > 0) {

                myfile1 << "facility" << ",id" <<  ",tier" << ",status" << ",disease" << ",active" << ",age" << ",alive" << ",infected" << ",test.pos" << ",md.override";
                myfile1 << ",time.from.onset" << ",time.in.treatment" << ",time.not.in.treatment" << ",time.onset.to.treatment" << ",time.step.created" << "\n";
            }

            myfile9 << "-----------------------------------------------------------------------------------  " << "\n";
            myfile9 << "stats.fac" << ",id" <<  ",tier" << ",patients" <<  "\n";
            myfile9 << facilityStats[0][count].name << "," <<  facilityStats[0][count].id << "," <<  facilityStats[0][count].tier << "," << facilityCountP << "\n";
            if (facilityCountP  > 0) {
            
                myfile9 << "facility" << ",id" <<  ",tier" << ",status" << ",disease" << ",active" << ",age" << ",alive" << ",infected" << ",test.pos" << ",md.override";
                myfile9  << ",time.from.onset" << ",time.in.treatment" << ",time.not.in.treatment" << ",time.onset.to.treatment" << ",time.step.created" << "\n";
            }

            // just patient data - adjust population for population growth each year
            newPop = facilityStats[0][count].pop;
            startIndex = referenceYear - 2015;
            endIndex = startIndex + noOfYears;
            if (noOfYears > 0) {

                for (int i = startIndex; i < endIndex; i++) {

                    // multiply by growth rate for x number of years
                    newPop = (int) (newPop * popGrowthRateArray[i][1]);
                }
            }
            myfile11 << "facility" << "," << facilityStats[0][count].name << "," <<  newPop << "," << facilityCountP << "\n";
        }

        // debug - true
        if (debug) std::cout << "stats: inside agent stats before patient loop no. of facility = " << "," << facilityStats[0][count].name << ", no.of patients = " << facilityCountP << "\n";

        for (auto pat : p) {

            // find sample associated with Patient - it is either here or at testing Facility
            // check here first
            samplePtr = pat->getSamplePtr();

            //debug - true -false
            if (debug) {
                std::cout << "stats: inside agent statsafter get sample ptr sample transit time = " << samplePtr->getTransitTimeSave() << "\n";
            }

            
            // couldn't find it in the active or inactive containers
            if (samplePtr == nullptr) {

                std::cout << "stats: couldn't find sample for active patient  status = " << pat->getStatus() << " for facility = " << pat->getFacility();
                std::cout << " testing facility = " << pat->getSampleTestFacility() << " getUpName() = " << facility->getUpName() << "\n";
            }
            else {

                sampleStatusString = "unknown";

                if (debug) std::cout << "stats: inside agent statsafter get status " << samplePtr->getStatus() << "\n";
                    
                switch(samplePtr->getStatus()) {
                        
                    case 0:
                        
                        sampleStatusString = "SampCreatedUnTest -created but untested";
                        break;
                    case 1:
                        
                        sampleStatusString = "SampCreatedUnTestBatch - created untested and waiting for batch transit";
                        break;
                    case 2:
                        
                        sampleStatusString = "SampUnTestInTransit - in transit";
                        break;
                    case 3:
                        
                        sampleStatusString = "SampWaitForTest - waiting for test";
                        break;
                    case 4:
                        
                        sampleStatusString = "SampLostExpired - lost/expired";
                        break;
                    case 5:
                        
                        sampleStatusString = "SampTestedInTransit - results in transit back to orig facility";
                        break;
                    case 6:
                        
                        sampleStatusString = "SampReturnedAvail - results returned - available";
                        break;
                    case 7:
                        
                        sampleStatusString = "SampReturnedLostExpired - lost/expired and returned";
                        break;
                    case 8:
                        
                        sampleStatusString = "SampLtoF - loss to follow up";
                        break;
                    case 9:
                        
                        sampleStatusString = "SampPatDead - patient dead";
                        break;
                    default:
                        
                        sampleStatusString = "unknown";
                }

                // mark whether sample was tested in House
                if (samplePtr->getOrigFacility()  ==  samplePtr->getTestingFacility()) {
                    
                    testedInHouse = true;
                }
                else {

                    testedInHouse = false;
                }
            }
                
            if (debug) std::cout << "stats: inside agent stats sample string = " << sampleStatusString  << "\n";

            // get which year patient is created - include last day in previous year (365, 730, etc.)
            patientCreated = pat->getTimestepPresented();
            
            // if only doing yearly output, skip if not in current year
            if (yearly && ((patientCreated < lowerBound) || (patientCreated > upperBound))) {
               
                // skip patient if yearly output and not in this year
                //std::cout << " skipping patient lower bound = " << lowerBound << ", upperbound  = " << upperBound << " timestep of patient = " << patientCreated << " yearly = " << yearly << "\n";
            }
            else {
            
                //std::cout << "time step = " << timeStep << ", noOfYears = " << noOfYears << "\n";

                // keep track of total patients
                noTotalPatients++;
                
                // store stats by disease
                diseaseName = pat->getDisease();

                // get the index for the disease
                diseaseIndex = 0;

                for (int w = 0; w < diseaseCount; w++) {

                    if (diseaseName == diseaseList[w]) {
                        diseaseIndex = w;

                        // increase the sample count for this disease for this facility
                        patientCount[w]++;
                    }
                }


                // 0 - away from facility - hasn't returned for results
                // 1 - in-house
                // 2 - sample results returned, undergoing treatment
                // 3 - sample results returned, treatment failure
                // 4 - sample results returned, no treatment
                // 5 - no sample results returned, no treatment
                // 6 - sample results returned, no treatment - loss to follow up
                // 7 - patient dead - PatDead 
                // 8 - patient cured
                // 9 - patient uninfected - returned sample
                // 10 - patient uninfected - didn't return for sample
                // 11 - patient uninfected - under treatment
                status = pat->getStatus();

                if (debug) std::cout << "patient status = " << status << "\n";

                switch (status) {
                case 0:
                    if (testedInHouse) {
                    
                        //std::cout << "patient testing out but patient seems in status = " << status << "\n";
                    }
                    else {
                    
                        total.countPU++;
                        diseaseTotal[diseaseIndex].countPU++;
                        facilityStats[diseaseIndex][count].countPU++;
                    }
                    if (individualFlag) {
                        myfile4 << "stats.patient.ptr_" << pat << ",facility_" << pat->getFacility() << "\n";
                    }
                    statusString = "PatAwayUnTest";
                    break;
                case 1:
                    if (testedInHouse) {
                    
                        total.countPI++;
                        diseaseTotal[diseaseIndex].countPI++;
                        facilityStats[diseaseIndex][count].countPI++;
                    }
                    else {

                        //std::cout << "patient inhouse but testing out status = " << status << "\n";
                    }
                    if (individualFlag) {
                        myfile5 << "stats.patient.ptr_" << pat << ",facility_" << pat->getFacility() << "\n";
                    }
                    statusString = "PatInUnTest";
                    break;
                    
                case 2:
                    total.countPT++;
                    diseaseTotal[diseaseIndex].countPT++;
                    facilityStats[diseaseIndex][count].countPT++;

                    // update time from onset to treatment for patients undergoing treatment
                    presentToTreatTime = pat->getStartedTreatmentTime() - pat->getTimestepPresented();
                    
                    // swapped (pat->getStartedTreatmentTime() - pat->getTimestepPresented()) for pat->getOnsetTilTreatment()
                    total.onsetToTreat = total.onsetToTreat + presentToTreatTime;
                    diseaseTotal[diseaseIndex].onsetToTreat = diseaseTotal[diseaseIndex].onsetToTreat + presentToTreatTime;
                    facilityStats[diseaseIndex][count].onsetToTreat = facilityStats[diseaseIndex][count].onsetToTreat + presentToTreatTime;

                    // update time from onset to moving to inactive for patients undergoing treatment
                    total.timeInfectious = total.timeInfectious + (pat->getInfectEndTime() - pat->getTimestepCreated());
                    diseaseTotal[diseaseIndex].timeInfectious = diseaseTotal[diseaseIndex].timeInfectious + (pat->getInfectEndTime() - pat->getTimestepCreated());
                    facilityStats[diseaseIndex][count].timeInfectious = facilityStats[diseaseIndex][count].timeInfectious + (pat->getInfectEndTime() - pat->getTimestepCreated());
                   
                    statusString = "PatInfTestTreat";
                    break;

                case 3:
                    total.countPR++;
                    diseaseTotal[diseaseIndex].countPR++;
                    facilityStats[diseaseIndex][count].countPR++;

                    // update time from onset to treatment for patients undergoing treatment
                    //total.onsetToTreat = total.onsetToTreat + (pat->getStartedTreatmentTime() - pat->getTimestepPresented());
                    //diseaseTotal[diseaseIndex].onsetToTreat = diseaseTotal[diseaseIndex].onsetToTreat + (pat->getStartedTreatmentTime() - pat->getTimestepPresented());
                    //facilityStats[diseaseIndex][count].onsetToTreat = facilityStats[diseaseIndex][count].onsetToTreat + (pat->getStartedTreatmentTime() - pat->getTimestepPresented());

                    // update time from onset to moving to inactive for patients undergoing treatment
                    total.timeInfectious = total.timeInfectious + (pat->getInfectEndTime() - pat->getTimestepCreated());
                    diseaseTotal[diseaseIndex].timeInfectious = diseaseTotal[diseaseIndex].timeInfectious + (pat->getInfectEndTime() - pat->getTimestepCreated());
                    facilityStats[diseaseIndex][count].timeInfectious = facilityStats[diseaseIndex][count].timeInfectious + (pat->getInfectEndTime() - pat->getTimestepCreated());

                    statusString = "PatInfTestRelapse";
                    break;
                case 4:
                    total.countPUT++;
                    diseaseTotal[diseaseIndex].countPUT++;
                    facilityStats[diseaseIndex][count].countPUT++;

                    total.countPUTFN++;
                    diseaseTotal[diseaseIndex].countPUTFN++;
                    facilityStats[diseaseIndex][count].countPUTFN++;

                    // update time from onset to moving to inactive for patients undergoing treatment
                    total.timeInfectious = total.timeInfectious + (pat->getInfectEndTime() - pat->getTimestepCreated());
                    diseaseTotal[diseaseIndex].timeInfectious = diseaseTotal[diseaseIndex].timeInfectious + (pat->getInfectEndTime() - pat->getTimestepCreated());
                    facilityStats[diseaseIndex][count].timeInfectious = facilityStats[diseaseIndex][count].timeInfectious + (pat->getInfectEndTime() - pat->getTimestepCreated());

                    statusString = "PatInfTestNoTreat";
                    break;
                case 5:

                    //std::cout << "stats.patient status = 5" << "\n";
                    total.countPUT++;
                    diseaseTotal[diseaseIndex].countPUT++;
                    facilityStats[diseaseIndex][count].countPUT++;
                    
                    total.countPUTNS++;
                    diseaseTotal[diseaseIndex].countPUTNS++;
                    facilityStats[diseaseIndex][count].countPUTNS++;

                    // update time from onset to moving to inactive for patients undergoing treatment
                    total.timeInfectious = total.timeInfectious + (pat->getInfectEndTime() - pat->getTimestepCreated());
                    diseaseTotal[diseaseIndex].timeInfectious = diseaseTotal[diseaseIndex].timeInfectious + (pat->getInfectEndTime() - pat->getTimestepCreated());
                    facilityStats[diseaseIndex][count].timeInfectious = facilityStats[diseaseIndex][count].timeInfectious + (pat->getInfectEndTime() - pat->getTimestepCreated());

                    statusString = "PatInfNoTestNoTreat";
                    break;
                case 6:
                    total.countPUT++;
                    diseaseTotal[diseaseIndex].countPUT++;
                    facilityStats[diseaseIndex][count].countPUT++;
  
                    total.countPUTLtoF++;
                    diseaseTotal[diseaseIndex].countPUTLtoF++;
                    facilityStats[diseaseIndex][count].countPUTLtoF++;

                    statusString = "PatInfTestLtofNoTreat";
                    break;
                case 7:

                    if (debug) std::cout << "stats: inside agent stats active patients dead infected = " << pat->getInfected() << " reason = " << pat->getReasonNoTreat() << "\n";

                    // NoTreatDeath OldAge TreatDeath RelapseDeath

                    if (pat->getInfected()) {

                         // debug - true
                         if (debug) std::cout << "stats: inside agent stats active patients dead reason = " << pat->getReasonNoTreat() << ", reason died = " << pat->getReasonDied() << "\n";
                            
                        // track untreated dead and treated dead
                        reason = pat->getReasonNoTreat();
                        if (reason == "PatInfTestNoTreat") {
                        
                            total.countPDIUTSR++;
                            diseaseTotal[diseaseIndex].countPDIUTSR++;
                            facilityStats[diseaseIndex][count].countPDIUTSR++;
                            total.countPDIUT++;
                            diseaseTotal[diseaseIndex].countPDIUT++;
                            facilityStats[diseaseIndex][count].countPDIUT++;
                        }
                        else if ((reason == "PatInfNoTestNoTreat") || (reason == "PatInfNoTestLtofPreTreat")) {
                        
                            total.countPDIUTSNR++;
                            diseaseTotal[diseaseIndex].countPDIUTSNR++;
                            facilityStats[diseaseIndex][count].countPDIUTSNR++;
                            total.countPDIUT++;
                            diseaseTotal[diseaseIndex].countPDIUT++;
                            facilityStats[diseaseIndex][count].countPDIUT++;
                        }
                        else if (reason == "PatInfTestLtofNoTreat") {
                        
                            total.countPDIUTLtF++;
                            diseaseTotal[diseaseIndex].countPDIUTLtF++;
                            facilityStats[diseaseIndex][count].countPDIUTLtF++;
                            total.countPDIUT++;
                            diseaseTotal[diseaseIndex].countPDIUT++;
                            facilityStats[diseaseIndex][count].countPDIUT++;
                        }
                        else if (reason == "PatInfNoTestLtofPreTreat") {
                        
                            total.countPDIUT++;
                            diseaseTotal[diseaseIndex].countPDIUT++;
                            facilityStats[diseaseIndex][count].countPDIUT++;
                        }
                        else if (reason == "PatInfTestLtofAssignNoShow") {
                        
                            total.countPDIUT++;
                            diseaseTotal[diseaseIndex].countPDIUT++;
                            facilityStats[diseaseIndex][count].countPDIUT++;
                        }
                        else if (reason == "PatInfPreTreatLtoF") {
                        
                            total.countPDIUT++;
                            diseaseTotal[diseaseIndex].countPDIUT++;
                            facilityStats[diseaseIndex][count].countPDIUT++;
                        }
                        else {
                            total.countPDI++;
                            diseaseTotal[diseaseIndex].countPDI++;
                            facilityStats[diseaseIndex][count].countPDI++;

                            // update time from onset to treatment for patients undergoing treatment
                            //total.onsetToTreat = total.onsetToTreat + (pat->getStartedTreatmentTime() - pat->getTimestepPresented());
                            //diseaseTotal[diseaseIndex].onsetToTreat = diseaseTotal[diseaseIndex].onsetToTreat + (pat->getStartedTreatmentTime() - pat->getTimestepPresented());
                            //facilityStats[diseaseIndex][count].onsetToTreat = facilityStats[diseaseIndex][count].onsetToTreat + (pat->getStartedTreatmentTime() - pat->getTimestepPresented());
                        }

                        if (pat->getStartedTreatmentTime() > 0) {
                        
                            total.countPDIT++;
                            diseaseTotal[diseaseIndex].countPDIT++;
                            facilityStats[diseaseIndex][count].countPDIT++;
                        }

                        // update time from onset to moving to inactive for patients undergoing treatment
                        total.timeInfectious = total.timeInfectious + (pat->getInfectEndTime() - pat->getTimestepCreated());
                        diseaseTotal[diseaseIndex].timeInfectious = diseaseTotal[diseaseIndex].timeInfectious + (pat->getInfectEndTime() - pat->getTimestepCreated());
                        facilityStats[diseaseIndex][count].timeInfectious = facilityStats[diseaseIndex][count].timeInfectious + (pat->getInfectEndTime() - pat->getTimestepCreated());

                        // track reason dead
                        if (pat->getReasonDied() == "NoTreatDeath") {

                            //countPDIUTPTLtoF - patients dead - infected - untreated - prediagnostic loss to follow up
                            //countPDIUTSRLtoF - patients dead - infected - sample results returned, no treatement - patient didn't start treatment
                            //countPDITLtoF - patients dead - infected - treated - patient loss to follow up after starting teatment
                            //PatInfTestLtofAssignNoShow - patient LtoF after treatment started
                            //PatInfTestLtofNoTreat - patient LtoF before treatment started
                            //PatInfPreTreatLtoF, PatInfNoTestLtofPreTreat - patient LtoF in pre-diagnostic
                            // debug - true
                            if (debug) std::cout << "stats: inside agent stats active patients dead reason = " << pat->getReasonNoTreat() << ", reason died = " << pat->getReasonDied() << "\n";
                            
                            if ((reason == "PatInfPreTreatLtoF") || (reason == "PatInfNoTestLtofPreTreat")) {
                
                                // keep track of died due to loss of follow up during treatment
                                total.countPDIUTPTLtoF++;
                                diseaseTotal[diseaseIndex].countPDIUTPTLtoF++;
                                facilityStats[diseaseIndex][count].countPDIUTPTLtoF++;
                            }
                            else if (reason == "PatInfTestLtofNoTreat") {

                                // keep track of died due to loss of follow up during treatment
                                total.countPDIUTSRLtoF++;
                                diseaseTotal[diseaseIndex].countPDIUTSRLtoF++;
                                facilityStats[diseaseIndex][count].countPDIUTSRLtoF++;
                            }
                            else if (reason == "PatInfTestLtofAssignNoShow") {

                                // keep track of died due to loss of follow up before treatment
                                total.countPDITLtoF++;
                                diseaseTotal[diseaseIndex].countPDITLtoF++;
                                facilityStats[diseaseIndex][count].countPDITLtoF++;
                            }
                        }
                    }
                    else {
                        total.countPDU++;
                        diseaseTotal[diseaseIndex].countPDU++;
                        facilityStats[diseaseIndex][count].countPDU++;
                    }
                    statusString = "PatDead";
                    if (debug) if (pat->getRelapseFlag()) std::cout << "stats: pat dead and relapsed" << "\n";
                    //if ((pat->getResultsReturnedTime() < 1) && (pat->getReasonDied() == "OldAge")) std::cout << "stats: dead patient, no results returned " << pat->getReasonNoTreat() << ", reason died = " << pat->getReasonDied() << "\n";
                    break;
                case 8:
                    total.countPC++;
                    diseaseTotal[diseaseIndex].countPC++;
                    facilityStats[diseaseIndex][count].countPC++;

                    if (debug) if (pat->getRelapseFlag()) std::cout << "stats: patient cured and relapsed" << "\n";
                    
                    // update time from onset to treatment for patients undergoing treatment
                    presentToTreatTime = pat->getStartedTreatmentTime() - pat->getTimestepPresented();

                    total.onsetToTreat = total.onsetToTreat + presentToTreatTime;
                    diseaseTotal[diseaseIndex].onsetToTreat = diseaseTotal[diseaseIndex].onsetToTreat + presentToTreatTime;
                    facilityStats[diseaseIndex][count].onsetToTreat = facilityStats[diseaseIndex][count].onsetToTreat + presentToTreatTime;

                    // update time from onset to moving to inactive for patients undergoing treatment
                    total.timeInfectious = total.timeInfectious + (pat->getInfectEndTime() - pat->getTimestepCreated());
                    diseaseTotal[diseaseIndex].timeInfectious = diseaseTotal[diseaseIndex].timeInfectious + (pat->getInfectEndTime() - pat->getTimestepCreated());
                    facilityStats[diseaseIndex][count].timeInfectious = facilityStats[diseaseIndex][count].timeInfectious + (pat->getInfectEndTime() - pat->getTimestepCreated());

                    statusString = "PatCured";
                    break;
                case 9:
                    total.countPUI++;
                    diseaseTotal[diseaseIndex].countPUI++;
                    facilityStats[diseaseIndex][count].countPUI++;
                    statusString = "PatUninfTest";
                    break;
                case 10:

                    //std::cout << "stats.patient status = 10" << "\n";
                    total.countPUINR++;
                    diseaseTotal[diseaseIndex].countPUINR++;
                    facilityStats[diseaseIndex][count].countPUINR++;
                    statusString = "PatUninfNoTest";
                    break;
                case 11:
                    total.countPUIUT++;
                    diseaseTotal[diseaseIndex].countPUIUT++;
                    facilityStats[diseaseIndex][count].countPUIUT++;
                    statusString = "PatUninfTestTreat";
                    break;
                default:
                    total.countPO++;
                    diseaseTotal[diseaseIndex].countPO++;
                    facilityStats[diseaseIndex][count].countPO++;
                    statusString = "Other";
                }

                
                // track sample length of time until returned
                if (pat->getResultsReturnedTime() > 0) {

                    tempTime = pat->getResultsReturnedTime() - pat->getCreateTime();
                    total.sampleReturnTime = total.sampleReturnTime + tempTime;
                    diseaseTotal[diseaseIndex].sampleReturnTime = diseaseTotal[diseaseIndex].sampleReturnTime + tempTime;
                    facilityStats[diseaseIndex][count].sampleReturnTime = facilityStats[diseaseIndex][count].sampleReturnTime + tempTime;
                    total.sampleReturnedCount++;
                    diseaseTotal[diseaseIndex].sampleReturnedCount++;
                    facilityStats[diseaseIndex][count].sampleReturnedCount++;

                    if (testedInHouse) {
                        
                        total.sampleReturnedInCount++;
                        diseaseTotal[diseaseIndex].sampleReturnedInCount++;
                        facilityStats[diseaseIndex][count].sampleReturnedInCount++;
                    }
                    else {

                        total.sampleReturnedOutCount++;
                        diseaseTotal[diseaseIndex].sampleReturnedOutCount++;
                        facilityStats[diseaseIndex][count].sampleReturnedOutCount++;
                    }
                }

                // track sample length of time until tested
                if (samplePtr->getTestedTime() > 0) {

                    //tempTime = pat->getResultsReturnedTime() - pat->getCreateTime();
                    //total.sampleReturnTime = total.sampleReturnTime + tempTime;
                    //diseaseTotal[diseaseIndex].sampleReturnTime = diseaseTotal[diseaseIndex].sampleReturnTime + tempTime;
                    //facilityStats[diseaseIndex][count].sampleReturnTime = facilityStats[diseaseIndex][count].sampleReturnTime + tempTime;
                    //total.sampleReturnedCount++;
                    //diseaseTotal[diseaseIndex].sampleReturnedCount++;
                    //facilityStats[diseaseIndex][count].sampleReturnedCount++;
                    //if (pat->getPreTestLtoFCount() > 0) std::cout << "stats:pre-diag ltof alive  = "  << pat->isAlive() <<  "\n";

                    if (testedInHouse) {
                        
                        total.sampleTestedInCount++;
                        diseaseTotal[diseaseIndex].sampleTestedInCount++;
                        facilityStats[diseaseIndex][count].sampleTestedInCount++;
                    }
                    else {

                        total.sampleTestedOutCount++;
                        diseaseTotal[diseaseIndex].sampleTestedOutCount++;
                        facilityStats[diseaseIndex][count].sampleTestedOutCount++;
                    }
                }
                else {

                    if (testedInHouse) {
                        
                        total.sampleNotTestedInCount++;
                        diseaseTotal[diseaseIndex].sampleNotTestedInCount++;
                        facilityStats[diseaseIndex][count].sampleNotTestedInCount++;
                    }
                    else {

                        total.sampleNotTestedOutCount++;
                        diseaseTotal[diseaseIndex].sampleNotTestedOutCount++;
                        facilityStats[diseaseIndex][count].sampleNotTestedOutCount++;
                    }
                }

                // keep track of time sample from collection to  test
                //std::cout << "stats: wait for sample = " << samplePtr->getWait() << "\n";
                if (testedInHouse) {
                
                    total.sampleWaitForResourceInTime = total.sampleWaitForResourceInTime + samplePtr->getWait();
                    diseaseTotal[diseaseIndex].sampleWaitForResourceInTime = diseaseTotal[diseaseIndex].sampleWaitForResourceInTime + samplePtr->getWait();
                    facilityStats[diseaseIndex][count].sampleWaitForResourceInTime = facilityStats[diseaseIndex][count].sampleWaitForResourceInTime + samplePtr->getWait();
                    total.sampleWaitForResourceInCount++;
                    diseaseTotal[diseaseIndex].sampleWaitForResourceInCount++;
                    facilityStats[diseaseIndex][count].sampleWaitForResourceInCount++;
                }
                else {
                
                    total.sampleWaitForResourceOutTime = total.sampleWaitForResourceOutTime + samplePtr->getWait();
                    diseaseTotal[diseaseIndex].sampleWaitForResourceOutTime = diseaseTotal[diseaseIndex].sampleWaitForResourceOutTime + samplePtr->getWait();
                    facilityStats[diseaseIndex][count].sampleWaitForResourceOutTime = facilityStats[diseaseIndex][count].sampleWaitForResourceOutTime + samplePtr->getWait();
                    total.sampleWaitForResourceOutCount++;
                    diseaseTotal[diseaseIndex].sampleWaitForResourceOutCount++;
                    facilityStats[diseaseIndex][count].sampleWaitForResourceOutCount++;
                }
                
                
                // track pos sample results 
                if (pat->getMDPos() || samplePtr->isPositive()) {

                    total.countDiagnosis++;
                    diseaseTotal[diseaseIndex].countDiagnosis++;
                    facilityStats[diseaseIndex][count].countDiagnosis++;
                    //if (!pat->getTreated()) {

                    //    std::cout << "stats:diagnosis but not teated dignosis time =  "  << samplePtr->getResultsReturnedToOrigFacilityTime() << ", started treatment = "  << pat->getStartedTreatmentTime() << "\n";
                    //}

                    if (testedInHouse) {
                        
                        total.countDiagnosisIn++;
                        diseaseTotal[diseaseIndex].countDiagnosisIn++;
                        facilityStats[diseaseIndex][count].countDiagnosisIn++;
                    }
                    else {

                        total.countDiagnosisOut++;
                        diseaseTotal[diseaseIndex].countDiagnosisOut++;
                        facilityStats[diseaseIndex][count].countDiagnosisOut++;
                    }
                }
                
                // track pos sample results 
                if (pat->getMDPos() || samplePtr->isPositive()) {

                    tempTime = pat->getResultsReturnedTime() - pat->getCreateTime(); 
                    
                    if ((pat->getResultsReturnedTime() == 0) && (pat->getCreateTime()  > 0)) {

                        // ignore those results that are near the end of year - their diagnosis time hasn't been set yet
                    }
                    else if (pat->getInfected()) {

                        // only keep track for infected patients
                        if (testedInHouse) {
                        
                            total.sampleTestPosTimeIn = total.sampleTestPosTimeIn + tempTime;
                            diseaseTotal[diseaseIndex].sampleTestPosTimeIn = diseaseTotal[diseaseIndex].sampleTestPosTimeIn + tempTime;
                            facilityStats[diseaseIndex][count].sampleTestPosTimeIn = facilityStats[diseaseIndex][count].sampleTestPosTimeIn + tempTime;
                            total.sampleTestPosCountIn++;
                            diseaseTotal[diseaseIndex].sampleTestPosCountIn++;
                            facilityStats[diseaseIndex][count].sampleTestPosCountIn++;
                        }
                        else {

                            total.sampleTestPosTimeOut = total.sampleTestPosTimeOut + tempTime;
                            diseaseTotal[diseaseIndex].sampleTestPosTimeOut = diseaseTotal[diseaseIndex].sampleTestPosTimeOut + tempTime;
                            facilityStats[diseaseIndex][count].sampleTestPosTimeOut = facilityStats[diseaseIndex][count].sampleTestPosTimeOut + tempTime;
                            total.sampleTestPosCountOut++;
                            diseaseTotal[diseaseIndex].sampleTestPosCountOut++;
                            facilityStats[diseaseIndex][count].sampleTestPosCountOut++;
                        }
                    }
                }

                // track pre-diagnostic loss to followup patients 
                if (pat->getPreTestLtoFFlag()) {

                    //std::cout << "stats:pre-diag active ltof = "  << pat->getPreTestLtoFFlag() << " alive = " << pat->isAlive() << " return time = " << pat->getFacilityReturnTime() << " infected = " << pat->getInfected() << "\n";

                    total.countPreTestLtoFOut++;
                    diseaseTotal[diseaseIndex].countPreTestLtoFOut++;
                    facilityStats[diseaseIndex][count].countPreTestLtoFOut++;

                    //if (pat->getPreTestLtoFCount() > 1) std::cout << "stats:pre-diag ltof count  = "  << pat->getPreTestLtoFCount() <<  "\n";

                    if (samplePtr->getTestedTime() > 0) {
                    
                        total.countPreTestLtoFOutTested++;
                        diseaseTotal[diseaseIndex].countPreTestLtoFOutTested++;
                        facilityStats[diseaseIndex][count].countPreTestLtoFOutTested++;
                    }
                    else {

                        total.countPreTestLtoFOutMinusTestedSamp++;
                        diseaseTotal[diseaseIndex].countPreTestLtoFOutMinusTestedSamp++;
                        facilityStats[diseaseIndex][count].countPreTestLtoFOutMinusTestedSamp++;
                    }

                    // only infected patients count as re-presenting or dying- uninfected ones just get better
                    if (pat->getInfected()) {
                        
                        if (pat->getRepresentFlag()) {
                        
                            total.countPreTestLtoFORepresent++;
                            diseaseTotal[diseaseIndex].countPreTestLtoFORepresent++;
                            facilityStats[diseaseIndex][count].countPreTestLtoFORepresent++;

                            if (samplePtr->getTestedTime() < 1) {

                                total.countPreTestLtoFOutUnTested++;
                                diseaseTotal[diseaseIndex].countPreTestLtoFOutUnTested++;
                                facilityStats[diseaseIndex][count].countPreTestLtoFOutUnTested++;
                            }
                            else {

                                total.countPreTestLtoFOutUnTested2++;
                                diseaseTotal[diseaseIndex].countPreTestLtoFOutUnTested2++;
                                facilityStats[diseaseIndex][count].countPreTestLtoFOutUnTested2++;
                            }
                        }
                        else {

                            total.countPreTestLtoFDead++;
                            diseaseTotal[diseaseIndex].countPreTestLtoFDead++;
                            facilityStats[diseaseIndex][count].countPreTestLtoFDead++;
                        }
                    }
                }
                
                // track relapse patients 
                if (pat->getRelapseFlag()) {

                    //std::cout << "stats:relapse flag pos status =  "  << status << "\n";
                    
                    total.countRetreat++;
                    diseaseTotal[diseaseIndex].countRetreat++;
                    facilityStats[diseaseIndex][count].countRetreat++;

                    if (pat->isAlive()) {
                        
                        total.countRetreatCure++;
                        diseaseTotal[diseaseIndex].countRetreatCure++;
                        facilityStats[diseaseIndex][count].countRetreatCure++;
                    }
                    else {

                        total.countRetreatDead++;
                        diseaseTotal[diseaseIndex].countRetreatDead++;
                        facilityStats[diseaseIndex][count].countRetreatDead++;
                    }
                }

                // keep track of time sample has to wait until testing - ignore patients in-house or out waiting for sample results
                //if ((status > 1) && (pat->getReasonDied() != "NoTreatDeath")) {
                
                // make sure sample has been tested from infected patients
                if ((samplePtr->getTestedTime() >=0) && (pat->getInfected())) {

                    collectTime = samplePtr->getTestedTime() - samplePtr->getCreateTime();

                    if (testedInHouse) {
                
                        total.sampleCollectToTestInTime = total.sampleCollectToTestInTime + collectTime;
                        diseaseTotal[diseaseIndex].sampleCollectToTestInTime = diseaseTotal[diseaseIndex].sampleCollectToTestInTime + collectTime;
                        facilityStats[diseaseIndex][count].sampleCollectToTestInTime = facilityStats[diseaseIndex][count].sampleCollectToTestInTime + collectTime;
                        total.sampleCollectToTestInCount++;
                        diseaseTotal[diseaseIndex].sampleCollectToTestInCount++;
                        facilityStats[diseaseIndex][count].sampleCollectToTestInCount++;
                    }
                    else {
                
                        total.sampleCollectToTestOutTime = total.sampleCollectToTestOutTime + collectTime;
                        diseaseTotal[diseaseIndex].sampleCollectToTestOutTime = diseaseTotal[diseaseIndex].sampleCollectToTestOutTime + collectTime;
                        facilityStats[diseaseIndex][count].sampleCollectToTestOutTime = facilityStats[diseaseIndex][count].sampleCollectToTestOutTime + collectTime;
                        total.sampleCollectToTestOutCount++;
                        diseaseTotal[diseaseIndex].sampleCollectToTestOutCount++;
                        facilityStats[diseaseIndex][count].sampleCollectToTestOutCount++;
                    }
                }

                // track pre-diagnostic loss to follow up for patients that returned for diagnosis
                if (pat->getRepresentFlag()) {
                    
                    total.countPLtoF++;
                    diseaseTotal[diseaseIndex].countPLtoF++;
                    facilityStats[diseaseIndex][count].countPLtoF++;
                }
                
                // track various deaths - pre-diagnostic loss to follow up, treatment, retreatment
                if ((!pat->isAlive()) && (pat->getInfected())) {

                    if (pat->getTimeOfDeath()  > 0) {

                        sampleToDeathTime = pat->getTimeOfDeath() - pat->getTimestepPresented();
                    
                        total.sampleReturnToDeathTime = total.sampleReturnToDeathTime + sampleToDeathTime;
                        diseaseTotal[diseaseIndex].sampleReturnToDeathTime = diseaseTotal[diseaseIndex].sampleReturnToDeathTime + sampleToDeathTime;
                        facilityStats[diseaseIndex][count].sampleReturnToDeathTime = facilityStats[diseaseIndex][count].sampleReturnToDeathTime + sampleToDeathTime;
                        total.sampleReturnToDeathCount++;
                        diseaseTotal[diseaseIndex].sampleReturnToDeathCount++;
                        facilityStats[diseaseIndex][count].sampleReturnToDeathCount++;

                        // pre-diagnostic loss to follow up, might include those that re-present and die in treatment
                        if (pat->getPreTestLtoFFlag()) {

                            total.sampleReturnToDeathPreDiagLtoFTime = total.sampleReturnToDeathPreDiagLtoFTime + sampleToDeathTime;
                            diseaseTotal[diseaseIndex].sampleReturnToDeathPreDiagLtoFTime = diseaseTotal[diseaseIndex].sampleReturnToDeathPreDiagLtoFTime + sampleToDeathTime;
                            facilityStats[diseaseIndex][count].sampleReturnToDeathPreDiagLtoFTime = facilityStats[diseaseIndex][count].sampleReturnToDeathPreDiagLtoFTime + sampleToDeathTime;
                            total.sampleReturnToDeathPreDiagLtoFCount++;
                            diseaseTotal[diseaseIndex].sampleReturnToDeathPreDiagLtoFCount++;
                            facilityStats[diseaseIndex][count].sampleReturnToDeathPreDiagLtoFCount++;
                        }

                        // died in treatment, either original or relapse retreatment
                        if (pat->getTimeStepsInTreatment() > 0) {

                            // retreatment
                            if (pat->getRelapseFlag()) {

                                total.sampleReturnToDeathInRetrtTime = total.sampleReturnToDeathInRetrtTime + sampleToDeathTime;
                                diseaseTotal[diseaseIndex].sampleReturnToDeathInRetrtTime = diseaseTotal[diseaseIndex].sampleReturnToDeathInRetrtTime + sampleToDeathTime;
                                facilityStats[diseaseIndex][count].sampleReturnToDeathInRetrtTime = facilityStats[diseaseIndex][count].sampleReturnToDeathInRetrtTime + sampleToDeathTime;
                                total.sampleReturnToDeathInRetrtCount++;
                                diseaseTotal[diseaseIndex].sampleReturnToDeathInRetrtCount++;
                                facilityStats[diseaseIndex][count].sampleReturnToDeathInRetrtCount++;
                            }
                            else {
                                
                                total.sampleReturnToDeathInTrtTime = total.sampleReturnToDeathInTrtTime + sampleToDeathTime;
                                diseaseTotal[diseaseIndex].sampleReturnToDeathInTrtTime = diseaseTotal[diseaseIndex].sampleReturnToDeathInTrtTime + sampleToDeathTime;
                                facilityStats[diseaseIndex][count].sampleReturnToDeathInTrtTime = facilityStats[diseaseIndex][count].sampleReturnToDeathInTrtTime + sampleToDeathTime;
                                total.sampleReturnToDeathInTrtCount++;
                                diseaseTotal[diseaseIndex].sampleReturnToDeathInTrtCount++;
                                facilityStats[diseaseIndex][count].sampleReturnToDeathInTrtCount++;
                            }
                        }
                    }
                }

                //count treated patients
                if ((pat->getInfected()) && (pat->getStartedTreatmentTime() > 0)) {

                    if ((!pat->isAlive()) && (!pat->getRelapseFlag())) {

                        total.countPTD++;
                        diseaseTotal[diseaseIndex].countPTD++;
                        facilityStats[diseaseIndex][count].countPTD++;
                    }
                    if ((pat->isAlive()) && (!pat->getRelapseFlag())) {

                        total.countPTC++;
                        diseaseTotal[diseaseIndex].countPTC++;
                        facilityStats[diseaseIndex][count].countPTC++;
                    }
                    if (pat->getRelapseFlag()) {

                        total.countPTR++;
                        diseaseTotal[diseaseIndex].countPTR++;
                        facilityStats[diseaseIndex][count].countPTR++;
                    }
                }


                // track time until exit
                if (pat->getInfected()) {

                    if ((!pat->isAlive()) || (pat->getStartedTreatmentTime() > 0)) {

                        sampleToDeathTime = pat->getInfectEndTime() - pat->getTimestepPresented();

                        total.sampleReturnToExitTime = total.sampleReturnToExitTime + sampleToDeathTime;
                        diseaseTotal[diseaseIndex].sampleReturnToExitTime = diseaseTotal[diseaseIndex].sampleReturnToExitTime + sampleToDeathTime;
                        facilityStats[diseaseIndex][count].sampleReturnToExitTime = facilityStats[diseaseIndex][count].sampleReturnToExitTime + sampleToDeathTime;
                        total.sampleReturnToExitCount++;
                        diseaseTotal[diseaseIndex].sampleReturnToExitCount++;
                        facilityStats[diseaseIndex][count].sampleReturnToExitCount++;
                    }
                }
                
                // track old age death
                if (!pat->isAlive()) {

                    if (pat->getReasonDied() == "OldAge") {

                        total.countPDOA++;
                        diseaseTotal[diseaseIndex].countPDOA++;
                        facilityStats[diseaseIndex][count].countPDOA++;
                    }
                }
                //std::cout << "stats: pos = "  << pat->getTestPos() << " infected = " << pat->getInfected() << "\n";

                if (pat->getInfected()) {

                    total.countPInf++;
                    diseaseTotal[diseaseIndex].countPInf++;
                    facilityStats[diseaseIndex][count].countPInf++;

                    if (pat->getTreated()) {

                        // update time from onset to treatment for patients undergoing treatment
                        presentToTreatTime = pat->getStartedTreatmentTime() - pat->getTimestepPresented();

                        total.countPInfTreated++;
                        diseaseTotal[diseaseIndex].countPInfTreated++;
                        facilityStats[diseaseIndex][count].countPInfTreated++;

                        if (testedInHouse) {
                    
                            total.onsetToTreatIn = total.onsetToTreatIn + presentToTreatTime;
                            diseaseTotal[diseaseIndex].onsetToTreatIn = diseaseTotal[diseaseIndex].onsetToTreatIn + presentToTreatTime;
                            facilityStats[diseaseIndex][count].onsetToTreatIn = facilityStats[diseaseIndex][count].onsetToTreatIn + presentToTreatTime;
                        
                            total.countPInfTreatedIn ++;
                            diseaseTotal[diseaseIndex].countPInfTreatedIn ++;
                            facilityStats[diseaseIndex][count].countPInfTreatedIn ++;
                        }
                        else {

                            total.onsetToTreatOut = total.onsetToTreatOut + presentToTreatTime;
                            diseaseTotal[diseaseIndex].onsetToTreatOut = diseaseTotal[diseaseIndex].onsetToTreatOut + presentToTreatTime;
                            facilityStats[diseaseIndex][count].onsetToTreatOut = facilityStats[diseaseIndex][count].onsetToTreatOut + presentToTreatTime;

                            total.countPInfTreatedOut++;
                            diseaseTotal[diseaseIndex].countPInfTreatedOut++;
                            facilityStats[diseaseIndex][count].countPInfTreatedOut++;
                        }
                    }
                }
                else {

                    if (pat->getTreated()) {
                        
                        total.countPUninfTreated++;
                        diseaseTotal[diseaseIndex].countPUninfTreated++;
                        facilityStats[diseaseIndex][count].countPUninfTreated++;
                    }
                }
                
                if (pat->getTestPos()) {
                    
                    total.countTestPos++;
                    diseaseTotal[diseaseIndex].countTestPos++;
                    facilityStats[diseaseIndex][count].countTestPos++;

                    // count true positives
                    if (pat->getInfected()) {
                        
                        total.countTruePos++;
                        diseaseTotal[diseaseIndex].countTruePos++;
                        facilityStats[diseaseIndex][count].countTruePos++;
                    }
                    else {

                       total.countFalsePos++;
                        diseaseTotal[diseaseIndex].countFalsePos++;
                        facilityStats[diseaseIndex][count].countFalsePos++; 
                    }
                }
                else {
                    
                    // if test negative - it is possible it hasn't ever been tested (LtoF death)
                    if (samplePtr->getTestedTime() >= 0) {

                        total.countTestNeg++;
                        diseaseTotal[diseaseIndex].countTestNeg++;
                        facilityStats[diseaseIndex][count].countTestNeg++;

                        // count false negatives
                        if (pat->getInfected()) {
                        
                            total.countFalseNeg++;
                            diseaseTotal[diseaseIndex].countFalseNeg++;
                            facilityStats[diseaseIndex][count].countFalseNeg++;
                        }
                        else {

                            total.countTrueNeg++;
                            diseaseTotal[diseaseIndex].countTrueNeg++;
                            facilityStats[diseaseIndex][count].countTrueNeg++;
                        }
                    }
                }

                if (pat->getMDPos()) {

                    if (pat->getInfected()) {
                        total.countMDInf++;
                        diseaseTotal[diseaseIndex].countMDInf++;
                        facilityStats[diseaseIndex][count].countMDInf++;
                    }
                    else {
                        total.countMDUninf++;
                        diseaseTotal[diseaseIndex].countMDUninf++;
                        facilityStats[diseaseIndex][count].countMDUninf++;
                    }
                }
                
                // keep track of samples waiting to be tested
                if ((!pat->getPreTestLtoFFlag()) && (samplePtr->getTestedTime() < 0)) {

                    total.sampleWaitTestCount++;
                    diseaseTotal[diseaseIndex].sampleWaitTestCount++;
                    facilityStats[diseaseIndex][count].sampleWaitTestCount++;

                    if (testedInHouse) {

                        total.sampleWaitTestCountIn++;
                        diseaseTotal[diseaseIndex].sampleWaitTestCountIn++;
                        facilityStats[diseaseIndex][count].sampleWaitTestCountIn++;
                    }
                    else {

                        total.sampleWaitTestCountOut++;
                        diseaseTotal[diseaseIndex].sampleWaitTestCountOut++;
                        facilityStats[diseaseIndex][count].sampleWaitTestCountOut++;
                    }
                }
                
                // keep track of samples have been returned but patient hasn't been notified
                if ((samplePtr->getResultsReturnedToOrigFacilityTime() > 0) && (samplePtr->getResultsReturnedTime() < 1)) {

                    //std::cout << "stats: results returned orig facility -results not returned to patient  tested time = "  << samplePtr->getTestedTime() << "\n";
                    
                    total.patientNotNotified++;
                    diseaseTotal[diseaseIndex].patientNotNotified++;
                    facilityStats[diseaseIndex][count].patientNotNotified++;

                    if (testedInHouse) {

                        total.patientNotNotifiedIn++;
                        diseaseTotal[diseaseIndex].patientNotNotifiedIn++;
                        facilityStats[diseaseIndex][count].patientNotNotifiedIn++;
                    }
                    else {

                        total.patientNotNotifiedOut++;
                        diseaseTotal[diseaseIndex].patientNotNotifiedOut++;
                        facilityStats[diseaseIndex][count].patientNotNotifiedOut++;
                    }

                    if ((pat->getTestPos()) || (pat->getMDPos())) {

                        total.patientNotNotifiedPos++;
                        diseaseTotal[diseaseIndex].patientNotNotifiedPos++;
                        facilityStats[diseaseIndex][count].patientNotNotifiedPos++;
                        
                        //dumpPatientInfo(pat, timeStep);
                        //std::cout << "patient recieved pos results returned = " << pat->getResultsReturnedTime() << ", sample tested time = " << samplePtr->getTestedTime() << ", sample returned time = " << samplePtr->getResultsReturnedTime() << ", sample returned to facility = " << samplePtr->getResultsReturnedToOrigFacilityTime() << ", samp status = " << samplePtr->getStatus() << ", pat status = " << pat->getStatus() << ", tran time = " << samplePtr->getReturnTransitTime() << "\n";
                    }
                    //std::cout << "patient recieved results = " << pat->getResultsReturnedTime() << ", sample returned time = " << samplePtr->getResultsReturnedTime() << ", sample returned to facility = " << samplePtr->getResultsReturnedToOrigFacilityTime() << "\n";
                }


                // count tests used
                testName = pat->getSampleTest();
                //std::cout << "stats: inside agent  test name = "  << testName << "\n";

                if (total.test1Name == "") {

                    // ignore counts for patients that are pre-treatment ltof and died
                    if (samplePtr->getTestedTime() > 0) {
                    
                        total.test1Name = testName;
                        diseaseTotal[diseaseIndex].test1Name = testName;
                        facilityStats[diseaseIndex][count].test1Name = testName;
                        total.test1Count++;
                        diseaseTotal[diseaseIndex].test1Count++;
                        facilityStats[diseaseIndex][count].test1Count++;

                        if (testedInHouse) {

                            total.test1CountIn++;
                            diseaseTotal[diseaseIndex].test1CountIn++;
                            facilityStats[diseaseIndex][count].test1CountIn++;
                        }
                        else {

                            // Only count negative samples that have been tested (LtoF death)
                            if (samplePtr->getTestedTime()  >= 0) {
                            
                                total.test1CountOut++;
                                diseaseTotal[diseaseIndex].test1CountOut++;
                                facilityStats[diseaseIndex][count].test1CountOut++;
                            }
                        }
                        if (samplePtr->isPositive()) {

                            total.test1PosCount++;
                            diseaseTotal[diseaseIndex].test1PosCount++;
                            facilityStats[diseaseIndex][count].test1PosCount++;

                            if (pat->getInfected()) {

                                total.test1TruePosCount++;
                                diseaseTotal[diseaseIndex].test1TruePosCount++;
                                facilityStats[diseaseIndex][count].test1TruePosCount++;
                            }
                        }
                        else {

                            // Only count negative samples that have been tested (LtoF death)
                            if (samplePtr->getTestedTime()  >= 0) {
                        
                                total.test1NegCount++;
                                diseaseTotal[diseaseIndex].test1NegCount++;
                                facilityStats[diseaseIndex][count].test1NegCount++;

                                if (!pat->getInfected()) {

                                    total.test1TrueNegCount++;
                                    diseaseTotal[diseaseIndex].test1TrueNegCount++;
                                    facilityStats[diseaseIndex][count].test1TrueNegCount++;
                                }
                            }
                        }
                    }
                }
                else if (testName == total.test1Name) {

                    // ignore counts for patients that are pre-treatment ltof and died
                    if (samplePtr->getTestedTime() > 0) {
                    
                        total.test1Count++;
                        diseaseTotal[diseaseIndex].test1Count++;
                        facilityStats[diseaseIndex][count].test1Count++;

                        if (testedInHouse) {

                            total.test1CountIn++;
                            diseaseTotal[diseaseIndex].test1CountIn++;
                            facilityStats[diseaseIndex][count].test1CountIn++;
                        }
                        else {

                            // Only count negative samples that have been tested (LtoF death)
                            if (samplePtr->getTestedTime()  >= 0) {
                            
                                total.test1CountOut++;
                                diseaseTotal[diseaseIndex].test1CountOut++;
                                facilityStats[diseaseIndex][count].test1CountOut++;
                            }
                        }
                        if (samplePtr->isPositive()) {

                            total.test1PosCount++;
                            diseaseTotal[diseaseIndex].test1PosCount++;
                            facilityStats[diseaseIndex][count].test1PosCount++;

                            if (pat->getInfected()) {

                                total.test1TruePosCount++;
                                diseaseTotal[diseaseIndex].test1TruePosCount++;
                                facilityStats[diseaseIndex][count].test1TruePosCount++;
                            }
                        }
                        else {

                            // Only count negative samples that have been tested (LtoF death)
                            if (samplePtr->getTestedTime()  >= 0) {
                        
                                total.test1NegCount++;
                                diseaseTotal[diseaseIndex].test1NegCount++;
                                facilityStats[diseaseIndex][count].test1NegCount++;

                                if (!pat->getInfected()) {

                                    total.test1TrueNegCount++;
                                    diseaseTotal[diseaseIndex].test1TrueNegCount++;
                                    facilityStats[diseaseIndex][count].test1TrueNegCount++;
                                }
                            }
                        }
                    }
                }
                else if (total.test2Name == "") {

                    // ignore counts for patients that are pre-treatment ltof and died
                    if (samplePtr->getTestedTime() > 0) {
                    
                        total.test2Name = testName;
                        diseaseTotal[diseaseIndex].test2Name = testName;
                        facilityStats[diseaseIndex][count].test2Name = testName;
                        total.test2Count++;
                        diseaseTotal[diseaseIndex].test2Count++;
                        facilityStats[diseaseIndex][count].test2Count++;

                        if (testedInHouse) {

                            total.test2CountIn++;
                            diseaseTotal[diseaseIndex].test2CountIn++;
                            facilityStats[diseaseIndex][count].test2CountIn++;
                        }
                        else {

                            // Only count negative samples that have been tested (LtoF death)
                            if (samplePtr->getTestedTime()  >= 0) {
                        
                                total.test2CountOut++;
                                diseaseTotal[diseaseIndex].test2CountOut++;
                                facilityStats[diseaseIndex][count].test2CountOut++;
                            }
                        }
                        if (samplePtr->isPositive()) {

                            total.test2PosCount++;
                            diseaseTotal[diseaseIndex].test2PosCount++;
                            facilityStats[diseaseIndex][count].test2PosCount++;

                            if (pat->getInfected()) {

                                total.test2TruePosCount++;
                                diseaseTotal[diseaseIndex].test2TruePosCount++;
                                facilityStats[diseaseIndex][count].test2TruePosCount++;
                            }
                        }
                        else {

                            // Only count negative samples that have been tested (LtoF death)
                            if (samplePtr->getTestedTime()  >= 0) {
                        
                                total.test2NegCount++;
                                diseaseTotal[diseaseIndex].test2NegCount++;
                                facilityStats[diseaseIndex][count].test2NegCount++;

                                if (!pat->getInfected()) {

                                    total.test2TrueNegCount++;
                                    diseaseTotal[diseaseIndex].test2TrueNegCount++;
                                    facilityStats[diseaseIndex][count].test2TrueNegCount++;
                                }
                            }
                        }
                    }
                }
                else if (testName == total.test2Name) {

                    // ignore counts for patients that are pre-treatment ltof and died
                    if (samplePtr->getTestedTime() > 0) {
                    
                        total.test2Count++;
                        diseaseTotal[diseaseIndex].test2Count++;
                        facilityStats[diseaseIndex][count].test2Count++;

                        if (testedInHouse) {

                            total.test2CountIn++;
                            diseaseTotal[diseaseIndex].test2CountIn++;
                            facilityStats[diseaseIndex][count].test2CountIn++;
                        }
                        else {

                            // Only count negative samples that have been tested (LtoF death)
                            if (samplePtr->getTestedTime()  >= 0) {
                            
                                total.test2CountOut++;
                                diseaseTotal[diseaseIndex].test2CountOut++;
                                facilityStats[diseaseIndex][count].test2CountOut++;
                            }
                        }
                        if (samplePtr->isPositive()) {

                            total.test2PosCount++;
                            diseaseTotal[diseaseIndex].test2PosCount++;
                            facilityStats[diseaseIndex][count].test2PosCount++;

                            if (pat->getInfected()) {

                                total.test2TruePosCount++;
                                diseaseTotal[diseaseIndex].test2TruePosCount++;
                                facilityStats[diseaseIndex][count].test2TruePosCount++;
                            }
                        }
                        else {

                            // Only count negative samples that have been tested (LtoF death)
                            if (samplePtr->getTestedTime()  >= 0) {
                        
                                total.test2NegCount++;
                                diseaseTotal[diseaseIndex].test2NegCount++;
                                facilityStats[diseaseIndex][count].test2NegCount++;

                                if (!pat->getInfected()) {

                                    total.test2TrueNegCount++;
                                    diseaseTotal[diseaseIndex].test2TrueNegCount++;
                                    facilityStats[diseaseIndex][count].test2TrueNegCount++;
                                }
                            }
                        }
                    }
                }
                else if (total.test3Name == "") {

                    // ignore counts for patients that are pre-treatment ltof and died
                    if (samplePtr->getTestedTime() > 0) {
                    
                        total.test3Name = testName;
                        diseaseTotal[diseaseIndex].test3Name = testName;
                        facilityStats[diseaseIndex][count].test3Name = testName;
                        total.test3Count++;
                        diseaseTotal[diseaseIndex].test3Count++;
                        facilityStats[diseaseIndex][count].test3Count++;
                    }
                }
                else if (testName == total.test3Name) {

                    // ignore counts for patients that are pre-treatment ltof and died
                    if (samplePtr->getTestedTime() > 0) {

                        total.test3Name = testName;
                        diseaseTotal[diseaseIndex].test3Name = testName;
                        facilityStats[diseaseIndex][count].test3Name = testName;
                        total.test3Count++;
                        diseaseTotal[diseaseIndex].test3Count++;
                        facilityStats[diseaseIndex][count].test3Count++;
                    }
                }
                else {

                    std::cout << "stats: inside agent couldn't find test name = "  << testName << "\n";
                }

                // track whether sample was sent out for analysis or tested at patient home facility 
                // if sample tested - test time is positive
                if (samplePtr->getTestedTime() >= 0) {

                    if (pat->getFacility() == pat->getSampleTestFacility()) {

                        total.inTestCount++;
                        diseaseTotal[diseaseIndex].inTestCount++;
                        facilityStats[diseaseIndex][count].inTestCount++;
                    }
                    else {

                        total.outTestCount++;
                        diseaseTotal[diseaseIndex].outTestCount++;
                        facilityStats[diseaseIndex][count].outTestCount++;
                    }
                }

                // track whether sample was sent out for analysis or tested at patient home facility 
                // track collected samples - including those from pre-treatment ltfu - which will be counted twice (they have to be alive and infected to re-present)
                if (testedInHouse) {

                    total.inCollectedCount++;
                    diseaseTotal[diseaseIndex].inCollectedCount++;
                    facilityStats[diseaseIndex][count].inCollectedCount++;
                }
                else {

                    total.outCollectedCount++;
                    diseaseTotal[diseaseIndex].outCollectedCount++;
                    facilityStats[diseaseIndex][count].outCollectedCount++;
                    
                    // update sample transit time variables for samples that were tested = even include (sampleTempTime > 0)
                    sampleTempTime = samplePtr->getTransitTimeSave();

                    total.sampleTransTime = total.sampleTransTime + sampleTempTime;
                    diseaseTotal[diseaseIndex].sampleTransTime = diseaseTotal[diseaseIndex].sampleTransTime + sampleTempTime;
                    facilityStats[diseaseIndex][count].sampleTransTime = facilityStats[diseaseIndex][count].sampleTransTime + sampleTempTime;
                    total.sampleTransCount++;
                    diseaseTotal[diseaseIndex].sampleTransCount++;
                    facilityStats[diseaseIndex][count].sampleTransCount++;            

                    // count samples collected from those who re-present after pre-test LTFU (count first lost sample, then second sample from re-presentation)
                    if (pat->getRepresentFlag()) {

                        total.outCollectedCount++;
                        diseaseTotal[diseaseIndex].outCollectedCount++;
                        facilityStats[diseaseIndex][count].outCollectedCount++;

                        total.sampleTransTime = total.sampleTransTime + sampleTempTime;
                        diseaseTotal[diseaseIndex].sampleTransTime = diseaseTotal[diseaseIndex].sampleTransTime + sampleTempTime;
                        facilityStats[diseaseIndex][count].sampleTransTime = facilityStats[diseaseIndex][count].sampleTransTime + sampleTempTime;
                        total.sampleTransCount++;
                        diseaseTotal[diseaseIndex].sampleTransCount++;
                        facilityStats[diseaseIndex][count].sampleTransCount++;
                    }
                }

                // dump statistics for each patient - only do it to file
                if (individualFlag) {

                    myfile9 << facilityStats[0][count].name << "," <<  facilityStats[0][count].id << "," <<  facilityStats[0][count].tier << "," << status << "," << pat->getDisease() << ",active" << "," << pat->getAge() << "," << pat->isAlive() << "," << pat->getInfected();
                    myfile9 << "," << pat->getTestPos() << "," << pat->getMDPos() << "," << pat->getTimeFromOnset() << "," << pat->getTimeStepsInTreatment() << "," << pat->getTimeStepsNotInTreatment();
                    myfile9 << "," << (pat->getStartedTreatmentTime() - pat->getTimestepPresented()) << "," << pat->getTimestepPresented() << "\n";

                    myfile10 << facilityStats[0][count].name << "," << facilityStats[0][count].district << "," << facilityStats[0][count].region << "," << facilityStats[0][count].id << "," <<  facilityStats[0][count].tier;
                    myfile10 << "," << status << "," << statusString << "," << pat->getPrevStatus() << "," << pat->getDisease() << ",active" << "," << pat->getAge() << "," << pat->isAlive() << "," << pat->getInfected();

                    if ((status == PatInfTestLtofNoTreat) || (status == PatInfNoTestNoTreat)) {
                    
                        myfile10 << "," << "NA" << "," << "NA";
                    }
                    else if (pat->getTestPos()) {
                        myfile10 << "," << "TRUE" << "," << "FALSE";
                    }
                    else {
                        myfile10 << "," << "FALSE" << "," << "TRUE";
                    }
                   if ((status == PatInfTestLtofNoTreat) || (status == PatInfNoTestNoTreat)) {
                    
                        myfile10 << "," << "NA";
                   }
                   else if (pat->getMDPos()) {
                        myfile10 << "," << "TRUE";
                   }
                   else {
                        myfile10 << "," << "FALSE";
                   }

                   myfile10 << "," << pat->getTimestepPresented() << "," << pat->getCreateTime() << "," << samplePtr->getStatus() << "," << sampleStatusString << "," << samplePtr->getPrevStatus();
                   myfile10 << "," << pat->getResultsReturnedTime();
                   if (pat->getResultsReturnedTime() > 0) {
                        
                        myfile10 << "," << (pat->getResultsReturnedTime() - pat->getTimestepPresented());
                   }
                   else { 
                        
                        myfile10 << "," << "0";
                   }
                   myfile10 << "," << pat->getPatientRecievedMDResultTime() << "," << pat->getStartedTreatmentTime();
                   if (pat->getStartedTreatmentTime() > 0) {
                        
                        myfile10 << "," << (pat->getStartedTreatmentTime() - pat->getTimestepPresented());
                   }
                   else { 
                        
                        myfile10 << "," << "0";
                   }
                   myfile10 << "," << pat->getTimeStepsInTreatment() << "," << pat->getTimeStepsNotInTreatment();
                   myfile10 << "," << (pat->getStartedTreatmentTime() - pat->getTimestepPresented()) << "," << pat->getTimePatientLtoF() << "," << pat->getNoReturnsForResults() << "," << samplePtr->getTransitTimeSave() << "," << samplePtr->getReturnTransitTimeSave();
                   myfile10 << "," << samplePtr->getTestedTime() << "," << pat->getTimestepPresented() << "," << pat->getSampleTest();
                   myfile10 << "," << samplePtr->getTestingFacility() << "," << samplePtr->getTestingDistance() << "\n";

                   myfile1 << facilityStats[0][count].name << "," <<  facilityStats[0][count].id << "," <<  facilityStats[0][count].tier << "," << status << "," << pat->getDisease() << ",active" << "," << pat->getAge() << "," << pat->isAlive() << "," << pat->getInfected();
                   myfile1 << "," << pat->getTestPos() << "," << pat->getMDPos() << "," << pat->getTimeFromOnset() << "," << pat->getTimeStepsInTreatment() << "," << pat->getTimeStepsNotInTreatment();
                   myfile1 << "," << (pat->getStartedTreatmentTime() - pat->getTimestepPresented()) << "," << pat->getTimestepPresented() << "\n";
                }

                if (individualSamplingFlag) {
                    
                    // patient raw sampling data at sampling rate
                    if (noTotalPatients % patientSamplingRate == 0) {
                    
                        myfile12 << facilityStats[0][count].name << "," << facilityStats[0][count].district << "," << facilityStats[0][count].region << "," << facilityStats[0][count].id << "," <<  facilityStats[0][count].tier;
                        myfile12 << "," << status << "," << statusString << "," << pat->getPrevStatus() << "," << pat->getDisease() << ",active" << "," << pat->getAge() << "," << pat->isAlive() << "," << pat->getInfected();

                        if ((status == PatInfTestLtofNoTreat) || (status == PatInfNoTestNoTreat)) {
                    
                            myfile12 << "," << "NA" << "," << "NA";
                        }
                        else if (pat->getTestPos()) {
                            myfile12 << "," << "TRUE" << "," << "FALSE";
                        }
                        else {
                            myfile12 << "," << "FALSE" << "," << "TRUE";
                        }
                        if ((status == PatInfTestLtofNoTreat) || (status == PatInfNoTestNoTreat)) {
                    
                            myfile12 << "," << "NA";
                        }
                        else if (pat->getMDPos()) {
                            myfile12 << "," << "TRUE";
                        }
                        else {
                            myfile12 << "," << "FALSE";
                        }

                        myfile12 << "," << pat->getTimestepPresented() << "," << pat->getCreateTime() << "," << samplePtr->getStatus() << "," << sampleStatusString << "," << samplePtr->getPrevStatus();
                        myfile12 << "," << pat->getResultsReturnedTime();
                        if (pat->getResultsReturnedTime() > 0) {
                        
                            myfile12 << "," << (pat->getResultsReturnedTime() - pat->getTimestepPresented());
                        }
                        else { 
                        
                            myfile12 << "," << "0";
                        }
                        myfile12 << "," << pat->getPatientRecievedMDResultTime() << "," << pat->getStartedTreatmentTime();
                        if (pat->getStartedTreatmentTime() > 0) {
                        
                            myfile12 << "," << (pat->getStartedTreatmentTime() - pat->getTimestepPresented());
                        }
                        else { 
                        
                            myfile12 << "," << "0";
                        }
                        myfile12 << "," << pat->getTimeStepsInTreatment() << "," << pat->getTimeStepsNotInTreatment();
                        myfile12 << "," << (pat->getStartedTreatmentTime() - pat->getTimestepPresented()) << "," << pat->getTimePatientLtoF() << "," << samplePtr->getTransitTimeSave() << "," << samplePtr->getReturnTransitTimeSave();
                        myfile12 << "," << samplePtr->getTestedTime() << "," << samplePtr->getResultsReturnedToOrigFacilityTime() << "," << pat->getSampleTest();
                        myfile12 << "," << samplePtr->getTestingFacility() << "," << samplePtr->getTestingDistance();
                        myfile12 << "," << noTotalPatients << "\n";
                    }
                }
                
                if ((individualFlag) || (individualSamplingFlag)) {
                
                    // update totals for individual patients
                    indNoPatients[diseaseIndex]++;
                    indAge[diseaseIndex] = indAge[diseaseIndex] + pat->getAge();
                    if (pat->isAlive()) indNoAlive[diseaseIndex]++;
                    if (pat->getInfected()) {
                        indNoInfected[diseaseIndex]++;
                        indTotalTimeFromOnset[diseaseIndex] = indTotalTimeFromOnset[diseaseIndex] + pat->getTimeFromOnset();
                        indTotalTimeInfectious[diseaseIndex] = indTotalTimeInfectious[diseaseIndex] + pat->getTimeFromOnset();
                        indTotaltimeInTreatment[diseaseIndex] = indTotaltimeInTreatment[diseaseIndex] + pat->getTimeStepsInTreatment();
                    }
                    else {
                        indTotalTimeNotInTreatment[diseaseIndex] = indTotalTimeNotInTreatment[diseaseIndex] + pat->getTimeStepsNotInTreatment();
                    }
                    if (pat->getTestPos()) indNoTestPos[diseaseIndex]++;
                    if (pat->getMDPos()) indNoMDOverride[diseaseIndex]++;
                    indDisease[diseaseIndex] = pat->getDisease();
                    indTotalTimeFacilityReturnTime[diseaseIndex] = indTotalTimeFacilityReturnTime[diseaseIndex] + pat->getFacilityReturnTime();
                    if (pat->getResultsReturnedTime() > 0) {

                        indTotalTimeResultsReturned[diseaseIndex] = indTotalTimeResultsReturned[diseaseIndex] + (pat->getResultsReturnedTime() - pat->getCreateTime());
                        indTotalNoOfReturnVisits[diseaseIndex] = indTotalNoOfReturnVisits[diseaseIndex] + pat->getNoReturnsForResults();
                    }
                }

                // count dead people from old age
                // age = pat->getAge();
                //if (!pat->isAlive()) {
                //    countPD++;
                //}
            }
        }

        if (debug) std::cout << "stats: inside agent stats before inactive patient loop"  << "\n";

        // dump out no. of untreated dead at this time
        //std::cout << "debugCounter (untreated dead) = " << debugCounter << "\n";

        // if for some reason you don't want to include the inactive patients, set this to false
        if (true) {

            if (debug) std::cout << "stats: inside agent stats before patient loop no. of inactive patients = " << pI.size() << "\n";

            // now loop through the inactive patients
            for (auto pat : pI) {

                // find sample associated with Patient - it is either here or at testing Facility
                // check here first
                samplePtr = pat->getSamplePtr();

                // couldn't find it in the active or inactive containers
                if (samplePtr == nullptr) {

                    std::cout << "stats: couldn't find sample for inactive patient  status = " << pat->getStatus() << " for facility = " << pat->getFacility();
                    std::cout << " testing facility = " << pat->getSampleTestFacility() << " getUpName() = " << facility->getUpName() << "\n";
                }
                else {

                    //debug - true -false
                    if (debug)  {
                        std::cout << "stats: inside agent statsafter get sample ptr sample transit time = " << samplePtr->getTransitTimeSave() << "\n";
                    }
            
                    sampleStatusString = "unknown";

                    switch(samplePtr->getStatus()) {
                        
                        case 0:
                        
                            sampleStatusString = "SampCreatedUnTest -created but untested";
                            break;
                        case 1:
                        
                            sampleStatusString = "SampCreatedUnTestBatch - created untested and waiting for batch transit";
                            break;
                        case 2:
                        
                            sampleStatusString = "SampUnTestInTransit - in transit";
                            break;
                        case 3:
                        
                            sampleStatusString = "SampWaitForTest - waiting for test";
                            break;
                        case 4:
                        
                            sampleStatusString = "SampLostExpired - lost/expired";
                            break;
                        case 5:
                        
                            sampleStatusString = "SampTestedInTransit - results in transit back to orig facility";
                            break;
                        case 6:
                        
                            sampleStatusString = "SampReturnedAvail - results returned - available";
                            break;
                        case 7:
                        
                            sampleStatusString = "SampReturnedLostExpired - lost/expired and returned";
                            break;
                        case 8:
                        
                            sampleStatusString = "SampLtoF - loss to follow up";
                            break;
                        case 9:
                        
                            sampleStatusString = "SampPatDead - patient dead";
                            break;
                        default:
                        
                            sampleStatusString = "unknown";
                    }

                     // mark whether sample was tested in House
                    if (samplePtr->getOrigFacility()  ==  samplePtr->getTestingFacility()) {
                    
                        testedInHouse = true;
                    }
                    else {

                        testedInHouse = false;
                    }
                }
              
            
                // get which year patient is created
                patientCreated = pat->getTimestepPresented();
            
                // if only doing yearly output, skip if not in current year
                if (yearly && ((patientCreated < lowerBound) || (patientCreated > upperBound))) {
               
                    // skip patient if yearly output and not in this year
                }
                else {
            
                    // count inactive and total patients
                    noPatientInactive++;
                    noTotalPatients++;

                    // store stats by disease
                    diseaseName = pat->getDisease();

                    // get the index for the disease
                    diseaseIndex = 0;

                    for (int w = 0; w < diseaseCount; w++) {

                        if (diseaseName == diseaseList[w]) {
                            diseaseIndex = w;

                            // increase the sample count for this disease for this facility
                            patientCount[w]++;
                        }
                    }

                    // 0 - away from facility - hasn't returned for results
                    // 1 - in-house
                    // 2 - sample results returned, undergoing treatment
                    // 3 - sample results returned, treatment failure
                    // 4 - sample results returned, no treatment
                    // 5 - no sample results returned, no treatment
                    // 6 - sample results returned, no treatment -loss to follow up
                    // 7 - patient dead - PatDead 
                    // 8 - patient cured
                    // 9 - patient uninfected - returned sample
                    // 10 - patient uninfected - didn't return for sample
                    // 11 - patient uninfected - under treatment
                    status = pat->getStatus();

                    switch (status) {
                    case 0:
                        if (testedInHouse) {
                    
                            //std::cout << "patient testing out but patient seems in status = " << status << "\n";
                        }
                        else {
                    
                            total.countPU++;
                            diseaseTotal[diseaseIndex].countPU++;
                            facilityStats[diseaseIndex][count].countPU++;
                        }
                        if (individualFlag) {
                            myfile4 << "stats.patient.ptr_" << pat << ",facility_" << pat->getFacility() << "\n";
                        }
                        statusString = "PatAwayUnTest";
                        break;
                    case 1:
                        if (testedInHouse) {
                        
                            total.countPI++;
                            diseaseTotal[diseaseIndex].countPI++;
                            facilityStats[diseaseIndex][count].countPI++;
                            }
                        else {

                            //std::cout << "patient inhouse but testing out status = " << status << "\n";
                        }
                        if (individualFlag) {
                            myfile5 << "stats.patient.ptr_" << pat << ",facility_" << pat->getFacility() << "\n";
                        }
                        statusString = "PatInUnTest";
                        break;
                    case 2:
                        total.countPT++;
                        diseaseTotal[diseaseIndex].countPT++;
                        facilityStats[diseaseIndex][count].countPT++;

                        // update time from onset to treatment for patients undergoing treatment
                        presentToTreatTime = pat->getStartedTreatmentTime() - pat->getTimestepPresented();

                        total.onsetToTreat = total.onsetToTreat + presentToTreatTime;
                        diseaseTotal[diseaseIndex].onsetToTreat = diseaseTotal[diseaseIndex].onsetToTreat + presentToTreatTime;
                        facilityStats[diseaseIndex][count].onsetToTreat = facilityStats[diseaseIndex][count].onsetToTreat + presentToTreatTime;
                        if (debug) std::cout << "stats: inside agent stats patients onset til treatment= " << presentToTreatTime << "\n";

                        // update time from onset to moving to inactive for patients undergoing treatment
                        total.timeInfectious = total.timeInfectious + (pat->getInfectEndTime() - pat->getTimestepCreated());
                        diseaseTotal[diseaseIndex].timeInfectious = diseaseTotal[diseaseIndex].timeInfectious + (pat->getInfectEndTime() - pat->getTimestepCreated());
                        facilityStats[diseaseIndex][count].timeInfectious = facilityStats[diseaseIndex][count].timeInfectious + (pat->getInfectEndTime() - pat->getTimestepCreated());

                        statusString = "PatInfTestTreat";
                        break;
                    case 3:
                        total.countPR++;
                        diseaseTotal[diseaseIndex].countPR++;
                        facilityStats[diseaseIndex][count].countPR++;

                        // update time from onset to treatment for patients undergoing treatment
                        //total.onsetToTreat = total.onsetToTreat + (pat->getStartedTreatmentTime() - pat->getTimestepPresented());
                        //diseaseTotal[diseaseIndex].onsetToTreat = diseaseTotal[diseaseIndex].onsetToTreat + (pat->getStartedTreatmentTime() - pat->getTimestepPresented());
                        //facilityStats[diseaseIndex][count].onsetToTreat = facilityStats[diseaseIndex][count].onsetToTreat + (pat->getStartedTreatmentTime() - pat->getTimestepPresented());
                        if (debug) std::cout << "stats: inside agent stats patients onset til treatment= " << (pat->getStartedTreatmentTime() - pat->getTimestepPresented()) << "\n";

                        // update time from onset to moving to inactive for patients undergoing treatment
                        total.timeInfectious = total.timeInfectious + (pat->getInfectEndTime() - pat->getTimestepCreated());
                        diseaseTotal[diseaseIndex].timeInfectious = diseaseTotal[diseaseIndex].timeInfectious + (pat->getInfectEndTime() - pat->getTimestepCreated());
                        facilityStats[diseaseIndex][count].timeInfectious = facilityStats[diseaseIndex][count].timeInfectious + (pat->getInfectEndTime() - pat->getTimestepCreated());

                        statusString = "PatInfTestRelapse";
                        break;
                    case 4:
                        total.countPUT++;
                        diseaseTotal[diseaseIndex].countPUT++;
                        facilityStats[diseaseIndex][count].countPUT++;

                        total.countPUTFN++;
                        diseaseTotal[diseaseIndex].countPUTFN++;
                        facilityStats[diseaseIndex][count].countPUTFN++;

                        // update time from onset to moving to inactive for patients undergoing treatment
                        total.timeInfectious = total.timeInfectious + (pat->getInfectEndTime() - pat->getTimestepCreated());
                        diseaseTotal[diseaseIndex].timeInfectious = diseaseTotal[diseaseIndex].timeInfectious + (pat->getInfectEndTime() - pat->getTimestepCreated());
                        facilityStats[diseaseIndex][count].timeInfectious = facilityStats[diseaseIndex][count].timeInfectious + (pat->getInfectEndTime() - pat->getTimestepCreated());

                        statusString = "PatInfTestNoTreat";
                        break;
                    case 5:

                        //std::cout << "stats.patient status = 5" << "\n";
                        total.countPUT++;
                        diseaseTotal[diseaseIndex].countPUT++;
                        facilityStats[diseaseIndex][count].countPUT++;

                        total.countPUTNS++;
                        diseaseTotal[diseaseIndex].countPUTNS++;
                        facilityStats[diseaseIndex][count].countPUTNS++;

                        // update time from onset to moving to inactive for patients undergoing treatment
                        total.timeInfectious = total.timeInfectious + (pat->getInfectEndTime() - pat->getTimestepCreated());
                        diseaseTotal[diseaseIndex].timeInfectious = diseaseTotal[diseaseIndex].timeInfectious + (pat->getInfectEndTime() - pat->getTimestepCreated());
                        facilityStats[diseaseIndex][count].timeInfectious = facilityStats[diseaseIndex][count].timeInfectious + (pat->getInfectEndTime() - pat->getTimestepCreated());

                        statusString = "PatInfNoTestNoTreat";
                        break;
                    case 6:
                        total.countPUT++;
                        diseaseTotal[diseaseIndex].countPUT++;
                        facilityStats[diseaseIndex][count].countPUT++;

                        total.countPUTLtoF++;
                        diseaseTotal[diseaseIndex].countPUTLtoF++;
                        facilityStats[diseaseIndex][count].countPUTLtoF++;

                        // update time from onset to moving to inactive for patients undergoing treatment
                        total.timeInfectious = total.timeInfectious + (pat->getInfectEndTime() - pat->getTimestepCreated());
                        diseaseTotal[diseaseIndex].timeInfectious = diseaseTotal[diseaseIndex].timeInfectious + (pat->getInfectEndTime() - pat->getTimestepCreated());
                        facilityStats[diseaseIndex][count].timeInfectious = facilityStats[diseaseIndex][count].timeInfectious + (pat->getInfectEndTime() - pat->getTimestepCreated());

                        statusString = "PatInfTestLtofNoTreat";
                        break;
                    case 7:
                        if (debug) std::cout << "stats: inside agent stats active patients dead infected = " << pat->getInfected() << " reason = " << pat->getReasonNoTreat() << "\n";
                        
                        if (pat->getInfected()) {
                            
                            // debug - true
                            if (debug) std::cout << "stats: inside agent stats inactive patients dead reason = " << pat->getReasonNoTreat() << ", reason died = " << pat->getReasonDied() << "\n";
                            
                            // track untreated dead and treated dead
                            reason = pat->getReasonNoTreat();
                            if (reason == "PatInfTestNoTreat") {
                        
                                total.countPDIUTSR++;
                                diseaseTotal[diseaseIndex].countPDIUTSR++;
                                facilityStats[diseaseIndex][count].countPDIUTSR++;
                                total.countPDIUT++;
                                diseaseTotal[diseaseIndex].countPDIUT++;
                                facilityStats[diseaseIndex][count].countPDIUT++;
                            }
                            else if ((reason == "PatInfNoTestNoTreat") || (reason == "PatInfNoTestLtofPreTreat")) {
                        
                                total.countPDIUTSNR++;
                                diseaseTotal[diseaseIndex].countPDIUTSNR++;
                                facilityStats[diseaseIndex][count].countPDIUTSNR++;
                                total.countPDIUT++;
                                diseaseTotal[diseaseIndex].countPDIUT++;
                                facilityStats[diseaseIndex][count].countPDIUT++;
                            }
                            else if (reason == "PatInfTestLtofNoTreat") {
                        
                                total.countPDIUTLtF++;
                                diseaseTotal[diseaseIndex].countPDIUTLtF++;
                                facilityStats[diseaseIndex][count].countPDIUTLtF++;
                                total.countPDIUT++;
                                diseaseTotal[diseaseIndex].countPDIUT++;
                                facilityStats[diseaseIndex][count].countPDIUT++;
                            }
                            else if (reason == "PatInfNoTestLtofPreTreat") {
                        
                                total.countPDIUT++;
                                diseaseTotal[diseaseIndex].countPDIUT++;
                                facilityStats[diseaseIndex][count].countPDIUT++;
                            }
                            else if (reason == "PatInfTestLtofAssignNoShow") {
                        
                                total.countPDIUT++;
                                diseaseTotal[diseaseIndex].countPDIUT++;
                                facilityStats[diseaseIndex][count].countPDIUT++;
                            }
                            else if (reason == "PatInfPreTreatLtoF") {
                        
                                total.countPDIUT++;
                                diseaseTotal[diseaseIndex].countPDIUT++;
                                facilityStats[diseaseIndex][count].countPDIUT++;
                            }
                            else {
                            
                                total.countPDI++;
                                diseaseTotal[diseaseIndex].countPDI++;
                                facilityStats[diseaseIndex][count].countPDI++;

                                // update time from onset to treatment for patients undergoing treatment
                                //total.onsetToTreat = total.onsetToTreat + (pat->getStartedTreatmentTime() - pat->getTimestepPresented());
                                //diseaseTotal[diseaseIndex].onsetToTreat = diseaseTotal[diseaseIndex].onsetToTreat + (pat->getStartedTreatmentTime() - pat->getTimestepPresented());
                                //facilityStats[diseaseIndex][count].onsetToTreat = facilityStats[diseaseIndex][count].onsetToTreat + (pat->getStartedTreatmentTime() - pat->getTimestepPresented());
                            }

                            // dead - started treatment
                            if (pat->getStartedTreatmentTime() > 0) {
                        
                                total.countPDIT++;
                                diseaseTotal[diseaseIndex].countPDIT++;
                                facilityStats[diseaseIndex][count].countPDIT++;
                            }

                            // update time from onset to moving to inactive for patients undergoing treatment
                            total.timeInfectious = total.timeInfectious + (pat->getInfectEndTime() - pat->getTimestepCreated());
                            diseaseTotal[diseaseIndex].timeInfectious = diseaseTotal[diseaseIndex].timeInfectious + (pat->getInfectEndTime() - pat->getTimestepCreated());
                            facilityStats[diseaseIndex][count].timeInfectious = facilityStats[diseaseIndex][count].timeInfectious + (pat->getInfectEndTime() - pat->getTimestepCreated());

                            // track reason dead
                            if (pat->getReasonDied() == "NoTreatDeath") {

                                //countPDIUTPTLtoF - patients dead - infected - untreated - prediagnostic loss to follow up
                                //countPDIUTSRLtoF - patients dead - infected - sample results returned, no treatement - patient didn't start treatment
                                //countPDITLtoF - patients dead - infected - treated - patient loss to follow up after starting teatment
                                //PatInfTestLtofAssignNoShow - patient LtoF after treatment started
                                //PatInfTestLtofNoTreat - patient LtoF before treatment started
                                //PatInfPreTreatLtoF, PatInfNoTestLtofPreTreat - patient LtoF in pre-diagnostic
                            
                                // debug - true
                                if (debug) std::cout << "stats: inside agent stats inactive patients dead reason = " << pat->getReasonNoTreat() << ", reason died = " << pat->getReasonDied() << "\n";
                            
                                if ((reason == "PatInfPreTreatLtoF") || (reason == "PatInfNoTestLtofPreTreat")) {
                                    if (debug) std::cout << "stats: inside agent stats prediagnostic dead reason = " << pat->getReasonNoTreat() << ", reason died = " << pat->getReasonDied() << "\n";
                            
                                    // keep track of died due to loss of follow up during treatment
                                    total.countPDIUTPTLtoF++;
                                    diseaseTotal[diseaseIndex].countPDIUTPTLtoF++;
                                    facilityStats[diseaseIndex][count].countPDIUTPTLtoF++;
                                }
                                else if (reason == "PatInfTestLtofNoTreat") {

                                    // keep track of died due to loss of follow up during treatment
                                    total.countPDIUTSRLtoF++;
                                    diseaseTotal[diseaseIndex].countPDIUTSRLtoF++;
                                    facilityStats[diseaseIndex][count].countPDIUTSRLtoF++;
                                }
                                else if (reason == "PatInfTestLtofAssignNoShow") {

                                    // keep track of died due to loss of follow up before treatment
                                    total.countPDITLtoF++;
                                    diseaseTotal[diseaseIndex].countPDITLtoF++;
                                    facilityStats[diseaseIndex][count].countPDITLtoF++;
                                }
                            }
                        }
                        else {
                            total.countPDU++;
                            diseaseTotal[diseaseIndex].countPDU++;
                            facilityStats[diseaseIndex][count].countPDU++;
                        }

                        statusString = "PatDead";
                        if (debug) if (pat->getRelapseFlag()) std::cout << "stats: pat dead and relapsed" << "\n";
                        //if ((pat->getResultsReturnedTime() < 1) && (pat->getReasonDied() == "OldAge")) std::cout << "stats: dead patient, no results returned " << pat->getReasonNoTreat() << ", reason died = " << pat->getReasonDied() << "\n";
                        break;
                    case 8:
                        total.countPC++;
                        diseaseTotal[diseaseIndex].countPC++;
                        facilityStats[diseaseIndex][count].countPC++;

                        if (debug) if (pat->getRelapseFlag()) std::cout << "stats: patient cured and relapsed" << "\n";
                        
                        // update time from onset to treatment for patients undergoing treatment
                        presentToTreatTime = pat->getStartedTreatmentTime() - pat->getTimestepPresented();

                        total.onsetToTreat = total.onsetToTreat + presentToTreatTime;
                        diseaseTotal[diseaseIndex].onsetToTreat = diseaseTotal[diseaseIndex].onsetToTreat + presentToTreatTime;
                        facilityStats[diseaseIndex][count].onsetToTreat = facilityStats[diseaseIndex][count].onsetToTreat + presentToTreatTime;
                        if (debug) std::cout << "stats: inside agent stats patients onset til treatment= " << presentToTreatTime << "\n";

                        // update time from onset to moving to inactive for patients undergoing treatment
                        total.timeInfectious = total.timeInfectious + (pat->getInfectEndTime() - pat->getTimestepCreated());
                        diseaseTotal[diseaseIndex].timeInfectious = diseaseTotal[diseaseIndex].timeInfectious + (pat->getInfectEndTime() - pat->getTimestepCreated());
                        facilityStats[diseaseIndex][count].timeInfectious = facilityStats[diseaseIndex][count].timeInfectious + (pat->getInfectEndTime() - pat->getTimestepCreated());

                        statusString = "PatCured";
                        break;
                    case 9:
                        total.countPUI++;
                        diseaseTotal[diseaseIndex].countPUI++;
                        facilityStats[diseaseIndex][count].countPUI++;
                        statusString = "PatUninfTest";
                        break;
                    case 10:

                        //std::cout << "stats.patient status = 10" << "\n";
                        total.countPUINR++;
                        diseaseTotal[diseaseIndex].countPUINR++;
                        facilityStats[diseaseIndex][count].countPUINR++;
                        statusString = "PatUninfNoTest";
                        break;
                    case 11:
                        total.countPUIUT++;
                        diseaseTotal[diseaseIndex].countPUIUT++;
                        facilityStats[diseaseIndex][count].countPUIUT++;
                        statusString = "PatUninfTestTreat";
                        break;
                    default:
                        total.countPO++;
                        diseaseTotal[diseaseIndex].countPO++;
                        facilityStats[diseaseIndex][count].countPO++;
                        statusString = "Other";

                    }

                    
                    // track sample length of time until returned
                    if (pat->getResultsReturnedTime() > 0) {

                        tempTime = pat->getResultsReturnedTime() - pat->getCreateTime();
                        total.sampleReturnTime = total.sampleReturnTime + tempTime;
                        diseaseTotal[diseaseIndex].sampleReturnTime = diseaseTotal[diseaseIndex].sampleReturnTime + tempTime;
                        facilityStats[diseaseIndex][count].sampleReturnTime = facilityStats[diseaseIndex][count].sampleReturnTime + tempTime;
                        total.sampleReturnedCount++;
                        diseaseTotal[diseaseIndex].sampleReturnedCount++;
                        facilityStats[diseaseIndex][count].sampleReturnedCount++;

                        if (testedInHouse) {
                        
                            total.sampleReturnedInCount++;
                            diseaseTotal[diseaseIndex].sampleReturnedInCount++;
                            facilityStats[diseaseIndex][count].sampleReturnedInCount++;
                        }
                        else {

                            total.sampleReturnedOutCount++;
                            diseaseTotal[diseaseIndex].sampleReturnedOutCount++;
                            facilityStats[diseaseIndex][count].sampleReturnedOutCount++;
                        }
                    }

                    // track sample length of time until tested
                    if (samplePtr->getTestedTime() > 0) {

                        //tempTime = pat->getResultsReturnedTime() - pat->getCreateTime();
                        //total.sampleReturnTime = total.sampleReturnTime + tempTime;
                        //diseaseTotal[diseaseIndex].sampleReturnTime = diseaseTotal[diseaseIndex].sampleReturnTime + tempTime;
                        //facilityStats[diseaseIndex][count].sampleReturnTime = facilityStats[diseaseIndex][count].sampleReturnTime + tempTime;
                        //total.sampleReturnedCount++;
                        //diseaseTotal[diseaseIndex].sampleReturnedCount++;
                        //facilityStats[diseaseIndex][count].sampleReturnedCount++;
                        //if (pat->getPreTestLtoFCount() > 0) std::cout << "stats:pre-diag ltof alive  = "  << pat->isAlive() <<  "\n";

                        if (testedInHouse) {
                        
                            total.sampleTestedInCount++;
                            diseaseTotal[diseaseIndex].sampleTestedInCount++;
                            facilityStats[diseaseIndex][count].sampleTestedInCount++;
                        }
                        else {

                            total.sampleTestedOutCount++;
                            diseaseTotal[diseaseIndex].sampleTestedOutCount++;
                            facilityStats[diseaseIndex][count].sampleTestedOutCount++;
                        }
                    }
                    else {

                        if (testedInHouse) {
                        
                            total.sampleNotTestedInCount++;
                            diseaseTotal[diseaseIndex].sampleNotTestedInCount++;
                            facilityStats[diseaseIndex][count].sampleNotTestedInCount++;
                        }
                        else {

                            total.sampleNotTestedOutCount++;
                            diseaseTotal[diseaseIndex].sampleNotTestedOutCount++;
                            facilityStats[diseaseIndex][count].sampleNotTestedOutCount++;
                        }
                    }

                    //std::cout << "stats: wait for sample = " << samplePtr->getWait() << "\n";
                    
                    // keep track of time sample has to wait for resources
                    if (testedInHouse) {
                
                        total.sampleWaitForResourceInTime = total.sampleWaitForResourceInTime + samplePtr->getWait();
                        diseaseTotal[diseaseIndex].sampleWaitForResourceInTime = diseaseTotal[diseaseIndex].sampleWaitForResourceInTime + samplePtr->getWait();
                        facilityStats[diseaseIndex][count].sampleWaitForResourceInTime = facilityStats[diseaseIndex][count].sampleWaitForResourceInTime + samplePtr->getWait();
                        total.sampleWaitForResourceInCount++;
                        diseaseTotal[diseaseIndex].sampleWaitForResourceInCount++;
                        facilityStats[diseaseIndex][count].sampleWaitForResourceInCount++;
                    }
                    else {
                
                        total.sampleWaitForResourceOutTime = total.sampleWaitForResourceOutTime + samplePtr->getWait();
                        diseaseTotal[diseaseIndex].sampleWaitForResourceOutTime = diseaseTotal[diseaseIndex].sampleWaitForResourceOutTime + samplePtr->getWait();
                        facilityStats[diseaseIndex][count].sampleWaitForResourceOutTime = facilityStats[diseaseIndex][count].sampleWaitForResourceOutTime + samplePtr->getWait();
                        total.sampleWaitForResourceOutCount++;
                        diseaseTotal[diseaseIndex].sampleWaitForResourceOutCount++;
                        facilityStats[diseaseIndex][count].sampleWaitForResourceOutCount++;
                    }
                
                    // track pos sample results 
                    if (pat->getMDPos() || samplePtr->isPositive()) {

                        total.countDiagnosis++;
                        diseaseTotal[diseaseIndex].countDiagnosis++;
                        facilityStats[diseaseIndex][count].countDiagnosis++;
                        //if (!pat->getTreated()) {

                        //    std::cout << "stats:diagnosis but not teated dignosis time =  "  << samplePtr->getResultsReturnedToOrigFacilityTime() << ", started treatment = "  << pat->getStartedTreatmentTime() << "\n";
                        //}

                        if (testedInHouse) {
                        
                            total.countDiagnosisIn++;
                            diseaseTotal[diseaseIndex].countDiagnosisIn++;
                            facilityStats[diseaseIndex][count].countDiagnosisIn++;
                        }
                        else {

                            total.countDiagnosisOut++;
                            diseaseTotal[diseaseIndex].countDiagnosisOut++;
                            facilityStats[diseaseIndex][count].countDiagnosisOut++;
                        }
                    }

                    // track pos sample results 
                    if (pat->getMDPos() || samplePtr->isPositive()) {

                        tempTime = pat->getResultsReturnedTime() - pat->getCreateTime(); 
                        
                        // ignore those results that are near the end of year - thier diagnosis time hasn't been set yet
                        if ((pat->getResultsReturnedTime() == 0) && (pat->getCreateTime()  > 0)){
                                                    
                            // ignore those results that are near the end of year - thier diagnosis time hasn't been set yet
                        }
                        else if (pat->getInfected()) {

                            if (testedInHouse) {
                        
                                total.sampleTestPosTimeIn = total.sampleTestPosTimeIn + tempTime;
                                diseaseTotal[diseaseIndex].sampleTestPosTimeIn = diseaseTotal[diseaseIndex].sampleTestPosTimeIn + tempTime;
                                facilityStats[diseaseIndex][count].sampleTestPosTimeIn = facilityStats[diseaseIndex][count].sampleTestPosTimeIn + tempTime;
                                total.sampleTestPosCountIn++;
                                diseaseTotal[diseaseIndex].sampleTestPosCountIn++;
                                facilityStats[diseaseIndex][count].sampleTestPosCountIn++;
                            }
                            else {

                                total.sampleTestPosTimeOut = total.sampleTestPosTimeOut + tempTime;
                                diseaseTotal[diseaseIndex].sampleTestPosTimeOut = diseaseTotal[diseaseIndex].sampleTestPosTimeOut + tempTime;
                                facilityStats[diseaseIndex][count].sampleTestPosTimeOut = facilityStats[diseaseIndex][count].sampleTestPosTimeOut + tempTime;
                                total.sampleTestPosCountOut++;
                                diseaseTotal[diseaseIndex].sampleTestPosCountOut++;
                                facilityStats[diseaseIndex][count].sampleTestPosCountOut++;
                            }
                        }
                    }
                    // track pre-diagnostic loss to followup patients 
                    if (pat->getPreTestLtoFFlag()) {

                        //std::cout << "stats:pre-diag inactive ltof = "  << pat->getPreTestLtoFFlag() << " alive = " << pat->isAlive() << " return time = " << pat->getFacilityReturnTime() << "\n";

                        total.countPreTestLtoFOut++;
                        diseaseTotal[diseaseIndex].countPreTestLtoFOut++;
                        facilityStats[diseaseIndex][count].countPreTestLtoFOut++;

                        //if (pat->getPreTestLtoFCount() > 1) std::cout << "stats:pre-diag ltof count  = "  << pat->getPreTestLtoFCount() <<  "\n";
                        
                        if (samplePtr->getTestedTime() > 0) {
                    
                            total.countPreTestLtoFOutTested++;
                            diseaseTotal[diseaseIndex].countPreTestLtoFOutTested++;
                            facilityStats[diseaseIndex][count].countPreTestLtoFOutTested++;
                        }
                        else {

                            total.countPreTestLtoFOutMinusTestedSamp++;
                            diseaseTotal[diseaseIndex].countPreTestLtoFOutMinusTestedSamp++;
                            facilityStats[diseaseIndex][count].countPreTestLtoFOutMinusTestedSamp++;
                        }

                        // only infected patients count as re-presenting or dying - uninfected ones just get better
                        if (pat->getInfected()) {

                            if (pat->getRepresentFlag()) {
                        
                                total.countPreTestLtoFORepresent++;
                                diseaseTotal[diseaseIndex].countPreTestLtoFORepresent++;
                                facilityStats[diseaseIndex][count].countPreTestLtoFORepresent++;

                                if (samplePtr->getTestedTime() < 1) {

                                    total.countPreTestLtoFOutUnTested++;
                                    diseaseTotal[diseaseIndex].countPreTestLtoFOutUnTested++;
                                    facilityStats[diseaseIndex][count].countPreTestLtoFOutUnTested++;
                                }
                                else {

                                    total.countPreTestLtoFOutUnTested2++;
                                    diseaseTotal[diseaseIndex].countPreTestLtoFOutUnTested2++;
                                    facilityStats[diseaseIndex][count].countPreTestLtoFOutUnTested2++;
                                }
                            }
                            else {

                                total.countPreTestLtoFDead++;
                                diseaseTotal[diseaseIndex].countPreTestLtoFDead++;
                                facilityStats[diseaseIndex][count].countPreTestLtoFDead++;
                            }
                        }
                    }
                
                    //count treated patients
                    if ((pat->getInfected()) && (pat->getStartedTreatmentTime() > 0)) {

                        if ((!pat->isAlive()) && (!pat->getRelapseFlag())) {

                            total.countPTD++;
                            diseaseTotal[diseaseIndex].countPTD++;
                            facilityStats[diseaseIndex][count].countPTD++;
                        }
                        if ((pat->isAlive()) && (!pat->getRelapseFlag())) {

                            total.countPTC++;
                            diseaseTotal[diseaseIndex].countPTC++;
                            facilityStats[diseaseIndex][count].countPTC++;
                        }
                        if (pat->getRelapseFlag()) {

                            total.countPTR++;
                            diseaseTotal[diseaseIndex].countPTR++;
                            facilityStats[diseaseIndex][count].countPTR++;
                        }
                    }
                
                    // track relapse patients 
                    if (pat->getRelapseFlag()) {
                    
                        //std::cout << "stats:relapse flag pos status =  "  << status << "\n";
                        
                        total.countRetreat++;
                        diseaseTotal[diseaseIndex].countRetreat++;
                        facilityStats[diseaseIndex][count].countRetreat++;

                        if (pat->isAlive()) {
                        
                            total.countRetreatCure++;
                            diseaseTotal[diseaseIndex].countRetreatCure++;
                            facilityStats[diseaseIndex][count].countRetreatCure++;
                        }
                        else {

                            total.countRetreatDead++;
                            diseaseTotal[diseaseIndex].countRetreatDead++;
                            facilityStats[diseaseIndex][count].countRetreatDead++;
                        }
                    }
                
                    // keep track of time sample has to wait for resources -  ignore patients in-house or away waiting for results
                    //if ((status > 1) && (pat->getReasonDied() != "NoTreatDeath")) {
                    
                    // make sure sample has been tested from infected patients
                    if ((samplePtr->getTestedTime() >=0) && (pat->getInfected())) {
                    
                        collectTime = samplePtr->getTestedTime() - samplePtr->getCreateTime();
                        //if (collectTime < 0) std::cout << "stats: time collect to test = "  << samplePtr->getTestedTime() << " created = " << samplePtr->getCreateTime() << " collect time = " << collectTime  << " in/out = " << testedInHouse  << " patient status = " << status  << " dead = " << pat->isAlive() <<  " inactive " << " reason = " << pat->getReasonDied() <<  " results returned " << pat->getResultsReturnedTime() << " sample status = " << samplePtr->getStatus() << "\n";

                        if (testedInHouse) {
                
                            total.sampleCollectToTestInTime = total.sampleCollectToTestInTime + collectTime;
                            diseaseTotal[diseaseIndex].sampleCollectToTestInTime = diseaseTotal[diseaseIndex].sampleCollectToTestInTime + collectTime;
                            facilityStats[diseaseIndex][count].sampleCollectToTestInTime = facilityStats[diseaseIndex][count].sampleCollectToTestInTime + collectTime;
                            total.sampleCollectToTestInCount++;
                            diseaseTotal[diseaseIndex].sampleCollectToTestInCount++;
                            facilityStats[diseaseIndex][count].sampleCollectToTestInCount++;
                        }
                        else {
                
                            total.sampleCollectToTestOutTime = total.sampleCollectToTestOutTime + collectTime;
                            diseaseTotal[diseaseIndex].sampleCollectToTestOutTime = diseaseTotal[diseaseIndex].sampleCollectToTestOutTime + collectTime;
                            facilityStats[diseaseIndex][count].sampleCollectToTestOutTime = facilityStats[diseaseIndex][count].sampleCollectToTestOutTime + collectTime;
                            total.sampleCollectToTestOutCount++;
                            diseaseTotal[diseaseIndex].sampleCollectToTestOutCount++;
                            facilityStats[diseaseIndex][count].sampleCollectToTestOutCount++;
                        }
                    }
                
                    // track pre-diagnostic loss to follow up for patients that returned for diagnosis
                    if (pat->getRepresentFlag()) {
                    
                        total.countPLtoF++;
                        diseaseTotal[diseaseIndex].countPLtoF++;
                        facilityStats[diseaseIndex][count].countPLtoF++;
                    }
                
                    // track various deaths - pre-diagnostic loss to follow up, treatment, retreatment
                    if ((!pat->isAlive()) && (pat->getInfected())) {

                        if (pat->getTimeOfDeath()  > 0) {
                        
                            sampleToDeathTime = pat->getTimeOfDeath() - pat->getTimestepPresented();
                    
                            total.sampleReturnToDeathTime = total.sampleReturnToDeathTime + sampleToDeathTime;
                            diseaseTotal[diseaseIndex].sampleReturnToDeathTime = diseaseTotal[diseaseIndex].sampleReturnToDeathTime + sampleToDeathTime;
                            facilityStats[diseaseIndex][count].sampleReturnToDeathTime = facilityStats[diseaseIndex][count].sampleReturnToDeathTime + sampleToDeathTime;
                            total.sampleReturnToDeathCount++;
                            diseaseTotal[diseaseIndex].sampleReturnToDeathCount++;
                            facilityStats[diseaseIndex][count].sampleReturnToDeathCount++;

                            // pre-diagnostic loss to follow up, might include those that re-present and die in treatment
                            if (pat->getPreTestLtoFFlag()) {

                                total.sampleReturnToDeathPreDiagLtoFTime = total.sampleReturnToDeathPreDiagLtoFTime + sampleToDeathTime;
                                diseaseTotal[diseaseIndex].sampleReturnToDeathPreDiagLtoFTime = diseaseTotal[diseaseIndex].sampleReturnToDeathPreDiagLtoFTime + sampleToDeathTime;
                                facilityStats[diseaseIndex][count].sampleReturnToDeathPreDiagLtoFTime = facilityStats[diseaseIndex][count].sampleReturnToDeathPreDiagLtoFTime + sampleToDeathTime;
                                total.sampleReturnToDeathPreDiagLtoFCount++;
                                diseaseTotal[diseaseIndex].sampleReturnToDeathPreDiagLtoFCount++;
                                facilityStats[diseaseIndex][count].sampleReturnToDeathPreDiagLtoFCount++;
                            }

                            // died in treatment, either original or relapse retreatment
                            if (pat->getTimeStepsInTreatment() > 0) {

                                // retreatment
                                if (pat->getRelapseFlag()) {

                                    total.sampleReturnToDeathInRetrtTime = total.sampleReturnToDeathInRetrtTime + sampleToDeathTime;
                                    diseaseTotal[diseaseIndex].sampleReturnToDeathInRetrtTime = diseaseTotal[diseaseIndex].sampleReturnToDeathInRetrtTime + sampleToDeathTime;
                                    facilityStats[diseaseIndex][count].sampleReturnToDeathInRetrtTime = facilityStats[diseaseIndex][count].sampleReturnToDeathInRetrtTime + sampleToDeathTime;
                                    total.sampleReturnToDeathInRetrtCount++;
                                    diseaseTotal[diseaseIndex].sampleReturnToDeathInRetrtCount++;
                                    facilityStats[diseaseIndex][count].sampleReturnToDeathInRetrtCount++;
                                }
                                else {
                                    
                                    total.sampleReturnToDeathInTrtTime = total.sampleReturnToDeathInTrtTime + sampleToDeathTime;
                                    diseaseTotal[diseaseIndex].sampleReturnToDeathInTrtTime = diseaseTotal[diseaseIndex].sampleReturnToDeathInTrtTime + sampleToDeathTime;
                                    facilityStats[diseaseIndex][count].sampleReturnToDeathInTrtTime = facilityStats[diseaseIndex][count].sampleReturnToDeathInTrtTime + sampleToDeathTime;
                                    total.sampleReturnToDeathInTrtCount++;
                                    diseaseTotal[diseaseIndex].sampleReturnToDeathInTrtCount++;
                                    facilityStats[diseaseIndex][count].sampleReturnToDeathInTrtCount++;
                                }
                            }
                        }
                    }
                
                    // track time until exit
                    if (pat->getInfected()) {

                        if ((!pat->isAlive()) || (pat->getStartedTreatmentTime() > 0)) {

                            sampleToDeathTime = pat->getInfectEndTime() - pat->getTimestepPresented();

                            total.sampleReturnToExitTime = total.sampleReturnToExitTime + sampleToDeathTime;
                            diseaseTotal[diseaseIndex].sampleReturnToExitTime = diseaseTotal[diseaseIndex].sampleReturnToExitTime + sampleToDeathTime;
                            facilityStats[diseaseIndex][count].sampleReturnToExitTime = facilityStats[diseaseIndex][count].sampleReturnToExitTime + sampleToDeathTime;
                            total.sampleReturnToExitCount++;
                            diseaseTotal[diseaseIndex].sampleReturnToExitCount++;
                            facilityStats[diseaseIndex][count].sampleReturnToExitCount++;
                        }
                    }
                
                    // track old age death
                    if (!pat->isAlive()) {

                        if (pat->getReasonDied() == "OldAge") {

                            //std::cout << "stats: inside agent patient dead because  = "  << pat->getReasonDied() << "\n";

                            total.countPDOA++;
                            diseaseTotal[diseaseIndex].countPDOA++;
                            facilityStats[diseaseIndex][count].countPDOA++;
                        }
                    }

                    if (pat->getInfected()) {

                        total.countPInf++;
                        diseaseTotal[diseaseIndex].countPInf++;
                        facilityStats[diseaseIndex][count].countPInf++;

                        if (pat->getTreated()) {

                            // update time from onset to treatment for patients undergoing treatment
                            presentToTreatTime = pat->getStartedTreatmentTime() - pat->getTimestepPresented();
                        
                            total.countPInfTreated++;
                            diseaseTotal[diseaseIndex].countPInfTreated++;
                            facilityStats[diseaseIndex][count].countPInfTreated++;

                            if (testedInHouse) {
                    
                                total.onsetToTreatIn = total.onsetToTreatIn + presentToTreatTime;
                                diseaseTotal[diseaseIndex].onsetToTreatIn = diseaseTotal[diseaseIndex].onsetToTreatIn + presentToTreatTime;
                                facilityStats[diseaseIndex][count].onsetToTreatIn = facilityStats[diseaseIndex][count].onsetToTreatIn + presentToTreatTime;
                        
                                total.countPInfTreatedIn ++;
                                diseaseTotal[diseaseIndex].countPInfTreatedIn ++;
                                facilityStats[diseaseIndex][count].countPInfTreatedIn ++;
                            }
                            else {

                                total.onsetToTreatOut = total.onsetToTreatOut + presentToTreatTime;
                                diseaseTotal[diseaseIndex].onsetToTreatOut = diseaseTotal[diseaseIndex].onsetToTreatOut + presentToTreatTime;
                                facilityStats[diseaseIndex][count].onsetToTreatOut = facilityStats[diseaseIndex][count].onsetToTreatOut + presentToTreatTime;

                                total.countPInfTreatedOut++;
                                diseaseTotal[diseaseIndex].countPInfTreatedOut++;
                                facilityStats[diseaseIndex][count].countPInfTreatedOut++;
                            }
                        }
                    }
                    else {

                        if (pat->getTreated()) {
                            
                            total.countPUninfTreated++;
                            diseaseTotal[diseaseIndex].countPUninfTreated++;
                            facilityStats[diseaseIndex][count].countPUninfTreated++;
                        }
                    }

                    if (pat->getTestPos()) {
                        
                        total.countTestPos++;
                        diseaseTotal[diseaseIndex].countTestPos++;
                        facilityStats[diseaseIndex][count].countTestPos++;

                        // count true positives
                        if (pat->getInfected()) {
                        
                            total.countTruePos++;
                            diseaseTotal[diseaseIndex].countTruePos++;
                            facilityStats[diseaseIndex][count].countTruePos++;
                        }
                        else {

                            total.countFalsePos++;
                            diseaseTotal[diseaseIndex].countFalsePos++;
                            facilityStats[diseaseIndex][count].countFalsePos++;
                        }
                    }
                    else {
                        
                        // if test negative - it is possible it hasn't ever been tested (LtoF death)
                        if (samplePtr->getTestedTime() >= 0) {

                            total.countTestNeg++;
                            diseaseTotal[diseaseIndex].countTestNeg++;
                            facilityStats[diseaseIndex][count].countTestNeg++;

                            // count false negatives
                            if (pat->getInfected()) {
                        
                                total.countFalseNeg++;
                                diseaseTotal[diseaseIndex].countFalseNeg++;
                                facilityStats[diseaseIndex][count].countFalseNeg++;
                            }
                            else {

                                total.countTrueNeg++;
                                diseaseTotal[diseaseIndex].countTrueNeg++;
                                facilityStats[diseaseIndex][count].countTrueNeg++;
                            }
                        }
                    }

                    if (pat->getMDPos()) {

                        if (pat->getInfected()) {
                            total.countMDInf++;
                            diseaseTotal[diseaseIndex].countMDInf++;
                            facilityStats[diseaseIndex][count].countMDInf++;
                        }
                        else {
                            total.countMDUninf++;
                            diseaseTotal[diseaseIndex].countMDUninf++;
                            facilityStats[diseaseIndex][count].countMDUninf++;
                        }
                    }

                    // keep track of samples waiting to be tested
                    if ((!pat->getPreTestLtoFFlag()) && (samplePtr->getTestedTime() < 0)) {
                    
                        total.sampleWaitTestCount++;
                        diseaseTotal[diseaseIndex].sampleWaitTestCount++;
                        facilityStats[diseaseIndex][count].sampleWaitTestCount++;

                        if (testedInHouse) {

                            total.sampleWaitTestCountIn++;
                            diseaseTotal[diseaseIndex].sampleWaitTestCountIn++;
                            facilityStats[diseaseIndex][count].sampleWaitTestCountIn++;
                        }
                        else {

                            total.sampleWaitTestCountOut++;
                            diseaseTotal[diseaseIndex].sampleWaitTestCountOut++;
                            facilityStats[diseaseIndex][count].sampleWaitTestCountOut++;
                        }
                    }
                    
                    // keep track of samples have been returned but patient hasn't been notified
                    if ((samplePtr->getResultsReturnedToOrigFacilityTime() > 0) && (samplePtr->getResultsReturnedTime() < 1)) {

                        //std::cout << "stats: results returned orig facility -results not returned to patient  tested time = "  << samplePtr->getTestedTime() << "\n";
                        
                        total.patientNotNotified++;
                        diseaseTotal[diseaseIndex].patientNotNotified++;
                        facilityStats[diseaseIndex][count].patientNotNotified++;

                        if (testedInHouse) {

                            total.patientNotNotifiedIn++;
                            diseaseTotal[diseaseIndex].patientNotNotifiedIn++;
                            facilityStats[diseaseIndex][count].patientNotNotifiedIn++;
                        }
                        else {

                            total.patientNotNotifiedOut++;
                            diseaseTotal[diseaseIndex].patientNotNotifiedOut++;
                            facilityStats[diseaseIndex][count].patientNotNotifiedOut++;
                        }
                        if ((pat->getTestPos()) || (pat->getMDPos())) {

                            total.patientNotNotifiedPos++;
                            diseaseTotal[diseaseIndex].patientNotNotifiedPos++;
                            facilityStats[diseaseIndex][count].patientNotNotifiedPos++;

                            //dumpPatientInfo(pat, timeStep);
                            //std::cout << "patient recieved pos results returned = " << pat->getResultsReturnedTime() << ", sample tested time = " << samplePtr->getTestedTime() << ", sample returned time = " << samplePtr->getResultsReturnedTime() << ", sample returned to facility = " << samplePtr->getResultsReturnedToOrigFacilityTime() << ", samp status = " << samplePtr->getStatus() << ", pat status = " << pat->getStatus() << ", tran time = " << samplePtr->getReturnTransitTime() << "\n";
                        }
                    }


                    // keep track of samples have been returned but patient hasn't been notified
                    if ((samplePtr->getResultsReturnedToOrigFacilityTime() > 0) && (samplePtr->getResultsReturnedTime() < 1)) {

                        total.patientNotNotified++;
                        diseaseTotal[diseaseIndex].patientNotNotified++;
                        facilityStats[diseaseIndex][count].patientNotNotified++;

                        if (testedInHouse) {

                            total.patientNotNotifiedIn++;
                            diseaseTotal[diseaseIndex].patientNotNotifiedIn++;
                            facilityStats[diseaseIndex][count].patientNotNotifiedIn++;
                        }
                        else {

                            total.patientNotNotifiedOut++;
                            diseaseTotal[diseaseIndex].patientNotNotifiedOut++;
                            facilityStats[diseaseIndex][count].patientNotNotifiedOut++;
                        }
                        if ((pat->getTestPos()) || (pat->getMDPos())) {

                            total.patientNotNotifiedPos++;
                            diseaseTotal[diseaseIndex].patientNotNotifiedPos++;
                            facilityStats[diseaseIndex][count].patientNotNotifiedPos++;

                            //dumpPatientInfo(pat, timeStep);
                            //std::cout << "patient recieved pos results returned = " << pat->getResultsReturnedTime() << ", sample tested time = " << samplePtr->getTestedTime() << ", sample returned time = " << samplePtr->getResultsReturnedTime() << ", sample returned to facility = " << samplePtr->getResultsReturnedToOrigFacilityTime() << ", samp status = " << samplePtr->getStatus() << ", pat status = " << pat->getStatus() << ", tran time = " << samplePtr->getReturnTransitTime() << "\n";
                        }
                        //std::cout << "patient recieved results = " << pat->getResultsReturnedTime() << ", sample returned time = " << samplePtr->getResultsReturnedTime() << ", sample returned to facility = " << samplePtr->getResultsReturnedToOrigFacilityTime() << "\n";
                    }

                    // count tests used
                    testName = pat->getSampleTest();
                    //std::cout << "stats: inside agent  test name = "  << testName << "\n";

                    if (total.test1Name == "") {

                        // ignore counts for patients that are pre-treatment ltof and died
                        if (samplePtr->getTestedTime() > 0) {
                        
                            total.test1Name = testName;
                            diseaseTotal[diseaseIndex].test1Name = testName;
                            facilityStats[diseaseIndex][count].test1Name = testName;
                            total.test1Count++;
                            diseaseTotal[diseaseIndex].test1Count++;
                            facilityStats[diseaseIndex][count].test1Count++;

                            if (testedInHouse) {

                                total.test1CountIn++;
                                diseaseTotal[diseaseIndex].test1CountIn++;
                                facilityStats[diseaseIndex][count].test1CountIn++;
                            }
                            else {

                                total.test1CountOut++;
                                diseaseTotal[diseaseIndex].test1CountOut++;
                                facilityStats[diseaseIndex][count].test1CountOut++;
                            }
                            if (samplePtr->isPositive()) {

                                total.test1PosCount++;
                                diseaseTotal[diseaseIndex].test1PosCount++;
                                facilityStats[diseaseIndex][count].test1PosCount++;

                                if (pat->getInfected()) {

                                    total.test1TruePosCount++;
                                    diseaseTotal[diseaseIndex].test1TruePosCount++;
                                    facilityStats[diseaseIndex][count].test1TruePosCount++;
                                }
                            }
                            else {

                                // Only count negative samples that have been tested (LtoF death)
                                if (samplePtr->getTestedTime()  >= 0) {
                        
                                    total.test1NegCount++;
                                    diseaseTotal[diseaseIndex].test1NegCount++;
                                    facilityStats[diseaseIndex][count].test1NegCount++;

                                    if (!pat->getInfected()) {

                                        total.test1TrueNegCount++;
                                        diseaseTotal[diseaseIndex].test1TrueNegCount++;
                                        facilityStats[diseaseIndex][count].test1TrueNegCount++;
                                    }
                                }
                            }
                        }
                    }
                    else if (testName == total.test1Name) {

                        // ignore counts for patients that are pre-treatment ltof and died
                        if (samplePtr->getTestedTime() > 0) {
                        
                            total.test1Count++;
                            diseaseTotal[diseaseIndex].test1Count++;
                            facilityStats[diseaseIndex][count].test1Count++;

                            if (testedInHouse) {

                                total.test1CountIn++;
                                diseaseTotal[diseaseIndex].test1CountIn++;
                                facilityStats[diseaseIndex][count].test1CountIn++;
                            }
                            else {

                                total.test1CountOut++;
                                diseaseTotal[diseaseIndex].test1CountOut++;
                                facilityStats[diseaseIndex][count].test1CountOut++;
                            }
                            if (samplePtr->isPositive()) {

                                total.test1PosCount++;
                                diseaseTotal[diseaseIndex].test1PosCount++;
                                facilityStats[diseaseIndex][count].test1PosCount++;

                                if ( pat->getInfected()) {

                                    total.test1TruePosCount++;
                                    diseaseTotal[diseaseIndex].test1TruePosCount++;
                                    facilityStats[diseaseIndex][count].test1TruePosCount++;
                                }
                            }
                            else {

                                // Only count negative samples that have been tested (LtoF death)
                                if (samplePtr->getTestedTime()  >= 0) {
                        
                                    total.test1NegCount++;
                                    diseaseTotal[diseaseIndex].test1NegCount++;
                                    facilityStats[diseaseIndex][count].test1NegCount++;

                                    if (!pat->getInfected()) {

                                        total.test1TrueNegCount++;
                                        diseaseTotal[diseaseIndex].test1TrueNegCount++;
                                        facilityStats[diseaseIndex][count].test1TrueNegCount++;
                                    }
                                }
                            }
                        }
                    }
                    else if (total.test2Name == "") {

                        // ignore counts for patients that are pre-treatment ltof and died
                        if (samplePtr->getTestedTime() > 0) {
                        
                            total.test2Name = testName;
                            diseaseTotal[diseaseIndex].test2Name = testName;
                            facilityStats[diseaseIndex][count].test2Name = testName;
                            total.test2Count++;
                            diseaseTotal[diseaseIndex].test2Count++;
                            facilityStats[diseaseIndex][count].test2Count++;

                            if (testedInHouse) {

                                total.test2CountIn++;
                                diseaseTotal[diseaseIndex].test2CountIn++;
                                facilityStats[diseaseIndex][count].test2CountIn++;
                            }
                            else {

                                total.test2CountOut++;
                                diseaseTotal[diseaseIndex].test2CountOut++;
                                facilityStats[diseaseIndex][count].test2CountOut++;
                            }
                            if (samplePtr->isPositive()) {

                                total.test2PosCount++;
                                diseaseTotal[diseaseIndex].test2PosCount++;
                                facilityStats[diseaseIndex][count].test2PosCount++;

                                if (pat->getInfected()) {

                                    total.test2TruePosCount++;
                                    diseaseTotal[diseaseIndex].test2TruePosCount++;
                                    facilityStats[diseaseIndex][count].test2TruePosCount++;
                                }
                            }
                            else {

                                // Only count negative samples that have been tested (LtoF death)
                                if (samplePtr->getTestedTime()  >= 0) {
                        
                                    total.test2NegCount++;
                                    diseaseTotal[diseaseIndex].test2NegCount++;
                                    facilityStats[diseaseIndex][count].test2NegCount++;

                                    if (!pat->getInfected()) {

                                        total.test2TrueNegCount++;
                                        diseaseTotal[diseaseIndex].test2TrueNegCount++;
                                        facilityStats[diseaseIndex][count].test2TrueNegCount++;
                                    }
                                }
                            }
                        }
                    }
                    else if (testName == total.test2Name) {

                        // ignore counts for patients that are pre-treatment ltof and died
                        if (samplePtr->getTestedTime() > 0) {
                        
                            total.test2Count++;
                            diseaseTotal[diseaseIndex].test2Count++;
                            facilityStats[diseaseIndex][count].test2Count++;

                            if (testedInHouse) {

                                total.test2CountIn++;
                                diseaseTotal[diseaseIndex].test2CountIn++;
                                facilityStats[diseaseIndex][count].test2CountIn++;
                            }
                            else {

                                total.test2CountOut++;
                                diseaseTotal[diseaseIndex].test2CountOut++;
                                facilityStats[diseaseIndex][count].test2CountOut++;
                            }
                            if (samplePtr->isPositive()) {

                                total.test2PosCount++;
                                diseaseTotal[diseaseIndex].test2PosCount++;
                                facilityStats[diseaseIndex][count].test2PosCount++;

                                if (pat->getInfected()) {

                                    total.test2TruePosCount++;
                                    diseaseTotal[diseaseIndex].test2TruePosCount++;
                                    facilityStats[diseaseIndex][count].test2TruePosCount++;
                                }
                            }
                            else {

                                // Only count negative samples that have been tested (LtoF death)
                                if (samplePtr->getTestedTime()  >= 0) {
                        
                                    total.test2NegCount++;
                                    diseaseTotal[diseaseIndex].test2NegCount++;
                                    facilityStats[diseaseIndex][count].test2NegCount++;

                                    if (!pat->getInfected()) {

                                        total.test2TrueNegCount++;
                                        diseaseTotal[diseaseIndex].test2TrueNegCount++;
                                        facilityStats[diseaseIndex][count].test2TrueNegCount++;
                                    }
                                }
                            }
                        }
                    }
                    else if (total.test3Name == "") {

                        // ignore counts for patients that are pre-treatment ltof and died
                        if (samplePtr->getTestedTime() > 0) {
                        
                            total.test3Name = testName;
                            diseaseTotal[diseaseIndex].test3Name = testName;
                            facilityStats[diseaseIndex][count].test3Name = testName;
                            total.test3Count++;
                            diseaseTotal[diseaseIndex].test3Count++;
                            facilityStats[diseaseIndex][count].test3Count++;
                        }
                    }
                    else if (testName == total.test3Name) {

                        // ignore counts for patients that are pre-treatment ltof and died
                        if (samplePtr->getTestedTime() > 0) {
                        
                            total.test3Name = testName;
                            diseaseTotal[diseaseIndex].test3Name = testName;
                            facilityStats[diseaseIndex][count].test3Name = testName;
                            total.test3Count++;
                            diseaseTotal[diseaseIndex].test3Count++;
                            facilityStats[diseaseIndex][count].test3Count++;
                        }
                    }
                    else {

                        std::cout << "stats: inside agent couldn't find test name = "  << testName << "\n";
                    }

                    // track whether sample was sent out for analysis or tested at patient home facility
                    // if sample tested - test time is positive
                    if (samplePtr->getTestedTime() >= 0) {

                        if (pat->getFacility() == pat->getSampleTestFacility()) { 

                            total.inTestCount++;
                            diseaseTotal[diseaseIndex].inTestCount++;
                            facilityStats[diseaseIndex][count].inTestCount++;
                        }
                        else {

                            total.outTestCount++;
                            diseaseTotal[diseaseIndex].outTestCount++;
                            facilityStats[diseaseIndex][count].outTestCount++;
                        }
                    }
                
                    // track whether sample was sent out for analysis or tested at patient home facility 
                    // track collected samples - including those from pre-treatment ltfu - which will be counted twice they have to be alive and infected to re-present)
                    if (testedInHouse) {

                        total.inCollectedCount++;
                        diseaseTotal[diseaseIndex].inCollectedCount++;
                        facilityStats[diseaseIndex][count].inCollectedCount++;
                    }
                    else {

                        total.outCollectedCount++;
                        diseaseTotal[diseaseIndex].outCollectedCount++;
                        facilityStats[diseaseIndex][count].outCollectedCount++;
                        
                        // update sample transit time variables - even include (sampleTempTime > 0)
                        sampleTempTime = samplePtr->getTransitTimeSave();

                        total.sampleTransTime = total.sampleTransTime + sampleTempTime;
                        diseaseTotal[diseaseIndex].sampleTransTime = diseaseTotal[diseaseIndex].sampleTransTime + sampleTempTime;
                        facilityStats[diseaseIndex][count].sampleTransTime = facilityStats[diseaseIndex][count].sampleTransTime + sampleTempTime;
                        total.sampleTransCount++;
                        diseaseTotal[diseaseIndex].sampleTransCount++;
                        facilityStats[diseaseIndex][count].sampleTransCount++;

                        // count samples collected from those who re-present after pre-test LTFU (count first lost sample, then second sample from re-presentation)
                        if (pat->getRepresentFlag()) {

                            total.outCollectedCount++;
                            diseaseTotal[diseaseIndex].outCollectedCount++;
                            facilityStats[diseaseIndex][count].outCollectedCount++;

                            total.sampleTransTime = total.sampleTransTime + sampleTempTime;
                            diseaseTotal[diseaseIndex].sampleTransTime = diseaseTotal[diseaseIndex].sampleTransTime + sampleTempTime;
                            facilityStats[diseaseIndex][count].sampleTransTime = facilityStats[diseaseIndex][count].sampleTransTime + sampleTempTime;
                            total.sampleTransCount++;
                            diseaseTotal[diseaseIndex].sampleTransCount++;
                            facilityStats[diseaseIndex][count].sampleTransCount++;
                        }
                    }

                    // dump statistics for each patient - only do it to file
                    if (individualFlag) {

                        // get time since patient has turned inactive
                        lagTime = timeStep - pat->getTimestepInactive();

                        myfile9 << facilityStats[0][count].name << "," <<  facilityStats[0][count].id << "," <<  facilityStats[0][count].tier << "," << status << "," << pat->getDisease() << ",inactive" << "," << pat->getAge();
                        myfile9 << "," << pat->isAlive() << "," << pat->getInfected();
                        myfile9 << "," << pat->getTestPos() << "," << pat->getMDPos() << "," << pat->getTimeFromOnset() + lagTime << "," << pat->getTimeStepsInTreatment() << "," << pat->getTimeStepsNotInTreatment() + lagTime;
                        myfile9 << "," << (pat->getStartedTreatmentTime() - pat->getTimestepPresented()) << "," << pat->getTimestepPresented() << "\n";
                          
                        myfile10 << facilityStats[0][count].name << "," <<  facilityStats[0][count].district << "," <<  facilityStats[0][count].region << "," <<  facilityStats[0][count].id << "," <<  facilityStats[0][count].tier;
                        myfile10 << "," << status << "," << statusString << "," << pat->getPrevStatus() << "," << pat->getDisease() << ",inactive" << "," << pat->getAge() << "," << pat->isAlive() << "," << pat->getInfected();
                        if ((status == PatInfTestLtofNoTreat) || (status == PatInfNoTestNoTreat)) {
                        
                            myfile10 << "," << "NA" << "," << "NA";
                        }
                        else if (pat->getTestPos()) {
                            myfile10 << "," << "TRUE" << "," << "FALSE";
                        }
                        else {
                            myfile10 << "," << "FALSE" << "," << "TRUE";
                        }
                        if ((status == PatInfTestLtofNoTreat) || (status == PatInfNoTestNoTreat)) {
                        
                            myfile10 << "," << "NA";
                        }
                        else if (pat->getMDPos()) {
                            myfile10 << "," << "TRUE";
                        }
                        else {
                            myfile10 << "," << "FALSE";
                        }

                        myfile10 << "," << pat->getTimestepPresented() << "," << pat->getCreateTime() << "," << samplePtr->getStatus() << "," << sampleStatusString;
                        myfile10 << "," << samplePtr->getPrevStatus() << "," << pat->getResultsReturnedTime();
                        if (pat->getResultsReturnedTime() > 0) {
                        
                            myfile10 << "," << (pat->getResultsReturnedTime() - pat->getTimestepPresented());
                        }
                        else { 
                        
                            myfile10 << "," << "0";
                        }
                        myfile10  << "," << pat->getPatientRecievedMDResultTime()<< "," << pat->getStartedTreatmentTime();
                        if (pat->getStartedTreatmentTime() > 0) {
                        
                            myfile10 << "," << (pat->getStartedTreatmentTime() - pat->getTimestepPresented());
                        }
                        else { 
                        
                            myfile10 << "," << "0";
                        }
                        myfile10 << "," << pat->getTimeStepsInTreatment() << "," << pat->getTimeStepsNotInTreatment() + lagTime;
                        myfile10 << "," << (pat->getStartedTreatmentTime() - pat->getTimestepPresented()) << "," << pat->getTimePatientLtoF() << "," << pat->getNoReturnsForResults() << "," << samplePtr->getTransitTimeSave() << "," << samplePtr->getReturnTransitTimeSave();
                        myfile10 << "," << samplePtr->getTestedTime() << "," << samplePtr->getResultsReturnedToOrigFacilityTime();
                        myfile10 << "," << pat->getSampleTest() << "," << samplePtr->getTestingFacility() << "," << samplePtr->getTestingDistance() << "\n";
                        
                        myfile1 << facilityStats[0][count].name << "," <<  facilityStats[0][count].id << "," <<  facilityStats[0][count].tier << "," << status << "," << pat->getDisease() << ",inactive" << "," << pat->getAge() << "," << pat->isAlive() << "," << pat->getInfected();
                        myfile1 << "," << pat->getTestPos() << "," << pat->getMDPos() << "," << pat->getTimeFromOnset() + lagTime << "," << pat->getTimeStepsInTreatment() << "," << pat->getTimeStepsNotInTreatment() + lagTime;
                        myfile1 << "," << (pat->getStartedTreatmentTime() - pat->getTimestepPresented()) << "," << pat->getTimestepPresented() << "\n";
                    }

                    if (individualSamplingFlag)  {
                    
                        if (noTotalPatients % patientSamplingRate == 0) {

                            // get time since patient has turned inactive
                            lagTime = timeStep - pat->getTimestepInactive();
                        
                            // patient sampling raw data
                            myfile12 << facilityStats[0][count].name << "," <<  facilityStats[0][count].district << "," <<  facilityStats[0][count].region << "," <<  facilityStats[0][count].id << "," <<  facilityStats[0][count].tier;
                            myfile12 << "," << status << "," << statusString << "," << pat->getPrevStatus() << "," << pat->getDisease() << ",inactive" << "," << pat->getAge() << "," << pat->isAlive() << "," << pat->getInfected();
                            if ((status == PatInfTestLtofNoTreat) || (status == PatInfNoTestNoTreat)) {
                        
                                myfile12 << "," << "NA" << "," << "NA";
                            }
                            else if (pat->getTestPos()) {
                                myfile12 << "," << "TRUE" << "," << "FALSE";
                            }
                            else {
                                myfile12 << "," << "FALSE" << "," << "TRUE";
                            }
                            if ((status == PatInfTestLtofNoTreat) || (status == PatInfNoTestNoTreat)) {
                        
                                myfile12 << "," << "NA";
                            }
                            else if (pat->getMDPos()) {
                                myfile12 << "," << "TRUE";
                            }
                            else {
                                myfile12 << "," << "FALSE";
                            }

                            myfile12 << "," << pat->getTimestepPresented() << "," << pat->getCreateTime() << "," << samplePtr->getStatus() << "," << sampleStatusString;
                            myfile12 << "," << samplePtr->getPrevStatus() << "," << pat->getResultsReturnedTime();
                            if (pat->getResultsReturnedTime() > 0) {
                        
                                myfile12 << "," << (pat->getResultsReturnedTime() - pat->getTimestepPresented());
                            }
                            else { 
                        
                                myfile12 << "," << "0";
                            }
                            myfile12  << "," << pat->getPatientRecievedMDResultTime()<< "," << pat->getStartedTreatmentTime();
                            if (pat->getStartedTreatmentTime() > 0) {
                        
                                myfile12 << "," << (pat->getStartedTreatmentTime() - pat->getTimestepPresented());
                            }
                            else { 
                        
                                myfile12 << "," << "0";
                            }
                            myfile12 << "," << pat->getTimeStepsInTreatment() << "," << pat->getTimeStepsNotInTreatment() + lagTime;
                            myfile12 << "," << (pat->getStartedTreatmentTime() - pat->getTimestepPresented()) << "," << pat->getTimePatientLtoF() << "," << samplePtr->getTransitTimeSave() << "," << samplePtr->getReturnTransitTimeSave();
                            myfile12 << "," << samplePtr->getTestedTime() << "," << samplePtr->getResultsReturnedToOrigFacilityTime();
                            myfile12 << "," << pat->getSampleTest() << "," << samplePtr->getTestingFacility() << "," << samplePtr->getTestingDistance();
                            myfile12 << "," << noTotalPatients << "\n"; 
                        }
                    }
                    
                    if ((individualFlag) || (individualSamplingFlag))  {
                        
                        // get time since patient has turned inactive
                        lagTime = timeStep - pat->getTimestepInactive();
                        
                        // update totals for individual patients
                        indNoPatients[diseaseIndex]++;
                        indAge[diseaseIndex] = indAge[diseaseIndex] + pat->getAge();
                        if (pat->isAlive()) indNoAlive[diseaseIndex]++;
                        if (pat->getInfected()) {
                            indNoInfected[diseaseIndex]++;
                            indTotalTimeFromOnset[diseaseIndex] = indTotalTimeFromOnset[diseaseIndex] + pat->getTimeFromOnset() + lagTime ;
                            indTotalTimeInfectious[diseaseIndex] = indTotalTimeInfectious[diseaseIndex] + pat->getInfectEndTime() + lagTime ;
                            indTotaltimeInTreatment[diseaseIndex] = indTotaltimeInTreatment[diseaseIndex] + pat->getTimeStepsInTreatment();
                        }
                        else {
                            indTotalTimeNotInTreatment[diseaseIndex] = indTotalTimeNotInTreatment[diseaseIndex] + pat->getTimeStepsNotInTreatment();
                        }
                        if (pat->getTestPos()) indNoTestPos[diseaseIndex]++;
                        if (pat->getMDPos()) indNoMDOverride[diseaseIndex]++;
                        indDisease[diseaseIndex] = pat->getDisease();
                        indTotalTimeFacilityReturnTime[diseaseIndex] = indTotalTimeFacilityReturnTime[diseaseIndex] + pat->getFacilityReturnTime();
                        if (pat->getResultsReturnedTime() > 0) {

                            indTotalTimeResultsReturned[diseaseIndex] = indTotalTimeResultsReturned[diseaseIndex] + (pat->getResultsReturnedTime() - pat->getCreateTime());
                            indTotalNoOfReturnVisits[diseaseIndex] = indTotalNoOfReturnVisits[diseaseIndex] + pat->getNoReturnsForResults();
                            patReturnedCount++;
                        }
                    }

                    // count dead people from old age
                    // age = pat->getAge();
                    //if (!pat->isAlive()) {
                    //    countPD++;
                    //}
                }
            }

            // get patient stats
            facilityCountP = facilityCountP + noPatientInactive;
            total.countP = total.countP + noPatientInactive;
        }

        // dump total individual statistics per facility
        if (individualFlag) {

            // loop through diseeases and output totals
            for (int diseaseIndex = 0; diseaseIndex < diseaseCount; diseaseIndex++) {

                if (debug) std::cout << "stats: inside agent stats before individual totals no. patients = " << indNoPatients[diseaseIndex] << ", no. infected = " << indNoInfected[diseaseIndex] << "\n";

                // no patients - no totals
                if (indNoPatients[diseaseIndex] > 0) {

                    myfile9 << "-----------------------------------------------------------------------------------  " << "\n";
                    myfile9 << "" << "," << "," << "," << ",disease" << ",age" << ",alive" << ",infected" << ",test.pos" << ",md.override" << ",time.from.onset" << ",time.in.treatment" << ",time.not.in.treatment" << ",std.pat.return.time";
                    myfile9 << ",avg.total.pat.return.time" << ",avg.no.pat.returns" <<  "\n";
                    
                    myfile9  << facilityStats[0][count].name << "," <<  facilityStats[0][count].id << "," <<  facilityStats[0][count].tier << ",Total"<< "," << indDisease[diseaseIndex] << "," << indAge[diseaseIndex]/indNoPatients[diseaseIndex] << "," << indNoAlive[diseaseIndex] << "," << indNoInfected[diseaseIndex] << "," << indNoTestPos[diseaseIndex] << "," << indNoMDOverride[diseaseIndex];
                
                    myfile1 << "-----------------------------------------------------------------------------------  " << "\n";
                    myfile1 << "" << "," << "," << "," << ",disease" << ",age" << ",alive" << ",infected" << ",test.pos" << ",md.override" << ",time.from.onset" << ",time.in.treatment" << ",time.not.in.treatment" << ",std.pat.return.time";
                    myfile1 << ",avg.total.pat.return.time" << ",avg.no.pat.returns" <<  "\n";
                    
                    myfile1  << facilityStats[0][count].name << "," <<  facilityStats[0][count].id << "," <<  facilityStats[0][count].tier << ",Total"<< "," << indDisease[diseaseIndex] << "," << indAge[diseaseIndex]/indNoPatients[diseaseIndex] << "," << indNoAlive[diseaseIndex] << "," << indNoInfected[diseaseIndex] << "," << indNoTestPos[diseaseIndex] << "," << indNoMDOverride[diseaseIndex];
                
                    // change output if no infected patients
                    if (indNoInfected[diseaseIndex] > 0) {
               
                        if (indNoInfected[diseaseIndex] == indNoPatients[diseaseIndex]) {

                            myfile9 << "," << indTotalTimeFromOnset[diseaseIndex]/indNoInfected[diseaseIndex] << "," << indTotaltimeInTreatment[diseaseIndex]/indNoInfected[diseaseIndex] << "," << "0";
                            
                            myfile1 << "," << indTotalTimeFromOnset[diseaseIndex]/indNoInfected[diseaseIndex] << "," << indTotaltimeInTreatment[diseaseIndex]/indNoInfected[diseaseIndex] << "," << "0";
                        }
                        else {
                    
                            myfile9 << "," << indTotalTimeFromOnset[diseaseIndex]/indNoInfected[diseaseIndex] << "," << indTotaltimeInTreatment[diseaseIndex]/indNoInfected[diseaseIndex] << "," << indTotalTimeNotInTreatment[diseaseIndex]/(indNoPatients[diseaseIndex] -indNoInfected[diseaseIndex]);
                            
                            myfile1 << "," << indTotalTimeFromOnset[diseaseIndex]/indNoInfected[diseaseIndex] << "," << indTotaltimeInTreatment[diseaseIndex]/indNoInfected[diseaseIndex] << "," << indTotalTimeNotInTreatment[diseaseIndex]/(indNoPatients[diseaseIndex] -indNoInfected[diseaseIndex]);
                        }
                    }
                    else {

                        myfile9 << "," << "0" << "," << "0" << "," << indTotalTimeNotInTreatment[diseaseIndex]/(indNoPatients[diseaseIndex] -indNoInfected[diseaseIndex]);
                        
                        myfile1 << "," << "0" << "," << "0" << "," << indTotalTimeNotInTreatment[diseaseIndex]/(indNoPatients[diseaseIndex] -indNoInfected[diseaseIndex]);
                    }
                    // myfile9 << "," << indTotalTimeFacilityReturnTime[diseaseIndex] << "," << indNoPatients[diseaseIndex];
                    // myfile1 << "," << indTotalTimeFacilityReturnTime[diseaseIndex] << "," << indNoPatients[diseaseIndex];

                    if (patReturnedCount > 0) {
                        
                        myfile9 << "," << (float) indTotalTimeFacilityReturnTime[diseaseIndex]/indNoPatients[diseaseIndex] << "," << (float) indTotalTimeResultsReturned[diseaseIndex]/patReturnedCount << "," << (float) indTotalNoOfReturnVisits[diseaseIndex]/patReturnedCount << "\n";
                        
                        myfile1 << "," << (float) indTotalTimeFacilityReturnTime[diseaseIndex]/indNoPatients[diseaseIndex] << "," << (float) indTotalTimeResultsReturned[diseaseIndex]/patReturnedCount << "," << (float) indTotalNoOfReturnVisits[diseaseIndex]/patReturnedCount << "\n";
                    }
                    else {
                        
                        myfile9 << "," << (float) indTotalTimeFacilityReturnTime[diseaseIndex]/indNoPatients[diseaseIndex] << "," << "0" << "," << "0" << "\n";
                    
                        myfile1 << "," << (float) indTotalTimeFacilityReturnTime[diseaseIndex]/indNoPatients[diseaseIndex] << "," << "0" << "," << "0" << "\n";
                    }
                }
            }
        }

        if (debug) std::cout << "stats: inside agent stats before disease count loop"  << "\n";

        // loop through each disease in model run and update statistics
        for (diseaseIndex = 0; diseaseIndex < diseaseCount; diseaseIndex++) {

            // update the sample count for each disease for this facility
            facilityStats[diseaseIndex][count].countP = patientCount[diseaseIndex];

            facilityDisease = diseaseList[diseaseIndex];

            // loop through list of disease objects and find one that matches
            //for (auto k = 0u; k < dis.size(); k++) {

            //    // if disease found update the disease parameters
            //    if (facilityDisease == dis[k]->getName()) {

            //        std::cout << "stats: inside agent stats patient count from stat counter = " << (dis[k]->getStatsPtr())->return_nPatient() << "  infected patients = " << (dis[k]->getStatsPtr())->return_nPatientInf() << "\n";

            //        break;
            //    }
            //}

            // dump statistics for each facility to a file
            if (fileFlag) {

                withTB = facilityStats[diseaseIndex][count].countPT + facilityStats[diseaseIndex][count].countPR + facilityStats[diseaseIndex][count].countPUT + facilityStats[diseaseIndex][count].countPDI + facilityStats[diseaseIndex][count].countPDIUT;

                // update population by population growth if it goes on more that 1 year
                newPop = facilityStats[diseaseIndex][count].pop;
                startIndex = referenceYear - 2015;
                endIndex = startIndex + noOfYears;
                if (noOfYears > 0) {

                    for (int i = startIndex; i < endIndex; i++) {

                        // multiply by growth rate for x number of years
                        newPop = (int) (newPop * popGrowthRateArray[i][1]);
                    }
                }
                
                // prevalence = no. with TB * (100,000/population)                                  (no. with TB/pop = prevalence/100000)
                if (newPop == 0) {
                    prevalence = 0;
                }
                else {
                    //prevalence = int((float(withTB) * 100000) / float(newPop));
                    prevalence = int((float(facilityStats[diseaseIndex][count].countPInf) * 100000) / float(newPop));
                }

                // count up all the sample results
                totalSampleCountPos = facilityStats[diseaseIndex][count].countTruePos + facilityStats[diseaseIndex][count].countFalsePos;
                totalSampleCountNeg = facilityStats[diseaseIndex][count].countTrueNeg + facilityStats[diseaseIndex][count].countFalseNeg;

               if (debug4) std::cout << "stats: inside agent stats dead old age = " << facilityStats[diseaseIndex][count].countPDOA  << ", timestep = " << timeStep << "\n";

               //treated = facilityStats[diseaseIndex][count].countPT + facilityStats[diseaseIndex][count].countPR + facilityStats[diseaseIndex][count].countPC + facilityStats[diseaseIndex][count].countPDI;
               //infected = treated + facilityStats[diseaseIndex][count].countPUT + facilityStats[diseaseIndex][count].countPDIUT;
               //treated = facilityStats[diseaseIndex][count].countPT + facilityStats[diseaseIndex][count].countPC;
               //infected = treated + facilityStats[diseaseIndex][count].countPUT + facilityStats[diseaseIndex][count].countPDIUT + facilityStats[diseaseIndex][count].countPR + facilityStats[diseaseIndex][count].countPDI;

               myfile << facilityStats[diseaseIndex][count].name << "," << facilityStats[diseaseIndex][count].id << "," << facilityStats[diseaseIndex][count].tier << "," << facilityStats[diseaseIndex][count].district << "," << facilityStats[diseaseIndex][count].region;
               myfile << "," << newPop << "," << diseaseList[diseaseIndex] << "," << prevalence;
               myfile << "," << facilityStats[diseaseIndex][count].countP << "," << facilityStats[diseaseIndex][count].countPInf << "," << facilityStats[diseaseIndex][count].countPLtoF;
               myfile << "," << facilityStats[diseaseIndex][count].outCollectedCount + facilityStats[diseaseIndex][count].inCollectedCount << "," << facilityStats[diseaseIndex][count].outCollectedCount << "," << facilityStats[diseaseIndex][count].inCollectedCount;
               if ((facilityStats[diseaseIndex][count].sampleWaitForResourceInCount + facilityStats[diseaseIndex][count].sampleWaitForResourceOutCount) < 1) {
                    myfile << "," << "0";
               }
               else {
                    myfile << "," << float(facilityStats[diseaseIndex][count].sampleWaitForResourceInTime + facilityStats[diseaseIndex][count].sampleWaitForResourceOutTime)/float(facilityStats[diseaseIndex][count].sampleWaitForResourceInCount + facilityStats[diseaseIndex][count].sampleWaitForResourceOutCount);
               }
               if (facilityStats[diseaseIndex][count].sampleWaitForResourceInCount < 1) {
                    myfile << "," << "0";
               }
               else {
                    myfile << "," << float(facilityStats[diseaseIndex][count].sampleWaitForResourceInTime)/float(facilityStats[diseaseIndex][count].sampleWaitForResourceInCount);
               }
               if (facilityStats[diseaseIndex][count].sampleWaitForResourceOutCount < 1) {
                    myfile << "," << "0" << ",";
               }
               else {
                    myfile << "," << float(facilityStats[diseaseIndex][count].sampleWaitForResourceOutTime)/float(facilityStats[diseaseIndex][count].sampleWaitForResourceOutCount) << ",";
               }
               if (facilityStats[diseaseIndex][count].sampleTransCount < 1) {
                    myfile << "0" << "," << facilityStats[diseaseIndex][count].sampleTransCount;
               }
               else {
                    myfile << float(facilityStats[diseaseIndex][count].sampleTransTime)/float(facilityStats[diseaseIndex][count].sampleTransCount) << "," << facilityStats[diseaseIndex][count].sampleTransCount;
               }
               myfile << "," << facilityStats[diseaseIndex][count].sampleTestedInCount + facilityStats[diseaseIndex][count].sampleTestedOutCount << "," << facilityStats[diseaseIndex][count].sampleTestedInCount << "," << facilityStats[diseaseIndex][count].sampleTestedOutCount;
               //myfile << "," << facilityStats[diseaseIndex][count].sampleLtoFCount;
               if (facilityStats[diseaseIndex][count].sampleCollectToTestInCount < 1) {
                    myfile << "," << "0";
               }
               else {
                    myfile << "," << float(facilityStats[diseaseIndex][count].sampleCollectToTestInTime)/float(facilityStats[diseaseIndex][count].sampleCollectToTestInCount);
               }
               myfile << "," << float(facilityStats[diseaseIndex][count].sampleCollectToTestInTime) << "," << facilityStats[diseaseIndex][count].sampleCollectToTestInCount;
               if (facilityStats[diseaseIndex][count].sampleCollectToTestOutCount < 1) {
                    myfile << "," << "0";
               }
               else {
                    myfile << "," << float(facilityStats[diseaseIndex][count].sampleCollectToTestOutTime)/float(facilityStats[diseaseIndex][count].sampleCollectToTestOutCount);
               }
               myfile << "," << float(facilityStats[diseaseIndex][count].sampleCollectToTestOutTime) << "," << facilityStats[diseaseIndex][count].sampleCollectToTestOutCount;
               myfile << "," << facilityStats[diseaseIndex][count].countTestPos << ",";
               if (totalSampleCountPos > 0) {
                    myfile << float(facilityStats[diseaseIndex][count].countTruePos)/float(totalSampleCountPos) << ",";
                    myfile << float(facilityStats[diseaseIndex][count].countFalsePos)/float(totalSampleCountPos) << ",";
               }
               else {
                    myfile << "0" << "," << "0" << ",";
               }
               myfile << facilityStats[diseaseIndex][count].countTestNeg << ",";
               if (totalSampleCountNeg > 0) {
                    myfile << float(facilityStats[diseaseIndex][count].countTrueNeg)/float(totalSampleCountNeg) << ",";
                    myfile << float(facilityStats[diseaseIndex][count].countFalseNeg)/float(totalSampleCountNeg);
               }
               else {
                    myfile << "0" << "," << "0";
               }
               if (debug2) myfile << "," << "marker";

               myfile << "," << facilityStats[diseaseIndex][count].countMDInf << "," << facilityStats[diseaseIndex][count].countMDUninf;
               myfile << "," << facilityStats[diseaseIndex][count].countMDInf+facilityStats[diseaseIndex][count].countTestPos + facilityStats[diseaseIndex][count].countMDUninf;
               myfile << "," << facilityStats[diseaseIndex][count].countFalsePos << "," << facilityStats[diseaseIndex][count].countFalseNeg;
               myfile << "," << facilityStats[diseaseIndex][count].countDiagnosis << "," << facilityStats[diseaseIndex][count].countDiagnosisIn << "," << facilityStats[diseaseIndex][count].countDiagnosisOut << "," << facilityStats[diseaseIndex][count].countPreTestLtoFOut << "," << facilityStats[diseaseIndex][count].countPreTestLtoFOutMinusTestedSamp << "," << facilityStats[diseaseIndex][count].countPreTestLtoFDead << "," << facilityStats[diseaseIndex][count].countPreTestLtoFORepresent;
               if (facilityStats[diseaseIndex][count].sampleTestPosCountIn < 1) {
                    myfile << "," << "0";
                }
                else {
                    myfile << "," << float(facilityStats[diseaseIndex][count].sampleTestPosTimeIn)/float(facilityStats[diseaseIndex][count].sampleTestPosCountIn);
                }
                myfile << "," << float(facilityStats[diseaseIndex][count].sampleTestPosTimeIn) << "," << facilityStats[diseaseIndex][count].sampleTestPosCountIn;
                if (facilityStats[diseaseIndex][count].sampleTestPosCountOut < 1) {
                    myfile << "," << "0";
                }
                else {
                    myfile << "," << float(facilityStats[diseaseIndex][count].sampleTestPosTimeOut)/float(facilityStats[diseaseIndex][count].sampleTestPosCountOut);
                }
                myfile << "," << float(facilityStats[diseaseIndex][count].sampleTestPosTimeOut) << "," << facilityStats[diseaseIndex][count].sampleTestPosCountOut;
               if(debug2) myfile << "," << "marker";
               
               myfile << "," << facilityStats[diseaseIndex][count].countPInfTreated+facilityStats[diseaseIndex][count].countPUninfTreated << "," << facilityStats[diseaseIndex][count].countPInfTreated << "," << facilityStats[diseaseIndex][count].countPUninfTreated << "," << facilityStats[diseaseIndex][count].countPTC << "," << facilityStats[diseaseIndex][count].countPTD << "," << facilityStats[diseaseIndex][count].countPTR  << "," << facilityStats[diseaseIndex][count].countRetreat << "," << facilityStats[diseaseIndex][count].countRetreatCure << "," << facilityStats[diseaseIndex][count].countRetreatDead;
               if ((facilityStats[diseaseIndex][count].countPInfTreatedIn + facilityStats[diseaseIndex][count].countPInfTreatedOut) < 1) {
                    myfile << "," << "0";
               }
               else {
                    myfile << "," << float (facilityStats[diseaseIndex][count].onsetToTreat)/float(facilityStats[diseaseIndex][count].countPInfTreatedIn + facilityStats[diseaseIndex][count].countPInfTreatedOut);
               }
               myfile << "," << float (facilityStats[diseaseIndex][count].onsetToTreat) << "," << (facilityStats[diseaseIndex][count].countPInfTreatedIn + facilityStats[diseaseIndex][count].countPInfTreatedOut);
               if (facilityStats[diseaseIndex][count].countPInfTreatedIn < 1) {
                    myfile << "," << "0";
               }
               else {
                    myfile << "," << float (facilityStats[diseaseIndex][count].onsetToTreatIn)/float(facilityStats[diseaseIndex][count].countPInfTreatedIn);
               }
               myfile << "," << float (facilityStats[diseaseIndex][count].onsetToTreatIn) << "," << facilityStats[diseaseIndex][count].countPInfTreatedIn;
               if (facilityStats[diseaseIndex][count].countPInfTreatedOut < 1) {
                    myfile << "," << "0";
               }
               else {
                    myfile << "," << float (facilityStats[diseaseIndex][count].onsetToTreatOut)/float(facilityStats[diseaseIndex][count].countPInfTreatedOut);
               }
               myfile << "," << float (facilityStats[diseaseIndex][count].onsetToTreatOut) << "," << facilityStats[diseaseIndex][count].countPInfTreatedOut;
               if(debug2) myfile << "," << "marker";
               
               if (facilityStats[diseaseIndex][count].sampleReturnToDeathCount < 1) {
                    myfile << "," << "0";
                }
                else {
                    myfile << "," << float (facilityStats[diseaseIndex][count].sampleReturnToDeathTime)/float(facilityStats[diseaseIndex][count].sampleReturnToDeathCount);
                }
                myfile << "," << float (facilityStats[diseaseIndex][count].sampleReturnToDeathTime) << "," << facilityStats[diseaseIndex][count].sampleReturnToDeathCount;
                if (facilityStats[diseaseIndex][count].sampleReturnToDeathPreDiagLtoFCount < 1) {
                    myfile << "," << "0";
                }
                else {
                    myfile << "," << float (facilityStats[diseaseIndex][count].sampleReturnToDeathPreDiagLtoFTime)/float(facilityStats[diseaseIndex][count].sampleReturnToDeathPreDiagLtoFCount);
                }
                myfile << "," << float (facilityStats[diseaseIndex][count].sampleReturnToDeathPreDiagLtoFTime) << "," << facilityStats[diseaseIndex][count].sampleReturnToDeathPreDiagLtoFCount;
                if (facilityStats[diseaseIndex][count].sampleReturnToDeathInTrtCount < 1) {
                    myfile << "," << "0";
                }
                else {
                    myfile << "," << float (facilityStats[diseaseIndex][count].sampleReturnToDeathInTrtTime)/float(facilityStats[diseaseIndex][count].sampleReturnToDeathInTrtCount);
                }
                myfile << "," << float (facilityStats[diseaseIndex][count].sampleReturnToDeathInTrtTime) << "," << facilityStats[diseaseIndex][count].sampleReturnToDeathInTrtCount;
                if (facilityStats[diseaseIndex][count].sampleReturnToDeathInRetrtCount < 1) {
                    myfile << "," << "0";
                }
                else {
                    myfile << "," << float (facilityStats[diseaseIndex][count].sampleReturnToDeathInRetrtTime)/float(facilityStats[diseaseIndex][count].sampleReturnToDeathInRetrtCount);
                }
                myfile << "," << float (facilityStats[diseaseIndex][count].sampleReturnToDeathInRetrtTime) << "," << facilityStats[diseaseIndex][count].sampleReturnToDeathInRetrtCount;
                if (facilityStats[diseaseIndex][count].sampleReturnToExitCount < 1) {
                    myfile << "," << "0";
                }
                else {
                    myfile << "," << float (facilityStats[diseaseIndex][count].sampleReturnToExitTime)/float(facilityStats[diseaseIndex][count].sampleReturnToExitCount);
                }
                myfile << "," << float (facilityStats[diseaseIndex][count].sampleReturnToExitTime) << "," << facilityStats[diseaseIndex][count].sampleReturnToExitCount;
                myfile << "," << facilityStats[diseaseIndex][count].test1CountIn + facilityStats[diseaseIndex][count].test1CountOut << "," << facilityStats[diseaseIndex][count].test1CountIn << "," << facilityStats[diseaseIndex][count].test1CountOut << "," << facilityStats[diseaseIndex][count].test2Count << "," << facilityStats[diseaseIndex][count]. test2PosCount;
                if (facilityStats[diseaseIndex][count].test2PosCount < 1) {
                    myfile << "," << "0";
                }
                else {
                    myfile << "," << float (facilityStats[diseaseIndex][count].test2TruePosCount)/float(facilityStats[diseaseIndex][count].test2PosCount);
                }
                myfile << "," << facilityStats[diseaseIndex][count].test2NegCount;
                if (facilityStats[diseaseIndex][count].test2NegCount < 1) {
                    myfile << "," << "0";
                }
                else {
                    myfile << "," << float (facilityStats[diseaseIndex][count].test2TrueNegCount)/float(facilityStats[diseaseIndex][count].test2NegCount);
                }
                myfile << "," << facilityStats[diseaseIndex][count].test1PosCount;
                if (facilityStats[diseaseIndex][count].test1PosCount < 1) {
                    myfile << "," << "0";
                }
                else {
                    myfile << "," << float (facilityStats[diseaseIndex][count].test1TruePosCount)/float(facilityStats[diseaseIndex][count].test1PosCount);
                }
                myfile << "," << facilityStats[diseaseIndex][count].test1NegCount;
                if (facilityStats[diseaseIndex][count].test1NegCount < 1) {
                    myfile << "," << "0";
                }
                else {
                    myfile << "," << float (facilityStats[diseaseIndex][count].test1TrueNegCount)/float(facilityStats[diseaseIndex][count].test1NegCount);
                }
                if(debug2) myfile << "," << "marker";
               
               myfile << "," << facilityStats[diseaseIndex][count].countP << "," << facilityStats[diseaseIndex][count].countPI << "," << facilityStats[diseaseIndex][count].countPU  << "," << facilityStats[diseaseIndex][count].countPT;
               myfile << "," << facilityStats[diseaseIndex][count].sampleWaitTestCount << "," << facilityStats[diseaseIndex][count].sampleWaitTestCountIn << "," << facilityStats[diseaseIndex][count].sampleWaitTestCountOut;
               myfile << "," << facilityStats[diseaseIndex][count].countPreTestLtoFOutTested << "," << facilityStats[diseaseIndex][count].patientNotNotified << "," << facilityStats[diseaseIndex][count].patientNotNotifiedPos << "\n";

               myfile6 << facilityStats[diseaseIndex][count].name << ","<< facilityStats[diseaseIndex][count].id << ","<< facilityStats[diseaseIndex][count].tier << "," << facilityStats[diseaseIndex][count].district << "," << facilityStats[diseaseIndex][count].region;
               myfile6 << "," << newPop << "," << diseaseList[diseaseIndex] << "," << prevalence ;
               myfile6 << "," << facilityStats[diseaseIndex][count].countP << "," << facilityStats[diseaseIndex][count].countPInf << "," << facilityStats[diseaseIndex][count].countPLtoF;
               myfile6 << "," << facilityStats[diseaseIndex][count].outCollectedCount + facilityStats[diseaseIndex][count].inCollectedCount << "," << facilityStats[diseaseIndex][count].outCollectedCount << "," << facilityStats[diseaseIndex][count].inCollectedCount;
               if ((facilityStats[diseaseIndex][count].sampleWaitForResourceInCount + facilityStats[diseaseIndex][count].sampleWaitForResourceOutCount) < 1) {
                    myfile6 << "," << "0";
               }
               else {
                    myfile6 << "," << float(facilityStats[diseaseIndex][count].sampleWaitForResourceInTime + facilityStats[diseaseIndex][count].sampleWaitForResourceOutTime)/float(facilityStats[diseaseIndex][count].sampleWaitForResourceInCount + facilityStats[diseaseIndex][count].sampleWaitForResourceOutCount);
               }
               if (facilityStats[diseaseIndex][count].sampleWaitForResourceInCount < 1) {
                    myfile6 << "," << "0";
               }
               else {
                    myfile6 << "," << float(facilityStats[diseaseIndex][count].sampleWaitForResourceInTime)/float(facilityStats[diseaseIndex][count].sampleWaitForResourceInCount);
               }
               if (facilityStats[diseaseIndex][count].sampleWaitForResourceOutCount < 1) {
                    myfile6 << "," << "0" << ",";
               }
               else {
                    myfile6 << "," << float(facilityStats[diseaseIndex][count].sampleWaitForResourceOutTime)/float(facilityStats[diseaseIndex][count].sampleWaitForResourceOutCount) << ",";
               }

               if (facilityStats[diseaseIndex][count].sampleTransCount < 1) {
                    myfile6 << "0" << "," << facilityStats[diseaseIndex][count].sampleTransCount;
               }
               else {
                    myfile6 << float(facilityStats[diseaseIndex][count].sampleTransTime)/float(facilityStats[diseaseIndex][count].sampleTransCount) << "," << facilityStats[diseaseIndex][count].sampleTransCount;
               }
               myfile6 << "," << facilityStats[diseaseIndex][count].sampleTestedInCount + facilityStats[diseaseIndex][count].sampleTestedOutCount << "," << facilityStats[diseaseIndex][count].sampleTestedInCount << "," << facilityStats[diseaseIndex][count].sampleTestedOutCount;
               //myfile6 << "," << facilityStats[diseaseIndex][count].sampleLtoFCount;
               if (facilityStats[diseaseIndex][count].sampleCollectToTestInCount < 1) {
                    myfile6 << "," << "0";
               }
               else {
                    myfile6 << "," << float(facilityStats[diseaseIndex][count].sampleCollectToTestInTime)/float(facilityStats[diseaseIndex][count].sampleCollectToTestInCount);
               }
               myfile6 << "," << float(facilityStats[diseaseIndex][count].sampleCollectToTestInTime) << "," << facilityStats[diseaseIndex][count].sampleCollectToTestInCount;
               if (facilityStats[diseaseIndex][count].sampleCollectToTestOutCount < 1) {
                    myfile6 << "," << "0";
               }
               else {
                    myfile6 << "," << float(facilityStats[diseaseIndex][count].sampleCollectToTestOutTime)/float(facilityStats[diseaseIndex][count].sampleCollectToTestOutCount);
               }
               myfile6 << "," << float(facilityStats[diseaseIndex][count].sampleCollectToTestOutTime) << "," << facilityStats[diseaseIndex][count].sampleCollectToTestOutCount;
               myfile6 << "," << facilityStats[diseaseIndex][count].countTestPos << ",";
               if (totalSampleCountPos  > 0) {
                    myfile6 << float(facilityStats[diseaseIndex][count].countTruePos)/float(totalSampleCountPos) << ",";
                    myfile6 << float(facilityStats[diseaseIndex][count].countFalsePos)/float(totalSampleCountPos) << ",";
               }
               else {
                    myfile6 << "0" << "," << "0" << ",";
               }
               myfile6 << facilityStats[diseaseIndex][count].countTestNeg << ",";
               if (totalSampleCountNeg  > 0) {
                    myfile6 << float(facilityStats[diseaseIndex][count].countTrueNeg)/float(totalSampleCountNeg) << ",";
                    myfile6 << float(facilityStats[diseaseIndex][count].countFalseNeg)/float(totalSampleCountNeg);
               }
               else {
                    myfile6 << "0" << "," << "0";
               }
               if (debug2) myfile6 << "," << "marker";

                myfile6 << "," << facilityStats[diseaseIndex][count].countMDInf << "," << facilityStats[diseaseIndex][count].countMDUninf;
               myfile6 << "," << facilityStats[diseaseIndex][count].countMDInf+facilityStats[diseaseIndex][count].countTestPos + facilityStats[diseaseIndex][count].countMDUninf;
               myfile6 << "," << facilityStats[diseaseIndex][count].countFalsePos << "," << facilityStats[diseaseIndex][count].countFalseNeg;
               myfile6 << "," << facilityStats[diseaseIndex][count].countDiagnosis << "," << facilityStats[diseaseIndex][count].countDiagnosisIn << "," << facilityStats[diseaseIndex][count].countDiagnosisOut << "," << facilityStats[diseaseIndex][count].countPreTestLtoFOut  << "," << facilityStats[diseaseIndex][count].countPreTestLtoFOutMinusTestedSamp << "," << facilityStats[diseaseIndex][count].countPreTestLtoFDead << "," << facilityStats[diseaseIndex][count].countPreTestLtoFORepresent;
               if (facilityStats[diseaseIndex][count].sampleTestPosCountIn < 1) {
                    myfile6 << "," << "0";
                }
                else {
                    myfile6 << "," << float(facilityStats[diseaseIndex][count].sampleTestPosTimeIn)/float(facilityStats[diseaseIndex][count].sampleTestPosCountIn);
                }
                myfile6 << "," << float(facilityStats[diseaseIndex][count].sampleTestPosTimeIn) << "," << facilityStats[diseaseIndex][count].sampleTestPosCountIn;
                if (facilityStats[diseaseIndex][count].sampleTestPosCountOut < 1) {
                    myfile6 << "," << "0";
                }
                else {
                    myfile6 << "," << float(facilityStats[diseaseIndex][count].sampleTestPosTimeOut)/float(facilityStats[diseaseIndex][count].sampleTestPosCountOut);
                }
                myfile6 << "," << float(facilityStats[diseaseIndex][count].sampleTestPosTimeOut) << "," << facilityStats[diseaseIndex][count].sampleTestPosCountOut;
                if(debug2) myfile6 << "," << "marker";
               
               myfile6 << "," << facilityStats[diseaseIndex][count].countPInfTreated+facilityStats[diseaseIndex][count].countPUninfTreated << "," << facilityStats[diseaseIndex][count].countPInfTreated << "," << facilityStats[diseaseIndex][count].countPUninfTreated << "," << facilityStats[diseaseIndex][count].countPTC << "," << facilityStats[diseaseIndex][count].countPTD << "," << facilityStats[diseaseIndex][count].countPTR  << "," << facilityStats[diseaseIndex][count].countRetreat << "," << facilityStats[diseaseIndex][count].countRetreatCure << "," << facilityStats[diseaseIndex][count].countRetreatDead;
               if ((facilityStats[diseaseIndex][count].countPInfTreatedIn + facilityStats[diseaseIndex][count].countPInfTreatedOut) < 1) {
                    myfile6 << "," << "0";
               }
               else {
                    myfile6 << "," << float (facilityStats[diseaseIndex][count].onsetToTreat)/float(facilityStats[diseaseIndex][count].countPInfTreatedIn + facilityStats[diseaseIndex][count].countPInfTreatedOut);
               }
               myfile6 << "," << float (facilityStats[diseaseIndex][count].onsetToTreat) << "," << (facilityStats[diseaseIndex][count].countPInfTreatedIn + facilityStats[diseaseIndex][count].countPInfTreatedOut);
               if (facilityStats[diseaseIndex][count].countPInfTreatedIn < 1) {
                    myfile6 << "," << "0";
               }
               else {
                    myfile6 << "," << float (facilityStats[diseaseIndex][count].onsetToTreatIn)/float(facilityStats[diseaseIndex][count].countPInfTreatedIn);
               }
               myfile6 << "," << float (facilityStats[diseaseIndex][count].onsetToTreatIn) << "," << facilityStats[diseaseIndex][count].countPInfTreatedIn;
               if (facilityStats[diseaseIndex][count].countPInfTreatedOut < 1) {
                    myfile6 << "," << "0";
               }
               else {
                    myfile6 << "," << float (facilityStats[diseaseIndex][count].onsetToTreatOut)/float(facilityStats[diseaseIndex][count].countPInfTreatedOut);
               }
               myfile6 << "," << float (facilityStats[diseaseIndex][count].onsetToTreatOut) << "," << facilityStats[diseaseIndex][count].countPInfTreatedOut;
               if(debug2) myfile6 << "," << "marker";
               
               if (facilityStats[diseaseIndex][count].sampleReturnToDeathCount < 1) {
                    myfile6 << "," << "0";
                }
                else {
                    myfile6 << "," << float (facilityStats[diseaseIndex][count].sampleReturnToDeathTime)/float(facilityStats[diseaseIndex][count].sampleReturnToDeathCount);
                }
                myfile6 << "," << float (facilityStats[diseaseIndex][count].sampleReturnToDeathTime) << "," << facilityStats[diseaseIndex][count].sampleReturnToDeathCount;
                if (facilityStats[diseaseIndex][count].sampleReturnToDeathPreDiagLtoFCount < 1) {
                    myfile6 << "," << "0";
                }
                else {
                    myfile6 << "," << float (facilityStats[diseaseIndex][count].sampleReturnToDeathPreDiagLtoFTime)/float(facilityStats[diseaseIndex][count].sampleReturnToDeathPreDiagLtoFCount);
                }
                myfile6 << "," << float (facilityStats[diseaseIndex][count].sampleReturnToDeathPreDiagLtoFTime) << "," << facilityStats[diseaseIndex][count].sampleReturnToDeathPreDiagLtoFCount;
                if (facilityStats[diseaseIndex][count].sampleReturnToDeathInTrtCount < 1) {
                    myfile6 << "," << "0";
                }
                else {
                    myfile6 << "," << float (facilityStats[diseaseIndex][count].sampleReturnToDeathInTrtTime)/float(facilityStats[diseaseIndex][count].sampleReturnToDeathInTrtCount);
                }
                myfile6 << "," << float (facilityStats[diseaseIndex][count].sampleReturnToDeathInTrtTime) << "," << facilityStats[diseaseIndex][count].sampleReturnToDeathInTrtCount;
                if (facilityStats[diseaseIndex][count].sampleReturnToDeathInRetrtCount < 1) {
                    myfile6 << "," << "0";
                }
                else {
                    myfile6 << "," << float (facilityStats[diseaseIndex][count].sampleReturnToDeathInRetrtTime)/float(facilityStats[diseaseIndex][count].sampleReturnToDeathInRetrtCount);
                }
                myfile6 << "," << float (facilityStats[diseaseIndex][count].sampleReturnToDeathInRetrtTime) << "," << facilityStats[diseaseIndex][count].sampleReturnToDeathInRetrtCount;
                if (facilityStats[diseaseIndex][count].sampleReturnToExitCount < 1) {
                    myfile6 << "," << "0";
                }
                else {
                    myfile6 << "," << float (facilityStats[diseaseIndex][count].sampleReturnToExitTime)/float(facilityStats[diseaseIndex][count].sampleReturnToExitCount);
                }
                myfile6 << "," << float (facilityStats[diseaseIndex][count].sampleReturnToExitTime) << "," << facilityStats[diseaseIndex][count].sampleReturnToExitCount;
                myfile6 << "," << facilityStats[diseaseIndex][count].test1CountIn + facilityStats[diseaseIndex][count].test1CountOut << "," << facilityStats[diseaseIndex][count].test1CountIn << "," << facilityStats[diseaseIndex][count].test1CountOut << "," << facilityStats[diseaseIndex][count].test2Count << "," << facilityStats[diseaseIndex][count]. test2PosCount;
                if (facilityStats[diseaseIndex][count].test2PosCount < 1) {
                    myfile6 << "," << "0";
                }
                else {
                    myfile6 << "," << float (facilityStats[diseaseIndex][count].test2TruePosCount)/float(facilityStats[diseaseIndex][count].test2PosCount);
                }
                myfile6 << "," << facilityStats[diseaseIndex][count].test2NegCount;
                if (facilityStats[diseaseIndex][count].test2NegCount < 1) {
                    myfile6 << "," << "0";
                }
                else {
                    myfile6 << "," << float (facilityStats[diseaseIndex][count].test2TrueNegCount)/float(facilityStats[diseaseIndex][count].test2NegCount);
                }
                myfile6 << "," << facilityStats[diseaseIndex][count].test1PosCount;
                if (facilityStats[diseaseIndex][count].test1PosCount < 1) {
                    myfile6 << "," << "0";
                }
                else {
                    myfile6 << "," << float (facilityStats[diseaseIndex][count].test1TruePosCount)/float(facilityStats[diseaseIndex][count].test1PosCount);
                }
                myfile6 << "," << facilityStats[diseaseIndex][count].test1NegCount;
                if (facilityStats[diseaseIndex][count].test1NegCount < 1) {
                    myfile6 << "," << "0";
                }
                else {
                    myfile6 << "," << float (facilityStats[diseaseIndex][count].test1TrueNegCount)/float(facilityStats[diseaseIndex][count].test1NegCount);
                }
                if(debug2) myfile6 << "," << "marker";
               
                myfile6 << "," << facilityStats[diseaseIndex][count].countP << "," << facilityStats[diseaseIndex][count].countPI << "," << facilityStats[diseaseIndex][count].countPU  << "," << facilityStats[diseaseIndex][count].countPT;
                myfile6 << "," << facilityStats[diseaseIndex][count].sampleWaitTestCount << "," << facilityStats[diseaseIndex][count].sampleWaitTestCountIn << "," << facilityStats[diseaseIndex][count].sampleWaitTestCountOut;
                myfile6 << "," << facilityStats[diseaseIndex][count].countPreTestLtoFOutTested << "," << facilityStats[diseaseIndex][count].patientNotNotified << "," << facilityStats[diseaseIndex][count].patientNotNotifiedPos << "\n";
            } 

            // update totalFacility statisticst
            // update population by population growth if it goes on more that 1 year
            newPop = facilityStats[diseaseIndex][count].pop;
            startIndex = referenceYear - 2015;
            endIndex = startIndex + noOfYears;
            if (noOfYears > 0) {

                for (int i = startIndex; i < endIndex; i++) {

                    // multiply by growth rate for x number of years
                    newPop = (int) (newPop * popGrowthRateArray[i][1]);
                }
            }
            totalFacility.pop = totalFacility.pop + newPop;
            totalFacility.countP = totalFacility.countP + facilityStats[diseaseIndex][count].countP;
            totalFacility.countPLtoF = totalFacility.countPLtoF + facilityStats[diseaseIndex][count].countPLtoF;
            totalFacility.countPInf = totalFacility.countPInf + facilityStats[diseaseIndex][count].countPInf;
            totalFacility.countPInfTreated = totalFacility.countPInfTreated + facilityStats[diseaseIndex][count].countPInfTreated;
            totalFacility.countPInfTreatedIn = totalFacility.countPInfTreatedIn + facilityStats[diseaseIndex][count].countPInfTreatedIn;
            totalFacility.countPInfTreatedOut = totalFacility.countPInfTreatedOut + facilityStats[diseaseIndex][count].countPInfTreatedOut;
            totalFacility.countPUninfTreated = totalFacility.countPUninfTreated + facilityStats[diseaseIndex][count].countPUninfTreated;
            totalFacility.countPU = totalFacility.countPU + facilityStats[diseaseIndex][count].countPU;
            totalFacility.countPI = totalFacility.countPI + facilityStats[diseaseIndex][count].countPI;
            totalFacility.countPT = totalFacility.countPT + facilityStats[diseaseIndex][count].countPT;
            totalFacility.countPR = totalFacility.countPR + facilityStats[diseaseIndex][count].countPR;
            totalFacility.countPUT = totalFacility.countPUT + facilityStats[diseaseIndex][count].countPUT;
            totalFacility.countPUTFN = totalFacility.countPUTFN + facilityStats[diseaseIndex][count].countPUTFN;
            totalFacility.countPUTNS = totalFacility.countPUTNS + facilityStats[diseaseIndex][count].countPUTNS;
            totalFacility.countPUTLtoF = totalFacility.countPUTLtoF + facilityStats[diseaseIndex][count].countPUTLtoF;
            totalFacility.countPC = totalFacility.countPC + facilityStats[diseaseIndex][count].countPC;
            totalFacility.countPDI = totalFacility.countPDI + facilityStats[diseaseIndex][count].countPDI;
            totalFacility.countPDIT = totalFacility.countPDIT + facilityStats[diseaseIndex][count].countPDIT;
            totalFacility.countPDIUT = totalFacility.countPDIUT + facilityStats[diseaseIndex][count].countPDIUT;
            totalFacility.countPDIUTSR = totalFacility.countPDIUTSR + facilityStats[diseaseIndex][count].countPDIUTSR;
            totalFacility.countPDIUTSNR = totalFacility.countPDIUTSNR + facilityStats[diseaseIndex][count].countPDIUTSNR;
            totalFacility.countPDIUTLtF = totalFacility.countPDIUTLtF + facilityStats[diseaseIndex][count].countPDIUTLtF;
            totalFacility.countPDU = totalFacility.countPDU + facilityStats[diseaseIndex][count].countPDU;
            totalFacility.countPDOA = totalFacility.countPDOA + facilityStats[diseaseIndex][count].countPDOA;
            totalFacility.countPDIUTPTLtoF = totalFacility.countPDIUTPTLtoF + facilityStats[diseaseIndex][count].countPDIUTPTLtoF;
            totalFacility.countPDIUTSRLtoF = totalFacility.countPDIUTSRLtoF + facilityStats[diseaseIndex][count].countPDIUTSRLtoF;
            totalFacility.countPDITLtoF = totalFacility.countPDITLtoF + facilityStats[diseaseIndex][count].countPDITLtoF;
            totalFacility.countPTC = totalFacility.countPTC + facilityStats[diseaseIndex][count].countPTC;
            totalFacility.countPTD = totalFacility.countPTD + facilityStats[diseaseIndex][count].countPTD;
            totalFacility.countPTR = totalFacility.countPTR + facilityStats[diseaseIndex][count].countPTR;
            totalFacility.countRetreat = totalFacility.countRetreat + facilityStats[diseaseIndex][count].countRetreat;
            totalFacility.countRetreatCure = totalFacility.countRetreatCure + facilityStats[diseaseIndex][count].countRetreatCure;
            totalFacility.countRetreatDead = totalFacility.countRetreatDead + facilityStats[diseaseIndex][count].countRetreatDead;
            totalFacility.countPO = totalFacility.countPO + facilityStats[diseaseIndex][count].countPO;
            totalFacility.countPUI = totalFacility.countPUI + facilityStats[diseaseIndex][count].countPUI;
            totalFacility.countPUINR = totalFacility.countPUINR + facilityStats[diseaseIndex][count].countPUINR;
            totalFacility.countPUIUT = totalFacility.countPUIUT + facilityStats[diseaseIndex][count].countPUIUT;
            totalFacility.countMDInf = totalFacility.countMDInf + facilityStats[diseaseIndex][count].countMDInf;
            totalFacility.countMDUninf = totalFacility.countMDUninf + facilityStats[diseaseIndex][count].countMDUninf;
            totalFacility.countTestPos = totalFacility.countTestPos + facilityStats[diseaseIndex][count].countTestPos;
            totalFacility.countTestNeg = totalFacility.countTestNeg + facilityStats[diseaseIndex][count].countTestNeg;
            totalFacility.countTruePos = totalFacility.countTruePos + facilityStats[diseaseIndex][count].countTruePos;
            totalFacility.countFalsePos = totalFacility.countFalsePos + facilityStats[diseaseIndex][count].countFalsePos;
            totalFacility.countTrueNeg = totalFacility.countTrueNeg + facilityStats[diseaseIndex][count].countTrueNeg;
            totalFacility.countFalseNeg = totalFacility.countFalseNeg + facilityStats[diseaseIndex][count].countFalseNeg;
            totalFacility.countDiagnosis = totalFacility.countDiagnosis + facilityStats[diseaseIndex][count].countDiagnosis;
            totalFacility.countDiagnosisIn = totalFacility.countDiagnosisIn + facilityStats[diseaseIndex][count].countDiagnosisIn;
            totalFacility.countDiagnosisOut = totalFacility.countDiagnosisOut + facilityStats[diseaseIndex][count].countDiagnosisOut;
            totalFacility.countPreTestLtoFOut = totalFacility.countPreTestLtoFOut + facilityStats[diseaseIndex][count].countPreTestLtoFOut;
            totalFacility.countPreTestLtoFOutTested = totalFacility.countPreTestLtoFOutTested + facilityStats[diseaseIndex][count].countPreTestLtoFOutTested;
            totalFacility.countPreTestLtoFOutUnTested = totalFacility.countPreTestLtoFOutUnTested + facilityStats[diseaseIndex][count].countPreTestLtoFOutUnTested;
            totalFacility.countPreTestLtoFOutUnTested2 = totalFacility.countPreTestLtoFOutUnTested2 + facilityStats[diseaseIndex][count].countPreTestLtoFOutUnTested2;
            totalFacility.countPreTestLtoFDead = totalFacility.countPreTestLtoFDead + facilityStats[diseaseIndex][count].countPreTestLtoFDead;
            totalFacility.countPreTestLtoFORepresent = totalFacility.countPreTestLtoFORepresent + facilityStats[diseaseIndex][count].countPreTestLtoFORepresent;
            totalFacility.onsetToTreat = totalFacility.onsetToTreat + facilityStats[diseaseIndex][count].onsetToTreat;
            totalFacility.onsetToTreatIn  = totalFacility.onsetToTreatIn  + facilityStats[diseaseIndex][count].onsetToTreatIn ;
            totalFacility.onsetToTreatOut = totalFacility.onsetToTreatOut + facilityStats[diseaseIndex][count].onsetToTreatOut;
            totalFacility.timeInfectious = totalFacility.timeInfectious + facilityStats[diseaseIndex][count].timeInfectious;
            totalFacility.sampleTransTime = totalFacility.sampleTransTime + facilityStats[diseaseIndex][count].sampleTransTime;
            totalFacility.sampleTransCount = totalFacility.sampleTransCount + facilityStats[diseaseIndex][count].sampleTransCount;
            totalFacility.sampleWaitForResourceInTime = totalFacility.sampleWaitForResourceInTime + facilityStats[diseaseIndex][count].sampleWaitForResourceInTime;
            totalFacility.sampleWaitForResourceInCount = totalFacility.sampleWaitForResourceInCount + facilityStats[diseaseIndex][count].sampleWaitForResourceInCount;
            totalFacility.sampleWaitForResourceOutTime = totalFacility.sampleWaitForResourceOutTime + facilityStats[diseaseIndex][count].sampleWaitForResourceOutTime;
            totalFacility.sampleWaitForResourceOutCount = totalFacility.sampleWaitForResourceOutCount + facilityStats[diseaseIndex][count].sampleWaitForResourceOutCount;
            totalFacility.sampleCollectToTestInTime = totalFacility.sampleCollectToTestInTime + facilityStats[diseaseIndex][count].sampleCollectToTestInTime;
            totalFacility.sampleCollectToTestOutTime = totalFacility.sampleCollectToTestOutTime + facilityStats[diseaseIndex][count].sampleCollectToTestOutTime;
            totalFacility.sampleCollectToTestInCount = totalFacility.sampleCollectToTestInCount + facilityStats[diseaseIndex][count].sampleCollectToTestInCount;
            totalFacility.sampleCollectToTestOutCount = totalFacility.sampleCollectToTestOutCount + facilityStats[diseaseIndex][count].sampleCollectToTestOutCount;
            totalFacility.sampleTestTime = totalFacility.sampleTestTime + facilityStats[diseaseIndex][count].sampleTestTime;
            totalFacility.sampleTestPosTimeIn = totalFacility.sampleTestPosTimeIn + facilityStats[diseaseIndex][count].sampleTestPosTimeIn;
            totalFacility.sampleTestPosTimeOut = totalFacility.sampleTestPosTimeOut + facilityStats[diseaseIndex][count].sampleTestPosTimeOut;
            totalFacility.sampleTestPosCountIn = totalFacility.sampleTestPosCountIn + facilityStats[diseaseIndex][count].sampleTestPosCountIn;
            totalFacility.sampleTestPosCountOut = totalFacility.sampleTestPosCountOut + facilityStats[diseaseIndex][count].sampleTestPosCountOut;
            totalFacility.sampleReturnTime = totalFacility.sampleReturnTime + facilityStats[diseaseIndex][count].sampleReturnTime;
            totalFacility.sampleReturnedCount = totalFacility.sampleReturnedCount + facilityStats[diseaseIndex][count].sampleReturnedCount;
            totalFacility.sampleReturnedInCount = totalFacility.sampleReturnedInCount + facilityStats[diseaseIndex][count].sampleReturnedInCount;
            totalFacility.sampleReturnedOutCount = totalFacility.sampleReturnedOutCount + facilityStats[diseaseIndex][count].sampleReturnedOutCount;
            totalFacility.sampleTestedInCount = totalFacility.sampleTestedInCount + facilityStats[diseaseIndex][count].sampleTestedInCount;
            totalFacility.sampleTestedOutCount = totalFacility.sampleTestedOutCount + facilityStats[diseaseIndex][count].sampleTestedOutCount;
            totalFacility.sampleNotTestedInCount = totalFacility.sampleNotTestedInCount + facilityStats[diseaseIndex][count].sampleNotTestedInCount;
            totalFacility.sampleNotTestedOutCount = totalFacility.sampleNotTestedOutCount + facilityStats[diseaseIndex][count].sampleNotTestedOutCount;
            totalFacility.sampleReturnToDeathTime = totalFacility.sampleReturnToDeathTime + facilityStats[diseaseIndex][count].sampleReturnToDeathTime;
            totalFacility.sampleReturnToDeathPreDiagLtoFTime = totalFacility.sampleReturnToDeathPreDiagLtoFTime + facilityStats[diseaseIndex][count].sampleReturnToDeathPreDiagLtoFTime;
            totalFacility.sampleReturnToDeathInTrtTime = totalFacility.sampleReturnToDeathInTrtTime + facilityStats[diseaseIndex][count].sampleReturnToDeathInTrtTime;
            totalFacility.sampleReturnToDeathInRetrtTime = totalFacility.sampleReturnToDeathInRetrtTime + facilityStats[diseaseIndex][count].sampleReturnToDeathInRetrtTime;
            totalFacility.sampleReturnToExitTime = totalFacility.sampleReturnToExitTime + facilityStats[diseaseIndex][count].sampleReturnToExitTime;
            totalFacility.sampleReturnToDeathCount = totalFacility.sampleReturnToDeathCount + facilityStats[diseaseIndex][count].sampleReturnToDeathCount;
            totalFacility.sampleReturnToDeathPreDiagLtoFCount = totalFacility.sampleReturnToDeathPreDiagLtoFCount + facilityStats[diseaseIndex][count].sampleReturnToDeathPreDiagLtoFCount;
            totalFacility.sampleReturnToDeathInTrtCount = totalFacility.sampleReturnToDeathInTrtCount + facilityStats[diseaseIndex][count].sampleReturnToDeathInTrtCount;
            totalFacility.sampleReturnToDeathInRetrtCount = totalFacility.sampleReturnToDeathInRetrtCount + facilityStats[diseaseIndex][count].sampleReturnToDeathInRetrtCount;
            totalFacility.sampleReturnToExitCount = totalFacility.sampleReturnToExitCount + facilityStats[diseaseIndex][count].sampleReturnToExitCount;
            totalFacility.sampleLtoFCount = totalFacility.sampleLtoFCount + facilityStats[diseaseIndex][count].sampleLtoFCount;
            totalFacility.sampleWaitTestCount = totalFacility.sampleWaitTestCount + facilityStats[diseaseIndex][count].sampleWaitTestCount;
            totalFacility.sampleWaitTestCountIn = totalFacility.sampleWaitTestCountIn + facilityStats[diseaseIndex][count].sampleWaitTestCountIn;
            totalFacility.sampleWaitTestCountOut = totalFacility.sampleWaitTestCountOut + facilityStats[diseaseIndex][count].sampleWaitTestCountOut;
            totalFacility.patientNotNotified = totalFacility.patientNotNotified + facilityStats[diseaseIndex][count].patientNotNotified;
            totalFacility.patientNotNotifiedPos = totalFacility.patientNotNotifiedPos + facilityStats[diseaseIndex][count].patientNotNotifiedPos;
            totalFacility.patientNotNotifiedIn = totalFacility.patientNotNotifiedIn + facilityStats[diseaseIndex][count].patientNotNotifiedIn;
            totalFacility.patientNotNotifiedOut = totalFacility.patientNotNotifiedOut + facilityStats[diseaseIndex][count].patientNotNotifiedOut;
            totalFacility.inTestCount = totalFacility.inTestCount + facilityStats[diseaseIndex][count].inTestCount;
            totalFacility.outTestCount = totalFacility.outTestCount + facilityStats[diseaseIndex][count].outTestCount;
            totalFacility.inCollectedCount = totalFacility.inCollectedCount + facilityStats[diseaseIndex][count].inCollectedCount;
            totalFacility.outCollectedCount = totalFacility.outCollectedCount + facilityStats[diseaseIndex][count].outCollectedCount;
            totalFacility.test1Count = totalFacility.test1Count + facilityStats[diseaseIndex][count].test1Count;
            totalFacility.test1CountIn = totalFacility.test1CountIn + facilityStats[diseaseIndex][count].test1CountIn;
            totalFacility.test1CountOut = totalFacility.test1CountOut + facilityStats[diseaseIndex][count].test1CountOut;
            totalFacility.test1PosCount = totalFacility.test1PosCount + facilityStats[diseaseIndex][count].test1PosCount;
            totalFacility.test1TruePosCount = totalFacility.test1TruePosCount + facilityStats[diseaseIndex][count].test1TruePosCount;
            totalFacility.test1NegCount = totalFacility.test1NegCount + facilityStats[diseaseIndex][count].test1NegCount;
            totalFacility.test1TrueNegCount = totalFacility.test1TrueNegCount + facilityStats[diseaseIndex][count].test1TrueNegCount;
            totalFacility.test2Count = totalFacility.test2Count + facilityStats[diseaseIndex][count].test2Count;
            totalFacility.test2CountIn = totalFacility.test2CountIn + facilityStats[diseaseIndex][count].test2CountIn;
            totalFacility.test2CountOut = totalFacility.test2CountOut + facilityStats[diseaseIndex][count].test2CountOut;
            totalFacility.test2PosCount = totalFacility.test2PosCount + facilityStats[diseaseIndex][count].test2PosCount;
            totalFacility.test1TruePosCount = totalFacility.test1TruePosCount + facilityStats[diseaseIndex][count].test1TruePosCount;
            totalFacility.test2NegCount = totalFacility.test2NegCount + facilityStats[diseaseIndex][count].test2NegCount;
            totalFacility.test2TrueNegCount = totalFacility.test2TrueNegCount + facilityStats[diseaseIndex][count].test2TrueNegCount;
            totalFacility.test3Count = totalFacility.test3Count + facilityStats[diseaseIndex][count].test3Count;
            totalFacility.countPreTestLtoFOutMinusTestedSamp = totalFacility.countPreTestLtoFOutMinusTestedSamp + facilityStats[diseaseIndex][count].countPreTestLtoFOutMinusTestedSamp;
            totalFacility.dummy2Count = totalFacility.dummy2Count + facilityStats[diseaseIndex][count].dummy2Count;
            totalFacility.dummy3Count = totalFacility.dummy3Count + facilityStats[diseaseIndex][count].dummy3Count;
            totalFacility.dummy4Count = totalFacility.dummy4Count + facilityStats[diseaseIndex][count].dummy4Count;
            totalFacility.dummy5Count = totalFacility.dummy5Count + facilityStats[diseaseIndex][count].dummy5Count;
            totalFacility.dummy6Count = totalFacility.dummy6Count + facilityStats[diseaseIndex][count].dummy6Count;

            if (totalFacility.test1Name == "") {

                totalFacility.test1Name = facilityStats[diseaseIndex][count].test1Name;
            }
            if (totalFacility.test2Name == "") {

                totalFacility.test2Name = facilityStats[diseaseIndex][count].test2Name;
            }
            if (totalFacility.test3Name == "") {

                totalFacility.test3Name = facilityStats[diseaseIndex][count].test3Name;
            }

            // now update stats for regions - only compile if output is to file
            if (fileFlag) {

                regionNotFound = true;

                for (int i = 0; i < regionCount[diseaseIndex]; i++) {

                    if (region == regions[diseaseIndex][i].name) {
                        if (debug) std::cout << "stats: agents region found =  " << region << "\n";

                        //update statistics for that region
                        regionNotFound = false;

                        regions[diseaseIndex][i].region = regions[diseaseIndex][i].name;
                        regions[diseaseIndex][i].pop = regions[diseaseIndex][i].pop + facilityStats[diseaseIndex][count].pop;
                        regions[diseaseIndex][i].countP = regions[diseaseIndex][i].countP + facilityStats[diseaseIndex][count].countP;
                        regions[diseaseIndex][i].countPLtoF = regions[diseaseIndex][i].countPLtoF + facilityStats[diseaseIndex][count].countPLtoF;
                        regions[diseaseIndex][i].countPInf = regions[diseaseIndex][i].countPInf + facilityStats[diseaseIndex][count].countPInf;
                        regions[diseaseIndex][i].countPInfTreated = regions[diseaseIndex][i].countPInfTreated + facilityStats[diseaseIndex][count].countPInfTreated;
                        regions[diseaseIndex][i].countPInfTreatedIn = regions[diseaseIndex][i].countPInfTreatedIn + facilityStats[diseaseIndex][count].countPInfTreatedIn;
                        regions[diseaseIndex][i].countPInfTreatedOut = regions[diseaseIndex][i].countPInfTreatedOut + facilityStats[diseaseIndex][count].countPInfTreatedOut;
                        regions[diseaseIndex][i].countPUninfTreated = regions[diseaseIndex][i].countPUninfTreated + facilityStats[diseaseIndex][count].countPUninfTreated;
                        regions[diseaseIndex][i].countPU = regions[diseaseIndex][i].countPU + facilityStats[diseaseIndex][count].countPU;
                        regions[diseaseIndex][i].countPI = regions[diseaseIndex][i].countPI + facilityStats[diseaseIndex][count].countPI;
                        regions[diseaseIndex][i].countPT = regions[diseaseIndex][i].countPT + facilityStats[diseaseIndex][count].countPT;
                        regions[diseaseIndex][i].countPR = regions[diseaseIndex][i].countPR + facilityStats[diseaseIndex][count].countPR;
                        regions[diseaseIndex][i].countPUT = regions[diseaseIndex][i].countPUT + facilityStats[diseaseIndex][count].countPUT;
                        regions[diseaseIndex][i].countPUTFN = regions[diseaseIndex][i].countPUTFN + facilityStats[diseaseIndex][count].countPUTFN;
                        regions[diseaseIndex][i].countPUTNS = regions[diseaseIndex][i].countPUTNS + facilityStats[diseaseIndex][count].countPUTNS;
                        regions[diseaseIndex][i].countPUTLtoF = regions[diseaseIndex][i].countPUTLtoF + facilityStats[diseaseIndex][count].countPUTLtoF;
                        regions[diseaseIndex][i].countPC = regions[diseaseIndex][i].countPC + facilityStats[diseaseIndex][count].countPC;
                        regions[diseaseIndex][i].countPDI = regions[diseaseIndex][i].countPDI + facilityStats[diseaseIndex][count].countPDI;
                        regions[diseaseIndex][i].countPDIT = regions[diseaseIndex][i].countPDIT + facilityStats[diseaseIndex][count].countPDIT;
                        regions[diseaseIndex][i].countPDIUT = regions[diseaseIndex][i].countPDIUT + facilityStats[diseaseIndex][count].countPDIUT;
                        regions[diseaseIndex][i].countPDIUTSR = regions[diseaseIndex][i].countPDIUTSR + facilityStats[diseaseIndex][count].countPDIUTSR;
                        regions[diseaseIndex][i].countPDIUTSNR = regions[diseaseIndex][i].countPDIUTSNR + facilityStats[diseaseIndex][count].countPDIUTSNR;
                        regions[diseaseIndex][i].countPDIUTLtF = regions[diseaseIndex][i].countPDIUTLtF + facilityStats[diseaseIndex][count].countPDIUTLtF;
                        regions[diseaseIndex][i].countPDU = regions[diseaseIndex][i].countPDU + facilityStats[diseaseIndex][count].countPDU;
                        regions[diseaseIndex][i].countPDOA = regions[diseaseIndex][i].countPDOA + facilityStats[diseaseIndex][count].countPDOA;
                        regions[diseaseIndex][i].countPDIUTPTLtoF = regions[diseaseIndex][i].countPDIUTPTLtoF + facilityStats[diseaseIndex][count].countPDIUTPTLtoF;
                        regions[diseaseIndex][i].countPDIUTSRLtoF = regions[diseaseIndex][i].countPDIUTSRLtoF + facilityStats[diseaseIndex][count].countPDIUTSRLtoF;
                        regions[diseaseIndex][i].countPDITLtoF = regions[diseaseIndex][i].countPDITLtoF + facilityStats[diseaseIndex][count].countPDITLtoF;
                        regions[diseaseIndex][i].countPTC = regions[diseaseIndex][i].countPTC + facilityStats[diseaseIndex][count].countPTC;
                        regions[diseaseIndex][i].countPTD = regions[diseaseIndex][i].countPTD + facilityStats[diseaseIndex][count].countPTD;
                        regions[diseaseIndex][i].countPTR = regions[diseaseIndex][i].countPTR + facilityStats[diseaseIndex][count].countPTR;
                        regions[diseaseIndex][i].countRetreat = regions[diseaseIndex][i].countRetreat + facilityStats[diseaseIndex][count].countRetreat;
                        regions[diseaseIndex][i].countRetreatCure = regions[diseaseIndex][i].countRetreatCure + facilityStats[diseaseIndex][count].countRetreatCure;
                        regions[diseaseIndex][i].countRetreatDead = regions[diseaseIndex][i].countRetreatDead + facilityStats[diseaseIndex][count].countRetreatDead;
                        regions[diseaseIndex][i].countPO = regions[diseaseIndex][i].countPO + facilityStats[diseaseIndex][count].countPO;
                        regions[diseaseIndex][i].countPUI = regions[diseaseIndex][i].countPUI + facilityStats[diseaseIndex][count].countPUI;
                        regions[diseaseIndex][i].countPUINR = regions[diseaseIndex][i].countPUINR + facilityStats[diseaseIndex][count].countPUINR;
                        regions[diseaseIndex][i].countPUIUT = regions[diseaseIndex][i].countPUIUT + facilityStats[diseaseIndex][count].countPUIUT;
                        regions[diseaseIndex][i].countMDInf = regions[diseaseIndex][i].countMDInf + facilityStats[diseaseIndex][count].countMDInf;
                        regions[diseaseIndex][i].countMDUninf = regions[diseaseIndex][i].countMDUninf + facilityStats[diseaseIndex][count].countMDUninf;
                        regions[diseaseIndex][i].countTestPos = regions[diseaseIndex][i].countTestPos + facilityStats[diseaseIndex][count].countTestPos;
                        regions[diseaseIndex][i].countTestNeg = regions[diseaseIndex][i].countTestNeg + facilityStats[diseaseIndex][count].countTestNeg;
                        regions[diseaseIndex][i].countTruePos = regions[diseaseIndex][i].countTruePos + facilityStats[diseaseIndex][count].countTruePos;
                        regions[diseaseIndex][i].countFalsePos = regions[diseaseIndex][i].countFalsePos + facilityStats[diseaseIndex][count].countFalsePos;
                        regions[diseaseIndex][i].countTrueNeg = regions[diseaseIndex][i].countTrueNeg + facilityStats[diseaseIndex][count].countTrueNeg;
                        regions[diseaseIndex][i].countFalseNeg = regions[diseaseIndex][i].countFalseNeg + facilityStats[diseaseIndex][count].countFalseNeg;
                        regions[diseaseIndex][i].countDiagnosis = regions[diseaseIndex][i].countDiagnosis + facilityStats[diseaseIndex][count].countDiagnosis;
                        regions[diseaseIndex][i].countDiagnosisIn = regions[diseaseIndex][i].countDiagnosisIn + facilityStats[diseaseIndex][count].countDiagnosisIn;
                        regions[diseaseIndex][i].countDiagnosisOut = regions[diseaseIndex][i].countDiagnosisOut + facilityStats[diseaseIndex][count].countDiagnosisOut;
                        regions[diseaseIndex][i].countPreTestLtoFOut = regions[diseaseIndex][i].countPreTestLtoFOut + facilityStats[diseaseIndex][count].countPreTestLtoFOut;
                        regions[diseaseIndex][i].countPreTestLtoFOutTested = regions[diseaseIndex][i].countPreTestLtoFOutTested + facilityStats[diseaseIndex][count].countPreTestLtoFOutTested;
                        regions[diseaseIndex][i].countPreTestLtoFOutUnTested = regions[diseaseIndex][i].countPreTestLtoFOutUnTested + facilityStats[diseaseIndex][count].countPreTestLtoFOutUnTested;
                        regions[diseaseIndex][i].countPreTestLtoFOutUnTested2 = regions[diseaseIndex][i].countPreTestLtoFOutUnTested2 + facilityStats[diseaseIndex][count].countPreTestLtoFOutUnTested2;
                        regions[diseaseIndex][i].countPreTestLtoFDead = regions[diseaseIndex][i].countPreTestLtoFDead + facilityStats[diseaseIndex][count].countPreTestLtoFDead;
                        regions[diseaseIndex][i].countPreTestLtoFORepresent = regions[diseaseIndex][i].countPreTestLtoFORepresent + facilityStats[diseaseIndex][count].countPreTestLtoFORepresent;
                        regions[diseaseIndex][i].onsetToTreat = regions[diseaseIndex][i].onsetToTreat + facilityStats[diseaseIndex][count].onsetToTreat;
                        regions[diseaseIndex][i].onsetToTreatIn  = regions[diseaseIndex][i].onsetToTreatIn  + facilityStats[diseaseIndex][count].onsetToTreatIn ;
                        regions[diseaseIndex][i].onsetToTreatOut = regions[diseaseIndex][i].onsetToTreatOut + facilityStats[diseaseIndex][count].onsetToTreatOut;
                        regions[diseaseIndex][i].timeInfectious = regions[diseaseIndex][i].timeInfectious + facilityStats[diseaseIndex][count].timeInfectious;
                        regions[diseaseIndex][i].sampleTransTime = regions[diseaseIndex][i].sampleTransTime + facilityStats[diseaseIndex][count].sampleTransTime;
                        regions[diseaseIndex][i].sampleTransCount = regions[diseaseIndex][i].sampleTransCount + facilityStats[diseaseIndex][count].sampleTransCount;
                        regions[diseaseIndex][i].sampleWaitForResourceInTime = regions[diseaseIndex][i].sampleWaitForResourceInTime + facilityStats[diseaseIndex][count].sampleWaitForResourceInTime;
                        regions[diseaseIndex][i].sampleWaitForResourceInCount = regions[diseaseIndex][i].sampleWaitForResourceInCount + facilityStats[diseaseIndex][count].sampleWaitForResourceInCount;
                        regions[diseaseIndex][i].sampleWaitForResourceOutTime = regions[diseaseIndex][i].sampleWaitForResourceOutTime + facilityStats[diseaseIndex][count].sampleWaitForResourceOutTime;
                        regions[diseaseIndex][i].sampleWaitForResourceOutCount = regions[diseaseIndex][i].sampleWaitForResourceOutCount + facilityStats[diseaseIndex][count].sampleWaitForResourceOutCount;
                        regions[diseaseIndex][i].sampleCollectToTestInTime = regions[diseaseIndex][i].sampleCollectToTestInTime + facilityStats[diseaseIndex][count].sampleCollectToTestInTime;
                        regions[diseaseIndex][i].sampleCollectToTestOutTime = regions[diseaseIndex][i].sampleCollectToTestOutTime + facilityStats[diseaseIndex][count].sampleCollectToTestOutTime;
                        regions[diseaseIndex][i].sampleCollectToTestInCount = regions[diseaseIndex][i].sampleCollectToTestInCount + facilityStats[diseaseIndex][count].sampleCollectToTestInCount;
                        regions[diseaseIndex][i].sampleCollectToTestOutCount = regions[diseaseIndex][i].sampleCollectToTestOutCount + facilityStats[diseaseIndex][count].sampleCollectToTestOutCount;
                        regions[diseaseIndex][i].sampleTestTime = regions[diseaseIndex][i].sampleTestTime + facilityStats[diseaseIndex][count].sampleTestTime;
                        regions[diseaseIndex][i].sampleTestPosTimeIn = regions[diseaseIndex][i].sampleTestPosTimeIn + facilityStats[diseaseIndex][count].sampleTestPosTimeIn;
                        regions[diseaseIndex][i].sampleTestPosTimeOut = regions[diseaseIndex][i].sampleTestPosTimeOut + facilityStats[diseaseIndex][count].sampleTestPosTimeOut;
                        regions[diseaseIndex][i].sampleTestPosCountIn = regions[diseaseIndex][i].sampleTestPosCountIn + facilityStats[diseaseIndex][count].sampleTestPosCountIn;
                        regions[diseaseIndex][i].sampleTestPosCountOut = regions[diseaseIndex][i].sampleTestPosCountOut + facilityStats[diseaseIndex][count].sampleTestPosCountOut;
                        regions[diseaseIndex][i].sampleReturnTime = regions[diseaseIndex][i].sampleReturnTime + facilityStats[diseaseIndex][count].sampleReturnTime;
                        regions[diseaseIndex][i].sampleReturnedCount = regions[diseaseIndex][i].sampleReturnedCount + facilityStats[diseaseIndex][count].sampleReturnedCount;
                        regions[diseaseIndex][i].sampleReturnedInCount = regions[diseaseIndex][i].sampleReturnedInCount + facilityStats[diseaseIndex][count].sampleReturnedInCount;
                        regions[diseaseIndex][i].sampleReturnedOutCount = regions[diseaseIndex][i].sampleReturnedOutCount + facilityStats[diseaseIndex][count].sampleReturnedOutCount;
                        regions[diseaseIndex][i].sampleTestedInCount = regions[diseaseIndex][i].sampleTestedInCount + facilityStats[diseaseIndex][count].sampleTestedInCount;
                        regions[diseaseIndex][i].sampleTestedOutCount = regions[diseaseIndex][i].sampleTestedOutCount + facilityStats[diseaseIndex][count].sampleTestedOutCount;
                        regions[diseaseIndex][i].sampleNotTestedInCount = regions[diseaseIndex][i].sampleNotTestedInCount + facilityStats[diseaseIndex][count].sampleNotTestedInCount;
                        regions[diseaseIndex][i].sampleNotTestedOutCount = regions[diseaseIndex][i].sampleNotTestedOutCount + facilityStats[diseaseIndex][count].sampleNotTestedOutCount;
                        regions[diseaseIndex][i].sampleReturnToDeathTime = regions[diseaseIndex][i].sampleReturnToDeathTime + facilityStats[diseaseIndex][count].sampleReturnToDeathTime;
                        regions[diseaseIndex][i].sampleReturnToDeathPreDiagLtoFTime = regions[diseaseIndex][i].sampleReturnToDeathPreDiagLtoFTime + facilityStats[diseaseIndex][count].sampleReturnToDeathPreDiagLtoFTime;
                        regions[diseaseIndex][i].sampleReturnToDeathInTrtTime = regions[diseaseIndex][i].sampleReturnToDeathInTrtTime + facilityStats[diseaseIndex][count].sampleReturnToDeathInTrtTime;
                        regions[diseaseIndex][i].sampleReturnToDeathInRetrtTime = regions[diseaseIndex][i].sampleReturnToDeathInRetrtTime + facilityStats[diseaseIndex][count].sampleReturnToDeathInRetrtTime;
                        regions[diseaseIndex][i].sampleReturnToExitTime = regions[diseaseIndex][i].sampleReturnToExitTime + facilityStats[diseaseIndex][count].sampleReturnToExitTime;
                        regions[diseaseIndex][i].sampleReturnToDeathCount = regions[diseaseIndex][i].sampleReturnToDeathCount + facilityStats[diseaseIndex][count].sampleReturnToDeathCount;
                        regions[diseaseIndex][i].sampleReturnToDeathPreDiagLtoFCount = regions[diseaseIndex][i].sampleReturnToDeathPreDiagLtoFCount + facilityStats[diseaseIndex][count].sampleReturnToDeathPreDiagLtoFCount;
                        regions[diseaseIndex][i].sampleReturnToDeathInTrtCount = regions[diseaseIndex][i].sampleReturnToDeathInTrtCount + facilityStats[diseaseIndex][count].sampleReturnToDeathInTrtCount;
                        regions[diseaseIndex][i].sampleReturnToDeathInRetrtCount = regions[diseaseIndex][i].sampleReturnToDeathInRetrtCount + facilityStats[diseaseIndex][count].sampleReturnToDeathInRetrtCount;
                        regions[diseaseIndex][i].sampleReturnToExitCount = regions[diseaseIndex][i].sampleReturnToExitCount + facilityStats[diseaseIndex][count].sampleReturnToExitCount;
                        regions[diseaseIndex][i].sampleLtoFCount = regions[diseaseIndex][i].sampleLtoFCount + facilityStats[diseaseIndex][count].sampleLtoFCount;
                        regions[diseaseIndex][i].sampleWaitTestCount = regions[diseaseIndex][i].sampleWaitTestCount + facilityStats[diseaseIndex][count].sampleWaitTestCount;
                        regions[diseaseIndex][i].sampleWaitTestCountIn = regions[diseaseIndex][i].sampleWaitTestCountIn + facilityStats[diseaseIndex][count].sampleWaitTestCountIn;
                        regions[diseaseIndex][i].sampleWaitTestCountOut = regions[diseaseIndex][i].sampleWaitTestCountOut + facilityStats[diseaseIndex][count].sampleWaitTestCountOut;
                        regions[diseaseIndex][i].patientNotNotified = regions[diseaseIndex][i].patientNotNotified + facilityStats[diseaseIndex][count].patientNotNotified;
                        regions[diseaseIndex][i].patientNotNotifiedPos = regions[diseaseIndex][i].patientNotNotifiedPos + facilityStats[diseaseIndex][count].patientNotNotifiedPos;
                        regions[diseaseIndex][i].patientNotNotifiedIn = regions[diseaseIndex][i].patientNotNotifiedIn + facilityStats[diseaseIndex][count].patientNotNotifiedIn;
                        regions[diseaseIndex][i].patientNotNotifiedOut = regions[diseaseIndex][i].patientNotNotifiedOut + facilityStats[diseaseIndex][count].patientNotNotifiedOut;
                        regions[diseaseIndex][i].inTestCount = regions[diseaseIndex][i].inTestCount + facilityStats[diseaseIndex][count].inTestCount;
                        regions[diseaseIndex][i].outTestCount = regions[diseaseIndex][i].outTestCount + facilityStats[diseaseIndex][count].outTestCount;
                        regions[diseaseIndex][i].inCollectedCount = regions[diseaseIndex][i].inCollectedCount + facilityStats[diseaseIndex][count].inCollectedCount;
                        regions[diseaseIndex][i].outCollectedCount = regions[diseaseIndex][i].outCollectedCount + facilityStats[diseaseIndex][count].outCollectedCount;
                        regions[diseaseIndex][i].test1Count = regions[diseaseIndex][i].test1Count + facilityStats[diseaseIndex][count].test1Count;
                        regions[diseaseIndex][i].test1CountIn = regions[diseaseIndex][i].test1CountIn + facilityStats[diseaseIndex][count].test1CountIn;
                        regions[diseaseIndex][i].test1CountOut = regions[diseaseIndex][i].test1CountOut + facilityStats[diseaseIndex][count].test1CountOut;
                        regions[diseaseIndex][i].test1PosCount = regions[diseaseIndex][i].test1PosCount + facilityStats[diseaseIndex][count].test1PosCount;
                        regions[diseaseIndex][i].test1TruePosCount = regions[diseaseIndex][i].test1TruePosCount + facilityStats[diseaseIndex][count].test1TruePosCount;
                        regions[diseaseIndex][i].test1NegCount = regions[diseaseIndex][i].test1NegCount + facilityStats[diseaseIndex][count].test1NegCount;
                        regions[diseaseIndex][i].test1TrueNegCount = regions[diseaseIndex][i].test1TrueNegCount + facilityStats[diseaseIndex][count].test1TrueNegCount;
                        regions[diseaseIndex][i].test2Count = regions[diseaseIndex][i].test2Count + facilityStats[diseaseIndex][count].test2Count;
                        regions[diseaseIndex][i].test2CountIn = regions[diseaseIndex][i].test2CountIn + facilityStats[diseaseIndex][count].test2CountIn;
                        regions[diseaseIndex][i].test2CountOut = regions[diseaseIndex][i].test2CountOut + facilityStats[diseaseIndex][count].test2CountOut;
                        regions[diseaseIndex][i].test2PosCount = regions[diseaseIndex][i].test2PosCount + facilityStats[diseaseIndex][count].test2PosCount;
                        regions[diseaseIndex][i].test2TruePosCount = regions[diseaseIndex][i].test2TruePosCount + facilityStats[diseaseIndex][count].test2TruePosCount;
                        regions[diseaseIndex][i].test2NegCount = regions[diseaseIndex][i].test2NegCount + facilityStats[diseaseIndex][count].test2NegCount;
                        regions[diseaseIndex][i].test2TrueNegCount = regions[diseaseIndex][i].test2TrueNegCount + facilityStats[diseaseIndex][count].test2TrueNegCount;
                        regions[diseaseIndex][i].test3Count = regions[diseaseIndex][i].test3Count + facilityStats[diseaseIndex][count].test3Count;
                        if (regions[diseaseIndex][i].test1Name == "") {

                            regions[diseaseIndex][i].test1Name = facilityStats[diseaseIndex][count].test1Name;
                        }
                        if (regions[diseaseIndex][i].test2Name == "") {

                            regions[diseaseIndex][i].test2Name = facilityStats[diseaseIndex][count].test2Name;
                        }
                        if (regions[diseaseIndex][i].test3Name == "") {

                            regions[diseaseIndex][i].test3Name = facilityStats[diseaseIndex][count].test3Name;
                        }
                        regions[diseaseIndex][i].countPreTestLtoFOutMinusTestedSamp = regions[diseaseIndex][i].countPreTestLtoFOutMinusTestedSamp + facilityStats[diseaseIndex][count].countPreTestLtoFOutMinusTestedSamp;
                        regions[diseaseIndex][i].dummy2Count = regions[diseaseIndex][i].dummy2Count + facilityStats[diseaseIndex][count].dummy2Count;
                        regions[diseaseIndex][i].dummy3Count = regions[diseaseIndex][i].dummy3Count + facilityStats[diseaseIndex][count].dummy3Count;
                        regions[diseaseIndex][i].dummy4Count = regions[diseaseIndex][i].dummy4Count + facilityStats[diseaseIndex][count].dummy4Count;
                        regions[diseaseIndex][i].dummy5Count = regions[diseaseIndex][i].dummy5Count + facilityStats[diseaseIndex][count].dummy5Count;
                        regions[diseaseIndex][i].dummy6Count = regions[diseaseIndex][i].dummy6Count + facilityStats[diseaseIndex][count].dummy6Count;
            

                        // store away patient container pointer - active + inactive
                        regions[diseaseIndex][i].patientContainer.push_back(p);
                        regions[diseaseIndex][i].patientContainer.push_back(pI);
                    }
                }
                if (regionNotFound) {
                    if (debug) std::cout << "stats: agents region not found =  " << region << "\n";

                    // add it to the region array
                    regions[diseaseIndex][regionCount[diseaseIndex]].name = region;
                    regions[diseaseIndex][regionCount[diseaseIndex]].region = region;
                    regions[diseaseIndex][regionCount[diseaseIndex]].pop = facilityStats[diseaseIndex][count].pop;
                    regions[diseaseIndex][regionCount[diseaseIndex]].countP = facilityStats[diseaseIndex][count].countP;
                    regions[diseaseIndex][regionCount[diseaseIndex]].countPLtoF = facilityStats[diseaseIndex][count].countPLtoF;
                    regions[diseaseIndex][regionCount[diseaseIndex]].countPInf = facilityStats[diseaseIndex][count].countPInf;
                    regions[diseaseIndex][regionCount[diseaseIndex]].countPInfTreated = facilityStats[diseaseIndex][count].countPInfTreated;
                    regions[diseaseIndex][regionCount[diseaseIndex]].countPInfTreatedIn = facilityStats[diseaseIndex][count].countPInfTreatedIn;
                    regions[diseaseIndex][regionCount[diseaseIndex]].countPInfTreatedOut = facilityStats[diseaseIndex][count].countPInfTreatedOut;
                    regions[diseaseIndex][regionCount[diseaseIndex]].countPUninfTreated = facilityStats[diseaseIndex][count].countPUninfTreated;
                    regions[diseaseIndex][regionCount[diseaseIndex]].countPU = facilityStats[diseaseIndex][count].countPU;
                    regions[diseaseIndex][regionCount[diseaseIndex]].countPI = facilityStats[diseaseIndex][count].countPI;
                    regions[diseaseIndex][regionCount[diseaseIndex]].countPT = facilityStats[diseaseIndex][count].countPT;
                    regions[diseaseIndex][regionCount[diseaseIndex]].countPR = facilityStats[diseaseIndex][count].countPR;
                    regions[diseaseIndex][regionCount[diseaseIndex]].countPUT = facilityStats[diseaseIndex][count].countPUT;
                    regions[diseaseIndex][regionCount[diseaseIndex]].countPUTFN = facilityStats[diseaseIndex][count].countPUTFN;
                    regions[diseaseIndex][regionCount[diseaseIndex]].countPUTNS = facilityStats[diseaseIndex][count].countPUTNS;
                    regions[diseaseIndex][regionCount[diseaseIndex]].countPUTLtoF = facilityStats[diseaseIndex][count].countPUTLtoF;
                    regions[diseaseIndex][regionCount[diseaseIndex]].countPC = facilityStats[diseaseIndex][count].countPC;
                    regions[diseaseIndex][regionCount[diseaseIndex]].countPDI = facilityStats[diseaseIndex][count].countPDI;
                    regions[diseaseIndex][regionCount[diseaseIndex]].countPDIT = facilityStats[diseaseIndex][count].countPDIT;
                    regions[diseaseIndex][regionCount[diseaseIndex]].countPDIUT = facilityStats[diseaseIndex][count].countPDIUT;
                    regions[diseaseIndex][regionCount[diseaseIndex]].countPDIUTSR = facilityStats[diseaseIndex][count].countPDIUTSR;
                    regions[diseaseIndex][regionCount[diseaseIndex]].countPDIUTSNR = facilityStats[diseaseIndex][count].countPDIUTSNR;
                    regions[diseaseIndex][regionCount[diseaseIndex]].countPDIUTLtF = facilityStats[diseaseIndex][count].countPDIUTLtF;
                    regions[diseaseIndex][regionCount[diseaseIndex]].countPDU = facilityStats[diseaseIndex][count].countPDU;
                    regions[diseaseIndex][regionCount[diseaseIndex]].countPDOA = facilityStats[diseaseIndex][count].countPDOA;
                    regions[diseaseIndex][regionCount[diseaseIndex]].countPDIUTPTLtoF = facilityStats[diseaseIndex][count].countPDIUTPTLtoF;
                    regions[diseaseIndex][regionCount[diseaseIndex]].countPDIUTSRLtoF = facilityStats[diseaseIndex][count].countPDIUTSRLtoF;
                    regions[diseaseIndex][regionCount[diseaseIndex]].countPDITLtoF = facilityStats[diseaseIndex][count].countPDITLtoF;
                    regions[diseaseIndex][regionCount[diseaseIndex]].countPTC = facilityStats[diseaseIndex][count].countPTC;
                    regions[diseaseIndex][regionCount[diseaseIndex]].countPTD = facilityStats[diseaseIndex][count].countPTD;
                    regions[diseaseIndex][regionCount[diseaseIndex]].countPTR = facilityStats[diseaseIndex][count].countPTR;
                    regions[diseaseIndex][regionCount[diseaseIndex]].countRetreat = facilityStats[diseaseIndex][count].countRetreat;
                    regions[diseaseIndex][regionCount[diseaseIndex]].countRetreatCure = facilityStats[diseaseIndex][count].countRetreatCure;
                    regions[diseaseIndex][regionCount[diseaseIndex]].countRetreatDead = facilityStats[diseaseIndex][count].countRetreatDead;
                    regions[diseaseIndex][regionCount[diseaseIndex]].countPO = facilityStats[diseaseIndex][count].countPO;
                    regions[diseaseIndex][regionCount[diseaseIndex]].countPUI = facilityStats[diseaseIndex][count].countPUI;
                    regions[diseaseIndex][regionCount[diseaseIndex]].countPUINR = facilityStats[diseaseIndex][count].countPUINR;
                    regions[diseaseIndex][regionCount[diseaseIndex]].countPUIUT = facilityStats[diseaseIndex][count].countPUIUT;
                    regions[diseaseIndex][regionCount[diseaseIndex]].countMDInf = facilityStats[diseaseIndex][count].countMDInf;
                    regions[diseaseIndex][regionCount[diseaseIndex]].countMDUninf = facilityStats[diseaseIndex][count].countMDUninf;
                    regions[diseaseIndex][regionCount[diseaseIndex]].countTestPos = facilityStats[diseaseIndex][count].countTestPos;
                    regions[diseaseIndex][regionCount[diseaseIndex]].countTestNeg = facilityStats[diseaseIndex][count].countTestNeg;
                    regions[diseaseIndex][regionCount[diseaseIndex]].countTruePos = facilityStats[diseaseIndex][count].countTruePos;
                    regions[diseaseIndex][regionCount[diseaseIndex]].countFalsePos = facilityStats[diseaseIndex][count].countFalsePos;
                    regions[diseaseIndex][regionCount[diseaseIndex]].countTrueNeg = facilityStats[diseaseIndex][count].countTrueNeg;
                    regions[diseaseIndex][regionCount[diseaseIndex]].countFalseNeg = facilityStats[diseaseIndex][count].countFalseNeg;
                    regions[diseaseIndex][regionCount[diseaseIndex]].countDiagnosis = facilityStats[diseaseIndex][count].countDiagnosis;
                    regions[diseaseIndex][regionCount[diseaseIndex]].countDiagnosisIn = facilityStats[diseaseIndex][count].countDiagnosisIn;
                    regions[diseaseIndex][regionCount[diseaseIndex]].countDiagnosisOut = facilityStats[diseaseIndex][count].countDiagnosisOut;
                    regions[diseaseIndex][regionCount[diseaseIndex]].countPreTestLtoFOut = facilityStats[diseaseIndex][count].countPreTestLtoFOut;
                    regions[diseaseIndex][regionCount[diseaseIndex]].countPreTestLtoFOutTested = facilityStats[diseaseIndex][count].countPreTestLtoFOutTested;
                    regions[diseaseIndex][regionCount[diseaseIndex]].countPreTestLtoFOutUnTested = facilityStats[diseaseIndex][count].countPreTestLtoFOutUnTested;
                    regions[diseaseIndex][regionCount[diseaseIndex]].countPreTestLtoFOutUnTested2 = facilityStats[diseaseIndex][count].countPreTestLtoFOutUnTested2;
                    regions[diseaseIndex][regionCount[diseaseIndex]].countPreTestLtoFDead = facilityStats[diseaseIndex][count].countPreTestLtoFDead;
                    regions[diseaseIndex][regionCount[diseaseIndex]].countPreTestLtoFORepresent = facilityStats[diseaseIndex][count].countPreTestLtoFORepresent;
                    regions[diseaseIndex][regionCount[diseaseIndex]].onsetToTreat = facilityStats[diseaseIndex][count].onsetToTreat;
                    regions[diseaseIndex][regionCount[diseaseIndex]].onsetToTreatIn  = facilityStats[diseaseIndex][count].onsetToTreatIn ;
                    regions[diseaseIndex][regionCount[diseaseIndex]].onsetToTreatOut = facilityStats[diseaseIndex][count].onsetToTreatOut;
                    regions[diseaseIndex][regionCount[diseaseIndex]].timeInfectious = facilityStats[diseaseIndex][count].timeInfectious;
                    regions[diseaseIndex][regionCount[diseaseIndex]].sampleTransTime = facilityStats[diseaseIndex][count].sampleTransTime;
                    regions[diseaseIndex][regionCount[diseaseIndex]].sampleTransCount = facilityStats[diseaseIndex][count].sampleTransCount;
                    regions[diseaseIndex][regionCount[diseaseIndex]].sampleWaitForResourceInTime = facilityStats[diseaseIndex][count].sampleWaitForResourceInTime;
                    regions[diseaseIndex][regionCount[diseaseIndex]].sampleWaitForResourceInCount = facilityStats[diseaseIndex][count].sampleWaitForResourceInCount;
                    regions[diseaseIndex][regionCount[diseaseIndex]].sampleWaitForResourceOutTime = facilityStats[diseaseIndex][count].sampleWaitForResourceOutTime;
                    regions[diseaseIndex][regionCount[diseaseIndex]].sampleWaitForResourceOutCount = facilityStats[diseaseIndex][count].sampleWaitForResourceOutCount;
                    regions[diseaseIndex][regionCount[diseaseIndex]].sampleCollectToTestInTime = facilityStats[diseaseIndex][count].sampleCollectToTestInTime;
                    regions[diseaseIndex][regionCount[diseaseIndex]].sampleCollectToTestOutTime = facilityStats[diseaseIndex][count].sampleCollectToTestOutTime;
                    regions[diseaseIndex][regionCount[diseaseIndex]].sampleCollectToTestInCount = facilityStats[diseaseIndex][count].sampleCollectToTestInCount;
                    regions[diseaseIndex][regionCount[diseaseIndex]].sampleCollectToTestOutCount = facilityStats[diseaseIndex][count].sampleCollectToTestOutCount;
                    regions[diseaseIndex][regionCount[diseaseIndex]].sampleTestTime = facilityStats[diseaseIndex][count].sampleTestTime;
                    regions[diseaseIndex][regionCount[diseaseIndex]].sampleTestPosTimeIn = facilityStats[diseaseIndex][count].sampleTestPosTimeIn;
                    regions[diseaseIndex][regionCount[diseaseIndex]].sampleTestPosTimeOut = facilityStats[diseaseIndex][count].sampleTestPosTimeOut;
                    regions[diseaseIndex][regionCount[diseaseIndex]].sampleTestPosCountIn = facilityStats[diseaseIndex][count].sampleTestPosCountIn;
                    regions[diseaseIndex][regionCount[diseaseIndex]].sampleTestPosCountOut = facilityStats[diseaseIndex][count].sampleTestPosCountOut;
                    regions[diseaseIndex][regionCount[diseaseIndex]].sampleReturnTime = facilityStats[diseaseIndex][count].sampleReturnTime;
                    regions[diseaseIndex][regionCount[diseaseIndex]].sampleReturnedCount = facilityStats[diseaseIndex][count].sampleReturnedCount;
                    regions[diseaseIndex][regionCount[diseaseIndex]].sampleReturnedInCount  = facilityStats[diseaseIndex][count].sampleReturnedInCount;
                    regions[diseaseIndex][regionCount[diseaseIndex]].sampleReturnedOutCount  = facilityStats[diseaseIndex][count].sampleReturnedOutCount;
                    regions[diseaseIndex][regionCount[diseaseIndex]].sampleTestedInCount  = facilityStats[diseaseIndex][count].sampleTestedInCount;
                    regions[diseaseIndex][regionCount[diseaseIndex]].sampleTestedOutCount  = facilityStats[diseaseIndex][count].sampleTestedOutCount;
                    regions[diseaseIndex][regionCount[diseaseIndex]].sampleNotTestedInCount  = facilityStats[diseaseIndex][count].sampleNotTestedInCount;
                    regions[diseaseIndex][regionCount[diseaseIndex]].sampleNotTestedOutCount  = facilityStats[diseaseIndex][count].sampleNotTestedOutCount;
                    regions[diseaseIndex][regionCount[diseaseIndex]].sampleReturnToDeathTime  = facilityStats[diseaseIndex][count].sampleReturnToDeathTime;
                    regions[diseaseIndex][regionCount[diseaseIndex]].sampleReturnToDeathPreDiagLtoFTime  = facilityStats[diseaseIndex][count].sampleReturnToDeathPreDiagLtoFTime;
                    regions[diseaseIndex][regionCount[diseaseIndex]].sampleReturnToDeathInTrtTime  = facilityStats[diseaseIndex][count].sampleReturnToDeathInTrtTime;
                    regions[diseaseIndex][regionCount[diseaseIndex]].sampleReturnToDeathInRetrtTime  = facilityStats[diseaseIndex][count].sampleReturnToDeathInRetrtTime;
                    regions[diseaseIndex][regionCount[diseaseIndex]].sampleReturnToExitTime  = facilityStats[diseaseIndex][count].sampleReturnToExitTime;
                    regions[diseaseIndex][regionCount[diseaseIndex]].sampleReturnToDeathCount  = facilityStats[diseaseIndex][count].sampleReturnToDeathCount;
                    regions[diseaseIndex][regionCount[diseaseIndex]].sampleReturnToDeathPreDiagLtoFCount  = facilityStats[diseaseIndex][count].sampleReturnToDeathPreDiagLtoFCount;
                    regions[diseaseIndex][regionCount[diseaseIndex]].sampleReturnToDeathInTrtCount  = facilityStats[diseaseIndex][count].sampleReturnToDeathInTrtCount;
                    regions[diseaseIndex][regionCount[diseaseIndex]].sampleReturnToDeathInRetrtCount  = facilityStats[diseaseIndex][count].sampleReturnToDeathInRetrtCount;
                    regions[diseaseIndex][regionCount[diseaseIndex]].sampleReturnToExitCount  = facilityStats[diseaseIndex][count].sampleReturnToExitCount;
                    regions[diseaseIndex][regionCount[diseaseIndex]].sampleLtoFCount = facilityStats[diseaseIndex][count].sampleLtoFCount;
                    regions[diseaseIndex][regionCount[diseaseIndex]].sampleWaitTestCount = facilityStats[diseaseIndex][count].sampleWaitTestCount;
                    regions[diseaseIndex][regionCount[diseaseIndex]].sampleWaitTestCountIn = facilityStats[diseaseIndex][count].sampleWaitTestCountIn;
                    regions[diseaseIndex][regionCount[diseaseIndex]].sampleWaitTestCountOut = facilityStats[diseaseIndex][count].sampleWaitTestCountOut;
                    regions[diseaseIndex][regionCount[diseaseIndex]].patientNotNotified = facilityStats[diseaseIndex][count].patientNotNotified;
                    regions[diseaseIndex][regionCount[diseaseIndex]].patientNotNotifiedPos = facilityStats[diseaseIndex][count].patientNotNotifiedPos;
                    regions[diseaseIndex][regionCount[diseaseIndex]].patientNotNotifiedIn = facilityStats[diseaseIndex][count].patientNotNotifiedIn;
                    regions[diseaseIndex][regionCount[diseaseIndex]].patientNotNotifiedOut = facilityStats[diseaseIndex][count].patientNotNotifiedOut;
                    regions[diseaseIndex][regionCount[diseaseIndex]].inTestCount = facilityStats[diseaseIndex][count].inTestCount;
                    regions[diseaseIndex][regionCount[diseaseIndex]].outTestCount = facilityStats[diseaseIndex][count].outTestCount;
                    regions[diseaseIndex][regionCount[diseaseIndex]].inCollectedCount = facilityStats[diseaseIndex][count].inCollectedCount;
                    regions[diseaseIndex][regionCount[diseaseIndex]].outCollectedCount = facilityStats[diseaseIndex][count].outCollectedCount;
                    regions[diseaseIndex][regionCount[diseaseIndex]].test1Count = facilityStats[diseaseIndex][count].test1Count;
                    regions[diseaseIndex][regionCount[diseaseIndex]].test1CountIn = facilityStats[diseaseIndex][count].test1CountIn;
                    regions[diseaseIndex][regionCount[diseaseIndex]].test1CountOut = facilityStats[diseaseIndex][count].test1CountOut;
                    regions[diseaseIndex][regionCount[diseaseIndex]].test1PosCount = facilityStats[diseaseIndex][count].test1PosCount;
                    regions[diseaseIndex][regionCount[diseaseIndex]].test1TruePosCount = facilityStats[diseaseIndex][count].test1TruePosCount;
                    regions[diseaseIndex][regionCount[diseaseIndex]].test1NegCount = facilityStats[diseaseIndex][count].test1NegCount;
                    regions[diseaseIndex][regionCount[diseaseIndex]].test1TrueNegCount = facilityStats[diseaseIndex][count].test1TrueNegCount;
                    regions[diseaseIndex][regionCount[diseaseIndex]].test2Count = facilityStats[diseaseIndex][count].test2Count;
                    regions[diseaseIndex][regionCount[diseaseIndex]].test2CountIn = facilityStats[diseaseIndex][count].test2CountIn;
                    regions[diseaseIndex][regionCount[diseaseIndex]].test2CountOut = facilityStats[diseaseIndex][count].test2CountOut;
                    regions[diseaseIndex][regionCount[diseaseIndex]].test2PosCount = facilityStats[diseaseIndex][count].test2PosCount;
                    regions[diseaseIndex][regionCount[diseaseIndex]].test2TruePosCount = facilityStats[diseaseIndex][count].test2TruePosCount;
                    regions[diseaseIndex][regionCount[diseaseIndex]].test2NegCount = facilityStats[diseaseIndex][count].test2NegCount;
                    regions[diseaseIndex][regionCount[diseaseIndex]].test2TrueNegCount = facilityStats[diseaseIndex][count].test2TrueNegCount;
                    regions[diseaseIndex][regionCount[diseaseIndex]].test3Count = facilityStats[diseaseIndex][count].test3Count;
                    if (regions[diseaseIndex][regionCount[diseaseIndex]].test1Name == "") {

                        regions[diseaseIndex][regionCount[diseaseIndex]].test1Name = facilityStats[diseaseIndex][count].test1Name;
                    }
                    if (regions[diseaseIndex][regionCount[diseaseIndex]].test2Name == "") {

                        regions[diseaseIndex][regionCount[diseaseIndex]].test2Name = facilityStats[diseaseIndex][count].test2Name;
                    }
                    if (regions[diseaseIndex][regionCount[diseaseIndex]].test3Name == "") {

                        regions[diseaseIndex][regionCount[diseaseIndex]].test3Name = facilityStats[diseaseIndex][count].test3Name;
                    }
                    regions[diseaseIndex][regionCount[diseaseIndex]].countPreTestLtoFOutMinusTestedSamp = facilityStats[diseaseIndex][count].countPreTestLtoFOutMinusTestedSamp;
                    regions[diseaseIndex][regionCount[diseaseIndex]].dummy2Count = facilityStats[diseaseIndex][count].dummy2Count;
                    regions[diseaseIndex][regionCount[diseaseIndex]].dummy3Count = facilityStats[diseaseIndex][count].dummy3Count;
                    regions[diseaseIndex][regionCount[diseaseIndex]].dummy4Count = facilityStats[diseaseIndex][count].dummy4Count;
                    regions[diseaseIndex][regionCount[diseaseIndex]].dummy5Count = facilityStats[diseaseIndex][count].dummy5Count;
                    regions[diseaseIndex][regionCount[diseaseIndex]].dummy6Count = facilityStats[diseaseIndex][count].dummy6Count;

                    // store away patient container pointer - active and inactive
                    regions[diseaseIndex][regionCount[diseaseIndex]].patientContainer.push_back(p);
                    regions[diseaseIndex][regionCount[diseaseIndex]].patientContainer.push_back(pI);

                    regionCount[diseaseIndex] = regionCount[diseaseIndex] + 1;
                }

                // now update stats for districts
                districtNotFound = true;
                for (int i = 0; i < districtCount[diseaseIndex]; i++) {

                    if (district == districts[diseaseIndex][i].name) {
                        if (debug) std::cout << "stats: agents district found =  " << district << "\n";

                        //update statistics for that region
                        districtNotFound = false;

                        districts[diseaseIndex][i].district = district;
                        districts[diseaseIndex][i].region = region;
                        districts[diseaseIndex][i].pop = districts[diseaseIndex][i].pop + facilityStats[diseaseIndex][count].pop;
                        districts[diseaseIndex][i].countP = districts[diseaseIndex][i].countP + facilityStats[diseaseIndex][count].countP;
                        districts[diseaseIndex][i].countPLtoF = districts[diseaseIndex][i].countPLtoF + facilityStats[diseaseIndex][count].countPLtoF;
                        districts[diseaseIndex][i].countPInf = districts[diseaseIndex][i].countPInf + facilityStats[diseaseIndex][count].countPInf;
                        districts[diseaseIndex][i].countPInfTreated = districts[diseaseIndex][i].countPInfTreated + facilityStats[diseaseIndex][count].countPInfTreated;
                        districts[diseaseIndex][i].countPInfTreatedIn = districts[diseaseIndex][i].countPInfTreatedIn + facilityStats[diseaseIndex][count].countPInfTreatedIn;
                        districts[diseaseIndex][i].countPInfTreatedOut = districts[diseaseIndex][i].countPInfTreatedOut + facilityStats[diseaseIndex][count].countPInfTreatedOut;
                        districts[diseaseIndex][i].countPUninfTreated = districts[diseaseIndex][i].countPUninfTreated + facilityStats[diseaseIndex][count].countPUninfTreated;
                        districts[diseaseIndex][i].countPU = districts[diseaseIndex][i].countPU + facilityStats[diseaseIndex][count].countPU;
                        districts[diseaseIndex][i].countPI = districts[diseaseIndex][i].countPI + facilityStats[diseaseIndex][count].countPI;
                        districts[diseaseIndex][i].countPT = districts[diseaseIndex][i].countPT + facilityStats[diseaseIndex][count].countPT;
                        districts[diseaseIndex][i].countPR = districts[diseaseIndex][i].countPR + facilityStats[diseaseIndex][count].countPR;
                        districts[diseaseIndex][i].countPUT = districts[diseaseIndex][i].countPUT + facilityStats[diseaseIndex][count].countPUT;
                        districts[diseaseIndex][i].countPUTFN = districts[diseaseIndex][i].countPUTFN + facilityStats[diseaseIndex][count].countPUTFN;
                        districts[diseaseIndex][i].countPUTNS = districts[diseaseIndex][i].countPUTNS + facilityStats[diseaseIndex][count].countPUTNS;
                        districts[diseaseIndex][i].countPUTLtoF = districts[diseaseIndex][i].countPUTLtoF + facilityStats[diseaseIndex][count].countPUTLtoF;
                        districts[diseaseIndex][i].countPC = districts[diseaseIndex][i].countPC + facilityStats[diseaseIndex][count].countPC;
                        districts[diseaseIndex][i].countPDI = districts[diseaseIndex][i].countPDI + facilityStats[diseaseIndex][count].countPDI;
                        districts[diseaseIndex][i].countPDIT = districts[diseaseIndex][i].countPDIT + facilityStats[diseaseIndex][count].countPDIT;
                        districts[diseaseIndex][i].countPDIUT = districts[diseaseIndex][i].countPDIUT + facilityStats[diseaseIndex][count].countPDIUT;
                        districts[diseaseIndex][i].countPDIUTSR = districts[diseaseIndex][i].countPDIUTSR + facilityStats[diseaseIndex][count].countPDIUTSR;
                        districts[diseaseIndex][i].countPDIUTSNR = districts[diseaseIndex][i].countPDIUTSNR + facilityStats[diseaseIndex][count].countPDIUTSNR;
                        districts[diseaseIndex][i].countPDIUTLtF = districts[diseaseIndex][i].countPDIUTLtF + facilityStats[diseaseIndex][count].countPDIUTLtF;
                        districts[diseaseIndex][i].countPDU = districts[diseaseIndex][i].countPDU + facilityStats[diseaseIndex][count].countPDU;
                        districts[diseaseIndex][i].countPDOA = districts[diseaseIndex][i].countPDOA + facilityStats[diseaseIndex][count].countPDOA;
                        districts[diseaseIndex][i].countPDIUTPTLtoF = districts[diseaseIndex][i].countPDIUTPTLtoF + facilityStats[diseaseIndex][count].countPDIUTPTLtoF;
                        districts[diseaseIndex][i].countPDIUTSRLtoF = districts[diseaseIndex][i].countPDIUTSRLtoF + facilityStats[diseaseIndex][count].countPDIUTSRLtoF;
                        districts[diseaseIndex][i].countPDITLtoF = districts[diseaseIndex][i].countPDITLtoF + facilityStats[diseaseIndex][count].countPDITLtoF;
                        districts[diseaseIndex][i].countPTC = districts[diseaseIndex][i].countPTC + facilityStats[diseaseIndex][count].countPTC;
                        districts[diseaseIndex][i].countPTD = districts[diseaseIndex][i].countPTD + facilityStats[diseaseIndex][count].countPTD;
                        districts[diseaseIndex][i].countPTR = districts[diseaseIndex][i].countPTR + facilityStats[diseaseIndex][count].countPTR;
                        districts[diseaseIndex][i].countRetreat = districts[diseaseIndex][i].countRetreat + facilityStats[diseaseIndex][count].countRetreat;
                        districts[diseaseIndex][i].countRetreatCure = districts[diseaseIndex][i].countRetreatCure + facilityStats[diseaseIndex][count].countRetreatCure;
                        districts[diseaseIndex][i].countRetreatDead = districts[diseaseIndex][i].countRetreatDead + facilityStats[diseaseIndex][count].countRetreatDead;
                        districts[diseaseIndex][i].countPO = districts[diseaseIndex][i].countPO + facilityStats[diseaseIndex][count].countPO;
                        districts[diseaseIndex][i].countPUI = districts[diseaseIndex][i].countPUI + facilityStats[diseaseIndex][count].countPUI;
                        districts[diseaseIndex][i].countPUINR = districts[diseaseIndex][i].countPUINR + facilityStats[diseaseIndex][count].countPUINR;
                        districts[diseaseIndex][i].countPUIUT = districts[diseaseIndex][i].countPUIUT + facilityStats[diseaseIndex][count].countPUIUT;
                        districts[diseaseIndex][i].countMDInf = districts[diseaseIndex][i].countMDInf + facilityStats[diseaseIndex][count].countMDInf;
                        districts[diseaseIndex][i].countMDUninf = districts[diseaseIndex][i].countMDUninf + facilityStats[diseaseIndex][count].countMDUninf;
                        districts[diseaseIndex][i].countTestPos = districts[diseaseIndex][i].countTestPos + facilityStats[diseaseIndex][count].countTestPos;
                        districts[diseaseIndex][i].countTestNeg = districts[diseaseIndex][i].countTestNeg + facilityStats[diseaseIndex][count].countTestNeg;
                        districts[diseaseIndex][i].countTruePos = districts[diseaseIndex][i].countTruePos + facilityStats[diseaseIndex][count].countTruePos;
                        districts[diseaseIndex][i].countFalsePos = districts[diseaseIndex][i].countFalsePos + facilityStats[diseaseIndex][count].countFalsePos;
                        districts[diseaseIndex][i].countTrueNeg = districts[diseaseIndex][i].countTrueNeg + facilityStats[diseaseIndex][count].countTrueNeg;
                        districts[diseaseIndex][i].countFalseNeg = districts[diseaseIndex][i].countFalseNeg + facilityStats[diseaseIndex][count].countFalseNeg;
                        districts[diseaseIndex][i].countDiagnosis = districts[diseaseIndex][i].countDiagnosis + facilityStats[diseaseIndex][count].countDiagnosis;
                        districts[diseaseIndex][i].countDiagnosisIn = districts[diseaseIndex][i].countDiagnosisIn + facilityStats[diseaseIndex][count].countDiagnosisIn;
                        districts[diseaseIndex][i].countDiagnosisOut = districts[diseaseIndex][i].countDiagnosisOut + facilityStats[diseaseIndex][count].countDiagnosisOut;
                        districts[diseaseIndex][i].countPreTestLtoFOut = districts[diseaseIndex][i].countPreTestLtoFOut + facilityStats[diseaseIndex][count].countPreTestLtoFOut;
                        districts[diseaseIndex][i].countPreTestLtoFOutTested = districts[diseaseIndex][i].countPreTestLtoFOutTested + facilityStats[diseaseIndex][count].countPreTestLtoFOutTested;
                        districts[diseaseIndex][i].countPreTestLtoFOutUnTested = districts[diseaseIndex][i].countPreTestLtoFOutUnTested + facilityStats[diseaseIndex][count].countPreTestLtoFOutUnTested;
                        districts[diseaseIndex][i].countPreTestLtoFOutUnTested2 = districts[diseaseIndex][i].countPreTestLtoFOutUnTested2 + facilityStats[diseaseIndex][count].countPreTestLtoFOutUnTested2;
                        districts[diseaseIndex][i].countPreTestLtoFDead = districts[diseaseIndex][i].countPreTestLtoFDead + facilityStats[diseaseIndex][count].countPreTestLtoFDead;
                        districts[diseaseIndex][i].countPreTestLtoFORepresent = districts[diseaseIndex][i].countPreTestLtoFORepresent + facilityStats[diseaseIndex][count].countPreTestLtoFORepresent;
                        districts[diseaseIndex][i].onsetToTreat = districts[diseaseIndex][i].onsetToTreat + facilityStats[diseaseIndex][count].onsetToTreat;
                        districts[diseaseIndex][i].onsetToTreatIn  = districts[diseaseIndex][i].onsetToTreatIn  + facilityStats[diseaseIndex][count].onsetToTreatIn ;
                        districts[diseaseIndex][i].onsetToTreatOut = districts[diseaseIndex][i].onsetToTreatOut + facilityStats[diseaseIndex][count].onsetToTreatOut;
                        districts[diseaseIndex][i].timeInfectious = districts[diseaseIndex][i].timeInfectious + facilityStats[diseaseIndex][count].timeInfectious;
                        districts[diseaseIndex][i].sampleTransTime = districts[diseaseIndex][i].sampleTransTime + facilityStats[diseaseIndex][count].sampleTransTime;
                        districts[diseaseIndex][i].sampleTransCount = districts[diseaseIndex][i].sampleTransCount + facilityStats[diseaseIndex][count].sampleTransCount;
                        districts[diseaseIndex][i].sampleWaitForResourceInTime = districts[diseaseIndex][i].sampleWaitForResourceInTime + facilityStats[diseaseIndex][count].sampleWaitForResourceInTime;
                        districts[diseaseIndex][i].sampleWaitForResourceInCount = districts[diseaseIndex][i].sampleWaitForResourceInCount + facilityStats[diseaseIndex][count].sampleWaitForResourceInCount;
                        districts[diseaseIndex][i].sampleWaitForResourceOutTime = districts[diseaseIndex][i].sampleWaitForResourceOutTime + facilityStats[diseaseIndex][count].sampleWaitForResourceOutTime;
                        districts[diseaseIndex][i].sampleWaitForResourceOutCount = districts[diseaseIndex][i].sampleWaitForResourceOutCount + facilityStats[diseaseIndex][count].sampleWaitForResourceOutCount;
                        districts[diseaseIndex][i].sampleCollectToTestInTime = districts[diseaseIndex][i].sampleCollectToTestInTime + facilityStats[diseaseIndex][count].sampleCollectToTestInTime;
                        districts[diseaseIndex][i].sampleCollectToTestOutTime = districts[diseaseIndex][i].sampleCollectToTestOutTime + facilityStats[diseaseIndex][count].sampleCollectToTestOutTime;
                        districts[diseaseIndex][i].sampleCollectToTestInCount = districts[diseaseIndex][i].sampleCollectToTestInCount + facilityStats[diseaseIndex][count].sampleCollectToTestInCount;
                        districts[diseaseIndex][i].sampleCollectToTestOutCount = districts[diseaseIndex][i].sampleCollectToTestOutCount + facilityStats[diseaseIndex][count].sampleCollectToTestOutCount;
                        districts[diseaseIndex][i].sampleTestTime = districts[diseaseIndex][i].sampleTestTime + facilityStats[diseaseIndex][count].sampleTestTime;
                        districts[diseaseIndex][i].sampleTestPosTimeIn = districts[diseaseIndex][i].sampleTestPosTimeIn + facilityStats[diseaseIndex][count].sampleTestPosTimeIn;
                        districts[diseaseIndex][i].sampleTestPosTimeOut = districts[diseaseIndex][i].sampleTestPosTimeOut + facilityStats[diseaseIndex][count].sampleTestPosTimeOut;
                        districts[diseaseIndex][i].sampleTestPosCountIn = districts[diseaseIndex][i].sampleTestPosCountIn + facilityStats[diseaseIndex][count].sampleTestPosCountIn;
                        districts[diseaseIndex][i].sampleTestPosCountOut = districts[diseaseIndex][i].sampleTestPosCountOut + facilityStats[diseaseIndex][count].sampleTestPosCountOut;
                        districts[diseaseIndex][i].sampleReturnTime = districts[diseaseIndex][i].sampleReturnTime + facilityStats[diseaseIndex][count].sampleReturnTime;
                        districts[diseaseIndex][i].sampleReturnedCount = districts[diseaseIndex][i].sampleReturnedCount + facilityStats[diseaseIndex][count].sampleReturnedCount;
                        districts[diseaseIndex][i].sampleReturnedInCount  = districts[diseaseIndex][i].sampleReturnedInCount  + facilityStats[diseaseIndex][count].sampleReturnedInCount;
                        districts[diseaseIndex][i].sampleReturnedOutCount  = districts[diseaseIndex][i].sampleReturnedOutCount  + facilityStats[diseaseIndex][count].sampleReturnedOutCount;
                        districts[diseaseIndex][i].sampleTestedInCount  = districts[diseaseIndex][i].sampleTestedInCount  + facilityStats[diseaseIndex][count].sampleTestedInCount;
                        districts[diseaseIndex][i].sampleTestedOutCount  = districts[diseaseIndex][i].sampleTestedOutCount  + facilityStats[diseaseIndex][count].sampleTestedOutCount;
                        districts[diseaseIndex][i].sampleNotTestedInCount  = districts[diseaseIndex][i].sampleNotTestedInCount  + facilityStats[diseaseIndex][count].sampleNotTestedInCount;
                        districts[diseaseIndex][i].sampleNotTestedOutCount  = districts[diseaseIndex][i].sampleNotTestedOutCount  + facilityStats[diseaseIndex][count].sampleNotTestedOutCount;
                        districts[diseaseIndex][i].sampleReturnToDeathTime  = districts[diseaseIndex][i].sampleReturnToDeathTime  + facilityStats[diseaseIndex][count].sampleReturnToDeathTime;
                        districts[diseaseIndex][i].sampleReturnToDeathPreDiagLtoFTime  = districts[diseaseIndex][i].sampleReturnToDeathPreDiagLtoFTime  + facilityStats[diseaseIndex][count].sampleReturnToDeathPreDiagLtoFTime;
                        districts[diseaseIndex][i].sampleReturnToDeathInTrtTime  = districts[diseaseIndex][i].sampleReturnToDeathInTrtTime  + facilityStats[diseaseIndex][count].sampleReturnToDeathInTrtTime;
                        districts[diseaseIndex][i].sampleReturnToDeathInRetrtTime  = districts[diseaseIndex][i].sampleReturnToDeathInRetrtTime  + facilityStats[diseaseIndex][count].sampleReturnToDeathInRetrtTime;
                        districts[diseaseIndex][i].sampleReturnToExitTime  = districts[diseaseIndex][i].sampleReturnToExitTime  + facilityStats[diseaseIndex][count].sampleReturnToExitTime;
                        districts[diseaseIndex][i].sampleReturnToDeathCount  = districts[diseaseIndex][i].sampleReturnToDeathCount  + facilityStats[diseaseIndex][count].sampleReturnToDeathCount;
                        districts[diseaseIndex][i].sampleReturnToDeathPreDiagLtoFCount  = districts[diseaseIndex][i].sampleReturnToDeathPreDiagLtoFCount  + facilityStats[diseaseIndex][count].sampleReturnToDeathPreDiagLtoFCount;
                        districts[diseaseIndex][i].sampleReturnToDeathInTrtCount  = districts[diseaseIndex][i].sampleReturnToDeathInTrtCount  + facilityStats[diseaseIndex][count].sampleReturnToDeathInTrtCount;
                        districts[diseaseIndex][i].sampleReturnToDeathInRetrtCount  = districts[diseaseIndex][i].sampleReturnToDeathInRetrtCount  + facilityStats[diseaseIndex][count].sampleReturnToDeathInRetrtCount;
                        districts[diseaseIndex][i].sampleReturnToExitCount  = districts[diseaseIndex][i].sampleReturnToExitCount  + facilityStats[diseaseIndex][count].sampleReturnToExitCount;
                        districts[diseaseIndex][i].sampleLtoFCount = districts[diseaseIndex][i].sampleLtoFCount + facilityStats[diseaseIndex][count].sampleLtoFCount;
                        districts[diseaseIndex][i].sampleWaitTestCount = districts[diseaseIndex][i].sampleWaitTestCount + facilityStats[diseaseIndex][count].sampleWaitTestCount;
                        districts[diseaseIndex][i].sampleWaitTestCountIn = districts[diseaseIndex][i].sampleWaitTestCountIn + facilityStats[diseaseIndex][count].sampleWaitTestCountIn;
                        districts[diseaseIndex][i].sampleWaitTestCountOut = districts[diseaseIndex][i].sampleWaitTestCountOut + facilityStats[diseaseIndex][count].sampleWaitTestCountOut;
                        districts[diseaseIndex][i].patientNotNotified = districts[diseaseIndex][i].patientNotNotified + facilityStats[diseaseIndex][count].patientNotNotified;
                        districts[diseaseIndex][i].patientNotNotifiedPos = districts[diseaseIndex][i].patientNotNotifiedPos + facilityStats[diseaseIndex][count].patientNotNotifiedPos;
                        districts[diseaseIndex][i].patientNotNotifiedIn = districts[diseaseIndex][i].patientNotNotifiedIn + facilityStats[diseaseIndex][count].patientNotNotifiedIn;
                        districts[diseaseIndex][i].patientNotNotifiedOut = districts[diseaseIndex][i].patientNotNotifiedOut + facilityStats[diseaseIndex][count].patientNotNotifiedOut;
                        districts[diseaseIndex][i].inTestCount = districts[diseaseIndex][i].inTestCount + facilityStats[diseaseIndex][count].inTestCount;
                        districts[diseaseIndex][i].outTestCount = districts[diseaseIndex][i].outTestCount + facilityStats[diseaseIndex][count].outTestCount;
                        districts[diseaseIndex][i].inCollectedCount = districts[diseaseIndex][i].inCollectedCount + facilityStats[diseaseIndex][count].inCollectedCount;
                        districts[diseaseIndex][i].outCollectedCount = districts[diseaseIndex][i].outCollectedCount + facilityStats[diseaseIndex][count].outCollectedCount;
                        districts[diseaseIndex][i].test1Count = districts[diseaseIndex][i].test1Count + facilityStats[diseaseIndex][count].test1Count;
                        districts[diseaseIndex][i].test1CountIn = districts[diseaseIndex][i].test1CountIn + facilityStats[diseaseIndex][count].test1CountIn;
                        districts[diseaseIndex][i].test1CountOut = districts[diseaseIndex][i].test1CountOut + facilityStats[diseaseIndex][count].test1CountOut;
                        districts[diseaseIndex][i].test1PosCount = districts[diseaseIndex][i].test1PosCount + facilityStats[diseaseIndex][count].test1PosCount;
                        districts[diseaseIndex][i].test1TruePosCount = districts[diseaseIndex][i].test1TruePosCount + facilityStats[diseaseIndex][count].test1TruePosCount;
                        districts[diseaseIndex][i].test1NegCount = districts[diseaseIndex][i].test1NegCount + facilityStats[diseaseIndex][count].test1NegCount;
                        districts[diseaseIndex][i].test1TrueNegCount = districts[diseaseIndex][i].test1TrueNegCount + facilityStats[diseaseIndex][count].test1TrueNegCount;
                        districts[diseaseIndex][i].test2Count = districts[diseaseIndex][i].test2Count + facilityStats[diseaseIndex][count].test2Count;
                        districts[diseaseIndex][i].test2CountIn = districts[diseaseIndex][i].test2CountIn + facilityStats[diseaseIndex][count].test2CountIn;
                        districts[diseaseIndex][i].test2CountOut = districts[diseaseIndex][i].test2CountOut + facilityStats[diseaseIndex][count].test2CountOut;
                        districts[diseaseIndex][i].test2PosCount = districts[diseaseIndex][i].test2PosCount + facilityStats[diseaseIndex][count].test2PosCount;
                        districts[diseaseIndex][i].test2TruePosCount = districts[diseaseIndex][i].test2TruePosCount + facilityStats[diseaseIndex][count].test2TruePosCount;
                        districts[diseaseIndex][i].test2NegCount = districts[diseaseIndex][i].test2NegCount + facilityStats[diseaseIndex][count].test2NegCount;
                        districts[diseaseIndex][i].test2TrueNegCount = districts[diseaseIndex][i].test2TrueNegCount + facilityStats[diseaseIndex][count].test2TrueNegCount;
                        districts[diseaseIndex][i].test3Count = districts[diseaseIndex][i].test3Count + facilityStats[diseaseIndex][count].test3Count;
                        if (districts[diseaseIndex][i].test1Name == "") {

                            districts[diseaseIndex][i].test1Name = facilityStats[diseaseIndex][count].test1Name;
                        }
                        if (districts[diseaseIndex][i].test2Name == "") {

                            districts[diseaseIndex][i].test2Name = facilityStats[diseaseIndex][count].test2Name;
                        }
                        if (districts[diseaseIndex][i].test3Name == "") {

                            districts[diseaseIndex][i].test3Name = facilityStats[diseaseIndex][count].test3Name;
                        }
                        districts[diseaseIndex][i].countPreTestLtoFOutMinusTestedSamp = districts[diseaseIndex][i].countPreTestLtoFOutMinusTestedSamp + facilityStats[diseaseIndex][count].countPreTestLtoFOutMinusTestedSamp;
                        districts[diseaseIndex][i].dummy2Count = districts[diseaseIndex][i].dummy2Count + facilityStats[diseaseIndex][count].dummy2Count;
                        districts[diseaseIndex][i].dummy3Count = districts[diseaseIndex][i].dummy3Count + facilityStats[diseaseIndex][count].dummy3Count;
                        districts[diseaseIndex][i].dummy4Count = districts[diseaseIndex][i].dummy4Count + facilityStats[diseaseIndex][count].dummy4Count;
                        districts[diseaseIndex][i].dummy5Count = districts[diseaseIndex][i].dummy5Count + facilityStats[diseaseIndex][count].dummy5Count;
                        districts[diseaseIndex][i].dummy6Count = districts[diseaseIndex][i].dummy6Count + facilityStats[diseaseIndex][count].dummy6Count;

                        // store away patient container pointer - active + inactive
                        districts[diseaseIndex][i].patientContainer.push_back(p);
                        districts[diseaseIndex][i].patientContainer.push_back(pI);
                    }
                }
                if (districtNotFound) {
                    if (debug) std::cout << "stats: agents district not found =  " << district << "\n";

                    // add it to the district array
                    districts[diseaseIndex][districtCount[diseaseIndex]].name = district;
                    districts[diseaseIndex][districtCount[diseaseIndex]].district = district;
                    districts[diseaseIndex][districtCount[diseaseIndex]].region = region;
                    districts[diseaseIndex][districtCount[diseaseIndex]].pop = facilityStats[diseaseIndex][count].pop;
                    districts[diseaseIndex][districtCount[diseaseIndex]].countP = facilityStats[diseaseIndex][count].countP;
                    districts[diseaseIndex][districtCount[diseaseIndex]].countPLtoF = facilityStats[diseaseIndex][count].countPLtoF;
                    districts[diseaseIndex][districtCount[diseaseIndex]].countPInf = facilityStats[diseaseIndex][count].countPInf;
                    districts[diseaseIndex][districtCount[diseaseIndex]].countPInfTreated = facilityStats[diseaseIndex][count].countPInfTreated;
                    districts[diseaseIndex][districtCount[diseaseIndex]].countPInfTreatedIn = facilityStats[diseaseIndex][count].countPInfTreatedIn;
                    districts[diseaseIndex][districtCount[diseaseIndex]].countPInfTreatedOut = facilityStats[diseaseIndex][count].countPInfTreatedOut;
                    districts[diseaseIndex][districtCount[diseaseIndex]].countPUninfTreated = facilityStats[diseaseIndex][count].countPUninfTreated;
                    districts[diseaseIndex][districtCount[diseaseIndex]].countPU = facilityStats[diseaseIndex][count].countPU;
                    districts[diseaseIndex][districtCount[diseaseIndex]].countPI = facilityStats[diseaseIndex][count].countPI;
                    districts[diseaseIndex][districtCount[diseaseIndex]].countPT = facilityStats[diseaseIndex][count].countPT;
                    districts[diseaseIndex][districtCount[diseaseIndex]].countPR = facilityStats[diseaseIndex][count].countPR;
                    districts[diseaseIndex][districtCount[diseaseIndex]].countPUT = facilityStats[diseaseIndex][count].countPUT;
                    districts[diseaseIndex][districtCount[diseaseIndex]].countPUTFN = facilityStats[diseaseIndex][count].countPUTFN;
                    districts[diseaseIndex][districtCount[diseaseIndex]].countPUTNS = facilityStats[diseaseIndex][count].countPUTNS;
                    districts[diseaseIndex][districtCount[diseaseIndex]].countPUTLtoF = facilityStats[diseaseIndex][count].countPUTLtoF;
                    districts[diseaseIndex][districtCount[diseaseIndex]].countPC = facilityStats[diseaseIndex][count].countPC;
                    districts[diseaseIndex][districtCount[diseaseIndex]].countPDI = facilityStats[diseaseIndex][count].countPDI;
                    districts[diseaseIndex][districtCount[diseaseIndex]].countPDIT = facilityStats[diseaseIndex][count].countPDIT;
                    districts[diseaseIndex][districtCount[diseaseIndex]].countPDIUT = facilityStats[diseaseIndex][count].countPDIUT;
                    districts[diseaseIndex][districtCount[diseaseIndex]].countPDIUTSR = facilityStats[diseaseIndex][count].countPDIUTSR;
                    districts[diseaseIndex][districtCount[diseaseIndex]].countPDIUTSNR = facilityStats[diseaseIndex][count].countPDIUTSNR;
                    districts[diseaseIndex][districtCount[diseaseIndex]].countPDIUTLtF = facilityStats[diseaseIndex][count].countPDIUTLtF;
                    districts[diseaseIndex][districtCount[diseaseIndex]].countPDU = facilityStats[diseaseIndex][count].countPDU;
                    districts[diseaseIndex][districtCount[diseaseIndex]].countPDOA = facilityStats[diseaseIndex][count].countPDOA;
                    districts[diseaseIndex][districtCount[diseaseIndex]].countPDIUTPTLtoF = facilityStats[diseaseIndex][count].countPDIUTPTLtoF;
                    districts[diseaseIndex][districtCount[diseaseIndex]].countPDIUTSRLtoF = facilityStats[diseaseIndex][count].countPDIUTSRLtoF;
                    districts[diseaseIndex][districtCount[diseaseIndex]].countPDITLtoF = facilityStats[diseaseIndex][count].countPDITLtoF;
                    districts[diseaseIndex][districtCount[diseaseIndex]].countPTC = facilityStats[diseaseIndex][count].countPTC;
                    districts[diseaseIndex][districtCount[diseaseIndex]].countPTD = facilityStats[diseaseIndex][count].countPTD;
                    districts[diseaseIndex][districtCount[diseaseIndex]].countPTR = facilityStats[diseaseIndex][count].countPTR;
                    districts[diseaseIndex][districtCount[diseaseIndex]].countRetreat = facilityStats[diseaseIndex][count].countRetreat;
                    districts[diseaseIndex][districtCount[diseaseIndex]].countRetreatCure = facilityStats[diseaseIndex][count].countRetreatCure;
                    districts[diseaseIndex][districtCount[diseaseIndex]].countRetreatDead = facilityStats[diseaseIndex][count].countRetreatDead;
                    districts[diseaseIndex][districtCount[diseaseIndex]].countPO = facilityStats[diseaseIndex][count].countPO;
                    districts[diseaseIndex][districtCount[diseaseIndex]].countPUI = facilityStats[diseaseIndex][count].countPUI;
                    districts[diseaseIndex][districtCount[diseaseIndex]].countPUINR = facilityStats[diseaseIndex][count].countPUINR;
                    districts[diseaseIndex][districtCount[diseaseIndex]].countPUIUT = facilityStats[diseaseIndex][count].countPUIUT;
                    districts[diseaseIndex][districtCount[diseaseIndex]].countMDInf = facilityStats[diseaseIndex][count].countMDInf;
                    districts[diseaseIndex][districtCount[diseaseIndex]].countMDUninf = facilityStats[diseaseIndex][count].countMDUninf;
                    districts[diseaseIndex][districtCount[diseaseIndex]].countTestPos = facilityStats[diseaseIndex][count].countTestPos;
                    districts[diseaseIndex][districtCount[diseaseIndex]].countTestNeg = facilityStats[diseaseIndex][count].countTestNeg;
                    districts[diseaseIndex][districtCount[diseaseIndex]].countTruePos = facilityStats[diseaseIndex][count].countTruePos;
                    districts[diseaseIndex][districtCount[diseaseIndex]].countFalsePos = facilityStats[diseaseIndex][count].countFalsePos;
                    districts[diseaseIndex][districtCount[diseaseIndex]].countTrueNeg = facilityStats[diseaseIndex][count].countTrueNeg;
                    districts[diseaseIndex][districtCount[diseaseIndex]].countFalseNeg = facilityStats[diseaseIndex][count].countFalseNeg;
                    districts[diseaseIndex][districtCount[diseaseIndex]].countDiagnosis = facilityStats[diseaseIndex][count].countDiagnosis;
                    districts[diseaseIndex][districtCount[diseaseIndex]].countDiagnosisIn = facilityStats[diseaseIndex][count].countDiagnosisIn;
                    districts[diseaseIndex][districtCount[diseaseIndex]].countDiagnosisOut = facilityStats[diseaseIndex][count].countDiagnosisOut;
                    districts[diseaseIndex][districtCount[diseaseIndex]].countPreTestLtoFOut = facilityStats[diseaseIndex][count].countPreTestLtoFOut;
                    districts[diseaseIndex][districtCount[diseaseIndex]].countPreTestLtoFOutTested = facilityStats[diseaseIndex][count].countPreTestLtoFOutTested;
                    districts[diseaseIndex][districtCount[diseaseIndex]].countPreTestLtoFOutUnTested = facilityStats[diseaseIndex][count].countPreTestLtoFOutUnTested;
                    districts[diseaseIndex][districtCount[diseaseIndex]].countPreTestLtoFOutUnTested2 = facilityStats[diseaseIndex][count].countPreTestLtoFOutUnTested2;
                    districts[diseaseIndex][districtCount[diseaseIndex]].countPreTestLtoFDead = facilityStats[diseaseIndex][count].countPreTestLtoFDead;
                    districts[diseaseIndex][districtCount[diseaseIndex]].countPreTestLtoFORepresent = facilityStats[diseaseIndex][count].countPreTestLtoFORepresent;
                    districts[diseaseIndex][districtCount[diseaseIndex]].onsetToTreat = facilityStats[diseaseIndex][count].onsetToTreat;
                    districts[diseaseIndex][districtCount[diseaseIndex]].onsetToTreatIn = facilityStats[diseaseIndex][count].onsetToTreatIn;
                    districts[diseaseIndex][districtCount[diseaseIndex]].onsetToTreatOut = facilityStats[diseaseIndex][count].onsetToTreatOut;
                    districts[diseaseIndex][districtCount[diseaseIndex]].timeInfectious = facilityStats[diseaseIndex][count].timeInfectious;
                    districts[diseaseIndex][districtCount[diseaseIndex]].sampleTransTime = facilityStats[diseaseIndex][count].sampleTransTime;
                    districts[diseaseIndex][districtCount[diseaseIndex]].sampleTransCount = facilityStats[diseaseIndex][count].sampleTransCount;
                    districts[diseaseIndex][districtCount[diseaseIndex]].sampleWaitForResourceInTime = facilityStats[diseaseIndex][count].sampleWaitForResourceInTime;
                    districts[diseaseIndex][districtCount[diseaseIndex]].sampleWaitForResourceInCount = facilityStats[diseaseIndex][count].sampleWaitForResourceInCount;
                    districts[diseaseIndex][districtCount[diseaseIndex]].sampleWaitForResourceOutTime = facilityStats[diseaseIndex][count].sampleWaitForResourceOutTime;
                    districts[diseaseIndex][districtCount[diseaseIndex]].sampleWaitForResourceOutCount = facilityStats[diseaseIndex][count].sampleWaitForResourceOutCount;
                    districts[diseaseIndex][districtCount[diseaseIndex]].sampleCollectToTestInTime = facilityStats[diseaseIndex][count].sampleCollectToTestInTime;
                    districts[diseaseIndex][districtCount[diseaseIndex]].sampleCollectToTestOutTime = facilityStats[diseaseIndex][count].sampleCollectToTestOutTime;
                    districts[diseaseIndex][districtCount[diseaseIndex]].sampleCollectToTestInCount = facilityStats[diseaseIndex][count].sampleCollectToTestInCount;
                    districts[diseaseIndex][districtCount[diseaseIndex]].sampleCollectToTestOutCount = facilityStats[diseaseIndex][count].sampleCollectToTestOutCount;
                    districts[diseaseIndex][districtCount[diseaseIndex]].sampleTestTime = facilityStats[diseaseIndex][count].sampleTestTime;
                    districts[diseaseIndex][districtCount[diseaseIndex]].sampleTestPosTimeIn = facilityStats[diseaseIndex][count].sampleTestPosTimeIn;
                    districts[diseaseIndex][districtCount[diseaseIndex]].sampleTestPosTimeOut = facilityStats[diseaseIndex][count].sampleTestPosTimeOut;
                    districts[diseaseIndex][districtCount[diseaseIndex]].sampleTestPosCountIn = facilityStats[diseaseIndex][count].sampleTestPosCountIn;
                    districts[diseaseIndex][districtCount[diseaseIndex]].sampleTestPosCountOut = facilityStats[diseaseIndex][count].sampleTestPosCountOut;
                    districts[diseaseIndex][districtCount[diseaseIndex]].sampleReturnTime = facilityStats[diseaseIndex][count].sampleReturnTime;
                    districts[diseaseIndex][districtCount[diseaseIndex]].sampleReturnedCount = facilityStats[diseaseIndex][count].sampleReturnedCount;
                    districts[diseaseIndex][districtCount[diseaseIndex]].sampleReturnedInCount = facilityStats[diseaseIndex][count].sampleReturnedInCount;
                    districts[diseaseIndex][districtCount[diseaseIndex]].sampleReturnedOutCount = facilityStats[diseaseIndex][count].sampleReturnedOutCount;
                    districts[diseaseIndex][districtCount[diseaseIndex]].sampleTestedInCount = facilityStats[diseaseIndex][count].sampleTestedInCount;
                    districts[diseaseIndex][districtCount[diseaseIndex]].sampleTestedOutCount = facilityStats[diseaseIndex][count].sampleTestedOutCount;
                    districts[diseaseIndex][districtCount[diseaseIndex]].sampleNotTestedInCount = facilityStats[diseaseIndex][count].sampleNotTestedInCount;
                    districts[diseaseIndex][districtCount[diseaseIndex]].sampleNotTestedOutCount = facilityStats[diseaseIndex][count].sampleNotTestedOutCount;
                    districts[diseaseIndex][districtCount[diseaseIndex]].sampleReturnToDeathTime = facilityStats[diseaseIndex][count].sampleReturnToDeathTime;
                    districts[diseaseIndex][districtCount[diseaseIndex]].sampleReturnToDeathPreDiagLtoFTime = facilityStats[diseaseIndex][count].sampleReturnToDeathPreDiagLtoFTime;
                    districts[diseaseIndex][districtCount[diseaseIndex]].sampleReturnToDeathInTrtTime = facilityStats[diseaseIndex][count].sampleReturnToDeathInTrtTime;
                    districts[diseaseIndex][districtCount[diseaseIndex]].sampleReturnToDeathInRetrtTime = facilityStats[diseaseIndex][count].sampleReturnToDeathInRetrtTime;
                    districts[diseaseIndex][districtCount[diseaseIndex]].sampleReturnToExitTime = facilityStats[diseaseIndex][count].sampleReturnToExitTime;
                    districts[diseaseIndex][districtCount[diseaseIndex]].sampleReturnToDeathCount = facilityStats[diseaseIndex][count].sampleReturnToDeathCount;
                    districts[diseaseIndex][districtCount[diseaseIndex]].sampleReturnToDeathPreDiagLtoFCount = facilityStats[diseaseIndex][count].sampleReturnToDeathPreDiagLtoFCount;
                    districts[diseaseIndex][districtCount[diseaseIndex]].sampleReturnToDeathInTrtCount = facilityStats[diseaseIndex][count].sampleReturnToDeathInTrtCount;
                    districts[diseaseIndex][districtCount[diseaseIndex]].sampleReturnToDeathInRetrtCount = facilityStats[diseaseIndex][count].sampleReturnToDeathInRetrtCount;
                    districts[diseaseIndex][districtCount[diseaseIndex]].sampleReturnToExitCount = facilityStats[diseaseIndex][count].sampleReturnToExitCount;
                    districts[diseaseIndex][districtCount[diseaseIndex]].sampleLtoFCount = facilityStats[diseaseIndex][count].sampleLtoFCount;
                    districts[diseaseIndex][districtCount[diseaseIndex]].sampleWaitTestCount = facilityStats[diseaseIndex][count].sampleWaitTestCount;
                    districts[diseaseIndex][districtCount[diseaseIndex]].sampleWaitTestCountIn = facilityStats[diseaseIndex][count].sampleWaitTestCountIn;
                    districts[diseaseIndex][districtCount[diseaseIndex]].sampleWaitTestCountOut = facilityStats[diseaseIndex][count].sampleWaitTestCountOut;
                    districts[diseaseIndex][districtCount[diseaseIndex]].patientNotNotified = facilityStats[diseaseIndex][count].patientNotNotified;
                    districts[diseaseIndex][districtCount[diseaseIndex]].patientNotNotifiedPos = facilityStats[diseaseIndex][count].patientNotNotifiedPos;
                    districts[diseaseIndex][districtCount[diseaseIndex]].patientNotNotifiedIn = facilityStats[diseaseIndex][count].patientNotNotifiedIn;
                    districts[diseaseIndex][districtCount[diseaseIndex]].patientNotNotifiedOut = facilityStats[diseaseIndex][count].patientNotNotifiedOut;
                    districts[diseaseIndex][districtCount[diseaseIndex]].inTestCount = facilityStats[diseaseIndex][count].inTestCount;
                    districts[diseaseIndex][districtCount[diseaseIndex]].outTestCount = facilityStats[diseaseIndex][count].outTestCount;
                    districts[diseaseIndex][districtCount[diseaseIndex]].inCollectedCount = facilityStats[diseaseIndex][count].inCollectedCount;
                    districts[diseaseIndex][districtCount[diseaseIndex]].outCollectedCount = facilityStats[diseaseIndex][count].outCollectedCount;
                    districts[diseaseIndex][districtCount[diseaseIndex]].test1Count = facilityStats[diseaseIndex][count].test1Count;
                    districts[diseaseIndex][districtCount[diseaseIndex]].test1CountIn = facilityStats[diseaseIndex][count].test1CountIn;
                    districts[diseaseIndex][districtCount[diseaseIndex]].test1CountOut = facilityStats[diseaseIndex][count].test1CountOut;
                    districts[diseaseIndex][districtCount[diseaseIndex]].test1PosCount = facilityStats[diseaseIndex][count].test1PosCount;
                    districts[diseaseIndex][districtCount[diseaseIndex]].test1TruePosCount = facilityStats[diseaseIndex][count].test1TruePosCount;
                    districts[diseaseIndex][districtCount[diseaseIndex]].test1NegCount = facilityStats[diseaseIndex][count].test1NegCount;
                    districts[diseaseIndex][districtCount[diseaseIndex]].test1TrueNegCount = facilityStats[diseaseIndex][count].test1TrueNegCount;
                    districts[diseaseIndex][districtCount[diseaseIndex]].test2Count = facilityStats[diseaseIndex][count].test2Count;
                    districts[diseaseIndex][districtCount[diseaseIndex]].test2CountIn = facilityStats[diseaseIndex][count].test2CountIn;
                    districts[diseaseIndex][districtCount[diseaseIndex]].test2CountOut = facilityStats[diseaseIndex][count].test2CountOut;
                    districts[diseaseIndex][districtCount[diseaseIndex]].test2PosCount = facilityStats[diseaseIndex][count].test2PosCount;
                    districts[diseaseIndex][districtCount[diseaseIndex]].test2TruePosCount = facilityStats[diseaseIndex][count].test2TruePosCount;
                    districts[diseaseIndex][districtCount[diseaseIndex]].test2NegCount = facilityStats[diseaseIndex][count].test2NegCount;
                    districts[diseaseIndex][districtCount[diseaseIndex]].test2TrueNegCount = facilityStats[diseaseIndex][count].test2TrueNegCount;
                    districts[diseaseIndex][districtCount[diseaseIndex]].test3Count = facilityStats[diseaseIndex][count].test3Count;
                    if (districts[diseaseIndex][districtCount[diseaseIndex]].test1Name == "") {

                        districts[diseaseIndex][districtCount[diseaseIndex]].test1Name = facilityStats[diseaseIndex][count].test1Name;
                    }
                    if (districts[diseaseIndex][districtCount[diseaseIndex]].test2Name == "") {

                        districts[diseaseIndex][districtCount[diseaseIndex]].test2Name = facilityStats[diseaseIndex][count].test2Name;
                    }
                    if (districts[diseaseIndex][districtCount[diseaseIndex]].test3Name == "") {

                        districts[diseaseIndex][districtCount[diseaseIndex]].test3Name = facilityStats[diseaseIndex][count].test3Name;
                    }
                    districts[diseaseIndex][districtCount[diseaseIndex]].countPreTestLtoFOutMinusTestedSamp = facilityStats[diseaseIndex][count].countPreTestLtoFOutMinusTestedSamp;
                    districts[diseaseIndex][districtCount[diseaseIndex]].dummy2Count = facilityStats[diseaseIndex][count].dummy2Count;
                    districts[diseaseIndex][districtCount[diseaseIndex]].dummy3Count = facilityStats[diseaseIndex][count].dummy3Count;
                    districts[diseaseIndex][districtCount[diseaseIndex]].dummy4Count = facilityStats[diseaseIndex][count].dummy4Count;
                    districts[diseaseIndex][districtCount[diseaseIndex]].dummy5Count = facilityStats[diseaseIndex][count].dummy5Count;
                    districts[diseaseIndex][districtCount[diseaseIndex]].dummy6Count = facilityStats[diseaseIndex][count].dummy6Count;

                    // store away patient container pointer - active and inactive
                    districts[diseaseIndex][districtCount[diseaseIndex]].patientContainer.push_back(p);
                    districts[diseaseIndex][districtCount[diseaseIndex]].patientContainer.push_back(pI);

                    districtCount[diseaseIndex] = districtCount[diseaseIndex] + 1;

                }
            }
        }

        count = count + 1;
    }

    // update total facility statistics
    if (fileFlag) {
    
        //treated = totalFacility.countPT + totalFacility.countPR + totalFacility.countPC + totalFacility.countPDI;
        //infected = treated + totalFacility.countPUT + totalFacility.countPDIUT;
        //treated = totalFacility.countPT + totalFacility.countPC;
        //infected = treated + totalFacility.countPUT + totalFacility.countPDIUT + totalFacility.countPR + totalFacility.countPDI;

        // count up all the sample results
        totalSampleCountPos = totalFacility.countTruePos + totalFacility.countFalsePos;
        totalSampleCountNeg = totalFacility.countTrueNeg + totalFacility.countFalseNeg;

        myfile << "Total" << "," << "0" << "," << totalFacility.tier << "," << totalFacility.district << "," << totalFacility.region << "," << totalFacility.pop << "," << facilityDisease << "," << int((float(totalFacility.countPInf) * 100000) / float(totalFacility.pop));
        myfile << "," << totalFacility.countP << "," << totalFacility.countPInf << "," << totalFacility.countPLtoF;
        myfile << "," << totalFacility.outCollectedCount + totalFacility.inCollectedCount << "," << totalFacility.outCollectedCount << "," << totalFacility.inCollectedCount;
        if ((totalFacility.sampleWaitForResourceInCount + totalFacility.sampleWaitForResourceOutCount) < 1) {
            myfile << "," << "0";
        }
        else {
            myfile << "," << float(totalFacility.sampleWaitForResourceInTime + totalFacility.sampleWaitForResourceOutTime)/float(totalFacility.sampleWaitForResourceInCount + totalFacility.sampleWaitForResourceOutCount);
        }
        if (totalFacility.sampleWaitForResourceInCount < 1) {
            myfile << "," << "0";
        }
        else {
            myfile << "," << float(totalFacility.sampleWaitForResourceInTime)/float(totalFacility.sampleWaitForResourceInCount);
        }
        if (totalFacility.sampleWaitForResourceOutCount < 1) {
            myfile << "," << "0" << ",";
        }
        else {
            myfile << "," << float(totalFacility.sampleWaitForResourceOutTime)/float(totalFacility.sampleWaitForResourceOutCount) << ",";
        }
        if (totalFacility.sampleTransCount  < 1) {
            myfile << "0" << "," << totalFacility.sampleTransCount;
        }
        else {
            myfile << float(totalFacility.sampleTransTime)/float(totalFacility.sampleTransCount) << "," << totalFacility.sampleTransCount;
        }
        myfile << "," << totalFacility.sampleTestedInCount +  totalFacility.sampleTestedOutCount << "," << totalFacility.sampleTestedInCount << "," << totalFacility.sampleTestedOutCount;
        //myfile << "," << totalFacility.sampleLtoFCount;
        
        
        if (debugOutput) {
        
            std::cout << "samples not tested in = " << totalFacility.sampleNotTestedInCount << "\n";
            std::cout << "samples not tested out = " << totalFacility.sampleNotTestedOutCount << "\n";
            std::cout << "sample returned, patient not notified in = " << totalFacility.sampleWaitTestCountIn << "\n";
            std::cout << "sample returned, patient not notified out = " << totalFacility.sampleWaitTestCountOut << "\n";
            std::cout << "samples waiting to be tested - in = " << totalFacility.patientNotNotifiedIn << "\n";
            std::cout << "samples waiting to be tested - out = " << totalFacility.patientNotNotifiedOut << "\n";
            std::cout << "pre-test LtFU - tested sample = " << totalFacility.countPreTestLtoFOutTested << "\n";
            std::cout << "pre-test LtFU - un tested sample = " << totalFacility.countPreTestLtoFOutUnTested << "\n";
            std::cout << "pre-test LtFU - tested2 sample = " << totalFacility.countPreTestLtoFOutUnTested2 << "\n";
            std::cout << "dead - treated -  infected = " << totalFacility.countPDIT << "\n";
            std::cout << "patients waiting sample not tested = " << totalFacility.sampleWaitTestCount << "\n";
            std::cout << "sample returned, patient not notified = " << totalFacility.patientNotNotified << "\n";
        }
        if (totalFacility.sampleCollectToTestInCount < 1) {
            myfile << "," << "0";
        }
        else {
            myfile << "," << float(totalFacility.sampleCollectToTestInTime)/float(totalFacility.sampleCollectToTestInCount);
        }
        myfile << "," << float(totalFacility.sampleCollectToTestInTime) << "," << totalFacility.sampleCollectToTestInCount;
        if (totalFacility.sampleCollectToTestOutCount < 1) {
            myfile << "," << "0";
        }
        else {
            myfile << "," << float(totalFacility.sampleCollectToTestOutTime)/float(totalFacility.sampleCollectToTestOutCount);
        }
        myfile << "," << float(totalFacility.sampleCollectToTestOutTime) << "," << totalFacility.sampleCollectToTestOutCount;
        myfile << "," << totalFacility.countTestPos << ",";
        if (totalSampleCountPos > 0) {
            myfile << float(totalFacility.countTruePos)/float(totalSampleCountPos) << ",";
            myfile << float(totalFacility.countFalsePos)/float(totalSampleCountPos) << ",";
        }
        else {
            myfile << "0" << "," << "0" << ",";
        }
        if (debugOutput) {
        
            std::cout << "facility countTestPos = " << totalFacility.countTestPos << ", totalSampleCountPos " << totalSampleCountPos  << "\n";
            std::cout << "facility true pos = " << totalFacility.countTruePos << ", false pos " << totalFacility.countFalsePos << ", true neg = " << totalFacility.countTrueNeg << ", false neg " << totalFacility.countFalseNeg << "\n";
        }
        myfile << totalFacility.countTestNeg << ",";
        if (totalSampleCountNeg > 0) {
            myfile << float(totalFacility.countTrueNeg)/float(totalSampleCountNeg) << ",";
            myfile << float(totalFacility.countFalseNeg)/float(totalSampleCountNeg);
        }
        else {
            myfile << "0" << "," << "0";
        }
        if (debug2) myfile << "," << "marker";

        myfile << "," << totalFacility.countMDInf << "," << totalFacility.countMDUninf;
        myfile << "," << totalFacility.countMDInf + totalFacility.countMDUninf + totalFacility.countTestPos;
        myfile << "," << totalFacility.countFalsePos << "," << totalFacility.countFalseNeg;
        myfile << "," << totalFacility.countDiagnosis << "," << totalFacility.countDiagnosisIn << "," << totalFacility.countDiagnosisOut << "," << totalFacility.countPreTestLtoFOut << "," << totalFacility.countPreTestLtoFOutMinusTestedSamp << "," << totalFacility.countPreTestLtoFDead << "," << totalFacility.countPreTestLtoFORepresent;
        if (totalFacility.sampleTestPosCountIn < 1) {
            myfile << "," << "0";
        }
        else {
            myfile << "," << float(totalFacility.sampleTestPosTimeIn)/float(totalFacility.sampleTestPosCountIn);
        }
        myfile << "," << float(totalFacility.sampleTestPosTimeIn) << "," << totalFacility.sampleTestPosCountIn;
        if (totalFacility.sampleTestPosCountOut < 1) {
            myfile << "," << "0";
        }
        else {
            myfile << "," << float(totalFacility.sampleTestPosTimeOut)/float(totalFacility.sampleTestPosCountOut);
        }
        myfile << "," << float(totalFacility.sampleTestPosTimeOut) << "," << totalFacility.sampleTestPosCountOut;
        if(debug2) myfile << "," << "marker";
               
        myfile << "," << totalFacility.countPInfTreated+totalFacility.countPUninfTreated << "," << totalFacility.countPInfTreated << "," << totalFacility.countPUninfTreated << "," << totalFacility.countPTC << "," << totalFacility.countPTD << "," << totalFacility.countPTR  << "," << totalFacility.countRetreat << "," << totalFacility.countRetreatCure << "," << totalFacility.countRetreatDead;
        if ((totalFacility.countPInfTreatedIn + totalFacility.countPInfTreatedOut) < 1) {
            myfile << "," << "0";
        }
        else {
            myfile << "," << float (totalFacility.onsetToTreat)/float(totalFacility.countPInfTreatedIn + totalFacility.countPInfTreatedOut);
        }
        myfile << "," << float (totalFacility.onsetToTreat) << "," << (totalFacility.countPInfTreatedIn + totalFacility.countPInfTreatedOut);
        if (totalFacility.countPInfTreatedIn < 1) {
            myfile << "," << "0";
        }
        else {
            myfile << "," << float (totalFacility.onsetToTreatIn)/float(totalFacility.countPInfTreatedIn);
        }
        myfile << "," << float (totalFacility.onsetToTreatIn) << "," << totalFacility.countPInfTreatedIn;
        if (totalFacility.countPInfTreatedOut < 1) {
            myfile << "," << "0";
        }
        else {
            myfile << "," << float (totalFacility.onsetToTreatOut)/float(totalFacility.countPInfTreatedOut);
        }
        myfile << "," << float (totalFacility.onsetToTreatOut) << "," << totalFacility.countPInfTreatedOut;
        if(debug2) myfile << "," << "marker";
               
        if (totalFacility.sampleReturnToDeathCount < 1) {
            myfile << "," << "0";
        }
        else {
            myfile << "," << float (totalFacility.sampleReturnToDeathTime)/float(totalFacility.sampleReturnToDeathCount);
        }
        myfile << "," << float (totalFacility.sampleReturnToDeathTime) << "," << totalFacility.sampleReturnToDeathCount;
        if (totalFacility.sampleReturnToDeathPreDiagLtoFCount < 1) {
            myfile << "," << "0";
        }
        else {
            myfile << "," << float (totalFacility.sampleReturnToDeathPreDiagLtoFTime)/float(totalFacility.sampleReturnToDeathPreDiagLtoFCount);
        }
        myfile << "," << float (totalFacility.sampleReturnToDeathPreDiagLtoFTime) << "," << totalFacility.sampleReturnToDeathPreDiagLtoFCount;
        if (totalFacility.sampleReturnToDeathInTrtCount < 1) {
            myfile << "," << "0";
        }
        else {
            myfile << "," << float (totalFacility.sampleReturnToDeathInTrtTime)/float(totalFacility.sampleReturnToDeathInTrtCount);
        }
        myfile << "," << float (totalFacility.sampleReturnToDeathInTrtTime) << "," << totalFacility.sampleReturnToDeathInTrtCount;
        if (totalFacility.sampleReturnToDeathInRetrtCount < 1) {
            myfile << "," << "0";
        }
        else {
            myfile << "," << float (totalFacility.sampleReturnToDeathInRetrtTime)/float(totalFacility.sampleReturnToDeathInRetrtCount);
        }
        myfile << "," << float (totalFacility.sampleReturnToDeathInRetrtTime) << "," << totalFacility.sampleReturnToDeathInRetrtCount;
        if (totalFacility.sampleReturnToExitCount < 1) {
            myfile << "," << "0";
        }
        else {
            myfile << "," << float (totalFacility.sampleReturnToExitTime)/float(totalFacility.sampleReturnToExitCount);
        }
        myfile << "," << float (totalFacility.sampleReturnToExitTime) << "," << totalFacility.sampleReturnToExitCount;
        myfile << "," << totalFacility.test1CountIn + totalFacility.test1CountOut  << "," << totalFacility.test1CountIn << "," << totalFacility.test1CountOut << "," << totalFacility.test2Count << "," << totalFacility. test2PosCount;
        if (totalFacility.test2PosCount < 1) {
            myfile << "," << "0";
        }
        else {
            myfile << "," << float (totalFacility.test2TruePosCount)/float(totalFacility.test2PosCount);
        }
        myfile << "," << totalFacility.test2NegCount;
        if (totalFacility.test2NegCount < 1) {
            myfile << "," << "0";
        }
        else {
            myfile << "," << float (totalFacility.test2TrueNegCount)/float(totalFacility.test2NegCount);
        }
        myfile << "," << totalFacility.test1PosCount;
        if (totalFacility.test1PosCount < 1) {
            myfile << "," << "0";
        }
        else {
            myfile << "," << float (totalFacility.test1TruePosCount)/float(totalFacility.test1PosCount);
        }
        myfile << "," << totalFacility.test1NegCount;
        if (totalFacility.test1NegCount < 1) {
            myfile << "," << "0";
        }
        else {
            myfile << "," << float (totalFacility.test1TrueNegCount)/float(totalFacility.test1NegCount);
        }
        if(debug2) myfile << "," << "marker";
               
        myfile << "," << totalFacility.countP << "," << totalFacility.countPI << "," << totalFacility.countPU  << "," << totalFacility.countPT;
        myfile << "," << totalFacility.sampleWaitTestCount << "," << totalFacility.sampleWaitTestCountIn << "," << totalFacility.sampleWaitTestCountOut;
        myfile << "," << totalFacility.countPreTestLtoFOutTested << "," << totalFacility.patientNotNotified << "," << totalFacility.patientNotNotifiedPos << "\n";

        myfile6 << "Total" << "," << "0" << ","  << totalFacility.tier << ","  << totalFacility.district << "," << totalFacility.region << "," << totalFacility.pop << "," << facilityDisease << "," << int((float(totalFacility.countPInf) * 100000) / float(totalFacility.pop));
        myfile6 << "," << totalFacility.countP << "," << totalFacility.countPInf << "," << totalFacility.countPLtoF;
        myfile6 << "," << totalFacility.outCollectedCount + totalFacility.inCollectedCount << "," << totalFacility.outCollectedCount << "," << totalFacility.inCollectedCount;
        if ((totalFacility.sampleWaitForResourceInCount + totalFacility.sampleWaitForResourceOutCount) < 1) {
            myfile6 << "," << "0";
        }
        else {
            myfile6 << "," << float(totalFacility.sampleWaitForResourceInTime + totalFacility.sampleWaitForResourceOutTime)/float(totalFacility.sampleWaitForResourceInCount + totalFacility.sampleWaitForResourceOutCount);
        }
        if (totalFacility.sampleWaitForResourceInCount < 1) {
            myfile6 << "," << "0";
        }
        else {
            myfile6 << "," << float(totalFacility.sampleWaitForResourceInTime)/float(totalFacility.sampleWaitForResourceInCount);
        }
        if (totalFacility.sampleWaitForResourceOutCount < 1) {
            myfile6 << "," << "0" << ",";
        }
        else {
            myfile6 << "," << float(totalFacility.sampleWaitForResourceOutTime)/float(totalFacility.sampleWaitForResourceOutCount) << ",";
        }
        if (totalFacility.sampleTransCount  < 1) {
            myfile6 << "0" << "," << totalFacility.sampleTransCount;
        }
        else {
            myfile6 << float(totalFacility.sampleTransTime)/float(totalFacility.sampleTransCount) << "," << totalFacility.sampleTransCount;
        }
        myfile6 << "," << totalFacility.sampleTestedInCount +  totalFacility.sampleTestedOutCount << "," << totalFacility.sampleTestedInCount << "," << totalFacility.sampleTestedOutCount;
        //myfile6 << "," << totalFacility.sampleLtoFCount;
        if (totalFacility.sampleCollectToTestInCount < 1) {
            myfile6 << "," << "0";
        }
        else {
            myfile6 << "," << float(totalFacility.sampleCollectToTestInTime)/float(totalFacility.sampleCollectToTestInCount);
        }
        myfile6 << "," << float(totalFacility.sampleCollectToTestInTime) << "," << totalFacility.sampleCollectToTestInCount;
        if (totalFacility.sampleCollectToTestOutCount < 1) {
            myfile6 << "," << "0";
        }
        else {
            myfile6 << "," << float(totalFacility.sampleCollectToTestOutTime)/float(totalFacility.sampleCollectToTestOutCount);
        }
        myfile6 << "," << float(totalFacility.sampleCollectToTestOutTime) << "," << totalFacility.sampleCollectToTestOutCount;
        myfile6 << "," << totalFacility.countTestPos << ",";
        if (totalSampleCountPos > 0) {
            myfile6 << float(totalFacility.countTruePos)/float(totalSampleCountPos) << ",";
            myfile6 << float(totalFacility.countFalsePos)/float(totalSampleCountPos) << ",";
        }
        else {
            myfile6 << "0" << "," << "0" << ",";
        }
        myfile6 << totalFacility.countTestNeg << ",";
        if (totalSampleCountNeg > 0) {
            myfile6 << float(totalFacility.countTrueNeg)/float(totalSampleCountNeg) << ",";
            myfile6 << float(totalFacility.countFalseNeg)/float(totalSampleCountNeg);
        }
        else {
            myfile6 << "0" << "," << "0";
        }
        if (debug2) myfile6 << "," << "marker";

        myfile6 << "," << totalFacility.countMDInf << "," << totalFacility.countMDUninf;
        myfile6 << "," << totalFacility.countMDInf + totalFacility.countMDUninf + totalFacility.countTestPos;
        myfile6 << "," << totalFacility.countFalsePos << "," << totalFacility.countFalseNeg;
        myfile6 << "," << totalFacility.countDiagnosis << "," << totalFacility.countDiagnosisIn << "," << totalFacility.countDiagnosisOut << "," << totalFacility.countPreTestLtoFOut << "," << totalFacility.countPreTestLtoFOutMinusTestedSamp << "," << totalFacility.countPreTestLtoFDead << "," << totalFacility.countPreTestLtoFORepresent;
        if (totalFacility.sampleTestPosCountIn < 1) {
            myfile6 << "," << "0";
        }
        else {
            myfile6 << "," << float(totalFacility.sampleTestPosTimeIn)/float(totalFacility.sampleTestPosCountIn);
        }
        myfile6 << "," << float(totalFacility.sampleTestPosTimeIn) << "," << totalFacility.sampleTestPosCountIn;
        if (totalFacility.sampleTestPosCountOut < 1) {
            myfile6 << "," << "0";
        }
        else {
            myfile6 << "," << float(totalFacility.sampleTestPosTimeOut)/float(totalFacility.sampleTestPosCountOut);
        }
        myfile6 << "," << float(totalFacility.sampleTestPosTimeOut) << "," << totalFacility.sampleTestPosCountOut;
        if(debug2) myfile6 << "," << "marker";
               
        myfile6 << "," << totalFacility.countPInfTreated+totalFacility.countPUninfTreated << "," << totalFacility.countPInfTreated << "," << totalFacility.countPUninfTreated << "," << totalFacility.countPTC << "," << totalFacility.countPTD << "," << totalFacility.countPTR  << "," << totalFacility.countRetreat << "," << totalFacility.countRetreatCure << "," << totalFacility.countRetreatDead;
        if ((totalFacility.countPInfTreatedIn + totalFacility.countPInfTreatedOut) < 1) {
            myfile6 << "," << "0";
        }
        else {
            myfile6 << "," << float (totalFacility.onsetToTreat)/float(totalFacility.countPInfTreatedIn + totalFacility.countPInfTreatedOut);
        }
        myfile6 << "," << float (totalFacility.onsetToTreat) << "," << (totalFacility.countPInfTreatedIn + totalFacility.countPInfTreatedOut);
        if (totalFacility.countPInfTreatedIn < 1) {
            myfile6 << "," << "0";
        }
        else {
            myfile6 << "," << float (totalFacility.onsetToTreatIn)/float(totalFacility.countPInfTreatedIn);
        }
        myfile6 << "," << float (totalFacility.onsetToTreatIn) << "," << totalFacility.countPInfTreatedIn;
        if (totalFacility.countPInfTreatedOut < 1) {
            myfile6 << "," << "0";
        }
        else {
            myfile6 << "," << float (totalFacility.onsetToTreatOut)/float(totalFacility.countPInfTreatedOut);
        }
        myfile6 << "," << float (totalFacility.onsetToTreatOut) << "," << totalFacility.countPInfTreatedOut;
        if(debug2) myfile6 << "," << "marker";
               
        if (totalFacility.sampleReturnToDeathCount < 1) {
            myfile6 << "," << "0";
        }
        else {
            myfile6 << "," << float (totalFacility.sampleReturnToDeathTime)/float(totalFacility.sampleReturnToDeathCount);
        }
        myfile6 << "," << float (totalFacility.sampleReturnToDeathTime) << "," << totalFacility.sampleReturnToDeathCount;
        if (totalFacility.sampleReturnToDeathPreDiagLtoFCount < 1) {
            myfile6 << "," << "0";
        }
        else {
            myfile6 << "," << float (totalFacility.sampleReturnToDeathPreDiagLtoFTime)/float(totalFacility.sampleReturnToDeathPreDiagLtoFCount);
        }
        myfile6 << "," << float (totalFacility.sampleReturnToDeathPreDiagLtoFTime) << "," << totalFacility.sampleReturnToDeathPreDiagLtoFCount;
        if (totalFacility.sampleReturnToDeathInTrtCount < 1) {
            myfile6 << "," << "0";
        }
        else {
            myfile6 << "," << float (totalFacility.sampleReturnToDeathInTrtTime)/float(totalFacility.sampleReturnToDeathInTrtCount);
        }
        myfile6 << "," << float (totalFacility.sampleReturnToDeathInTrtTime) << "," << totalFacility.sampleReturnToDeathInTrtCount;
        if (totalFacility.sampleReturnToDeathInRetrtCount < 1) {
            myfile6 << "," << "0";
        }
        else {
            myfile6 << "," << float (totalFacility.sampleReturnToDeathInRetrtTime)/float(totalFacility.sampleReturnToDeathInRetrtCount);
        }
        myfile6 << "," << float (totalFacility.sampleReturnToDeathInRetrtTime) << "," << totalFacility.sampleReturnToDeathInRetrtCount;
        if (totalFacility.sampleReturnToExitCount < 1) {
            myfile6 << "," << "0";
        }
        else {
            myfile6 << "," << float (totalFacility.sampleReturnToExitTime)/float(totalFacility.sampleReturnToExitCount);
        }
        myfile6 << "," << float (totalFacility.sampleReturnToExitTime) << "," << totalFacility.sampleReturnToExitCount;
        myfile6 << "," << totalFacility.test1CountIn + totalFacility.test1CountOut << "," << totalFacility.test1CountIn << "," << totalFacility.test1CountOut << "," << totalFacility.test2Count << "," << totalFacility. test2PosCount;
        if (totalFacility.test2PosCount < 1) {
            myfile6 << "," << "0";
        }
        else {
            myfile6 << "," << float (totalFacility.test2TruePosCount)/float(totalFacility.test2PosCount);
        }
        myfile6 << "," << totalFacility.test2NegCount;
        if (totalFacility.test2NegCount < 1) {
            myfile6 << "," << "0";
        }
        else {
            myfile6 << "," << float (totalFacility.test2TrueNegCount)/float(totalFacility.test2NegCount);
        }
        myfile6 << "," << totalFacility.test1PosCount;
        if (totalFacility.test1PosCount < 1) {
            myfile6 << "," << "0";
        }
        else {
            myfile6 << "," << float (totalFacility.test1TruePosCount)/float(totalFacility.test1PosCount);
        }
        myfile6 << "," << totalFacility.test1NegCount;
        if (totalFacility.test1NegCount < 1) {
            myfile6 << "," << "0";
        }
        else {
            myfile6 << "," << float (totalFacility.test1TrueNegCount)/float(totalFacility.test1NegCount);
        }
        if(debug2) myfile6 << "," << "marker";
               
        myfile6 << "," << totalFacility.countP << "," << totalFacility.countPI << "," << totalFacility.countPU  << "," << totalFacility.countPT;
        myfile6 << "," << totalFacility.sampleWaitTestCount << "," << totalFacility.sampleWaitTestCountIn << "," << totalFacility.sampleWaitTestCountOut;
        myfile6 << "," << totalFacility.countPreTestLtoFOutTested << "," << totalFacility.patientNotNotified << "," << totalFacility.patientNotNotifiedPos << "\n";

    }


    if (debug) std::cout << "stats: inside agent stats before regions and districts to file"  << "\n";

    // dump statistics for each region and district to a file
    if (fileFlag) {
        myfile << "-------------------------------------------------------- " << "\n";

        for (int w = 0; w < diseaseCount; w++) {

            if (debug) std::cout << "stats.patient.region.count_" << regionCount[w] << ",disease_" << diseaseList[w] << "\n";
            myfile << "region.count" << ",disease" << "\n";
            myfile << regionCount[w] << "," << diseaseList[w] << "\n";

            myfile << statHeader1 << statHeader2 << statHeader3 << statHeader4 << statHeader5 << statHeader5a << statHeader6 << statHeader7 << statHeader8;
            myfile << statHeader9 << statHeader10 << statHeader10a << statHeader11 << statHeader12 << statHeader12a << statHeader12b;
            myfile << statHeader13 << statHeader13a << statHeader13b << statHeader13c << statHeader13d << statHeader14 << statHeader15 << statHeader16 << statHeader17;
        
            myfile8 << statHeader1 << statHeader2 << statHeader3 << statHeader4 << statHeader5 << statHeader5a << statHeader6 << statHeader7 << statHeader8;
            myfile8 << statHeader9 << statHeader10 << statHeader10a << statHeader11 << statHeader12 << statHeader12a << statHeader12b;
            myfile8 << statHeader13 << statHeader13a << statHeader13b << statHeader13c << statHeader13d << statHeader14 << statHeader15 << statHeader16 << statHeader17;          
            
            if (individualFlag) {

                myfile9 << "-----------------------------------------------------------------------------------  " << "\n";
                myfile9 << "stats.patient.region.count_" << regionCount[w] << ",disease_" << diseaseList[w] << "\n";
                
                myfile2 << "-----------------------------------------------------------------------------------  " << "\n";
                myfile2 << "stats.patient.region.count_" << regionCount[w] << ",disease_" << diseaseList[w] << "\n";
            }

            for (int i = 0; i < regionCount[w]; i++) {

                // reset total counts for region
                regNoPatients[w] = 0;
                regAge[w] = 0;
                regNoAlive[w] = 0;
                regNoInfected[w] = 0;
                regNoTestPos[w] = 0;
                regNoMDOverride[w] = 0;
                regTotalTimeFromOnset[w] = 0;
                regTotalTimeInfectious[w] = 0;
                regTotaltimeInTreatment[w] = 0;
                regTotalTimeNotInTreatment[w] = 0;
                regTotalTimeFacilityReturnTime[w] = 0;
                regTotalTimeResultsReturned[w] = 0;
                regTotalNoOfReturnVisits[w] = 0;
                regDisease[w] = diseaseList[w];

                withTB = regions[w][i].countPT + regions[w][i].countPR + regions[w][i].countPUT + regions[w][i].countPDI + regions[w][i].countPDIUT;

                // prevalence = no. with TB * (100,000/population)          (no. with TB/pop = prevalence/100000)
                if (regions[w][i].pop == 0) {
                    prevalence = 0;
                }
                else {
                    //prevalence = int((float(withTB) * 100000) / float(regions[w][i].pop));
                    prevalence = int((float(regions[w][i].countPInf) * 100000) / float(regions[w][i].pop));
                }

                //treated = regions[w][i].countPT + regions[w][i].countPR + regions[w][i].countPC + regions[w][i].countPDI;
                //infected = treated + regions[w][i].countPUT + regions[w][i].countPDIUT;
                //treated = regions[w][i].countPT + regions[w][i].countPC;
                //infected = treated + regions[w][i].countPUT + regions[w][i].countPDIUT + regions[w][i].countPR + regions[w][i].countPDI;

                // update population by population growth if it goes on more that 1 year
                newPop = regions[w][i].pop;
                startIndex = referenceYear - 2015;
                endIndex = startIndex + noOfYears;
                if (noOfYears > 0) {

                    for (int i = startIndex; i < endIndex; i++) {

                        // multiply by growth rate for x number of years
                        newPop = (int) (newPop * popGrowthRateArray[i][1]);
                    }
                }
                // count up all the sample results
                totalSampleCountPos = regions[w][i].countTruePos + regions[w][i].countFalsePos;
                totalSampleCountNeg = regions[w][i].countTrueNeg + regions[w][i].countFalseNeg;

                if (debug4) std::cout << "stats: inside agent stats regions dead old age = " << regions[w][i].countPDOA  <<  ", timestep = " << timeStep <<"\n";

                //myfile << "stats: region =  " << regions[w][i].name << ", population = " << regions[w][i].pop << ", patients = " << regions[w][i].countP << ", calculated prevalence = " << prevalence << "\n";
                //myfile << "stats: patient" << " Untested-out = " << regions[w][i].countPU << " Untested-in =  " << regions[w][i].countPI << ", under treat = " << regions[w][i].countPT << ", failed treat = " << regions[w][i].countPR << ", untreated = " << regions[w][i].countPUT << ", cured = " << regions[w][i].countPC << ", uninfec-treat = " << regions[w][i].countPUIUT << ", Other = " << regions[w][i].countPO << "\n";
                //myfile << "               Non-infected - not treated  = " << regions[w][i].countPUI << ", Non-infected - not returned = " << regions[w][i].countPUINR << ", dead - infected = " << regions[w][i].countPDI << ", dead - infected untreated = " << regions[w][i].countPDIUT << ", dead - uninfected = " << regions[w][i].countPDU << "\n";
                myfile << regions[w][i].name << "," << regions[w][i].id << "," << regions[w][i].tier << "," << regions[w][i].district << "," << regions[w][i].region << "," << newPop << "," << diseaseList[w] << "," << prevalence;
                myfile << "," << regions[w][i].countP << "," << regions[w][i].countPInf << "," << regions[w][i].countPLtoF;
                myfile << "," << regions[w][i].outCollectedCount + regions[w][i].inCollectedCount << "," << regions[w][i].outCollectedCount << "," << regions[w][i].inCollectedCount;
                if ((regions[w][i].sampleWaitForResourceInCount + regions[w][i].sampleWaitForResourceOutCount) < 1) {
                    myfile << "," << "0";
                }
                else {
                    myfile << "," << float(regions[w][i].sampleWaitForResourceInTime + regions[w][i].sampleWaitForResourceOutTime)/float(regions[w][i].sampleWaitForResourceInCount + regions[w][i].sampleWaitForResourceOutCount);
                }
                if (regions[w][i].sampleWaitForResourceInCount < 1) {
                    myfile << "," << "0";
                }
                else {
                    myfile << "," << float(regions[w][i].sampleWaitForResourceInTime)/float(regions[w][i].sampleWaitForResourceInCount);
                }
                if (regions[w][i].sampleWaitForResourceOutCount < 1) {
                    myfile << "," << "0" << ",";
                }
                else {
                    myfile << "," << float(regions[w][i].sampleWaitForResourceOutTime)/float(regions[w][i].sampleWaitForResourceOutCount) << ",";
                }
                if (regions[w][i].sampleTransCount  < 1) {
                    myfile << "0" << "," << regions[w][i].sampleTransCount;
                }
                else {
                    myfile << float(regions[w][i].sampleTransTime)/float(regions[w][i].sampleTransCount) << "," << regions[w][i].sampleTransCount;
                }
                myfile << "," << regions[w][i].sampleTestedInCount +  regions[w][i].sampleTestedOutCount << "," << regions[w][i].sampleTestedInCount << "," << regions[w][i].sampleTestedOutCount;
                //myfile << "," << regions[w][i].sampleLtoFCount;
                if (regions[w][i].sampleCollectToTestInCount < 1) {
                    myfile << "," << "0";
                }
                else {
                    myfile << "," << float(regions[w][i].sampleCollectToTestInTime)/float(regions[w][i].sampleCollectToTestInCount);
                }
                myfile << "," << float(regions[w][i].sampleCollectToTestInTime) << "," << regions[w][i].sampleCollectToTestInCount;
                if (regions[w][i].sampleCollectToTestOutCount < 1) {
                    myfile << "," << "0";
                }
                else {
                    myfile << "," << float(regions[w][i].sampleCollectToTestOutTime)/float(regions[w][i].sampleCollectToTestOutCount);
                }
                myfile << "," << float(regions[w][i].sampleCollectToTestOutTime) << "," << regions[w][i].sampleCollectToTestOutCount;
                myfile << "," << regions[w][i].countTestPos << ",";
                if (totalSampleCountPos > 0) {
                    myfile << float(regions[w][i].countTruePos)/float(totalSampleCountPos) << ",";
                    myfile << float(regions[w][i].countFalsePos)/float(totalSampleCountPos) << ",";
                }
                else {
                    myfile << "0" << "," << "0" << ",";
                }
                myfile << regions[w][i].countTestNeg << ",";
                if (totalSampleCountNeg > 0) {
                    myfile << float(regions[w][i].countTrueNeg)/float(totalSampleCountNeg) << ",";
                    myfile << float(regions[w][i].countFalseNeg)/float(totalSampleCountNeg);
                }
                else {
                    myfile << "0" << "," << "0";
                }
                if (debug2) myfile << "," << "marker";

                myfile << "," << regions[w][i].countMDInf << "," << regions[w][i].countMDUninf;
                myfile << "," << regions[w][i].countMDInf + regions[w][i].countMDUninf + regions[w][i].countTestPos;
                myfile << "," << regions[w][i].countFalsePos << "," << regions[w][i].countFalseNeg;
                myfile << "," << regions[w][i].countDiagnosis << "," << regions[w][i].countDiagnosisIn << "," << regions[w][i].countDiagnosisOut << "," << regions[w][i].countPreTestLtoFOut << "," << regions[w][i].countPreTestLtoFOutMinusTestedSamp << "," << regions[w][i].countPreTestLtoFDead << "," << regions[w][i].countPreTestLtoFORepresent;
                if (regions[w][i].sampleTestPosCountIn < 1) {
                    myfile << "," << "0";
                }
                else {
                    myfile << "," << float(regions[w][i].sampleTestPosTimeIn)/float(regions[w][i].sampleTestPosCountIn);
                }
                myfile << "," << float(regions[w][i].sampleTestPosTimeIn) << "," << regions[w][i].sampleTestPosCountIn;
                if (regions[w][i].sampleTestPosCountOut < 1) {
                    myfile << "," << "0";
                }
                else {
                    myfile << "," << float(regions[w][i].sampleTestPosTimeOut)/float(regions[w][i].sampleTestPosCountOut);
                }
                myfile << "," << float(regions[w][i].sampleTestPosTimeOut) << "," << regions[w][i].sampleTestPosCountOut;
                if(debug2) myfile << "," << "marker";
               
                myfile << "," << regions[w][i].countPInfTreated+regions[w][i].countPUninfTreated << "," << regions[w][i].countPInfTreated << "," << regions[w][i].countPUninfTreated << "," << regions[w][i].countPTC << "," << regions[w][i].countPTD << "," << regions[w][i].countPTR  << "," << regions[w][i].countRetreat << "," << regions[w][i].countRetreatCure << "," << regions[w][i].countRetreatDead;
                if ((regions[w][i].countPInfTreatedIn + regions[w][i].countPInfTreatedOut) < 1) {
                    myfile << "," << "0";
                }
                else {
                    myfile << "," << float (regions[w][i].onsetToTreat)/float(regions[w][i].countPInfTreatedIn + regions[w][i].countPInfTreatedOut);
                }
                myfile << "," << float (regions[w][i].onsetToTreat) << "," << (regions[w][i].countPInfTreatedIn + regions[w][i].countPInfTreatedOut);
                if (regions[w][i].countPInfTreatedIn < 1) {
                    myfile << "," << "0";
                }
                else {
                    myfile << "," << float (regions[w][i].onsetToTreatIn)/float(regions[w][i].countPInfTreatedIn);
                }
                myfile << "," << float (regions[w][i].onsetToTreatIn) << "," << regions[w][i].countPInfTreatedIn;
                if (regions[w][i].countPInfTreatedOut < 1) {
                    myfile << "," << "0";
                }
                else {
                    myfile << "," << float (regions[w][i].onsetToTreatOut)/float(regions[w][i].countPInfTreatedOut);
                }
                myfile << "," << float (regions[w][i].onsetToTreatOut) << "," << regions[w][i].countPInfTreatedOut;
                if(debug2) myfile << "," << "marker";
               
                if (regions[w][i].sampleReturnToDeathCount < 1) {
                    myfile << "," << "0";
                }
                else {
                    myfile << "," << float (regions[w][i].sampleReturnToDeathTime)/float(regions[w][i].sampleReturnToDeathCount);
                }
                myfile << "," << float (regions[w][i].sampleReturnToDeathTime) << "," << regions[w][i].sampleReturnToDeathCount;
                if (regions[w][i].sampleReturnToDeathPreDiagLtoFCount < 1) {
                    myfile << "," << "0";
                }
                else {
                    myfile << "," << float (regions[w][i].sampleReturnToDeathPreDiagLtoFTime)/float(regions[w][i].sampleReturnToDeathPreDiagLtoFCount);
                }
                myfile << "," << float (regions[w][i].sampleReturnToDeathPreDiagLtoFTime) << "," << regions[w][i].sampleReturnToDeathPreDiagLtoFCount;
                if (regions[w][i].sampleReturnToDeathInTrtCount < 1) {
                    myfile << "," << "0";
                }
                else {
                    myfile << "," << float (regions[w][i].sampleReturnToDeathInTrtTime)/float(regions[w][i].sampleReturnToDeathInTrtCount);
                }
                myfile << "," << float (regions[w][i].sampleReturnToDeathInTrtTime) << "," << regions[w][i].sampleReturnToDeathInTrtCount;
                if (regions[w][i].sampleReturnToDeathInRetrtCount < 1) {
                    myfile << "," << "0";
                }
                else {
                    myfile << "," << float (regions[w][i].sampleReturnToDeathInRetrtTime)/float(regions[w][i].sampleReturnToDeathInRetrtCount);
                }
                myfile << "," << float (regions[w][i].sampleReturnToDeathInRetrtTime) << "," << regions[w][i].sampleReturnToDeathInRetrtCount;
                if (regions[w][i].sampleReturnToExitCount < 1) {
                    myfile << "," << "0";
                }
                else {
                    myfile << "," << float (regions[w][i].sampleReturnToExitTime)/float(regions[w][i].sampleReturnToExitCount);
                }
                myfile << "," << float (regions[w][i].sampleReturnToExitTime) << "," << regions[w][i].sampleReturnToExitCount;
                myfile << "," << regions[w][i].test1CountIn + regions[w][i].test1CountOut << "," << regions[w][i].test1CountIn << "," << regions[w][i].test1CountOut << "," << regions[w][i].test2Count << "," << regions[w][i]. test2PosCount;
                if (regions[w][i].test2PosCount < 1) {
                    myfile << "," << "0";
                }
                else {
                    myfile << "," << float (regions[w][i].test2TruePosCount)/float(regions[w][i].test2PosCount);
                }
                myfile << "," << regions[w][i].test2NegCount;
                if (regions[w][i].test2NegCount < 1) {
                    myfile << "," << "0";
                }
                else {
                    myfile << "," << float (regions[w][i].test2TrueNegCount)/float(regions[w][i].test2NegCount);
                }
                myfile << "," << regions[w][i].test1PosCount;
                if (regions[w][i].test1PosCount < 1) {
                    myfile << "," << "0";
                }
                else {
                    myfile << "," << float (regions[w][i].test1TruePosCount)/float(regions[w][i].test1PosCount);
                }
                myfile << "," << regions[w][i].test1NegCount;
                if (regions[w][i].test1NegCount < 1) {
                    myfile << "," << "0";
                }
                else {
                    myfile << "," << float (regions[w][i].test1TrueNegCount)/float(regions[w][i].test1NegCount);
                }
                if(debug2) myfile << "," << "marker";
               
                myfile << "," << regions[w][i].countP << "," << regions[w][i].countPI << "," << regions[w][i].countPU  << "," << regions[w][i].countPT;
                myfile << "," << regions[w][i].sampleWaitTestCount << "," << regions[w][i].sampleWaitTestCountIn << "," << regions[w][i].sampleWaitTestCountOut;
                myfile << "," << regions[w][i].countPreTestLtoFOutTested << "," << regions[w][i].patientNotNotified << "," << regions[w][i].patientNotNotifiedPos << "\n";



                myfile8 << regions[w][i].name << "," << regions[w][i].id << "," << regions[w][i].tier << "," << regions[w][i].district << "," << regions[w][i].region << "," << newPop << "," << diseaseList[w] << "," << prevalence;
                myfile8 << "," << regions[w][i].countP << "," << regions[w][i].countPInf << "," << regions[w][i].countPLtoF;
                myfile8 << "," << regions[w][i].outCollectedCount + regions[w][i].inCollectedCount << "," << regions[w][i].outCollectedCount << "," << regions[w][i].inCollectedCount;
                if ((regions[w][i].sampleWaitForResourceInCount + regions[w][i].sampleWaitForResourceOutCount) < 1) {
                    myfile8 << "," << "0";
                }
                else {
                    myfile8 << "," << float(regions[w][i].sampleWaitForResourceInTime + regions[w][i].sampleWaitForResourceOutTime)/float(regions[w][i].sampleWaitForResourceInCount + regions[w][i].sampleWaitForResourceOutCount);
                }
                if (regions[w][i].sampleWaitForResourceInCount < 1) {
                    myfile8 << "," << "0";
                }
                else {
                    myfile8 << "," << float(regions[w][i].sampleWaitForResourceInTime)/float(regions[w][i].sampleWaitForResourceInCount);
                }
                if (regions[w][i].sampleWaitForResourceOutCount < 1) {
                    myfile8 << "," << "0" << ",";
                }
                else {
                    myfile8 << "," << float(regions[w][i].sampleWaitForResourceOutTime)/float(regions[w][i].sampleWaitForResourceOutCount) << ",";
                }
                if (regions[w][i].sampleTransCount  < 1) {
                    myfile8 << "0" << "," << regions[w][i].sampleTransCount;
                }
                else {
                    myfile8 << float(regions[w][i].sampleTransTime)/float(regions[w][i].sampleTransCount) << "," << regions[w][i].sampleTransCount;
                }
                myfile8 << "," << regions[w][i].sampleTestedInCount +  regions[w][i].sampleTestedOutCount << "," << regions[w][i].sampleTestedInCount << "," << regions[w][i].sampleTestedOutCount;
                //myfile8 << "," << regions[w][i].sampleLtoFCount;
                if (regions[w][i].sampleCollectToTestInCount < 1) {
                    myfile8 << "," << "0";
                }
                else {
                    myfile8 << "," << float(regions[w][i].sampleCollectToTestInTime)/float(regions[w][i].sampleCollectToTestInCount);
                }
                myfile8 << "," << float(regions[w][i].sampleCollectToTestInTime) << "," << regions[w][i].sampleCollectToTestInCount;
                if (regions[w][i].sampleCollectToTestOutCount < 1) {
                    myfile8 << "," << "0";
                }
                else {
                    myfile8 << "," << float(regions[w][i].sampleCollectToTestOutTime)/float(regions[w][i].sampleCollectToTestOutCount);
                }
                myfile8 << "," << float(regions[w][i].sampleCollectToTestOutTime) << "," << regions[w][i].sampleCollectToTestOutCount;
                myfile8 << "," << regions[w][i].countTestPos << ",";
                if (totalSampleCountPos > 0) {
                    myfile8 << float(regions[w][i].countTruePos)/float(totalSampleCountPos) << ",";
                    myfile8 << float(regions[w][i].countFalsePos)/float(totalSampleCountPos) << ",";
                }
                else {
                    myfile8 << "0" << "," << "0" << ",";
                }
                myfile8 << regions[w][i].countTestNeg << ",";
                if (totalSampleCountNeg > 0) {
                    myfile8 << float(regions[w][i].countTrueNeg)/float(totalSampleCountNeg) << ",";
                    myfile8 << float(regions[w][i].countFalseNeg)/float(totalSampleCountNeg);
                }
                else {
                    myfile8 << "0" << "," << "0";
                }
                if (debug2) myfile8 << "," << "marker";

                myfile8 << "," << regions[w][i].countMDInf << "," << regions[w][i].countMDUninf;
                myfile8 << "," << regions[w][i].countMDInf + regions[w][i].countMDUninf + regions[w][i].countTestPos;
                myfile8 << "," << regions[w][i].countFalsePos << "," << regions[w][i].countFalseNeg;
                myfile8 << "," << regions[w][i].countDiagnosis << "," << regions[w][i].countDiagnosisIn << "," << regions[w][i].countDiagnosisOut << "," << regions[w][i].countPreTestLtoFOut << "," << regions[w][i].countPreTestLtoFOutMinusTestedSamp << "," << regions[w][i].countPreTestLtoFDead << "," << regions[w][i].countPreTestLtoFORepresent;
                if (regions[w][i].sampleTestPosCountIn < 1) {
                    myfile8 << "," << "0";
                }
                else {
                    myfile8 << "," << float(regions[w][i].sampleTestPosTimeIn)/float(regions[w][i].sampleTestPosCountIn);
                }
                myfile8 << "," << float(regions[w][i].sampleTestPosTimeIn) << "," << regions[w][i].sampleTestPosCountIn;
                if (regions[w][i].sampleTestPosCountOut < 1) {
                    myfile8 << "," << "0";
                }
                else {
                    myfile8 << "," << float(regions[w][i].sampleTestPosTimeOut)/float(regions[w][i].sampleTestPosCountOut);
                }
                myfile8 << "," << float(regions[w][i].sampleTestPosTimeOut) << "," << regions[w][i].sampleTestPosCountOut;
                if(debug2) myfile8 << "," << "marker";
               
                myfile8 << "," << regions[w][i].countPInfTreated+regions[w][i].countPUninfTreated << "," << regions[w][i].countPInfTreated << "," << regions[w][i].countPUninfTreated << "," << regions[w][i].countPTC << "," << regions[w][i].countPTD << "," << regions[w][i].countPTR  << "," << regions[w][i].countRetreat << "," << regions[w][i].countRetreatCure << "," << regions[w][i].countRetreatDead;
                if ((regions[w][i].countPInfTreatedIn + regions[w][i].countPInfTreatedOut) < 1) {
                    myfile8 << "," << "0";
                }
                else {
                    myfile8 << "," << float (regions[w][i].onsetToTreat)/float(regions[w][i].countPInfTreatedIn + regions[w][i].countPInfTreatedOut);
                }
                myfile8 << "," << float (regions[w][i].onsetToTreat) << "," << (regions[w][i].countPInfTreatedIn + regions[w][i].countPInfTreatedOut);
                if (regions[w][i].countPInfTreatedIn < 1) {
                    myfile8 << "," << "0";
                }
                else {
                    myfile8 << "," << float (regions[w][i].onsetToTreatIn)/float(regions[w][i].countPInfTreatedIn);
                }
                myfile8 << "," << float (regions[w][i].onsetToTreatIn) << "," << regions[w][i].countPInfTreatedIn;
                if (regions[w][i].countPInfTreatedOut < 1) {
                    myfile8 << "," << "0";
                }
                else {
                    myfile8 << "," << float (regions[w][i].onsetToTreatOut)/float(regions[w][i].countPInfTreatedOut);
                }
                myfile8 << "," << float (regions[w][i].onsetToTreatOut) << "," << regions[w][i].countPInfTreatedOut;
                if(debug2) myfile8 << "," << "marker";
               
                if (regions[w][i].sampleReturnToDeathCount < 1) {
                    myfile8 << "," << "0";
                }
                else {
                    myfile8 << "," << float (regions[w][i].sampleReturnToDeathTime)/float(regions[w][i].sampleReturnToDeathCount);
                }
                myfile8 << "," << float (regions[w][i].sampleReturnToDeathTime) << "," << regions[w][i].sampleReturnToDeathCount;
                if (regions[w][i].sampleReturnToDeathPreDiagLtoFCount < 1) {
                    myfile8 << "," << "0";
                }
                else {
                    myfile8 << "," << float (regions[w][i].sampleReturnToDeathPreDiagLtoFTime)/float(regions[w][i].sampleReturnToDeathPreDiagLtoFCount);
                }
                myfile8 << "," << float (regions[w][i].sampleReturnToDeathPreDiagLtoFTime) << "," << regions[w][i].sampleReturnToDeathPreDiagLtoFCount;
                if (regions[w][i].sampleReturnToDeathInTrtCount < 1) {
                    myfile8 << "," << "0";
                }
                else {
                    myfile8 << "," << float (regions[w][i].sampleReturnToDeathInTrtTime)/float(regions[w][i].sampleReturnToDeathInTrtCount);
                }
                myfile8 << "," << float (regions[w][i].sampleReturnToDeathInTrtTime) << "," << regions[w][i].sampleReturnToDeathInTrtCount;
                if (regions[w][i].sampleReturnToDeathInRetrtCount < 1) {
                    myfile8 << "," << "0";
                }
                else {
                    myfile8 << "," << float (regions[w][i].sampleReturnToDeathInRetrtTime)/float(regions[w][i].sampleReturnToDeathInRetrtCount);
                }
                myfile8 << "," << float (regions[w][i].sampleReturnToDeathInRetrtTime) << "," << regions[w][i].sampleReturnToDeathInRetrtCount;
                if (regions[w][i].sampleReturnToExitCount < 1) {
                    myfile8 << "," << "0";
                }
                else {
                    myfile8 << "," << float (regions[w][i].sampleReturnToExitTime)/float(regions[w][i].sampleReturnToExitCount);
                }
                myfile8 << "," << float (regions[w][i].sampleReturnToExitTime) << "," << regions[w][i].sampleReturnToExitCount;
                myfile8 << "," << regions[w][i].test1CountIn + regions[w][i].test1CountOut << "," << regions[w][i].test1CountIn << "," << regions[w][i].test1CountOut << "," << regions[w][i].test2Count << "," << regions[w][i]. test2PosCount;
                if (regions[w][i].test2PosCount < 1) {
                    myfile8 << "," << "0";
                }
                else {
                    myfile8 << "," << float (regions[w][i].test2TruePosCount)/float(regions[w][i].test2PosCount);
                }
                myfile8 << "," << regions[w][i].test2NegCount;
                if (regions[w][i].test2NegCount < 1) {
                    myfile8 << "," << "0";
                }
                else {
                    myfile8 << "," << float (regions[w][i].test2TrueNegCount)/float(regions[w][i].test2NegCount);
                }
                myfile8 << "," << regions[w][i].test1PosCount;
                if (regions[w][i].test1PosCount < 1) {
                    myfile8 << "," << "0";
                }
                else {
                    myfile8 << "," << float (regions[w][i].test1TruePosCount)/float(regions[w][i].test1PosCount);
                }
                myfile8 << "," << regions[w][i].test1NegCount;
                if (regions[w][i].test1NegCount < 1) {
                    myfile8 << "," << "0";
                }
                else {
                    myfile8 << "," << float (regions[w][i].test1TrueNegCount)/float(regions[w][i].test1NegCount);
                }
                if(debug2) myfile8 << "," << "marker";
               
                myfile8 << "," << regions[w][i].countP << "," << regions[w][i].countPI << "," << regions[w][i].countPU  << "," << regions[w][i].countPT;
                myfile8 << "," << regions[w][i].sampleWaitTestCount << "," << regions[w][i].sampleWaitTestCountIn << "," << regions[w][i].sampleWaitTestCountOut;
                myfile8 << "," << regions[w][i].countPreTestLtoFOutTested << "," << regions[w][i].patientNotNotified << "," << regions[w][i].patientNotNotifiedPos << "\n";

                diseaseTotalRegion[w].pop = diseaseTotalRegion[w].pop + regions[w][i].pop;
                diseaseTotalRegion[w].countP = diseaseTotalRegion[w].countP + regions[w][i].countP;
                diseaseTotalRegion[w].countPLtoF = diseaseTotalRegion[w].countPLtoF + regions[w][i].countPLtoF;
                diseaseTotalRegion[w].countPInf = diseaseTotalRegion[w].countPInf + regions[w][i].countPInf;
                diseaseTotalRegion[w].countPInfTreated = diseaseTotalRegion[w].countPInfTreated + regions[w][i].countPInfTreated;
                diseaseTotalRegion[w].countPInfTreatedIn = diseaseTotalRegion[w].countPInfTreatedIn + regions[w][i].countPInfTreatedIn;
                diseaseTotalRegion[w].countPInfTreatedOut = diseaseTotalRegion[w].countPInfTreatedOut + regions[w][i].countPInfTreatedOut;
                diseaseTotalRegion[w].countPUninfTreated = diseaseTotalRegion[w].countPUninfTreated + regions[w][i].countPUninfTreated;
                diseaseTotalRegion[w].countPU = diseaseTotalRegion[w].countPU + regions[w][i].countPU;
                diseaseTotalRegion[w].countPI = diseaseTotalRegion[w].countPI + regions[w][i].countPI;
                diseaseTotalRegion[w].countPT = diseaseTotalRegion[w].countPT + regions[w][i].countPT;
                diseaseTotalRegion[w].countPR = diseaseTotalRegion[w].countPR + regions[w][i].countPR;
                diseaseTotalRegion[w].countPUT = diseaseTotalRegion[w].countPUT + regions[w][i].countPUT;
                diseaseTotalRegion[w].countPUTFN = diseaseTotalRegion[w].countPUTFN + regions[w][i].countPUTFN;
                diseaseTotalRegion[w].countPUTNS = diseaseTotalRegion[w].countPUTNS + regions[w][i].countPUTNS;
                diseaseTotalRegion[w].countPUTLtoF = diseaseTotalRegion[w].countPUTLtoF + regions[w][i].countPUTLtoF;
                diseaseTotalRegion[w].countPC = diseaseTotalRegion[w].countPC + regions[w][i].countPC;
                diseaseTotalRegion[w].countPDI = diseaseTotalRegion[w].countPDI + regions[w][i].countPDI;
                diseaseTotalRegion[w].countPDIT = diseaseTotalRegion[w].countPDIT + regions[w][i].countPDIT;
                diseaseTotalRegion[w].countPDIUT = diseaseTotalRegion[w].countPDIUT + regions[w][i].countPDIUT;
                diseaseTotalRegion[w].countPDIUTSR = diseaseTotalRegion[w].countPDIUTSR + regions[w][i].countPDIUTSR;
                diseaseTotalRegion[w].countPDIUTSNR = diseaseTotalRegion[w].countPDIUTSNR + regions[w][i].countPDIUTSNR;
                diseaseTotalRegion[w].countPDIUTLtF = diseaseTotalRegion[w].countPDIUTLtF + regions[w][i].countPDIUTLtF;
                diseaseTotalRegion[w].countPDU = diseaseTotalRegion[w].countPDU + regions[w][i].countPDU;
                diseaseTotalRegion[w].countPDOA = diseaseTotalRegion[w].countPDOA + regions[w][i].countPDOA;
                diseaseTotalRegion[w].countPDIUTPTLtoF = diseaseTotalRegion[w].countPDIUTPTLtoF + regions[w][i].countPDIUTPTLtoF;
                diseaseTotalRegion[w].countPDIUTSRLtoF = diseaseTotalRegion[w].countPDIUTSRLtoF + regions[w][i].countPDIUTSRLtoF;
                diseaseTotalRegion[w].countPDITLtoF = diseaseTotalRegion[w].countPDITLtoF + regions[w][i].countPDITLtoF;
                diseaseTotalRegion[w].countPTC = diseaseTotalRegion[w].countPTC + regions[w][i].countPTC;
                diseaseTotalRegion[w].countPTD = diseaseTotalRegion[w].countPTD + regions[w][i].countPTD;
                diseaseTotalRegion[w].countPTR = diseaseTotalRegion[w].countPTR + regions[w][i].countPTR;
                diseaseTotalRegion[w].countRetreat = diseaseTotalRegion[w].countRetreat + regions[w][i].countRetreat;
                diseaseTotalRegion[w].countRetreatCure = diseaseTotalRegion[w].countRetreatCure + regions[w][i].countRetreatCure;
                diseaseTotalRegion[w].countRetreatDead = diseaseTotalRegion[w].countRetreatDead + regions[w][i].countRetreatDead;
                diseaseTotalRegion[w].countPO = diseaseTotalRegion[w].countPO + regions[w][i].countPO;
                diseaseTotalRegion[w].countPUI = diseaseTotalRegion[w].countPUI + regions[w][i].countPUI;
                diseaseTotalRegion[w].countPUINR = diseaseTotalRegion[w].countPUINR + regions[w][i].countPUINR;
                diseaseTotalRegion[w].countPUIUT = diseaseTotalRegion[w].countPUIUT + regions[w][i].countPUIUT;
                diseaseTotalRegion[w].countMDInf = diseaseTotalRegion[w].countMDInf + regions[w][i].countMDInf;
                diseaseTotalRegion[w].countMDUninf = diseaseTotalRegion[w].countMDUninf + regions[w][i].countMDUninf;
                diseaseTotalRegion[w].countTestPos = diseaseTotalRegion[w].countTestPos + regions[w][i].countTestPos;
                diseaseTotalRegion[w].countTestNeg = diseaseTotalRegion[w].countTestNeg + regions[w][i].countTestNeg;
                diseaseTotalRegion[w].countTruePos = diseaseTotalRegion[w].countTruePos + regions[w][i].countTruePos;
                diseaseTotalRegion[w].countFalsePos = diseaseTotalRegion[w].countFalsePos + regions[w][i].countFalsePos;
                diseaseTotalRegion[w].countTrueNeg = diseaseTotalRegion[w].countTrueNeg + regions[w][i].countTrueNeg;
                diseaseTotalRegion[w].countFalseNeg = diseaseTotalRegion[w].countFalseNeg + regions[w][i].countFalseNeg;
                diseaseTotalRegion[w].countDiagnosis = diseaseTotalRegion[w].countDiagnosis + regions[w][i].countDiagnosis;
                diseaseTotalRegion[w].countDiagnosisIn = diseaseTotalRegion[w].countDiagnosisIn + regions[w][i].countDiagnosisIn;
                diseaseTotalRegion[w].countDiagnosisOut = diseaseTotalRegion[w].countDiagnosisOut + regions[w][i].countDiagnosisOut;
                diseaseTotalRegion[w].countPreTestLtoFOut = diseaseTotalRegion[w].countPreTestLtoFOut + regions[w][i].countPreTestLtoFOut;
                diseaseTotalRegion[w].countPreTestLtoFOutTested = diseaseTotalRegion[w].countPreTestLtoFOutTested + regions[w][i].countPreTestLtoFOutTested;
                diseaseTotalRegion[w].countPreTestLtoFOutUnTested = diseaseTotalRegion[w].countPreTestLtoFOutUnTested + regions[w][i].countPreTestLtoFOutUnTested;
                diseaseTotalRegion[w].countPreTestLtoFOutUnTested2 = diseaseTotalRegion[w].countPreTestLtoFOutUnTested2 + regions[w][i].countPreTestLtoFOutUnTested2;
                diseaseTotalRegion[w].countPreTestLtoFDead = diseaseTotalRegion[w].countPreTestLtoFDead + regions[w][i].countPreTestLtoFDead;
                diseaseTotalRegion[w].countPreTestLtoFORepresent = diseaseTotalRegion[w].countPreTestLtoFORepresent + regions[w][i].countPreTestLtoFORepresent;
                diseaseTotalRegion[w].onsetToTreat = diseaseTotalRegion[w].onsetToTreat + regions[w][i].onsetToTreat;
                diseaseTotalRegion[w].onsetToTreatIn = diseaseTotalRegion[w].onsetToTreatIn + regions[w][i].onsetToTreatIn;
                diseaseTotalRegion[w].onsetToTreatOut = diseaseTotalRegion[w].onsetToTreatOut + regions[w][i].onsetToTreatOut;
                diseaseTotalRegion[w].timeInfectious = diseaseTotalRegion[w].timeInfectious + regions[w][i].timeInfectious;
                diseaseTotalRegion[w].sampleTransTime = diseaseTotalRegion[w].sampleTransTime + regions[w][i].sampleTransTime;
                diseaseTotalRegion[w].sampleTransCount = diseaseTotalRegion[w].sampleTransCount + regions[w][i].sampleTransCount;
                diseaseTotalRegion[w].sampleWaitForResourceInTime = diseaseTotalRegion[w].sampleWaitForResourceInTime + regions[w][i].sampleWaitForResourceInTime;
                diseaseTotalRegion[w].sampleWaitForResourceInCount = diseaseTotalRegion[w].sampleWaitForResourceInCount + regions[w][i].sampleWaitForResourceInCount;
                diseaseTotalRegion[w].sampleWaitForResourceOutTime = diseaseTotalRegion[w].sampleWaitForResourceOutTime + regions[w][i].sampleWaitForResourceOutTime;
                diseaseTotalRegion[w].sampleWaitForResourceOutCount = diseaseTotalRegion[w].sampleWaitForResourceOutCount + regions[w][i].sampleWaitForResourceOutCount;
                diseaseTotalRegion[w].sampleCollectToTestInTime = diseaseTotalRegion[w].sampleCollectToTestInTime + regions[w][i].sampleCollectToTestInTime;
                diseaseTotalRegion[w].sampleCollectToTestOutTime = diseaseTotalRegion[w].sampleCollectToTestOutTime + regions[w][i].sampleCollectToTestOutTime;
                diseaseTotalRegion[w].sampleCollectToTestInCount = diseaseTotalRegion[w].sampleCollectToTestInCount + regions[w][i].sampleCollectToTestInCount;
                diseaseTotalRegion[w].sampleCollectToTestOutCount = diseaseTotalRegion[w].sampleCollectToTestOutCount + regions[w][i].sampleCollectToTestOutCount;
                diseaseTotalRegion[w].sampleTestTime = diseaseTotalRegion[w].sampleTestTime + regions[w][i].sampleTestTime;
                diseaseTotalRegion[w].sampleTestPosTimeIn = diseaseTotalRegion[w].sampleTestPosTimeIn + regions[w][i].sampleTestPosTimeIn;
                diseaseTotalRegion[w].sampleTestPosTimeOut = diseaseTotalRegion[w].sampleTestPosTimeOut + regions[w][i].sampleTestPosTimeOut;
                diseaseTotalRegion[w].sampleTestPosCountIn = diseaseTotalRegion[w].sampleTestPosCountIn + regions[w][i].sampleTestPosCountIn;
                diseaseTotalRegion[w].sampleTestPosCountOut = diseaseTotalRegion[w].sampleTestPosCountOut + regions[w][i].sampleTestPosCountOut;
                diseaseTotalRegion[w].sampleReturnTime = diseaseTotalRegion[w].sampleReturnTime + regions[w][i].sampleReturnTime;
                diseaseTotalRegion[w].sampleReturnedCount = diseaseTotalRegion[w].sampleReturnedCount + regions[w][i].sampleReturnedCount;
                diseaseTotalRegion[w].sampleReturnedInCount = diseaseTotalRegion[w].sampleReturnedInCount + regions[w][i].sampleReturnedInCount;
                diseaseTotalRegion[w].sampleReturnedOutCount = diseaseTotalRegion[w].sampleReturnedOutCount + regions[w][i].sampleReturnedOutCount;
                diseaseTotalRegion[w].sampleTestedInCount = diseaseTotalRegion[w].sampleTestedInCount + regions[w][i].sampleTestedInCount;
                diseaseTotalRegion[w].sampleTestedOutCount = diseaseTotalRegion[w].sampleTestedOutCount + regions[w][i].sampleTestedOutCount;
                diseaseTotalRegion[w].sampleNotTestedInCount = diseaseTotalRegion[w].sampleNotTestedInCount + regions[w][i].sampleNotTestedInCount;
                diseaseTotalRegion[w].sampleNotTestedOutCount = diseaseTotalRegion[w].sampleNotTestedOutCount + regions[w][i].sampleNotTestedOutCount;
                diseaseTotalRegion[w].sampleReturnToDeathTime = diseaseTotalRegion[w].sampleReturnToDeathTime + regions[w][i].sampleReturnToDeathTime;
                diseaseTotalRegion[w].sampleReturnToDeathPreDiagLtoFTime = diseaseTotalRegion[w].sampleReturnToDeathPreDiagLtoFTime + regions[w][i].sampleReturnToDeathPreDiagLtoFTime;
                diseaseTotalRegion[w].sampleReturnToDeathInTrtTime = diseaseTotalRegion[w].sampleReturnToDeathInTrtTime + regions[w][i].sampleReturnToDeathInTrtTime;
                diseaseTotalRegion[w].sampleReturnToDeathInRetrtTime = diseaseTotalRegion[w].sampleReturnToDeathInRetrtTime + regions[w][i].sampleReturnToDeathInRetrtTime;
                diseaseTotalRegion[w].sampleReturnToExitTime = diseaseTotalRegion[w].sampleReturnToExitTime + regions[w][i].sampleReturnToExitTime;
                diseaseTotalRegion[w].sampleReturnToDeathCount = diseaseTotalRegion[w].sampleReturnToDeathCount + regions[w][i].sampleReturnToDeathCount;
                diseaseTotalRegion[w].sampleReturnToDeathPreDiagLtoFCount = diseaseTotalRegion[w].sampleReturnToDeathPreDiagLtoFCount + regions[w][i].sampleReturnToDeathPreDiagLtoFCount;
                diseaseTotalRegion[w].sampleReturnToDeathInTrtCount = diseaseTotalRegion[w].sampleReturnToDeathInTrtCount + regions[w][i].sampleReturnToDeathInTrtCount;
                diseaseTotalRegion[w].sampleReturnToDeathInRetrtCount = diseaseTotalRegion[w].sampleReturnToDeathInRetrtCount + regions[w][i].sampleReturnToDeathInRetrtCount;
                diseaseTotalRegion[w].sampleReturnToExitCount = diseaseTotalRegion[w].sampleReturnToExitCount + regions[w][i].sampleReturnToExitCount;
                diseaseTotalRegion[w].sampleLtoFCount = diseaseTotalRegion[w].sampleLtoFCount + regions[w][i].sampleLtoFCount;
                diseaseTotalRegion[w].sampleWaitTestCount = diseaseTotalRegion[w].sampleWaitTestCount + regions[w][i].sampleWaitTestCount;
                diseaseTotalRegion[w].sampleWaitTestCountIn = diseaseTotalRegion[w].sampleWaitTestCountIn + regions[w][i].sampleWaitTestCountIn;
                diseaseTotalRegion[w].sampleWaitTestCountOut = diseaseTotalRegion[w].sampleWaitTestCountOut + regions[w][i].sampleWaitTestCountOut;
                diseaseTotalRegion[w].patientNotNotified = diseaseTotalRegion[w].patientNotNotified + regions[w][i].patientNotNotified;
                diseaseTotalRegion[w].patientNotNotifiedPos = diseaseTotalRegion[w].patientNotNotifiedPos + regions[w][i].patientNotNotifiedPos;
                diseaseTotalRegion[w].patientNotNotifiedIn = diseaseTotalRegion[w].patientNotNotifiedIn + regions[w][i].patientNotNotifiedIn;
                diseaseTotalRegion[w].patientNotNotifiedOut = diseaseTotalRegion[w].patientNotNotifiedOut + regions[w][i].patientNotNotifiedOut;
                diseaseTotalRegion[w].inTestCount = diseaseTotalRegion[w].inTestCount + regions[w][i].inTestCount;
                diseaseTotalRegion[w].outTestCount = diseaseTotalRegion[w].outTestCount + regions[w][i].outTestCount;
                diseaseTotalRegion[w].inCollectedCount = diseaseTotalRegion[w].inCollectedCount + regions[w][i].inCollectedCount;
                diseaseTotalRegion[w].outCollectedCount = diseaseTotalRegion[w].outCollectedCount + regions[w][i].outCollectedCount;
                diseaseTotalRegion[w].test1Count = diseaseTotalRegion[w].test1Count + regions[w][i].test1Count;
                diseaseTotalRegion[w].test1CountIn = diseaseTotalRegion[w].test1CountIn + regions[w][i].test1CountIn;
                diseaseTotalRegion[w].test1CountOut = diseaseTotalRegion[w].test1CountOut + regions[w][i].test1CountOut;
                diseaseTotalRegion[w].test1PosCount = diseaseTotalRegion[w].test1PosCount + regions[w][i].test1PosCount;
                diseaseTotalRegion[w].test1TruePosCount = diseaseTotalRegion[w].test1TruePosCount + regions[w][i].test1TruePosCount;
                diseaseTotalRegion[w].test1NegCount = diseaseTotalRegion[w].test1NegCount + regions[w][i].test1NegCount;
                diseaseTotalRegion[w].test1TrueNegCount = diseaseTotalRegion[w].test1TrueNegCount + regions[w][i].test1TrueNegCount;
                diseaseTotalRegion[w].test2Count = diseaseTotalRegion[w].test2Count + regions[w][i].test2Count;
                diseaseTotalRegion[w].test2CountIn = diseaseTotalRegion[w].test2CountIn + regions[w][i].test2CountIn;
                diseaseTotalRegion[w].test2CountOut = diseaseTotalRegion[w].test2CountOut + regions[w][i].test2CountOut;
                diseaseTotalRegion[w].test2PosCount = diseaseTotalRegion[w].test2PosCount + regions[w][i].test2PosCount;
                diseaseTotalRegion[w].test2TruePosCount = diseaseTotalRegion[w].test2TruePosCount + regions[w][i].test2TruePosCount;
                diseaseTotalRegion[w].test2NegCount = diseaseTotalRegion[w].test2NegCount + regions[w][i].test2NegCount;
                diseaseTotalRegion[w].test2TrueNegCount = diseaseTotalRegion[w].test2TrueNegCount + regions[w][i].test2TrueNegCount;
                diseaseTotalRegion[w].test3Count = diseaseTotalRegion[w].test3Count + regions[w][i].test3Count;
                if (diseaseTotalRegion[w].test1Name == "") {

                    diseaseTotalRegion[w].test1Name = regions[w][i].test1Name;
                }
                if (diseaseTotalRegion[w].test2Name == "") {

                    diseaseTotalRegion[w].test2Name = regions[w][i].test2Name;
                }
                if (diseaseTotalRegion[w].test3Name == "") {

                    diseaseTotalRegion[w].test3Name = regions[w][i].test3Name;
                }
                diseaseTotalRegion[w].countPreTestLtoFOutMinusTestedSamp = diseaseTotalRegion[w].countPreTestLtoFOutMinusTestedSamp + regions[w][i].countPreTestLtoFOutMinusTestedSamp;
                diseaseTotalRegion[w].dummy2Count = diseaseTotalRegion[w].dummy2Count + regions[w][i].dummy2Count;
                diseaseTotalRegion[w].dummy3Count = diseaseTotalRegion[w].dummy3Count + regions[w][i].dummy3Count;
                diseaseTotalRegion[w].dummy4Count = diseaseTotalRegion[w].dummy4Count + regions[w][i].dummy4Count;
                diseaseTotalRegion[w].dummy5Count = diseaseTotalRegion[w].dummy5Count + regions[w][i].dummy5Count;
                diseaseTotalRegion[w].dummy6Count = diseaseTotalRegion[w].dummy6Count + regions[w][i].dummy6Count;

                totalRegion.pop = totalRegion.pop + regions[w][i].pop;
                totalRegion.countP = totalRegion.countP + regions[w][i].countP;
                totalRegion.countPLtoF = totalRegion.countPLtoF + regions[w][i].countPLtoF;
                totalRegion.countPInf = totalRegion.countPInf + regions[w][i].countPInf;
                totalRegion.countPInfTreated = totalRegion.countPInfTreated + regions[w][i].countPInfTreated;
                totalRegion.countPInfTreatedIn = totalRegion.countPInfTreatedIn + regions[w][i].countPInfTreatedIn;
                totalRegion.countPInfTreatedOut = totalRegion.countPInfTreatedOut + regions[w][i].countPInfTreatedOut;
                totalRegion.countPUninfTreated = totalRegion.countPUninfTreated + regions[w][i].countPUninfTreated;
                totalRegion.countPU = totalRegion.countPU + regions[w][i].countPU;
                totalRegion.countPI = totalRegion.countPI + regions[w][i].countPI;
                totalRegion.countPT = totalRegion.countPT + regions[w][i].countPT;
                totalRegion.countPR = totalRegion.countPR + regions[w][i].countPR;
                totalRegion.countPUT = totalRegion.countPUT + regions[w][i].countPUT;
                totalRegion.countPUTFN = totalRegion.countPUTFN + regions[w][i].countPUTFN;
                totalRegion.countPUTNS = totalRegion.countPUTNS + regions[w][i].countPUTNS;
                totalRegion.countPUTLtoF = totalRegion.countPUTLtoF + regions[w][i].countPUTLtoF;
                totalRegion.countPC = totalRegion.countPC + regions[w][i].countPC;
                totalRegion.countPDI = totalRegion.countPDI + regions[w][i].countPDI;
                totalRegion.countPDIT = totalRegion.countPDIT + regions[w][i].countPDIT;
                totalRegion.countPDIUT = totalRegion.countPDIUT + regions[w][i].countPDIUT;
                totalRegion.countPDIUTSR = totalRegion.countPDIUTSR + regions[w][i].countPDIUTSR;
                totalRegion.countPDIUTSNR = totalRegion.countPDIUTSNR + regions[w][i].countPDIUTSNR;
                totalRegion.countPDIUTLtF = totalRegion.countPDIUTLtF + regions[w][i].countPDIUTLtF;
                totalRegion.countPDU = totalRegion.countPDU + regions[w][i].countPDU;
                totalRegion.countPDOA = totalRegion.countPDOA + regions[w][i].countPDOA;
                totalRegion.countPDIUTPTLtoF = totalRegion.countPDIUTPTLtoF + regions[w][i].countPDIUTPTLtoF;
                totalRegion.countPDIUTSRLtoF = totalRegion.countPDIUTSRLtoF + regions[w][i].countPDIUTSRLtoF;
                totalRegion.countPDITLtoF = totalRegion.countPDITLtoF + regions[w][i].countPDITLtoF;
                totalRegion.countPTC = totalRegion.countPTC + regions[w][i].countPTC;
                totalRegion.countPTD = totalRegion.countPTD + regions[w][i].countPTD;
                totalRegion.countPTR = totalRegion.countPTR + regions[w][i].countPTR;
                totalRegion.countRetreat = totalRegion.countRetreat + regions[w][i].countRetreat;
                totalRegion.countRetreatCure = totalRegion.countRetreatCure + regions[w][i].countRetreatCure;
                totalRegion.countRetreatDead = totalRegion.countRetreatDead + regions[w][i].countRetreatDead;
                totalRegion.countPO = totalRegion.countPO + regions[w][i].countPO;
                totalRegion.countPUI = totalRegion.countPUI + regions[w][i].countPUI;
                totalRegion.countPUINR = totalRegion.countPUINR + regions[w][i].countPUINR;
                totalRegion.countPUIUT = totalRegion.countPUIUT + regions[w][i].countPUIUT;
                totalRegion.countMDInf = totalRegion.countMDInf + regions[w][i].countMDInf;
                totalRegion.countMDUninf = totalRegion.countMDUninf + regions[w][i].countMDUninf;
                totalRegion.countTestPos = totalRegion.countTestPos + regions[w][i].countTestPos;
                totalRegion.countTestNeg = totalRegion.countTestNeg + regions[w][i].countTestNeg;
                totalRegion.countTruePos = totalRegion.countTruePos + regions[w][i].countTruePos;
                totalRegion.countFalsePos = totalRegion.countFalsePos + regions[w][i].countFalsePos;
                totalRegion.countTrueNeg = totalRegion.countTrueNeg + regions[w][i].countTrueNeg;
                totalRegion.countFalseNeg = totalRegion.countFalseNeg + regions[w][i].countFalseNeg;
                totalRegion.countDiagnosis = totalRegion.countDiagnosis + regions[w][i].countDiagnosis;
                totalRegion.countDiagnosisIn = totalRegion.countDiagnosisIn + regions[w][i].countDiagnosisIn;
                totalRegion.countDiagnosisOut = totalRegion.countDiagnosisOut + regions[w][i].countDiagnosisOut;
                totalRegion.countPreTestLtoFOut = totalRegion.countPreTestLtoFOut + regions[w][i].countPreTestLtoFOut;
                totalRegion.countPreTestLtoFOutTested = totalRegion.countPreTestLtoFOutTested + regions[w][i].countPreTestLtoFOutTested;
                totalRegion.countPreTestLtoFOutUnTested = totalRegion.countPreTestLtoFOutUnTested + regions[w][i].countPreTestLtoFOutUnTested;
                totalRegion.countPreTestLtoFOutUnTested2 = totalRegion.countPreTestLtoFOutUnTested2 + regions[w][i].countPreTestLtoFOutUnTested2;
                totalRegion.countPreTestLtoFDead = totalRegion.countPreTestLtoFDead + regions[w][i].countPreTestLtoFDead;
                totalRegion.countPreTestLtoFORepresent = totalRegion.countPreTestLtoFORepresent + regions[w][i].countPreTestLtoFORepresent;
                totalRegion.onsetToTreat = totalRegion.onsetToTreat + regions[w][i].onsetToTreat;
                totalRegion.onsetToTreatIn = totalRegion.onsetToTreatIn + regions[w][i].onsetToTreatIn;
                totalRegion.onsetToTreatOut = totalRegion.onsetToTreatOut + regions[w][i].onsetToTreatOut;
                totalRegion.timeInfectious = totalRegion.timeInfectious + regions[w][i].timeInfectious;
                totalRegion.sampleTransTime = totalRegion.sampleTransTime + regions[w][i].sampleTransTime;
                totalRegion.sampleTransCount = totalRegion.sampleTransCount + regions[w][i].sampleTransCount;
                totalRegion.sampleWaitForResourceInTime = totalRegion.sampleWaitForResourceInTime + regions[w][i].sampleWaitForResourceInTime;
                totalRegion.sampleWaitForResourceInCount = totalRegion.sampleWaitForResourceInCount + regions[w][i].sampleWaitForResourceInCount;
                totalRegion.sampleWaitForResourceOutTime = totalRegion.sampleWaitForResourceOutTime + regions[w][i].sampleWaitForResourceOutTime;
                totalRegion.sampleWaitForResourceOutCount = totalRegion.sampleWaitForResourceOutCount + regions[w][i].sampleWaitForResourceOutCount;
                totalRegion.sampleCollectToTestInTime = totalRegion.sampleCollectToTestInTime + regions[w][i].sampleCollectToTestInTime;
                totalRegion.sampleCollectToTestOutTime = totalRegion.sampleCollectToTestOutTime + regions[w][i].sampleCollectToTestOutTime;
                totalRegion.sampleCollectToTestInCount = totalRegion.sampleCollectToTestInCount + regions[w][i].sampleCollectToTestInCount;
                totalRegion.sampleCollectToTestOutCount = totalRegion.sampleCollectToTestOutCount + regions[w][i].sampleCollectToTestOutCount;
                totalRegion.sampleTestTime = totalRegion.sampleTestTime + regions[w][i].sampleTestTime;
                totalRegion.sampleTestPosTimeIn = totalRegion.sampleTestPosTimeIn + regions[w][i].sampleTestPosTimeIn;
                totalRegion.sampleTestPosTimeOut = totalRegion.sampleTestPosTimeOut + regions[w][i].sampleTestPosTimeOut;
                totalRegion.sampleTestPosCountIn = totalRegion.sampleTestPosCountIn + regions[w][i].sampleTestPosCountIn;
                totalRegion.sampleTestPosCountOut = totalRegion.sampleTestPosCountOut + regions[w][i].sampleTestPosCountOut;
                totalRegion.sampleReturnTime = totalRegion.sampleReturnTime + regions[w][i].sampleReturnTime;
                totalRegion.sampleReturnedCount = totalRegion.sampleReturnedCount + regions[w][i].sampleReturnedCount;
                totalRegion.sampleReturnedInCount = totalRegion.sampleReturnedInCount + regions[w][i].sampleReturnedInCount;
                totalRegion.sampleReturnedOutCount = totalRegion.sampleReturnedOutCount + regions[w][i].sampleReturnedOutCount;
                totalRegion.sampleTestedInCount = totalRegion.sampleTestedInCount + regions[w][i].sampleTestedInCount;
                totalRegion.sampleTestedOutCount = totalRegion.sampleTestedOutCount + regions[w][i].sampleTestedOutCount;
                totalRegion.sampleNotTestedInCount = totalRegion.sampleNotTestedInCount + regions[w][i].sampleNotTestedInCount;
                totalRegion.sampleNotTestedOutCount = totalRegion.sampleNotTestedOutCount + regions[w][i].sampleNotTestedOutCount;
                totalRegion.sampleReturnToDeathTime = totalRegion.sampleReturnToDeathTime + regions[w][i].sampleReturnToDeathTime;
                totalRegion.sampleReturnToDeathPreDiagLtoFTime = totalRegion.sampleReturnToDeathPreDiagLtoFTime + regions[w][i].sampleReturnToDeathPreDiagLtoFTime;
                totalRegion.sampleReturnToDeathInTrtTime = totalRegion.sampleReturnToDeathInTrtTime + regions[w][i].sampleReturnToDeathInTrtTime;
                totalRegion.sampleReturnToDeathInRetrtTime = totalRegion.sampleReturnToDeathInRetrtTime + regions[w][i].sampleReturnToDeathInRetrtTime;
                totalRegion.sampleReturnToExitTime = totalRegion.sampleReturnToExitTime + regions[w][i].sampleReturnToExitTime;
                totalRegion.sampleReturnToDeathCount = totalRegion.sampleReturnToDeathCount + regions[w][i].sampleReturnToDeathCount;
                totalRegion.sampleReturnToDeathPreDiagLtoFCount = totalRegion.sampleReturnToDeathPreDiagLtoFCount + regions[w][i].sampleReturnToDeathPreDiagLtoFCount;
                totalRegion.sampleReturnToDeathInTrtCount = totalRegion.sampleReturnToDeathInTrtCount + regions[w][i].sampleReturnToDeathInTrtCount;
                totalRegion.sampleReturnToDeathInRetrtCount = totalRegion.sampleReturnToDeathInRetrtCount + regions[w][i].sampleReturnToDeathInRetrtCount;
                totalRegion.sampleReturnToExitCount = totalRegion.sampleReturnToExitCount + regions[w][i].sampleReturnToExitCount;
                totalRegion.sampleLtoFCount = totalRegion.sampleLtoFCount + regions[w][i].sampleLtoFCount;
                totalRegion.sampleWaitTestCount = totalRegion.sampleWaitTestCount + regions[w][i].sampleWaitTestCount;
                totalRegion.sampleWaitTestCountIn = totalRegion.sampleWaitTestCountIn + regions[w][i].sampleWaitTestCountIn;
                totalRegion.sampleWaitTestCountOut = totalRegion.sampleWaitTestCountOut + regions[w][i].sampleWaitTestCountOut;
                totalRegion.patientNotNotified = totalRegion.patientNotNotified + regions[w][i].patientNotNotified;
                totalRegion.patientNotNotifiedPos = totalRegion.patientNotNotifiedPos + regions[w][i].patientNotNotifiedPos;
                totalRegion.patientNotNotifiedIn = totalRegion.patientNotNotifiedIn + regions[w][i].patientNotNotifiedIn;
                totalRegion.patientNotNotifiedOut = totalRegion.patientNotNotifiedOut + regions[w][i].patientNotNotifiedOut;
                totalRegion.inTestCount = totalRegion.inTestCount + regions[w][i].inTestCount;
                totalRegion.outTestCount = totalRegion.outTestCount + regions[w][i].outTestCount;
                totalRegion.inCollectedCount = totalRegion.inCollectedCount + regions[w][i].inCollectedCount;
                totalRegion.outCollectedCount = totalRegion.outCollectedCount + regions[w][i].outCollectedCount;
                totalRegion.test1Count = totalRegion.test1Count + regions[w][i].test1Count;
                totalRegion.test1CountIn = totalRegion.test1CountIn + regions[w][i].test1CountIn;
                totalRegion.test1CountOut = totalRegion.test1CountOut + regions[w][i].test1CountOut;
                totalRegion.test1PosCount = totalRegion.test1PosCount + regions[w][i].test1PosCount;
                totalRegion.test1TruePosCount = totalRegion.test1TruePosCount + regions[w][i].test1TruePosCount;
                totalRegion.test1NegCount = totalRegion.test1NegCount + regions[w][i].test1NegCount;
                totalRegion.test1TrueNegCount = totalRegion.test1TrueNegCount + regions[w][i].test1TrueNegCount;
                totalRegion.test2Count = totalRegion.test2Count + regions[w][i].test2Count;
                totalRegion.test2CountIn = totalRegion.test2CountIn + regions[w][i].test2CountIn;
                totalRegion.test2CountOut = totalRegion.test2CountOut + regions[w][i].test2CountOut;
                totalRegion.test2PosCount = totalRegion.test2PosCount + regions[w][i].test2PosCount;
                totalRegion.test2TruePosCount = totalRegion.test2TruePosCount + regions[w][i].test2TruePosCount;
                totalRegion.test2NegCount = totalRegion.test2NegCount + regions[w][i].test2NegCount;
                totalRegion.test2TrueNegCount = totalRegion.test2TrueNegCount + regions[w][i].test2TrueNegCount;
                totalRegion.test3Count = totalRegion.test3Count + regions[w][i].test3Count;
                if (totalRegion.test1Name == "") {

                    totalRegion.test1Name = regions[w][i].test1Name;
                }
                if (totalRegion.test2Name == "") {

                    totalRegion.test2Name = regions[w][i].test2Name;
                }
                if (totalRegion.test3Name == "") {

                    totalRegion.test3Name = regions[w][i].test3Name;
                }
                totalRegion.countPreTestLtoFOutMinusTestedSamp = totalRegion.countPreTestLtoFOutMinusTestedSamp + regions[w][i].countPreTestLtoFOutMinusTestedSamp;
                totalRegion.dummy2Count = totalRegion.dummy2Count + regions[w][i].dummy2Count;
                totalRegion.dummy3Count = totalRegion.dummy3Count + regions[w][i].dummy3Count;
                totalRegion.dummy4Count = totalRegion.dummy4Count + regions[w][i].dummy4Count;
                totalRegion.dummy5Count = totalRegion.dummy5Count + regions[w][i].dummy5Count;
                totalRegion.dummy6Count = totalRegion.dummy6Count + regions[w][i].dummy6Count;

                if (individualFlag) {

                    myfile9 << "-----------------------------------------------------------------------------------" << "\n";
                    myfile9 << "stats.Region" << ",population" << ",patients" << ".for.disease" <<  "\n";
                    myfile9 << regions[w][i].name << "," << newPop << "," << regions[w][i].countP << "," << diseaseList[w] << "\n";
                    myfile9 << "," << "status" << ",disease" << ",age" << ",alive" << ",infected" << ",test.pos" << ",md.override" << ",time.from.onset" << ",time.in.treatment" << ",time.not.in.treatment" << "\n";

                    myfile2 << "-----------------------------------------------------------------------------------  " << "\n";
                    myfile2 << "stats.Region" << ",population" << ",patients" << ".for.disease" <<  "\n";
                    myfile2 << regions[w][i].name << "," << newPop << "," << regions[w][i].countP << "," << diseaseList[w] << "\n";
                    myfile2 << "," << "status" << ",disease" << ",age" << ",alive" << ",infected" << ",test.pos" << ",md.override" << ",time.from.onset" << ",time.in.treatment" << ",time.not.in.treatment" << "\n";

                    // admin file
                    myfile11 << "region" << "," << regions[w][i].name << "," << newPop << "," << regions[w][i].countP << "," <<  "\n";

                    // keep track of patients that have returned samples
                    patReturnedCount = 0;
                    
                    // dump all patients in this region - use patientContainer vector to facility patientContainer pointers
                    for (auto patCon : regions[w][i].patientContainer) {

                        // for each facility patientContainer pointer - get patients
                        for (auto facPat : patCon) {

                            myfile9 << "," << facPat->getStatus()  << "," << facPat->getDisease() << "," << facPat->getAge() << "," << facPat->isAlive() << "," << facPat->getInfected() << "," << facPat->getTestPos();
                            myfile9 << "," << facPat->getMDPos() << "," << facPat->getTimeFromOnset() << "," << facPat->getTimeStepsInTreatment() << "," << facPat->getTimeStepsNotInTreatment() << "\n";

                            myfile2 << "," << facPat->getStatus()  << "," << facPat->getDisease() << "," << facPat->getAge() << "," << facPat->isAlive() << "," << facPat->getInfected() << "," << facPat->getTestPos();
                            myfile2 << "," << facPat->getMDPos() << "," << facPat->getTimeFromOnset() << "," << facPat->getTimeStepsInTreatment() << "," << facPat->getTimeStepsNotInTreatment() << "\n";

                            // keep track to patient totals by region 
                            regNoPatients[w]++;
                            regAge[w] = regAge[w] + facPat->getAge();
                            if (facPat->isAlive()) regNoAlive[w]++;
                            if (facPat->getInfected()) {
                                regNoInfected[w]++;
                                regTotalTimeFromOnset[w] = regTotalTimeFromOnset[w] + facPat->getTimeFromOnset();
                                regTotalTimeInfectious[w] = regTotalTimeInfectious[w] + facPat->getInfectEndTime();
                                regTotaltimeInTreatment[w] = regTotaltimeInTreatment[w] + facPat->getTimeStepsInTreatment();
                            }
                            else {

                                regTotalTimeNotInTreatment[w] = regTotalTimeNotInTreatment[w] + facPat->getTimeStepsNotInTreatment();
                            }
                            if (facPat->getTestPos()) regNoTestPos[w]++;
                            if (facPat->getMDPos()) regNoMDOverride[w]++;
                            regTotalTimeFacilityReturnTime[w] = regTotalTimeFacilityReturnTime[w] + facPat->getFacilityReturnTime();
                            if (facPat->getResultsReturnedTime() > 0) {

                                regTotalTimeResultsReturned[w] = regTotalTimeResultsReturned[w] + (facPat->getResultsReturnedTime() - facPat->getCreateTime());
                                regTotalNoOfReturnVisits[w] = regTotalNoOfReturnVisits[w] + facPat->getNoReturnsForResults();
                                patReturnedCount++;
                            }
                        }
                    }
                }

                if (individualFlag) {
            
                    // no patients - no totals
                    if (regNoPatients[w] > 0) {

                        myfile9 << "-----------------------------------------------------------------------------------  " << "\n";
                        myfile9 << "  " << ",disease" << ",age" << ",alive" << ",infected" << ",test.Pos" << ",md.override" << ",time.from.onset" << ",time.in.treatment" << ",time.not.in.treatment" << ",std.pat.return.time";
                        myfile9 << ",avg.total.pat.return.time" << ",avg.no.pat.returns" <<  "\n";
                    
                        myfile9 << "Total" << "," << regDisease[w] << "," << regAge[w]/regNoPatients[w] << "," << regNoAlive[w] << "," << regNoInfected[w] << "," << regNoTestPos[w] << "," << regNoMDOverride[w];
                
                        myfile2 << "-----------------------------------------------------------------------------------  " << "\n";
                        myfile2 << "  " << ",disease" << ",age" << ",alive" << ",infected" << ",test.Pos" << ",md.override" << ",time.from.onset" << ",time.in.treatment" << ",.time.not.in.treatment" << ",std.pat.return.time";
                        myfile2 << ",avg.total.pat.return.time" << ",avg.no.pat.returns" <<  "\n";
                    
                        myfile2 << "Total" << "," << regDisease[w] << "," << regAge[w]/regNoPatients[w] << "," << regNoAlive[w] << "," << regNoInfected[w] << "," << regNoTestPos[w] << "," << regNoMDOverride[w];
                
                        // change output if no infected patients
                        if (regNoInfected[w] > 0) {
               
                            if (regNoInfected[w] == regNoPatients[w]) {

                                myfile9 << "," << regTotalTimeFromOnset[w]/regNoInfected[w] << "," << regTotaltimeInTreatment[w]/regNoInfected[w] << "," << "0";
                                
                                myfile2 << "," << regTotalTimeFromOnset[w]/regNoInfected[w] << "," << regTotaltimeInTreatment[w]/regNoInfected[w] << "," << "0";
                            }
                            else {
                    
                                myfile9 << "," << regTotalTimeFromOnset[w]/regNoInfected[w] << "," << regTotaltimeInTreatment[w]/regNoInfected[w] << "," << regTotalTimeNotInTreatment[w]/(regNoPatients[w] -regNoInfected[w]);
                                
                                myfile2 << "," << regTotalTimeFromOnset[w]/regNoInfected[w] << "," << regTotaltimeInTreatment[w]/regNoInfected[w] << "," << regTotalTimeNotInTreatment[w]/(regNoPatients[w] -regNoInfected[w]);
                            }
                        }
                        else {

                            myfile9 << "," << "0" << "," << "0" << "," << regTotalTimeNotInTreatment[w]/(regNoPatients[w] -regNoInfected[w]);
                            
                            myfile2 << "," << "0" << "," << "0" << "," << regTotalTimeNotInTreatment[w]/(regNoPatients[w] -regNoInfected[w]);
                        }

                        //std::cout << "stats: regions patients count = " << regNoPatients[w] << ", patReturnedCount = " <<  patReturnedCount << "\n";

                        if (patReturnedCount > 0) {

                            myfile9 << "," << (float) regTotalTimeFacilityReturnTime[w]/regNoPatients[w] << "," << (float) regTotalTimeResultsReturned[w]/patReturnedCount << "," << (float) regTotalNoOfReturnVisits[w]/patReturnedCount << "\n";

                            myfile2 << "," << (float) regTotalTimeFacilityReturnTime[w]/regNoPatients[w] << "," << (float) regTotalTimeResultsReturned[w]/patReturnedCount << "," << (float) regTotalNoOfReturnVisits[w]/patReturnedCount << "\n";
                        }
                        else {

                            myfile9 << "," << (float) regTotalTimeFacilityReturnTime[w]/regNoPatients[w] << "," << "0" << "," << "0" << "\n";

                            myfile2 << "," << (float) regTotalTimeFacilityReturnTime[w]/regNoPatients[w] << "," << "0" << "," << "0" << "\n";
                        }
                    }
                }
            }

            myfile << "-----------------------------------------------------------------------------------" << "\n";
            myfile << statHeader1 << statHeader2 << statHeader3 << statHeader4 << statHeader5 << statHeader5a << statHeader6 << statHeader7 << statHeader8;
            myfile << statHeader9 << statHeader10 << statHeader10a << statHeader11 << statHeader12 << statHeader12a << statHeader12b;
            myfile << statHeader13 << statHeader13a << statHeader13b << statHeader13c << statHeader13d << statHeader14 << statHeader15 << statHeader16 << statHeader17;



            //std::cout << "stats: district =  " << diseaseTotalRegion[w].name <<  " true pos =  " << diseaseTotalRegion[w].countTruePos <<  " false neg =  " << diseaseTotalRegion[w].countFalseNeg;
            //std::cout << " true neg =  " << diseaseTotalRegion[w].countTrueNeg <<  " false pos =  " << diseaseTotalRegion[w].countFalsePos <<  " pat count =  " << diseaseTotalRegion[w].countP;
            //std::cout <<  " pat inf =  " << diseaseTotalRegion[w].countPInf <<  " pat ltof =  " << diseaseTotalRegion[w].countPreTestLtoFOut << "\n";

            //treated = diseaseTotalRegion[w].countPT + diseaseTotalRegion[w].countPR + diseaseTotalRegion[w].countPC + diseaseTotalRegion[w].countPDI;
            //infected = treated + diseaseTotalRegion[w].countPUT + diseaseTotalRegion[w].countPDIUT;
            //treated = diseaseTotalRegion[w].countPT + diseaseTotalRegion[w].countPC;
            //infected = treated + diseaseTotalRegion[w].countPUT + diseaseTotalRegion[w].countPDIUT + diseaseTotalRegion[w].countPR + diseaseTotalRegion[w].countPDI;

            // update population by population growth if it goes on more that 1 year
            newPop = diseaseTotalRegion[w].pop;
            startIndex = referenceYear - 2015;
            endIndex = startIndex + noOfYears;
            if (noOfYears > 0) {

                for (int i = startIndex; i < endIndex; i++) {

                    // multiply by growth rate for x number of years
                    newPop = (int) (newPop * popGrowthRateArray[i][1]);
                }
            }

            // count up all the sample results
            totalSampleCountPos = diseaseTotalRegion[w].countTruePos + diseaseTotalRegion[w].countFalsePos;
            totalSampleCountNeg = diseaseTotalRegion[w].countTrueNeg + diseaseTotalRegion[w].countFalseNeg;

            //myfile << "stats: total region population = " << diseaseTotalRegion[w].pop << ", patients = " << diseaseTotalRegion[w].countP << " for disease = " << diseaseList[w] << "\n";
            //myfile << "stats: patient" << " Untested-out = " << diseaseTotalRegion[w].countPU << " Untested-in =  " << diseaseTotalRegion[w].countPI << ", under treat = " << diseaseTotalRegion[w].countPT << ", failed treat = " << diseaseTotalRegion[w].countPR << ", untreated = " << diseaseTotalRegion[w].countPUT << ", cured = " << diseaseTotalRegion[w].countPC << ", uninfec-treat = " << diseaseTotalRegion[w].countPUIUT << ", Other = " << diseaseTotalRegion[w].countPO << "\n";
            // myfile << "               Non-infected - not treated  = " << diseaseTotalRegion[w].countPUI << ", Non-infected - not returned = " << diseaseTotalRegion[w].countPUINR << ", dead - infected = " << diseaseTotalRegion[w].countPDI << ", dead - uninfected = " << diseaseTotalRegion[w].countPDU << "\n";
            myfile << "," << "," << diseaseTotalRegion[w].tier << "," << diseaseTotalRegion[w].district << "," << diseaseTotalRegion[w].region << "," << newPop << "," << diseaseList[w] << "," << int((float(diseaseTotalRegion[w].countPInf) * 100000) / float(newPop));
            myfile << "," << diseaseTotalRegion[w].countP << "," << diseaseTotalRegion[w].countPInf << "," << diseaseTotalRegion[w].countPLtoF;
            myfile << "," << diseaseTotalRegion[w].outCollectedCount + diseaseTotalRegion[w].inCollectedCount << "," << diseaseTotalRegion[w].outCollectedCount << "," << diseaseTotalRegion[w].inCollectedCount;
            if ((diseaseTotalRegion[w].sampleWaitForResourceInCount + diseaseTotalRegion[w].sampleWaitForResourceOutCount) < 1) {
                myfile << "," << "0";
            }
            else {
                myfile << "," << float(diseaseTotalRegion[w].sampleWaitForResourceInTime + diseaseTotalRegion[w].sampleWaitForResourceOutTime)/float(diseaseTotalRegion[w].sampleWaitForResourceInCount + diseaseTotalRegion[w].sampleWaitForResourceOutCount);
            }
            if (diseaseTotalRegion[w].sampleWaitForResourceInCount < 1) {
                myfile << "," << "0";
            }
            else {
                myfile << "," << float(diseaseTotalRegion[w].sampleWaitForResourceInTime)/float(diseaseTotalRegion[w].sampleWaitForResourceInCount);
            }
            if (diseaseTotalRegion[w].sampleWaitForResourceOutCount < 1) {
                myfile << "," << "0" << ",";
            }
            else {
                myfile << "," << float(diseaseTotalRegion[w].sampleWaitForResourceOutTime)/float(diseaseTotalRegion[w].sampleWaitForResourceOutCount) << ",";
            }
            if (diseaseTotalRegion[w].sampleTransCount  < 1) {
                myfile << "0" << "," << diseaseTotalRegion[w].sampleTransCount;
            }
            else {
                myfile << float(diseaseTotalRegion[w].sampleTransTime)/float(diseaseTotalRegion[w].sampleTransCount) << "," << diseaseTotalRegion[w].sampleTransCount;
            }
            myfile << "," << diseaseTotalRegion[w].sampleTestedInCount +  diseaseTotalRegion[w].sampleTestedOutCount << "," << diseaseTotalRegion[w].sampleTestedInCount << "," << diseaseTotalRegion[w].sampleTestedOutCount;
            //myfile << "," << diseaseTotalRegion[w].sampleLtoFCount;
            if (diseaseTotalRegion[w].sampleCollectToTestInCount < 1) {
                myfile << "," << "0";
            }
            else {
                myfile << "," << float(diseaseTotalRegion[w].sampleCollectToTestInTime)/float(diseaseTotalRegion[w].sampleCollectToTestInCount);
            }
            myfile << "," << float(diseaseTotalRegion[w].sampleCollectToTestInTime) << "," << diseaseTotalRegion[w].sampleCollectToTestInCount;
            if (diseaseTotalRegion[w].sampleCollectToTestOutCount < 1) {
                myfile << "," << "0";
            }
            else {
                myfile << "," << float(diseaseTotalRegion[w].sampleCollectToTestOutTime)/float(diseaseTotalRegion[w].sampleCollectToTestOutCount);
            }
            myfile << "," << float(diseaseTotalRegion[w].sampleCollectToTestOutTime) << "," << diseaseTotalRegion[w].sampleCollectToTestOutCount;
            myfile << "," << diseaseTotalRegion[w].countTestPos << ",";
            if (totalSampleCountPos > 0) {
                myfile << float(diseaseTotalRegion[w].countTruePos)/float(totalSampleCountPos) << ",";
                myfile << float(diseaseTotalRegion[w].countFalsePos)/float(totalSampleCountPos) << ",";
            }
            else {
                myfile << "0" << "," << "0" << ",";
            }
            myfile << diseaseTotalRegion[w].countTestNeg << ",";
            if (totalSampleCountNeg > 0) {
                myfile << float(diseaseTotalRegion[w].countTrueNeg)/float(totalSampleCountNeg) << ",";
                myfile << float(diseaseTotalRegion[w].countFalseNeg)/float(totalSampleCountNeg);
            }
            else {
                myfile << "0" << "," << "0";
            }              
            if (debug2) myfile << "," << "marker";

            myfile << "," << diseaseTotalRegion[w].countMDInf << "," << diseaseTotalRegion[w].countMDUninf;
            myfile << "," << diseaseTotalRegion[w].countMDInf + diseaseTotalRegion[w].countMDUninf + diseaseTotalRegion[w].countTestPos;
            myfile << "," << diseaseTotalRegion[w].countFalsePos << "," << diseaseTotalRegion[w].countFalseNeg;
            myfile << "," << diseaseTotalRegion[w].countDiagnosis << "," << diseaseTotalRegion[w].countDiagnosisIn << "," << diseaseTotalRegion[w].countDiagnosisOut << "," << diseaseTotalRegion[w].countPreTestLtoFOut << "," << diseaseTotalRegion[w].countPreTestLtoFOutMinusTestedSamp << "," << diseaseTotalRegion[w].countPreTestLtoFDead << "," << diseaseTotalRegion[w].countPreTestLtoFORepresent;
            if (diseaseTotalRegion[w].sampleTestPosCountIn < 1) {
                myfile << "," << "0";
            }
            else {
                myfile << "," << float(diseaseTotalRegion[w].sampleTestPosTimeIn)/float(diseaseTotalRegion[w].sampleTestPosCountIn);
            }
            myfile << "," << float(diseaseTotalRegion[w].sampleTestPosTimeIn) << "," << diseaseTotalRegion[w].sampleTestPosCountIn;
            if (diseaseTotalRegion[w].sampleTestPosCountOut < 1) {
                myfile << "," << "0";
            }
            else {
                myfile << "," << float(diseaseTotalRegion[w].sampleTestPosTimeOut)/float(diseaseTotalRegion[w].sampleTestPosCountOut);
            }
            myfile << "," << float(diseaseTotalRegion[w].sampleTestPosTimeOut) << "," << diseaseTotalRegion[w].sampleTestPosCountOut;
            if(debug2) myfile << "," << "marker";
               
            myfile << "," << diseaseTotalRegion[w].countPInfTreated+diseaseTotalRegion[w].countPUninfTreated << "," << diseaseTotalRegion[w].countPInfTreated << "," << diseaseTotalRegion[w].countPUninfTreated << "," << diseaseTotalRegion[w].countPTC << "," << diseaseTotalRegion[w].countPTD << "," << diseaseTotalRegion[w].countPTR  << "," << diseaseTotalRegion[w].countRetreat << "," << diseaseTotalRegion[w].countRetreatCure << "," << diseaseTotalRegion[w].countRetreatDead;
            if ((diseaseTotalRegion[w].countPInfTreatedIn + diseaseTotalRegion[w].countPInfTreatedOut) < 1) {
                myfile << "," << "0";
            }
            else {
                myfile << "," << float (diseaseTotalRegion[w].onsetToTreat)/float(diseaseTotalRegion[w].countPInfTreatedIn + diseaseTotalRegion[w].countPInfTreatedOut);
            }
            myfile << "," << float (diseaseTotalRegion[w].onsetToTreat) << "," << (diseaseTotalRegion[w].countPInfTreatedIn + diseaseTotalRegion[w].countPInfTreatedOut);
            if (diseaseTotalRegion[w].countPInfTreatedIn < 1) {
                myfile << "," << "0";
            }
            else {
                myfile << "," << float (diseaseTotalRegion[w].onsetToTreatIn)/float(diseaseTotalRegion[w].countPInfTreatedIn);
            }
            myfile << "," << float (diseaseTotalRegion[w].onsetToTreatIn) << "," << diseaseTotalRegion[w].countPInfTreatedIn;
            if (diseaseTotalRegion[w].countPInfTreatedOut < 1) {
                myfile << "," << "0";
            }
            else {
                myfile << "," << float (diseaseTotalRegion[w].onsetToTreatOut)/float(diseaseTotalRegion[w].countPInfTreatedOut);
            }
            myfile << "," << float (diseaseTotalRegion[w].onsetToTreatOut) << "," << diseaseTotalRegion[w].countPInfTreatedOut;
            if(debug2) myfile << "," << "marker";
               
            if (diseaseTotalRegion[w].sampleReturnToDeathCount < 1) {
            myfile << "," << "0";
            }
            else {
                myfile << "," << float (diseaseTotalRegion[w].sampleReturnToDeathTime)/float(diseaseTotalRegion[w].sampleReturnToDeathCount);
            }
            myfile << "," << float (diseaseTotalRegion[w].sampleReturnToDeathTime) << "," << diseaseTotalRegion[w].sampleReturnToDeathCount;
            if (diseaseTotalRegion[w].sampleReturnToDeathPreDiagLtoFCount < 1) {
                myfile << "," << "0";
            }
            else {
                myfile << "," << float (diseaseTotalRegion[w].sampleReturnToDeathPreDiagLtoFTime)/float(diseaseTotalRegion[w].sampleReturnToDeathPreDiagLtoFCount);
            }
            myfile << "," << float (diseaseTotalRegion[w].sampleReturnToDeathPreDiagLtoFTime) << "," << diseaseTotalRegion[w].sampleReturnToDeathPreDiagLtoFCount;
            if (diseaseTotalRegion[w].sampleReturnToDeathInTrtCount < 1) {
                myfile << "," << "0";
            }
            else {
                myfile << "," << float (diseaseTotalRegion[w].sampleReturnToDeathInTrtTime)/float(diseaseTotalRegion[w].sampleReturnToDeathInTrtCount);
            }
            myfile << "," << float (diseaseTotalRegion[w].sampleReturnToDeathInTrtTime) << "," << diseaseTotalRegion[w].sampleReturnToDeathInTrtCount;
            if (diseaseTotalRegion[w].sampleReturnToDeathInRetrtCount < 1) {
                myfile << "," << "0";
            }
            else {
                myfile << "," << float (diseaseTotalRegion[w].sampleReturnToDeathInRetrtTime)/float(diseaseTotalRegion[w].sampleReturnToDeathInRetrtCount);
            }
            myfile << "," << float (diseaseTotalRegion[w].sampleReturnToDeathInRetrtTime) << "," << diseaseTotalRegion[w].sampleReturnToDeathInRetrtCount;
            if (diseaseTotalRegion[w].sampleReturnToExitCount < 1) {
                myfile << "," << "0";
            }
            else {
                myfile << "," << float (diseaseTotalRegion[w].sampleReturnToExitTime)/float(diseaseTotalRegion[w].sampleReturnToExitCount);
            }
            myfile << "," << float (diseaseTotalRegion[w].sampleReturnToExitTime) << "," << diseaseTotalRegion[w].sampleReturnToExitCount;
            myfile << "," << diseaseTotalRegion[w].test1CountIn + diseaseTotalRegion[w].test1CountOut << "," << diseaseTotalRegion[w].test1CountIn << "," << diseaseTotalRegion[w].test1CountOut << "," << diseaseTotalRegion[w].test2Count << "," << diseaseTotalRegion[w]. test2PosCount;
            if (diseaseTotalRegion[w].test2PosCount < 1) {
                myfile << "," << "0";
            }
            else {
                myfile << "," << float (diseaseTotalRegion[w].test2TruePosCount)/float(diseaseTotalRegion[w].test2PosCount);
            }
            myfile << "," << diseaseTotalRegion[w].test2NegCount;
            if (diseaseTotalRegion[w].test2NegCount < 1) {
                myfile << "," << "0";
            }
            else {
                myfile << "," << float (diseaseTotalRegion[w].test2TrueNegCount)/float(diseaseTotalRegion[w].test2NegCount);
            }
            myfile << "," << diseaseTotalRegion[w].test1PosCount;
            if (diseaseTotalRegion[w].test1PosCount < 1) {
                myfile << "," << "0";
            }
            else {
                myfile << "," << float (diseaseTotalRegion[w].test1TruePosCount)/float(diseaseTotalRegion[w].test1PosCount);
            }
            myfile << "," << diseaseTotalRegion[w].test1NegCount;
            if (diseaseTotalRegion[w].test1NegCount < 1) {
                myfile << "," << "0";
            }
            else {
                myfile << "," << float (diseaseTotalRegion[w].test1TrueNegCount)/float(diseaseTotalRegion[w].test1NegCount);
            }
            if(debug2) myfile << "," << "marker";
               
            myfile << "," << diseaseTotalRegion[w].countP << "," << diseaseTotalRegion[w].countPI << "," << diseaseTotalRegion[w].countPU  << "," << diseaseTotalRegion[w].countPT;
            myfile << "," << diseaseTotalRegion[w].sampleWaitTestCount << "," << diseaseTotalRegion[w].sampleWaitTestCountIn << "," << diseaseTotalRegion[w].sampleWaitTestCountOut;
            myfile << "," << diseaseTotalRegion[w].countPreTestLtoFOutTested << "," << diseaseTotalRegion[w].patientNotNotified << "," << diseaseTotalRegion[w].patientNotNotifiedPos << "\n";

            myfile8 << "Total," << diseaseTotalRegion[w].id << "," << diseaseTotalRegion[w].tier << "," << diseaseTotalRegion[w].district << "," << diseaseTotalRegion[w].region << "," << newPop << "," << diseaseList[w] << "," << int((float(diseaseTotalRegion[w].countPInf) * 100000) / float(newPop));
            myfile8 << "," << diseaseTotalRegion[w].countP << "," << diseaseTotalRegion[w].countPInf << "," << diseaseTotalRegion[w].countPLtoF;
            myfile8 << "," << diseaseTotalRegion[w].outCollectedCount + diseaseTotalRegion[w].inCollectedCount << "," << diseaseTotalRegion[w].outCollectedCount << "," << diseaseTotalRegion[w].inCollectedCount;
            if ((diseaseTotalRegion[w].sampleWaitForResourceInCount + diseaseTotalRegion[w].sampleWaitForResourceOutCount) < 1) {
                myfile8 << "," << "0";
            }
            else {
                myfile8 << "," << float(diseaseTotalRegion[w].sampleWaitForResourceInTime + diseaseTotalRegion[w].sampleWaitForResourceOutTime)/float(diseaseTotalRegion[w].sampleWaitForResourceInCount + diseaseTotalRegion[w].sampleWaitForResourceOutCount);
            }
            if (diseaseTotalRegion[w].sampleWaitForResourceInCount < 1) {
                myfile8 << "," << "0";
            }
            else {
                myfile8 << "," << float(diseaseTotalRegion[w].sampleWaitForResourceInTime)/float(diseaseTotalRegion[w].sampleWaitForResourceInCount);
            }
            if (diseaseTotalRegion[w].sampleWaitForResourceOutCount < 1) {
                myfile8 << "," << "0" << ",";
            }
            else {
                myfile8 << "," << float(diseaseTotalRegion[w].sampleWaitForResourceOutTime)/float(diseaseTotalRegion[w].sampleWaitForResourceOutCount) << ",";
            }
            if (diseaseTotalRegion[w].sampleTransCount  < 1) {
                myfile8 << "0" << "," << diseaseTotalRegion[w].sampleTransCount;
            }
            else {
                myfile8 << float(diseaseTotalRegion[w].sampleTransTime)/float(diseaseTotalRegion[w].sampleTransCount) << "," << diseaseTotalRegion[w].sampleTransCount;
            }
            myfile8 << "," << diseaseTotalRegion[w].sampleTestedInCount +  diseaseTotalRegion[w].sampleTestedOutCount << "," << diseaseTotalRegion[w].sampleTestedInCount << "," << diseaseTotalRegion[w].sampleTestedOutCount;
            //myfile8 << "," << diseaseTotalRegion[w].sampleLtoFCount;
            if (diseaseTotalRegion[w].sampleCollectToTestInCount < 1) {
                myfile8 << "," << "0";
            }
            else {
                myfile8 << "," << float(diseaseTotalRegion[w].sampleCollectToTestInTime)/float(diseaseTotalRegion[w].sampleCollectToTestInCount);
            }
            myfile8 << "," << float(diseaseTotalRegion[w].sampleCollectToTestInTime) << "," << diseaseTotalRegion[w].sampleCollectToTestInCount;
            if (diseaseTotalRegion[w].sampleCollectToTestOutCount < 1) {
                myfile8 << "," << "0";
            }
            else {
                myfile8 << "," << float(diseaseTotalRegion[w].sampleCollectToTestOutTime)/float(diseaseTotalRegion[w].sampleCollectToTestOutCount);
            }
            myfile8 << "," << float(diseaseTotalRegion[w].sampleCollectToTestOutTime) << "," << diseaseTotalRegion[w].sampleCollectToTestOutCount;
            myfile8 << "," << diseaseTotalRegion[w].countTestPos << ",";
            if (totalSampleCountPos > 0) {
                myfile8 << float(diseaseTotalRegion[w].countTruePos)/float(totalSampleCountPos) << ",";
                myfile8 << float(diseaseTotalRegion[w].countFalsePos)/float(totalSampleCountPos) << ",";
            }
            else {
                myfile8 << "0" << "," << "0" << ",";
            }
            myfile8 << diseaseTotalRegion[w].countTestNeg << ",";
            if (totalSampleCountNeg > 0) {
                myfile8 << float(diseaseTotalRegion[w].countTrueNeg)/float(totalSampleCountNeg) << ",";
                myfile8 << float(diseaseTotalRegion[w].countFalseNeg)/float(totalSampleCountNeg);
            }
            else {
                myfile8 << "0" << "," << "0";
            }
            if (debug2) myfile8 << "," << "marker";

            myfile8 << "," << diseaseTotalRegion[w].countMDInf << "," << diseaseTotalRegion[w].countMDUninf;
            myfile8 << "," << diseaseTotalRegion[w].countMDInf + diseaseTotalRegion[w].countMDUninf + diseaseTotalRegion[w].countTestPos;
            myfile8 << "," << diseaseTotalRegion[w].countFalsePos << "," << diseaseTotalRegion[w].countFalseNeg;
            myfile8 << "," << diseaseTotalRegion[w].countDiagnosis << "," << diseaseTotalRegion[w].countDiagnosisIn << "," << diseaseTotalRegion[w].countDiagnosisOut << "," << diseaseTotalRegion[w].countPreTestLtoFOut << "," << diseaseTotalRegion[w].countPreTestLtoFOutMinusTestedSamp << "," << diseaseTotalRegion[w].countPreTestLtoFDead << "," << diseaseTotalRegion[w].countPreTestLtoFORepresent;
             if (diseaseTotalRegion[w].sampleTestPosCountIn < 1) {
                myfile8 << "," << "0";
            }
            else {
                myfile8 << "," << float(diseaseTotalRegion[w].sampleTestPosTimeIn)/float(diseaseTotalRegion[w].sampleTestPosCountIn);
            }
            myfile8 << "," << float(diseaseTotalRegion[w].sampleTestPosTimeIn) << "," << diseaseTotalRegion[w].sampleTestPosCountIn;
            if (diseaseTotalRegion[w].sampleTestPosCountOut < 1) {
                myfile8 << "," << "0";
            }
            else {
                myfile8 << "," << float(diseaseTotalRegion[w].sampleTestPosTimeOut)/float(diseaseTotalRegion[w].sampleTestPosCountOut);
            }
            myfile8 << "," << float(diseaseTotalRegion[w].sampleTestPosTimeOut) << "," << diseaseTotalRegion[w].sampleTestPosCountOut;
            if(debug2) myfile8 << "," << "marker";
               
            myfile8 << "," << diseaseTotalRegion[w].countPInfTreated+diseaseTotalRegion[w].countPUninfTreated << "," << diseaseTotalRegion[w].countPInfTreated << "," << diseaseTotalRegion[w].countPUninfTreated << "," << diseaseTotalRegion[w].countPTC << "," << diseaseTotalRegion[w].countPTD << "," << diseaseTotalRegion[w].countPTR  << "," << diseaseTotalRegion[w].countRetreat << "," << diseaseTotalRegion[w].countRetreatCure << "," << diseaseTotalRegion[w].countRetreatDead;
            if ((diseaseTotalRegion[w].countPInfTreatedIn + diseaseTotalRegion[w].countPInfTreatedOut) < 1) {
                myfile8 << "," << "0";
            }
            else {
                myfile8 << "," << float (diseaseTotalRegion[w].onsetToTreat)/float(diseaseTotalRegion[w].countPInfTreatedIn + diseaseTotalRegion[w].countPInfTreatedOut);
            }
            myfile8 << "," << float (diseaseTotalRegion[w].onsetToTreat) << "," << (diseaseTotalRegion[w].countPInfTreatedIn + diseaseTotalRegion[w].countPInfTreatedOut);
            if (diseaseTotalRegion[w].countPInfTreatedIn < 1) {
                myfile8 << "," << "0";
            }
            else {
                myfile8 << "," << float (diseaseTotalRegion[w].onsetToTreatIn)/float(diseaseTotalRegion[w].countPInfTreatedIn);
            }
            myfile8 << "," << float (diseaseTotalRegion[w].onsetToTreatIn) << "," << diseaseTotalRegion[w].countPInfTreatedIn;
            if (diseaseTotalRegion[w].countPInfTreatedOut < 1) {
                myfile8 << "," << "0";
            }
            else {
                myfile8 << "," << float (diseaseTotalRegion[w].onsetToTreatOut)/float(diseaseTotalRegion[w].countPInfTreatedOut);
            }
            myfile8 << "," << float (diseaseTotalRegion[w].onsetToTreatOut) << "," << diseaseTotalRegion[w].countPInfTreatedOut;
            if(debug2) myfile8 << "," << "marker";
               
            if (diseaseTotalRegion[w].sampleReturnToDeathCount < 1) {
                myfile8 << "," << "0";
            }
            else {
                myfile8 << "," << float (diseaseTotalRegion[w].sampleReturnToDeathTime)/float(diseaseTotalRegion[w].sampleReturnToDeathCount);
            }
            myfile8 << "," << float (diseaseTotalRegion[w].sampleReturnToDeathTime) << "," << diseaseTotalRegion[w].sampleReturnToDeathCount;
            if (diseaseTotalRegion[w].sampleReturnToDeathPreDiagLtoFCount < 1) {
                myfile8 << "," << "0";
            }
            else {
                myfile8 << "," << float (diseaseTotalRegion[w].sampleReturnToDeathPreDiagLtoFTime)/float(diseaseTotalRegion[w].sampleReturnToDeathPreDiagLtoFCount);
            }
            myfile8 << "," << float (diseaseTotalRegion[w].sampleReturnToDeathPreDiagLtoFTime) << "," << diseaseTotalRegion[w].sampleReturnToDeathPreDiagLtoFCount;
            if (diseaseTotalRegion[w].sampleReturnToDeathInTrtCount < 1) {
                myfile8 << "," << "0";
            }
            else {
                myfile8 << "," << float (diseaseTotalRegion[w].sampleReturnToDeathInTrtTime)/float(diseaseTotalRegion[w].sampleReturnToDeathInTrtCount);
            }
            myfile8 << "," << float (diseaseTotalRegion[w].sampleReturnToDeathInTrtTime) << "," << diseaseTotalRegion[w].sampleReturnToDeathInTrtCount;
            if (diseaseTotalRegion[w].sampleReturnToDeathInRetrtCount < 1) {
                myfile8 << "," << "0";
            }
            else {
                myfile8 << "," << float (diseaseTotalRegion[w].sampleReturnToDeathInRetrtTime)/float(diseaseTotalRegion[w].sampleReturnToDeathInRetrtCount);
            }
            myfile8 << "," << float (diseaseTotalRegion[w].sampleReturnToDeathInRetrtTime) << "," << diseaseTotalRegion[w].sampleReturnToDeathInRetrtCount;
            if (diseaseTotalRegion[w].sampleReturnToExitCount < 1) {
                myfile8 << "," << "0";
            }
            else {
                myfile8 << "," << float (diseaseTotalRegion[w].sampleReturnToExitTime)/float(diseaseTotalRegion[w].sampleReturnToExitCount);
            }
            myfile8 << "," << float (diseaseTotalRegion[w].sampleReturnToExitTime) << "," << diseaseTotalRegion[w].sampleReturnToExitCount;
            myfile8 << "," << diseaseTotalRegion[w].test1CountIn + diseaseTotalRegion[w].test1CountOut << "," << diseaseTotalRegion[w].test1CountIn << "," << diseaseTotalRegion[w].test1CountOut << "," << diseaseTotalRegion[w].test2Count << "," << diseaseTotalRegion[w]. test2PosCount;
            if (diseaseTotalRegion[w].test2PosCount < 1) {
                myfile8 << "," << "0";
            }
            else {
                myfile8 << "," << float (diseaseTotalRegion[w].test2TruePosCount)/float(diseaseTotalRegion[w].test2PosCount);
            }
            myfile8 << "," << diseaseTotalRegion[w].test2NegCount;
            if (diseaseTotalRegion[w].test2NegCount < 1) {
                myfile8 << "," << "0";
            }
            else {
                myfile8 << "," << float (diseaseTotalRegion[w].test2TrueNegCount)/float(diseaseTotalRegion[w].test2NegCount);
            }
            myfile8 << "," << diseaseTotalRegion[w].test1PosCount;
            if (diseaseTotalRegion[w].test1PosCount < 1) {
                myfile8 << "," << "0";
            }
            else {
                myfile8 << "," << float (diseaseTotalRegion[w].test1TruePosCount)/float(diseaseTotalRegion[w].test1PosCount);
            }
            myfile8 << "," << diseaseTotalRegion[w].test1NegCount;
            if (diseaseTotalRegion[w].test1NegCount < 1) {
                myfile8 << "," << "0";
            }
            else {
                myfile8 << "," << float (diseaseTotalRegion[w].test1TrueNegCount)/float(diseaseTotalRegion[w].test1NegCount);
            }
            if(debug2) myfile8 << "," << "marker";
               
            myfile8 << "," << diseaseTotalRegion[w].countP << "," << diseaseTotalRegion[w].countPI << "," << diseaseTotalRegion[w].countPU  << "," << diseaseTotalRegion[w].countPT;
            myfile8 << "," << diseaseTotalRegion[w].sampleWaitTestCount << "," << diseaseTotalRegion[w].sampleWaitTestCountIn << "," << diseaseTotalRegion[w].sampleWaitTestCountOut;
            myfile8 << "," << diseaseTotalRegion[w].countPreTestLtoFOutTested << "," << diseaseTotalRegion[w].patientNotNotified << "," << diseaseTotalRegion[w].patientNotNotifiedPos << "\n";

            if (individualFlag) {

                //treated = diseaseTotalRegion[w].countPT + diseaseTotalRegion[w].countPR + diseaseTotalRegion[w].countPC + diseaseTotalRegion[w].countPDI;
                //treated = diseaseTotalRegion[w].countPT + diseaseTotalRegion[w].countPC;

                myfile9 << "-----------------------------------------------------------------------------------" << "\n";
                myfile9 << "stats.total.region.population_" << newPop << ",patients." << diseaseTotalRegion[w].countP << ",for.disease." << diseaseList[w] << "\n";
                myfile9 << "stats.patient" << ",untested.out" << ",untested.in" <<  ",under.treat" << ",failed.treat" << ",untreated" << ",cured" << ",infec.treat" << ",uninfec.treat" << ",other";
                myfile9 << ",non.infected.not.treated" << ",non.infected.not.returned" << ",dead.infected.treated" << ",dead.infected.untreated" << ",dead.uninfected" << "\n";
                myfile9 << "," <<  diseaseTotalRegion[w].countPU << "," << diseaseTotalRegion[w].countPI << "," << diseaseTotalRegion[w].countPT << "," << diseaseTotalRegion[w].countPR << "," << diseaseTotalRegion[w].countPUT;
                myfile9 << "," << diseaseTotalRegion[w].countPC  << "," << diseaseTotalRegion[w].countPInfTreated << "," << diseaseTotalRegion[w].countPUninfTreated << "," << diseaseTotalRegion[w].countPO;
                myfile9 << "," << diseaseTotalRegion[w].countPUI << "," << diseaseTotalRegion[w].countPUINR << "," << diseaseTotalRegion[w].countPDI << "," << diseaseTotalRegion[w].countPDIUT << "," << diseaseTotalRegion[w].countPDU << "\n";
                
                myfile2 << "-----------------------------------------------------------------------------------  " << "\n";
                myfile2 << "stats.total.region.population_" << newPop << ",patients." << diseaseTotalRegion[w].countP << ",for.disease." << diseaseList[w] << "\n";
                myfile2 << "stats.patient" << ",untested.out" << ",untested.in" <<  ",under.treat" << ",failed.treat" << ",untreated" << ",cured" << ",infec.treat" << ",uninfec.treat" << ",other";
                myfile2 << ",non.infected.not.treated" << ",non.infected.not.returned" << ",dead.infected.treated" << ",dead.infected.untreated" << ",dead.uninfected" << "\n";
                myfile2 << "," <<  diseaseTotalRegion[w].countPU << "," << diseaseTotalRegion[w].countPI << "," << diseaseTotalRegion[w].countPT << "," << diseaseTotalRegion[w].countPR << "," << diseaseTotalRegion[w].countPUT;
                myfile2 << "," << diseaseTotalRegion[w].countPC  << "," << diseaseTotalRegion[w].countPInfTreated << "," << diseaseTotalRegion[w].countPUninfTreated << "," << diseaseTotalRegion[w].countPO;
                myfile2 << "," << diseaseTotalRegion[w].countPUI << "," << diseaseTotalRegion[w].countPUINR << "," << diseaseTotalRegion[w].countPDI << "," << diseaseTotalRegion[w].countPDIUT << "," << diseaseTotalRegion[w].countPDU << "\n";
            }
        }
        if (debug) std::cout << "stats: inside agent stats mark 1"  << "\n";

        myfile << "-----------------------------------------------------------------------------------  " << "\n";
        myfile << statHeader1 << statHeader2 << statHeader3 << statHeader4 << statHeader5 << statHeader5a << statHeader6 << statHeader7 << statHeader8;
        myfile << statHeader9 << statHeader10 << statHeader10a << statHeader11 << statHeader12 << statHeader12a << statHeader12b;
        myfile << statHeader13 << statHeader13a << statHeader13b << statHeader13c << statHeader13d << statHeader14 << statHeader15 << statHeader16 << statHeader17;

        //treated = totalRegion.countPT + totalRegion.countPR + totalRegion.countPC + totalRegion.countPDI;
        //infected = treated + totalRegion.countPUT + totalRegion.countPDIUT;
        //treated = totalRegion.countPT + totalRegion.countPC;
        //infected = treated + totalRegion.countPUT + totalRegion.countPDIUT + totalRegion.countPR + totalRegion.countPDI;

        // update population by population growth if it goes on more that 1 year
        newPop = totalRegion.pop;
        startIndex = referenceYear - 2015;
        endIndex = startIndex + noOfYears;
        if (noOfYears > 0) {

            for (int i = startIndex; i < endIndex; i++) {

                // multiply by growth rate for x number of years
                newPop = (int) (newPop * popGrowthRateArray[i][1]);
            }
        }
        
        // count up all the sample results
        totalSampleCountPos = totalRegion.countTruePos + totalRegion.countFalsePos;
        totalSampleCountNeg = totalRegion.countTrueNeg + totalRegion.countFalseNeg;

        //myfile << "stats: total region  population = " << totalRegion.pop << ", patients = " << totalRegion.countP << "\n";
        //myfile << "stats: patient" << " Untested-out = " << totalRegion.countPU << " Untested-in =  " << totalRegion.countPI << ", under treat = " << totalRegion.countPT << ", failed treat = " << totalRegion.countPR << ", untreated = " << totalRegion.countPUT << ", cured = " << totalRegion.countPC << ", uninfec-treat = " << totalRegion.countPUIUT << ", Other = " << totalRegion.countPO << "\n";
        //myfile << "               Non-infected - not treated  = " << totalRegion.countPUI << ", Non-infected - not returned = " << totalRegion.countPUINR << ", dead - infected = " << totalRegion.countPDI << ", dead - infected untreated = " << totalRegion.countPDIUT << ", dead - uninfected = " << totalRegion.countPDU << "\n";
        myfile << "," << "," << totalRegion.tier << "," << totalRegion.district << "," << totalRegion.region << "," << newPop << "," << "" << "," << "";
        myfile << "," << totalRegion.countP << "," << totalRegion.countPInf << "," << totalRegion.countPLtoF;
        myfile << "," << totalRegion.outCollectedCount + totalRegion.inCollectedCount << "," << totalRegion.outCollectedCount << "," << totalRegion.inCollectedCount;
        if ((totalRegion.sampleWaitForResourceInCount + totalRegion.sampleWaitForResourceOutCount) < 1) {
            myfile << "," << "0";
        }
        else {
            myfile << "," << float(totalRegion.sampleWaitForResourceInTime + totalRegion.sampleWaitForResourceOutTime)/float(totalRegion.sampleWaitForResourceInCount + totalRegion.sampleWaitForResourceOutCount);
        }
        if (totalRegion.sampleWaitForResourceInCount < 1) {
            myfile << "," << "0";
        }
        else {
            myfile << "," << float(totalRegion.sampleWaitForResourceInTime)/float(totalRegion.sampleWaitForResourceInCount);
        }
        if (totalRegion.sampleWaitForResourceOutCount < 1) {
            myfile << "," << "0" << ",";
        }
        else {
            myfile << "," << float(totalRegion.sampleWaitForResourceOutTime)/float(totalRegion.sampleWaitForResourceOutCount) << ",";
        }
        if (totalRegion.sampleTransCount  < 1) {
            myfile << "0" << "," << totalRegion.sampleTransCount;
        }
        else {
            myfile << float(totalRegion.sampleTransTime)/float(totalRegion.sampleTransCount) << "," << totalRegion.sampleTransCount;
        }
        myfile << "," << totalRegion.sampleTestedInCount +  totalRegion.sampleTestedOutCount << "," << totalRegion.sampleTestedInCount << "," << totalRegion.sampleTestedOutCount;
        //myfile << "," << totalRegion.sampleLtoFCount;
        if (totalRegion.sampleCollectToTestInCount < 1) {
            myfile << "," << "0";
        }
        else {
            myfile << "," << float(totalRegion.sampleCollectToTestInTime)/float(totalRegion.sampleCollectToTestInCount);
        }
        myfile << "," << float(totalRegion.sampleCollectToTestInTime) << "," << totalRegion.sampleCollectToTestInCount;
        if (totalRegion.sampleCollectToTestOutCount < 1) {
            myfile << "," << "0";
        }
        else {
            myfile << "," << float(totalRegion.sampleCollectToTestOutTime)/float(totalRegion.sampleCollectToTestOutCount);
        }
        myfile << "," << float(totalRegion.sampleCollectToTestOutTime) << "," << totalRegion.sampleCollectToTestOutCount;
        myfile << "," << totalRegion.countTestPos << ",";
        if (totalSampleCountPos > 0) {
            myfile << float(totalRegion.countTruePos)/float(totalSampleCountPos) << ",";
            myfile << float(totalRegion.countFalsePos)/float(totalSampleCountPos) << ",";
        }
        else {
            myfile << "0" << "," << "0" << ",";
        }
        if (debugOutput) {
        
            std::cout << "region countTestPos = " << totalRegion.countTestPos << ", totalSampleCountPos " << totalSampleCountPos  << "\n";
            std::cout << "region true pos = " << totalRegion.countTruePos << ", false pos " << totalRegion.countFalsePos << ", true neg = " << totalRegion.countTrueNeg << ", false neg " << totalRegion.countFalseNeg << "\n";
        }
        myfile << totalRegion.countTestNeg << ",";
        if (totalSampleCountNeg > 0) {
            myfile << float(totalRegion.countTrueNeg)/float(totalSampleCountNeg) << ",";
            myfile << float(totalRegion.countFalseNeg)/float(totalSampleCountNeg);
        }
        else {
            myfile << "0" << "," << "0";
        }
        if (debug2) myfile << "," << "marker";

        myfile << "," << totalRegion.countMDInf << "," << totalRegion.countMDUninf;
        myfile << "," << totalRegion.countMDInf + totalRegion.countMDUninf + totalRegion.countTestPos;
        myfile << "," << totalRegion.countFalsePos << "," << totalRegion.countFalseNeg;
        myfile << "," << totalRegion.countDiagnosis << "," << totalRegion.countDiagnosisIn << "," << totalRegion.countDiagnosisOut << "," << totalRegion.countPreTestLtoFOut << "," << totalRegion.countPreTestLtoFOutMinusTestedSamp << "," << totalRegion.countPreTestLtoFDead << "," << totalRegion.countPreTestLtoFORepresent;
        if (totalRegion.sampleTestPosCountIn < 1) {
            myfile << "," << "0";
        }
        else {
            myfile << "," << float(totalRegion.sampleTestPosTimeIn)/float(totalRegion.sampleTestPosCountIn);
        }
        myfile << "," << float(totalRegion.sampleTestPosTimeIn) << "," << totalRegion.sampleTestPosCountIn;
        if (totalRegion.sampleTestPosCountOut < 1) {
            myfile << "," << "0";
        }
        else {
            myfile << "," << float(totalRegion.sampleTestPosTimeOut)/float(totalRegion.sampleTestPosCountOut);
        }
        myfile << "," << float(totalRegion.sampleTestPosTimeOut) << "," << totalRegion.sampleTestPosCountOut;
        if(debug2) myfile << "," << "marker";
               
        myfile << "," << totalRegion.countPInfTreated+totalRegion.countPUninfTreated << "," << totalRegion.countPInfTreated << "," << totalRegion.countPUninfTreated << "," << totalRegion.countPTC << "," << totalRegion.countPTD << "," << totalRegion.countPTR  << "," << totalRegion.countRetreat << "," << totalRegion.countRetreatCure << "," << totalRegion.countRetreatDead;
        if ((totalRegion.countPInfTreatedIn + totalRegion.countPInfTreatedOut) < 1) {
            myfile << "," << "0";
        }
        else {
            myfile << "," << float (totalRegion.onsetToTreat)/float(totalRegion.countPInfTreatedIn + totalRegion.countPInfTreatedOut);
        }
        myfile << "," << float (totalRegion.onsetToTreat) << "," << (totalRegion.countPInfTreatedIn + totalRegion.countPInfTreatedOut);
        if (totalRegion.countPInfTreatedIn < 1) {
            myfile << "," << "0";
        }
        else {
            myfile << "," << float (totalRegion.onsetToTreatIn)/float(totalRegion.countPInfTreatedIn);
        }
        myfile << "," << float (totalRegion.onsetToTreatIn) << "," << totalRegion.countPInfTreatedIn;
        if (totalRegion.countPInfTreatedOut < 1) {
            myfile << "," << "0";
        }
        else {
            myfile << "," << float (totalRegion.onsetToTreatOut)/float(totalRegion.countPInfTreatedOut);
        }
        myfile << "," << float (totalRegion.onsetToTreatOut) << "," << totalRegion.countPInfTreatedOut;
        if(debug2) myfile << "," << "marker";
               
        if (totalRegion.sampleReturnToDeathCount < 1) {
            myfile << "," << "0";
        }
        else {
            myfile << "," << float (totalRegion.sampleReturnToDeathTime)/float(totalRegion.sampleReturnToDeathCount);
        }
        myfile << "," << float (totalRegion.sampleReturnToDeathTime) << "," << totalRegion.sampleReturnToDeathCount;
        if (totalRegion.sampleReturnToDeathPreDiagLtoFCount < 1) {
            myfile << "," << "0";
        }
        else {
            myfile << "," << float (totalRegion.sampleReturnToDeathPreDiagLtoFTime)/float(totalRegion.sampleReturnToDeathPreDiagLtoFCount);
        }
        myfile << "," << float (totalRegion.sampleReturnToDeathPreDiagLtoFTime) << "," << totalRegion.sampleReturnToDeathPreDiagLtoFCount;
        if (totalRegion.sampleReturnToDeathInTrtCount < 1) {
            myfile << "," << "0";
        }
        else {
            myfile << "," << float (totalRegion.sampleReturnToDeathInTrtTime)/float(totalRegion.sampleReturnToDeathInTrtCount);
        }
        myfile << "," << float (totalRegion.sampleReturnToDeathInTrtTime) << "," << totalRegion.sampleReturnToDeathInTrtCount;
        if (totalRegion.sampleReturnToDeathInRetrtCount < 1) {
            myfile << "," << "0";
        }
        else {
            myfile << "," << float (totalRegion.sampleReturnToDeathInRetrtTime)/float(totalRegion.sampleReturnToDeathInRetrtCount);
        }
        myfile << "," << float (totalRegion.sampleReturnToDeathInRetrtTime) << "," << totalRegion.sampleReturnToDeathInRetrtCount;
        if (totalRegion.sampleReturnToExitCount < 1) {
            myfile << "," << "0";
        }
        else {
            myfile << "," << float (totalRegion.sampleReturnToExitTime)/float(totalRegion.sampleReturnToExitCount);
        }
        myfile << "," << float (totalRegion.sampleReturnToExitTime) << "," << totalRegion.sampleReturnToExitCount;
        myfile << "," << totalRegion.test1CountIn + totalRegion.test1CountOut << "," << totalRegion.test1CountIn << "," << totalRegion.test1CountOut << "," << totalRegion.test2Count << "," << totalRegion. test2PosCount;
        if (totalRegion.test2PosCount < 1) {
            myfile << "," << "0";
        }
        else {
            myfile << "," << float (totalRegion.test2TruePosCount)/float(totalRegion.test2PosCount);
        }
        myfile << "," << totalRegion.test2NegCount;
        if (totalRegion.test2NegCount < 1) {
            myfile << "," << "0";
        }
        else {
            myfile << "," << float (totalRegion.test2TrueNegCount)/float(totalRegion.test2NegCount);
        }
        myfile << "," << totalRegion.test1PosCount;
        if (totalRegion.test1PosCount < 1) {
            myfile << "," << "0";
        }
        else {
            myfile << "," << float (totalRegion.test1TruePosCount)/float(totalRegion.test1PosCount);
        }
        myfile << "," << totalRegion.test1NegCount;
        if (totalRegion.test1NegCount < 1) {
            myfile << "," << "0";
        }
        else {
            myfile << "," << float (totalRegion.test1TrueNegCount)/float(totalRegion.test1NegCount);
        }
        if(debug2) myfile << "," << "marker";
               
        myfile << "," << totalRegion.countP << "," << totalRegion.countPI << "," << totalRegion.countPU  << "," << totalRegion.countPT;
        myfile << "," << totalRegion.sampleWaitTestCount << "," << totalRegion.sampleWaitTestCountIn << "," << totalRegion.sampleWaitTestCountOut;
        myfile << "," << totalRegion.countPreTestLtoFOutTested << "," << totalRegion.patientNotNotified << "," << totalRegion.patientNotNotifiedPos << "\n";

        myfile << "-----------------------------------------------------------------------------------  " << "\n";

        if (individualFlag) {

            //treated = totalRegion.countPT + totalRegion.countPR + totalRegion.countPC + totalRegion.countPDI;
            //treated = totalRegion.countPT + totalRegion.countPC;

            myfile9 << "-----------------------------------------------------------------------------------  " << "\n";
            myfile9 << "stats.total.region .population" <<  ",patients" <<  "\n";
            myfile9 <<  totalRegion.pop << "," << totalRegion.countP << "\n";
            myfile9 << "stats.patient" << ",s.untested-out" << ",s.untested-in" << ",s.under.treat" << ",failed.treat" << ",untreatedtrue.negative" << ",unteated.no.sample" << ",untreated.ltof";
            myfile9 << ",cured" << ",infec.treat" << ",uninfec.treat" << ",other";
            myfile9 << ",non.infected.not.treated" << ",non.infected.not.returned" << ",dead.infected.treated" << ",dead-infected.untreated" << ",dead.uninfected" << "\n";
            myfile9 << "," << totalRegion.countPU << "," << totalRegion.countPI << "," << totalRegion.countPT << "," << totalRegion.countPR << "," << totalRegion.countPUTFN << "," << totalRegion.countPUTNS;
            myfile9 << "," << totalRegion.countPUTLtoF << "," << totalRegion.countPC  << "," << totalRegion.countPInfTreated << "," << totalRegion.countPUninfTreated << "," << totalRegion.countPO;
            myfile9 << "," << totalRegion.countPUI << "," << totalRegion.countPUINR << "," << totalRegion.countPDI << "," << totalRegion.countPDIUT << "," << totalRegion.countPDU << "\n";
            
            myfile2 << "-----------------------------------------------------------------------------------  " << "\n";
            myfile2 << "stats.total.region.population" <<  ",patients" <<  "\n";
            myfile2 <<  totalRegion.pop << "," << totalRegion.countP << "\n";
            myfile2 << "stats.patient" << ",s.untested.out" << ",s.untested.in" << ",s.under.treat" << ",failed.treat" << ",untreatedtrue.negative" << ",unteated.no.sample" << ",untreated.ltof";
            myfile2 << ",cured" << ",infec.treat" << ",uninfec.treat" << ",other";
            myfile2 << ",non.infected.not.treated" << ",non.infected.not.returned" << ",dead.infected.treated" << ",dead.infected.untreated" << ",dead.uninfected" << "\n";
            myfile2 << "," << totalRegion.countPU << "," << totalRegion.countPI << "," << totalRegion.countPT << "," << totalRegion.countPR <<  "," << totalRegion.countPUTFN << "," << totalRegion.countPUTNS;
            myfile2 << "," << totalRegion.countPUTLtoF << "," << totalRegion.countPC  << "," << totalRegion.countPInfTreated << "," << totalRegion.countPUninfTreated << "," << totalRegion.countPO;
            myfile2 << "," << totalRegion.countPUI << "," << totalRegion.countPUINR << "," << totalRegion.countPDI << "," << totalRegion.countPDIUT << "," << totalRegion.countPDU << "\n";
            myfile2.close();
        }


        myfile << statHeader1 << statHeader2 << statHeader3 << statHeader4 << statHeader5 << statHeader5a << statHeader6 << statHeader7 << statHeader8;
        myfile << statHeader9 << statHeader10 << statHeader10a << statHeader11 << statHeader12 << statHeader12a << statHeader12b;
        myfile << statHeader13 << statHeader13a << statHeader13b << statHeader13c << statHeader13d << statHeader14 << statHeader15 << statHeader16 << statHeader17;

        myfile7 << statHeader1 << statHeader2 << statHeader3 << statHeader4 << statHeader5 << statHeader5a << statHeader6 << statHeader7 << statHeader8;
        myfile7 << statHeader9 << statHeader10 << statHeader10a << statHeader11 << statHeader12 << statHeader12a << statHeader12b;
        myfile7 << statHeader13 << statHeader13a << statHeader13b << statHeader13c << statHeader13d << statHeader14 << statHeader15 << statHeader16 << statHeader17;

        for (int w = 0; w < diseaseCount; w++) {

            if (debug) {
                std::cout << "stats: patients  district count = " << districtCount[w] << ", disease = " << diseaseList[w] << "\n";
            }

            myfile << "stats.patients.district.count" <<  ",disease" <<  "\n";
            myfile << districtCount[w] << "," << diseaseList[w] << "\n";

            if (individualFlag) {

                myfile9 << "-----------------------------------------------------------------------------------  " << "\n";
                myfile9 << "stats.patients.district.count" <<  ",disease" <<  "\n";
                myfile9 <<  districtCount[w] << "," << diseaseList[w] << "\n";
                
                myfile3 << "-----------------------------------------------------------------------------------  " << "\n";
                myfile3 << "stats.patients.district.count" <<  ",disease" <<  "\n";
                myfile3 <<  districtCount[w] << "," << diseaseList[w] << "\n";
            }

            for (int i = 0; i < districtCount[w]; i++) {

                // reset total counts for district
                distNoPatients[w] = 0;
                distAge[w] = 0;
                distNoAlive[w] = 0;
                distNoInfected[w] = 0;
                distNoTestPos[w] = 0;
                distNoMDOverride[w] = 0;
                distTotalTimeFromOnset[w] = 0;
                distTotalTimeInfectious[w] = 0;
                distTotaltimeInTreatment[w] = 0;
                distTotalTimeNotInTreatment[w] = 0;
                distTotalTimeFacilityReturnTime[w] = 0;
                distTotalTimeResultsReturned[w] = 0;
                distTotalNoOfReturnVisits[w] = 0;
                distDisease[w] = diseaseList[w];

                withTB = districts[w][i].countPT + districts[w][i].countPR + districts[w][i].countPUT + districts[w][i].countPDI + districts[w][i].countPDIUT;

                //treated = districts[w][i].countPT + districts[w][i].countPR + districts[w][i].countPC + districts[w][i].countPDI;
                //infected = treated + districts[w][i].countPUT + districts[w][i].countPDIUT;
                //treated = districts[w][i].countPT + districts[w][i].countPC;
                //infected = treated + districts[w][i].countPUT + districts[w][i].countPDIUT + districts[w][i].countPR + districts[w][i].countPDI;

                // update population by population growth if it goes on more that 1 year
                newPop = districts[w][i].pop;
                startIndex = referenceYear - 2015;
                endIndex = startIndex + noOfYears;
                if (noOfYears > 0) {

                    for (int i = startIndex; i < endIndex; i++) {

                        // multiply by growth rate for x number of years
                        newPop = (int) (newPop * popGrowthRateArray[i][1]);
                    }
                }

                // prevalence = no. with TB * (100,000/population)                   (no. with TB/pop = prevalence/100000)
                if (newPop == 0) {
                    prevalence = 0;
                }
                else {
                    //prevalence = int((float(withTB) * 100000) / float(newPop));
                    prevalence = int((float(districts[w][i].countPInf) * 100000) / float(newPop));
                }

                // count up all the sample results
                totalSampleCountPos = districts[w][i].countTruePos + districts[w][i].countFalsePos;
                totalSampleCountNeg = districts[w][i].countTrueNeg + districts[w][i].countFalseNeg;

                //myfile << "stats: district =  " << districts[w][i].name << ", population = " << districts[w][i].pop << ", patients = " << districts[w][i].countP << ", with calculated prevalence = " << prevalence << "\n";
                //myfile << "stats: patient" << " Untested-out = " << districts[w][i].countPU << " Untested-in =  " << districts[w][i].countPI << ", under treat = " << districts[w][i].countPT << ", failed treat = " << districts[w][i].countPR << ", untreated = " << districts[w][i].countPUT << ", cured = " << districts[w][i].countPC << ", uninfec-treat = " << districts[w][i].countPUIUT << ", Other = " << districts[w][i].countPO << "\n";
                //myfile << "               Non-infected - not treated  = " << districts[w][i].countPUI << ", Non-infected - not returned = " << districts[w][i].countPUINR << ", dead - infected = " << districts[w][i].countPDI << ", dead - infected untreated = " << districts[w][i].countPDIUT << ", dead - uninfected = " << districts[w][i].countPDU << "\n";
                myfile << districts[w][i].name << "," << districts[w][i].id << "," << districts[w][i].tier << "," << districts[w][i].district << "," << districts[w][i].region << "," << newPop << "," << diseaseList[w] << "," << prevalence;
                myfile << "," << districts[w][i].countP << "," << districts[w][i].countPInf << "," << districts[w][i].countPLtoF;
                myfile << "," << districts[w][i].outCollectedCount + districts[w][i].inCollectedCount << "," << districts[w][i].outCollectedCount << "," << districts[w][i].inCollectedCount;
                if ((districts[w][i].sampleWaitForResourceInCount + districts[w][i].sampleWaitForResourceOutCount) < 1) {
                    myfile << "," << "0";
                }
                else {
                    myfile << "," << float(districts[w][i].sampleWaitForResourceInTime + districts[w][i].sampleWaitForResourceOutTime)/float(districts[w][i].sampleWaitForResourceInCount + districts[w][i].sampleWaitForResourceOutCount);
                }
                if (districts[w][i].sampleWaitForResourceInCount < 1) {
                    myfile << "," << "0";
                }
                else {
                    myfile << "," << float(districts[w][i].sampleWaitForResourceInTime)/float(districts[w][i].sampleWaitForResourceInCount);
                }
                if (districts[w][i].sampleWaitForResourceOutCount < 1) {
                    myfile << "," << "0" << ",";
                }
                else {
                    myfile << "," << float(districts[w][i].sampleWaitForResourceOutTime)/float(districts[w][i].sampleWaitForResourceOutCount) << ",";
                }
                if (districts[w][i].sampleTransCount  < 1) {
                    myfile << "0" << "," << districts[w][i].sampleTransCount;
                }
                else {
                    myfile << float(districts[w][i].sampleTransTime)/float(districts[w][i].sampleTransCount) << "," << districts[w][i].sampleTransCount;
                }
                myfile << "," << districts[w][i].sampleTestedInCount +  districts[w][i].sampleTestedOutCount << "," << districts[w][i].sampleTestedInCount << "," << districts[w][i].sampleTestedOutCount;
                //myfile << "," << districts[w][i].sampleLtoFCount;
                if (districts[w][i].sampleCollectToTestInCount < 1) {
                    myfile << "," << "0";
                }
                else {
                    myfile << "," << float(districts[w][i].sampleCollectToTestInTime)/float(districts[w][i].sampleCollectToTestInCount);
                }
                myfile << "," << float(districts[w][i].sampleCollectToTestInTime) << "," << districts[w][i].sampleCollectToTestInCount;
                if (districts[w][i].sampleCollectToTestOutCount < 1) {
                    myfile << "," << "0";
                }
                else {
                    myfile << "," << float(districts[w][i].sampleCollectToTestOutTime)/float(districts[w][i].sampleCollectToTestOutCount);
                }
                myfile << "," << float(districts[w][i].sampleCollectToTestOutTime) << "," << districts[w][i].sampleCollectToTestOutCount;
                myfile << "," << districts[w][i].countTestPos << ",";
                if (totalSampleCountPos > 0) {
                    myfile << float(districts[w][i].countTruePos)/float(totalSampleCountPos) << ",";
                    myfile << float(districts[w][i].countFalsePos)/float(totalSampleCountPos) << ",";
                }
                else {
                    myfile << "0" << "," << "0" << ",";
                }
                myfile << districts[w][i].countTestNeg << ",";
                if (totalSampleCountNeg > 0) {
                    myfile << float(districts[w][i].countTrueNeg)/float(totalSampleCountNeg) << ",";
                    myfile << float(districts[w][i].countFalseNeg)/float(totalSampleCountNeg);
                }
                else {
                    myfile << "0" << "," << "0";
                }
                if (debug2) myfile << "," << "marker";

                myfile << "," << districts[w][i].countMDInf << "," << districts[w][i].countMDUninf;
                myfile << "," << districts[w][i].countMDInf + districts[w][i].countMDUninf + districts[w][i].countTestPos;
                myfile << "," << districts[w][i].countFalsePos << "," << districts[w][i].countFalseNeg;
                myfile << "," << districts[w][i].countDiagnosis << "," << districts[w][i].countDiagnosisIn << "," << districts[w][i].countDiagnosisOut << "," << districts[w][i].countPreTestLtoFOut << "," << districts[w][i].countPreTestLtoFOutMinusTestedSamp << "," << districts[w][i].countPreTestLtoFDead << "," << districts[w][i].countPreTestLtoFORepresent;
                if (districts[w][i].sampleTestPosCountIn < 1) {
                    myfile << "," << "0";
                }
                else {
                    myfile << "," << float(districts[w][i].sampleTestPosTimeIn)/float(districts[w][i].sampleTestPosCountIn);
                }
                myfile << "," << float(districts[w][i].sampleTestPosTimeIn) << "," << districts[w][i].sampleTestPosCountIn;
                if (districts[w][i].sampleTestPosCountOut < 1) {
                    myfile << "," << "0";
                }
                else {
                    myfile << "," << float(districts[w][i].sampleTestPosTimeOut)/float(districts[w][i].sampleTestPosCountOut);
                }
                myfile << "," << float(districts[w][i].sampleTestPosTimeOut) << "," << districts[w][i].sampleTestPosCountOut;
                if(debug2) myfile << "," << "marker";
               
                myfile << "," << districts[w][i].countPInfTreated+districts[w][i].countPUninfTreated << "," << districts[w][i].countPInfTreated << "," << districts[w][i].countPUninfTreated << "," << districts[w][i].countPTC << "," << districts[w][i].countPTD << "," << districts[w][i].countPTR  << "," << districts[w][i].countRetreat << "," << districts[w][i].countRetreatCure << "," << districts[w][i].countRetreatDead;
                if ((districts[w][i].countPInfTreatedIn + districts[w][i].countPInfTreatedOut) < 1) {
                    myfile << "," << "0";
                }
                else {
                    myfile << "," << float (districts[w][i].onsetToTreat)/float(districts[w][i].countPInfTreatedIn + districts[w][i].countPInfTreatedOut);
                }
                myfile << "," << float (districts[w][i].onsetToTreat) << "," << (districts[w][i].countPInfTreatedIn + districts[w][i].countPInfTreatedOut);
                if (districts[w][i].countPInfTreatedIn < 1) {
                    myfile << "," << "0";
                }
                else {
                    myfile << "," << float (districts[w][i].onsetToTreatIn)/float(districts[w][i].countPInfTreatedIn);
                }
                myfile << "," << float (districts[w][i].onsetToTreatIn) << "," << districts[w][i].countPInfTreatedIn;
                if (districts[w][i].countPInfTreatedOut < 1) {
                    myfile << "," << "0";
                }
                else {
                    myfile << "," << float (districts[w][i].onsetToTreatOut)/float(districts[w][i].countPInfTreatedOut);
                }
                myfile << "," << float (districts[w][i].onsetToTreatOut) << "," << districts[w][i].countPInfTreatedOut;
                if(debug2) myfile << "," << "marker";
               
                if (districts[w][i].sampleReturnToDeathCount < 1) {
                    myfile << "," << "0";
                }
                else {
                    myfile << "," << float (districts[w][i].sampleReturnToDeathTime)/float(districts[w][i].sampleReturnToDeathCount);
                }
                myfile << "," << float (districts[w][i].sampleReturnToDeathTime) << "," << districts[w][i].sampleReturnToDeathCount;
                if (districts[w][i].sampleReturnToDeathPreDiagLtoFCount < 1) {
                    myfile << "," << "0";
                }
                else {
                    myfile << "," << float (districts[w][i].sampleReturnToDeathPreDiagLtoFTime)/float(districts[w][i].sampleReturnToDeathPreDiagLtoFCount);
                }
                myfile << "," << float (districts[w][i].sampleReturnToDeathPreDiagLtoFTime) << "," << districts[w][i].sampleReturnToDeathPreDiagLtoFCount;
                if (districts[w][i].sampleReturnToDeathInTrtCount < 1) {
                    myfile << "," << "0";
                }
                else {
                    myfile << "," << float (districts[w][i].sampleReturnToDeathInTrtTime)/float(districts[w][i].sampleReturnToDeathInTrtCount);
                }
                myfile << "," << float (districts[w][i].sampleReturnToDeathInTrtTime) << "," << districts[w][i].sampleReturnToDeathInTrtCount;
                if (districts[w][i].sampleReturnToDeathInRetrtCount < 1) {
                    myfile << "," << "0";
                }
                else {
                    myfile << "," << float (districts[w][i].sampleReturnToDeathInRetrtTime)/float(districts[w][i].sampleReturnToDeathInRetrtCount);
                }
                myfile << "," << float (districts[w][i].sampleReturnToDeathInRetrtTime) << "," << districts[w][i].sampleReturnToDeathInRetrtCount;
                if (districts[w][i].sampleReturnToExitCount < 1) {
                    myfile << "," << "0";
                }
                else {
                    myfile << "," << float (districts[w][i].sampleReturnToExitTime)/float(districts[w][i].sampleReturnToExitCount);
                }
                myfile << "," << float (districts[w][i].sampleReturnToExitTime) << "," << districts[w][i].sampleReturnToExitCount;
                myfile << "," << districts[w][i].test1CountIn + districts[w][i].test1CountOut << "," << districts[w][i].test1CountIn << "," << districts[w][i].test1CountOut << "," << districts[w][i].test2Count << "," << districts[w][i]. test2PosCount;
                if (districts[w][i].test2PosCount < 1) {
                    myfile << "," << "0";
                }
                else {
                    myfile << "," << float (districts[w][i].test2TruePosCount)/float(districts[w][i].test2PosCount);
                }
                myfile << "," << districts[w][i].test2NegCount;
                if (districts[w][i].test2NegCount < 1) {
                    myfile << "," << "0";
                }
                else {
                    myfile << "," << float (districts[w][i].test2TrueNegCount)/float(districts[w][i].test2NegCount);
                }
                myfile << "," << districts[w][i].test1PosCount;
                if (districts[w][i].test1PosCount < 1) {
                    myfile << "," << "0";
                }
                else {
                    myfile << "," << float (districts[w][i].test1TruePosCount)/float(districts[w][i].test1PosCount);
                }
                myfile << "," << districts[w][i].test1NegCount;
                if (districts[w][i].test1NegCount < 1) {
                    myfile << "," << "0";
                }
                else {
                    myfile << "," << float (districts[w][i].test1TrueNegCount)/float(districts[w][i].test1NegCount);
                }
                if(debug2) myfile << "," << "marker";
               
                myfile << "," << districts[w][i].countP << "," << districts[w][i].countPI << "," << districts[w][i].countPU  << "," << districts[w][i].countPT;
                myfile << "," << districts[w][i].sampleWaitTestCount << "," << districts[w][i].sampleWaitTestCountIn << "," << districts[w][i].sampleWaitTestCountOut;
                myfile << "," << districts[w][i].countPreTestLtoFOutTested << "," << districts[w][i].patientNotNotified << "," << districts[w][i].patientNotNotifiedPos << "\n";

                myfile7 << districts[w][i].name << "," << districts[w][i].id << "," << districts[w][i].tier << "," << districts[w][i].district << "," << districts[w][i].region << "," << newPop << "," << diseaseList[w] << "," << prevalence;
                myfile7 << "," << districts[w][i].countP << "," << districts[w][i].countPInf << "," << districts[w][i].countPLtoF;
                myfile7 << "," << districts[w][i].outCollectedCount + districts[w][i].inCollectedCount << "," << districts[w][i].outCollectedCount << "," << districts[w][i].inCollectedCount;
                if ((districts[w][i].sampleWaitForResourceInCount + districts[w][i].sampleWaitForResourceOutCount) < 1) {
                    myfile7 << "," << "0";
                }
                else {
                    myfile7 << "," << float(districts[w][i].sampleWaitForResourceInTime + districts[w][i].sampleWaitForResourceOutTime)/float(districts[w][i].sampleWaitForResourceInCount + districts[w][i].sampleWaitForResourceOutCount);
                }
                if (districts[w][i].sampleWaitForResourceInCount < 1) {
                    myfile7 << "," << "0";
                }
                else {
                    myfile7 << "," << float(districts[w][i].sampleWaitForResourceInTime)/float(districts[w][i].sampleWaitForResourceInCount);
                }
                if (districts[w][i].sampleWaitForResourceOutCount < 1) {
                    myfile7 << "," << "0" << ",";
                }
                else {
                    myfile7 << "," << float(districts[w][i].sampleWaitForResourceOutTime)/float(districts[w][i].sampleWaitForResourceOutCount) << ",";
                }
                if (districts[w][i].sampleTransCount  < 1) {
                    myfile7 << "0" << "," << districts[w][i].sampleTransCount;
                }
                else {
                    myfile7 << float(districts[w][i].sampleTransTime)/float(districts[w][i].sampleTransCount) << "," << districts[w][i].sampleTransCount;
                }
                myfile7 << "," << districts[w][i].sampleTestedInCount +  districts[w][i].sampleTestedOutCount << "," << districts[w][i].sampleTestedInCount << "," << districts[w][i].sampleTestedOutCount;
                //myfile7 << "," << districts[w][i].sampleLtoFCount;
                if (districts[w][i].sampleCollectToTestInCount < 1) {
                    myfile7 << "," << "0";
                }
                else {
                    myfile7 << "," << float(districts[w][i].sampleCollectToTestInTime)/float(districts[w][i].sampleCollectToTestInCount);
                }
                myfile7 << "," << float(districts[w][i].sampleCollectToTestInTime) << "," << districts[w][i].sampleCollectToTestInCount;
                if (districts[w][i].sampleCollectToTestOutCount < 1) {
                    myfile7 << "," << "0";
                }
                else {
                    myfile7 << "," << float(districts[w][i].sampleCollectToTestOutTime)/float(districts[w][i].sampleCollectToTestOutCount);
                }
                myfile7 << "," << float(districts[w][i].sampleCollectToTestOutTime) << "," << districts[w][i].sampleCollectToTestOutCount;
                 myfile7 << "," << districts[w][i].countTestPos << ",";
                if (totalSampleCountPos > 0) {
                    myfile7 << float(districts[w][i].countTruePos)/float(totalSampleCountPos) << ",";
                    myfile7 << float(districts[w][i].countFalsePos)/float(totalSampleCountPos) << ",";
                }
                else {
                    myfile7 << "0" << "," << "0" << ",";
                }
                myfile7 << districts[w][i].countTestNeg << ",";
                if (totalSampleCountNeg > 0) {
                    myfile7 << float(districts[w][i].countTrueNeg)/float(totalSampleCountNeg) << ",";
                    myfile7 << float(districts[w][i].countFalseNeg)/float(totalSampleCountNeg);
                }
                else {
                    myfile7 << "0" << "," << "0";
                }
                if (debug2) myfile7 << "," << "marker";

                myfile7 << "," << districts[w][i].countMDInf << "," << districts[w][i].countMDUninf;
                myfile7 << "," << districts[w][i].countMDInf + districts[w][i].countMDUninf + districts[w][i].countTestPos;
                myfile7 << "," << districts[w][i].countFalsePos << "," << districts[w][i].countFalseNeg;
                myfile7 << "," << districts[w][i].countDiagnosis << "," << districts[w][i].countDiagnosisIn << "," << districts[w][i].countDiagnosisOut << "," << districts[w][i].countPreTestLtoFOut << "," << districts[w][i].countPreTestLtoFOutMinusTestedSamp << "," << districts[w][i].countPreTestLtoFDead << "," << districts[w][i].countPreTestLtoFORepresent;
                if (districts[w][i].sampleTestPosCountIn < 1) {
                    myfile7 << "," << "0";
                }
                else {
                    myfile7 << "," << float(districts[w][i].sampleTestPosTimeIn)/float(districts[w][i].sampleTestPosCountIn);
                }
                myfile7 << "," << float(districts[w][i].sampleTestPosTimeIn) << "," << districts[w][i].sampleTestPosCountIn;
                if (districts[w][i].sampleTestPosCountOut < 1) {
                    myfile7 << "," << "0";
                }
                else {
                    myfile7 << "," << float(districts[w][i].sampleTestPosTimeOut)/float(districts[w][i].sampleTestPosCountOut);
                }
                myfile7 << "," << float(districts[w][i].sampleTestPosTimeOut) << "," << districts[w][i].sampleTestPosCountOut;
                if(debug2) myfile7 << "," << "marker";
               
                myfile7 << "," << districts[w][i].countPInfTreated+districts[w][i].countPUninfTreated << "," << districts[w][i].countPInfTreated << "," << districts[w][i].countPUninfTreated << "," << districts[w][i].countPTC << "," << districts[w][i].countPTD << "," << districts[w][i].countPTR  << "," << districts[w][i].countRetreat << "," << districts[w][i].countRetreatCure << "," << districts[w][i].countRetreatDead;
                if ((districts[w][i].countPInfTreatedIn + districts[w][i].countPInfTreatedOut) < 1) {
                    myfile7 << "," << "0";
                }
                else {
                    myfile7 << "," << float (districts[w][i].onsetToTreat)/float(districts[w][i].countPInfTreatedIn + districts[w][i].countPInfTreatedOut);
                }
                myfile7 << "," << float (districts[w][i].onsetToTreat) << "," << (districts[w][i].countPInfTreatedIn + districts[w][i].countPInfTreatedOut);
                if (districts[w][i].countPInfTreatedIn < 1) {
                    myfile7 << "," << "0";
                }
                else {
                    myfile7 << "," << float (districts[w][i].onsetToTreatIn)/float(districts[w][i].countPInfTreatedIn);
                }
                myfile7 << "," << float (districts[w][i].onsetToTreatIn) << "," << districts[w][i].countPInfTreatedIn;
                if (districts[w][i].countPInfTreatedOut < 1) {
                    myfile7 << "," << "0";
                }
                else {
                    myfile7 << "," << float (districts[w][i].onsetToTreatOut)/float(districts[w][i].countPInfTreatedOut);
                }
                myfile7 << "," << float (districts[w][i].onsetToTreatOut) << "," << districts[w][i].countPInfTreatedOut;
                if(debug2) myfile7 <<  "," << "marker";
               
                if (districts[w][i].sampleReturnToDeathCount < 1) {
                    myfile7 << "," << "0";
                }
                else {
                    myfile7 << "," << float (districts[w][i].sampleReturnToDeathTime)/float(districts[w][i].sampleReturnToDeathCount);
                }
                myfile7 << "," << float (districts[w][i].sampleReturnToDeathTime) << "," << districts[w][i].sampleReturnToDeathCount;
                if (districts[w][i].sampleReturnToDeathPreDiagLtoFCount < 1) {
                    myfile7 << "," << "0";
                }
                else {
                    myfile7 << "," << float (districts[w][i].sampleReturnToDeathPreDiagLtoFTime)/float(districts[w][i].sampleReturnToDeathPreDiagLtoFCount);
                }
                myfile7 << "," << float (districts[w][i].sampleReturnToDeathPreDiagLtoFTime) << "," << districts[w][i].sampleReturnToDeathPreDiagLtoFCount;
                if (districts[w][i].sampleReturnToDeathInTrtCount < 1) {
                    myfile7 << "," << "0";
                }
                else {
                    myfile7 << "," << float (districts[w][i].sampleReturnToDeathInTrtTime)/float(districts[w][i].sampleReturnToDeathInTrtCount);
                }
                myfile7 << "," << float (districts[w][i].sampleReturnToDeathInTrtTime) << "," << districts[w][i].sampleReturnToDeathInTrtCount;
                if (districts[w][i].sampleReturnToDeathInRetrtCount < 1) {
                    myfile7 << "," << "0";
                }
                else {
                    myfile7 << "," << float (districts[w][i].sampleReturnToDeathInRetrtTime)/float(districts[w][i].sampleReturnToDeathInRetrtCount);
                }
                myfile7 << "," << float (districts[w][i].sampleReturnToDeathInRetrtTime) << "," << districts[w][i].sampleReturnToDeathInRetrtCount;
                if (districts[w][i].sampleReturnToExitCount < 1) {
                    myfile7 << "," << "0";
                }
                else {
                    myfile7 << "," << float (districts[w][i].sampleReturnToExitTime)/float(districts[w][i].sampleReturnToExitCount);
                }
                myfile7 << "," << float (districts[w][i].sampleReturnToExitTime) << "," << districts[w][i].sampleReturnToExitCount;
                myfile7 << "," << districts[w][i].test1CountIn + districts[w][i].test1CountOut << "," << districts[w][i].test1CountIn << "," << districts[w][i].test1CountOut << "," << districts[w][i].test2Count << "," << districts[w][i]. test2PosCount;
                if (districts[w][i].test2PosCount < 1) {
                    myfile7 << "," << "0";
                }
                else {
                    myfile7 << "," << float (districts[w][i].test2TruePosCount)/float(districts[w][i].test2PosCount);
                }
                myfile7 << "," << districts[w][i].test2NegCount;
                if (districts[w][i].test2NegCount < 1) {
                    myfile7 << "," << "0";
                }
                else {
                    myfile7 << "," << float (districts[w][i].test2TrueNegCount)/float(districts[w][i].test2NegCount);
                }
                myfile7 << "," << districts[w][i].test1PosCount;
                if (districts[w][i].test1PosCount < 1) {
                    myfile7 << "," << "0";
                }
                else {
                    myfile7 << "," << float (districts[w][i].test1TruePosCount)/float(districts[w][i].test1PosCount);
                }
                myfile7 << "," << districts[w][i].test1NegCount;
                if (districts[w][i].test1NegCount < 1) {
                    myfile7 << "," << "0";
                }
                else {
                    myfile7 << "," << float (districts[w][i].test1TrueNegCount)/float(districts[w][i].test1NegCount);
                }
                if(debug2) myfile7 << "," << "marker";
               
                myfile7 << "," << districts[w][i].countP << "," << districts[w][i].countPI << "," << districts[w][i].countPU  << "," << districts[w][i].countPT;
                myfile7 << "," << districts[w][i].sampleWaitTestCount << "," << districts[w][i].sampleWaitTestCountIn << "," << districts[w][i].sampleWaitTestCountOut;
                myfile7 << "," << districts[w][i].countPreTestLtoFOutTested << "," << districts[w][i].patientNotNotified << "," << districts[w][i].patientNotNotifiedPos << "\n";

                diseaseTotalDistrict[w].pop = diseaseTotalDistrict[w].pop + districts[w][i].pop;
                diseaseTotalDistrict[w].countP = diseaseTotalDistrict[w].countP + districts[w][i].countP;
                diseaseTotalDistrict[w].countPLtoF = diseaseTotalDistrict[w].countPLtoF + districts[w][i].countPLtoF;
                diseaseTotalDistrict[w].countPInf = diseaseTotalDistrict[w].countPInf + districts[w][i].countPInf;
                diseaseTotalDistrict[w].countPInfTreated = diseaseTotalDistrict[w].countPInfTreated + districts[w][i].countPInfTreated;
                diseaseTotalDistrict[w].countPInfTreatedIn = diseaseTotalDistrict[w].countPInfTreatedIn + districts[w][i].countPInfTreatedIn;
                diseaseTotalDistrict[w].countPInfTreatedOut = diseaseTotalDistrict[w].countPInfTreatedOut + districts[w][i].countPInfTreatedOut;
                diseaseTotalDistrict[w].countPUninfTreated = diseaseTotalDistrict[w].countPUninfTreated + districts[w][i].countPUninfTreated;
                diseaseTotalDistrict[w].countPU = diseaseTotalDistrict[w].countPU + districts[w][i].countPU;
                diseaseTotalDistrict[w].countPI = diseaseTotalDistrict[w].countPI + districts[w][i].countPI;
                diseaseTotalDistrict[w].countPT = diseaseTotalDistrict[w].countPT + districts[w][i].countPT;
                diseaseTotalDistrict[w].countPR = diseaseTotalDistrict[w].countPR + districts[w][i].countPR;
                diseaseTotalDistrict[w].countPUT = diseaseTotalDistrict[w].countPUT + districts[w][i].countPUT;
                diseaseTotalDistrict[w].countPUTFN = diseaseTotalDistrict[w].countPUTFN + districts[w][i].countPUTFN;
                diseaseTotalDistrict[w].countPUTNS = diseaseTotalDistrict[w].countPUTNS + districts[w][i].countPUTNS;
                diseaseTotalDistrict[w].countPUTLtoF = diseaseTotalDistrict[w].countPUTLtoF + districts[w][i].countPUTLtoF;
                diseaseTotalDistrict[w].countPC = diseaseTotalDistrict[w].countPC + districts[w][i].countPC;
                diseaseTotalDistrict[w].countPDI = diseaseTotalDistrict[w].countPDI + districts[w][i].countPDI;
                diseaseTotalDistrict[w].countPDIT = diseaseTotalDistrict[w].countPDIT + districts[w][i].countPDIT;
                diseaseTotalDistrict[w].countPDIUT = diseaseTotalDistrict[w].countPDIUT + districts[w][i].countPDIUT;
                diseaseTotalDistrict[w].countPDIUTSR = diseaseTotalDistrict[w].countPDIUTSR + districts[w][i].countPDIUTSR;
                diseaseTotalDistrict[w].countPDIUTSNR = diseaseTotalDistrict[w].countPDIUTSNR + districts[w][i].countPDIUTSNR;
                diseaseTotalDistrict[w].countPDIUTLtF = diseaseTotalDistrict[w].countPDIUTLtF + districts[w][i].countPDIUTLtF;
                diseaseTotalDistrict[w].countPDU = diseaseTotalDistrict[w].countPDU + districts[w][i].countPDU;
                diseaseTotalDistrict[w].countPDOA = diseaseTotalDistrict[w].countPDOA + districts[w][i].countPDOA;
                diseaseTotalDistrict[w].countPDIUTPTLtoF = diseaseTotalDistrict[w].countPDIUTPTLtoF + districts[w][i].countPDIUTPTLtoF;
                diseaseTotalDistrict[w].countPDIUTSRLtoF = diseaseTotalDistrict[w].countPDIUTSRLtoF + districts[w][i].countPDIUTSRLtoF;
                diseaseTotalDistrict[w].countPDITLtoF = diseaseTotalDistrict[w].countPDITLtoF + districts[w][i].countPDITLtoF;
                diseaseTotalDistrict[w].countPTC = diseaseTotalDistrict[w].countPTC + districts[w][i].countPTC;
                diseaseTotalDistrict[w].countPTD = diseaseTotalDistrict[w].countPTD + districts[w][i].countPTD;
                diseaseTotalDistrict[w].countPTR = diseaseTotalDistrict[w].countPTR + districts[w][i].countPTR;
                diseaseTotalDistrict[w].countRetreat = diseaseTotalDistrict[w].countRetreat + districts[w][i].countRetreat;
                diseaseTotalDistrict[w].countRetreatCure = diseaseTotalDistrict[w].countRetreatCure + districts[w][i].countRetreatCure;
                diseaseTotalDistrict[w].countRetreatDead = diseaseTotalDistrict[w].countRetreatDead + districts[w][i].countRetreatDead;
                diseaseTotalDistrict[w].countPO = diseaseTotalDistrict[w].countPO + districts[w][i].countPO;
                diseaseTotalDistrict[w].countPUI = diseaseTotalDistrict[w].countPUI + districts[w][i].countPUI;
                diseaseTotalDistrict[w].countPUINR = diseaseTotalDistrict[w].countPUINR + districts[w][i].countPUINR;
                diseaseTotalDistrict[w].countPUIUT = diseaseTotalDistrict[w].countPUIUT + districts[w][i].countPUIUT;
                diseaseTotalDistrict[w].countMDInf = diseaseTotalDistrict[w].countMDInf + districts[w][i].countMDInf;
                diseaseTotalDistrict[w].countMDUninf = diseaseTotalDistrict[w].countMDUninf + districts[w][i].countMDUninf;
                diseaseTotalDistrict[w].countTestPos = diseaseTotalDistrict[w].countTestPos + districts[w][i].countTestPos;
                diseaseTotalDistrict[w].countTestNeg = diseaseTotalDistrict[w].countTestNeg + districts[w][i].countTestNeg;
                diseaseTotalDistrict[w].countTruePos = diseaseTotalDistrict[w].countTruePos + districts[w][i].countTruePos;
                diseaseTotalDistrict[w].countFalsePos = diseaseTotalDistrict[w].countFalsePos + districts[w][i].countFalsePos;
                diseaseTotalDistrict[w].countTrueNeg = diseaseTotalDistrict[w].countTrueNeg + districts[w][i].countTrueNeg;
                diseaseTotalDistrict[w].countFalseNeg = diseaseTotalDistrict[w].countFalseNeg + districts[w][i].countFalseNeg;
                diseaseTotalDistrict[w].countDiagnosis = diseaseTotalDistrict[w].countDiagnosis + districts[w][i].countDiagnosis;
                diseaseTotalDistrict[w].countDiagnosisIn = diseaseTotalDistrict[w].countDiagnosisIn + districts[w][i].countDiagnosisIn;
                diseaseTotalDistrict[w].countDiagnosisOut = diseaseTotalDistrict[w].countDiagnosisOut + districts[w][i].countDiagnosisOut;
                diseaseTotalDistrict[w].countPreTestLtoFOut = diseaseTotalDistrict[w].countPreTestLtoFOut + districts[w][i].countPreTestLtoFOut;
                diseaseTotalDistrict[w].countPreTestLtoFOutTested = diseaseTotalDistrict[w].countPreTestLtoFOutTested + districts[w][i].countPreTestLtoFOutTested;
                diseaseTotalDistrict[w].countPreTestLtoFOutUnTested = diseaseTotalDistrict[w].countPreTestLtoFOutUnTested + districts[w][i].countPreTestLtoFOutUnTested;
                diseaseTotalDistrict[w].countPreTestLtoFOutUnTested2 = diseaseTotalDistrict[w].countPreTestLtoFOutUnTested2 + districts[w][i].countPreTestLtoFOutUnTested2;
                diseaseTotalDistrict[w].countPreTestLtoFDead = diseaseTotalDistrict[w].countPreTestLtoFDead + districts[w][i].countPreTestLtoFDead;
                diseaseTotalDistrict[w].countPreTestLtoFORepresent = diseaseTotalDistrict[w].countPreTestLtoFORepresent + districts[w][i].countPreTestLtoFORepresent;
                diseaseTotalDistrict[w].onsetToTreat = diseaseTotalDistrict[w].onsetToTreat + districts[w][i].onsetToTreat;
                diseaseTotalDistrict[w].onsetToTreatIn = diseaseTotalDistrict[w].onsetToTreatIn + districts[w][i].onsetToTreatIn;
                diseaseTotalDistrict[w].onsetToTreatOut = diseaseTotalDistrict[w].onsetToTreatOut + districts[w][i].onsetToTreatOut;
                diseaseTotalDistrict[w].timeInfectious = diseaseTotalDistrict[w].timeInfectious + districts[w][i].timeInfectious;
                diseaseTotalDistrict[w].sampleTransTime = diseaseTotalDistrict[w].sampleTransTime + districts[w][i].sampleTransTime;
                diseaseTotalDistrict[w].sampleTransCount = diseaseTotalDistrict[w].sampleTransCount + districts[w][i].sampleTransCount;
                diseaseTotalDistrict[w].sampleWaitForResourceInTime = diseaseTotalDistrict[w].sampleWaitForResourceInTime + districts[w][i].sampleWaitForResourceInTime;
                diseaseTotalDistrict[w].sampleWaitForResourceInCount = diseaseTotalDistrict[w].sampleWaitForResourceInCount + districts[w][i].sampleWaitForResourceInCount;
                diseaseTotalDistrict[w].sampleWaitForResourceOutTime = diseaseTotalDistrict[w].sampleWaitForResourceOutTime + districts[w][i].sampleWaitForResourceOutTime;
                diseaseTotalDistrict[w].sampleWaitForResourceOutCount = diseaseTotalDistrict[w].sampleWaitForResourceOutCount + districts[w][i].sampleWaitForResourceOutCount;
                diseaseTotalDistrict[w].sampleCollectToTestInTime = diseaseTotalDistrict[w].sampleCollectToTestInTime + districts[w][i].sampleCollectToTestInTime;
                diseaseTotalDistrict[w].sampleCollectToTestOutTime = diseaseTotalDistrict[w].sampleCollectToTestOutTime + districts[w][i].sampleCollectToTestOutTime;
                diseaseTotalDistrict[w].sampleCollectToTestInCount = diseaseTotalDistrict[w].sampleCollectToTestInCount + districts[w][i].sampleCollectToTestInCount;
                diseaseTotalDistrict[w].sampleCollectToTestOutCount = diseaseTotalDistrict[w].sampleCollectToTestOutCount + districts[w][i].sampleCollectToTestOutCount;
                diseaseTotalDistrict[w].sampleTestTime = diseaseTotalDistrict[w].sampleTestTime + districts[w][i].sampleTestTime;
                diseaseTotalDistrict[w].sampleTestPosTimeIn = diseaseTotalDistrict[w].sampleTestPosTimeIn + districts[w][i].sampleTestPosTimeIn;
                diseaseTotalDistrict[w].sampleTestPosTimeOut = diseaseTotalDistrict[w].sampleTestPosTimeOut + districts[w][i].sampleTestPosTimeOut;
                diseaseTotalDistrict[w].sampleTestPosCountIn = diseaseTotalDistrict[w].sampleTestPosCountIn + districts[w][i].sampleTestPosCountIn;
                diseaseTotalDistrict[w].sampleTestPosCountOut = diseaseTotalDistrict[w].sampleTestPosCountOut + districts[w][i].sampleTestPosCountOut;
                diseaseTotalDistrict[w].sampleReturnTime = diseaseTotalDistrict[w].sampleReturnTime + districts[w][i].sampleReturnTime;
                diseaseTotalDistrict[w].sampleReturnedCount = diseaseTotalDistrict[w].sampleReturnedCount + districts[w][i].sampleReturnedCount;
                diseaseTotalDistrict[w].sampleReturnedInCount = diseaseTotalDistrict[w].sampleReturnedInCount + districts[w][i].sampleReturnedInCount;
                diseaseTotalDistrict[w].sampleReturnedOutCount = diseaseTotalDistrict[w].sampleReturnedOutCount + districts[w][i].sampleReturnedOutCount;
                diseaseTotalDistrict[w].sampleTestedInCount = diseaseTotalDistrict[w].sampleTestedInCount + districts[w][i].sampleTestedInCount;
                diseaseTotalDistrict[w].sampleTestedOutCount = diseaseTotalDistrict[w].sampleTestedOutCount + districts[w][i].sampleTestedOutCount;
                diseaseTotalDistrict[w].sampleNotTestedInCount = diseaseTotalDistrict[w].sampleNotTestedInCount + districts[w][i].sampleNotTestedInCount;
                diseaseTotalDistrict[w].sampleNotTestedOutCount = diseaseTotalDistrict[w].sampleNotTestedOutCount + districts[w][i].sampleNotTestedOutCount;
                diseaseTotalDistrict[w].sampleReturnToDeathTime = diseaseTotalDistrict[w].sampleReturnToDeathTime + districts[w][i].sampleReturnToDeathTime;
                diseaseTotalDistrict[w].sampleReturnToDeathPreDiagLtoFTime = diseaseTotalDistrict[w].sampleReturnToDeathPreDiagLtoFTime + districts[w][i].sampleReturnToDeathPreDiagLtoFTime;
                diseaseTotalDistrict[w].sampleReturnToDeathInTrtTime = diseaseTotalDistrict[w].sampleReturnToDeathInTrtTime + districts[w][i].sampleReturnToDeathInTrtTime;
                diseaseTotalDistrict[w].sampleReturnToDeathInRetrtTime = diseaseTotalDistrict[w].sampleReturnToDeathInRetrtTime + districts[w][i].sampleReturnToDeathInRetrtTime;
                diseaseTotalDistrict[w].sampleReturnToExitTime = diseaseTotalDistrict[w].sampleReturnToExitTime + districts[w][i].sampleReturnToExitTime;
                diseaseTotalDistrict[w].sampleReturnToDeathCount = diseaseTotalDistrict[w].sampleReturnToDeathCount + districts[w][i].sampleReturnToDeathCount;
                diseaseTotalDistrict[w].sampleReturnToDeathPreDiagLtoFCount = diseaseTotalDistrict[w].sampleReturnToDeathPreDiagLtoFCount + districts[w][i].sampleReturnToDeathPreDiagLtoFCount;
                diseaseTotalDistrict[w].sampleReturnToDeathInTrtCount = diseaseTotalDistrict[w].sampleReturnToDeathInTrtCount + districts[w][i].sampleReturnToDeathInTrtCount;
                diseaseTotalDistrict[w].sampleReturnToDeathInRetrtCount = diseaseTotalDistrict[w].sampleReturnToDeathInRetrtCount + districts[w][i].sampleReturnToDeathInRetrtCount;
                diseaseTotalDistrict[w].sampleReturnToExitCount = diseaseTotalDistrict[w].sampleReturnToExitCount + districts[w][i].sampleReturnToExitCount;
                diseaseTotalDistrict[w].sampleLtoFCount = diseaseTotalDistrict[w].sampleLtoFCount + districts[w][i].sampleLtoFCount;
                diseaseTotalDistrict[w].sampleWaitTestCount = diseaseTotalDistrict[w].sampleWaitTestCount + districts[w][i].sampleWaitTestCount;
                diseaseTotalDistrict[w].sampleWaitTestCountIn = diseaseTotalDistrict[w].sampleWaitTestCountIn + districts[w][i].sampleWaitTestCountIn;
                diseaseTotalDistrict[w].sampleWaitTestCountOut = diseaseTotalDistrict[w].sampleWaitTestCountOut + districts[w][i].sampleWaitTestCountOut;
                diseaseTotalDistrict[w].patientNotNotified = diseaseTotalDistrict[w].patientNotNotified + districts[w][i].patientNotNotified;
                diseaseTotalDistrict[w].patientNotNotifiedPos = diseaseTotalDistrict[w].patientNotNotifiedPos + districts[w][i].patientNotNotifiedPos;
                diseaseTotalDistrict[w].patientNotNotifiedIn = diseaseTotalDistrict[w].patientNotNotifiedIn + districts[w][i].patientNotNotifiedIn;
                diseaseTotalDistrict[w].patientNotNotifiedOut = diseaseTotalDistrict[w].patientNotNotifiedOut + districts[w][i].patientNotNotifiedOut;
                diseaseTotalDistrict[w].inTestCount = diseaseTotalDistrict[w].inTestCount + districts[w][i].inTestCount;
                diseaseTotalDistrict[w].outTestCount = diseaseTotalDistrict[w].outTestCount + districts[w][i].outTestCount;
                diseaseTotalDistrict[w].inCollectedCount = diseaseTotalDistrict[w].inCollectedCount + districts[w][i].inCollectedCount;
                diseaseTotalDistrict[w].outCollectedCount = diseaseTotalDistrict[w].outCollectedCount + districts[w][i].outCollectedCount;
                diseaseTotalDistrict[w].test1Count = diseaseTotalDistrict[w].test1Count + districts[w][i].test1Count;
                diseaseTotalDistrict[w].test1CountIn = diseaseTotalDistrict[w].test1CountIn + districts[w][i].test1CountIn;
                diseaseTotalDistrict[w].test1CountOut = diseaseTotalDistrict[w].test1CountOut + districts[w][i].test1CountOut;
                diseaseTotalDistrict[w].test1PosCount = diseaseTotalDistrict[w].test1PosCount + districts[w][i].test1PosCount;
                diseaseTotalDistrict[w].test1TruePosCount = diseaseTotalDistrict[w].test1TruePosCount + districts[w][i].test1TruePosCount;
                diseaseTotalDistrict[w].test1NegCount = diseaseTotalDistrict[w].test1NegCount + districts[w][i].test1NegCount;
                diseaseTotalDistrict[w].test1TrueNegCount = diseaseTotalDistrict[w].test1TrueNegCount + districts[w][i].test1TrueNegCount;
                diseaseTotalDistrict[w].test2Count = diseaseTotalDistrict[w].test2Count + districts[w][i].test2Count;
                diseaseTotalDistrict[w].test2CountIn = diseaseTotalDistrict[w].test2CountIn + districts[w][i].test2CountIn;
                diseaseTotalDistrict[w].test2CountOut = diseaseTotalDistrict[w].test2CountOut + districts[w][i].test2CountOut;
                diseaseTotalDistrict[w].test2PosCount = diseaseTotalDistrict[w].test2PosCount + districts[w][i].test2PosCount;
                diseaseTotalDistrict[w].test2TruePosCount = diseaseTotalDistrict[w].test2TruePosCount + districts[w][i].test2TruePosCount;
                diseaseTotalDistrict[w].test2NegCount = diseaseTotalDistrict[w].test2NegCount + districts[w][i].test2NegCount;
                diseaseTotalDistrict[w].test2TrueNegCount = diseaseTotalDistrict[w].test2TrueNegCount + districts[w][i].test2TrueNegCount;
                diseaseTotalDistrict[w].test3Count = diseaseTotalDistrict[w].test3Count + districts[w][i].test3Count;
                if (diseaseTotalDistrict[w].test1Name == "") {

                    diseaseTotalDistrict[w].test1Name = districts[w][i].test1Name;
                }
                if (diseaseTotalDistrict[w].test2Name == "") {

                    diseaseTotalDistrict[w].test2Name = districts[w][i].test2Name;
                }
                if (diseaseTotalDistrict[w].test3Name == "") {

                    diseaseTotalDistrict[w].test3Name = districts[w][i].test3Name;
                }
                diseaseTotalDistrict[w].countPreTestLtoFOutMinusTestedSamp = diseaseTotalDistrict[w].countPreTestLtoFOutMinusTestedSamp + districts[w][i].countPreTestLtoFOutMinusTestedSamp;
                diseaseTotalDistrict[w].dummy2Count = diseaseTotalDistrict[w].dummy2Count + districts[w][i].dummy2Count;
                diseaseTotalDistrict[w].dummy3Count = diseaseTotalDistrict[w].dummy3Count + districts[w][i].dummy3Count;
                diseaseTotalDistrict[w].dummy4Count = diseaseTotalDistrict[w].dummy4Count + districts[w][i].dummy4Count;
                diseaseTotalDistrict[w].dummy5Count = diseaseTotalDistrict[w].dummy5Count + districts[w][i].dummy5Count;
                diseaseTotalDistrict[w].dummy6Count = diseaseTotalDistrict[w].dummy6Count + districts[w][i].dummy6Count;
                
                totalDistrict.pop = totalDistrict.pop + districts[w][i].pop;
                totalDistrict.countP = totalDistrict.countP + districts[w][i].countP;
                totalDistrict.countPLtoF = totalDistrict.countPLtoF + districts[w][i].countPLtoF;
                totalDistrict.countPInf = totalDistrict.countPInf + districts[w][i].countPInf;
                totalDistrict.countPInfTreated = totalDistrict.countPInfTreated + districts[w][i].countPInfTreated;
                totalDistrict.countPInfTreatedIn = totalDistrict.countPInfTreatedIn + districts[w][i].countPInfTreatedIn;
                totalDistrict.countPInfTreatedOut = totalDistrict.countPInfTreatedOut + districts[w][i].countPInfTreatedOut;
                totalDistrict.countPUninfTreated = totalDistrict.countPUninfTreated + districts[w][i].countPUninfTreated;
                totalDistrict.countPU = totalDistrict.countPU + districts[w][i].countPU;
                totalDistrict.countPI = totalDistrict.countPI + districts[w][i].countPI;
                totalDistrict.countPT = totalDistrict.countPT + districts[w][i].countPT;
                totalDistrict.countPR = totalDistrict.countPR + districts[w][i].countPR;
                totalDistrict.countPUT = totalDistrict.countPUT + districts[w][i].countPUT;
                totalDistrict.countPUTFN = totalDistrict.countPUTFN + districts[w][i].countPUTFN;
                totalDistrict.countPUTNS = totalDistrict.countPUTNS + districts[w][i].countPUTNS;
                totalDistrict.countPUTLtoF = totalDistrict.countPUTLtoF + districts[w][i].countPUTLtoF;
                totalDistrict.countPC = totalDistrict.countPC + districts[w][i].countPC;
                totalDistrict.countPDI = totalDistrict.countPDI + districts[w][i].countPDI;
                totalDistrict.countPDIT = totalDistrict.countPDIT + districts[w][i].countPDIT;
                totalDistrict.countPDIUT = totalDistrict.countPDIUT + districts[w][i].countPDIUT;
                totalDistrict.countPDIUTSR = totalDistrict.countPDIUTSR + districts[w][i].countPDIUTSR;
                totalDistrict.countPDIUTSNR = totalDistrict.countPDIUTSNR + districts[w][i].countPDIUTSNR;
                totalDistrict.countPDIUTLtF = totalDistrict.countPDIUTLtF + districts[w][i].countPDIUTLtF;
                totalDistrict.countPDU = totalDistrict.countPDU + districts[w][i].countPDU;
                totalDistrict.countPDOA = totalDistrict.countPDOA + districts[w][i].countPDOA;
                totalDistrict.countPDIUTPTLtoF = totalDistrict.countPDIUTPTLtoF + districts[w][i].countPDIUTPTLtoF;
                totalDistrict.countPDIUTSRLtoF = totalDistrict.countPDIUTSRLtoF + districts[w][i].countPDIUTSRLtoF;
                totalDistrict.countPDITLtoF = totalDistrict.countPDITLtoF + districts[w][i].countPDITLtoF;
                totalDistrict.countPTC = totalDistrict.countPTC + districts[w][i].countPTC;
                totalDistrict.countPTD = totalDistrict.countPTD + districts[w][i].countPTD;
                totalDistrict.countPTR = totalDistrict.countPTR + districts[w][i].countPTR;
                totalDistrict.countRetreat = totalDistrict.countRetreat + districts[w][i].countRetreat;
                totalDistrict.countRetreatCure = totalDistrict.countRetreatCure + districts[w][i].countRetreatCure;
                totalDistrict.countRetreatDead = totalDistrict.countRetreatDead + districts[w][i].countRetreatDead;
                totalDistrict.countPO = totalDistrict.countPO + districts[w][i].countPO;
                totalDistrict.countPUI = totalDistrict.countPUI + districts[w][i].countPUI;
                totalDistrict.countPUINR = totalDistrict.countPUINR + districts[w][i].countPUINR;
                totalDistrict.countPUIUT = totalDistrict.countPUIUT + districts[w][i].countPUIUT;
                totalDistrict.countMDInf = totalDistrict.countMDInf + districts[w][i].countMDInf;
                totalDistrict.countMDUninf = totalDistrict.countMDUninf + districts[w][i].countMDUninf;
                totalDistrict.countTestPos = totalDistrict.countTestPos + districts[w][i].countTestPos;
                totalDistrict.countTestNeg = totalDistrict.countTestNeg + districts[w][i].countTestNeg;
                totalDistrict.countTruePos = totalDistrict.countTruePos + districts[w][i].countTruePos;
                totalDistrict.countFalsePos = totalDistrict.countFalsePos + districts[w][i].countFalsePos;
                totalDistrict.countTrueNeg = totalDistrict.countTrueNeg + districts[w][i].countTrueNeg;
                totalDistrict.countFalseNeg = totalDistrict.countFalseNeg + districts[w][i].countFalseNeg;
                totalDistrict.countDiagnosis = totalDistrict.countDiagnosis + districts[w][i].countDiagnosis;
                totalDistrict.countDiagnosisIn = totalDistrict.countDiagnosisIn + districts[w][i].countDiagnosisIn;
                totalDistrict.countDiagnosisOut = totalDistrict.countDiagnosisOut + districts[w][i].countDiagnosisOut;
                totalDistrict.countPreTestLtoFOut = totalDistrict.countPreTestLtoFOut + districts[w][i].countPreTestLtoFOut;
                totalDistrict.countPreTestLtoFOutTested = totalDistrict.countPreTestLtoFOutTested + districts[w][i].countPreTestLtoFOutTested;
                totalDistrict.countPreTestLtoFOutUnTested = totalDistrict.countPreTestLtoFOutUnTested + districts[w][i].countPreTestLtoFOutUnTested;
                totalDistrict.countPreTestLtoFOutUnTested2 = totalDistrict.countPreTestLtoFOutUnTested2 + districts[w][i].countPreTestLtoFOutUnTested2;
                totalDistrict.countPreTestLtoFDead = totalDistrict.countPreTestLtoFDead + districts[w][i].countPreTestLtoFDead;
                totalDistrict.countPreTestLtoFORepresent = totalDistrict.countPreTestLtoFORepresent + districts[w][i].countPreTestLtoFORepresent;
                totalDistrict.onsetToTreat = totalDistrict.onsetToTreat + districts[w][i].onsetToTreat;
                totalDistrict.onsetToTreatIn = totalDistrict.onsetToTreatIn + districts[w][i].onsetToTreatIn;
                totalDistrict.onsetToTreatOut = totalDistrict.onsetToTreatOut + districts[w][i].onsetToTreatOut;
                totalDistrict.timeInfectious = totalDistrict.timeInfectious + districts[w][i].timeInfectious;
                totalDistrict.sampleTransTime = totalDistrict.sampleTransTime + districts[w][i].sampleTransTime;
                totalDistrict.sampleTransCount = totalDistrict.sampleTransCount + districts[w][i].sampleTransCount;
                totalDistrict.sampleWaitForResourceInTime = totalDistrict.sampleWaitForResourceInTime + districts[w][i].sampleWaitForResourceInTime;
                totalDistrict.sampleWaitForResourceInCount = totalDistrict.sampleWaitForResourceInCount + districts[w][i].sampleWaitForResourceInCount;
                totalDistrict.sampleWaitForResourceOutTime = totalDistrict.sampleWaitForResourceOutTime + districts[w][i].sampleWaitForResourceOutTime;
                totalDistrict.sampleWaitForResourceOutCount = totalDistrict.sampleWaitForResourceOutCount + districts[w][i].sampleWaitForResourceOutCount;
                totalDistrict.sampleCollectToTestInTime = totalDistrict.sampleCollectToTestInTime + districts[w][i].sampleCollectToTestInTime;
                totalDistrict.sampleCollectToTestOutTime = totalDistrict.sampleCollectToTestOutTime + districts[w][i].sampleCollectToTestOutTime;
                totalDistrict.sampleCollectToTestInCount = totalDistrict.sampleCollectToTestInCount + districts[w][i].sampleCollectToTestInCount;
                totalDistrict.sampleCollectToTestOutCount = totalDistrict.sampleCollectToTestOutCount + districts[w][i].sampleCollectToTestOutCount;
                totalDistrict.sampleTestTime = totalDistrict.sampleTestTime + districts[w][i].sampleTestTime;
                totalDistrict.sampleTestPosTimeIn = totalDistrict.sampleTestPosTimeIn + districts[w][i].sampleTestPosTimeIn;
                totalDistrict.sampleTestPosTimeOut = totalDistrict.sampleTestPosTimeOut + districts[w][i].sampleTestPosTimeOut;
                totalDistrict.sampleTestPosCountIn = totalDistrict.sampleTestPosCountIn + districts[w][i].sampleTestPosCountIn;
                totalDistrict.sampleTestPosCountOut = totalDistrict.sampleTestPosCountOut + districts[w][i].sampleTestPosCountOut;
                totalDistrict.sampleReturnTime = totalDistrict.sampleReturnTime + districts[w][i].sampleReturnTime;
                totalDistrict.sampleReturnedCount = totalDistrict.sampleReturnedCount + districts[w][i].sampleReturnedCount;
                totalDistrict.sampleReturnedInCount = totalDistrict.sampleReturnedInCount + districts[w][i].sampleReturnedInCount;
                totalDistrict.sampleReturnedOutCount = totalDistrict.sampleReturnedOutCount + districts[w][i].sampleReturnedOutCount;
                totalDistrict.sampleTestedInCount = totalDistrict.sampleTestedInCount + districts[w][i].sampleTestedInCount;
                totalDistrict.sampleTestedOutCount = totalDistrict.sampleTestedOutCount + districts[w][i].sampleTestedOutCount;
                totalDistrict.sampleNotTestedInCount = totalDistrict.sampleNotTestedInCount + districts[w][i].sampleNotTestedInCount;
                totalDistrict.sampleNotTestedOutCount = totalDistrict.sampleNotTestedOutCount + districts[w][i].sampleNotTestedOutCount;
                totalDistrict.sampleReturnToDeathTime = totalDistrict.sampleReturnToDeathTime + districts[w][i].sampleReturnToDeathTime;
                totalDistrict.sampleReturnToDeathPreDiagLtoFTime = totalDistrict.sampleReturnToDeathPreDiagLtoFTime + districts[w][i].sampleReturnToDeathPreDiagLtoFTime;
                totalDistrict.sampleReturnToDeathInTrtTime = totalDistrict.sampleReturnToDeathInTrtTime + districts[w][i].sampleReturnToDeathInTrtTime;
                totalDistrict.sampleReturnToDeathInRetrtTime = totalDistrict.sampleReturnToDeathInRetrtTime + districts[w][i].sampleReturnToDeathInRetrtTime;
                totalDistrict.sampleReturnToExitTime = totalDistrict.sampleReturnToExitTime + districts[w][i].sampleReturnToExitTime;
                totalDistrict.sampleReturnToDeathCount = totalDistrict.sampleReturnToDeathCount + districts[w][i].sampleReturnToDeathCount;
                totalDistrict.sampleReturnToDeathPreDiagLtoFCount = totalDistrict.sampleReturnToDeathPreDiagLtoFCount + districts[w][i].sampleReturnToDeathPreDiagLtoFCount;
                totalDistrict.sampleReturnToDeathInTrtCount = totalDistrict.sampleReturnToDeathInTrtCount + districts[w][i].sampleReturnToDeathInTrtCount;
                totalDistrict.sampleReturnToDeathInRetrtCount = totalDistrict.sampleReturnToDeathInRetrtCount + districts[w][i].sampleReturnToDeathInRetrtCount;
                totalDistrict.sampleReturnToExitCount = totalDistrict.sampleReturnToExitCount + districts[w][i].sampleReturnToExitCount;
                totalDistrict.sampleLtoFCount = totalDistrict.sampleLtoFCount + districts[w][i].sampleLtoFCount;
                totalDistrict.sampleWaitTestCount = totalDistrict.sampleWaitTestCount + districts[w][i].sampleWaitTestCount;
                totalDistrict.sampleWaitTestCountIn = totalDistrict.sampleWaitTestCountIn + districts[w][i].sampleWaitTestCountIn;
                totalDistrict.sampleWaitTestCountOut = totalDistrict.sampleWaitTestCountOut + districts[w][i].sampleWaitTestCountOut;
                totalDistrict.patientNotNotified = totalDistrict.patientNotNotified + districts[w][i].patientNotNotified;
                totalDistrict.patientNotNotifiedPos = totalDistrict.patientNotNotifiedPos + districts[w][i].patientNotNotifiedPos;
                totalDistrict.patientNotNotifiedIn = totalDistrict.patientNotNotifiedIn + districts[w][i].patientNotNotifiedIn;
                totalDistrict.patientNotNotifiedOut = totalDistrict.patientNotNotifiedOut + districts[w][i].patientNotNotifiedOut;
                totalDistrict.inTestCount = totalDistrict.inTestCount + districts[w][i].inTestCount;
                totalDistrict.outTestCount = totalDistrict.outTestCount + districts[w][i].outTestCount;
                totalDistrict.inCollectedCount = totalDistrict.inCollectedCount + districts[w][i].inCollectedCount;
                totalDistrict.outCollectedCount = totalDistrict.outCollectedCount + districts[w][i].outCollectedCount;
                totalDistrict.test1Count = totalDistrict.test1Count + districts[w][i].test1Count;
                totalDistrict.test1CountIn = totalDistrict.test1CountIn + districts[w][i].test1CountIn;
                totalDistrict.test1CountOut = totalDistrict.test1CountOut + districts[w][i].test1CountOut;
                totalDistrict.test1PosCount = totalDistrict.test1PosCount + districts[w][i].test1PosCount;
                totalDistrict.test1TruePosCount = totalDistrict.test1TruePosCount + districts[w][i].test1TruePosCount;
                totalDistrict.test1NegCount = totalDistrict.test1NegCount + districts[w][i].test1NegCount;
                totalDistrict.test1TrueNegCount = totalDistrict.test1TrueNegCount + districts[w][i].test1TrueNegCount;
                totalDistrict.test2Count = totalDistrict.test2Count + districts[w][i].test2Count;
                totalDistrict.test2CountIn = totalDistrict.test2CountIn + districts[w][i].test2CountIn;
                totalDistrict.test2CountOut = totalDistrict.test2CountOut + districts[w][i].test2CountOut;
                totalDistrict.test2PosCount = totalDistrict.test2PosCount + districts[w][i].test2PosCount;
                totalDistrict.test2TruePosCount = totalDistrict.test2TruePosCount + districts[w][i].test2TruePosCount;
                totalDistrict.test2NegCount = totalDistrict.test2NegCount + districts[w][i].test2NegCount;
                totalDistrict.test2TrueNegCount = totalDistrict.test2TrueNegCount + districts[w][i].test2TrueNegCount;
                totalDistrict.test3Count = totalDistrict.test3Count + districts[w][i].test3Count;
                if (totalDistrict.test1Name == "") {

                    totalDistrict.test1Name = districts[w][i].test1Name;
                }
                if (totalDistrict.test2Name == "") {

                    totalDistrict.test2Name = districts[w][i].test2Name;
                }
                if (totalDistrict.test3Name == "") {

                    totalDistrict.test3Name = districts[w][i].test3Name;
                }
                totalDistrict.countPreTestLtoFOutMinusTestedSamp = totalDistrict.countPreTestLtoFOutMinusTestedSamp + districts[w][i].countPreTestLtoFOutMinusTestedSamp;
                totalDistrict.dummy2Count = totalDistrict.dummy2Count + districts[w][i].dummy2Count;
                totalDistrict.dummy3Count = totalDistrict.dummy3Count + districts[w][i].dummy3Count;
                totalDistrict.dummy4Count = totalDistrict.dummy4Count + districts[w][i].dummy4Count;
                totalDistrict.dummy5Count = totalDistrict.dummy5Count + districts[w][i].dummy5Count;
                totalDistrict.dummy6Count = totalDistrict.dummy6Count + districts[w][i].dummy6Count;
                

                if (individualFlag) {

                    myfile9 << "-----------------------------------------------------------------------------------  " << "\n";
                    myfile9 << "stats.District" <<  ",population" << ",patients" <<  "\n";
                    myfile9 <<  districts[w][i].name << "," << districts[w][i].pop << "," << districts[w][i].countP << "," << "\n";
                    myfile9 << "," << "status" << ",disease" << ",age" << ",alive" << ",infected" << ",test.pos" << ",md.override" << ",time.from.onset" << ",time.in.treatment" << ",time.not.in.treatment" << "\n";
  
                    myfile3 << "-----------------------------------------------------------------------------------  " << "\n";
                    myfile3 << "stats.District" <<  ",population" << ",patients" <<  "\n";
                    myfile3 <<  districts[w][i].name << "," << districts[w][i].pop << "," << districts[w][i].countP << "\n";
                    myfile3 << "," << "status" << ",disease" << ",age" << ",alive" << ",infected" << ",test.pos" << ",md.override" << ",time.from.onset" << ",time.in.treatment" << ",time.not.in.treatment" << "\n";
  
                    // admin file - adjust population for population growth each year
                    newPop = districts[w][i].pop;
                    startIndex = referenceYear - 2015;
                    endIndex = startIndex + noOfYears;
                    if (noOfYears > 0) {

                        for (int i = startIndex; i < endIndex; i++) {

                            // multiply by growth rate for x number of years
                            newPop = (int) (newPop * popGrowthRateArray[i][1]);
                        }
                    }
                    myfile11 << "district" << "," << districts[w][i].name << "," << newPop << "," << districts[w][i].countP << "," << "\n";


                    patReturnedCount = 0;
            
                    // dump all patients in this region - use patientContainer vector to facility patientContainer pointers
                    for (auto patCon : districts[w][i].patientContainer) {

                        // for each facility patientContainer pointer - get patients
                        for (auto facPat : patCon) {

                            myfile9 << "," << facPat->getStatus() << "," << facPat->getDisease() << "," << facPat->getAge() << "," << facPat->isAlive() << "," << facPat->getInfected() << "," << facPat->getTestPos();
                            myfile9 << "," << facPat->getMDPos() << "," << facPat->getTimeFromOnset() << "," << facPat->getTimeStepsInTreatment() << "," << facPat->getTimeStepsNotInTreatment() << "\n";
                        
                            myfile3 << "," << facPat->getStatus() << "," << facPat->getDisease() << "," << facPat->getAge() << "," << facPat->isAlive() << "," << facPat->getInfected() << "," << facPat->getTestPos();
                            myfile3 << "," << facPat->getMDPos() << "," << facPat->getTimeFromOnset() << "," << facPat->getTimeStepsInTreatment() << "," << facPat->getTimeStepsNotInTreatment() << "\n";
                        
                            // keep track to patient totals by district 
                            distNoPatients[w]++;
                            distAge[w] = distAge[w] + facPat->getAge();
                            if (facPat->isAlive()) distNoAlive[w]++;
                            if (facPat->getInfected()) {
                                distNoInfected[w]++;
                                distTotalTimeFromOnset[w] = distTotalTimeFromOnset[w] + facPat->getTimeFromOnset();
                                distTotalTimeInfectious[w] = distTotalTimeInfectious[w] + facPat->getInfectEndTime();
                                distTotaltimeInTreatment[w] = distTotaltimeInTreatment[w] + facPat->getTimeStepsInTreatment();
                            }
                            else {

                                distTotalTimeNotInTreatment[w] = distTotalTimeNotInTreatment[w] + facPat->getTimeStepsNotInTreatment();
                            }
                            if (facPat->getTestPos()) distNoTestPos[w]++;
                            if (facPat->getMDPos()) distNoMDOverride[w]++;
                            distTotalTimeFacilityReturnTime[w] = distTotalTimeFacilityReturnTime[w] + facPat->getFacilityReturnTime();
                            if (facPat->getResultsReturnedTime() > 0) {

                                distTotalTimeResultsReturned[w] = distTotalTimeResultsReturned[w] + (facPat->getResultsReturnedTime() - facPat->getCreateTime());
                                distTotalNoOfReturnVisits[w] = distTotalNoOfReturnVisits[w] + facPat->getNoReturnsForResults();
                                patReturnedCount++;
                                //std::cout << "stats: districts patients returned time = " << facPat->getResultsReturnedTime() << ", create time = " << facPat->getCreateTime() << ", patient count = " << patReturnedCount << "\n";
                            }
                        }
                    }
                }

                if (individualFlag) {
                    
                    // no patients - no totals
                    if (distNoPatients[w] > 0) {

                        myfile9 << "-----------------------------------------------------------------------------------  " << "\n";
                        myfile9 << "  " << ",disease" << ",age" << ",alive" << ",infected" << ",test.pos" << ",md.override" << ",time.from.onset" << ",time.in.treatment" << ",time.not.in.treatment" << ",std.pat.return.time";
                        myfile9 << ",avg.total.pat.return.time" << ",avg.no.pat.returns" <<  "\n";
                            
                        myfile9 << "Total" << "," << distDisease[w] << "," << distAge[w]/distNoPatients[w] << "," << distNoAlive[w] << "," << distNoInfected[w] << "," << distNoTestPos[w] << "," << distNoMDOverride[w];
                
                        myfile3 << "-----------------------------------------------------------------------------------  " << "\n";
                        myfile3 << "  " << ",disease" << ",age" << ",alive" << ",infected" << ",test.pos" << ",md.override" << ",time.from.onset" << ",time.in.treatment" << ",time.not.in.treatment" << ",std.pat.return.time";
                        myfile3 << ",avg.total.pat.return.time" << ",avg.no.pat.returns" <<  "\n";
                            
                        myfile3 << "Total" << "," << distDisease[w] << "," << distAge[w]/distNoPatients[w] << "," << distNoAlive[w] << "," << distNoInfected[w] << "," << distNoTestPos[w] << "," << distNoMDOverride[w];
                
                        // change output if no infected patients
                        if (distNoInfected[w] > 0) {
               
                            if (distNoInfected[w] == distNoPatients[w]) {

                                myfile9 << "," << distTotalTimeFromOnset[w]/distNoInfected[w] << "," << distTotaltimeInTreatment[w]/distNoInfected[w] << "," << "0";
                                
                                myfile3 << "," << distTotalTimeFromOnset[w]/distNoInfected[w] << "," << distTotaltimeInTreatment[w]/distNoInfected[w] << "," << "0";
                            }
                            else {
                    
                                myfile9 << "," << distTotalTimeFromOnset[w]/distNoInfected[w] << "," << distTotaltimeInTreatment[w]/distNoInfected[w] << "," << distTotalTimeNotInTreatment[w]/(distNoPatients[w] -distNoInfected[w]);
                                
                                myfile3 << "," << distTotalTimeFromOnset[w]/distNoInfected[w] << "," << distTotaltimeInTreatment[w]/distNoInfected[w] << "," << distTotalTimeNotInTreatment[w]/(distNoPatients[w] -distNoInfected[w]);
                            }
                        }
                        else {

                            myfile9 << "," << "0" << "," << "0" << "," << distTotalTimeNotInTreatment[w]/(distNoPatients[w] -distNoInfected[w]);
                            
                            myfile3 << "," << "0" << "," << "0" << "," << distTotalTimeNotInTreatment[w]/(distNoPatients[w] -distNoInfected[w]);
                        }
                          
                        //std::cout << "stats: districts patients count = " << distNoPatients[w] << ", patReturnedCount = " <<  patReturnedCount << "\n";
                        
                        if (patReturnedCount > 0) {
                            
                        
                            myfile9 << "," << (float) distTotalTimeFacilityReturnTime[w]/distNoPatients[w] << "," << (float) distTotalTimeResultsReturned[w]/patReturnedCount << "," << (float) distTotalNoOfReturnVisits[w]/patReturnedCount << "\n";
                        
                            myfile3 << "," << (float) distTotalTimeFacilityReturnTime[w]/distNoPatients[w] << "," << (float) distTotalTimeResultsReturned[w]/patReturnedCount << "," << (float) distTotalNoOfReturnVisits[w]/patReturnedCount << "\n";
                        }
                        else {

                            myfile9 << "," << (float) distTotalTimeFacilityReturnTime[w]/distNoPatients[w] << "," << "0" << "," << "0"<< "\n";

                            myfile3 << "," << (float) distTotalTimeFacilityReturnTime[w]/distNoPatients[w] << "," << "0" << "," << "0"<< "\n";
                        }
                    }
                }
            }

            myfile << "-----------------------------------------------------------------------------------  " << "\n";
            myfile << statHeader1 << statHeader2 << statHeader3 << statHeader4 << statHeader5 << statHeader5a << statHeader6 << statHeader7 << statHeader8;
            myfile << statHeader9 << statHeader10 << statHeader10a << statHeader11 << statHeader12 << statHeader12a << statHeader12b;
            myfile << statHeader13 << statHeader13a << statHeader13b << statHeader13c << statHeader13d << statHeader14 << statHeader15 << statHeader16 << statHeader17;

            //treated = diseaseTotalDistrict[w].countPT + diseaseTotalDistrict[w].countPR + diseaseTotalDistrict[w].countPC + diseaseTotalDistrict[w].countPDI;
            //infected = treated + diseaseTotalDistrict[w].countPUT + diseaseTotalDistrict[w].countPDIUT;
            //treated = diseaseTotalDistrict[w].countPT + diseaseTotalDistrict[w].countPC;
            //infected = treated + diseaseTotalDistrict[w].countPUT + diseaseTotalDistrict[w].countPDIUT + diseaseTotalDistrict[w].countPR + diseaseTotalDistrict[w].countPDI;

            // update population by population growth if it goes on more that 1 year
            newPop = diseaseTotalDistrict[w].pop;
            startIndex = referenceYear - 2015;
            endIndex = startIndex + noOfYears;
            if (noOfYears > 0) {

                for (int i = startIndex; i < endIndex; i++) {

                    // multiply by growth rate for x number of years
                    newPop = (int) (newPop * popGrowthRateArray[i][1]);
                }
            }

            // count up all the sample results
            totalSampleCountPos = diseaseTotalDistrict[w].countTruePos + diseaseTotalDistrict[w].countFalsePos;
            totalSampleCountNeg = diseaseTotalDistrict[w].countTrueNeg + diseaseTotalDistrict[w].countFalseNeg;

            if (debug4) std::cout << "stats: inside agent stats districts dead old age = " << diseaseTotalDistrict[w].countPDOA  << ", timestep = " << timeStep << "\n";

            //myfile << "stats: total district population = " << diseaseTotalDistrict[w].pop << ", patients = " << diseaseTotalDistrict[w].countP << " for disease = " << diseaseList[w] << "\n";
            //myfile << "stats: patient" << " Untested-out = " << diseaseTotalDistrict[w].countPU << " Untested-in =  " << diseaseTotalDistrict[w].countPI << ", under treat = " << diseaseTotalDistrict[w].countPT << ", failed treat = " << diseaseTotalDistrict[w].countPR;
            //myfile << ", untreated = " << diseaseTotalDistrict[w].countPUT << ", cured = " << diseaseTotalDistrict[w].countPC << ", uninfec-treat = " << diseaseTotalDistrict[w].countPUIUT << ", Other = " << diseaseTotalDistrict[w].countPO << "\n";
            //myfile << "               Non-infected - not treated  = " << diseaseTotalDistrict[w].countPUI << ", Non-infected - not returned = " << diseaseTotalDistrict[w].countPUINR << ", dead - infected = " << diseaseTotalDistrict[w].countPDI << ", dead - uninfected = " << diseaseTotalDistrict[w].countPDU << "\n";
            myfile << "," << ","  << diseaseTotalDistrict[w].tier << "," << diseaseTotalDistrict[w].district << "," << diseaseTotalDistrict[w].region << "," << newPop << "," << diseaseList[w] << "," << int((float(diseaseTotalDistrict[w].countPInf) * 100000) / float(newPop));
            myfile << "," << diseaseTotalDistrict[w].countP << "," << diseaseTotalDistrict[w].countPInf << "," << diseaseTotalDistrict[w].countPLtoF;
            myfile << "," << diseaseTotalDistrict[w].outCollectedCount + diseaseTotalDistrict[w].inCollectedCount << "," << diseaseTotalDistrict[w].outCollectedCount << "," << diseaseTotalDistrict[w].inCollectedCount;
            if ((diseaseTotalDistrict[w].sampleWaitForResourceInCount + diseaseTotalDistrict[w].sampleWaitForResourceOutCount) < 1) {
                myfile << "," << "0";
            }
            else {
                myfile << "," << float(diseaseTotalDistrict[w].sampleWaitForResourceInTime + diseaseTotalDistrict[w].sampleWaitForResourceOutTime)/float(diseaseTotalDistrict[w].sampleWaitForResourceInCount + diseaseTotalDistrict[w].sampleWaitForResourceOutCount);
            }
            if (diseaseTotalDistrict[w].sampleWaitForResourceInCount < 1) {
                myfile << "," << "0";
            }
            else {
                myfile << "," << float(diseaseTotalDistrict[w].sampleWaitForResourceInTime)/float(diseaseTotalDistrict[w].sampleWaitForResourceInCount);
            }
            if (diseaseTotalDistrict[w].sampleWaitForResourceOutCount < 1) {
                myfile << "," << "0" << ",";
            }
            else {
                myfile << "," << float(diseaseTotalDistrict[w].sampleWaitForResourceOutTime)/float(diseaseTotalDistrict[w].sampleWaitForResourceOutCount) << ",";
            }
            if (diseaseTotalDistrict[w].sampleTransCount  < 1) {
                myfile << "0" << "," << diseaseTotalDistrict[w].sampleTransCount;
            }
            else {
                myfile << float(diseaseTotalDistrict[w].sampleTransTime)/float(diseaseTotalDistrict[w].sampleTransCount) << "," << diseaseTotalDistrict[w].sampleTransCount;
            }
            myfile << "," << diseaseTotalDistrict[w].sampleTestedInCount + diseaseTotalDistrict[w].sampleTestedOutCount << "," << diseaseTotalDistrict[w].sampleTestedInCount << "," << diseaseTotalDistrict[w].sampleTestedOutCount;
            //myfile << "," << diseaseTotalDistrict[w].sampleLtoFCount;
            if (diseaseTotalDistrict[w].sampleCollectToTestInCount < 1) {
                myfile << "," << "0";
            }
            else {
                myfile << "," << float(diseaseTotalDistrict[w].sampleCollectToTestInTime)/float(diseaseTotalDistrict[w].sampleCollectToTestInCount);
            }
            myfile << "," << float(diseaseTotalDistrict[w].sampleCollectToTestInTime) << "," << diseaseTotalDistrict[w].sampleCollectToTestInCount;
            if (diseaseTotalDistrict[w].sampleCollectToTestOutCount < 1) {
                myfile << "," << "0";
            }
            else {
                myfile << "," << float(diseaseTotalDistrict[w].sampleCollectToTestOutTime)/float(diseaseTotalDistrict[w].sampleCollectToTestOutCount);
            }
            myfile << "," << float(diseaseTotalDistrict[w].sampleCollectToTestOutTime) << "," << diseaseTotalDistrict[w].sampleCollectToTestOutCount;
            myfile << "," << diseaseTotalDistrict[w].countTestPos << ",";
            if (totalSampleCountPos > 0) {
                myfile << float(diseaseTotalDistrict[w].countTruePos)/float(totalSampleCountPos) << ",";
                myfile << float(diseaseTotalDistrict[w].countFalsePos)/float(totalSampleCountPos) << ",";
            }
            else {
                myfile << "0" << "," << "0" << ",";
            }
            myfile << diseaseTotalDistrict[w].countTestNeg << ",";
            if (totalSampleCountNeg > 0) {
                myfile << float(diseaseTotalDistrict[w].countTrueNeg)/float(totalSampleCountNeg) << ",";
                myfile << float(diseaseTotalDistrict[w].countFalseNeg)/float(totalSampleCountNeg);
            }
            else {
                myfile << "0" << "," << "0";
            }
            if (debug2) myfile << "," << "marker";

            myfile << "," << diseaseTotalDistrict[w].countMDInf << "," << diseaseTotalDistrict[w].countMDUninf;
            myfile << "," << diseaseTotalDistrict[w].countMDInf + diseaseTotalDistrict[w].countMDUninf + diseaseTotalDistrict[w].countTestPos;
            myfile << "," << diseaseTotalDistrict[w].countFalsePos << "," << diseaseTotalDistrict[w].countFalseNeg;
            myfile << "," << diseaseTotalDistrict[w].countDiagnosis << "," << diseaseTotalDistrict[w].countDiagnosisIn << "," << diseaseTotalDistrict[w].countDiagnosisOut << "," << diseaseTotalDistrict[w].countPreTestLtoFOut << "," << diseaseTotalDistrict[w].countPreTestLtoFOutMinusTestedSamp << "," << diseaseTotalDistrict[w].countPreTestLtoFDead << "," << diseaseTotalDistrict[w].countPreTestLtoFORepresent;
            if (diseaseTotalDistrict[w].sampleTestPosCountIn < 1) {
                myfile << "," << "0";
            }
            else {
                myfile << "," << float(diseaseTotalDistrict[w].sampleTestPosTimeIn)/float(diseaseTotalDistrict[w].sampleTestPosCountIn);
            }
            myfile << "," << float(diseaseTotalDistrict[w].sampleTestPosTimeIn) << "," << diseaseTotalDistrict[w].sampleTestPosCountIn;
            if (diseaseTotalDistrict[w].sampleTestPosCountOut < 1) {
                myfile << "," << "0";
            }
            else {
                myfile << "," << float(diseaseTotalDistrict[w].sampleTestPosTimeOut)/float(diseaseTotalDistrict[w].sampleTestPosCountOut);
            }
            myfile << "," << float(diseaseTotalDistrict[w].sampleTestPosTimeOut) << "," << diseaseTotalDistrict[w].sampleTestPosCountOut;
            if(debug2) myfile << "," << "marker";
               
            myfile << "," << diseaseTotalDistrict[w].countPInfTreated+ diseaseTotalDistrict[w].countPUninfTreated << "," << diseaseTotalDistrict[w].countPInfTreated << "," << diseaseTotalDistrict[w].countPUninfTreated << "," << diseaseTotalDistrict[w].countPTC << "," << diseaseTotalDistrict[w].countPTD << "," << diseaseTotalDistrict[w].countPTR  << "," << diseaseTotalDistrict[w].countRetreat << "," << diseaseTotalDistrict[w].countRetreatCure << "," << diseaseTotalDistrict[w].countRetreatDead;
            if ((diseaseTotalDistrict[w].countPInfTreatedIn + diseaseTotalDistrict[w].countPInfTreatedOut) < 1) {
                myfile << "," << "0";
            }
            else {
                myfile << "," << float (diseaseTotalDistrict[w].onsetToTreat)/float(diseaseTotalDistrict[w].countPInfTreatedIn + diseaseTotalDistrict[w].countPInfTreatedOut);
            }
            myfile << "," << float (diseaseTotalDistrict[w].onsetToTreat) << "," << (diseaseTotalDistrict[w].countPInfTreatedIn + diseaseTotalDistrict[w].countPInfTreatedOut);
            if (diseaseTotalDistrict[w].countPInfTreatedIn < 1) {
                myfile << "," << "0";
            }
            else {
                myfile << "," << float (diseaseTotalDistrict[w].onsetToTreatIn)/float(diseaseTotalDistrict[w].countPInfTreatedIn);
            }
            myfile << "," << float (diseaseTotalDistrict[w].onsetToTreatIn) << "," << diseaseTotalDistrict[w].countPInfTreatedIn;
            if (diseaseTotalDistrict[w].countPInfTreatedOut < 1) {
                myfile << "," << "0";
            }
            else {
                myfile << "," << float (diseaseTotalDistrict[w].onsetToTreatOut)/float(diseaseTotalDistrict[w].countPInfTreatedOut);
            }
            myfile << "," << float (diseaseTotalDistrict[w].onsetToTreatOut) << "," << diseaseTotalDistrict[w].countPInfTreatedOut;
            if(debug2) myfile << "," << "marker";
               
            if (diseaseTotalDistrict[w].sampleReturnToDeathCount < 1) {
            myfile << "," << "0";
            }
            else {
                myfile << "," << float (diseaseTotalDistrict[w].sampleReturnToDeathTime)/float(diseaseTotalDistrict[w].sampleReturnToDeathCount);
            }
            myfile << "," << float (diseaseTotalDistrict[w].sampleReturnToDeathTime) << "," << diseaseTotalDistrict[w].sampleReturnToDeathCount;
            if (diseaseTotalDistrict[w].sampleReturnToDeathPreDiagLtoFCount < 1) {
                myfile << "," << "0";
            }
            else {
                myfile << "," << float (diseaseTotalDistrict[w].sampleReturnToDeathPreDiagLtoFTime)/float(diseaseTotalDistrict[w].sampleReturnToDeathPreDiagLtoFCount);
            }
            myfile << "," << float (diseaseTotalDistrict[w].sampleReturnToDeathPreDiagLtoFTime) << "," << diseaseTotalDistrict[w].sampleReturnToDeathPreDiagLtoFCount;
            if (diseaseTotalDistrict[w].sampleReturnToDeathInTrtCount < 1) {
                myfile << "," << "0";
            }
            else {
                myfile << "," << float (diseaseTotalDistrict[w].sampleReturnToDeathInTrtTime)/float(diseaseTotalDistrict[w].sampleReturnToDeathInTrtCount);
            }
            myfile << "," << float (diseaseTotalDistrict[w].sampleReturnToDeathInTrtTime) << "," << diseaseTotalDistrict[w].sampleReturnToDeathInTrtCount;
            if (diseaseTotalDistrict[w].sampleReturnToDeathInRetrtCount < 1) {
                myfile << "," << "0";
            }
            else {
                myfile << "," << float (diseaseTotalDistrict[w].sampleReturnToDeathInRetrtTime)/float(diseaseTotalDistrict[w].sampleReturnToDeathInRetrtCount);
            }
            myfile << "," << float (diseaseTotalDistrict[w].sampleReturnToDeathInRetrtTime) << "," << diseaseTotalDistrict[w].sampleReturnToDeathInRetrtCount;
            if (diseaseTotalDistrict[w].sampleReturnToExitCount < 1) {
                myfile << "," << "0";
            }
            else {
                myfile << "," << float (diseaseTotalDistrict[w].sampleReturnToExitTime)/float(diseaseTotalDistrict[w].sampleReturnToExitCount);
            }
            myfile << "," << float (diseaseTotalDistrict[w].sampleReturnToExitTime) << "," << diseaseTotalDistrict[w].sampleReturnToExitCount;
            myfile << "," << diseaseTotalDistrict[w].test1CountIn + diseaseTotalDistrict[w].test1CountOut << "," << diseaseTotalDistrict[w].test1CountIn << "," << diseaseTotalDistrict[w].test1CountOut << "," << diseaseTotalDistrict[w].test2Count << "," << diseaseTotalDistrict[w]. test2PosCount;
            if (diseaseTotalDistrict[w].test2PosCount < 1) {
                myfile << "," << "0";
            }
            else {
                myfile << "," << float (diseaseTotalDistrict[w].test2TruePosCount)/float(diseaseTotalDistrict[w].test2PosCount);
            }
            myfile << "," << diseaseTotalDistrict[w].test2NegCount;
            if (diseaseTotalDistrict[w].test2NegCount < 1) {
                myfile << "," << "0";
            }
            else {
                myfile << "," << float (diseaseTotalDistrict[w].test2TrueNegCount)/float(diseaseTotalDistrict[w].test2NegCount);
            }
            myfile << "," << diseaseTotalDistrict[w].test1PosCount;
            if (diseaseTotalDistrict[w].test1PosCount < 1) {
                myfile << "," << "0";
            }
            else {
                myfile << "," << float (diseaseTotalDistrict[w].test1TruePosCount)/float(diseaseTotalDistrict[w].test1PosCount);
            }
            myfile << "," << diseaseTotalDistrict[w].test1NegCount;
            if (diseaseTotalDistrict[w].test1NegCount < 1) {
                myfile << "," << "0";
            }
            else {
                myfile << "," << float (diseaseTotalDistrict[w].test1TrueNegCount)/float(diseaseTotalDistrict[w].test1NegCount);
            }
            if(debug2) myfile << "," << "marker";
               
            myfile << "," << diseaseTotalDistrict[w].countP << "," << diseaseTotalDistrict[w].countPI << "," << diseaseTotalDistrict[w].countPU  << "," << diseaseTotalDistrict[w].countPT;
            myfile << "," << diseaseTotalDistrict[w].sampleWaitTestCount << "," << diseaseTotalDistrict[w].sampleWaitTestCountIn << "," << diseaseTotalDistrict[w].sampleWaitTestCountOut;
            myfile << "," << diseaseTotalDistrict[w].countPreTestLtoFOutTested << "," << diseaseTotalDistrict[w].patientNotNotified << "," << diseaseTotalDistrict[w].patientNotNotifiedPos << "\n";
            myfile << "\n";

            myfile7 << "Total," << diseaseTotalDistrict[w].id << "," << diseaseTotalDistrict[w].tier << "," << diseaseTotalDistrict[w].district << "," << diseaseTotalDistrict[w].region << "," << newPop << "," << diseaseList[w] << "," << int((float(diseaseTotalDistrict[w].countPInf) * 100000) / float(newPop));
            myfile7 << "," << diseaseTotalDistrict[w].countP << "," << diseaseTotalDistrict[w].countPInf << "," << diseaseTotalDistrict[w].countPLtoF;
            myfile7 << "," << diseaseTotalDistrict[w].outCollectedCount + diseaseTotalDistrict[w].inCollectedCount << "," << diseaseTotalDistrict[w].outCollectedCount << "," << diseaseTotalDistrict[w].inCollectedCount;
            if ((diseaseTotalDistrict[w].sampleWaitForResourceInCount + diseaseTotalDistrict[w].sampleWaitForResourceOutCount) < 1) {
                myfile7 << "," << "0";
            }
            else {
                myfile7 << "," << float(diseaseTotalDistrict[w].sampleWaitForResourceInTime + diseaseTotalDistrict[w].sampleWaitForResourceOutTime)/float(diseaseTotalDistrict[w].sampleWaitForResourceInCount + diseaseTotalDistrict[w].sampleWaitForResourceOutCount);
            }
            if (diseaseTotalDistrict[w].sampleWaitForResourceInCount < 1) {
                myfile7 << "," << "0";
            }
            else {
                myfile7 << "," << float(diseaseTotalDistrict[w].sampleWaitForResourceInTime)/float(diseaseTotalDistrict[w].sampleWaitForResourceInCount);
            }
            if (diseaseTotalDistrict[w].sampleWaitForResourceOutCount < 1) {
                myfile7 << "," << "0" << ",";
            }
            else {
                myfile7 << "," << float(diseaseTotalDistrict[w].sampleWaitForResourceOutTime)/float(diseaseTotalDistrict[w].sampleWaitForResourceOutCount) << ",";
            }
            if (diseaseTotalDistrict[w].sampleTransCount  < 1) {
                myfile7 << "0" << "," << diseaseTotalDistrict[w].sampleTransCount;
            }
            else {
                myfile7 << float(diseaseTotalDistrict[w].sampleTransTime)/float(diseaseTotalDistrict[w].sampleTransCount) << "," << diseaseTotalDistrict[w].sampleTransCount;
            }
            myfile7 << "," << diseaseTotalDistrict[w].sampleTestedInCount + diseaseTotalDistrict[w].sampleTestedOutCount << "," << diseaseTotalDistrict[w].sampleTestedInCount << "," << diseaseTotalDistrict[w].sampleTestedOutCount;
            //myfile7 << "," << diseaseTotalDistrict[w].sampleLtoFCount;
             if (diseaseTotalDistrict[w].sampleCollectToTestInCount < 1) {
                myfile7 << "," << "0";
            }
            else {
                myfile7 << "," << float(diseaseTotalDistrict[w].sampleCollectToTestInTime)/float(diseaseTotalDistrict[w].sampleCollectToTestInCount);
            }
            myfile7 << "," << float(diseaseTotalDistrict[w].sampleCollectToTestInTime) << "," << diseaseTotalDistrict[w].sampleCollectToTestInCount;
            if (diseaseTotalDistrict[w].sampleCollectToTestOutCount < 1) {
                myfile7 << "," << "0";
            }
            else {
                myfile7 << "," << float(diseaseTotalDistrict[w].sampleCollectToTestOutTime)/float(diseaseTotalDistrict[w].sampleCollectToTestOutCount);
            }
            myfile7 << "," << float(diseaseTotalDistrict[w].sampleCollectToTestOutTime) << "," << diseaseTotalDistrict[w].sampleCollectToTestOutCount;
            myfile7 << "," << diseaseTotalDistrict[w].countTestPos << ",";
            if (totalSampleCountPos > 0) {
                myfile7 << float(diseaseTotalDistrict[w].countTruePos)/float(totalSampleCountPos) << ",";
                myfile7 << float(diseaseTotalDistrict[w].countFalsePos)/float(totalSampleCountPos) << ",";
            }
            else {
                myfile7 << "0" << "," << "0" << ",";
            }
            myfile7 << diseaseTotalDistrict[w].countTestNeg << ",";
            if (totalSampleCountNeg > 0) {
                myfile7 << float(diseaseTotalDistrict[w].countTrueNeg)/float(totalSampleCountNeg) << ",";
                myfile7 << float(diseaseTotalDistrict[w].countFalseNeg)/float(totalSampleCountNeg);
            }
            else {
                myfile7 << "0" << "," << "0";
            }
            if (debug2) myfile7 << "," << "marker";

            myfile7 << "," << diseaseTotalDistrict[w].countMDInf << "," << diseaseTotalDistrict[w].countMDUninf;
            myfile7 << "," << diseaseTotalDistrict[w].countMDInf + diseaseTotalDistrict[w].countMDUninf + diseaseTotalDistrict[w].countTestPos;
            myfile7 << "," << diseaseTotalDistrict[w].countFalsePos << "," << diseaseTotalDistrict[w].countFalseNeg;
            myfile7 << "," << diseaseTotalDistrict[w].countDiagnosis << "," << diseaseTotalDistrict[w].countDiagnosisIn << "," << diseaseTotalDistrict[w].countDiagnosisOut << "," << diseaseTotalDistrict[w].countPreTestLtoFOut << "," << diseaseTotalDistrict[w].countPreTestLtoFOutMinusTestedSamp << "," << diseaseTotalDistrict[w].countPreTestLtoFDead << "," << diseaseTotalDistrict[w].countPreTestLtoFORepresent;
            if (diseaseTotalDistrict[w].sampleTestPosCountIn < 1) {
                myfile7 << "," << "0";
            }
            else {
                myfile7 << "," << float(diseaseTotalDistrict[w].sampleTestPosTimeIn)/float(diseaseTotalDistrict[w].sampleTestPosCountIn);
            }
            myfile7 << "," << float(diseaseTotalDistrict[w].sampleTestPosTimeIn) << "," << diseaseTotalDistrict[w].sampleTestPosCountIn;
            if (diseaseTotalDistrict[w].sampleTestPosCountOut < 1) {
                myfile7 << "," << "0";
            }
            else {
                myfile7 << "," << float(diseaseTotalDistrict[w].sampleTestPosTimeOut)/float(diseaseTotalDistrict[w].sampleTestPosCountOut);
            }
            myfile7 << "," << float(diseaseTotalDistrict[w].sampleTestPosTimeOut) << "," << diseaseTotalDistrict[w].sampleTestPosCountOut;
            if(debug2) myfile7 << "," << "marker";
               
            myfile7 << "," << diseaseTotalDistrict[w].countPInfTreated+ diseaseTotalDistrict[w].countPUninfTreated << "," << diseaseTotalDistrict[w].countPInfTreated << "," << diseaseTotalDistrict[w].countPUninfTreated << "," << diseaseTotalDistrict[w].countPTC << "," << diseaseTotalDistrict[w].countPTD << "," << diseaseTotalDistrict[w].countPTR  << "," << diseaseTotalDistrict[w].countRetreat << "," << diseaseTotalDistrict[w].countRetreatCure << "," << diseaseTotalDistrict[w].countRetreatDead;
            if ((diseaseTotalDistrict[w].countPInfTreatedIn + diseaseTotalDistrict[w].countPInfTreatedOut) < 1) {
                myfile7 << "," << "0";
            }
            else {
                myfile7 << "," << float (diseaseTotalDistrict[w].onsetToTreat)/float(diseaseTotalDistrict[w].countPInfTreatedIn + diseaseTotalDistrict[w].countPInfTreatedOut);
            }
            myfile7 << "," << float (diseaseTotalDistrict[w].onsetToTreat) << "," << (diseaseTotalDistrict[w].countPInfTreatedIn + diseaseTotalDistrict[w].countPInfTreatedOut);
            if (diseaseTotalDistrict[w].countPInfTreatedIn < 1) {
                myfile7 << "," << "0";
            }
            else {
                myfile7 << "," << float (diseaseTotalDistrict[w].onsetToTreatIn)/float(diseaseTotalDistrict[w].countPInfTreatedIn);
            }
            myfile7 << "," << float (diseaseTotalDistrict[w].onsetToTreatIn) << "," << diseaseTotalDistrict[w].countPInfTreatedIn;
            if (diseaseTotalDistrict[w].countPInfTreatedOut < 1) {
                myfile7 << "," << "0";
            }
            else {
                myfile7 << "," << float (diseaseTotalDistrict[w].onsetToTreatOut)/float(diseaseTotalDistrict[w].countPInfTreatedOut);
            }
            myfile7 << "," << float (diseaseTotalDistrict[w].onsetToTreatOut) << "," << diseaseTotalDistrict[w].countPInfTreatedOut;
            if(debug2) myfile7 << "," << "marker";
               
            if (diseaseTotalDistrict[w].sampleReturnToDeathCount < 1) {
                myfile7 << "," << "0";
            }
            else {
                myfile7 << "," << float (diseaseTotalDistrict[w].sampleReturnToDeathTime)/float(diseaseTotalDistrict[w].sampleReturnToDeathCount);
            }
            myfile7 << "," << float (diseaseTotalDistrict[w].sampleReturnToDeathTime) << "," << diseaseTotalDistrict[w].sampleReturnToDeathCount;
            if (diseaseTotalDistrict[w].sampleReturnToDeathPreDiagLtoFCount < 1) {
                myfile7 << "," << "0";
            }
            else {
                myfile7 << "," << float (diseaseTotalDistrict[w].sampleReturnToDeathPreDiagLtoFTime)/float(diseaseTotalDistrict[w].sampleReturnToDeathPreDiagLtoFCount);
            }
            myfile7 << "," << float (diseaseTotalDistrict[w].sampleReturnToDeathPreDiagLtoFTime) << "," << diseaseTotalDistrict[w].sampleReturnToDeathPreDiagLtoFCount;
            if (diseaseTotalDistrict[w].sampleReturnToDeathInTrtCount < 1) {
                myfile7 << "," << "0";
            }
            else {
                myfile7 << "," << float (diseaseTotalDistrict[w].sampleReturnToDeathInTrtTime)/float(diseaseTotalDistrict[w].sampleReturnToDeathInTrtCount);
            }
            myfile7 << "," << float (diseaseTotalDistrict[w].sampleReturnToDeathInTrtTime) << "," << diseaseTotalDistrict[w].sampleReturnToDeathInTrtCount;
            if (diseaseTotalDistrict[w].sampleReturnToDeathInRetrtCount < 1) {
                myfile7 << "," << "0";
            }
            else {
                myfile7 << "," << float (diseaseTotalDistrict[w].sampleReturnToDeathInRetrtTime)/float(diseaseTotalDistrict[w].sampleReturnToDeathInRetrtCount);
            }
            myfile7 << "," << float (diseaseTotalDistrict[w].sampleReturnToDeathInRetrtTime) << "," << diseaseTotalDistrict[w].sampleReturnToDeathInRetrtCount;
            if (diseaseTotalDistrict[w].sampleReturnToExitCount < 1) {
                myfile7 << "," << "0";
            }
            else {
                myfile7 << "," << float (diseaseTotalDistrict[w].sampleReturnToExitTime)/float(diseaseTotalDistrict[w].sampleReturnToExitCount);
            }
            myfile7 << "," << float (diseaseTotalDistrict[w].sampleReturnToExitTime) << "," << diseaseTotalDistrict[w].sampleReturnToExitCount;
            myfile7 << "," << diseaseTotalDistrict[w].test1CountIn + diseaseTotalDistrict[w].test1CountOut << "," << diseaseTotalDistrict[w].test1CountIn << "," << diseaseTotalDistrict[w].test1CountOut << "," << diseaseTotalDistrict[w].test2Count << "," << diseaseTotalDistrict[w]. test2PosCount;
            if (diseaseTotalDistrict[w].test2PosCount < 1) {
                myfile7 << "," << "0";
            }
            else {
                myfile7 << "," << float (diseaseTotalDistrict[w].test2TruePosCount)/float(diseaseTotalDistrict[w].test2PosCount);
            }
            myfile7 << "," << diseaseTotalDistrict[w].test2NegCount;
            if (diseaseTotalDistrict[w].test2NegCount < 1) {
                myfile7 << "," << "0";
            }
            else {
                myfile7 << "," << float (diseaseTotalDistrict[w].test2TrueNegCount)/float(diseaseTotalDistrict[w].test2NegCount);
            }
            myfile7 << "," << diseaseTotalDistrict[w].test1PosCount;
            if (diseaseTotalDistrict[w].test1PosCount < 1) {
                myfile7 << "," << "0";
            }
            else {
                myfile7 << "," << float (diseaseTotalDistrict[w].test1TruePosCount)/float(diseaseTotalDistrict[w].test1PosCount);
            }
            myfile7 << "," << diseaseTotalDistrict[w].test1NegCount;
            if (diseaseTotalDistrict[w].test1NegCount < 1) {
                myfile7 << "," << "0";
            }
            else {
                myfile7 << "," << float (diseaseTotalDistrict[w].test1TrueNegCount)/float(diseaseTotalDistrict[w].test1NegCount);
            }
            if(debug2) myfile7 << "," << "marker";
               
            myfile7 << "," << diseaseTotalDistrict[w].countP << "," << diseaseTotalDistrict[w].countPI << "," << diseaseTotalDistrict[w].countPU  << "," << diseaseTotalDistrict[w].countPT;
            myfile7 << "," << diseaseTotalDistrict[w].sampleWaitTestCount << "," << diseaseTotalDistrict[w].sampleWaitTestCountIn << "," << diseaseTotalDistrict[w].sampleWaitTestCountOut;
            myfile7 << "," << diseaseTotalDistrict[w].countPreTestLtoFOutTested << "," << diseaseTotalDistrict[w].patientNotNotified << "," << diseaseTotalDistrict[w].patientNotNotifiedPos << "\n";


            if (individualFlag) {

                //treated = diseaseTotalDistrict[w].countPT + diseaseTotalDistrict[w].countPR + diseaseTotalDistrict[w].countPC + diseaseTotalDistrict[w].countPDI;
                //treated = diseaseTotalDistrict[w].countPT + diseaseTotalDistrict[w].countPC;
                
                myfile9 << "-----------------------------------------------------------------------------------  " << "\n";
                myfile9 << "stats.total.district.population" <<  ",patients" <<  ".for.disease" <<  "\n";
                myfile9 <<  diseaseTotalDistrict[w].pop << "," << diseaseTotalDistrict[w].countP << "," << diseaseList[w] << "\n";
                myfile9 << "stats.patient" << ",s.untested.out" << ",s.untested.in" << ",s.under.treat" <<  ",failed.treat" << ",s.untreated.false.neg" << ",s.untreated.no.sample" << ",s.untreated.ltof";
                myfile9 << ",cured" << ",infec.treat" << ",uninfec.treat" << ",other";
                myfile9 << ",non.infected.not.treated" << ",non.infected.not.returned" << ",dead.infected.treated" << ",dead.infected.untreated" << ",dead.uninfected" << "\n";
                myfile9 << "," << diseaseTotalDistrict[w].countPU << "," << diseaseTotalDistrict[w].countPI << "," << diseaseTotalDistrict[w].countPT << "," << diseaseTotalDistrict[w].countPR;
                myfile9 << "," << diseaseTotalDistrict[w].countPUTFN << "," << diseaseTotalDistrict[w].countPUTNS << "," << diseaseTotalDistrict[w].countPUTLtoF << "," << diseaseTotalDistrict[w].countPC;
                myfile9 << "," << diseaseTotalDistrict[w].countPInfTreated << "," << diseaseTotalDistrict[w].countPUninfTreated << "," << diseaseTotalDistrict[w].countPO;
                myfile9 << "," << diseaseTotalDistrict[w].countPUI << "," << diseaseTotalDistrict[w].countPUINR << "," << diseaseTotalDistrict[w].countPDI << "," << diseaseTotalDistrict[w].countPDIUT << "," << diseaseTotalDistrict[w].countPDU << "\n";
                
                myfile3 << "-----------------------------------------------------------------------------------  " << "\n";
                myfile3 << "stats.total.district.population" <<  ",patients" <<  ".for.disease" <<  "\n";
                myfile3 <<  diseaseTotalDistrict[w].pop << "," << diseaseTotalDistrict[w].countP << "," << diseaseList[w] << "\n";
                myfile3 << "stats.patient" << ",s.untested.out" << ",s.untested.in" << ",s.under.treat" <<  ",failed.treat" <<  ",s.untreated.false.neg" << ",s.untreated.no.sample" << ",s.untreated.ltof";
                myfile3 << ",cured" << ",infec.treat" << ",uninfec.treat" << ",other";
                myfile3 << ",non.infected.not.treated" << ",non.infected.not.returned" << ",dead.infected.treated" << ",dead.infected.untreated" << ",dead.uninfected" << "\n";
                myfile3 << "," << diseaseTotalDistrict[w].countPU << "," << diseaseTotalDistrict[w].countPI << "," << diseaseTotalDistrict[w].countPT << "," << diseaseTotalDistrict[w].countPR;
                myfile3 << "," << diseaseTotalDistrict[w].countPUTFN << "," << diseaseTotalDistrict[w].countPUTNS << "," << diseaseTotalDistrict[w].countPUTLtoF << "," << diseaseTotalDistrict[w].countPC;
                myfile3 << "," << diseaseTotalDistrict[w].countPInfTreated << "," << diseaseTotalDistrict[w].countPUninfTreated << "," << diseaseTotalDistrict[w].countPO;
                myfile3 << "," << diseaseTotalDistrict[w].countPUI << "," << diseaseTotalDistrict[w].countPUINR << "," << diseaseTotalDistrict[w].countPDI << "," << diseaseTotalDistrict[w].countPDIUT << "," << diseaseTotalDistrict[w].countPDU << "\n";
            }
        }
        if (debug) std::cout << "stats: inside agent stats mark 2"  << "\n";

        myfile << "-----------------------------------------------------------------------------------  " << "\n";
        myfile << statHeader1 << statHeader2 << statHeader3 << statHeader4 << statHeader5 << statHeader5a << statHeader6 << statHeader7 << statHeader8;
        myfile << statHeader9 << statHeader10 << statHeader10a << statHeader11 << statHeader12 << statHeader12a << statHeader12b;
        myfile << statHeader13 << statHeader13a << statHeader13b << statHeader13c << statHeader13d << statHeader14 << statHeader15 << statHeader16 << statHeader17;
        

        //treated = totalDistrict.countPT + totalDistrict.countPR + totalDistrict.countPC + totalDistrict.countPDI;
        //infected = treated + totalDistrict.countPUT + totalDistrict.countPDIUT;
        //treated = totalDistrict.countPT + totalDistrict.countPC;
        //infected = treated + totalDistrict.countPUT + totalDistrict.countPDIUT + totalDistrict.countPR + totalDistrict.countPDI;

        // update population by population growth if it goes on more that 1 year
        newPop = totalDistrict.pop;
        startIndex = referenceYear - 2015;
        endIndex = startIndex + noOfYears;
        if (noOfYears > 0) {

            for (int i = startIndex; i < endIndex; i++) {

                // multiply by growth rate for x number of years
                newPop = (int) (newPop * popGrowthRateArray[i][1]);
            }
        }
        
        // count up all the sample results
        totalSampleCountPos = totalDistrict.countTruePos + totalDistrict.countFalsePos;
        totalSampleCountNeg = totalDistrict.countTrueNeg + totalDistrict.countFalseNeg;

        //myfile << "stats: total district  population = " << totalDistrict.pop << ", patients = " << totalDistrict.countP << "\n";
        //myfile << "stats: patient" << " Untested-out = " << totalDistrict.countPU << " Untested-in =  " << totalDistrict.countPI << ", under treat = " << totalDistrict.countPT << ", failed treat = " << totalDistrict.countPR << ", untreated = " << totalDistrict.countPUT << ", cured = " << totalDistrict.countPC << ", uninfec-treat = " << totalDistrict.countPUIUT << ", Other = " << totalDistrict.countPO << "\n";
        //myfile << "               Non-infected - not treated  = " << totalDistrict.countPUI << ", Non-infected - not returned = " << totalDistrict.countPUINR << ", dead - infected = " << totalDistrict.countPDI << ", dead - uninfected = " << totalDistrict.countPDU << "\n";
        myfile << "," << "," << totalDistrict.tier << "," << totalDistrict.district << "," << totalDistrict.region << "," << newPop << "," << "" << "," << "";
        myfile << "," << totalDistrict.countP << "," << totalDistrict.countPInf << "," << totalDistrict.countPLtoF;
        myfile << "," << totalDistrict.outCollectedCount + totalDistrict.inCollectedCount << "," << totalDistrict.outCollectedCount << "," << totalDistrict.inCollectedCount;
        if ((totalDistrict.sampleWaitForResourceInCount + totalDistrict.sampleWaitForResourceOutCount) < 1) {
            myfile << "," << "0";
        }
        else {
            myfile << "," << float(totalDistrict.sampleWaitForResourceInTime + totalDistrict.sampleWaitForResourceOutTime)/float(totalDistrict.sampleWaitForResourceInCount + totalDistrict.sampleWaitForResourceOutCount);
        }
        if (totalDistrict.sampleWaitForResourceInCount < 1) {
            myfile << "," << "0";
        }
        else {
            myfile << "," << float(totalDistrict.sampleWaitForResourceInTime)/float(totalDistrict.sampleWaitForResourceInCount);
        }
        if (totalDistrict.sampleWaitForResourceOutCount < 1) {
            myfile << "," << "0" << ",";
        }
        else {
            myfile << "," << float(totalDistrict.sampleWaitForResourceOutTime)/float(totalDistrict.sampleWaitForResourceOutCount) << ",";
        }
        if (totalDistrict.sampleTransCount  < 1) {
            myfile << "0" << "," << totalDistrict.sampleTransCount;
        }
        else {
            myfile << float(totalDistrict.sampleTransTime)/float(totalDistrict.sampleTransCount) << "," << totalDistrict.sampleTransCount;
        }
        myfile << "," << totalDistrict.sampleTestedInCount + totalDistrict.sampleTestedOutCount << "," << totalDistrict.sampleTestedInCount << "," << totalDistrict.sampleTestedOutCount;
        //myfile << "," << totalDistrict.sampleLtoFCount;
        if (totalDistrict.sampleCollectToTestInCount < 1) {
            myfile << "," << "0";
        }
        else {
            myfile << "," << float(totalDistrict.sampleCollectToTestInTime)/float(totalDistrict.sampleCollectToTestInCount);
        }
        myfile << "," << float(totalDistrict.sampleCollectToTestInTime) << "," << totalDistrict.sampleCollectToTestInCount;
        if (totalDistrict.sampleCollectToTestOutCount < 1) {
            myfile << "," << "0";
        }
        else {
            myfile << "," << float(totalDistrict.sampleCollectToTestOutTime)/float(totalDistrict.sampleCollectToTestOutCount);
        }
        myfile << "," << float(totalDistrict.sampleCollectToTestOutTime) << "," << totalDistrict.sampleCollectToTestOutCount;
        myfile << "," << totalDistrict.countTestPos << ",";
        if (totalSampleCountPos > 0) {
            myfile << float(totalDistrict.countTruePos)/float(totalSampleCountPos) << ",";
            myfile << float(totalDistrict.countFalsePos)/float(totalSampleCountPos) << ",";
        }
        else {
            myfile << "0" << "," << "0" << ",";
        }
        if (debugOutput) {
        
            std::cout << "district countTestPos = " << totalDistrict.countTestPos << ", totalSampleCountPos " << totalSampleCountPos  << "\n";
            std::cout << "district true pos = " << totalDistrict.countTruePos << ", false pos " << totalDistrict.countFalsePos << ", true neg = " << totalDistrict.countTrueNeg << ", false neg " << totalDistrict.countFalseNeg << "\n";
        }
        myfile << totalDistrict.countTestNeg << ",";
        if (totalSampleCountNeg > 0) {
            myfile << float(totalDistrict.countTrueNeg)/float(totalSampleCountNeg) << ",";
            myfile << float(totalDistrict.countFalseNeg)/float(totalSampleCountNeg);
        }
        else {
            myfile << "0" << "," << "0";
        }
        if (debug2) myfile << "," << "marker";

        myfile << "," << totalDistrict.countMDInf << "," << totalDistrict.countMDUninf;
        myfile << "," << totalDistrict.countMDInf + totalDistrict.countMDUninf + totalDistrict.countTestPos;
        myfile << "," << totalDistrict.countFalsePos << "," << totalDistrict.countFalseNeg;
        myfile << "," << totalDistrict.countDiagnosis << "," << totalDistrict.countDiagnosisIn << "," << totalDistrict.countDiagnosisOut << "," << totalDistrict.countPreTestLtoFOut << "," << totalDistrict.countPreTestLtoFOutMinusTestedSamp << "," << totalDistrict.countPreTestLtoFDead << "," << totalDistrict.countPreTestLtoFORepresent;
        if (totalDistrict.sampleTestPosCountIn < 1) {
            myfile << "," << "0";
        }
        else {
            myfile << "," << float(totalDistrict.sampleTestPosTimeIn)/float(totalDistrict.sampleTestPosCountIn);
        }
        myfile << "," << float(totalDistrict.sampleTestPosTimeIn) << "," << totalDistrict.sampleTestPosCountIn;
        if (totalDistrict.sampleTestPosCountOut < 1) {
            myfile << "," << "0";
        }
        else {
            myfile << "," << float(totalDistrict.sampleTestPosTimeOut)/float(totalDistrict.sampleTestPosCountOut);
        }
        myfile << "," << float(totalDistrict.sampleTestPosTimeOut) << "," << totalDistrict.sampleTestPosCountOut;
        if(debug2) myfile << "," << "marker";
               
        myfile << "," << totalDistrict.countPInfTreated+ totalDistrict.countPUninfTreated << "," << totalDistrict.countPInfTreated << "," << totalDistrict.countPUninfTreated << "," << totalDistrict.countPTC << "," << totalDistrict.countPTD << "," << totalDistrict.countPTR  << "," << totalDistrict.countRetreat << "," << totalDistrict.countRetreatCure << "," << totalDistrict.countRetreatDead;
        if ((totalDistrict.countPInfTreatedIn + totalDistrict.countPInfTreatedOut) < 1) {
            myfile << "," << "0";
        }
        else {
            myfile << "," << float (totalDistrict.onsetToTreat)/float(totalDistrict.countPInfTreatedIn + totalDistrict.countPInfTreatedOut);
        }
        myfile << "," << float (totalDistrict.onsetToTreat) << "," << (totalDistrict.countPInfTreatedIn + totalDistrict.countPInfTreatedOut);
        if (totalDistrict.countPInfTreatedIn < 1) {
            myfile << "," << "0";
        }
        else {
            myfile << "," << float (totalDistrict.onsetToTreatIn)/float(totalDistrict.countPInfTreatedIn);
        }
        myfile << "," << float (totalDistrict.onsetToTreatIn) << "," << totalDistrict.countPInfTreatedIn;
        if (totalDistrict.countPInfTreatedOut < 1) {
            myfile << "," << "0";
        }
        else {
            myfile << "," << float (totalDistrict.onsetToTreatOut)/float(totalDistrict.countPInfTreatedOut);
        }
        myfile << "," << float (totalDistrict.onsetToTreatOut) << "," << totalDistrict.countPInfTreatedOut;
        if(debug2) myfile <<  "," << "marker";
               
        if (totalDistrict.sampleReturnToDeathCount < 1) {
            myfile << "," << "0";
        }
        else {
            myfile << "," << float (totalDistrict.sampleReturnToDeathTime)/float(totalDistrict.sampleReturnToDeathCount);
        }
        myfile << "," << float (totalDistrict.sampleReturnToDeathTime) << "," << totalDistrict.sampleReturnToDeathCount;
        if (totalDistrict.sampleReturnToDeathPreDiagLtoFCount < 1) {
            myfile << "," << "0";
        }
        else {
            myfile << "," << float (totalDistrict.sampleReturnToDeathPreDiagLtoFTime)/float(totalDistrict.sampleReturnToDeathPreDiagLtoFCount);
        }
        myfile << "," << float (totalDistrict.sampleReturnToDeathPreDiagLtoFTime) << "," << totalDistrict.sampleReturnToDeathPreDiagLtoFCount;
        if (totalDistrict.sampleReturnToDeathInTrtCount < 1) {
            myfile << "," << "0";
        }
        else {
            myfile << "," << float (totalDistrict.sampleReturnToDeathInTrtTime)/float(totalDistrict.sampleReturnToDeathInTrtCount);
        }
        myfile << "," << float (totalDistrict.sampleReturnToDeathInTrtTime) << "," << totalDistrict.sampleReturnToDeathInTrtCount;
        if (totalDistrict.sampleReturnToDeathInRetrtCount < 1) {
            myfile << "," << "0";
        }
        else {
            myfile << "," << float (totalDistrict.sampleReturnToDeathInRetrtTime)/float(totalDistrict.sampleReturnToDeathInRetrtCount);
        }
        myfile << "," << float (totalDistrict.sampleReturnToDeathInRetrtTime) << "," << totalDistrict.sampleReturnToDeathInRetrtCount;
        if (totalDistrict.sampleReturnToExitCount < 1) {
            myfile << "," << "0";
        }
        else {
            myfile << "," << float (totalDistrict.sampleReturnToExitTime)/float(totalDistrict.sampleReturnToExitCount);
        }
        myfile << "," << float (totalDistrict.sampleReturnToExitTime) << "," << totalDistrict.sampleReturnToExitCount;
        myfile << "," << totalDistrict.test1CountIn + totalDistrict.test1CountOut << "," << totalDistrict.test1CountIn << "," << totalDistrict.test1CountOut << "," << totalDistrict.test2Count << "," << totalDistrict. test2PosCount;
        if (totalDistrict.test2PosCount < 1) {
            myfile << "," << "0";
        }
        else {
            myfile << "," << float (totalDistrict.test2TruePosCount)/float(totalDistrict.test2PosCount);
        }
        myfile << "," << totalDistrict.test2NegCount;
        if (totalDistrict.test2NegCount < 1) {
            myfile << "," << "0";
        }
        else {
            myfile << "," << float (totalDistrict.test2TrueNegCount)/float(totalDistrict.test2NegCount);
        }
        myfile << "," << totalDistrict.test1PosCount;
        if (totalDistrict.test1PosCount < 1) {
            myfile << "," << "0";
        }
        else {
            myfile << "," << float (totalDistrict.test1TruePosCount)/float(totalDistrict.test1PosCount);
        }
        myfile << "," << totalDistrict.test1NegCount;
        if (totalDistrict.test1NegCount < 1) {
            myfile << "," << "0";
        }
        else {
            myfile << "," << float (totalDistrict.test1TrueNegCount)/float(totalDistrict.test1NegCount);
        }
        if(debug2) myfile << "," << "marker";
               
        myfile << "," << totalDistrict.countP << "," << totalDistrict.countPI << "," << totalDistrict.countPU  << "," << totalDistrict.countPT;
        myfile << "," << totalDistrict.sampleWaitTestCount << "," << totalDistrict.sampleWaitTestCountIn << "," << totalDistrict.sampleWaitTestCountOut;
        myfile << "," << totalDistrict.countPreTestLtoFOutTested << "," << totalDistrict.patientNotNotified << "," << totalDistrict.patientNotNotifiedPos << "\n";

        myfile << "-----------------------------------------------------------------------------------  " << "\n";

        if (individualFlag) {
            
            //treated = totalDistrict.countPT + totalDistrict.countPR + totalDistrict.countPC + totalDistrict.countPDI;
            //treated = totalDistrict.countPT + totalDistrict.countPC;
            
            myfile9 << "-----------------------------------------------------------------------------------  " << "\n";
            myfile9 << "stats.total.district.population" << ",patients" << "\n";
            myfile9 <<  newPop << "," << totalDistrict.countP << "\n";
            myfile9 << "stats.patient" << ",s.untested.out" << ",s.untested.in" << ",s.under.treat" << ",failed.treat" << ",s.untreated.false.neg" << ",s.untreated.no.sample" << ",s.untreated.ltof";
            myfile9 << ",cured" << ",infec.treat" << ",uninfec.treat" << ",other";
            myfile9 << ",non.infected.not.treated" << ",non.infected.not.returned" << ",dead.infected.treated" << ",dead.infected.untreated" << ",dead.uninfected" << "\n";
            myfile9 << "," << totalDistrict.countPU << "," << totalDistrict.countPI << "," << totalDistrict.countPT << "," << totalDistrict.countPR;
            myfile9 << "," << totalDistrict.countPUTFN << "," << totalDistrict.countPUTNS << "," << totalDistrict.countPUTLtoF << "," << totalDistrict.countPC;
            myfile9 << "," << totalDistrict.countPInfTreated << "," << totalDistrict.countPUninfTreated << "," << totalDistrict.countPO;
            myfile9 << "," << totalDistrict.countPUI << "," << totalDistrict.countPUINR << "," << totalDistrict.countPDI << "," << totalDistrict.countPDIUT << "," << totalDistrict.countPDU << "\n";
            myfile9 << "\n";
            
            myfile3 << "-----------------------------------------------------------------------------------  " << "\n";
            myfile3 << "stats.total.district.population" << ",patients" << "\n";
            myfile3 <<  newPop << "," << totalDistrict.countP << "\n";
            myfile3 << "stats.patient" << ",s.untested.out" << ",s.untested.in" << ",s.under.treat" << ",failed.treat" << ",s.untreated.false.neg" << "s.,untreated.no.sample" << ",s.untreated.ltof";
            myfile3 <<  ",cured" << ",infec.treat" << ",uninfec.treat" << ",other";
            myfile3 << ",non.infected.not.treated" << ",non.infected.not.returned" << ",dead.infected.treated" << ",dead.infected.untreated" << ",dead.uninfected" << "\n";
            myfile3 << "," << totalDistrict.countPU << "," << totalDistrict.countPI << "," << totalDistrict.countPT << "," << totalDistrict.countPR;
            myfile3 << "," << totalDistrict.countPUTFN << "," << totalDistrict.countPUTNS << "," << totalDistrict.countPUTLtoF << "," << totalDistrict.countPC;
            myfile3 << "," << totalDistrict.countPInfTreated << "," << totalDistrict.countPUninfTreated << "," << totalDistrict.countPO;
            myfile3 << "," << totalDistrict.countPUI << "," << totalDistrict.countPUINR << "," << totalDistrict.countPDI << "," << totalDistrict.countPDIUT << "," << totalDistrict.countPDU << "\n";
            myfile3 << "\n";
            myfile3.close();
        }
    }

    //if (debug) std::cout << "stats: inside agent stats mark 3 total_infected = " << infected << "\n";

    // DALY calculation is yyl = (no. of patients dead from disease / .03) * (1 - e^-(.03*life expectancy))
    //                           (no. of patients dead from disease / .03) * (1 - e^-(.03*63))
    //                           (no. of patients dead from disease) * 5.035733
    yll = (total.countPDI + total.countPDIUT) * 5.035733;

    // update population by population growth if it goes on more that 1 year
    newPop = total.pop;
    startIndex = referenceYear - 2015;
    endIndex = startIndex + noOfYears;
    if (noOfYears > 0) {

        for (int i = startIndex; i < endIndex; i++) {

            // multiply by growth rate for x number of years
            newPop = (int) (newPop * popGrowthRateArray[i][1]);
        }
    }


    if (fileFlag) {
        myfile << statHeader1 << statHeader2 << statHeader3 << statHeader4 << statHeader5 << statHeader5a << statHeader6 << statHeader7 << statHeader8;
        myfile << statHeader9 << statHeader10 << statHeader10a << statHeader11 << statHeader12 << statHeader12a << statHeader12b;
        myfile << statHeader13 << statHeader13a << statHeader13b << statHeader13c << statHeader13d << statHeader14 << statHeader15 << statHeader16 << statHeader17;

        // count up all the sample results
        totalSampleCountPos = total.countTruePos + total.countFalsePos;
        totalSampleCountNeg = total.countTrueNeg + total.countFalseNeg;

        //treated = total.countPT + total.countPR + total.countPC + total.countPDI;
        //infected = treated + total.countPUT + total.countPDIUT;
        //treated = total.countPT + total.countPC;
        //infected = treated + total.countPUT + total.countPDIUT + total.countPR + total.countPDI;

        //myfile << "stats: fac =  " << countF << ", population = " << total.pop << ", patients = " << total.countP << ",      DALY YLL = " << yll << "\n";
        //myfile << "stats: patient" << " Untested-out = " << total.countPU << " Untested-in =  " << total.countPI << ", under treat = " << total.countPT << ", failed treat = " << total.countPR << ", untreated = " << total.countPUT << ", cured = " << total.countPC << ", uninfec-treat = " << total.countPUIUT << ", Other = " << total.countPO << "\n";
        //myfile << "               Non-infected - not treated  = " << total.countPUI << ", Non-infected - not returned = " << total.countPUINR << ", dead - infected = " << total.countPDI << ", dead - infected untreated = " << total.countPDIUT << ", dead - uninfected = " << total.countPDU << "\n";
        myfile << countF << "," << "," << total.tier << "," << total.district << "," << total.region << "," << newPop << "," << "" << "," << yll;
        myfile << "," << total.countP << "," << total.countPInf << "," << total.countPLtoF;
        myfile << "," << total.outCollectedCount + total.inCollectedCount << "," << total.outCollectedCount << "," << total.inCollectedCount;
        if ((total.sampleWaitForResourceInCount + total.sampleWaitForResourceOutCount) < 1) {
            myfile << "," << "0";
        }
        else {
            myfile << "," << float(total.sampleWaitForResourceInTime + total.sampleWaitForResourceOutTime)/float(total.sampleWaitForResourceInCount + total.sampleWaitForResourceOutCount);
        }
        if (total.sampleWaitForResourceInCount < 1) {
            myfile << "," << "0";
        }
        else {
            myfile << "," << float(total.sampleWaitForResourceInTime)/float(total.sampleWaitForResourceInCount);
        }
        if (total.sampleWaitForResourceOutCount < 1) {
            myfile << "," << "0" << ",";
        }
        else {
            myfile << "," << float(total.sampleWaitForResourceOutTime)/float(total.sampleWaitForResourceOutCount) << ",";
        }
        if (total.sampleTransCount  < 1) {
            myfile << "0" << "," << total.sampleTransCount;
        }
        else {
            myfile << float(total.sampleTransTime)/float(total.sampleTransCount) << "," << total.sampleTransCount;
        }
        myfile << "," << total.sampleTestedInCount + total.sampleTestedOutCount << "," << total.sampleTestedInCount << "," << total.sampleTestedOutCount;
        //myfile << "," << total.sampleLtoFCount;
        if (total.sampleCollectToTestInCount < 1) {
            myfile << "," << "0";
        }
        else {
            myfile << "," << float(total.sampleCollectToTestInTime)/float(total.sampleCollectToTestInCount);
        }
        myfile << "," << float(total.sampleCollectToTestInTime) << "," << total.sampleCollectToTestInCount;
        if (total.sampleCollectToTestOutCount < 1) {
            myfile << "," << "0";
        }
        else {
            myfile << "," << float(total.sampleCollectToTestOutTime)/float(total.sampleCollectToTestOutCount);
        }
        myfile << "," << float(total.sampleCollectToTestOutTime) << "," << total.sampleCollectToTestOutCount;
        myfile << "," << total.countTestPos << ",";
        if (totalSampleCountPos > 0) {
            myfile << float(total.countTruePos)/float(totalSampleCountPos) << ",";
            myfile << float(total.countFalsePos)/float(totalSampleCountPos) << ",";
        }
        else {
            myfile << "0" << "," << "0" << ",";
        }
        myfile << total.countTestNeg << ",";
        if (totalSampleCountNeg > 0) {
            myfile << float(total.countTrueNeg)/float(totalSampleCountNeg) << ",";
            myfile << float(total.countFalseNeg)/float(totalSampleCountNeg);
        }
        else {
            myfile << "0" << "," << "0";
        }
        if (debug2) myfile << "," << "marker";

        myfile << "," << total.countMDInf << "," << total.countMDUninf;
        myfile << "," << total.countMDInf + total.countMDUninf + total.countTestPos;
        myfile << "," << total.countFalsePos << "," << total.countFalseNeg;
        myfile << "," << total.countDiagnosis << "," << total.countDiagnosisIn << "," << total.countDiagnosisOut << "," << total.countPreTestLtoFOut << "," << total.countPreTestLtoFOutMinusTestedSamp << "," << total.countPreTestLtoFDead << "," << total.countPreTestLtoFORepresent;
        if (total.sampleTestPosCountIn < 1) {
            myfile << "," << "0";
        }
        else {
            myfile << "," << float(total.sampleTestPosTimeIn)/float(total.sampleTestPosCountIn);
        }
        myfile << "," << float(total.sampleTestPosTimeIn) << "," << total.sampleTestPosCountIn;
        if (total.sampleTestPosCountOut < 1) {
            myfile << "," << "0";
        }
        else {
            myfile << "," << float(total.sampleTestPosTimeOut)/float(total.sampleTestPosCountOut);
        }
        myfile << "," << float(total.sampleTestPosTimeOut) << "," << total.sampleTestPosCountOut;
        if(debug2) myfile << "," << "marker";
               
        myfile << "," << total.countPInfTreated+ total.countPUninfTreated << "," << total.countPInfTreated << "," << total.countPUninfTreated << "," << total.countPTC << "," << total.countPTD << "," << total.countPTR  << "," << total.countRetreat << "," << total.countRetreatCure << "," << total.countRetreatDead;
        if ((total.countPInfTreatedIn + total.countPInfTreatedOut) < 1) {
            myfile << "," << "0";
        }
        else {
            myfile << "," << float (total.onsetToTreat)/float(total.countPInfTreatedIn + total.countPInfTreatedOut);
        }
        myfile << "," << float (total.onsetToTreat) << "," << (total.countPInfTreatedIn + total.countPInfTreatedOut);
        if (total.countPInfTreatedIn < 1) {
            myfile << "," << "0";
        }
        else {
            myfile << "," << float (total.onsetToTreatIn)/float(total.countPInfTreatedIn);
        }
        myfile << "," << float (total.onsetToTreatIn) << "," << total.countPInfTreatedIn;
        if (total.countPInfTreatedOut < 1) {
            myfile << "," << "0";
        }
        else {
            myfile << "," << float (total.onsetToTreatOut)/float(total.countPInfTreatedOut);
        }
        myfile << "," << float (total.onsetToTreatOut) << "," << total.countPInfTreatedOut;
        if(debug2) myfile << "," << "marker";
               
        if (total.sampleReturnToDeathCount < 1) {
            myfile << "," << "0";
        }
        else {
            myfile << "," << float (total.sampleReturnToDeathTime)/float(total.sampleReturnToDeathCount);
        }
        myfile << "," << float (total.sampleReturnToDeathTime) << "," << total.sampleReturnToDeathCount;
        if (total.sampleReturnToDeathPreDiagLtoFCount < 1) {
            myfile << "," << "0";
        }
        else {
            myfile << "," << float (total.sampleReturnToDeathPreDiagLtoFTime)/float(total.sampleReturnToDeathPreDiagLtoFCount);
        }
        myfile << "," << float (total.sampleReturnToDeathPreDiagLtoFTime) << "," << total.sampleReturnToDeathPreDiagLtoFCount;
        if (total.sampleReturnToDeathInTrtCount < 1) {
            myfile << "," << "0";
        }
        else {
            myfile << "," << float (total.sampleReturnToDeathInTrtTime)/float(total.sampleReturnToDeathInTrtCount);
        }
        myfile << "," << float (total.sampleReturnToDeathInTrtTime) << "," << total.sampleReturnToDeathInTrtCount;
        if (total.sampleReturnToDeathInRetrtCount < 1) {
            myfile << "," << "0";
        }
        else {
            myfile << "," << float (total.sampleReturnToDeathInRetrtTime)/float(total.sampleReturnToDeathInRetrtCount);
        }
        myfile << "," << float (total.sampleReturnToDeathInRetrtTime) << "," << total.sampleReturnToDeathInRetrtCount;
        if (total.sampleReturnToExitCount < 1) {
            myfile << "," << "0";
        }
        else {
            myfile << "," << float (total.sampleReturnToExitTime)/float(total.sampleReturnToExitCount);
        }
        myfile << "," << float (total.sampleReturnToExitTime) << "," << total.sampleReturnToExitCount;
        myfile << "," << total.test1CountIn + total.test1CountOut << "," << total.test1CountIn << "," << total.test1CountOut << "," << total.test2Count << "," << total. test2PosCount;
        if (total.test2PosCount < 1) {
            myfile << "," << "0";
        }
        else {
            myfile << "," << float (total.test2TruePosCount)/float(total.test2PosCount);
        }
        myfile << "," << total.test2NegCount;
        if (total.test2NegCount < 1) {
            myfile << "," << "0";
        }
        else {
            myfile << "," << float (total.test2TrueNegCount)/float(total.test2NegCount);
        }
        myfile << "," << total.test1PosCount;
        if (total.test1PosCount < 1) {
            myfile << "," << "0";
        }
        else {
            myfile << "," << float (total.test1TruePosCount)/float(total.test1PosCount);
        }
        myfile << "," << total.test1NegCount;
        if (total.test1NegCount < 1) {
            myfile << "," << "0";
        }
        else {
            myfile << "," << float (total.test1TrueNegCount)/float(total.test1NegCount);
        }
        if(debug2) myfile << "," << "marker";
               
        myfile << "," << total.countP << "," << total.countPI << "," << total.countPU  << "," << total.countPT;
        myfile << "," << total.sampleWaitTestCount << "," << total.sampleWaitTestCountIn << "," << total.sampleWaitTestCountOut;
        myfile << "," << total.countPreTestLtoFOutTested << "," << total.patientNotNotified << "," << total.patientNotNotifiedPos << "\n";

        myfile.close();
        myfile6.close();
        myfile7.close();
        myfile8.close();

    }
    else {
        std::cout << "stats: fac =  " << countF << ", population = " << newPop << ", patients = " << total.countP << ",      DALY YLL = " << yll << "\n";
        std::cout << "stats: patient" << " untested.out = " << total.countPU << " untested.in =  " << total.countPI << ", under treat = " << total.countPT << ", failed treat = " << total.countPR << ", untreated = " << total.countPUT << ", cured = " << total.countPC << ", uninfec-treat = " << total.countPUIUT << ", Other = " << total.countPO << "\n";
        std::cout << "               Non-infected - not treated  = " << total.countPUI << ", Non-infected - not returned = " << total.countPUINR << ", dead - infected = " << total.countPDI <<", dead - infected untreated = " << total.countPDIUT << ", dead - uninfected = " << total.countPDU << "\n";

    }

    if (individualFlag) {

        //treated = total.countPT + total.countPR + total.countPC + total.countPDI;
        //treated = total.countPT + total.countPC;
        
        myfile9 << "stats.fac" << ",population" << ",patients" << ",daly.yll" << "\n";
        myfile9 << countF << "," << newPop << "," << total.countP << "," << yll << "\n";
        myfile9 << "stats.patient" << ",s.untested.out" << ",s.untested.in" << ",s.under.treat" << ",failed.treat" << ",s.untreated.true.neg" << ",s.untreated.no.sample" << ",s.untreated.ltof";
        myfile9 << ",cured" << ",infec-treat" << ",uninfec.treat" << ",other";
        myfile9 << ",non.infected.not.treated" << ",non.infected.not.returned" << ",dead.infected.treated" << ",dead.infected.untreated" << ",dead.uninfected" << "\n";
        myfile9 << "," << total.countPU << "," << total.countPI << "," << total.countPT << "," << total.countPR << "," << total.countPUTFN << "," << total.countPUTNS << "," << total.countPUTLtoF;
        myfile9 << "," << total.countPC  << "," << total.countPInfTreated << "," << total.countPUninfTreated << "," << total.countPO;
        myfile9 << "," << total.countPUI << "," << total.countPUINR << "," << total.countPDI << "," << total.countPDIUT << "," << total.countPDU << "\n";
        
        myfile1 << "stats.fac" << ",population" << ",patients" << ",daly.yll" << "\n";
        myfile1 << countF << "," << newPop << "," << total.countP << "," << yll << "\n";
        myfile1 << "stats.patient" << ",s.untested.out" << ",s.untested.in" << ",s.under.treat" << ",failed.treat" << ",s.untreated.true.neg" << ",s.untreated.no.sample" << ",s.untreated.ltof";
        myfile1 << ",cured" << ",infec.treat" << ",uninfec.treat" << ",other";
        myfile1 << ",non.infected.not.treated" << ",non.infected.not.returned" << ",dead.infected.treated" << ",dead.infected.untreated" << ",dead.uninfected" << "\n";
        myfile1 << "," << total.countPU << "," << total.countPI << "," << total.countPT << "," << total.countPR << "," << total.countPUTFN << "," << total.countPUTNS << "," << total.countPUTLtoF;
        myfile1 << "," << total.countPC  << "," << total.countPInfTreated << "," << total.countPUninfTreated << "," << total.countPO;
        myfile1 << "," << total.countPUI << "," << total.countPUINR << "," << total.countPDI << "," << total.countPDIUT << "," << total.countPDU << "\n";
        
        if (debug) std::cout << "stats: inside agent stats at end - withTB = " << withTB << "\n";
        
        myfile1.close();
        myfile4.close();
        myfile5.close();
        myfile9.close();
        myfile10.close();
        myfile11.close();
    }

    if (individualSamplingFlag) {
        
        myfile12.close();
    }

    if (debug) std::cout << "stats: inside agent stats at end"  << "\n";
}




// count how many Patients currently in existence 
void dumpPatientInfo(Patient* pat,int timeStep) {

    string fileName;
    Sample* samp;

    // get Sample pointer
    samp = pat->getSamplePtr();

    // dump patient info to file
    ofstream myfile33;

    fileName = "./stats/patientInfo" + to_string(timeStep) + ".csv";

    myfile33.open(fileName, std::ios_base::app);

    //get facility name where patient presented
    myfile33 << "patient home facility";

    //get whether patient is infected or not
    myfile33 << ",infected";

    //get distance 
    myfile33 << ",distance home facility";

    //get whether patient is infected or not
    myfile33 << ",drug resistance";

    //is patient alive
    myfile33 << ",is alive";

    //get patient active status
    myfile33 << ",is active";

    //is patient treated
    myfile33 << ",is treated";

    //get status 
    myfile33 << ",status";

    //get previous status 
    myfile33 << ",previous status";

    //get pre-diagnostic ltof flag
    myfile33 << ",pre-test ltfu";
    
    //get pre-diagnostic ltof flag
    myfile33 << ",pre-test ltfu count";

    //get relapse status flag
    myfile33 << ",relapse";

    //get reason infected patient didn't receive treatement 
    myfile33 << ",reason no-treat";

    //get reason patient died 
    myfile33 << ",reason died";

    //get test result status 
    myfile33 << ",test pos";

    //get doctor's diagnosis status 
    myfile33 << ",MD pos";

    //get age
    myfile33 << ",age";

    //get gender
    myfile33 << ",gender";

    //get disease associated with Patient
    myfile33 << ",disease";

    //get patient life expectancy
    myfile33 << ",life expectancy";
    
    //increment time from onset
    myfile33 << ",time onset -treat";
    
    //get time from getting results
    myfile33 << ",time in treat";

    // get time from getting results
    myfile33 << ",time not in treat";

    // get time of death
    myfile33 << ",time of death";

    // get time from getting results
    myfile33 << ",time from onset";

    // get time from getting results
    myfile33 << ",no. visits";

    // get timestep patient was created
    myfile33 << ",create time";

    // get timestep patient was created
    myfile33 << ",inactive time";

    // get time step sample was created
    myfile33 << ",sample create time";

    // get time step relapse treatment should start
    myfile33 << ",relapse time";

    // get time step test results were returned
    myfile33 << ",results returned time";

    // get time step test results were returned
    myfile33 << ",treat start time";

    // get time step test results were returned
    myfile33 << ",patient received results time";

    // get time step test results were returned
    myfile33 << ",MD results received time";

    // get time step rinfectiousnes for patient ends
    myfile33 << ",infected end time";

    // get time step test results were returned
    myfile33 << ",LtFU time";  

    // get no. of time steps to wait to return to facility to get results
    myfile33 << ",facility return time";

    // get no of times returned for sample results
    myfile33 << ",no. time return for results";

    // get infectiousness
    myfile33 << ",infectiousness";

    // get immunity
    myfile33 << ",immunity";

    //get test name used on sample
    myfile33 << ",sample test name";

     //get test name used on sample
    myfile33 << ",sample test status";

     //get test name used on sample
    myfile33 << ",sample test facility";

    myfile33 << ",xx";
    
    // get original facility
    myfile33 << ",sample orig facility";

    // get testing facility name
    myfile33 << ",sample testing facility";

    // get current status
    myfile33 << ",sample status";
    
    // get previous status of sample
    myfile33 << ",previous status";

    // get status of patient if infected or not
    myfile33 << ",infected";

    // set result of Sample - positive or negative
    myfile33 << ",positive";

    // get age
    myfile33 << ",age";

    // get transit time to testing
    myfile33 << ",transit time";

    // get transit time to testing and save for reference
    myfile33 << ",save transit time";

    // get time sample was tested
    myfile33 << ",tested time";

    // get transit time to return sample
    myfile33 << ",return transit time";

    // get transit time to return sample for reference
    myfile33 << ",return save transit time";

    // get percentage of lost samples
    myfile33 << ",per lost samples";

    // get time step sample was created
    myfile33 << ",create time";

    // get time step test results were returned to original facility
    myfile33 << ",time returned to orig facility";
    
    // get time step test results were returned
    myfile33 << ",treat recieved time";
    
    // get time step test results were returned
    myfile33 << ",time test result returned";

    // get time steps in batch cycle before sending out for testing
    myfile33 << ",batch wait time";

    // get time steps waiting in batch cycle before sending out for testing
    myfile33 << ",ime waiting in batch";

    // get time steps waiting
    myfile33 << ",waiting time";

    // get time waiting for resources
     myfile33 << ",time waiting for resource";

    // get priority of test, lower is higher priority
     myfile33 << ",priority";

    // get distance to testing facility from patient facility
    myfile33 << ",testing facility distance";

    // get name of disease for this sample to be tested
    myfile33 << ",disease";

    // get name of test used on this sample
    myfile33 << ",test name";

    // get pointer for test used on this sample
    //inline Test* getTestPtr() { return _testPtr; };

     myfile33 << "\n";
    
    //get facility name where patient presented
    myfile33 << pat->getFacility();

    //get whether patient is infected or not
    myfile33 << "," << pat->getInfected();

    //get distance 
    myfile33 << "," << pat->getDistance();

    //get whether patient is infected or not
    myfile33 << "," << pat->isDrugResistant();

    //is patient alive
    myfile33 << "," << pat->isAlive();

    //get patient active status
    myfile33 << "," << pat->getActive();

    //is patient treated
    myfile33 << "," << pat->getTreated();

    //get status 
    myfile33 << "," << pat->getStatus();

    //get previous status 
    myfile33 << "," << pat->getPrevStatus();

    //get pre-diagnostic ltof flag
    myfile33 << "," << pat->getPreTestLtoFFlag();
    
    //get pre-diagnostic ltof flag
    myfile33 << "," << pat->getPreTestLtoFCount();

    //get relapse status flag
    myfile33 << "," << pat->getRelapseFlag();

    //get reason infected patient didn't receive treatement 
    myfile33 << "," << pat->getReasonNoTreat();

    //get reason patient died 
    myfile33 << "," << pat->getReasonDied();

    //get test result status 
    myfile33 << "," << pat->getTestPos();

    //get doctor's diagnosis status 
    myfile33 << "," << pat->getMDPos();

    //get age
    myfile33 << "," << pat->getAge();

    //get gender
    myfile33 << "," << pat->getGender();

    //get disease associated with Patient
    myfile33 << "," << pat->getDisease();

    //get patient life expectancy
    myfile33 << "," << pat->getLifeExpectancy();
    
    //increment time from onset
    myfile33 << "," << pat->getOnsetTilTreatment();
    
    //get time from getting results
    myfile33 << "," << pat->getTimeStepsInTreatment();

    // get time from getting results
    myfile33 << "," << pat->getTimeStepsNotInTreatment();

    // get time of death
    myfile33 << "," << pat->getTimeOfDeath();

    // get time from getting results
    myfile33 << "," << pat->getTimeFromOnset();

    // get time from getting results
    myfile33 << "," << pat->getNoVisits();

    // get timestep patient was created
    myfile33 << "," << pat->getTimestepPresented();

    // get timestep patient was created
    myfile33 << "," << pat->getTimestepInactive();

    // get time step sample was created
    myfile33 << "," << pat->getCreateTime();

    // get time step relapse treatment should start
    myfile33 << "," << pat->getRelapseTime();

    // get time step test results were returned
    myfile33 << "," << pat->getResultsReturnedTime();

    // get time step test results were returned
    myfile33 << "," << pat->getStartedTreatmentTime();

    // get time step test results were returned
    myfile33 << "," << pat->getPatientRecievedResultTime();

    // get time step test results were returned
    myfile33 << "," << pat->getPatientRecievedMDResultTime();

    // get time step rinfectiousnes for patient ends
    myfile33 << "," << pat->getInfectEndTime();

    // get time step test results were returned
    myfile33 << "," << pat->getTimePatientLtoF();  

    // get no. of time steps to wait to return to facility to get results
    myfile33 << "," << pat->getFacilityReturnTime();

    // get no of times returned for sample results
    myfile33 << "," << pat->getNoReturnsForResults();

    // get infectiousness
    myfile33 << "," << pat->getinfectiousness();

    // get immunity
    myfile33 << "," << pat->getImmunity();

    //get test name used on sample
    myfile33 << "," << pat->getSampleTest();

     //get test name used on sample
    myfile33 << "," << pat->getSampleTestStatus();

     //get test name used on sample
    myfile33 << "," << pat->getSampleTestFacility();

    myfile33 << ",xx";


    // sample info

    // get original facility
    myfile33 << "," << samp->getOrigFacility();

    // get testing facility name
    myfile33 << "," << samp->getTestingFacility();

    // get current status
    myfile33 << "," << samp->getStatus();
    
    // get previous status of sample
    myfile33 << "," << samp->getPrevStatus();

    // get status of patient if infected or not
    myfile33 << "," << samp->getInfected();

    // set result of Sample - positive or negative
    myfile33 << "," << samp->getPositive();

    // get age
    myfile33 << "," << samp->getAge();

    // get transit time to testing
    myfile33 << "," << samp->getTransitTime();

    // get transit time to testing and save for reference
    myfile33 << "," << samp->getTransitTimeSave();

    // get time sample was tested
    myfile33 << "," << samp->getTestedTime();

    // get transit time to return sample
    myfile33 << "," << samp->getReturnTransitTime();

    // get transit time to return sample for reference
    myfile33 << "," << samp->getReturnTransitTimeSave();

    // get percentage of lost samples
    myfile33 << "," << samp->getPerLostSamples();

    // get time step sample was created
    myfile33 << "," << samp->getCreateTime();

    // get time step test results were returned to original facility
    myfile33 << "," << samp->getResultsReturnedToOrigFacilityTime();

    // get time step test results were recieved at original facility
    myfile33 << "," << samp->getResultsRecievedToOrigFacilityTime();
    
    // get time step test results were returned
    myfile33 << "," << samp->getResultsReturnedTime();

    // get time steps in batch cycle before sending out for testing
    myfile33 << "," << samp->getBatchWaitTime();
    
    // get time steps waiting in batch cycle before sending out for testing
    myfile33 << "," << samp->getBatchWaitTimeSave();

    // get time steps waiting
    myfile33 << "," << samp->getWait();

    // get time steps waiting for resources
    myfile33 << "," << samp->getWaitForResource();

    // get priority of test, lower is higher priority
    myfile33 << "," << samp->getPriority();

    // get distance to testing facility from patient facility
    myfile33 << "," << samp->getTestingDistance();

    // get name of disease for this sample to be tested
    myfile33 << "," << samp->getDisease();

    // get name of test used on this sample
    myfile33 << "," << samp->getTest();

    // get pointer for test used on this sample
    //inline Test* getTestPtr() { return _testPtr; };

    myfile33 << "\n";

    myfile33.close();


}
