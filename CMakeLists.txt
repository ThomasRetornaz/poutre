#Most of the initial cmake skeleton  was heavely inspired from yayi project see https://bitbucket.org/renficiaud/yayi.git. and rapidjosn
cmake_minimum_required (VERSION 3.0)
project(Poutre C CXX)
#set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 20)

# some global properties
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set_property(GLOBAL PROPERTY PREDEFINED_TARGETS_FOLDER "CMakePredefined")

# build type, by default to release (with optimisations)
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message(STATUS "Setting build type to 'Release' as none was specified.")
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build." FORCE)
  # Set the possible values of build type for cmake-gui
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()

OPTION(POUTRE_BUILD_TEST "Build Tests Suites" ON)
OPTION(POUTRE_BUILD_BENCH "Build Benchmark Suites" ON)
OPTION(POUTRE_BUILD_PY "Build python exports" ON)

if(POUTRE_BUILD_TEST)
    enable_testing()
endif()


# current version
set(POUTRE_MAJOR    0 )
set(POUTRE_MINOR    1 )
set(POUTRE_SUBMINOR 0 )
set(POUTRE_VERSION ${POUTRE_MAJOR}.${POUTRE_MINOR}.${POUTRE_SUBMINOR})


option(BUILD_SHARED_LIBS "Build Shared Libraries" ON)

set(ENABLE_NUMPY FALSE CACHE BOOL "Enable NumPy support for IOPython (should be installed in your python distribution")

set(POUTRE_SOURCE_DIR         ${CMAKE_CURRENT_SOURCE_DIR}   CACHE PATH "Base of the input directory")
set(POUTRE_ROOT_DIR         ${POUTRE_SOURCE_DIR}   CACHE PATH "Base of the input directory")

# General library settings
set(POUTRE_SRC_DIR
    ${POUTRE_ROOT_DIR}/src
    CACHE PATH
    "Root path of the Core of POUTRE")

set(POUTRE_NRT_DIR
    ${POUTRE_ROOT_DIR}/nrt
    CACHE PATH
    "Root path of the Core of POUTRE's tests")

set(POUTRE_BENCH_DIR
	${POUTRE_ROOT_DIR}/bench
    CACHE PATH
    "Root path of the Core of POUTRE's bench")

# set(POUTRE_PYTHONEXT_DIR
    # ${POUTRE_root_dir}/python
    # CACHE PATH
    # "Python extension root path of POUTRE")

set(THIRD_PARTIES_PATH
    ${POUTRE_ROOT_DIR}/thirdparties
    CACHE PATH
    "Directory of the external third parties installation (built outside the project)")

set(TEMPORARY_DIRECTORY
    ${CMAKE_BINARY_DIR}/temporary/
    CACHE PATH
    "Directory for temporary files")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

set(POUTRE_BINARY_INSTALLATION_RELATIVE_PATH          "bin" CACHE PATH "Where to install binaries")
set(POUTRE_HEADER_INSTALLATION_RELATIVE_PATH          "include" CACHE PATH "Where to install headers")
set(POUTRE_LIBRARIES_INSTALLATION_RELATIVE_PATH       "lib" CACHE PATH "Where to install libraries")
set(POUTRE_DOCUMENTATION_INSTALLATION_RELATIVE_PATH   "documentation" CACHE PATH "Where to install doc")

set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})

# Compiler specific configurations
include(cmake/AddCompilerFlag.cmake)
include(cmake/AddCXXCompilerFlag.cmake)
include(cmake/c++-standards.cmake)
include(cmake/code-coverage.cmake)
include(cmake/formatting.cmake)
include(cmake/CMakeCompilerSpecific.cmake)
include(cmake/OptimizeForArchitecture.cmake)
OptimizeForArchitecture()
foreach(flag ${Vc_ARCHITECTURE_FLAGS})
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${flag}")
endforeach()

message(STATUS "")
message(STATUS "******** Summary ********")
message(STATUS "General:")
message(STATUS "  CMake version         : ${CMAKE_VERSION}")
message(STATUS "  CMake command         : ${CMAKE_COMMAND}")
message(STATUS "  CMake generator       : ${CMAKE_GENERATOR}")
message(STATUS "  Host System           : ${CMAKE_HOST_SYSTEM_NAME}")
message(STATUS "  System                : ${CMAKE_SYSTEM_NAME}")
message(STATUS "  Target OS             : ${TARGET_OS}")
message(STATUS "  C++ compiler          : ${CMAKE_CXX_COMPILER}")
message(STATUS "  C++ compiler id       : ${CMAKE_CXX_COMPILER_ID}")
message(STATUS "  C++ compiler version  : ${CMAKE_CXX_COMPILER_VERSION}")
message(STATUS "  C compiler            : ${CMAKE_C_COMPILER}")
message(STATUS "  C compiler id         : ${CMAKE_C_COMPILER_ID}")
message(STATUS "  C compiler version    : ${CMAKE_C_COMPILER_VERSION}")
message(STATUS "  Link flags             : ${CMAKE_EXE_LINKER_FLAGS}")
message(STATUS "  CXX flags             : ${CMAKE_CXX_FLAGS}")
message(STATUS "  Build type            : ${CMAKE_BUILD_TYPE}")
message(STATUS "  CMAKE_PREFIX_PATH     : ${CMAKE_PREFIX_PATH}")
message(STATUS "  CMAKE_INSTALL_PREFIX  : ${CMAKE_INSTALL_PREFIX}")
message(STATUS "NumPy                   : ${ENABLE_NUMPY}")


if(BUILD_SHARED_LIBS)
  message(STATUS "Building shared libraries")
  add_definitions(-DPOUTRE_DYNAMIC)
else()
  message(STATUS "Building static libraries")
endif()

if(CMAKE_BUILD_TYPE)
    string(TOLOWER ${CMAKE_BUILD_TYPE} CMAKE_BUILD_TYPE_LOWER)
    if (${CMAKE_BUILD_TYPE_LOWER} MATCHES "coverage")
        set(CODE_COVERAGE cache bool ON)
    endif()
endif()

# Some useful functions
include(CMakeParseArguments)
include(cmake/CMakeVersion.cmake)
include(cmake/CMakeInstallUtils.cmake)
#include(cmake/CMakePythonUtils.cmake)

# Python packaging
#message(STATUS "Configuring the python packaging")
#include(cmake/CMakePythonPackage.cmake)

# Third parties
add_subdirectory(${THIRD_PARTIES_PATH})
add_definitions(-DBOOST_TEST_DYN_LINK)

#FIXME
include_directories(BEFORE "${SIMD_INLCUDE_DIRECTORY}")
include_directories(${Boost_INCLUDE_DIRS})
include_directories(${JSONCPP_INCLUDE_DIR})
include_directories(BEFORE ${FMT_INCLUDES})
#message(FATAL_ERROR "${FMT_INCLUDES}")

# warning the order is important
include_directories(${POUTRE_SRC_DIR})
include_directories(${POUTRE_NRT_DIR})
include_directories(${POUTRE_PYTHONEXT_DIR})



if(CODE_COVERAGE)
   add_code_coverage_all_targets(EXCLUDE */boost/* */7/*)
endif()

# the poutre core libraries
add_subdirectory (${POUTRE_SRC_DIR})
if(POUTRE_BUILD_TEST)
    include(CTest)
    add_subdirectory (${POUTRE_NRT_DIR})
endif()
if(POUTRE_BUILD_BENCH)
    add_subdirectory (${POUTRE_BENCH_DIR})
endif()    
if(POUTRE_BUILD_PY)
    # creates the python package
    #create_python_package()
endif()    

message(STATUS "Configuring the documentation")
include(cmake/CMakeDoc.cmake)

#add_custom_target(
#  CMakeFiles
#  SOURCES
#  cmake/CMakeCompilerSpecific.cmake
#  # cmake/CMakeCPackConf.cmake
#  cmake/CMakeDoc.cmake
#  cmake/CMakeInstallUtils.cmake
#  # cmake/CMakePythonPackage.cmake
#  # cmake/CMakePythonUtils.cmake
#  cmake/CMakeTestUtils.cmake
#  cmake/CMakeVersion.cmake
#)

# Should be last
#include(cmake/CMakeCPackConf.cmake)

