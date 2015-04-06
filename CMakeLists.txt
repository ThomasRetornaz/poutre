#Most of the initial cmake skeleton  was stolen from yayi project see https://bitbucket.org/renficiaud/yayi.git. 
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
set(POUTRE_root_dir         ${POUTRE_SOURCE_DIR}   CACHE PATH "Base of the input directory")

# General library settings
set(POUTRE_CORE_DIR
    ${POUTRE_root_dir}/core
    CACHE PATH
    "Root path of the Core of POUTRE")

set(POUTRE_CORE_TEST_DIR
    ${POUTRE_root_dir}/coreTests
    CACHE PATH
    "Root path of the Core of POUTRE's tests")

# set(POUTRE_PYTHONEXT_DIR
    # ${POUTRE_root_dir}/python
    # CACHE PATH
    # "Python extension root path of POUTRE")

set(THIRD_PARTIES_PATH
    ${POUTRE_root_dir}/thirdparties
    CACHE PATH
    "Directory of the external third parties installation (built outside the project)")

set(TEMPORARY_DIRECTORY
    ${CMAKE_BINARY_DIR}/temporary/
    CACHE PATH
    "Directory for temporary files")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/)

set(POUTRE_HEADER_INSTALLATION_RELATIVE_PATH          "include")
set(POUTRE_LIBRARIES_INSTALLATION_RELATIVE_PATH       "lib")
set(POUTRE_DOCUMENTATION_INSTALLATION_RELATIVE_PATH   "documentation")

# this is where the python package is located. The POUTRE binaries will be copied
# into this place when creating the python package. This is also used for the Sphinx documentation.
# if(NOT DEFINED POUTRE_PYTHON_PACKAGE_LOCATION)
  # set(POUTRE_PYTHON_PACKAGE_LOCATION ${POUTRE_root_dir}/plugins/PythonPackage)
# endif()

# Compiler specific configurations
# this also includes some boost configurations
include(cmake/CMakeCompilerSpecific.cmake)



#set(CMAKE_SKIP_RPATH                FALSE)
#set(CMAKE_SKIP_BUILD_RPATH          FALSE)
#set(CMAKE_BUILD_WITH_INSTALL_RPATH  TRUE)
#set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

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
include(cmake/CMakePCHUtils.cmake NO_POLICY_SCOPE)
include(cmake/CMakeInstallUtils.cmake)
#include(cmake/CMakePythonUtils.cmake)
include(cmake/CMakeTestUtils.cmake)

# Python packaging
message(STATUS "Configuring the python packaging")
#include(cmake/CMakePythonPackage.cmake)

# Third parties
#add_subdirectory(${THIRD_PARTIES_PATH} ${TEMPORARY_DIRECTORY}/thirdParties)
add_subdirectory(${THIRD_PARTIES_PATH})
add_definitions(-DBOOST_TEST_DYN_LINK)
#add_compile_options(${NT2_COMPILE_FLAGS}) #USE CACHE INSTEAD ? #USE dispatch to properly set ?
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} ${NT2_COMPILE_FLAGS}")
set(CMAKE_C_FLAGS_RELEASE   "${CMAKE_C_FLAGS_RELEASE} ${NT2_COMPILE_FLAGS}")
#todo filter duplicate and conflict
Set(CMAKE_SHARED_LINKER_FLAGS_RELEASE "${CMAKE_SHARED_LINKER_FLAGS_RELEASE} ${NT2_LINK_FLAGS}")
set(CMAKE_LINKER_FLAGS_RELEASE "${CMAKE_SHARED_LINKER_FLAGS_RELEASE} ${NT2_LINK_FLAGS}")

# warning the order is important
include_directories(${POUTRE_CORE_DIR})
include_directories(${POUTRE_CORE_TEST_DIR})
include_directories(${POUTRE_PYTHONEXT_DIR})

include_directories(${Boost_INCLUDE_DIRS})
include_directories(${NT2_INCLUDE_DIR})
include_directories(${JSONCPP_INCLUDE_DIR})

# the poutre core libraries
add_subdirectory (${POUTRE_CORE_DIR})
add_subdirectory (${POUTRE_CORE_TEST_DIR})

# creates the python package
#create_python_package()

# this should be at the end, in order to retrieve all the include paths (could be also a bit earlier)
message(STATUS "[POUTREDoc] Configuring the documentation")
#include(cmake/CMakeDoc.cmake)

add_custom_target(
  CMakeFiles
  SOURCES
  cmake/CMakeCompilerSpecific.cmake
  # cmake/CMakeCPackConf.cmake
  # cmake/CMakeDoc.cmake
  cmake/CMakeInstallUtils.cmake
  cmake/CMakePCHUtils.cmake
  # cmake/CMakePythonPackage.cmake
  # cmake/CMakePythonUtils.cmake
  cmake/CMakeTestUtils.cmake
  cmake/CMakeVersion.cmake
)

# Should be last
#include(cmake/CMakeCPackConf.cmake)

