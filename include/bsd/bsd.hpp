// SPDX-License-Identifier: MIT
//
// bsd.hpp -- umbrella header for the Binary Serializer / Deserializer library.
//
// Including this file is sufficient to access every public name the library
// currently exposes. Individual headers may also be included directly when a
// translation unit only needs a narrow subset.
//
// @invariant All public symbols live in `bsd::inline v1`. ABI-breaking
//            changes require bumping the inline namespace (e.g. `inline v2`)
//            so that consumers compiled against the previous version
//            continue to link against their original symbols.
// @invariant This header transitively includes every other public header.
//            Adding a new public header to the project requires adding it
//            here too; otherwise an "include bsd.hpp" consumer would silently
//            miss the new API.

#ifndef BSD_BSD_HPP
#define BSD_BSD_HPP

#include <bsd/concepts.hpp>
#include <bsd/config.hpp>
#include <bsd/error.hpp>
#include <bsd/serialize_expected.hpp>
#include <bsd/fwd.hpp>
#include <bsd/version.hpp>

namespace bsd::inline v1 {

// Functional declarations land in later milestones.

} // namespace bsd::inline v1

#endif // BSD_BSD_HPP
