

include_directories(${POUTRE_SRC_DIR})
include_directories(${GOOGLE_BENCHMARK_INCLUDE_DIRS})

# Bench project
set(PoutreImageProcessingPixelOperationBenchSubdirectory ${POUTRE_BENCH_DIR}/poutreImageProcessingPixelProcessing)

set(PoutreImageProcessingPixelProcessingBenchSRC
  ${PoutreImageProcessingPixelOperationBenchSubdirectory}/main.hpp
  ${PoutreImageProcessingPixelOperationBenchSubdirectory}/main.cpp
  ${PoutreImageProcessingPixelOperationBenchSubdirectory}/arithmetics.cpp
  ${PoutreImageProcessingPixelOperationBenchSubdirectory}/compare.cpp
  ${PoutreImageProcessingPixelOperationBenchSubdirectory}/transpose.cpp
  )

#UGLY fix this later
if(WIN32)
  LINK_DIRECTORIES(${CMAKE_BINARY_DIR}/$(ConfigurationName))
endif()  

add_executable(PoutreIPPixelProcessingBenchs ${PoutreImageProcessingPixelProcessingBenchSRC})
set_target_properties(PoutreIPPixelProcessingBenchs PROPERTIES  
  FOLDER "Bench/")

IF(WIN32)
target_link_libraries(PoutreIPPixelProcessingBenchs 
  PUBLIC PoutreBase
  PUBLIC benchmark  
  #INTERFACE ${SIMD_LIB}
  PUBLIC shlwapi.lib
  PUBLIC PoutreIPPixelProcessing
  PUBLIC ${Boost_SYSTEM_LIBRARY}
  )
ELSE()
target_link_libraries(PoutreIPPixelProcessingBenchs 
  PUBLIC PoutreBase
  PUBLIC benchmark  
  #INTERFACE ${SIMD_LIB}
  PUBLIC PoutreIPPixelProcessing 
  PUBLIC ${Boost_SYSTEM_LIBRARY}   
  )
ENDIF()

add_dependencies(PoutreIPPixelProcessingBenchs ${GOOGLE_BENCHMARK})
