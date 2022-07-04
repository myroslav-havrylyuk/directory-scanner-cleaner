#include <QDebug>

#include "filetreereversebreadthiterator.h"

void FileTreeElementReverseBreadthIterator::setupIterator()
{
    m_CurrentParentElement = getLastElementInDepthInSearchManner(m_RootElement);
}

FileTreeElement *FileTreeElementReverseBreadthIterator::next()
{
    if (m_CurrentParentElement == nullptr)
        return nullptr;

    --m_CurrentElementIndex;

    if (m_CurrentElementIndex < 0)
    {
        if (m_CurrentParentElement == m_RootElement)
        {
            m_CurrentParentElement = nullptr;
            m_CurrentElementIndex = 0;
            return m_RootElement;
        }

        m_CurrentElementIndex =  m_CurrentParentElement->row();
        m_CurrentParentElement = m_CurrentParentElement->getParent();
    } else {
        FileTreeElement *currentElement =
                m_CurrentParentElement->getChildAt(m_CurrentElementIndex);

        if (currentElement && currentElement->hasChildElements())
        {
            m_CurrentParentElement = currentElement;
            m_CurrentElementIndex = m_CurrentParentElement->getChildsCount() - 1;
        }
    }

    return m_CurrentParentElement->getChildAt(m_CurrentElementIndex);
}

FileTreeElement *FileTreeElementReverseBreadthIterator::value() const
{
    if (m_CurrentParentElement == nullptr)
        return m_RootElement;

    return m_CurrentParentElement->getChildAt(m_CurrentElementIndex);
}

bool FileTreeElementReverseBreadthIterator::hasNext() const
{
    return (m_CurrentParentElement != nullptr);
}

FileTreeElement *FileTreeElementReverseBreadthIterator::getLastElementInDepthInSearchManner(FileTreeElement *parent)
{
    if (parent == nullptr)
        return nullptr;

    if (!parent->hasChildElements())
    {
        if (parent == m_RootElement)
        {
            m_CurrentElementIndex = 0;
            m_CurrentParentElement = nullptr;
            return parent;
        }

        m_CurrentElementIndex = parent->getParent()->getChildsCount();
        return parent->getParent();
    }


    return getLastElementInDepthInSearchManner(parent->getChildElements().back());
}

FileTreeElementReverseBreadthIterator::FileTreeElementReverseBreadthIterator(FileTreeElement *rootElement)
    : m_RootElement(rootElement)
{
    if (rootElement == nullptr)
        return;

    setupIterator();
}
