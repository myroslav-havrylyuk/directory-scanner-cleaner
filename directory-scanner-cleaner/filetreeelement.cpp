#include "filetreeelement.h"

FileTreeElement::FileTreeElement(const QString &fileName,
                FileTreeElement *parentElement,
                QList<FileTreeElement *> childFiles)
    : m_FileName(fileName),
      m_childFiles(childFiles),
      m_ParentElement(parentElement)
{
}

FileTreeElement::~FileTreeElement()
{
    for (auto &fileTreeElement : m_childFiles)
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
    return m_childFiles.count();
}

QVariant FileTreeElement::getData(int role) const
{
    if (role < 0 || role > FileTreeElementRole::FILE_ROLES_SIZE)
        return QVariant();

    switch(role)
    {
        case FileTreeElementRole::FILE_NAME_ROLE:
            return m_FileName;
        default:
            return "Unknown role for file tree element";
    }
}

FileTreeElement *FileTreeElement::getParent() const
{
    return m_ParentElement;
}

FileTreeElement *FileTreeElement::getChildAt(int row)
{
    if (row < 0 || row > m_childFiles.count())
        return nullptr;

    return m_childFiles.at(row);
}

void FileTreeElement::setChildElements(QList<FileTreeElement *> childFiles)
{
    m_childFiles = childFiles;
}

QList<FileTreeElement *> FileTreeElement::getChildElements() const
{
    return m_childFiles;
}

const QString &FileTreeElement::fileName() const
{
    return m_FileName;
}

