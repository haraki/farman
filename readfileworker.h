#ifndef FILEREADWORKER_H
#define FILEREADWORKER_H

#include "worker.h"

class QString;
class QFile;

namespace Farman
{

class ReadFileWorker : public Worker
{
    Q_OBJECT

public:
    ReadFileWorker(const QString& filePath, QByteArray* buffer, QObject *parent = Q_NULLPTR);
    ~ReadFileWorker();

public Q_SLOTS:
    void run() Q_DECL_OVERRIDE;

private:
    QString m_filePath;
    QByteArray* m_buffer;
};

}

#endif // FILEREADWORKER_H
