
# Test project
set(PoutreNeuralNetsTestSubdirectory ${POUTRE_CORE_TEST_DIR}/poutreNeuralNets)
set(PoutreNeuralNetsTestSRC
    ${PoutreNeuralNetsTestSubdirectory}/main.hpp
    ${PoutreNeuralNetsTestSubdirectory}/main.cpp
)

add_executable(PoutreNeuralNetsTests ${PoutreNeuralNetsTestSRC})
set_target_properties(PoutreNeuralNetsTests PROPERTIES 
                      DEFINE_SYMBOL "JSON_DLL"
                      FOLDER "Core/")

target_link_libraries(PoutreNeuralNetsTests
    PoutreBase 
    PoutreNeuralNets
    jsoncpp	
    ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
)
	
add_test(NAME PoutreNeuralNetsTests-1 
         COMMAND PoutreNeuralNetsTests)
set_property(TARGET PoutreNeuralNetsTests PROPERTY FOLDER "UnitTests/")
test_environment_setup(PoutreNeuralNetsTests-1)

