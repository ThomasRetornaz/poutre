cmake_minimum_required(VERSION 3.0)

include(CheckTypeSize)
include(CheckFunctionExists)
include(CheckIncludeFile)
include(CheckCSourceCompiles)

check_include_file(sys/types.h HAVE_SYS_TYPES_H)
check_include_file(stdint.h    HAVE_STDINT_H)
check_include_file(stddef.h    HAVE_STDDEF_H)
if ( WIN32 AND NOT CYGWIN )
  #improve? see jconfig.*
  set ( TWO_FILE_COMMANDLINE true )
  # jconfig.h
endif ( )

include_directories ( ${CMAKE_CURRENT_BINARY_DIR} )
configure_file ( jconfig.cfg jconfig.h )


#============================================================================
# jpeg
#============================================================================

set ( LIBJPEG_SRC jmemnobs.c jaricom.c jcapimin.c jcapistd.c jcarith.c jccoefct.c jccolor.c 
  jcdctmgr.c jchuff.c jcinit.c jcmainct.c jcmarker.c jcmaster.c jcomapi.c jcparam.c 
  jcprepct.c jcsample.c jctrans.c jdapimin.c jdapistd.c jdarith.c jdatadst.c jdatasrc.c 
  jdcoefct.c jdcolor.c jddctmgr.c jdhuff.c jdinput.c jdmainct.c jdmarker.c jdmaster.c 
  jdmerge.c jdpostct.c jdsample.c jdtrans.c jerror.c jfdctflt.c jfdctfst.c jfdctint.c 
  jidctflt.c jidctfst.c jidctint.c jquant1.c jquant2.c jutils.c jmemmgr.c cderror.h 
  cdjpeg.h jdct.h jinclude.h jmemsys.h jpegint.h jversion.h transupp.h )

		   
# dep headers
set(LIBJPEG_PUBLIC_HEADERS
${CMAKE_CURRENT_BINARY_DIR}/jconfig.h
jpeglib.h
jmorecfg.h
jerror.h
jinclude.h
jpegint.h
jversion.h
)

if (BUILD_SHARED_LIBS)
    add_library(jpeg SHARED ${LIBJPEG_SRC} ${LIBJPEG_SRC})
ELSE(BUILD_SHARED_LIBS)
    add_library(jpeg STATIC ${LIBJPEG_SRC} ${LIBJPEG_SRC})
ENDIF(BUILD_SHARED_LIBS)
	
set_property(TARGET jpeg PROPERTY FOLDER "Dependencies/")	
set_target_properties(jpeg PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")

set(JPEG                jpeg              PARENT_SCOPE)
set(JPEG_LIB_SRC        ${LIBJPEG_SRC}   PARENT_SCOPE)
set(JPEG_INCLUDE_DIR       ${LIBJPEG_PUBLIC_HEADERS}    PARENT_SCOPE)

