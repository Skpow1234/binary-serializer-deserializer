# Configure a C++23-capable standard library when using Clang on Linux.
#
# Ubuntu's `clang++-18` package often defaults to an older libstdc++ (from the
# system default gcc) that does not implement `std::expected`, even with
# `-std=c++23`. Pointing Clang at a recent GCC toolchain (g++-13) fixes that.
# If no suitable GCC is found, fall back to libc++ from the LLVM install.
#
# Detection runs once at configure time. Flags are applied directory-wide
# (add_compile_options) so every target — including executables that only link
# the static `bsd` library — actually receives them on the compile line.

include_guard(GLOBAL)

function(_bsd_detect_cxx_stdlib)
    set(_compile "")
    set(_link "")

    if(NOT CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
        set(BSD_CXX_STDLIB_COMPILE_OPTIONS "${_compile}" PARENT_SCOPE)
        set(BSD_CXX_STDLIB_LINK_OPTIONS "${_link}" PARENT_SCOPE)
        return()
    endif()

    if(APPLE OR WIN32)
        set(BSD_CXX_STDLIB_COMPILE_OPTIONS "${_compile}" PARENT_SCOPE)
        set(BSD_CXX_STDLIB_LINK_OPTIONS "${_link}" PARENT_SCOPE)
        return()
    endif()

    if(NOT CMAKE_SYSTEM_NAME STREQUAL "Linux")
        set(BSD_CXX_STDLIB_COMPILE_OPTIONS "${_compile}" PARENT_SCOPE)
        set(BSD_CXX_STDLIB_LINK_OPTIONS "${_link}" PARENT_SCOPE)
        return()
    endif()

    find_program(_bsd_gxx
        NAMES g++-15 g++-14 g++-13 g++-12
        PATHS /usr/bin /usr/local/bin
    )
    if(_bsd_gxx)
        get_filename_component(_bsd_gxx_bin "${_bsd_gxx}" DIRECTORY)
        get_filename_component(_bsd_gcc_toolchain "${_bsd_gxx_bin}" DIRECTORY)
        list(APPEND _compile "--gcc-toolchain=${_bsd_gcc_toolchain}")
        message(STATUS
            "bsd: Clang will use libstdc++ from ${_bsd_gxx} "
            "(--gcc-toolchain=${_bsd_gcc_toolchain})"
        )
        set(BSD_CXX_STDLIB_COMPILE_OPTIONS "${_compile}" PARENT_SCOPE)
        set(BSD_CXX_STDLIB_LINK_OPTIONS "${_link}" PARENT_SCOPE)
        return()
    endif()

    find_library(_bsd_libcxx c++
        PATHS
            /usr/lib/llvm-18/lib
            /usr/lib/llvm-19/lib
            /usr/lib/llvm-20/lib
            /usr/lib/${CMAKE_LIBRARY_ARCHITECTURE}
    )
    find_library(_bsd_libcxxabi c++abi
        PATHS
            /usr/lib/llvm-18/lib
            /usr/lib/llvm-19/lib
            /usr/lib/llvm-20/lib
            /usr/lib/${CMAKE_LIBRARY_ARCHITECTURE}
    )
    if(_bsd_libcxx AND _bsd_libcxxabi)
        list(APPEND _compile -stdlib=libc++)
        list(APPEND _link -stdlib=libc++ -lc++abi)
        message(STATUS "bsd: Clang will use libc++ (${_bsd_libcxx})")
        set(BSD_CXX_STDLIB_COMPILE_OPTIONS "${_compile}" PARENT_SCOPE)
        set(BSD_CXX_STDLIB_LINK_OPTIONS "${_link}" PARENT_SCOPE)
        return()
    endif()

    message(WARNING
        "bsd: could not locate g++-13+ or libc++ for Clang on Linux. "
        "Builds that include <expected> may fail; install g++-13 and "
        "libstdc++-13-dev, or libc++-18-dev and libc++abi-18-dev."
    )
    set(BSD_CXX_STDLIB_COMPILE_OPTIONS "${_compile}" PARENT_SCOPE)
    set(BSD_CXX_STDLIB_LINK_OPTIONS "${_link}" PARENT_SCOPE)
endfunction()

# Run detection when this module is first included in a configure pass
# (`include_guard` prevents duplicate work within the same pass).
_bsd_detect_cxx_stdlib()

function(bsd_apply_cxx_stdlib target visibility)
    if(BSD_CXX_STDLIB_COMPILE_OPTIONS)
        target_compile_options(${target} ${visibility} ${BSD_CXX_STDLIB_COMPILE_OPTIONS})
    endif()
    if(BSD_CXX_STDLIB_LINK_OPTIONS)
        target_link_options(${target} ${visibility} ${BSD_CXX_STDLIB_LINK_OPTIONS})
    endif()
endfunction()

# Apply to the current directory and all subdirectories (examples, tests, …).
function(bsd_apply_cxx_stdlib_directory)
    if(BSD_CXX_STDLIB_COMPILE_OPTIONS)
        add_compile_options(${BSD_CXX_STDLIB_COMPILE_OPTIONS})
    endif()
    if(BSD_CXX_STDLIB_LINK_OPTIONS)
        add_link_options(${BSD_CXX_STDLIB_LINK_OPTIONS})
    endif()
endfunction()
