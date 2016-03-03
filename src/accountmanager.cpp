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
    Json::Value budget;
    budget["accountName"] = accountName.toStdString();
    budget["balance"] = 0;

    QString budgetFilePath = filePath.toLocalFile() + "/" + accountName + ".mbgt";
    QUrl constructedFilePath = QUrl::fromLocalFile(budgetFilePath);
    saveFile(constructedFilePath, budget);
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
    Json::FastWriter budgetWriter;

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
