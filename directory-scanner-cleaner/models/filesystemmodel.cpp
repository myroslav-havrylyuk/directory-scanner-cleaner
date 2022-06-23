#include "filesystemmodel.h"
#include <QThread>

FileSystemModel::FileSystemModel()
    : m_FileTreeRoot(nullptr)
{
    connectToFileSystemManager();
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
    // Special handler for root element(need to return itself not its childrens)
    if (!parent.isValid() && m_FileTreeRoot != nullptr)
        return createIndex(row, column, m_FileTreeRoot);

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
    FileTreeElement *childTreeElement = indexToFileTreeElement(child);
    if (childTreeElement == nullptr)
        return QModelIndex();

    FileTreeElement *parentTreeElement = childTreeElement->getParent();
    if (parentTreeElement == nullptr)
            return QModelIndex();

    int rows = parentTreeElement->getChildsCount();
    int cols = parentTreeElement->getRolesCount();
    return createIndex(rows, cols, parentTreeElement);
}

int FileSystemModel::rowCount(const QModelIndex &parent) const
{
    // Special handler for root element
    if (!parent.isValid() && m_FileTreeRoot != nullptr)
        return 1;

    FileTreeElement *fileTreeElement = indexToFileTreeElement(parent);

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
    FileTreeElement *fileTreeElement = indexToFileTreeElement(index);

    return (fileTreeElement == nullptr) ? QVariant()
                                        : fileTreeElement->getData(role);
}

bool FileSystemModel::hasChildren(const QModelIndex &parent) const
{
    FileTreeElement *fileTreeElement = indexToFileTreeElement(parent);

    return (fileTreeElement == nullptr) ? false
                                        : (fileTreeElement->getChildsCount() != 0);
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
    return m_FileTreeRoot ? m_FileTreeRoot->fileName() : QString();
}

void FileSystemModel::setupModel(const QString &rootPath)
{
    QString normalizedRootPath = QDir::cleanPath(rootPath);
    if (m_FileTreeRoot != nullptr)
    {
        if (m_FileTreeRoot->fileName() == normalizedRootPath){
            return;
        }
    }
    emit modelSetupStarted(normalizedRootPath);
    delete m_FileTreeRoot;
    m_FileTreeRoot = nullptr;
    m_FileSystemManager.generateFileTreeAsync(normalizedRootPath);
}

void FileSystemModel::connectToFileSystemManager()
{
    connect(&m_FileSystemManager, &FileSystemManager::fileTreeGenerated, this, &FileSystemModel::fileTreeGeneratedHandler);
    connect(&m_FileSystemManager, &FileSystemManager::setupModelCanceled, this, &FileSystemModel::setupModelCanceledHandler);
    connect(this, &FileSystemModel::cancelSetupModel, &m_FileSystemManager, &FileSystemManager::cancelSetupModelHandler);
}

void FileSystemModel::fileTreeGeneratedHandler(FileTreeElement *fileTreeRoot)
{
    qDebug() << "handled fileTreeGenerated signal in FileSystemModel";
    emit beginResetModel();
    m_FileTreeRoot = fileTreeRoot;
    emit endResetModel();
    emit modelSetupFinished();
}

void FileSystemModel::cancelSetupModelHandler()
{
    qDebug() << "cancel setup model handler in FileSystemModel";
    emit cancelSetupModel();
}

void FileSystemModel::setupModelCanceledHandler()
{
    emit setupModelCanceled();
    qDebug() << "setup model canceled handler in FileSystemModel";
}

FileTreeElement *FileSystemModel::indexToFileTreeElement(const QModelIndex &index) const
{
    if (!index.isValid())
        return m_FileTreeRoot;

    return static_cast<FileTreeElement *>(index.internalPointer());
}
