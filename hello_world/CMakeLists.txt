
cmake_minimum_required(VERSION 3.20)


include(toolchain_arm-none-eabi-gcc-13.3.Rel1.cmake)
include(common_settings.cmake)

# CMSIS Core library
set(CMSISCORE "${CMAKE_CURRENT_LIST_DIR}/CMSIS/Core")

# CMSIS DSP library
set(CMSISDSP "${CMAKE_CURRENT_LIST_DIR}/CMSIS_DSP")


# set up project and enable languages
project(hello_world C CXX ASM)

# project specific defines
list(APPEND compile_definitions "CORE_M4")
list(APPEND compile_definitions "__CORTEX_M4F")
list(APPEND compile_definitions "ARM_MATH_CM4")
list(APPEND compile_definitions "__FPU_PRESENT=1")
# list(APPEND compile_definitions "ARM_MATH_LOOPUNROLL")
# list(APPEND compile_definitions "ARM_MATH_ROUNDING")


# config-specific defines from CMakePresets.json
list(APPEND compile_definitions ${config_compile_definitions})

# project specific includes
list(APPEND include_directories "${CMAKE_CURRENT_LIST_DIR}")
list(APPEND include_directories "${CMSISCORE}/Include")
list(APPEND include_directories "${CMSISDSP}/Include")
list(APPEND include_directories "${CMAKE_CURRENT_LIST_DIR}/system/include/platform")
list(APPEND include_directories "${CMAKE_CURRENT_LIST_DIR}/system/include/S6E2CCA/PDL")


# build CMSIS-DSP library
add_subdirectory("${CMSISDSP}/Source" lib_dsp)
target_compile_definitions(CMSISDSP PRIVATE ${compile_definitions} )


#
# firmware image
#
set(target_name "${PROJECT_NAME}.axf")
add_executable(${target_name})

target_compile_definitions(${target_name} PRIVATE ${compile_definitions} )
target_include_directories(${target_name} PRIVATE ${include_directories} )

# linker script
set(linker_script "${CMAKE_CURRENT_LIST_DIR}/ldscripts/s6e2cca_rom.ld")
target_link_options(${target_name} PRIVATE "-T${linker_script}")

# create map file (post build)
target_link_options(${target_name} PRIVATE "-Wl,-Map=${PROJECT_NAME}.map")

# create .hex file (post build)
add_custom_command(TARGET ${target_name} POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} --output-target ihex "${PROJECT_NAME}.axf" "${PROJECT_NAME}.hex"
    VERBATIM
    )


# add libs to link with
target_link_libraries(${target_name} 
    CMSISDSP
    )

    
# add startup code to the project
target_sources(${target_name} PRIVATE
    system/include/S6E2CCA/startup/startup_s6e2cc.S
    )

# add system specific source code to the project
target_sources(${target_name} PRIVATE
    system/include/S6E2CCA/PDL/pdl.c
    system/include/S6E2CCA/PDL/system_s6e2cc.c
    system/include/S6E2CCA/PDL/mfs.c
    system/include/platform/utils.c
    system/include/platform/platform.c
    )

# add custom code to the project
target_sources(${target_name} PRIVATE
    crt_dummy_syscalls.c
    hello_world.cpp
    debug_utils.cpp
    )


