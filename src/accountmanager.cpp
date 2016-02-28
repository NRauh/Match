#include "accountmanager.h"
#include <QFile>
#include <QJsonObject>
#include <QJsonDocument>
#include <QDebug>

AccountManager::AccountManager(QObject *parent) : QObject(parent)
{

}

void AccountManager::createBudget(QUrl filePath, QString accountName)
{
    QJsonObject budget;
    budget["accountName"] = accountName;
    budget["balance"] = 0;

    QString budgetFilePath = filePath.toLocalFile() + "/" + accountName + ".mbgt";
    QFile budgetFile(budgetFilePath);
    QJsonDocument jsonDoc(budget);
    budgetFile.open(QIODevice::WriteOnly);
    budgetFile.write(jsonDoc.toJson());
    budgetFile.close();
}

QJsonObject AccountManager::loadFile(QUrl filePath)
{
    QFile budgetFile(filePath.toLocalFile());

    budgetFile.open(QIODevice::ReadOnly);
    QString rawJson = budgetFile.readAll();
    QJsonDocument budgetDoc = QJsonDocument::fromJson(rawJson.toUtf8());
    QJsonObject budget = budgetDoc.object();

    budgetFile.close();
    return budget;
}
