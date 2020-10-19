include_directories(${POUTRE_SRC_DIR})

set(PoutreIPLLMSubdirectory ${POUTRE_SRC_DIR}/poutreImageProcessingLowLevelMorphology)

set(PoutreIPLLMRC_DETAILS
  ${PoutreIPLLMSubdirectory}/include/poutreImageProcessingEroDil.hpp
  ${PoutreIPLLMSubdirectory}/include/poutreImageProcessingEroDilNLS.hpp
  ${PoutreIPLLMSubdirectory}/include/poutreImageProcessingEroDilLine.hpp
  )

set(PoutreIPLLMSRC_PUBLICHEADERS  
  ${PoutreIPLLMSubdirectory}/poutreImageProcessingLowLevelMorphology.hpp
	)

set(PoutreIPLLMSRC_CPP
  ${PoutreIPLLMSubdirectory}/src/dummy.cpp
  )
source_group(details FILES ${PoutreIPLLMRC_DETAILS})
source_group(src FILES ${PoutreIPLLMSRC_CPP})
source_group(header FILES ${PoutreIPLLMSRC_PUBLICHEADERS})

set(PoutreIPLLMSRC
	${PoutreIPLLMRC_DETAILS}
	${PoutreIPLLMSRC_CPP}
	${PoutreIPLLMSRC_PUBLICHEADERS}
	)

add_library(PoutreIPLowLevelMorphology ${PoutreIPLLMSRC})

target_link_libraries(PoutreIPLowLevelMorphology 
  PUBLIC ${Boost_THREAD_LIBRARY}  
  PUBLIC ${Boost_SYSTEM_LIBRARY} 
  PUBLIC ${Boost_FILESYSTEM_LIBRARY}
  PUBLIC ${Boost_LOG_LIBRARY}
  PUBLIC ${Boost_LOG_SETUP_LIBRARY}
  PUBLIC jsoncpp_lib 
  # INTERFACE Eigen  
  INTERFACE xsimd 
  PUBLIC fmt
  PUBLIC PoutreBase
  PUBLIC   PoutreIPCore
  PUBLIC   PoutreIPPixelProcessing
  )
set_target_properties(PoutreIPLowLevelMorphology PROPERTIES FOLDER "src/")

IF(BUILD_SHARED_LIBS)
  SET_TARGET_PROPERTIES(PoutreIPLowLevelMorphology PROPERTIES COMPILE_FLAGS -DJSON_DLL)
  SET_TARGET_PROPERTIES(PoutreIPLowLevelMorphology PROPERTIES COMPILE_FLAGS -DBOOST_LOG_DLL) #FIXME not true should be linked with boost
  SET_TARGET_PROPERTIES(PoutreIPLowLevelMorphology PROPERTIES COMPILE_FLAGS -DBOOST_LOG_DYN_LINK) #FIXME not true should be linked with boost
ENDIF(BUILD_SHARED_LIBS)


install_POUTRE_targets(
  TARGET PoutreIPLowLevelMorphology 
  CONFIGURATION Release 
  HEADER_FILES ${PoutreIPLLMSRC_PUBLICHEADERS}
  )
