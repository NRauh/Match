#define CATCH_CONFIG_RUNNER
#include "catch.hpp"
#include "../src/accountmanager.h"
#include "../src/account.h"

int main(int argc, char* const argv[]) {
    AccountManager accManager;
    QUrl filePath = QUrl::fromLocalFile(".");
    accManager.createBudget(filePath, "Foo Budget");

    int result = Catch::Session().run( argc, argv );

    remove("Foo Budget.mbgt");

    return result;
}
