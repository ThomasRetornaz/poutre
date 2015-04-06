cmake_minimum_required(VERSION 2.8)
project(nt2Builder)


#TODO make switch is already installed
if(NOT WIN32 )  	
set(nt2_PREFIX /home/thomas/src/nt2)
else()
set(nt2_PREFIX "C:/Program Files/NT2")
endif()

###############################################
##            GIT APPROACH                   ##
###############################################
#find_package(Git REQUIRED)
#set( NT2_VERSION release-3.1)
#message( STATUS "NT2_Version: " ${NT2_VERSION} )
#
#include(ExternalProject)
#
## Below is a CMake command to download, build and install NT2 on the user's computer
#ExternalProject_Add(
#	NT2
#	PREFIX ${CMAKE_CURRENT_BINARY_DIR}
#	GIT_REPOSITORY https://github.com/MetaScale/nt2.git
#	GIT_TAG release-3.1
#	SOURCE_DIR nt2
#	BINARY_DIR nt2-build
#	UPDATE_COMMAND ""
#	PATCH_COMMAND ""
#	CMAKE_GENERATOR ${gen}
#	CMAKE_ARGS
#    ${ep_common_args} -DBOOST_ROOT:PATH=${BOOST_ROOT}
#	)
#set_property( TARGET NT2 PROPERTY FOLDER "Dependencies")
#ExternalProject_Get_Property( NT2 source_dir )
#ExternalProject_Get_Property( NT2 binary_dir )

# set( NT2_INCLUDE_DIRS ${source_dir} )
# set( NT2_LIBRARIES debug;${binary_dir}/stage/lib/libboost_program_options-vc110-mt-gd-1_49.lib;optimized;${binary_dir}/stage/lib/libboost_program_options-vc110-mt-1_49.lib )

set(NT2_DIR ${nt2_PREFIX}/cmake)
find_package(NT2 REQUIRED)
include(${NT2_USE_FILE})
include_directories(${NT2_INCLUDE_DIR})
#MESSAGE(FATAL_ERROR "NT2_INCLUDE_DIR ${NT2_INCLUDE_DIR}")
#MESSAGE(FATAL_ERROR "NT2_COMPILE_FLAGS ${NT2_COMPILE_FLAGS}")

# add_library(BoostSimd INTERFACE)
# set_property(TARGET BoostSimd
   # INTERFACE_INCLUDE_DIRECTORIES
     # "${nt2_PREFIX}/include"
 # )
 
add_library(BoostSimd INTERFACE)
target_include_directories(BoostSimd INTERFACE
  $<BUILD_INTERFACE:${nt2_PREFIX}/include> 
)
# target_compile_definitions(BoostSimd 
  # INTERFACE ${NT2_COMPILE_FLAGS}
  # )
  
add_library(NT2INTERFACE INTERFACE)
target_include_directories(NT2INTERFACE INTERFACE
  $<BUILD_INTERFACE:${nt2_PREFIX}/include> #we can do better
)
# target_compile_definitions(NT2INTERFACE 
  # INTERFACE ${NT2_COMPILE_FLAGS}
  # )
 
SET(NT2_COMPILE_FLAGS ${NT2_COMPILE_FLAGS} PARENT_SCOPE) #we can do better
SET(NT2_LINK_FLAGS ${NT2_LINK_FLAGS} PARENT_SCOPE) #we can do better
#MESSAGE(FATAL_ERROR "NT2_COMPILE_FLAGS ${NT2_COMPILE_FLAGS}")

