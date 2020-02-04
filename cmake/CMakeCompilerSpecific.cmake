cmake_minimum_required (VERSION 3.0)
include(AddCXXCompilerFlag)
include(CheckCXXCompilerFlag)
message(STATUS "host processor " ${CMAKE_HOST_SYSTEM_PROCESSOR} " target processor " ${CMAKE_SYSTEM_PROCESSOR})



###### MSVC ######
if(MSVC)
  if(MSVC_VERSION LESS 1800)
    message(FATAL_ERROR "Visual Studio 2013 (aka Visual Studio 12) or later is required.")
  endif()
  
  message("Flags for MSVC")

  # supress these annoying message
  add_definitions(-D_SCL_SECURE_NO_WARNINGS)
  add_definitions(-D_CRT_SECURE_NO_DEPRECATE)
  add_definitions(-D_CRT_NONSTDC_NO_DEPRECATE)

  #TR i don't find a way to reject generation of project if ${MSVC_VERSION}"<1500
  #message(STATUS "MSVC_VERSION ${MSVC_VERSION}")
  #Visual C++, 32-bit, version 6.0         1200
  #Visual C++, 32-bit, version .net 2002   1300
  #Visual C++, 32-bit, version .net 2003   1310
  #Visual C++, 32-bit, version 2005        1400 (vc80)
  #Visual C++, 32-bit, version 2005 SP1    14?? (vc80_sp1)
  #Visual C++, 32-bit, version 2008        1500 (vc90) 
  
  # Setup 64bit and 64bit windows systems
  if(CMAKE_CL_64)
    add_definitions("-D_WIN64")
    message(STATUS "- MSVC: 64-bit platform, enforced -D_WIN64 parameter")

    #Enable extended object support for debug compiles on X64 (not required on X86)
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /bigobj")
    message(STATUS "- MSVC: Enabled extended object-support for debug-compiles")

  endif()

  #64 bits only ?
  #add_definitions(/bigobj)	
  
  set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /RTC1") #/rtc1 full runtime check 
  set(CMAKE_C_FLAGS_DEBUG   "${CMAKE_C_FLAGS_DEBUG} /RTC1")

  #/Ox full optim 
  #/Ob (Inline Function Expansion), where the option parameter is 2 (/Ob2)
  #and  /Og (Global Optimizations)
  #and  /Oi (Generate Intrinsic Functions)
  #and /Ot (Favor Fast Code)
  #AND /Oy (Frame-Pointer Omission) TODO check on x64
  #/GF pools strings as read-only.
  #/GL global optim https://msdn.microsoft.com/fr-fr/library/vstudio/0zza0de8(v=vs.120).aspx
  set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /Ox /Oi /Ob2 /Ot /Oy /GF /GL /D_SCL_SECURE_NO_WARNINGS /D\"_SECURE_SCL=0\" /D\"_HAS_ITERATOR_DEBUGGING=0\"") #remove all kind off safe_stl in release mode, default in "modern" visual compiler
  set(CMAKE_C_FLAGS_RELEASE   "${CMAKE_C_FLAGS_RELEASE} /Ox /Oi /Ob2 /Oy /GF /GL")

  #set(value_tmp ${CMAKE_CXX_FLAGS_RELEASE})
  #unset(CMAKE_CXX_FLAGS_RELEASE CACHE)
  #set(CMAKE_CXX_FLAGS_RELEASE ${CMAKE_CXX_FLAGS_RELEASE} CACHE STRING "Release flags (overwritten) for CPP files")
  #set(CMAKE_C_FLAGS_RELEASE CACHE STRING "Release flags (overwritten) for C files")
  #message("Flags for MSVC ${CMAKE_CXX_FLAGS_RELEASE}")
  
  # Linker
  #https://msdn.microsoft.com/fr-fr/library/bxwfs976.aspx
  # /OPT:REF enables also /OPT:ICF and disables INCREMENTAL
  set(CMAKE_SHARED_LINKER_FLAGS_RELEASE "${CMAKE_SHARED_LINKER_FLAGS_RELEASE} /LTCG /OPT:REF")
  # /OPT:NOICF is recommended when /DEBUG is used (http://msdn.microsoft.com/en-us/library/xe4t6fc1.aspx)
  set(CMAKE_SHARED_LINKER_FLAGS_RELWITHDEBINFO "${CMAKE_SHARED_LINKER_FLAGS_RELEASE} /DEBUG /OPT:NOICF")
  set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} /LTCG /OPT:REF")
  set(CMAKE_EXE_LINKER_FLAGS_RELWITHDEBINFO "${CMAKE_EXE_LINKER_FLAGS_RELEASE} /DEBUG /OPT:NOICF")
   
  #unset(CMAKE_SHARED_LINKER_FLAGS_RELEASE CACHE)
  #set(CMAKE_SHARED_LINKER_FLAGS_RELEASE "${value_tmp}" CACHE STRING "Release flags (overwritten) for CPP files")

 
  # turn off various warnings
  #4250: 'class1' : inherits 'class2::member' via dominance  polymorphism generate 10^50 warning ....	
  #4702 unreachable code 
  #4715 not all control paths return a value
  foreach(warning 4250 4702)  
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd${warning}")
  endforeach(warning)
 
 
  # This option is to enable the /DYNAMICBASE switch
  # It is used to workaround a bug in Windows 7 when linking in release, which results in corrupt
  # binaries. See this page for details: http://www.wintellect.com/CS/blogs/jrobbins/archive/2009/01/24/the-case-of-the-corrupt-pe-binaries.aspx
  OPTION(WIN32_USE_DYNAMICBASE "Set to ON to build with the /DYNAMICBASE option to work around a bug when linking release executables on Windows 7." ON)
  MARK_AS_ADVANCED(WIN32_USE_DYNAMICBASE)
  if(WIN32_USE_DYNAMICBASE)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /DYNAMICBASE")
  endif()
  
  #more than 3gb
  OPTION(WIN32_LARGEADRESSAWARE "Set to ON to build with large adress aware option (usefull on x64 systems)" ON)
  if(WIN32_LARGEADRESSAWARE)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /LARGEADDRESSAWARE")
    set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} /LARGEADDRESSAWARE")
  endif()
 
  
  
  #This option is to enable the /MP switch for Visual Studio 2005 and above compilers
  OPTION(WIN32_USE_MP "Set to ON to build with the /MP option (Visual Studio 2005 and above)." ON)
  MARK_AS_ADVANCED(WIN32_USE_MP)
  if(WIN32_USE_MP)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP")
    message(STATUS "Set /MP on for msvc")
  endif()


  
 
  # Boost specific
  remove_definitions(-DBOOST_TEST_DYN_LINK)
  add_definitions(-DBOOST_ALL_NO_LIB)
  #add_definitions(-DBOOST_TEST_NO_LIB)
endif(MSVC)

# Require C++14
cxx_14()

if((CMAKE_CXX_COMPILER_ID MATCHES "Clang") OR (CMAKE_CXX_COMPILER_ID MATCHES "GNU"))
  message("Flags for GNUCXX And clang")
  # some problems with the amd 64bits and gcc
  if("${CMAKE_SYSTEM_PROCESSOR}" STREQUAL "x86_64")
    message("Configuring with the fPIC option")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -fPIC")
    set(CMAKE_CXX_FLAGS_DEBUG   "${CMAKE_CXX_FLAGS_DEBUG} -fPIC"  )
    set(CMAKE_CXX_FLAGS         "${CMAKE_CXX_FLAGS} -fPIC"        )
 
    set(CMAKE_C_FLAGS_RELEASE   "${CMAKE_C_FLAGS_RELEASE} -fPIC"  )
    set(CMAKE_C_FLAGS_DEBUG     "${CMAKE_C_FLAGS_DEBUG} -fPIC"    )
    set(CMAKE_C_FLAGS           "${CMAKE_C_FLAGS} -fPIC"          )
  endif()
  
  set(CMAKE_CXX_FLAGS_RELEASE   "${CMAKE_CXX_FLAGS_RELEASE} -O3"  )
  set(CMAKE_C_FLAGS_RELEASE     "${CMAKE_C_FLAGS_RELEASE} -O3"    )

  CHECK_CXX_COMPILER_FLAG("-std=c++17" COMPILER_SUPPORTS_CXX17)
  CHECK_CXX_COMPILER_FLAG("-std=c++1z" COMPILER_SUPPORTS_CXX1z)
  CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
  CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
  if(COMPILER_SUPPORTS_CXX17) 
     set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
  elseif(COMPILER_SUPPORTS_CXX1z)     
     set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1z")
  elseif(COMPILER_SUPPORTS_CXX14)
     set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
   elseif(COMPILER_SUPPORTS_CXX11)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
  elseif(COMPILER_SUPPORTS_CXX0X)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
  else()
    message(FATAL_ERROR "The compiler ${CMAKE_CXX_COMPILER} has no C++11/c++14 support. Please use a different C++ compiler.")
  endif()

  if(APPLE)
    set(CMAKE_OSX_ARCHITECTURES "i386;x86_64")
  endif()
endif()

IF(NOT APPLE AND NOT MSVC)
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
ENDIF()

#openMP for all
OPTION (USE_OpenMP "Use OpenMP" ON)
IF(USE_OpenMP)
  FIND_PACKAGE(OpenMP)
  IF(OPENMP_FOUND)
    SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
  ENDIF()
ENDIF()

#libsimdpp dispatch 
