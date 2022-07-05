#ifndef FILETREEREVERSEBREADTHITERATOR_H
#define FILETREEREVERSEBREADTHITERATOR_H

#include "models/filetreeelement.h"

class FileTreeElementReverseBreadthIterator {
private:
    FileTreeElement *m_RootElement;
    FileTreeElement *m_CurrentParentElement;
    int m_CurrentElementIndex;

public:
    FileTreeElementReverseBreadthIterator(FileTreeElement *rootElement);

    FileTreeElement *next();
    FileTreeElement *value() const;
    bool hasNext() const;

private:
    void setupIterator();
    FileTreeElement *getLastElementInDepthInSearchManner(FileTreeElement *parent);
};

#endif // FILETREEREVERSEBREADTHITERATOR_H
