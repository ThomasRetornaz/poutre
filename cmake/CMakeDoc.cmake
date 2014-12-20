

# to avoid having some ../.. in the paths
get_filename_component(POUTRE_root_dir_absolute ${POUTRE_root_dir} ABSOLUTE)
set(POUTRE_DOCUMENTATION_ROOT ${POUTRE_root_dir_absolute}/doc)


# SPHINX
# Getting sphinx availability
execute_process(
  COMMAND ${PYTHON_EXECUTABLE} -c "try:\n\timport sphinx\n\tprint \"TRUE\"\nexcept Exception, e:\n\tprint \"FALSE\""
  OUTPUT_VARIABLE SPHINX_AVAILABLE
  ERROR_VARIABLE  SPHINX_ERROR
  OUTPUT_STRIP_TRAILING_WHITESPACE
)

# error in checking ? should not occur
if(NOT (SPHINX_ERROR STREQUAL ""))
  message(FATAL_ERROR "Error: something wrong with the sphinx detection script: ${SPHINX_ERROR}")
endif()

if(SPHINX_AVAILABLE STREQUAL "FALSE")
  message(STATUS "[POUTREDoc] Sphinx not available")
else()
  message(STATUS "[POUTREDoc] Sphinx available from python: adding sphinx target")

  if(UNIX)
    # adds POUTREPATH to make environment
    if(APPLE)
      set(VARRPATH "DYLD_LIBRARY_PATH")
    else()
      set(VARRPATH "LD_LIBRARY_PATH")
    endif()
    set(SPHINX_COMMAND make POUTREPATH=$<TARGET_FILE_DIR:POUTRECommonPython> ${VARRPATH}=$ENV{${VARRPATH}}:${Boost_LIBRARY_DIRS})
  elseif(WIN32)

    # test how to do it under windows...
    set(SPHINX_COMMAND "set POUTREPATH=$<TARGET_FILE_DIR:POUTRECommonPython>\nset PATH=%PATH%\;${Boost_LIBRARY_DIRS}\nmake.bat")
  endif()




  add_custom_target(Sphinx
    #${SPHINX_COMMAND} html
    sphinx-build -b html ${POUTRE_DOCUMENTATION_ROOT}/sphinx ${POUTRE_PYTHON_PACKAGE_LOCATION}/docs/build/html
    WORKING_DIRECTORY ${POUTRE_PYTHON_PACKAGE_LOCATION}
    COMMENT "Generating sphinx documentation sphinx-build -b html ${POUTRE_DOCUMENTATION_ROOT}/sphinx ${POUTRE_PYTHON_PACKAGE_LOCATION}/docs/build/html"
    #VERBATIM
    # we want the binaries to be there also
    DEPENDS PythonPackageSetup
    SOURCES
      ${POUTRE_DOCUMENTATION_ROOT}/sphinx/conf.py
      ${POUTRE_DOCUMENTATION_ROOT}/sphinx/index.rst 
      ${POUTRE_DOCUMENTATION_ROOT}/sphinx/color_utilities.rst
      ${POUTRE_DOCUMENTATION_ROOT}/sphinx/complete_example.rst
      ${POUTRE_DOCUMENTATION_ROOT}/sphinx/draw.rst
      ${POUTRE_DOCUMENTATION_ROOT}/sphinx/helper_scripts.rst
      ${POUTRE_DOCUMENTATION_ROOT}/sphinx/image_utilities.rst
      ${POUTRE_DOCUMENTATION_ROOT}/sphinx/installing_POUTRE.rst
      ${POUTRE_DOCUMENTATION_ROOT}/sphinx/morpho_utilities.rst
      ${POUTRE_DOCUMENTATION_ROOT}/sphinx/POUTRE_core.rst
      ${POUTRE_DOCUMENTATION_ROOT}/sphinx/POUTRE_core_files/common.rst
      ${POUTRE_DOCUMENTATION_ROOT}/sphinx/POUTRE_core_files/io.rst
    )
  set_property(TARGET Sphinx PROPERTY FOLDER "Documentation/")

endif()




# Doxygen
set(POUTRE_DOXYGEN_PATH ${POUTRE_DOCUMENTATION_ROOT}/Doxygen)
find_file(DOXYGEN_CONFIG_FILE Doxyfile ${POUTRE_DOXYGEN_PATH})
mark_as_advanced(DOXYGEN_CONFIG_FILE)


find_package(Doxygen)
if(NOT DOXYGEN_FOUND)
  message("Doxygen not found")
elseif(DOXYGEN_CONFIG_FILE)
  message(STATUS "[POUTREDoc] Doxygen: configuring documentation with file ${DOXYGEN_CONFIG_FILE}")
  set(POUTRE_HTML_FILES_DIR "${POUTRE_root_dir}/plugins/Website" CACHE FILEPATH "POUTRE_HTML_FILES_DIR")
  set(DOXY_LAYOUT_FILE ${POUTRE_DOXYGEN_PATH}/DoxygenLayout_new.xml)
  
  if(DOXYGEN_DOT_FOUND)
    message("[POUTREDoc] Doxygen dot found at ${DOXYGEN_DOT_PATH}")
  else(DOXYGEN_DOT_FOUND)
    message("[POUTREDoc] Doxygen dot not found")
  endif(DOXYGEN_DOT_FOUND)

  #message("POUTRE_HTML_FILES_DIR ${POUTRE_HTML_FILES_DIR}")

  ###### GENERAL ######
  set(DOXY_OUTPUT_DIR           ${CMAKE_BINARY_DIR}/documentation/doxygen)
  set(DOXY_CONFIG_FILE          ${DOXY_OUTPUT_DIR}/Doxyfile.generated)

  set(DOXY_STRIP_FROM_PATH      ${POUTRE_root_dir_absolute})
  set(DOXY_STRIP_FROM_INC_PATH  ${POUTRE_root_dir_absolute})
    
  option(DOXYGEN_GENERATE_XML   "Generates xml documentation"   TRUE)
  option(DOXYGEN_GENERATE_LATEX "Generates LateX documentation" FALSE)
  option(DOXYGEN_GENERATE_HTML  "Generates HTML documentation"  TRUE)
  

  mark_as_advanced(DOXYGEN_GENERATE_HTML)
  mark_as_advanced(DOXYGEN_GENERATE_XML)
  mark_as_advanced(DOXYGEN_GENERATE_HTML)

  # mark_as_advanced(DOXYGEN_HTML_HEADER)
  # mark_as_advanced(DOXYGEN_HTML_FOOTER)
  # mark_as_advanced(DOXYGEN_HTML_STYLESHEET)


  ###### INPUT FILES ######
  get_property(POUTRE_CORE_DIR GLOBAL PROPERTY POUTRE_CORE_DIR)
  set(DOXY_INPUT_DIRS \"${POUTRE_DOCUMENTATION_ROOT}/Doxygen\")
  foreach(_DIR ${POUTRE_CORE_DIR})
    list(APPEND DOXY_INPUT_DIRS \"${_DIR}/\")
  endforeach(_DIR ${POUTRE_CORE_DIR})


  set(DOXY_FILES_PATTERN *.h *.hpp *.dox) #add dox for mainpage and others see http://entrenchant.blogspot.com/2009/09/doxygen-gotchas.html
  set(DOXY_IMAGE_PATH   \"${POUTRE_HTML_FILES_DIR}\" CACHE FILEPATH  "Doxygen image path")
  set(DOXY_EXAMPLE_PATH \"${POUTRE_HTML_FILES_DIR}\" \"${POUTRE_root_dir}/coreTests\" CACHE FILEPATH  "Doxygen example path")
  #set(DOXY_LOGO_PATH    "${POUTRE_root_dir}/plugins/Logos/POUTRE_logo_seul.png" CACHE FILEPATH  "Doxygen logo")
  set(DOXY_INCLUDE_PATH \"${JPEG_LIB_SRC}\" \"${PNG_LIB_SRC}\" \"${ZLIB_SRC_DIR}\" \"${Boost_INCLUDE_DIRS}\" CACHE FILEPATH "Doxygen preprocessor paths")
  set(DOXY_CITE_BIB_FILE "${POUTRE_DOCUMENTATION_ROOT}/Latex/bib_all_utf8.bib")

  file(GLOB doxygen_extra_files ${POUTRE_DOXYGEN_PATH}/icons/*.*)
  set(doxygen_extra_files2)
  foreach(f ${doxygen_extra_files})
    list(APPEND doxygen_extra_files2 \"${f}\")
  endforeach(f ${doxygen_extra_files})

  set(DOXY_EXTRA_FILES   "${doxygen_extra_files2}")

  # translating the cmake lists into dox
  string(REGEX REPLACE ";" " \\\\ \n" DOXY_FILES_PATTERN    "${DOXY_FILES_PATTERN}")
  string(REGEX REPLACE ";" " \\\\ \n" DOXY_INPUT_DIRS       "${DOXY_INPUT_DIRS}")
  string(REGEX REPLACE ";" " \\\\ \n" DOXY_IMAGE_PATH       "${DOXY_IMAGE_PATH}")
  string(REGEX REPLACE ";" " \\\\ \n" DOXY_EXAMPLE_PATH     "${DOXY_EXAMPLE_PATH}")
  #string(REGEX REPLACE ";" " \\\\ \n" DOXY_LOGO_PATH        "${DOXY_LOGO_PATH}")
  string(REGEX REPLACE ";" " \\\\ \n" DOXY_INCLUDE_PATH     "${DOXY_INCLUDE_PATH}")
  string(REGEX REPLACE ";" " \\\\ \n" DOXY_EXTRA_FILES      "${DOXY_EXTRA_FILES}")

  
  #### XML ####

  ###DOCBOOK ?
  #http://www.sagehill.net/docbookxsl/
  #check XML is ON
  #check docbook xsl
  #check xslt proc somewhere
  #and or add py to launch

  ###### HTML HELP ######

  if (FALSE)
    # we do not generate this shit
    find_package(HTMLHelp)
    if(HTML_HELP_COMPILER)
      option(DOXYGEN_GENERATE_HTML_HELP "Generate html-help documentation" ON)
      if(DOXYGEN_GENERATE_HTML_HELP)
        set(DOXY_GENERATE_HTMLHELP YES)
        set(DOXY_CHM_FILE ${DOXY_OUTPUT_DIR}/${PROJECT_NAME}.chm)
        string(REGEX REPLACE "[/]" "\\\\" DOXY_CHM_FILE ${DOXY_CHM_FILE})
      endif()
    endif()
  endif()



  #### DOT ####
  if(DOXYGEN_DOT_FOUND)
    set(DOXY_HAVE_DOT YES)
    set(DOT_BINARY_PATH ${DOXYGEN_DOT_PATH})
  else()
    set(DOXY_HAVE_DOT NO)
    set(DOT_BINARY_PATH "")
  endif()

   ###### WARNINGS ######
  if(CMAKE_BUILD_TOOL MATCHES "(msdev|devenv)")
    set(DOXY_WARN_FORMAT "\"$file($line) : $text \"")
  else()
    set(DOXY_WARN_FORMAT "\"$file:$line: $text \"")
  endif()

  message(STATUS "[POUTREDoc] Configuring file ${DOXYGEN_CONFIG_FILE} to ${DOXY_CONFIG_FILE}")
  configure_file(${DOXYGEN_CONFIG_FILE} ${DOXY_CONFIG_FILE} @ONLY)

  file(GLOB doxygen_pages ${POUTRE_DOXYGEN_PATH}/*.dox)
  set(doxygen_layout_files 
      ${POUTRE_DOXYGEN_PATH}/DoxygenLayout_new.xml
      ${DOXY_HTML_STYLESHEET}
      ${DOXY_HTML_STYLESHEET_EXT}
      ${POUTRE_DOXYGEN_PATH}/footer.html
      ${POUTRE_DOXYGEN_PATH}/header.html)

  add_custom_target(Doxygen
    ${DOXYGEN_EXECUTABLE} ${DOXY_CONFIG_FILE}
    COMMENT "Generating doxygen documentation"
    DEPENDS ${DOXYGEN_CONFIG_FILE}
    SOURCES 
      ${POUTRE_root_dir}/cmake/CMakeDoc.cmake
      ${POUTRE_DOXYGEN_PATH}/Doxyfile
      ${DOXY_CONFIG_FILE}
      ${doxygen_pages}
      ${doxygen_layout_files}
      )
  set_property(TARGET Doxygen PROPERTY FOLDER "Documentation/")
  source_group(Pages FILES ${doxygen_pages})  
  source_group(Layout FILES ${doxygen_layout_files})  


  install(
    DIRECTORY ${DOXY_OUTPUT_DIR}/html/
    DESTINATION ${POUTRE_DOCUMENTATION_INSTALLATION_RELATIVE_PATH}
    COMPONENT api_doc
    PATTERN "html/*")


endif()
