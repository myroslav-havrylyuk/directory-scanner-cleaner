#include "controllers/filesystemcontroller.h"
#include "models/filesystemmodel.h"

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QThread>
#include <QQuickStyle>

QQmlApplicationEngine *gEngine;

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    qDebug() << "Main thread: " << QThread::currentThread();
    QQuickStyle::setStyle("Material");

    gEngine = new QQmlApplicationEngine();

    QString rootFilePath = app.applicationDirPath();

    FileSystemModel fileSystemModel(rootFilePath);
    FileSystemController fileSystemController(fileSystemModel);

    const QUrl url(u"qrc:/directory-scanner-cleaner/views/main.qml"_qs);
    QQmlContext *mainQmlContext = gEngine->rootContext();

    mainQmlContext->setContextProperty("FileSystemModel", &fileSystemModel);
    mainQmlContext->setContextProperty("FileSystemController", &fileSystemController);

    QObject::connect(gEngine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    gEngine->load(url);

    return app.exec();
}
