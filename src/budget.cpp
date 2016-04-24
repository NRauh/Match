#include "budget.h"
#include "sqlite/sqlite.hpp"
#include <iostream>
#include <QDate>
#include <QDebug>
#include <QJsonObject>
#include <QJsonValue>

Budget::Budget(QObject *parent) : QObject(parent)
{

}

void Budget::addCategory(QUrl filePath, QString categoryName, int initialAmount)
{
    io::sqlite::db mbgt(filePath.toLocalFile().toStdString());
    io::sqlite::stmt query(mbgt, "INSERT INTO budgets (categoryName, monthOne, monthOneDate,"
                                 "monthTwo, monthTwoDate, monthThree, monthThreeDate,"
                                 "prevOne, prevOneDate, prevTwo, prevTwoDate)"
                                 "VALUES (?, ?, ?, 0, ?, 0, ?, 0, ?, 0, ?)");

    query.bind().text(1, categoryName.toStdString());
    query.bind().int32(2, initialAmount);

    QString currentMonth = QDate::currentDate().toString("yyyy-MM");
    QString monthPlusOne = QDate::currentDate().addMonths(1).toString("yyyy-MM");
    QString monthPlusTwo = QDate::currentDate().addMonths(2).toString("yyyy-MM");
    QString monthSubOne = QDate::currentDate().addMonths(-1).toString("yyyy-MM");
    QString monthSubTwo = QDate::currentDate().addMonths(-2).toString("yyyy-MM");

    query.bind().text(3, currentMonth.toStdString());
    query.bind().text(4, monthPlusOne.toStdString());
    query.bind().text(5, monthPlusTwo.toStdString());
    query.bind().text(6, monthSubOne.toStdString());
    query.bind().text(7, monthSubTwo.toStdString());

    query.exec();
}

QJsonArray Budget::getCategories(QUrl filePath, int month)
{
    QJsonArray categoryArray;
    QDate selectedMonth = QDate::currentDate();
    std::string sqlQuery;

    if (month < 3 && month > -3) {
        selectedMonth = selectedMonth.addMonths(month);
    } else {
        QJsonObject err;
        err.insert("err", "Out of stored range");
        categoryArray.append(err);
        return categoryArray;
    }

    switch (month) {
    case -2:
        sqlQuery = "prevTwo";
        break;
    case -1:
        sqlQuery = "prevOne";
        break;
    case 0:
        sqlQuery = "monthOne";
        break;
    case 1:
        sqlQuery = "monthTwo";
        break;
    case 2:
        sqlQuery = "monthThree";
        break;
    default:
        break;
    }

    sqlQuery = "SELECT categoryName, " + sqlQuery + " FROM budgets WHERE " + sqlQuery + "Date == ?";
    io::sqlite::db mbgt(filePath.toLocalFile().toStdString());
    io::sqlite::stmt query(mbgt, sqlQuery.c_str());
    query.bind().text(1, selectedMonth.toString("yyyy-MM").toStdString());

    while (query.step()) {
        QJsonObject category;
        QString categoryName = QString::fromStdString(query.row().text(0));
        QString amount = QString::number(query.row().int32(1));

        if (amount.length() == 1) {
            amount.prepend("00");
        } else if (amount.length() == 2) {
            amount.prepend("0");
        }

        amount = amount.insert(amount.length() - 2, ".");

        category.insert("categoryName", categoryName);
        category.insert("amount", amount);

        categoryArray.append(category);
    }

    return categoryArray;
}
