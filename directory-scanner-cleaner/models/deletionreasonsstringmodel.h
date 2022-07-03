#ifndef DELETIONREASONSSTRINGMODEL_H
#define DELETIONREASONSSTRINGMODEL_H

#include <QStringListModel>
#include <QObject>

class DeletionReasonsStringModel : public QStringListModel
{
    Q_OBJECT

    Q_PROPERTY(QString activeDeletionReason READ getActiveDeletionReason WRITE setActiveDeletionReason)

public:
    explicit DeletionReasonsStringModel(QObject *parent = nullptr);

    void setInitialDeletionReason(const QString &deletionReason);
    void setActiveDeletionReason(const QString &newDeletionReason);
    QString getActiveDeletionReason() const;

private:
    QString m_ActiveDeletionReason;
    QString m_InitialDeletionReason;
};

#endif // DELETIONREASONSSTRINGMODEL_H
