#include <QDebug>
#include "historymanager.h"

namespace Farman
{

HistoryManager::HistoryManager(QObject* parent/* = Q_NULLPTR*/)
    : QObject(parent)
    , m_listMaxSize(DEFAULT_HISTORY_MAX_SIZE)
    , m_index(0)
{

}

void HistoryManager::initialize(int maxSize/* = DEFAULT_HISTORY_MAX_SIZE*/)
{
    m_listMaxSize = maxSize;
    clear();
}

void HistoryManager::clear()
{
    m_list.clear();
}

const QList<QString>& HistoryManager::getList() const
{
    return m_list;
}

int HistoryManager::setList(const QList<QString>& list)
{
    clear();

    if(list.size() <= m_listMaxSize)
    {
        m_list = list;
    }
    else
    {
        for(int i = 0;i < m_listMaxSize;i++)
        {
            m_list.push_back(list[i]);
        }
    }

    return 0;
}

int HistoryManager::search(const QString& path) const
{
    for(int i = 0;i < m_list.size();i++)
    {
        if(path == m_list[i])
        {
            return i;
        }
    }

    return -1;
}

int HistoryManager::setPath(const QString& path)
{
    if(m_index == 0)
    {
        m_list.push_front(path);

        for(int num = m_list.size() - m_listMaxSize;num > 0;num--)
        {
            m_list.pop_back();
        }
    }
    else
    {
        m_index--;
        m_list[m_index] = path;
        for(;m_index > 0;m_index--)
        {
            m_list.pop_front();
        }
    }

    qDebug() << "index : " << m_index << ", History : " << m_list;

    return 0;
}

QString HistoryManager::getPrevious() const
{
    if(m_index >= m_list.size() - 1)
    {
        return QString();
    }

    return m_list[m_index + 1];
}

QString HistoryManager::getNext() const
{
    if(m_index <= 0)
    {
        return QString();
    }

    return m_list[m_index - 1];
}

int HistoryManager::setPreviousIndex()
{
    if(m_index >= m_list.size() - 1)
    {
        return -1;
    }

    m_index++;

    return m_index;
}

int HistoryManager::setNextIndex()
{
    if(m_index <= 0)
    {
        return -1;
    }

    m_index--;

    return m_index;
}

}           // namespace Farman
