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
        io::sqlite::stmt query(mbgt, "SELECT categoryName, monthOne, monthOneDate, monthTwo, monthTwoDate, monthThree, monthThreeDate FROM budgets");

        QString currentMonth = QDate::currentDate().toString("yyyy-MM");
        QString monthPlusOne = QDate::currentDate().addMonths(1).toString("yyyy-MM");
        QString monthPlusTwo = QDate::currentDate().addMonths(2).toString("yyyy-MM");

        while (query.step()) {
            std::cout << "1: addCategory(1)\n";
            REQUIRE(query.row().text(0) == "Test Budget");
            REQUIRE(query.row().int32(1) == 10000);
            REQUIRE(query.row().text(2) == currentMonth.toStdString());
            REQUIRE(query.row().int32(3) == 0);
            REQUIRE(query.row().text(4) == monthPlusOne.toStdString());
            REQUIRE(query.row().int32(5) == 0);
            REQUIRE(query.row().text(6) == monthPlusTwo.toStdString());
        }

        query.reset();
        /*query = io::sqlite::stmt(mbgt, "SELECT categoryName, prevOne, prevOneDate, prevTwo, prevTwoDate FROM prevBudgets");

        QString monthMinusOne = QDate::currentDate().addMonths(-1).toString("yyyy-MM");
        QString monthMinusTwo = QDate::currentDate().addMonths(-2).toString("yyyy-MM");

        while (query.step()) {
            std::cout << "1: addCategory(2)\n";
            REQUIRE(query.row().text(0) == "Test Budget");
            REQUIRE(query.row().int32(1) == 0);
            REQUIRE(query.row().text(2) == monthMinusOne.toStdString());
            REQUIRE(query.row().int32(3) == 0);
            REQUIRE(query.row().text(4) == monthMinusTwo.toStdString());
        }*/
    }
}
