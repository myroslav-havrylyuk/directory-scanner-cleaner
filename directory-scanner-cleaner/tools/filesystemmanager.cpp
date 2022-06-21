#include "filesystemmanager.h"

#include <QtConcurrent/QtConcurrent>
#include <QDirIterator>

FileSystemManager::FileSystemManager(QObject *parent)
    : QObject{parent}
{

}

FileTreeElement *FileSystemManager::generateFileTree(const QString &rootPath)
{
    QString normalizedRootPath = QDir::cleanPath(rootPath);
    if (!QFile::exists(normalizedRootPath))
        return {};

    QDir currentDir(normalizedRootPath);
    FileTreeElement *fileTreeRoot = new FileTreeElement(normalizedRootPath, getDirectorySize(currentDir.absolutePath()), nullptr);
    future = QtConcurrent::run(&FileSystemManager::getInnerFilesAsync, this, currentDir, fileTreeRoot);

    fileTreeRoot->setChildElements(future.results());
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

void FileSystemManager::getInnerFilesAsync(QPromise<FileTreeElement *> &promise, const QDir &currenDir, FileTreeElement *parent)
{
    qDebug() << "Getting inner files in thread:" << QThread::currentThread();
    QList<FileTreeElement *> innerFiles;
    for (auto &fileElement : currenDir.entryInfoList(QDir::NoDot | QDir::NoDotDot | QDir::AllEntries))
    {
         FileTreeElement *fileTreeElement = new FileTreeElement(fileElement.fileName(), fileElement.size(), parent);
         qDebug() << "currently reading:" << fileTreeElement->fileName();

         if (fileElement.isDir())
         {
             QList<FileTreeElement *> innerFiles = getInnerFiles(QDir(fileElement.absoluteFilePath()), fileTreeElement);
             fileTreeElement->setChildElements(innerFiles);
             fileTreeElement->setFileSize(getDirectorySize(fileElement.absoluteFilePath()));
         }

         //innerFiles.append(fileTreeElement);
         promise.addResult(fileTreeElement);
    }
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
