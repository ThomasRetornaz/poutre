## 
##
## Installation 
##
set(CPACK_PACKAGE_NAME                  "POUTRE")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY   "")
set(CPACK_PACKAGE_VENDOR                "Thomas Retornaz")

set(CPACK_PACKAGE_DESCRIPTION_FILE      "${POUTRE_root_dir}/README.txt")
set(CPACK_RESOURCE_FILE_LICENSE         "${POUTRE_root_dir}/LICENSE_1_0.txt")
set(CPACK_PACKAGE_VERSION               ${POUTRE_VERSION})
set(CPACK_PACKAGE_VERSION_MAJOR         ${POUTRE_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR         ${POUTRE_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH         ${POUTRE_SUBMINOR})
set(CPACK_PACKAGE_INSTALL_DIRECTORY     "POUTRE ${CPACK_PACKAGE_VERSION}")
set(CPACK_PACKAGE_CONTACT               "Thomas Retornaz<thomas.retornaz@free.fr>")

set(CPACK_COMPONENTS_ALL core python api_doc python_doc)
set(CPACK_COMPONENT_core_DISPLAY_NAME           "Core C++ libraries and headers")
set(CPACK_COMPONENT_python_DISPLAY_NAME         "Python extensions")
set(CPACK_COMPONENT_api_doc_DISPLAY_NAME        "Developer C++ documentation")
set(CPACK_COMPONENT_python_doc_DISPLAY_NAME     "Developer Python documentation")

set(CPACK_COMPONENT_core_DESCRIPTION            "DLL and C++ headers for building programs with POUTRE")
set(CPACK_COMPONENT_python_DESCRIPTION          "Python extensions of POUTRE")
set(CPACK_COMPONENT_api_doc_DESCRIPTION         "Auto generated doxygen documentation of the C++ part of POUTRE")
set(CPACK_COMPONENT_python_doc_DESCRIPTION      "Auto generated Sphinx documentation of the Python bindings and Python packages of POUTRE")

set(CPACK_COMPONENT_python_DEPENDS              core)

set(CPACK_COMPONENT_python_doc_GROUP                "Documentation")
set(CPACK_COMPONENT_api_doc_GROUP                   "Documentation")
set(CPACK_COMPONENT_GROUP_DOCUMENTATION_DESCRIPTION "Documentation")

set(CPACK_SOURCE_IGNORE_FILES
    ${CPACK_SOURCE_IGNORE_FILES}
    ${CMAKE_BINARY_DIR}
    ${POUTRE_root_dir}/.hg
    ${POUTRE_root_dir}/.hgignore
)

if(WIN32 AND NOT UNIX)
  # There is a bug in NSI that does not handle full unix paths properly. Make
  # sure there is at least one set of four (4) backlasshes.
  #set(CPACK_PACKAGE_ICON                "")
  #set(CPACK_NSIS_INSTALLED_ICON_NAME    "")
  set(CPACK_NSIS_DISPLAY_NAME           "${CPACK_PACKAGE_INSTALL_DIRECTORY}")
  set(CPACK_NSIS_HELP_LINK              "http:////thomas.retornaz.free.fr")
  set(CPACK_NSIS_URL_INFO_ABOUT         "http:////thomas.retornaz.fr")
  set(CPACK_NSIS_CONTACT                "thomas.retornaz@free.fr")
  set(CPACK_NSIS_MODIFY_PATH            ON)

  set(CPACK_NSIS_ENABLE_UNINSTALL_BEFORE_INSTALL ON)
  set(CPACK_NSIS_URL_INFO_ABOUT ${CPACK_NSIS_HELP_LINK})
  set(CPACK_NSIS_MENU_LINKS
       "documentation/index.html" "C++ library documentation"
       ) 
  if(CMAKE_CL_64)
    set(CPACK_NSIS_INSTALL_ROOT "$PROGRAMFILES64")
    set(CPACK_NSIS_PACKAGE_NAME "${CPACK_PACKAGE_INSTALL_DIRECTORY} (Win64)")
    set(CPACK_PACKAGE_INSTALL_REGISTRY_KEY "${CPACK_PACKAGE_NAME} ${CPACK_PACKAGE_VERSION} (Win64)")
  else()
    set(CPACK_NSIS_INSTALL_ROOT "$PROGRAMFILES")
    set(CPACK_NSIS_PACKAGE_NAME "${CPACK_PACKAGE_INSTALL_DIRECTORY}")
    set(CPACK_PACKAGE_INSTALL_REGISTRY_KEY "${CPACK_PACKAGE_NAME} ${CPACK_PACKAGE_VERSION}")
  endif()


else()
  set(CPACK_STRIP_FILES                 TRUE)
  #set(CPACK_SOURCE_STRIP_FILES          TRUE)
endif()



# debian package generation
if(UNIX AND NOT APPLE)
  set(CPACK_GENERATOR DEB)
  SET(CPACK_DEB_COMPONENT_INSTALL 1)
  #set(CPACK_COMPONENTS_GROUPING IGNORE)
  file(READ ${CPACK_PACKAGE_DESCRIPTION_FILE} CPACK_DEBIAN_PACKAGE_DESCRIPTION)
  string(REPLACE "\n" "" CPACK_DEBIAN_PACKAGE_DESCRIPTION "${CPACK_DEBIAN_PACKAGE_DESCRIPTION}")
  string(REPLACE "\r" "" CPACK_DEBIAN_PACKAGE_DESCRIPTION "${CPACK_DEBIAN_PACKAGE_DESCRIPTION}")

  message(${CPACK_DEBIAN_PACKAGE_DESCRIPTION})



  set(CPACK_DEBIAN_PACKAGE_DEPENDS)
  list(LENGTH debian_dependencies debian_dependencies_lenght)
  if(${debian_dependencies_lenght} GREATER 0)
    list(GET debian_dependencies 0 CPACK_DEBIAN_PACKAGE_DEPENDS)
    list(REMOVE_AT debian_dependencies 0)
  endif()
  
  foreach(var IN LISTS debian_dependencies)
    set(CPACK_DEBIAN_PACKAGE_DEPENDS "${CPACK_DEBIAN_PACKAGE_DEPENDS}, ${var}")
  endforeach()

endif()
include(CPack)

