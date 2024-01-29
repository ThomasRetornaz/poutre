
include_directories(${POUTRE_SRC_DIR})
include_directories(${GTEST_INCLUDE_DIR})

# Test project
set(PoutreBaseTestSubdirectory ${POUTRE_NRT_DIR}/poutreBase)
set(PoutreBaseTestSRC
  ${PoutreBaseTestSubdirectory}/main.hpp
  ${PoutreBaseTestSubdirectory}/main.cpp
  ${PoutreBaseTestSubdirectory}/chronos.cpp
  ${PoutreBaseTestSubdirectory}/jsonSerialize.cpp
  ${PoutreBaseTestSubdirectory}/registrar.cpp
  ${PoutreBaseTestSubdirectory}/coordinate.cpp
  ${PoutreBaseTestSubdirectory}/types.cpp
  ${PoutreBaseTestSubdirectory}/containerview.cpp
  ${PoutreBaseTestSubdirectory}/threading.cpp
  #${PoutreBaseTestSubdirectory}/tree.cpp
  )

add_executable(PoutreBaseTests ${PoutreBaseTestSRC})

target_link_libraries(PoutreBaseTests
  PUBLIC PoutreBase
  PRIVATE gtest
  PRIVATE gtest_main
  )
add_dependencies(PoutreBaseTests ${SIMD_LIB_DEP})

IF(BUILD_SHARED_LIBS)
  SET_TARGET_PROPERTIES(PoutreBaseTests PROPERTIES COMPILE_FLAGS -DJSON_DLL)
ENDIF(BUILD_SHARED_LIBS)

IF(POUTRE_CI)
  SET_TARGET_PROPERTIES(PoutreBaseTests PROPERTIES COMPILE_FLAGS -DPOUTRE_CI)
ENDIF()

add_custom_command(TARGET PoutreBaseTests POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:gtest> $<TARGET_FILE_DIR:PoutreBaseTests>)
add_custom_command(TARGET PoutreBaseTests POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:gtest_main> $<TARGET_FILE_DIR:PoutreBaseTests>)

add_test(NAME PoutreBaseTests-1
  COMMAND PoutreBaseTests)
set_property(TARGET PoutreBaseTests PROPERTY FOLDER "UnitTests/")

