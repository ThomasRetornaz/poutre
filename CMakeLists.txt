#Most of the initial cmake skeleton  was heavely inspired from yayi project see https://bitbucket.org/renficiaud/yayi.git. and rapidjosn
cmake_minimum_required (VERSION 3.0)
project(Poutre)

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


enable_testing()

# current version
set(POUTRE_MAJOR    0 )
set(POUTRE_MINOR    1 )
set(POUTRE_SUBMINOR 0 )
set(POUTRE_VERSION ${POUTRE_MAJOR}.${POUTRE_MINOR}.${POUTRE_SUBMINOR})


option(BUILD_SHARED_LIBS "Build Shared Libraries" ON)

set(ENABLE_HDF5  FALSE CACHE BOOL "Enable HDF5 support (you should have it already built)")
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

include(OptimizeForArchitecture)
OptimizeForArchitecture()
# Compiler specific configurations
include(cmake/CMakeCompilerSpecific.cmake)

# if (USE_AVX)
    # message(STATUS "Compiling with AVX support")
# elseif (USE_SSE4_2)
    # message(STATUS "Compiling with SSE4.2 support")
# elseif (USE_SSSE3)
    # message(STATUS "Compiling with SSE3 support")
# else()
    # message(FATAL_ERROR "The target machine does not support SSE3. At least SSE3 is required")
# endif()



message(STATUS "CMake running on OS ${CMAKE_HOST_SYSTEM_NAME} version ${CMAKE_HOST_SYSTEM_VERSION} on processor ${CMAKE_HOST_SYSTEM_PROCESSOR}")
message(STATUS "HDF5 support is ${ENABLE_HDF5}")
message(STATUS "NumPy support is ${ENABLE_NUMPY}")

if(BUILD_SHARED_LIBS)
  message(STATUS "Building shared libraries")
  add_definitions(-DPOUTRE_DYNAMIC)
else()
  message(STATUS "Building static libraries")
endif()

# Some useful functions
include(CMakeParseArguments)
include(cmake/CMakeVersion.cmake)
include(cmake/CMakeInstallUtils.cmake)
#include(cmake/CMakePythonUtils.cmake)
include(cmake/CMakeTestUtils.cmake)

# Python packaging
#message(STATUS "Configuring the python packaging")
#include(cmake/CMakePythonPackage.cmake)

# Third parties
add_subdirectory(${THIRD_PARTIES_PATH})
add_definitions(-DBOOST_TEST_DYN_LINK)
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} ${NT2_COMPILE_FLAGS}")
set(CMAKE_C_FLAGS_RELEASE   "${CMAKE_C_FLAGS_RELEASE} ${NT2_COMPILE_FLAGS}")
#todo filter duplicate and conflict
Set(CMAKE_SHARED_LINKER_FLAGS_RELEASE "${CMAKE_SHARED_LINKER_FLAGS_RELEASE} ${NT2_LINK_FLAGS}")
set(CMAKE_LINKER_FLAGS_RELEASE "${CMAKE_SHARED_LINKER_FLAGS_RELEASE} ${NT2_LINK_FLAGS}")

#FIXME
include_directories(BEFORE "${SIMD_INLCUDE_DIRECTORY}")

# warning the order is important
include_directories(${POUTRE_SRC_DIR})
include_directories(${POUTRE_NRT_DIR})
include_directories(${POUTRE_PYTHONEXT_DIR})

include_directories(${Boost_INCLUDE_DIRS})
include_directories(${JSONCPP_INCLUDE_DIR})

# the poutre core libraries
add_subdirectory (${POUTRE_SRC_DIR})
add_subdirectory (${POUTRE_NRT_DIR})
add_subdirectory (${POUTRE_BENCH_DIR})

# creates the python package
#create_python_package()

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

