
# Test project
set(PoutreImageProcessingLLMSubdirectory ${POUTRE_NRT_DIR}/poutreImageProcessingLowLevelMorphology)
set(PoutreImageProcessingLLMTestSRC
    ${PoutreImageProcessingLLMSubdirectory}/main.hpp
    ${PoutreImageProcessingLLMSubdirectory}/main.cpp
	${PoutreImageProcessingLLMSubdirectory}/ImageErodeDilate.cpp
    #${PoutreImageProcessingLLMSubdirectory}/ImageErodeDilateLin.cpp

)

add_executable(PoutreIPLowLevelMorphologyTests ${PoutreImageProcessingLLMTestSRC})
set_target_properties(PoutreIPLowLevelMorphologyTests PROPERTIES 
                      DEFINE_SYMBOL "JSON_DLL"
                      )

target_link_libraries(PoutreIPLowLevelMorphologyTests
    PUBLIC PoutreBase 
    PUBLIC PoutreIPCore
	PUBLIC PoutreIO
    PUBLIC PoutreIPPixelProcessing
    PUBLIC ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
)
	
add_test(NAME PoutreIPLowLevelMorphologyTests-1 
         COMMAND PoutreIPLowLevelMorphologyTests)
set_property(TARGET PoutreIPLowLevelMorphologyTests PROPERTY FOLDER "UnitTests/")
test_environment_setup(PoutreIPLowLevelMorphologyTests-1)

