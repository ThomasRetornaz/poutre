# ----------------------------------------------------------------------------
#  CMake file for libpng. See root CMakeLists.txt
#
# ----------------------------------------------------------------------------

set(POUTRE_png_version_minimal 1.6.19)
find_package(PNG ${POUTRE_png_version_minimal} QUIET)
if(PNG_FOUND)
	message("LibPNG found on the system at location is ${PNG_INCLUDE_DIR} so use it")
  set(PNG_INCLUDE_DIR       ${PNG_INCLUDE_DIR}    PARENT_SCOPE)
  set(PNG_LIBRARY       ${PNG_LIBRARY}      PARENT_SCOPE)

else()

set(PNG_LIBRARY libpng)
    
if(ENABLE_NEON)
  project(${PNG_LIBRARY} C ASM)
else()
  project(${PNG_LIBRARY} C)
endif()

if(NOT WIN32)
  find_library(M_LIBRARY
    NAMES m
    PATHS /usr/lib /usr/local/lib
  )
  if(NOT M_LIBRARY)
    message(STATUS "math lib 'libm' not found; floating point support disabled")
  endif()
else()
  # not needed on windows
  set(M_LIBRARY "")
endif()

include_directories(${CMAKE_CURRENT_SOURCE_DIR}
                    ${ZLIB_INCLUDE_DIR}
                    ${CMAKE_CURRENT_BINARY_DIR})

file(GLOB lib_srcs *.c)
file(GLOB lib_hdrs *.h)


if(ENABLE_NEON)
  list(APPEND lib_srcs arm/arm_init.c arm/filter_neon.S arm/filter_neon_intrinsics.c)
  add_definitions(-DPNG_ARM_NEON_OPT=2)
endif()

# ----------------------------------------------------------------------------------
#         Define the library target:
# ----------------------------------------------------------------------------------

if(MSVC)
  add_definitions(-D_CRT_SECURE_NO_DEPRECATE)
endif(MSVC)

add_library(${PNG_LIBRARY} STATIC ${lib_srcs} ${lib_hdrs})
target_link_libraries(${PNG_LIBRARY} ${ZLIB_LIBRARIE})

if(UNIX)
  if(CMAKE_COMPILER_IS_GNUCXX OR CV_ICC)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fPIC")
  endif()
endif()

# ocv_warnings_disable(CMAKE_C_FLAGS -Wcast-align)

# set_target_properties(${PNG_LIBRARY}
  # PROPERTIES OUTPUT_NAME ${PNG_LIBRARY}
  # DEBUG_POSTFIX "${OPENCV_DEBUG_POSTFIX}"
  # COMPILE_PDB_NAME ${PNG_LIBRARY}
  # COMPILE_PDB_NAME_DEBUG "${PNG_LIBRARY}${OPENCV_DEBUG_POSTFIX}"
  # ARCHIVE_OUTPUT_DIRECTORY ${3P_LIBRARY_OUTPUT_PATH}
  # )
set_target_properties(${PNG_LIBRARY} PROPERTIES FOLDER "3rdparty")
set(PNG_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(PNG_INCLUDE_DIR       ${PNG_INCLUDE_DIR}    PARENT_SCOPE)
set(PNG_LIBRARY         ${PNG_LIBRARY}      PARENT_SCOPE)

if(NOT BUILD_SHARED_LIBS)
  #ocv_install_target(${PNG_LIBRARY} EXPORT OpenCVModules ARCHIVE DESTINATION ${OPENCV_3P_LIB_INSTALL_PATH} COMPONENT dev)
endif()
endif()
