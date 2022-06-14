#include "filesystemcontroller.h"

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;

    QString rootFilePath = app.applicationDirPath();
    FileSystemController fileSystemController;

    const QUrl url(u"qrc:/directory-scanner-cleaner/main.qml"_qs);
    QQmlContext *mainQmlContext = engine.rootContext();

    mainQmlContext->setContextProperty("FileSystemController", &fileSystemController);

    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
