

set(PoutreIPSubdirectory ${POUTRE_CORE_DIR}/poutreImageProcessing)

#generate_library_version(${PoutreNNSubdirectory}/src/current_configuration_generated.cpp)

set(PoutreIPSRC
  ${PoutreIPSubdirectory}/src/dummy.cpp
  # auto generated file
  #${PoutreIPSubdirectory}/src/current_configuration_generated.cpp
  )

add_library(PoutreNeuralNets ${PoutreNeuralNetsSRC})
target_include_directories(PoutreNeuralNets 
  PRIVATE ${PoutreIPSubdirectory}
  PRIVATE ${PoutreIPSubdirectory}/include
  PRIVATE ${PoutreIPSubdirectory}/src
  PRIVATE ${PoutreBase}
  PRIVATE ${JSONCPP_INCLUDE_DIR}
  )

target_link_libraries(PoutreImageProcessing
  # PRIVATE ${Boost_GRAPH_LIBRARY} 
  # PRIVATE ${Boost_DATE_TIME_LIBRARY} 
  # PRIVATE ${Boost_REGEX_LIBRARY} 
  # PRIVATE ${Boost_THREAD_LIBRARY} 
  # PRIVATE ${Boost_SYSTEM_LIBRARY}
  PRIVATE jsoncpp
  PoutreBase 
  )
set_target_properties(PoutreImageProcessing PROPERTIES FOLDER "Core/")

IF(BUILD_SHARED_LIBS)
  SET_TARGET_PROPERTIES(PoutreImageProcessing PROPERTIES COMPILE_FLAGS -DJSON_DLL)
ENDIF(BUILD_SHARED_LIBS)


install_POUTRE_targets(
  TARGET PoutreImageProcessing 
  CONFIGURATION Release 
  HEADER_FILES ${PoutreIPSRC}
  )
