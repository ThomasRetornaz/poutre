

set(PoutreBaseSubdirectory ${POUTRE_CORE_DIR}/poutreBase)

#generate_library_version(${PoutreBaseSubdirectory}/src/current_configuration_generated.cpp)

set(PoutreBaseSRC
  ${PoutreBaseSubdirectory}/poutreBase.hpp
  ${PoutreBaseSubdirectory}/poutreConfig.hpp
  ${PoutreBaseSubdirectory}/poutreTypes.hpp
  ${PoutreBaseSubdirectory}/poutreJsonSerialization.hpp
  ${PoutreBaseSubdirectory}/poutreTrace.hpp
  ${PoutreBaseSubdirectory}/poutreRegistrar.hpp
  ${PoutreBaseSubdirectory}/poutreChronos.hpp
  ${PoutreBaseSubdirectory}/src/poutreJsonSerialization.cpp
  ${PoutreBaseSubdirectory}/src/poutreTrace.cpp
  #${PoutreBaseSubdirectory}/src/dummy.cpp
  
  # auto generated file
  #${PoutreBaseSubdirectory}/src/current_configuration_generated.cpp
  )

add_library(PoutreBase ${PoutreBaseSRC})
target_include_directories(PoutreBase 
  PRIVATE ${PoutreBaseSubdirectory}
  PRIVATE ${PoutreBaseSubdirectory}/include
  PRIVATE ${PoutreBaseSubdirectory}/src
  #PRIVATE ${JSONCPP_INCLUDE_DIR}
  )

target_link_libraries(PoutreBase 
  # PRIVATE ${Boost_GRAPH_LIBRARY} 
  # PRIVATE ${Boost_DATE_TIME_LIBRARY} 
  # PRIVATE ${Boost_REGEX_LIBRARY} 
  PRIVATE ${Boost_THREAD_LIBRARY} 
  PRIVATE ${Boost_SYSTEM_LIBRARY} 
  PRIVATE ${Boost_LOG_LIBRARY}
  PRIVATE ${Boost_LOG_SETUP_LIBRARY}
  PRIVATE jsoncpp
  INTERFACE Eigen
  INTERFACE BoostSimd
  )
set_target_properties(PoutreBase PROPERTIES FOLDER "Core/")
#force custom target before start
add_dependencies(PoutreBase BoostSimddep)
add_dependencies(PoutreBase Eigendep)

IF(BUILD_SHARED_LIBS)
  SET_TARGET_PROPERTIES(PoutreBase PROPERTIES COMPILE_FLAGS -DJSON_DLL)
  SET_TARGET_PROPERTIES(PoutreBase PROPERTIES COMPILE_FLAGS -DBOOST_LOG_DLL) #FIXME not true should be linked with boost
ENDIF(BUILD_SHARED_LIBS)


install_POUTRE_targets(
  TARGET PoutreBase 
  CONFIGURATION Release 
  HEADER_FILES ${PoutreBaseSRC}
  )
