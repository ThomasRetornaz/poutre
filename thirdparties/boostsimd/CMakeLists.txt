cmake_minimum_required (VERSION 3.0)


#project(boostsimd)
#http://eigen.tuxfamily.org/index.php?title=Main_Page
message("Create ExternalProject boost simd has thirdpartie lib")
include(ExternalProject)

# clone approach
find_package(git REQUIRED)
ExternalProject_Add(BoostSimDdep
  GIT_REPOSITORY https://github.com/NumScale/boost.simd
  #GIT_TAG develop
  GIT_TAG 3695ff6babc63deab068ccc0776825478dac27cb
  PREFIX ${CMAKE_CURRENT_BINARY_DIR}
  CONFIGURE_COMMAND ""
  BUILD_COMMAND ""
  INSTALL_COMMAND "" 
  ) #SET CONFIGURE_COMMAND, BUILD_COMMAND and INSTALL_COMMAND to "" compile nothing

ExternalProject_Get_Property( BoostSimDdep source_dir )  
message("Build BoostSimDdep src ${SOURCE_DIR} in ${BINARY_DIR}")

  
add_library(BoostSimd INTERFACE)
target_include_directories(BoostSimd INTERFACE
  $<BUILD_INTERFACE:${source_dir}/include> #we can do better
)

set_target_properties(BoostSimDdep PROPERTIES FOLDER "3rdparty/")	
#set_target_properties(Eigen PROPERTIES FOLDER "3rdparty/")	

#think about this later
# install(TARGETS Eigen EXPORT eigenExport)
# install(EXPORT eigenExport NAMESPACE Upstream::
  # DESTINATION lib/cmake/Eigen
# )
# install(FILES
    # ${CMAKE_CURRENT_SOURCE_DIR}/src/eigen.h
    # ${CMAKE_CURRENT_SOURCE_DIR}/src/vector.h
    # ${CMAKE_CURRENT_SOURCE_DIR}/src/matrix.h
  # DESTINATION include/Eigen
# )
