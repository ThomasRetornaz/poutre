#
# Define compil/link flags
#

cmake_minimum_required (VERSION 3.5)
# include(AddCXXCompilerFlag)
# include(CheckCXXCompilerFlag)
message(STATUS "host processor " ${CMAKE_HOST_SYSTEM_PROCESSOR} " target processor " ${CMAKE_SYSTEM_PROCESSOR})

# C version setting
if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
  set(CMAKE_C_STANDARD 11) # no C99 support: https://learn.microsoft.com/en-us/cpp/build/reference/std-specify-language-standard-version
else()
  set(CMAKE_C_STANDARD 99)
endif()

# C++ version setting
cxx_20()

#############################
# Generic Compiler Settings #
#############################

# Reset compiler settings which may come from embedding projects
foreach (flag_var
    CMAKE_C_FLAGS CMAKE_C_FLAGS_DEBUG CMAKE_C_FLAGS_RELEASE
    CMAKE_C_FLAGS_MINSIZEREL CMAKE_C_FLAGS_RELWITHDEBINFO
    CMAKE_CXX_FLAGS CMAKE_CXX_FLAGS_DEBUG CMAKE_CXX_FLAGS_RELEASE
    CMAKE_CXX_FLAGS_MINSIZEREL CMAKE_CXX_FLAGS_RELWITHDEBINFO)
  unset(${flag_var})
endforeach()

#############################
# MSVC Compiler Settings    #
#############################
if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")

  if(MSVC_VERSION LESS 1910)
    message(FATAL_ERROR "Visual Studio 2019 (aka Visual Studio 16) or later is required.")
  endif()

  # FIXME
  # add_compile_options(
  #      $<$<CONFIG:>:/MT> #---------|
  #      $<$<CONFIG:Debug>:/MTd> #---|-- Statically link the runtime libraries
  #      $<$<CONFIG:Release>:/MT> #--|
  #)

  # WIN32 option setting
  remove_definitions(-D_WIN32_WINNT=0x4000)

  # Reactivate usefull warnings that are deactivated by default
  add_definitions(/w14545) # (lvl 1) expression before comma evaluates to a function which is missing an argument list
  add_definitions(/w14546) # (lvl 1) function call before comma missing argument list
  add_definitions(/w14547) # (lvl 1) 'operator' : operator before comma has no effect; expected operator with side-effect
  add_definitions(/w14548) # (lvl 1) expression before comma has no effect; expected expression with side-effect
  add_definitions(/w14549) # (lvl 1) 'operator' : operator before comma has no effect; did you intend 'operator'?
  add_definitions(/w14555) # (lvl 1) expression has no effect; expected expression with side-effect
  add_definitions(/w14905) # (lvl 1) wide string literal cast to 'LPSTR'
  add_definitions(/w14906) # (lvl 1) string literal cast to 'LPWSTR'
  add_definitions(/w14928) # (lvl 1) illegal copy-initialization; more than one user-defined conversion has been implicitly applied
  add_definitions(/w34265) # (lvl 3) 'classname': class has virtual functions, but destructor is not virtual\n instances of this class may not be destructed correctly
  add_definitions(/w34287) # (lvl 3) 'operator': unsigned/negative constant mismatch
  add_definitions(/w34619) # (lvl 3) #pragma warning : there is no warning number 'number'
  add_definitions(/w44062) # (lvl 4) enumerator 'identifier' in switch of enum 'enumeration' is not handled
  add_definitions(/w44242) # (lvl 4) 'identifier': conversion from 'type1' to 'type2', possible loss of data
  add_definitions(/w44254) # (lvl 4) 'operator': conversion from 'type1:field_bits' to 'type2:field_bits', possible loss of data
  add_definitions(/w44263) # (lvl 4) 'function': member function does not override any base class virtual member function (useful ?)
  add_definitions(/w44289) # (lvl 4) nonstandard extension used : 'var' : loop control variable declared in the for-loop is used outside the for-loop scope
  add_definitions(/w44296) # (lvl 4) 'operator': expression is always 'boolean_value'
  # FIXME add_definitions(/w44355) # (lvl 4) 'this': used in base member initializer list
  # FIXME GTEST add_definitions(/w44388) # (lvl 4) 'token' : signed/unsigned mismatch
  add_definitions(/w44826) # (lvl 2) Conversion from 'type1' to 'type2' is sign-extended. This may cause unexpected runtime behavior.

  # Remove unwanted warnings
  add_definitions(/wd4010) # (lvl 1) single-line comment contains line-continuation character
  add_definitions(/wd4100) # (lvl 4) 'identifier': unreferenced formal parameter
  add_definitions(/wd4127) # (lvl 4) conditional expression is constant
  add_definitions(/wd4132) # (lvl 4) 'object' : const object should be initialized
  add_definitions(/wd4152) # (lvl 4) non standard extension, function/data ptr conversion in expression
  add_definitions(/wd4201) # (lvl 4) nonstandard extension used : nameless struct/union
  add_definitions(/wd4204) # (lvl 4) nonstandard extension used : non-constant aggregate initializer
  add_definitions(/wd4206) # (lvl 4) nonstandard extension used : translation unit is empty
  add_definitions(/wd4221) # (lvl 4) nonstandard extension used : 'identifier' : cannot be initialized using address of automatic variable
  add_definitions(/wd4458) # (lvl 4) remove declaration hides class member warnings (edk_cpp wrappers)
  add_definitions(/wd4701) # (lvl 4) potentially uninitialized local variable 'name' used
  add_definitions(/wd4703) # (lvl 4) potentially uninitialized local pointer variable '%s' used
  add_definitions(/wd4706) # (lvl 4) assignment within conditional expression
  add_definitions(/wd4459) # declaration of ? hides global declaration
  add_definitions(/wd4456) # declaration of ? hides previous local declaration
  add_definitions(/wd4457) # declaration of ? hides function parameter
  add_definitions(/wd4250) # 'class1' : inherits 'class2::member' via dominance  polymorphism generate 10^50 warning ....
  
  # FIXME
  add_definitions(/wd4251) # DLL linkage STL
  add_definitions(/wd4355) # 'this': used in base member initializer list


  add_definitions(/WX) #warn as errors



 # Define some warnings as errors
  add_definitions(/we4002) # (lvl 1) too many actual parameters for macro 'identifier'
  add_definitions(/we4003) # (lvl 1) not enough actual parameters for macro 'identifier'
  add_definitions(/we4013) # (lvl 1) 'function' undefined; assuming extern returning int
  add_definitions(/we4265) # (lvl 3) 'classname': class has virtual functions, but destructor is not virtual\n instances of this class may not be destructed correctly
  add_definitions(/we4289) # (lvl 4) nonstandard extension used : 'var' : loop control variable declared in the for-loop is used outside the for-loop scope
  add_definitions(/we4545) # (lvl 1) expression before comma evaluates to a function which is missing an argument list
  add_definitions(/we4546) # (lvl 1) function call before comma missing argument list
  add_definitions(/we4547) # (lvl 1) 'operator': operator before comma has no effect; expected operator with side-effect
  add_definitions(/we4548) # (lvl 1) expression before comma has no effect; expected expression with side-effect
  add_definitions(/we4549) # (lvl 1) 'operator': operator before comma has no effect; did you intend 'operator'?
  add_definitions(/we4555) # (lvl 1) expression has no effect; expected expression with side-effect
  add_definitions(/we4619) # (lvl 3) #pragma warning: there is no warning number 'number'
  add_definitions(/we4905) # (lvl 1) wide string literal cast to 'LPSTR'
  add_definitions(/we4906) # (lvl 1) string literal cast to 'LPWSTR'
  add_definitions(/we4928) # (lvl 1) llegal copy-initialization; more than one user-defined conversion has been implicitly applied  if (WINRT_SUPPORT)

  # Force warning to level 4
  # There's no API to set warning level using a standard cmake function,
  # so replacing existing compiler flags is the correct way to do this. In this
  # case we are replacing the existing default warning level '/W3' with '/W4'
  foreach (flag_var
           CMAKE_C_FLAGS CMAKE_C_FLAGS_DEBUG CMAKE_C_FLAGS_RELEASE
           CMAKE_C_FLAGS_MINSIZEREL CMAKE_C_FLAGS_RELWITHDEBINFO
           CMAKE_CXX_FLAGS CMAKE_CXX_FLAGS_DEBUG CMAKE_CXX_FLAGS_RELEASE
           CMAKE_CXX_FLAGS_MINSIZEREL CMAKE_CXX_FLAGS_RELWITHDEBINFO)
    if (${flag_var} MATCHES "/W[0-9]")
      string(REGEX REPLACE "/W[0-9]" "/W4" ${flag_var} "${${flag_var}}")
    else()
      set(${flag_var} "${${flag_var}} /W4")
    endif()
  endforeach()

    # Setup 64bit and 64bit windows systems
  if(CMAKE_CL_64)
    add_definitions("-D_WIN64")
    message(STATUS "- MSVC: 64-bit platform, enforced -D_WIN64 parameter")

    #Enable extended object support for debug compiles on X64 (not required on X86)
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /bigobj")
    message(STATUS "- MSVC: Enabled extended object-support for debug-compiles")

  endif()

    # Linker
  #https://msdn.microsoft.com/fr-fr/library/bxwfs976.aspx
  # /OPT:REF enables also /OPT:ICF and disables INCREMENTAL
  set(CMAKE_SHARED_LINKER_FLAGS_RELEASE "${CMAKE_SHARED_LINKER_FLAGS_RELEASE} /LTCG /OPT:REF")
  # /OPT:NOICF is recommended when /DEBUG is used (http://msdn.microsoft.com/en-us/library/xe4t6fc1.aspx)
  set(CMAKE_SHARED_LINKER_FLAGS_RELWITHDEBINFO "${CMAKE_SHARED_LINKER_FLAGS_RELEASE} /DEBUG /OPT:NOICF")
  set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} /LTCG /OPT:REF")
  set(CMAKE_EXE_LINKER_FLAGS_RELWITHDEBINFO "${CMAKE_EXE_LINKER_FLAGS_RELEASE} /DEBUG /OPT:NOICF")

  # remove link warnings
  set(ADDITIONAL_LINK_FLAGS )
  # ltcg, incremental, optims
  set(CMAKE_SHARED_LINKER_FLAGS_RELEASE "${CMAKE_SHARED_LINKER_FLAGS_RELEASE} ${ADDITIONAL_LINK_FLAGS} /OPT:REF /OPT:ICF /INCREMENTAL:NO /LTCG")
  set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} ${ADDITIONAL_LINK_FLAGS} /OPT:REF /OPT:ICF /INCREMENTAL:NO /LTCG:incremental")
  set(CMAKE_STATIC_LINKER_FLAGS_RELEASE "${CMAKE_STATIC_LINKER_FLAGS_RELEASE} ${ADDITIONAL_LINK_FLAGS} /LTCG")

  # multi processor build
  #This option is to enable the /MP switch for Visual Studio 2005 and above compilers
  OPTION(WIN32_USE_MP "Set to ON to build with the /MP option (Visual Studio 2005 and above)." ON)
  MARK_AS_ADVANCED(WIN32_USE_MP)
  if(WIN32_USE_MP)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP")
    message(STATUS "Set /MP on for msvc")
  endif()

  # utf-8 flag is requested to enable litteral string support utf8
  # enabled by default by iink_sdk embeddig CMake, has to be disabled here because of ZoningFeatExtractor_impl1.h
  remove_definitions(/utf-8)

  add_definitions(-D_CRT_SECURE_NO_WARNINGS)
  add_definitions(-D_SCL_SECURE_NO_WARNINGS)
  add_definitions(-D_CRT_SECURE_NO_DEPRECATE)
  add_definitions(-D_CRT_NONSTDC_NO_DEPRECATE)
  # required by gtest on windows (at least with visual)
  add_definitions(-D_VARIADIC_MAX=10)

  set(CMAKE_CXX_FLAGS ${CMKAE_CXX_FLAGS} "/EHsc") # Following recommendations from https://msdn.microsoft.com/fr-fr/library/1deeycx5.aspx

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
  set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /RTC1") #/rtc1 full runtime check
  set(CMAKE_C_FLAGS_DEBUG   "${CMAKE_C_FLAGS_DEBUG} /RTC1")

  # Enable extended object support for debug compiles on X64 (not required on X86)
  set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /bigobj")
  # common flags
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Zi /diagnostics:caret /Gy /Oi")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /Zi /diagnostics:caret /Gy /Oi")

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
 
  if(CMAKE_SYSTEM_PROCESSOR MATCHES "amd64.*|x86_64.*|AMD64.*|AMD64")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /arch:AVX2")
  elseif(CMAKE_SYSTEM_PROCESSOR MATCHES "i686.*|i386.*|x86.*")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /arch:SSE2")
  else()
    #todo ? AVX512
  endif()
endif() # MSVC flags


#############################
# clang Compiler Settings   #
#############################

if (CMAKE_CXX_COMPILER_ID MATCHES "Clang") # for Clang and AppleClang

  set(ADDITIONAL_CLANG_FLAGS "${ADDITIONAL_CLANG_FLAGS} -Wall")
  set(ADDITIONAL_CLANG_FLAGS "${ADDITIONAL_CLANG_FLAGS} -Wextra")
  set(ADDITIONAL_CLANG_FLAGS "${ADDITIONAL_CLANG_FLAGS} -Werror")
  set(ADDITIONAL_CLANG_FLAGS "${ADDITIONAL_CLANG_FLAGS} -Wno-unused-parameter")
  set(ADDITIONAL_CLANG_FLAGS "${ADDITIONAL_CLANG_FLAGS} -Wno-missing-braces")
  set(ADDITIONAL_CLANG_FLAGS "${ADDITIONAL_CLANG_FLAGS} -Wfloat-conversion")
  set(ADDITIONAL_CLANG_FLAGS "${ADDITIONAL_CLANG_FLAGS} -Wno-missing-field-initializers")
  set(ADDITIONAL_CLANG_FLAGS "${ADDITIONAL_CLANG_FLAGS} -Wno-unused-function")
  set(ADDITIONAL_CLANG_FLAGS "${ADDITIONAL_CLANG_FLAGS} -Wno-missing-braces")
  set(ADDITIONAL_CLANG_FLAGS "${ADDITIONAL_CLANG_FLAGS} -Wno-deprecated-declarations")

  set(ADDITIONAL_CLANG_FLAGS "${ADDITIONAL_CLANG_FLAGS} -Wno-float-conversion")

  if(CMAKE_SYSTEM_PROCESSOR MATCHES "amd64.*|x86_64.*|AMD64.*|AMD64")
    set(ADDITIONAL_CLANG_FLAGS "${ADDITIONAL_CLANG_FLAGS} -mavx2")
  elseif(CMAKE_SYSTEM_PROCESSOR MATCHES "i686.*|i386.*|x86.*")
    set(ADDITIONAL_CLANG_FLAGS "${ADDITIONAL_CLANG_FLAGS} -msse2")
  else()
    #todo ? AVX512/NEON/SVE
  endif()

  set(ADDITIONAL_CLANG_FLAGS "${ADDITIONAL_CLANG_FLAGS} -fdiagnostics-color=always")

  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${ADDITIONAL_CLANG_FLAGS}")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${ADDITIONAL_CLANG_FLAGS}")

 # specific debug/release optimizations flags
 set(CMAKE_CXX_FLAGS_RELEASE   "${CMAKE_CXX_FLAGS_RELEASE} -O3 -g -DNDEBUG")
 set(CMAKE_C_FLAGS_RELEASE     "${CMAKE_C_FLAGS_RELEASE} -O3 -g -DNDEBUG")
 set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0 -g3")
 set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -O0 -g3")


endif() # Clang flags

#############################
# gcc Compiler Settings     #
#############################
if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")

  set(ADDITIONAL_GCC_FLAGS "${ADDITIONAL_GCC_FLAGS} -Wall")
  set(ADDITIONAL_GCC_FLAGS "${ADDITIONAL_GCC_FLAGS} -Wdeprecated-declarations")
  set(ADDITIONAL_GCC_FLAGS "${ADDITIONAL_GCC_FLAGS} -Wformat")
  set(ADDITIONAL_GCC_FLAGS "${ADDITIONAL_GCC_FLAGS} -Werror")
  set(ADDITIONAL_GCC_FLAGS "${ADDITIONAL_GCC_FLAGS} -Wno-unused-local-typedefs")
  set(ADDITIONAL_GCC_FLAGS "${ADDITIONAL_GCC_FLAGS} -Wno-missing-braces")
  set(ADDITIONAL_GCC_FLAGS "${ADDITIONAL_GCC_FLAGS} -Wfloat-conversion")
  set(ADDITIONAL_GCC_FLAGS "${ADDITIONAL_GCC_FLAGS} -fno-common")
  set(ADDITIONAL_GCC_FLAGS "${ADDITIONAL_GCC_FLAGS} -fno-strict-aliasing")
  set(ADDITIONAL_GCC_FLAGS "${ADDITIONAL_GCC_FLAGS} -fno-strict-overflow")
  set(ADDITIONAL_GCC_FLAGS "${ADDITIONAL_GCC_FLAGS} -fno-omit-frame-pointer")
  set(ADDITIONAL_GCC_FLAGS "${ADDITIONAL_GCC_FLAGS} -fdiagnostics-show-option")

  if(CMAKE_SYSTEM_PROCESSOR MATCHES "amd64.*|x86_64.*|AMD64.*|AMD64")
    set(ADDITIONAL_CLANG_FLAGS "${ADDITIONAL_CLANG_FLAGS} -mavx2")
  elseif(CMAKE_SYSTEM_PROCESSOR MATCHES "i686.*|i386.*|x86.*")
    set(ADDITIONAL_CLANG_FLAGS "${ADDITIONAL_CLANG_FLAGS} -msse2")
  else()
    #todo ? AVX512/NEON/SVE
  endif()

  if (CMAKE_CXX_COMPILER_VERSION VERSION_GREATER 4.9.2)
    # Not working under Windows Linux Subsystem shipped with gcc 4.9.2
    set(ADDITIONAL_GCC_FLAGS "${ADDITIONAL_GCC_FLAGS} -fdiagnostics-color=always")
  endif()

  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${ADDITIONAL_GCC_FLAGS}")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${ADDITIONAL_GCC_FLAGS}")

  # Flags specific for C language
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Winit-self -Wsign-compare -Wmissing-prototypes")

  # Flags specific for C++ language
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-subobject-linkage")

  # specific debug/release optimizations flags
  set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -g -DNDEBUG")
  set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -O0 -g3")
  set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -g -DNDEBUG")
  set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0 -g3")

endif() # GCC flags


#############################
# XCODE specific Settings   #
#############################
if (CMAKE_GENERATOR STREQUAL "Xcode")
  if(IOS)
    # ---common
    set(CMAKE_XCODE_ATTRIBUTE_DERIVE_MACCATALYST_PRODUCT_BUNDLE_IDENTIFIER "YES")
    set(CMAKE_XCODE_ATTRIBUTE_SUPPORTS_MACCATALYST "NO")
    set(CMAKE_XCODE_ATTRIBUTE_SKIP_INSTALL "NO")
    set(CMAKE_XCODE_ATTRIBUTE_DEPLOYMENT_LOCATION "YES")
    set(CMAKE_XCODE_ATTRIBUTE_DEBUG_INFORMATION_FORMAT "dwarf-with-dsym")
    set(CMAKE_XCODE_ATTRIBUTE_CLANG_X86_VECTOR_INSTRUCTIONS "AVX2")
    # ---release
    # debugging
    set(CMAKE_XCODE_ATTRIBUTE_GCC_GENERATE_DEBUGGING_SYMBOLS[variant=Release] "YES")
    # preprocessor
    set(CMAKE_XCODE_ATTRIBUTE_GCC_PREPROCESSOR_DEFINITIONS[variant=Release] "NDEBUG")
    # optimizations
    set(CMAKE_XCODE_ATTRIBUTE_GCC_OPTIMIZATION_LEVEL[variant=Release] "3")
    set(CMAKE_XCODE_ATTRIBUTE_GCC_FAST_MATH[variant=Release] "NO")
    set(CMAKE_XCODE_ATTRIBUTE_GCC_UNROLL_LOOPS[variant=Release] "YES")
    set(CMAKE_XCODE_ATTRIBUTE_LLVM_LTO[variant=Release] "NO")
    # deployment
    set(CMAKE_XCODE_ATTRIBUTE_STRIP_INSTALLED_PRODUCT[variant=Release] "YES")
    set(CMAKE_XCODE_ATTRIBUTE_STRIP_STYLE[variant=Release] "debugging")
    set(CMAKE_XCODE_ATTRIBUTE_DEPLOYMENT_POSTPROCESSING[variant=Release] "YES")
    # --- debug
    set(CMAKE_XCODE_ATTRIBUTE_ONLY_ACTIVE_ARCH[variant=Debug] "YES")
    # debugging
    set(CMAKE_XCODE_ATTRIBUTE_GCC_GENERATE_DEBUGGING_SYMBOLS[variant=Debug] "YES")
    # optimizations
    set(CMAKE_XCODE_ATTRIBUTE_GCC_OPTIMIZATION_LEVEL[variant=Debug] "0")
    set(CMAKE_XCODE_ATTRIBUTE_GCC_FAST_MATH[variant=Debug] "NO")
  else()
    # ---common
    # compiler
    set(CMAKE_XCODE_ATTRIBUTE_GCC_VERSION "") # no value means default compiler
    set(CMAKE_XCODE_ATTRIBUTE_CLANG_ENABLE_OBJC_WEAK "YES")
    set(CMAKE_XCODE_ATTRIBUTE_GCC_C_LANGUAGE_STANDARD "c99")
    set(CMAKE_XCODE_ATTRIBUTE_GCC_ENABLE_OBJC_EXCEPTIONS "NO")
    set(CMAKE_XCODE_ATTRIBUTE_GCC_ENABLE_PASCAL_STRINGS "NO")
    set(CMAKE_XCODE_ATTRIBUTE_GCC_INLINES_ARE_PRIVATE_EXTERN "YES")
    set(CMAKE_XCODE_ATTRIBUTE_GCC_NO_COMMON_BLOCKS "YES")
    set(CMAKE_XCODE_ATTRIBUTE_GCC_SYMBOLS_PRIVATE_EXTERN "NO")
    set(CMAKE_XCODE_ATTRIBUTE_GCC_THREADSAFE_STATICS "NO")
    set(CMAKE_XCODE_ATTRIBUTE_ENABLE_STRICT_OBJC_MSGSEND "YES")
    set(CMAKE_XCODE_ATTRIBUTE_ENABLE_TESTABILITY "YES")
    # c++ standard and stl
    set(CMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LANGUAGE_STANDARD "c++11")
    set(CMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LIBRARY "libc++")
    # warnings
    set(CMAKE_XCODE_ATTRIBUTE_CLANG_WARN_ASSIGN_ENUM "YES")
    set(CMAKE_XCODE_ATTRIBUTE_CLANG_WARN_BLOCK_CAPTURE_AUTORELEASING "YES")
    set(CMAKE_XCODE_ATTRIBUTE_CLANG_WARN_BOOL_CONVERSION "YES")
    set(CMAKE_XCODE_ATTRIBUTE_CLANG_WARN_COMMA "YES")
    set(CMAKE_XCODE_ATTRIBUTE_CLANG_WARN_CONSTANT_CONVERSION "YES")
    set(CMAKE_XCODE_ATTRIBUTE_CLANG_WARN_DEPRECATED_OBJC_IMPLEMENTATIONS"YES")
    set(CMAKE_XCODE_ATTRIBUTE_CLANG_WARN_EMPTY_BODY "YES")
    set(CMAKE_XCODE_ATTRIBUTE_CLANG_WARN_ENUM_CONVERSION "YES")
    set(CMAKE_XCODE_ATTRIBUTE_CLANG_WARN_IMPLICIT_SIGN_CONVERSION "NO")
    set(CMAKE_XCODE_ATTRIBUTE_CLANG_WARN_INFINITE_RECURSION "YES")
    set(CMAKE_XCODE_ATTRIBUTE_CLANG_WARN_INT_CONVERSION "YES")
    set(CMAKE_XCODE_ATTRIBUTE_CLANG_WARN_OBJC_IMPLICIT_RETAIN_SELF "YES")
    set(CMAKE_XCODE_ATTRIBUTE_CLANG_WARN_QUOTED_INCLUDE_IN_FRAMEWORK_HEADER "YES")
    set(CMAKE_XCODE_ATTRIBUTE_CLANG_WARN_RANGE_LOOP_ANALYSIS "YES")
    set(CMAKE_XCODE_ATTRIBUTE_CLANG_WARN_STRICT_PROTOTYPES "YES")
    set(CMAKE_XCODE_ATTRIBUTE_CLANG_WARN_SUSPICIOUS_IMPLICIT_CONVERSION "YES")
    set(CMAKE_XCODE_ATTRIBUTE_CLANG_WARN_SUSPICIOUS_MOVE "YES")
    set(CMAKE_XCODE_ATTRIBUTE_CLANG_WARN_UNREACHABLE_CODE "YES")
    set(CMAKE_XCODE_ATTRIBUTE_CLANG_WARN__DUPLICATE_METHOD_MATCH "YES")
    set(CMAKE_XCODE_ATTRIBUTE_GCC_TREAT_INCOMPATIBLE_POINTER_TYPE_WARNINGS_AS_ERRORS "NO")
    set(CMAKE_XCODE_ATTRIBUTE_GCC_TREAT_WARNINGS_AS_ERRORS "YES")
    set(CMAKE_XCODE_ATTRIBUTE_GCC_WARN_64_TO_32_BIT_CONVERSION "YES")
    set(CMAKE_XCODE_ATTRIBUTE_GCC_WARN_ABOUT_DEPRECATED_FUNCTIONS "YES")
    set(CMAKE_XCODE_ATTRIBUTE_GCC_WARN_ABOUT_INVALID_OFFSETOF_MACRO "YES")
    set(CMAKE_XCODE_ATTRIBUTE_GCC_WARN_ABOUT_MISSING_NEWLINE "YES")
    set(CMAKE_XCODE_ATTRIBUTE_GCC_WARN_ABOUT_MISSING_PROTOTYPES "YES")
    set(CMAKE_XCODE_ATTRIBUTE_GCC_WARN_ABOUT_POINTER_SIGNEDNESS "YES")
    set(CMAKE_XCODE_ATTRIBUTE_GCC_WARN_ABOUT_RETURN_TYPE "YES")
    set(CMAKE_XCODE_ATTRIBUTE_GCC_WARN_CHECK_SWITCH_STATEMENTS "YES")
    set(CMAKE_XCODE_ATTRIBUTE_GCC_WARN_HIDDEN_VIRTUAL_FUNCTIONS "YES")
    set(CMAKE_XCODE_ATTRIBUTE_GCC_WARN_INITIALIZER_NOT_FULLY_BRACKETED "NO")
    set(CMAKE_XCODE_ATTRIBUTE_GCC_WARN_MISSING_PARENTHESES "YES")
    set(CMAKE_XCODE_ATTRIBUTE_GCC_WARN_NON_VIRTUAL_DESTRUCTOR "YES")
    set(CMAKE_XCODE_ATTRIBUTE_GCC_WARN_SIGN_COMPARE "YES")
    set(CMAKE_XCODE_ATTRIBUTE_GCC_WARN_TYPECHECK_CALLS_TO_PRINTF "YES")
    set(CMAKE_XCODE_ATTRIBUTE_GCC_WARN_UNDECLARED_SELECTOR "YES")
    set(CMAKE_XCODE_ATTRIBUTE_GCC_WARN_UNINITIALIZED_AUTOS "YES")
    set(CMAKE_XCODE_ATTRIBUTE_GCC_WARN_UNKNOWN_PRAGMAS "YES")
    set(CMAKE_XCODE_ATTRIBUTE_GCC_WARN_UNUSED_FUNCTION "YES")
    set(CMAKE_XCODE_ATTRIBUTE_GCC_WARN_UNUSED_LABEL "YES")
    set(CMAKE_XCODE_ATTRIBUTE_GCC_WARN_UNUSED_PARAMETER "NO")
    set(CMAKE_XCODE_ATTRIBUTE_GCC_WARN_UNUSED_VALUE "YES")
    set(CMAKE_XCODE_ATTRIBUTE_GCC_WARN_UNUSED_VARIABLE "YES")
    # link
    set(CMAKE_XCODE_ATTRIBUTE_DEAD_CODE_STRIPPING "YES")
    set(CMAKE_XCODE_ATTRIBUTE_LINK_WITH_STANDARD_LIBRARIES "YES")
    # deployment
    set(CMAKE_XCODE_ATTRIBUTE_SKIP_INSTALL "NO")
    set(CMAKE_XCODE_ATTRIBUTE_DEPLOYMENT_LOCATION "YES")

    # ---release
    set(CMAKE_XCODE_ATTRIBUTE_GCC_GENERATE_DEBUGGING_SYMBOLS[variant=Release] "YES")
    set(CMAKE_XCODE_ATTRIBUTE_DEBUG_INFORMATION_FORMAT[variant=Release] "dwarf-with-dsym")
    # preprocessor
    set(CMAKE_XCODE_ATTRIBUTE_GCC_PREPROCESSOR_DEFINITIONS[variant=Release] "NDEBUG")
    # optimizations
    set(CMAKE_XCODE_ATTRIBUTE_CLANG_X86_VECTOR_INSTRUCTIONS[variant=Release] "ssse3")
    set(CMAKE_XCODE_ATTRIBUTE_GCC_OPTIMIZATION_LEVEL[variant=Release] "3")
    set(CMAKE_XCODE_ATTRIBUTE_GCC_FAST_MATH[variant=Release] "NO")
    set(CMAKE_XCODE_ATTRIBUTE_GCC_UNROLL_LOOPS[variant=Release] "YES")
    set(CMAKE_XCODE_ATTRIBUTE_LLVM_LTO[variant=Release] "NO")
    # deployment
    set(CMAKE_XCODE_ATTRIBUTE_STRIP_INSTALLED_PRODUCT[variant=Release] "YES")
    set(CMAKE_XCODE_ATTRIBUTE_STRIP_STYLE[variant=Release] "non-global")
    set(CMAKE_XCODE_ATTRIBUTE_DEPLOYMENT_POSTPROCESSING[variant=Release] "YES")

    # ---debug
    set(CMAKE_XCODE_ATTRIBUTE_ONLY_ACTIVE_ARCH[variant=Debug] "YES")
    # debugging
    set(CMAKE_XCODE_ATTRIBUTE_GCC_GENERATE_DEBUGGING_SYMBOLS "YES")
    set(CMAKE_XCODE_ATTRIBUTE_DEBUG_INFORMATION_FORMAT[variant=Debug] "dwarf-with-dsym")
    # optimizations
    set(CMAKE_XCODE_ATTRIBUTE_CLANG_X86_VECTOR_INSTRUCTIONS[variant=Debug] "AVX2")
    set(CMAKE_XCODE_ATTRIBUTE_GCC_OPTIMIZATION_LEVEL[variant=Debug] "0")
    set(CMAKE_XCODE_ATTRIBUTE_GCC_FAST_MATH[variant=Debug] "NO")
  endif()
endif()

IF(NOT APPLE AND NOT MSVC)
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
ENDIF()

#openMP for all
OPTION (USE_OpenMP "Use OpenMP" ON)
IF(USE_OpenMP)
  FIND_PACKAGE(OpenMP REQUIRED)
  IF(OPENMP_FOUND)
    SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
  ENDIF()
ENDIF()