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

QString fooCuId;
QString testTransactionId;

TEST_CASE("Can add checking accounts", "[addChecking]") {
    SECTION("File path, account name, balance, and balance date are given") {
        Account account;
        AccountManager accManager;
        QUrl filePath = QUrl::fromLocalFile("Foo Budget.mbgt");
        QDate balanceDate = QDate(2016, 2, 29);

        account.addChecking(filePath, "Foo CU", 80000, balanceDate);

        io::sqlite::db budget("Foo Budget.mbgt");
        io::sqlite::stmt query(budget, "SELECT accountName, balance FROM accounts");
        while (query.step()) {
            std::cout << "1: addChecking (1)\n";
            REQUIRE(query.row().text(0) == "Foo CU");
            REQUIRE(query.row().int32(1) == 80000);
        }

        query.reset();
        query = io::sqlite::stmt(budget, "SELECT toAccount, amount FROM transactions WHERE id == 1");
        query.exec();
        while (query.step()) {
            std::cout << "1: addChecking (2)\n";
            REQUIRE(query.row().int32(0) == 1);
            REQUIRE(query.row().int32(1) == 80000);
        }
    }
}

TEST_CASE("Can add transactions to account", "[addTransaction]") {
    SECTION("Given file path, account ID, date, payee, if outflow, amount, and note") {
        Account account;
        AccountManager accManager;
        QUrl filePath = QUrl::fromLocalFile("Foo Budget.mbgt");
        QDate transactionDate = QDate(2016, 2, 29);

        account.addTransaction(filePath, 1, transactionDate, "Caffe Nero", true, 125, "Espresso");
        io::sqlite::db budget("Foo Budget.mbgt");
        io::sqlite::stmt query(budget, "SELECT toAccount, transactionDate, payee, amount, outflow, note FROM transactions WHERE id == 2");
        while (query.step()) {
            std::cout << "2: addTransaction (1)\n";
            REQUIRE(query.row().int32(0) == 1);
            REQUIRE(query.row().text(1) == "2016-02-29");
            REQUIRE(query.row().text(2) == "Caffe Nero");
            REQUIRE(query.row().int32(3) == 125);
            REQUIRE(query.row().int32(4) == 1);
            REQUIRE(query.row().text(5) == "Espresso");
        }
        io::sqlite::stmt q(budget, "SELECT balance FROM accounts WHERE id == 1");
        while (q.step()) {
            std::cout << "2: addTransaction (2)\n";
            REQUIRE(q.row().int32(0) == 79875);
        }
    }

    SECTION("If outflow is false, then it's income and should be added") {
        Account account;
        AccountManager accManager;
        QUrl filePath = QUrl::fromLocalFile("Foo Budget.mbgt");
        QDate transactionDate = QDate(2016, 2, 29);

        account.addTransaction(filePath, 1, transactionDate, "Tip", false, 1000, "");
        io::sqlite::db budget("Foo Budget.mbgt");
        io::sqlite::stmt query(budget, "SELECT balance FROM accounts WHERE id == 1");
        while (query.step()) {
            std::cout << "2: addTransaction (3)\n";
            REQUIRE(query.row().int32(0) == 80875);
        }
    }
}

TEST_CASE("Can delete transactions", "[deleteTransaction]") {
    SECTION("Filepath, and transaction ID will delete that transaction") {
        Account account;
        QUrl filePath = QUrl::fromLocalFile("Foo Budget.mbgt");

        account.deleteTransaction(filePath, 3);

        io::sqlite::db budget("Foo Budget.mbgt");
        io::sqlite::stmt query(budget, "SELECT * FROM transactions WHERE id == 3");
        REQUIRE(query.step() == false);
        query.reset();
        query = io::sqlite::stmt(budget, "SELECT balance FROM accounts WHERE id == 1");
        query.exec();
        while (query.step()) {
            std::cout << "3: deleteTransaction\n";
            REQUIRE(query.row().int32(0) == 79875);
        }
    }
}

TEST_CASE("Can get a list of accounts and balances", "[getAccountList]") {
    SECTION("Give file path, returns name, balance, and id") {
        Account account;
        QUrl filePath = QUrl::fromLocalFile("Foo Budget.mbgt");

        QJsonObject accountList = account.getAccountList(filePath);
        REQUIRE(accountList["balance"] == "798.75");
        REQUIRE(accountList["accounts"].toArray()[0].toObject()["accountId"] == 1);
        REQUIRE(accountList["accounts"].toArray()[0].toObject()["accountName"] == "Foo CU");
        REQUIRE(accountList["accounts"].toArray()[0].toObject()["accountBalance"] == "798.75");
    }
}

/*
TEST_CASE("Can get list of transactions, and account balance", "[getTransactions]") {
    SECTION("Filepath, and ID returns balance and array of transactions") {
        Account account;
        QUrl filePath = QUrl::fromLocalFile("Foo Budget.mbgt");

        Json::Value transactions = account.getTransactions(filePath, fooCuId);

        REQUIRE(transactions["balance"] == "808.75");
        REQUIRE(transactions["transactions"][0]["amount"] == "+800.00");
        REQUIRE(transactions["transactions"][1]["date"] == "2/29/16");
        REQUIRE(transactions["transactions"][1]["payee"] == "Caffe Nero");
        REQUIRE(transactions["transactions"][1]["note"] == "Espresso");
        REQUIRE(transactions["transactions"][1]["amount"] == "-1.25");
        REQUIRE(transactions["transactions"][1]["outflow"] == true);
        REQUIRE(transactions["transactions"][1]["intDate"] == "2016-02-29");
        REQUIRE(transactions["transactions"][1]["id"].asString() == testTransactionId.toStdString());
    }

    SECTION("Can run as QString") {
        Account account;
        QUrl filePath = QUrl::fromLocalFile("Foo Budget.mbgt");

        QString transactionsString = account.getTransactionsString(filePath, fooCuId);
        Json::Value transactions;
        Json::Reader reader;
        reader.parse(transactionsString.toStdString(), transactions);

        REQUIRE(transactions["balance"] == "808.75");
    }
}*/
