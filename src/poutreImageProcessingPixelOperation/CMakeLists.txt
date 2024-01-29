include_directories(${POUTRE_SRC_DIR})

set(PoutreIPPPSubdirectory
    ${POUTRE_SRC_DIR}/poutreImageProcessingPixelOperation)

set(PoutreIPPPSRC_DETAILS
    ${PoutreIPPPSubdirectory}/include/poutreImageProcessingUnaryOp.hpp
    ${PoutreIPPPSubdirectory}/include/poutreImageProcessingUnaryOp.hpp
    ${PoutreIPPPSubdirectory}/include/poutreImageProcessingBinaryOp.hpp
    ${PoutreIPPPSubdirectory}/include/poutreImageProcessingTernaryOp.hpp
    ${PoutreIPPPSubdirectory}/include/poutreImageProcessingQuaternaryOp.hpp
    ${PoutreIPPPSubdirectory}/include/poutreImageProcessingArith.hpp
    ${PoutreIPPPSubdirectory}/include/poutreImageProcessingCompare.hpp
    ${PoutreIPPPSubdirectory}/include/poutreImageProcessingTranspose.hpp)

set(PoutreIPPPSRC_PUBLICHEADERS
    ${PoutreIPPPSubdirectory}/poutreImageProcessingPixelProcessing.hpp)

set(PoutreIPPPSRC_CPP ${PoutreIPPPSubdirectory}/src/dummy.cpp)
source_group(details FILES ${PoutreIPPPSRC_DETAILS})
source_group(src FILES ${PoutreIPPPSRC_CPP})
source_group(header FILES ${PoutreIPPPSRC_PUBLICHEADERS})

set(PoutreIPPPSRC ${PoutreIPPPSRC_DETAILS} ${PoutreIPPPSRC_CPP}
                  ${PoutreIPPPSRC_PUBLICHEADERS})

add_library(PoutreIPPixelProcessing ${PoutreIPPPSRC})

target_link_libraries(
  PoutreIPPixelProcessing
  PUBLIC jsoncpp_lib
  INTERFACE xsimd
  PUBLIC PoutreBase
  PUBLIC PoutreIPCore)
set_target_properties(PoutreIPPixelProcessing PROPERTIES FOLDER "src/")

if(BUILD_SHARED_LIBS)
  set_target_properties(PoutreIPPixelProcessing PROPERTIES COMPILE_FLAGS
                                                           -DJSON_DLL)
endif(BUILD_SHARED_LIBS)

install_poutre_targets(TARGET PoutreIPPixelProcessing CONFIGURATION Release
                       HEADER_FILES ${PoutreIPSRC})
