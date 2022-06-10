#include "filemanagermodel.h"
#include "filemanagercontroller.h"

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QDir>

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;

    QString rootFilePath = app.applicationDirPath();
    FileManagerModel fileManagerModel("D:/Eleks_practise/directory-scanner-cleaner");

    const QUrl url(u"qrc:/demo_app/main.qml"_qs);
    QQmlContext *mainQmlContext = engine.rootContext();
    mainQmlContext->setContextProperty("FileManagerModel", &fileManagerModel);
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    FileManagerController::registerMe();

    return app.exec();
}
