cmake_minimum_required(VERSION 3.0)

#Adapted from https://github.com/dream3d/Dream3DSdkBuild/blob/master/Boost.cmake
message("Create ExternalProject HDF5 has thirdpartie lib")

set(HDF5_VERSION "1.8.16")
set(HDF5_PREFIX "${CMAKE_BINARY_DIR}/HDF5")

#https://cmake.org/cmake/help/v3.0/module/ExternalProject.html
set(HDF5_PREFIX   "${CMAKE_BINARY_DIR}/HDF5")
set_property(DIRECTORY PROPERTY EP_BASE ${HDF5_PREFIX})
#TMP_DIR      = <base>/tmp/<name>
#STAMP_DIR    = <base>/Stamp/<name>
#DOWNLOAD_DIR = <base>/Download/<name>
#SOURCE_DIR   = <base>/Source/<name>
#BINARY_DIR   = <base>/Build/<name>
#INSTALL_DIR  = <base>/Install/<name>

if(MSVC)
	set(HDF5_URL "https://www.hdfgroup.org/ftp/HDF5/releases/hdf5-1.8.16/src/hdf5-1.8.16.zip")
else()
	set(HDF5_URL "https://www.hdfgroup.org/ftp/HDF5/releases/hdf5-1.8.16/src/hdf5-1.8.16.tar.gz")
endif()

include(ExternalProject)
ExternalProject_Add(hdf5
	#--Download step--------------
	URL "${HDF5_URL}"
	#--Update/Patch step----------
	UPDATE_COMMAND ""
  	PATCH_COMMAND ""
	#--Configure step-------------
	CMAKE_ARGS
		-DBUILD_SHARED_LIBS:BOOL=${BUILD_SHARED_LIBS}
		-DBUILD_TESTING:BOOL=OFF
		-DHDF5_BUILD_EXAMPLES:BOOL=OFF
		-DHDF5_BUILD_CPP_LIB=ON
		-DHDF5_BUILD_HL_LIB=ON
		-DHDF5_BUILD_TOOLS=OFF
		#-DHDF5_ENABLE_DEPRECATED_SYMBOLS=OFF
		#-DCMAKE_INSTALL_PREFIX=hdf5-${HDF5_VERSION}
		-DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
	#--Build step-----------------
	#--Install step-----------------
	#INSTALL_COMMAND ""
	#DEPENDS ${HDF5_DEPENDENCIES} #todo use same zlib? 
)

ExternalProject_Get_Property(hdf5 TMP_DIR STAMP_DIR DOWNLOAD_DIR SOURCE_DIR BINARY_DIR INSTALL_DIR)
message("Build hdf5 src ${SOURCE_DIR} in ${BINARY_DIR}")
set_target_properties(hdf5 PROPERTIES FOLDER "3rdparties")
