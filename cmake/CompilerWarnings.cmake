# Centralised compiler-warning configuration.
#
# Apply with:
#     bsd_set_warnings(<target> [PUBLIC|PRIVATE|INTERFACE])
#
# Defaults to PRIVATE for buildable targets. Honors the BSD_WARNINGS_AS_ERRORS
# option (declared in ProjectOptions.cmake). Per project rules, no compiler
# flags are hardcoded outside this module.

include_guard(GLOBAL)

set(_BSD_WARN_GNU_LIKE
    -Wall
    -Wextra
    -Wpedantic
    -Wshadow
    -Wnon-virtual-dtor
    -Wold-style-cast
    -Wcast-align
    -Wunused
    -Woverloaded-virtual
    -Wconversion
    -Wsign-conversion
    -Wnull-dereference
    -Wdouble-promotion
    -Wformat=2
    -Wimplicit-fallthrough
)

set(_BSD_WARN_GCC_EXTRA
    -Wmisleading-indentation
    -Wduplicated-cond
    -Wduplicated-branches
    -Wlogical-op
    -Wuseless-cast
)

set(_BSD_WARN_MSVC
    /W4
    /permissive-
    /w14242 /w14254 /w14263 /w14265 /w14287
    /we4289
    /w14296 /w14311 /w14545 /w14546 /w14547 /w14549 /w14555
    /w14619 /w14640 /w14826 /w14905 /w14906 /w14928
    /Zc:__cplusplus
    /Zc:preprocessor
)

function(bsd_set_warnings target)
    set(scope "PRIVATE")
    if(ARGC GREATER 1)
        set(scope "${ARGV1}")
    endif()

    get_target_property(_type ${target} TYPE)
    if(_type STREQUAL "INTERFACE_LIBRARY")
        set(scope "INTERFACE")
    endif()

    if(MSVC)
        target_compile_options(${target} ${scope} ${_BSD_WARN_MSVC})
        if(BSD_WARNINGS_AS_ERRORS)
            target_compile_options(${target} ${scope} /WX)
        endif()
    else()
        target_compile_options(${target} ${scope} ${_BSD_WARN_GNU_LIKE})
        if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
            target_compile_options(${target} ${scope} ${_BSD_WARN_GCC_EXTRA})
        endif()
        if(BSD_WARNINGS_AS_ERRORS)
            target_compile_options(${target} ${scope} -Werror)
        endif()
    endif()
endfunction()
