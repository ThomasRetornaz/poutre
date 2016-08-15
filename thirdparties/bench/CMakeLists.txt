cmake_minimum_required(VERSION 3.0)

message("Create ExternalProject google_benchmark has thirdpartie lib")


#https://cmake.org/cmake/help/v3.0/module/ExternalProject.html
set(BENCH_PREFIX   "${CMAKE_BINARY_DIR}/google_benchmark")
set_property(DIRECTORY PROPERTY EP_BASE ${BENCH_PREFIX})
#TMP_DIR      = <base>/tmp/<name>
#STAMP_DIR    = <base>/Stamp/<name>
#DOWNLOAD_DIR = <base>/Download/<name>
#SOURCE_DIR   = <base>/Source/<name>
#BINARY_DIR   = <base>/Build/<name>
#INSTALL_DIR  = <base>/Install/<name>

include(ExternalProject)
ExternalProject_Add(
 	Benchmark
	#--Download step--------------
	#GIT_REPOSITORY    "https://github.com/google/benchmark.git"
	#GIT_TAG           "master"
	URL "${THIRD_PARTIES_PATH}/bench/benchmark.7z"
	#--Update/Patch step----------
	UPDATE_COMMAND ""
  	PATCH_COMMAND ""
	#--Configure step-------------
	CMAKE_ARGS
		#-DBUILD_SHARED_LIBS:BOOL=${BUILD_SHARED_LIBS}
		#-DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
    	-DBENCHMARK_ENABLE_TESTING:BOOL=OFF
		-DBENCHMARK_ENABLE_LTO:BOOL=OFF
		-DCMAKE_INSTALL_PREFIX=${CMAKE_BINARY_DIR}/google_benchmark
	#--Build step-----------------
	#--Install step-----------------
	#INSTALL_COMMAND ""
	#DEPENDS 
)

ExternalProject_Get_Property(Benchmark TMP_DIR STAMP_DIR DOWNLOAD_DIR SOURCE_DIR BINARY_DIR INSTALL_DIR)
message("Build google_benchmark src ${SOURCE_DIR} in ${BINARY_DIR}")


##grab auto install #Not work as expected don't no why 
#ExternalProject_Add_Step(
#  google_benchmark CopyToBin
#  COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_BINARY_DIR}/google_benchmark/bin ${CMAKE_BINARY_DIR}/${ConfigurationName}
#  COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_BINARY_DIR}/google_benchmark/lib ${CMAKE_BINARY_DIR}/${ConfigurationName}
#  DEPENDEES install
#)
#so swicth to old scool custom command

add_custom_command(TARGET Benchmark POST_BUILD
   COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/$(ConfigurationName) #just in case
   COMMAND ${CMAKE_COMMAND}  -E copy_directory  ${CMAKE_BINARY_DIR}/GOOGLE_BENCHMARK/bin ${CMAKE_BINARY_DIR}/$(ConfigurationName)
   COMMAND ${CMAKE_COMMAND}  -E copy_directory  ${CMAKE_BINARY_DIR}/GOOGLE_BENCHMARK/lib ${CMAKE_BINARY_DIR}/$(ConfigurationName)
)

set(GOOGLE_BENCHMARK_INCLUDE_DIRS "${CMAKE_BINARY_DIR}/google_benchmark/include")
set(GOOGLE_BENCHMARK_INCLUDE_DIRS ${GOOGLE_BENCHMARK_INCLUDE_DIRS}    PARENT_SCOPE)
set(GOOGLE_BENCHMARK         Benchmark      PARENT_SCOPE) #?

set_target_properties(Benchmark PROPERTIES FOLDER "3rdparty")	
