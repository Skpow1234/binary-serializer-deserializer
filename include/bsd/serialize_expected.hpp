// SPDX-License-Identifier: MIT
//
// serialize_expected.hpp -- `std::expected` aliases and monadic macros.
//
// This file is intentionally *not* named `expected.hpp`: on several
// toolchains `#include <expected>` from a header whose path ends in
// `.../expected.hpp` incorrectly resolves the angle-bracket include back to
// that same file, skipping the standard library entirely. The result is that
// `std::expected` appears "missing" even though `-std=c++23` is in effect.
// Keeping a distinct basename avoids that entire class of bugs.
//
// @invariant `expected<T>` is always `std::expected<T, serialize_error>`.
//            No other error type is permitted at this abstraction boundary.
// @invariant `BSD_TRY` / `BSD_TRYV` may only appear inside a function whose
//            return type is `expected<U>` for some `U` (including `void`),
//            because failure is propagated via `return std::unexpected(...)`.

#ifndef BSD_SERIALIZE_EXPECTED_HPP
#define BSD_SERIALIZE_EXPECTED_HPP

#include <bsd/error.hpp>

#include <expected>
#include <type_traits>
#include <utility>
#include <version>

namespace bsd::inline v1 {

template <class T>
using expected = std::expected<T, serialize_error>;

using unexpected_serialize = std::unexpected<serialize_error>;

[[nodiscard]] constexpr unexpected_serialize make_unexpected(serialize_error e) noexcept {
    return unexpected_serialize{e};
}

} // namespace bsd::inline v1

#define BSD_TRY_PRIVATE_CAT(a, b) a##b
#define BSD_TRY_PRIVATE_LINE(a)  BSD_TRY_PRIVATE_CAT(a, __LINE__)

// Evaluates `expr` (an `expected<T, serialize_error>`). On success, binds the
// unwrapped value to `var` (via `auto var = …`). On failure, returns
// `std::unexpected(error)` from the enclosing function.
//
// Example:
//     bsd::expected<int> read_int();
//     bsd::expected<long> compose() {
//         BSD_TRY(n, read_int());
//         return static_cast<long>(n) * 2;
//     }
#define BSD_TRY(var, expr)                                                                         \
    auto BSD_TRY_PRIVATE_LINE(_bsd_try_) = (expr);                                                 \
    if (!BSD_TRY_PRIVATE_LINE(_bsd_try_)) [[unlikely]]                                             \
        return ::std::unexpected(BSD_TRY_PRIVATE_LINE(_bsd_try_).error());                         \
    auto var = ::std::move(*BSD_TRY_PRIVATE_LINE(_bsd_try_))

// Like `BSD_TRY`, but for `expected<void, serialize_error>`: no binding, only
// early-return propagation.
#define BSD_TRYV(expr)                                                                               \
    do {                                                                                           \
        auto BSD_TRY_PRIVATE_LINE(_bsd_tryv_) = (expr);                                            \
        if (!BSD_TRY_PRIVATE_LINE(_bsd_tryv_)) [[unlikely]]                                        \
            return ::std::unexpected(BSD_TRY_PRIVATE_LINE(_bsd_tryv_).error());                    \
    } while (false)

#endif // BSD_SERIALIZE_EXPECTED_HPP
