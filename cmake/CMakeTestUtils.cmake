

# this macro just sets the proper environment variable for running the unit tests
# (PATH, LD_LIBRARY_PATH or DYLD_LIBRARY_PATH depending on the platform, mainly because
# boost.test is in a shared library).
macro(test_environment_setup test_name)

  #message(STATUS "test_environment_setup with ${test_name}")

  # getting BOOST path, from boost.test release version (debug non always available)
  get_filename_component(boost_unit_test_location ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY_RELEASE} PATH)
  file(TO_NATIVE_PATH ${boost_unit_test_location} boost_binaries_path)

  if(WIN32)
    set_tests_properties(${test_name} PROPERTIES ENVIRONMENT "PATH=${boost_binaries_path}\;$ENV{PATH}")
  else()
    if(APPLE)
      set(SHARED_LIB_PATH "DYLD_LIBRARY_PATH")
    else()
      set(SHARED_LIB_PATH "LD_LIBRARY_PATH")
    endif()
    #todo add old environment
    set_tests_properties(${test_name} PROPERTIES ENVIRONMENT "${SHARED_LIB_PATH}=${boost_binaries_path}")
  endif()

endmacro(test_environment_setup)
