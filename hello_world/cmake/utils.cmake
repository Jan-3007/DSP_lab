
#
# Add a linker script to the given target.
# The script will be passed through the preprocessor using current include paths
# and definitions to produce the effective script passed to the linker.
#

function(add_linker_script tgt ldscript)

    # dir where target will be placed
    get_target_property(bin_dir ${tgt} BINARY_DIR)

    # target name without ext
    cmake_path(GET ldscript FILENAME ldscript_name)
    cmake_path(REMOVE_EXTENSION ldscript_name OUTPUT_VARIABLE ldscript_basename)

    # generated script file path
    set(generated_linker_script ${bin_dir}/${ldscript_basename}_generated.ld)

    # pass it to the linker
    target_link_options(${tgt} PRIVATE "-T${generated_linker_script}")

    # add to target sources so that compilation gets triggerd
    target_sources(${tgt} PRIVATE ${generated_linker_script})

    # add a custom command which preprocesses the script
    add_custom_command(
        OUTPUT "${generated_linker_script}"
        DEPENDS "${ldscript}"
        COMMENT "Generating linker script ${generated_linker_script}"
        COMMAND ${CMAKE_C_COMPILER}
            -o ${generated_linker_script}
            -E ${ldscript}
            -P
            "-I$<JOIN:$<TARGET_PROPERTY:${tgt},INCLUDE_DIRECTORIES>,;-I>"
            "-D$<JOIN:$<TARGET_PROPERTY:${tgt},COMPILE_DEFINITIONS>,;-D>"
        COMMAND_EXPAND_LISTS
        VERBATIM
    )

    # remember file names in custom target properties
    set_target_properties(${tgt} PROPERTIES
        SourceLinkerScript "${ldscript}"
        EffectiveLinkerScript "${generated_linker_script}"
    )

endfunction()

