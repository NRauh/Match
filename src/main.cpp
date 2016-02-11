#include <QApplication>
#include <QQmlApplicationEngine>
#include <QtQml>
#include "accountmanager.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QQmlApplicationEngine engine;
    qmlRegisterType<AccountManager>("com.nrauh", 1, 0, "AccountManager");
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    return app.exec();
}
