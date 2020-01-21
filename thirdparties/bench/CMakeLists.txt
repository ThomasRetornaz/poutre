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
    #UPDATE_COMMAND ""
    #BUILD_COMMAND     ""
    #INSTALL_COMMAND   ""
    TEST_COMMAND      ""
  ) 

# Add googlebenchmark directly to our build. This defines
add_subdirectory(${CMAKE_CURRENT_BINARY_DIR}/googlebenchmark-src
    ${CMAKE_CURRENT_BINARY_DIR}/googlebenchmark-build)

set(GBENCHMARK_INCLUDE_DIRS "${CMAKE_CURRENT_BINARY_DIR}/src/googlebenchmark/include" PARENT_SCOPE)
set(GOOGLE_BENCHMARK  benchmark_main benchmark PARENT_SCOPE)

set_target_properties(googlebenchmark PROPERTIES FOLDER "3rdparty")	
