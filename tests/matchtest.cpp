#define CATCH_CONFIG_RUNNER
#include "catch.hpp"
#include "../src/accountmanager.h"
#include "../src/account.h"

int main(int argc, char* const argv[]) {
    int result = Catch::Session().run( argc, argv );

    return result;
}
