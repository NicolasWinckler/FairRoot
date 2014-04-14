# Create a library called "libFairTestDetector" which includes the source files given in
# the array .
# The extension is already found.  Any number of sources could be listed here.


set(INCLUDE_DIRECTORIES
${BASE_INCLUDE_DIRECTORIES}
${ROOT_INCLUDE_DIR}
${ZMQ_INCLUDE_DIR}
${CUBA_INCLUDE_DIR}
${BAT_INCLUDE_DIR}
${CMAKE_SOURCE_DIR}/example/Oscillation
${CMAKE_SOURCE_DIR}/example/Oscillation/src
${CMAKE_SOURCE_DIR}/example/Oscillation/src/model
${CMAKE_SOURCE_DIR}/example/Oscillation/src/macro
${CMAKE_SOURCE_DIR}/example/Oscillation/src/data
)

include_directories(${INCLUDE_DIRECTORIES})

set(LINK_DIRECTORIES
${ROOT_LIBRARY_DIR}
${Boost_LIBRARY_DIRS}
#${BAT_LIBRARY_DIR}
#${CUBA_LIBRARY_DIR}
#${CUBA_LIBRARY_STATIC}
#${BAT_LIBRARIES_SHARED} 
) 

link_directories(${LINK_DIRECTORIES})

Set(NO_DICT_SRCS
  src/data/FileReader.cxx
  src/data/SidsDataSet.cxx
  src/data/SidsParameters.cxx
  src/model/CountingExp.cxx
  src/model/OscModel.cxx
  src/model/ExpModel.cxx
  src/macro/Analysis.cxx
)


Set(LINKDEF OscAnalysisLinkDef.h)
Set(LIBRARY_NAME OscillationAnalysis)

  Set(DEPENDENCIES 
    ${CUBA_LIBRARY_STATIC}
    ${BAT_LIBRARIES_SHARED}
    #Proof GeomPainter Geom VMC EG MathCore Physics 
    Matrix Tree Hist RIO Cint Core
    Minuit RooFitCore RooFit RooStats Foam MathMore
    #Tree Hist RIO Cint Core
    #BATmodels BATmtf BAT cuba
)
message(STATUS "BAT_LIBRARIES_SHARED (2nd call) is = ${BAT_LIBRARIES_SHARED}")


GENERATE_LIBRARY()


If (Boost_FOUND)
  Set(Exe_Names runoscillation)

  set(Exe_Source
    src/macro/runOscAnalysis.cxx
    )

  List(LENGTH Exe_Names _length)
  Math(EXPR _length ${_length}-1)

  ForEach(_file RANGE 0 ${_length})
    List(GET Exe_Names ${_file} _name)
    List(GET Exe_Source ${_file} _src)
    Set(EXE_NAME ${_name})
    Set(SRCS ${_src})
    Set(DEPENDENCIES OscillationAnalysis)
    GENERATE_EXECUTABLE()
  EndForEach(_file RANGE 0 ${_length})
EndIf (Boost_FOUND)
