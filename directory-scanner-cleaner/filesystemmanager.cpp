#include "filesystemmanager.h"

#include <QDirIterator>

FileSystemManager::FileSystemManager()
{

}

FileTreeElement *FileSystemManager::generateFileTree(const QString &rootPath)
{
    if (!QFile::exists(rootPath))
        return {};

    QDir currentDir(rootPath);
    FileTreeElement *fileTreeRoot = new FileTreeElement(rootPath, getDirectorySize(currentDir.absolutePath()), nullptr);
    fileTreeRoot->setChildElements(getInnerFiles(QDir(rootPath), fileTreeRoot));

    return fileTreeRoot;
}

QList<FileTreeElement *> FileSystemManager::getInnerFiles(const QDir &currenDir, FileTreeElement *parent)
{
    QList<FileTreeElement *> innerFiles;

    for (auto &fileElement : currenDir.entryInfoList(QDir::NoDot | QDir::NoDotDot | QDir::AllEntries))
    {
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

quint64 FileSystemManager::getDirectorySize(const QString &directory)
{
    quint64 size = 0;
    QDirIterator iterator(directory, QDir::Files | QDir::NoSymLinks, QDirIterator::Subdirectories);
    while (iterator.hasNext()) {
        iterator.next();
        size += iterator.fileInfo().size();
    }
    return size;
}
