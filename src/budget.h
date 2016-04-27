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
    Q_INVOKABLE QList<QString> getCategoryNames(QUrl filePath);
    Q_INVOKABLE bool addToSpent(QUrl filePath, QString category, QString month, int amount);
    Q_INVOKABLE void updateBudget(QUrl filePath, int month, QString category, int amount);

signals:

public slots:
};

#endif // BUDGET_H
