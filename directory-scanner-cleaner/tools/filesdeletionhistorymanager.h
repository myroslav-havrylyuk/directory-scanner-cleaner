#ifndef FILESDELETIONHISTORYMANAGER_H
#define FILESDELETIONHISTORYMANAGER_H

#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>
#include <QJsonValue>
#include <QList>
#include <QObject>

class FilesDeletionHistoryManager : public QObject {

  Q_OBJECT

public:
  explicit FilesDeletionHistoryManager(QObject *parent = nullptr);
    ~FilesDeletionHistoryManager();

public slots:
  void updateHistory(QList<QString> files, QString reason,
                     QString filePath);
};

#endif // FILESDELETIONHISTORYMANAGER_H
