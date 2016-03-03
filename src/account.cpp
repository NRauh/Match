#include "account.h"
#include "json/json.h"
#include "accountmanager.h"

Account::Account(QObject *parent) : QObject(parent)
{

}

void Account::addChecking(QUrl filePath, QString accountName, int balance, QDate balanceDate)
{
    AccountManager accManager;
    Json::Value newAccount;
    QString formattedDate = balanceDate.toString("yyyy-MM-dd");
    newAccount["accountName"] = accountName.toStdString();
    newAccount["balance"] = balance;
    newAccount["transactions"][0]["transactionDate"] = formattedDate.toStdString();
    newAccount["transactions"][0]["payee"] = Json::nullValue;
    newAccount["transactions"][0]["note"] = "Initial Balance";
    newAccount["transactions"][0]["outflow"] = false;

    Json::Value budget = accManager.loadFile(filePath);
    budget["onBudgetAccounts"].append(newAccount);
    budget["balance"] = budget["balance"].asInt() + balance;
    accManager.saveFile(filePath, budget);
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
