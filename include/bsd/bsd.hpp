// SPDX-License-Identifier: MIT
//
// bsd.hpp -- umbrella header for the Binary Serializer / Deserializer library.
//
// @invariant All public symbols live in `bsd::inline v1`. ABI-breaking changes
//            require bumping the inline namespace (e.g. `inline v2`) so that
//            consumers compiled against the previous version continue to link
//            against their original symbols.
//
// Milestone 01 status: this header intentionally contains only the namespace
// scaffolding and the generated version information. Functional headers are
// added in subsequent milestones (see docs/DESIGN.md).

#ifndef BSD_BSD_HPP
#define BSD_BSD_HPP

#include <bsd/version.hpp>

namespace bsd::inline v1 {

// Functional declarations land in later milestones.

} // namespace bsd::inline v1

#endif // BSD_BSD_HPP
