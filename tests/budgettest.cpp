#include "catch.hpp"
#include "../src/budget.h"
#include "../src/sqlite/sqlite.hpp"
#include <QUrl>
#include <QDate>
#include <iostream>
#include <QDebug>
#include <QJsonArray>
#include <QJsonObject>

// Budget account and stuff made in matchtest file
// Separated to make sure there's a control

QUrl budgetFilePath = QUrl::fromLocalFile("BudgetTestAccount.mbgt");
Budget budget;

TEST_CASE("Can add budget categories", "[addCategory]") {
    SECTION("Give file path, name, and initial amount") {
        budget.addCategory(budgetFilePath, "Test Budget", 10000);

        io::sqlite::db mbgt("BudgetTestAccount.mbgt");
        io::sqlite::stmt query(mbgt, "SELECT categoryName, monthOne,"
                                     "monthOneRemaining, monthOneDate,"
                                     "monthTwo, monthTwoRemaining, monthTwoDate,"
                                     "monthThree, monthThreeRemaining, monthThreeDate,"
                                     "prevOne, prevOneRemaining, prevOneDate,"
                                     "prevTwo, prevTwoRemaining, prevTwoDate FROM budgets");

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
            // monthOneRemaining
            REQUIRE(query.row().int32(2) == 10000);
            // monthOneDate
            REQUIRE(query.row().text(3) == currentMonth.toStdString());
            // monthTwo
            REQUIRE(query.row().int32(4) == 0);
            // monthTwoRemaining
            REQUIRE(query.row().int32(5) == 0);
            // monthTwoDate
            REQUIRE(query.row().text(6) == monthPlusOne.toStdString());
            // monthThree
            REQUIRE(query.row().int32(7) == 0);
            // monthThreeRemaining
            REQUIRE(query.row().int32(8) == 0);
            // monthThreeDate
            REQUIRE(query.row().text(9) == monthPlusTwo.toStdString());
            // prevOne
            REQUIRE(query.row().int32(10) == 0);
            // prevOneRemaining
            REQUIRE(query.row().int32(11) == 0);
            // prevOneDate
            REQUIRE(query.row().text(12) == monthMinusOne.toStdString());
            // prevTwo
            REQUIRE(query.row().int32(13) == 0);
            // prevTwoRemaining
            REQUIRE(query.row().int32(14) == 0);
            // prevTwoDate
            REQUIRE(query.row().text(15) == monthMinusTwo.toStdString());
        }
    }
}

TEST_CASE("Can get a list of categories with amounts for a month at relative index", "[getCategories]") {
    SECTION("Zero is current month") {
        QJsonArray categories = budget.getCategories(budgetFilePath, 0);

        REQUIRE(categories[0].toObject()["categoryName"] == "Test Budget");
        REQUIRE(categories[0].toObject()["amount"] == "100.00");
    }

    SECTION("Can go positive or negative up to 2 out") {
        QJsonArray categories = budget.getCategories(budgetFilePath, 1);
        REQUIRE(categories[0].toObject()["categoryName"] == "Test Budget");
        REQUIRE(categories[0].toObject()["amount"] == "0.00");

        categories = budget.getCategories(budgetFilePath, 2);
        REQUIRE(categories[0].toObject()["categoryName"] == "Test Budget");

        categories = budget.getCategories(budgetFilePath, -1);
        REQUIRE(categories[0].toObject()["categoryName"] == "Test Budget");

        categories = budget.getCategories(budgetFilePath, -2);
        REQUIRE(categories[0].toObject()["categoryName"] == "Test Budget");
    }

    SECTION("Out of that range, has error") {
        QJsonArray categories = budget.getCategories(budgetFilePath, 3);
        REQUIRE(categories[0].toObject()["err"] == "Out of stored range");

        categories = budget.getCategories(budgetFilePath, -3);
        REQUIRE(categories[0].toObject()["err"] == "Out of stored range");
    }
}

TEST_CASE("Can get a list of only category names", "[getCategoryNames]") {
    SECTION("Returns an QList of the category names") {
        QList<QString> categories = budget.getCategoryNames(budgetFilePath);

        REQUIRE(categories.at(0) == "Test Budget");
    }

    SECTION("The list is alphabetized") {
        budget.addCategory(budgetFilePath, "Hello World", 20000);
        QList<QString> categories = budget.getCategoryNames(budgetFilePath);

        REQUIRE(categories.at(0) == "Hello World");
        REQUIRE(categories.at(1) == "Test Budget");
    }
}

TEST_CASE("Can subtract from remaining amount", "[subRemainingAmount]") {
    SECTION("Give file path, category, month date, and amount; returns bool") {
        QString currentMonth = QDate::currentDate().toString("yyyy-MM");
        bool changeSuccess = budget.subRemainingAmount(budgetFilePath,
                                                       "Test Budget",
                                                       currentMonth, 5000);

        REQUIRE(changeSuccess == true);

        io::sqlite::db mbgt("BudgetTestAccount.mbgt");
        io::sqlite::stmt query(mbgt, "SELECT monthOneRemaining FROM budgets WHERE id == 1");

        while (query.step()) {
            std::cout << "2: subRemainingAmount (1)\n";
            REQUIRE(query.row().int32(0) == 5000);
        }
    }
}
