
include_directories(${GTEST_INCLUDE_DIR})

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

IF(BUILD_SHARED_LIBS)
  SET_TARGET_PROPERTIES(PoutreIPPixelProcessingTests PROPERTIES COMPILE_FLAGS -DJSON_DLL)
ENDIF(BUILD_SHARED_LIBS)

IF(POUTRE_CI)
	SET_TARGET_PROPERTIES(PoutreIPPixelProcessingTests PROPERTIES COMPILE_FLAGS -DPOUTRE_CI)
ENDIF()

target_link_libraries(PoutreIPPixelProcessingTests
    PRIVATE gtest  
    PRIVATE gtest_main
    PUBLIC PoutreBase 
    PUBLIC PoutreIPCore
	PUBLIC PoutreIO
    PUBLIC PoutreIPPixelProcessing
    PUBLIC ${OpenImageIO_LIBRARIES}
)
	
add_custom_command(TARGET PoutreIPPixelProcessingTests POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:gtest> $<TARGET_FILE_DIR:PoutreIPPixelProcessingTests>)
add_custom_command(TARGET PoutreIPPixelProcessingTests POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:gtest_main> $<TARGET_FILE_DIR:PoutreIPPixelProcessingTests>)

add_test(NAME PoutreIPPixelProcessingTests-1 
         COMMAND PoutreIPPixelProcessingTests)
set_property(TARGET PoutreIPPixelProcessingTests PROPERTY FOLDER "UnitTests/")

