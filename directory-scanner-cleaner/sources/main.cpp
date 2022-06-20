#include "controllers/filesystemcontroller.h"
#include "models/filesystemmodel.h"

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

QQmlApplicationEngine *gEngine;

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    gEngine = new QQmlApplicationEngine();

    QString rootFilePath = app.applicationDirPath();

    QItemSelectionModel testItemSelectionModel;
    FileSystemModel fileSystemModel(&testItemSelectionModel, "D:/GL");
    FileSystemController fileSystemController(fileSystemModel);

    const QUrl url(u"qrc:/directory-scanner-cleaner/views/main.qml"_qs);
    QQmlContext *mainQmlContext = gEngine->rootContext();

    mainQmlContext->setContextProperty("FileSystemModel", &fileSystemModel);
    mainQmlContext->setContextProperty("FileSystemController", &fileSystemController);
    mainQmlContext->setContextProperty("TestItemSelectionModel", &testItemSelectionModel);

    QObject::connect(gEngine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    gEngine->load(url);

    return app.exec();
}
