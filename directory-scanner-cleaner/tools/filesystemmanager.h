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
public:
    explicit FileSystemManager(QObject *parent = nullptr);

    FileTreeElement *generateFileTree(const QString &rootPath);
    void generateFileTreeAsync(const QString &rootPath);
    quint64 getDirectorySize(const QString &directory);
    void getDirectorySizeAsync(QPromise<quint64> &promise, const QString &directory);

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
    QFuture<FileTreeElement *> *m_GetInnerFilesFuture = nullptr;
    QFuture<quint64> *m_GetRootElementSizeFuture = nullptr;
    FileTreeElement *m_FileTreeRoot;
    FileTreeElement *m_FilesystemRootElement;
    QList<FileTreeElement *> getInnerFiles(const QDir &currenDir, FileTreeElement *parent);
    void getInnerFilesAsync(QPromise<FileTreeElement *> &promise, const QDir &currenDir, FileTreeElement *parent);
    FileTreeElement *generateFileTreeElementAsync(const QString &rootPath);
public slots:
    void handleGetInnerFilesFinished();
    void handleGetRootElementSizeFinished();
    void cancelSetupModelHandler();
signals:
    void fileTreeGenerated(FileTreeElement * fileTreeRoot);
    void setupModelCanceled();
};

#endif // FILESYSTEMMANAGER_H
