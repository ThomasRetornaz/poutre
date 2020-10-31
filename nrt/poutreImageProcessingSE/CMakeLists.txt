
include_directories(${GTEST_INCLUDE_DIR})
# Test project
set(PoutreImageProcessingSESubdirectory ${POUTRE_NRT_DIR}/poutreImageProcessingSE)
set(PoutreImageProcessingSETestSRC
    ${PoutreImageProcessingSESubdirectory}/main.hpp
    ${PoutreImageProcessingSESubdirectory}/main.cpp
)

add_executable(PoutreIPSETests ${PoutreImageProcessingSETestSRC})
IF(BUILD_SHARED_LIBS)
  SET_TARGET_PROPERTIES(PoutreIPSETests PROPERTIES COMPILE_FLAGS -DJSON_DLL)
ENDIF(BUILD_SHARED_LIBS)

IF(POUTRE_CI)
	SET_TARGET_PROPERTIES(PoutreIPSETests PROPERTIES COMPILE_FLAGS -DPOUTRE_CI)
ENDIF()


target_link_libraries(PoutreIPSETests
    PRIVATE gtest  
    PRIVATE gtest_main
    PoutreBase 
)
	
add_custom_command(TARGET PoutreIPSETests POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:gtest> $<TARGET_FILE_DIR:PoutreIPSETests>)
add_custom_command(TARGET PoutreIPSETests POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:gtest_main> $<TARGET_FILE_DIR:PoutreIPSETests>)
add_test(NAME PoutreIPSETests-1 
         COMMAND PoutreIPSETests)
set_property(TARGET PoutreIPSETests PROPERTY FOLDER "UnitTests/")

