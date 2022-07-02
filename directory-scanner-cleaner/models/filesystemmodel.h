#ifndef FILESYSTEMMODEL_H
#define FILESYSTEMMODEL_H

#include <QItemSelectionModel>
#include <QAbstractItemModel>
#include <QFileInfo>
#include <QDir>
#include <QPromise>

#include "QtConcurrent/QtConcurrent"
#include "filetreeelement.h"
#include "tools/filesystemmanager.h"

class FileSystemModel : public QAbstractItemModel
{
    Q_OBJECT

    Q_PROPERTY(QItemSelectionModel* itemSelectionModel READ getItemSelectionModel NOTIFY itemSelectionModelChanged)

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
    void setupModel(const QString &rootPath, uint recursionDepth);
    QString getRootPath();
    void selectFile(QModelIndex index);

    FileSystemManager *getFileSystemManager() const;
    template<typename UnaryPredicate>
    void selectFilesIf(QPromise<void> &promise, QModelIndex root, UnaryPredicate pred);
    template<typename UnaryPredicate>
    void selectFilesIfAsync(/*QModelIndex root, */UnaryPredicate pred);

private:
    QItemSelectionModel m_ItemSelectionModel;
    FileSystemManager *m_FileSystemManager = nullptr;
    FileTreeElement *indexToFileTreeElement(const QModelIndex &index) const;
    FileTreeElement *m_FilesystemRootElement;
    FileTreeElement *m_FileTreeRoot;
    void connectToFileSystemManager();
    QItemSelectionModel *getItemSelectionModel();

    QFutureWatcher<void> *m_SelectionWatcher = nullptr;
    QFuture<void> *m_SelectionFuture = nullptr;

signals:
    void modelSetupStarted(const QString &rootPath);
    void modelSetupFinished();
    void cancelSetupModel();
    void setupModelCanceled();
    void itemSelectionModelChanged();

    void selectionStarted();
    void selectionFinished();


public slots:
    void fileTreeGeneratedHandler(FileTreeElement * fileTreeRoot);
    void cancelSetupModelHandler();
    void setupModelCanceledHandler();
};

template<typename UnaryPredicate>
void FileSystemModel::selectFilesIfAsync(UnaryPredicate pred)
{
    QModelIndex root = createIndex(0,0, m_FilesystemRootElement);

    if(m_SelectionFuture != nullptr){
        delete m_SelectionFuture;
        m_SelectionFuture = nullptr;
    }
    if(m_SelectionWatcher != nullptr){
        delete m_SelectionWatcher;
        m_SelectionWatcher = nullptr;
    }

    emit selectionStarted();
    m_SelectionFuture = new QFuture<void>();
    m_SelectionWatcher = new QFutureWatcher<void>();
    *m_SelectionFuture = QtConcurrent::run(&FileSystemModel::selectFilesIf<UnaryPredicate>, this, root, pred);
    QObject::connect(m_SelectionWatcher, &QFutureWatcher<void>::finished, this, &FileSystemModel::selectionFinished);
    m_SelectionWatcher->setFuture(*m_SelectionFuture);
}

template<typename UnaryPredicate>
void FileSystemModel::selectFilesIf(QPromise<void> &promise, QModelIndex root, UnaryPredicate pred)
{
    int row = 0;
    if(pred(indexToFileTreeElement(root)))
        m_ItemSelectionModel.select(root, QItemSelectionModel::Select);
    QModelIndex element = index(row, 0, root);

    qDebug() << indexToFileTreeElement(element)->fileName();

    while(element.isValid())
    {
        if(pred(indexToFileTreeElement(element)))
        {
            if(hasChildren(element))
                selectFilesIf(promise, element, pred);
            else
                m_ItemSelectionModel.select(element, QItemSelectionModel::Select);
        }
        ++row;
        element = index(row, 0, root);
    }
}

#endif // FILESYSTEMMODEL_H
