

include_directories(${POUTRE_SRC_DIR})
include_directories(${GOOGLE_BENCHMARK_INCLUDE_DIRS})

# Bench project
set(PoutreImageProcessingCoreBenchSubdirectory ${POUTRE_BENCH_DIR}/poutreImageProcessingCore)

set(PoutreImageProcessingCoreBenchSRC
  ${PoutreImageProcessingCoreBenchSubdirectory}/main.hpp
  ${PoutreImageProcessingCoreBenchSubdirectory}/main.cpp
  ${PoutreImageProcessingCoreBenchSubdirectory}/priority_queue.cpp
  )

#UGLY fix this later
LINK_DIRECTORIES(${CMAKE_BINARY_DIR}/$(ConfigurationName))

add_executable(PoutreIPCoreBenchs ${PoutreImageProcessingCoreBenchSRC})
set_target_properties(PoutreIPCoreBenchs PROPERTIES  
  FOLDER "Bench/")

IF(WIN32)
target_link_libraries(PoutreIPCoreBenchs 
  PUBLIC benchmark  
  PUBLIC shlwapi.lib
  )
ELSE()
target_link_libraries(PoutreIPCoreBenchs 
  PUBLIC benchmark  
  )
ENDIF()

add_dependencies(PoutreIPCoreBenchs ${GOOGLE_BENCHMARK})
