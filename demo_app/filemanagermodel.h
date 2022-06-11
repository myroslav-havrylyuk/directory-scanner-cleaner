#ifndef FILEMANAGERMODEL_H
#define FILEMANAGERMODEL_H

#include <QAbstractItemModel>
#include <QFileInfo>
#include <QDir>

#include "filetreeelement.h"
#include "filemanager.h"

class FileManagerModel : public QAbstractItemModel
{
public:
    FileManagerModel();
    FileManagerModel(const QString &rootPath);

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

    void setRootPath(const QString &rootPath);
    QString getRootPath();


private:
    void setupModel(const QString &rootPath);
    FileTreeElement *indexToFileTreeElement(const QModelIndex &index) const;

private:
    FileTreeElement *m_FileTreeRoot;
};

#endif // FILEMANAGERMODEL_H
