cmake_minimum_required(VERSION 3.0)
project(googlebenchmark)

#Adapted from https://github.com/dream3d/Dream3DSdkBuild/blob/master/Boost.cmake
message("Create ExternalProject google benchmark has thirdpartie lib")
set(BENCH_FLAG
 -DBENCHMARK_ENABLE_TESTING:BOOL=OFF
 -DBENCHMARK_ENABLE_GTEST_TESTS:BOOL=OFF
 -DBENCHMARK_ENABLE_LTO:BOOL=OFF
 -DBENCHMARK_ENABLE_EXCEPTIONS:BOOL=ON
 )

include(ExternalProject)
ExternalProject_Add(googlebenchmark
    GIT_REPOSITORY https://github.com/google/benchmark.git
    GIT_TAG v1.5.0
    PREFIX ${CMAKE_CURRENT_BINARY_DIR}
    SOURCE_DIR        "${CMAKE_CURRENT_BINARY_DIR}/googlebenchmark-src"
    BINARY_DIR        "${CMAKE_CURRENT_BINARY_DIR}/googlebenchmark-build"
    #--Configure step-------------
    CMAKE_ARGS += ${BENCH_FLAG}
    UPDATE_COMMAND ""
    BUILD_COMMAND     --build . --config Release
    INSTALL_COMMAND   ""
    TEST_COMMAND      ""
  ) 

ExternalProject_Get_Property(googlebenchmark TMP_DIR STAMP_DIR DOWNLOAD_DIR SOURCE_DIR BINARY_DIR INSTALL_DIR)   
message("Build googlebenchmark src ${SOURCE_DIR} in ${BINARY_DIR}")

set(GOOGLE_BENCHMARK_INCLUDE_DIRS "${SOURCE_DIR}/include")
set(GOOGLE_BENCHMARK_INCLUDE_DIRS ${GOOGLE_BENCHMARK_INCLUDE_DIRS}    PARENT_SCOPE)
add_library(benchmark STATIC IMPORTED GLOBAL) #MARK AS IMPORTED AND GLOBAL SCOPE

if(WIN32)
set_target_properties(benchmark PROPERTIES IMPORTED_LOCATION ${BINARY_DIR}/src/Release/benchmark.lib)
else()
set_target_properties(benchmark PROPERTIES IMPORTED_LOCATION ${BINARY_DIR}/src/libbenchmark.a)
endif()
set(GOOGLE_BENCHMARK benchmark PARENT_SCOPE)