#ifndef MAINWINDOWCONTROLLER_H
#define MAINWINDOWCONTROLLER_H

#include "controllers/filesystemcontroller.h"
#include "models/filesystemmodel.h"
#include "settingscontroller.h"
#include "settingswindowcontroller.h"
#include "tools/configfilehandler.h"
#include "tools/filesdeletionhistorymanager.h"

#include <QDebug>
#include <QGuiApplication>
#include <QObject>
#include <QQmlApplicationEngine>
#include <QQmlComponent>
#include <QQmlContext>

extern QQmlApplicationEngine *gEngine;
extern QGuiApplication *gApp;
extern SettingsController *gSettingsController;

class MainWindowController : public QObject {
    Q_OBJECT

private:
    QObject *m_MainWindow = nullptr;

    QQmlContext *m_MainQmlContext;
    FilesDeletionHistoryManager m_FileDeletionHistoryManager;
    FileSystemModel *m_FileSystemModel;
    FileSystemController *m_FileSystemController;
    SettingsWindowController *m_SettingsWindowController = nullptr;

public:
    explicit MainWindowController(QObject *parent = nullptr);
    ~MainWindowController();
    QQmlContext *getMainQmlContext() const;
    void setMainQmlContext(QQmlContext *newMainQmlContext);

    const FilesDeletionHistoryManager &getFileDeletionHistoryManager() const;

public slots:
    void openSettingsWindow();
    void updateDeletionHistory(const QList<QString> &deletedFilesList,
                               const QString &fileDeletionReason);
    void setActivePath(QString activePath);
};

#endif // MAINWINDOWCONTROLLER_H
