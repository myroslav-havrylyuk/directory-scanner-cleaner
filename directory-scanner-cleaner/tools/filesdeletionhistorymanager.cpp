#include "filesdeletionhistorymanager.h"

FilesDeletionHistoryManager::FilesDeletionHistoryManager(QObject *parent)
    : QObject{parent} {}

void FilesDeletionHistoryManager::updateHistory(QList<QString> files,
                                                DELETE_REASON reason,
                                                QString filePath) {

  QFile file;
  file.setFileName(filePath);
  file.open(QIODevice::ReadOnly);
  QJsonParseError JsonParseError;
  QJsonDocument jsonDocument =
      QJsonDocument::fromJson(file.readAll(), &JsonParseError);
  file.close();
  if (jsonDocument.isObject() == false) {
    qDebug() << JsonParseError.errorString();
    return;
  }

  QJsonObject rootObject = jsonDocument.object();
  QJsonArray items = rootObject.take("items").toArray();
  QJsonObject currentDeletedInfo;
  currentDeletedInfo.insert("time", QDateTime::currentDateTime().toString());
  currentDeletedInfo.insert("reason", QVariant::fromValue(reason).toString());
  QString name = qgetenv("USER");
  if (name.isEmpty())
    name = qgetenv("USERNAME");
  currentDeletedInfo.insert("user name", name);

  QJsonArray jsonFiles;
  for (int i = 0; i < files.size(); ++i) {
    jsonFiles.push_back(files.at(i));
  }

  currentDeletedInfo.insert("files", jsonFiles);
  items.push_back(currentDeletedInfo);

  rootObject.insert("items", items);
  QJsonDocument jsonDoc;
  jsonDoc.setObject(rootObject);
  QFile file1;
  file1.setFileName(filePath);
  file1.open(QIODevice::WriteOnly);
  file1.write(jsonDoc.toJson());
  file1.close();
  qDebug() << "history of deleted files updated";
}
