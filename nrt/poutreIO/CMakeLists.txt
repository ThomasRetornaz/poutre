
include_directories(${GTEST_INCLUDE_DIR})
# Test project
set(PoutreIOTestSubdirectory ${POUTRE_NRT_DIR}/poutreIO)
set(PoutreIOTestSRC
    ${PoutreIOTestSubdirectory}/main.hpp
    ${PoutreIOTestSubdirectory}/main.cpp
    ${PoutreIOTestSubdirectory}/iostring.cpp
    ${PoutreIOTestSubdirectory}/oiio.cpp
    ${PoutreIOTestSubdirectory}/buffer.cpp
    ${PoutreIOTestSubdirectory}/hdf5.cpp
)

INCLUDE_DIRECTORIES(BEFORE SYSTEM
  ${OPENEXR_INCLUDE_DIR} 
  ${OIIO_INCLUDE_DIR}
  ${HIGH_FIVE_INCLUDE_DIR}
  ${HDF5_INCLUDE_DIRS}
  ${CMAKE_BINARY_DIR}/include
  ${CMAKE_BINARY_DIR}
)

set(POUTREIO_NRT_DATAS "\"${CMAKE_CURRENT_SOURCE_DIR}/Datas\"")
add_definitions(-DPOUTREIO_NRT_DATAS=${POUTREIO_NRT_DATAS})

add_executable(PoutreIOTests ${PoutreIOTestSRC})

IF(BUILD_SHARED_LIBS)
  SET_TARGET_PROPERTIES(PoutreIOTests PROPERTIES COMPILE_FLAGS -DJSON_DLL)
ENDIF(BUILD_SHARED_LIBS)

IF(POUTRE_CI)
	SET_TARGET_PROPERTIES(PoutreIOTests PROPERTIES COMPILE_FLAGS -DPOUTRE_CI)
ENDIF()

target_link_libraries(PoutreIOTests
    PUBLIC PoutreBase 
    PUBLIC PoutreIPCore 
    PUBLIC PoutreIO
    PUBLIC ${OpenImageIO_LIBRARIES}
    PRIVATE gtest  
    PRIVATE gtest_main
)


add_custom_command(TARGET PoutreIOTests POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:gtest> $<TARGET_FILE_DIR:PoutreIOTests>)
add_custom_command(TARGET PoutreIOTests POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:gtest_main> $<TARGET_FILE_DIR:PoutreIOTests>)

add_test(NAME PoutreIOTests-1 
         COMMAND PoutreIOTests)
set_property(TARGET PoutreIOTests PROPERTY FOLDER "UnitTests/")

