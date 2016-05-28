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
    Q_INVOKABLE void addAccount(QUrl filePath, QString accountName, int balance, QDate balanceDate, bool onBudget);
    Q_INVOKABLE void addTransaction(QUrl filePath, int accountId, QDate date, QString payee, bool outflow, int amount, QString category, QString note);
    Q_INVOKABLE void deleteTransaction(QUrl filePath, int transactionId);
    Q_INVOKABLE QJsonObject getAccountList(QUrl filePath, int selection = 0);
    Q_INVOKABLE QJsonObject getTransactions(QUrl filePath, int accountId);
    Q_INVOKABLE bool isOnBudget(QUrl filePath, int accountId);
    Q_INVOKABLE void changeOnBudget(QUrl filePath, int accountId, bool newStatus);
    Q_INVOKABLE void deleteAccount(QUrl filePath, int accountId);
    /*
    Q_INVOKABLE QString getAccountListString(QUrl filePath);
    Q_INVOKABLE QString getTransactionsString(QUrl filePath, QString accountId);
    */

signals:

public slots:
};

#endif // ACCOUNT_H
