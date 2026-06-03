# tl::expected — single-header, API-compatible with std::expected.
#
# Used instead of <expected> so Clang on Ubuntu CI (old libstdc++ by default)
# does not break builds. Pin the tag; no hardcoded paths in consumer code.

include_guard(GLOBAL)
include(FetchContent)

FetchContent_Declare(
    tl_expected
    GIT_REPOSITORY https://github.com/TartanLlama/expected.git
    GIT_TAG        v1.1.0
    GIT_SHALLOW    TRUE
)

FetchContent_MakeAvailable(tl_expected)
