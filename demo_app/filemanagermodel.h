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

//        if (m_FileTreeRoot != nullptr)
//            printInnerFolders(m_FileTreeRoot);
    }

//    void printInnerFolders(FileTreeElement *rootFile)
//    {
//        qDebug() << "<<<";
//        if (rootFile == NULL)
//            return;

//        QList<FileTreeElement *> childElements = rootFile->getChildElements();
//        QString rootFileName = rootFile->FileName();
//        for (auto &fileElement : childElements)
//        {
//            QString fileElementPath = fileElement->data(FileColumn::FILE_NAME).toString();
//            QFileInfo fileInfo(fileElementPath);
//            if (fileInfo.isFile())
//            {
//                qDebug() << "File: " << fileInfo.fileName();
//            } else {
//                if (fileInfo.fileName() == "." ||
//                    fileInfo.fileName() == "..")
//                    continue;
//                qDebug() << "Folder: " << fileInfo.fileName();

//                printInnerFolders(fileElement);
//            }
//        }
//    }

    // QAbstractItemModel interface
public:
    QHash<int, QByteArray> roleNames() const
    {
        QHash<int, QByteArray> roleNames;
        roleNames[FileColumn::FILE_NAME] = "file_name";

        return roleNames;
    }

    virtual QModelIndex index(int row, int column, const QModelIndex &parent) const
    {
        Q_UNUSED(column);

        if (!hasIndex(row, column, parent))
        {
            qDebug() << "Doesnt have index";
            return QModelIndex();
        }

        FileTreeElement *fileTreeElement = nullptr;

        if (!parent.isValid() && m_FileTreeRoot != nullptr)
        {
            qDebug() << "index: Parent row: " << row << ", column: " << column;
            return createIndex(row, column, m_FileTreeRoot);
        }

        qDebug() << "index: Child row: " << row << ", column: " << column;
        fileTreeElement = indexToFileTreeElement(parent);
        if (fileTreeElement == nullptr)
        {
            qDebug() << "index: invalid fileTreeElement: return Invalid QModelIndex";
            return QModelIndex();
        }

        if (row < 0 || row > fileTreeElement->getRowsCount() ||
            column < 0 || column > fileTreeElement->getColumnsCount())
        {
            qDebug() << "index: invalid row: return Invalid QModelIndex";
            return QModelIndex();
        }

        FileTreeElement *childFileTreeElement = fileTreeElement->getChildAt(row);
        if (childFileTreeElement == nullptr)
        {
            qDebug() << "index: invalid childFileTreeElement: return Invalid QModelIndex";
            return QModelIndex();
        }

        printFileTreeElemenInfoFromIndexInfo(createIndex(row, column, childFileTreeElement));
        return createIndex(row, column, childFileTreeElement);
    }

    virtual QModelIndex parent(const QModelIndex &child) const
    {
        FileTreeElement *fileTreeElement = indexToFileTreeElement(child);
        if (fileTreeElement == nullptr)
        {
            qDebug() << "parent: invalid child to determine parent";
            return QModelIndex();
        }

        FileTreeElement *parentTreeElement = fileTreeElement->getParent();
        if (parentTreeElement == nullptr)
        {
                qDebug() << "parent: no parent for child with name: " <<
                            static_cast<FileTreeElement *>(child.internalPointer())->FileName();
                return QModelIndex();
        }

        qDebug() << "parent: Child: ";
        printFileTreeElemenInfoFromIndexInfo(child);

        qDebug() << "parent: Parent: " << parentTreeElement->FileName();

        int rows = parentTreeElement->getRowsCount();
        int cols = parentTreeElement->getColumnsCount();
        qDebug() << "parent: Parent: rows: " << rows << ", columns: " << cols;
        return createIndex(rows, cols, parentTreeElement);
    }

    virtual int rowCount(const QModelIndex &parent) const
    {
        if (!parent.isValid() && m_FileTreeRoot != nullptr)
        {
            qDebug() << "rowCount: rootElement: " << 1;
            return 1;
        }

        FileTreeElement *fileTreeElement = indexToFileTreeElement(parent);
        if (fileTreeElement == nullptr)
            return 0;

        qDebug() << "rowCount: innerElement: " << fileTreeElement->getRowsCount() << " for " <<
            fileTreeElement->FileName();
        printFileTreeElemenInfoFromIndexInfo(parent);

        return fileTreeElement->getRowsCount();
    }

    virtual int columnCount(const QModelIndex &parent) const
    {
        if (!parent.isValid() && m_FileTreeRoot != nullptr)
            return 1;

        FileTreeElement *fileTreeElement = indexToFileTreeElement(parent);
        if (fileTreeElement == nullptr)
            return 0;

        return fileTreeElement->getColumnsCount();
    }

    virtual QVariant data(const QModelIndex &index, int role) const
    {
        qDebug() << "data: row: " << index.row() << ", column: " << index.column() <<
                 ", role: " << role;
        printFileTreeElemenInfoFromIndexInfo(index);
        FileTreeElement *fileTreeElement = indexToFileTreeElement(index);
        if (fileTreeElement == nullptr)
            return QVariant();
        qDebug() << "Push data to view: " << fileTreeElement->data(role).toString();
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
            return nullptr;     // should return m_FileTreeRoot

        return static_cast<FileTreeElement *>(index.internalPointer());
    }

    void printFileTreeElemenInfoFromIndexInfo(const QModelIndex &index) const
    {
        if (!index.isValid())
        {
            qDebug() << "\tIndex is invalid!";
            return;
        }

        FileTreeElement *fileTreeElement =
            static_cast<FileTreeElement *>(index.internalPointer());
        if (fileTreeElement == nullptr)
        {
            qDebug() << "\tInvalid free tree element";
            return;
        }

        qDebug() << "\tFile tree element name: " << fileTreeElement->FileName();
    }

private:
    FileTreeElement *m_FileTreeRoot;

    // QAbstractItemModel interface
public:
    bool hasChildren(const QModelIndex &parent) const
    {
        if (!parent.isValid())
            return (m_FileTreeRoot->getRowsCount() != 0);

        FileTreeElement *fileTreeElement = indexToFileTreeElement(parent);
        if (fileTreeElement == nullptr)
            return false;

        return (fileTreeElement->getRowsCount() != 0);
    }

    bool hasIndex(int row, int column, const QModelIndex &parent = QModelIndex()) const
    {
        if (!parent.isValid() && m_FileTreeRoot != nullptr &&
            row == 0 && column == 0)
        {
            return true;
        } else {
            FileTreeElement *fileTreeElement = indexToFileTreeElement(parent);
            if (fileTreeElement == nullptr)
                return false;

            return (row >= 0 && row < fileTreeElement->getRowsCount() &&
                    column >= 0 && column < fileTreeElement->getColumnsCount());
        }
    }
};

#endif // FILEMANAGERMODEL_H
