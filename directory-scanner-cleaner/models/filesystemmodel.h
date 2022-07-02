#ifndef FILESYSTEMMODEL_H
#define FILESYSTEMMODEL_H

#include <QItemSelectionModel>
#include <QAbstractItemModel>
#include <QFileInfo>
#include <QDir>
#include <QMutex>
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
    void selectFilesBySizeIfAsync(UnaryPredicate pred);
    template<typename UnaryPredicate>
    void selectFilesByDateIfAsync(UnaryPredicate pred);

private:
    QItemSelectionModel m_ItemSelectionModel;
    FileSystemManager *m_FileSystemManager = nullptr;
    FileTreeElement *indexToFileTreeElement(const QModelIndex &index) const;
    FileTreeElement *m_FilesystemRootElement;
    FileTreeElement *m_FileTreeRoot;
    void connectToFileSystemManager();
    QItemSelectionModel *getItemSelectionModel();

    QFutureWatcher<void> *m_SelectionBySizeWatcher = nullptr;
    QFuture<void> *m_SelectionBySizeFuture = nullptr;
    QFutureWatcher<void> *m_SelectionByDateWatcher = nullptr;
    QFuture<void> *m_SelectionByDateFuture = nullptr;

    QMutex mutex;

signals:
    void modelSetupStarted(const QString &rootPath);
    void modelSetupFinished();
    void cancelSetupModel();
    void setupModelCanceled();
    void itemSelectionModelChanged();

    void selectionBySizeStarted();
    void selectionBySizeFinished();
    void selectionByDateStarted();
    void selectionByDateFinished();

public slots:
    void fileTreeGeneratedHandler(FileTreeElement * fileTreeRoot);
    void cancelSetupModelHandler();
    void setupModelCanceledHandler();
};

template<typename UnaryPredicate>
void FileSystemModel::selectFilesByDateIfAsync(UnaryPredicate pred)
{
    QModelIndex root = createIndex(0,0, m_FilesystemRootElement);

    if(m_SelectionByDateFuture != nullptr){
        delete m_SelectionByDateFuture;
        m_SelectionByDateFuture = nullptr;
    }
    if(m_SelectionByDateWatcher != nullptr){
        delete m_SelectionByDateWatcher;
        m_SelectionByDateWatcher = nullptr;
    }

    emit selectionByDateStarted();
    m_SelectionByDateFuture = new QFuture<void>();
    m_SelectionByDateWatcher = new QFutureWatcher<void>();
    *m_SelectionByDateFuture = QtConcurrent::run(&FileSystemModel::selectFilesIf<UnaryPredicate>, this, root, pred);
    QObject::connect(m_SelectionByDateWatcher, &QFutureWatcher<void>::finished, this, &FileSystemModel::selectionByDateFinished);
    m_SelectionByDateWatcher->setFuture(*m_SelectionByDateFuture);
}

template<typename UnaryPredicate>
void FileSystemModel::selectFilesBySizeIfAsync(UnaryPredicate pred)
{
    QModelIndex root = createIndex(0,0, m_FilesystemRootElement);

    if(m_SelectionBySizeFuture != nullptr){
        delete m_SelectionBySizeFuture;
        m_SelectionBySizeFuture = nullptr;
    }
    if(m_SelectionBySizeWatcher != nullptr){
        delete m_SelectionBySizeWatcher;
        m_SelectionBySizeWatcher = nullptr;
    }

    emit selectionBySizeStarted();
    m_SelectionBySizeFuture = new QFuture<void>();
    m_SelectionBySizeWatcher = new QFutureWatcher<void>();
    *m_SelectionBySizeFuture = QtConcurrent::run(&FileSystemModel::selectFilesIf<UnaryPredicate>, this, root, pred);
    QObject::connect(m_SelectionBySizeWatcher, &QFutureWatcher<void>::finished, this, &FileSystemModel::selectionBySizeFinished);
    m_SelectionBySizeWatcher->setFuture(*m_SelectionBySizeFuture);


}

template<typename UnaryPredicate>
void FileSystemModel::selectFilesIf(QPromise<void> &promise, QModelIndex root, UnaryPredicate pred)
{
    int row = 0;

    if(pred(indexToFileTreeElement(root)))
    {
        mutex.lock();
        m_ItemSelectionModel.select(root, QItemSelectionModel::Select);
        mutex.unlock();
    }

    QModelIndex element = index(row, 0, root);

    while(element.isValid())
    {
        if(pred(indexToFileTreeElement(element)))
        {
            if(hasChildren(element))
                selectFilesIf(promise, element, pred);
            else
            {
                mutex.lock();
                m_ItemSelectionModel.select(element, QItemSelectionModel::Select);
                mutex.unlock();
            }
        }
        ++row;
        element = index(row, 0, root);
    }
}

#endif // FILESYSTEMMODEL_H
