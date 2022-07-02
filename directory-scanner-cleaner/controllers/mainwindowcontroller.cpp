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

    m_FileSystemModel = new FileSystemModel();
    m_FileSystemController = new FileSystemController(*m_FileSystemModel);
    QString rootFilePath = gApp->applicationDirPath();
    setActivePath(rootFilePath);
    m_MainQmlContext = gEngine->rootContext();
    m_MainQmlContext->setContextProperty("FileSystemModel", m_FileSystemModel);
    m_MainQmlContext->setContextProperty("FileSystemController", m_FileSystemController);
    m_MainQmlContext->setContextProperty("MainWindowController", this);

    // QObject *test = mainQmlContext->objectForName("SettingsController");
    // qDebug() << ((SettingsController *)test)->getHistoryPath();

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
    QObject *folderDialog =
            m_MainWindow->findChild<QObject *>("folder_dialog");
    QObject::connect(folderDialog, SIGNAL(activePathChanged(QString)),
                     this, SLOT(setActivePath(QString)));
    QObject *filterButton = m_MainWindow->findChild<QObject *>("filter_button");
    QObject::connect(filterButton, SIGNAL(selectByFilter()), m_FileSystemController,
                     SLOT(selectByFilter()));
}

void MainWindowController::openSettingsWindow() {
    SettingsWindowController *settingsWindowController = new SettingsWindowController();
    qDebug() << "MainWindowController: openSettingsWindow func called";
}

void MainWindowController::updateDeletionHistory() {
    m_FileDeletionHistoryManager.updateHistory(
                QList<QString>{"test file 1.docx", "test file 2.pdf"},
                "reason",
                gSettingsController->getHistoryPath()+"/filesDeletionHistory"+" - "+QDate::currentDate().toString("dd.MM.yyyy")+".json");
}

void MainWindowController::setActivePath(QString activePath)
{
    m_FileSystemController->setActivePath(activePath, gSettingsController->getRecursionDepth());
}
