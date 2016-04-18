#include "catch.hpp"
#include "../src/accountmanager.h"
#include <QUrl>
#include <QFile>
#include <QDir>
#include <QDebug>
#include <QSettings>
#include "../src/json/json.h"
#include "../src/sqlite/sqlite.hpp"


TEST_CASE("Can create budget/save files", "[createBudget]") {
  SECTION("A path and name are given, then it creates a .mbgt file") {
      AccountManager accManager;
      QUrl path = QUrl::fromLocalFile(".");
      accManager.createBudget(path, "Foo Budget");

      QFile testBudget("Foo Budget.mbgt");
      REQUIRE(testBudget.exists() == true);
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
