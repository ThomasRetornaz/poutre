# FORMAT
file(GLOB FFILES *.[hc] *.[hc]pp)
clang_format(format ${FFILES})

cmake_format(cmake-format ${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt
             CMakeLists.txt)

add_subdirectory (poutreBase  ${TEMPORARY_DIRECTORY}/poutreBase)
add_subdirectory (poutreImageProcessingCore  ${TEMPORARY_DIRECTORY}/poutreImageProcessingCore)
add_subdirectory (poutreImageProcessingPixelOperation  ${TEMPORARY_DIRECTORY}/poutreImageProcessingPixelOperation)
add_subdirectory (poutreImageProcessingSE ${TEMPORARY_DIRECTORY}/poutreImageProcessingSE)
add_subdirectory (poutreIO  ${TEMPORARY_DIRECTORY}/poutreIO)
add_subdirectory (poutreImageProcessingLowLevelMorphology ${TEMPORARY_DIRECTORY}/poutreLowLevelMorphology)