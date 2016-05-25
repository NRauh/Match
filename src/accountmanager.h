#ifndef ACCOUNTMANAGER_H
#define ACCOUNTMANAGER_H

#include <QObject>
#include <QUrl>
#include "json/json.h"
#include <QDate>

class AccountManager : public QObject
{
    Q_OBJECT
public:
    explicit AccountManager(QObject *parent = 0);
    Q_INVOKABLE void createBudget(QUrl filePath, QString accountName);
    Q_INVOKABLE void setLastFile(QUrl lastFilePath);
    Q_INVOKABLE QUrl getLastFile();
    void shiftOneMonth(QUrl filePath);

signals:

public slots:
};

#endif // ACCOUNTMANAGER_H
