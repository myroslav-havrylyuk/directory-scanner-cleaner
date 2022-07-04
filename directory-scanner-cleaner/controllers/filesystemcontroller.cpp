#include "filesystemcontroller.h"
#include "models/filesystemmodel.h"

#include <QDir>
#include <QQmlApplicationEngine>
#include <QQmlEngine>
#include <QQuickView>
#include <QRegularExpression>

FileSystemController::FileSystemController(FileSystemModel &fileSystemModel)
    : m_FileSystemModel(fileSystemModel) {
    connectToFileSystemModel();
}

FileSystemController::~FileSystemController() {}

void FileSystemController::setActivePath(const QString &newActivePath,
                                         uint recursionDepth) {
    qDebug() << "setActivePath function has been called";
    QString validActivePath = newActivePath;
    validActivePath.remove(QRegularExpression("file:///"));
    validActivePath = QDir::cleanPath(validActivePath);
    qDebug() << "New active path has been set: " << newActivePath;
    qDebug() << "Edited active path has been set: " << validActivePath;

    QDir activePath(validActivePath);
    if (activePath.exists()) {
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

    m_FileSystemModel.selectFileManually(m_CurrentRow);

    m_isSelectionStateChanged = true;
    emit selectionStateChanged();
}

QModelIndex FileSystemController::getCurrentlySelectedIndex() const {
    return m_CurrentRow;
}

QString FileSystemController::ActivePath() const {
    return QDir::toNativeSeparators(m_ActivePath);
}

void FileSystemController::setSizeFilter(const QString &filterValue) {
    if (!filterValue.isEmpty()) {
        m_SizeFilter = QVariant(filterValue).toDouble() < 0.001
                ? 0
                : QVariant(filterValue).toDouble();
        emit sizeFilterChanged();
    } else {
        m_SizeFilter = -1;
    }
}

void FileSystemController::selectBySize() {
    if (!m_ActivePath.isEmpty() && m_SizeFilter >= 0) {
        quint64 value = m_SizeFilter * 1024 * 1024;
        m_FileSystemModel.selectFilesBySizeIfAsync(
                    [value](FileTreeElement *x) { return x->getFileSize() > value; });
        selectionFlag = true;
    }
}

void FileSystemController::connectToFileSystemModel() {
    QObject::connect(&m_FileSystemModel,
                     &FileSystemModel::selectionBySizeFinished, this,
                     &FileSystemController::selectionBySizeEndedHandler);
    QObject::connect(&m_FileSystemModel,
                     &FileSystemModel::selectionByDateFinished, this,
                     &FileSystemController::selectionByDateEndedHandler);
}

void FileSystemController::selectionBySizeEndedHandler() {
    qDebug() << QTime::currentTime()
             << "handled selectionEnded signal in FileSystemModel";

    m_isSelectionStateChanged = true;
    emit selectionStateChanged();
}

void FileSystemController::selectionByDateEndedHandler() {
    qDebug() << QTime::currentTime()
             << "handled selectionEnded signal in FileSystemModel";

    m_isSelectionStateChanged = true;
    emit selectionStateChanged();
}

QString FileSystemController::getSizeFilter() {
    return QVariant(m_SizeFilter).toString();
}

void FileSystemController::setDaysAfterModificationFilter(
        const QString &filterValue) {
    if (!filterValue.isEmpty()) {
        m_DaysAfterModificationFilter = QVariant(filterValue).toInt();
        emit daysAfterModificationFilterChanged();
    } else {
        m_DaysAfterModificationFilter = -1;
    }
}

void FileSystemController::selectByDate() {
    if (!m_ActivePath.isEmpty() && m_DaysAfterModificationFilter != -1) {
        int value = m_DaysAfterModificationFilter;
        m_FileSystemModel.selectFilesByDateIfAsync([value](FileTreeElement *x) {
            return ((QDate::currentDate().toJulianDay() -
                     x->getLastModificationDate().toJulianDay()) > value);
        });
    }
}

QString FileSystemController::getDaysAfterModificationFilter() {
    return QVariant(m_DaysAfterModificationFilter).toString();
}

void FileSystemController::selectByFilter() {
    selectBySize();
    selectByDate();
}
