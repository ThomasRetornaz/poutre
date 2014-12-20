

set(PoutreBaseSubdirectory ${POUTRE_CORE_DIR}/PoutreBase)

generate_library_version(${PoutreBaseSubdirectory}/src/current_configuration_generated.cpp)

set(PoutreBaseSRC
  ${PoutreBaseSubdirectory}/poutreBase.hpp
  # auto generated file
  ${PoutreBaseSubdirectory}/src/current_configuration_generated.cpp
)

add_library(PoutreBase ${PoutreBaseSRC})
target_include_directories(PoutreBase 
                           PRIVATE ${PoutreBaseSubdirectory}
                           PRIVATE ${PoutreBaseSubdirectory}/include
                           PRIVATE ${PoutreBaseSubdirectory}/src)
target_link_libraries(PoutreBase 
                      # PRIVATE ${Boost_GRAPH_LIBRARY} 
                      # PRIVATE ${Boost_DATE_TIME_LIBRARY} 
                      # PRIVATE ${Boost_REGEX_LIBRARY} 
                      # PRIVATE ${Boost_THREAD_LIBRARY} 
                      # PRIVATE ${Boost_SYSTEM_LIBRARY}
                      )
set_target_properties(PoutreBase PROPERTIES 
                      DEFINE_SYMBOL "POUTRE_EXPORT_COMMON_"
                      FOLDER "Core/")
install_POUTRE_targets(
  TARGET PoutreBase 
  CONFIGURATION Release 
  HEADER_FILES ${PoutreBaseSRC}
  )




# Test project
set(PoutreBaseTestSubdirectory ${POUTRE_CORE_TEST_DIR}/PoutreBaseCoreTests)
set(PoutreBaseTestSRC
  ${PoutreBaseTestSubdirectory}/build_date.cpp
  ${PoutreBaseTestSubdirectory}/coordinates.cpp
  ${PoutreBaseTestSubdirectory}/dispatch_tests.cpp
  ${PoutreBaseTestSubdirectory}/main.cpp
  ${PoutreBaseTestSubdirectory}/main.hpp
  ${PoutreBaseTestSubdirectory}/types_and_variant_tests.cpp
  ${PoutreBaseTestSubdirectory}/priority_queues.cpp
  ${PoutreBaseTestSubdirectory}/graphs.cpp
  ${PoutreBaseTestSubdirectory}/histograms.cpp
  ${PoutreBaseTestSubdirectory}/trees.cpp
  ${PoutreBaseTestSubdirectory}/matrices_tests.cpp
  ${PoutreBaseTestSubdirectory}/color_space.cpp
  ${PoutreBaseTestSubdirectory}/thread_pool_test.cpp
)
add_executable(PoutreBaseTests ${PoutreBaseTestSRC})
target_link_libraries(PoutreBaseTests PoutreBase ${Boost_GRAPH_LIBRARY} ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})
add_test(NAME PoutreBaseTests-1 
         COMMAND PoutreBaseTests)
set_property(TARGET PoutreBaseTests PROPERTY FOLDER "UnitTests/")
test_environment_setup(PoutreBaseTests-1)

# Python project
set(PoutreBasePythonSubdirectory ${POUTRE_PYTHONEXT_DIR}/PoutreBasePython)
set(PoutreBasePythonSRC
  ${PoutreBasePythonSubdirectory}/common_python.hpp
  ${PoutreBasePythonSubdirectory}/common_python.cpp

  ${PoutreBasePythonSubdirectory}/common_utils.cpp
  ${PoutreBasePythonSubdirectory}/common_enums.cpp
  ${PoutreBasePythonSubdirectory}/common_variant.cpp
  ${PoutreBasePythonSubdirectory}/common_error.cpp
  ${PoutreBasePythonSubdirectory}/common_object.cpp
  ${PoutreBasePythonSubdirectory}/common_coordinate.cpp
  ${PoutreBasePythonSubdirectory}/common_returncode.cpp
  ${PoutreBasePythonSubdirectory}/common_graph.cpp
  ${PoutreBasePythonSubdirectory}/common_colorspace.cpp
)


add_python_library(PoutreBasePython "${PoutreBasePythonSRC}" "PoutreBase")
target_include_directories(PoutreBasePython 
                           PRIVATE ${PoutreBasePythonSubdirectory}/include)
add_to_python_packaging(PoutreBasePython)

#target_compile_options(PoutreBasePython PRIVATE "-fvisibility=hidden")
#target_compile_options(PoutreBasePython PRIVATE "-s")
#target_compile_options(PoutreBasePython PRIVATE "-fvisibility-inlines-hidden")

#      #get_target_property(
#      #  _previous_link_flags 
#      #  PoutreBasePython
#      #  LINK_FLAGS)
#set_target_properties(PoutreBasePython
#          PROPERTIES 
#            LINK_FLAGS "-Wl,-exported_symbol,_initPoutreBasePython"
#        )

add_test(NAME PoutreBaseTestsPython-1 
         COMMAND ${PYTHON_EXECUTABLE} ${PoutreBasePythonSubdirectory}/common_test.py $<TARGET_FILE_DIR:PoutreBasePython>)
test_environment_setup(PoutreBaseTestsPython-1)

