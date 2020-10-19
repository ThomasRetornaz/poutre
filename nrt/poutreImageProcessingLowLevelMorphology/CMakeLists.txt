
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
set_target_properties(PoutreIPLowLevelMorphologyTests PROPERTIES 
                      DEFINE_SYMBOL "JSON_DLL"
                      )

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

