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
    fileTreeRoot->setChildElements(getInnerFiles(QDir(normalizedRootPath), fileTreeRoot));

    return fileTreeRoot;
}

void FileSystemManager::generateFileTreeAsync(const QString &rootPath)
{
    QString normalizedRootPath = QDir::cleanPath(rootPath);
    if (!QFile::exists(normalizedRootPath))
        return ;

    QDir currentDir(normalizedRootPath);
    m_FileTreeRoot = new FileTreeElement(normalizedRootPath, 0, nullptr);
    //getting inner files of root
    m_GetInnerFiles = QtConcurrent::run(&FileSystemManager::getInnerFilesAsync, this, currentDir, m_FileTreeRoot);
    QObject::connect(&m_GetInnerFilesWatcher, &QFutureWatcher<FileTreeElement *>::finished, this, &FileSystemManager::handleGetInnerFilesFinished);
    m_GetInnerFilesWatcher.setFuture(m_GetInnerFiles);
    //getting root directory size
    m_GetRootElementSizeFuture = QtConcurrent::run(&FileSystemManager::getDirectorySize, this, currentDir.absolutePath());
    connect(&m_GetRootElementSizeWatcher, &QFutureWatcher<FileTreeElement *>::finished, this, &FileSystemManager::handleGetRootElementSizeFinished);
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
    int counter = 0;
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
         promise.addResult(fileTreeElement);
         qDebug() << "files/folders read:" << ++counter;
    }
}

void FileSystemManager::handleGetInnerFilesFinished()
{
    m_FileTreeRoot->setChildElements(m_GetInnerFiles.results());
    m_FileTreeGenerationFlags.setGotInnerFilesFlag(true);
    if(m_FileTreeGenerationFlags.isAllFlagsSet()){
        emit fileTreeGenerated(m_FileTreeRoot);
        m_FileTreeGenerationFlags.resetFlags();
    }
    qDebug() << "finished getting inner files";
}

void FileSystemManager::handleGetRootElementSizeFinished()
{
    m_FileTreeRoot->setFileSize(m_GetRootElementSizeFuture.result());
     m_FileTreeGenerationFlags.setGotDirectorySizeFlag(true);
     if(m_FileTreeGenerationFlags.isAllFlagsSet()){
         emit fileTreeGenerated(m_FileTreeRoot);
         m_FileTreeGenerationFlags.resetFlags();
     }
    qDebug() << "finished getting root element size";
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
