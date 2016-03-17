#include "account.h"
#include "accountmanager.h"
#include <regex>
#include <QUuid>

Account::Account(QObject *parent) : QObject(parent)
{

}

void Account::addChecking(QUrl filePath, QString accountName, int balance, QDate balanceDate)
{
    AccountManager accManager;
    Json::Value newAccount;
    QUuid accountId = QUuid::createUuid();
    newAccount["accountId"] = accountId.toString().toStdString();
    newAccount["accountName"] = accountName.toStdString();

    Json::Value budget = accManager.loadFile(filePath);
    budget["onBudgetAccounts"].append(newAccount);
    accManager.saveFile(filePath, budget);

    addTransaction(filePath, accountId.toString(), balanceDate, QString("Self"), false, balance, QString("Initial Balance"));
}

void Account::addTransaction(QUrl filePath, QString accountId, QDate date, QString payee, bool outflow, int amount, QString note)
{
    AccountManager accManager;
    Json::Value budget = accManager.loadFile(filePath);
    QString formattedDate = date.toString("yyyy-MM-dd");
    QUuid transactionId = QUuid::createUuid();
    int accountIndex;

    Json::Value transaction;
    transaction["id"] = transactionId.toString().toStdString();
    transaction["date"] = formattedDate.toStdString();
    transaction["payee"] = payee.toStdString();
    transaction["outflow"] = outflow;
    transaction["amount"] = amount;
    transaction["note"] = note.toStdString();

    for (int i = 0; i < (int)budget["onBudgetAccounts"].size(); i++) {
        if (budget["onBudgetAccounts"][i]["accountId"].asString() == accountId.toStdString()) {
            accountIndex = i;
            break;
        }
    }

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
    std::string totalBalance = budget["balance"].asString();
    totalBalance = totalBalance.insert(totalBalance.length() - 2, ".");

    accountList["balance"] = totalBalance;

    for (int i = 0; i < (int)budget["onBudgetAccounts"].size(); i++) {
        std::string accountBalance = budget["onBudgetAccounts"][i]["balance"].asString();
        accountBalance = accountBalance.insert(accountBalance.length() - 2, ".");

        accountList["accounts"][i]["accountName"] = budget["onBudgetAccounts"][i]["accountName"];
        accountList["accounts"][i]["accountBalance"] = accountBalance;
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

QString Account::getTransactionsString(QUrl filePath, int accountIndex)
{
    Json::Value transactions = getTransactions(filePath, accountIndex);
    QString transactionString = transactions.toStyledString().c_str();
    return transactionString;
}
