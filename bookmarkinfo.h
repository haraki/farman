#ifndef BOOKMARKINFO_H
#define BOOKMARKINFO_H

#include <QString>

namespace Farman
{

// ブックマーク情報
class BookmarkInfo
{
public:
    enum class Type : int
    {
        System,
        User,
    };

    BookmarkInfo(Type type, const QString& name, const QString& path)
    {
        m_type = type;
        m_name = name;
        m_path = path;
    }

    ~BookmarkInfo() = default;

    Type getType() const { return m_type; }
    QString getTypeName() const { return (m_type == Type::System) ? "System" : (m_type == Type::User) ? "User" : "Unknown"; }
    QString getName() const { return m_name; }
    QString getPath() const { return m_path; }

private:
    BookmarkInfo() = delete;

    Type m_type;
    QString m_name;
    QString m_path;
};

}           // namespace Farman

#endif // BOOKMARKINFO_H
