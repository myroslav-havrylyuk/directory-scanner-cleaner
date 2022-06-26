#ifndef FILESYSTEMMODEL_H
#define FILESYSTEMMODEL_H

#include <QAbstractItemModel>
#include <QFileInfo>
#include <QDir>

#include "filetreeelement.h"
#include "tools/filesystemmanager.h"

class FileSystemModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    FileSystemModel();
    //FileSystemModel(const QString &rootPath);

    ~FileSystemModel();

    // QAbstractItemModel interface
    QHash<int, QByteArray> roleNames() const;
    virtual QModelIndex index(int row, int column, const QModelIndex &parent) const;
    virtual QModelIndex parent(const QModelIndex &child) const;
    virtual int rowCount(const QModelIndex &parent) const;
    virtual int columnCount(const QModelIndex &parent) const;
    virtual QVariant data(const QModelIndex &index, int role) const;
    bool hasChildren(const QModelIndex &parent) const;
    bool hasIndex(int row, int column, const QModelIndex &parent) const;
    void setupModel(const QString &rootPath);
    QString getRootPath();

private:
    FileSystemManager *m_FileSystemManager = nullptr;
    FileTreeElement *indexToFileTreeElement(const QModelIndex &index) const;
    FileTreeElement *m_FilesystemRootElement;
    FileTreeElement *m_FileTreeRoot;
    void connectToFileSystemManager();

signals:
    void modelSetupStarted(const QString &rootPath);
    void modelSetupFinished();
    void cancelSetupModel();
    void setupModelCanceled();


public slots:
    void fileTreeGeneratedHandler(FileTreeElement * fileTreeRoot);
    void cancelSetupModelHandler();
    void setupModelCanceledHandler();
};

#endif // FILESYSTEMMODEL_H
