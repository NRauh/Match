#include "account.h"
#include "accountmanager.h"
#include <QUuid>
#include "sqlite/sqlite.hpp"
#include <iostream>
#include <QJsonArray>
#include "budget.h"

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

    addTransaction(filePath, accountId, balanceDate, "Self", false, balance, "Income", "Initial Balance");
}

void Account::addTransaction(QUrl filePath, int accountId, QDate date, QString payee, bool outflow, int amount, QString category, QString note)
{
    Budget mbgt;
    QString formattedDate = date.toString("yyyy-MM-dd");
    io::sqlite::db budget(filePath.toLocalFile().toStdString());
    io::sqlite::stmt query(budget, "INSERT INTO transactions"
                                   "(toAccount, transactionDate, payee, amount, outflow, category, note)"
                                   "VALUES (?, ?, ?, ?, ?, ?, ?)");
    query.bind().int32(1, accountId);
    query.bind().text(2, formattedDate.toStdString());
    query.bind().text(3, payee.toStdString());
    query.bind().int32(4, amount);
    query.bind().int32(5, outflow);
    query.bind().text(6, category.toStdString());
    query.bind().text(7, note.toStdString());
    query.exec();

    query.reset();

    if (outflow) {
        query = io::sqlite::stmt(budget, "UPDATE accounts SET balance = balance - ? WHERE id == ?");
        mbgt.addToSpent(filePath, category, date.toString("yyyy-MM"), amount);
    } else {
        query = io::sqlite::stmt(budget, "UPDATE accounts SET balance = balance + ? WHERE id == ?");
    }

    query.bind().int32(1, amount);
    query.bind().int32(2, accountId);
    query.exec();
}

void Account::updateTransaction(QUrl filePath, int transactionId, QDate date, QString payee, bool outflow, int amount, QString note)
{
    io::sqlite::db budget(filePath.toLocalFile().toStdString());
    io::sqlite::stmt query(budget, "SELECT outflow, amount, toAccount FROM transactions WHERE id == ?");

    query.bind().int32(1, transactionId);
    query.exec();

    while (query.step()) {
        bool previousOutflow = query.row().int32(0);
        int previousAmount = query.row().int32(1);
        int accountId = query.row().int32(2);
        io::sqlite::stmt loopQuery;

        if (previousOutflow) {
            loopQuery = io::sqlite::stmt(budget, "UPDATE accounts SET balance = balance + ? WHERE id == ?");
        } else {
            loopQuery = io::sqlite::stmt(budget, "UPDATE accounts SET balance = balance - ? WHERE id == ?");
        }

        loopQuery.bind().int32(1, previousAmount);
        loopQuery.bind().int32(2, accountId);
        loopQuery.exec();
        loopQuery.reset();

        if (outflow) {
            loopQuery = io::sqlite::stmt(budget, "UPDATE accounts SET balance = balance - ? WHERE id == ?");
        } else {
            loopQuery = io::sqlite::stmt(budget, "UPDATE accounts SET balance = balance + ? WHERE id == ?");
        }

        loopQuery.bind().int32(1, amount);
        loopQuery.bind().int32(2, accountId);
        loopQuery.exec();
    }

    query = io::sqlite::stmt(budget, "UPDATE transactions SET transactionDate = ?, payee = ?, amount = ?, outflow = ?, note = ? WHERE id == ?");

    QString formattedDate = date.toString("yyyy-MM-dd");
    query.bind().text(1, formattedDate.toStdString());
    query.bind().text(2, payee.toStdString());
    query.bind().int32(3, amount);
    query.bind().int32(4, outflow);
    query.bind().text(5, note.toStdString());
    query.bind().int32(6, transactionId);
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
        outflow = query.row().int32(2);
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
    int totalBalance = 0;

    io::sqlite::db budget(filePath.toLocalFile().toStdString());
    io::sqlite::stmt query(budget, "SELECT id, accountName, balance FROM accounts");

    while (query.step()) {
        QJsonObject account;
        QString accountName = QString::fromStdString(query.row().text(1));
        QString accountBalance = QString::number(query.row().int32(2));
        accountBalance.insert(accountBalance.length() - 2, ".");

        account.insert("accountId", query.row().int32(0));
        account.insert("accountName", accountName);
        account.insert("accountBalance", accountBalance);

        QJsonValue accountValue(account);
        accounts.append(accountValue);
        totalBalance += query.row().int32(2);
    }

    QString formattedBalance = QString::number(totalBalance);
    formattedBalance.insert(formattedBalance.length() - 2, ".");
    QJsonValue accountsValue(accounts);

    accountList.insert("balance", formattedBalance);
    accountList.insert("accounts", accountsValue);

    return accountList;
}

QJsonObject Account::getTransactions(QUrl filePath, int accountId)
{
    //AccountManager accManager;
    //Json::Value budget = accManager.loadFile(filePath);

    //for (int i = 0; i < (int)budget["onBudgetAccounts"].size(); i++) {
        //if (budget["onBudgetAccounts"][i]["accountId"].asString() == accountId.toStdString()) {
            //budget = budget["onBudgetAccounts"][i];
            //break;
        //}
    //}

    //Json::Value transactions;
    QJsonObject transactionList;
    int balance;
    //std::string accountBalance = budget["balance"].asString();

    //transactions["balance"] = accountBalance.insert(accountBalance.length() - 2, ".");

    io::sqlite::db budget(filePath.toLocalFile().toStdString());
    io::sqlite::stmt query(budget, "SELECT balance FROM accounts WHERE id == ?");
    query.bind().int32(1, accountId);
    query.exec();

    while (query.step()) {
        balance = query.row().int32(0);
    }

    query.reset();
    query = io::sqlite::stmt(budget, "SELECT transactionDate, payee, amount,"
                                     "category, outflow, note, id"
                                     " FROM transactions WHERE toAccount == ?");
    query.bind().int32(1, accountId);
    query.exec();

    QJsonArray transactions;
    while (query.step()) {
        QJsonObject transaction;
        bool outflow = query.row().int32(4);
        QString formattedAmount = QString::fromStdString(query.row().text(2));
        formattedAmount.insert(formattedAmount.length() - 2, ".");
        if (outflow) {
            formattedAmount.prepend("-");
        } else {
            formattedAmount.prepend("+");
        }

        QString date = QString::fromStdString(query.row().text(0));
        QDate formattedDate = QDate::fromString(date, QString("yyyy-MM-dd"));

        QString payee = QString::fromStdString(query.row().text(1));
        QString category = QString::fromStdString(query.row().text(3));
        QString note = QString::fromStdString(query.row().text(5));
        int transactionId = query.row().int32(6);

        transaction.insert("amount", formattedAmount);
        transaction.insert("date", formattedDate.toString("M/d/yy"));
        transaction.insert("payee", payee);
        transaction.insert("category", category);
        transaction.insert("note", note);
        transaction.insert("outflow", outflow);
        transaction.insert("intDate", date);
        transaction.insert("id", transactionId);
        transactions.append(transaction);
    }

    QString formattedBalance = QString::number(balance);
    formattedBalance.insert(formattedBalance.length() - 2, ".");
    transactionList.insert("balance", formattedBalance);
    transactionList.insert("transactions", transactions);

    return transactionList;
}
