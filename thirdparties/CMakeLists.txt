# this is the place where config files will be copied, if needed
set(config_folder ${CMAKE_BINARY_DIR}/configs)
file(MAKE_DIRECTORY ${config_folder})

if(MSVC)
  # looking for appropriate paths
  if(CMAKE_CL_64)
    set_property(GLOBAL PROPERTY FIND_LIBRARY_USE_LIB64_PATHS ON)
  else()
    set_property(GLOBAL PROPERTY FIND_LIBRARY_USE_LIB64_PATHS OFF)
  endif()
endif()


#shut massive amount of warnings comming from thirdparties libaries
if (MSVC)
  add_compile_options(
      ##########################################
      /wd4018 # 'expression' : signed/unsigned mismatch
      /wd4065 # (3): switch with default but no case.
      /wd4146 # unary minus operator applied to unsigned type, result still unsigned
      /wd4244 # Conversion from 'type1' to 'type2', possible loss of data.
      /wd4251 # 'identifier' : class 'type' needs to have dll-interface to be used by clients of class 'type2'
      /wd4267 # Conversion from 'size_t' to 'type', possible loss of data.
      /wd4305 # 'identifier' : truncation from 'type1' to 'type2'
      /wd4355 # 'this' : used in base member initializer list
      /wd4661 # No suitable definition provided for explicit template instantiation request
      /wd4800 # 'type' : forcing value to bool 'true' or 'false' (performance warning)
      /wd4838 # (1): conversion from 'type_1' to 'type_2' requires a
              #      narrowing conversion
      /wd4267 # (3): Conversion of size_t to smaller type. Same as 4244.
      /wd4244
      /wd4273 # (1): inconsistent dll linkage. 
      /wd4090
      /wd4311
      /wd26451
      /wd4312
      /wd4267
      /wd26495
      /wd26812
      /wd26495
      /wd26498
      /wd28251
      /wd6385
      /wd4244
      /wd4334
      /wd4047
      /wd4013
      /wd4334
      /wd4101
      /wd4834
      /wd4005 #'_CRT_SECURE_NO_WARNINGS' : redéfinition de macro
      /wd4251
    )

  set(CMAKE_SHARED_LINKER_FLAGS
      "${CMAKE_SHARED_LINKER_FLAGS} 
      /ignore:4049 
      /ignore:4275 #https://docs.microsoft.com/en-us/cpp/error-messages/compiler-warnings/compiler-warning-level-2-c4275?view=vs-2019
      /ignore:4217") #symbol 'symbol' defined in 'filename.obj' is imported and  symbol 'symbol' defined in 'filename_1.obj' is imported by 'filename_2.obj' in function 'function' 
  set(CMAKE_EXE_LINKER_FLAGS
      "${CMAKE_EXE_LINKER_FLAGS} /ignore:4049 /ignore:4217 /ignore:4275")

else()

  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -fPIC")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-narrowing")
  # Eigen fails to build with some versions, so convert this to a warning
  # Details at http://eigen.tuxfamily.org/bz/show_bug.cgi?id=1459
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wextra")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror=return-type")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-missing-field-initializers")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-type-limits")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-array-bounds")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unknown-pragmas")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-sign-compare")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unused-parameter")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-unused-parameter")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unused-variable")
  #set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unused-but-set-variable")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unused-function")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unused-result")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-strict-overflow")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-strict-aliasing")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-error=deprecated-declarations")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-cast-qual") #hdf5 s***ks
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-ignored-qualifiers") #openexr
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-attributes") #hdf5
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-implicit-fallthrough") #oiio
  if(CMAKE_COMPILER_IS_GNUCXX AND NOT (CMAKE_CXX_COMPILER_VERSION VERSION_LESS 7.0.0))
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-stringop-overflow")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-clobbered")#jpeg
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-discarded-qualifiers") #hdf5
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-maybe-uninitialized") #idem
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-deprecated-copy") #oiio
  endif()
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-error=pedantic")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-error=redundant-decls")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-error=old-style-cast")
  # These flags are not available in GCC-4.8.5. Set only when using clang.
  # Compared against https://gcc.gnu.org/onlinedocs/gcc-4.8.5/gcc/Option-Summary.html
  if("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-deprecated")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-incompatible-pointer-types-discards-qualifiers")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-invalid-partial-specialization")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-typedef-redefinition")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unknown-warning-option")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unused-private-field")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-inconsistent-missing-override")
    #set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-aligned-allocation-unavailable")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-c++14-extensions")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-constexpr-not-const")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-missing-braces")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Qunused-arguments")
    if(${COLORIZE_OUTPUT})
      set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fcolor-diagnostics")
    endif()
  endif()
  if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU" AND CMAKE_CXX_COMPILER_VERSION VERSION_GREATER 4.9)
    if(${COLORIZE_OUTPUT})
      set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fdiagnostics-color=always")
    endif()
  endif()
  if((APPLE AND (NOT ("${CLANG_VERSION_STRING}" VERSION_LESS "9.0")))
    OR(CMAKE_COMPILER_IS_GNUCXX
    AND(CMAKE_CXX_COMPILER_VERSION VERSION_GREATER 7.0 AND NOT APPLE)))
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -faligned-new")
  endif()
  # if(WERROR)
  #   check_cxx_compiler_flag("-Werror" COMPILER_SUPPORT_WERROR)
  #   if(NOT COMPILER_SUPPORT_WERROR)
  #     set(WERROR FALSE)
  #   else()
  #     set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror")
  #   endif()
  # endif(WERROR)
  # if(NOT APPLE)
  #   set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unused-but-set-variable")
  #   set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-maybe-uninitialized")
  # endif()
  set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fno-omit-frame-pointer -O0")
  set(CMAKE_LINKER_FLAGS_DEBUG "${CMAKE_STATIC_LINKER_FLAGS_DEBUG} -fno-omit-frame-pointer -O0")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-math-errno")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-trapping-math")
  check_cxx_compiler_flag("-Werror=format" HAS_WERROR_FORMAT)
  if(HAS_WERROR_FORMAT)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror=format")
  endif()
endif()
# --------------------------------------------------------------------------------
# boost
if(NOT BOOST_ROOT)
  message(STATUS "Boost root not configured, taking the system boost version")
  set(POUTRE_BOOST_FROM_SYSTEM TRUE)
else()
  message(STATUS "Boost root directory set to ${BOOST_ROOT}")
  set(POUTRE_BOOST_FROM_SYSTEM FALSE)
endif()

if(UNIX AND NOT APPLE AND NOT POUTRE_BOOST_FROM_SYSTEM)
  message(WARNING "[POUTRE Boost] you are setting a different boost than the one provided by the system. This option should be taken with care.")
endif()

# disable auto link
add_definitions(-DBOOST_ALL_NO_LIB)
# linking with shared library version
set(Boost_USE_STATIC_LIBS OFF)
#set(Boost_USE_STATIC_LIBS   ON)

if(NOT Boost_USE_STATIC_LIBS)
  # link against dynamic libraries
  add_definitions(-DBOOST_ALL_DYN_LINK)
endif()

# if we are using the system version
if(POUTRE_BOOST_FROM_SYSTEM)
  set(Boost_REALPATH OFF)
else()
  set(Boost_REALPATH ON)
endif()

set(Boost_USE_MULTITHREADED ON)
set(Boost_DEBUG ON)
set(Boost_DETAILED_FAILURE_MSG ON)
if(DEFINED BOOST_ROOT)
  set(Boost_NO_SYSTEM_PATHS ON)
else()
  set(Boost_NO_SYSTEM_PATHS OFF)
endif()
set(Boost_NO_BOOST_CMAKE ON)

set(POUTRE_boost_version_minimal 1.65.1)
set(Boost_ADDITIONAL_VERSIONS   
	 "1.70.0"  "1.71.0"  "1.72.0" "1.73.0" "1.74.0")
find_package(Boost ${POUTRE_boost_version_minimal}
             REQUIRED 
            COMPONENTS 
						date_time 
						filesystem 
						system 
						thread 
						regex 
						graph 
            )
set(Boost_INCLUDE_DIRS ${Boost_INCLUDE_DIRS} PARENT_SCOPE)
set(Boost_LIBRARY_DIRS ${Boost_LIBRARY_DIRS} PARENT_SCOPE)
set(Boost_VERSION      ${Boost_VERSION}      PARENT_SCOPE)
# indicate to the build that we are using the system version (for packaging decisions)
set(POUTRE_BOOST_FROM_SYSTEM ${POUTRE_BOOST_FROM_SYSTEM} PARENT_SCOPE)

set(_POUTRE_current_boost_version  ${Boost_MAJOR_VERSION}.${Boost_MINOR_VERSION})

set(BoostFilesToInstall
  ${Boost_SYSTEM_LIBRARY_RELEASE}
  ${Boost_GRAPH_LIBRARY_RELEASE}  
  ${Boost_DATE_TIME_LIBRARY_RELEASE}
  ${Boost_THREAD_LIBRARY_RELEASE}
  ${Boost_FILESYSTEM_LIBRARY_RELEASE}
  ${Boost_REGEX_LIBRARY_RELEASE}
  )
set(BoostFilesToInstall ${BoostFilesToInstall} PARENT_SCOPE)

# to avoid a change in behaviour introduced between cmake 2.8.7 and 2.8.11
set(Boost_DATE_TIME_LIBRARY           ${Boost_DATE_TIME_LIBRARY}            PARENT_SCOPE)
set(Boost_FILESYSTEM_LIBRARY          ${Boost_FILESYSTEM_LIBRARY}           PARENT_SCOPE)
set(Boost_SYSTEM_LIBRARY              ${Boost_SYSTEM_LIBRARY}               PARENT_SCOPE)
set(Boost_THREAD_LIBRARY              ${Boost_THREAD_LIBRARY}               PARENT_SCOPE)
set(Boost_REGEX_LIBRARY               ${Boost_REGEX_LIBRARY}                PARENT_SCOPE)
set(Boost_GRAPH_LIBRARY               ${Boost_GRAPH_LIBRARY}                PARENT_SCOPE)

message(STATUS "Boost library found in " ${Boost_LIBRARY_DIRS})
message(STATUS "Boost major.minor.subminor=${Boost_MAJOR_VERSION}.${Boost_MINOR_VERSION}.${Boost_SUBMINOR_VERSION}")


#installation 
if(NOT POUTRE_BOOST_FROM_SYSTEM)
  install(FILES
    ${BoostFilesToInstall}
    CONFIGURATIONS Release
    DESTINATION ${POUTRE_LIBRARIES_INSTALLATION_RELATIVE_PATH}
    COMPONENT core)

  #add_files_to_python_packaging(FILES ${BoostFilesToInstall})
endif()


#test
if(POUTRE_BUILD_TEST)
    set(BUILD_GMOCK OFF CACHE BOOL "Disable google test mock." FORCE)
    set(INSTALL_GTEST OFF CACHE BOOL "Disable google test install." FORCE)
    set(gtest_build_tests OFF CACHE BOOL "Disable google test " FORCE)
    add_subdirectory(googletest)
    set_target_properties(gtest PROPERTIES FOLDER "3rdparties/")
    set_target_properties(gtest_main PROPERTIES FOLDER "3rdparties/")
    SET(GTEST_INCLUDE_DIR "${THIRD_PARTIES_PATH}/googletest/include" PARENT_SCOPE)
endif()


#SIMD library
add_subdirectory(xsimd)
SET(SIMD_INLCUDE_DIRECTORY "${SIMD_INLCUDE_DIRECTORY}" PARENT_SCOPE)
SET(SIMD_LIB xsimd PARENT_SCOPE)
SET(SIMD_LIB_DEP XsimdDep PARENT_SCOPE)

#add_subdirectory(nsimd)
#SET(SIMD_INLCUDE_DIRECTORY "${SIMD_INLCUDE_DIRECTORY}" PARENT_SCOPE)
#SET(SIMD_LIB nsimd PARENT_SCOPE)
#SET(SIMD_LIB_DEP NSimdDep PARENT_SCOPE) {% endcomment %}

# jsoncpp
set(JSONCPP_WITH_TESTS CACHE BOOL OFF FORCE)
set(JSONCPP_WITH_CMAKE_PACKAGE CACHE BOOL OFF FORCE)
set(JSONCPP_WITH_PKGCONFIG_SUPPORT CACHE BOOL OFF FORCE)
set(JSONCPP_WITH_POST_BUILD_UNITTEST CACHE BOOL OFF FORCE)
add_subdirectory(jsoncpp)
set_target_properties(jsoncpp_lib PROPERTIES FOLDER "3rdparties/")
set_target_properties(jsoncpp_object PROPERTIES FOLDER "3rdparties/")
set_target_properties(jsoncpp_static PROPERTIES FOLDER "3rdparties/")
SET(JSONCPP_INCLUDE_DIR "${THIRD_PARTIES_PATH}/jsoncpp/include" PARENT_SCOPE)

# zlib
ADD_SUBDIRECTORY(zlib)
INCLUDE_DIRECTORIES(
    ${CMAKE_CURRENT_SOURCE_DIR}/zlib
    ${CMAKE_CURRENT_BINARY_DIR}/zlib)
SET(ZLIB_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/zlib ${CMAKE_CURRENT_BINARY_DIR}/zlib)
SET(ZLIB_LIBRARY zlibstatic)
if(WIN32)
  add_definitions(-DWIN32)
ENDIF()
if(MSVC)
set_property(TARGET zlibstatic PROPERTY DEBUG_POSTFIX "") # turn off the d trailer on library name, too messy for import in openexr.
endif()
SET_PROPERTY(TARGET zlibstatic PROPERTY FOLDER "3rdparties")
SET_PROPERTY(TARGET zlib PROPERTY FOLDER "3rdparties")
set(ZLIB_LIBRARIES ${ZLIB_LIBRARY})
set(ZLIB_INCLUDE_DIRS ${ZLIB_INCLUDE_DIR})
set(ZLIB_LIBRARY ${ZLIB_LIBRARY})
set(ZLIB_FOUND TRUE)
message(STATUS "**************ZLIB: ${ZLIB_INCLUDE_DIR} ${ZLIB_LIBRARIES}")

# libjpeg-turbo
add_subdirectory(libjpeg-turbo)
set(JPEG_INCLUDE_DIR 
  ${CMAKE_CURRENT_SOURCE_DIR}/libjpeg-turbo
  ${CMAKE_CURRENT_BINARY_DIR}/libjpeg-turbo
  ) 
set(JPEG_LIBRARY jpeg-static)
set(JPEG_INCLUDE_DIRS ${JPEG_INCLUDE_DIR}) 
set(JPEG_LIBRARIES ${JPEG_LIBRARY})
INCLUDE_DIRECTORIES(
     ${CMAKE_CURRENT_SOURCE_DIR}/libjpeg-turbo 
     ${CMAKE_CURRENT_BINARY_DIR}/libjpeg-turbo
)
set(JPEG_FOUND TRUE)
message(STATUS "**************JPEG: ${JPEG_INCLUDE_DIRS} ${JPEG_LIBRARIES}")
SET_PROPERTY(TARGET jpeg-static PROPERTY FOLDER "3rdparties")
SET_PROPERTY(TARGET simd PROPERTY FOLDER "3rdparties")

# libpng
SET(PNG_STATIC ON CACHE BOOL "PNG STATIC BUILD" FORCE)
set(HAVE_LD_VERSION_SCRIPT OFF CACHE BOOL "PNG STATIC BUILD WORKAROUND" FORCE)
add_subdirectory(libpng)
set(PNG_PNG_INCLUDE_DIR 
  ${CMAKE_CURRENT_SOURCE_DIR}/libpng
  ${CMAKE_CURRENT_BINARY_DIR}/libpng)
set(PNG_INCLUDE_DIR  ${PNG_PNG_INCLUDE_DIR}) 
# set(PNG_LIBRARY png${PNG_LIB_TARGETS})
set(PNG_LIBRARY png_static)
set(PNG_LIBRARIES ${PNG_LIBRARY})
INCLUDE_DIRECTORIES(
    ${PNG_INCLUDE_DIR}
)
SET_PROPERTY(TARGET png_static PROPERTY FOLDER "3rdparties")
SET_PROPERTY(TARGET genfiles PROPERTY FOLDER "3rdparties")

set(PNG_FOUND TRUE)
message(STATUS "***************PNG: ${PNG_INCLUDE_DIR} ${PNG_LIBRARY}")
add_dependencies(png_static zlib zlibstatic)

# libtiff
add_subdirectory(tiff)
set(TIFF_INCLUDE_DIR 
  ${CMAKE_CURRENT_SOURCE_DIR}/tiff/libtiff
  ${CMAKE_CURRENT_BINARY_DIR}/tiff/libtiff) 
set(TIFF_LIBRARY tiff)
INCLUDE_DIRECTORIES(
    ${TIFF_INCLUDE_DIR}
)
SET_PROPERTY(TARGET tiff PROPERTY FOLDER "3rdparties")
SET_PROPERTY(TARGET tiffxx PROPERTY FOLDER "3rdparties")
SET_PROPERTY(TARGET port PROPERTY FOLDER "3rdparties")
message(STATUS "**************TIFF: ${TIFF_INCLUDE_DIR} ${TIFF_LIBRARY}")
add_dependencies(tiff ${JPEG_LIBRARY} png_static)


# hdf5
SET(HDF5_EXTERNALLY_CONFIGURED ON CACHE BOOL "HDF5_EXTERNALLY_CONFIGURED" FORCE)
set(HDF5_GENERATE_HEADERS OFF CACHE BOOL "Turn off header generation for HDF5" FORCE)
set(HDF5_DISABLE_COMPILER_WARNINGS ON CACHE BOOL "Turn off HDF5 compiler warnings" FORCE)

# # Avoid duplicating names of installed libraries
# set (HDF5_EXTERNAL_LIB_PREFIX "poutre" )
# # Export configuration to this export variable
# set (HDF5_EXPORTED_TARGETS "poutre-targets")
add_subdirectory(hdf5)

set(HDF5_FOUND TRUE)
set(HDF5_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/hdf5")
set(HDF5_INCLUDE_DIRS 
  ${CMAKE_CURRENT_SOURCE_DIR}/hdf5/hl/c++/src
  ${CMAKE_CURRENT_SOURCE_DIR}/hdf5/hl/src
  ${CMAKE_CURRENT_SOURCE_DIR}/hdf5/c++/src
  ${CMAKE_CURRENT_SOURCE_DIR}/hdf5/src
  ${CMAKE_CURRENT_BINARY_DIR}/hdf5
  ${CMAKE_CURRENT_BINARY_DIR}
)
set(HDF5_LIBRARIES 
  hdf5_cpp-shared
  hdf5-shared
  hdf5_hl_cpp-shared
  hdf5_hl-shared
)
set(HDF5_LIBRARIES 
  ${HDF5_LIBRARIES} PARENT_SCOPE
)
set(HDF5_INCLUDE_DIRS
  ${HDF5_INCLUDE_DIRS} PARENT_SCOPE
)
SET_PROPERTY(TARGET hdf5_cpp-shared PROPERTY FOLDER "3rdparties")
SET_PROPERTY(TARGET hdf5-shared PROPERTY FOLDER "3rdparties")
SET_PROPERTY(TARGET hdf5_hl_cpp-shared PROPERTY FOLDER "3rdparties")
SET_PROPERTY(TARGET hdf5_hl-shared PROPERTY FOLDER "3rdparties")
SET_PROPERTY(TARGET H5make_libsettings PROPERTY FOLDER "3rdparties")
SET_PROPERTY(TARGET gen_hdf5-shared PROPERTY FOLDER "3rdparties")
add_dependencies(hdf5_cpp-shared zlib zlibstatic)

# HighFive
# add_subdirectory(HighFive)
# add_dependencies(libdeps hdf5_cpp-shared hdf5-shared)
# set(HIGH_FIVE_INCLUDE_DIR 
#   ${CMAKE_CURRENT_SOURCE_DIR}/HighFive/include) 
# INCLUDE_DIRECTORIES(
#  ${HIGH_FIVE_INCLUDE_DIR}
# )
# SET(HIGH_FIVE_INCLUDE_DIR ${HIGH_FIVE_INCLUDE_DIR} PARENT_SCOPE)

#openxr/oiio
if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  # using regular Clang or AppleClang
  add_compile_options("-Wno-deprecated-register")
  add_compile_options("-Wno-implicit-float-conversion")
endif()

#openexr
add_subdirectory(openexr)
set(OPENEXR_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/openexr)
set(OPENEXR_INCLUDE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/openexr)
set(ILMBASE_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/openexr/IlmBase)
set(_OPENEXR_VERSION "2.3")
set(ILMBASE_INCLUDE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/openexr/IlmBase)
SET(OPENEXR_HALF_LIBRARY Half)
SET(OPENEXR_IEX_LIBRARY Iex)
set(OPENEXR_IMATH_LIBRARY Imath)
set(OPENEXR_ILMIMF_LIBRARY IlmImf)
set(OPENEXR_ILMTHREAD_LIBRARY IlmThread)
INCLUDE_DIRECTORIES(
    ${OPENEXR_INCLUDE_PATH}
    ${ILMBASE_INCLUDE_PATH}
    ${ILMBASE_INCLUDE_PATH}/Iex
    ${ILMBASE_INCLUDE_PATH}/Imath
    ${ILMBASE_INCLUDE_PATH}/Half
    ${CMAKE_CURRENT_BINARY_DIR}/openexr/OpenEXR/config
)
SET_PROPERTY(TARGET Half PROPERTY FOLDER "3rdparties")
SET_PROPERTY(TARGET Iex PROPERTY FOLDER "3rdparties")
SET_PROPERTY(TARGET Imath PROPERTY FOLDER "3rdparties")
SET_PROPERTY(TARGET IlmImf PROPERTY FOLDER "3rdparties")
SET_PROPERTY(TARGET IlmThread PROPERTY FOLDER "3rdparties")
SET_PROPERTY(TARGET IexMath PROPERTY FOLDER "3rdparties")
SET_PROPERTY(TARGET IlmImfUtil PROPERTY FOLDER "3rdparties")
SET(OPENEXR_INCLUDE_DIR 
  ${OPENEXR_INCLUDE_PATH}
  ${ILMBASE_INCLUDE_PATH}
  ${ILMBASE_INCLUDE_PATH}/Iex
  ${ILMBASE_INCLUDE_PATH}/Imath
  ${ILMBASE_INCLUDE_PATH}/Half
  ${CMAKE_CURRENT_BINARY_DIR}/openexr/OpenEXR/config
  ${CMAKE_CURRENT_BINARY_DIR}/openexr/IlmBase/config
  PARENT_SCOPE
)
INCLUDE_DIRECTORIES(${OPENEXR_INCLUDE_DIR})

###########################################################################
# fmtlib
include(FetchContent)
FetchContent_Declare(
  fmt
  GIT_REPOSITORY "https://github.com/fmtlib/fmt"
  # Uncomment and set the following to a specific hash to
  # lock down to a specific commit and avoid contacting
  # the remote once we've already cloned/fetched it
  GIT_TAG 7.0.3
  )

FetchContent_GetProperties(fmt) #mispelled name in original post
if(NOT fmt_POPULATED)   # name is lowercased
  FetchContent_Populate(fmt)
  message(STATUS "fmt source dir: ${fmt_SOURCE_DIR}")
  message(STATUS "fmt binary dir: ${fmt_BINARY_DIR}")
  SET(FMT_HEADER_ONLY ON CACHE BOOL "FMT_HEADER_ONLY" FORCE) 
  add_subdirectory(${fmt_SOURCE_DIR} ${fmt_BINARY_DIR}) # name is lowercased
  set (FMT_INCLUDE_DIR "${fmt_SOURCE_DIR}/include")
  set(CMAKE_PREFIX_PATH ${FMT_INSTALL_DIR})
  set(FMT_INCLUDES ${FMT_INCLUDE_DIR})
  set(FMT_INCLUDES ${FMT_INCLUDES} PARENT_SCOPE)
  #checked_find_package (fmt QUIET)
  set_target_properties(fmt PROPERTIES FOLDER "3rdparties/")

endif()

#spdlog
SET(SPDLOG_FMT_EXTERNAL OFF CACHE BOOL  "Use external fmt library instead of bundled" FORCE)
set(SPDLOG_FMT_EXTERNAL_HO ON CACHE BOOL "Use external fmt header-only library instead of bundled" FORCE)
add_subdirectory(spdlog)
set_target_properties(spdlog PROPERTIES FOLDER "3rdparties/")
INCLUDE_DIRECTORIES(
    ${CMAKE_CURRENT_SOURCE_DIR}/spdlog/include
    ${CMAKE_CURRENT_BINARY_DIR}/spdlog
    )
set(SPDLOG_INCLUDES
    ${CMAKE_CURRENT_SOURCE_DIR}/spdlog/include
    ${CMAKE_CURRENT_BINARY_DIR}/spdlog
    )
set(SPDLOG_INCLUDES ${SPDLOG_INCLUDES} PARENT_SCOPE)

#openImageIO
set(OpenImageIO_BUILD_TOOLS CACHE BOOL OFF FORCE)
set(OpenImageIO_BUILD_TESTS CACHE BOOL OFF FORCE)
set(OIIO_BUILD_TESTS CACHE BOOL OFF FORCE)
set(OIIO_BUILD_TOOLS CACHE BOOL OFF FORCE)
set(BUILD_DOCS CACHE BOOL OFF FORCE)
set(INSTALL_DOCS CACHE BOOL OFF FORCE)
set(INSTALL_FONTS CACHE BOOL OFF FORCE)
set(USE_QT CACHE BOOL OFF FORCE)
set(BUILD_FMT_FORCE CACHE BOOL ON FORCE)
set(USE_PYTHON CACHE BOOL OFF FORCE)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/oiio/src/include)
add_subdirectory(oiio)
SET_PROPERTY(TARGET OpenImageIO PROPERTY FOLDER "3rdparties")
#MESSAGE(FATAL_ERROR ${CMAKE_CURRENT_BINARY_DIR}/include/OpenImageIO)
SET(OIIO_INCLUDE_DIR 
  ${CMAKE_CURRENT_SOURCE_DIR}/oiio/src/include
  ${CMAKE_CURRENT_BINARY_DIR}/include
  ${CMAKE_BINARY_DIR}/include/OpenImageIO
  ${CMAKE_BINARY_DIR}/include
  ${fmt_SOURCE_DIR}/include
  PARENT_SCOPE
)
add_dependencies(OpenImageIO tiff ${JPEG_LIBRARY} png_static hdf5_cpp-shared)
set(OpenImageIO_LIBRARIES OpenImageIO PARENT_SCOPE)


# bench
if(POUTRE_BUILD_BENCH)
  set(BUILD_SHARED_LIBS OFF) #temporary
  # We will not need to test benchmark lib itself.
  set(BENCHMARK_ENABLE_TESTING OFF CACHE BOOL "Disable benchmark testing as we don't need it." FORCE)
  # We will not need to install benchmark since we link it statically.
  set(BENCHMARK_ENABLE_INSTALL OFF CACHE BOOL "Disable benchmark install to avoid overwriting vendor install." FORCE)
  #set(BENCHMARK_ENABLE_LTO OFF CACHE BOOL "" FORCE)
  set(BENCHMARK_ENABLE_EXCEPTIONS ON CACHE BOOL "" FORCE)
  add_subdirectory(benchmark)
  set(BUILD_SHARED_LIBS ON) #temporary
  set_target_properties(benchmark PROPERTIES FOLDER "3rdparties/")
  set_target_properties(benchmark_main PROPERTIES FOLDER "3rdparties/")
endif()


# Python libraries

find_package(PythonLibs REQUIRED)
find_package(PythonInterp REQUIRED)
set(PYTHON_LIBRARIES ${PYTHON_LIBRARIES} PARENT_SCOPE)
set(PYTHON_INCLUDE_PATH  ${PYTHON_INCLUDE_PATH} PARENT_SCOPE)
set(PYTHON_EXECUTABLE    ${PYTHON_EXECUTABLE}  PARENT_SCOPE)

string(REPLACE "." ";" VERSION_LIST ${PYTHONLIBS_VERSION_STRING})
list(GET VERSION_LIST 0 PYTHONLIBS_VERSION_MAJOR)
list(GET VERSION_LIST 1 PYTHONLIBS_VERSION_MINOR)

if(ENABLE_NUMPY)
  find_package(Python COMPONENTS NumPy)
  
  if(NOT Python_NumPy_FOUND)
    message(FATAL_ERROR "[POUTREIO | NUMPY] the following error occured: numpy not found")
  endif()
  message(STATUS "[POUTREIO] Getting headers for Numpy from ${NUMPY_INCLUDE_PATH}")
  set(NUMPY_INCLUDE_PATH   ${NUMPY_INCLUDE_PATH}     PARENT_SCOPE)
  
  set(debian_dependencies ${debian_dependencies} "python-numpy (>= 1:1.8)")
  
endif()

