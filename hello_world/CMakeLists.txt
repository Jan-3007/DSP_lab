
cmake_minimum_required(VERSION 3.20)


include(toolchain_arm-none-eabi-gcc-13.3.Rel1.cmake)
include(common_settings.cmake)

# CMSIS Core library
set(CMSISCORE "${CMAKE_CURRENT_LIST_DIR}/CMSIS/Core")

# CMSIS DSP library             temp
#set(CMSISDSP "${CMAKE_CURRENT_LIST_DIR}/CMSIS_DSP")


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
list(APPEND include_directories "${CMAKE_CURRENT_LIST_DIR}/device")
list(APPEND include_directories "${CMAKE_CURRENT_LIST_DIR}/device/PDL")
list(APPEND include_directories "${CMAKE_CURRENT_LIST_DIR}/utils")


# build CMSIS-DSP library           temp
#add_subdirectory("${CMSISDSP}/Source" lib_dsp)
#target_compile_definitions(CMSISDSP PRIVATE ${compile_definitions} )


#
# firmware image
#
set(target_name "${PROJECT_NAME}.axf")
add_executable(${target_name})

target_compile_definitions(${target_name} PRIVATE ${compile_definitions} )
target_include_directories(${target_name} PRIVATE ${include_directories} )

# generate final linker script
add_custom_target(generated_linker_script
  DEPENDS ${CMAKE_CURRENT_LIST_DIR}/device/S6E2CCAJOA.ld
  )

#if (CMAKE_HOST_SYSTEM_NAME STREQUAL "Windows")
    message(STATUS "Windows: building custom ld script")
    add_custom_command( 
                        OUTPUT ${CMAKE_CURRENT_LIST_DIR}/device/S6E2CCAJOA_generated.ld
                        MAIN_DEPENDENCY ${CMAKE_CURRENT_LIST_DIR}/device/S6E2CCAJOA.ld
                        COMMAND ${CMAKE_C_COMPILER} -E -x c -CC -I${CMAKE_CURRENT_LIST_DIR}/device S6E2CCAJOA.ld
                        #COMMAND sh ARGS -c "gcc -E -x c -CC -I${CMAKE_CURRENT_LIST_DIR}/device S6E2CCAJOA.ld | grep -v '^#' > S6E2CCAJOA_generated.ld"
                        WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
                        COMMENT "build preprocessed linker script"
                        VERBATIM
                        )
#endif()
if (CMAKE_HOST_SYSTEM_NAME STREQUAL "Linux")
    message(STATUS "Linux: building custom ld script")
    add_custom_command( OUTPUT ${CMAKE_CURRENT_LIST_DIR}/device/S6E2CCAJOA_generated.ld
                        COMMAND bash ARGS -c "gcc -E -x c -CC -I${CMAKE_CURRENT_LIST_DIR} /device/S6E2CCAJOA.ld | grep -v '^#' > /device/S6E2CCAJOA_generated.ld"
                        WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
                        COMMENT "build preprocessed linker script"
                        VERBATIM
                        )
endif()

# linker script
add_dependencies(${target_name} generated_linker_script)
#set(linker_script "${CMAKE_CURRENT_LIST_DIR}/device/S6E2CCAJOA_generated.ld")
set(linker_script "${CMAKE_CURRENT_LIST_DIR}/device/S6E2CCAJOA_generated2.ld")
target_link_options(${target_name} PRIVATE "-T${linker_script}")


# create map file (post build)
target_link_options(${target_name} PRIVATE "-Wl,-Map=${PROJECT_NAME}.map")

# create .hex file (post build)
add_custom_command(TARGET ${target_name} POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} --output-target ihex "${PROJECT_NAME}.axf" "${PROJECT_NAME}.hex"
    COMMENT "creating .hex file"
    VERBATIM
    )


# add libs to link with         temp
#target_link_libraries(${target_name} 
#    CMSISDSP
#    )

    
# add startup code to the project
target_sources(${target_name} PRIVATE
    device/startup_S6E2CCAJOA.c
    )

# add system specific source code to the project
target_sources(${target_name} PRIVATE
    device/PDL/pdl.c
    device/PDL/system_s6e2cc.c
    device/PDL/mfs.c
    device/PDL/clk.c
    device/PDL/dstc.c
    device/PDL/i2s.c
    device/PDL/interrupts_fm4_type_b.c
    device/PDL/wm8731.c
    )

# add custom code to the project
target_sources(${target_name} PRIVATE
    utils/crt_dummy_syscalls.c
    utils/platform.c
    utils/utils.c
    utils/debug_utils.cpp
    hello_world.cpp
    )



