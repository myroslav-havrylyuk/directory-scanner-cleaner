#ifndef FILESYSTEMCONTROLLER_H
#define FILESYSTEMCONTROLLER_H

#include <QObject>
#include <QQuickItem>
#include <QString>
#include <QModelIndex>

#include "models/filesystemmodel.h"

class FileSystemController : public QQuickItem
{
    Q_OBJECT

    Q_PROPERTY(QString activePath READ ActivePath WRITE setActivePath NOTIFY activePathChanged)
    Q_PROPERTY(bool selected READ getSelected NOTIFY selectedChanged)
    Q_PROPERTY(QModelIndex currentlySelectedIndex READ getCurrentlySelectedIndex WRITE setCurrentlySelectedIndex NOTIFY currentlySelectedIndexChanged)

public:
    FileSystemController(FileSystemModel &fileSystemModel);

    void setActivePath(const QString &newActivePath);
    QString ActivePath() const;

    void setCurrentlySelectedIndex(QModelIndex currentRow);
    QModelIndex getCurrentlySelectedIndex() const;
    bool getSelected() const;

signals:
    void activePathInvalid();
    void activePathChanged();
    void currentlySelectedIndexChanged();
    void selectedChanged();


private:
    QString m_ActivePath;
    FileSystemModel &m_FileSystemModel;
    QObject *m_warningMessage;
    QModelIndex m_CurrentRow;
    bool selected = false;
};

#endif // FILESYSTEMCONTROLLER_H
