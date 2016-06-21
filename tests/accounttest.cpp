#include "catch.hpp"
#include "../src/account.h"
#include <QUrl>
#include <QDate>
#include "../src/accountmanager.h"
#include "../src/sqlite/sqlite.hpp"
#include <iostream>
#include <QJsonObject>
#include <QDebug>
#include <QJsonArray>
#include "../src/budget.h"

QString fooCuId;
QString testTransactionId;

//Account account;
//Budget setupBudget;
QUrl pathToDir = QUrl::fromLocalFile(".");
QUrl accountTestPath = QUrl::fromLocalFile("AccountTestFile.mbgt");
QDate now = QDate::currentDate();

TEST_CASE("Can add checking accounts", "[addAccount]") {
    AccountManager accManager;
    Account account;
    accManager.createBudget(pathToDir, "AccountTestFile");

    SECTION("File path, account name, balance, and balance date are given") {
        account.addAccount(accountTestPath, "Foo CU", 10000, now, true);

        io::sqlite::db mbgt("AccountTestFile.mbgt");
        io::sqlite::stmt query(mbgt, "SELECT accountName, balance FROM accounts");

        while (query.step()) {
            std::cout << "1: addAccount (1)\n";
            REQUIRE(query.row().text(0) == "Foo CU");
            REQUIRE(query.row().int32(1) == 10000);
        }

    }

    SECTION("It adds an initial balance transaction") {
        io::sqlite::db mbgt("AccountTestFile.mbgt");
        io::sqlite::stmt query(mbgt, "SELECT toAccount, amount FROM transactions WHERE id == 1");

        while (query.step()) {
            std::cout << "1: addAccount (2)\n";
            REQUIRE(query.row().int32(0) == 1);
            REQUIRE(query.row().int32(1) == 80000);
        }
    }

    SECTION("Accounts can be off budget") {
        io::sqlite::db mbgt("AccountTestFile.mbgt");
        io::sqlite::stmt query(mbgt, "SELECT onBudget FROM accounts WHERE id == 1");

        while (query.step()) {
            std::cout << "1: addAccount (3)\n";
            REQUIRE(query.row().int32(0) == 1);
        }
    }

    remove("AccountTestFile.mbgt");
}

TEST_CASE("Can add transactions to account", "[addTransaction]") {
    AccountManager accManager;
    Account account;
    Budget budget;
    accManager.createBudget(pathToDir, "AccountTestFile");
    account.addAccount(accountTestPath, "Foo CU", 10000, now, true);
    budget.addCategory(accountTestPath, "Eating Out", 1000);

    account.addTransaction(accountTestPath, 1, now, "Caffe Nero", true, 125, "Eating Out", "Espresso");

    SECTION("Given file path, account ID, date, payee, if outflow, amount, and note") {
        io::sqlite::db mbgt("AccountTestFile.mbgt");
        io::sqlite::stmt query(mbgt, "SELECT toAccount, transactionDate,"
                                       "payee, amount, outflow, category, note"
                                       " FROM transactions WHERE id == 2");
        while (query.step()) {
            std::cout << "2: addTransaction (1)\n";
            REQUIRE(query.row().int32(0) == 1);
            REQUIRE(query.row().text(1) == now.toString("yyyy-MM-dd").toStdString());
            REQUIRE(query.row().text(2) == "Caffe Nero");
            REQUIRE(query.row().int32(3) == 125);
            REQUIRE(query.row().int32(4) == 1);
            REQUIRE(query.row().text(5) == "Eating Out");
            REQUIRE(query.row().text(6) == "Espresso");
        }
    }

    SECTION("It removes the amount from the account balance") {
        io::sqlite::db mbgt("AccountTestFile.mbgt");
        io::sqlite::stmt query(mbgt, "SELECT balance FROM accounts WHERE id == 1");

        while (query.step()) {
            std::cout << "2: addTransaction (2)\n";
            REQUIRE(query.row().int32(0) == 9875);
        }
    }

    SECTION("It adds to the spent amount for the budget") {
        io::sqlite::db mbgt("AccountTestFile.mbgt");
        io::sqlite::stmt query(mbgt, "SELECT monthOneSpent FROM budgets WHERE id == 1");

        while(query.step()) {
            std::cout << "2: addTransaction (3)\n";
            REQUIRE(query.row().int32(0) == 125);
        }
    }

    SECTION("If outflow is false, then it's income and should be added") {
        account.addTransaction(accountTestPath, 1, now, "Tip", false, 1000, "Income", "");

        io::sqlite::db mbgt("AccountTestFile.mbgt");
        io::sqlite::stmt query(mbgt, "SELECT balance FROM accounts WHERE id == 1");

        while (query.step()) {
            std::cout << "2: addTransaction (4)\n";
            REQUIRE(query.row().int32(0) == 10875);
        }
    }

    remove("AccountTestFile.mbgt");
}

TEST_CASE("Can delete transactions", "[deleteTransaction]") {
    AccountManager accManager;
    Account account;
    Budget budget;
    accManager.createBudget(pathToDir, "AccountTestFile");
    budget.addCategory(accountTestPath, "Test Category", 1000);
    account.addAccount(accountTestPath, "Foo CU", 10000, now, true);
    account.addTransaction(accountTestPath, 1, now, "Gas Station", true, 500, "Test Category", "Some gas");

    account.deleteTransaction(accountTestPath, 2);

    SECTION("Filepath, and transaction ID will delete that transaction") {
        io::sqlite::db mbgt("AccountTestFile.mbgt");
        io::sqlite::stmt query(mbgt, "SELECT * FROM transactions WHERE id == 2");
        REQUIRE(query.step() == false);
    }

    SECTION("It will add the deleted amount back to the balance") {
        io::sqlite::db mbgt("AccountTestFile.mbgt");
        io::sqlite::stmt query(mbgt, "SELECT balance FROM accounts WHERE id == 1");

        while (query.step()) {
            std::cout << "4: deleteTransaction\n";
            REQUIRE(query.row().int32(0) == 10000);
        }
    }

    SECTION("It removes from the budget spent") {
        io::sqlite::db mbgt("AccountTestFile.mbgt");
        io::sqlite::stmt query(mbgt, "SELECT monthOneSpent FROM budgets WHERE id == 2");

        while (query.step()) {
            std::cout << "4: deleteTransaction (3)\n";
            REQUIRE(query.row().int32(0) == 0);
        }
    }

    remove("AccountTestFile.mbgt");
}

TEST_CASE("Can get a list of accounts and balances", "[getAccountList]") {
    AccountManager accManager;
    Account account;
    accManager.createBudget(pathToDir, "AccountTestFile");
    account.addAccount(accountTestPath, "Foo CU", 10000, now, true);
    account.addAccount(accountTestPath, "A Bank", 20000, now, false);

    SECTION("Give file path, returns name, balance, id alphabetically ordered") {
        QJsonObject accountList = account.getAccountList(accountTestPath);

        REQUIRE(accountList["balance"] == "300.00");
        REQUIRE(accountList["accounts"].toArray()[1].toObject()["accountId"] == 1);
        REQUIRE(accountList["accounts"].toArray()[1].toObject()["accountName"] == "Foo CU");
        REQUIRE(accountList["accounts"].toArray()[1].toObject()["accountBalance"] == "100.00");
    }

    SECTION("If selection is set to 2, then it gets off budget accounts") {
        QJsonObject accountList = account.getAccountList(accountTestPath, 2);
        REQUIRE(accountList["accounts"].toArray().size() == 1);
    }

    SECTION("If selection is set to 1, then it gets on budget accounts") {
        QJsonObject accountList = account.getAccountList(accountTestPath, 1);
        REQUIRE(accountList["accounts"].toArray().size() == 1);
    }

    remove("AccountTestFile.mbgt");
}

TEST_CASE("Can get list of transactions and balance for account", "[getTransactions]") {
    AccountManager accManager;
    Account account;
    Budget budget;
    accManager.createBudget(pathToDir, "AccountTestFile");
    budget.addCategory(accountTestPath, "Eating Out", 1000);
    account.addAccount(accountTestPath, "Foo CU", 10000, now, true);
    account.addTransaction(accountTestPath, 1, now, "Caffe Nero", true, 125, "Eating Out", "Espresso");

    SECTION("Filepath, and accountId returns balance and array of transactions") {
        QJsonObject transactions = account.getTransactions(accountTestPath, 1);

        REQUIRE(transactions["balance"] == "98.75");
        REQUIRE(transactions["transactions"].toArray()[1].toObject()["amount"] == "+100.00");
        REQUIRE(transactions["transactions"].toArray()[0].toObject()["date"] == now.toString("M/d/yy"));
        REQUIRE(transactions["transactions"].toArray()[0].toObject()["payee"] == "Caffe Nero");
        REQUIRE(transactions["transactions"].toArray()[0].toObject()["note"] == "Espresso");
        REQUIRE(transactions["transactions"].toArray()[0].toObject()["amount"] == "-1.25");
        REQUIRE(transactions["transactions"].toArray()[0].toObject()["category"] == "Eating Out");
        REQUIRE(transactions["transactions"].toArray()[0].toObject()["outflow"] == true);
        REQUIRE(transactions["transactions"].toArray()[0].toObject()["intDate"] == now.toString("yyyy-MM-dd"));
        REQUIRE(transactions["transactions"].toArray()[0].toObject()["id"] == 2);
    }

    SECTION("It gets the transactions by date") {
        account.addTransaction(accountTestPath, 1, now.addDays(1), "Foo", true, 500, "Eating Out", "Espresso");
        QJsonObject transactions = account.getTransactions(accountTestPath, 1);

        REQUIRE(transactions["transactions"].toArray()[0].toObject()["payee"] == "Foo");
        REQUIRE(transactions["transactions"].toArray()[1].toObject()["payee"] == "Caffe Nero");
        REQUIRE(transactions["transactions"].toArray()[2].toObject()["payee"] == "Self");
    }

    remove("AccountTestFile.mbgt");
}

TEST_CASE("Can get budget status", "[isOnBudget]") {
    AccountManager accManager;
    Account account;
    Budget budget;
    accManager.createBudget(pathToDir, "AccountTestFile");
    budget.addCategory(accountTestPath, "Eating Out", 1000);
    account.addAccount(accountTestPath, "Foo CU", 10000, now, true);
    account.addAccount(accountTestPath, "Hello Bank", 20000, now, false);

    SECTION("File path and account ID returns a bool for it's on budget status") {
        bool onBudget = account.isOnBudget(accountTestPath, 1);
        REQUIRE(onBudget == true);

        onBudget = account.isOnBudget(accountTestPath, 2);
        REQUIRE(onBudget == false);
    }

    SECTION("Off budget accounts, don't affect the budget") {
        account.addTransaction(accountTestPath, 2, now, "Caffe Nero", true, 125, "Eating Out", "Espresso");

        io::sqlite::db mbgt("AccountTestFile.mbgt");
        io::sqlite::stmt query(mbgt, "SELECT monthOneSpent FROM budgets");

        while (query.step()) {
            std::cout << "5: isOnBudget (1)\n";
            REQUIRE(query.row().int32(0) == 0);
        }

        account.deleteAccount(accountTestPath, 3);

        query.exec();
        while (query.step()) {
            std::cout << "5: isOnBudget (2)\n";
            REQUIRE(query.row().int32(0) == 0);
        }
    }

    remove("AccountTestFile.mbgt");
}

TEST_CASE("Can set an account to be on or off budget (for the future)", "[changeOnBudget]") {
    AccountManager accManager;
    Account account;
    accManager.createBudget(pathToDir, "AccountTestFile");
    account.addAccount(accountTestPath, "Foo CU", 10000, now, false);
    account.addAccount(accountTestPath, "Hello Bank", 20000, now, true);

    SECTION("Give file path, account id and bool of new status") {
        account.changeOnBudget(accountTestPath, 1, true);

        io::sqlite::db mbgt("AccountTestFile.mbgt");
        io::sqlite::stmt query(mbgt, "SELECT onBudget FROM accounts WHERE id == 1");

        while (query.step()) {
            REQUIRE(query.row().int32(0) == 1);
        }

        account.changeOnBudget(accountTestPath, 2, false);

        query = io::sqlite::stmt(mbgt, "SELECT onBudget FROM accounts WHERE id == 2");

        while (query.step()) {
            REQUIRE(query.row().int32(0) == 0);
        }
    }

    remove("AccountTestFile.mbgt");
}

TEST_CASE("Can delete accounts", "[deleteAccount]") {
    AccountManager accManager;
    Account account;
    accManager.createBudget(pathToDir, "AccountTestFile");
    account.addAccount(accountTestPath, "Foo CU", 10000, now, true);

    SECTION("Give file path and account id, then it deletes all transactions and the account") {
        account.deleteAccount(accountTestPath, 1);

        io::sqlite::db mbgt("AccountTestFile.mbgt");
        io::sqlite::stmt query(mbgt, "SELECT transactionDate FROM transactions WHERE toAccount == 1");
        REQUIRE(query.step() == false);

        query = io::sqlite::stmt(mbgt, "SELECT accountName FROM accounts WHERE id == 1");
        REQUIRE(query.step() == false);
    }

    remove("AccountTestFile.mbgt");
}
