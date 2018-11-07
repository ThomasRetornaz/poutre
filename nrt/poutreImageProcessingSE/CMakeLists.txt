
# Test project
set(PoutreImageProcessingSESubdirectory ${POUTRE_NRT_DIR}/poutreImageProcessingSE)
set(PoutreImageProcessingSETestSRC
    ${PoutreImageProcessingSESubdirectory}/main.hpp
    ${PoutreImageProcessingSESubdirectory}/main.cpp
)

add_executable(PoutreIPSETests ${PoutreImageProcessingSETestSRC})
set_target_properties(PoutreIPSETests PROPERTIES 
                      DEFINE_SYMBOL "JSON_DLL"
                      FOLDER "Core/")

target_link_libraries(PoutreIPSETests
    PoutreBase 
    ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
)
	
add_test(NAME PoutreIPSETests-1 
         COMMAND PoutreIPSETests)
set_property(TARGET PoutreIPSETests PROPERTY FOLDER "UnitTests/")
test_environment_setup(PoutreIPSETests-1)

