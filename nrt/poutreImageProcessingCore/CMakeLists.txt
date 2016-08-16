
# Test project
set(poutreIPCoreTestSubdirectory ${POUTRE_NRT_DIR}/poutreImageProcessingCore)
set(poutreIPCoreTestSRC
    ${poutreIPCoreTestSubdirectory}/main.hpp
    ${poutreIPCoreTestSubdirectory}/main.cpp
    ${poutreIPCoreTestSubdirectory}/ImageProcessingType.cpp
    ${poutreIPCoreTestSubdirectory}/ImageContainer.cpp
    ${poutreIPCoreTestSubdirectory}/ImageCopieConvert.cpp
)

add_executable(poutreIPCoreTests ${poutreIPCoreTestSRC})

target_link_libraries(poutreIPCoreTests
    PoutreBase 
    PoutreIPCore
    PoutreIO
    ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
)
	
add_test(NAME poutreIPCoreTests-1 
         COMMAND poutreIPCoreTests)
set_property(TARGET poutreIPCoreTests PROPERTY FOLDER "UnitTests/")
test_environment_setup(poutreIPCoreTests-1)

