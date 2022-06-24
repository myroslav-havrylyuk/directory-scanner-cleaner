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
    if(m_GetInnerFilesFuture != nullptr){
        delete m_GetInnerFilesFuture;
        m_GetInnerFilesFuture = nullptr;
    }
    if(m_GetInnerFilesWatcher != nullptr){
        delete m_GetInnerFilesWatcher;
        m_GetInnerFilesWatcher = nullptr;
    }
    m_GetInnerFilesFuture = new QFuture<FileTreeElement *>();
    m_GetInnerFilesWatcher = new QFutureWatcher<FileTreeElement *>();
    *m_GetInnerFilesFuture = QtConcurrent::run(&FileSystemManager::getInnerFilesAsync, this, currentDir, m_FileTreeRoot);
    QObject::connect(m_GetInnerFilesWatcher, &QFutureWatcher<FileTreeElement *>::finished, this, &FileSystemManager::handleGetInnerFilesFinished);
    m_GetInnerFilesWatcher->setFuture(*m_GetInnerFilesFuture);
    //getting root directory size
    if(m_GetRootElementSizeFuture != nullptr){
        delete m_GetRootElementSizeFuture;
        m_GetRootElementSizeFuture = nullptr;
    }
    if(m_GetRootElementSizeWatcher != nullptr){
        delete m_GetRootElementSizeWatcher;
        m_GetRootElementSizeWatcher = nullptr;
    }
    m_GetRootElementSizeFuture = new QFuture<quint64>();
    m_GetRootElementSizeWatcher = new QFutureWatcher<quint64>();
    *m_GetRootElementSizeFuture = QtConcurrent::run(&FileSystemManager::getDirectorySizeAsync, this, currentDir.absolutePath());
    connect(m_GetRootElementSizeWatcher, &QFutureWatcher<FileTreeElement *>::finished, this, &FileSystemManager::handleGetRootElementSizeFinished);
    m_GetRootElementSizeWatcher->setFuture(*m_GetRootElementSizeFuture);
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
        if (promise.isCanceled())
            return;
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
    QList<FileTreeElement *> results = m_GetInnerFilesFuture->results();
    m_FileTreeRoot->setChildElements(results);
    m_FileTreeGenerationFlags.setGotInnerFilesFlag(true);
    if(m_FileTreeGenerationFlags.isAllFlagsSet()){
        emit fileTreeGenerated(m_FileTreeRoot);
        m_FileTreeGenerationFlags.resetFlags();
    }
    qDebug() << QTime::currentTime() << "finished getting inner files";
}

void FileSystemManager::handleGetRootElementSizeFinished()
{
    QList<quint64> results = m_GetRootElementSizeFuture->results();
    //in case operation is cancelled before even getting one result
    if(!results.isEmpty()){
        m_FileTreeRoot->setFileSize(results.last());
    }
    m_FileTreeGenerationFlags.setGotDirectorySizeFlag(true);
    if(m_FileTreeGenerationFlags.isAllFlagsSet()){
        emit fileTreeGenerated(m_FileTreeRoot);
        m_FileTreeGenerationFlags.resetFlags();
    }
    qDebug() << QTime::currentTime() << "finished getting root element size";
}

void FileSystemManager::cancelSetupModelHandler()
{
    m_GetRootElementSizeFuture->cancel();
    m_GetInnerFilesFuture->cancel();
    emit setupModelCanceled();
    qDebug() << QTime::currentTime() << "cancel setup model handler in FileSystemManager";
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

void FileSystemManager::getDirectorySizeAsync(QPromise<quint64> &promise, const QString &directory)
{
    quint64 size = 0;
    QDirIterator iterator(directory, QDir::Files | QDir::NoSymLinks, QDirIterator::Subdirectories);
    while (iterator.hasNext()) {
        if (promise.isCanceled())
            return;
        iterator.next();
        size += iterator.fileInfo().size();
        promise.addResult(size);
    }
}
