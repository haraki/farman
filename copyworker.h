#ifndef COPYWORKER_H
#define COPYWORKER_H

#include <QWaitCondition>
#include "types.h"
#include "worker.h"

class QString;
class QStringList;

namespace Farman
{

class CopyWorker : public Worker
{
    Q_OBJECT

public:
    explicit CopyWorker(const QStringList& srcPaths, const QString& dstPath, bool moveMode, QObject *parent = Q_NULLPTR);
    ~CopyWorker() Q_DECL_OVERRIDE;

    void finishConfirmOverwrite(OverwriteMethodType methodType, bool methodTypeKeep, const QString& renameFileName);
    void cancelConfirmOverwrite();

Q_SIGNALS:
    void confirmOverwrite(const QString& srcFilePath, const QString& dstFilePath, int methodType);

    void startSub(int min, int max);
    void progressSub(int value);

public Q_SLOTS:
    void run() Q_DECL_OVERRIDE;

private:
    int makeList(const QString& srcPath, const QString& dstDirPath, QMap<QString, QString>& copyList, QList<QString>& removeDirList);
    int copyExec(const QString& srcPath, const QString& dstPath);

    void showConfirmOverwrite(const QString& srcFilePath, const QString& dstFilePath, OverwriteMethodType methodType);
    void emitConfirmOverwrite(const QString& srcFilePath, const QString& dstFilePath, OverwriteMethodType methodType);

    void emitStartSub(int min, int max);
    void emitProgressSub(int value);

    int copy(const QString& srcPath, const QString& dstPath);

    QStringList m_srcPaths;
    QString m_dstPath;
    bool m_moveMode;

    OverwriteMethodType m_methodType;
    bool m_methodTypeKeep;
    QString m_renameFileName;

    QWaitCondition m_confirmWait;

    qint64 m_copyUnitSize;
};

}           // namespace Farman

#endif // COPYWORKER_H
