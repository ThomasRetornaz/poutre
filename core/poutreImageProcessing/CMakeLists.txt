include_directories(${POUTRE_CORE_DIR})

set(PoutreIPSubdirectory ${POUTRE_CORE_DIR}/poutreImageProcessing)

#generate_library_version(${PoutreNNSubdirectory}/src/current_configuration_generated.cpp)

set(PoutreIPSRC_DETAILS
  ${PoutreIPSubdirectory}/core/include/poutreImageProcessingContainer.hpp
  ${PoutreIPSubdirectory}/core/include/poutreImageProcessingContainerCopieConvert.hpp
  ${PoutreIPSubdirectory}/core/include/poutreImageProcessingCompare.hpp

  ${PoutreIPSubdirectory}/core/include/poutreImageProcessingUnaryOp.hpp
  ${PoutreIPSubdirectory}/core/include/poutreImageProcessingBinaryOp.hpp
  ${PoutreIPSubdirectory}/core/include/poutreImageProcessingTernaryOp.hpp
  ${PoutreIPSubdirectory}/core/include/poutreImageProcessingQuaternaryOp.hpp
  )

set(PoutreIPSRC_PUBLICHEADERS  
  ${PoutreIPSubdirectory}/poutreImageProcessing.hpp
  ${PoutreIPSubdirectory}/core/poutreImageProcessingInterface.hpp
  ${PoutreIPSubdirectory}/core/poutreImageProcessingType.hpp
  ${PoutreIPSubdirectory}/core/poutreImageProcessingInterfaceCopieConvert.hpp
	)

set(PoutreIPSRC_CPP
  #${PoutreIPSubdirectory}/core/src/dummy.cpp
  ${PoutreIPSubdirectory}/core/src/poutreImageProcessingType.cpp
  ${PoutreIPSubdirectory}/core/src/poutreImageProcessingContainer.cpp
  ${PoutreIPSubdirectory}/core/src/poutreImageProcessingInterface.cpp
  ${PoutreIPSubdirectory}/core/src/poutreImageProcessingContainerCopieConvert.cpp
  # auto generated file
  #${PoutreIPSubdirectory}/src/current_configuration_generated.cpp   
  )
source_group(details FILES ${PoutreIPSRC_DETAILS})
source_group(src FILES ${PoutreIPSRC_CPP})
source_group(header FILES ${PoutreIPSRC_PUBLICHEADERS})

set(PoutreIPSRC
	${PoutreIPSRC_DETAILS}
	${PoutreIPSRC_CPP}
	${PoutreIPSRC_PUBLICHEADERS}
	)

add_library(PoutreImageProcessing ${PoutreIPSRC})
# target_include_directories(PoutreImageProcessing 
  # PRIVATE ${PoutreIPSubdirectory}
  # PRIVATE ${PoutreIPSubdirectory}/core
  # PRIVATE ${PoutreBase}
  # PRIVATE ${JSONCPP_INCLUDE_DIR}
  # )

target_link_libraries(PoutreImageProcessing 
  PUBLIC ${Boost_THREAD_LIBRARY}  
  PUBLIC ${Boost_SYSTEM_LIBRARY} 
  PUBLIC ${Boost_FILESYSTEM_LIBRARY}
  PUBLIC ${Boost_LOG_LIBRARY}
  PUBLIC ${Boost_LOG_SETUP_LIBRARY}
  PUBLIC jsoncpp 
  #INTERFACE Eigen  
  INTERFACE BoostSimd 
  PUBLIC   PoutreBase 

  )
set_target_properties(PoutreImageProcessing PROPERTIES FOLDER "Core/")

IF(BUILD_SHARED_LIBS)
  SET_TARGET_PROPERTIES(PoutreBase PROPERTIES COMPILE_FLAGS -DJSON_DLL)
  SET_TARGET_PROPERTIES(PoutreBase PROPERTIES COMPILE_FLAGS -DBOOST_LOG_DLL) #FIXME not true should be linked with boost
  SET_TARGET_PROPERTIES(PoutreBase PROPERTIES COMPILE_FLAGS -DBOOST_LOG_DYN_LINK) #FIXME not true should be linked with boost
ENDIF(BUILD_SHARED_LIBS)


install_POUTRE_targets(
  TARGET PoutreImageProcessing 
  CONFIGURATION Release 
  HEADER_FILES ${PoutreIPSRC}
  )
