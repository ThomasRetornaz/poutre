include_directories(${POUTRE_SRC_DIR})

set(PoutreIPPPSubdirectory ${POUTRE_SRC_DIR}/poutreImageProcessingPixelOperation)

set(PoutreIPPPSRC_DETAILS
  ${PoutreIPPPSubdirectory}/include/poutreImageProcessingUnaryOp.hpp
  ${PoutreIPPPSubdirectory}/include/poutreImageProcessingUnaryOp.hpp
  ${PoutreIPPPSubdirectory}/include/poutreImageProcessingBinaryOp.hpp
  ${PoutreIPPPSubdirectory}/include/poutreImageProcessingTernaryOp.hpp
  ${PoutreIPPPSubdirectory}/include/poutreImageProcessingQuaternaryOp.hpp
  ${PoutreIPPPSubdirectory}/include/poutreImageProcessingArith.hpp
  ${PoutreIPPPSubdirectory}/include/poutreImageProcessingCompare.hpp
  )

set(PoutreIPPPSRC_PUBLICHEADERS  
  ${PoutreIPPPSubdirectory}/poutreImageProcessingPixelProcessing.hpp
	)

set(PoutreIPPPSRC_CPP
  ${PoutreIPPPSubdirectory}/src/dummy.cpp
  )
source_group(details FILES ${PoutreIPPPSRC_DETAILS})
source_group(src FILES ${PoutreIPPPSRC_CPP})
source_group(header FILES ${PoutreIPPPSRC_PUBLICHEADERS})

set(PoutreIPPPSRC
	${PoutreIPPPSRC_DETAILS}
	${PoutreIPPPSRC_CPP}
	${PoutreIPPPSRC_PUBLICHEADERS}
	)

add_library(PoutreIPPixelProcessing ${PoutreIPPPSRC})

target_link_libraries(PoutreIPPixelProcessing 
  PUBLIC ${Boost_THREAD_LIBRARY}  
  PUBLIC ${Boost_SYSTEM_LIBRARY} 
  PUBLIC ${Boost_FILESYSTEM_LIBRARY}
  PUBLIC ${Boost_LOG_LIBRARY}
  PUBLIC ${Boost_LOG_SETUP_LIBRARY}
  PUBLIC jsoncpp 
  INTERFACE Eigen  
  # INTERFACE ${SIMD_LIB} 
  PUBLIC   PoutreBase 
  PUBLIC   PoutreIPCore 
  )
set_target_properties(PoutreIPPixelProcessing PROPERTIES FOLDER "src/")

#IF(BUILD_SHARED_LIBS)
#  SET_TARGET_PROPERTIES(PoutreIPPixelProcessing PROPERTIES COMPILE_FLAGS -DJSON_DLL)
#  SET_TARGET_PROPERTIES(PoutreIPPixelProcessing PROPERTIES COMPILE_FLAGS -DBOOST_LOG_DLL) #FIXME not true should be linked with boost
#  SET_TARGET_PROPERTIES(PoutreIPPixelProcessing PROPERTIES COMPILE_FLAGS -DBOOST_LOG_DYN_LINK) #FIXME not true should be linked with boost
#ENDIF(BUILD_SHARED_LIBS)


install_POUTRE_targets(
  TARGET PoutreIPPixelProcessing 
  CONFIGURATION Release 
  HEADER_FILES ${PoutreIPSRC}
  )
