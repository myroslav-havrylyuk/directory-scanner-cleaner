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

private:
public:
  enum DELETE_REASON { MANUALLY, DUPLICATE, LARGE, OLD };
  Q_ENUM(DELETE_REASON)
  explicit FilesDeletionHistoryManager(QObject *parent = nullptr);

public slots:
  void updateHistory(QList<QString> files, DELETE_REASON reason,
                     QString filePath);
};

#endif // FILESDELETIONHISTORYMANAGER_H
