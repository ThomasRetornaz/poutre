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
)
    
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
    ${PoutreIOSubdirectory}/src/poutreIOProvider.cpp)

if(POUTRE_USE_HDF5)
  list (APPEND PoutreIOSRC_DETAILS ${PoutreIOSubdirectory}/include/poutreHDF5.hpp) 
  list (APPEND PoutreIOSRC_CPP ${PoutreIOSubdirectory}/src/poutreHDF5.cpp) 
  add_definitions(-DPOUTRE_USE_HDF5)
endif()

if(POUTRE_USE_OIIO)
  list (APPEND PoutreIOSRC_DETAILS ${PoutreIOSubdirectory}/include/poutreOIIO.hpp)
  list (APPEND PoutreIOSRC_CPP ${PoutreIOSubdirectory}/src/poutreOIIO.cpp)
  add_definitions(-DPOUTRE_USE_OIIO)
endif()

source_group(details FILES ${PoutreIOSRC_DETAILS})
source_group(src FILES ${PoutreIOSRC_CPP})
source_group(header FILES ${PoutreIOSRC_PUBLICHEADERS})

set(PoutreIOSRC ${PoutreIOSRC_PUBLICHEADERS} ${PoutreIOSRC_DETAILS}
                ${PoutreIOSRC_CPP})

add_library(PoutreIO ${PoutreIOSRC})


if(POUTRE_USE_HDF5)
  list (APPEND PoutreIOSRC_CPP ${PoutreIOSubdirectory}/src/poutreHDF5.cpp) 
endif()

if(POUTRE_USE_OIIO)
  list (APPEND PoutreIOSRC_CPP ${PoutreIOSubdirectory}/src/poutreOIIO.cpp)
endif()

set(POUTREIO_LINK_LIBRAIRIES
PRIVATE jsoncpp_lib
INTERFACE xsimd
INTERFACE fmt-header-only
PUBLIC PoutreBase
PUBLIC PoutreIPCore)

if(POUTRE_USE_OIIO)
  list (APPEND POUTREIO_LINK_LIBRAIRIES PUBLIC ${OpenImageIO_LIBRARIES})
  add_dependencies(PoutreIO OpenImageIO) # gcc warn -- ?
endif()

if(POUTRE_USE_HDF5)
  list (APPEND POUTREIO_LINK_LIBRAIRIES PUBLIC ${HDF5_LIBRARIES})
endif()

target_link_libraries(PoutreIO ${POUTREIO_LINK_LIBRAIRIES})

set_target_properties(PoutreIO PROPERTIES FOLDER "src/")


if(BUILD_SHARED_LIBS)
  set_target_properties(PoutreIO PROPERTIES COMPILE_FLAGS -DJSON_DLL)
  set_target_properties(PoutreIO PROPERTIES COMPILE_FLAGS -DBOOST_LOG_DLL
  )# FIXME not true should be linked with boost
  set_target_properties(PoutreIO PROPERTIES COMPILE_FLAGS -DBOOST_LOG_DYN_LINK
  )# FIXME not true should be linked with boost
endif(BUILD_SHARED_LIBS)

install_poutre_targets(TARGET PoutreIO CONFIGURATION Release HEADER_FILES
                       ${PoutreIOSRC})
