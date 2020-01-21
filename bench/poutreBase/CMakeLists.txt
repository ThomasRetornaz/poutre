

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
if(WIN32)
  LINK_DIRECTORIES(${CMAKE_BINARY_DIR}/$(ConfigurationName))
endif()  

add_executable(PoutreBaseBenchs ${PoutreBaseBenchSRC})
set_target_properties(PoutreBaseBenchs PROPERTIES  
  FOLDER "Bench/")

IF(WIN32)
target_link_libraries(PoutreBaseBenchs 
  ${GOOGLE_BENCHMARK}
  shlwapi.lib
  PoutreBase
  )
ELSE()
target_link_libraries(PoutreBaseBenchs 
  ${GOOGLE_BENCHMARK}
  PoutreBase
  )
ENDIF()

add_dependencies(PoutreBaseBenchs ${GOOGLE_BENCHMARK})
