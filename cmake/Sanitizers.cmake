# Sanitizer & coverage support.
#
# Apply with:
#     bsd_set_sanitizers(<target>)
#
# Activated by the BSD_ENABLE_{ASAN,UBSAN,TSAN,MSAN,COVERAGE} options declared
# in ProjectOptions.cmake. Per project rules, no sanitizer flags are hardcoded
# outside this module.
#
# @invariant Mutually exclusive sanitizers cannot be enabled simultaneously
#            (ASan vs TSan vs MSan). Misconfiguration produces a fatal CMake
#            error rather than silently picking one.
# @invariant Compile-time and link-time flags are always applied as a pair;
#            it is impossible to link with a different sanitizer set than was
#            compiled with through this helper.
# @invariant MSan is only supported on Clang.
# @invariant On MSVC, only AddressSanitizer is wired up; UBSan / TSan / MSan
#            options are ignored (with a one-time STATUS message) so a single
#            CI matrix can target all platforms uniformly.

include_guard(GLOBAL)

set(_BSD_SAN_COMMON_GNU
    -fno-omit-frame-pointer
    -g
)

set(_BSD_SAN_ASAN_GNU
    -fsanitize=address
)

set(_BSD_SAN_UBSAN_GNU
    -fsanitize=undefined
    -fno-sanitize-recover=undefined
)

set(_BSD_SAN_TSAN_GNU
    -fsanitize=thread
)

set(_BSD_SAN_MSAN_GNU
    -fsanitize=memory
    -fsanitize-memory-track-origins=2
)

set(_BSD_SAN_ASAN_MSVC
    /fsanitize=address
)

set(_BSD_COVERAGE_GCC
    --coverage
)

set(_BSD_COVERAGE_CLANG
    -fprofile-instr-generate
    -fcoverage-mapping
)

# Validate that no two mutually-exclusive sanitizers are enabled together
# and that any compiler-specific constraints are met.
function(_bsd_validate_sanitizers)
    set(active 0)
    if(BSD_ENABLE_ASAN)
        math(EXPR active "${active} + 1")
    endif()
    if(BSD_ENABLE_TSAN)
        math(EXPR active "${active} + 1")
    endif()
    if(BSD_ENABLE_MSAN)
        math(EXPR active "${active} + 1")
    endif()

    if(active GREATER 1)
        message(FATAL_ERROR
            "BSD: AddressSanitizer, ThreadSanitizer, and MemorySanitizer are "
            "mutually exclusive; enable at most one of "
            "BSD_ENABLE_ASAN / BSD_ENABLE_TSAN / BSD_ENABLE_MSAN."
        )
    endif()

    if(BSD_ENABLE_MSAN AND NOT CMAKE_CXX_COMPILER_ID MATCHES "Clang")
        message(FATAL_ERROR
            "BSD: MemorySanitizer (BSD_ENABLE_MSAN) is only supported on Clang; "
            "current compiler is ${CMAKE_CXX_COMPILER_ID}."
        )
    endif()

    if(MSVC)
        foreach(unsupported BSD_ENABLE_UBSAN BSD_ENABLE_TSAN BSD_ENABLE_MSAN)
            if(${unsupported})
                message(STATUS
                    "BSD: ${unsupported} is not supported on MSVC and will be ignored."
                )
            endif()
        endforeach()
    endif()
endfunction()

function(bsd_set_sanitizers target)
    _bsd_validate_sanitizers()

    set(compile_flags "")
    set(link_flags "")

    if(MSVC)
        if(BSD_ENABLE_ASAN)
            list(APPEND compile_flags ${_BSD_SAN_ASAN_MSVC})
        endif()
    else()
        if(BSD_ENABLE_ASAN OR BSD_ENABLE_UBSAN OR BSD_ENABLE_TSAN OR BSD_ENABLE_MSAN)
            list(APPEND compile_flags ${_BSD_SAN_COMMON_GNU})
            list(APPEND link_flags    ${_BSD_SAN_COMMON_GNU})
        endif()
        if(BSD_ENABLE_ASAN)
            list(APPEND compile_flags ${_BSD_SAN_ASAN_GNU})
            list(APPEND link_flags    ${_BSD_SAN_ASAN_GNU})
        endif()
        if(BSD_ENABLE_UBSAN)
            list(APPEND compile_flags ${_BSD_SAN_UBSAN_GNU})
            list(APPEND link_flags    ${_BSD_SAN_UBSAN_GNU})
        endif()
        if(BSD_ENABLE_TSAN)
            list(APPEND compile_flags ${_BSD_SAN_TSAN_GNU})
            list(APPEND link_flags    ${_BSD_SAN_TSAN_GNU})
        endif()
        if(BSD_ENABLE_MSAN)
            list(APPEND compile_flags ${_BSD_SAN_MSAN_GNU})
            list(APPEND link_flags    ${_BSD_SAN_MSAN_GNU})
        endif()
    endif()

    if(BSD_ENABLE_COVERAGE)
        if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
            list(APPEND compile_flags ${_BSD_COVERAGE_GCC})
            list(APPEND link_flags    ${_BSD_COVERAGE_GCC})
        elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
            list(APPEND compile_flags ${_BSD_COVERAGE_CLANG})
            list(APPEND link_flags    ${_BSD_COVERAGE_CLANG})
        else()
            message(WARNING
                "BSD: code-coverage instrumentation is not wired up for "
                "${CMAKE_CXX_COMPILER_ID}; BSD_ENABLE_COVERAGE will be ignored."
            )
        endif()
    endif()

    if(compile_flags)
        target_compile_options(${target} PRIVATE ${compile_flags})
    endif()
    if(link_flags)
        target_link_options(${target} PRIVATE ${link_flags})
    endif()
endfunction()
