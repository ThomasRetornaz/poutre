set(PoutreIOSubdirectory ${POUTRE_SRC_DIR}/poutreIO)

find_package(OpenCV QUIET) # todo add converter to/from opencv

include_directories(
  BEFORE
  SYSTEM
  ${OPENEXR_INCLUDE_DIR}
  ${OIIO_INCLUDE_DIR}
  ${HIGH_FIVE_INCLUDE_DIR}
  ${HDF5_INCLUDE_DIRS}
  ${CMAKE_BINARY_DIR}/include
  ${CMAKE_BINARY_DIR})

set(PoutreIOSRC_DETAILS
    ${PoutreIOSubdirectory}/include/poutreIOBuffer.hpp
    ${PoutreIOSubdirectory}/include/poutreOIIO.hpp
    ${PoutreIOSubdirectory}/include/poutreHDF5.hpp)

set(PoutreIOSRC_PUBLICHEADERS
    ${PoutreIOSubdirectory}/poutreIO.hpp
    ${PoutreIOSubdirectory}/poutreIOString.hpp
    ${PoutreIOSubdirectory}/poutreIOLoader.hpp
    ${PoutreIOSubdirectory}/poutreIOWriter.hpp
    ${PoutreIOSubdirectory}/poutreIOProvider.hpp)

set(PoutreIOSRC_CPP
    ${PoutreIOSubdirectory}/src/poutreIOString.cpp
    ${PoutreIOSubdirectory}/src/poutreIOLoader.cpp
    ${PoutreIOSubdirectory}/src/poutreIOWriter.cpp
    ${PoutreIOSubdirectory}/src/poutreIOProvider.cpp
    ${PoutreIOSubdirectory}/src/poutreOIIO.cpp
    ${PoutreIOSubdirectory}/src/poutreHDF5.cpp)
source_group(details FILES ${PoutreIOSRC_DETAILS})
source_group(src FILES ${PoutreIOSRC_CPP})
source_group(header FILES ${PoutreIOSRC_PUBLICHEADERS})

set(PoutreIOSRC ${PoutreIOSRC_PUBLICHEADERS} ${PoutreIOSRC_DETAILS}
                ${PoutreIOSRC_CPP})

add_library(PoutreIO ${PoutreIOSRC})
target_link_libraries(
  PoutreIO
  PRIVATE jsoncpp_lib
  PUBLIC ${OpenImageIO_LIBRARIES}
  PUBLIC ${HDF5_LIBRARIES}
  INTERFACE xsimd
  INTERFACE fmt-header-only
  PUBLIC PoutreBase
  PUBLIC PoutreIPCore)
set_target_properties(PoutreIO PROPERTIES FOLDER "src/")
add_dependencies(PoutreIO OpenImageIO) # gcc warn -- ?

if(BUILD_SHARED_LIBS)
  set_target_properties(PoutreIO PROPERTIES COMPILE_FLAGS -DJSON_DLL)
  set_target_properties(PoutreIO PROPERTIES COMPILE_FLAGS -DBOOST_LOG_DLL
  )# FIXME not true should be linked with boost
  set_target_properties(PoutreIO PROPERTIES COMPILE_FLAGS -DBOOST_LOG_DYN_LINK
  )# FIXME not true should be linked with boost
endif(BUILD_SHARED_LIBS)

install_poutre_targets(TARGET PoutreIO CONFIGURATION Release HEADER_FILES
                       ${PoutreIOSRC})
