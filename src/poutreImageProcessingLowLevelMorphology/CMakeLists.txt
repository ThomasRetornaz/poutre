include_directories(${POUTRE_SRC_DIR})

set(PoutreIPLLMSubdirectory
    ${POUTRE_SRC_DIR}/poutreImageProcessingLowLevelMorphology)

set(PoutreIPLLMRC_DETAILS
    ${PoutreIPLLMSubdirectory}/include/poutreImageProcessingEroDil.hpp
    ${PoutreIPLLMSubdirectory}/include/poutreImageProcessingEroDilNLS.hpp
    ${PoutreIPLLMSubdirectory}/include/poutreImageProcessingEroDilLine.hpp)

set(PoutreIPLLMSRC_PUBLICHEADERS
    ${PoutreIPLLMSubdirectory}/poutreImageProcessingLowLevelMorphology.hpp)

set(PoutreIPLLMSRC_CPP ${PoutreIPLLMSubdirectory}/src/dummy.cpp)
source_group(details FILES ${PoutreIPLLMRC_DETAILS})
source_group(src FILES ${PoutreIPLLMSRC_CPP})
source_group(header FILES ${PoutreIPLLMSRC_PUBLICHEADERS})

set(PoutreIPLLMSRC ${PoutreIPLLMRC_DETAILS} ${PoutreIPLLMSRC_CPP}
                   ${PoutreIPLLMSRC_PUBLICHEADERS})

add_library(PoutreIPLowLevelMorphology ${PoutreIPLLMSRC})

target_link_libraries(
  PoutreIPLowLevelMorphology
  PUBLIC jsoncpp_lib
  INTERFACE xsimd
  PUBLIC PoutreBase
  PUBLIC PoutreIPCore
  PUBLIC PoutreIPPixelProcessing)
set_target_properties(PoutreIPLowLevelMorphology PROPERTIES FOLDER "src/")

if(BUILD_SHARED_LIBS)
  set_target_properties(PoutreIPLowLevelMorphology PROPERTIES COMPILE_FLAGS
                                                              -DJSON_DLL)
endif(BUILD_SHARED_LIBS)

install_poutre_targets(TARGET PoutreIPLowLevelMorphology CONFIGURATION Release
                       HEADER_FILES ${PoutreIPLLMSRC_PUBLICHEADERS})
