cmake_minimum_required(VERSION 3.0)

#Adapted from https://github.com/dream3d/Dream3DSdkBuild/blob/master/Boost.cmake
message("Create ExternalProject google benchmark has thirdpartie lib")

#https://cmake.org/cmake/help/v3.0/module/ExternalProject.html
set(BENCH_PREFIX   "${CMAKE_BINARY_DIR}/google_benchmark")
set_property(DIRECTORY PROPERTY EP_BASE ${BENCH_PREFIX})
#TMP_DIR      = <base>/tmp/<name>
#STAMP_DIR    = <base>/Stamp/<name>
#DOWNLOAD_DIR = <base>/Download/<name>
#SOURCE_DIR   = <base>/Source/<name>
#BINARY_DIR   = <base>/Build/<name>
#INSTALL_DIR  = <base>/Install/<name>

set(BENCH_URL "${THIRD_PARTIES_PATH}/bench/benchmark.7z")

include(ExternalProject)
ExternalProject_Add(Benchmark
	#--Download step--------------
	URL "${BENCH_URL}"
	#--Update/Patch step----------
	UPDATE_COMMAND ""
	PATCH_COMMAND ""
	#--Configure step-------------
	CMAKE_ARGS
		-DBUILD_TESTING:BOOL=OFF
		-DBENCHMARK_ENABLE_TESTING:BOOL=OFF
		-DBENCHMARK_ENABLE_LTO:BOOL=OFF
		-DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
	#--Build step-----------------
	#--Install step-----------------
	INSTALL_COMMAND ""
)

ExternalProject_Get_Property(Benchmark TMP_DIR STAMP_DIR DOWNLOAD_DIR SOURCE_DIR BINARY_DIR INSTALL_DIR)
message("Build Benchmark src ${SOURCE_DIR} in ${BINARY_DIR}")
set_target_properties(Benchmark PROPERTIES FOLDER "3rdparty")

ExternalProject_Add_Step(
  Benchmark CopyToBin
  COMMAND ${CMAKE_COMMAND} -E copy_directory ${BINARY_DIR}/src/${ConfigurationName} ${CMAKE_BINARY_DIR}
  DEPENDEES install
)

set(GOOGLE_BENCHMARK_INCLUDE_DIRS "${SOURCE_DIR}/include")
set(GOOGLE_BENCHMARK_INCLUDE_DIRS ${GOOGLE_BENCHMARK_INCLUDE_DIRS}    PARENT_SCOPE)
set(GOOGLE_BENCHMARK         Benchmark      PARENT_SCOPE) #?

set_target_properties(Benchmark PROPERTIES FOLDER "3rdparty")	
