
# Test project
set(poutreIPPixelOperationTestSubdirectory ${POUTRE_NRT_DIR}/poutreImageProcessingPixelProcessing)
set(poutreIPPixelOperationTestSRC
    ${poutreIPPixelOperationTestSubdirectory}/main.hpp
    ${poutreIPPixelOperationTestSubdirectory}/main.cpp
	${poutreIPPixelOperationTestSubdirectory}/ImageAryOp.cpp
	${poutreIPPixelOperationTestSubdirectory}/ImageCompareOp.cpp
	${poutreIPPixelOperationTestSubdirectory}/ImageTransposeOp.cpp
)

add_executable(PoutreIPPixelProcessingTests ${poutreIPPixelOperationTestSRC})

target_link_libraries(PoutreIPPixelProcessingTests
    PUBLIC PoutreBase 
    PUBLIC PoutreIPCore
	PUBLIC PoutreIO
    PUBLIC PoutreIPPixelProcessing
    PUBLIC ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
)
	
add_test(NAME PoutreIPPixelProcessingTests-1 
         COMMAND PoutreIPPixelProcessingTests)
set_property(TARGET PoutreIPPixelProcessingTests PROPERTY FOLDER "UnitTests/")
test_environment_setup(PoutreIPPixelProcessingTests-1)

