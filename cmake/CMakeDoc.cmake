
# Doxygen
get_filename_component(POUTRE_root_dir_abs ${POUTRE_ROOT_DIR} ABSOLUTE)
set(POUTRE_DOCUMENTATION_ROOT ${POUTRE_root_dir_abs}/doc)
set(POUTRE_DOXYGEN_PATH ${POUTRE_DOCUMENTATION_ROOT}/Doxygen)
find_file(DOXYGEN_CONFIG_FILE Doxyfile.in ${POUTRE_DOXYGEN_PATH})

find_package(Doxygen)
if(NOT DOXYGEN_FOUND)
  message("Doxygen not found")
elseif(DOXYGEN_CONFIG_FILE)
  message(STATUS "[POUTREDoc] Doxygen: use ${DOXYGEN_CONFIG_FILE}")
  set(POUTRE_HTML_FILES_DIR "${POUTRE_ROOT_DIR}/doc/Doxygen/misc" CACHE FILEPATH "POUTRE_HTML_FILES_DIR")
  
  if(DOXYGEN_DOT_FOUND)
    message("[POUTREDoc] DoxyDot found at ${DOXYGEN_DOT_PATH}")
  else(DOXYGEN_DOT_FOUND)
    message("[POUTREDoc] DoxyDot not found")
  endif(DOXYGEN_DOT_FOUND)

  set(DOXY_OUTPUT_DIR           ${CMAKE_BINARY_DIR}/documentation/doxygen)
  set(DOXY_CONFIG_FILE          ${DOXY_OUTPUT_DIR}/Doxyfile.generated)

  set(DOXY_STRIP_FROM_PATH      ${POUTRE_root_dir_abs})
  set(DOXY_STRIP_FROM_INC_PATH  ${POUTRE_root_dir_abs})
    
  option(DOXYGEN_GENERATE_XML   "Generates xml documentation"   OFF)
  option(DOXYGEN_GENERATE_LATEX "Generates LateX documentation" OFF)
  option(DOXYGEN_GENERATE_HTML  "Generates HTML documentation"  ON)
  
  mark_as_advanced(DOXYGEN_GENERATE_HTML)
  mark_as_advanced(DOXYGEN_GENERATE_XML)
  mark_as_advanced(DOXYGEN_GENERATE_HTML)

  #GRAB FOLDER TO DOCUMENT
  get_property(POUTRE_CORE_DIR GLOBAL PROPERTY POUTRE_CORE_DIR)
  set(DOXY_INPUT_DIRS \"${POUTRE_DOCUMENTATION_ROOT}/Doxygen\")
  foreach(_DIR ${POUTRE_CORE_DIR})
    list(APPEND DOXY_INPUT_DIRS \"${_DIR}/\")
  endforeach(_DIR ${POUTRE_CORE_DIR})


  set(DOXY_FILES_PATTERN *.h *.hpp *.dox) 
  set(DOXY_IMAGE_PATH   \"${POUTRE_HTML_FILES_DIR}\" CACHE FILEPATH  "Doxygen image path")
  set(DOXY_UNIT_TEST_PATH \"${POUTRE_HTML_FILES_DIR}\" \"${POUTRE_ROOT_DIR}/coreTests\" CACHE FILEPATH  "Doxygen unit tests path")
  set(DOXY_LOGO_PATH    "${POUTRE_ROOT_DIR}/doc/Doxygen/misc/yun_5048_small.jpg" CACHE FILEPATH  "Doxygen logo")
  set(DOXY_INCLUDE_PATH \"${JPEG_LIB_SRC}\" \"${PNG_LIB_SRC}\" \"${ZLIB_SRC_DIR}\" \"${Boost_INCLUDE_DIRS}\" CACHE FILEPATH "Doxygen preprocessor paths")
  set(DOXY_BIB_FILE "${POUTRE_DOCUMENTATION_ROOT}/Bib/bib_full.bib")

  string(REGEX REPLACE ";" " \\\\ \n" DOXY_FILES_PATTERN    "${DOXY_FILES_PATTERN}")
  string(REGEX REPLACE ";" " \\\\ \n" DOXY_INPUT_DIRS       "${DOXY_INPUT_DIRS}")
  string(REGEX REPLACE ";" " \\\\ \n" DOXY_IMAGE_PATH       "${DOXY_IMAGE_PATH}")
  string(REGEX REPLACE ";" " \\\\ \n" DOXY_EXAMPLE_PATH     "${DOXY_EXAMPLE_PATH}")
  string(REGEX REPLACE ";" " \\\\ \n" DOXY_LOGO_PATH        "${DOXY_LOGO_PATH}")
  #string(REGEX REPLACE ";" " \\\\ \n" DOXY_INCLUDE_PATH     "${DOXY_INCLUDE_PATH}")
  string(REGEX REPLACE ";" " \\\\ \n" DOXY_EXTRA_FILES      "${DOXY_EXTRA_FILES}")

  
  # Doxygen XML 


  # CHM Windows help
  IF (WIN32)
    # we do not generate this shit
    find_package(HTMLHelp)
    if(HTML_HELP_COMPILER)
      option(DOXYGEN_GENERATE_HTML_HELP "Generate html-help documentation" ON)
      if(DOXYGEN_GENERATE_HTML_HELP)
        set(DOXY_GENERATE_HTMLHELP YES)
        set(DOXY_CHM_FILE ${DOXY_OUTPUT_DIR}/${PROJECT_NAME}.chm)
        string(REGEX REPLACE "[/]" "\\\\" DOXY_CHM_FILE ${DOXY_CHM_FILE})
      endif(DOXYGEN_GENERATE_HTML_HELP)
    endif(HTML_HELP_COMPILER)
  endif(WIN32)



  #### DOT ####
  if(DOXYGEN_DOT_FOUND)
    set(DOXY_HAVE_DOT YES)
    set(DOT_BINARY_PATH ${DOXYGEN_DOT_PATH})
  else()
    set(DOXY_HAVE_DOT NO)
    set(DOT_BINARY_PATH "")
  endif()

  if(CMAKE_BUILD_TOOL MATCHES "(msdev|devenv)")
    set(DOXY_WARN_FORMAT "\"$file($line) : $text \"")
  else()
    set(DOXY_WARN_FORMAT "\"$file:$line: $text \"")
  endif()

  message(STATUS "[POUTREDoc] Configuring file ${DOXYGEN_CONFIG_FILE} to ${DOXY_CONFIG_FILE}")
  configure_file(${DOXYGEN_CONFIG_FILE} ${DOXY_CONFIG_FILE} @ONLY)

  file(GLOB doxygen_pages ${POUTRE_DOXYGEN_PATH}/*.dox)
  set(doxygen_layout_files 
      ${POUTRE_DOXYGEN_PATH}/misc/DoxygenLayout.xml
      ${DOXY_HTML_STYLESHEET}
      ${DOXY_HTML_STYLESHEET_EXT}
      ${POUTRE_DOXYGEN_PATH}/misc/footer.html
      ${POUTRE_DOXYGEN_PATH}/misc/header.html)

  add_custom_target(Doxygen
    ${DOXYGEN_EXECUTABLE} ${DOXY_CONFIG_FILE}
    COMMENT "Generating doxygen documentation"
    DEPENDS ${DOXYGEN_CONFIG_FILE}
    SOURCES 
      ${POUTRE_ROOT_DIR}/cmake/CMakeDoc.cmake
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
