// SPDX-License-Identifier: MIT
//
// Singleton `std::error_category` for `serialize_error`.
//
// This translation unit exists solely so `serialize_error_category()` has a
// single static storage duration object across the entire program — a
// requirement for `std::error_code` equality and hashing to behave correctly
// when codes are produced from different translation units.

#include <bsd/error.hpp>

#include <string>

namespace {

struct serialize_error_category_impl final : std::error_category {
    [[nodiscard]] const char* name() const noexcept override { return "bsd"; }

    [[nodiscard]] std::string message(int ev) const override {
        const auto e = static_cast<bsd::serialize_error>(ev);
        return std::string{bsd::to_string(e)};
    }
};

} // namespace

namespace bsd::inline v1 {

const std::error_category& serialize_error_category() noexcept {
    static const serialize_error_category_impl cat;
    return cat;
}

} // namespace bsd::inline v1
