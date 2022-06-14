#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include "filetreeelement.h"

#include <QList>
#include <QDir>
#include <QDirIterator>

class FileManager
{
public:
    FileManager();

    FileTreeElement *generateFileTree(const QString &rootPath);
    quint64 getDirectorySize(const QString &directory);

private:
    QList<FileTreeElement *> getInnerFiles(const QDir &currenDir, FileTreeElement *parent);
};

#endif // FILEMANAGER_H
