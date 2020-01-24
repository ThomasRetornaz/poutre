

include_directories(${POUTRE_SRC_DIR})
include_directories(${GOOGLE_BENCHMARK_INCLUDE_DIRS})

# Bench project
set(PoutreImageProcessingCoreBenchSubdirectory ${POUTRE_BENCH_DIR}/poutreImageProcessingCore)

set(PoutreImageProcessingCoreBenchSRC
  ${PoutreImageProcessingCoreBenchSubdirectory}/main.hpp
  ${PoutreImageProcessingCoreBenchSubdirectory}/main.cpp
  ${PoutreImageProcessingCoreBenchSubdirectory}/priority_queue.cpp
  )

add_executable(PoutreIPCoreBenchs ${PoutreImageProcessingCoreBenchSRC})
set_target_properties(PoutreIPCoreBenchs PROPERTIES  
  FOLDER "Bench/")

IF(WIN32)
target_link_libraries(PoutreIPCoreBenchs 
  PUBLIC benchmark  
  PUBLIC benchmark_main 
  PUBLIC shlwapi.lib
  PUBLIC PoutreIPCore
  )
ELSE()
target_link_libraries(PoutreIPCoreBenchs 
  PUBLIC benchmark  
  PUBLIC PoutreIPCore
  )
ENDIF()