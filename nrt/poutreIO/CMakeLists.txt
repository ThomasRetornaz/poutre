
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
set_target_properties(PoutreIOTests PROPERTIES 
                      DEFINE_SYMBOL "JSON_DLL")

target_link_libraries(PoutreIOTests
    PUBLIC PoutreBase 
    PUBLIC PoutreIPCore 
    PUBLIC PoutreIO
    PUBLIC ${OpenImageIO_LIBRARIES}
    ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
)

#target_include_directories(PoutreIOTests
#    ${OPENEXR_INCLUDE_DIR} 
#	${OIIO_INCLUDE_DIR}
#)	

add_test(NAME PoutreIOTests-1 
         COMMAND PoutreIOTests)
set_property(TARGET PoutreIOTests PROPERTY FOLDER "UnitTests/")
test_environment_setup(PoutreIOTests-1)

