// SPDX-License-Identifier: MIT
//
// config.hpp -- single source of truth for every compile-time tunable.
//
// Every default value the library uses (size limits, buffer sizes, magic
// bytes, version numbers, default endianness, sentinel byte values, ...) is
// declared in this header *and nowhere else*. Implementation code must
// reference these constants by name; literal magic numbers anywhere outside
// this file are a project-rule violation.
//
// Each default is overridable by defining the matching `BSD_CONFIG_*` macro
// before including any bsd header. This lets downstream projects re-tune the
// library without forking it.
//
// @invariant Every tunable is declared here exactly once. There is no other
//            place in the codebase where these values may be re-introduced
//            as a literal.
// @invariant Every override macro carries a default; consumers may safely
//            include this header without defining anything.
// @invariant All limits are expressed in their natural unit (bytes, elements,
//            nesting levels, ...) and documented inline.
// @invariant `wire_format_version` is independent of `library_version` (see
//            version.hpp): the library version may change without the wire
//            format changing, and vice versa.

#ifndef BSD_CONFIG_HPP
#define BSD_CONFIG_HPP

#include <bsd/fwd.hpp>

#include <array>
#include <cstddef>
#include <cstdint>

// ---------------------------------------------------------------------------
// Wire-format identity
// ---------------------------------------------------------------------------

#ifndef BSD_CONFIG_WIRE_FORMAT_VERSION
#  define BSD_CONFIG_WIRE_FORMAT_VERSION 1u
#endif

#ifndef BSD_CONFIG_MAGIC_BYTE_0
#  define BSD_CONFIG_MAGIC_BYTE_0 0x42 /* 'B' */
#endif
#ifndef BSD_CONFIG_MAGIC_BYTE_1
#  define BSD_CONFIG_MAGIC_BYTE_1 0x53 /* 'S' */
#endif
#ifndef BSD_CONFIG_MAGIC_BYTE_2
#  define BSD_CONFIG_MAGIC_BYTE_2 0x44 /* 'D' */
#endif
#ifndef BSD_CONFIG_MAGIC_BYTE_3
#  define BSD_CONFIG_MAGIC_BYTE_3 0x31 /* '1' */
#endif

// ---------------------------------------------------------------------------
// Default safety limits (DoS hardening, finalised in M32)
// ---------------------------------------------------------------------------

// Maximum number of elements admitted by a length-prefixed container decode.
#ifndef BSD_CONFIG_DEFAULT_MAX_CONTAINER
#  define BSD_CONFIG_DEFAULT_MAX_CONTAINER (16u * 1024u * 1024u)
#endif

// Maximum length in bytes admitted by a length-prefixed string decode.
#ifndef BSD_CONFIG_DEFAULT_MAX_STRING
#  define BSD_CONFIG_DEFAULT_MAX_STRING (16u * 1024u * 1024u)
#endif

// Maximum nesting depth admitted before a decode bails with depth_exceeded.
// Exists to bound stack usage on hostile input (recursive variants/tuples).
#ifndef BSD_CONFIG_DEFAULT_MAX_DEPTH
#  define BSD_CONFIG_DEFAULT_MAX_DEPTH 64u
#endif

// ---------------------------------------------------------------------------
// I/O & encoding tunables
// ---------------------------------------------------------------------------

// Default buffer chunk size used by streaming writers/readers.
#ifndef BSD_CONFIG_DEFAULT_IO_CHUNK_SIZE
#  define BSD_CONFIG_DEFAULT_IO_CHUNK_SIZE (64u * 1024u)
#endif

// Default endianness of the wire format. Default is little because little is
// the dominant host endianness in 2026, which lets the fast paths skip the
// byte-swap step on most targets.
#ifndef BSD_CONFIG_DEFAULT_ENDIAN_MODE
#  define BSD_CONFIG_DEFAULT_ENDIAN_MODE ::bsd::v1::endian_mode::little
#endif

// Sentinel byte values used by the strict bool codec (M08).
#ifndef BSD_CONFIG_BOOL_FALSE_BYTE
#  define BSD_CONFIG_BOOL_FALSE_BYTE 0x00
#endif
#ifndef BSD_CONFIG_BOOL_TRUE_BYTE
#  define BSD_CONFIG_BOOL_TRUE_BYTE 0x01
#endif

// ---------------------------------------------------------------------------
// Reflection / aggregate-helper limits (consumed in M20)
// ---------------------------------------------------------------------------

// Maximum aggregate field arity supported by the structured-bindings-based
// reflection helper. Larger aggregates require the explicit BSD_FIELDS macro.
#ifndef BSD_CONFIG_MAX_AGGREGATE_ARITY
#  define BSD_CONFIG_MAX_AGGREGATE_ARITY 64u
#endif

// ---------------------------------------------------------------------------
// Inline-constant interface
//
// Consumers should always reach for these `bsd::config::*` symbols rather
// than the BSD_CONFIG_* macros, which exist purely as the override surface.
// ---------------------------------------------------------------------------

namespace bsd::inline v1::config {

// Number of bytes an unsigned LEB128 varint can occupy for a 64-bit payload:
// ceil(64 / 7) == 10. Mathematically derived; declared here so the rest of
// the codebase never spells the number out.
inline constexpr std::size_t max_varint_bytes_64 = 10;

// Same derivation for 32-bit payloads: ceil(32 / 7) == 5.
inline constexpr std::size_t max_varint_bytes_32 = 5;

inline constexpr std::uint16_t wire_format_version =
    static_cast<std::uint16_t>(BSD_CONFIG_WIRE_FORMAT_VERSION);

// 4-byte magic that introduces a framed bsd payload.
// @invariant `magic_bytes.size()` is fixed at 4 by the wire format.
inline constexpr std::array<std::byte, 4> magic_bytes{
    std::byte{BSD_CONFIG_MAGIC_BYTE_0},
    std::byte{BSD_CONFIG_MAGIC_BYTE_1},
    std::byte{BSD_CONFIG_MAGIC_BYTE_2},
    std::byte{BSD_CONFIG_MAGIC_BYTE_3},
};

inline constexpr std::size_t default_max_container =
    static_cast<std::size_t>(BSD_CONFIG_DEFAULT_MAX_CONTAINER);

inline constexpr std::size_t default_max_string =
    static_cast<std::size_t>(BSD_CONFIG_DEFAULT_MAX_STRING);

inline constexpr std::size_t default_max_depth =
    static_cast<std::size_t>(BSD_CONFIG_DEFAULT_MAX_DEPTH);

inline constexpr std::size_t default_io_chunk_size =
    static_cast<std::size_t>(BSD_CONFIG_DEFAULT_IO_CHUNK_SIZE);

inline constexpr endian_mode default_endian_mode = BSD_CONFIG_DEFAULT_ENDIAN_MODE;

inline constexpr std::byte bool_false_byte = std::byte{BSD_CONFIG_BOOL_FALSE_BYTE};
inline constexpr std::byte bool_true_byte  = std::byte{BSD_CONFIG_BOOL_TRUE_BYTE};

inline constexpr std::size_t max_aggregate_arity =
    static_cast<std::size_t>(BSD_CONFIG_MAX_AGGREGATE_ARITY);

// ---------------------------------------------------------------------------
// Structural invariants enforced at compile time
// ---------------------------------------------------------------------------

static_assert(default_max_depth     >  0,                      "depth limit must be positive");
static_assert(default_max_container >  0,                      "container limit must be positive");
static_assert(default_max_string    >  0,                      "string limit must be positive");
static_assert(default_io_chunk_size >  0,                      "I/O chunk size must be positive");
static_assert(max_varint_bytes_64   == 10,                     "max LEB128 length for uint64 is 10 bytes");
static_assert(max_varint_bytes_32   == 5,                      "max LEB128 length for uint32 is 5 bytes");
static_assert(magic_bytes.size()    == 4,                      "wire format requires a 4-byte magic");
static_assert(bool_true_byte != bool_false_byte,               "bool sentinel bytes must differ");
static_assert(default_endian_mode != endian_mode::native,
              "default endianness must be a concrete endianness, never `native`");

} // namespace bsd::inline v1::config

#endif // BSD_CONFIG_HPP
