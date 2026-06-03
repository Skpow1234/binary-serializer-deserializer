# Configure a C++23-capable standard library when using Clang on Linux.
#
# Ubuntu's `clang++-18` package often defaults to an older libstdc++ (from the
# system default gcc) that does not implement `std::expected`, even with
# `-std=c++23`. Pointing Clang at a recent GCC toolchain (g++-13) fixes that.
# If no suitable GCC is found, fall back to libc++ from the LLVM install.

include_guard(GLOBAL)

function(bsd_apply_cxx_stdlib target visibility)
    if(NOT CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
        return()
    endif()

    if(APPLE OR WIN32)
        return()
    endif()

    if(NOT CMAKE_SYSTEM_NAME STREQUAL "Linux")
        return()
    endif()

    find_program(_bsd_gxx NAMES g++-15 g++-14 g++-13 g++-12)
    if(_bsd_gxx)
        get_filename_component(_bsd_gxx_bin "${_bsd_gxx}" DIRECTORY)
        get_filename_component(_bsd_gcc_toolchain "${_bsd_gxx_bin}" DIRECTORY)
        target_compile_options(${target} ${visibility}
            "--gcc-toolchain=${_bsd_gcc_toolchain}"
        )
        message(STATUS
            "bsd: Clang will use libstdc++ from ${_bsd_gxx} "
            "(--gcc-toolchain=${_bsd_gcc_toolchain})"
        )
        return()
    endif()

    find_library(_bsd_libcxx c++
        PATHS
            /usr/lib/llvm-18/lib
            /usr/lib/llvm-19/lib
            /usr/lib/llvm-20/lib
            /usr/lib/${CMAKE_LIBRARY_ARCHITECTURE}
        NO_DEFAULT_PATH
    )
    find_library(_bsd_libcxxabi c++abi
        PATHS
            /usr/lib/llvm-18/lib
            /usr/lib/llvm-19/lib
            /usr/lib/llvm-20/lib
            /usr/lib/${CMAKE_LIBRARY_ARCHITECTURE}
        NO_DEFAULT_PATH
    )
    if(_bsd_libcxx AND _bsd_libcxxabi)
        target_compile_options(${target} ${visibility} -stdlib=libc++)
        target_link_options(${target} ${visibility} -stdlib=libc++ -lc++abi)
        message(STATUS "bsd: Clang will use libc++ (${_bsd_libcxx})")
        return()
    endif()

    message(WARNING
        "bsd: could not locate g++-13+ or libc++ for Clang on Linux. "
        "Builds that include <expected> may fail; install g++-13 and "
        "libstdc++-13-dev, or libc++-18-dev and libc++abi-18-dev."
    )
endfunction()
