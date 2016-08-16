
# Test project
set(poutreIPPixelOperationTestSubdirectory ${POUTRE_NRT_DIR}/poutreImageProcessingPixelOperation)
set(poutreIPPixelOperationTestSRC
    ${poutreIPPixelOperationTestSubdirectory}/main.hpp
    ${poutreIPPixelOperationTestSubdirectory}/main.cpp
	${poutreIPPixelOperationTestSubdirectory}/ImageAryOp.cpp
	${poutreIPPixelOperationTestSubdirectory}/ImageCompareOp.cpp
)

add_executable(PoutreIPPixelOperationsTests ${poutreIPPixelOperationTestSRC})

target_link_libraries(PoutreIPPixelOperationsTests
    PoutreBase 
    PoutreIPCore
	#PoutreIPPixelProcessing
    PoutreIO
    ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
)
	
add_test(NAME PoutreIPPixelOperationsTests-1 
         COMMAND PoutreIPPixelOperationsTests)
set_property(TARGET PoutreIPPixelOperationsTests PROPERTY FOLDER "UnitTests/")
test_environment_setup(PoutreIPPixelOperationsTests-1)

