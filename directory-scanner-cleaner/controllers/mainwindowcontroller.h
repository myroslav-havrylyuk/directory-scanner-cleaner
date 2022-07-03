#ifndef MAINWINDOWCONTROLLER_H
#define MAINWINDOWCONTROLLER_H

#include "models/filesystemmodel.h"
#include "controllers/filesystemcontroller.h"
#include "settingscontroller.h"
#include "settingswindowcontroller.h"
#include "tools/configfilehandler.h"
#include "tools/filesdeletionhistorymanager.h"

#include <QObject>
#include <QDebug>
#include <QQmlApplicationEngine>
#include <QQmlComponent>
#include <QQmlContext>
#include <QGuiApplication>

extern QQmlApplicationEngine *gEngine;
extern QGuiApplication *gApp;
extern SettingsController *gSettingsController;

class MainWindowController : public QObject
{
    Q_OBJECT

private:
    QObject *m_MainWindow = nullptr;

    QQmlContext *m_MainQmlContext;
    FilesDeletionHistoryManager m_FileDeletionHistoryManager;
    FileSystemModel *m_FileSystemModel;
    FileSystemController *m_FileSystemController;
public:
    explicit MainWindowController(QObject *parent = nullptr);


    QQmlContext *getMainQmlContext() const;
    void setMainQmlContext(QQmlContext *newMainQmlContext);

    const FilesDeletionHistoryManager &getFileDeletionHistoryManager() const;

signals:

public slots:
    void openSettingsWindow();
    void updateDeletionHistory(const QList<QString> &deletedFilesList, const QString &fileDeletionReason);
    void setActivePath(QString activePath);
};

#endif // MAINWINDOWCONTROLLER_H
