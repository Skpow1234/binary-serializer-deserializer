# Unit tests (GoogleTest via FetchContent).

include(FetchContent)

set(BUILD_GMOCK OFF CACHE BOOL "" FORCE)
set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)
if(MSVC)
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
endif()

FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG        v1.15.2
    GIT_SHALLOW    TRUE
)

set(CMAKE_POLICY_DEFAULT_CMP0077 NEW)
FetchContent_MakeAvailable(googletest)

add_executable(bsd_tests
    error_category_test.cpp
    try_macro_test.cpp
)
target_link_libraries(bsd_tests PRIVATE bsd::bsd GTest::gtest_main)
bsd_set_warnings(bsd_tests PRIVATE)
bsd_set_sanitizers(bsd_tests)
set_target_properties(bsd_tests PROPERTIES
    CXX_STANDARD          23
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS        OFF
)

include(GoogleTest)
gtest_discover_tests(bsd_tests)
