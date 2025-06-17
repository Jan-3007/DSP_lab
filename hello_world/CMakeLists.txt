
cmake_minimum_required(VERSION 3.20)


include(cmake/toolchain_arm-none-eabi-gcc-13.3.Rel1.cmake)
include(cmake/common_settings.cmake)
include(cmake/utils.cmake)



# CMSIS Core library
set(CMSISCORE "${CMAKE_CURRENT_LIST_DIR}/CMSIS/Core")

# CMSIS DSP library
set(CMSISDSP "${CMAKE_CURRENT_LIST_DIR}/CMSIS_DSP")


#
# set up project and enable languages
#
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
list(APPEND include_directories "${CMAKE_CURRENT_LIST_DIR}/PDL")
list(APPEND include_directories "${CMAKE_CURRENT_LIST_DIR}/utils")
list(APPEND include_directories "${CMAKE_CURRENT_LIST_DIR}/device")

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


#
# linker script
#
add_linker_script(${target_name} ${CMAKE_CURRENT_LIST_DIR}/device/S6E2CCAJOA.ld.cc)


#
# post build
#

# create map file (post build)
target_link_options(${target_name} PRIVATE "-Wl,-Map=${PROJECT_NAME}.map")

# create .hex file (post build)
add_custom_command(TARGET ${target_name} POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} --output-target ihex "${PROJECT_NAME}.axf" "${PROJECT_NAME}.hex"
    COMMENT "creating .hex file"
    VERBATIM
    )

# create .symbols file (post build)
add_custom_command(TARGET ${target_name} POST_BUILD
COMMAND ${CMAKE_NM} --numeric-sort --line-numbers --demangle --print-size "${PROJECT_NAME}.axf" ">" "${PROJECT_NAME}.symbols"
VERBATIM
)

# create .disasm file (post build)
add_custom_command(TARGET ${target_name} POST_BUILD
COMMAND ${CMAKE_OBJDUMP} --disassemble --demangle "${PROJECT_NAME}.axf" ">" "${PROJECT_NAME}.disasm"
VERBATIM
)


#
# add sources to target
#

# add libs to link with
target_link_libraries(${target_name} 
    CMSISDSP
    )

# add startup code to the project
target_sources(${target_name} PRIVATE
    device/startup_S6E2CCAJOA.c
    )

# add system specific source code to the project
target_sources(${target_name} PRIVATE
    PDL/pdl.c
    PDL/system_s6e2cc.c
    PDL/mfs.c
    PDL/clk.c
    PDL/dstc.c
    PDL/i2s.c
    PDL/interrupts_fm4_type_b.c
    PDL/wm8731.c
    )

# add utils files to the project
target_sources(${target_name} PRIVATE
    utils/crt_dummy_syscalls.c
    utils/platform.cpp
    utils/utils.cpp
    utils/debug_utils.cpp
    )

# add custom code to the project
target_sources(${target_name} PRIVATE
    hello_world.cpp
    hello_world_circ_buffer.cpp
    )
