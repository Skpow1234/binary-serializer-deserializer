# Project-wide CMake options.
#
# Every tunable that affects the build lives here so it can be discovered with
# `cmake -LH` and overridden from the command line or a preset. Per the project
# rules, no defaults are hardcoded inside targets.

include_guard(GLOBAL)

option(BSD_BUILD_EXAMPLES   "Build example programs"                ${BSD_IS_TOPLEVEL})
option(BSD_BUILD_TESTS      "Build the unit-test suite"             ${BSD_IS_TOPLEVEL})
option(BSD_BUILD_BENCHMARKS "Build the micro-benchmark suite"       OFF) # M35
option(BSD_BUILD_FUZZERS    "Build the libFuzzer harnesses"         OFF) # M33
option(BSD_BUILD_TOOLS      "Build CLI tools (bsd-dump, ...)"       OFF) # M37

option(BSD_WARNINGS_AS_ERRORS "Treat compiler warnings as errors"   ON)

# Sanitizer toggles are wired up in Milestone 02; declared here so the option
# surface is centralised and consumers see them with `cmake -LH`.
option(BSD_ENABLE_ASAN     "Enable AddressSanitizer"                OFF)
option(BSD_ENABLE_UBSAN    "Enable UndefinedBehaviorSanitizer"      OFF)
option(BSD_ENABLE_TSAN     "Enable ThreadSanitizer"                 OFF)
option(BSD_ENABLE_MSAN     "Enable MemorySanitizer (Clang only)"    OFF)
option(BSD_ENABLE_COVERAGE "Enable code-coverage instrumentation"   OFF)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE "RelWithDebInfo" CACHE STRING
        "Choose the build type" FORCE)
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS
        "Debug" "Release" "RelWithDebInfo" "MinSizeRel")
endif()

if(NOT DEFINED CMAKE_CXX_EXTENSIONS)
    set(CMAKE_CXX_EXTENSIONS OFF)
endif()

if(NOT DEFINED CMAKE_EXPORT_COMPILE_COMMANDS)
    set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
endif()
