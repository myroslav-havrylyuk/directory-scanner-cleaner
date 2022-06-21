#ifndef FILESYSTEMMANAGER_H
#define FILESYSTEMMANAGER_H

#include "models/filetreeelement.h"

#include <QList>
#include <QDir>
#include <QPromise>
#include <QObject>
#include <QFutureWatcher>
#include <QFuture>

class FileSystemManager : public QObject
{
    Q_OBJECT
public:
    explicit FileSystemManager(QObject *parent = nullptr);

    FileTreeElement *generateFileTree(const QString &rootPath);
    quint64 getDirectorySize(const QString &directory);

private:
    QFutureWatcher<FileTreeElement *> watcher;
    QFuture<FileTreeElement *> future;
    QList<FileTreeElement *> getInnerFiles(const QDir &currenDir, FileTreeElement *parent);
    void getInnerFilesAsync(QPromise<FileTreeElement *> &promise, const QDir &currenDir, FileTreeElement *parent);
public slots:
    void progressLog(int progress);
    void handleFinished();
};

#endif // FILESYSTEMMANAGER_H
