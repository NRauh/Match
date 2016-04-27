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
    io::sqlite::stmt query(mbgt, "INSERT INTO budgets (categoryName,"
                                 "monthOne, monthOneSpent, monthOneDate,"
                                 "monthTwo, monthTwoSpent, monthTwoDate,"
                                 "monthThree, monthThreeSpent, monthThreeDate,"
                                 "prevOne, prevOneSpent, prevOneDate,"
                                 "prevTwo, prevTwoSpent, prevTwoDate)"
                                 "VALUES (?, ?, 0, ?, 0, 0, ?, 0, 0, ?, 0, 0, ?, 0, 0, ?)");

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
    std::string selectedMonthSpent;

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
        selectedMonthSpent = "prevTwoSpent";
        break;
    case -1:
        sqlQuery = "prevOne";
        selectedMonthSpent = "prevOneSpent";
        break;
    case 0:
        sqlQuery = "monthOne";
        selectedMonthSpent = "monthOneSpent";
        break;
    case 1:
        sqlQuery = "monthTwo";
        selectedMonthSpent = "monthTwoSpent";
        break;
    case 2:
        sqlQuery = "monthThree";
        selectedMonthSpent = "monthThreeSpent";
        break;
    default:
        break;
    }

    sqlQuery = "SELECT categoryName," +
               sqlQuery + ", " + selectedMonthSpent +
               " FROM budgets WHERE " + sqlQuery + "Date == ?";

    io::sqlite::db mbgt(filePath.toLocalFile().toStdString());
    io::sqlite::stmt query(mbgt, sqlQuery.c_str());
    query.bind().text(1, selectedMonth.toString("yyyy-MM").toStdString());

    while (query.step()) {
        int initialAmount = query.row().int32(1);
        int remainingAmount = query.row().int32(2);
        QJsonObject category;
        QString categoryName = QString::fromStdString(query.row().text(0));
        QString amount = QString::number(initialAmount);
        QString remaining = QString::number(initialAmount - remainingAmount);

        if (amount.length() == 1) {
            amount.prepend("00");
        } else if (amount.length() == 2) {
            amount.prepend("0");
        }

        if (remaining.length() == 1) {
            remaining.prepend("00");
        } else if (remaining.length() == 2) {
            remaining.prepend("0");
        }

        amount = amount.insert(amount.length() - 2, ".");
        remaining = remaining.insert(remaining.length() - 2, ".");

        category.insert("categoryName", categoryName);
        category.insert("amount", amount);
        category.insert("remaining", remaining);

        categoryArray.append(category);
    }

    return categoryArray;
}

QList<QString> Budget::getCategoryNames(QUrl filePath)
{
    QList<QString> categoryList;
    io::sqlite::db mbgt(filePath.toLocalFile().toStdString());
    io::sqlite::stmt query(mbgt, "SELECT categoryName FROM budgets");

    while (query.step()) {
        categoryList << query.row().text(0).c_str();
    }

    categoryList.sort();

    return categoryList;
}

bool Budget::addToSpent(QUrl filePath, QString category, QString month, int amount)
{
    QDate currentMonth = QDate::currentDate();
    std::string toUpdate;

    if (month == currentMonth.toString("yyyy-MM")) {
        toUpdate = "monthOneSpent";
    } else if (month == currentMonth.addMonths(-2).toString("yyyy-MM")) {
        toUpdate = "prevTwoSpent";
    } else if (month == currentMonth.addMonths(-1).toString("yyyy-MM")) {
        toUpdate = "prevOneSpent";
    } else if (month == currentMonth.addMonths(1).toString("yyyy-MM")) {
        toUpdate = "monthTwoSpent";
    } else if (month == currentMonth.addMonths(2).toString("yyyy-MM")) {
        toUpdate = "monthThreeSpent";
    } else {
        return false;
    }

    io::sqlite::db mbgt(filePath.toLocalFile().toStdString());
    std::string formattedQuery;
    formattedQuery = "UPDATE budgets SET " + toUpdate + " = " + toUpdate + " + ? WHERE categoryName == ?";
    io::sqlite::stmt query(mbgt, formattedQuery.c_str());

    query.bind().int32(1, amount);
    query.bind().text(2, category.toStdString());
    query.exec();

    return true;
}

void Budget::updateBudget(QUrl filePath, int month, QString category, int amount)
{
    std::string selectedMonth;

    switch (month) {
    case -2:
        selectedMonth = "prevTwo";
        break;
    case -1:
        selectedMonth = "prevOne";
        break;
    case 0:
        selectedMonth = "monthOne";
        break;
    case 1:
        selectedMonth = "monthTwo";
        break;
    case 2:
        selectedMonth = "monthThree";
        break;
    default:
        break;
    }

    std::string prepQuery;
    prepQuery = "UPDATE budgets SET " + selectedMonth + " = ? WHERE categoryName == ?";

    io::sqlite::db mbgt(filePath.toLocalFile().toStdString());
    io::sqlite::stmt query(mbgt, prepQuery.c_str());

    query.bind().int32(1, amount);
    query.bind().text(2, category.toStdString());
    query.exec();
}
