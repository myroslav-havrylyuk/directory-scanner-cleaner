
#include "controllers/mainwindowcontroller.h"
#include "tools/configfilehandler.h"

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QThread>
#include <QQuickStyle>
#include <QQuickView>

QQmlApplicationEngine *gEngine;
QGuiApplication *app;

int main(int argc, char *argv[])
{
    app = new QGuiApplication(argc, argv);

    qDebug() << "Main thread: " << QThread::currentThread();
    QQuickStyle::setStyle("Fusion");

    gEngine = new QQmlApplicationEngine();

    MainWindowController mainWindowController;

    //gEngine->load(url);

    return app->exec();
}
