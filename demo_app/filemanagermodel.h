#ifndef FILEMANAGERMODEL_H
#define FILEMANAGERMODEL_H

#include <QAbstractItemModel>
#include <QFileInfo>
#include <QDir>

#include "filetreeelement.h"
#include "filemanager.h"

class FileManagerModel : public QAbstractItemModel
{
public:
    FileManagerModel();
    FileManagerModel(const QString &rootPath)
        : FileManagerModel()
    {
        if (!QFile::exists(rootPath))
        {
            m_FileTreeRoot = nullptr;
            return;
        }

        setupModel(rootPath);

        if (m_FileTreeRoot != nullptr)
            printInnerFolders(m_FileTreeRoot);
    }

    void printInnerFolders(FileTreeElement *rootFile)
    {
        qDebug() << "<<<";
        if (rootFile == NULL)
            return;

        QList<FileTreeElement *> childElements = rootFile->getChildElements();
        for (auto &fileElement : childElements)
        {
            QString fileElementPath = fileElement->data(FileColumn::FILE_NAME).toString();
            QFileInfo fileInfo(fileElementPath);
            if (fileInfo.isFile())
            {
                qDebug() << "File: " << fileInfo.fileName();
            } else {
                if (fileInfo.fileName() == "." ||
                    fileInfo.fileName() == "..")
                    continue;
                qDebug() << "Folder: " << fileInfo.fileName();

                printInnerFolders(fileElement);
            }
        }
    }

    // QAbstractItemModel interface
public:
    virtual QModelIndex index(int row, int column, const QModelIndex &parent) const
    {
        Q_UNUSED(column);

        FileTreeElement *fileTreeElement = nullptr;

        if (!parent.isValid() && m_FileTreeRoot != nullptr)
        {
            fileTreeElement = m_FileTreeRoot;
        } else {
            fileTreeElement = indexToFileTreeElement(parent);
        }

        if (fileTreeElement == nullptr)
            return QModelIndex();

        if (row < 0 || row > fileTreeElement->getRowsCount())
            return QModelIndex();

        int rows = fileTreeElement->getRowsCount();
        int cols = fileTreeElement->getColumnsCount();
        FileTreeElement *childFileTreeElement = fileTreeElement->getChildAt(row);
        return createIndex(rows, cols, childFileTreeElement);
    }

    virtual QModelIndex parent(const QModelIndex &child) const
    {
        FileTreeElement *fileTreeElement = indexToFileTreeElement(child);
        if (fileTreeElement == nullptr)
            return QModelIndex();

        FileTreeElement *parentTreeElement = fileTreeElement->getParent();
        if (parentTreeElement == nullptr)
            return QModelIndex();

        int rows = parentTreeElement->getRowsCount();
        int cols = parentTreeElement->getColumnsCount();
        return createIndex(rows, cols, parentTreeElement);
    }

    virtual int rowCount(const QModelIndex &parent) const
    {
        FileTreeElement *fileTreeElement = indexToFileTreeElement(parent);
        if (fileTreeElement == nullptr)
            return 0;

        return fileTreeElement->getRowsCount();
    }

    virtual int columnCount(const QModelIndex &parent) const
    {
        Q_UNUSED(parent);

        return FileColumn::FILE_COLUMNS_SIZE;
    }

    virtual QVariant data(const QModelIndex &index, int role) const
    {
        FileTreeElement *fileTreeElement = indexToFileTreeElement(index);
        if (fileTreeElement == nullptr)
            return QVariant();

        return fileTreeElement->data(role);
    }

    void setupModel(const QString &rootPath)
    {
        FileManager fileManager;
        m_FileTreeRoot = fileManager.generateFileTree(rootPath);
    }

private:
    FileTreeElement *indexToFileTreeElement(const QModelIndex &index) const
    {
        if (!index.isValid())
            return nullptr;

        return static_cast<FileTreeElement *>(index.internalPointer());
    }

private:
    FileTreeElement *m_FileTreeRoot;
};

#endif // FILEMANAGERMODEL_H
