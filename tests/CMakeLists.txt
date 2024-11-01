find_package(Catch2 REQUIRED)

set(TEST_SOURCES
    test_main.cpp
    test_polynomial.cpp
    test_encryption.cpp
)

add_executable(SimpleTFHETest ${TEST_SOURCES})

target_link_libraries(SimpleTFHETest PRIVATE SimpleTFHE Catch2::Catch2)

include(CTest)
include(Catch)
catch_discover_tests(SimpleTFHETest)
