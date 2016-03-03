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
