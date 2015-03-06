 ################################################################################
 #    Copyright (C) 2014 GSI Helmholtzzentrum fuer Schwerionenforschung GmbH    #
 #                                                                              #
 #              This software is distributed under the terms of the             # 
 #         GNU Lesser General Public Licence version 3 (LGPL) version 3,        #  
 #                  copied verbatim in the file "LICENSE"                       #
 ################################################################################
# Create a library called "libFairTestDetector" which includes the source files given in
# the array .
# The extension is already found.  Any number of sources could be listed here.


set(INCLUDE_DIRECTORIES
${BASE_INCLUDE_DIRECTORIES}
${CMAKE_SOURCE_DIR}/fairmq
${CMAKE_SOURCE_DIR}/fairmq/devices
${CMAKE_SOURCE_DIR}/base/MQ
${CMAKE_SOURCE_DIR}/base/MQ/policies/Sampler
${CMAKE_SOURCE_DIR}/base/MQ/policies/Serialization
${CMAKE_SOURCE_DIR}/base/MQ/policies/Storage
${CMAKE_SOURCE_DIR}/base/MQ/baseMQtools
${CMAKE_SOURCE_DIR}/example/OscillationExperiment
${CMAKE_SOURCE_DIR}/example/OscillationExperiment/tools
${CMAKE_SOURCE_DIR}/example/OscillationExperiment/tools/data
${CMAKE_SOURCE_DIR}/example/OscillationExperiment/tools/stat
${CMAKE_SOURCE_DIR}/example/OscillationExperiment/analysis
${CMAKE_SOURCE_DIR}/example/OscillationExperiment/data
${CMAKE_SOURCE_DIR}/example/OscillationExperiment/model
)

Set(SYSTEM_INCLUDE_DIRECTORIES
    ${SYSTEM_INCLUDE_DIRECTORIES}
    ${ZMQ_INCLUDE_DIR}
    ${CUBA_INCLUDE_DIR}
    ${BAT_INCLUDE_DIR}
)


include_directories(${INCLUDE_DIRECTORIES})
Include_Directories(SYSTEM ${SYSTEM_INCLUDE_DIRECTORIES})


If (Boost_FOUND)
  configure_file( ${CMAKE_SOURCE_DIR}/example/OscillationExperiment/macro/runOscAnalysis.sh.in ${CMAKE_BINARY_DIR}/bin/runOscAnalysis.sh )
  configure_file( ${CMAKE_SOURCE_DIR}/example/OscillationExperiment/macro/OscillationAnalysis.cfg.in ${CMAKE_BINARY_DIR}/bin/OscillationAnalysis.cfg )
  #configure_file( ${CMAKE_SOURCE_DIR}/example/OscillationExperiment/macro/startTest.sh.in ${CMAKE_BINARY_DIR}/bin/startTest.sh )
  configure_file( ${CMAKE_SOURCE_DIR}/example/OscillationExperiment/macro/startOscSampler.sh.in ${CMAKE_BINARY_DIR}/bin/startOscSampler.sh )
  configure_file( ${CMAKE_SOURCE_DIR}/example/OscillationExperiment/macro/startOscSampler1.sh.in ${CMAKE_BINARY_DIR}/bin/startOscSampler1.sh )
  configure_file( ${CMAKE_SOURCE_DIR}/example/OscillationExperiment/macro/startOscSampler2.sh.in ${CMAKE_BINARY_DIR}/bin/startOscSampler2.sh )
  configure_file( ${CMAKE_SOURCE_DIR}/example/OscillationExperiment/macro/startOscSampler3.sh.in ${CMAKE_BINARY_DIR}/bin/startOscSampler3.sh )
  configure_file( ${CMAKE_SOURCE_DIR}/example/OscillationExperiment/macro/startOscSampler4.sh.in ${CMAKE_BINARY_DIR}/bin/startOscSampler4.sh )
  configure_file( ${CMAKE_SOURCE_DIR}/example/OscillationExperiment/macro/startOscSink.sh.in ${CMAKE_BINARY_DIR}/bin/startOscSink.sh )
 
  EXEC_PROGRAM(/bin/chmod ARGS "u+x  ${CMAKE_BINARY_DIR}/bin/runOscAnalysis.sh")
  #EXEC_PROGRAM(/bin/chmod ARGS "u+x  ${CMAKE_BINARY_DIR}/bin/startTest.sh")
  EXEC_PROGRAM(/bin/chmod ARGS "u+x  ${CMAKE_BINARY_DIR}/bin/startOscSampler.sh")
  EXEC_PROGRAM(/bin/chmod ARGS "u+x  ${CMAKE_BINARY_DIR}/bin/startOscSampler1.sh")
  EXEC_PROGRAM(/bin/chmod ARGS "u+x  ${CMAKE_BINARY_DIR}/bin/startOscSampler2.sh")
  EXEC_PROGRAM(/bin/chmod ARGS "u+x  ${CMAKE_BINARY_DIR}/bin/startOscSampler3.sh")
  EXEC_PROGRAM(/bin/chmod ARGS "u+x  ${CMAKE_BINARY_DIR}/bin/startOscSampler4.sh")
  EXEC_PROGRAM(/bin/chmod ARGS "u+x  ${CMAKE_BINARY_DIR}/bin/startOscSink.sh")
EndIf (Boost_FOUND)

set(LINK_DIRECTORIES
${ROOT_LIBRARY_DIR}
${Boost_LIBRARY_DIRS}
${BAT_LIBRARY_DIR}
#${CUBA_LIBRARY_DIR}
) 

link_directories(${LINK_DIRECTORIES})

Set(SRCS
    model/RooMyAnalyticalPdf.cxx
    data/OscMCPoint.cxx
)
Set(NO_DICT_SRCS
    tools/data/IOManager.cxx
    tools/data/FileReader.cxx
    tools/stat/BatAnalysis.cxx
    analysis/OscAnaManager.cxx
    analysis/OscSimulation.cxx
    data/OscDataSet.cxx
    
    model/OscModel.cxx
    model/ExpModel.cxx
    
    )


Set(LINKDEF OscAnalysisLinkDef.h)
Set(LIBRARY_NAME OscAnalysis)

If (Boost_FOUND)
  Set(DEPENDENCIES
    Base FairMQ BaseMQ
    #${CUBA_LIBRARY_STATIC}
    ${BAT_LIBRARIES_SHARED}
    boost_thread boost_system boost_serialization boost_program_options 
    RooFitCore RooFit Foam MathMore 
    )
EndIf (Boost_FOUND)



GENERATE_LIBRARY()

#Add_Subdirectory(macro)


If (Boost_FOUND)
  Set(Exe_Names
    runOscAnalysis
    #runtest
    runOscSampler
    runOscFileSink
  )



  set(Exe_Source
    
    run/runOscAnalysis.cxx
    #run/test.cxx
    run/runOscSampler.cxx
    run/runOscFileSink.cxx
    )

  List(LENGTH Exe_Names _length)
  Math(EXPR _length ${_length}-1)

  ForEach(_file RANGE 0 ${_length})
    List(GET Exe_Names ${_file} _name)
    List(GET Exe_Source ${_file} _src)
    Set(EXE_NAME ${_name})
    Set(SRCS ${_src})
    Set(DEPENDENCIES OscAnalysis)
    GENERATE_EXECUTABLE()
  EndForEach(_file RANGE 0 ${_length})
EndIf (Boost_FOUND)
