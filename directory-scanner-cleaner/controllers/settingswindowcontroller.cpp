#include "settingswindowcontroller.h"

SettingsWindowController::SettingsWindowController(QObject *parent)
    : QObject{parent} {
  m_Handler = new ConfigFileHandler();
  gSettingsController = new SettingsController(*m_Handler);
  const QUrl url(u"qrc:/directory-scanner-cleaner/views/settingswindow.qml"_qs);
  if (m_SettingsWindow == nullptr) {
    m_SettingsWindowComponent = new QQmlComponent(gEngine, url);
    m_SettingsQmlContext = new QQmlContext(gEngine->rootContext());
    m_SettingsQmlContext->setContextProperty("SettingsController",
                                             gSettingsController);
    m_SettingsWindow = m_SettingsWindowComponent->create(m_SettingsQmlContext);
  } else {
    delete m_SettingsWindow;
    m_SettingsWindow = nullptr;
    delete m_SettingsWindowComponent;
    m_SettingsWindowComponent = nullptr;
    delete m_SettingsQmlContext;
    m_SettingsQmlContext = nullptr;
    delete m_Handler;
    m_Handler = nullptr;
    delete gSettingsController;
    gSettingsController = nullptr;

    m_SettingsWindowComponent = new QQmlComponent(gEngine, url);
    m_Handler = new ConfigFileHandler();
    gSettingsController = new SettingsController(*m_Handler);
    m_SettingsQmlContext = new QQmlContext(gEngine->rootContext());
    m_SettingsQmlContext->setContextProperty("SettingsController",
                                             gSettingsController);
    m_SettingsWindow = m_SettingsWindowComponent->create(m_SettingsQmlContext);
  }
  QObject *saveButton = m_SettingsWindow->findChild<QObject *>("save_button");
  QObject::connect(saveButton, SIGNAL(saveSettings()), gSettingsController,
                   SLOT(saveSettings()));
}
