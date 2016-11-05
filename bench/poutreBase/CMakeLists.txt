

include_directories(${POUTRE_SRC_DIR})
include_directories(${GOOGLE_BENCHMARK_INCLUDE_DIRS})

# Bench project
set(PoutreBaseBenchSubdirectory ${POUTRE_BENCH_DIR}/poutreBase)

set(PoutreBaseBenchSRC
  ${PoutreBaseBenchSubdirectory}/main.hpp
  ${PoutreBaseBenchSubdirectory}/main.cpp
  ${PoutreBaseBenchSubdirectory}/containerview.cpp
  )

#UGLY fix this later
LINK_DIRECTORIES(${CMAKE_BINARY_DIR}/$(ConfigurationName))

add_executable(PoutreBaseBenchs ${PoutreBaseBenchSRC})
set_target_properties(PoutreBaseBenchs PROPERTIES  
  FOLDER "Bench/")

IF(WIN32)
target_link_libraries(PoutreBaseBenchs 
  PUBLIC benchmark  
  PUBLIC shlwapi.lib
  PUBLIC PoutreBase
  )
ELSE()
target_link_libraries(PoutreBaseBenchs 
  PUBLIC benchmark  
  PUBLIC PoutreBase
  )
ENDIF()

add_dependencies(PoutreBaseBenchs ${GOOGLE_BENCHMARK})
