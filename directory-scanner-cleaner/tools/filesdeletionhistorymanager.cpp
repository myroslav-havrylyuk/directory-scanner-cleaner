#include "filesdeletionhistorymanager.h"

FilesDeletionHistoryManager::FilesDeletionHistoryManager(QObject *parent)
    : QObject{parent} {}

FilesDeletionHistoryManager::~FilesDeletionHistoryManager() {}

void FilesDeletionHistoryManager::updateHistory(QList<QString> files,
                                                QString reason,
                                                QString filePath) {

    QFile file;
    file.setFileName(filePath);
    QJsonObject rootObject;
    QJsonArray currentReasonItems;
    QJsonDocument jsonDocument;
    if (file.exists()) {
        file.open(QIODevice::ReadOnly);
        QJsonParseError JsonParseError;
        jsonDocument = QJsonDocument::fromJson(file.readAll(), &JsonParseError);
        file.close();
        if (jsonDocument.isObject() == false) {
            qDebug() << JsonParseError.errorString();
            return;
        }
        rootObject = jsonDocument.object();
        currentReasonItems = rootObject.take(reason).toArray();
    }

    QJsonObject currentDeletedInfo;
    currentDeletedInfo.insert("time", QTime::currentTime().toString());
    QString name = qgetenv("USER");
    if (name.isEmpty())
        name = qgetenv("USERNAME");
    currentDeletedInfo.insert("userName", name);

    QJsonArray jsonFiles;
    for (int i = 0; i < files.size(); ++i) {
        jsonFiles.push_back(files.at(i));
    }

    currentDeletedInfo.insert("files", jsonFiles);
    currentReasonItems.push_back(currentDeletedInfo);

    rootObject.insert(reason, currentReasonItems);
    QJsonDocument jsonDoc;
    jsonDoc.setObject(rootObject);
    file.open(QIODevice::WriteOnly);
    file.write(jsonDoc.toJson());
    file.close();

    qDebug() << "history of deleted files updated";
}
