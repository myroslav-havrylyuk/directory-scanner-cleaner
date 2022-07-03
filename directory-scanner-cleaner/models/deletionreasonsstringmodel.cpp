#include "deletionreasonsstringmodel.h"

DeletionReasonsStringModel::DeletionReasonsStringModel(QObject *parent)
    : QStringListModel{parent}
{
}

void DeletionReasonsStringModel::setInitialDeletionReason(const QString &deletionReason)
{
    if (m_InitialDeletionReason == deletionReason)
        return;

    int deletionReasonIndex = stringList().indexOf(deletionReason);
    if (deletionReasonIndex < 0)
        return;

    QModelIndex deletionReasonElementIndex =
            createIndex(deletionReasonIndex, 0, &stringList().at(deletionReasonIndex));
    QModelIndex firstElementIndex =
            createIndex(0, 0, &stringList().front());


    setData(deletionReasonElementIndex, stringList().front());
    setData(firstElementIndex, deletionReason);
}

void DeletionReasonsStringModel::setActiveDeletionReason(const QString &newDeletionReason)
{
    m_ActiveDeletionReason = newDeletionReason;
}

QString DeletionReasonsStringModel::getActiveDeletionReason() const
{
    return m_ActiveDeletionReason;
}
