
set(poutreImageProcessingSE ${POUTRE_SRC_DIR}/poutreImageProcessingSE)

set(poutreImageProcessingSE_SRC_DETAILS
  )

set(poutreImageProcessingSE_PUBLICHEADERS  
  ${poutreImageProcessingSE}/poutreImageProcessingSE.hpp
)

set(poutreImageProcessingSE_CPP
  ${poutreImageProcessingSE}/src/dummy.cpp
  )
  
source_group(details FILES ${poutreImageProcessingSE_SRC_DETAILS})
source_group(src FILES ${poutreImageProcessingSE_CPP})
source_group(header FILES ${poutreImageProcessingSE_PUBLICHEADERS})

set(poutreImageProcessingSE_SRC
	${poutreImageProcessingSE_SRC_DETAILS}
	${poutreImageProcessingSE_PUBLICHEADERS}
	${poutreImageProcessingSE_CPP}
	)

add_library(PoutreIPSE ${poutreImageProcessingSE_SRC})
target_link_libraries(PoutreIPSE 
  # PRIVATE ${Boost_GRAPH_LIBRARY} 
  # PRIVATE ${Boost_DATE_TIME_LIBRARY} 
  # PRIVATE ${Boost_REGEX_LIBRARY} 
  # PRIVATE ${Boost_THREAD_LIBRARY} 
  # PRIVATE ${Boost_SYSTEM_LIBRARY}
  PRIVATE jsoncpp
  INTERFACE Eigen
  #INTERFACE libsimdpp 
  PoutreBase
  PoutreIPCore  
  )
set_target_properties(PoutreIPSE PROPERTIES FOLDER "src/")

IF(BUILD_SHARED_LIBS)
  SET_TARGET_PROPERTIES(PoutreIPSE PROPERTIES COMPILE_FLAGS -DJSON_DLL)
ENDIF(BUILD_SHARED_LIBS)


install_POUTRE_targets(
  TARGET PoutreIPSE 
  CONFIGURATION Release 
  HEADER_FILES ${poutreImageProcessingSE_PUBLICHEADERS}
  )
