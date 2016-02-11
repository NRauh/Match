#ifndef ACCOUNTMANAGER_H
#define ACCOUNTMANAGER_H

#include <QObject>
#include <QUrl>

class AccountManager : public QObject
{
    Q_OBJECT
public:
    explicit AccountManager(QObject *parent = 0);
    Q_INVOKABLE void createBudget(QUrl filePath, QString accountName);

signals:

public slots:
};

#endif // ACCOUNTMANAGER_H
