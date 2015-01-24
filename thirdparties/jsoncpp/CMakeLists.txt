cmake_minimum_required(VERSION 2.8)
project(jsoncpp)

set(jsoncppSubdirectory ${THIRD_PARTIES_PATH}/jsoncpp)

SET(jsoncppInclude 
  ${jsoncppSubdirectory}/json/json-forwards.h
  ${jsoncppSubdirectory}/json/json.h   
  )
set(jsoncppSRC
  ${jsoncppSubdirectory}/jsoncpp.cpp
)

add_library(jsoncpp ${jsoncppSRC} ${jsoncppInclude})
target_include_directories(jsoncpp
                           PRIVATE ${jsoncppSubdirectory}
                           PRIVATE ${jsoncppSubdirectory}/json
                           )
set_target_properties(jsoncpp PROPERTIES FOLDER "Dependencies/")		
IF(BUILD_SHARED_LIBS)
	SET_TARGET_PROPERTIES(jsoncpp PROPERTIES COMPILE_FLAGS -DJSON_DLL_BUILD)
ENDIF(BUILD_SHARED_LIBS)
					  
# install_POUTRE_targets(
  # TARGET jsoncpp 
  # CONFIGURATION Release 
  # HEADER_FILES ${jsoncppSRC})
set(JSONCPP_INCLUDE_DIR  ${jsoncppSubdirectory} PARENT_SCOPE)
  