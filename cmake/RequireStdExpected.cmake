# Verify that the selected C++ toolchain + standard library actually provide
# `std::expected` in C++23 mode. This fails fast at configure time instead of
# halfway through a sanitizer build with a cryptic "no template named expected".

include_guard(GLOBAL)
include(CheckCXXSourceCompiles)

set(_bsd_std_expected_probe [[
#include <expected>
int main() {
    std::expected<int, int> e{42};
    return *e;
}
]])

set(_bsd_saved_required_flags "${CMAKE_REQUIRED_FLAGS}")
if(MSVC)
    list(APPEND CMAKE_REQUIRED_FLAGS "/std:c++latest")
else()
    list(APPEND CMAKE_REQUIRED_FLAGS "-std=c++23")
endif()

check_cxx_source_compiles("${_bsd_std_expected_probe}" BSD_HAS_STD_EXPECTED)

set(CMAKE_REQUIRED_FLAGS "${_bsd_saved_required_flags}")

if(NOT BSD_HAS_STD_EXPECTED)
    message(FATAL_ERROR
        "This compiler / standard library does not provide std::expected in "
        "C++23 mode. Use GCC >= 12 with libstdc++ >= 12, or Clang with a "
        "recent libstdc++ or libc++."
    )
endif()
