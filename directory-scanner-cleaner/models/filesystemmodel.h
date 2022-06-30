#ifndef FILESYSTEMMODEL_H
#define FILESYSTEMMODEL_H

#include <QItemSelectionModel>
#include <QAbstractItemModel>
#include <QFileInfo>
#include <QDir>

#include "filetreeelement.h"
#include "tools/filesystemmanager.h"

class FileSystemModel : public QAbstractItemModel
{
    Q_OBJECT

    Q_PROPERTY(QItemSelectionModel* itemSelectionModel READ getItemSelectionModel NOTIFY itemSelectionModelChanged)
    Q_PROPERTY(QModelIndex rootIndex MEMBER m_RootIndex)

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
    void selectFile(QModelIndex index);
    QModelIndex getRootIndex();

    template<typename UnaryPredicate>
    void selectFilesIf(QModelIndex root, UnaryPredicate pred);

private:
    QModelIndex m_RootIndex;
    QItemSelectionModel m_ItemSelectionModel;
    FileSystemManager *m_FileSystemManager = nullptr;
    FileTreeElement *indexToFileTreeElement(const QModelIndex &index) const;
    FileTreeElement *m_FilesystemRootElement;
    FileTreeElement *m_FileTreeRoot;
    void connectToFileSystemManager();
    QItemSelectionModel *getItemSelectionModel();

signals:
    void modelSetupStarted(const QString &rootPath);
    void modelSetupFinished();
    void cancelSetupModel();
    void setupModelCanceled();
    void itemSelectionModelChanged();


public slots:
    void fileTreeGeneratedHandler(FileTreeElement * fileTreeRoot);
    void cancelSetupModelHandler();
    void setupModelCanceledHandler();
};

template<typename UnaryPredicate>
void FileSystemModel::selectFilesIf(QModelIndex root, UnaryPredicate pred)
{
    int row = 0;
    QModelIndex el = index(row, 0, root);
     while(el.internalPointer() != nullptr)
    {
        qDebug() << indexToFileTreeElement(el)->fileName();
        if(pred(indexToFileTreeElement(el)))
        {
            qDebug() << indexToFileTreeElement(el)->formattedSize();
            m_ItemSelectionModel.select(el, QItemSelectionModel::Select);

            if(hasChildren(el))
            {
                selectFilesIf(el, pred);
            }
        }
        ++row;
        el = index(row, 0, root);
    }
}

#endif // FILESYSTEMMODEL_H
