#include "accountmanager.h"
#include <QFile>
#include <QSettings>
#include <QDebug>
#include "sqlite/sqlite.hpp"

AccountManager::AccountManager(QObject *parent) : QObject(parent)
{

}

void AccountManager::createBudget(QUrl filePath, QString accountName)
{
    QString budgetFilePath = filePath.toLocalFile() + "/" + accountName + ".mbgt";
    io::sqlite::db budget(budgetFilePath.toStdString());
    budget.exec("CREATE TABLE IF NOT EXISTS accounts(id integer primary key, accountName text, balance integer)");
    budget.exec("CREATE TABLE IF NOT EXISTS transactions(id integer primary key, toAccount integer,"
                "transactionDate text, payee text, amount integer, outflow integer, note text)");
    budget.exec("CREATE TABLE IF NOT EXISTS budgets(id integer primary key, categoryName text,"
                "monthOne integer, monthOneRemaining integer, monthOneDate text,"
                "monthTwo integer, monthTwoRemaining integer, monthTwoDate text,"
                "monthThree integer, monthThreeRemaining integer, monthThreeDate text,"
                "prevOne integer, prevOneRemaining integer, prevOneDate text,"
                "prevTwo integer, prevTwoRemaining integer, prevTwoDate text)");
}

void AccountManager::setLastFile(QUrl lastFilePath)
{
    QSettings settings("nrauh", "Match");
    settings.setValue("lastOpenedFile", lastFilePath);
}

QUrl AccountManager::getLastFile()
{
    QSettings settings("nrauh", "Match");
    return settings.value("lastOpenedFile").value<QUrl>();
}
