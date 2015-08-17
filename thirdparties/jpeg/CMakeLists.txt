cmake_minimum_required(VERSION 3.0)

find_package(JPEG QUIET)
if(JPEG_FOUND)
	message("LibJPEG found on the system at location is ${JPEG_INCLUDE_DIR} so use it")
    set(JPEG_INCLUDE_DIR       ${JPEG_INCLUDE_DIR}    PARENT_SCOPE)
    set(JPEG_LIBRARIES         ${JPEG_LIBRARIES}      PARENT_SCOPE)

else()
    message("LibJPEG not found on the system so grab one trough internet")  
	message("Create ExternalProject JPEG has thirdpartie lib")
	SET(libjpeg_PREFIX ${TEMPORARY_DIRECTORY})
	IF(BUILD_SHARED_LIBS)
		SET(JPEG_SHARED ON)
	else()
		SET(JPEG_SHARED OFF)
	endif()
	include(ExternalProject)
	
	# archive approach
	ExternalProject_Add(jpeg #see http://www.cmake.org/cmake/help/v3.0/module/ExternalProject.html
	  URL http://www.ijg.org/files/jpegsr9a.zip
	  PREFIX ${libjpeg_PREFIX}
      PATCH_COMMAND ${CMAKE_COMMAND} -E copy_if_different ${THIRD_PARTIES_PATH}/jpeg/jpeg.cmake <SOURCE_DIR>/CMakeLists.txt
	  INSTALL_DIR ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
	  CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=<INSTALL_DIR>
	  ) #SET CONFIGURE_COMMAND, BUILD_COMMAND and INSTALL_COMMAND to "" compile nothing
	  
	
endif()
