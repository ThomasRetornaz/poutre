cmake_minimum_required(VERSION 3.0)

#Adapted from https://github.com/dream3d/Dream3DSdkBuild/blob/master/Boost.cmake
message("Create ExternalProject google benchmark has thirdpartie lib")

include(ExternalProject)
# clone approach
find_package(git REQUIRED)
ExternalProject_Add(googlebenchmark
  GIT_REPOSITORY https://github.com/google/benchmark.git
  GIT_TAG master
  PREFIX ${CMAKE_CURRENT_BINARY_DIR}
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

ExternalProject_Get_Property(googlebenchmark TMP_DIR STAMP_DIR DOWNLOAD_DIR SOURCE_DIR BINARY_DIR INSTALL_DIR)
message("Build googlebenchmark src ${SOURCE_DIR} in ${BINARY_DIR}")

ExternalProject_Add_Step(
  googlebenchmark CopyToBin
  COMMAND ${CMAKE_COMMAND} -E copy_directory ${BINARY_DIR}/src/${ConfigurationName} ${CMAKE_BINARY_DIR}
  DEPENDEES install
)

set(GOOGLE_BENCHMARK_INCLUDE_DIRS "${SOURCE_DIR}/include")
set(GOOGLE_BENCHMARK_INCLUDE_DIRS ${GOOGLE_BENCHMARK_INCLUDE_DIRS}    PARENT_SCOPE)
set(GOOGLE_BENCHMARK         googlebenchmark      PARENT_SCOPE) #?

set_target_properties(googlebenchmark PROPERTIES FOLDER "3rdparty")	
