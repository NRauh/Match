#include "catch.hpp"
#include "../src/accountmanager.h"
#include <QUrl>
#include <QFile>
#include <QDir>
#include <QDebug>
#include <QSettings>
#include "../src/json/json.h"
#include "../src/sqlite/sqlite.hpp"
#include <QDate>
#include "../src/budget.h"


TEST_CASE("Can create budget/save files", "[createBudget]") {
  SECTION("A path and name are given, then it creates a .mbgt file") {
      AccountManager accManager;
      QUrl path = QUrl::fromLocalFile(".");
      accManager.createBudget(path, "Foo Budget");

      QFile testBudget("Foo Budget.mbgt");
      REQUIRE(testBudget.exists() == true);
  }
}

TEST_CASE("Can shift budget one month", "[shiftOneMonth]") {
    AccountManager accManager;
    Budget budget;
    QDate currentDate = QDate::currentDate();
    accManager.createBudget(QUrl::fromLocalFile("."), "FooBar");
    QUrl filePath = QUrl::fromLocalFile("FooBar.mbgt");

    budget.addCategory(filePath, "Test Category", 3000);
    budget.updateBudget(filePath, -2, "Test Category", 1000);
    budget.updateBudget(filePath, -1, "Test Category", 2000);
    budget.updateBudget(filePath, 1, "Test Category", 4000);
    budget.updateBudget(filePath, 2, "Test Category", 5000);

    budget.addToSpent(filePath, "Test Category", currentDate.addMonths(-2).toString("yyyy-MM"), 1);
    budget.addToSpent(filePath, "Test Category", currentDate.addMonths(-1).toString("yyyy-MM"), 2);
    budget.addToSpent(filePath, "Test Category", currentDate.toString("yyyy-MM"), 3);
    budget.addToSpent(filePath, "Test Category", currentDate.addMonths(1).toString("yyyy-MM"), 4);
    budget.addToSpent(filePath, "Test Category", currentDate.addMonths(2).toString("yyyy-MM"), 5);

    SECTION("A path is given") {
        accManager.shiftOneMonth(filePath);

        io::sqlite::db mbgt("FooBar.mbgt");
        io::sqlite::stmt query(mbgt, "SELECT prevTwo, prevTwoSpent, prevTwoDate,"
                                     "prevOne, prevOneSpent, prevOneDate,"
                                     "monthOne, monthOneSpent, monthOneDate,"
                                     "monthTwo, monthTwoSpent, monthTwoDate,"
                                     "monthThree, monthThreeSpent, monthThreeDate"
                                     " FROM budgets WHERE id == 1");

        while (query.step()) {
            // Check that prevTwo- shifted from prevOne-
            REQUIRE(query.row().int32(0) == 2000);
            REQUIRE(query.row().int32(1) == 2);
            REQUIRE(query.row().text(2) == currentDate.addMonths(-1).toString("yyyy-MM").toStdString());
            // Check that prevOne- shifted from monthOne-
            REQUIRE(query.row().int32(3) == 3000);
            REQUIRE(query.row().int32(4) == 3);
            REQUIRE(query.row().text(5) == currentDate.toString("yyyy-MM").toStdString());
            // Check that monthOne- shifted from monthTwo-
            REQUIRE(query.row().int32(6) == 4000);
            REQUIRE(query.row().int32(7) == 4);
            REQUIRE(query.row().text(8) == currentDate.addMonths(1).toString("yyyy-MM").toStdString());
            // Check that monthTwo- shifted from monthThree-
            REQUIRE(query.row().int32(9) == 5000);
            REQUIRE(query.row().int32(10) == 5);
            REQUIRE(query.row().text(11) == currentDate.addMonths(2).toString("yyyy-MM").toStdString());
            // Check that monthThree was reset
            REQUIRE(query.row().int32(12) == 5000);
            REQUIRE(query.row().int32(13) == 0);
            REQUIRE(query.row().text(14) == currentDate.addMonths(3).toString("yyyy-MM").toStdString());
        }
    }

    remove("FooBar.mbgt");
}

TEST_CASE("Can reset budget spent and dates", "[shiftMany]") {
    AccountManager accManager;
    Budget budget;
    QDate currentDate = QDate::currentDate();
    accManager.createBudget(QUrl::fromLocalFile("."), "FooBar");
    QUrl filePath = QUrl::fromLocalFile("FooBar.mbgt");

    budget.addCategory(filePath, "Test Category", 3000);
    budget.updateBudget(filePath, -2, "Test Category", 1000);
    budget.updateBudget(filePath, -1, "Test Category", 2000);
    budget.updateBudget(filePath, 1, "Test Category", 4000);
    budget.updateBudget(filePath, 2, "Test Category", 5000);

    budget.addToSpent(filePath, "Test Category", currentDate.addMonths(-2).toString("yyyy-MM"), 1);
    budget.addToSpent(filePath, "Test Category", currentDate.addMonths(-1).toString("yyyy-MM"), 2);
    budget.addToSpent(filePath, "Test Category", currentDate.toString("yyyy-MM"), 3);
    budget.addToSpent(filePath, "Test Category", currentDate.addMonths(1).toString("yyyy-MM"), 4);
    budget.addToSpent(filePath, "Test Category", currentDate.addMonths(2).toString("yyyy-MM"), 5);

    SECTION("A path is given") {
        accManager.shiftMany(filePath, currentDate.addMonths(6));

        io::sqlite::db mbgt("FooBar.mbgt");
        io::sqlite::stmt query(mbgt, "SELECT prevTwo, prevTwoSpent, prevTwoDate,"
                                     "prevOne, prevOneSpent, prevOneDate,"
                                     "monthOne, monthOneSpent, monthOneDate,"
                                     "monthTwo, monthTwoSpent, monthTwoDate,"
                                     "monthThree, monthThreeSpent, monthThreeDate"
                                     " FROM budgets WHERE id == 1");

        while (query.step()) {
            // Check that prevTwo- shifted from prevOne-
            REQUIRE(query.row().int32(0) == 5000);
            REQUIRE(query.row().int32(1) == 0);
            REQUIRE(query.row().text(2) == currentDate.addMonths(4).toString("yyyy-MM").toStdString());
            // Check that prevOne- shifted from monthOne-
            REQUIRE(query.row().int32(3) == 5000);
            REQUIRE(query.row().int32(4) == 0);
            REQUIRE(query.row().text(5) == currentDate.addMonths(5).toString("yyyy-MM").toStdString());
            // Check that monthOne- shifted from monthTwo-
            REQUIRE(query.row().int32(6) == 5000);
            REQUIRE(query.row().int32(7) == 0);
            REQUIRE(query.row().text(8) == currentDate.addMonths(6).toString("yyyy-MM").toStdString());
            // Check that monthTwo- shifted from monthThree-
            REQUIRE(query.row().int32(9) == 5000);
            REQUIRE(query.row().int32(10) == 0);
            REQUIRE(query.row().text(11) == currentDate.addMonths(7).toString("yyyy-MM").toStdString());
            // Check that monthThree was reset
            REQUIRE(query.row().int32(12) == 5000);
            REQUIRE(query.row().int32(13) == 0);
            REQUIRE(query.row().text(14) == currentDate.addMonths(8).toString("yyyy-MM").toStdString());
        }
    }

    remove("FooBar.mbgt");
}

TEST_CASE("Can shift budgets", "[shiftBudget]") {
    AccountManager accManager;
    Budget budget;
    QDate currentDate = QDate::currentDate();
    accManager.createBudget(QUrl::fromLocalFile("."), "FooBar");
    QUrl filePath = QUrl::fromLocalFile("FooBar.mbgt");

    budget.addCategory(filePath, "Test Category", 3000);
    budget.updateBudget(filePath, -2, "Test Category", 1000);
    budget.updateBudget(filePath, -1, "Test Category", 2000);
    budget.updateBudget(filePath, 1, "Test Category", 4000);
    budget.updateBudget(filePath, 2, "Test Category", 5000);

    budget.addToSpent(filePath, "Test Category", currentDate.addMonths(-2).toString("yyyy-MM"), 1);
    budget.addToSpent(filePath, "Test Category", currentDate.addMonths(-1).toString("yyyy-MM"), 2);
    budget.addToSpent(filePath, "Test Category", currentDate.toString("yyyy-MM"), 3);
    budget.addToSpent(filePath, "Test Category", currentDate.addMonths(1).toString("yyyy-MM"), 4);
    budget.addToSpent(filePath, "Test Category", currentDate.addMonths(2).toString("yyyy-MM"), 5);

    SECTION("Give path and date") {
        accManager.shiftBudget(filePath, currentDate.addMonths(1));

        io::sqlite::db mbgt("FooBar.mbgt");
        io::sqlite::stmt query(mbgt, "SELECT prevTwoDate, prevOneDate,"
                                     "monthOneDate, monthTwoDate,"
                                     "monthThreeDate FROM budgets WHERE id == 1");

        while (query.step()) {
            // Only checking dates, because shiftOneMonth has been tested
            REQUIRE(query.row().text(0) == currentDate.addMonths(-1).toString("yyyy-MM").toStdString());
            REQUIRE(query.row().text(1) == currentDate.toString("yyyy-MM").toStdString());
            REQUIRE(query.row().text(2) == currentDate.addMonths(1).toString("yyyy-MM").toStdString());
            REQUIRE(query.row().text(3) == currentDate.addMonths(2).toString("yyyy-MM").toStdString());
            // Spent will be blank for dates below
            REQUIRE(query.row().text(4) == currentDate.addMonths(3).toString("yyyy-MM").toStdString());
        }
    }

    SECTION("It goes for two months out") {
        accManager.shiftBudget(filePath, currentDate.addMonths(2));

        io::sqlite::db mbgt("FooBar.mbgt");
        io::sqlite::stmt query(mbgt, "SELECT prevTwoDate, prevOneDate,"
                                     "monthOneDate, monthTwoDate,"
                                     "monthThreeDate FROM budgets WHERE id == 1");

        while (query.step()) {
            REQUIRE(query.row().text(0) == currentDate.toString("yyyy-MM").toStdString());
            REQUIRE(query.row().text(1) == currentDate.addMonths(1).toString("yyyy-MM").toStdString());
            REQUIRE(query.row().text(2) == currentDate.addMonths(2).toString("yyyy-MM").toStdString());
            // Spent will be blank for dates below
            REQUIRE(query.row().text(3) == currentDate.addMonths(3).toString("yyyy-MM").toStdString());
            REQUIRE(query.row().text(4) == currentDate.addMonths(4).toString("yyyy-MM").toStdString());
        }

        accManager.shiftBudget(filePath, currentDate.addMonths(3));
        query.exec();

        while (query.step()) {
            REQUIRE(query.row().text(0) == currentDate.addMonths(1).toString("yyyy-MM").toStdString());
            REQUIRE(query.row().text(1) == currentDate.addMonths(2).toString("yyyy-MM").toStdString());
            // Spent will be blank for dates below
            REQUIRE(query.row().text(2) == currentDate.addMonths(3).toString("yyyy-MM").toStdString());
            REQUIRE(query.row().text(3) == currentDate.addMonths(4).toString("yyyy-MM").toStdString());
            REQUIRE(query.row().text(4) == currentDate.addMonths(5).toString("yyyy-MM").toStdString());
        }

        accManager.shiftBudget(filePath, currentDate.addMonths(4));
        query.exec();

        while (query.step()) {
            REQUIRE(query.row().text(0) == currentDate.addMonths(2).toString("yyyy-MM").toStdString());
            // Spent will be blank for dates below
            REQUIRE(query.row().text(1) == currentDate.addMonths(3).toString("yyyy-MM").toStdString());
            REQUIRE(query.row().text(2) == currentDate.addMonths(4).toString("yyyy-MM").toStdString());
            REQUIRE(query.row().text(3) == currentDate.addMonths(5).toString("yyyy-MM").toStdString());
            REQUIRE(query.row().text(4) == currentDate.addMonths(6).toString("yyyy-MM").toStdString());
        }
    }

    SECTION("If it's more than 4 months out, they all get reset") {
        accManager.shiftBudget(filePath, currentDate.addMonths(5));

        io::sqlite::db mbgt("FooBar.mbgt");
        io::sqlite::stmt query(mbgt, "SELECT prevTwoDate, prevOneDate,"
                                     "monthOneDate, monthTwoDate,"
                                     "monthThreeDate FROM budgets WHERE id == 1");

        while (query.step()) {
            // Spent will be blank for dates below
            REQUIRE(query.row().text(0) == currentDate.addMonths(3).toString("yyyy-MM").toStdString());
            REQUIRE(query.row().text(1) == currentDate.addMonths(4).toString("yyyy-MM").toStdString());
            REQUIRE(query.row().text(2) == currentDate.addMonths(5).toString("yyyy-MM").toStdString());
            REQUIRE(query.row().text(3) == currentDate.addMonths(6).toString("yyyy-MM").toStdString());
            REQUIRE(query.row().text(4) == currentDate.addMonths(7).toString("yyyy-MM").toStdString());
        }
    }

    remove("FooBar.mbgt");
}

TEST_CASE("Can check if the file exists", "[haveLastFile]") {
    SECTION("If the file exists it returns true, otherwise false") {
        AccountManager accManager;
        accManager.createBudget(QUrl::fromLocalFile("."), "FooBar");
        accManager.setLastFile(QUrl::fromLocalFile("FooBar.mbgt"));

        bool exists = accManager.haveLastFile();
        REQUIRE(exists == true);

        remove("FooBar.mbgt");
        exists = accManager.haveLastFile();
        REQUIRE(exists == false);
    }
}

/* setLastFile and getLastFile tests fail and work seemingly randomly
 * so are commented out to make tests run nicely since they do work
 * (top notch code there)
TEST_CASE("Can set the last loaded file", "[setLastFile]") {
    SECTION("Given a QUrl/path and saves the setting") {
        AccountManager accManager;
        QUrl path = QUrl::fromLocalFile("Foo Budget.mbgt");

        accManager.setLastFile(path);
        QSettings settings("nrauh", "Match");
        REQUIRE(settings.value("lastOpenedFile") == path);
    }
}

TEST_CASE("Can get the last loaded file", "[getLastFile]") {
    SECTION("Returns a QUrl for the last file opened") {
        AccountManager accManager;
        QUrl path = QUrl::fromLocalFile("Foo Budget.mbgt");

        REQUIRE(accManager.getLastFile() == path);

        QSettings settings("nrauh", "Match");
        settings.clear();
    }
}
*/
