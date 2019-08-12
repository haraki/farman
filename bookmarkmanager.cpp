#include <QDebug>
#include "bookmarkmanager.h"

namespace Farman
{

BookmarkManager* BookmarkManager::s_instance = Q_NULLPTR;

void BookmarkManager::create()
{
    s_instance = new BookmarkManager();

    s_instance->initialize();
}

void BookmarkManager::destroy()
{
    delete s_instance;

    s_instance = Q_NULLPTR;
}

BookmarkManager* BookmarkManager::getInstance()
{
    return s_instance;
}

BookmarkManager::BookmarkManager()
{

}

void BookmarkManager::initialize()
{
    m_list.clear();
}

const QList<QPair<QString, QString>>& BookmarkManager::getList() const
{
    return m_list;
}

int BookmarkManager::getSize() const
{
    return m_list.size();
}

QPair<QString, QString> BookmarkManager::get(const QString& name) const
{
    for(int i = 0;i < m_list.size();i++)
    {
        if(m_list[i].first == name)
        {
            return get(i);
        }
    }

    return QPair<QString, QString>();
}

QPair<QString, QString> BookmarkManager::get(int index) const
{
    if(index < 0 || index >= m_list.count())
    {
        return QPair<QString, QString>();
    }

    return m_list[index];
}

int BookmarkManager::search(const QString& path) const
{
    for(int i = 0;i < m_list.size();i++)
    {
        if(m_list[i].second == path)
        {
            return i;
        }
    }

    return -1;
}

int BookmarkManager::append(const QString& name, const QString& path)
{
    return append({name, path});
}

int BookmarkManager::append(const QPair<QString, QString>& bookmark)
{
    return push_back(bookmark);
}

int BookmarkManager::insert(const QString path, int index/* = -1*/)
{
    return insert({path, path}, index);
}

int BookmarkManager::insert(const QPair<QString, QString>& bookmark, int index/* = -1*/)
{
    if(index < 0 || index >= m_list.count())
    {
        index = push_back(bookmark);
    }
    else
    {
        m_list.insert(index, bookmark);
    }

    qDebug() << "insert bookmark : " << bookmark << ", index : " << index;

    return index;
}

int BookmarkManager::push_back(const QPair<QString, QString>& bookmark)
{
    m_list.push_back(bookmark);

    return m_list.count() - 1;
}

int BookmarkManager::remove(const QPair<QString, QString>& bookmark)
{
    for(int i = 0;i < m_list.size();i++)
    {
        if(m_list[i] == bookmark)
        {
            return remove(i);
        }
    }

    return -1;
}

int BookmarkManager::remove(int index)
{
    if(index < 0 || index >= m_list.count())
    {
        return -1;
    }

    qDebug() << "remove bookmark : " << m_list[index] << ", index : " << index;

    m_list.removeAt(index);

    return index;
}

}           // namespace Farman
