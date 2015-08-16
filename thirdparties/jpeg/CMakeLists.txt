cmake_minimum_required(VERSION 3.0)

find_package(JPEG QUIET)
if(JPEG_FOUND)
	message("LibJPEG found on the system at location is ${JPEG_INCLUDE_DIR} so use it")
    set(JPEG_INCLUDE_DIR       ${JPEG_INCLUDE_DIR}    PARENT_SCOPE)
    set(JPEG_LIBRARIES         ${JPEG_LIBRARIES}      PARENT_SCOPE)

else()
    message("LibJPEG not found on the system so grab one trough internet")  
	message("Create ExternalProject JPEG has thirdpartie lib")
	#SET(libjpeg_PREFIX ${THIRD_PARTIES_PATH})
	SET(libjpeg_PREFIX ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})
	IF(BUILD_SHARED_LIBS)
		SET(JPEG_SHARED ON)
	else()
		SET(JPEG_SHARED OFF)
	endif()
	SET(libjpeg_CMAKE_ARGS
		-DCMAKE_CXX_FLAGS=${CMAKE_CXX_FLAGS}
		-DCMAKE_INSTALL_PREFIX:PATH=<INSTALL_DIR> 
		-DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
		-DCMAKE_TOOLCHAIN_FILE=${CMAKE_TOOLCHAIN_FILE}
		-DCMAKE_OSX_ARCHITECTURES=${CMAKE_OSX_ARCHITECTURES}
		-DJPEG_SHARED=${JPEG_SHARED}
		-DBUILD_SHARED_LIBS=${BUILD_SHARED_LIBS}
		-DSKIP_INSTALL_FILES=1
	)


	include(ExternalProject)
	
	# archive approach
	ExternalProject_Add(jpeg #see http://www.cmake.org/cmake/help/v3.0/module/ExternalProject.html
	  URL http://www.ijg.org/files/jpegsr9a.zip
	  PREFIX ${libjpeg_PREFIX}
	  CMAKE_ARGS ${libjpeg_CMAKE_ARGS}
	  # BINARY_DIR  ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}
	  # TMP_DIR  ${TEMPORARY_DIRECTORY}
      PATCH_COMMAND ${CMAKE_COMMAND} -E copy_if_different ${THIRD_PARTIES_PATH}/jpeg/jpeg.cmake <SOURCE_DIR>/CMakeLists.txt
	  #URL_MD5 7a6f1907f4251f1440382d3095d5c371
	  # CONFIGURE_COMMAND ""
	  # BUILD_COMMAND ""
	  # INSTALL_COMMAND "" 	  
	  ) #SET CONFIGURE_COMMAND, BUILD_COMMAND and INSTALL_COMMAND to "" compile nothing
	  
	# add_dependencies(jpeg zlib)
	set_target_properties(jpeg PROPERTIES FOLDER "Dependencies/")
endif()
