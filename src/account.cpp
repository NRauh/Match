#include "account.h"
#include "accountmanager.h"
#include <QUuid>
#include "sqlite/sqlite.hpp"
#include <iostream>
#include <QJsonArray>

Account::Account(QObject *parent) : QObject(parent)
{

}

void Account::addChecking(QUrl filePath, QString accountName, int balance, QDate balanceDate)
{
    io::sqlite::db budget(filePath.toLocalFile().toStdString());
    io::sqlite::stmt query(budget, "INSERT INTO accounts (accountName, balance) VALUES (?, 0)");
    query.bind().text(1, accountName.toStdString());
    query.exec();

    query.reset();
    query = io::sqlite::stmt(budget, "SELECT last_insert_rowid() FROM accounts");

    int accountId;
    query.exec();
    while (query.step()) {
        accountId = query.row().int32(0);
    }

    addTransaction(filePath, accountId, balanceDate, "Self", false, balance, "Initial Balance");
}

void Account::addTransaction(QUrl filePath, int accountId, QDate date, QString payee, bool outflow, int amount, QString note)
{
    QString formattedDate = date.toString("yyyy-MM-dd");
    io::sqlite::db budget(filePath.toLocalFile().toStdString());
    io::sqlite::stmt query(budget, "INSERT INTO transactions"
                                   "(toAccount, transactionDate, payee, amount, outflow, note)"
                                   "VALUES (?, ?, ?, ?, ?, ?)");
    query.bind().int32(1, accountId);
    query.bind().text(2, formattedDate.toStdString());
    query.bind().text(3, payee.toStdString());
    query.bind().int32(4, amount);
    query.bind().int32(5, 1);
    query.bind().text(6, note.toStdString());
    query.exec();

    query.reset();

    if (outflow) {
        query = io::sqlite::stmt(budget, "UPDATE accounts SET balance = balance - ? WHERE id == ?");
    } else {
        query = io::sqlite::stmt(budget, "UPDATE accounts SET balance = balance + ? WHERE id == ?");
    }

    query.bind().int32(1, amount);
    query.bind().int32(2, accountId);
    query.exec();
}

void Account::deleteTransaction(QUrl filePath, int transactionId)
{
    io::sqlite::db budget(filePath.toLocalFile().toStdString());
    io::sqlite::stmt query(budget, "SELECT toAccount, amount, outflow FROM transactions WHERE id == ?");
    query.bind().int32(1, transactionId);
    query.exec();

    int account;
    int amount;
    bool outflow;
    while (query.step()) {
        account = query.row().int32(0);
        amount = query.row().int32(1);
        outflow = query.row().int32(3);
    }

    query.reset();
    if (outflow) {
        query = io::sqlite::stmt(budget, "UPDATE accounts SET balance = balance + ? WHERE id == ?");
    } else {
        query = io::sqlite::stmt(budget, "UPDATE accounts SET balance = balance - ? WHERE id == ?");
    }
    query.bind().int32(1, amount);
    query.bind().int32(2, account);
    query.exec();

    query = io::sqlite::stmt(budget, "DELETE FROM transactions WHERE id == ?");
    query.bind().int32(1, transactionId);
    query.exec();
}

QJsonObject Account::getAccountList(QUrl filePath)
{
    QJsonObject accountList;
    QJsonArray accounts;
    double totalBalance = 0;

    io::sqlite::db budget(filePath.toLocalFile().toStdString());
    io::sqlite::stmt query(budget, "SELECT id, accountName, balance FROM accounts");

    while (query.step()) {
        QJsonObject account;
        QString accountName = QString::fromStdString(query.row().text(1));
        double balance = query.row().int32(2);
        QString accountBalance = QString::number(balance / 100);

        account.insert("accountId", query.row().int32(0));
        account.insert("accountName", accountName);
        account.insert("accountBalance", accountBalance);

        QJsonValue accountValue(account);
        accounts.append(accountValue);
        totalBalance += query.row().int32(2);
    }

    totalBalance = totalBalance / 100;
    QString formattedNumber = QString::number(totalBalance);
    QJsonValue accountsValue(accounts);

    accountList.insert("balance", formattedNumber);
    accountList.insert("accounts", accountsValue);

    return accountList;
}

/*
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


*/
