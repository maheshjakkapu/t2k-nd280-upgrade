# T2K ND280 upgrade simulation package

## Installation

1. At T2KND280Up_CLHEP.sh specify 4 correct paths at the beginning of the file:
```bash
export GEANT4_DIR=
export CLHEP_DIR=
export CMT_DIR=
export ROOT_DIR=
```
all the other paths will be assigned automatically. CLHEP could be taken from the GEANT4 installation. CMT is really necessary only if you will use highland. For the G4 sim and read out the CMT installation is not obligate.
2. Run 
```bash
source T2KND280Up_CLHEP.sh $PWD
```
3. Compile the package
```bash
cd build
source cmake_compile.sh
```

## Run the code

### Run G4 simulation
The example of the command you may run for the G4 simulation of the ND280 upgrade
```bash
./submit/submit_nd280upgrade_sim 
--nruns 40 --nexpts 2500 
--config  config/RealTarget/configuration_cluster_SuperFGD_proto2.xml 
--vis-file mac/vis_cluster_pion1000.mac
--job-dir /t2k/users/suvorov/AnalysisResults/ndUP/SuperFGD/proto2/ 
--tag proto2_pi_1000 --softw-dir /t2k/users/suvorov/dev/t2k-nd280-upgrade/
```
- ./submit/submit_nd280upgrade_sim - script from github
config/RealTarget/configuration_cluster_SuperFGD_proto2.xml - the detector configuration XML file, where you can specify the subdetectors position and size and switch some of them on/off

Notice1: ND280 basket center is hardcoded 16 mm down due to mechanical compression of the support structure. With this trick the particle gun from Y=0 will start 16 mm down by Y axis. If you want to prevent it, put the target 16 mm up along Y axis.


Notice2: File GENIE_geometry_SuperFGD_1e21_numu.root in the 28 line is obligatory but not used in the sim (I hope we will find time to solve lots of such bugs soon). Can use any genie file or download from IRODS QMULZone1/home/ND280Upgrade/GENIE

- --vis-file mac/vis_cluster_pion1000.mac attached GEANT4 command file. Configure PG here with /gun/*
- --nruns 40 --nexpts 2500 40 runs with 2500 events each
- --job-dir output dir
- --tag output file name

In order to optimize CPU and disk space usage you can change the output details. For example you can store the hits from all particles or from the primaries only. This should be set up at /src/app/EffStudy/src/ExN02TrackerSD.cc:106. Also you can specify which trajectories and which trajectory points you want to save. By default only primary trajectories and start/end point in each subdetector is stored. You can change these settings in /src/app/EffStudy/src/ND280RootPersistencyManager.cc.

### Run the G4 output analysis
Then you can run a macro. The examples could be found in the macro folder. To run the macro you need to tell root how to read ND280 events, so it will be like this:
```bash
root -l -q -b /t2k/users/suvorov/dev/t2k-nd280-upgrade/src/app/nd280UpEvent/TND280UpTrackPoint.cc+ 
/t2k/users/suvorov/dev/t2k-nd280-upgrade/src/app/nd280UpEvent/TND280UpTrack.cc+ 
/t2k/users/suvorov/dev/t2k-nd280-upgrade/src/app/nd280UpEvent/TND280UpVertex.cc+ 
/t2k/users/suvorov/dev/t2k-nd280-upgrade/src/app/nd280UpEvent/TND280UpHit.cc+ 
/t2k/users/suvorov/dev/t2k-nd280-upgrade/src/app/nd280UpEvent/TND280UpEvent.cc+ 'macro/your_macro.C’
```
The other way to analyse the G4 output is to run the reconstruction code that includes ReadOut simulation. The example of the run command is:
```bash
./submit/Reconstruction/submit_nd280upgrade_targetreco 
--inputfile /t2k/users/suvorov/AnalysisResults/ndUP/SuperFGD/proto1/proto1_p_1000MeV_c12-50000.root 
--job-dir /t2k/users/suvorov/AnalysisResults/ndUP/SuperFGD/proto1/ 
--softw-dir /t2k/users/suvorov/dev/t2k-nd280-upgrade/ 
--nruns 40 --nexpts 2500 --cut-truth-fv 0  --debug 0 --debug-plot 0 --tag proto1_p_1000_c12_noCT

```
To run the specific analysis you need to uncomment one in the /src/app/Reconstruction/Main.cc and recompile.

### Run the Highland analysis

For more information see
http://nd280.lancs.ac.uk/devel/invariant/highlandND280Doc/
and 
https://indico.cern.ch/event/724624/contributions/3083209/attachments/1693650/2725603/highlandUp_documentation_v1.pdf





## Obsolete README

go to t2k-nd280-upgrade and do:

0.) Set the environment

source T2KND280Up_CLHEP.sh $PWD --> for external CLHEP

1.) To build the code do:

cd build
source cmake_compile.sh

2.) To run the code do:

cd /PATH/TO/OUTPUT/DIRECTORY

$T2KND280UP/submit/submit_nd280upgrade_sim --softw-dir $T2KND280UP --job-dir $PWD --resource interactive --nexpts 1 --nruns 1

It runs the executable file in $T2KND280UP/bin
Depending on the options chosen in submit_nd280upgrade_sim The following output files are produced:
- EffStudy.root: root file containing the GEANT4 output tree
- g4_*.wrl: VRML files with event display
- geometry.root: root file containing the detector geometry

Then also the simplified selection on the primary tracks can be run as following:

$T2KND280UP/submit/submit_nd280upgrade_sel --softw-dir $T2KND280UP --job-dir $PWD --resource interactive --nexpts 1 --nruns 1

It's a simple ROOT macro in $T2KND280UP/macro that is run through a bash scripts.
Both scripts can be run on cluster to parallelize jobs.

All the macros (e.g. read submit_nd280upgrade_sel output) are in $T2KND280UP/macros


3.) Add packages

In order to add packages to be compiled you need to:

open $T2KND280UP/src/CMakeLists.txt and add (or replace)

INCLUDE_DIRECTORIES(${CMAKE_BINARY_DIR}/../src/app/PATH_TO_INCLUDE) ### header files

open  $T2KND280UP/src/app/CMakeLists.txt and add (or replace)

AUX_SOURCE_DIRECTORY(EffStudy/src sourcefile) ###

ADD_EXECUTABLE(EffStudy.exe          EffStudy/Main.cc ${sourcefile}) ###

TARGET_LINK_LIBRARIES(EffStudy.exe   ${ROOT_LIBRARIES}
				     ...           )


5.) Installation

In order to get this framework working the following software need to be installed in the following order:

- Xerces-C++: needed to store the geometry in GDML format. Installed xerces-c-3.1.3 with default configuration options (CURRENTLY NOT NEEDED) 
 
- CLHEP: Install version 2.2.0.4 (recommended for GEANT4.10.1.3)

- GEANT4: the cmake must be configured with the following options: USE_GDML=ON, INSTALL_DATA=ON, XERCESC_ROOT_DIR=/path/to/install (contains lib, bin and include directories), DGEANT4_USE_SYSTEM_CLHEP and DCLHEP_ROOT_DIR. For this framework the version geant4.10.01.p03 is recommended.

Enabling GDML --> need Xerces-c 
cmake -DCMAKE_INSTALL_PREFIX=/atlas/users/dsgalabe/GEANT4/geant4.10.01.p03-install -DGEANT4_USE_GDML=ON -DGEANT4_INSTALL_DATA=ON -DXERCESC_ROOT_DIR=/atlas/users/dsgalabe/xerces-c-3.1.3/xerces-c-3.1.3-install -XERCESC_INCLUDE_DIR=/atlas/users/dsgalabe/xerces-c-3.1.3/xerces-c-3.1.3-install/include -XERCESC_LIBRARY=/atlas/users/dsgalabe/xerces-c-3.1.3/xerces-c-3.1.3-install/lib/libxerces-c-3.1.so -DGEANT4_USE_SYSTEM_CLHEP=ON -DCLHEP_ROOT_DIR=/atlas/users/dsgalabe/CLHEP/clhep_install/ /atlas/users/dsgalabe/GEANT4/geant4.10.01.p03

Without GDML --> don't need Xerces-c (RECOMMENDED)
cmake -DCMAKE_INSTALL_PREFIX=/atlas/users/dsgalabe/GEANT4/geant4.10.01.p03-install -DGEANT4_INSTALL_DATA=ON -DGEANT4_USE_SYSTEM_CLHEP=ON -DCLHEP_ROOT_DIR=/atlas/users/dsgalabe/CLHEP/clhep_install/ /atlas/users/dsgalabe/GEANT4/geant4.10.01.p03







6.) Running NEUT (using 5.3.6 version)

cd /path_to_neut/src/neutgeom
source setup.sh

./event_rate -f /neutrino/data7/davide/files/ND280Upgrade/JNUBEAM/RHC_fluxfiles/nd5/hadd/nu.nd5_flukain.0-0.root -g /neutrino/data7/davide/files/ND280Upgrade/G4geometry/geometry_classic.root -v +Basket -o /atlas/users/dsgalabe/STUFF_T2KREWEIGHT/NEUT/neut_5.3.6/src/neutgeom/setup_output.root -d 5

./genev -j /neutrino/data7/davide/files/ND280Upgrade/JNUBEAM/RHC_fluxfiles/nd5/hadd/nu.nd5_flukain.0-0.root -g /neutrino/data7/davide/files/ND280Upgrade/G4geometry/geometry_classic.root -v +Basket -o genev_output.root -i /atlas/users/dsgalabe/STUFF_T2KREWEIGHT/NEUT/neut_5.3.6/src/neutgeom/setup_output.root -f rootracker

More detailed instructions can be found at http://www.t2k.org/asg/xsec/niwgdocs/neut/neut_nd280




7.) Running highlandUp 

see instruction at the following link:

http://www.t2k.org/nd280/physics/xsec/meetings/2016/pre-collaboration-meeting-workshop/highland/view

and the README.txt file in /highlandUp