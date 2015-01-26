

set(PoutreNNSubdirectory ${POUTRE_CORE_DIR}/poutreNeuralNets)

#generate_library_version(${PoutreNNSubdirectory}/src/current_configuration_generated.cpp)

set(PoutreNeuralNetsSRC
  ${PoutreNNSubdirectory}/poutreNeuralNets.hpp
  ${PoutreNNSubdirectory}/src/dummy.cpp
  # auto generated file
  #${PoutreNNSubdirectory}/src/current_configuration_generated.cpp
  )

add_library(PoutreNeuralNets ${PoutreNeuralNetsSRC})
target_include_directories(PoutreNeuralNets 
  PRIVATE ${PoutreNNSubdirectory}
  PUBLIC ${PoutreNNSubdirectory}/include
  PRIVATE ${JSONCPP_INCLUDE_DIR}
  )

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
set_target_properties(PoutreNeuralNets PROPERTIES FOLDER "Core/")

IF(BUILD_SHARED_LIBS)
  SET_TARGET_PROPERTIES(PoutreNeuralNets PROPERTIES COMPILE_FLAGS -DJSON_DLL)
ENDIF(BUILD_SHARED_LIBS)


install_POUTRE_targets(
  TARGET PoutreNeuralNets 
  CONFIGURATION Release 
  HEADER_FILES ${PoutreNeuralNetsSRC}
  )
