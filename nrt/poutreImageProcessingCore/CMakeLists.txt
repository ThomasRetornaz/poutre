
# Test project
set(poutreIPCoreTestSubdirectory ${POUTRE_NRT_DIR}/poutreImageProcessingCore)
set(poutreIPCoreTestSRC
    ${poutreIPCoreTestSubdirectory}/main.hpp
    ${poutreIPCoreTestSubdirectory}/main.cpp
    ${poutreIPCoreTestSubdirectory}/ImageProcessingType.cpp
    ${poutreIPCoreTestSubdirectory}/ImageContainer.cpp
    ${poutreIPCoreTestSubdirectory}/ImageCopieConvert.cpp
	${poutreIPCoreTestSubdirectory}/PriorityQueue.cpp
)

add_executable(PoutreIPCoreTests ${poutreIPCoreTestSRC})

target_link_libraries(PoutreIPCoreTests
    PUBLIC PoutreBase 
    PUBLIC PoutreIPCore
    PUBLIC PoutreIO
    PUBLIC ${OpenImageIO_LIBRARIES}
    PUBLIC ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
)
	
add_test(NAME PoutreIPCoreTests-1 
         COMMAND PoutreIPCoreTests)
set_property(TARGET PoutreIPCoreTests PROPERTY FOLDER "UnitTests/")
test_environment_setup(PoutreIPCoreTests-1)

