
# This cmake will integrate the setup for the python package


set(manifest_config_file      ${POUTRE_PYTHON_PACKAGE_LOCATION}/MANIFEST.in.config)
set(manifest_file             ${POUTRE_PYTHON_PACKAGE_LOCATION}/MANIFEST.in)
set(python_package_binary_dir ${POUTRE_PYTHON_PACKAGE_LOCATION}/POUTRE/bin/)
get_filename_component(python_package_binary_dir ${python_package_binary_dir} ABSOLUTE)

if(NOT EXISTS ${python_package_binary_dir})
  file(MAKE_DIRECTORY ${python_package_binary_dir})
endif()
set(python_package_binary_dir ${python_package_binary_dir} CACHE INTERNAL "python package binary destination path")



# this variable will contain additional include commands generated at setup.py time
set(POUTRE_ADDITIONAL_INCLUDE_CMDS CACHE INTERNAL "manifest additional files")

# this variable will be filled during calls to add_files_to_python_packaging and add_to_python_packaging
set(POUTRE_PYTHON_BINARY_EXTENSIONS "" CACHE INTERNAL "targets to copy to the python packaging")
set(POUTRE_PYTHON_BINARY_EXTENSIONS_ADDITIONAL_FILES "" CACHE INTERNAL "external files to copy to the python packaging")

# ######################################################
#
# Adds the specified target to the python packaging system.
macro(add_to_python_packaging target_name)

  #message(STATUS "Adding ${target_name} to python packaging")
  get_target_property(test_var ${target_name} INTERFACE_LINK_LIBRARIES)

  #list_prerequisites(${target_name} 0 0 1)

  # parsing the dependencies
  foreach(v ${test_var})
    if(TARGET ${v})
      # target known, we copy it
      set(POUTRE_PYTHON_BINARY_EXTENSIONS ${POUTRE_PYTHON_BINARY_EXTENSIONS} ${v} CACHE INTERNAL "source file to copy")
      
      install(
        TARGETS ${v}
        DESTINATION ${python_package_binary_dir}
        COMPONENT python_package_install_intermediate
      )
      
    endif()
    # We should also copy things that are not targets (check for thirdparties), but we do not want to copy system libraries.
    # This is performed in another function which does not check the graph of dependencies
  endforeach()

  # copy the main target
  # @todo better if the files here are explicitely stated (rather than a *.so in the manifest)
  set(POUTRE_PYTHON_BINARY_EXTENSIONS ${POUTRE_PYTHON_BINARY_EXTENSIONS} ${target_name} CACHE INTERNAL "source file to copy")

  install(
    TARGETS ${target_name}
    DESTINATION ${python_package_binary_dir}
    COMPONENT python_package_install_intermediate
  )


endmacro(add_to_python_packaging)


# ######################################################
#
# Add the specified files to the python packaging system
# add_files_to_python_packaging(FILES file1 file2 ...)
macro(add_files_to_python_packaging)

  set(options )
  set(oneValueArgs)
  set(multiValueArgs FILES)
  cmake_parse_arguments(_local_vars "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

  message(STATUS "Adding ${_local_vars_FILES} to python packaging" ${ARGN})
  set(POUTRE_PYTHON_BINARY_EXTENSIONS_ADDITIONAL_FILES ${POUTRE_PYTHON_BINARY_EXTENSIONS_ADDITIONAL_FILES} ${_local_vars_FILES} CACHE INTERNAL "source file to copy")
  foreach(v ${_local_vars_FILES})
    get_filename_component(v_file_name ${v} NAME)
    set(POUTRE_ADDITIONAL_INCLUDE_CMDS ${POUTRE_ADDITIONAL_INCLUDE_CMDS} "include POUTRE/bin/${v_file_name}" CACHE INTERNAL "manifest additional files")
    
    install(
      FILES ${v}
      DESTINATION ${python_package_binary_dir}
      COMPONENT python_package_install_intermediate
    )

    
  endforeach()


endmacro(add_files_to_python_packaging)



# ######################################################
#
# This function gathers all information collected by the build tree to create the appropriate python packaging
macro(create_python_package)
  message(STATUS "[POUTREPython] Configuring Python package manifest ${POUTRE_ADDITIONAL_INCLUDE_CMDS}")

  # should have been cached
  if(NOT DEFINED PYTHON_MODULES_EXTENSIONS)
    message(FATAL_ERROR "Something wrong in the configuration (PYTHON_MODULES_EXTENSIONS not defined)")
  endif()


  set(POUTRE_PYTHON_EXT ${PYTHON_MODULES_EXTENSIONS})
  set(POUTRE_PLATFORM_SO_EXT ${CMAKE_SHARED_LIBRARY_SUFFIX})
  set(_var_concat "")
  foreach(c IN LISTS POUTRE_ADDITIONAL_INCLUDE_CMDS)
    set(_var_concat "${_var_concat}\n${c}")
  endforeach(c)
  set(POUTRE_ADDITIONAL_INCLUDE_CMDS ${_var_concat})
  
  if(EXISTS ${manifest_config_file})
    configure_file(${manifest_config_file} ${manifest_file} @ONLY)
  else()
    message(STATUS "[POUTREPython] not configuring the MANIFEST.in since the template ${manifest_config_file} is not found."
                    "Should be an archive distribution, MANIFEST.in already settled?")
  endif()

  set(POUTRE_python_package_SRC
      ${POUTRE_PYTHON_PACKAGE_LOCATION}/setup.py)
  if(EXISTS ${manifest_config_file})
    set(POUTRE_python_package_SRC
        ${POUTRE_python_package_SRC}
        ${manifest_file}
       )
  endif()
  
  # custom target just for copying the files to the appropriate place
  add_custom_target(
    PythonPackageSetup
    COMMENT "Python packaging"
    VERBATIM
    SOURCES ${POUTRE_python_package_SRC} )

  # copy the known dependencies before the build
  list(LENGTH POUTRE_PYTHON_BINARY_EXTENSIONS ext_length)
  if(${ext_length})
    #set(list_copy ${POUTRE_PYTHON_BINARY_EXTENSIONS})
    #list(REMOVE_DUPLICATES list_copy)
    #foreach(current_dependency IN LISTS list_copy)
    #  add_custom_command(
    #    TARGET PythonPackageSetup
    #    PRE_BUILD
    #    COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE_DIR:${current_dependency}>/$<TARGET_FILE_NAME:${current_dependency}> .
    #    WORKING_DIRECTORY ${python_package_binary_dir}
    #    COMMENT "Copy ${current_dependency}"
    #    VERBATIM
    #  )
    #endforeach(current_dependency)
    
    # launches the install command with this specific component
    add_custom_command(
      TARGET PythonPackageSetup
      PRE_BUILD
      COMMAND echo ${CMAKE_COMMAND} -DCOMPONENT=python_package_install_intermediate -P ${CMAKE_BINARY_DIR}/cmake_install.cmake
      COMMAND ${CMAKE_COMMAND} -DCOMPONENT=python_package_install_intermediate -P ${CMAKE_BINARY_DIR}/cmake_install.cmake
    )
    add_dependencies(PythonPackageSetup ${POUTRE_PYTHON_BINARY_EXTENSIONS})
  endif()

  # copy additional files declared
  #list(LENGTH POUTRE_PYTHON_BINARY_EXTENSIONS_ADDITIONAL_FILES ext_length)
  #if(${ext_length})
  #  set(list_copy ${POUTRE_PYTHON_BINARY_EXTENSIONS_ADDITIONAL_FILES})
  #  list(REMOVE_DUPLICATES list_copy)
  #  foreach(current_dependency IN LISTS list_copy)
  #    add_custom_command(
  #      TARGET PythonPackageSetup
  #      PRE_BUILD
  #      COMMAND ${CMAKE_COMMAND} -E copy_if_different ${current_dependency} .
  #      WORKING_DIRECTORY ${python_package_binary_dir}
  #      COMMENT "Copy ${current_dependency}"
  #      VERBATIM
  #    )
  #  endforeach(current_dependency)
  #endif()


endmacro(create_python_package)
