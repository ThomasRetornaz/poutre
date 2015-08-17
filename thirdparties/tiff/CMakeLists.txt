cmake_minimum_required(VERSION 3.0)

find_package(TIFF QUIET)
if(TIFF_FOUND)
	message("LibTIFF found on the system at location is ${TIFF_INCLUDE_DIR} so use it")
    set(TIFF_INCLUDE_DIR       	 ${TIFF_INCLUDE_DIR}    PARENT_SCOPE)
    set(TIFF_LIBRARIES           ${TIFF_LIBRARIES}      PARENT_SCOPE)
    #set(TIFF_DEFINITIONS         ${TIFF_DEFINITIONS}    PARENT_SCOPE)
else()
    message("LibTIFF not found on the system so grab one trough internet")  
	message("Create ExternalProject TIFF has thirdpartie lib")
	SET(libTIFF_PREFIX ${TEMPORARY_DIRECTORY})
	IF(BUILD_SHARED_LIBS)
		SET(TIFF_SHARED ON)
		SET(BUILD_SHARED_LIBS TRUE)
	else()
		SET(TIFF_SHARED OFF)
		SET(BUILD_SHARED_LIBS FALSE)
	endif()
	SET(libTIFF_CMAKE_ARGS
		-DCMAKE_CXX_FLAGS=${CMAKE_CXX_FLAGS}
		-DCMAKE_INSTALL_PREFIX:PATH=<INSTALL_DIR> 
		-DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
		-DCMAKE_TOOLCHAIN_FILE=${CMAKE_TOOLCHAIN_FILE}
		-DCMAKE_OSX_ARCHITECTURES=${CMAKE_OSX_ARCHITECTURES}
		-DTIFF_SHARED=${TIFF_SHARED}
		-DBUILD_SHARED_LIBS=${BUILD_SHARED_LIBS}
		-DSKIP_INSTALL_FILES=1
	)


	include(ExternalProject)
	
	
	# archive approach
	ExternalProject_Add(TIFF #see http://www.cmake.org/cmake/help/v3.0/module/ExternalProject.html
	  URL http://download.osgeo.org/libtiff/tiff-4.0.4.zip
      PREFIX ${libTIFF_PREFIX}
	  DEPENDS zlib jpeg
	  CMAKE_ARGS ${libTIFF_CMAKE_ARGS}
      PATCH_COMMAND ${CMAKE_COMMAND} -E copy_if_different ${THIRD_PARTIES_PATH}/tiff/tiff.cmake <SOURCE_DIR>/CMakeLists.txt
	  #URL_MD5 7a6f1907f4251f1440382d3095d5c371
	  # CONFIGURE_COMMAND ""
	  # BUILD_COMMAND ""
	  # INSTALL_COMMAND "" 
	  # CMAKE_ARGS ${libTIFF_CMAKE_ARGS} -DCMAKE_PREFIX_PATH=${install_dir} # to find zlib
	  ) #SET CONFIGURE_COMMAND, BUILD_COMMAND and INSTALL_COMMAND to "" compile nothing
		
	set_target_properties(TIFF PROPERTIES FOLDER "Dependencies/")
endif()
