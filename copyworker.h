﻿#ifndef COPYWORKER_H
#define COPYWORKER_H

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
    explicit CopyWorker(const QStringList& srcPaths, const QString& dstPath, bool moveMode, QObject *parent = 0);
    ~CopyWorker();

public Q_SLOTS:
    void process() Q_DECL_OVERRIDE;

private:
    int makeList(const QString& srcPath, const QString& dstDirPath, QMap<QString, QString>& copyList, QList<QString>& removeDirList);
    int copyExec(const QString& srcPath, const QString& dstPath);

    QStringList m_srcPaths;
    QString m_dstPath;
    bool m_moveMode;

    OverwriteMethodType m_methodType;
    bool m_methodTypeKeep;
};

}           // namespace Farman

#endif // COPYWORKER_H
