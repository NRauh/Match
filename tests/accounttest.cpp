#include "catch.hpp"
#include "../src/account.h"
#include <QUrl>
#include <QDate>
#include "../src/json/json.h"
#include "../src/accountmanager.h"

TEST_CASE("Can add checking accounts", "[addChecking]") {
    SECTION("File path, account name, balance, and balance date are given") {
        Account account;
        AccountManager accManager;
        QUrl filePath = QUrl::fromLocalFile("Foo Budget.mbgt");
        QDate balanceDate = QDate(2016, 2, 29);

        account.addChecking(filePath, "Foo CU", 80000, balanceDate);
        Json::Value budget = accManager.loadFile(filePath);
        REQUIRE(budget["onBudgetAccounts"][0]["accountName"] == "Foo CU");
        REQUIRE(budget["onBudgetAccounts"][0]["balance"] == 80000);
        REQUIRE(budget["onBudgetAccounts"][0]["transactions"][0]["note"] == "Initial Balance");
        REQUIRE(budget["balance"] == 80000);
    }
}
