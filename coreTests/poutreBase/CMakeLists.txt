
# Test project
set(PoutreBaseTestSubdirectory ${POUTRE_CORE_TEST_DIR}/poutreBase)
set(PoutreBaseTestSRC
    ${PoutreBaseTestSubdirectory}/main.hpp
    ${PoutreBaseTestSubdirectory}/main.cpp
	#dummy tests to validate proper import of third parties dependencies
    #${PoutreBaseTestSubdirectory}/nt2TablePlayground.cpp
	#${PoutreBaseTestSubdirectory}/eigenCleanImport.cpp
	${PoutreBaseTestSubdirectory}/boostSimdCleanImport.cpp
	${PoutreBaseTestSubdirectory}/chronos.cpp
    ${PoutreBaseTestSubdirectory}/jsonSerialize.cpp
    ${PoutreBaseTestSubdirectory}/registrar.cpp
	${PoutreBaseTestSubdirectory}/trace.cpp
)

add_executable(PoutreBaseTests ${PoutreBaseTestSRC})
set_target_properties(PoutreBaseTests PROPERTIES 
                      DEFINE_SYMBOL "JSON_DLL"
                      FOLDER "Core/")

target_link_libraries(PoutreBaseTests 
    PRIVATE PoutreBase
	PRIVATE jsoncpp
	INTERFACE Eigen
	INTERFACE BoostSimd
    PRIVATE ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
)
	
add_test(NAME PoutreBaseTests-1 
         COMMAND PoutreBaseTests)
set_property(TARGET PoutreBaseTests PROPERTY FOLDER "UnitTests/")
test_environment_setup(PoutreBaseTests-1)

