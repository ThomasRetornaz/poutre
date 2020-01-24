

include_directories(${POUTRE_SRC_DIR})
include_directories(${GOOGLE_BENCHMARK_INCLUDE_DIRS})

# Bench project
set(PoutreBaseBenchSubdirectory ${POUTRE_BENCH_DIR}/poutreBase)

set(PoutreBaseBenchSRC
  ${PoutreBaseBenchSubdirectory}/main.hpp
  ${PoutreBaseBenchSubdirectory}/main.cpp
  ${PoutreBaseBenchSubdirectory}/containerview.cpp
  )

add_executable(PoutreBaseBenchs ${PoutreBaseBenchSRC})
set_target_properties(PoutreBaseBenchs PROPERTIES  
  FOLDER "Bench/")

IF(WIN32)
target_link_libraries(PoutreBaseBenchs 
  PUBLIC benchmark
  PUBLIC benchmark_main  
  PUBLIC shlwapi.lib
  PUBLIC PoutreBase
  )
ELSE()
target_link_libraries(PoutreBaseBenchs 
  PUBLIC benchmark
  PUBLIC benchmark_main  
  PUBLIC PoutreBase
  )
ENDIF()
