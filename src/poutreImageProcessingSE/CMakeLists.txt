set(poutreImageProcessingSE ${POUTRE_SRC_DIR}/poutreImageProcessingSE)

set(poutreImageProcessingSE_SRC_DETAILS
    ${poutreImageProcessingSE}/include/poutreImageProcessingSENeighborhood.hpp
    ${poutreImageProcessingSE}/include/poutreImageProcessingSETypesAndTags.hpp)

set(poutreImageProcessingSE_PUBLICHEADERS
    ${poutreImageProcessingSE}/poutreImageProcessingSECore.hpp
    ${poutreImageProcessingSE}/poutreImageProcessingSEInterface.hpp
    ${poutreImageProcessingSE}/poutreImageProcessingSENeighborList.hpp)

set(poutreImageProcessingSE_CPP
    ${poutreImageProcessingSE}/src/poutreImageProcessingSEInterface.cpp
    ${poutreImageProcessingSE}/src/poutreImageProcessingSENeighborList.cpp)

source_group(details FILES ${poutreImageProcessingSE_SRC_DETAILS})
source_group(src FILES ${poutreImageProcessingSE_CPP})
source_group(header FILES ${poutreImageProcessingSE_PUBLICHEADERS})

set(poutreImageProcessingSE_SRC
    ${poutreImageProcessingSE_SRC_DETAILS}
    ${poutreImageProcessingSE_PUBLICHEADERS} ${poutreImageProcessingSE_CPP})

add_library(PoutreIPSE ${poutreImageProcessingSE_SRC})
target_link_libraries(
  PoutreIPSE
  PRIVATE jsoncpp_lib
  INTERFACE Eigen
  INTERFACE xsimd
  PoutreBase PoutreIPCore)
set_target_properties(PoutreIPSE PROPERTIES FOLDER "src/")

if(BUILD_SHARED_LIBS)
  set_target_properties(PoutreIPSE PROPERTIES COMPILE_FLAGS -DJSON_DLL)
endif(BUILD_SHARED_LIBS)

install_poutre_targets(TARGET PoutreIPSE CONFIGURATION Release HEADER_FILES
                       ${poutreImageProcessingSE_PUBLICHEADERS})
