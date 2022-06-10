#ifndef FILETREEELEMENT_H
#define FILETREEELEMENT_H

#include <QVariant>
#include <QList>
#include <QDir>

enum FileColumn {
    FILE_NAME = 0,
    FILE_COLUMNS_SIZE
};

class FileTreeElement
{
public:
    FileTreeElement(const QString &fileName, FileTreeElement *parentElement, QList<FileTreeElement *> childFiles = {})
        : m_FileName(fileName),
          m_childFiles(childFiles),
          m_ParentElement(parentElement)
    {

    }

    int getColumnsCount() const
    {
        return FileColumn::FILE_COLUMNS_SIZE;
    }

    int getRowsCount() const {
        return m_childFiles.count();
    }

    QVariant data(int column) const
    {
        if (column < 0 || column > FileColumn::FILE_COLUMNS_SIZE ||
                !QFile::exists(m_FileName))
        {
            return QVariant();
        }

        switch(column)
        {
            case FileColumn::FILE_NAME:
                return m_FileName;
            default:
                return "Unknown file column";
        }
    }

    FileTreeElement *getParent()
    {
        return m_ParentElement;
    }

    FileTreeElement *getChildAt(int row)
    {
        if (row < 0 || row > m_childFiles.count())
            return nullptr;

        return m_childFiles.at(row);
    }

    void setChildElements(QList<FileTreeElement *> childFiles)
    {
        m_childFiles = childFiles;
    }

    QList<FileTreeElement *> getChildElements() const
    {
        return m_childFiles;
    }

private:
    QString m_FileName;
    QList<FileTreeElement *> m_childFiles;
    FileTreeElement *m_ParentElement;
};

#endif // FILETREEELEMENT_H
