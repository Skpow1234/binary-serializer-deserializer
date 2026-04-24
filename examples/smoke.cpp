// SPDX-License-Identifier: MIT
//
// Milestone 01 smoke example.
//
// The only purpose of this translation unit is to (a) include the umbrella
// header so that any header-level regression breaks the warnings-as-errors
// build, and (b) print the library version so a human can confirm the binary
// actually executed end-to-end. It deliberately performs no library work
// because no functional API exists yet.

#include <bsd/bsd.hpp>

#include <cstdio>

int main() {
    constexpr auto v = bsd::library_version;
    std::printf(
        "bsd %u.%u.%u (%.*s)\n",
        v.major,
        v.minor,
        v.patch,
        static_cast<int>(bsd::library_version_string.size()),
        bsd::library_version_string.data());
    return 0;
}
