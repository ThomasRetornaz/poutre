
# Test project
set(poutreImageProcessingTestSubdirectory ${POUTRE_NRT_DIR}/poutreImageProcessing)
set(poutreImageProcessingTestSRC
    ${poutreImageProcessingTestSubdirectory}/main.hpp
    ${poutreImageProcessingTestSubdirectory}/main.cpp
    ${poutreImageProcessingTestSubdirectory}/ImageProcessingType.cpp
    ${poutreImageProcessingTestSubdirectory}/ImageContainer.cpp
    ${poutreImageProcessingTestSubdirectory}/ImageCopieConvert.cpp
	${poutreImageProcessingTestSubdirectory}/ImageAryOp.cpp
)

add_executable(PoutreImageProcessingTests ${poutreImageProcessingTestSRC})
set_target_properties(PoutreImageProcessingTests PROPERTIES 
                      DEFINE_SYMBOL "JSON_DLL"
                      FOLDER "Core/")

target_link_libraries(PoutreImageProcessingTests
    PoutreBase 
    PoutreImageProcessing
    jsoncpp	
    ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
)
	
add_test(NAME PoutreImageProcessingTests-1 
         COMMAND PoutreImageProcessingTests)
set_property(TARGET PoutreImageProcessingTests PROPERTY FOLDER "UnitTests/")
test_environment_setup(PoutreImageProcessingTests-1)

