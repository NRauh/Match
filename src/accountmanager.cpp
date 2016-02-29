#include "accountmanager.h"
#include <QFile>
#include <QJsonObject>
#include <QJsonDocument>
#include <QSettings>
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
    QUrl constructedFilePath = QUrl::fromLocalFile(budgetFilePath);
    saveFile(constructedFilePath, budget);
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

void AccountManager::saveFile(QUrl filePath, QJsonObject jsonData)
{
    QFile budgetFile(filePath.toLocalFile());
    QJsonDocument jsonDoc(jsonData);

    budgetFile.open(QIODevice::WriteOnly);
    budgetFile.write(jsonDoc.toJson());
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
