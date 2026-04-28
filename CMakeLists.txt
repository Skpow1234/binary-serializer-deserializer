cmake_minimum_required(VERSION 3.26)

# -----------------------------------------------------------------------------
# Project metadata
#
# The project version is the single source of truth for the library version
# and is propagated to C++ via a configured header (see include/bsd/version.hpp.in).
# Per the project rules: do not hardcode the version anywhere else.
# -----------------------------------------------------------------------------
project(bsd
    VERSION      0.1.0
    DESCRIPTION  "Binary Serializer / Deserializer for modern C++23"
    HOMEPAGE_URL "https://example.invalid/bsd"
    LANGUAGES    CXX
)

# Detect whether we are the top-level project (controls defaults for examples,
# install rules, etc.). Consumers via add_subdirectory() should not be surprised.
if(CMAKE_VERSION VERSION_GREATER_EQUAL 3.21)
    set(BSD_IS_TOPLEVEL "${PROJECT_IS_TOP_LEVEL}")
else()
    string(COMPARE EQUAL "${CMAKE_SOURCE_DIR}" "${PROJECT_SOURCE_DIR}" BSD_IS_TOPLEVEL)
endif()

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

include(ProjectOptions)
include(CompilerWarnings)
include(Sanitizers)

# -----------------------------------------------------------------------------
# Public interface library (header-only for now; M03+ may add compiled units).
# -----------------------------------------------------------------------------
add_library(bsd INTERFACE)
add_library(bsd::bsd ALIAS bsd)

target_compile_features(bsd INTERFACE cxx_std_23)

target_include_directories(bsd INTERFACE
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>"
    "$<INSTALL_INTERFACE:include>"
)

# Generate include/bsd/version.hpp from the project version (no hardcoding).
configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/include/bsd/version.hpp.in"
    "${CMAKE_CURRENT_BINARY_DIR}/include/bsd/version.hpp"
    @ONLY
)

# -----------------------------------------------------------------------------
# Optional sub-projects (each guarded so consumers can opt out)
# -----------------------------------------------------------------------------
if(BSD_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

# tests/, fuzz/, benchmarks/, tools/ land in later milestones.

# -----------------------------------------------------------------------------
# Install / packaging are deferred to Milestone 36.
# -----------------------------------------------------------------------------
