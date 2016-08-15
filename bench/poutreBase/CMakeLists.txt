

include_directories(${POUTRE_SRC_DIR})
include_directories(${GOOGLE_BENCHMARK_INCLUDE_DIRS})

# Bench project
set(PoutreBaseBenchSubdirectory ${POUTRE_BENCH_DIR}/poutreBase)

set(PoutreBaseBenchSRC
  ${PoutreBaseBenchSubdirectory}/main.hpp
  ${PoutreBaseBenchSubdirectory}/main.cpp
  )

add_executable(PoutreBaseBenchs ${PoutreBaseBenchSRC})
set_target_properties(PoutreBaseBenchs PROPERTIES  
  FOLDER "Bench/")

target_link_libraries(PoutreBaseBenchs 
  PRIVATE benchmark  
  PUBLIC PoutreBase
  )
add_dependencies(PoutreBaseBenchs ${GOOGLE_BENCHMARK})
