// SPDX-License-Identifier: MIT
//
// Milestone 03 smoke example.
//
// Includes the umbrella header so any header-level regression breaks the
// warnings-as-errors build, and prints a few configuration values so a human
// (or CI) can confirm the binary actually executed and the configuration
// surface is reachable.
//
// The static_asserts below are the *only* place outside config.hpp where
// these constants are read; the example exists precisely so that the
// compile-time invariants get exercised in every preset.

#include <bsd/bsd.hpp>

#include <cstdio>

namespace {

// Sanity-check that the public type vocabulary is reachable through the
// umbrella header alone.
static_assert(bsd::config::magic_bytes.size()       == 4);
static_assert(bsd::config::default_max_depth        > 0);
static_assert(bsd::config::default_max_container    > 0);
static_assert(bsd::config::default_max_string       > 0);
static_assert(bsd::config::wire_format_version      > 0);
static_assert(bsd::config::default_endian_mode      != bsd::endian_mode::native);

// Concept stubs default to false; this confirms the surface compiles and is
// honest about the (currently empty) set of supported types.
static_assert(!bsd::Serializable<int>);
static_assert(!bsd::Deserializable<int>);
static_assert(!bsd::Codec<int>);

// Error vocabulary is constexpr-friendly all the way through.
static_assert(bsd::to_string(bsd::serialize_error::none) == "none");
static_assert(bsd::to_string(bsd::serialize_error::truncated_input) == "truncated_input");

} // namespace

int main() {
    constexpr auto v = bsd::library_version;
    std::printf(
        "bsd %u.%u.%u (wire format v%u, %.*s)\n",
        v.major,
        v.minor,
        v.patch,
        static_cast<unsigned>(bsd::config::wire_format_version),
        static_cast<int>(bsd::library_version_string.size()),
        bsd::library_version_string.data());
    std::printf(
        "  defaults: max_container=%zu, max_string=%zu, max_depth=%zu, io_chunk=%zu\n",
        bsd::config::default_max_container,
        bsd::config::default_max_string,
        bsd::config::default_max_depth,
        bsd::config::default_io_chunk_size);
    return 0;
}
