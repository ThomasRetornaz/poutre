# FORMAT
file(GLOB_RECURSE FFILES *.[hc] *.[hc]pp)
poutre_clang_format(poutre-format ${FFILES})
# tidy
poutre_tidy(poutre-tidy ${FFILES})

add_subdirectory(poutreBase ${TEMPORARY_DIRECTORY}/poutreBase)
add_subdirectory(poutreImageProcessingCore
                 ${TEMPORARY_DIRECTORY}/poutreImageProcessingCore)
add_subdirectory(poutreImageProcessingPixelOperation
                 ${TEMPORARY_DIRECTORY}/poutreImageProcessingPixelOperation)
add_subdirectory(poutreImageProcessingSE
                 ${TEMPORARY_DIRECTORY}/poutreImageProcessingSE)
add_subdirectory(poutreIO ${TEMPORARY_DIRECTORY}/poutreIO)
add_subdirectory(poutreImageProcessingLowLevelMorphology
                 ${TEMPORARY_DIRECTORY}/poutreLowLevelMorphology)
