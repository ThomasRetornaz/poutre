# ----------------------------------------------------------------------------
#  CMake file for zlib. See root CMakeLists.txt
#
# ----------------------------------------------------------------------------

set(POUTRE_zlib_version_minimal 1.2.8)
find_package(ZLIB ${POUTRE_zlib_version_minimal} QUIET)
if(ZLIB_FOUND)
	message("LibJPEG found on the system at location is ${JPEG_INCLUDE_DIR} so use it")
    set(ZLIB_INCLUDE_DIR       ${ZLIB_INCLUDE_DIR}    PARENT_SCOPE)
    set(ZLIB_LIBRARY        ${ZLIB_LIBRARY}      PARENT_SCOPE)
else()

SET(ZLIB_LIBRARY zlib)
project(${ZLIB_LIBRARY} C)

include(CheckFunctionExists)
include(CheckIncludeFile)
include(CheckCSourceCompiles)
include(CheckTypeSize)

#
# Check for fseeko
#
check_function_exists(fseeko HAVE_FSEEKO)
if(NOT HAVE_FSEEKO)
  add_definitions(-DNO_FSEEKO)
endif()

#
# Check for unistd.h
#
check_include_file(unistd.h Z_HAVE_UNISTD_H)

if(MSVC)
  add_definitions(-D_CRT_SECURE_NO_DEPRECATE)
  add_definitions(-D_CRT_NONSTDC_NO_DEPRECATE)
endif()

#
# Check to see if we have large file support
#
check_type_size(off64_t OFF64_T)
if(HAVE_OFF64_T)
  add_definitions(-D_LARGEFILE64_SOURCE=1)
endif()

configure_file("${CMAKE_CURRENT_SOURCE_DIR}/zconf.h.cmakein"
               "${CMAKE_CURRENT_BINARY_DIR}/zconf.h" @ONLY)
               
include_directories(${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR})

set(ZLIB_PUBLIC_HDRS
    "${CMAKE_CURRENT_BINARY_DIR}/zconf.h"
    zlib.h
)
set(ZLIB_PRIVATE_HDRS
    crc32.h
    deflate.h
    gzguts.h
    inffast.h
    inffixed.h
    inflate.h
    inftrees.h
    trees.h
    zutil.h
)
set(ZLIB_SRCS
    adler32.c
    compress.c
    crc32.c
    deflate.c
    gzclose.c
    gzlib.c
    gzread.c
    gzwrite.c
    inflate.c
    infback.c
    inftrees.c
    inffast.c
    trees.c
    uncompr.c
    zutil.c
)

source_group(details FILES ${ZLIB_PRIVATE_HDRS})
source_group(src FILES ${ZLIB_SRCS})
source_group(header FILES ${ZLIB_PUBLIC_HDRS})

add_library(${ZLIB_LIBRARY} STATIC ${ZLIB_SRCS} ${ZLIB_PUBLIC_HDRS} ${ZLIB_PRIVATE_HDRS})
set_target_properties(${ZLIB_LIBRARY} PROPERTIES DEFINE_SYMBOL ZLIB_DLL)

if(UNIX)
  if(CMAKE_COMPILER_IS_GNUCXX OR CV_ICC)
     set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fPIC")
  endif()
endif()

set_target_properties(${ZLIB_LIBRARY} PROPERTIES FOLDER "3rdparty")
set(ZLIB_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR}) #for zconf
set(ZLIB_INCLUDE_DIR       ${ZLIB_INCLUDE_DIR}    PARENT_SCOPE)
set(ZLIB_LIBRARY         ${ZLIB_LIBRARY}      PARENT_SCOPE)

endif()