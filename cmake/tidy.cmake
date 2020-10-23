#
# clang-tidy
#
find_program(CLANG_TIDY_EXE "clang-tidy")
mark_as_advanced(FORCE CLANG_TIDY_EXE)
if(CLANG_TIDY_EXE)
  message(STATUS "clang-tidy found: ${CLANG_TIDY_EXE}")
else()
  message(STATUS "clang-tidy not found!")
endif()

function(poutre_tidy TARGET_NAME)
  if(CLANG_TIDY_EXE)
    set(CHECK_FILES)
    # Check through the ARGN's, determine existent files
    foreach(item IN LISTS ARGN)
      if(TARGET ${item})
        # If the item is a target, then we'll attempt to grab the associated
        # source files from it.
        get_target_property(_TARGET_TYPE ${item} TYPE)
        if(NOT _TARGET_TYPE STREQUAL "INTERFACE_LIBRARY")
          get_property(
            _TEMP
            TARGET ${item}
            PROPERTY SOURCES)
          foreach(iter IN LISTS _TEMP)
            if(EXISTS ${iter})
              set(CHECK_FILES ${CHECK_FILES} ${iter})
            endif()
          endforeach()
        endif()
      elseif(EXISTS ${item})
        # Check if it's a full file path
        set(CHECK_FILES ${CHECK_FILES} ${item})
      elseif(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${item})
        # Check if it's based on the current source dir
        set(CHECK_FILES ${CHECK_FILES} ${CMAKE_CURRENT_SOURCE_DIR}/${item})
      endif()
    endforeach()

    # Make the target
    if(CHECK_FILES)
      if(TARGET ${TARGET_NAME})
        message(
          ERROR
          "Cannot create clang-tidy target '${TARGET_NAME}', already exists.")
      else()
        add_custom_target(${TARGET_NAME} COMMAND ${CLANG_TIDY_EXE}                                                  
                                                  #-std=c++20
                                                  #${INCLUDE_DIRECTORIES}
                                                  ${CHECK_FILES}
                                                  ${POUTRE_SRC_DIR}
                                                  #EXCLUDE_FROM_ALL
                                                  )

        # if(NOT TARGET poutre-tidy)
        #   add_custom_target(poutre-tidy EXCLUDE_FROM_ALL)
        # endif()

        # add_dependencies(poutre-tidy ${TARGET_NAME})
      endif()
    endif()

  endif()
endfunction()