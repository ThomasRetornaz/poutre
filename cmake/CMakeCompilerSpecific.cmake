
set(INTERPROCEDURAL_OPTIMIZATION_RELEASE)

message(STATUS "host processor " ${CMAKE_HOST_SYSTEM_PROCESSOR} " target processor " ${CMAKE_SYSTEM_PROCESSOR})
# Allow the developer to enable mmx support
option(USE_SSE    "Enable SEE instructions,target processor must support it" ON)
option(USE_SSE2   "Enable SEE2 instructions,target processor must support it" ON)
#option(USE_SSE3   "Enable SEE3 instructions,target processor must support it" ON)


###### MSVC ######
if(MSVC)

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
    set(PLATFORM_LINKFLAGS "${PLATFORM_LINKFLAGS} /MACHINE:X64")
    # This definition is necessary to work around a bug with Intellisense described
    # here: http://tinyurl.com/2cb428.  Syntax highlighting is important for proper
    # debugger functionality.
    add_definitions("-D_WIN64")
    message(STATUS "- MSVC: 64-bit platform, enforced -D_WIN64 parameter")

    #Enable extended object support for debug compiles on X64 (not required on X86)
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /bigobj")
    message(STATUS "- MSVC: Enabled extended object-support for debug-compiles")

    set(USE_SSE OFF)
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

  #TODO remove some flag not compatible with 64 bits
  set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /Ox /Oi /Ob2 /Ot /Oy /GF /GL /D\"_SECURE_SCL=0\"")
  set(CMAKE_C_FLAGS_RELEASE   "${CMAKE_C_FLAGS_RELEASE} /Ox /Oi /Ob2 /Oy /GF /GL")
  if(USE_SSE2)
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /arch:SSE2")
    set(CMAKE_C_FLAGS_RELEASE   "${CMAKE_C_FLAGS_RELEASE} /arch:SSE2")
  elseif(USE_SSE)
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /arch:SSE")
    set(CMAKE_C_FLAGS_RELEASE   "${CMAKE_C_FLAGS_RELEASE} /arch:SSE")
  endif()
  
  #set(value_tmp ${CMAKE_CXX_FLAGS_RELEASE})
  #unset(CMAKE_CXX_FLAGS_RELEASE CACHE)
  #set(CMAKE_CXX_FLAGS_RELEASE ${CMAKE_CXX_FLAGS_RELEASE} CACHE STRING "Release flags (overwritten) for CPP files")
  #set(CMAKE_C_FLAGS_RELEASE CACHE STRING "Release flags (overwritten) for C files")
  #message("Flags for MSVC ${CMAKE_CXX_FLAGS_RELEASE}")
  
  set(CMAKE_SHARED_LINKER_FLAGS_RELEASE "${CMAKE_SHARED_LINKER_FLAGS_RELEASE} /LTCG")
  #unset(CMAKE_SHARED_LINKER_FLAGS_RELEASE CACHE)
  #set(CMAKE_SHARED_LINKER_FLAGS_RELEASE "${value_tmp}" CACHE STRING "Release flags (overwritten) for CPP files")

 
  # turn off various warnings
  #4250: 'class1' : inherits 'class2::member' via dominance  polymorphism generate 10^50 warning ....	
  #4702 unreachable code 
  #4715 not all control paths return a value
  foreach(warning 4250 4702 4745 ) 
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
  OPTION(WIN32_USE_MP "Set to ON to build with the /MP option (Visual Studio 2005 and above)." OFF)
  MARK_AS_ADVANCED(WIN32_USE_MP)
  if(WIN32_USE_MP)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP")
  endif()
  
 
  # Boost specific
  remove_definitions(-DBOOST_TEST_DYN_LINK)
  add_definitions(-DBOOST_ALL_NO_LIB)
  #add_definitions(-DBOOST_TEST_NO_LIB)
endif(MSVC)



if(CMAKE_COMPILER_IS_GNUCXX)
  message("Flags for GNUCXX")
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
  
  if(USE_MMX)
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -mmmx")
  endif()
  if(USE_SEE)
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -msse")  
  endif()
  if(USE_SEE2)
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -msse2")
  endif()
  if(USE_SEE3)
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -msse3")
  endif()

  set(CMAKE_CXX_FLAGS_RELEASE   "${CMAKE_CXX_FLAGS_RELEASE} -O3"  )
  set(CMAKE_C_FLAGS_RELEASE     "${CMAKE_C_FLAGS_RELEASE} -O3"    )

  if(APPLE)
    set(CMAKE_OSX_ARCHITECTURES "i386;x86_64")
  endif()
endif()
