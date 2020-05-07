
# Test project
set(PoutreIOTestSubdirectory ${POUTRE_NRT_DIR}/poutreIO)
set(PoutreIOTestSRC
    ${PoutreIOTestSubdirectory}/main.hpp
    ${PoutreIOTestSubdirectory}/main.cpp
    ${PoutreIOTestSubdirectory}/iostring.cpp
	${PoutreIOTestSubdirectory}/png.cpp
	${PoutreIOTestSubdirectory}/buffer.cpp
)

add_executable(PoutreIOTests ${PoutreIOTestSRC})
set_target_properties(PoutreIOTests PROPERTIES 
                      DEFINE_SYMBOL "JSON_DLL")

target_link_libraries(PoutreIOTests
    PUBLIC PoutreBase 
    PUBLIC PoutreIPCore 
    PUBLIC PoutreIO
    PUBLIC ${OpenImageIO_LIBRARIES}
    ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
)
	
add_test(NAME PoutreIOTests-1 
         COMMAND PoutreIOTests)
set_property(TARGET PoutreIOTests PROPERTY FOLDER "UnitTests/")
test_environment_setup(PoutreIOTests-1)

