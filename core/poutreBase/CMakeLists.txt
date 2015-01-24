

set(PoutreBaseSubdirectory ${POUTRE_CORE_DIR}/poutreBase)

#generate_library_version(${PoutreBaseSubdirectory}/src/current_configuration_generated.cpp)

set(PoutreBaseSRC
  ${PoutreBaseSubdirectory}/poutreBase.hpp
  ${PoutreBaseSubdirectory}/poutreConfig.hpp
  ${PoutreBaseSubdirectory}/poutreTypes.hpp
  ${PoutreBaseSubdirectory}/poutreJsonSerialization.hpp
  ${PoutreBaseSubdirectory}/poutreJsonSerialization.cpp
  ${PoutreBaseSubdirectory}/poutreRegistrar.hpp
  ${PoutreBaseSubdirectory}/src/dummy.cpp
  # auto generated file
  #${PoutreBaseSubdirectory}/src/current_configuration_generated.cpp
  )

add_library(PoutreBase ${PoutreBaseSRC})
target_include_directories(PoutreBase 
  PRIVATE ${PoutreBaseSubdirectory}
  PRIVATE ${PoutreBaseSubdirectory}/include
  PRIVATE ${PoutreBaseSubdirectory}/src
  PRIVATE ${JSONCPP_INCLUDE_DIR}
  )

target_link_libraries(PoutreBase 
  # PRIVATE ${Boost_GRAPH_LIBRARY} 
  # PRIVATE ${Boost_DATE_TIME_LIBRARY} 
  # PRIVATE ${Boost_REGEX_LIBRARY} 
  # PRIVATE ${Boost_THREAD_LIBRARY} 
  # PRIVATE ${Boost_SYSTEM_LIBRARY}
  PRIVATE jsoncpp
  )
set_target_properties(PoutreBase PROPERTIES FOLDER "Core/")

IF(BUILD_SHARED_LIBS)
  SET_TARGET_PROPERTIES(PoutreBase PROPERTIES COMPILE_FLAGS -DJSON_DLL)
ENDIF(BUILD_SHARED_LIBS)


install_POUTRE_targets(
  TARGET PoutreBase 
  CONFIGURATION Release 
  HEADER_FILES ${PoutreBaseSRC}
  )
