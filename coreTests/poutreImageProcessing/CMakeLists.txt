
# Test project
set(poutreImageProcessingTestSubdirectory ${POUTRE_CORE_TEST_DIR}/poutreImageProcessing)
set(poutreImageProcessingTestSRC
    ${poutreImageProcessingTestSubdirectory}/main.hpp
    ${poutreImageProcessingTestSubdirectory}/main.cpp
	${poutreImageProcessingTestSubdirectory}/ImageContainer.cpp
	${poutreImageProcessingTestSubdirectory}/ImageCopieConvert.cpp
)

add_executable(poutreImageProcessingTests ${poutreImageProcessingTestSRC})
set_target_properties(poutreImageProcessingTests PROPERTIES 
                      DEFINE_SYMBOL "JSON_DLL"
                      FOLDER "Core/")

target_link_libraries(poutreImageProcessingTests
    PoutreBase 
    PoutreImageProcessing
    jsoncpp	
    ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
)
	
add_test(NAME poutreImageProcessingTests-1 
         COMMAND poutreImageProcessingTests)
set_property(TARGET poutreImageProcessingTests PROPERTY FOLDER "UnitTests/")
test_environment_setup(poutreImageProcessingTests-1)

