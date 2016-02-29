#ifndef ACCOUNTMANAGER_H
#define ACCOUNTMANAGER_H

#include <QObject>
#include <QUrl>
#include <QJsonObject>

class AccountManager : public QObject
{
    Q_OBJECT
public:
    explicit AccountManager(QObject *parent = 0);
    Q_INVOKABLE void createBudget(QUrl filePath, QString accountName);
    Q_INVOKABLE QJsonObject loadFile(QUrl filePath);
    Q_INVOKABLE void saveFile(QUrl filePath, QJsonObject jsonData);
    Q_INVOKABLE void setLastFile(QUrl lastFilePath);
    Q_INVOKABLE QUrl getLastFile();

signals:

public slots:
};

#endif // ACCOUNTMANAGER_H
