#ifndef FILESYSTEMMODEL_H
#define FILESYSTEMMODEL_H

#include <QAbstractItemModel>
#include <QFileInfo>
#include <QDir>
#include <QItemSelectionModel>

#include "filetreeelement.h"
#include "sources/filesystemmanager.h"

class FileSystemModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    FileSystemModel(QItemSelectionModel *itemSelectionModel);
    FileSystemModel(QItemSelectionModel *itemSelectionModel, const QString &rootPath);

    ~FileSystemModel();

    // QAbstractItemModel interface
public:
    QHash<int, QByteArray> roleNames() const;
    virtual QModelIndex index(int row, int column, const QModelIndex &parent) const;
    virtual QModelIndex parent(const QModelIndex &child) const;
    virtual int rowCount(const QModelIndex &parent) const;
    virtual int columnCount(const QModelIndex &parent) const;
    virtual QVariant data(const QModelIndex &index, int role) const;
    bool hasChildren(const QModelIndex &parent) const;
    bool hasIndex(int row, int column, const QModelIndex &parent) const;

    void addToSelection(const QModelIndex &index);

    void setRootPath(const QString &rootPath);
    QString getRootPath();

private:
    void setupModel(const QString &rootPath);
    FileTreeElement *indexToFileTreeElement(const QModelIndex &index) const;

private:
    FileTreeElement *m_FileTreeRoot;
    QItemSelectionModel *m_ItemSelectionModel;
};

#endif // FILESYSTEMMODEL_H
