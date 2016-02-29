#include "catch.hpp"
#include "../src/accountmanager.h"
#include <QUrl>
#include <QFile>
#include <QJsonObject>
#include <QJsonDocument>
#include <QDir>
#include <QDebug>
#include <QSettings>

TEST_CASE("Can create budget/save files", "[createBudget]") {
  SECTION("A path and name are given, then it creates a .mbgt file") {
      AccountManager accManager;
      QUrl path = QUrl::fromLocalFile(".");
      accManager.createBudget(path, "Foo Budget");

      QFile testBudget("Foo Budget.mbgt");
      REQUIRE(testBudget.exists() == true);

      path = QUrl::fromLocalFile("Foo Budget.mbgt");
      QJsonObject budget = accManager.loadFile(path);
      REQUIRE(budget["accountName"] == "Foo Budget");

      testBudget.close();
  }
}

TEST_CASE("Can save a file", "[saveFile]") {
    SECTION("A path and QJsonObject is given and it writes a file") {
        AccountManager accManager;
        QUrl path = QUrl::fromLocalFile("FooBar.json");
        QJsonObject testObject;
        testObject["greeting"] = "Hello World";

        accManager.saveFile(path, testObject);
        QFile testFile("FooBar.json");
        REQUIRE(testFile.exists() == true);
    }
}

TEST_CASE("Can load a file", "[loadFile]") {
    SECTION("A path to a budget file is given and it loads it as a QJsonObject") {
        AccountManager accManager;
        QUrl path = QUrl::fromLocalFile("FooBar.json");

        QJsonObject testFile = accManager.loadFile(path);
        REQUIRE(testFile["greeting"] == "Hello World");
    }
}

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
    }
}

TEST_CASE("Can add checking account", "[addChecking]") {
  SECTION("The details are given, a table for a checking account is created") {
  }
}
