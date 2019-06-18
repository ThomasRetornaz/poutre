
# Test project
set(PoutreImageProcessingLLMSubdirectory ${POUTRE_NRT_DIR}/poutreImageProcessingLowLevelMorphology)
set(PoutreImageProcessingLLMTestSRC
    ${PoutreImageProcessingLLMSubdirectory}/main.hpp
    ${PoutreImageProcessingLLMSubdirectory}/main.cpp
	${PoutreImageProcessingLLMSubdirectory}/ImageErodeDilate.cpp
)

add_executable(PoutreIPLLMTests ${PoutreImageProcessingLLMTestSRC})
set_target_properties(PoutreIPLLMTests PROPERTIES 
                      DEFINE_SYMBOL "JSON_DLL"
                      )

target_link_libraries(PoutreIPLLMTests
    PUBLIC PoutreBase 
    PUBLIC PoutreIPCore
	PUBLIC PoutreIO
    PUBLIC PoutreIPPixelProcessing
    PUBLIC ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
)
	
add_test(NAME PoutreIPLLMTests-1 
         COMMAND PoutreIPLLMTests)
set_property(TARGET PoutreIPLLMTests PROPERTY FOLDER "UnitTests/")
test_environment_setup(PoutreIPLLMTests-1)

