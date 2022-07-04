#ifndef DELETIONREASONSSTRINGMODEL_H
#define DELETIONREASONSSTRINGMODEL_H

#include <QStringListModel>
#include <QObject>

class DeletionReasonsStringModel : public QStringListModel
{
    Q_OBJECT

    Q_PROPERTY(QString activeDeletionReason READ getActiveDeletionReason WRITE setActiveDeletionReason)

private:
    QString m_ActiveDeletionReason;
    QString m_InitialDeletionReason;

public:
    explicit DeletionReasonsStringModel(QObject *parent = nullptr);
    void setInitialDeletionReason(const QString &deletionReason);
    void setActiveDeletionReason(const QString &newDeletionReason);
    QString getActiveDeletionReason() const;

};

#endif // DELETIONREASONSSTRINGMODEL_H
