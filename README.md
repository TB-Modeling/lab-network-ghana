## Running the ABM


1. The ABM is written in C++ and compiled with g++ in the Linux OS. All the .h and .cpp files reside in the main directory.  There is also a makefile to help facilitate building.  The makefile will build main(.exe).  This file will use various input files that are stored in ./defaults/input/xxxx sub-directories. Check the Input-Files.docx file to get a list of the input files and their uses. 
2. In order to run the ABM out of main menu, you need to execute the main executable.  This is done by running ./main arg1 arg2 keyword in a Linux environment. This will put output in the ./stats/scenario/job_no. directory for each scenario.  You will need a copy of the ABM program (main.exe), the ./defaults/input sub-directory and an empty ./stats sub-directory.  Make sure the directory that contains the executable (main.exe) also contains the ./stats and ./defaults sub-directories. Keywords control the output of different statistical outputs to the ./stats/scenario/job no. directory.  Including the desired keyword will output the appropriate statistical file.

Example - ./main 1 7 8 9 10 keywords

Argument 1 = job_no. This is the sub-directory in the ./stats/scenario sub-directory where the output will be placed

Argument 2-4 = scenario numbers to run –the scenarios in the following table are the defaults used in the model. They can be changed by changing the input files in ./defaults/input/input-file-names/input-file-names.csv file.  The scenarios listed (7,8,9 and 10) are the current valid scenarios for this model.

<table>

<tr><th style="width:33%">Scenario no.</th><th>Xpert tiers</th><th>smear tiers</th><th>Clinical diagnosis tiers</th><th>Notes</th></tr>
<tr><td>7</td><td>1, 2, 3</td><td>none</td><td>All</td></tr>
<tr><td>8</td><td>1, 2, 3, 4</td><td>none</td><td>All</td></tr> 
<tr><td>9 Baseline (170)</td><td>1, 2, 3 and some 4</td><td>none</td><td>All</td></tr>
<tr><td>10</td><td>All</td><td>none</td><td>All</td></tr>
<tr><td>11 Base+DNO</td><td>1, 2, 3 and some 4</td><td>none</td><td>All</td></tr>
<tr><td>21 Base to Central</td><td>Start 9 switch to 7</td><td>none</td><td>All</td></tr>
<tr><td>22 Base to Decent</td><td>Start 9 switch to 8</td><td>none</td><td>All</td></tr>
<tr><td>23 Base to Full Decent</td><td>Start 9 switch to 10</td><td>none</td><td>All</td></tr>
<tr><td>24 Base to DNO</td><td>Start 9 switch to 11</td><td>none</td><td>All</td></tr>
<tr><td>91 Baseline (170)</td><td>1, 2, 3 and some 4</td><td>none</td><td>All</td><td>adds 10000 to job no.</td></tr>
<tr><td>92 Baseline (170)</td><td>1, 2, 3 and some 4</td><td>none</td><td>All</td><td>adds 20000 to job no.</td></tr>
<tr><td>93 Baseline (170)</td><td>1, 2, 3 and some 4</td><td>none</td><td>All</td><td>adds 30000 to job no.</td></tr>
<tr><td>94 Baseline (170)</td><td>1, 2, 3 and some 4</td><td>none</td><td>All</td><td>adds 40000 to job no.</td></tr>

</table>

Keywords = **SampleStats**, **ResourceStats**, **VisualizationStats**, **TableauStats**, **PatientStats**, **PatientSamplingStats**.  The different keywords will output different statistics from the model.

The ABM will create output files: **AgentStats.csv**, **AgentStats-Region.csv**, **AgentStats-District.csv** and Input parameters.  **AgentStatsXXXX.csv** files hold the year by year statistical totals for each disease, broken down at the national, regional and district results.  The file **InputParameters.csv** will also be created that contains the randomized parameter values used for this job. The ABM will copy all the input files used for this run from ./defaults/input to a ./stats/scenario/job_no/input sub-directory.

**In order to run statistic compilation after multiple ABM runs, the user will need to run stat_comp.exe executable.**  It runs out of ./stat-compilation sub-directory.  It can be built using the makefile and .h and .cpp files in the ./stat-compilation sub-directory.  It assumes that the output files for each scenario exist in ./stats/scenario sub-directory. It will store the aggregate and individual output in ./stats/scenario directory.

Example - Stat_comp 1 2 3

argument 1 = number of jobs to compile
argument 2 = number of years in each job
argument 3 = scenario – number of scenario that run using the model runs. Scenario numbers between 1 and 24. Same as #2 above.

The routine will create files **AgentStats-Aggregate.csv** and **AgentStats-Aggregate-_MC.csv**, **AgentStats-Region-Aggregate.csv** and **AgentStats-Region-Aggregate-_MC.csv**, **AgentStats-District-Aggregate.csv** and **AgentStats-District-Aggregate-_MC.csv**.  The first file will compile averages for all the statistical values.  The second file contains output from each of the runs for Monte Carlo analysis. 



