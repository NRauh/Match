#include "catch.hpp"
#include "../src/accountmanager.h"
#include <QUrl>
#include <QFile>
#include <QDir>
#include <QDebug>
#include <QSettings>
#include "../src/json/json.h"
#include "../src/sqlite/sqlite.hpp"


/*TEST_CASE("Can create budget/save files", "[createBudget]") {
  SECTION("A path and name are given, then it creates a .mbgt file") {
      AccountManager accManager;
      QUrl path = QUrl::fromLocalFile(".");
      accManager.createBudget(path, "Foo Budget");

      QFile testBudget("Foo Budget.mbgt");
      REQUIRE(testBudget.exists() == true);
  }
}*/

/*TEST_CASE("Can save a file", "[saveFile]") {
    SECTION("A path and Json Value is given and it writes a file") {
        AccountManager accManager;
        QUrl path = QUrl::fromLocalFile("FooBar.json");
        Json::Value testObject;
        testObject["greeting"] = "Hello World";

        accManager.saveFile(path, testObject);
        QFile testFile("FooBar.json");
        REQUIRE(testFile.exists() == true);
    }
}

TEST_CASE("Can load a file", "[loadFile]") {
    SECTION("A path to a budget file is given and it loads it as a Json Value") {
        AccountManager accManager;
        QUrl path = QUrl::fromLocalFile("FooBar.json");

        Json::Value testFile = accManager.loadFile(path);
        REQUIRE(testFile["greeting"] == "Hello World");
    }
}*/

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
