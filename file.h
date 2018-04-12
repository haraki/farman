#ifndef FILE_H
#define FILE_H

#include <QObject>
#include <QFile>
#include <QDateTime>

namespace Farman
{

class File : public QObject
{
    Q_OBJECT

public:
    static void create(QObject *parent = Q_NULLPTR);
    static File* getInstance();

    bool copyFile(const QStringList& srcPaths, const QString& dstPath);
    bool moveFile(const QStringList& srcPaths, const QString& dstPath);
    bool removeFile(const QStringList& paths);
    bool makeDirectory(const QString& path, const QString& dirName);
    bool renameFile(const QString& path, const QString& oldName, const QString& newName);
    bool changeFileAttributes(const QString& path,
                              const QFile::Permissions& newPermissions,
                              const QDateTime& newCreated,
                              const QDateTime& newLastModified);

Q_SIGNALS:
    void outputConsole(const QString& consoleString);

private:
    explicit File(QObject *parent = Q_NULLPTR);
    ~File();

    void emitOutputConsole(const QString& consoleString);

private Q_SLOTS:
    void onCopyFileFinished(int result);
    void onCopyFileError(const QString& err);
    void onMoveFileFinished(int result);
    void onMoveFileError(const QString& err);
    void onRemoveFileFinished(int result);
    void onRemoveFileError(const QString& err);

    void onConfirmOverwrite(const QString& srcFilePath, const QString& dstFilePath, int methodType);

    void onOutputConsole(const QString& consoleString);

private:
    static File* s_instance;
};

}           // namespace Farman

#endif // FILE_H
