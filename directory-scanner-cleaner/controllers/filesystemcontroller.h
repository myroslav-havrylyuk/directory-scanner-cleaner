#ifndef FILESYSTEMCONTROLLER_H
#define FILESYSTEMCONTROLLER_H

#include <QModelIndex>
#include <QObject>
#include <QQuickItem>
#include <QString>

#include "models/filesystemmodel.h"

class FileSystemController : public QQuickItem {
    Q_OBJECT

    Q_PROPERTY(QString activePath READ ActivePath NOTIFY activePathChanged)
    Q_PROPERTY(
            QModelIndex currentlySelectedIndex READ getCurrentlySelectedIndex WRITE
            setCurrentlySelectedIndex NOTIFY currentlySelectedIndexChanged)
    Q_PROPERTY(bool isSelectionStateChanged MEMBER m_isSelectionStateChanged
               NOTIFY selectionStateChanged)
    Q_PROPERTY(QString sizeFilter READ getSizeFilter WRITE setSizeFilter NOTIFY
               sizeFilterChanged)
    Q_PROPERTY(
            QString daysAfterModificationFilter READ getDaysAfterModificationFilter
            WRITE setDaysAfterModificationFilter NOTIFY
            daysAfterModificationFilterChanged)

public:
    FileSystemController(FileSystemModel &fileSystemModel);
    ~FileSystemController();

    QString ActivePath() const;
    void setActivePath(const QString &newActivePath, uint recursionDepth);
    void setCurrentlySelectedIndex(QModelIndex currentRow);
    QModelIndex getCurrentlySelectedIndex() const;
    void setSizeFilter(const QString &filterValue);
    QString getSizeFilter();
    void setDaysAfterModificationFilter(const QString &filterValue);
    QString getDaysAfterModificationFilter();
    void connectToFileSystemModel();
    void selectBySize();
    void selectByDate();

signals:
    void activePathInvalid();
    void activePathChanged();
    void currentlySelectedIndexChanged();
    void selectionStateChanged();
    void sizeFilterChanged();
    void daysAfterModificationFilterChanged();

private:
    QString m_ActivePath;
    FileSystemModel &m_FileSystemModel;
    QModelIndex m_CurrentRow;
    bool m_isSelectionStateChanged = false;
    double m_SizeFilter = 0;
    int m_DaysAfterModificationFilter = 0;

public slots:
    void selectionBySizeEndedHandler();
    void selectionByDateEndedHandler();
    void selectByFilter();
    void deselectionEndedHandler();
    void deselectFiles();
};

#endif // FILESYSTEMCONTROLLER_H
