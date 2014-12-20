# @file
# This file contains python related macros

# @author Thomas Retornaz

set(POUTRE_USE_DEBUG_PYTHON OFF)

# This macro generates the proper extensions for Python add-ons, and generates the corresponding target
# it also adds the python include path
macro(add_python_library name sources dependencies)

  if(NOT DEFINED PYTHON_MODULES_EXTENSIONS)
  
  
    #used to switch python extension on windows dll -> pyd
    execute_process(
      COMMAND ${PYTHON_EXECUTABLE} -c "import sys; v=sys.version_info[:2]; print '%d.%d' % v"
      OUTPUT_VARIABLE PYTHON_VERSION
      OUTPUT_STRIP_TRAILING_WHITESPACE
    )
    string  (REPLACE "." "" PYTHON_VERSION_NO_DOTS ${PYTHON_VERSION})
  
    # something like 
    # python -c "import imp; print [i[0] for i in imp.get_suffixes() if i[2] == imp.C_EXTENSION][0]"
    # might also do
    if((${PYTHON_VERSION} VERSION_GREATER "2.6") OR (${PYTHON_VERSION} VERSION_EQUAL "2.6"))
      if((UNIX) OR (MINGW))
        set(PYTHON_MODULES_EXTENSIONS_TEMP ".so")
      else()
        set(PYTHON_MODULES_EXTENSIONS_TEMP ".pyd")
      endif()
    else()
      if(APPLE)
        set(PYTHON_MODULES_EXTENSIONS_TEMP ".so")
      else()
        set(PYTHON_MODULES_EXTENSIONS_TEMP ${CMAKE_SHARED_LIBRARY_SUFFIX})
      endif()
    endif()

    set(PYTHON_MODULES_EXTENSIONS ${PYTHON_MODULES_EXTENSIONS_TEMP} CACHE STRING "Python modules extensions")
    unset(PYTHON_MODULES_EXTENSIONS_TEMP)
  endif()

  add_library(${name} SHARED ${sources})
  target_include_directories(${name} PRIVATE ${PYTHON_INCLUDE_PATH})
  
  if(WIN32 AND POUTRE_USE_DEBUG_PYTHON)
    # on Win32, the _d should be appended otherwise Python does not load it
    set_target_properties(${name} 
      PROPERTIES 
      OUTPUT_NAME_DEBUG ${name}_d
      )
  endif()
  
  # if the link to the debug version (WIN32) of python is required, ${PYTHON_LIBRARIES} should be used for linking
  # and the python27_d.lib should be detected properly. Also, the BOOST_DEBUG_PYTHON
  # macro should be defined accordingly, otherwise boost disable the python debug library (see eg. 
  # http://www.boost.org/doc/libs/1_56_0/libs/python/doc/building.html)
  if(POUTRE_USE_DEBUG_PYTHON)
    set(POUTRE_PYTHON_LIBS_VAR ${PYTHON_LIBRARIES})
  else()
    set(POUTRE_PYTHON_LIBS_VAR ${PYTHON_LIBRARY})
  endif()
  
  target_link_libraries(${name} "${dependencies}" ${Boost_PYTHON_LIBRARY} ${POUTRE_PYTHON_LIBS_VAR}) 
  set_target_properties(${name} PROPERTIES 
    SUFFIX ${PYTHON_MODULES_EXTENSIONS}
    PREFIX ""
    FOLDER "Python/")
    
  if(APPLE)
    set_target_properties(${name} PROPERTIES 
      INSTALL_NAME_DIR "@loader_path"
    )
  elseif(UNIX)
    set_target_properties(${name}
      PROPERTIES
        INSTALL_RPATH "$ORIGIN/."
        BUILD_WITH_INSTALL_RPATH TRUE)
  endif()
    
  if(WIN32)
    # Visual C++ workaround, the hypot is generating a lot of noise, but it seems that
    # the problem is related to visual C++
    target_compile_definitions(${name}
      PRIVATE "HAVE_ROUND=1" 
      #PRIVATE "HAVE_HYPOT=1"
    )
  endif()
  
  target_compile_definitions(${name} PRIVATE "BOOST_PYTHON_DYNAMIC_LIB")
  
  
  if(POUTRE_USE_DEBUG_PYTHON)
    target_compile_definitions(${name} PRIVATE "BOOST_DEBUG_PYTHON")
  endif()

endmacro(add_python_library)


