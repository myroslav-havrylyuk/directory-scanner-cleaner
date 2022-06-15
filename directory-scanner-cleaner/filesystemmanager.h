#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include "filetreeelement.h"

#include <QList>
#include <QDir>

class FileSystemManager
{
public:
    FileSystemManager();

    FileTreeElement *generateFileTree(const QString &rootPath);

private:
    QList<FileTreeElement *> getInnerFiles(const QDir &currenDir, FileTreeElement *parent);
};

#endif // FILEMANAGER_H
