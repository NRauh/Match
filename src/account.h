#ifndef ACCOUNT_H
#define ACCOUNT_H

#include <QObject>
#include <QUrl>
#include <QDate>

class Account : public QObject
{
    Q_OBJECT
public:
    explicit Account(QObject *parent = 0);
    Q_INVOKABLE void addChecking(QUrl filePath, QString accountName, int balance, QDate balanceDate);

signals:

public slots:
};

#endif // ACCOUNT_H
