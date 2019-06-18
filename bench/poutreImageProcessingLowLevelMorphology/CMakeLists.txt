

include_directories(${POUTRE_SRC_DIR})
include_directories(${GOOGLE_BENCHMARK_INCLUDE_DIRS})

# Bench project
set(PoutreImageProcessingLLMBenchSubdirectory ${POUTRE_BENCH_DIR}/poutreImageProcessingLowLevelMorphology)

set(PoutreImageProcessingLLMBenchSRC
  ${PoutreImageProcessingLLMBenchSubdirectory}/main.hpp
  ${PoutreImageProcessingLLMBenchSubdirectory}/main.cpp
  ${PoutreImageProcessingLLMBenchSubdirectory}/erodil.cpp
  )

#UGLY fix this later
LINK_DIRECTORIES(${CMAKE_BINARY_DIR}/$(ConfigurationName))

add_executable(PoutreIPLowLevelMorphologyBenchs ${PoutreImageProcessingLLMBenchSRC})
set_target_properties(PoutreIPLowLevelMorphologyBenchs PROPERTIES  
  FOLDER "Bench/")

IF(WIN32)
target_link_libraries(PoutreIPLowLevelMorphologyBenchs  
  PUBLIC PoutreBase
  PUBLIC benchmark  
  #INTERFACE ${SIMD_LIB}
  PUBLIC shlwapi.lib
  #PUBLIC PoutreIPPixelProcessing
  PUBLIC ${Boost_SYSTEM_LIBRARY}
  )
ELSE()
target_link_libraries(PoutreIPPixelProcessingBenchs 
  PUBLIC PoutreBase
  PUBLIC benchmark  
  #INTERFACE ${SIMD_LIB}
  #PUBLIC PoutreIPPixelProcessing 
  PUBLIC ${Boost_SYSTEM_LIBRARY}   
  )
ENDIF()

add_dependencies(PoutreIPLowLevelMorphologyBenchs ${GOOGLE_BENCHMARK})
