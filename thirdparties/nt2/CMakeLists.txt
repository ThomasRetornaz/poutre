cmake_minimum_required(VERSION 2.8)
project(nt2Builder)
include(ExternalProject)

find_package(Git REQUIRED)

if(NOT WIN32 )  
#set(nt2_PREFIX /home/thomas/nt2install)
set(nt2_PREFIX /home/thomas/src/nt2)
else()
set(nt2_PREFIX E:/programmation/nt2)
endif()


#todo list include to install ?
# ExternalProject_Add(nt2dep
  # PREFIX ${nt2_PREFIX}
  # GIT_REPOSITORY https://github.com/MetaScale/nt2.git
  # GIT_TAG release-3.1
  # CONFIGURE_COMMAND ""
  # BUILD_COMMAND ""
  # INSTALL_COMMAND "" 
# ) #SET CONFIGURE_COMMAND, BUILD_COMMAND and INSTALL_COMMAND to "" compile nothing

set(NT2_ROOT ${nt2_PREFIX})
set(NT2_DIR ${nt2_PREFIX}/cmake)
set(NT2_LIBRARY_ROOT ${nt2_PREFIX}/build/lib)
set(NT2_INCLUDE_ROOT ${nt2_PREFIX}/build/include)

find_package(NT2 REQUIRED)

if(WIN32)
set_property(TARGET nt2 PROPERTY FOLDER "Dependencies/")
set_property(TARGET nt2_bench PROPERTY FOLDER "Dependencies/")
set_property(TARGET nt2_bench_static PROPERTY FOLDER "Dependencies/")
set_property(TARGET nt2_random PROPERTY FOLDER "Dependencies/")
set_property(TARGET nt2_random_static PROPERTY FOLDER "Dependencies/")
set_property(TARGET nt2_static PROPERTY FOLDER "Dependencies/")
set_property(TARGET nt2_unit PROPERTY FOLDER "Dependencies/")
set_property(TARGET nt2_unit_static PROPERTY FOLDER "Dependencies/")
endif()

include(${NT2_USE_FILE})
SET(NT2_INCLUDE_DIR ${NT2_INCLUDE_DIR} PARENT_SCOPE)
