#include "controllers/filesystemcontroller.h"
#include "models/filesystemmodel.h"
#include "controllers/settingscontroller.h"
#include "tools/configfilehandler.h"

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QThread>
#include <QQuickStyle>
#include <QQuickView>

QQmlApplicationEngine *gEngine;
QObject *mainWindow;

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    qDebug() << "Main thread: " << QThread::currentThread();
    QQuickStyle::setStyle("Material");

    gEngine = new QQmlApplicationEngine();

    QString rootFilePath = app.applicationDirPath();

    FileSystemModel fileSystemModel;
    //FileSystemModel fileSystemModel(rootFilePath);
    FileSystemController fileSystemController(fileSystemModel);
    ConfigFileHandler handler;
    SettingsController settingsController(handler);
    fileSystemController.setActivePath("C:\\My\\Study\\My_3_course\\2_term\\ZPKG");

    const QUrl url(u"qrc:/directory-scanner-cleaner/views/main.qml"_qs);
    QQmlContext *mainQmlContext = gEngine->rootContext();

    mainQmlContext->setContextProperty("FileSystemModel", &fileSystemModel);
    mainQmlContext->setContextProperty("FileSystemController", &fileSystemController);
    mainQmlContext->setContextProperty("SettingsController", &settingsController);

    /*QObject::connect(gEngine, &QQmlApplicationEngine::objectCreated,
                     &app, [url, &mainWindow](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
        mainWindow = obj;
    }, Qt::QueuedConnection);*/
            QQmlComponent mainWindowComponent(gEngine, url);

            mainWindow = mainWindowComponent.create();

            QObject *progressDialog = mainWindow->findChild<QObject*>("progress_dialog");
            QObject::connect(progressDialog, SIGNAL(cancelSetupModel()),
                             &fileSystemController, SLOT(cancelSetupModelHandler()));
            //                 &fileSystemController, SLOT(cancelOperation()));

    //gEngine->load(url);

    return app.exec();
}
