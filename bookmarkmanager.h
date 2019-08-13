#ifndef BOOKMARKMANAGER_H
#define BOOKMARKMANAGER_H

#include <QObject>

namespace Farman
{

class BookmarkManager : public QObject
{
    Q_OBJECT

public:
    static void create();
    static void destroy();

    static BookmarkManager* getInstance();

    void initialize();
    void setDefault();

    const QList<QPair<QString, QString>>& getList() const;
    int getSize() const;
    QPair<QString, QString> get(const QString& name) const;
    QPair<QString, QString> get(int index) const;
    int search(const QString& path) const;
    int append(const QString& name, const QString& path);
    int append(const QPair<QString, QString>& bookmark);
    int insert(const QString path, int index = -1);
    int insert(const QPair<QString, QString>& bookmark, int index = -1);
    int push_back(const QPair<QString, QString>& bookmark);
    int remove(const QPair<QString, QString>& bookmark);
    int remove(int index);

private:
    BookmarkManager();

    static BookmarkManager* s_instance;

    QList<QPair<QString, QString>> m_list;
};

}           // namespace Farman

#endif // BOOKMARKMANAGER_H
