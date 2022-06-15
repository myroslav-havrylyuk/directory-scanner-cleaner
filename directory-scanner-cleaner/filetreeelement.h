#ifndef FILETREEELEMENT_H
#define FILETREEELEMENT_H

#include <QVariant>
#include <QList>
#include <QDir>

enum FileTreeElementRole {
    FILE_NAME_ROLE = Qt::UserRole + 1,
    FILE_ROLES_SIZE
};

class FileTreeElement
{
public:
    FileTreeElement(const QString &fileName,
                    FileTreeElement *parentElement,
                    QList<FileTreeElement *> childFiles = {});

    ~FileTreeElement();

    int getRolesCount() const;
    int getChildsCount() const;
    QVariant getData(int role) const;
    FileTreeElement *getParent() const;
    FileTreeElement *getChildAt(int row);
    void setChildElements(QList<FileTreeElement *> childFiles);
    QList<FileTreeElement *> getChildElements() const;
    const QString &fileName() const;

private:
    QString m_FileName;
    QList<FileTreeElement *> m_childFiles;
    FileTreeElement *m_ParentElement;
};

#endif // FILETREEELEMENT_H
