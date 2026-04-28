// SPDX-License-Identifier: MIT
//
// error.hpp -- error reporting vocabulary.
//
// Milestone 03 introduces only the enum and a constexpr `to_string` for
// printable diagnostics. The full error infrastructure (`std::error_category`
// adapter, `std::expected` aliases, `BSD_TRY` propagation macro, exhaustive
// unit tests) lands in Milestone 04.
//
// @invariant `serialize_error::none` is the zero value and represents
//            "no error". All other enumerators denote a failure mode and
//            must be non-zero.
// @invariant The numeric value of every named enumerator is part of the
//            wire-stable error contract; new errors append to the end of the
//            enum, never reorder existing ones.
// @invariant `to_string(e)` is total: every defined enumerator yields a
//            non-empty diagnostic string.

#ifndef BSD_ERROR_HPP
#define BSD_ERROR_HPP

#include <bsd/fwd.hpp>

#include <cstdint>
#include <string_view>

namespace bsd::inline v1 {

enum class serialize_error : std::uint16_t {
    none = 0,
    out_of_space,
    truncated_input,
    invalid_tag,
    invalid_bool,
    invalid_utf8,
    depth_exceeded,
    size_limit_exceeded,
    checksum_mismatch,
    version_mismatch,
    unknown_type,
    io_error,
    compression_error,
    not_supported,
};

// Total mapping from enumerator to a stable, human-readable string. Used by
// the future error_category (M04) and by ad-hoc diagnostics. The strings are
// intentionally short and stable; they are part of the public API.
[[nodiscard]] constexpr std::string_view to_string(serialize_error e) noexcept {
    using enum serialize_error;
    switch (e) {
        case none:                return "none";
        case out_of_space:        return "out_of_space";
        case truncated_input:     return "truncated_input";
        case invalid_tag:         return "invalid_tag";
        case invalid_bool:        return "invalid_bool";
        case invalid_utf8:        return "invalid_utf8";
        case depth_exceeded:      return "depth_exceeded";
        case size_limit_exceeded: return "size_limit_exceeded";
        case checksum_mismatch:   return "checksum_mismatch";
        case version_mismatch:    return "version_mismatch";
        case unknown_type:        return "unknown_type";
        case io_error:            return "io_error";
        case compression_error:   return "compression_error";
        case not_supported:       return "not_supported";
    }
    // Defensive default: covers any enumerator added later but not yet
    // reflected here. Keeps to_string total at runtime even if a header is
    // out of sync with its translation unit; the build still surfaces a
    // -Wswitch-enum warning when one of the cases above is forgotten.
    return "unknown_serialize_error";
}

} // namespace bsd::inline v1

#endif // BSD_ERROR_HPP
