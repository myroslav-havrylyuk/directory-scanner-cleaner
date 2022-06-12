#include "filemanagermodel.h"

FileManagerModel::FileManagerModel()
    : m_FileTreeRoot(nullptr)
{
}

FileManagerModel::FileManagerModel(const QString &rootPath)
    : FileManagerModel()
{
    if (!QFile::exists(rootPath)){
        qDebug() << "rootPath for FileManagerModel does not exist";
        return;
    }
    qDebug() << "Starting model setup";
    setupModel(rootPath);
}

QHash<int, QByteArray> FileManagerModel::roleNames() const
{
    QHash<int, QByteArray> roleNames;
    roleNames[FileTreeElementRole::FILE_NAME_ROLE] = "file_name";

    return roleNames;
}

QModelIndex FileManagerModel::index(int row, int column, const QModelIndex &parent) const
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

QModelIndex FileManagerModel::parent(const QModelIndex &child) const
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

int FileManagerModel::rowCount(const QModelIndex &parent) const
{
    // Special handler for root element
    if (!parent.isValid() && m_FileTreeRoot != nullptr)
        return 1;

    FileTreeElement *fileTreeElement = indexToFileTreeElement(parent);

    return (fileTreeElement == nullptr) ? 0
                                        : fileTreeElement->getChildsCount();
}

int FileManagerModel::columnCount(const QModelIndex &parent) const
{
    FileTreeElement *fileTreeElement = indexToFileTreeElement(parent);

    return (fileTreeElement == nullptr) ? 0
                                        : fileTreeElement->getRolesCount();
}

QVariant FileManagerModel::data(const QModelIndex &index, int role) const
{
    FileTreeElement *fileTreeElement = indexToFileTreeElement(index);

    return (fileTreeElement == nullptr) ? QVariant()
                                        : fileTreeElement->getData(role);
}

bool FileManagerModel::hasChildren(const QModelIndex &parent) const
{
    FileTreeElement *fileTreeElement = indexToFileTreeElement(parent);

    return (fileTreeElement == nullptr) ? false
                                        : (fileTreeElement->getChildsCount() != 0);
}

bool FileManagerModel::hasIndex(int row, int column, const QModelIndex &parent = QModelIndex()) const
{
    FileTreeElement *fileTreeElement = indexToFileTreeElement(parent);
    if (fileTreeElement == nullptr)
        return false;

    return (row >= 0 && row < fileTreeElement->getChildsCount() &&
            column >= 0 && column < fileTreeElement->getRolesCount());
}

void FileManagerModel::setRootPath(const QString &rootPath)
{
    if (m_FileTreeRoot != nullptr)
    {
        delete m_FileTreeRoot;
        m_FileTreeRoot = nullptr;
    }

    setupModel(rootPath);
}

QString FileManagerModel::getRootPath()
{
    return m_FileTreeRoot ? m_FileTreeRoot->fileName() : QString();
}

void FileManagerModel::setupModel(const QString &rootPath)
{
    FileManager fileManager;
    emit beginResetModel();
    m_FileTreeRoot = fileManager.generateFileTree(rootPath);
    emit endResetModel();
}

FileTreeElement *FileManagerModel::indexToFileTreeElement(const QModelIndex &index) const
{
    if (!index.isValid())
        return m_FileTreeRoot;

    return static_cast<FileTreeElement *>(index.internalPointer());
}
