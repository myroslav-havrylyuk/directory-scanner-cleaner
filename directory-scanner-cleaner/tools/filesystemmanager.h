#ifndef FILESYSTEMMANAGER_H
#define FILESYSTEMMANAGER_H

#include "models/filetreeelement.h"

#include <QList>
#include <QDir>
#include <QObject>

class FileSystemManager : public QObject
{
    Q_OBJECT
public:
    explicit FileSystemManager(QObject *parent = nullptr);

    FileTreeElement *generateFileTree(const QString &rootPath);
    quint64 getDirectorySize(const QString &directory);

private:
    QList<FileTreeElement *> getInnerFiles(const QDir &currenDir, FileTreeElement *parent);
};

#endif // FILESYSTEMMANAGER_H
