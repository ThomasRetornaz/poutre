cmake_minimum_required(VERSION 3.0)
#project(Boost_Compute)
#http://boostorg.github.io/compute/
message("Create ExternalProject Boost::Compute has thirdpartie lib")
include(ExternalProject)

# archive approach
ExternalProject_Add(BoostComputedep 
  URL https://github.com/boostorg/compute/archive/v0.5.zip
  PREFIX ${CMAKE_CURRENT_BINARY_DIR}
  CONFIGURE_COMMAND ""
  BUILD_COMMAND ""
  INSTALL_COMMAND "" 
  ) #SET CONFIGURE_COMMAND, BUILD_COMMAND and INSTALL_COMMAND to "" compile nothing
ExternalProject_Get_Property( BoostComputedep source_dir )  
  
add_library(BoostCompute INTERFACE)
target_include_directories(BoostCompute INTERFACE
  $<BUILD_INTERFACE:${source_dir}> #we can do better
)
set_target_properties(BoostComputedep PROPERTIES FOLDER "3rdparty/")	

#think about installation this later
