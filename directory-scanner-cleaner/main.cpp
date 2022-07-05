#include "controllers/mainwindowcontroller.h"
#include "tools/configfilehandler.h"

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickStyle>
#include <QQuickView>
#include <QThread>

QQmlApplicationEngine *gEngine;
QGuiApplication *gApp;
SettingsController *gSettingsController;

int main(int argc, char *argv[]) {
    gApp = new QGuiApplication(argc, argv);
    gEngine = new QQmlApplicationEngine();
    ConfigFileHandler *handler = new ConfigFileHandler();
    gSettingsController = new SettingsController(*handler);
    qDebug() << "Main thread: " << QThread::currentThread();
    QQuickStyle::setStyle("Fusion");
    MainWindowController mainWindowController;
    return gApp->exec();
}
