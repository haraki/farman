﻿#ifndef REMOVEWORKER_H
#define REMOVEWORKER_H

#include "types.h"
#include "worker.h"

class QString;
class QStringList;

namespace Farman
{

class RemoveWorker : public Worker
{
    Q_OBJECT

public:
    explicit RemoveWorker(const QStringList& paths, QObject *parent = 0);
    ~RemoveWorker();

public Q_SLOTS:
    void process() Q_DECL_OVERRIDE;

private:
    int makeList(const QString& path, QList<QString>& removeList);
    int copyExec(const QString& path);

    QStringList m_paths;
};

}           // namespace Farman

#endif // REMOVEWORKER_H