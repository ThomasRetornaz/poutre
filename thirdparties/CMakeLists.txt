# this is the place where config files will be copied, if needed
set(config_folder ${CMAKE_BINARY_DIR}/configs)
file(MAKE_DIRECTORY ${config_folder})

# Third parties builds
#include(select_library_configurations)
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

set(Boost_ADDITIONAL_VERSIONS   
    "1.57" "1.57.0" 
	"1.58" "1.58.0"
	"1.59" "1.59.0")

# disable auto link
add_definitions(-DBOOST_ALL_NO_LIB)
# linking with shared library version
set(Boost_USE_STATIC_LIBS OFF)
#set(Boost_USE_STATIC_LIBS   ON)

if(NOT Boost_USE_STATIC_LIBS)
  # link against dynamic libraries
  add_definitions(-DBOOST_ALL_DYN_LINK)
endif()

# if we are using the system version, we do not want to have the exact version embedded in the rpath/ldd
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

set(POUTRE_boost_version_minimal 1.57)

find_package(Boost ${POUTRE_boost_version_minimal}
             REQUIRED 
             COMPONENTS 
						date_time 
						filesystem 
						system 
						thread 
						regex 
						unit_test_framework 
						python 
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


# emit the installation command
if(NOT POUTRE_BOOST_FROM_SYSTEM)
  install(FILES
    ${BoostFilesToInstall}
    CONFIGURATIONS Release
    DESTINATION ${POUTRE_LIBRARIES_INSTALLATION_RELATIVE_PATH}
    COMPONENT core)

  #add_files_to_python_packaging(FILES ${BoostFilesToInstall})
endif()

# nt2 unusable for the moment wait new release 
add_subdirectory(nt2)
SET(NT2_COMPILE_FLAGS ${NT2_COMPILE_FLAGS} PARENT_SCOPE) #we can do better
SET(NT2_LINK_FLAGS ${NT2_LINK_FLAGS} PARENT_SCOPE) #we can do better
SET(NT2_INCLUDE_DIR ${NT2_INCLUDE_DIR} PARENT_SCOPE) #we can do better
# boostsimd
#add_subdirectory(boostsimd)
SET(NT2_COMPILE_FLAGS ${NT2_COMPILE_FLAGS} PARENT_SCOPE) 

# jsoncpp
add_subdirectory(jsoncpp)
#SET(JSONCPP_INCLUDE_DIR ${JSONCPP_INCLUDE_DIR} PARENT_SCOPE)


# png
add_subdirectory(png)
# jpeg
add_subdirectory(jpeg)
# eigen
add_subdirectory(eigen)




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

# HDF5 IO capabilities
if(0)
  if(WIN32)
    #TODO use /install__Windows_x86 default path
    set(HDF5 hdf5)
    set(HDF5_SOURCE_DIR
        hdf5
        CACHE PATH
        "hdf5 sources directory")
    message("hdf5 directory is ${HDF5_SOURCE_DIR}")

    # use http://www.hdfgroup.org/HDF5/release/obtain5.html
    #put it in external_libraries\hdf5
    set(HDF5_INSTALL_DIR ${HDF5_SOURCE_DIR} CACHE PATH "HDF5 installation root path")
    set(HDF5_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/${HDF5_INSTALL_DIR}/include PARENT_SCOPE)
  else(WIN32)
    # to compile with ./configure --prefix=$HOME/usr/local/ --with-zlib=$HOME/usr/local/
    set(HDF5_INSTALL_DIR  ${THIRD_PARTIES_PATH} CACHE PATH "HDF5 installation root path")
    set(HDF5_INCLUDE_DIRS ${HDF5_INSTALL_DIR}/include PARENT_SCOPE)
  endif(WIN32)

  find_library(HDF5LIBB hdf5 PATHS ${HDF5_INSTALL_DIR}/lib/
      DOC "HDF5 library found on the system"
      NO_DEFAULT_PATH
      NO_CMAKE_ENVIRONMENT_PATH
      NO_CMAKE_PATH
      NO_SYSTEM_ENVIRONMENT_PATH
      NO_CMAKE_SYSTEM_PATH
  )
  find_library(HDF5LIBL hdf5_hl PATHS ${HDF5_INSTALL_DIR}/lib/
      DOC "HDF5 Lite library found on the system"
      NO_DEFAULT_PATH
      NO_CMAKE_ENVIRONMENT_PATH
      NO_CMAKE_PATH
      NO_SYSTEM_ENVIRONMENT_PATH
      NO_CMAKE_SYSTEM_PATH
  )

  if(NOT(HDF5LIBB) OR NOT(HDF5LIBL))
    message("Not found HDF5 from the specified path")
    unset(HDF5LIBB CACHE)
    unset(HDF5LIBL CACHE)
    find_package(HDF5)
    message("HDF5 include :" ${HDF5_INCLUDE_DIRS})
    set(HDF5LIBB ${HDF5_hdf5_LIBRARIES})
    set(HDF5LIBL ${HDF5_hdf5_hl_LIBRARIES})
    set(HDF5LIBB ${HDF5_hdf5_LIBRARIES} PARENT_SCOPE)
    set(HDF5LIBL ${HDF5_hdf5_hl_LIBRARIES} PARENT_SCOPE)
  else()
    get_filename_component(HDF5LIBB2 ${HDF5LIBB} REALPATH)
    get_filename_component(HDF5LIBL2 ${HDF5LIBL} REALPATH)
    unset(HDF5LIBB CACHE)
    unset(HDF5LIBL CACHE)
    set(HDF5LIBB ${HDF5LIBB2} CACHE INTERNAL "HDF5 real path" FORCE)
    set(HDF5LIBL ${HDF5LIBL2} CACHE INTERNAL "HDF5HL real path" FORCE)
    set(HDF5LIBB ${HDF5LIBB2} PARENT_SCOPE)
    set(HDF5LIBL ${HDF5LIBL2} PARENT_SCOPE)

    set(HDF5FilesToInstall ${HDF5LIBB} ${HDF5LIBL})
    if(WIN32)
      install_lib_and_dll("${HDF5FilesToInstall}" lib libraries)
    else()
      install_realpath_with_rename("${HDF5FilesToInstall}" lib libraries Release)
    endif()
  endif()
endif()
