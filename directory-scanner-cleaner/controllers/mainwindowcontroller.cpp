#include "mainwindowcontroller.h"

QQmlContext *MainWindowController::getMainQmlContext() const {
    return m_MainQmlContext;
}

void MainWindowController::setMainQmlContext(QQmlContext *newMainQmlContext) {
    m_MainQmlContext = newMainQmlContext;
}

const FilesDeletionHistoryManager &
MainWindowController::getFileDeletionHistoryManager() const {
    return m_FileDeletionHistoryManager;
}

MainWindowController::MainWindowController(QObject *parent) : QObject{parent} {
    m_Handler = new ConfigFileHandler();
    m_SettingsController = new SettingsController(*m_Handler);

    m_FileSystemModel = new FileSystemModel();
    m_FileSystemController = new FileSystemController(*m_FileSystemModel);
    QString rootFilePath = app->applicationDirPath();
    m_MainQmlContext = gEngine->rootContext();
    m_MainQmlContext->setContextProperty("FileSystemModel", m_FileSystemModel);
    m_MainQmlContext->setContextProperty("FileSystemController", m_FileSystemController);
    m_MainQmlContext->setContextProperty("MainWindowController", this);



    const QUrl url(u"qrc:/directory-scanner-cleaner/views/main.qml"_qs);
    QQmlComponent mainWindowComponent(gEngine, url);

    m_MainWindow = mainWindowComponent.create();

    QObject *progressDialog =
            m_MainWindow->findChild<QObject *>("progress_dialog");
    QObject::connect(progressDialog, SIGNAL(cancelSetupModel()),
                     m_FileSystemModel, SLOT(cancelSetupModelHandler()));
    QObject *settingsMenuItem =
            m_MainWindow->findChild<QObject *>("settings_menu_item");
    QObject::connect(settingsMenuItem, SIGNAL(openSettingsWindow()),
                     this, SLOT(openSettingsWindow()));
    QObject *deleteButton = m_MainWindow->findChild<QObject *>("delete_button");
    QObject::connect(deleteButton, SIGNAL(deleteFiles()), this,
                     SLOT(updateDeletionHistory()));
}

void MainWindowController::openSettingsWindow() {

    const QUrl url(u"qrc:/directory-scanner-cleaner/views/settingswindow.qml"_qs);
    if (m_SettingsWindow == nullptr) {
        m_SettingsWindowComponent = new QQmlComponent(gEngine, url);
        m_SettingsQmlContext = new QQmlContext(gEngine->rootContext());
        m_SettingsQmlContext->setContextProperty("SettingsController",
                                                 m_SettingsController);
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
        delete m_SettingsController;
        m_SettingsController = nullptr;

        m_SettingsWindowComponent = new QQmlComponent(gEngine, url);
        m_Handler = new ConfigFileHandler();
        m_SettingsController = new SettingsController(*m_Handler);
        m_SettingsQmlContext = new QQmlContext(gEngine->rootContext());
        m_SettingsQmlContext->setContextProperty("SettingsController",
                                                 m_SettingsController);
        m_SettingsWindow = m_SettingsWindowComponent->create(m_SettingsQmlContext);
    }
    QObject *saveButton = m_SettingsWindow->findChild<QObject *>("save_button");
    QObject::connect(saveButton, SIGNAL(saveSettings()), m_SettingsController,
                     SLOT(saveSettings()));
    qDebug() << "MainWindowController: openSettingsWindow func called";
}

void MainWindowController::updateDeletionHistory() {
    m_FileDeletionHistoryManager.updateHistory(
                QList<QString>{"test file 1.docx", "test file 2.pdf"},
                FilesDeletionHistoryManager::DELETE_REASON::OLD,
                m_SettingsController->getHistoryPath()+"/filesDeletionHistory.json");
}
