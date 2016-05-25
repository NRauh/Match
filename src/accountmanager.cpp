#include "accountmanager.h"
#include <QFile>
#include <QSettings>
#include <QDebug>
#include "sqlite/sqlite.hpp"

AccountManager::AccountManager(QObject *parent) : QObject(parent)
{

}

void AccountManager::createBudget(QUrl filePath, QString accountName)
{
    QString budgetFilePath = filePath.toLocalFile() + "/" + accountName + ".mbgt";
    io::sqlite::db budget(budgetFilePath.toStdString());
    budget.exec("CREATE TABLE IF NOT EXISTS accounts(id integer primary key, accountName text, balance integer, onBudget integer)");
    budget.exec("CREATE TABLE IF NOT EXISTS transactions(id integer primary key, toAccount integer,"
                "transactionDate text, payee text, amount integer, outflow integer, category text, note text)");
    budget.exec("CREATE TABLE IF NOT EXISTS budgets(id integer primary key, categoryName text,"
                "monthOne integer, monthOneSpent integer, monthOneDate text,"
                "monthTwo integer, monthTwoSpent integer, monthTwoDate text,"
                "monthThree integer, monthThreeSpent integer, monthThreeDate text,"
                "prevOne integer, prevOneSpent integer, prevOneDate text,"
                "prevTwo integer, prevTwoSpent integer, prevTwoDate text)");
}

void AccountManager::setLastFile(QUrl lastFilePath)
{
    QSettings settings("nrauh", "Match");
    settings.setValue("lastOpenedFile", lastFilePath);
}

QUrl AccountManager::getLastFile()
{
    QSettings settings("nrauh", "Match");
    return settings.value("lastOpenedFile").value<QUrl>();
}

void AccountManager::shiftOneMonth(QUrl filePath, QDate month)
{
    io::sqlite::db mbgt(filePath.toLocalFile().toStdString());

    io::sqlite::stmt query(mbgt, "UPDATE budgets SET "
                                   "prevTwo = prevOne,"
                                   "prevTwoSpent = prevOneSpent,"
                                   "prevTwoDate = prevOneDate,"
                                   "prevOne = monthOne,"
                                   "prevOneSpent = monthOneSpent,"
                                   "prevOneDate = monthOneDate,"
                                   "monthOne = monthTwo,"
                                   "monthOneSpent = monthTwoSpent,"
                                   "monthOneDate = monthTwoDate,"
                                   "monthTwo = monthThree,"
                                   "monthTwoSpent = monthThreeSpent,"
                                   "monthTwoDate = monthThreeDate,"
                                   "monthThreeSpent = 0,"
                                   "monthThreeDate = ?");

    std::string monthString = month.addMonths(3).toString("yyyy-MM").toStdString();

    query.bind().text(1, monthString);
    query.exec();
}

void AccountManager::shiftMany(QUrl filePath, QDate month)
{
    io::sqlite::db mbgt(filePath.toLocalFile().toStdString());
    io::sqlite::stmt query(mbgt, "UPDATE budgets SET "
                                 "prevTwo = monthThree,"
                                 "prevTwoSpent = 0,"
                                 "prevTwoDate = ?,"
                                 "prevOne = monthThree,"
                                 "prevOneSpent = 0,"
                                 "prevOneDate = ?,"
                                 "monthOne = monthThree,"
                                 "monthOneSpent = 0,"
                                 "monthOneDate = ?,"
                                 "monthTwo = monthThree,"
                                 "monthTwoSpent = 0,"
                                 "monthTwoDate = ?,"
                                 "monthThreeSpent = 0,"
                                 "monthThreeDate = ?");

    query.bind().text(1, month.addMonths(-2).toString("yyyy-MM").toStdString());
    query.bind().text(2, month.addMonths(-1).toString("yyyy-MM").toStdString());
    query.bind().text(3, month.toString("yyyy-MM").toStdString());
    query.bind().text(4, month.addMonths(1).toString("yyyy-MM").toStdString());
    query.bind().text(5, month.addMonths(2).toString("yyyy-MM").toStdString());
    query.exec();
}

void AccountManager::shiftBudget(QUrl filePath, QDate date, int step)
{
    io::sqlite::db mbgt(filePath.toLocalFile().toStdString());
    io::sqlite::stmt query(mbgt, "SELECT monthOneDate FROM budgets WHERE id == 1");
    std::string lastCurrentMonth;

    while (query.step()) {
        lastCurrentMonth = query.row().text(0);
    }

    if (lastCurrentMonth != date.toString("yyyy-MM").toStdString() && step < 4) {
        shiftOneMonth(filePath, QDate::fromString(lastCurrentMonth.c_str(), "yyyy-MM"));
        shiftBudget(filePath, date, step + 1);
    } else if (step >= 4) {
        shiftMany(filePath, date);
    }
}
