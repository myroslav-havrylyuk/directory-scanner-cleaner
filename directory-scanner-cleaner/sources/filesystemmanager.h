#ifndef FILESYSTEMMANAGER_H
#define FILESYSTEMMANAGER_H

#include "models/filetreeelement.h"

#include <QList>
#include <QDir>

class FileSystemManager
{
public:
    FileSystemManager();

    FileTreeElement *generateFileTree(const QString &rootPath);
    quint64 getDirectorySize(const QString &directory);

private:
    QList<FileTreeElement *> getInnerFiles(const QDir &currenDir, FileTreeElement *parent);
};

#endif // FILESYSTEMMANAGER_H
