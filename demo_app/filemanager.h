#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include "filetreeelement.h"

#include <QList>
#include <QDir>

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
        fileTreeRoot->setChildElements(getInnerFiles(QDir(rootPath), fileTreeRoot));

        return fileTreeRoot;
    }

private:
    QList<FileTreeElement *> getInnerFiles(const QDir &currenDir, FileTreeElement *parent)
    {
        QList<FileTreeElement *> innerFiles;

        for (auto &fileElement : currenDir.entryInfoList())
        {
             if (fileElement.isDir())
             {
                 if (fileElement.fileName().endsWith(".") || fileElement.fileName().endsWith(".."))
                     continue;
             }

             FileTreeElement *fileTreeElement = new FileTreeElement(fileElement.fileName(), parent);

             if (fileElement.isDir())
             {
                 QList<FileTreeElement *> innerFiles = getInnerFiles(QDir(fileElement.absoluteFilePath()), fileTreeElement);
                 fileTreeElement->setChildElements(innerFiles);
             }

             innerFiles.append(fileTreeElement);
        }

        return innerFiles;
    }
};

#endif // FILEMANAGER_H
