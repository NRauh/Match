#include "account.h"
#include "accountmanager.h"
#include <QUuid>
#include "sqlite/sqlite.hpp"

Account::Account(QObject *parent) : QObject(parent)
{

}

void Account::addChecking(QUrl filePath, QString accountName, int balance, QDate balanceDate)
{
    //SQLite::Database budget(filePath.toLocalFile().toStdString());
    //SQLite::Statement query(budget, "INSERT INTO accounts (accountName, balance) VALUES (?, 0)");
    //query.bind(1, accountName.toStdString());
    //query.reset();
    io::sqlite::db budget(filePath.toLocalFile().toStdString());
    io::sqlite::stmt query(budget, "INSERT INTO accounts (accountName, balance) VALUES (?, 0)");
    query.bind().text(1, accountName.toStdString());
    query.exec();
}

/*
void Account::addTransaction(QUrl filePath, int accountId, QDate date, QString payee, bool outflow, int amount, QString note)
{
    AccountManager accManager;
    //Json::Value budget = accManager.loadFile(filePath);
    QString formattedDate = date.toString("yyyy-MM-dd");
    QUuid transactionId = QUuid::createUuid();
    int accountIndex;

    //Json::Value transaction;
    //transaction["id"] = transactionId.toString().toStdString();
    //transaction["date"] = formattedDate.toStdString();
    //transaction["payee"] = payee.toStdString();
    //transaction["outflow"] = outflow;
    //transaction["amount"] = amount;
    //transaction["note"] = note.toStdString();

    //for (int i = 0; i < (int)budget["onBudgetAccounts"].size(); i++) {
        //if (budget["onBudgetAccounts"][i]["accountId"].asString() == accountId.toStdString()) {
            //accountIndex = i;
            //break;
        //}
    //}
    //SQLite::Database budget(filePath.toString().toStdString());
    SQLite::Database budget(filePath.toString().toStdString(), SQLITE_OPEN_READWRITE);
    SQLite::Statement query(budget, "INSERT INTO transactions(toAccount, transactionDate,"
                                    "payee, amount, outflow, note) VALUES (?, ?, ?, ?, ?, ?)");

    std::cout << "HELLO WORLD" << std::endl;
    query.bind(1, accountId);
    query.bind(1, formattedDate.toStdString());
    query.bind(1, payee.toStdString());
    query.bind(1, amount);
    query.bind(1, (int)outflow);
    query.bind(1, note.toStdString());
    query.reset();

    //int accBalance = budget["onBudgetAccounts"][accountIndex]["balance"].asInt();
    //int balance = budget["balance"].asInt();

    //if (outflow) {
        //budget["onBudgetAccounts"][accountIndex]["balance"] = accBalance - amount;
        //budget["balance"] = balance - amount;
    //} else {
        //budget["onBudgetAccounts"][accountIndex]["balance"] = accBalance + amount;
        //budget["balance"] = balance + amount;
    //}

    //budget["onBudgetAccounts"][accountIndex]["transactions"].append(transaction);
    //accManager.saveFile(filePath, budget);
}

Json::Value Account::getAccountList(QUrl filePath)
{
    AccountManager accManager;
    Json::Value budget = accManager.loadFile(filePath);
    Json::Value accountList;
    std::string totalBalance = budget["balance"].asString();
    totalBalance = totalBalance.insert(totalBalance.length() - 2, ".");

    accountList["balance"] = totalBalance;

    for (int i = 0; i < (int)budget["onBudgetAccounts"].size(); i++) {
        std::string accountBalance = budget["onBudgetAccounts"][i]["balance"].asString();
        accountBalance = accountBalance.insert(accountBalance.length() - 2, ".");

        accountList["accounts"][i]["accountName"] = budget["onBudgetAccounts"][i]["accountName"];
        accountList["accounts"][i]["accountBalance"] = accountBalance;
        accountList["accounts"][i]["accountId"] = budget["onBudgetAccounts"][i]["accountId"];
    }

    return accountList;
}

QString Account::getAccountListString(QUrl filePath)
{
    Json::Value accountList = getAccountList(filePath);
    QString accounts = accountList.toStyledString().c_str();
    return accounts;
}

Json::Value Account::getTransactions(QUrl filePath, QString accountId)
{
    AccountManager accManager;
    Json::Value budget = accManager.loadFile(filePath);

    for (int i = 0; i < (int)budget["onBudgetAccounts"].size(); i++) {
        if (budget["onBudgetAccounts"][i]["accountId"].asString() == accountId.toStdString()) {
            budget = budget["onBudgetAccounts"][i];
            break;
        }
    }

    Json::Value transactions;
    std::string accountBalance = budget["balance"].asString();

    transactions["balance"] = accountBalance.insert(accountBalance.length() - 2, ".");

    for (int i = 0; i < (int)budget["transactions"].size(); i++) {
        std::string amountString = budget["transactions"][i]["amount"].asString();
        QString date = budget["transactions"][i]["date"].asCString();
        // TODO: Allow people to chose this format
        QDate formattedDate = QDate::fromString(date, QString("yyyy-MM-dd"));

        transactions["transactions"][i]["id"] = budget["transactions"][i]["id"];
        transactions["transactions"][i]["date"] = formattedDate.toString("M/d/yy").toStdString();
        transactions["transactions"][i]["intDate"] = date.toStdString();
        transactions["transactions"][i]["payee"] = budget["transactions"][i]["payee"];
        transactions["transactions"][i]["note"] = budget["transactions"][i]["note"];
        transactions["transactions"][i]["outflow"] = budget["transactions"][i]["outflow"].asBool();

        amountString.insert(amountString.length() - 2, ".");
        if (budget["transactions"][i]["outflow"].asBool()) {
            amountString.insert(0, "-");
        } else {
            amountString.insert(0, "+");
        }
        transactions["transactions"][i]["amount"] = amountString;
    }

    return transactions;
}

QString Account::getTransactionsString(QUrl filePath, QString accountId)
{
    Json::Value transactions = getTransactions(filePath, accountId);
    QString transactionString = transactions.toStyledString().c_str();
    return transactionString;
}

void Account::deleteTransaction(QUrl filePath, QString accountId, QString transactionId)
{
    AccountManager accManager;
    Json::Value budget = accManager.loadFile(filePath);
    Json::Value account;
    Json::Value transactions;
    int accountIndex;

    for (int i = 0; i < (int)budget.size(); i++) {
        if (budget["onBudgetAccounts"][i]["accountId"] == accountId.toStdString()) {
            accountIndex = i;
            account = budget["onBudgetAccounts"][i];
            break;
        }
    }

    for (int i = 0; i < (int)account["transactions"].size(); i++) {
        if (account["transactions"][i]["id"] == transactionId.toStdString()) {
            int amount = account["transactions"][i]["amount"].asInt();
            if (account["transactions"][i]["outflow"].asBool()) {
                account["balance"] = account["balance"].asInt() + amount;
                budget["balance"] = budget["balance"].asInt() + amount;
            } else {
                account["balance"] = account["balance"].asInt() - amount;
                budget["balance"] = budget["balance"].asInt() - amount;
            }
        } else {
            transactions[transactions.size()] = account["transactions"][i];
        }
    }

    account["transactions"] = transactions;
    budget["onBudgetAccounts"][accountIndex] = account;
    accManager.saveFile(filePath, budget);
}
*/
