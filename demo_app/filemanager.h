#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include "filetreeelement.h"

#include <QList>
#include <QDirIterator>

class FileManager
{
public:
    FileManager();

    FileTreeElement *generateFileTree(const QString &rootPath)
    {
        if (!QFile::exists(rootPath))
            return {};

        QDir currentDir(rootPath);
        FileTreeElement *fileTreeRoot = new FileTreeElement(rootPath, nullptr);
        fileTreeRoot->setChildElements(getInnerFiles(rootPath, fileTreeRoot));

        return fileTreeRoot;
    }

private:
    QList<FileTreeElement *> getInnerFiles(const QString& currentFilename, FileTreeElement *parent)
    {
        QDirIterator it(currentFilename, QDirIterator::Subdirectories);
        QList<FileTreeElement *> innerFiles;
        while (it.hasNext())
        {
             QString currentFilePath = it.next();
             if (currentFilePath.endsWith(".") || currentFilePath.endsWith(".."))
                 continue;

             FileTreeElement *fileTreeElement = new FileTreeElement(currentFilePath, parent);
             if (!QFileInfo(currentFilePath).isFile())
             {
                fileTreeElement->setChildElements(getInnerFiles(currentFilePath, fileTreeElement));
             }

             innerFiles.append(fileTreeElement);
        }

        return innerFiles;
    }
};

#endif // FILEMANAGER_H
