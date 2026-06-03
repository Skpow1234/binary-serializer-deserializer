# tl::expected — vendored under third_party/tl/ (tag v1.1.0).
#
# Kept in-tree (not FetchContent) so CI and offline builds never depend on
# network access or a working <expected> from the platform standard library.

include_guard(GLOBAL)

set(_bsd_tl_expected_root "${CMAKE_CURRENT_LIST_DIR}/../third_party")

if(NOT EXISTS "${_bsd_tl_expected_root}/tl/expected.hpp")
    message(FATAL_ERROR
        "bsd: missing vendored header at ${_bsd_tl_expected_root}/tl/expected.hpp"
    )
endif()

add_library(bsd_tl_expected INTERFACE)
add_library(tl::expected ALIAS bsd_tl_expected)
target_include_directories(bsd_tl_expected INTERFACE "${_bsd_tl_expected_root}")
