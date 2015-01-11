# @file
# @author 

MACRO(Hg_WC_INFO dir prefix)
EXECUTE_PROCESS(COMMAND ${HG_EXECUTABLE} id -i -n
   WORKING_DIRECTORY ${dir}
    ERROR_VARIABLE Hg_error
   OUTPUT_VARIABLE ${prefix}_WC_DATA
    OUTPUT_STRIP_TRAILING_WHITESPACE)
IF(NOT ${Hg_error} EQUAL 0)
  MESSAGE(SEND_ERROR "Command \"${HG_EXECUTABLE} id -i -n\" in directory ${dir} failed with output:\n${Hg_error}")
ENDIF(NOT ${Hg_error} EQUAL 0)
STRING(REPLACE " " ";" ${prefix}_WC_DATA ${${prefix}_WC_DATA})
LIST(GET ${prefix}_WC_DATA 0 ${prefix}_WC_CHANGESET)
LIST(GET ${prefix}_WC_DATA 1 ${prefix}_WC_REVISION)
STRING(REGEX REPLACE "\\+$" "" ${prefix}_WC_REVISION ${${prefix}_WC_REVISION})
STRING(REGEX REPLACE "\\+$" "" ${prefix}_WC_CHANGESET ${${prefix}_WC_CHANGESET})
ENDMACRO(Hg_WC_INFO)

macro(get_hg_revisions HG_REV HG_CHANGSET)
    find_package(Hg)
    if(HG_FOUND)
        message("hg found: ${HG_EXECUTABLE}")
        Hg_WC_INFO(${PROJECT_SOURCE_DIR} Project)
        message("Current revision is ${Project_WC_REVISION}")
        message("Current changeset is ${Project_WC_CHANGESET}")
        SET(HG_REV "${Project_WC_REVISION}")
        SET(HG_CHANGSET "${Project_WC_CHANGESET}") 
    else(HG_FOUND)
        message(FATAL_ERROR "HG not found")
    endif()
endmacro(get_hg_revisions)


macro(generate_library_version filename_to_generate)
  set(_revision_file_name ${POUTRE_root_dir}/cmake/POUTRE_revision.cpp.config)

  set(_should_generate FALSE)
  set(_is_archive FALSE)
  
  set(_current_rev)
  set(_current_changeset)
  get_hg_revisions(_current_rev _current_changeset)
  if("${_current_rev}" STREQUAL "none")
    message(STATUS "[GIT] looks like a tarball")
    set(_is_archive TRUE)
    if(NOT EXISTS ${filename_to_generate})
      set(_current_rev "archive")
      set(_should_generate TRUE)
      message(FATAL_ERROR "Non existing")
    else()
      message(STATUS "[HG] file ${filename_to_generate} already contains the version")
    endif()
  endif()
  
  if(NOT ${_is_archive})
    if(DEFINED _POUTRE_previous_revision)
      if(NOT (${_POUTRE_previous_revision} STREQUAL ${_current_rev}) OR (NOT EXISTS ${filename_to_generate}))
        set(_should_generate TRUE)
      endif()
    else()
      set(_should_generate TRUE)
    endif()
  endif()
  
  if(_should_generate)
    message(STATUS "Repository at revision ${_current_rev} commited at ${_current_date}")
    set(_POUTRE_previous_revision ${_current_rev} CACHE INTERNAL "internal revision to avoid repeated rebuild" FORCE)
    set(revision_version  ${_current_rev})
    configure_file(${_revision_file_name} ${filename_to_generate})
    set_source_files_properties(${filename_to_generate} PROPERTIES GENERATED TRUE)
    unset(revision_version)
  endif()
  
  unset(_should_generate)
  unset(_is_archive)
  unset(_current_rev)
  unset(_revision_file_name)
  
endmacro(generate_library_version)


