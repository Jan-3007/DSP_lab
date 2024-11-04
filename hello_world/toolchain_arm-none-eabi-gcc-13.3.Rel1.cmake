#
# This file specifies the toolchain.
#

set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR arm)

# ABI triplet
set(toolchain_triplet "arm-none-eabi-")

#
# We use Arm GNU Toolchain Version 13.3.Rel1 (July 04, 2024) which is based on GCC 13.3
# https://developer.arm.com/downloads/-/arm-gnu-toolchain-downloads
# https://developer.arm.com/-/media/Files/downloads/gnu/13.3.rel1/binrel/arm-gnu-toolchain-13.3.rel1-mingw-w64-i686-arm-none-eabi.zip
#

# specify path to toolchain  
if (CMAKE_HOST_SYSTEM_NAME STREQUAL "Windows")
    message(STATUS "OS: Windows")
    set(GCC_ARM_V13_3_R1_ROOT "C:/GCC/arm-gnu-toolchain-13.3.rel1-mingw-w64-i686-arm-none-eabi")
endif()
if (CMAKE_HOST_SYSTEM_NAME STREQUAL "Linux")
    message(STATUS "OS: Linux")
    set(GCC_ARM_V13_3_R1_ROOT "/opt/arm-gnu-toolchain-13.3.rel1-x86_64-arm-none-eabi")
endif()


# Start search in the specified dir, suffix bin will be apppended automatically.
list(PREPEND CMAKE_PREFIX_PATH ${GCC_ARM_V13_3_R1_ROOT})


find_program(compiler_exe_file_path
    "${toolchain_triplet}gcc"
    PATH_SUFFIXES bin   # try subdir bin
    REQUIRED            # fail if not found
    )

get_filename_component(toolchain_bin_dir ${compiler_exe_file_path} DIRECTORY)
get_filename_component(toolchain_ext ${compiler_exe_file_path} LAST_EXT)

message(STATUS "toolchain_bin_dir=${toolchain_bin_dir}")

set(toolchain_prefix ${toolchain_bin_dir}/${toolchain_triplet})

# Configure compilers, cmake will find other tools automatically.
# see https://cmake.org/cmake/help/book/mastering-cmake/chapter/Cross%20Compiling%20With%20CMake.html
set(CMAKE_C_COMPILER   "${toolchain_prefix}gcc${toolchain_ext}")
set(CMAKE_ASM_COMPILER "${toolchain_prefix}gcc${toolchain_ext}")
set(CMAKE_CXX_COMPILER "${toolchain_prefix}g++${toolchain_ext}")

# some utilities which cmake does not automatically look up
set(CMAKE_SIZE ${toolchain_prefix}size${toolchain_ext} CACHE INTERNAL "")

# Tell CMake not to try to link executables during compiler checks because this requires linker scripts etc.
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)
