# cmake/tests.cmake
#
# Translation of the xmake add_test_target / setup_tests logic.
# Include it from the top-level CMakeLists.txt AFTER the Runtime and Embedded
# targets are defined (end of the file), then call grn_setup_tests(...).
#
#   include(cmake/tests.cmake)
#   grn_setup_tests(
#       TARGET_TESTS foo bar
#       AUTO_TESTS   baz
#   )
#
# Run with:
#   cmake --build build --target tests_foo        (a single test)
#   cmake --build build --target tests            (all TARGET_TESTS)
#   cmake --build build --target auto_tests       (all AUTO_TESTS)
#
# Note: xmake used "tests/<name>" as target name. A slash in a CMake target
# name is legal but awkward with some generators, so targets are "tests_<name>".
# The python scripts still receive "tests/<name>" and "build/tests/<name>"-style
# paths exactly like before.

if (IS_DESKTOP)
    find_package(Python3 COMPONENTS Interpreter REQUIRED)
endif ()

function(grn_add_test_target name)
    set(tgt "tests_${name}")
    if (TARGET ${tgt})
        return() # same test listed in both lists
    endif ()

    set(target_name "tests/${name}")                       # what the python scripts expect
    set(build_dir   "${CMAKE_BINARY_DIR}/tests/${name}")   # was build/<target_name>

    # --- "xmake build -y": make sure everything is built first ---------------
    set(deps Runtime)
    if (TARGET Embedded)
        list(APPEND deps Embedded)
    endif ()

    # --- pipeline step (desktop only) -----------------------------------------
    if (IS_DESKTOP)
        if (NOT GRNGAME_EMBED_ASSETS)
            set(pipeline_cmd
                    ${CMAKE_COMMAND} -E env "GRN_RUNTIME_DIR=$<TARGET_FILE_DIR:Runtime>"
                    ${Python3_EXECUTABLE}
                    "${CMAKE_SOURCE_DIR}/scripts/test/dev_pipeline.py"
                    "${target_name}"
                    "${build_dir}")
        else ()
            set(pipeline_cmd
                    ${CMAKE_COMMAND} -E env "GRN_RUNTIME_DIR=$<TARGET_FILE_DIR:Runtime>"
                    ${Python3_EXECUTABLE}
                    "${CMAKE_SOURCE_DIR}/scripts/test/embedded_pipeline.py"
                    "${target_name}"
                    "${build_dir}"
                    "${CMAKE_SOURCE_DIR}"
                    "${CMAKE_BINARY_DIR}/Embedded")
        endif ()
    else ()
        set(pipeline_cmd ${CMAKE_COMMAND} -E echo "You cant execute tests on mobile/web")
    endif ()

    # --- final run of the runtime ---------------------------------------------
    # $<TARGET_FILE_NAME:Runtime> already resolves to
    # Runtime-<plat>-<arch>-<config>[embedded][.exe], same as the manual
    # path building in the xmake version.
    add_custom_target(${tgt}
            COMMAND ${pipeline_cmd}
            COMMAND ${CMAKE_COMMAND} -E chdir "${build_dir}"
            "${build_dir}/$<TARGET_FILE_NAME:Runtime>"
            WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
            DEPENDS ${deps}
            USES_TERMINAL
            COMMENT "Running ${target_name}"
            VERBATIM)
    add_dependencies(${tgt} ${deps})
endfunction()

function(grn_add_group_target group)
    set(tests ${ARGN})
    set(commands)
    foreach (name IN LISTS tests)
        # sequential, like the xmake loop (add_dependencies would not guarantee order)
        list(APPEND commands
                COMMAND ${CMAKE_COMMAND} --build "${CMAKE_BINARY_DIR}"
                --config $<CONFIG> --target tests_${name})
    endforeach ()

    if (commands)
        add_custom_target(${group} ${commands} USES_TERMINAL VERBATIM)
    else ()
        add_custom_target(${group} COMMAND ${CMAKE_COMMAND} -E echo "No tests in ${group}")
    endif ()
endfunction()

function(grn_setup_tests)
    cmake_parse_arguments(ARG "" "" "TARGET_TESTS;AUTO_TESTS" ${ARGN})

    foreach (name IN LISTS ARG_TARGET_TESTS ARG_AUTO_TESTS)
        grn_add_test_target(${name})
    endforeach ()

    grn_add_group_target(tests      ${ARG_TARGET_TESTS})
    grn_add_group_target(auto_tests ${ARG_AUTO_TESTS})
endfunction()