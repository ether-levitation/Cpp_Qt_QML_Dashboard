#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "dashboard.h"

#include <QLocale>

#include <QCoreApplication>
#include <QDir>
#include <QFileInfo>
#include <QFileInfoList>

/* Current Folder
void test_current()
{
    qInfo() << QDir::currentPath();
}
*/

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    qmlRegisterSingletonType<Dashboard>("backend",1,0,"Dashboard",keyListenerInstance);

    QQmlApplicationEngine engine;
    const QUrl url(u"qrc:/main.qml"_qs);
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
