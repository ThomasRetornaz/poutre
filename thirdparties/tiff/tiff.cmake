#inspired from https://github.com/LuaDist/libtiff/blob/master/CMakeLists.txt
cmake_minimum_required(VERSION 3.0)

#CONFIGURE
message ( "Configure: libtiff" )
include ( CheckFunctionExists )
include ( CheckIncludeFiles )
include ( CheckLibraryExists )
include ( CheckSymbolExists )
include ( CheckTypeSize )

set (CCITT_SUPPORT 1 )
set (CHECK_JPEG_YCBCR_SUBSAMPLING 1 )
set (CXX_SUPPORT 1 )
set (DEFAULT_EXTRASAMPLE_AS_ALPHA 1 )


check_include_files ( assert.h HAVE_ASSERT_H )
check_include_files ( dlfcn.h HAVE_DLFCN_H )
check_include_files ( fcntl.h HAVE_FCNTL_H )
check_function_exists ( floor HAVE_FLOOR )
check_function_exists ( getopt HAVE_GETOPT )

set ( HAVE_IEEEFP 1 )

check_include_files ( inttypes.h HAVE_INTTYPES_H )
check_include_files ( isascii HAVE_ISASCII )
check_function_exists ( lfind HAVE_LFIND )

check_library_exists ( c "" "" HAVE_LIBC )
check_library_exists ( m "" "" HAVE_LIBM )

check_include_files ( limits.h HAVE_LIMITS_H )
check_include_files ( malloc.h HAVE_MALLOC_H )
check_function_exists ( memmove HAVE_MEMMOVE )

check_include_files ( memory.h HAVE_MEMORY_H )

check_function_exists ( memset HAVE_MEMSET )
check_function_exists ( mmap HAVE_MMAP )
check_function_exists ( pow HAVE_POW )

# Define if you have POSIX threads libraries and header files.
#undef HAVE_PTHREAD

check_include_files ( search.h HAVE_SEARCH_H )
check_function_exists ( sqrt HAVE_SQRT )
check_include_files ( stdint.h HAVE_STDINT_H )
check_include_files ( stdlib.h HAVE_STDLIB_H )
check_function_exists ( strcasecmp HAVE_STRCASECMP )
check_function_exists ( strchr HAVE_STRCHR )
check_include_files ( strings.h HAVE_STRINGS_H )
check_include_files ( string.h HAVE_STRING_H )
check_function_exists ( strrchr HAVE_STRRCHR )
check_function_exists ( strstr HAVE_STRSTR )
check_function_exists ( strtol HAVE_STRTOL )
check_function_exists ( strtoul HAVE_STRTOUL )
check_include_files ( "sys/stat.h" HAVE_SYS_STAT_H )
check_include_files ( "sys/time.h" HAVE_SYS_TIME_H )
check_include_files ( "sys/types.h" HAVE_SYS_TYPES_H )
check_include_files ( unistd.h HAVE_UNISTD_H )
check_include_files ( windows.h HAVE_WINDOWS_H )

# Native cpu byte order: 1 if big-endian (Motorola) or 0 if little-endian
# (Intel)
set ( HOST_BIGENDIAN 0 )

# Set the native cpu bit order (FILLORDER_LSB2MSB or FILLORDER_MSB2LSB)
set ( HOST_FILLORDER "FILLORDER_LSB2MSB" )

if ( HAVE_JPEG )
  set ( JPEG_SUPPORT 1 )
endif ( )
set ( LOGLUV_SUPPORT 1 )

set ( LZW_SUPPORT 1 )
set ( MDI_SUPPORT 1 )
set ( NEXT_SUPPORT 1 )

# Define to 1 if your C compiler doesn't accept -c and -o together.
#undef NO_MINUS_C_MINUS_O

# Support Old JPEG compresson (read contrib/ojpeg/README first!
# Compilation fails with unpatched IJG JPEG library)
#undef OJPEG_SUPPORT

set ( PACKAGE "tiff" )
set ( PACKAGE_BUGREPORT "tiff@lists.maptools.org" )
set ( PACKAGE_NAME "LibTIFF Software" )
set ( PACKAGE_STRING "LibTIFF Software 4.0.4" )
set ( PACKAGE_TARNAME "tiff" )
set ( PACKAGE_VERSION "4.0.4" )
set ( PACKBITS_SUPPORT 1 )
set ( PIXARLOG_SUPPORT 1 )

#Define to necessary symbol if this constant uses a non-standard name onyour system.
#undef PTHREAD_CREATE_JOINABLE

check_type_size ( "int" SIZEOF_INT )
check_type_size ( "long" SIZEOF_LONG )

set ( STDC_HEADERS 1 )
set ( STRIPCHOP_DEFAULT "TIFF_STRIPCHOP" )
set ( STRIP_SIZE_DEFAULT 8192 )
set ( SUBIFD_SUPPORT 1 )
set ( THUNDER_SUPPORT 1 )
set ( TIME_WITH_SYS_TIME 1 )

check_symbol_exists ( "struct tm" "sys/time.h" TM_IN_SYS_TIME )
set ( VERSION "4.0.4" )

# Define to 1 if your processor stores words with the most significant byte first
# (like Motorola and SPARC, unlike Intel and VAX).
#undef WORDS_BIGENDIAN

set ( X_DISPLAY_MISSING 1 )
set ( ZIP_SUPPORT 1 )
# Number of bits in a file offset, on hosts where this is settable.
#undef _FILE_OFFSET_BITS

# Define for large files, on AIX-style hosts.
#undef _LARGE_FILES

# Define to empty if `const' does not conform to ANSI C.
set ( const const )

# Define to `__inline__' or `__inline' if that's what the C compiler
# calls it, or to nothing if 'inline' is not supported under any name.
#set ( inline ) # improve?
if( APPLE )
  if( CMAKE_C_COMPILER_ID MATCHES "Clang" )
    set( inline inline )
  endif()
endif()

#Define to `long' if <sys/types.h> does not define.
#undef off_t

#Define to `unsigned' if <sys/types.h> does not define.
#undef size_t

include_directories ( ${CMAKE_CURRENT_BINARY_DIR} )

# all is defined, so generate config.h
configure_file ( libtiff/tif_config.h.in tif_config.h )
configure_file ( libtiff/tiffconf.h.in tiffconf.h )
message ( "Configure: libtiff - done." )
add_definitions ( -DHAVE_CONFIG_H )


set ( LIBTIFF_SRC tif_aux.c tif_close.c tif_codec.c tif_color.c tif_compress.c tif_dir.c 
  tif_dirinfo.c tif_dirread.c tif_dirwrite.c tif_dumpmode.c tif_error.c tif_extension.c 
  tif_fax3.c tif_fax3sm.c tif_flush.c tif_getimage.c tif_jpeg.c tif_luv.c tif_lzw.c 
  tif_next.c tif_ojpeg.c tif_open.c tif_packbits.c tif_pixarlog.c tif_predict.c tif_print.c 
  tif_read.c tif_strip.c tif_swab.c tif_thunder.c tif_tile.c tif_version.c tif_warning.c 
  tif_write.c tif_zip.c )
if ( UNIX )
  list ( APPEND LIBTIFF_SRC tif_unix.c )
elseif ( WIN32 )
list ( APPEND LIBTIFF_SRC tif_win32.c )
endif ( )

set(LIBTIFF_PUBLIC_HEADERS
tiff.h
tiffio.h
tiffio.hxx
${CMAKE_CURRENT_BINARY_DIR}/tif_config.h
)

if ( NOT WIN32 )
  find_library ( M_LIBRARY NAMES m PATHS /usr/lib /usr/local/lib )
  if ( NOT M_LIBRARY )
    message ( STATUS "math library 'libm' not found - floating point support disabled" )
  endif ( )
else ( )
  set ( M_LIBRARY )
endif ( )

if ( WIN32 )
  add_definitions ( -DUSE_WIN32_FILEIO )
endif ( )

if(BUILD_SHARED)
	add_library(tiff SHARED ${LIBTIFF_SRC} ${LIBTIFF_PUBLIC_HEADERS})
else()
	add_library(tiff STATIC ${LIBTIFF_SRC} ${LIBTIFF_PUBLIC_HEADERS})
endif()
target_include_directories(tiff
    PRIVATE zlib
    PRIVATE jpeg
)
target_link_libraries(tiff LINK_PRIVATE zlib jpeg)
set_property(TARGET tiff PROPERTY FOLDER "Dependencies/")

set(tiff               tiff              PARENT_SCOPE)
set(TIFF_LIB_SRC       ${LIBTIFF_SOURCE_DIR}   PARENT_SCOPE)







