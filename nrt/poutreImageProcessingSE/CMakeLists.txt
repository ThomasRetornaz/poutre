
include_directories(${GTEST_INCLUDE_DIR})
# Test project
set(PoutreImageProcessingSESubdirectory ${POUTRE_NRT_DIR}/poutreImageProcessingSE)
set(PoutreImageProcessingSETestSRC
    ${PoutreImageProcessingSESubdirectory}/main.hpp
    ${PoutreImageProcessingSESubdirectory}/main.cpp
)

add_executable(PoutreIPSETests ${PoutreImageProcessingSETestSRC})
set_target_properties(PoutreIPSETests PROPERTIES 
                      DEFINE_SYMBOL "JSON_DLL"
					 )

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
test_environment_setup(PoutreIPSETests-1)

