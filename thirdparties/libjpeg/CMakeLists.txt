# ----------------------------------------------------------------------------
#  CMake file for libjpeg. See root CMakeLists.txt
#
# ----------------------------------------------------------------------------
#find_package(JPEG 9 QUIET)
find_package(JPEG  QUIET)
if(JPEG_FOUND)
  message("LibJPEG found on the system at location is ${JPEG_INCLUDE_DIR} so use it")
  
  set(JPEG_INCLUDE_DIR       ${JPEG_INCLUDE_DIR}    PARENT_SCOPE)
  set(JPEG_LIBRARY         ${JPEG_LIBRARY}      PARENT_SCOPE)
  
else()

set(JPEG_LIBRARY libjpeg)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}
                    ${CMAKE_CURRENT_BINARY_DIR}
                    ${ZLIB_INCLUDE_DIRS})

file(GLOB lib_srcs *.c)
file(GLOB lib_hdrs *.h)

source_group(src FILES ${lib_srcs})
source_group(header FILES ${lib_hdrs})

if(ANDROID OR IOS OR APPLE)
  LIST(REMOVE_ITEM  lib_srcs jmemansi.c )
else()
  LIST(REMOVE_ITEM lib_srcs jmemnobs.c )
endif()

if(WINRT)
    add_definitions(-DNO_GETENV)
    get_directory_property( DirDefs COMPILE_DEFINITIONS )
    message(STATUS "Adding NO_GETENV to compiler definitions for WINRT:")
    message(STATUS "   COMPILE_DEFINITIONS = ${DirDefs}")
endif()

# ----------------------------------------------------------------------------------
#         Define the library target:
# ----------------------------------------------------------------------------------

add_library(${JPEG_LIBRARY} STATIC ${lib_srcs} ${lib_hdrs})

if(UNIX)
  if(CMAKE_COMPILER_IS_GNUCXX OR CV_ICC)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fPIC")
  endif()
endif()

if(CMAKE_COMPILER_IS_GNUCXX)
  set_source_files_properties(jcdctmgr.c PROPERTIES COMPILE_FLAGS "-O1")
endif()

set_target_properties(${JPEG_LIBRARY} PROPERTIES FOLDER "3rdparty")

set(JPEG_INCLUDE_DIR       ${lib_hdrs}    PARENT_SCOPE)
set(JPEG_LIBRARY         ${JPEG_LIBRARY}      PARENT_SCOPE)
 
endif()
