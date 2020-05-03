# this is the place where config files will be copied, if needed
set(config_folder ${CMAKE_BINARY_DIR}/configs)
file(MAKE_DIRECTORY ${config_folder})

if(MSVC)
  # looking for appropriate paths
  if(CMAKE_CL_64)
    set_property(GLOBAL PROPERTY FIND_LIBRARY_USE_LIB64_PATHS ON)
  else()
    set_property(GLOBAL PROPERTY FIND_LIBRARY_USE_LIB64_PATHS OFF)
  endif()
endif()

# --------------------------------------------------------------------------------
# boost
if(NOT BOOST_ROOT)
  message(STATUS "Boost root not configured, taking the system boost version")
  set(POUTRE_BOOST_FROM_SYSTEM TRUE)
else()
  message(STATUS "Boost root directory set to ${BOOST_ROOT}")
  set(POUTRE_BOOST_FROM_SYSTEM FALSE)
endif()

if(UNIX AND NOT APPLE AND NOT POUTRE_BOOST_FROM_SYSTEM)
  message(WARNING "[POUTRE Boost] you are setting a different boost than the one provided by the system. This option should be taken with care.")
endif()

# disable auto link
add_definitions(-DBOOST_ALL_NO_LIB)
# linking with shared library version
set(Boost_USE_STATIC_LIBS OFF)
#set(Boost_USE_STATIC_LIBS   ON)

if(NOT Boost_USE_STATIC_LIBS)
  # link against dynamic libraries
  add_definitions(-DBOOST_ALL_DYN_LINK)
endif()

# if we are using the system version
if(POUTRE_BOOST_FROM_SYSTEM)
  set(Boost_REALPATH OFF)
else()
  set(Boost_REALPATH ON)
endif()

set(Boost_USE_MULTITHREADED ON)
set(Boost_DEBUG ON)
set(Boost_DETAILED_FAILURE_MSG ON)
if(DEFINED BOOST_ROOT)
  set(Boost_NO_SYSTEM_PATHS ON)
else()
  set(Boost_NO_SYSTEM_PATHS OFF)
endif()
set(Boost_NO_BOOST_CMAKE ON)

set(POUTRE_boost_version_minimal 1.65.1)
set(Boost_ADDITIONAL_VERSIONS   
	"1.66.0" "1.67.0" "1.68.0" "1.69.0" "1.70.0"  "1.71.0"  "1.72.0" "1.73.0")
find_package(Boost ${POUTRE_boost_version_minimal}
             REQUIRED 
            COMPONENTS 
						date_time 
						filesystem 
						system 
						thread 
						regex 
						unit_test_framework 
						#python 
						graph 
						log
						log_setup)
set(Boost_INCLUDE_DIRS ${Boost_INCLUDE_DIRS} PARENT_SCOPE)
set(Boost_LIBRARY_DIRS ${Boost_LIBRARY_DIRS} PARENT_SCOPE)
set(Boost_VERSION      ${Boost_VERSION}      PARENT_SCOPE)
# indicate to the build that we are using the system version (for packaging decisions)
set(POUTRE_BOOST_FROM_SYSTEM ${POUTRE_BOOST_FROM_SYSTEM} PARENT_SCOPE)

set(_POUTRE_current_boost_version  ${Boost_MAJOR_VERSION}.${Boost_MINOR_VERSION})

set(BoostFilesToInstall
  ${Boost_PYTHON_LIBRARY_RELEASE}
  ${Boost_SYSTEM_LIBRARY_RELEASE}
  ${Boost_GRAPH_LIBRARY_RELEASE}  
  ${Boost_DATE_TIME_LIBRARY_RELEASE}
  ${Boost_THREAD_LIBRARY_RELEASE}
  ${Boost_FILESYSTEM_LIBRARY_RELEASE}
  ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY_RELEASE}
  ${Boost_REGEX_LIBRARY_RELEASE}
  ${Boost_LOG_LIBRARY_RELEASE}  
  ${Boost_LOG_SETUP_LIBRARY_RELEASE}  
  )
set(BoostFilesToInstall ${BoostFilesToInstall} PARENT_SCOPE)

# to avoid a change in behaviour introduced between cmake 2.8.7 and 2.8.11
set(Boost_DATE_TIME_LIBRARY           ${Boost_DATE_TIME_LIBRARY}            PARENT_SCOPE)
set(Boost_FILESYSTEM_LIBRARY          ${Boost_FILESYSTEM_LIBRARY}           PARENT_SCOPE)
set(Boost_SYSTEM_LIBRARY              ${Boost_SYSTEM_LIBRARY}               PARENT_SCOPE)
set(Boost_THREAD_LIBRARY              ${Boost_THREAD_LIBRARY}               PARENT_SCOPE)
set(Boost_REGEX_LIBRARY               ${Boost_REGEX_LIBRARY}                PARENT_SCOPE)
set(Boost_UNIT_TEST_FRAMEWORK_LIBRARY ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}  PARENT_SCOPE)
set(Boost_PYTHON_LIBRARY              ${Boost_PYTHON_LIBRARY}               PARENT_SCOPE)
set(Boost_GRAPH_LIBRARY               ${Boost_GRAPH_LIBRARY}                PARENT_SCOPE)
set(Boost_LOG_LIBRARY                 ${Boost_LOG_LIBRARY}                  PARENT_SCOPE)
set(Boost_LOG_SETUP_LIBRARY           ${Boost_LOG_SETUP_LIBRARY}            PARENT_SCOPE)

message(STATUS "Boost library found in " ${Boost_LIBRARY_DIRS})
message(STATUS "Boost major.minor.subminor=${Boost_MAJOR_VERSION}.${Boost_MINOR_VERSION}.${Boost_SUBMINOR_VERSION}")


#installation 
if(NOT POUTRE_BOOST_FROM_SYSTEM)
  install(FILES
    ${BoostFilesToInstall}
    CONFIGURATIONS Release
    DESTINATION ${POUTRE_LIBRARIES_INSTALLATION_RELATIVE_PATH}
    COMPONENT core)

  #add_files_to_python_packaging(FILES ${BoostFilesToInstall})
endif()

#SIMD library
add_subdirectory(xsimd)
SET(SIMD_INLCUDE_DIRECTORY "${SIMD_INLCUDE_DIRECTORY}" PARENT_SCOPE)
SET(SIMD_LIB xsimd PARENT_SCOPE)
SET(SIMD_LIB_DEP XsimdDep PARENT_SCOPE)

#add_subdirectory(nsimd)
#SET(SIMD_INLCUDE_DIRECTORY "${SIMD_INLCUDE_DIRECTORY}" PARENT_SCOPE)
#SET(SIMD_LIB nsimd PARENT_SCOPE)
#SET(SIMD_LIB_DEP NSimdDep PARENT_SCOPE) {% endcomment %}

# jsoncpp
set(JSONCPP_WITH_TESTS CACHE BOOL OFF FORCE)
set(JSONCPP_WITH_CMAKE_PACKAGE CACHE BOOL OFF FORCE)
set(JSONCPP_WITH_PKGCONFIG_SUPPORT CACHE BOOL OFF FORCE)
set(JSONCPP_WITH_POST_BUILD_UNITTEST CACHE BOOL OFF FORCE)
add_subdirectory(jsoncpp)
set_target_properties(jsoncpp_lib PROPERTIES FOLDER "3rdparties/")
SET(JSONCPP_INCLUDE_DIR "${THIRD_PARTIES_PATH}/jsoncpp/include" PARENT_SCOPE)

# zlib
ADD_SUBDIRECTORY(zlib)
INCLUDE_DIRECTORIES(
    ${CMAKE_CURRENT_SOURCE_DIR}/zlib
    ${CMAKE_CURRENT_BINARY_DIR}/zlib)
SET(ZLIB_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/zlib ${CMAKE_CURRENT_BINARY_DIR}/zlib)
SET(ZLIB_LIBRARY zlibstatic)
if(WIN32)
  add_definitions(-DWIN32)
ENDIF()
if(MSVC)
set_property(TARGET zlibstatic PROPERTY DEBUG_POSTFIX "") # turn off the d trailer on library name, too messy for import in openexr.
endif()
SET_PROPERTY(TARGET zlibstatic PROPERTY FOLDER "3rdparties")
SET_PROPERTY(TARGET zlib PROPERTY FOLDER "3rdparties")
set(ZLIB_LIBRARIES ${ZLIB_LIBRARY})
set(ZLIB_INCLUDE_DIRS ${ZLIB_INCLUDE_DIR})
set(ZLIB_LIBRARY ${ZLIB_LIBRARY})
set(ZLIB_FOUND TRUE)
message(STATUS "**************ZLIB: ${ZLIB_INCLUDE_DIR} ${ZLIB_LIBRARIES}")

# libjpeg
set(BUILD_JPEG_STATIC ON)
add_subdirectory(libjpeg)
set(JPEG_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/libjpeg ${CMAKE_CURRENT_BINARY_DIR}/libjpeg) 
set(JPEG_LIBRARY jpeg)
set(JPEG_INCLUDE_DIRS ${JPEG_INCLUDE_DIR}) 
set(JPEG_LIBRARIES ${JPEG_LIBRARY})
INCLUDE_DIRECTORIES(
    ${CMAKE_CURRENT_SOURCE_DIR}/libjpeg 
    ${CMAKE_CURRENT_BINARY_DIR}/libjpeg
)
SET_PROPERTY(TARGET jpeg PROPERTY FOLDER "3rdparties")
set(JPEG_FOUND TRUE)
message(STATUS "**************JPEG: ${JPEG_INCLUDE_DIRS} ${JPEG_LIBRARIES}")

# libpng
set(PNG_STATIC ON)
add_subdirectory(libpng)
set(PNG_PNG_INCLUDE_DIR 
  ${CMAKE_CURRENT_SOURCE_DIR}/libpng
  ${CMAKE_CURRENT_BINARY_DIR}/libpng)
set(PNG_INCLUDE_DIR  ${PNG_PNG_INCLUDE_DIR}) 
# set(PNG_LIBRARY png${PNG_LIB_TARGETS})
set(PNG_LIBRARY png_static)
set(PNG_LIBRARIES ${PNG_LIBRARY})
INCLUDE_DIRECTORIES(
    ${PNG_INCLUDE_DIR}
)
SET_PROPERTY(TARGET png_static PROPERTY FOLDER "3rdparties")
SET_PROPERTY(TARGET genfiles PROPERTY FOLDER "3rdparties")

set(PNG_FOUND TRUE)
message(STATUS "***************PNG: ${PNG_INCLUDE_DIR} ${PNG_LIBRARY}")

# libtiff
add_subdirectory(tiff)
set(TIFF_INCLUDE_DIR 
  ${CMAKE_CURRENT_SOURCE_DIR}/tiff/libtiff
  ${CMAKE_CURRENT_BINARY_DIR}/tiff/libtiff) 
set(TIFF_LIBRARY tiff)
INCLUDE_DIRECTORIES(
    ${TIFF_INCLUDE_DIR}
)
SET_PROPERTY(TARGET tiff PROPERTY FOLDER "3rdparties")
SET_PROPERTY(TARGET tiffxx PROPERTY FOLDER "3rdparties")
SET_PROPERTY(TARGET port PROPERTY FOLDER "3rdparties")
message(STATUS "**************TIFF: ${TIFF_INCLUDE_DIR} ${TIFF_LIBRARY}")

# hdf5
SET(HDF5_EXTERNALLY_CONFIGURED ON CACHE BOOL "HDF5_EXTERNALLY_CONFIGURED" FORCE)
# # Avoid duplicating names of installed libraries
# set (HDF5_EXTERNAL_LIB_PREFIX "poutre" )
# # Export configuration to this export variable
# set (HDF5_EXPORTED_TARGETS "poutre-targets")
add_subdirectory(hdf5)

set(HDF5_FOUND TRUE)
set(HDF5_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/hdf5")
set(HDF5_INCLUDE_DIRS 
  ${CMAKE_CURRENT_SOURCE_DIR}/hdf5/c++/src/header_files
  ${CMAKE_CURRENT_SOURCE_DIR}/hdf5/hl/c++/src
  ${CMAKE_CURRENT_SOURCE_DIR}/hdf5/hl/src
  ${CMAKE_CURRENT_SOURCE_DIR}/hdf5/src
  ${CMAKE_CURRENT_BINARY_DIR}/hdf5
)
set(HDF5_LIBRARIES 
  hdf5_cpp-shared
  hdf5-shared
  # hdf5_hl_cpp-shared
  # hdf5_hl-shared

)
SET_PROPERTY(TARGET hdf5_cpp-shared PROPERTY FOLDER "3rdparties")
SET_PROPERTY(TARGET hdf5-shared PROPERTY FOLDER "3rdparties")
SET_PROPERTY(TARGET hdf5_hl_cpp-shared PROPERTY FOLDER "3rdparties")
SET_PROPERTY(TARGET hdf5_hl-shared PROPERTY FOLDER "3rdparties")
SET_PROPERTY(TARGET H5make_libsettings PROPERTY FOLDER "3rdparties")
SET_PROPERTY(TARGET gen_hdf5-shared PROPERTY FOLDER "3rdparties")

# HighFive
add_subdirectory(HighFive)

#openxr/oiio
if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  # using regular Clang or AppleClang
  add_compile_options("-Wno-deprecated-register")
  add_compile_options("-Wno-implicit-float-conversion")
endif()

#openexr
add_subdirectory(openexr)
set(OPENEXR_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/openexr)
set(OPENEXR_INCLUDE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/openexr)
set(ILMBASE_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/openexr/IlmBase)
set(_OPENEXR_VERSION "2.3")
set(ILMBASE_INCLUDE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/openexr/IlmBase)
SET(OPENEXR_HALF_LIBRARY Half)
SET(OPENEXR_IEX_LIBRARY Iex)
set(OPENEXR_IMATH_LIBRARY Imath)
set(OPENEXR_ILMIMF_LIBRARY IlmImf)
set(OPENEXR_ILMTHREAD_LIBRARY IlmThread)
INCLUDE_DIRECTORIES(
    ${OPENEXR_INCLUDE_PATH}
    ${ILMBASE_INCLUDE_PATH}
    ${ILMBASE_INCLUDE_PATH}/Iex
    ${ILMBASE_INCLUDE_PATH}/Imath
    ${ILMBASE_INCLUDE_PATH}/Half
    ${CMAKE_CURRENT_BINARY_DIR}/openexr/OpenEXR/config
)
SET_PROPERTY(TARGET Half PROPERTY FOLDER "3rdparties")
SET_PROPERTY(TARGET Iex PROPERTY FOLDER "3rdparties")
SET_PROPERTY(TARGET Imath PROPERTY FOLDER "3rdparties")
SET_PROPERTY(TARGET IlmImf PROPERTY FOLDER "3rdparties")
SET_PROPERTY(TARGET IlmThread PROPERTY FOLDER "3rdparties")
SET_PROPERTY(TARGET IexMath PROPERTY FOLDER "3rdparties")
SET_PROPERTY(TARGET IlmImfUtil PROPERTY FOLDER "3rdparties")




#openImageIO
set(OpenImageIO_BUILD_TOOLS CACHE BOOL OFF FORCE)
set(OpenImageIO_BUILD_TESTS CACHE BOOL OFF FORCE)
set(OIIO_BUILD_TESTS CACHE BOOL OFF FORCE)
set(OIIO_BUILD_TOOLS CACHE BOOL OFF FORCE)
set(BUILD_DOCS CACHE BOOL OFF FORCE)
set(INSTALL_DOCS CACHE BOOL OFF FORCE)
set(INSTALL_FONTS CACHE BOOL OFF FORCE)
set(USE_QT CACHE BOOL OFF FORCE)
set(BUILD_FMT_FORCE CACHE BOOL ON FORCE)
set(USE_PYTHON CACHE BOOL OFF FORCE)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/oiio/src/include)
add_subdirectory(oiio)
SET_PROPERTY(TARGET OpenImageIO PROPERTY FOLDER "3rdparties")
SET_PROPERTY(TARGET OpenImageIO_Util PROPERTY FOLDER "3rdparties")
SET(OIIO_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/oiio/src/include PARENT_SCOPE)

# bench
if(POUTRE_BUILD_BENCH)
  set(BUILD_SHARED_LIBS OFF) #temporary
  # We will not need to test benchmark lib itself.
  set(BENCHMARK_ENABLE_TESTING OFF CACHE BOOL "Disable benchmark testing as we don't need it." FORCE)
  # We will not need to install benchmark since we link it statically.
  set(BENCHMARK_ENABLE_INSTALL OFF CACHE BOOL "Disable benchmark install to avoid overwriting vendor install." FORCE)
  #set(BENCHMARK_ENABLE_LTO OFF CACHE BOOL "" FORCE)
  set(BENCHMARK_ENABLE_EXCEPTIONS ON CACHE BOOL "" FORCE)
  add_subdirectory(benchmark)
  set(BUILD_SHARED_LIBS ON) #temporary
  set_target_properties(benchmark PROPERTIES FOLDER "3rdparties/")
  set_target_properties(benchmark_main PROPERTIES FOLDER "3rdparties/")
endif()


# Python libraries

find_package(PythonLibs REQUIRED)
find_package(PythonInterp REQUIRED)
set(PYTHON_LIBRARIES ${PYTHON_LIBRARIES} PARENT_SCOPE)
set(PYTHON_INCLUDE_PATH  ${PYTHON_INCLUDE_PATH} PARENT_SCOPE)
set(PYTHON_EXECUTABLE    ${PYTHON_EXECUTABLE}  PARENT_SCOPE)

string(REPLACE "." ";" VERSION_LIST ${PYTHONLIBS_VERSION_STRING})
list(GET VERSION_LIST 0 PYTHONLIBS_VERSION_MAJOR)
list(GET VERSION_LIST 1 PYTHONLIBS_VERSION_MINOR)

# notes:
# the way boost.python was compiled should be verified (linking against the found python lib)
# the python interpreter should be verified against the python libs (a distribution mismatch might happen)
# set(debian_dependencies ${debian_dependencies} "libpython-dev (= ${PYTHONLIBS_VERSION_MAJOR}.${PYTHONLIBS_VERSION_MINOR})")
# set(debian_dependencies ${debian_dependencies} "python${PYTHON_VERSION_MAJOR}.${PYTHON_VERSION_MINOR}")

if(ENABLE_NUMPY)
  execute_process(
    COMMAND ${PYTHON_EXECUTABLE} -c "import numpy; print numpy.get_include()"
    OUTPUT_VARIABLE NUMPY_INCLUDE_PATH
    ERROR_VARIABLE  NUMPY_ERROR
    OUTPUT_STRIP_TRAILING_WHITESPACE
    )
  if(NOT (NUMPY_ERROR STREQUAL "") OR (NUMPY_INCLUDE_PATH STREQUAL ""))
    message(FATAL_ERROR "[POUTREIO | NUMPY] the following error occured: ${NUMPY_ERROR} - Consider setting PYTHON_ROOT in the environment")
  endif()
  message(STATUS "[POUTREIO] Getting headers for Numpy from ${NUMPY_INCLUDE_PATH}")
  set(NUMPY_INCLUDE_PATH   ${NUMPY_INCLUDE_PATH}     PARENT_SCOPE)
  
  set(debian_dependencies ${debian_dependencies} "python-numpy (>= 1:1.8)")
  
endif()


