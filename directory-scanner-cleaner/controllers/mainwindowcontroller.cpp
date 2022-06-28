#include "mainwindowcontroller.h"


MainWindowController::MainWindowController(QObject *parent)
    : QObject{parent}
{

}

void MainWindowController::openSettingsWindow()
{

    const QUrl url(u"qrc:/directory-scanner-cleaner/views/settingswindow.qml"_qs);
    if(m_SettingsWindow == nullptr){
        m_Component = new QQmlComponent(gEngine, url);
        m_Handler = new ConfigFileHandler();
        m_SettingsController = new SettingsController(*m_Handler);
        m_SettingsQmlContext = new QQmlContext(gEngine->rootContext());
        m_SettingsQmlContext->setContextProperty("SettingsController", m_SettingsController);
        m_SettingsWindow  = m_Component->create(m_SettingsQmlContext);
    } else {
        delete m_SettingsWindow;
        m_SettingsWindow = nullptr;
        delete m_Component;
        m_Component = nullptr;
        delete m_SettingsQmlContext;
        m_SettingsQmlContext = nullptr;
        delete m_Handler;
        m_Handler = nullptr;
        delete m_SettingsController;
        m_SettingsController = nullptr;

        m_Component = new QQmlComponent(gEngine, url);
        m_Handler = new ConfigFileHandler();
        m_SettingsController = new SettingsController(*m_Handler);
        m_SettingsQmlContext = new QQmlContext(gEngine->rootContext());
        m_SettingsQmlContext->setContextProperty("SettingsController", m_SettingsController);
        m_SettingsWindow  = m_Component->create(m_SettingsQmlContext);
    }
    QObject *saveButton = m_SettingsWindow->findChild<QObject*>("save_button");
    QObject::connect(saveButton, SIGNAL(saveSettings()),
                     m_SettingsController, SLOT(saveSettings()));
    qDebug() << "MainWindowController: openSettingsWindow func called";
}
