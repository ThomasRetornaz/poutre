# @file
# This file contains precompilied header related macros/functions

# @author Raffi Enficiaud





function(create_pch_headers library_name file_names additionnal_files_in_pch)
  set(headers ;)
  set(cpp ;)
  get_filename_component(POUTREabs ${POUTRE_root_dir} ABSOLUTE)
  list(APPEND additionnal_files_in_pch ${file_names})
  foreach(loop_var ${additionnal_files_in_pch})
    get_filename_component(filepathabs ${loop_var} ABSOLUTE)
    get_filename_component(filepath ${filepathabs} PATH)
    file(RELATIVE_PATH relative_p "${POUTREabs}" "${filepathabs}")

    get_filename_component(extensiont ${loop_var} EXT)
    string(TOLOWER "${extensiont}" extension)
    if("${extension}" STREQUAL ".hpp" OR "${extension}" STREQUAL ".h")
      list(APPEND headers "${relative_p}")
    elseif("${extension}" STREQUAL ".cpp" OR "${extension}" STREQUAL ".c")
      list(APPEND cpp ${relative_p})
    endif()
  endforeach()
  list(SORT headers)
  list(REMOVE_DUPLICATES headers)

  message("Headers are ")
  foreach(elem ${headers})
    message("\t ${elem} \n")
  endforeach()

  set(cache_entry ${library_name}_pch_headers)
  # message("Entry "${cache_entry} )
  # message("Entry ""${headers}" )
  set(NEED_REGENERATE FALSE)
  if(NOT(DEFINED ${cache_entry}) OR NOT("${cache_entry}" STREQUAL "${headers}"))
    set(${cache_entry} "${headers}" CACHE INTERNAL "documentation cache ${library_name}" FORCE)
    set(NEED_REGENERATE TRUE)
  endif()

  set(pch_file_cpp0 ${TEMPORARY_DIRECTORY}/${library_name}_pch_headers/${cache_entry}.cpp )
  set(pch_file_hpp0 ${TEMPORARY_DIRECTORY}/${library_name}_pch_headers/${cache_entry}.hpp )
  include_directories(${TEMPORARY_DIRECTORY}/${library_name}_pch_headers)
  get_filename_component(pch_file_hpp ${pch_file_hpp0} REALPATH)
  get_filename_component(pch_file_cpp ${pch_file_cpp0} REALPATH)
  get_filename_component(pch_file_hppn ${pch_file_hpp0} NAME)
  get_filename_component(pch_file_cppn ${pch_file_cpp0} NAME)

  get_filename_component(pch_hpp_abs ${pch_file_hpp} REALPATH)
  get_filename_component(pch_cpp_abs ${pch_file_cpp} REALPATH)
  set_source_files_properties(${pch_hpp_abs} PROPERTIES GENERATED 1)
  set_source_files_properties(${pch_cpp_abs} PROPERTIES GENERATED 1) # this force the regeneration of the file
  
  
  if(${NEED_REGENERATE})

    

    file(WRITE ${pch_file_cpp} "//cmake automatically generated file - do not edit \n//" ${pch_file_cppn} "\n")
    file(APPEND ${pch_file_cpp} "#include \"${pch_file_hppn}\"\n")
    file(WRITE ${pch_file_hpp} "//cmake automatically generated file - do not edit \n")
    file(APPEND ${pch_file_hpp} "// ${pch_file_cppn}\n")
    foreach(loop_var ${headers})
      file(TO_CMAKE_PATH ${loop_var} res)
      
      file(APPEND ${pch_file_hpp} "#include <POUTRE/${res}>\n" )
    endforeach()
    
    #set_target_properties(${library_name} PROPERTIES COMPILE_FLAGS "/FI\"${pch_hpp_abs}\"")
    #set_source_files_properties(${pch_cpp_abs} PROPERTIES COMPILE_FLAGS "/Yc\"${pch_hpp_abs}\"")
    #set_target_properties(${library_name} PROPERTIES COMPILE_FLAGS "/Yu\"${pch_hpp_abs}\"")
  endif()
  
  get_filename_component(precompiled_base_name ${pch_cpp_abs} NAME_WE)
  set(precompiled_binary "${precompiled_base_name}.pch")
  set_source_files_properties(${pch_cpp_abs}
    PROPERTIES COMPILE_FLAGS "/Yc\"${pch_file_hppn}\" /Fp\"${precompiled_binary}\""
    OBJECT_OUTPUTS "${precompiled_binary}")
  #set_source_files_properties(${cpp}
  #    PROPERTIES COMPILE_FLAGS "/Yu\"${precompiled_binary}\" /FI\"${pch_file_hppn}\" /Fp\"${precompiled_binary}\""
  #    OBJECT_DEPENDS "${PrecompiledBinary}")  
  
  #set_property(TARGET ${library_name} APPEND PROPERTY SOURCES ${pch_hpp_abs} ${pch_cpp_abs})
  add_library(${library_name} ${pch_hpp_abs} ${pch_cpp_abs} ${file_names})
  set_target_properties(${library_name}
      PROPERTIES COMPILE_FLAGS "/Yu\"${pch_file_hppn}\" /FI\"${pch_file_hppn}\" /Fp\"${precompiled_binary}\"")  
    

  
endfunction(create_pch_headers)
