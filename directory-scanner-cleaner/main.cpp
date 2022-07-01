
#include "controllers/mainwindowcontroller.h"
#include "tools/configfilehandler.h"

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QThread>
#include <QQuickStyle>
#include <QQuickView>

QQmlApplicationEngine *gEngine;
QGuiApplication *gApp;
SettingsController *gSettingsController;

int main(int argc, char *argv[])
{
    gApp = new QGuiApplication(argc, argv);

    qDebug() << "Main thread: " << QThread::currentThread();
    QQuickStyle::setStyle("Fusion");

    gEngine = new QQmlApplicationEngine();


    ConfigFileHandler *handler = new ConfigFileHandler();
    gSettingsController = new SettingsController(*handler);
    MainWindowController mainWindowController;

    //gEngine->load(url);

    return gApp->exec();
}
