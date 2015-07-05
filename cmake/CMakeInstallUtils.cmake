# @file
# This file contains installation related macros/functions

function(install_lib_and_dll file_names path_to_install component)
  foreach(loop_var ${file_names})
    message("file ${loop_var} ")
    get_filename_component(filepathabs ${loop_var} ABSOLUTE)
    get_filename_component(filepath ${filepathabs} PATH)
    get_filename_component(filename ${filepathabs} NAME_WE)
    set(file_to_install ${filepath}/${filename}${CMAKE_SHARED_LIBRARY_SUFFIX})
    if(NOT EXISTS ${file_to_install})
      message("file ${file_to_install} does not EXISTS !!!")
    endif()
    message("Installing ${file_to_install} to ${path_to_install}")
    install(FILES ${file_to_install} DESTINATION ${path_to_install} COMPONENT ${component})  
  endforeach()
endfunction(install_lib_and_dll)

function(install_realpath_with_rename file_names path_to_install component configuration)
  foreach(loop_var ${file_names})
    message("file ${loop_var} ")
    get_filename_component(filepathreal ${loop_var} REALPATH)
    get_filename_component(filename ${loop_var} NAME)
  
    if(NOT EXISTS ${filepathreal})
      message("file ${filepathreal} does not EXISTS !!!")
    endif()
    message("Installing ${loop_var} (${filepathreal}) to ${path_to_install} with name ${filename}")
    install(FILES ${loop_var} CONFIGURATIONS ${configuration} DESTINATION ${path_to_install} COMPONENT ${component} RENAME ${filename})  
  endforeach()
endfunction(install_realpath_with_rename)




#
# This function creates installation rule for poutre targets
# install_POUTRE_targets(TARGET target_name HEADER_FILES files CONFIGURATION configuration)
function(install_POUTRE_targets)

  set(options )
  set(oneValueArgs CONFIGURATION TARGET)
  set(multiValueArgs HEADER_FILES)
  cmake_parse_arguments(my_install "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

  # installing the given targets
  if(my_install_TARGET)
    install(TARGETS ${my_install_TARGET}
            DESTINATION ${POUTRE_LIBRARIES_INSTALLATION_RELATIVE_PATH}
            COMPONENT core
            CONFIGURATIONS ${my_install_CONFIGURATION})
    
    
    # trying relocatable targets, works, except for the problem on boost 
    # on apple, the name of the so should be changed with the install_name_tool
    # with options -change to change the name of the dependency, and -id to change
    # the name of the shared object itself.
    if(APPLE)
      set_target_properties(${my_install_TARGET} PROPERTIES INSTALL_NAME_DIR "@loader_path")
    elseif(UNIX)
      set_target_properties(${my_install_TARGET}
        PROPERTIES
          INSTALL_RPATH "$ORIGIN/."
          BUILD_WITH_INSTALL_RPATH TRUE)
    endif()
  endif()


  # installing the header files
  get_filename_component(POUTREabs ${POUTRE_ROOT_DIR} ABSOLUTE)
  
  foreach(loop_var ${my_install_HEADER_FILES})
    get_filename_component(filepathabs ${loop_var} ABSOLUTE)
    get_filename_component(filepath ${filepathabs} PATH)
    file(RELATIVE_PATH relative_p "${POUTREabs}" "${filepath}")
    
    get_filename_component(extensiont ${loop_var} EXT)
    string(TOLOWER "${extensiont}" extension)
    
    if("${extension}" STREQUAL ".hpp" OR "${extension}" STREQUAL ".h")
      install(FILES ${loop_var} 
              DESTINATION ${POUTRE_HEADER_INSTALLATION_RELATIVE_PATH}/${relative_p}
              COMPONENT core
              CONFIGURATIONS ${my_install_CONFIGURATION}) 
    endif()
  endforeach()
  
endfunction(install_POUTRE_targets)
