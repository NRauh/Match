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
    qDebug() << budgetFilePath;
    QFile budgetFile(budgetFilePath);
    QJsonDocument jsonDoc(budget);
    budgetFile.open(QIODevice::WriteOnly);
    budgetFile.write(jsonDoc.toJson());
    budgetFile.close();
}
