include_directories(${POUTRE_SRC_DIR})

set(PoutreIPCoreSubdirectory ${POUTRE_SRC_DIR}/poutreImageProcessingCore)

set(PoutreIPCORESRC_DETAILS
    ${PoutreIPCoreSubdirectory}/include/poutreImageProcessingContainer.hpp
    ${PoutreIPCoreSubdirectory}/include/poutreImageProcessingContainerCopieConvert.hpp
    ${PoutreIPCoreSubdirectory}/include/poutreImageProcessingPriorityQueue.hpp)

set(PoutreIPCORESRC_PUBLICHEADERS
    ${PoutreIPCoreSubdirectory}/poutreImageProcessingCore.hpp
    ${PoutreIPCoreSubdirectory}/poutreImageProcessingInterface.hpp
    ${PoutreIPCoreSubdirectory}/poutreImageProcessingType.hpp
    ${PoutreIPCoreSubdirectory}/poutreImageProcessingInterfaceCopieConvert.hpp)

set(PoutreIPCORESRC_CPP
    ${PoutreIPCoreSubdirectory}/src/poutreImageProcessingType.cpp
    ${PoutreIPCoreSubdirectory}/src/poutreImageProcessingContainer.cpp
    ${PoutreIPCoreSubdirectory}/src/poutreImageProcessingInterface.cpp
    ${PoutreIPCoreSubdirectory}/src/poutreImageProcessingContainerCopieConvert.cpp
)
source_group(details FILES ${PoutreIPCORESRC_DETAILS})
source_group(src FILES ${PoutreIPCORESRC_CPP})
source_group(header FILES ${PoutreIPCORESRC_PUBLICHEADERS})

set(PoutreIPCORESRC ${PoutreIPCORESRC_DETAILS} ${PoutreIPCORESRC_CPP}
                    ${PoutreIPCORESRC_PUBLICHEADERS})

add_library(PoutreIPCore ${PoutreIPCORESRC})

target_link_libraries(
  PoutreIPCore
  PUBLIC jsoncpp_lib
  INTERFACE xsimd
  PUBLIC PoutreBase)
set_target_properties(PoutreIPCore PROPERTIES FOLDER "src/")

if(BUILD_SHARED_LIBS)
  set_target_properties(PoutreIPCore PROPERTIES COMPILE_FLAGS -DJSON_DLL)
endif(BUILD_SHARED_LIBS)

install_poutre_targets(TARGET PoutreIPCore CONFIGURATION Release HEADER_FILES
                       ${PoutreIPCORESRC_PUBLICHEADERS})
