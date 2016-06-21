#include <QJsonArray>
#include <QVector>
#include "account.h"
#include "accountmanager.h"
#include "sqlite/sqlite.hpp"
#include "budget.h"
#include "helpers.h"

Account::Account(QObject *parent) : QObject(parent)
{

}

void Account::addAccount(QUrl filePath, QString accountName, int balance, QDate balanceDate, bool onBudget)
{
    int accountId;

    io::sqlite::db budget(filePath.toLocalFile().toStdString());
    io::sqlite::stmt query(budget, "INSERT INTO accounts (accountName, balance, onBudget) VALUES (?, 0, ?)");

    query.bind().text(1, accountName.toStdString());
    query.bind().int32(2, onBudget);
    query.exec();

    query = io::sqlite::stmt(budget, "SELECT last_insert_rowid() FROM accounts");

    while (query.step()) {
        accountId = query.row().int32(0);
    }

    addTransaction(filePath, accountId, balanceDate, "Self", false, balance, "Income", "Initial Balance");
}

void Account::addTransaction(QUrl filePath, int accountId, QDate date, QString payee, bool outflow, int amount, QString category, QString note)
{
    Budget mbgt;
    std::string prepQuery;
    QString formattedDate = date.toString("yyyy-MM-dd");

    // add the transaction
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

    // lower account balance and possibly add to spent for a budget category
    // or increase account balance
    if (outflow) {
        prepQuery = "UPDATE accounts SET balance = balance - ? WHERE id == ?";

        if (isOnBudget(filePath, accountId)) {
            mbgt.addToSpent(filePath, category, date.toString("yyyy-MM"), amount);
        }
    } else {
        prepQuery = "UPDATE accounts SET balance = balance + ? WHERE id == ?";
    }

    query = io::sqlite::stmt(budget, prepQuery.c_str());

    query.bind().int32(1, amount);
    query.bind().int32(2, accountId);
    query.exec();
}

void Account::deleteTransaction(QUrl filePath, int transactionId)
{
    Budget mbgt;

    // getting transaction details
    io::sqlite::db budget(filePath.toLocalFile().toStdString());
    io::sqlite::stmt query(budget, "SELECT toAccount, amount,"
                                   " outflow, category, transactionDate"
                                   " FROM transactions WHERE id == ?");

    query.bind().int32(1, transactionId);
    query.exec();

    int account;
    int amount;
    bool outflow;
    QString category;
    QDate date;
    QString month;

    while (query.step()) {
        account = query.row().int32(0);
        amount = query.row().int32(1);
        outflow = query.row().int32(2);
        category = QString::fromStdString(query.row().text(3));
        date = QDate::fromString(query.row().text(4).c_str(), "yyyy-MM-dd");
        month = date.toString("yyyy-MM");
    }

    // negating transaction effect on balances and budget category
    if (outflow) {
        query = io::sqlite::stmt(budget, "UPDATE accounts SET balance = balance + ? WHERE id == ?");

        if (isOnBudget(filePath, account)) {
            mbgt.addToSpent(filePath, category, month, amount * -1);
        }
    } else {
        query = io::sqlite::stmt(budget, "UPDATE accounts SET balance = balance - ? WHERE id == ?");
    }

    query.bind().int32(1, amount);
    query.bind().int32(2, account);
    query.exec();

    // deleting the transaction
    query = io::sqlite::stmt(budget, "DELETE FROM transactions WHERE id == ?");
    query.bind().int32(1, transactionId);
    query.exec();
}

QJsonObject Account::getAccountList(QUrl filePath, int selection)
{
    QJsonObject accountList;
    QJsonArray accounts;
    std::string prepQuery;
    int totalBalance = 0;

    if (selection == 2) {
        prepQuery = "SELECT id, accountName, balance FROM accounts WHERE onBudget == 0 ORDER BY accountName";
    } else if (selection == 1) {
        prepQuery = "SELECT id, accountName, balance FROM accounts WHERE onBudget == 1 ORDER BY accountName";
    } else {
        prepQuery = "SELECT id, accountName, balance FROM accounts ORDER BY accountName";
    }

    io::sqlite::db budget(filePath.toLocalFile().toStdString());
    io::sqlite::stmt query(budget, prepQuery.c_str());

    while (query.step()) {
        QJsonObject account;
        QString accountName = QString::fromStdString(query.row().text(1));
        QString accountBalance = intToQs(query.row().int32(2));

        account.insert("accountId", query.row().int32(0));
        account.insert("accountName", accountName);
        account.insert("accountBalance", accountBalance);

        accounts.append(account);
        totalBalance += query.row().int32(2);
    }

    QString formattedTotalBalance = intToQs(totalBalance);

    accountList.insert("balance", formattedTotalBalance);
    accountList.insert("accounts", accounts);

    return accountList;
}

QJsonObject Account::getTransactions(QUrl filePath, int accountId)
{
    QJsonObject transactionList;
    QJsonArray transactions;
    int balance;

    // getting account balance
    io::sqlite::db budget(filePath.toLocalFile().toStdString());
    io::sqlite::stmt query(budget, "SELECT balance FROM accounts WHERE id == ?");

    query.bind().int32(1, accountId);
    query.exec();

    while (query.step()) {
        balance = query.row().int32(0);
    }

    // getting individual transactions
    query = io::sqlite::stmt(budget, "SELECT transactionDate, payee, amount,"
                                     "category, outflow, note, id"
                                     " FROM transactions WHERE toAccount == ?"
                                     " ORDER BY transactionDate DESC, id DESC");
    query.bind().int32(1, accountId);
    query.exec();

    while (query.step()) {
        QJsonObject transaction;
        bool outflow = query.row().int32(4);
        QString formattedAmount = intToQs(query.row().int32(2));

        if (outflow) {
            formattedAmount.prepend("-");
        } else {
            formattedAmount.prepend("+");
        }

        QString date = QString::fromStdString(query.row().text(0));
        QDate formattedDate = QDate::fromString(date, QString("yyyy-MM-dd"));

        transaction.insert("date", formattedDate.toString("M/d/yy"));
        transaction.insert("intDate", date);
        transaction.insert("payee", query.row().text(1).c_str());
        transaction.insert("amount", formattedAmount);
        transaction.insert("category", query.row().text(3).c_str());
        transaction.insert("outflow", outflow);
        transaction.insert("note", query.row().text(5).c_str());
        transaction.insert("id", query.row().int32(6));

        transactions.append(transaction);
    }

    // inserting the balance and transactions
    QString formattedBalance = intToQs(balance);
    transactionList.insert("balance", formattedBalance);
    transactionList.insert("transactions", transactions);

    return transactionList;
}

bool Account::isOnBudget(QUrl filePath, int accountId)
{
    bool onBudgetStatus;
    io::sqlite::db budget(filePath.toLocalFile().toStdString());
    io::sqlite::stmt query(budget, "SELECT onBudget FROM accounts WHERE id == ?");

    query.bind().int32(1, accountId);
    query.exec();

    while (query.step()) {
        onBudgetStatus = query.row().int32(0);
    }
    return onBudgetStatus;
}

void Account::changeOnBudget(QUrl filePath, int accountId, bool newStatus)
{
    io::sqlite::db budget(filePath.toLocalFile().toStdString());
    io::sqlite::stmt query(budget, "UPDATE accounts SET onBudget = ? WHERE id == ?");

    query.bind().int32(1, newStatus);
    query.bind().int32(2, accountId);
    query.exec();
}

void Account::deleteAccount(QUrl filePath, int accountId)
{
    QVector<int> transactions;
    io::sqlite::db mbgt(filePath.toLocalFile().toStdString());
    io::sqlite::stmt query(mbgt, "SELECT id FROM transactions WHERE toAccount == ?");

    query.bind().int32(1, accountId);
    query.exec();

    // for some reason just deleting the transaction in this loop resulted in a SQL error
    while (query.step()) {
        transactions.append(query.row().int32(0));
    }

    for (int i = 0; i < transactions.size(); i++) {
        deleteTransaction(filePath, transactions.at(i));
    }

    query = io::sqlite::stmt(mbgt, "DELETE FROM accounts WHERE id == ?");
    query.bind().int32(1, accountId);
    query.exec();
}
