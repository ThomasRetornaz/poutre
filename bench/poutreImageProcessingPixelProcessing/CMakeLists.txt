

include_directories(${POUTRE_SRC_DIR})
include_directories(${GOOGLE_BENCHMARK_INCLUDE_DIRS})

# Bench project
set(PoutreImageProcessingPixelOperationBenchSubdirectory ${POUTRE_BENCH_DIR}/poutreImageProcessingPixelProcessing)

set(PoutreImageProcessingPixelProcessingBenchSRC
  ${PoutreImageProcessingPixelOperationBenchSubdirectory}/main.hpp
  ${PoutreImageProcessingPixelOperationBenchSubdirectory}/main.cpp
  ${PoutreImageProcessingPixelOperationBenchSubdirectory}/arithmetics.cpp
  ${PoutreImageProcessingPixelOperationBenchSubdirectory}/compare.cpp
  )

#UGLY fix this later
LINK_DIRECTORIES(${CMAKE_BINARY_DIR}/$(ConfigurationName))

add_executable(PoutreIPPixelProcessingBenchs ${PoutreImageProcessingPixelProcessingBenchSRC})
set_target_properties(PoutreIPPixelProcessingBenchs PROPERTIES  
  FOLDER "Bench/")

IF(WIN32)
target_link_libraries(PoutreIPPixelProcessingBenchs 
  PUBLIC benchmark  
  PUBLIC BoostSimd
  PUBLIC shlwapi.lib
  #PUBLIC PoutreIPPixelProcessing
  PUBLIC ${Boost_SYSTEM_LIBRARY} 
  )
ELSE()
target_link_libraries(PoutreIPPixelProcessingBenchs 
  PUBLIC benchmark  
  PUBLIC BoostSimd
  #PUBLIC PoutreIPPixelProcessing 
  PUBLIC ${Boost_SYSTEM_LIBRARY}   
  )
ENDIF()

add_dependencies(PoutreIPPixelProcessingBenchs ${GOOGLE_BENCHMARK})
