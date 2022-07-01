#ifndef FILESYSTEMCONTROLLER_H
#define FILESYSTEMCONTROLLER_H

#include <QObject>
#include <QQuickItem>
#include <QString>
#include <QModelIndex>

//class FileSystemModel;
#include "models/filesystemmodel.h"

class FileSystemController : public QQuickItem
{
    Q_OBJECT

    Q_PROPERTY(QString activePath READ ActivePath NOTIFY activePathChanged)
    Q_PROPERTY(QModelIndex currentlySelectedIndex READ getCurrentlySelectedIndex WRITE setCurrentlySelectedIndex NOTIFY currentlySelectedIndexChanged)
    Q_PROPERTY(bool isSelectionStateChanged MEMBER m_isSelectionStateChanged NOTIFY selectionStateChanged)
    Q_PROPERTY(QString sizeFilter READ getSizeFilter WRITE setSizeFilter NOTIFY sizeFilterChanged)

public:
    FileSystemController(FileSystemModel &fileSystemModel);

    QString ActivePath() const;
    void setActivePath(const QString &newActivePath, uint recursionDepth);
    void setCurrentlySelectedIndex(QModelIndex currentRow);
    QModelIndex getCurrentlySelectedIndex() const;
    void setSizeFilter(const QString &filterValue);
    QString getSizeFilter();
    void connectToFileSystemModel();

signals:
    void activePathInvalid();
    void activePathChanged();
    void currentlySelectedIndexChanged();
    void selectionStateChanged();
    void sizeFilterChanged();

private:
    QString m_ActivePath;
    FileSystemModel &m_FileSystemModel;
    QModelIndex m_CurrentRow;
    bool m_isSelectionStateChanged = false;
    double m_SizeFilter = 0;
    bool connectionSet = false;

public slots:
    void selectionEndedHandler();
};

#endif // FILESYSTEMCONTROLLER_H
