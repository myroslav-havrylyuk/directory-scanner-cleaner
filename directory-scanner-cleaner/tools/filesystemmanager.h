#ifndef FILESYSTEMMANAGER_H
#define FILESYSTEMMANAGER_H

#include "models/filetreeelement.h"

#include <QList>
#include <QDir>
#include <QPromise>
#include <QObject>
#include <QFutureWatcher>
#include <QFuture>

class FileSystemManager : public QObject
{
    Q_OBJECT
private:
    class FILE_TREE_GENERATION_FLAGS{
    private:
        bool m_GotInnerFiles;
        bool m_GotDirectorySize;
    public:
        void resetFlags(){
            m_GotInnerFiles = false;
            m_GotDirectorySize = false;
        }
        bool isAllFlagsSet(){
            if(m_GotInnerFiles == m_GotDirectorySize == true){
                return true;
            }
            return false;
        }
        void setGotInnerFilesFlag(bool value){
            m_GotInnerFiles = value;
        }
        void setGotDirectorySizeFlag(bool value){
            m_GotDirectorySize = value;
        }
    };
    FILE_TREE_GENERATION_FLAGS m_FileTreeGenerationFlags;
    QFutureWatcher<FileTreeElement *> *m_GetInnerFilesWatcher = nullptr;
    QFutureWatcher<quint64> *m_GetRootElementSizeWatcher = nullptr;
    QFutureWatcher<void> *m_CancelationWatcher = nullptr;
    QFuture<FileTreeElement *> *m_GetInnerFilesFuture = nullptr;
    QFuture<quint64> *m_GetRootElementSizeFuture = nullptr;
    QFuture<void> *m_CancelationFuture = nullptr;
    FileTreeElement *m_FileTreeRoot;
    FileTreeElement *m_FilesystemRootElement;
    QList<FileTreeElement *> getInnerFiles(QPromise<FileTreeElement *> &promise, bool &outWasCanceled, const QDir &currenDir, FileTreeElement *parent, uint recursionDepth, uint currentRecursionDepth);
    void getInnerFilesAsync(QPromise<FileTreeElement *> &promise, const QDir &currenDir, FileTreeElement *parent, uint recursionDepth);
    FileTreeElement *generateFileTreeElementAsync(const QString &rootPath);
    void waitForCancelation(QPromise<void> &promise);

public:
    explicit FileSystemManager(QObject *parent = nullptr);
    void generateFileTreeAsync(const QString &rootPath, uint recursionDepth);
    quint64 getDirectorySize(const QString &directory);
    void getDirectorySizeAsync(QPromise<quint64> &promise, const QString &directory);
    bool deleteFile(const QString &filename);

public slots:
    void handleGetInnerFilesFinished();
    void handleGetRootElementSizeFinished();
    void handleWaitForCancelationFinished();
    void cancelSetupModelHandler();

signals:
    void fileTreeGenerated(FileTreeElement * fileTreeRoot);
    void setupModelCanceled();
};

#endif // FILESYSTEMMANAGER_H
