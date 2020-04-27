cmake_minimum_required (VERSION 3.0)


#project(xsimd)
#http://eigen.tuxfamily.org/index.php?title=Main_Page
message("Create ExternalProject xsimd has thirdpartie lib")
include(ExternalProject)

# clone approach
find_package(git REQUIRED)
ExternalProject_Add(XsimdDep
  #https://github.com/QuantStack/xsimd.git
  GIT_REPOSITORY  https://github.com/ThomasRetornaz/xsimd.git
  #GIT_TAG master  
  GIT_TAG  ref_314
  PREFIX ${CMAKE_CURRENT_BINARY_DIR}
  #--Update/Patch step----------
	UPDATE_COMMAND ""
  PATCH_COMMAND ""
  INSTALL_COMMAND ""
  #--Configure step-------------
	# 
	BUILD_COMMAND ""	
  )


ExternalProject_Get_Property(XsimdDep TMP_DIR STAMP_DIR DOWNLOAD_DIR SOURCE_DIR BINARY_DIR INSTALL_DIR)   
message("Build XsimdDep src ${SOURCE_DIR} in ${BINARY_DIR}")

add_library(xsimd INTERFACE)
target_include_directories(xsimd INTERFACE
  $<BUILD_INTERFACE:${SOURCE_DIR}/include> #we can do better
  $<INSTALL_INTERFACE:include/xsimd>
)
SET(SIMD_INLCUDE_DIRECTORY "${SOURCE_DIR}/include" PARENT_SCOPE)
set_target_properties(XsimdDep PROPERTIES FOLDER "3rdparties/")	

