#include "catch.hpp"
#include "../src/accountmanager.h"
#include <QUrl>
#include <QFile>
#include <QJsonObject>
#include <QJsonDocument>
#include <QDir>
#include <QDebug>

TEST_CASE("Can create budget/save files", "[createBudget]") {
  SECTION("A path is given, it creates a .mbgt file") {
      AccountManager accManager;
      QUrl path = QUrl::fromLocalFile(".");
      accManager.createBudget(path, "Foo Budget");

      QFile testBudget("Foo Budget.mbgt");
      REQUIRE(testBudget.exists() == true);

      testBudget.open(QIODevice::ReadOnly);
      QString rawJson = testBudget.readAll();
      QJsonDocument budgetDoc = QJsonDocument::fromJson(rawJson.toUtf8());
      QJsonObject budget = budgetDoc.object();
      REQUIRE(budget.value(QString("accountName")) == "Foo Budget");

      testBudget.close();
  }
}

TEST_CASE("Can add checking account", "[addChecking]") {
  SECTION("The details are given, a table for a checking account is created") {
  }
}
