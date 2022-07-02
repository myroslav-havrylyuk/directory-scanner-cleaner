#include "filesystemcontroller.h"
#include "models/filesystemmodel.h"

#include <QQmlApplicationEngine>
#include <QRegularExpression>
#include <QQmlEngine>
#include <QDir>
#include <QQuickView>

FileSystemController::FileSystemController(FileSystemModel &fileSystemModel)
    : m_FileSystemModel(fileSystemModel)
{
    connectToFileSystemModel();
}

void FileSystemController::setActivePath(const QString &newActivePath, uint recursionDepth)
{
    qDebug() << "setActivePath function has been called";
    QString validActivePath = newActivePath;
    validActivePath.remove(QRegularExpression("file:///"));
    validActivePath = QDir::cleanPath(validActivePath);
    //better ask user if he really want to scan the same directory again, as if
    //he canceled scanning last time, he wouldn't be able to scan this directory
    //again because of this check
    //if(m_ActivePath != validActivePath){
        qDebug() << "New active path has been set: " << newActivePath;
        qDebug() << "Edited active path has been set: " << validActivePath;

        QDir activePath(validActivePath);
        if (activePath.exists())
        {
            m_ActivePath = validActivePath;
            m_FileSystemModel.setupModel(m_ActivePath, recursionDepth);
            emit activePathChanged();
        } else {
            qDebug() << "invalid path has been entered";
            emit activePathInvalid();
        }
}

void FileSystemController::setCurrentlySelectedIndex(QModelIndex currentRow) {
    if (!currentRow.isValid())
        return;

    m_CurrentRow = currentRow;
    emit currentlySelectedIndexChanged();

    m_FileSystemModel.selectFile(m_CurrentRow);

    m_isSelectionStateChanged = true;
    emit selectionStateChanged();
}

QModelIndex FileSystemController::getCurrentlySelectedIndex() const {
    return m_CurrentRow;
}

QString FileSystemController::ActivePath() const
{
    return QDir::toNativeSeparators(m_ActivePath);
}

void FileSystemController::setSizeFilter(const QString &filterValue)
{
    if(!filterValue.isEmpty())
    {
        m_SizeFilter = QVariant(filterValue).toDouble() < 0.001 ? 0 : QVariant(filterValue).toDouble();
        emit sizeFilterChanged();

        if(!m_ActivePath.isEmpty())
        {
            quint64 value = m_SizeFilter * 1024 * 1024;
            m_FileSystemModel.selectFilesIfAsync([value](FileTreeElement* x){ return x->getFileSize() > value; });
        }
    }
}

void FileSystemController::connectToFileSystemModel()
{
    QObject::connect(&m_FileSystemModel, &FileSystemModel::selectionFinished, this, &FileSystemController::selectionEndedHandler);
}

void FileSystemController::selectionEndedHandler()
{
    qDebug() << QTime::currentTime() << "handled selectionEnded signal in FileSystemModel";

    m_isSelectionStateChanged = true;
    emit selectionStateChanged();
}

QString FileSystemController::getSizeFilter()
{
    return QVariant(m_SizeFilter).toString();
}
