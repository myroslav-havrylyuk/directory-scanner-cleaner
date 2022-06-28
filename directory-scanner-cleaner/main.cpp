#include "controllers/filesystemcontroller.h"
#include "controllers/mainwindowcontroller.h"
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
    QQuickStyle::setStyle("Fusion");

    gEngine = new QQmlApplicationEngine();

    QString rootFilePath = app.applicationDirPath();

    FileSystemModel fileSystemModel;
    FileSystemController fileSystemController(fileSystemModel);
    fileSystemController.setActivePath("C:\\My\\Study\\My_3_course\\2_term\\ZPKG");

    const QUrl url(u"qrc:/directory-scanner-cleaner/views/main.qml"_qs);
    QQmlContext *mainQmlContext = gEngine->rootContext();

    MainWindowController mainWindowController;
    mainQmlContext->setContextProperty("FileSystemModel", &fileSystemModel);
    mainQmlContext->setContextProperty("FileSystemController", &fileSystemController);
    mainQmlContext->setContextProperty("MainWindowController", &mainWindowController);
    //QObject *test = mainQmlContext->objectForName("SettingsController");
    //qDebug() << ((SettingsController *)test)->getHistoryPath();


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
                             &fileSystemModel, SLOT(cancelSetupModelHandler()));
            QObject *settingsMenuItem = mainWindow->findChild<QObject*>("settings_menu_item");
            QObject::connect(settingsMenuItem, SIGNAL(openSettingsWindow()),
                             &mainWindowController, SLOT(openSettingsWindow()));

    //gEngine->load(url);

    return app.exec();
}
