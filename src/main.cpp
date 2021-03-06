#include <QApplication>
#include <QQmlApplicationEngine>
#include <QtQml>
#include "accountmanager.h"
#include "account.h"
#include <QDebug>
#include "budget.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QQmlApplicationEngine engine;
    qmlRegisterType<AccountManager>("com.nrauh", 1, 0, "AccountManager");
    qmlRegisterType<Account>("com.nrauh", 1, 0, "Account");
    qmlRegisterType<Budget>("com.nrauh", 1, 0, "Budget");

    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    return app.exec();
}
