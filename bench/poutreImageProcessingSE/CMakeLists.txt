

include_directories(${POUTRE_SRC_DIR})
include_directories(${GOOGLE_BENCHMARK_INCLUDE_DIRS})

# Bench project
set(PoutreImageProcessingSESubdirectory ${POUTRE_BENCH_DIR}/poutreImageProcessingSE)

set(PoutreImageProcessingSEBenchSRC
  ${PoutreImageProcessingSESubdirectory}/main.hpp
  ${PoutreImageProcessingSESubdirectory}/main.cpp
  )

#UGLY fix this later
if(WIN32)
  LINK_DIRECTORIES(${CMAKE_BINARY_DIR}/$(ConfigurationName))
endif()  

add_executable(PoutreIPSEBenchs ${PoutreImageProcessingSEBenchSRC})
set_target_properties(PoutreIPSEBenchs PROPERTIES  
  FOLDER "Bench/")

IF(WIN32)
target_link_libraries(PoutreIPSEBenchs 
  ${GOOGLE_BENCHMARK}
  shlwapi.lib
  #PUBLIC PoutreIPSE
  ${Boost_SYSTEM_LIBRARY} 
  )
ELSE()
target_link_libraries(PoutreIPSEBenchs 
  ${GOOGLE_BENCHMARK}
  #PUBLIC PoutreIPSE 
  ${Boost_SYSTEM_LIBRARY}   
  )
ENDIF()

add_dependencies(PoutreIPSEBenchs ${GOOGLE_BENCHMARK})
