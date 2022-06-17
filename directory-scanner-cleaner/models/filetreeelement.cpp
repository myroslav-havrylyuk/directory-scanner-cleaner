#include "filetreeelement.h"

FileTreeElement::FileTreeElement(const QString &fileName,
                quint64 fileSize,
                FileTreeElement *parentElement,
                QList<FileTreeElement *> childFiles)
    : m_FileName(fileName),
      m_FileSize(fileSize),
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
            return this->getChildsCount();
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

QList<FileTreeElement *> FileTreeElement::getChildElements() const
{
    return m_ChildFiles;
}

const QString &FileTreeElement::fileName() const
{
    return m_FileName;
}

