#include "filesystemmodel.h"
#include <QThread>

FileSystemModel::FileSystemModel()
    : m_FileTreeRoot(nullptr),
      m_FilesystemRootElement(nullptr)
{
    m_ItemSelectionModel.setModel(this);
}

//Probably should not be used, as it may cause unhandled signals.
//Instead, use setActivePath() function of FileSystemController class after connecting to
//signals of this class.
/*FileSystemModel::FileSystemModel(const QString &rootPath)
{
    connectToFileSystemManager();
    QString normalizedRootPath = QDir::cleanPath(rootPath);
    if (!QFile::exists(normalizedRootPath)){
        qDebug() << "rootPath for FileSystemModel does not exist";
        return;
    }
    setupModel(rootPath);
}*/

FileSystemModel::~FileSystemModel()
{
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
    FileTreeElement *childFileTreeElement = fileTreeElement->getChildAt(row);
    if (childFileTreeElement == nullptr)
        return QModelIndex();

    return createIndex(row, column, childFileTreeElement);
}

QModelIndex FileSystemModel::parent(const QModelIndex &child) const
{
    if (!child.isValid())
        return QModelIndex();

    FileTreeElement *childTreeElement = indexToFileTreeElement(child);
    if (childTreeElement == nullptr)
        return QModelIndex();

    FileTreeElement *parentTreeElement = childTreeElement->getParent();
    if (parentTreeElement == nullptr || parentTreeElement == m_FileTreeRoot)
            return QModelIndex();

    int rows = parentTreeElement->getChildsCount();
    return createIndex(rows, 0, parentTreeElement);
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

void FileSystemModel::selectFile(QModelIndex index)
{
    m_ItemSelectionModel.select(index, QItemSelectionModel::Toggle);
}

void FileSystemModel::setupModel(const QString &rootPath)
{
    QString normalizedRootPath = QDir::cleanPath(rootPath);
    emit modelSetupStarted(normalizedRootPath);
    if (m_FileTreeRoot != nullptr)
    {
        /*if (m_FileTreeRoot->fileName() == normalizedRootPath){
            return;
        }*/
        delete m_FileTreeRoot;
        m_FileTreeRoot = nullptr;
    }
    if(m_FileSystemManager != nullptr){
        delete m_FileSystemManager;
        m_FileSystemManager = nullptr;
    }
    m_FileSystemManager = new FileSystemManager();
    connectToFileSystemManager();
    m_FileSystemManager->generateFileTreeAsync(normalizedRootPath);
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

void FileSystemModel::fileTreeGeneratedHandler(FileTreeElement *fileTreeRoot)
{
    qDebug() << QTime::currentTime() << "handled fileTreeGenerated signal in FileSystemModel";
    emit beginResetModel();
    m_FileTreeRoot = fileTreeRoot;
    if (m_FileTreeRoot != nullptr)
        m_FilesystemRootElement = m_FileTreeRoot->getChildElements().front();

    m_ItemSelectionModel.clearSelection();

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
