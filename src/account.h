#ifndef ACCOUNT_H
#define ACCOUNT_H

#include <QObject>
#include <QUrl>
#include <QDate>
#include "json/json.h"
#include <QJsonObject>

class Account : public QObject
{
    Q_OBJECT
public:
    explicit Account(QObject *parent = 0);
    Q_INVOKABLE void addChecking(QUrl filePath, QString accountName, int balance, QDate balanceDate);
    Q_INVOKABLE void addTransaction(QUrl filePath, int accountId, QDate date, QString payee, bool outflow, int amount, QString note);
    Q_INVOKABLE void deleteTransaction(QUrl filePath, int transactionId);
    Q_INVOKABLE QJsonObject getAccountList(QUrl filePath);
    Q_INVOKABLE QJsonObject getTransactions(QUrl filePath, int accountId);
    /*
    Q_INVOKABLE QString getAccountListString(QUrl filePath);
    Q_INVOKABLE QString getTransactionsString(QUrl filePath, QString accountId);
    */

signals:

public slots:
};

#endif // ACCOUNT_H
