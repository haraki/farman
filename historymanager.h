#ifndef HISTORYMANAGER_H
#define HISTORYMANAGER_H

#include <QObject>
#include <QList>
#include <QString>
#include "types.h"
#include "default_settings.h"

namespace Farman
{

class HistoryManager : public QObject
{
    Q_OBJECT

public:
    HistoryManager(QObject* parent = Q_NULLPTR);

    void initialize(int maxSize = DEFAULT_HISTORY_MAX_SIZE);
    void clear();
    const QList<QString>& getList() const;
    int setList(const QList<QString>& list);
    int search(const QString& path) const;
    int setPath(const QString& path);
    QString getPrevious() const;
    QString getNext() const;
    int setNextIndex();
    int setPreviousIndex();

private:
    QList<QString> m_list;
    int m_listMaxSize;
    int m_index;
};

}           // namespace Farman

#endif // HISTORYMANAGER_H
