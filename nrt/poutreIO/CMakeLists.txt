
include_directories(${GTEST_INCLUDE_DIR})
# Test project
set(PoutreIOTestSubdirectory ${POUTRE_NRT_DIR}/poutreIO)

set(PoutreIOTestSRC
    ${PoutreIOTestSubdirectory}/main.hpp
    ${PoutreIOTestSubdirectory}/main.cpp
    ${PoutreIOTestSubdirectory}/iostring.cpp
    ${PoutreIOTestSubdirectory}/buffer.cpp   
)

if(POUTRE_USE_HDF5)
list(APPEND PoutreIOTestSRC ${PoutreIOTestSubdirectory}/hdf5.cpp)
endif()

if(POUTRE_USE_OIIO)
list(APPEND PoutreIOTestSRC ${PoutreIOTestSubdirectory}/oiio.cpp)
endif()

set(POUTREIO_NRT_DATAS "\"${CMAKE_CURRENT_SOURCE_DIR}/Datas\"")
add_definitions(-DPOUTREIO_NRT_DATAS=${POUTREIO_NRT_DATAS})

add_executable(PoutreIOTests ${PoutreIOTestSRC})

IF(BUILD_SHARED_LIBS)
  SET_TARGET_PROPERTIES(PoutreIOTests PROPERTIES COMPILE_FLAGS -DJSON_DLL)
ENDIF(BUILD_SHARED_LIBS)

IF(POUTRE_CI)
	SET_TARGET_PROPERTIES(PoutreIOTests PROPERTIES COMPILE_FLAGS -DPOUTRE_CI)
ENDIF()


set(PoutreIOTests_LINK_LIBRAIRIES
  PUBLIC PoutreBase 
  PUBLIC PoutreIPCore 
  PUBLIC PoutreIO
  PRIVATE gtest  
  PRIVATE gtest_main
)

if(POUTRE_USE_OIIO)
  list (APPEND PoutreIOTests_LINK_LIBRAIRIES PUBLIC ${OpenImageIO_LIBRARIES})
endif()

if(POUTRE_USE_HDF5)
  list (APPEND PoutreIOTests_LINK_LIBRAIRIES PUBLIC ${HDF5_LIBRARIES})
endif()

target_link_libraries(PoutreIOTests ${PoutreIOTests_LINK_LIBRAIRIES}    
)
target_include_directories(PoutreIOTests SYSTEM BEFORE
PUBLIC ${OPENEXR_INCLUDE_DIR} 
PUBLIC ${OIIO_INCLUDE_DIR}
PUBLIC ${HIGH_FIVE_INCLUDE_DIR}
PUBLIC ${HDF5_INCLUDE_DIRS}
PUBLIC ${CMAKE_BINARY_DIR}/include
PUBLIC ${CMAKE_BINARY_DIR}
)
add_custom_command(TARGET PoutreIOTests POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:gtest> $<TARGET_FILE_DIR:PoutreIOTests>)
add_custom_command(TARGET PoutreIOTests POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:gtest_main> $<TARGET_FILE_DIR:PoutreIOTests>)

add_test(NAME PoutreIOTests-1 
         COMMAND PoutreIOTests)
set_property(TARGET PoutreIOTests PROPERTY FOLDER "UnitTests/")

