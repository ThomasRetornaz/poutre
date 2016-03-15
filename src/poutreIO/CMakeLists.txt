

set(PoutreIOSubdirectory ${POUTRE_SRC_DIR}/poutreIO)

#generate_library_version(${PoutreIOSubdirectory}/src/current_configuration_generated.cpp)

set(PoutreIOSRC_PUBLICHEADERS  
  ${PoutreIOSubdirectory}/poutreIO.hpp
  ${PoutreIOSubdirectory}/poutreIOString.hpp
  ${PoutreIOSubdirectory}/poutreIOLoader.hpp
)

set(PoutreIOSRC_CPP
  ${PoutreIOSubdirectory}/src/dummy.cpp
  ${PoutreIOSubdirectory}/src/poutreIOString.cpp
  # auto generated file
  #${PoutreBaseSubdirectory}/src/current_configuration_generated.cpp
  )
source_group(details FILES ${PoutreIOSRC_DETAILS})
source_group(src FILES ${PoutreIOSRC_CPP})
source_group(header FILES ${PoutreIOSRC_PUBLICHEADERS})

set(PoutreIOSRC
	${PoutreIOSRC_PUBLICHEADERS}
	${PoutreIOSRC_CPP}
	)

add_library(PoutreIO ${PoutreIOSRC})
target_link_libraries(PoutreIO 
  PRIVATE jsoncpp
  PRIVATE ${JPEG_LIBRARY}
  PRIVATE ${ZLIB_LIBRARY} 
  PRIVATE ${PNG_LIBRARY} 
  PRIVATE ${TIFF_LIBRARY} 
  INTERFACE Eigen
  INTERFACE BoostSimd
  PUBLIC PoutreBase 
  PUBLIC PoutreImageProcessing
  )
set_target_properties(PoutreIO PROPERTIES FOLDER "src/")

IF(BUILD_SHARED_LIBS)
  SET_TARGET_PROPERTIES(PoutreIO PROPERTIES COMPILE_FLAGS -DJSON_DLL)
ENDIF(BUILD_SHARED_LIBS)


install_POUTRE_targets(
  TARGET PoutreIO 
  CONFIGURATION Release 
  HEADER_FILES ${PoutreIOSRC}
  )
