#ifndef ACCOUNT_H
#define ACCOUNT_H

#include <QObject>
#include <QUrl>
#include <QDate>
#include "json/json.h"

class Account : public QObject
{
    Q_OBJECT
public:
    explicit Account(QObject *parent = 0);
    Q_INVOKABLE void addChecking(QUrl filePath, QString accountName, int balance, QDate balanceDate);
    Q_INVOKABLE void addTransaction(QUrl filePath, int accountIndex, QDate date, QString payee, bool outflow, int amount, QString note);
    Q_INVOKABLE Json::Value getAccountList(QUrl filePath);
    Q_INVOKABLE QString getAccountListString(QUrl filePath);

signals:

public slots:
};

#endif // ACCOUNT_H
