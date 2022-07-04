#include "filetreeelement.h"

FileTreeElement::FileTreeElement(const QString &fileName,
                                 quint64 fileSize,
                                 QDate lastModificationDate,
                                 FileTreeElement *parentElement,
                                 QList<FileTreeElement *> childFiles)
    : m_FileName(fileName),
      m_FileSize(fileSize),
      m_LastModificationDate(lastModificationDate),
      m_ChildFiles(childFiles),
      m_ParentElement(parentElement)
{
}

FileTreeElement::~FileTreeElement()
{
    for (auto &fileTreeElement : m_ChildFiles)
    {
        delete fileTreeElement;
    }
}

int FileTreeElement::getRolesCount() const
{
    return (FILE_ROLES_SIZE - FILE_NAME_ROLE);
}

int FileTreeElement::getChildsCount() const
{
    return m_ChildFiles.count();
}

QVariant FileTreeElement::getData(int role) const
{
    if (role < 0 || role > FileTreeElementRole::FILE_ROLES_SIZE)
        return QVariant();

    switch(role)
    {
    case FileTreeElementRole::FILE_NAME_ROLE:
        return m_FileName;
    case FileTreeElementRole::FILE_INNER_FILES_ROLE:
        return this->formattedInnerFilesCount();
    case FileTreeElementRole::FILE_SIZE_ROLE:
        return this->formattedSize();
    default:
        return "Unknown role for file tree element";
    }
}

quint64 FileTreeElement::getFileSize() const
{
    return m_FileSize;
}

void FileTreeElement::setFileSize(quint64 fileSize)
{
    m_FileSize = fileSize;
}

QString FileTreeElement::formattedSize() const
{
    QLocale locale;
    return locale.formattedDataSize(m_FileSize, 2, QLocale::DataSizeTraditionalFormat);
}

FileTreeElement *FileTreeElement::getParent() const
{
    return m_ParentElement;
}

FileTreeElement *FileTreeElement::getChildAt(int row)
{
    if (row < 0 || row > m_ChildFiles.count())
        return nullptr;

    return m_ChildFiles.at(row);
}

void FileTreeElement::setChildElements(QList<FileTreeElement *> childFiles)
{
    m_ChildFiles = childFiles;
}

void FileTreeElement::appendChild(FileTreeElement *element)
{
    if (element == nullptr)
        return;

    m_ChildFiles.append(element);
}

QList<FileTreeElement *> FileTreeElement::getChildElements() const
{
    return m_ChildFiles;
}

const QString &FileTreeElement::fileName() const
{
    return m_FileName;
}

void FileTreeElement::setLastModificationDate(QDate lastModificationDate)
{
    m_LastModificationDate = lastModificationDate;
}

QDate FileTreeElement::getLastModificationDate()
{
    return m_LastModificationDate;
}

int FileTreeElement::row() const
{
    if (m_ParentElement == nullptr)
        return 0;

    return m_ParentElement->getChildElements().indexOf(this);
}

QString FileTreeElement::getPathPrefix() const
{
    FileTreeElement *parentElement = m_ParentElement;
    QString filePrefix = "";

    while (parentElement->m_ParentElement)
    {
        filePrefix.prepend(parentElement->fileName() + "/");
        parentElement = parentElement->m_ParentElement;
    }

    return filePrefix;
}

void FileTreeElement::removeChildAt(int index)
{
    if (index >= 0 && index < m_ChildFiles.size())
        m_ChildFiles.removeAt(index);

    m_InnerFilesCount -= 1;
}

bool FileTreeElement::hasChildElements() const
{
    return m_ChildFiles.size();
}

void FileTreeElement::setAbsoluteFilePath(QString path)
{
    m_AbsoluteFilePath = path;
    setInnerFilesCount();
}

QString FileTreeElement::getAbsoluteFilePath() const
{
    return m_AbsoluteFilePath;
}

void FileTreeElement::setInnerFilesCount()
{
    if(QFileInfo(m_AbsoluteFilePath).isDir())
        m_InnerFilesCount = QDir(m_AbsoluteFilePath).entryInfoList().size() - 2;
}

uint FileTreeElement::getInnerFilesCount() const
{
    return m_InnerFilesCount;
}

QString FileTreeElement::formattedInnerFilesCount() const
{
    return QFileInfo(m_AbsoluteFilePath).isFile() ? "" : QVariant(m_InnerFilesCount).toString();
}
