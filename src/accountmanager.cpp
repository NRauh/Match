#include "accountmanager.h"
#include <QFile>
#include <QSettings>
#include <QDebug>
#include <QSqlDatabase>
#include <QSqlQuery>

AccountManager::AccountManager(QObject *parent) : QObject(parent)
{

}

void AccountManager::createBudget(QUrl filePath, QString accountName)
{
    QString budgetFilePath = filePath.toLocalFile() + "/" + accountName + ".mbgt";
    QSqlDatabase budget = QSqlDatabase::addDatabase("QSQLITE");

    budget.setDatabaseName(budgetFilePath);
    budget.open();
    budget.exec("CREATE TABLE accounts(id integer primary key, accountName text, balance integer)");
    budget.exec("CREATE TABLE transactions(id integer primary key, toAccount integer,"
                "trasactionDate text, payee text, amount integer, outflow bool, note text)");
    budget.close();
}

Json::Value AccountManager::loadFile(QUrl filePath)
{
    QFile budgetFile(filePath.toLocalFile());

    budgetFile.open(QIODevice::ReadOnly);
    QString rawJson = budgetFile.readAll();
    Json::Value budget = rawJson.toStdString();
    Json::Reader budgetReader;

    budgetReader.parse(rawJson.toStdString(), budget);

    budgetFile.close();
    return budget;
}

void AccountManager::saveFile(QUrl filePath, Json::Value jsonData)
{
    QFile budgetFile(filePath.toLocalFile());
    Json::StyledWriter budgetWriter;

    budgetFile.open(QIODevice::WriteOnly);
    budgetFile.write(budgetWriter.write(jsonData).c_str());
    budgetFile.close();
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
