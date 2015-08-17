cmake_minimum_required(VERSION 3.0)

find_package(PNG)
if(PNG_FOUND)
	message("LibPNG found on the system at location is ${PNG_INCLUDE_DIR} so use it")
    set(PNG_INCLUDE_DIR       ${PNG_INCLUDE_DIR}    PARENT_SCOPE)
    set(PNG_LIBRARIES               ${PNG_LIBRARIES}      PARENT_SCOPE)
    set(PNG_DEFINITIONS          ${PNG_DEFINITIONS}    PARENT_SCOPE)
else()
    message("LibPNG not found on the system so grab one trough internet")  
	message("Create ExternalProject PNG has thirdpartie lib")
	SET(libpng_PREFIX ${TEMPORARY_DIRECTORY})
	IF(BUILD_SHARED_LIBS)
		SET(PNG_SHARED ON)
		SET(BUILD_SHARED_LIBS TRUE)
	else()
		SET(PNG_SHARED OFF)
		SET(BUILD_SHARED_LIBS FALSE)
	endif()
	SET(libpng_CMAKE_ARGS
		-DCMAKE_CXX_FLAGS=${CMAKE_CXX_FLAGS}
		-DCMAKE_INSTALL_PREFIX:PATH=<INSTALL_DIR> 
		-DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
		-DCMAKE_TOOLCHAIN_FILE=${CMAKE_TOOLCHAIN_FILE}
		-DCMAKE_OSX_ARCHITECTURES=${CMAKE_OSX_ARCHITECTURES}
		-DPNG_SHARED=${PNG_SHARED}
		-DBUILD_SHARED_LIBS=${BUILD_SHARED_LIBS}
		-DSKIP_INSTALL_FILES=1
	)


	include(ExternalProject)
	
	EXTERNALPROJECT_ADD(zlib
	    PREFIX ${libpng_PREFIX}
		URL http://zlib.net/zlib128.zip
		URL_MD5 126f8676442ffbd97884eb4d6f32afb4
		INSTALL_DIR ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
		CMAKE_ARGS ${libpng_CMAKE_ARGS}
		# CONFIGURE_COMMAND ""
		# BUILD_COMMAND ""
		# INSTALL_COMMAND ""
	)

	ExternalProject_Get_Property(zlib install_dir)
	
	# archive approach
	ExternalProject_Add(png #see http://www.cmake.org/cmake/help/v3.0/module/ExternalProject.html
	  URL http://prdownloads.sourceforge.net/libpng/lpng1618.zip
      PREFIX ${libpng_PREFIX}
	  DEPENDS zlib
	  URL_MD5 7a6f1907f4251f1440382d3095d5c371
	  # CONFIGURE_COMMAND ""
	  # BUILD_COMMAND ""
	  # INSTALL_COMMAND "" 
	  INSTALL_DIR ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
	   CMAKE_ARGS ${libpng_CMAKE_ARGS} -DCMAKE_PREFIX_PATH=${install_dir} # to find zlib
	  ) #SET CONFIGURE_COMMAND, BUILD_COMMAND and INSTALL_COMMAND to "" compile nothing
	  
	set_target_properties(zlib PROPERTIES FOLDER "Dependencies/")
	set_target_properties(png PROPERTIES FOLDER "Dependencies/")
endif()
