#include "catch.hpp"
#include <QString>
#include "../src/helpers.h"

TEST_CASE("Can convert integers to a formatted QString", "[intToQs") {
    SECTION("It works for numbers larger than three digits") {
        QString number = intToQs(100);
        REQUIRE(number == "1.00");
    }

    SECTION("It works for numbers with two digits") {
        QString number = intToQs(10);
        REQUIRE(number == "0.10");
    }

    SECTION("It works for number that are one digit") {
        QString number = intToQs(1);
        REQUIRE(number == "0.01");
    }
}
