// SPDX-License-Identifier: MIT
//
// Tests for `BSD_TRY` / `BSD_TRYV` (Milestone 04).

#include <bsd/expected.hpp>

#include <gtest/gtest.h>

namespace {

using bsd::expected;
using bsd::serialize_error;

[[nodiscard]] static expected<int> returns_int(int v) { return v; }

[[nodiscard]] static expected<int> fails(serialize_error e) {
    return std::unexpected(e);
}

[[nodiscard]] static expected<long> chain_success() {
    BSD_TRY(x, returns_int(7));
    return static_cast<long>(x) * 10L;
}

[[nodiscard]] static expected<long> chain_fail() {
    BSD_TRY(x, fails(serialize_error::invalid_bool));
    (void)x;
    return 0L;
}

[[nodiscard]] static expected<void> void_ok() { return {}; }

[[nodiscard]] static expected<void> void_fail() {
    return std::unexpected(serialize_error::io_error);
}

[[nodiscard]] static expected<int> uses_void_ok() {
    BSD_TRYV(void_ok());
    return 42;
}

[[nodiscard]] static expected<int> uses_void_fail() {
    BSD_TRYV(void_fail());
    return 1;
}

} // namespace

TEST(TryMacro, SuccessUnwraps) {
    const auto r = chain_success();
    ASSERT_TRUE(r);
    EXPECT_EQ(*r, 70);
}

TEST(TryMacro, FailurePropagates) {
    const auto r = chain_fail();
    ASSERT_FALSE(r);
    EXPECT_EQ(r.error(), serialize_error::invalid_bool);
}

TEST(TryMacroV, VoidSuccessContinues) {
    const auto r = uses_void_ok();
    ASSERT_TRUE(r);
    EXPECT_EQ(*r, 42);
}

TEST(TryMacroV, VoidFailurePropagates) {
    const auto r = uses_void_fail();
    ASSERT_FALSE(r);
    EXPECT_EQ(r.error(), serialize_error::io_error);
}
