#ifndef REMOVEWORKER_H
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
    explicit RemoveWorker(const QStringList& paths, QObject *parent = Q_NULLPTR);
    ~RemoveWorker() Q_DECL_OVERRIDE;

public Q_SLOTS:
    void run() Q_DECL_OVERRIDE;

private:
    QStringList m_paths;
};

}           // namespace Farman

#endif // REMOVEWORKER_H
