#include "budget.h"
#include "sqlite/sqlite.hpp"
#include <iostream>
#include <QDate>
#include <QDebug>
#include <QJsonValue>
#include "helpers.h"

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
    std::string sqlQuery = monthFromIndex(month);
    std::string selectedMonthSpent = sqlQuery + "Spent";

    if (month < 3 && month > -3) {
        selectedMonth = selectedMonth.addMonths(month);
    } else {
        QJsonObject err;
        err.insert("err", "Out of stored range");
        categoryArray.append(err);
        return categoryArray;
    }

    sqlQuery = "SELECT categoryName," +
               sqlQuery + ", " + selectedMonthSpent +
               " FROM budgets WHERE " + sqlQuery + "Date == ? ORDER BY categoryName";

    io::sqlite::db mbgt(filePath.toLocalFile().toStdString());
    io::sqlite::stmt query(mbgt, sqlQuery.c_str());
    query.bind().text(1, selectedMonth.toString("yyyy-MM").toStdString());

    while (query.step()) {
        int amount = query.row().int32(1);
        int spent = query.row().int32(2);
        QJsonObject category;
        QString categoryName = QString::fromStdString(query.row().text(0));

        QString formattedAmount = intToQs(amount);
        QString formattedRemainingAmount = intToQs(amount - spent);

        category.insert("categoryName", categoryName);
        category.insert("amount", formattedAmount);
        category.insert("remaining", formattedRemainingAmount);

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

    categoryList << "Income" << "Uncategorized";

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
    std::string selectedMonth = monthFromIndex(month);

    std::string prepQuery;
    prepQuery = "UPDATE budgets SET " + selectedMonth + " = ? WHERE categoryName == ?";

    io::sqlite::db mbgt(filePath.toLocalFile().toStdString());
    io::sqlite::stmt query(mbgt, prepQuery.c_str());

    query.bind().int32(1, amount);
    query.bind().text(2, category.toStdString());
    query.exec();
}

QJsonObject Budget::getMeta(QUrl filePath, int month)
{
    QJsonObject meta;

    QDate monthLongform = QDate::currentDate().addMonths(month);
    meta.insert("month", monthLongform.toString("MMMM, yyyy"));
    meta.insert("monthInt", monthLongform.toString("yyyy-MM"));

    int amount = 0;
    int spentAmount = 0;
    std::string selectedMonth = monthFromIndex(month);

    std::string prepQuery;
    prepQuery = "SELECT " + selectedMonth + "Spent, " + selectedMonth + " FROM budgets";

    io::sqlite::db mbgt(filePath.toLocalFile().toStdString());
    io::sqlite::stmt query(mbgt, prepQuery.c_str());

    while (query.step()) {
        spentAmount += query.row().int32(0);
        amount += query.row().int32(1);
    }

    int remainingAmount = amount - spentAmount;
    QString formattedRemainingAmount = intToQs(remainingAmount);

    meta.insert("remaining", formattedRemainingAmount);

    return meta;
}

QString Budget::getAvailableMoney(QUrl filePath)
{
    int totalBalance = 0;
    int totalBudgets = 0;

    io::sqlite::db mbgt(filePath.toLocalFile().toStdString());
    io::sqlite::stmt query(mbgt, "SELECT balance FROM accounts WHERE onBudget == 1");

    while (query.step()) {
        totalBalance += query.row().int32(0);
    }

    query = io::sqlite::stmt(mbgt, "SELECT monthOne, monthTwo, monthThree FROM budgets");

    while (query.step()) {
        int budgetUpcoming = query.row().int32(0) + query.row().int32(1) + query.row().int32(2);
        totalBudgets += budgetUpcoming;
    }

    int available = totalBalance - totalBudgets;
    QString formattedAvailable = intToQs(available);

    return formattedAvailable;
}

QJsonArray Budget::getCategoryTransactions(QUrl filePath, QString category, QString date)
{
    QJsonArray transactions;
    io::sqlite::db mbgt(filePath.toLocalFile().toStdString());
    io::sqlite::stmt query(mbgt, "SELECT transactionDate, payee, note, amount, accounts.accountName"
                                 " FROM transactions JOIN accounts ON"
                                 " accounts.id == transactions.toAccount"
                                 " WHERE category == ?"
                                 " AND transactionDate LIKE ? || '%'"
                                 " AND accounts.onBudget == 1");
    // the LIKE bit isn't tested

    query.bind().text(1, category.toStdString());
    query.bind().text(2, date.toStdString());
    query.exec();

    while (query.step()) {
        QJsonObject transaction;
        QDate formattedDate = QDate::fromString(query.row().text(0).c_str(), "yyyy-MM-dd");
        QString toAccount;

        int amount = query.row().int32(3);
        QString formattedAmount = intToQs(amount);

        transaction.insert("date", formattedDate.toString("M/d/yy"));
        transaction.insert("payee", query.row().text(1).c_str());
        transaction.insert("note", query.row().text(2).c_str());
        transaction.insert("amount", formattedAmount);
        transaction.insert("account", toAccount);
        transaction.insert("account", query.row().text(4).c_str());
        transactions.append(transaction);
    }

    return transactions;
}
