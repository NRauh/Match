#include "catch.hpp"
#include "../src/budget.h"
#include "../src/sqlite/sqlite.hpp"
#include <QUrl>
#include <QDate>
#include <iostream>
#include <QDebug>
#include <QJsonArray>
#include <QJsonObject>
#include "../src/account.h"
#include "../src/accountmanager.h"

QUrl dirPath = QUrl::fromLocalFile(".");
QUrl budgetFilePath = QUrl::fromLocalFile("BudgetTestAccount.mbgt");

QDate currentDate = QDate::currentDate();
QString currentMonth = currentDate.toString("yyyy-MM");
QString monthPlusOne = currentDate.addMonths(1).toString("yyyy-MM");
QString monthPlusTwo = currentDate.addMonths(2).toString("yyyy-MM");
QString monthMinusOne = currentDate.addMonths(-1).toString("yyyy-MM");
QString monthMinusTwo = currentDate.addMonths(-2).toString("yyyy-MM");

TEST_CASE("Can add budget categories", "[addCategory]") {
    AccountManager accManager;
    Budget budget;
    accManager.createBudget(dirPath, "BudgetTestAccount");

    SECTION("Give file path, name, and initial amount") {
        budget.addCategory(budgetFilePath, "Test Budget", 10000);

        io::sqlite::db mbgt("BudgetTestAccount.mbgt");
        io::sqlite::stmt query(mbgt, "SELECT categoryName, monthOne,"
                                     "monthOneSpent, monthOneDate,"
                                     "monthTwo, monthTwoSpent, monthTwoDate,"
                                     "monthThree, monthThreeSpent, monthThreeDate,"
                                     "prevOne, prevOneSpent, prevOneDate,"
                                     "prevTwo, prevTwoSpent, prevTwoDate FROM budgets");

        while (query.step()) {
            std::cout << "1: addCategory(1)\n";
            // categoryName
            REQUIRE(query.row().text(0) == "Test Budget");
            // monthOne
            REQUIRE(query.row().int32(1) == 10000);
            // monthOneSpent
            REQUIRE(query.row().int32(2) == 0);
            // monthOneDate
            REQUIRE(query.row().text(3) == currentMonth.toStdString());
            // monthTwo
            REQUIRE(query.row().int32(4) == 0);
            // monthTwoSpent
            REQUIRE(query.row().int32(5) == 0);
            // monthTwoDate
            REQUIRE(query.row().text(6) == monthPlusOne.toStdString());
            // monthThree
            REQUIRE(query.row().int32(7) == 0);
            // monthThreeSpent
            REQUIRE(query.row().int32(8) == 0);
            // monthThreeDate
            REQUIRE(query.row().text(9) == monthPlusTwo.toStdString());
            // prevOne
            REQUIRE(query.row().int32(10) == 0);
            // prevOneSpent
            REQUIRE(query.row().int32(11) == 0);
            // prevOneDate
            REQUIRE(query.row().text(12) == monthMinusOne.toStdString());
            // prevTwo
            REQUIRE(query.row().int32(13) == 0);
            // prevTwoSpent
            REQUIRE(query.row().int32(14) == 0);
            // prevTwoDate
            REQUIRE(query.row().text(15) == monthMinusTwo.toStdString());
        }
    }

    remove("BudgetTestAccount.mbgt");
}

TEST_CASE("Can get a list of categories with amounts for a month at relative index", "[getCategories]") {
    AccountManager accManager;
    Budget budget;
    accManager.createBudget(dirPath, "BudgetTestAccount");
    budget.addCategory(budgetFilePath, "Test Category", 10000);

    SECTION("Zero is current month") {
        QJsonArray categories = budget.getCategories(budgetFilePath, 0);

        REQUIRE(categories[0].toObject()["categoryName"] == "Test Category");
        REQUIRE(categories[0].toObject()["amount"] == "100.00");
        REQUIRE(categories[0].toObject()["remaining"] == "100.00");
    }

    SECTION("Can go positive or negative up to 2 out") {
        QJsonArray categories = budget.getCategories(budgetFilePath, 1);
        REQUIRE(categories[0].toObject()["categoryName"] == "Test Category");
        REQUIRE(categories[0].toObject()["amount"] == "0.00");

        categories = budget.getCategories(budgetFilePath, 2);
        REQUIRE(categories[0].toObject()["categoryName"] == "Test Category");

        categories = budget.getCategories(budgetFilePath, -1);
        REQUIRE(categories[0].toObject()["categoryName"] == "Test Category");

        categories = budget.getCategories(budgetFilePath, -2);
        REQUIRE(categories[0].toObject()["categoryName"] == "Test Category");
    }

    //This error is probably pointless
    SECTION("Out of that range, has error") {
        QJsonArray categories = budget.getCategories(budgetFilePath, 3);
        REQUIRE(categories[0].toObject()["err"] == "Out of stored range");

        categories = budget.getCategories(budgetFilePath, -3);
        REQUIRE(categories[0].toObject()["err"] == "Out of stored range");
    }

    // Any tests for adding a category will need to change other tests
    // I'm planning on isolating each test soon, so when I do I'll actually add this
    // For now, I'm only needing to add `ORDER BY categoryName` to the sqlQuery var
    //SECTION("Categories are alphabetized") {}

    remove("BudgetTestAccount.mbgt");
}

TEST_CASE("Can get a list of only category names", "[getCategoryNames]") {
    AccountManager accManager;
    Budget budget;
    accManager.createBudget(dirPath, "BudgetTestAccount");
    budget.addCategory(budgetFilePath, "Test Category", 10000);

    SECTION("Returns an QList of the category names") {
        QList<QString> categories = budget.getCategoryNames(budgetFilePath);
        REQUIRE(categories.at(0) == "Test Category");
    }

    SECTION("The list is alphabetized") {
        budget.addCategory(budgetFilePath, "Hello World", 20000);
        QList<QString> categories = budget.getCategoryNames(budgetFilePath);

        REQUIRE(categories.at(0) == "Hello World");
        REQUIRE(categories.at(1) == "Test Category");
    }

    remove("BudgetTestAccount.mbgt");
}

TEST_CASE("Can subtract from remaining amount", "[addToSpent]") {
    AccountManager accManager;
    Budget budget;
    accManager.createBudget(dirPath, "BudgetTestAccount");
    budget.addCategory(budgetFilePath, "Test Category", 10000);

    SECTION("Give file path, category, month date, and amount; returns bool") {
        bool changeSuccess = budget.addToSpent(budgetFilePath, "Test Category", currentMonth, 5000);

        REQUIRE(changeSuccess == true);

        io::sqlite::db mbgt("BudgetTestAccount.mbgt");
        io::sqlite::stmt query(mbgt, "SELECT monthOneSpent FROM budgets WHERE id == 1");

        while (query.step()) {
            std::cout << "2: addToSpent(1)\n";
            REQUIRE(query.row().int32(0) == 5000);
        }
    }

    SECTION("It works with past and future months") {
        budget.addToSpent(budgetFilePath, "Test Category", monthPlusOne, 5000);
        budget.addToSpent(budgetFilePath, "Test Category", monthPlusTwo, 5000);
        budget.addToSpent(budgetFilePath, "Test Category", monthMinusOne, 5000);
        budget.addToSpent(budgetFilePath, "Test Category", monthMinusTwo, 5000);

        io::sqlite::db mbgt("BudgetTestAccount.mbgt");
        io::sqlite::stmt query(mbgt, "SELECT monthTwoSpent, monthThreeSpent,"
                                     "prevOneSpent, prevTwoSpent"
                                     " FROM budgets WHERE id == 1");

        while (query.step()) {
            std::cout << "2: addToSpent(2)\n";
            REQUIRE(query.row().int32(0) == 5000);
            REQUIRE(query.row().int32(1) == 5000);
            REQUIRE(query.row().int32(2) == 5000);
            REQUIRE(query.row().int32(3) == 5000);
        }
    }

    SECTION ("If the month is out of range, it returns false") {
        QString outOfDate = currentDate.addMonths(4).toString("yyyy-MM");
        bool changeStatus = budget.addToSpent(budgetFilePath, "Test Category", outOfDate, 5000);
        REQUIRE(changeStatus == false);
    }

    remove("BudgetTestAccount.mbgt");
}

TEST_CASE("Can update budget for month", "[updateBudget]") {
    AccountManager accManager;
    Budget budget;
    accManager.createBudget(dirPath, "BudgetTestAccount");
    budget.addCategory(budgetFilePath, "Test Category", 10000);

    SECTION("File path, month number, categoryName, and updated amount") {
        budget.updateBudget(budgetFilePath, -2, "Test Category", 9000);
        budget.updateBudget(budgetFilePath, -1, "Test Category", 8000);
        budget.updateBudget(budgetFilePath, 0, "Test Category", 7000);
        budget.updateBudget(budgetFilePath, 1, "Test Category", 6000);
        budget.updateBudget(budgetFilePath, 2, "Test Category", 5000);

        io::sqlite::db mbgt("BudgetTestAccount.mbgt");
        io::sqlite::stmt query(mbgt, "SELECT "
                                     "prevTwo, prevOne,"
                                     "monthOne, monthTwo, monthThree "
                                     "FROM budgets WHERE id == 1");

        while (query.step()) {
            std::cout << "3: updateBudget (1)\n";
            REQUIRE(query.row().int32(0) == 9000);
            REQUIRE(query.row().int32(1) == 8000);
            REQUIRE(query.row().int32(2) == 7000);
            REQUIRE(query.row().int32(3) == 6000);
            REQUIRE(query.row().int32(4) == 5000);
        }
    }

    remove("BudgetTestAccount.mbgt");
}

TEST_CASE("Can get the month and remaing amount to spend", "[getMeta]") {
    AccountManager accManager;
    Budget budget;
    accManager.createBudget(dirPath, "BudgetTestAccount");
    budget.addCategory(budgetFilePath, "Test Category", 10000);
    budget.addToSpent(budgetFilePath, "Test Category", currentMonth, 5000);

    SECTION("File path, month number") {
        QJsonObject meta = budget.getMeta(budgetFilePath, 0);

        REQUIRE(meta["month"] == currentDate.toString("MMMM, yyyy"));
        REQUIRE(meta["remaining"] == "50.00");
        REQUIRE(meta["monthInt"] == currentDate.toString("yyyy-MM"));
    }

    remove("BudgetTestAccount.mbgt");
}

TEST_CASE("Can get amount of unbudgeted money", "[getAvailableMoney]") {
    AccountManager accManager;
    Account account;
    Budget budget;
    accManager.createBudget(dirPath, "BudgetTestAccount");
    budget.addCategory(budgetFilePath, "Test Category", 10000);
    account.addAccount(budgetFilePath, "Foo CU", 20000, currentDate, true);

    SECTION("File path") {
        QString unbudgeted = budget.getAvailableMoney(budgetFilePath);

        REQUIRE(unbudgeted == "100.00");
    }

    SECTION("It doesn't count off budget accounts") {
        Account account;

        account.addAccount(budgetFilePath, "Hello Bank", 30000, currentDate, false);
        QString unbudgeted = budget.getAvailableMoney(budgetFilePath);

        REQUIRE(unbudgeted == "100.00");
    }

    remove("BudgetTestAccount.mbgt");
}

TEST_CASE("Can get transactions for a category", "[getCategoryTransactions]") {
    AccountManager accManager;
    Account account;
    Budget budget;
    accManager.createBudget(dirPath, "BudgetTestAccount");
    account.addAccount(budgetFilePath, "Foo", 1000, currentDate, true);
    account.addAccount(budgetFilePath, "Bar", 1000, currentDate, false);
    budget.addCategory(budgetFilePath, "Test", 3000);
    account.addTransaction(budgetFilePath, 1, currentDate, "Corner Shop", true, 200, "Test", "Test transaction");
    account.addTransaction(budgetFilePath, 2, currentDate, "Corner Shop", true, 200, "Test", "Other test");

    SECTION("It gets the transactions for a budget") {
        QJsonArray transactions = budget.getCategoryTransactions(budgetFilePath, "Test", currentDate.toString("yyyy-MM"));

        REQUIRE(transactions[0].toObject()["date"] == currentDate.toString("M/d/yy"));
        REQUIRE(transactions[0].toObject()["payee"] == "Corner Shop");
        REQUIRE(transactions[0].toObject()["note"] == "Test transaction");
        REQUIRE(transactions[0].toObject()["amount"] == "2.00");
        REQUIRE(transactions[0].toObject()["account"] == "Foo");
        REQUIRE(transactions.size() == 1);
    }

    remove("BudgetTestAccount.mbgt");
}
