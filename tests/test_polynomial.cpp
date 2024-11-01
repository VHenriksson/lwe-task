#include <catch2/catch.hpp>
#include "polynomial.h"

TEST_CASE("TestPolynomialConstruction", "[Polynomial]") {

    SECTION("Test default polynomial construction") {
        auto p = Polynomial<4>();
        REQUIRE(p == Polynomial<4>(std::vector<uint32_t>{0, 0, 0, 0}));
    }

    SECTION("Test empty polynomial construction") {
        auto p = Polynomial<2>(std::vector<uint32_t>{});
        REQUIRE(p == Polynomial<2>(std::vector<uint32_t>{0, 0}));
    }

    SECTION("Test polynomial is not always zero") {
        auto p = Polynomial<4>(std::vector<uint32_t>{1, 2, 3});
        REQUIRE(p != Polynomial<4>(std::vector<uint32_t>{0}));
    }

    SECTION("Test polynomial construction") {
        auto p = Polynomial<4>(std::vector<uint32_t>{1, 2, 3});
        REQUIRE(p == Polynomial<4>(std::vector<uint32_t>{1, 2, 3, 0}));
    }

    SECTION("Test modulo") {
        auto p1 = Polynomial<8>(std::vector<uint32_t>{10, 9, 8, 7, 6, 5, 4, 3, 2, 1});
        auto p2 = Polynomial<8>(std::vector<uint32_t>{8, 8, 8, 7, 6, 5, 4, 3});
        REQUIRE(p1 == p2);
    }
}



TEST_CASE("TestPolynomialAddition", "[Polynomial]") {

    SECTION("Test easy case addition") {
        auto p1 = Polynomial<4>(std::vector<uint32_t>{1, 2, 3});
        auto p2 = Polynomial<4>(std::vector<uint32_t>{4, 5, 6});
        auto expected = Polynomial<4>(std::vector<uint32_t>{5, 7, 9});
        REQUIRE(p1+p2 == expected);
    }

    SECTION("Test addition with wrapped around values") {
        auto p1 = Polynomial<4>(std::vector<uint32_t>{UINT32_MAX, UINT32_MAX});
        auto p2 = Polynomial<4>(std::vector<uint32_t>{1, 1});
        auto expected = Polynomial<4>(std::vector<uint32_t>{0, 0});
        REQUIRE(p1+p2 == expected);
    }
}


TEST_CASE("TestPolynomialMultiplication", "[Polynomial]") {

    SECTION("Test easy case multiplication") {
        auto p1 = Polynomial<4>(std::vector<uint32_t>{1, 2, 3});
        auto p2 = Polynomial<4>(std::vector<uint32_t>{4, 5, 6});
        // Note that since the constructor takes the polynominal modulo X^N + 1,
        // comparing with the polynomial below is indeed correct.
        auto expected = Polynomial<4>(std::vector<uint32_t>{4, 13, 28, 27, 18});
        Polynomial<4> p = p1*p2;
        REQUIRE(p1*p2 == expected);
    }
}