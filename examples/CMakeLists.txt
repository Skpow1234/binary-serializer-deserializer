# Example programs.
#
# These exist primarily to exercise the public headers under the project's
# warning-as-errors policy so that header-only regressions are caught at CI
# build time even before a full unit-test suite exists (Milestone 04+).

add_executable(bsd_smoke smoke.cpp)
target_link_libraries(bsd_smoke PRIVATE bsd::bsd)
bsd_set_warnings(bsd_smoke PRIVATE)
bsd_set_sanitizers(bsd_smoke)
set_target_properties(bsd_smoke PROPERTIES
    CXX_STANDARD          23
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS        OFF
)
