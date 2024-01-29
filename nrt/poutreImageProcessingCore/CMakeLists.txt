
include_directories(${GTEST_INCLUDE_DIR})
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

if(MSVC)
    add_definitions(/wd4388) # in gtests '!=': signed/unsigned mismatch)
endif()

add_executable(PoutreIPCoreTests ${poutreIPCoreTestSRC})

IF(BUILD_SHARED_LIBS)
  SET_TARGET_PROPERTIES(PoutreIPCoreTests PROPERTIES COMPILE_FLAGS -DJSON_DLL)
ENDIF(BUILD_SHARED_LIBS)

IF(POUTRE_CI)
	SET_TARGET_PROPERTIES(PoutreIPCoreTests PROPERTIES COMPILE_FLAGS -DPOUTRE_CI)
ENDIF()

target_link_libraries(PoutreIPCoreTests
    PRIVATE gtest  
    PRIVATE gtest_main
    PUBLIC PoutreBase 
    PUBLIC PoutreIPCore
    PUBLIC PoutreIO
    PUBLIC ${OpenImageIO_LIBRARIES}
)



add_custom_command(TARGET PoutreIPCoreTests POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:gtest> $<TARGET_FILE_DIR:PoutreIPCoreTests>)
add_custom_command(TARGET PoutreIPCoreTests POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:gtest_main> $<TARGET_FILE_DIR:PoutreIPCoreTests>)

add_test(NAME PoutreIPCoreTests-1 
         COMMAND PoutreIPCoreTests)
set_property(TARGET PoutreIPCoreTests PROPERTY FOLDER "UnitTests/")

