#include "catch.hpp"
#include "../src/account.h"
#include <QUrl>
#include <QDate>
#include "../src/json/json.h"
#include "../src/accountmanager.h"
#include "../src/sqlite/sqlite.hpp"
#include <iostream>
#include <QJsonObject>
#include <QDebug>
#include <QJsonArray>
#include "../src/budget.h"

QString fooCuId;
QString testTransactionId;

Account account;
Budget setupBudget;
QUrl filePath = QUrl::fromLocalFile("Foo Budget.mbgt");
QDate transactionDate = QDate::currentDate();


TEST_CASE("Can add checking accounts", "[addAccount]") {
    SECTION("File path, account name, balance, and balance date are given") {
        account.addAccount(filePath, "Foo CU", 80000, transactionDate, true);

        io::sqlite::db budget("Foo Budget.mbgt");
        io::sqlite::stmt query(budget, "SELECT accountName, balance FROM accounts");
        while (query.step()) {
            std::cout << "1: addAccount (1)\n";
            REQUIRE(query.row().text(0) == "Foo CU");
            REQUIRE(query.row().int32(1) == 80000);
        }

        query.reset();
        query = io::sqlite::stmt(budget, "SELECT toAccount, amount FROM transactions WHERE id == 1");
        query.exec();
        while (query.step()) {
            std::cout << "1: addAccount (2)\n";
            REQUIRE(query.row().int32(0) == 1);
            REQUIRE(query.row().int32(1) == 80000);
        }
    }

    SECTION("Accounts can be off budget") {
        io::sqlite::db budget("Foo Budget.mbgt");
        io::sqlite::stmt query(budget, "SELECT onBudget FROM accounts WHERE id == 1");

        while (query.step()) {
            std::cout << "1: addAccount (3)\n";
            REQUIRE(query.row().int32(0) == 1);
        }
    }
}

TEST_CASE("Can add transactions to account", "[addTransaction]") {
    SECTION("Given file path, account ID, date, payee, if outflow, amount, and note") {
        setupBudget.addCategory(filePath, "Eating Out", 10000);
        account.addTransaction(filePath, 1, transactionDate, "Caffe Nero", true, 125, "Eating Out", "Espresso");
        io::sqlite::db budget("Foo Budget.mbgt");
        io::sqlite::stmt query(budget, "SELECT toAccount, transactionDate,"
                                       "payee, amount, outflow, category, note"
                                       " FROM transactions WHERE id == 2");
        while (query.step()) {
            std::cout << "2: addTransaction (1)\n";
            REQUIRE(query.row().int32(0) == 1);
            //REQUIRE(query.row().text(1) == "2016-02-29");
            REQUIRE(query.row().text(1) == transactionDate.toString("yyyy-MM-dd").toStdString());
            REQUIRE(query.row().text(2) == "Caffe Nero");
            REQUIRE(query.row().int32(3) == 125);
            REQUIRE(query.row().int32(4) == 1);
            REQUIRE(query.row().text(5) == "Eating Out");
            REQUIRE(query.row().text(6) == "Espresso");
        }
        io::sqlite::stmt q(budget, "SELECT balance FROM accounts WHERE id == 1");
        while (q.step()) {
            std::cout << "2: addTransaction (2)\n";
            REQUIRE(q.row().int32(0) == 79875);
        }
    }

    SECTION("It adds to the spent amount for the budget") {
        io::sqlite::db budget("Foo Budget.mbgt");
        io::sqlite::stmt query(budget, "SELECT monthOneSpent FROM budgets WHERE id == 1");

        while(query.step()) {
            std::cout << "2: addTransaction (3)\n";
            REQUIRE(query.row().int32(0) == 125);
        }
    }

    SECTION("If outflow is false, then it's income and should be added") {
        account.addTransaction(filePath, 1, transactionDate, "Tip", false, 1000, "Income", "");
        io::sqlite::db budget("Foo Budget.mbgt");
        io::sqlite::stmt query(budget, "SELECT balance FROM accounts WHERE id == 1");
        while (query.step()) {
            std::cout << "2: addTransaction (4)\n";
            REQUIRE(query.row().int32(0) == 80875);
        }
    }
}

/*
TEST_CASE("Can update a transaction", "[updateTransaction]") {
    SECTION("Filepath, transaction ID, date, payee, outflow, amount, and note") {
        QDate transactionDate = QDate(2016, 2, 28);

        account.updateTransaction(filePath, 3, transactionDate, "Tips", true, 1100, "Eating Out", "Tip I guess");

        io::sqlite::db budget("Foo Budget.mbgt");
        io::sqlite::stmt query(budget, "SELECT transactionDate, payee, amount, outflow, note FROM transactions WHERE id == 3");
        while (query.step()) {
            std::cout << "3: updateTransaction (1)\n";
            REQUIRE(query.row().text(0) == "2016-02-28");
            REQUIRE(query.row().text(1) == "Tips");
            REQUIRE(query.row().int32(2) == 1100);
            REQUIRE(query.row().int32(3) == true);
            REQUIRE(query.row().text(4) == "Tip I guess");
        }
    }

    SECTION("It shows on the balance") {
        io::sqlite::db budget("Foo Budget.mbgt");
        io::sqlite::stmt query(budget, "SELECT balance FROM accounts WHERE id == 1");
        while (query.step()) {
            std::cout << "3: updateTransaction (3)\n";
            REQUIRE(query.row().int32(0) == 78775);
        }
        //changing it back
        account.updateTransaction(filePath, 3, transactionDate, "Tip", false, 1000, "Income", "");
    }
}
*/

TEST_CASE("Can delete transactions", "[deleteTransaction]") {
    SECTION("Filepath, and transaction ID will delete that transaction") {
        account.deleteTransaction(filePath, 3);

        io::sqlite::db budget("Foo Budget.mbgt");
        io::sqlite::stmt query(budget, "SELECT * FROM transactions WHERE id == 3");
        REQUIRE(query.step() == false);
        query.reset();
        query = io::sqlite::stmt(budget, "SELECT balance FROM accounts WHERE id == 1");
        query.exec();
        while (query.step()) {
            std::cout << "4: deleteTransaction\n";
            REQUIRE(query.row().int32(0) == 79875);
        }

        setupBudget.addCategory(filePath, "Fuel", 10000);
        account.addTransaction(filePath, 1, transactionDate, "Gas Station", 1, 1100, "Fuel", "Some gas");
        account.deleteTransaction(filePath, 3);
        query.exec();
        while (query.step()) {
            std::cout << "4: deleteTransaction (2)\n";
            REQUIRE(query.row().int32(0) == 79875);
        }
    }

    SECTION("It removes from the budget spent") {
        io::sqlite::db budget("Foo Budget.mbgt");
        io::sqlite::stmt query(budget, "SELECT monthOneSpent FROM budgets WHERE id == 2");

        while (query.step()) {
            std::cout << "4: deleteTransaction (3)\n";
            REQUIRE(query.row().int32(0) == 0);
        }
    }
}

TEST_CASE("Can get a list of accounts and balances", "[getAccountList]") {
    // Getting it's budget status is to make the account settings look pretty, by having a toggle
    // But it may be useless to have the selection option because of that
    // If someone wants to do some funky sorting for the side bar I'd be fine removing the selection option

    SECTION("Give file path, returns name, balance, id, and if it's on or off budget") {
        QJsonObject accountList = account.getAccountList(filePath);
        REQUIRE(accountList["balance"] == "798.75");
        REQUIRE(accountList["accounts"].toArray()[0].toObject()["accountId"] == 1);
        REQUIRE(accountList["accounts"].toArray()[0].toObject()["accountName"] == "Foo CU");
        REQUIRE(accountList["accounts"].toArray()[0].toObject()["accountBalance"] == "798.75");
        REQUIRE(accountList["accounts"].toArray()[0].toObject()["accountOnBudget"] == true);
    }

    SECTION("If selection is set to 2, then it gets off budget accounts") {
        account.addAccount(filePath, "A bank", 10000, transactionDate, false);
        QJsonObject accountList = account.getAccountList(filePath, 2);
        REQUIRE(accountList["accounts"].toArray().size() == 1);
    }

    SECTION("If selection is set to 1, then it gets on budget accounts") {
        QJsonObject accountList = account.getAccountList(filePath, 1);
        REQUIRE(accountList["accounts"].toArray().size() == 1);
    }

    SECTION("Accounts are alphabetized") {
        QJsonObject accountList = account.getAccountList(filePath);
        REQUIRE(accountList["accounts"].toArray()[0].toObject()["accountName"] == "A bank");
        REQUIRE(accountList["accounts"].toArray()[1].toObject()["accountName"] == "Foo CU");
    }
}

TEST_CASE("Can get list of transactions and balance for account", "[getTransactions]") {
    SECTION("Filepath, and accountId returns balance and array of transactions") {
        QJsonObject transactions = account.getTransactions(filePath, 1);
        QString date = transactionDate.toString("M/d/yy");
        QString longDate = transactionDate.toString("yyyy-MM-dd");

        REQUIRE(transactions["balance"] == "798.75");
        REQUIRE(transactions["transactions"].toArray()[0].toObject()["amount"] == "+800.00");
        REQUIRE(transactions["transactions"].toArray()[1].toObject()["date"] == date);
        REQUIRE(transactions["transactions"].toArray()[1].toObject()["payee"] == "Caffe Nero");
        REQUIRE(transactions["transactions"].toArray()[1].toObject()["note"] == "Espresso");
        REQUIRE(transactions["transactions"].toArray()[1].toObject()["amount"] == "-1.25");
        REQUIRE(transactions["transactions"].toArray()[1].toObject()["category"] == "Eating Out");
        REQUIRE(transactions["transactions"].toArray()[1].toObject()["outflow"] == true);
        REQUIRE(transactions["transactions"].toArray()[1].toObject()["intDate"] == longDate);
        REQUIRE(transactions["transactions"].toArray()[1].toObject()["id"] == 2);
    }
}

TEST_CASE("Can get budget status", "[isOnBudget]") {
    SECTION("File path and account ID returns a bool for it's on budget status") {
        bool onBudget = account.isOnBudget(filePath, 1);
        REQUIRE(onBudget == true);

        account.addAccount(filePath, "Hello Bank", 50000, transactionDate, false);
        onBudget = account.isOnBudget(filePath, 2);
        REQUIRE(onBudget == false);
    }

    SECTION("Off budget accounts, don't affect the budget") {
        account.addTransaction(filePath, 2, transactionDate, "Caffe Nero", true, 125, "Eating Out", "Espresso");

        io::sqlite::db budget("Foo Budget.mbgt");
        io::sqlite::stmt query(budget, "SELECT balance FROM accounts WHERE id == 1");
        while (query.step()) {
            std::cout << "5: isOnBudget (1)\n";
            REQUIRE(query.row().int32(0) == 79875);
        }

        // This test seems to pass regardless, and it's actuall success needs to be checked manually
        // #QualityCode
        account.deleteTransaction(filePath, 4);

        query.exec();
        while (query.step()) {
            std::cout << "5: isOnBudget (2)\n";
            REQUIRE(query.row().int32(0) == 79875);
        }
    }
}

TEST_CASE("Can set an account to be on or off budget (for the future)", "[changeOnBudget]") {
    SECTION("Give file path, account id and bool of new status") {
        account.changeOnBudget(filePath, 2, true);

        io::sqlite::db budget("Foo Budget.mbgt");
        io::sqlite::stmt query(budget, "SELECT onBudget FROM accounts WHERE id == 2");

        while(query.step()) {
            REQUIRE(query.row().int32(0) == 1);
        }
    }
}

