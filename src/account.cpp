#include "account.h"
#include "accountmanager.h"
#include <QDebug>

Account::Account(QObject *parent) : QObject(parent)
{

}

void Account::addChecking(QUrl filePath, QString accountName, int balance, QDate balanceDate)
{
    AccountManager accManager;
    Json::Value newAccount;
    newAccount["accountName"] = accountName.toStdString();

    Json::Value budget = accManager.loadFile(filePath);
    budget["onBudgetAccounts"].append(newAccount);
    accManager.saveFile(filePath, budget);

    addTransaction(filePath, (int)budget["onBudgetAccounts"].size() - 1, balanceDate, QString("Self"), false, balance, QString("Initial Balance"));
}

void Account::addTransaction(QUrl filePath, int accountIndex, QDate date, QString payee, bool outflow, int amount, QString note)
{
    AccountManager accManager;
    Json::Value budget = accManager.loadFile(filePath);
    QString formattedDate = date.toString("yyyy-MM-dd");

    Json::Value transaction;
    transaction["date"] = formattedDate.toStdString();
    transaction["payee"] = payee.toStdString();
    transaction["outflow"] = outflow;
    transaction["amount"] = amount;
    transaction["note"] = note.toStdString();

    int accBalance = budget["onBudgetAccounts"][accountIndex]["balance"].asInt();
    int balance = budget["balance"].asInt();

    if (outflow) {
        budget["onBudgetAccounts"][accountIndex]["balance"] = accBalance - amount;
        budget["balance"] = balance - amount;
    } else {
        budget["onBudgetAccounts"][accountIndex]["balance"] = accBalance + amount;
        budget["balance"] = balance + amount;
    }

    budget["onBudgetAccounts"][accountIndex]["transactions"].append(transaction);
    accManager.saveFile(filePath, budget);
}

Json::Value Account::getAccountList(QUrl filePath)
{
    AccountManager accManager;
    Json::Value budget = accManager.loadFile(filePath);
    Json::Value accountList;

    accountList["balance"] = budget["balance"];

    for (int i = 0; i < (int)budget["onBudgetAccounts"].size(); i++) {
        accountList["accounts"][i]["accountName"] = budget["onBudgetAccounts"][i]["accountName"];
        accountList["accounts"][i]["accountBalance"] = budget["onBudgetAccounts"][i]["balance"];
        accountList["accounts"][i]["index"] = i;
    }

    return accountList;
}

QString Account::getAccountListString(QUrl filePath)
{
    Json::Value accountList = getAccountList(filePath);
    QString accounts = accountList.toStyledString().c_str();
    return accounts;
}

Json::Value Account::getTransactions(QUrl filePath, int accountIndex)
{
    AccountManager accManager;
    Json::Value budget = accManager.loadFile(filePath);
    budget = budget["onBudgetAccounts"][accountIndex];
    Json::Value transactions;

    transactions["balance"] = budget["balance"];

    for (int i = 0; i < (int)budget["transactions"].size(); i++) {
        transactions["transactions"][i]["date"] = budget["transactions"][i]["date"];
        transactions["transactions"][i]["payee"] = budget["transactions"][i]["payee"];
        transactions["transactions"][i]["note"] = budget["transactions"][i]["note"];
        transactions["transactions"][i]["amount"] = budget["transactions"][i]["amount"];
        transactions["transactions"][i]["outflow"] = budget["transactions"][i]["outflow"];
    }

    return transactions;
}

QString Account::getTransactionsString(QUrl filePath, int accountIndex)
{
    Json::Value transactions = getTransactions(filePath, accountIndex);
    QString transactionString = transactions.toStyledString().c_str();
    return transactionString;
}
