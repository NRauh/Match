#define CATCH_CONFIG_RUNNER
#include "catch.hpp"
#include "../src/accountmanager.h"
#include "../src/account.h"

int main(int argc, char* const argv[]) {
    AccountManager accManager;
    Account account;
    QUrl filePath = QUrl::fromLocalFile(".");
    accManager.createBudget(filePath, "Foo Budget");

    accManager.createBudget(filePath, "BudgetTestAccount");
    account.addChecking(QUrl::fromLocalFile("BudgetTestAccount.mbgt"), "Foo CU", 200000, QDate(2016, 4, 23));

    int result = Catch::Session().run( argc, argv );

    remove("Foo Budget.mbgt");
    remove("BudgetTestAccount.mbgt");

    return result;
}
