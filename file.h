#ifndef FILE_H
#define FILE_H

#include <QObject>
#include <QFile>
#include <QDateTime>

namespace Farman
{

class MainWindow;

class File : public QObject
{
    Q_OBJECT

public:
    explicit File(MainWindow* mainWindow);
    ~File();

Q_SIGNALS:
    void createNewFileFinished(const QString& filePath);

    void outputConsole(const QString& consoleString);

public Q_SLOTS:
    void onCopyFile(const QStringList& srcPaths, const QString& dstPath);
    void onMoveFile(const QStringList& srcPaths, const QString& dstPath);
    void onRemoveFile(const QStringList& paths);
    void onMakeDirectory(const QString& path, const QString& dirName);
    void onCreateNewFile(const QString& path, const QString& fileName);
    void onRenameFile(const QString& path, const QString& oldName, const QString& newName);
    void onChangeFileAttributes(const QString& path,
                                const QFile::Permissions& newPermissions,
                                const QDateTime& newCreated,
                                const QDateTime& newLastModified);

private:
    bool copyFile(const QStringList& srcPaths, const QString& dstPath);
    bool moveFile(const QStringList& srcPaths, const QString& dstPath);
    bool removeFile(const QStringList& paths);
    bool makeDirectory(const QString& path, const QString& dirName);
    bool createNewFile(const QString& path, const QString& fileName);
    bool renameFile(const QString& path, const QString& oldName, const QString& newName);
    bool changeFileAttributes(const QString& path,
                              const QFile::Permissions& newPermissions,
                              const QDateTime& newCreated,
                              const QDateTime& newLastModified);

    void emitCreateNewFileFinished(const QString& filePath);

    void emitOutputConsole(const QString& consoleString);

private Q_SLOTS:
    void onCopyFileFinished(int result);
    void onCopyFileError(const QString& err);
    void onMoveFileFinished(int result);
    void onMoveFileError(const QString& err);
    void onRemoveFileFinished(int result);
    void onRemoveFileError(const QString& err);

    void onConfirmOverwrite(const QString& srcFilePath, const QString& dstFilePath, int methodType);

private:
    MainWindow* m_mainWindow;
};

}           // namespace Farman

#endif // FILE_H
