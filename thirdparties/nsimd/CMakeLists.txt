cmake_minimum_required (VERSION 3.0)


#project(nsimd)
#http://eigen.tuxfamily.org/index.php?title=Main_Page
message("Create ExternalProject nsimd has thirdpartie lib")
include(ExternalProject)

# clone approach
find_package(git REQUIRED)
ExternalProject_Add(NSimdDep
  GIT_REPOSITORY https://github.com/agenium-scale/nsimd.git
  #GIT_TAG issue_519
  GIT_TAG master  
  PREFIX ${CMAKE_CURRENT_BINARY_DIR}
  #--Update/Patch step----------
	UPDATE_COMMAND ""
  	PATCH_COMMAND ""
  #--Configure step-------------
	CMAKE_ARGS		
		-DSIMD=${NSIMD_CPU_FLAG} -DSIMD_OPTIONALS=${NSIMD_FMA_FLAG}
	# BUILD_COMMAND ""
	# UPDATE_COMMAND "" # Skip annoying updates for every build
	INSTALL_COMMAND "" 
  ) #SET CONFIGURE_COMMAND, BUILD_COMMAND and INSTALL_COMMAND to "" compile nothing


ExternalProject_Get_Property(NSimdDep TMP_DIR STAMP_DIR DOWNLOAD_DIR SOURCE_DIR BINARY_DIR INSTALL_DIR)   
message("Build NSimdDep src ${SOURCE_DIR} in ${BINARY_DIR}")

# add_library(nsimd INTERFACE)
# target_include_directories(nsimd INTERFACE
  # $<BUILD_INTERFACE:${SOURCE_DIR}/include> #we can do better
  # $<INSTALL_INTERFACE:include/nsimd>
# )


add_library(nsimd STATIC IMPORTED GLOBAL) #MARK AS IMPORTED AND GLOBAL SCOPE
if(WIN32)
set_target_properties(nsimd PROPERTIES IMPORTED_LOCATION ${BINARY_DIR}/Release/libnsimd_x86_64.lib)
else()
set_target_properties(nsimd PROPERTIES IMPORTED_LOCATION ${BINARY_DIR}/libnsimd_x86_64.so)
endif()

#FIXME INTERFACE NOT WORKING
SET(SIMD_INLCUDE_DIRECTORY "${SOURCE_DIR}/include" PARENT_SCOPE)
set_target_properties(NSimdDep PROPERTIES FOLDER "3rdparties/")	
  
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
