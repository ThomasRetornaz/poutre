
include_directories(${POUTRE_SRC_DIR})

# Test project
set(PoutreBaseTestSubdirectory ${POUTRE_NRT_DIR}/poutreBase)
set(PoutreBaseTestSRC
  ${PoutreBaseTestSubdirectory}/main.hpp
  ${PoutreBaseTestSubdirectory}/main.cpp
  #dummy tests to validate proper import of third parties dependencies
  #${PoutreBaseTestSubdirectory}/nt2TablePlayground.cpp
  #${PoutreBaseTestSubdirectory}/eigenCleanImport.cpp
  ${PoutreBaseTestSubdirectory}/boostSimdCleanImport.cpp
  ${PoutreBaseTestSubdirectory}/chronos.cpp
  ${PoutreBaseTestSubdirectory}/jsonSerialize.cpp
  ${PoutreBaseTestSubdirectory}/registrar.cpp
  ${PoutreBaseTestSubdirectory}/trace.cpp
  ${PoutreBaseTestSubdirectory}/geometry.cpp
  ${PoutreBaseTestSubdirectory}/coordinate.cpp
  ${PoutreBaseTestSubdirectory}/static_array.cpp
  ${PoutreBaseTestSubdirectory}/types.cpp
  ${PoutreBaseTestSubdirectory}/containerview.cpp
  #${PoutreBaseTestSubdirectory}/tree.cpp
  )

add_executable(PoutreBaseTests ${PoutreBaseTestSRC})
set_target_properties(PoutreBaseTests PROPERTIES 
  DEFINE_SYMBOL "JSON_DLL"
  FOLDER "Core/")

target_link_libraries(PoutreBaseTests 
  INTERFACE Eigen  
  INTERFACE BoostSimd 
  PRIVATE ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}  
  # ${Boost_THREAD_LIBRARY} 
  # ${Boost_SYSTEM_LIBRARY} 
  # ${Boost_LOG_LIBRARY}
  # ${Boost_LOG_SETUP_LIBRARY}
  PUBLIC PoutreBase
  )
add_dependencies(PoutreBaseTests BoostSimd)
add_dependencies(PoutreBaseTests Eigendep)

IF(BUILD_SHARED_LIBS)
  SET_TARGET_PROPERTIES(PoutreBaseTests PROPERTIES COMPILE_FLAGS -DJSON_DLL)
  SET_TARGET_PROPERTIES(PoutreBaseTests PROPERTIES COMPILE_FLAGS -DBOOST_LOG_DLL) #FIXME not true should be linked with boost
  SET_TARGET_PROPERTIES(PoutreBaseTests PROPERTIES COMPILE_FLAGS -DBOOST_LOG_DYN_LINK) #FIXME not true should be linked with boost
ENDIF(BUILD_SHARED_LIBS)

add_test(NAME PoutreBaseTests-1 
  COMMAND PoutreBaseTests)
set_property(TARGET PoutreBaseTests PROPERTY FOLDER "UnitTests/")
test_environment_setup(PoutreBaseTests-1)

