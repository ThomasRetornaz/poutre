

include_directories(${POUTRE_SRC_DIR})
include_directories(${GOOGLE_BENCHMARK_INCLUDE_DIRS})

# Bench project
set(PoutreImageProcessingLLMBenchSubdirectory ${POUTRE_BENCH_DIR}/poutreImageProcessingLowLevelMorphology)

set(PoutreImageProcessingLLMBenchSRC
  ${PoutreImageProcessingLLMBenchSubdirectory}/main.hpp
  ${PoutreImageProcessingLLMBenchSubdirectory}/main.cpp
  ${PoutreImageProcessingLLMBenchSubdirectory}/erodil.cpp
  )

add_executable(PoutreIPLowLevelMorphologyBenchs ${PoutreImageProcessingLLMBenchSRC})
set_target_properties(PoutreIPLowLevelMorphologyBenchs PROPERTIES  
  FOLDER "Bench/")

IF(WIN32)
target_link_libraries(PoutreIPLowLevelMorphologyBenchs 
  PUBLIC benchmark  
  PUBLIC benchmark_main 
  PUBLIC shlwapi.lib
  PUBLIC PoutreIPCore
  )
ELSE()
target_link_libraries(PoutreIPLowLevelMorphologyBenchs 
  PUBLIC benchmark  
  PUBLIC benchmark_main 
  PUBLIC PoutreIPCore
  )
ENDIF()