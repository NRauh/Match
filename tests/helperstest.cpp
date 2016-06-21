#include "catch.hpp"
#include <QString>
#include "../src/helpers.h"

TEST_CASE("Can convert integers to a formatted QString", "[intToQs]") {
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

TEST_CASE("Can select month from relative months out", "[monthFromIndex]") {
    SECTION("Give -2..2 and it returns a string for which month to select") {
        REQUIRE(monthFromIndex(-2) == "prevTwo");
        REQUIRE(monthFromIndex(-1) == "prevOne");
        REQUIRE(monthFromIndex(0) == "monthOne");
        REQUIRE(monthFromIndex(1) == "monthTwo");
        REQUIRE(monthFromIndex(2) == "monthThree");
    }
}
