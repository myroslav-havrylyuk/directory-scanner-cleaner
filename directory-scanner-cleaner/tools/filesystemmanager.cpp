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
    QObject::connect(&watcher, &QFutureWatcher<FileTreeElement *>::progressValueChanged, this, &FileSystemManager::progressLog);
    QObject::connect(&watcher, &QFutureWatcher<FileTreeElement *>::finished, this, &FileSystemManager::handleFinished);
    watcher.setFuture(future);

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
    promise.setProgressRange(0, currenDir.count());
    int counter = 0;
    promise.setProgressValue(counter);
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
         promise.setProgressValue(++counter);
         qDebug() << "files/folders read:" << counter;
    }
}

void FileSystemManager::progressLog(int progress)
{
    qDebug() << "current progress:" << progress << "thread of this message:" << QThread::currentThread();
}

void FileSystemManager::handleFinished()
{
    qDebug() << "finished";
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
