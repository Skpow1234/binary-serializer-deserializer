// SPDX-License-Identifier: MIT
//
// Exhaustive tests for `serialize_error`, `to_string`, and the
// `std::error_category` bridge (Milestone 04).

#include <bsd/error.hpp>

#include <gtest/gtest.h>

#include <memory>
#include <string>
#include <type_traits>

namespace {

using bsd::make_error_code;
using bsd::serialize_error;
using bsd::serialize_error_category;
using bsd::to_string;

} // namespace

static_assert(std::is_error_code_enum_v<serialize_error>);

TEST(SerializeError, NoneIsZero) {
    EXPECT_EQ(static_cast<int>(serialize_error::none), 0);
}

TEST(SerializeError, EveryEnumeratorHasStableNumericValue) {
    using E = serialize_error;
    EXPECT_EQ(static_cast<int>(E::none), 0);
    EXPECT_EQ(static_cast<int>(E::out_of_space), 1);
    EXPECT_EQ(static_cast<int>(E::truncated_input), 2);
    EXPECT_EQ(static_cast<int>(E::invalid_tag), 3);
    EXPECT_EQ(static_cast<int>(E::invalid_bool), 4);
    EXPECT_EQ(static_cast<int>(E::invalid_utf8), 5);
    EXPECT_EQ(static_cast<int>(E::depth_exceeded), 6);
    EXPECT_EQ(static_cast<int>(E::size_limit_exceeded), 7);
    EXPECT_EQ(static_cast<int>(E::checksum_mismatch), 8);
    EXPECT_EQ(static_cast<int>(E::version_mismatch), 9);
    EXPECT_EQ(static_cast<int>(E::unknown_type), 10);
    EXPECT_EQ(static_cast<int>(E::io_error), 11);
    EXPECT_EQ(static_cast<int>(E::compression_error), 12);
    EXPECT_EQ(static_cast<int>(E::not_supported), 13);
}

TEST(SerializeError, ToStringIsNonEmptyForEveryEnumerator) {
    const auto check = [](serialize_error e) {
        const std::string_view sv = to_string(e);
        EXPECT_FALSE(sv.empty()) << static_cast<int>(e);
    };
    check(serialize_error::none);
    check(serialize_error::out_of_space);
    check(serialize_error::truncated_input);
    check(serialize_error::invalid_tag);
    check(serialize_error::invalid_bool);
    check(serialize_error::invalid_utf8);
    check(serialize_error::depth_exceeded);
    check(serialize_error::size_limit_exceeded);
    check(serialize_error::checksum_mismatch);
    check(serialize_error::version_mismatch);
    check(serialize_error::unknown_type);
    check(serialize_error::io_error);
    check(serialize_error::compression_error);
    check(serialize_error::not_supported);
}

TEST(ErrorCategory, SingletonIdentity) {
    const std::error_category& a = serialize_error_category();
    const std::error_category& b = serialize_error_category();
    EXPECT_EQ(std::addressof(a), std::addressof(b));
    EXPECT_STREQ(a.name(), "bsd");
}

TEST(ErrorCategory, MakeErrorCodeRoundTrip) {
    const auto verify = [](serialize_error e) {
        const std::error_code ec = make_error_code(e);
        EXPECT_EQ(ec.category(), serialize_error_category());
        EXPECT_EQ(ec.value(), static_cast<int>(e));
        EXPECT_EQ(ec.message(), std::string(to_string(e)));
    };
    verify(serialize_error::none);
    verify(serialize_error::out_of_space);
    verify(serialize_error::truncated_input);
    verify(serialize_error::invalid_tag);
    verify(serialize_error::invalid_bool);
    verify(serialize_error::invalid_utf8);
    verify(serialize_error::depth_exceeded);
    verify(serialize_error::size_limit_exceeded);
    verify(serialize_error::checksum_mismatch);
    verify(serialize_error::version_mismatch);
    verify(serialize_error::unknown_type);
    verify(serialize_error::io_error);
    verify(serialize_error::compression_error);
    verify(serialize_error::not_supported);
}

TEST(ErrorCategory, ImplicitConversionConstructsErrorCode) {
    const std::error_code ec = serialize_error::truncated_input;
    EXPECT_EQ(ec.category(), serialize_error_category());
    EXPECT_EQ(ec.value(), static_cast<int>(serialize_error::truncated_input));
}
