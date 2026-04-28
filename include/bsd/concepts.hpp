// SPDX-License-Identifier: MIT
//
// concepts.hpp -- public concept stubs.
//
// Milestone 03 introduces the *surface* of the trait/concept layer so that
// later milestones (M08 primitives, M12 user-type customization point, M13+
// containers) can plug their support in by specialising the underlying trait.
//
// The full functional definition arrives in Milestone 12, where the
// `bsd::serialize` / `bsd::deserialize` `tag_invoke` customization points are
// introduced and the concepts are rewritten in terms of them.
//
// @invariant `is_serializable<T>` and `is_deserializable<T>` are the unique
//            opt-in points. Specialising them is the only supported way to
//            advertise (de)serialization support for a type.
// @invariant Both traits default to false; types are not silently
//            (de)serializable.
// @invariant The `Serializable` / `Deserializable` concepts are exact aliases
//            of the corresponding `*_v` variable templates and add no extra
//            requirements at this stage.

#ifndef BSD_CONCEPTS_HPP
#define BSD_CONCEPTS_HPP

#include <bsd/fwd.hpp>

#include <type_traits>

namespace bsd::inline v1 {

template <class T>
struct is_serializable : std::false_type {};

template <class T>
struct is_deserializable : std::false_type {};

template <class T>
inline constexpr bool is_serializable_v = is_serializable<T>::value;

template <class T>
inline constexpr bool is_deserializable_v = is_deserializable<T>::value;

template <class T>
concept Serializable = is_serializable_v<T>;

template <class T>
concept Deserializable = is_deserializable_v<T>;

// Convenience: a type is fully (de)serializable when both directions are
// supported. Most built-in adapters will satisfy both, so this concept is
// what the high-level encode/decode entry points (M07+) will require.
template <class T>
concept Codec = Serializable<T> && Deserializable<T>;

} // namespace bsd::inline v1

#endif // BSD_CONCEPTS_HPP
