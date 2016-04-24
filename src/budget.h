#ifndef BUDGET_H
#define BUDGET_H

#include <QObject>
#include <QUrl>
#include <QJsonArray>

class Budget : public QObject
{
    Q_OBJECT
public:
    explicit Budget(QObject *parent = 0);
    Q_INVOKABLE void addCategory(QUrl filePath, QString categoryName, int initialAmount);
    Q_INVOKABLE QJsonArray getCategories(QUrl filePath, int month);

signals:

public slots:
};

#endif // BUDGET_H
