# Configure a C++23-capable standard library when using Clang on Linux.
#
# Ubuntu's `clang++-18` package often defaults to an older libstdc++ (from the
# system default gcc) that does not implement `std::expected`, even with
# `-std=c++23`. The reliable fix is to append `--gcc-toolchain=…` to the global
# `CMAKE_CXX_FLAGS` / linker flags so every target (including executables that
# only link the static `bsd` library) sees it on the compile line.

include_guard(GLOBAL)

if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang"
   AND CMAKE_SYSTEM_NAME STREQUAL "Linux"
   AND NOT APPLE
   AND NOT WIN32)

    set(_bsd_gxx "")
    foreach(_candidate g++-15 g++-14 g++-13 g++-12)
        find_program(_bsd_gxx NAMES "${_candidate}" PATHS /usr/bin /usr/local/bin)
        if(_bsd_gxx)
            break()
        endif()
    endforeach()

    if(_bsd_gxx)
        get_filename_component(_bsd_gxx_bin "${_bsd_gxx}" DIRECTORY)
        get_filename_component(_bsd_gcc_toolchain "${_bsd_gxx_bin}" DIRECTORY)
        set(_bsd_tc_flag "--gcc-toolchain=${_bsd_gcc_toolchain}")

        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${_bsd_tc_flag}"
            CACHE STRING "Flags used by the compiler during all build types." FORCE)
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${_bsd_tc_flag}"
            CACHE STRING "Flags used by the linker during executable creation." FORCE)
        set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} ${_bsd_tc_flag}"
            CACHE STRING "Flags used by the linker during shared library creation." FORCE)
        set(CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} ${_bsd_tc_flag}"
            CACHE STRING "Flags used by the linker during module creation." FORCE)

        set(BSD_CXX_STDLIB_COMPILE_OPTIONS "${_bsd_tc_flag}" CACHE INTERNAL "bsd stdlib compile flags")
        message(STATUS
            "bsd: Clang will use libstdc++ from ${_bsd_gxx} (${_bsd_tc_flag} on CMAKE_CXX_FLAGS)"
        )
    else()
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
            set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++"
                CACHE STRING "Flags used by the compiler during all build types." FORCE)
            foreach(_link_var CMAKE_EXE_LINKER_FLAGS CMAKE_SHARED_LINKER_FLAGS CMAKE_MODULE_LINKER_FLAGS)
                set(${_link_var} "${${_link_var}} -stdlib=libc++ -lc++abi"
                    CACHE STRING "" FORCE)
            endforeach()
            set(BSD_CXX_STDLIB_COMPILE_OPTIONS "-stdlib=libc++" CACHE INTERNAL "bsd stdlib compile flags")
            message(STATUS "bsd: Clang will use libc++ (${_bsd_libcxx})")
        else()
            message(WARNING
                "bsd: could not locate g++-13+ or libc++ for Clang on Linux. "
                "Install g++-13 and libstdc++-13-dev, or libc++-18-dev and libc++abi-18-dev."
            )
        endif()
    endif()
endif()

function(bsd_apply_cxx_stdlib target visibility)
    if(BSD_CXX_STDLIB_COMPILE_OPTIONS)
        target_compile_options(${target} ${visibility} ${BSD_CXX_STDLIB_COMPILE_OPTIONS})
    endif()
endfunction()

function(bsd_apply_cxx_stdlib_directory)
    # Global CMAKE_CXX_FLAGS already updated above; nothing else required.
endfunction()
