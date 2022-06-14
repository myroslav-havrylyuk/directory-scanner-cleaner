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

    FileTreeElement *generateFileTree(const QString &rootPath)
    {
        if (!QFile::exists(rootPath))
            return {};

        QDir currentDir(rootPath);
        FileTreeElement *fileTreeRoot = new FileTreeElement(rootPath, getDirectorySize(currentDir.absolutePath()), nullptr);
        fileTreeRoot->setChildElements(getInnerFiles(QDir(rootPath), fileTreeRoot));

        return fileTreeRoot;
    }

    quint64 getDirectorySize(const QString &directory)
    {
        quint64 size = 0;
        QDirIterator iterator(directory, QDir::Files | QDir::NoSymLinks, QDirIterator::Subdirectories);
        while (iterator.hasNext()) {
            iterator.next();
            size += iterator.fileInfo().size();
        }
        return size;
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

             FileTreeElement *fileTreeElement = new FileTreeElement(fileElement.fileName(), fileElement.size(), parent);

             if (fileElement.isDir())
             {
                 QList<FileTreeElement *> innerFiles = getInnerFiles(QDir(fileElement.absoluteFilePath()), fileTreeElement);
                 fileTreeElement->setChildElements(innerFiles);
                 fileTreeElement->setFileSize(getDirectorySize(fileElement.absoluteFilePath()));
             }

             innerFiles.append(fileTreeElement);
        }

        return innerFiles;
    }
};

#endif // FILEMANAGER_H
