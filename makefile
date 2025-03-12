CC=g++
DEBUG=-g -Og
OPT=-ftree-vectorize -msse2 -O3 #-Ofast
CFLAGS= -c -std=c++0x -I/usr/local/include -I/usr/include -I/lib -I/repos/GhanaFacilities/boost -I/repos/GhanaFacilities/boost/math -I/repos/GhanaFacilities/boost/math/distributions -I/repos/GhanaFacilities/boost/filesystem -I/repos/GhanaFacilities/boost/filesystem/detail -I. -Wall #-Wno-unused-but-set-variable -Wno-unused-variable
LDFLAGS= -L/usr/local/lib -lm -pthread


all: main.o

debug: main_debug.o

main_debug.o: GlobalParameters_D.o Test_D.o Patient_D.o Sample_D.o Diagnosis_D.o Testing_D.o FacilitiesList_D.o Facility_D.o District_D.o Region_D.o Disease_D.o InputOutput_D.o InputChange_D.o ChangeParameters_D.o CreateAgents_D.o CreateAgentStats_D.o CreateSampleStats_D.o CreateResourceStats_D.o CreateParameterStats_D.o CreateTableauStats_D.o CreateVisualizationStats_D.o MapFacilities_D.o Dump_D.o Clean_D.o  Stats_D.o Main_D.o
	$(CC) $(DEBUG) GlobalParameters_D.o Test_D.o Patient_D.o Sample_D.o Diagnosis_D.o Testing_D.o FacilitiesList_D.o Facility_D.o District_D.o Region_D.o Disease_D.o InputOutput_D.o InputChange_D.o ChangeParameters_D.o CreateAgents_D.o CreateAgentStats_D.o CreateSampleStats_D.o CreateResourceStats_D.o CreateParameterStats_D.o CreateTableauStats_D.o CreateVisualizationStats_D.o MapFacilities_D.o Dump_D.o Clean_D.o Stats_D.o Main_D.o -o main_debug.o $(LDFLAGS)

GlobalParameters_D.o: GlobalParameters.cpp GlobalParameters.h
	$(CC) $(CFLAGS) $(DEBUG) GlobalParameters.cpp -o GlobalParameters_D.o

Person_D.o: Person.cpp Person.h
	$(CC) $(CFLAGS) $(DEBUG) Person.cpp -o Person_D.o

Household_D.o: Household.cpp Household.h
	$(CC) $(CFLAGS) $(DEBUG) Household.cpp -o Household_D.o

Population_D.o: Population.cpp Population.h
	$(CC) $(CFLAGS) $(DEBUG) Population.cpp -o Population_D.o

Functions_D.o: Functions.cpp Functions.h
	$(CC) $(CFLAGS) $(DEBUG) Functions.cpp -o Functions_D.o

Test_D.o: Test.cpp Test.h
	$(CC) $(CFLAGS) $(DEBUG) Test.cpp -o Test_D.o

Params_D.o: Params.cpp Params.h
	$(CC) $(CFLAGS) $(DEBUG) Params.cpp -o Params_D.o

Driver_D.o: Driver.cpp Driver.h
	$(CC) $(CFLAGS) $(DEBUG) Driver.cpp -o Driver_D.o

Sample_D.o: Sample.cpp Sample.h
	$(CC) $(CFLAGS) $(DEBUG) Sample.cpp -o Sample_D.o

Patient_D.o: Patient.cpp Patient.h
	$(CC) $(CFLAGS) $(DEBUG) Patient.cpp -o Patient_D.o

Diagnosis_D.o: Diagnosis.cpp Diagnosis.h
	$(CC) $(CFLAGS) $(DEBUG) Diagnosis.cpp -o Diagnosis_D.o

Testing_D.o: Testing.cpp Testing.h
	$(CC) $(CFLAGS) $(DEBUG) Testing.cpp -o Testing_D.o

FacilitiesList_D.o: FacilitiesList.cpp FacilitiesList.h
	$(CC) $(CFLAGS) $(DEBUG) FacilitiesList.cpp -o FacilitiesList_D.o

Facility_D.o: Facility.cpp Facility.h
	$(CC) $(CFLAGS) $(DEBUG) Facility.cpp -o Facility_D.o

District_D.o: District.cpp District.h
	$(CC) $(CFLAGS) $(DEBUG) District.cpp -o District_D.o

Region_D.o: Region.cpp Region.h
	$(CC) $(CFLAGS) $(DEBUG) Region.cpp -o Region_D.o

Disease_D.o: Disease.cpp Disease.h
	$(CC) $(CFLAGS) $(DEBUG) Disease.cpp -o Disease_D.o

InputOutput_D.o: InputOutput.cpp InputOutput.h
	$(CC) $(CFLAGS) $(DEBUG) InputOutput.cpp -o InputOutput_D.o

InputChange_D.o: InputChange.cpp InputChange.h
	$(CC) $(CFLAGS) $(DEBUG) InputChange.cpp -o InputChange_D.o

ChangeParameters_D.o: ChangeParameters.cpp ChangeParameters.h
	$(CC) $(CFLAGS) $(DEBUG) ChangeParameters.cpp -o ChangeParameters_D.o

CreateAgents_D.o: CreateAgents.cpp CreateAgents.h
	$(CC) $(CFLAGS) $(DEBUG) CreateAgents.cpp -o CreateAgents_D.o

CreateAgentStats_D.o: CreateAgentStats.cpp CreateAgentStats.cpp 
	$(CC) $(CFLAGS) $(DEBUG) CreateAgentStats.cpp -o CreateAgentStats_D.o

CreateSampleStats_D.o: CreateSampleStats.cpp CreateSampleStats.h
	$(CC) $(CFLAGS) $(DEBUG) CreateSampleStats.cpp -o CreateSampleStats_D.o

CreateResourceStats_D.o: CreateResourceStats.cpp CreateResourceStats.h
	$(CC) $(CFLAGS) $(DEBUG) CreateResourceStats.cpp -o CreateResourceStats_D.o

CreateParameterStats_D.o: CreateParameterStats.cpp CreateParameterStats.h
	$(CC) $(CFLAGS) $(DEBUG) CreateParameterStats.cpp -o CreateParameterStats_D.o

CreateTableauStats_D.o: CreateTableauStats.cpp CreateTableauStats.h
	$(CC) $(CFLAGS) $(DEBUG) CreateTableauStats.cpp -o CreateTableauStats_D.o

CreateVisualizationStats_D.o: CreateVisualizationStats.cpp CreateVisualizationStats.h
	$(CC) $(CFLAGS) $(DEBUG) CreateVisualizationStats.cpp -o CreateVisualizationStats_D.o

MapFacilities_D.o: MapFacilities.cpp MapFacilities.h
	$(CC) $(CFLAGS) $(DEBUG) MapFacilities.cpp -o MapFacilities_D.o

Dump_D.o: Dump.cpp Dump.h
	$(CC) $(CFLAGS) $(DEBUG) Dump.cpp -o Dump_D.o

Clean_D.o: Clean.cpp Clean.h
	$(CC) $(CFLAGS) $(DEBUG) Clean.cpp -o Clean_D.o

Stats_D.o: Stats.cpp Stats.h
	$(CC) $(CFLAGS) $(DEBUG) Stats.cpp -o Stats_D.o

Main_D.o: Main.cpp
	$(CC) $(CFLAGS) $(DEBUG) Main.cpp -o Main_D.o

main.o: GlobalParameters.o  Test.o FacilitiesList.o Facility.o District.o Region.o Patient.o Sample.o Diagnosis.o Testing.o Disease.o InputOutput.o InputChange.o ChangeParameters.o CreateAgents.o CreateAgentStats.o CreateSampleStats.o CreateResourceStats.o CreateParameterStats.o CreateTableauStats.o CreateVisualizationStats.o MapFacilities.o Clean.o Dump.o Stats.o Main.o 
	$(CC) GlobalParameters.o Test.o FacilitiesList.o Facility.o District.o Region.o Patient.o Sample.o Diagnosis.o Testing.o Disease.o InputOutput.o InputChange.o ChangeParameters.o CreateAgents.o CreateAgentStats.o CreateSampleStats.o CreateResourceStats.o CreateParameterStats.o CreateTableauStats.o CreateVisualizationStats.o MapFacilities.o Clean.o Dump.o Stats.o Main.o -o main $(LDFLAGS)

GlobalParameters.o: GlobalParameters.cpp GlobalParameters.h
	$(CC) $(CFLAGS) $(OPT) GlobalParameters.cpp

Test.o: Test.cpp Test.h
	$(CC) $(CFLAGS) $(DEBUG) Test.cpp -o Test.o

Params.o: Params.cpp Params.h
	$(CC) $(CFLAGS) $(OPT) Params.cpp

Driver.o: Driver.cpp Driver.h
	$(CC) $(CFLAGS) $(OPT) Driver.cpp

Sample.o: Sample.cpp Sample.h
	$(CC) $(CFLAGS) $(OPT) Sample.cpp

Patient.o: Patient.cpp Patient.h
	$(CC) $(CFLAGS) $(OPT) Patient.cpp 

Diagnosis.o: Diagnosis.cpp Diagnosis.h
	$(CC) $(CFLAGS) $(OPT) Diagnosis.cpp 

FacilitiesList.o: FacilitiesList.cpp FacilitiesList.h
	$(CC) $(CFLAGS) $(OPT) FacilitiesList.cpp 

Facility.o: Facility.cpp Facility.h
	$(CC) $(CFLAGS) $(OPT) Facility.cpp 

District.o: District.cpp District.h
	$(CC) $(CFLAGS) $(OPT) District.cpp 

Region.o: Region.cpp Region.h
	$(CC) $(CFLAGS) $(OPT) Region.cpp 

Testing.o: Testing.cpp Testing.h
	$(CC) $(CFLAGS) $(OPT) Testing.cpp

Disease.o: Disease.cpp Disease.h
	$(CC) $(CFLAGS) $(OPT) Disease.cpp

InputOutput.o: InputOutput.cpp InputOutput.h
	$(CC) $(CFLAGS) $(OPT) InputOutput.cpp

InputChange.o: InputChange.cpp InputChange.h
	$(CC) $(CFLAGS) $(OPT) InputChange.cpp

ChangeParameters.o: ChangeParameters.cpp ChangeParameters.h
	$(CC) $(CFLAGS) $(OPT) ChangeParameters.cpp

CreateAgents.o: CreateAgents.cpp CreateAgents.h
	$(CC) $(CFLAGS) $(OPT) CreateAgents.cpp

CreateAgentStats.o: CreateAgentStats.cpp CreateAgentStats.h
	$(CC) $(CFLAGS) $(OPT) CreateAgentStats.cpp

CreateSampleStats.o: CreateSampleStats.cpp CreateSampleStats.h
	$(CC) $(CFLAGS) $(OPT) CreateSampleStats.cpp

CreateResourceStats.o: CreateResourceStats.cpp CreateResourceStats.h
	$(CC) $(CFLAGS) $(OPT) CreateResourceStats.cpp

CreateParameterStats.o: CreateParameterStats.cpp CreateParameterStats.h
	$(CC) $(CFLAGS) $(OPT) CreateParameterStats.cpp

CreateTableauStats.o: CreateTableauStats.cpp CreateTableauStats.h
	$(CC) $(CFLAGS) $(OPT) CreateTableauStats.cpp

CreateVisualizationStats.o: CreateVisualizationStats.cpp CreateVisualizationStats.h
	$(CC) $(CFLAGS) $(OPT) CreateVisualizationStats.cpp

MapFacilities.o: MapFacilities.cpp MapFacilities.h
	$(CC) $(CFLAGS) $(OPT) MapFacilities.cpp

Clean.o: Clean.cpp Clean.h
	$(CC) $(CFLAGS) $(OPT) Clean.cpp

Dump.o: Dump.cpp Dump.h
	$(CC) $(CFLAGS) $(OPT) Dump.cpp

Stats.o: Stats.cpp Stats.h
	$(CC) $(CFLAGS) $(OPT) Stats.cpp

Main.o: Main.cpp
	$(CC) $(CFLAGS) $(OPT) Main.cpp

clean:
	rm -rf *.o
