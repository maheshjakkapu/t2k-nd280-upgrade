MESSAGE(" Looking for Geant4...")

FIND_PATH(GEANT4_INCLUDE_DIR NAMES G4PVPlacement.hh          PATHS
                                                             $ENV{G4INCLUDE}/
                                                             NO_DEFAULT_PATH) 

#message("GEANT4_INCLUDE_DIRS : ${GEANT4_INCLUDE_DIR}")

FIND_PATH(GEANT4_LIBRARY_DIR NAMES libG4event.so             PATHS
                                                             $ENV{G4LIB}/$ENV{G4SYSTEM}/
                                                             NO_DEFAULT_PATH)
# message("GEANT4_LIBRARY_DIR : ${GEANT4_LIBRARY_DIR}")

#FIND_PROGRAM(GEANT4_CONFIG_EXECUTABLE  NAMES  geant4-config  PATHS
#                                                             $ENV{G4INCLUDE}/../../bin
#                                                             NO_DEFAULT_PATH)

FIND_PROGRAM(GEANT4_CONFIG_EXECUTABLE_LOAD NAMES  geant-load-flags  PATHS
                                                                 $ENV{G4BIN}/
                                                                 NO_DEFAULT_PATH)

FIND_PROGRAM(GEANT4_CONFIG_EXECUTABLE_CPP NAMES  geant-cpp-flags  PATHS
                                                                 $ENV{G4BIN}/
                                                                 NO_DEFAULT_PATH)

#message("GEANT4_CONFIG_EXECUTABLES : ${GEANT4_CONFIG_EXECUTABLE_CPP}")

if (GEANT4_INCLUDE_DIR AND GEANT4_LIBRARY_DIR AND GEANT4_CONFIG_EXECUTABLE_LOAD)
   set(GEANT4_FOUND TRUE)
endif (GEANT4_INCLUDE_DIR AND GEANT4_LIBRARY_DIR AND GEANT4_CONFIG_EXECUTABLE_LOAD)

if (GEANT4_FOUND)

#  EXEC_PROGRAM(${GEANT4_CONFIG_EXECUTABLE} ARGS "--version" OUTPUT_VARIABLE GEANT4VERSION)

  # ask geant4-config for the library varaibles

  EXEC_PROGRAM(${GEANT4_CONFIG_EXECUTABLE_LOAD} OUTPUT_VARIABLE GEANT4_LIBRARIES)
  EXEC_PROGRAM(${GEANT4_CONFIG_EXECUTABLE_CPP} OUTPUT_VARIABLE GEANT4_FLAGS)
  message(STATUS "found in ${GEANT4_INCLUDE_DIR}\n")
#   message(STATUS "libraries : ${GEANT4_LIBRARIES}")
#   message(STATUS "flags : ${GEANT4_FLAGS}\n")

  # message(STATUS "definitions : ${Gean4_DEFINITIONS} \n")

  SET(LD_LIBRARY_PATH ${LD_LIBRARY_PATH} ${GEANT4_LIBRARY_DIR})
  SET(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} ${GEANT4_FLAGS})

  INCLUDE_DIRECTORIES(${GEANT4_INCLUDE_DIR})
  LINK_DIRECTORIES   (${GEANT4_LIBRARY_DIR})

else (GEANT4_FOUND)

  message(FATAL_ERROR "Could NOT find Gean4!\n")

endif (GEANT4_FOUND)
