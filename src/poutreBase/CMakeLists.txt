set(PoutreBaseSubdirectory ${POUTRE_SRC_DIR}/poutreBase)

include_directories(BEFORE SYSTEM ${SPDLOG_INCLUDES})

set(PoutreBaseSRC_DETAILS
    ${PoutreBaseSubdirectory}/include/poutreSimd.hpp
    ${PoutreBaseSubdirectory}/include/poutreSimdAlgorithm.hpp
    ${PoutreBaseSubdirectory}/include/poutreThreading.hpp)

set(PoutreBaseSRC_PUBLICHEADERS
    ${PoutreBaseSubdirectory}/poutreBase.hpp
    ${PoutreBaseSubdirectory}/poutreConfig.hpp
    ${PoutreBaseSubdirectory}/poutreTypes.hpp
    ${PoutreBaseSubdirectory}/poutreJsonSerialization.hpp
    ${PoutreBaseSubdirectory}/poutreTrace.hpp
    ${PoutreBaseSubdirectory}/poutreRegistrar.hpp
    ${PoutreBaseSubdirectory}/poutreChronos.hpp
    ${PoutreBaseSubdirectory}/poutreCoordinate.hpp
    ${PoutreBaseSubdirectory}/poutreContainerHelper.hpp
    ${PoutreBaseSubdirectory}/poutreDenseIterator.hpp
    ${PoutreBaseSubdirectory}/poutreStaticContainer.hpp
    ${PoutreBaseSubdirectory}/poutreContainerView.hpp)

set(PoutreBaseSRC_CPP
    ${PoutreBaseSubdirectory}/src/poutreJsonSerialization.cpp
    ${PoutreBaseSubdirectory}/src/poutreTrace.cpp
    ${PoutreBaseSubdirectory}/src/poutreCoordinate.cpp
    ${PoutreBaseSubdirectory}/src/poutreType.cpp
    ${PoutreBaseSubdirectory}/src/poutreChronos.cpp
    ${PoutreBaseSubdirectory}/src/poutreThreading.cpp)
source_group(details FILES ${PoutreBaseSRC_DETAILS})
source_group(src FILES ${PoutreBaseSRC_CPP})
source_group(header FILES ${PoutreBaseSRC_PUBLICHEADERS})

set(PoutreBaseSRC ${PoutreBaseSRC_DETAILS} ${PoutreBaseSRC_CPP}
                  ${PoutreBaseSRC_PUBLICHEADERS})

add_library(PoutreBase ${PoutreBaseSRC})
# target_include_directories(PoutreBase PRIVATE ${PoutreBaseSubdirectory}
# PRIVATE ${PoutreBaseSubdirectory}/include PRIVATE
# ${PoutreBaseSubdirectory}/src )

target_link_libraries(
  PoutreBase
  PUBLIC jsoncpp_lib
  INTERFACE fmt-header-only
  INTERFACE spdlog_header_only
  INTERFACE xsimd
  PUBLIC OpenMP::OpenMP_CXX
         # INTERFACE Eigen INTERFACE ${SIMD_LIB}
)
set_target_properties(PoutreBase PROPERTIES FOLDER "src/")
# force custom target before start
add_dependencies(PoutreBase xsimd jsoncpp_lib)
# add_dependencies(PoutreBase Eigendep)
target_compile_definitions(PoutreBase INTERFACE FMT_HEADER_ONLY=1)
#target_compile_definitions(PoutreBase INTERFACE SPDLOG_HEADER_ONLY=1)

if(BUILD_SHARED_LIBS)
  set_target_properties(PoutreBase PROPERTIES COMPILE_FLAGS -DJSON_DLL)
endif(BUILD_SHARED_LIBS)
