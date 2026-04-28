// SPDX-License-Identifier: MIT
//
// error.hpp -- error reporting vocabulary.
//
// Milestone 04 adds `std::error_category` integration (`make_error_code`,
// `serialize_error_category`) so `serialize_error` interoperates with
// `std::error_code` / `std::error_condition`. Aliases and the `BSD_TRY` macros
// live in `expected.hpp` to keep this header focused on the enum + category.
//
// @invariant `serialize_error::none` is the zero value and represents
//            "no error". All other enumerators denote a failure mode and
//            must be non-zero.
// @invariant The numeric value of every named enumerator is part of the
//            wire-stable error contract; new errors append to the end of the
//            enum, never reorder existing ones.
// @invariant `to_string(e)` is total: every defined enumerator yields a
//            non-empty diagnostic string.
// @invariant `serialize_error_category()` returns a singleton whose address is
//            stable for the lifetime of the program; all `std::error_code`
//            values produced by `make_error_code` share that category.

#ifndef BSD_ERROR_HPP
#define BSD_ERROR_HPP

#include <bsd/fwd.hpp>

#include <cstdint>
#include <string_view>
#include <system_error>

namespace bsd::inline v1 {

// Explicit underlying values keep the wire / persistence contract stable
// across compiler releases. `none` is always 0; every failure is non-zero.
enum class serialize_error : std::uint16_t {
    none                = 0,
    out_of_space        = 1,
    truncated_input     = 2,
    invalid_tag         = 3,
    invalid_bool        = 4,
    invalid_utf8        = 5,
    depth_exceeded      = 6,
    size_limit_exceeded = 7,
    checksum_mismatch   = 8,
    version_mismatch    = 9,
    unknown_type        = 10,
    io_error            = 11,
    compression_error   = 12,
    not_supported       = 13,
};

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
    return "unknown_serialize_error";
}

// Singleton category backing every `std::error_code` created from
// `serialize_error`. Implemented in `src/error_category.cpp` so the object
// identity is unique across the whole program (required by the standard's
// equality semantics for `std::error_code`).
[[nodiscard]] const std::error_category& serialize_error_category() noexcept;

[[nodiscard]] inline std::error_code make_error_code(serialize_error e) noexcept {
    return {static_cast<int>(e), serialize_error_category()};
}

} // namespace bsd::inline v1

namespace std {

template <>
struct is_error_code_enum<bsd::serialize_error> : true_type {};

} // namespace std

#endif // BSD_ERROR_HPP
