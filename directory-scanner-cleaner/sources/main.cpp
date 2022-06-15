#include "controllers/filesystemcontroller.h"
#include "models/filesystemmodel.h"

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;

    QString rootFilePath = app.applicationDirPath();

    FileSystemModel fileSystemModel(rootFilePath);
    FileSystemController fileSystemController(fileSystemModel);

    const QUrl url(u"qrc:/directory-scanner-cleaner/views/main.qml"_qs);
    QQmlContext *mainQmlContext = engine.rootContext();

    mainQmlContext->setContextProperty("FileSystemModel", &fileSystemModel);
    mainQmlContext->setContextProperty("FileSystemController", &fileSystemController);

    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
