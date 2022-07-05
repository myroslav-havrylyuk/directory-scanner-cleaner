#include "filesystemmodel.h"
#include "tools/filetreereversebreadthiterator.h"

FileSystemModel::FileSystemModel()
    : m_FileTreeRoot(nullptr),
      m_FilesystemRootElement(nullptr),
      m_DeletionReasonsStringModel(this)
{
    m_ItemSelectionModel.setModel(this);

    m_DeletionReasons[DeletionReason::FILE_SIZE] = "File size";
    m_DeletionReasons[DeletionReason::FILE_OLDNESS] = "File oldness";
    m_DeletionReasons[DeletionReason::OTHER] = "Other";

    m_DeletionReasonsStringModel.setStringList(m_DeletionReasons.values());

    QObject::connect(&m_DeleteFilesFutureWatcher, &QFutureWatcher< QList<QString> >::finished, this, &FileSystemModel::handleDeleteSelectedFilesFinished);
}

FileSystemModel::~FileSystemModel()
{
    if (m_FileSystemManager != nullptr)
        delete m_FileSystemManager;
    if (m_FilesystemRootElement != nullptr)
        delete m_FilesystemRootElement;
    if (m_FileTreeRoot != nullptr)
        delete m_FileTreeRoot;
    if (m_SelectionBySizeWatcher != nullptr)
        delete m_SelectionBySizeWatcher;
    if (m_SelectionBySizeFuture != nullptr)
        delete m_SelectionBySizeFuture;
    if (m_SelectionByDateWatcher != nullptr)
        delete m_SelectionByDateWatcher;
    if (m_SelectionByDateFuture != nullptr)
        delete m_SelectionByDateFuture;
    if (m_FileTreeRoot != nullptr)
        delete m_FileTreeRoot;
    if (m_FileTreeRoot != nullptr)
        delete m_FileTreeRoot;
    if (m_FileTreeRoot != nullptr)
        delete m_FileTreeRoot;
}

QHash<int, QByteArray> FileSystemModel::roleNames() const
{
    QHash<int, QByteArray> roleNames;
    roleNames[FileTreeElementRole::FILE_NAME_ROLE] = "file_name";
    roleNames[FileTreeElementRole::FILE_INNER_FILES_ROLE] = "inner_files";
    roleNames[FileTreeElementRole::FILE_SIZE_ROLE] = "file_size";

    return roleNames;
}

QModelIndex FileSystemModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    FileTreeElement *fileTreeElement = indexToFileTreeElement(parent);
    if (fileTreeElement == nullptr)
        return QModelIndex();

    FileTreeElement *childFileTreeElement = fileTreeElement->getChildAt(row);
    if (childFileTreeElement == nullptr)
        return QModelIndex();

    return createIndex(row, column, childFileTreeElement);
}

QModelIndex FileSystemModel::parent(const QModelIndex &child) const
{
    if (!child.isValid())
        return QModelIndex();

    FileTreeElement *childTreeElement = static_cast<FileTreeElement *>(child.internalPointer());
    if (childTreeElement == nullptr)
        return QModelIndex();

    FileTreeElement *parentTreeElement = childTreeElement->getParent();
    if (parentTreeElement == nullptr || parentTreeElement == m_FileTreeRoot)
        return QModelIndex();

    return createIndex(parentTreeElement->row(), 0, parentTreeElement);
}

int FileSystemModel::rowCount(const QModelIndex &parent) const
{
    // Only element with column #1 should have child elements
    if (parent.column() > 0)
        return 0;
    // Special handler for root element
    FileTreeElement *fileTreeElement = nullptr;
    if (!parent.isValid() && m_FileTreeRoot != nullptr)
        fileTreeElement = m_FileTreeRoot;
    else
        fileTreeElement = indexToFileTreeElement(parent);

    return (fileTreeElement == nullptr) ? 0
                                        : fileTreeElement->getChildsCount();
}

int FileSystemModel::columnCount(const QModelIndex &parent) const
{
    FileTreeElement *fileTreeElement = indexToFileTreeElement(parent);

    return (fileTreeElement == nullptr) ? 0
                                        : fileTreeElement->getRolesCount();
}

QVariant FileSystemModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    FileTreeElement *fileTreeElement = indexToFileTreeElement(index);

    return (fileTreeElement == nullptr) ? QVariant()
                                        : fileTreeElement->getData(role);
}

bool FileSystemModel::removeRow(int row, const QModelIndex &parent)
{
    FileTreeElement *parentItem = indexToFileTreeElement(parent);
    if (!parentItem)
        return false;

    beginRemoveRows(parent, row, row);
    parentItem->removeChildAt(row);
    endRemoveRows();

    while ( (parentItem = parentItem->getParent()) != nullptr)
        emit dataChanged(createIndex(0, 0, parentItem), createIndex(0, 0, parentItem));

    return true;
}

bool FileSystemModel::hasChildren(const QModelIndex &parent) const
{

    FileTreeElement *fileTreeElement = indexToFileTreeElement(parent);

    return (fileTreeElement == nullptr) ? false
                                        : (!fileTreeElement->getChildElements().empty());
}

bool FileSystemModel::hasIndex(int row, int column, const QModelIndex &parent = QModelIndex()) const
{
    FileTreeElement *fileTreeElement = indexToFileTreeElement(parent);
    if (fileTreeElement == nullptr)
        return false;

    return (row >= 0 && row < fileTreeElement->getChildsCount() &&
            column >= 0 && column < fileTreeElement->getRolesCount());
}

QString FileSystemModel::getRootPath()
{
    return m_FilesystemRootElement ? m_FilesystemRootElement->fileName() : QString();
}

void FileSystemModel::selectFileManually(QModelIndex index)
{
    m_DeletionReasonsStringModel.
            setInitialDeletionReason(m_DeletionReasons[DeletionReason::OTHER]);

    m_ItemSelectionModel.select(index, QItemSelectionModel::Toggle);
}

FileSystemManager *FileSystemModel::getFileSystemManager() const
{
    return m_FileSystemManager;
}

void FileSystemModel::setupModel(const QString &rootPath, uint recursionDepth)
{
    QString normalizedRootPath = QDir::cleanPath(rootPath);
    emit modelSetupStarted(normalizedRootPath);
    if (m_FileTreeRoot != nullptr)
    {
        delete m_FileTreeRoot;
        m_FileTreeRoot = nullptr;
    }
    if(m_FileSystemManager != nullptr){
        delete m_FileSystemManager;
        m_FileSystemManager = nullptr;
    }
    m_FileSystemManager = new FileSystemManager();
    connectToFileSystemManager();
    m_FileSystemManager->generateFileTreeAsync(normalizedRootPath, recursionDepth);
}

void FileSystemModel::connectToFileSystemManager()
{
    connect(m_FileSystemManager, &FileSystemManager::fileTreeGenerated, this, &FileSystemModel::fileTreeGeneratedHandler);
    connect(m_FileSystemManager, &FileSystemManager::setupModelCanceled, this, &FileSystemModel::setupModelCanceledHandler);
    connect(this, &FileSystemModel::cancelSetupModel, m_FileSystemManager, &FileSystemManager::cancelSetupModelHandler);
}

QItemSelectionModel *FileSystemModel::getItemSelectionModel() {
    return &m_ItemSelectionModel;
}

DeletionReasonsStringModel *FileSystemModel::getDeletionReasonsStringModel()
{
    return &m_DeletionReasonsStringModel;
}

void FileSystemModel::fileTreeGeneratedHandler(FileTreeElement *fileTreeRoot)
{
    qDebug() << QTime::currentTime() << "handled fileTreeGenerated signal in FileSystemModel";
    emit beginResetModel();
    m_FileTreeRoot = fileTreeRoot;
    if (m_FileTreeRoot != nullptr)
        m_FilesystemRootElement = m_FileTreeRoot->getChildElements().front();

    //m_ItemSelectionModel.clearSelection();

    emit endResetModel();
    emit modelSetupFinished();
}

void FileSystemModel::cancelSetupModelHandler()
{
    qDebug() << QTime::currentTime() <<"FileSystemModel: cancel setup model handler";
    emit cancelSetupModel();
}

void FileSystemModel::setupModelCanceledHandler()
{
    emit setupModelCanceled();
    qDebug() << QTime::currentTime() <<"setup model canceled handler in FileSystemModel";
}

FileTreeElement *FileSystemModel::indexToFileTreeElement(const QModelIndex &index) const
{
    if (!index.isValid())
        return m_FileTreeRoot;

    return static_cast<FileTreeElement *>(index.internalPointer());
}

void FileSystemModel::handleDeleteSelectedFilesFinished()
{
    emit fileDeletionFinished();
}

void FileSystemModel::cancelDeletionOfSelectedFiles()
{
    m_DeleteFilesFuture.cancel();
}

void FileSystemModel::mockHandler()
{
}

void FileSystemModel::deleteSelectedFiles()
{
    if (m_FileSystemManager == nullptr)
        return;

    // Fixes unpecified signal catching for QAbstractItemModel which is not thread-safe by default.
    // If bad timing occurs(which happens pretty often) we will end up with invalid tree view displayed.
    // Sole purpose of that connect is to ensure handling of signal emmited by beginRemoveRows() in UI thread
    // before the next line executes which deletes data from model.
    connect(this, &QAbstractItemModel::rowsAboutToBeRemoved,
            this, &FileSystemModel::mockHandler,
            static_cast<Qt::ConnectionType>(Qt::BlockingQueuedConnection | Qt::UniqueConnection));

    m_DeleteFilesFuture = QtConcurrent::run(QOverload<QPromise<QList<QString>> &>::of(&FileSystemModel::deleteSelectedFiles), this);
    m_DeleteFilesFutureWatcher.setFuture(m_DeleteFilesFuture);
}

void FileSystemModel::deleteSelectedFiles(QPromise<QList<QString> > &promise)
{
    QList<QString> deletedFilenames;

    if (!m_ItemSelectionModel.hasSelection())
        return;

    emit fileDeletionStarted();

    while (!m_ItemSelectionModel.selectedIndexes().empty()) {
        QModelIndex selectedIndex = m_ItemSelectionModel.selectedIndexes().front();
        if (!selectedIndex.isValid())
            continue;

        FileTreeElement *selectedTreeElement =
                static_cast<FileTreeElement *>(selectedIndex.internalPointer());
        if (selectedTreeElement == nullptr)
            continue;

        FileTreeElementReverseBreadthIterator it(selectedTreeElement);

        while (it.hasNext())
        {
            FileTreeElement *fileElementToDelete = it.next();

            QModelIndex fileElementToDeleteModelIndex =
                    createIndex(fileElementToDelete->row(), 0, fileElementToDelete);

            // Reverse iterator first try to delete all of the inner files in the directory.
            // When we iterate right to the holding folder and find it empty we can just simply delete it.
            // In case it still contains some files that means some its inner files` deletion operation failed.
            if (fileElementToDelete->hasChildElements())
                continue;

            QString fileElementAbsolutePath =
                    fileElementToDelete->getAbsoluteFilePath();

            if (promise.isCanceled())
            {
                // Maybe we can even omit opening "Waiting for cancelation" window
                // as canceling operation perfrorms pretty fast anyways
                emit fileDeletionFinished(deletedFilenames, m_DeletionReasonsStringModel.getActiveDeletionReason());
                emit fileDeletionCancelingOperationFinished();
                return;
            }

            m_ItemSelectionModel.select(fileElementToDeleteModelIndex, QItemSelectionModel::Deselect);

            if (m_FileSystemManager->deleteFile(fileElementAbsolutePath))
            {
                deletedFilenames.append(fileElementAbsolutePath);

                removeRow(fileElementToDeleteModelIndex.row(), fileElementToDeleteModelIndex.parent());
            }
        }
    }

    emit fileDeletionFinished(deletedFilenames, m_DeletionReasonsStringModel.getActiveDeletionReason());
}

void FileSystemModel::deselectFilesAsync()
{
    if(m_DeselectionFuture != nullptr){
        delete m_DeselectionFuture;
        m_DeselectionFuture = nullptr;
    }
    if(m_DeselectionWatcher != nullptr){
        delete m_DeselectionWatcher;
        m_DeselectionWatcher = nullptr;
    }

    emit deselectionStarted();
    m_DeselectionFuture = new QFuture<void>();
    m_DeselectionWatcher = new QFutureWatcher<void>();
    *m_DeselectionFuture = QtConcurrent::run(&FileSystemModel::deselectFiles, this);
    QObject::connect(m_DeselectionWatcher, &QFutureWatcher<void>::finished, this, &FileSystemModel::deselectionFinished);
    m_DeselectionWatcher->setFuture(*m_DeselectionFuture);
}

void FileSystemModel::deselectFiles(QPromise<void> &promise)
{
    for(auto element : m_ItemSelectionModel.selectedIndexes())
    {
        m_ItemSelectionModel.select(element, QItemSelectionModel::Deselect);
    }
}
