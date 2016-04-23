#include "catch.hpp"
#include "../src/budget.h"
#include "../src/sqlite/sqlite.hpp"
#include <QUrl>
#include <QDate>
#include <iostream>
#include <QDebug>

// Budget account and stuff made in matchtest file
// Separated to make sure there's a control

QUrl budgetFilePath = QUrl::fromLocalFile("BudgetTestAccount.mbgt");

TEST_CASE("Can add budget categories", "[addCategory]") {
    SECTION("Give file path, name, and initial amount") {
        Budget budget;
        budget.addCategory(budgetFilePath, "Test Budget", 10000);

        io::sqlite::db mbgt("BudgetTestAccount.mbgt");
        io::sqlite::stmt query(mbgt, "SELECT categoryName, monthOne,"
                                     "monthOneDate, monthTwo, monthTwoDate,"
                                     "monthThree, monthThreeDate, prevOne,"
                                     "prevOneDate, prevTwo, prevTwoDate FROM budgets");

        QString currentMonth = QDate::currentDate().toString("yyyy-MM");
        QString monthPlusOne = QDate::currentDate().addMonths(1).toString("yyyy-MM");
        QString monthPlusTwo = QDate::currentDate().addMonths(2).toString("yyyy-MM");
        QString monthMinusOne = QDate::currentDate().addMonths(-1).toString("yyyy-MM");
        QString monthMinusTwo = QDate::currentDate().addMonths(-2).toString("yyyy-MM");

        while (query.step()) {
            std::cout << "1: addCategory(1)\n";
            // categoryName
            REQUIRE(query.row().text(0) == "Test Budget");
            // monthOne
            REQUIRE(query.row().int32(1) == 10000);
            // monthOneDate
            REQUIRE(query.row().text(2) == currentMonth.toStdString());
            // monthTwo
            REQUIRE(query.row().int32(3) == 0);
            // monthTwoDate
            REQUIRE(query.row().text(4) == monthPlusOne.toStdString());
            // monthThree
            REQUIRE(query.row().int32(5) == 0);
            // monthThreeDate
            REQUIRE(query.row().text(6) == monthPlusTwo.toStdString());
            // prevOne
            REQUIRE(query.row().int32(7) == 0);
            // prevOneDate
            REQUIRE(query.row().text(8) == monthMinusOne.toStdString());
            // prevTwo
            REQUIRE(query.row().int32(9) == 0);
            // prevTwoDate
            REQUIRE(query.row().text(10) == monthMinusTwo.toStdString());
        }
    }
}
