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

#set(NT2_ROOT ${CMAKE_CURRENT_BINARY_DIR}/src/BoostSimddep)
set(nt2_PREFIX ${CMAKE_CURRENT_BINARY_DIR}/src/BoostSimddep)  
set(NT2_DIR ${nt2_PREFIX}/cmake)
set(NT2_INCLUDE_ROOT ${nt2_PREFIX}/include)
find_package(NT2  #${POUTRE_nt2_version_minimal}
             REQUIRED 
             COMPONENTS boost.simd
						#boost.dispatch
						#boost.simd.config
						#boost.simd.sdk
			 )

add_library(BoostSimd INTERFACE)
target_include_directories(BoostSimd INTERFACE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/src/BoostSimddep/include> #we can do better
)
set_target_properties(BoostSimddep PROPERTIES FOLDER "Dependencies/")	

#think about this later
# install(TARGETS BoostSimd EXPORT BoostSimdExport)
# install(EXPORT BoostSimdExport NAMESPACE Upstream::
  # DESTINATION lib/cmake/BoostSimd
# )
# install(FILES
    # ${CMAKE_CURRENT_SOURCE_DIR}/src/....
  # DESTINATION include/BoostSimd
# )
