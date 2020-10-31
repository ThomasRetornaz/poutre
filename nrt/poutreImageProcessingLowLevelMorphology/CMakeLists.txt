
include_directories(${GTEST_INCLUDE_DIR})
# Test project
set(PoutreImageProcessingLLMSubdirectory ${POUTRE_NRT_DIR}/poutreImageProcessingLowLevelMorphology)
set(PoutreImageProcessingLLMTestSRC
    ${PoutreImageProcessingLLMSubdirectory}/main.hpp
    ${PoutreImageProcessingLLMSubdirectory}/main.cpp
	${PoutreImageProcessingLLMSubdirectory}/ImageErodeDilate.cpp
    #${PoutreImageProcessingLLMSubdirectory}/ImageErodeDilateLin.cpp

)

add_executable(PoutreIPLowLevelMorphologyTests ${PoutreImageProcessingLLMTestSRC})

IF(BUILD_SHARED_LIBS)
  SET_TARGET_PROPERTIES(PoutreIPLowLevelMorphologyTests PROPERTIES COMPILE_FLAGS -DJSON_DLL)
ENDIF(BUILD_SHARED_LIBS)

IF(POUTRE_CI)
	SET_TARGET_PROPERTIES(PoutreIPLowLevelMorphologyTests PROPERTIES COMPILE_FLAGS -DPOUTRE_CI)
ENDIF()

target_link_libraries(PoutreIPLowLevelMorphologyTests
    PUBLIC PoutreBase 
    PUBLIC PoutreIPCore
	PUBLIC PoutreIO
    PUBLIC PoutreIPPixelProcessing
    PUBLIC ${OpenImageIO_LIBRARIES}
    PRIVATE gtest  
    PRIVATE gtest_main
)


add_custom_command(TARGET PoutreIPLowLevelMorphologyTests POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:gtest> $<TARGET_FILE_DIR:PoutreIPLowLevelMorphologyTests>)
add_custom_command(TARGET PoutreIPLowLevelMorphologyTests POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:gtest_main> $<TARGET_FILE_DIR:PoutreIPLowLevelMorphologyTests>)

add_test(NAME PoutreIPLowLevelMorphologyTests-1 
         COMMAND PoutreIPLowLevelMorphologyTests)
set_property(TARGET PoutreIPLowLevelMorphologyTests PROPERTY FOLDER "UnitTests/")

