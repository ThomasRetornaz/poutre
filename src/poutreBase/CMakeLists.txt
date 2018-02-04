

set(PoutreBaseSubdirectory ${POUTRE_SRC_DIR}/poutreBase)

set(PoutreBaseSRCDETAILS
  )

set(PoutreBaseSRC_PUBLICHEADERS  
  ${PoutreBaseSubdirectory}/poutreBase.hpp
  ${PoutreBaseSubdirectory}/poutreConfig.hpp
  ${PoutreBaseSubdirectory}/poutreTypes.hpp
  ${PoutreBaseSubdirectory}/poutreJsonSerialization.hpp
  ${PoutreBaseSubdirectory}/poutreTrace.hpp
  ${PoutreBaseSubdirectory}/poutreRegistrar.hpp
  ${PoutreBaseSubdirectory}/poutreChronos.hpp
  ${PoutreBaseSubdirectory}/poutreGeometry.hpp
  ${PoutreBaseSubdirectory}/poutreCoordinate.hpp
  ${PoutreBaseSubdirectory}/poutreContainerHelper.hpp
  ${PoutreBaseSubdirectory}/poutreDenseIterator.hpp  
  ${PoutreBaseSubdirectory}/poutreStaticContainer.hpp
  ${PoutreBaseSubdirectory}/poutreContainerView.hpp
)

set(PoutreBaseSRC_CPP
  ${PoutreBaseSubdirectory}/src/poutreJsonSerialization.cpp
  ${PoutreBaseSubdirectory}/src/poutreTrace.cpp
  ${PoutreBaseSubdirectory}/src/poutreGeometry.cpp
  ${PoutreBaseSubdirectory}/src/poutreCoordinate.cpp
  ${PoutreBaseSubdirectory}/src/poutreType.cpp
  ${PoutreBaseSubdirectory}/src/poutreChronos.cpp
  )
source_group(details FILES ${PoutreBaseSRC_DETAILS})
source_group(src FILES ${PoutreBaseSRC_CPP})
source_group(header FILES ${PoutreBaseSRC_PUBLICHEADERS})

set(PoutreBaseSRC
	${PoutreBaseSRC_DETAILS}
	${PoutreBaseSRC_CPP}
	${PoutreBaseSRC_PUBLICHEADERS}
	)

add_library(PoutreBase ${PoutreBaseSRC})
# target_include_directories(PoutreBase 
  # PRIVATE ${PoutreBaseSubdirectory}
  # PRIVATE ${PoutreBaseSubdirectory}/include
  # PRIVATE ${PoutreBaseSubdirectory}/src
  # )

target_link_libraries(PoutreBase 
  PUBLIC ${Boost_THREAD_LIBRARY}  
  PUBLIC ${Boost_SYSTEM_LIBRARY}
  PUBLIC ${Boost_FILESYSTEM_LIBRARY} 
  PUBLIC ${Boost_LOG_LIBRARY}
  PUBLIC ${Boost_LOG_SETUP_LIBRARY}
  PUBLIC jsoncpp 
  INTERFACE Eigen  
  INTERFACE libsimdpp 
  )
set_target_properties(PoutreBase PROPERTIES FOLDER "src/")
#force custom target before start
add_dependencies(PoutreBase libsimdppdep)
add_dependencies(PoutreBase Eigendep)

IF(BUILD_SHARED_LIBS)
  SET_TARGET_PROPERTIES(PoutreBase PROPERTIES COMPILE_FLAGS -DJSON_DLL)
  SET_TARGET_PROPERTIES(PoutreBase PROPERTIES COMPILE_FLAGS -DBOOST_LOG_DLL) #FIXME not true should be linked with boost
  SET_TARGET_PROPERTIES(PoutreBase PROPERTIES COMPILE_FLAGS -DBOOST_LOG_DYN_LINK) #FIXME not true should be linked with boost
ENDIF(BUILD_SHARED_LIBS)
