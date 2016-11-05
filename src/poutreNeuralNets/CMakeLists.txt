
set(PoutreNNSubdirectory ${POUTRE_SRC_DIR}/poutreNeuralNets)

set(PoutreNeuralNetsSRC_DETAILS
  )

set(PoutreNeuralNetsSRC_PUBLICHEADERS  
  ${PoutreNNSubdirectory}/poutreNeuralNets.hpp
)

set(PoutreNeuralNetsSRC_CPP
  ${PoutreNNSubdirectory}/src/dummy.cpp
  )
source_group(details FILES ${PoutreNeuralNetsSRC_DETAILS})
source_group(src FILES ${PoutreNeuralNetsSRC_CPP})
source_group(header FILES ${PoutreNeuralNetsSRC_PUBLICHEADERS})

set(PoutreNeuralNetsSRC
	${PoutreNeuralNetsSRC_DETAILS}
	${PoutreNeuralNetsSRC_PUBLICHEADERS}
	${PoutreNeuralNetsSRC_CPP}
	)

add_library(PoutreNeuralNets ${PoutreNeuralNetsSRC})
target_link_libraries(PoutreNeuralNets 
  # PRIVATE ${Boost_GRAPH_LIBRARY} 
  # PRIVATE ${Boost_DATE_TIME_LIBRARY} 
  # PRIVATE ${Boost_REGEX_LIBRARY} 
  # PRIVATE ${Boost_THREAD_LIBRARY} 
  # PRIVATE ${Boost_SYSTEM_LIBRARY}
  PRIVATE jsoncpp
  INTERFACE Eigen
  INTERFACE BoostSimd
  PoutreBase 
  )
set_target_properties(PoutreNeuralNets PROPERTIES FOLDER "src/")

IF(BUILD_SHARED_LIBS)
  SET_TARGET_PROPERTIES(PoutreNeuralNets PROPERTIES COMPILE_FLAGS -DJSON_DLL)
ENDIF(BUILD_SHARED_LIBS)


install_POUTRE_targets(
  TARGET PoutreNeuralNets 
  CONFIGURATION Release 
  HEADER_FILES ${PoutreNeuralNetsSRC}
  )
