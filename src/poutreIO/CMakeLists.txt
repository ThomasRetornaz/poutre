set(PoutreIOSubdirectory ${POUTRE_SRC_DIR}/poutreIO)


find_package(OpenCV QUIET) #todo add converter to/from opencv

INCLUDE_DIRECTORIES(BEFORE SYSTEM 
	${ZLIB_INCLUDE_DIR}
	${PNG_INCLUDE_DIR}
	${JPEG_INCLUDE_DIR}
	${TIFF_INCLUDE_DIR}
	${HDF5_INCLUDE_DIR}
  ${CMAKE_CURRENT_BINARY_DIR} #for module with ./configure
	)

set(PoutreIOSRC_DETAILS
  ${PoutreIOSubdirectory}/include/poutreIOBuffer.hpp

)  

set(PoutreIOSRC_PUBLICHEADERS  
  ${PoutreIOSubdirectory}/poutreIO.hpp
  ${PoutreIOSubdirectory}/poutreIOString.hpp
  ${PoutreIOSubdirectory}/poutreIOLoader.hpp
  ${PoutreIOSubdirectory}/poutreIOWriter.hpp
  ${PoutreIOSubdirectory}/poutreIOProvider.hpp
)

set(PoutreIOSRC_CPP
  ${PoutreIOSubdirectory}/src/poutreIOString.cpp
  ${PoutreIOSubdirectory}/src/poutreIOLoader.cpp
  ${PoutreIOSubdirectory}/src/poutreIOWriter.cpp
  ${PoutreIOSubdirectory}/src/poutreIOProvider.cpp
  )
source_group(details FILES ${PoutreIOSRC_DETAILS})
source_group(src FILES ${PoutreIOSRC_CPP})
source_group(header FILES ${PoutreIOSRC_PUBLICHEADERS})

set(PoutreIOSRC
	${PoutreIOSRC_PUBLICHEADERS}
	${PoutreIOSRC_DETAILS}
	${PoutreIOSRC_CPP}
	)

add_library(PoutreIO ${PoutreIOSRC})
target_link_libraries(PoutreIO 
  PUBLIC ${Boost_SYSTEM_LIBRARY}
  PUBLIC ${Boost_FILESYSTEM_LIBRARY} 
  PUBLIC ${Boost_LOG_LIBRARY}
  PUBLIC ${Boost_LOG_SETUP_LIBRARY}
  PRIVATE jsoncpp_lib
  PUBLIC ${JPEG_LIBRARY}
  PUBLIC ${ZLIB_LIBRARY} 
  PUBLIC ${PNG_LIBRARY} 
  PUBLIC ${TIFF_LIBRARY} 
  INTERFACE xsimd 
  
  PUBLIC   PoutreBase 
  PUBLIC   PoutreIPCore 
  )
set_target_properties(PoutreIO PROPERTIES FOLDER "src/")

IF(BUILD_SHARED_LIBS)
  SET_TARGET_PROPERTIES(PoutreIO PROPERTIES COMPILE_FLAGS -DJSON_DLL)
  SET_TARGET_PROPERTIES(PoutreIO PROPERTIES COMPILE_FLAGS -DBOOST_LOG_DLL) #FIXME not true should be linked with boost
  SET_TARGET_PROPERTIES(PoutreIO PROPERTIES COMPILE_FLAGS -DBOOST_LOG_DYN_LINK) #FIXME not true should be linked with boost
ENDIF(BUILD_SHARED_LIBS)

install_POUTRE_targets(
  TARGET PoutreIO 
  CONFIGURATION Release 
  HEADER_FILES ${PoutreIOSRC}
  )
