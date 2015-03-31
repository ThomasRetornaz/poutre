cmake_minimum_required(VERSION 3.0)
#project(boostsimd)

#https://github.com/MetaScale/nt2/tree/master/modules/boost/simd
#http://fr.slideshare.net/joelfalcou/boostsimd-43671824
message("Create ExternalProject BoostSimd has thirdpartie lib")
include(ExternalProject)

# archive approach
ExternalProject_Add(BoostSimddep #see http://www.cmake.org/cmake/help/v3.0/module/ExternalProject.html
  URL http://nt2.metascale.org/downloads/boost-simd-3.1.0.tgz
  PREFIX ${CMAKE_CURRENT_BINARY_DIR}
  CONFIGURE_COMMAND ""
  BUILD_COMMAND ""
  INSTALL_COMMAND "" 
  ) #SET CONFIGURE_COMMAND, BUILD_COMMAND and INSTALL_COMMAND to "" compile nothing


set(nt2_PREFIX ${CMAKE_CURRENT_BINARY_DIR}/src/BoostSimddep)  
set(NT2_ROOT ${nt2_PREFIX})
set(NT2_DIR ${nt2_PREFIX}/cmake)
#set(NT2_LIBRARY_ROOT ${nt2_PREFIX}/lib)
set(NT2_SOURCE_ROOT  ${nt2_PREFIX})
set(NT2_INCLUDE_ROOT ${nt2_PREFIX}/include)
list(APPEND CMAKE_MODULE_PATH ${NT2_SOURCE_ROOT}/cmake)
find_package(NT2  #${POUTRE_nt2_version_minimal}
             REQUIRED 
             #COMPONENTS boost.simd
						#boost.dispatch
						#boost.simd.config
						#boost.simd.sdk
			 )
include(${NT2_USE_FILE})
add_library(BoostSimd INTERFACE)
target_include_directories(BoostSimd INTERFACE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/src/BoostSimddep/include> #we can do better
)
include_directories(${NT2_INCLUDE_DIR})
set_target_properties(BoostSimddep PROPERTIES FOLDER "Dependencies/")	
#workaround
#MESSAGE(FATAL_ERROR "${NT2_COMPILE_FLAGS}")
#add_compile_options(${NT2_COMPILE_FLAGS})
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${NT2_COMPILE_FLAGS}")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${NT2_LINK_FLAGS}")
set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} ${NT2_LINK_FLAGS}")
set(CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} ${NT2_LINK_FLAGS}")

#include(${NT2_USE_FILE})
#SET(NT2_INCLUDE_DIR ${NT2_INCLUDE_DIR} PARENT_SCOPE)

#think about this later
# install(TARGETS BoostSimd EXPORT BoostSimdExport)
# install(EXPORT BoostSimdExport NAMESPACE Upstream::
  # DESTINATION lib/cmake/BoostSimd
# )
# install(FILES
    # ${CMAKE_CURRENT_SOURCE_DIR}/src/....
  # DESTINATION include/BoostSimd
# )
