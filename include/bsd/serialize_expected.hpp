// SPDX-License-Identifier: MIT
//
// serialize_expected.hpp -- `expected` aliases and monadic propagation macros.
//
// We use tl::expected (vendored via CMake) instead of std::expected so builds
// stay portable on Clang + Ubuntu CI, where <expected> is often missing from
// the default libstdc++ even under -std=c++23. The public API matches
// std::expected closely enough for all library code paths.
//
// @invariant `expected<T>` is always `tl::expected<T, serialize_error>`.
//            No other error type is permitted at this abstraction boundary.
// @invariant `BSD_TRY` / `BSD_TRYV` may only appear inside a function whose
//            return type is `expected<U>` for some `U` (including `void`),
//            because failure is propagated via `return tl::unexpected(...)`.

#ifndef BSD_SERIALIZE_EXPECTED_HPP
#define BSD_SERIALIZE_EXPECTED_HPP

#include <bsd/error.hpp>

#include <tl/expected.hpp>

#include <type_traits>
#include <utility>

namespace bsd::inline v1 {

template <class T>
using expected = tl::expected<T, serialize_error>;

using unexpected_serialize = tl::unexpected<serialize_error>;

[[nodiscard]] constexpr unexpected_serialize make_unexpected(serialize_error e) noexcept {
    return unexpected_serialize{tl::in_place, e};
}

} // namespace bsd::inline v1

#define BSD_TRY_PRIVATE_CAT(a, b) a##b
#define BSD_TRY_PRIVATE_LINE(a)  BSD_TRY_PRIVATE_CAT(a, __LINE__)

// Evaluates `expr` (an `expected<T, serialize_error>`). On success, binds the
// unwrapped value to `var` (via `auto var = …`). On failure, returns
// `tl::unexpected(error)` from the enclosing function.
#define BSD_TRY(var, expr)                                                                         \
    auto BSD_TRY_PRIVATE_LINE(_bsd_try_) = (expr);                                                 \
    if (!BSD_TRY_PRIVATE_LINE(_bsd_try_)) [[unlikely]]                                             \
        return ::tl::unexpected(BSD_TRY_PRIVATE_LINE(_bsd_try_).error());                         \
    auto var = ::std::move(*BSD_TRY_PRIVATE_LINE(_bsd_try_))

// Like `BSD_TRY`, but for `expected<void, serialize_error>`: no binding, only
// early-return propagation.
#define BSD_TRYV(expr)                                                                               \
    do {                                                                                           \
        auto BSD_TRY_PRIVATE_LINE(_bsd_tryv_) = (expr);                                            \
        if (!BSD_TRY_PRIVATE_LINE(_bsd_tryv_)) [[unlikely]]                                        \
            return ::tl::unexpected(BSD_TRY_PRIVATE_LINE(_bsd_tryv_).error());                    \
    } while (false)

#endif // BSD_SERIALIZE_EXPECTED_HPP
