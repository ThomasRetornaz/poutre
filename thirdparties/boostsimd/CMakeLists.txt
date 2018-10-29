cmake_minimum_required (VERSION 3.0)


#project(boostsimd)
#http://eigen.tuxfamily.org/index.php?title=Main_Page
message("Create ExternalProject boost simd has thirdpartie lib")
include(ExternalProject)

# clone approach
find_package(git REQUIRED)
ExternalProject_Add(BoostSimDdep
  GIT_REPOSITORY https://github.com/ThomasRetornaz/boost.simd
  #GIT_TAG issue_519
  GIT_TAG develop  
  PREFIX ${CMAKE_CURRENT_BINARY_DIR}
  CONFIGURE_COMMAND ""
  BUILD_COMMAND ""
  UPDATE_COMMAND "" # Skip annoying updates for every build
  INSTALL_COMMAND "" 
  ) #SET CONFIGURE_COMMAND, BUILD_COMMAND and INSTALL_COMMAND to "" compile nothing


ExternalProject_Get_Property(BoostSimDdep TMP_DIR STAMP_DIR DOWNLOAD_DIR SOURCE_DIR BINARY_DIR INSTALL_DIR)   
message("Build BoostSimDdep src ${SOURCE_DIR} in ${BINARY_DIR}")

  
add_library(BoostSimd INTERFACE)
target_include_directories(BoostSimd INTERFACE
  $<BUILD_INTERFACE:${SOURCE_DIR}/include> #we can do better
  $<INSTALL_INTERFACE:include/BoostSimd>
)
#FIXME INTERFACE NOT WORKING
SET(SIMD_INLCUDE_DIRECTORY "${SOURCE_DIR}/include" PARENT_SCOPE)
set_target_properties(BoostSimDdep PROPERTIES FOLDER "3rdparty/")	
  
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