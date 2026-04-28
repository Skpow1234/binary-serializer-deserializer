// SPDX-License-Identifier: MIT
//
// fwd.hpp -- forward declarations and small public enums.
//
// This header is intentionally tiny so that other headers can pull in the
// public type vocabulary (writer, reader, options, endian_mode, ...) without
// dragging in their full definitions. Functional bodies live in later
// milestones (writer/reader: M07; options: M07; serializer<T>: M12).
//
// @invariant All public types of the bsd library are nameable through this
//            header alone. Adding a new public type requires forward-declaring
//            it here as well as defining it in its dedicated header.
// @invariant Public symbols live exclusively in the inline namespace `v1`,
//            so future ABI breaks rotate the inline namespace rather than
//            breaking already-built consumers.

#ifndef BSD_FWD_HPP
#define BSD_FWD_HPP

#include <cstdint>

namespace bsd::inline v1 {

// Endianness of the wire format. The library never silently assumes host
// endianness: the value used for any given encode/decode is always either
// taken from `options::endian` or a function-level argument.
//
// @invariant `endian_mode::native` resolves at compile time to `little` or
//            `big`; it is not a third encoding.
enum class endian_mode : std::uint8_t {
    little = 0,
    big    = 1,
    native = 2,
};

// Forward declarations for types defined in later milestones. Each is
// listed alongside the milestone in which the body lands.
class writer;                       // M07: stream abstraction
class reader;                       // M07: stream abstraction
struct options;                     // M07: per-call configuration

template <class T>
struct serializer;                  // M12: trait / customization-point layer

enum class serialize_error : std::uint16_t; // M03 (this milestone), see error.hpp
enum class checksum_kind   : std::uint8_t;  // M26
enum class compression_kind: std::uint8_t;  // M27

} // namespace bsd::inline v1

#endif // BSD_FWD_HPP
