cmake_minimum_required (VERSION 3.0)


#project(libsimdpp)
#http://eigen.tuxfamily.org/index.php?title=Main_Page
message("Create ExternalProject libsimdpp has thirdpartie lib")
include(ExternalProject)

# clone approach
find_package(git REQUIRED)
ExternalProject_Add(libsimdppdep
  GIT_REPOSITORY https://github.com/ThomasRetornaz/libsimdpp
  #GIT_TAG issue_519
  GIT_TAG dev
  PREFIX ${CMAKE_CURRENT_BINARY_DIR}
  CONFIGURE_COMMAND ""
  BUILD_COMMAND ""
  INSTALL_COMMAND "" 
  ) #SET CONFIGURE_COMMAND, BUILD_COMMAND and INSTALL_COMMAND to "" compile nothing

ExternalProject_Get_Property(libsimdppdep TMP_DIR STAMP_DIR DOWNLOAD_DIR SOURCE_DIR BINARY_DIR INSTALL_DIR)  
message("Build libsimddpdep src ${SOURCE_DIR} in ${BINARY_DIR}")

add_library(libsimdpp INTERFACE)
target_include_directories(libsimdpp INTERFACE
  $<BUILD_INTERFACE:${SOURCE_DIR}>
  $<INSTALL_INTERFACE:include/libsimdpp>
)

set_target_properties(libsimdppdep PROPERTIES FOLDER "3rdparty/")
  
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
