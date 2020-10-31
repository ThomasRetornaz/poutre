

include_directories(${POUTRE_SRC_DIR})
include_directories(${GOOGLE_BENCHMARK_INCLUDE_DIRS})

# Bench project
set(PoutreImageProcessingSESubdirectory ${POUTRE_BENCH_DIR}/poutreImageProcessingSE)

set(PoutreImageProcessingSEBenchSRC
  ${PoutreImageProcessingSESubdirectory}/main.hpp
  ${PoutreImageProcessingSESubdirectory}/main.cpp
  )

add_executable(PoutreIPSEBenchs ${PoutreImageProcessingSEBenchSRC})
set_target_properties(PoutreIPSEBenchs PROPERTIES  
  FOLDER "Bench/")

IF(WIN32)
target_link_libraries(PoutreIPSEBenchs 
  PUBLIC benchmark  
  PUBLIC benchmark_main 
  PUBLIC shlwapi.lib
  #PUBLIC PoutreIPSE
  )
ELSE()
target_link_libraries(PoutreIPSEBenchs 
  PUBLIC benchmark  
  PUBLIC benchmark_main 
  #PUBLIC PoutreIPSE 
  )
ENDIF()

