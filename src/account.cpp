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
    newAccount["accountName"] = accountName.toStdString();
    newAccount["balance"] = balance;

    Json::Value budget = accManager.loadFile(filePath);
    budget["onBudgetAccounts"].append(newAccount);
    budget["balance"] = budget["balance"].asInt() + balance;
    accManager.saveFile(filePath, budget);
}
