cmake_minimum_required (VERSION 3.0)


#project(Eigen)
#http://eigen.tuxfamily.org/index.php?title=Main_Page
message("Create ExternalProject Eigen has thirdpartie lib")
include(ExternalProject)

# clone approach
# find_package(Hg REQUIRED)
# ExternalProject_Add(Eigendep
  # HG_REPOSITORY https://bitbucket.org/eigen/eigen/
  # HG_TAG 3.2
  # PREFIX ${CMAKE_CURRENT_BINARY_DIR}
  # CONFIGURE_COMMAND ""
  # BUILD_COMMAND ""
  # INSTALL_COMMAND "" 
  # ) #SET CONFIGURE_COMMAND, BUILD_COMMAND and INSTALL_COMMAND to "" compile nothing

# archive approach
ExternalProject_Add(Eigendep #see http://www.cmake.org/cmake/help/v3.0/module/ExternalProject.html
  URL http://bitbucket.org/eigen/eigen/get/3.3.2.zip
  PREFIX ${CMAKE_CURRENT_BINARY_DIR}
  CONFIGURE_COMMAND ""
  BUILD_COMMAND ""
  INSTALL_COMMAND "" 
  ) #SET CONFIGURE_COMMAND, BUILD_COMMAND and INSTALL_COMMAND to "" compile nothing
ExternalProject_Get_Property(Eigendep TMP_DIR STAMP_DIR DOWNLOAD_DIR SOURCE_DIR BINARY_DIR INSTALL_DIR)
message("Build Eigendep src ${SOURCE_DIR} in ${BINARY_DIR}")
  
add_library(Eigen INTERFACE)
target_include_directories(Eigen INTERFACE
  $<BUILD_INTERFACE:${SOURCE_DIR}/Eigen> #we can do better
)
set_target_properties(Eigendep PROPERTIES FOLDER "3rdparty/")	
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
