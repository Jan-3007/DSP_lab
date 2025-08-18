#
# Author: Jan Eberhardt
#

#
# settings which apply to the project
#

include_guard(GLOBAL)


# language standard
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)  # produces -std=gnu11 which is gcc's default

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)  # produces -std=gnu++17


# CPU and FPU
list(APPEND cpu_flags "-mthumb")
list(APPEND cpu_flags "-mcpu=cortex-m4")
list(APPEND cpu_flags "-mfloat-abi=hard")
list(APPEND cpu_flags "-mfpu=fpv4-sp-d16") # single precision FPU

# common flags for ASM/C/C++
list(APPEND common_compile_flags "-g3")         # include extra information such as macro definitions
list(APPEND common_compile_flags "-ffunction-sections")     # remove unused functions
list(APPEND common_compile_flags "-fdata-sections")         # remove unused data
list(APPEND common_compile_flags "-fno-common")
list(APPEND common_compile_flags "-fsingle-precision-constant") # make floating point literals float (rather than double)
list(APPEND common_compile_flags "-Wall")
list(APPEND common_compile_flags "-Wno-unused-function")
list(APPEND common_compile_flags "-fno-tree-loop-distribute-patterns") # do not transform loops into memset/memcopy calls
                                                                       # IMPORTANT: We need to turn off this optimization, otherwise the startup code fails.
# config-specific flags from CMakePresets.json
list(APPEND common_compile_flags "${config_compile_flags}")



# ASM flags
list(APPEND asm_compile_flags "${cpu_flags}")
list(APPEND asm_compile_flags "${common_compile_flags}")

# C flags
list(APPEND c_compile_flags "${cpu_flags}")
list(APPEND c_compile_flags "${common_compile_flags}")

# C++ flags
list(APPEND cpp_compile_flags "${cpu_flags}")
list(APPEND cpp_compile_flags "${common_compile_flags}")
list(APPEND cpp_compile_flags "-fno-exceptions")
list(APPEND cpp_compile_flags "-fno-rtti")
list(APPEND cpp_compile_flags "-Wextra") # useful warnings which are not enabled via -Wall


# linker flags (Note: cmake will pass CMAKE_CXX_FLAGS as well)
list(APPEND linker_flags "-Wl,--gc-sections")   # remove unused functions and data
list(APPEND linker_flags "-Wl,--print-memory-usage")
#list(APPEND linker_flags "-Wl,-verbose")
list(APPEND linker_flags "-nostdlib")           # disable automatic linking of std libs

# clear out default flags
set(CMAKE_ASM_FLAGS         "" CACHE INTERNAL "")
set(CMAKE_ASM_FLAGS_DEBUG   "" CACHE INTERNAL "")
set(CMAKE_ASM_FLAGS_RELEASE "" CACHE INTERNAL "")
set(CMAKE_C_FLAGS           "" CACHE INTERNAL "")
set(CMAKE_C_FLAGS_DEBUG     "" CACHE INTERNAL "")
set(CMAKE_C_FLAGS_RELEASE   "" CACHE INTERNAL "")
set(CMAKE_CXX_FLAGS         "" CACHE INTERNAL "")
set(CMAKE_CXX_FLAGS_DEBUG   "" CACHE INTERNAL "")
set(CMAKE_CXX_FLAGS_RELEASE "" CACHE INTERNAL "")

# init CMAKE variables (separating options by spaces)
list(JOIN asm_compile_flags " " CMAKE_ASM_FLAGS)
list(JOIN c_compile_flags   " " CMAKE_C_FLAGS)
list(JOIN cpp_compile_flags " " CMAKE_CXX_FLAGS)
list(JOIN linker_flags      " " CMAKE_EXE_LINKER_FLAGS)

