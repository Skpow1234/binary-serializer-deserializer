# tl::expected — single-header, API-compatible with std::expected.
#
# Populated via FetchContent without building upstream's test suite.

include_guard(GLOBAL)
include(FetchContent)

FetchContent_Declare(
    tl_expected
    GIT_REPOSITORY https://github.com/TartanLlama/expected.git
    GIT_TAG        v1.1.0
    GIT_SHALLOW    TRUE
)

FetchContent_GetProperties(tl_expected)
if(NOT tl_expected_POPULATED)
    FetchContent_Populate(tl_expected)
endif()

add_library(bsd_tl_expected INTERFACE)
add_library(tl::expected ALIAS bsd_tl_expected)
target_include_directories(
    bsd_tl_expected
    INTERFACE
        "${tl_expected_SOURCE_DIR}/include"
)
